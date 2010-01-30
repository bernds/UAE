 /*
  * UAE - The Un*x Amiga Emulator
  *
  * Support for Linux/ALSA sound
  *
  * Copyright 1997 Bernd Schmidt
  * Copyright 2004 Heikki Orsila
  * Copyright 2006-2007 Richard Drummond
  *
  * BUGS: certainly
  * TODO:
  * - if setup_sound() fails, there may still be hope to get the
  *   sound device, but we totally give up.. see sd-uss.
  */

#include "sysconfig.h"
#include "sysdeps.h"

#include "options.h"
#include "gensound.h"
#include "memory.h"
#include "events.h"
#include "custom.h"
#include "newcpu.h"
#include "sounddep/sound.h"
#include "threaddep/thread.h"

#include <alsa/asoundlib.h>

static smp_comm_pipe to_sound_pipe;
static uae_sem_t sound_comm_sem;

static char alsa_device[256] = "default";
static int alsa_verbose = 0;

static int have_sound = 0;
static int dont_block;

static int which_buffer;
static uae_u16 sndbuffer[2][44100];
uae_u16 *sndbufpt, *sndbuf_base;
int sndbufsize;

static snd_pcm_t *alsa_playback_handle = 0;
static int alsa_to_frames_divisor;
static snd_pcm_uframes_t period_frames;

/* alsa_xrun_recovery() function is copied from ALSA manual. why the hell did
 * they make ALSA this hard?! i bet 95% of ALSA programmers would like a
 * simpler way to do error handling.. let the 5% use tricky APIs.  */
static int alsa_xrun_recovery (snd_pcm_t *handle, int err)
{
    if (err == -EPIPE) {
	/* under-run */
	err = snd_pcm_prepare (handle);
	if (err < 0)
	    fprintf (stderr, "uae: no recovery with alsa from underrun, prepare failed: %s\n", snd_strerror (err));
	return 0;
    } else if (err == -ESTRPIPE) {
	while ((err = snd_pcm_resume (handle)) == -EAGAIN) {
	    /* wait until the suspend flag is released */
	    fprintf (stderr, "uae: sleeping for alsa.\n");
	    sleep (1);
	}
	if (err < 0) {
	    err = snd_pcm_prepare (handle);
	    if (err < 0)
		fprintf (stderr, "uae: no recovery with alsa from suspend, prepare failed: %s\n", snd_strerror (err));
	}
	return 0;
    }
    return err;
}

static void write_sound_frames (uae_u16 *bufbase)
{
    char *buf = (char *) bufbase;
    int ret;

    int frames = period_frames;
    while (frames > 0) {
	ret = snd_pcm_writei (alsa_playback_handle, buf, frames);
	if (ret < 0) {
	    if (ret == -EAGAIN || ret == -EINTR)
		continue;
	    if (alsa_xrun_recovery (alsa_playback_handle, ret) < 0) {
		fprintf (stderr, "uae: write error with alsa: %s\n", snd_strerror (ret));
		exit (-1);
	    }
	    continue;
	}
	frames -= ret;
	buf += ret * alsa_to_frames_divisor;
    }
}

void finish_sound_buffers ()
{
    dont_block = currprefs.m68k_speed == -1 && (!regs.stopped || active_fs_packets > 0);
    if (!dont_block) {
	write_sound_frames (sndbuf_base);
    } else {
	write_comm_pipe_int (&to_sound_pipe, 2, 1);
	uae_sem_wait (&sound_comm_sem);
    }

    sndbufpt = sndbuf_base = sndbuffer[which_buffer ^ 1];
}

void close_sound (void)
{
    sync_with_sound = 0;
    if (alsa_playback_handle) {
	snd_pcm_close (alsa_playback_handle);
	alsa_playback_handle = 0;
    }
    write_comm_pipe_int (&to_sound_pipe, 1, 1);
    uae_sem_wait (&sound_comm_sem);
    uae_sem_destroy (&sound_comm_sem);
}

static int open_sound_device (void)
{
    return snd_pcm_open (&alsa_playback_handle, alsa_device, SND_PCM_STREAM_PLAYBACK, 0);
}

/* Try to determine whether sound is available.  This is only for GUI purposes.  */
int setup_sound (void)
{
    int err;
    sound_available = 0;
    if ((err = open_sound_device ()) < 0) {
	/* TODO: if the pcm was busy, we should the same as sd-uss does.
	   tell the caller that sound is available. in any other
	   condition we should just return 0. */
	write_log ("Cannot open audio device: %s.\n", snd_strerror (err));
	return 0;
    }
    snd_pcm_close (alsa_playback_handle);
    alsa_playback_handle = 0;
    sound_available = 1;
    return 1;
}

static int set_hw_params (snd_pcm_t *pcm, snd_pcm_hw_params_t *hw_params,
			  unsigned int *rate, unsigned int channels,
			  snd_pcm_format_t format, unsigned int *buffer_time,
			  snd_pcm_uframes_t *buffer_frames,
			  snd_pcm_uframes_t *per_frames)
{
    int err;
    unsigned int periods = 2;

    err = snd_pcm_hw_params_any (pcm, hw_params);
    if (err < 0)
	return err;
    err = snd_pcm_hw_params_set_access (pcm, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED);
    if (err < 0)
	return err;
    err = snd_pcm_hw_params_set_format (pcm, hw_params, format);
    if (err < 0)
	return err;
    err = snd_pcm_hw_params_set_channels (pcm, hw_params, channels);
    if (err < 0)
	return err;
    err = snd_pcm_hw_params_set_rate_near (pcm, hw_params, rate, 0);
    if (err < 0)
	return err;
    err = snd_pcm_hw_params_set_buffer_time_near (pcm, hw_params, buffer_time, NULL);
    if (err < 0)
	return err;
    snd_pcm_hw_params_get_buffer_size (hw_params, buffer_frames);
    err = snd_pcm_hw_params_set_periods_near (pcm, hw_params, &periods, NULL);
    if (err < 0)
	return err;
    if (periods == 1)
	return -EINVAL;
    err = snd_pcm_hw_params (pcm, hw_params);

    snd_pcm_hw_params_get_period_size (hw_params, per_frames, NULL);
    return 0;
}

static int set_sw_params (snd_pcm_t * pcm,
			  snd_pcm_sw_params_t * sw_params, snd_pcm_uframes_t buffer_frames, snd_pcm_uframes_t period_frames)
{
    int err;

    err = snd_pcm_sw_params_current (pcm, sw_params);
    if (err < 0)
	return err;
    err = snd_pcm_sw_params_set_start_threshold (pcm, sw_params, (buffer_frames / period_frames) * period_frames);
    if (err < 0)
	return err;
    err = snd_pcm_sw_params_set_avail_min (pcm, sw_params, period_frames);
    if (err < 0)
	return err;
    err = snd_pcm_sw_params_set_stop_threshold (pcm, sw_params, buffer_frames);
    if (err < 0)
	return err;
    err = snd_pcm_sw_params_set_xfer_align (pcm, sw_params, 1);
    if (err < 0)
	return err;
    err = snd_pcm_sw_params (pcm, sw_params);
    if (err < 0)
	return err;
    return 0;
}

static void open_sound (void)
{
    unsigned int rate;
    snd_pcm_format_t format;
    unsigned int channels;

    snd_pcm_hw_params_t *hw_params = 0;
    snd_pcm_sw_params_t *sw_params = 0;
    snd_pcm_uframes_t buffer_frames;
    unsigned int buffer_time;

    snd_output_t *alsa_out;

    int err;

    sync_with_sound = 0;

    snd_output_stdio_attach (&alsa_out, stderr, 0);

    channels = currprefs.sound_stereo ? 2 : 1;
    rate = currprefs.sound_freq;

    have_sound = 0;
    alsa_playback_handle = 0;
    if ((err = open_sound_device ()) < 0) {
	write_log ("Cannot open audio device: %s\n", snd_strerror (err));
	goto nosound;
    }

    buffer_time = currprefs.sound_maxbsiz * 1000;
    if (buffer_time < 1000 || buffer_time > 500000)
	buffer_time = 100000;

    if ((err = snd_pcm_hw_params_malloc (&hw_params)) < 0) {
	write_log ("Cannot allocate hardware parameter structure: %s.\n", snd_strerror (err));
	goto nosound;
    }
    if ((err = snd_pcm_sw_params_malloc (&sw_params)) < 0) {
	write_log ("Cannot allocate software parameter structure: %s.\n", snd_strerror (err));
	goto nosound;
    }

    format = SND_PCM_FORMAT_S16;

    alsa_to_frames_divisor = 2 * channels;

    if ((err =
	 set_hw_params (alsa_playback_handle, hw_params, &rate, channels, format, &buffer_time, &buffer_frames,
			&period_frames)) < 0) {
	write_log ("Cannot set hw parameters: %s.\n", snd_strerror (err));
	goto nosound;
    }

    if ((err = set_sw_params (alsa_playback_handle, sw_params, buffer_frames, period_frames)) < 0) {
	write_log ("Cannot set sw parameters: %s.\n", snd_strerror (err));
	goto nosound;
    }

    sndbufsize = period_frames * alsa_to_frames_divisor;
    snd_pcm_hw_params_free (hw_params);
    snd_pcm_sw_params_free (sw_params);

    if ((err = snd_pcm_prepare (alsa_playback_handle)) < 0) {
	write_log ("Cannot prepare audio interface for use: %s.\n", snd_strerror (err));
	goto nosound;
    }

    obtainedfreq = rate;

    init_sound_table16 ();
    sample_handler = currprefs.sound_stereo ? sample16s_handler : sample16_handler;

    have_sound = 1;
    sound_available = 1;

    write_log ("ALSA: Using device '%s'.\n", alsa_device);
    write_log ("ALSA: Sound configured for %d Hz. Buffer length is %u us, period size %d bytes.\n",
	       rate, buffer_time, period_frames * alsa_to_frames_divisor);

    if (alsa_verbose)
	snd_pcm_dump (alsa_playback_handle, alsa_out);

    sndbufpt = sndbuf_base = sndbuffer[which_buffer = 0];

    sync_with_sound = 1;
    return;

  nosound:
    have_sound = 0;
    if (hw_params)
	snd_pcm_hw_params_free (hw_params);
    if (sw_params)
	snd_pcm_sw_params_free (sw_params);

    close_sound ();
}

#if 0
/*
 * Handle audio specific cfgfile options
 */
void audio_default_options (struct uae_prefs *p)
{
    strncpy (alsa_device, "default", 256);
    alsa_verbose = 1;
}

void audio_save_options (FILE * f, const struct uae_prefs *p)
{
    cfgfile_write (f, "alsa.device=%s\n", alsa_device);
    cfgfile_write (f, "alsa.verbose=%s\n", alsa_verbose ? "true" : "false");
}

int audio_parse_option (struct uae_prefs *p, const char *option, const char *value)
{
    return (cfgfile_string (option, value, "device", alsa_device, 256)
	    || cfgfile_yesno (option, value, "verbose", &alsa_verbose));
}
#endif

static void *sound_thread (void *dummy)
{
    for (;;) {
	int cmd = read_comm_pipe_int_blocking (&to_sound_pipe);
	int n;

	switch (cmd) {
	case 0:
	    open_sound ();
	    uae_sem_post (&sound_comm_sem);
	    break;
	case 1:
	    uae_sem_post (&sound_comm_sem);
	    return 0;
	case 2:
	    /* If trying for maximum CPU speed, don't block the main
	       thread, instead set the delaying_for_sound variable.  If
	       not trying for maximum CPU speed, synchronize here by
	       delaying the sem_post until after the write.  */
	    delaying_for_sound = dont_block;
	    if (dont_block)
		uae_sem_post (&sound_comm_sem);

	    write_sound_frames (sndbuf_base);
	    if (!dont_block)
		uae_sem_post (&sound_comm_sem);

	    delaying_for_sound = 0;
	    break;
	}
    }
}

/* We use a thread so that we can use the time spent waiting for the sound
   driver for executing m68k instructions, rather than just blocking.  */
static void init_sound_thread (void)
{
    uae_thread_id tid;

    init_comm_pipe (&to_sound_pipe, 20, 1);
    uae_sem_init (&sound_comm_sem, 0, 0);
    uae_start_thread (sound_thread, NULL, &tid);
}

int init_sound (void)
{
    init_sound_thread ();
    write_comm_pipe_int (&to_sound_pipe, 0, 1);
    uae_sem_wait (&sound_comm_sem);

    return have_sound;
}

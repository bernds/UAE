 /*
  * UAE - The Un*x Amiga Emulator
  *
  * Support for SDL sound
  *
  * Copyright 1997 Bernd Schmidt
  * Copyright 2003 Richard Drummond
  */

#include "sysconfig.h"
#include "sysdeps.h"

#include "config.h"
#include "options.h"
#include "custom.h"
#include "gensound.h"
#include "sounddep/sound.h"
#include "threaddep/thread.h"
#include "SDL_audio.h"

static int have_sound = 0;
static int obtainedfreq;

uae_u16 sndbuffer[44100];
uae_u16 *sndbufpt;
int sndbufsize;
static SDL_AudioSpec spec;

static smp_comm_pipe to_sound_pipe;
static uae_sem_t data_available_sem, callback_done_sem, sound_init_sem;

static int in_callback, closing_sound;

static void clearbuffer (void)
{
    memset (sndbuffer, (spec.format == AUDIO_U8) ? SOUND8_BASE_VAL : SOUND16_BASE_VAL, sizeof (sndbuffer));
}

static void sound_callback (void *userdata, Uint8 *stream, int len)
{
    if (closing_sound)
	return;
    in_callback = 1;
    /* Wait for data to finish.  */
    uae_sem_wait (&data_available_sem);
    if (! closing_sound) {
	memcpy (stream, sndbuffer, sndbufsize);
	/* Notify writer that we're done.  */
	uae_sem_post (&callback_done_sem);
    }
    in_callback = 0;
}

void finish_sound_buffer (void)
{
    uae_sem_post (&data_available_sem);
    uae_sem_wait (&callback_done_sem);
}

void update_sound (int freq)
{
    int scaled_sample_evtime_orig;
    static int lastfreq =0;

    if (freq < 0)
        freq = lastfreq;
    lastfreq = freq;
    if (have_sound) {
	if (currprefs.gfx_vsync && currprefs.gfx_afullscreen) {
	    if (currprefs.ntscmode)
		scaled_sample_evtime_orig = (unsigned long)(MAXHPOS_NTSC * MAXVPOS_NTSC * freq * CYCLE_UNIT + obtainedfreq - 1) / obtainedfreq;
	else
	    scaled_sample_evtime_orig = (unsigned long)(MAXHPOS_PAL * MAXVPOS_PAL * freq * CYCLE_UNIT + obtainedfreq - 1) / obtainedfreq;
	} else {
	    scaled_sample_evtime_orig = (unsigned long)(312.0 * 50 * CYCLE_UNIT / (obtainedfreq  / 227.0));
	}
	scaled_sample_evtime = scaled_sample_evtime_orig;
    }
}

/* Try to determine whether sound is available.  This is only for GUI purposes.  */
int setup_sound (void)
{
    int size = currprefs.sound_maxbsiz;

    spec.freq = currprefs.sound_freq;
    spec.format = currprefs.sound_bits == 8 ? AUDIO_U8 : AUDIO_S16SYS;
    spec.channels = currprefs.stereo ? 2 : 1;
    size >>= spec.channels - 1;
    size >>= 1;
    while (size & (size - 1))
	size &= size - 1;
    if (size < 512)
	size = 512;
    spec.samples = size;
    spec.callback = sound_callback;
    spec.userdata = 0;


    if (SDL_OpenAudio (&spec, NULL) < 0) {
	write_log ("Couldn't open audio: %s\n", SDL_GetError());
	return 0;
    }
    sound_available = 1;
    SDL_CloseAudio ();
    return 1;
}

static int open_sound (void)
{
    int size = currprefs.sound_maxbsiz;

    spec.freq = currprefs.sound_freq;
    spec.format = currprefs.sound_bits == 8 ? AUDIO_U8 : AUDIO_S16SYS;
    spec.channels = currprefs.stereo ? 2 : 1;
    /* Always interpret buffer size as number of samples, not as actual
       buffer size.  Of course, since 8192 is the default, we'll have to
       scale that to a sane value (assuming that otherwise 16 bits and
       stereo would have been enabled and we'd have done the shift by
       two anyway).  */
    size >>= 2;
    while (size & (size - 1))
	size &= size - 1;
    if (size < 512)
	size = 512;
    spec.samples = size;
    spec.callback = sound_callback;
    spec.userdata = 0;

    clearbuffer();
    if (SDL_OpenAudio (&spec, NULL) < 0) {
	write_log ("Couldn't open audio: %s\n", SDL_GetError());
	return 0;
    }

    if (spec.format == AUDIO_S16SYS) {
	init_sound_table16 ();
	sample_handler = currprefs.stereo ? sample16s_handler : sample16_handler;
    } else {
	init_sound_table8 ();
	sample_handler = currprefs.stereo ? sample8s_handler : sample_ulaw_handler;
    }
    have_sound = 1;

    update_sound(vblank_hz);
    sound_available = 1;
    obtainedfreq = currprefs.sound_freq;
    write_log ("SDL sound driver found and configured for %d bits at %d Hz, buffer is %d samples\n",
	currprefs.sound_bits, spec.freq, spec.samples);
    sndbufpt = sndbuffer;
    sndbufsize = size * currprefs.sound_bits / 8 * spec.channels;
    return 1;
}

static void *sound_thread (void *dummy)
{
    for (;;) {
	int cmd = read_comm_pipe_int_blocking (&to_sound_pipe);
	int n;

	switch (cmd) {
	case 0:
	    open_sound ();
	    uae_sem_post (&sound_init_sem);
	    break;
	case 1:
	    uae_sem_post (&sound_init_sem);
	    return 0;
	}
    }
}

/* We need a thread for this, since communication between finish_sound_buffer
 * and the callback works through semaphores.  In theory, this is unnecessary,
 * since SDL uses a sound thread internally, and the callback runs in its
 * context.  But we don't want to depend on SDL's internals too much.  */
static void init_sound_thread (void)
{
    uae_thread_id tid;

    init_comm_pipe (&to_sound_pipe, 20, 1);
    uae_sem_init (&data_available_sem, 0, 0);
    uae_sem_init (&callback_done_sem, 0, 0);
    uae_sem_init (&sound_init_sem, 0, 0);
    uae_start_thread (sound_thread, NULL, &tid);
}

void close_sound (void)
{
    if (! have_sound)
	return;

    SDL_PauseAudio (1);
    clearbuffer();
    if (in_callback) {
	closing_sound = 1;
	uae_sem_post (&data_available_sem);
    }
    write_comm_pipe_int (&to_sound_pipe, 1, 1);
    uae_sem_wait (&sound_init_sem);
    SDL_CloseAudio ();
    uae_sem_destroy (&data_available_sem);
    uae_sem_destroy (&sound_init_sem);
    uae_sem_destroy (&callback_done_sem);
    have_sound = 0;
}

int init_sound (void)
{
    in_callback = 0;
    closing_sound = 0;

    init_sound_thread ();
    write_comm_pipe_int (&to_sound_pipe, 0, 1);
    uae_sem_wait (&sound_init_sem);
    SDL_PauseAudio (0);

    return have_sound;
}

void pause_sound (void)
{
    SDL_PauseAudio (1);
}

void resume_sound (void)
{
    clearbuffer();
    SDL_PauseAudio (0);
}

void reset_sound (void)
{
   clearbuffer();
   return;
}


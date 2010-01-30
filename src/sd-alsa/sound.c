 /*
  * UAE - The Un*x Amiga Emulator
  *
  * Support for Linux/ALSA sound
  *
  * Copyright 1997 Bernd Schmidt
  * Copyright 2004 Heikki Orsila
  *
  * BUGS: certainly
  * TODO:
  * - if setup_sound() fails, there may still be hope to get the
  *   sound device, but we totally give up.. see sd-uss.
  */

#include "sysconfig.h"
#include "sysdeps.h"

#include "config.h"
#include "options.h"
#include "memory.h"
#include "events.h"
#include "custom.h"
#include "gensound.h"
#include "sounddep/sound.h"

#include <alsa/asoundlib.h>

int sound_fd;
static int have_sound = 0;
static unsigned long formats;

uae_u16 sndbuffer[44100];
uae_u16 *sndbufpt;
int sndbufsize;

snd_pcm_t *alsa_playback_handle = 0;
int alsa_to_frames_divisor = 4;

void close_sound (void)
{
  if (alsa_playback_handle) {
    snd_pcm_close (alsa_playback_handle);
    alsa_playback_handle = 0;
  }
}

static int open_sound(void)
{
  return snd_pcm_open (&alsa_playback_handle, "default", SND_PCM_STREAM_PLAYBACK, 0);
}

/* Try to determine whether sound is available.  This is only for GUI purposes.  */
int setup_sound (void)
{
  int err;
  sound_available = 0;
  if ((err = open_sound()) < 0) {
    /* TODO: if the pcm was busy, we should the same as sd-uss does.
       tell the caller that sound is available. in any other
       condition we should just return 0. */
    fprintf (stderr, "cannot open audio device (%s)\n", snd_strerror (err));
    return 0;
  }
  snd_pcm_close (alsa_playback_handle);
  alsa_playback_handle = 0;
  sound_available = 1;
  return 1;
}

int init_sound (void)
{
  int tmp;
  int rate;
  int dspbits;
  int alsamode;
  int channels;
  int err;
  snd_pcm_hw_params_t *hw_params;
  snd_pcm_uframes_t buffer_frames;

  dspbits = currprefs.sound_bits;
  channels = currprefs.sound_stereo ? 2 : 1;
  rate    = currprefs.sound_freq;

  have_sound = 0;
  alsa_playback_handle = 0;
  if ((err = open_sound()) < 0) {
    fprintf (stderr, "cannot open audio device (%s)\n", snd_strerror (err));
    goto nosound;
  }

  if (currprefs.sound_maxbsiz < 128 || currprefs.sound_maxbsiz > 16384) {
    fprintf (stderr, "Sound buffer size %d out of range.\n", currprefs.sound_maxbsiz);
    currprefs.sound_maxbsiz = 8192;
  }
  sndbufsize = currprefs.sound_maxbsiz;

  if ((err = snd_pcm_hw_params_malloc (&hw_params)) < 0) {
    fprintf (stderr, "cannot allocate hardware parameter structure (%s)\n",
	     snd_strerror (err));
    goto nosound;
  }

  if ((err = snd_pcm_hw_params_any (alsa_playback_handle, hw_params)) < 0) {
    fprintf (stderr, "cannot initialize hardware parameter structure (%s)\n",
	     snd_strerror (err));
    goto nosound;
  }

  if ((err = snd_pcm_hw_params_set_access (alsa_playback_handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0) {
    fprintf (stderr, "cannot set access type (%s)\n",
	     snd_strerror (err));
    goto nosound;
  }

  switch (dspbits) {
  case 8:
    alsamode = SND_PCM_FORMAT_U8;
    break;
  case 16:
    alsamode = SND_PCM_FORMAT_S16;
    break;
  default:
    fprintf(stderr, "%d bit samples not supported with uae's alsa\n", dspbits);
    goto nosound;
  }

  if ((err = snd_pcm_hw_params_set_format (alsa_playback_handle, hw_params, alsamode)) < 0) {
    fprintf (stderr, "cannot set sample format (%s)\n",
	     snd_strerror (err));
    goto nosound;
  }

  if ((err = snd_pcm_hw_params_set_channels (alsa_playback_handle, hw_params, channels)) < 0) {
    fprintf (stderr, "cannot set channel count (%s)\n",
	     snd_strerror (err));
    goto nosound;
  }

  if ((err = snd_pcm_hw_params_set_rate_near (alsa_playback_handle, hw_params, &rate, 0)) < 0) {
    fprintf (stderr, "cannot set sample rate (%s)\n",
	     snd_strerror (err));
    goto nosound;
  }

  alsa_to_frames_divisor = channels * dspbits / 8;
  buffer_frames = sndbufsize / alsa_to_frames_divisor;
  if ((err = snd_pcm_hw_params_set_period_size_near(alsa_playback_handle, hw_params, &buffer_frames, 0)) < 0) {
    fprintf (stderr, "cannot set period size near (%s)\n", snd_strerror (err));
    goto nosound;
  }

  if ((err = snd_pcm_hw_params (alsa_playback_handle, hw_params)) < 0) {
    fprintf (stderr, "cannot set parameters (%s)\n",
	     snd_strerror (err));
    goto nosound;
  }

  snd_pcm_hw_params_free (hw_params);

  if ((err = snd_pcm_prepare (alsa_playback_handle)) < 0) {
    fprintf (stderr, "cannot prepare audio interface for use (%s)\n",
	     snd_strerror (err));
    goto nosound;
  }

  scaled_sample_evtime = (unsigned long) MAXHPOS_PAL * MAXVPOS_PAL * VBLANK_HZ_PAL * CYCLE_UNIT / rate;
  scaled_sample_evtime_ok = 1;

  if (dspbits == 16) {
    init_sound_table16 ();
    sample_handler = currprefs.sound_stereo ? sample16s_handler : sample16_handler;
  } else {
    init_sound_table8 ();
    sample_handler = currprefs.sound_stereo ? sample8s_handler : sample8_handler;
  }
  have_sound = 1;
  sound_available = 1;
  printf ("Sound driver found and configured for %d bits at %d Hz, buffer is %d bytes\n", dspbits, rate, sndbufsize);

  sndbufpt = sndbuffer;
  return 1;

 nosound:
  have_sound = 0;
  close_sound();
  return 0;
}

 /*
  * UAE - The Un*x Amiga Emulator
  *
  * Support for Linux/USS sound
  *
  * Copyright 1997 Bernd Schmidt
  */

#include "sysconfig.h"
#include "sysdeps.h"

#include "options.h"
#include "memory.h"
#include "events.h"
#include "custom.h"
#include "newcpu.h"
#include "gensound.h"
#include "sounddep/sound.h"
#include "threaddep/thread.h"
#include "SDL_audio.h"

int sound_fd;
static int have_sound = 0;
static unsigned long formats;

uae_u16 sndbuffer[2][44100];
uae_u16 *sndbufpt, *sndbuf_base, *callback_sndbuf;
int which_buffer;
int sndbufsize;
static SDL_AudioSpec spec;

static smp_comm_pipe to_sound_pipe;
static uae_sem_t data_available_sem, callback_done_sem, sound_init_sem;

static int dont_block;

static int in_callback, closing_sound;

static void sound_callback (void *userdata, Uint8 *stream, int len)
{
    if (closing_sound)
	return;
    in_callback = 1;
    delaying_for_sound = 0;

    /* Wait for data to finish.  */
    uae_sem_wait (&data_available_sem);
    if (! closing_sound) {
	memcpy (stream, callback_sndbuf, sndbufsize);

	/* Notify writer that we're done.  */
	if (!dont_block)
	    uae_sem_post (&callback_done_sem);
    }
    in_callback = 0;
}

void finish_sound_buffer (void)
{
    dont_block = currprefs.m68k_speed == -1 && (!regs.stopped || active_fs_packets > 0);
    callback_sndbuf = sndbuf_base;

    if (dont_block)
	delaying_for_sound = 1;
    uae_sem_post (&data_available_sem);
    if (!dont_block)
	uae_sem_wait (&callback_done_sem);
    sndbufpt = sndbuf_base = sndbuffer[which_buffer ^= 1];
}

/* Try to determine whether sound is available.  This is only for GUI purposes.  */
int setup_sound (void)
{
    int size = currprefs.sound_maxbsiz;

    spec.freq = currprefs.sound_freq;
    spec.format = AUDIO_S16;
    spec.channels = 2;
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
    SDL_AudioSpec obtained;
    int size = currprefs.sound_maxbsiz;

    sync_with_sound = 0;

    spec.freq = currprefs.sound_freq;
    spec.format = AUDIO_S16;
    spec.channels = 2;

    /* Always interpret buffer size as number of samples, not as actual
       buffer size.  Of course, since 8192 is the default, we'll have to
       scale that to a sane value (assuming that otherwise 16 bits and
       stereo would have been enabled and we'd have done the shift by
       two anyway).  */
    size >>= spec.channels;

    while (size & (size - 1))
	size &= size - 1;
    if (size < 512)
	size = 512;
    spec.samples = size;
    spec.callback = sound_callback;
    spec.userdata = 0;

    if (SDL_OpenAudio (&spec, &obtained) < 0) {
	write_log ("Couldn't open audio: %s\n", SDL_GetError());
	return 0;
    }
    have_sound = 1;

    obtainedfreq = obtained.freq;

    init_sound_table16 ();
    sample_handler = sample16s_handler;

    sound_available = 1;
    sndbufpt = sndbuf_base = sndbuffer[which_buffer = 0];
    sndbufsize = size * 2 * obtained.channels;
    write_log ("SDL sound driver found and configured at %d Hz, buffer is %d samples (%d ms).\n",
	       obtainedfreq, obtained.samples, obtained.samples * 1000 / obtainedfreq);
    sync_with_sound = 1;
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
   and the callback works through semaphores.  In theory, this is unnecessary,
   since SDL uses a sound thread internally, and the callback runs in its
   context.  But we don't want to depend on SDL's internals too much.  */
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
    sync_with_sound = 0;

    if (! have_sound)
	return;

    SDL_PauseAudio (1);
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
    SDL_PauseAudio (0);
}

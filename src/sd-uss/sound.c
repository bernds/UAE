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

#include <sys/ioctl.h>

#ifdef HAVE_SYS_SOUNDCARD_H
#include <sys/soundcard.h>
#elif defined HAVE_MACHINE_SOUNDCARD_H
#include <machine/soundcard.h>
#else
#error "Something went wrong during configuration."
#endif

static smp_comm_pipe to_sound_pipe;
static uae_sem_t sound_comm_sem;

static int sound_fd;
static int have_sound = 0;
static int dont_block;
static unsigned long formats;

static int which_buffer;
static uae_u16 sndbuffer[2][44100];
uae_u16 *sndbufpt, *sndbuf_base;
int sndbufsize;

static int exact_log2 (int v)
{
    int l = 0;
    while ((v >>= 1) != 0)
	l++;
    return l;
}

void finish_sound_buffers (void)
{
    dont_block = currprefs.m68k_speed == -1 && (!regs.stopped || active_fs_packets > 0);
    if (!dont_block) {
	write (sound_fd, sndbuffer[which_buffer], sndbufsize);
    } else {
	write_comm_pipe_int (&to_sound_pipe, 2, 1);
	uae_sem_wait (&sound_comm_sem);
    }
    sndbufpt = sndbuf_base = sndbuffer[which_buffer ^= 1];
}

void close_sound (void)
{
    sync_with_sound = 0;
    if (have_sound)
	close (sound_fd);

    write_comm_pipe_int (&to_sound_pipe, 1, 1);
    uae_sem_wait (&sound_comm_sem);
    uae_sem_destroy (&sound_comm_sem);
}

/* Try to determine whether sound is available.  This is only for GUI purposes.  */
int setup_sound (void)
{
    sound_fd = open ("/dev/dsp", O_WRONLY);

    sound_available = 0;

    if (sound_fd < 0) {
	perror ("Can't open /dev/dsp");
	if (errno == EBUSY) {
	    /* We can hope, can't we ;) */
	    sound_available = 1;
	    return 1;
	}
	return 0;
    }

    if (ioctl (sound_fd, SNDCTL_DSP_GETFMTS, &formats) == -1) {
	perror ("ioctl failed - can't use sound");
	close (sound_fd);
	return 0;
    }

    sound_available = 1;
    close (sound_fd);
    return 1;
}

static void open_sound (void)
{
    int tmp;
    int rate;
    int dspbits;

    sync_with_sound = 0;

    sound_fd = open ("/dev/dsp", O_WRONLY);
    have_sound = !(sound_fd < 0);
    if (! have_sound) {
	perror ("Can't open /dev/dsp");
	if (errno != EBUSY)
	    sound_available = 0;
	return;
    }
    if (ioctl (sound_fd, SNDCTL_DSP_GETFMTS, &formats) == -1) {
	perror ("ioctl failed - can't use sound");
	goto out_err;
    }

    if (currprefs.sound_maxbsiz < 128 || currprefs.sound_maxbsiz > 16384) {
	fprintf (stderr, "Sound buffer size %d out of range.\n", currprefs.sound_maxbsiz);
	currprefs.sound_maxbsiz = 8192;
    }

    tmp = 0x00040000 + exact_log2 (currprefs.sound_maxbsiz);
    ioctl (sound_fd, SNDCTL_DSP_SETFRAGMENT, &tmp);
    ioctl (sound_fd, SNDCTL_DSP_GETBLKSIZE, &sndbufsize);

    dspbits = 16;
    ioctl (sound_fd, SNDCTL_DSP_SAMPLESIZE, &dspbits);
    ioctl (sound_fd, SOUND_PCM_READ_BITS, &dspbits);
    if (dspbits != 16) {
	fprintf (stderr, "Can't use sound with 16 bits\n");
	goto out_err;
    }

    tmp = currprefs.sound_stereo;
    ioctl (sound_fd, SNDCTL_DSP_STEREO, &tmp);

    rate = currprefs.sound_freq;
    ioctl (sound_fd, SNDCTL_DSP_SPEED, &rate);
    ioctl (sound_fd, SOUND_PCM_READ_RATE, &rate);
    /* Some soundcards have a bit of tolerance here. */
    if (rate < currprefs.sound_freq * 90 / 100 || rate > currprefs.sound_freq * 110 / 100) {
	fprintf (stderr, "Can't use sound with desired frequency %d\n", currprefs.sound_freq);
	goto out_err;
    }

    obtainedfreq = rate;

    if (!(formats & AFMT_S16_NE))
	goto out_err;
    init_sound_table16 ();
    sample_handler = currprefs.sound_stereo ? sample16s_handler : sample16_handler;

    sound_available = 1;
    printf ("Sound driver found and configured at %d Hz, buffer is %d bytes (%d ms).\n",
	    rate, sndbufsize, sndbufsize * 1000 / (rate * 2 * (currprefs.sound_stereo ? 2 : 1)));
    sndbufpt = sndbuf_base = sndbuffer[which_buffer = 0];

    sync_with_sound = 1;
    return;

  out_err:
    close (sound_fd);
    have_sound = 0;
    return;
}

static void *sound_thread (void *dummy)
{
    uae_u16 *base;
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
	    base = sndbuf_base;
	    if (dont_block)
		uae_sem_post (&sound_comm_sem);

	    write (sound_fd, sndbuf_base, sndbufsize);
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

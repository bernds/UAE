 /*
  * UAE - The Un*x Amiga Emulator
  *
  * Support for Linux/USS sound
  *
  * Copyright 1997 Bernd Schmidt
  * Copyright 2003 Richard Drummond
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

#include <sys/ioctl.h>

#ifdef HAVE_SYS_SOUNDCARD_H
#include <sys/soundcard.h>
#elif defined HAVE_MACHINE_SOUNDCARD_H
#include <machine/soundcard.h>
#else
#error "Something went wrong during configuration."
#endif

int sound_fd;
static int have_sound = 0;
static unsigned long formats;

uae_u16 sndbuffer[44100];
uae_u16 *sndbufpt;
int sndbufsize;
static int obtainedfreq;

static int exact_log2 (int v)
{
    int l = 0;
    while ((v >>= 1) != 0)
	l++;
    return l;
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

void close_sound (void)
{
    if (have_sound)
	close (sound_fd);
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

int init_sound (void)
{
    int tmp;
    int rate;
    int dspbits;

    sound_fd = open ("/dev/dsp", O_WRONLY);
    have_sound = !(sound_fd < 0);
    if (! have_sound) {
	perror ("Can't open /dev/dsp");
	if (errno != EBUSY)
	    sound_available = 0;
	return 0;
    }
    if (ioctl (sound_fd, SNDCTL_DSP_GETFMTS, &formats) == -1) {
	perror ("ioctl failed - can't use sound");
	close (sound_fd);
	have_sound = 0;
	return 0;
    }

    if (currprefs.sound_maxbsiz < 128 || currprefs.sound_maxbsiz > 16384) {
	fprintf (stderr, "Sound buffer size %d out of range.\n", currprefs.sound_maxbsiz);
	currprefs.sound_maxbsiz = 8192;
    }

    tmp = 0x00040000 + exact_log2 (currprefs.sound_maxbsiz);
    ioctl (sound_fd, SNDCTL_DSP_SETFRAGMENT, &tmp);
    ioctl (sound_fd, SNDCTL_DSP_GETBLKSIZE, &sndbufsize);

    dspbits = currprefs.sound_bits;
    ioctl (sound_fd, SNDCTL_DSP_SAMPLESIZE, &dspbits);
    ioctl (sound_fd, SOUND_PCM_READ_BITS, &dspbits);
    if (dspbits != currprefs.sound_bits) {
	fprintf (stderr, "Can't use sound with %d bits\n", currprefs.sound_bits);
	return 0;
    }

    tmp = currprefs.stereo;
    ioctl (sound_fd, SNDCTL_DSP_STEREO, &tmp);

    rate = currprefs.sound_freq;
    ioctl (sound_fd, SNDCTL_DSP_SPEED, &rate);
    ioctl (sound_fd, SOUND_PCM_READ_RATE, &rate);
    /* Some soundcards have a bit of tolerance here. */
    if (rate < currprefs.sound_freq * 90 / 100 || rate > currprefs.sound_freq * 110 / 100) {
	fprintf (stderr, "Can't use sound with desired frequency %d\n", currprefs.sound_freq);
	return 0;
    }

    update_sound(vblank_hz );
    obtainedfreq = currprefs.sound_freq;

    if (dspbits == 16) {
	/* Will this break horribly on bigendian machines? Possible... Not any more - Rich */
	if (!(formats & AFMT_S16_NE))
	    return 0;
	init_sound_table16 ();
	sample_handler = currprefs.stereo ? sample16s_handler : sample16_handler;
    } else {
	if (!(formats & AFMT_U8))
	    return 0;
	init_sound_table8 ();
	sample_handler = currprefs.stereo ? sample8s_handler : sample8_handler;
    }
    sound_available = 1;
    printf ("Sound driver found and configured for %d bits at %d Hz, buffer is %d bytes\n",
	    dspbits, rate, sndbufsize);
    sndbufpt = sndbuffer;

#ifdef FRAME_RATE_HACK
    vsynctime = vsynctime * 9 / 10;
#endif

    return 1;
}

void reset_sound (void)
{
}

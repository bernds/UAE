 /* 
  * UAE - The Un*x Amiga Emulator
  * 
  * Support for the Silicon Graphics Audio Library (AL)
  * 
  * Copyright 1998 Ari Heikkinen
  */

#include "sysconfig.h"
#include "sysdeps.h"

#include "config.h"
#include "options.h"
#include "memory.h"
#include "events.h"
#include "custom.h"
#include "audio.h"
#include "gensound.h"
#include "sounddep/sound.h"

#include <dmedia/audio.h>

uae_u16 sndbuffer[88200];
uae_u16 *sndbufpt;
int sndblocksize;

ALport al_port = 0;
int to_frames_divisor = 1;

static double old_rate = 0;

static ALport open_audio(char *name, int width, int channels, int buffers)
{
	ALconfig alc;
	ALport alp;

	if (channels <= 0 || buffers <= 0)
		return 0;

	alc = alNewConfig();
	if (!alc)
		return 0;
	if (alSetWidth(alc, width)) {
		alFreeConfig(alc);
		return 0;
	}
	if (alSetChannels(alc, channels)) {
		alFreeConfig(alc);
		return 0;
	}
	if (alSetQueueSize(alc, buffers)) {
		alFreeConfig(alc);
		return 0;
	}
	alp = alOpenPort(name, "w", alc);
	alFreeConfig(alc);
	return alp ? alp : 0;
}

static double get_sample_rate(void)
{
	ALpv alpv;

	alpv.param = AL_RATE;
	if (alGetParams(AL_DEFAULT_OUTPUT, &alpv, 1) != 1)
		return 0;
	return alFixedToDouble(alpv.value.ll);
}

static int set_sample_rate(double rate)
{
	ALpv alpv[2];

	if (rate < 0)
		return -1;
	alpv[0].param = AL_MASTER_CLOCK;
	alpv[0].value.i = AL_CRYSTAL_MCLK_TYPE;
	alpv[1].param = AL_RATE;
	alpv[1].value.ll = alDoubleToFixed(rate);
	if (alSetParams(AL_DEFAULT_OUTPUT, alpv, 2) != 2)
		return -1;
	return 0;
}

int init_sound(void)
{
	int width = AL_SAMPLE_16;
/* if someone knows how to get 8bit sound working on SGI fix it and
   use this instead of the line above...
	int width = currprefs.sound_bits >= 12 ? AL_SAMPLE_16 : AL_SAMPLE_8;
*/
	int channels = currprefs.stereo ? 2 : 1;
	int rate = currprefs.sound_freq;

	if (currprefs.sound_maxbsiz < 128 || currprefs.sound_maxbsiz > 44100) {
		fprintf(stderr, "Sound buffer size %d out of range.\n",
			currprefs.sound_maxbsiz);
		currprefs.sound_maxbsiz = 8192;
	}

	al_port = open_audio("UAE Sound", width, channels,
				currprefs.sound_maxbsiz * 2);
	if (!al_port) {
		sound_available = 0;
		return 0;
	}

	old_rate = get_sample_rate();
	if (old_rate <= 0 || set_sample_rate((double)rate)
			|| (rate = (int)get_sample_rate()) <= 0) {
		alClosePort(al_port);
		sound_available = 0;
		return 0;
	}

 	scaled_sample_evtime = (unsigned long)MAXHPOS_PAL * MAXVPOS_PAL * VBLANK_HZ_PAL * CYCLE_UNIT / rate;
 	scaled_sample_evtime_ok = 1;

	sndbufpt = sndbuffer;
	if (width == AL_SAMPLE_16) {
		sndblocksize = currprefs.sound_maxbsiz * channels * 2;
		sample_handler = currprefs.stereo
					? sample16s_handler
					: sample16_handler;
		to_frames_divisor = channels * 2;
		init_sound_table16();
	} else {
		sndblocksize = currprefs.sound_maxbsiz * channels;
		sample_handler = currprefs.stereo
					? sample8s_handler
					: sample8_handler;
		to_frames_divisor = channels;
		init_sound_table8();
	}
	printf("Sound driver configured for %d bits (%s) at %d Hz, "
		"buffer is %d bytes\n", width == AL_SAMPLE_16 ? 16 : 8,
		channels == 2 ? "stereo" : "mono", rate, sndblocksize);
	return 1;
}

int setup_sound(void)
{
	ALport alp;

	sound_available = 0;
	alp = open_audio("UAE Sound (test)", AL_SAMPLE_16, 1, 44100);
	if (!alp) {
		printf("SetupSound failed - sound hardware unavailable.\n");
		return 0;
	}
	sound_available = 1;
	alClosePort(alp);
	printf("SetupSound ok - sound hardware available.\n");
	return 1;
}

void close_sound(void)
{
	if (old_rate > 0)
		(void)set_sample_rate(old_rate);
	if (al_port)
		alClosePort(al_port);
	printf("CloseSound ok\n");
}

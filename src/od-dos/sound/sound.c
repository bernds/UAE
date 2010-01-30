 /*
  * UAE - The Un*x Amiga Emulator
  *
  * Support for DOS
  *
  * Copyright 1997, 1998 Gustavo Goedert
  */

/****** This file is broken.  The sample handlers have to go.  ******/

#include "sysconfig.h"
#include "sysdeps.h"

#include "config.h"
#include "options.h"
#include "memory.h"
#include "custom.h"
#include "audio.h"
#include "gensound.h"
#include "sounddep/sound.h"
#include "events.h"

#include <go32.h>
#include <sys/farptr.h>
#include <time.h>

#include "sound/sb.h"

typedef uae_s8 sample8_t;
#define DO_CHANNEL_1(v, c) do { (v) *= audio_channel[c].vol; } while (0)
#define SBASEVAL8(logn) ((logn) == 1 ? SOUND8_BASE_VAL << 7 : SOUND8_BASE_VAL << 8)
#define SBASEVAL16(logn) ((logn) == 1 ? SOUND16_BASE_VAL >> 1 : SOUND16_BASE_VAL)
#define FINISH_DATA(b,logn) do { if (14 - (b) + (logn) > 0) data >>= 14 - (b) + (logn); else data <<= (b) - 14 - (logn); } while (0);

uae_u16 sndbuffer[44100];
uae_u16 *sndbufpt;
int sndbufsize;
extern int sound_table[256][64];
int dspbits;

int sound_curfreq;

void (*SND_DirectWrite)(unsigned int size, int freq); // Pointer to function that plays data on card
#define SND_DirectWrite SB_DirectWrite
volatile int IsPlaying = 0;
int CurrentBuffer = 0;
unsigned int direct_buffers[2], direct_sndbufpt;

void direct_mono_sample16_handler(void);
void direct_mono_sample8_handler(void);
void direct_stereo_sample16_handler(void);
void direct_stereo_sample8_handler(void);
void interpol_freq(int new_freq);
void direct_check_sound_buffers(void);

#define INTERPOL_SIZE 8
int freq_buf[INTERPOL_SIZE];
int buf_tot, buf_pos=0;

inline void interpol_freq(int new_freq) {
    buf_tot = buf_tot - freq_buf[buf_pos] + new_freq;
    freq_buf[buf_pos] = new_freq;
    buf_pos++;
    if (buf_pos == INTERPOL_SIZE) buf_pos = 0;
    sound_curfreq = buf_tot/INTERPOL_SIZE;
    if (sound_curfreq<currprefs.sound_freq) {
	sound_curfreq |= 0xff;
	if (sound_curfreq<5512)
	    sound_curfreq = 5512;
    }
    if (sound_curfreq>currprefs.sound_freq)
	sound_curfreq = currprefs.sound_freq;
}

unsigned int sound_bytes = 0, last_clock = 0;

inline void direct_check_sound_buffers(void) {
    int played_size = direct_sndbufpt - direct_buffers[CurrentBuffer];

    sound_bytes++;
    if ((!IsPlaying) && played_size >= currprefs.sound_minbsiz) {
	SND_DirectWrite(played_size, sound_curfreq);
	if (currprefs.sound_adjust) {
	    unsigned int cur_clock, new_freq;

	    if (last_clock == 0)
		last_clock = uclock();
	    else {
		cur_clock = uclock();
		new_freq = (unsigned long long) sound_bytes * UCLOCKS_PER_SEC / (cur_clock-last_clock);
		interpol_freq(new_freq);
		last_clock = cur_clock;
	    }
	    sound_bytes = 0;
	}
	CurrentBuffer = !CurrentBuffer;
	direct_sndbufpt = direct_buffers[CurrentBuffer];
    } else if (played_size >= currprefs.sound_maxbsiz) {
	if (currprefs.sound_adjust)
	    interpol_freq(currprefs.sound_freq);
	while(IsPlaying);
	SND_DirectWrite(currprefs.sound_maxbsiz, sound_curfreq);
	CurrentBuffer = !CurrentBuffer;
	direct_sndbufpt = direct_buffers[CurrentBuffer];
	last_clock=0;
    }
}

void direct_mono_sample16_handler(void)
{
    uae_u32 data0 = audio_channel[0].current_sample;
    uae_u32 data1 = audio_channel[1].current_sample;
    uae_u32 data2 = audio_channel[2].current_sample;
    uae_u32 data3 = audio_channel[3].current_sample;
    DO_CHANNEL_1 (data0, 0);
    DO_CHANNEL_1 (data1, 1);
    DO_CHANNEL_1 (data2, 2);
    DO_CHANNEL_1 (data3, 3);
    data0 &= audio_channel[0].adk_mask;
    data1 &= audio_channel[1].adk_mask;
    data2 &= audio_channel[2].adk_mask;
    data3 &= audio_channel[3].adk_mask;
    data0 += data1;
    data0 += data2;
    data0 += data3;
    {
	uae_u32 data = SBASEVAL16(2) + data0;
	FINISH_DATA(16, 2);
	_farnspokew(direct_sndbufpt, data);
	direct_sndbufpt += 2;
    }

    direct_check_sound_buffers();

    eventtab[ev_sample].evtime = cycles + sample_evtime;
    eventtab[ev_sample].oldcycles = cycles;
}

void direct_mono_sample8_handler(void)
{
    uae_u32 data0 = audio_channel[0].current_sample;
    uae_u32 data1 = audio_channel[1].current_sample;
    uae_u32 data2 = audio_channel[2].current_sample;
    uae_u32 data3 = audio_channel[3].current_sample;
    DO_CHANNEL_1 (data0, 0);
    DO_CHANNEL_1 (data1, 1);
    DO_CHANNEL_1 (data2, 2);
    DO_CHANNEL_1 (data3, 3);
    data0 &= audio_channel[0].adk_mask;
    data1 &= audio_channel[1].adk_mask;
    data2 &= audio_channel[2].adk_mask;
    data3 &= audio_channel[3].adk_mask;
    data0 += data1;
    data0 += data2;
    data0 += data3;
    {
	uae_u32 data = SBASEVAL8(2) + data0;
	FINISH_DATA(8, 2);
	_farnspokeb(direct_sndbufpt++, data);
    }

    direct_check_sound_buffers();

    eventtab[ev_sample].evtime = cycles + sample_evtime;
    eventtab[ev_sample].oldcycles = cycles;
}

void direct_stereo_sample16_handler(void)
{
    uae_u32 data0 = audio_channel[0].current_sample;
    uae_u32 data1 = audio_channel[1].current_sample;
    uae_u32 data2 = audio_channel[2].current_sample;
    uae_u32 data3 = audio_channel[3].current_sample;
    DO_CHANNEL_1 (data0, 0);
    DO_CHANNEL_1 (data1, 1);
    DO_CHANNEL_1 (data2, 2);
    DO_CHANNEL_1 (data3, 3);

    data0 &= audio_channel[0].adk_mask;
    data1 &= audio_channel[1].adk_mask;
    data2 &= audio_channel[2].adk_mask;
    data3 &= audio_channel[3].adk_mask;

    data0 += data3;
    {
	uae_u32 data = SBASEVAL16(1) + data0;
	FINISH_DATA (16, 1);
	_farnspokew(direct_sndbufpt, data);
	direct_sndbufpt += 2;
    }

    data1 += data2;
    {
	uae_u32 data = SBASEVAL16(1) + data1;
	FINISH_DATA (16, 1);
	_farnspokew(direct_sndbufpt, data);
	direct_sndbufpt += 2;
    }

    direct_check_sound_buffers();

    eventtab[ev_sample].evtime = cycles + sample_evtime;
    eventtab[ev_sample].oldcycles = cycles;
}

void direct_stereo_sample8_handler(void)
{
    uae_u32 data0 = audio_channel[0].current_sample;
    uae_u32 data1 = audio_channel[1].current_sample;
    uae_u32 data2 = audio_channel[2].current_sample;
    uae_u32 data3 = audio_channel[3].current_sample;
    DO_CHANNEL_1 (data0, 0);
    DO_CHANNEL_1 (data1, 1);
    DO_CHANNEL_1 (data2, 2);
    DO_CHANNEL_1 (data3, 3);

    data0 &= audio_channel[0].adk_mask;
    data1 &= audio_channel[1].adk_mask;
    data2 &= audio_channel[2].adk_mask;
    data3 &= audio_channel[3].adk_mask;

    data0 += data3;
    {
	uae_u32 data = SBASEVAL8(1) + data0;
	FINISH_DATA (8, 1);
	_farnspokeb(direct_sndbufpt++, data);
    }
    data1 += data2;
    {
	uae_u32 data = SBASEVAL8(1) + data1;
	FINISH_DATA (8, 1);
	_farnspokeb(direct_sndbufpt++, data);
    }

    direct_check_sound_buffers();

    eventtab[ev_sample].evtime = cycles + sample_evtime;
    eventtab[ev_sample].oldcycles = cycles;
}

int init_sound (void)
{
    int tmp;
    int rate;
    int i;

    if (currprefs.sound_minbsiz > currprefs.sound_maxbsiz) {
	fprintf(stderr, "Minimum sound buffer size bigger then maximum, exchanging.\n");
	tmp = currprefs.sound_minbsiz;
	currprefs.sound_minbsiz = currprefs.sound_maxbsiz;
	currprefs.sound_maxbsiz = tmp;
    }

    dspbits = currprefs.sound_bits;
    rate = currprefs.sound_freq;
    sndbufsize = currprefs.sound_maxbsiz;

    if (SB_DetectInitSound(&dspbits, &rate, &sndbufsize, direct_buffers, &currprefs.stereo));
    else if (0/*OTHER_CARD_DETECT_ROUTINE*/);
    else
	return 0;

    currprefs.sound_freq = rate;
    currprefs.sound_minbsiz = (currprefs.sound_minbsiz>>2)<<2;
    currprefs.sound_maxbsiz = sndbufsize;

    sample_evtime = (long)MAXHPOS_PAL * MAXVPOS_PAL * VBLANK_HZ_PAL / rate;

    if (dspbits == 16) {
	init_sound_table16 ();
	if (currprefs.stereo)
	    eventtab[ev_sample].handler = direct_stereo_sample16_handler;
	else
	    eventtab[ev_sample].handler = direct_mono_sample16_handler;
    } else {
	init_sound_table8 ();
	if (currprefs.stereo)
	    eventtab[ev_sample].handler = direct_stereo_sample8_handler;
	else
	    eventtab[ev_sample].handler = direct_mono_sample8_handler;
    }
    printf ("Sound driver found and configured for %d bits at %d Hz, buffer is %d:%d bytes\n",
	    dspbits, rate, currprefs.sound_minbsiz, currprefs.sound_maxbsiz);
    sndbufpt = sndbuffer;
    direct_sndbufpt = direct_buffers[0];

    sound_curfreq = currprefs.sound_freq;
    for (i=0; i<INTERPOL_SIZE; i++)
	freq_buf[i] = sound_curfreq;
    buf_tot = sound_curfreq * INTERPOL_SIZE;

#ifdef FRAME_RATE_HACK
    vsynctime = vsynctime * 9 / 10;
#endif

    return 1;
}

int setup_sound (void)
{
    sound_available = 1;
    return 1;
}

void close_sound(void)
{
}

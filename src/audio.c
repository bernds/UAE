 /*
  * UAE - The Un*x Amiga Emulator
  *
  * OS specific functions
  *
  * Copyright 1995, 1996, 1997 Bernd Schmidt
  * Copyright 1996 Marcus Sundberg
  * Copyright 1996 Manfred Thole
  */

#include "sysconfig.h"
#include "sysdeps.h"

#include "config.h"
#include "options.h"
#include "memory.h"
#include "custom.h"
#include "newcpu.h"
#include "autoconf.h"
#include "gensound.h"
#include "sounddep/sound.h"
#include "events.h"
#include "audio.h"
#include "savestate.h"

struct audio_channel_data audio_channel[6];
int sound_available = 0;
int sound_table[64][256];
void (*sample_handler) (void);

/* Zero if we want to produce Paula output, nonzero if we want to produce
   AHI output.  */
static int ahi_enabled;

/* Bit 0 is set if the right channel raised an interrupt, bit 1 is set if the
   left channel raised an interrupt.  */
static int ahi_interrupt_state;

unsigned long sample_evtime, scaled_sample_evtime;
int scaled_sample_evtime_ok;

static unsigned long last_cycles, next_sample_evtime;

void init_sound_table16 (void)
{
    int i,j;

    for (i = 0; i < 256; i++)
	for (j = 0; j < 64; j++)
	    sound_table[j][i] = j * (uae_s8)i * (currprefs.stereo ? 2 : 1);
}

void init_sound_table8 (void)
{
    int i,j;

    for (i = 0; i < 256; i++)
	for (j = 0; j < 64; j++)
	    sound_table[j][i] = (j * (uae_s8)i * (currprefs.stereo ? 2 : 1)) / 256;
}

#define MULTIPLICATION_PROFITABLE

#ifdef MULTIPLICATION_PROFITABLE
typedef uae_s8 sample8_t;
#define DO_CHANNEL_1(v, c) do { (v) *= audio_channel[c].vol; } while (0)
#define SBASEVAL8(logn) ((logn) == 1 ? SOUND8_BASE_VAL << 7 : SOUND8_BASE_VAL << 8)
#define SBASEVAL16(logn) ((logn) == 1 ? SOUND16_BASE_VAL >> 1 : SOUND16_BASE_VAL)
#define FINISH_DATA(data,b,logn) do { if (14 - (b) + (logn) > 0) (data) >>= 14 - (b) + (logn); else (data) <<= (b) - 14 - (logn); } while (0);
#else
typedef uae_u8 sample8_t;
#define DO_CHANNEL_1(v, c) do { (v) = audio_channel[c].voltbl[(v)]; } while (0)
#define SBASEVAL8(logn) SOUND8_BASE_VAL
#define SBASEVAL16(logn) SOUND16_BASE_VAL
#define FINISH_DATA(b,logn)
#endif

/* Always put the right word before the left word.  */
#define DELAY_BUFFER 32
static uae_u32 right_word_saved[DELAY_BUFFER];
static uae_u32 left_word_saved[DELAY_BUFFER];
static int saved_ptr;

STATIC_INLINE void put_sound_word_right (uae_u32 w)
{
    if (currprefs.mixed_stereo) {
	right_word_saved[saved_ptr] = w;
	return;
    }

    PUT_SOUND_WORD_RIGHT (w);
}

STATIC_INLINE void put_sound_word_left (uae_u32 w)
{
    if (currprefs.mixed_stereo) {
	uae_u32 rold, lold, rnew, lnew, tmp;

	left_word_saved[saved_ptr] = w;
	lnew = w - SOUND16_BASE_VAL;
	rnew = right_word_saved[saved_ptr] - SOUND16_BASE_VAL;

	saved_ptr = (saved_ptr + 1) & (DELAY_BUFFER - 1);
	lold = left_word_saved[saved_ptr] - SOUND16_BASE_VAL;
	tmp = (rnew * 5 + lold * 3) >> 3;
	tmp += SOUND16_BASE_VAL;
	PUT_SOUND_WORD_RIGHT (tmp);

	rold = right_word_saved[saved_ptr] - SOUND16_BASE_VAL;
	w = (lnew * 5 + rold * 3) >> 3;
    }
    PUT_SOUND_WORD_LEFT (w);
}

#define DO_CHANNEL(v, c) do { (v) &= audio_channel[c].adk_mask; data += v; } while (0);

void sample16_handler (void)
{
    if (ahi_enabled) {
	uae_u16 word = audio_channel[4].current_sample + audio_channel[5].current_sample;
	word >>= 1;
	word += SOUND16_BASE_VAL;
	PUT_SOUND_WORD (word);
    } else {
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
	    FINISH_DATA (data, 16, 2);
	    PUT_SOUND_WORD (data);
	}
    }
    check_sound_buffers ();
}

void sample16i_rh_handler (void)
{
    if (ahi_enabled) {
        uae_u16 word = audio_channel[4].current_sample + audio_channel[5].current_sample;
	word >>= 1;
	word += SOUND16_BASE_VAL;
	PUT_SOUND_WORD (word);
    } else {
	unsigned long delta, ratio;

	uae_u32 data0 = audio_channel[0].current_sample;
	uae_u32 data1 = audio_channel[1].current_sample;
	uae_u32 data2 = audio_channel[2].current_sample;
	uae_u32 data3 = audio_channel[3].current_sample;
	uae_u32 data0p = audio_channel[0].last_sample;
	uae_u32 data1p = audio_channel[1].last_sample;
	uae_u32 data2p = audio_channel[2].last_sample;
	uae_u32 data3p = audio_channel[3].last_sample;
	DO_CHANNEL_1 (data0, 0);
	DO_CHANNEL_1 (data1, 1);
	DO_CHANNEL_1 (data2, 2);
	DO_CHANNEL_1 (data3, 3);
	DO_CHANNEL_1 (data0p, 0);
	DO_CHANNEL_1 (data1p, 1);
	DO_CHANNEL_1 (data2p, 2);
	DO_CHANNEL_1 (data3p, 3);

	data0 &= audio_channel[0].adk_mask;
	data0p &= audio_channel[0].adk_mask;
	data1 &= audio_channel[1].adk_mask;
	data1p &= audio_channel[1].adk_mask;
	data2 &= audio_channel[2].adk_mask;
	data2p &= audio_channel[2].adk_mask;
	data3 &= audio_channel[3].adk_mask;
	data3p &= audio_channel[3].adk_mask;

	/* linear interpolation and summing up... */
	delta = audio_channel[0].per;
	ratio = ((audio_channel[0].evtime % delta) << 8) / delta;
	data0 = (data0 * (256 - ratio) + data0p * ratio) >> 8;
	delta = audio_channel[1].per;
	ratio = ((audio_channel[1].evtime % delta) << 8) / delta;
	data0 += (data1 * (256 - ratio) + data1p * ratio) >> 8;
	delta = audio_channel[2].per;
	ratio = ((audio_channel[2].evtime % delta) << 8) / delta;
	data0 += (data2 * (256 - ratio) + data2p * ratio) >> 8;
	delta = audio_channel[3].per;
	ratio = ((audio_channel[3].evtime % delta) << 8) / delta;
	data0 += (data3 * (256 - ratio) + data3p * ratio) >> 8;

	{
	    uae_u32 data = SBASEVAL16(2) + data0;
	    FINISH_DATA (data, 16, 2);
	    PUT_SOUND_WORD (data);
	}

	check_sound_buffers ();
    }
}

void sample16i_crux_handler (void)
{
    if (ahi_enabled) {
        uae_u16 word = audio_channel[4].current_sample + audio_channel[5].current_sample;
	word >>= 1;
	word += SOUND16_BASE_VAL;
	PUT_SOUND_WORD (word);
    } else {
	uae_u32 data0 = audio_channel[0].current_sample;
	uae_u32 data1 = audio_channel[1].current_sample;
	uae_u32 data2 = audio_channel[2].current_sample;
	uae_u32 data3 = audio_channel[3].current_sample;
	uae_u32 data0p = audio_channel[0].last_sample;
	uae_u32 data1p = audio_channel[1].last_sample;
	uae_u32 data2p = audio_channel[2].last_sample;
	uae_u32 data3p = audio_channel[3].last_sample;
	DO_CHANNEL_1 (data0, 0);
	DO_CHANNEL_1 (data1, 1);
	DO_CHANNEL_1 (data2, 2);
	DO_CHANNEL_1 (data3, 3);
	DO_CHANNEL_1 (data0p, 0);
	DO_CHANNEL_1 (data1p, 1);
	DO_CHANNEL_1 (data2p, 2);
	DO_CHANNEL_1 (data3p, 3);

	data0 &= audio_channel[0].adk_mask;
	data0p &= audio_channel[0].adk_mask;
	data1 &= audio_channel[1].adk_mask;
	data1p &= audio_channel[1].adk_mask;
	data2 &= audio_channel[2].adk_mask;
	data2p &= audio_channel[2].adk_mask;
	data3 &= audio_channel[3].adk_mask;
	data3p &= audio_channel[3].adk_mask;

	{    
	    struct audio_channel_data *cdp;
	    unsigned long ratio, ratio1;
#define INTERVAL (scaled_sample_evtime * 3)
	    cdp = audio_channel + 0;
	    ratio1 = cdp->per - cdp->evtime;
	    ratio = (ratio1 << 12) / INTERVAL;
	    if (cdp->evtime < scaled_sample_evtime || ratio1 >= INTERVAL)
		ratio = 4096;
	    data0 = (data0 * ratio + data0p * (4096 - ratio)) >> 12;

	    cdp = audio_channel + 1;
	    ratio1 = cdp->per - cdp->evtime;
	    ratio = (ratio1 << 12) / INTERVAL;
	    if (cdp->evtime < scaled_sample_evtime || ratio1 >= INTERVAL)
		ratio = 4096;
	    data1 = (data1 * ratio + data1p * (4096 - ratio)) >> 12;

	    cdp = audio_channel + 2;
	    ratio1 = cdp->per - cdp->evtime;
	    ratio = (ratio1 << 12) / INTERVAL;
	    if (cdp->evtime < scaled_sample_evtime || ratio1 >= INTERVAL)
		ratio = 4096;
	    data2 = (data2 * ratio + data2p * (4096 - ratio)) >> 12;

	    cdp = audio_channel + 3;
	    ratio1 = cdp->per - cdp->evtime;
	    ratio = (ratio1 << 12) / INTERVAL;
	    if (cdp->evtime < scaled_sample_evtime || ratio1 >= INTERVAL)
		ratio = 4096;
	    data3 = (data3 * ratio + data3p * (4096 - ratio)) >> 12;
	}
	data1 += data2;
	data0 += data3;
	data0 += data1;
	{
	    uae_u32 data = SBASEVAL16(2) + data0;
	    FINISH_DATA (data, 16, 2);
	    PUT_SOUND_WORD (data);
	}
    }
    check_sound_buffers ();
}

void sample8_handler (void)
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
	FINISH_DATA (data, 8, 2);
	PUT_SOUND_BYTE (data);
    }

    check_sound_buffers ();
}

#ifdef HAVE_STEREO_SUPPORT
void sample16s_handler (void)
{
    if (ahi_enabled) {
	PUT_SOUND_WORD_RIGHT (audio_channel[4].dat);
	PUT_SOUND_WORD_LEFT (audio_channel[5].dat);
    } else {
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
	    FINISH_DATA (data, 16, 1);
	    put_sound_word_right (data);
	}

	data1 += data2;
	{
	    uae_u32 data = SBASEVAL16(1) + data1;	
	    FINISH_DATA (data, 16, 1);
	    put_sound_word_left (data);
	}
    }

    check_sound_buffers ();
}

void sample16si_crux_handler (void)
{
    if (ahi_enabled) {
	PUT_SOUND_WORD_RIGHT (audio_channel[4].dat);
	PUT_SOUND_WORD_LEFT (audio_channel[5].dat);
    } else {
	unsigned long delta, ratio;

	uae_u32 data0 = audio_channel[0].current_sample;
	uae_u32 data1 = audio_channel[1].current_sample;
	uae_u32 data2 = audio_channel[2].current_sample;
	uae_u32 data3 = audio_channel[3].current_sample;
	uae_u32 data0p = audio_channel[0].last_sample;
	uae_u32 data1p = audio_channel[1].last_sample;
	uae_u32 data2p = audio_channel[2].last_sample;
	uae_u32 data3p = audio_channel[3].last_sample;

	DO_CHANNEL_1 (data0, 0);
	DO_CHANNEL_1 (data1, 1);
	DO_CHANNEL_1 (data2, 2);
	DO_CHANNEL_1 (data3, 3);
	DO_CHANNEL_1 (data0p, 0);
	DO_CHANNEL_1 (data1p, 1);
	DO_CHANNEL_1 (data2p, 2);
	DO_CHANNEL_1 (data3p, 3);

	data0 &= audio_channel[0].adk_mask;
	data0p &= audio_channel[0].adk_mask;
	data1 &= audio_channel[1].adk_mask;
	data1p &= audio_channel[1].adk_mask;
	data2 &= audio_channel[2].adk_mask;
	data2p &= audio_channel[2].adk_mask;
	data3 &= audio_channel[3].adk_mask;
	data3p &= audio_channel[3].adk_mask;

	{    
	    struct audio_channel_data *cdp;
	    unsigned long ratio, ratio1;
#define INTERVAL (scaled_sample_evtime * 3)
	    cdp = audio_channel + 0;
	    ratio1 = cdp->per - cdp->evtime;
	    ratio = (ratio1 << 12) / INTERVAL;
	    if (cdp->evtime < scaled_sample_evtime || ratio1 >= INTERVAL)
		ratio = 4096;
	    data0 = (data0 * ratio + data0p * (4096 - ratio)) >> 12;

	    cdp = audio_channel + 1;
	    ratio1 = cdp->per - cdp->evtime;
	    ratio = (ratio1 << 12) / INTERVAL;
	    if (cdp->evtime < scaled_sample_evtime || ratio1 >= INTERVAL)
		ratio = 4096;
	    data1 = (data1 * ratio + data1p * (4096 - ratio)) >> 12;

	    cdp = audio_channel + 2;
	    ratio1 = cdp->per - cdp->evtime;
	    ratio = (ratio1 << 12) / INTERVAL;
	    if (cdp->evtime < scaled_sample_evtime || ratio1 >= INTERVAL)
		ratio = 4096;
	    data2 = (data2 * ratio + data2p * (4096 - ratio)) >> 12;

	    cdp = audio_channel + 3;
	    ratio1 = cdp->per - cdp->evtime;
	    ratio = (ratio1 << 12) / INTERVAL;
	    if (cdp->evtime < scaled_sample_evtime || ratio1 >= INTERVAL)
		ratio = 4096;
	    data3 = (data3 * ratio + data3p * (4096 - ratio)) >> 12;
	}
	data1 += data2;
	data0 += data3;
	{
	    uae_u32 data = SBASEVAL16 (1) + data0;
	    FINISH_DATA (data, 16, 1);
	    put_sound_word_right (data);
	}

	{
	    uae_u32 data = SBASEVAL16 (1) + data1;
	    FINISH_DATA (data, 16, 1);
	    put_sound_word_left (data);
	}
    }    
    check_sound_buffers ();
}

void sample16si_rh_handler (void)
{
    if (ahi_enabled) {
	PUT_SOUND_WORD_RIGHT (audio_channel[4].dat);
	PUT_SOUND_WORD_LEFT (audio_channel[5].dat);
    } else {
	unsigned long delta, ratio;

	uae_u32 data0 = audio_channel[0].current_sample;
	uae_u32 data1 = audio_channel[1].current_sample;
	uae_u32 data2 = audio_channel[2].current_sample;
	uae_u32 data3 = audio_channel[3].current_sample;
	uae_u32 data0p = audio_channel[0].last_sample;
	uae_u32 data1p = audio_channel[1].last_sample;
	uae_u32 data2p = audio_channel[2].last_sample;
	uae_u32 data3p = audio_channel[3].last_sample;

	DO_CHANNEL_1 (data0, 0);
	DO_CHANNEL_1 (data1, 1);
	DO_CHANNEL_1 (data2, 2);
	DO_CHANNEL_1 (data3, 3);
	DO_CHANNEL_1 (data0p, 0);
	DO_CHANNEL_1 (data1p, 1);
	DO_CHANNEL_1 (data2p, 2);
	DO_CHANNEL_1 (data3p, 3);

	data0 &= audio_channel[0].adk_mask;
	data0p &= audio_channel[0].adk_mask;
	data1 &= audio_channel[1].adk_mask;
	data1p &= audio_channel[1].adk_mask;
	data2 &= audio_channel[2].adk_mask;
	data2p &= audio_channel[2].adk_mask;
	data3 &= audio_channel[3].adk_mask;
	data3p &= audio_channel[3].adk_mask;

	/* linear interpolation and summing up... */
	delta = audio_channel[0].per;
	ratio = ((audio_channel[0].evtime % delta) << 8) / delta;
	data0 = (data0 * (256 - ratio) + data0p * ratio) >> 8;
	delta = audio_channel[1].per;
	ratio = ((audio_channel[1].evtime % delta) << 8) / delta;
	data1 = (data1 * (256 - ratio) + data1p * ratio) >> 8;
	delta = audio_channel[2].per;
	ratio = ((audio_channel[2].evtime % delta) << 8) / delta;
	data1 += (data2 * (256 - ratio) + data2p * ratio) >> 8;
	delta = audio_channel[3].per;
	ratio = ((audio_channel[3].evtime % delta) << 8) / delta;
	data0 += (data3 * (256 - ratio) + data3p * ratio) >> 8;
	{
	    uae_u32 data = SBASEVAL16 (1) + data0;
	    FINISH_DATA (data, 16, 1);
	    put_sound_word_right (data);
	}

	{
	    uae_u32 data = SBASEVAL16 (1) + data1;
	    FINISH_DATA (data, 16, 1);
	    put_sound_word_left (data);
	}
    }    
    check_sound_buffers ();
}

void sample8s_handler (void)
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
	FINISH_DATA (data, 8, 1);
	PUT_SOUND_BYTE_RIGHT (data);
    }
    data1 += data2;
    {
	uae_u32 data = SBASEVAL8(1) + data1;
	FINISH_DATA (data, 8, 1);
	PUT_SOUND_BYTE_LEFT (data);
    }

    check_sound_buffers ();
}
#else
void sample8s_handler (void)
{
    sample8_handler();
}
void sample16s_handler (void)
{
    sample16_handler();
}
void sample16si_crux_handler (void)
{
    sample16i_crux_handler();
}
void sample16si_rh_handler (void)
{
    sample16i_rh_handler();
}
#endif

static uae_u8 int2ulaw (int ch)
{
    int mask;

    if (ch < 0) {
      ch = -ch;
      mask = 0x7f;
    }
    else {
      mask = 0xff;
    }

    if (ch < 32) {
	ch = 0xF0 | ( 15 - (ch/2) );
    } else if (ch < 96) {
	ch = 0xE0 | ( 15 - (ch-32)/4 );
    } else if (ch < 224) {
	ch = 0xD0 | ( 15 - (ch-96)/8 );
    } else if (ch < 480) {
	ch = 0xC0 | ( 15 - (ch-224)/16 );
    } else if (ch < 992 ) {
	ch = 0xB0 | ( 15 - (ch-480)/32 );
    } else if (ch < 2016) {
	ch = 0xA0 | ( 15 - (ch-992)/64 );
    } else if (ch < 4064) {
	ch = 0x90 | ( 15 - (ch-2016)/128 );
    } else if (ch < 8160) {
	ch = 0x80 | ( 15 - (ch-4064)/256 );
    } else {
	ch = 0x80;
    }
    return (uae_u8)(mask & ch);
}

void sample_ulaw_handler (void)
{
    int nr;
    uae_u32 data = 0;

    for (nr = 0; nr < 4; nr++) {
	if (!(adkcon & (0x11 << nr))) {
	    uae_u32 d = audio_channel[nr].current_sample;
	    DO_CHANNEL_1 (d, nr);
	    data += d;
	}
    }
    PUT_SOUND_BYTE (int2ulaw (data));
    check_sound_buffers ();
}

void schedule_audio (void)
{
    unsigned long best = ~0ul;
    int i;

    eventtab[ev_audio].active = 0;
    eventtab[ev_audio].oldcycles = get_cycles ();
    for (i = 0; i < 6; i++) {
	struct audio_channel_data *cdp = audio_channel + i;

	if (cdp->state != 0) {
	    if (best > cdp->evtime) {
		best = cdp->evtime;
		eventtab[ev_audio].active = 1;
	    }
	}	
    }
    eventtab[ev_audio].evtime = get_cycles () + best;
}

static void audio_handler (int nr)
{
    struct audio_channel_data *cdp = audio_channel + nr;

    switch (cdp->state) {
     case 0:
	write_log ("Bug in sound code\n");
	break;

     case 1:
	/* We come here at the first hsync after DMA was turned on. */
	cdp->evtime = maxhpos * CYCLE_UNIT;

	cdp->state = 5;
	INTREQ(0x8000 | (0x80 << nr));
	if (cdp->wlen != 1)
	    cdp->wlen = (cdp->wlen - 1) & 0xFFFF;
	cdp->nextdat = chipmem_wget (cdp->pt);

	cdp->pt += 2;
	break;

     case 5:
	/* We come here at the second hsync after DMA was turned on. */
	if (currprefs.produce_sound == 0)
	    cdp->per = PERIOD_MAX;

	cdp->evtime = cdp->per;
	cdp->dat = cdp->nextdat;
	cdp->last_sample = cdp->current_sample;
	cdp->current_sample = (sample8_t)(cdp->dat >> 8);

	cdp->state = 2;
	{
	    int audav = adkcon & (1 << nr);
	    int audap = adkcon & (16 << nr);
	    int napnav = (!audav && !audap) || audav;
	    if (napnav)
		cdp->data_written = 2;
	}
	break;

     case 2:
	/* We come here when a 2->3 transition occurs */
	if (currprefs.produce_sound == 0)
	    cdp->per = PERIOD_MAX;

	cdp->last_sample = cdp->current_sample;
	cdp->current_sample = (sample8_t)(cdp->dat & 0xFF);
	cdp->evtime = cdp->per;

	cdp->state = 3;

	/* Period attachment? */
	if (adkcon & (0x10 << nr)) {
	    if (cdp->intreq2 && cdp->dmaen)
		INTREQ (0x8000 | (0x80 << nr));
	    cdp->intreq2 = 0;

	    cdp->dat = cdp->nextdat;
	    if (cdp->dmaen)
		cdp->data_written = 2;
	    if (nr < 3) {
		if (cdp->dat == 0)
		    (cdp+1)->per = PERIOD_MAX;
		else if (cdp->dat < maxhpos * CYCLE_UNIT / 2 && currprefs.produce_sound < 3)
		    (cdp+1)->per = maxhpos * CYCLE_UNIT / 2;
		else
		    (cdp+1)->per = cdp->dat * CYCLE_UNIT;
	    }
	}
	break;

     case 3:
	/* We come here when a 3->2 transition occurs */
	if (currprefs.produce_sound == 0)
	    cdp->per = PERIOD_MAX;

	cdp->evtime = cdp->per;

	if ((INTREQR() & (0x80 << nr)) && !cdp->dmaen) {
	    cdp->state = 0;
	    cdp->last_sample = 0;
	    cdp->current_sample = 0;
	    break;
	} else {
	    int audav = adkcon & (1 << nr);
	    int audap = adkcon & (16 << nr);
	    int napnav = (!audav && !audap) || audav;
	    cdp->state = 2;

	    if ((cdp->intreq2 && cdp->dmaen && napnav)
		|| (napnav && !cdp->dmaen))
		INTREQ(0x8000 | (0x80 << nr));
	    cdp->intreq2 = 0;

	    cdp->dat = cdp->nextdat;
	    cdp->last_sample = cdp->current_sample;
	    cdp->current_sample = (sample8_t)(cdp->dat >> 8);

	    if (cdp->dmaen && napnav)
		cdp->data_written = 2;

	    /* Volume attachment? */
	    if (audav) {
		if (nr < 3) {
		    (cdp+1)->vol = cdp->dat;
#ifndef MULTIPLICATION_PROFITABLE
		    (cdp+1)->voltbl = sound_table[cdp->dat];
#endif
		}
	    }
	}
	break;

     default:
	cdp->state = 0;
	break;
    }
}

static void ahi_handler (int nr)
{
    struct audio_channel_data *cdp = audio_channel + nr;

    switch (cdp->state) {
    case 0:
	write_log ("Bug in sound code\n");
	break;

    case 1:
	cdp->evtime = cdp->per;
	cdp->intreq2 = 0;

	cdp->state = 2;
	if (cdp->wlen != 1)
	    cdp->wlen = (cdp->wlen - 1) & 0xFFFF;
	cdp->dat = chipmem_wget (cdp->pt);

	cdp->pt += 2;
	break;

    case 2:
	if (currprefs.produce_sound == 0)
	    cdp->per = PERIOD_MAX;

	/* Only do interrupts for one of the AHI channels.  */
	if (nr == 4 && cdp->intreq2)
	    /* Reuse AUD0 interrupt for AHI purposes.  This is easier than
	       fixing the driver source to use AddIntServer instead of SetIntVector
	       for the EXTER interrupt.  */
	    INTREQ (0x8080);
	cdp->intreq2 = 0;
	cdp->evtime = cdp->per;

	cdp->dat = chipmem_wget (cdp->pt);
	cdp->pt += 2;
	if (cdp->wlen == 1) {
	    cdp->pt = cdp->lc;
	    cdp->wlen = cdp->len;
	    cdp->intreq2 = 1;
	} else
	    cdp->wlen = (cdp->wlen - 1) & 0xFFFF;
	break;

    default:
	cdp->state = 0;
	break;
    }
}

void aud0_handler (void)
{
    audio_handler (0);
}
void aud1_handler (void)
{
    audio_handler (1);
}
void aud2_handler (void)
{
    audio_handler (2);
}
void aud3_handler (void)
{
    audio_handler (3);
}

void audio_channel_enable_dma (struct audio_channel_data *cdp)
{
    if (cdp->state == 0) {
	cdp->state = 1;
	cdp->pt = cdp->lc;
	cdp->wper = cdp->per;
	cdp->wlen = cdp->len;
	cdp->data_written = 2;
	cdp->evtime = eventtab[ev_hsync].evtime - get_cycles ();
    }
}

void audio_channel_disable_dma (struct audio_channel_data *cdp)
{
    if (cdp->state == 1 || cdp->state == 5) {
	cdp->state = 0;
	cdp->last_sample = 0;
	cdp->current_sample = 0;
    }
}

void audio_reset (void)
{
    int i;
    if (savestate_state != STATE_RESTORE) {
	memset (audio_channel, 0, 4 * sizeof *audio_channel);
	audio_channel[0].per = PERIOD_MAX;
	audio_channel[1].per = PERIOD_MAX;
	audio_channel[2].per = PERIOD_MAX;
	audio_channel[3].per = PERIOD_MAX;
	audio_channel[0].voltbl = sound_table[0];
	audio_channel[1].voltbl = sound_table[0];
	audio_channel[2].voltbl = sound_table[0];
	audio_channel[3].voltbl = sound_table[0];
    } else
	for (i = 0; i < 4; i++)
	    audio_channel[i].dmaen = (dmacon & 0x200) && (dmacon & (1 << i));


    memset (audio_channel + 4, 0, 2 * sizeof *audio_channel);
    audio_channel[4].per = PERIOD_MAX;
    audio_channel[5].per = PERIOD_MAX;

#ifndef MULTIPLICATION_PROFITABLE
    for (i = 0; i < 6; i++)
	audio_channel[nr].voltbl = sound_table[audio_channel[nr].vol];
#endif

    last_cycles = 0;
    ahi_enabled = 0;
    ahi_interrupt_state = 0;
    next_sample_evtime = scaled_sample_evtime;

    schedule_audio ();
}

STATIC_INLINE int sound_prefs_changed (void)
{
    return (changed_prefs.produce_sound != currprefs.produce_sound
	    || changed_prefs.stereo != currprefs.stereo
	    || changed_prefs.mixed_stereo != currprefs.mixed_stereo
	    || changed_prefs.sound_maxbsiz != currprefs.sound_maxbsiz
	    || changed_prefs.sound_freq != currprefs.sound_freq
	    || changed_prefs.sound_bits != currprefs.sound_bits);
}

void check_prefs_changed_audio (void)
{
    if (sound_available && sound_prefs_changed ()) {
	close_sound ();

	currprefs.produce_sound = changed_prefs.produce_sound;
	currprefs.stereo = changed_prefs.stereo;
	currprefs.mixed_stereo = changed_prefs.mixed_stereo;
	currprefs.sound_bits = changed_prefs.sound_bits;
	currprefs.sound_freq = changed_prefs.sound_freq;
	currprefs.sound_maxbsiz = changed_prefs.sound_maxbsiz;
	if (currprefs.produce_sound >= 2) {
	    if (init_audio ()) {
		last_cycles = get_cycles () - 1;
		next_sample_evtime = scaled_sample_evtime;
	    } else
		if (! sound_available) {
		    write_log ("Sound is not supported.\n");
		} else {
		    write_log ("Sorry, can't initialize sound.\n");
		    currprefs.produce_sound = 0;
		    /* So we don't do this every frame */
		    changed_prefs.produce_sound = 0;
		}
	}
	compute_vsynctime ();
    }
    /* Select the right interpolation method.  */
    if (sample_handler == sample16_handler
	|| sample_handler == sample16i_crux_handler
	|| sample_handler == sample16i_rh_handler)
	sample_handler = (currprefs.sound_interpol == 0 ? sample16_handler
			  : currprefs.sound_interpol == 1 ? sample16i_rh_handler
			  : sample16i_crux_handler);
    else if (sample_handler == sample16s_handler
	     || sample_handler == sample16si_crux_handler
	     || sample_handler == sample16si_rh_handler)
	sample_handler = (currprefs.sound_interpol == 0 ? sample16s_handler
			  : currprefs.sound_interpol == 1 ? sample16si_rh_handler
			  : sample16si_crux_handler);
    if (currprefs.produce_sound == 0) {
	eventtab[ev_audio].active = 0;
	events_schedule ();
    }
}

void update_audio (void)
{
    unsigned long int n_cycles;

    if (currprefs.produce_sound == 0 || savestate_state == STATE_RESTORE)
	return;

    n_cycles = get_cycles () - last_cycles;
    for (;;) {
	unsigned long int best_evtime = n_cycles + 1;
	if (audio_channel[0].state != 0 && best_evtime > audio_channel[0].evtime)
	    best_evtime = audio_channel[0].evtime;
	if (audio_channel[1].state != 0 && best_evtime > audio_channel[1].evtime)
	    best_evtime = audio_channel[1].evtime;
	if (audio_channel[2].state != 0 && best_evtime > audio_channel[2].evtime)
	    best_evtime = audio_channel[2].evtime;
	if (audio_channel[3].state != 0 && best_evtime > audio_channel[3].evtime)
	    best_evtime = audio_channel[3].evtime;
	if (audio_channel[4].state != 0 && best_evtime > audio_channel[4].evtime)
	    best_evtime = audio_channel[4].evtime;
	if (audio_channel[5].state != 0 && best_evtime > audio_channel[5].evtime)
	    best_evtime = audio_channel[5].evtime;
	if (currprefs.produce_sound > 1 && best_evtime > next_sample_evtime)
	    best_evtime = next_sample_evtime;

	if (best_evtime > n_cycles)
	    break;

	next_sample_evtime -= best_evtime;
	audio_channel[0].evtime -= best_evtime;
	audio_channel[1].evtime -= best_evtime;
	audio_channel[2].evtime -= best_evtime;
	audio_channel[3].evtime -= best_evtime;
	audio_channel[4].evtime -= best_evtime;
	audio_channel[5].evtime -= best_evtime;
	n_cycles -= best_evtime;
	if (next_sample_evtime == 0 && currprefs.produce_sound > 1) {
	    next_sample_evtime = scaled_sample_evtime;
	    (*sample_handler) ();
	}
	if (audio_channel[0].evtime == 0 && audio_channel[0].state != 0)
	    audio_handler (0);
	if (audio_channel[1].evtime == 0 && audio_channel[1].state != 0)
	    audio_handler (1);
	if (audio_channel[2].evtime == 0 && audio_channel[2].state != 0)
	    audio_handler (2);
	if (audio_channel[3].evtime == 0 && audio_channel[3].state != 0)
	    audio_handler (3);
	if (audio_channel[4].evtime == 0 && audio_channel[4].state != 0)
	    ahi_handler (4);
	if (audio_channel[5].evtime == 0 && audio_channel[5].state != 0)
	    ahi_handler (5);
    }
    last_cycles = get_cycles () - n_cycles;
}

void audio_evhandler (void)
{
    if (currprefs.produce_sound == 0)
	abort ();

    update_audio ();
    schedule_audio ();
}

void AUDxDAT (int nr, uae_u16 v)
{
    struct audio_channel_data *cdp = audio_channel + nr;

    if (currprefs.produce_sound == 0)
	return;

    update_audio ();

    cdp->dat = v;
    if (cdp->state == 0 && !(INTREQR() & (0x80 << nr))) {
	cdp->state = 2;
	INTREQ(0x8000 | (0x80 << nr));
	/* data_written = 2 ???? */
	cdp->evtime = cdp->per;
	schedule_audio ();
	events_schedule ();
    }
}

void AUDxLCH (int nr, uae_u16 v)
{
    update_audio ();

    audio_channel[nr].lc = (audio_channel[nr].lc & 0xffff) | ((uae_u32)v << 16);
}

void AUDxLCL (int nr, uae_u16 v)
{
    update_audio ();

    audio_channel[nr].lc = (audio_channel[nr].lc & ~0xffff) | (v & 0xFFFE);
}

void AUDxPER (int nr, uae_u16 v)
{
    unsigned long per = v * CYCLE_UNIT;
    update_audio ();

    if (per == 0)
	per = PERIOD_MAX;

    if (per < maxhpos * CYCLE_UNIT / 2 && currprefs.produce_sound < 3)
	per = maxhpos * CYCLE_UNIT / 2;

    if (audio_channel[nr].per == PERIOD_MAX
	&& per != PERIOD_MAX)
    {
	audio_channel[nr].evtime = CYCLE_UNIT;
	if (currprefs.produce_sound > 0) {
	    schedule_audio ();
	    events_schedule ();
	}
    }
    audio_channel[nr].per = per;
}

void AUDxLEN (int nr, uae_u16 v)
{
    update_audio ();
    audio_channel[nr].len = v;
}

void AUDxVOL (int nr, uae_u16 v)
{
    int v2 = v & 64 ? 63 : v & 63;

    update_audio ();

    audio_channel[nr].vol = v2;
#ifndef MULTIPLICATION_PROFITABLE
    audio_channel[nr].voltbl = sound_table[v2];
#endif
}

int init_audio (void)
{
    int retval;
    /* Some backward compatibility hacks until every port initializes
       scaled_sample_evtime...  */
    scaled_sample_evtime_ok = 0;
    retval = init_sound ();
    if (! scaled_sample_evtime_ok)
	scaled_sample_evtime = sample_evtime * CYCLE_UNIT;
    return retval;
}

static uae_u32 ahi_entry (void)
{
    struct audio_channel_data *cdp;
    int opcode = m68k_dreg (regs, 0);
    int was_enabled;

    /* D0 contains the function code:
       0: get current output sample rate.
       1: get scaled divider for that frequency
       2: enable/disable AHI output (boolean value in D1)
       3: set right AHI channel info (LC in A0, period in D1, length D2, dmaen D3).
       4: set left AHI channel info (LC in A0, period in D1, length D2, dmaen D3).
       5: return interrupt state.  */
    switch (opcode) {
    case 0:
	return currprefs.sound_freq;
    case 1:
	return scaled_sample_evtime;
    case 2:
	was_enabled = ahi_enabled;
	ahi_enabled = !!m68k_dreg (regs, 1);
	if (ahi_enabled > was_enabled) {
	    audio_channel[4].evtime = CYCLE_UNIT;
	    audio_channel[5].evtime = CYCLE_UNIT;
	    if (currprefs.produce_sound > 0) {
		schedule_audio ();
		events_schedule ();
	    }
	}
	return 0;
    case 3:
    case 4:
	/* Must set a frequency lower than sample rate, i.e. using a higher
	   divider.  */
	if (m68k_dreg (regs, 3) && m68k_dreg (regs, 1) < scaled_sample_evtime)
	    return 0;
	cdp = audio_channel + opcode + 1;
	was_enabled = cdp->dmaen;
	cdp->lc = m68k_areg (regs, 0);
	cdp->per = m68k_dreg (regs, 1);
	cdp->len = m68k_dreg (regs, 2);
	cdp->dmaen = m68k_dreg (regs, 3);
	if (currprefs.produce_sound > 0) {
	    if (cdp->dmaen && ! was_enabled) {
		cdp->state = 0;
		audio_channel_enable_dma (cdp);
	    } else if (cdp->dmaen == 0) {
		cdp->state = 0;
		cdp->dat = 0;
	    }
	    schedule_audio ();
	    events_schedule ();
	}
	return 1;
    case 5:
	return ahi_interrupt_state;
    default:
	return 0;
    }
}

void ahi_install (void)
{
    uaecptr a = here ();
    org (RTAREA_BASE + 0xFFB0);
    calltrap (deftrap (ahi_entry));
    dw (RTS);
    org (a);
}

/* audio save/restore code FIXME: not working correctly */
/* help needed */

uae_u8 *restore_audio (uae_u8 *src, int i)
{
    struct audio_channel_data *acd;
    uae_u16 p;

    acd = audio_channel + i;
    acd->state = restore_u8 ();
    acd->vol = restore_u8 ();
    acd->intreq2 = restore_u8 ();
    acd->data_written = restore_u8 ();
    acd->len = restore_u16 ();
    acd->wlen = restore_u16 ();
    p = restore_u16 ();
    acd->per = p ? p * CYCLE_UNIT : PERIOD_MAX;
    p = restore_u16 ();
    acd->wper = p ? p * CYCLE_UNIT : PERIOD_MAX;
    acd->lc = restore_u32 ();
    acd->pt = restore_u32 ();
    acd->evtime = restore_u32 ();

    return src;
}


uae_u8 *save_audio (int *len, int i)
{
    struct audio_channel_data *acd;
    uae_u8 *dst = malloc (100);
    uae_u8 *dstbak = dst;
    uae_u16 p;

    acd = audio_channel + i;
    save_u8 ((uae_u8)acd->state);
    save_u8 (acd->vol);
    save_u8 (acd->intreq2);
    save_u8 (acd->data_written);
    save_u16 (acd->len);
    save_u16 (acd->wlen);
    p = acd->per == PERIOD_MAX ? 0 : acd->per / CYCLE_UNIT;
    save_u16 (p);
    p = acd->per == PERIOD_MAX ? 0 : acd->wper / CYCLE_UNIT;
    save_u16 (p);
    save_u32 (acd->lc);
    save_u32 (acd->pt);
    save_u32 (acd->evtime);
    *len = dst - dstbak;
    return dstbak;
}

 /*
  * UAE - The Un*x Amiga Emulator
  *
  * Paula audio emulation
  *
  * Copyright 1995, 1996, 1997 Bernd Schmidt
  * Copyright 1996 Marcus Sundberg
  * Copyright 1996 Manfred Thole
  * Copyright 2006 Toni Wilen
  *
  * new filter algorithm and anti&sinc interpolators by Antti S. Lankila
  */

#include "sysconfig.h"
#include "sysdeps.h"

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
#include "sinctable.h"
#include "gui.h"

#define MAX_EV ~0ul

/* periods less than this value are replaced by this value. */
#define MIN_ALLOWED_PERIOD 16
/* reserve ~20 extra slots in sinc queue for cpu volume or some such updates
 * even at maximum period. This avoids sinc queue overflow on games like
 * battle squadron that write these low period values and do cpu-based
 * updates on paula registers, probably volume. */
#define NUMBER_OF_CPU_UPDATES_ALLOWED 20

#define SINC_QUEUE_LENGTH (SINC_QUEUE_MAX_AGE / MIN_ALLOWED_PERIOD + NUMBER_OF_CPU_UPDATES_ALLOWED)

typedef struct {
    int age;
    int output;
} sinc_queue_t;

struct audio_channel_data {
    unsigned long adk_mask;
    unsigned long evtime;
    unsigned long per;
    uae_u8 dmaen, intreq2, data_written;
    uaecptr lc, pt;
    int state, wper;
    unsigned int wlen;
    int current_sample, last_sample;
    int vol;
    int *voltbl;
    uae_u16 dat, nextdat, len;
    int sample_accum, sample_accum_time;
    int sinc_output_state;
    sinc_queue_t sinc_queue[SINC_QUEUE_LENGTH];
    int sinc_queue_length;
};

static struct audio_channel_data audio_channel[4];
int sound_available = 0;
int sound_table[64][256];
void (*sample_handler) (void);
static void (*sample_prehandler) (unsigned long best_evtime);

static unsigned long scaled_sample_evtime;
static unsigned long last_cycles, next_sample_evtime;

unsigned int obtainedfreq;

void init_sound_table16 (void)
{
    int i,j;

    for (i = 0; i < 256; i++)
	for (j = 0; j < 64; j++)
	    sound_table[j][i] = j * (uae_s8)i * (currprefs.sound_stereo ? 2 : 1);
}

#ifdef MULTIPLICATION_PROFITABLE
typedef uae_s8 sample8_t;
#define DO_CHANNEL_1(v, c) do { (v) *= audio_channel[c].vol; } while (0)
#define SBASEVAL16(logn) ((logn) == 1 ? SOUND16_BASE_VAL >> 1 : SOUND16_BASE_VAL)
#define FINISH_DATA(data,b,logn) do { if (14 - (b) + (logn) > 0) (data) >>= 14 - (b) + (logn); else (data) <<= (b) - 14 - (logn); } while (0);
#else
typedef uae_u8 sample8_t;
#define DO_CHANNEL_1(v, c) do { (v) = audio_channel[c].voltbl[(v)]; } while (0)
#define SBASEVAL16(logn) SOUND16_BASE_VAL
#define FINISH_DATA(data,b,logn)
#endif

static uae_u32 right_word_saved[SOUND_MAX_DELAY_BUFFER];
static uae_u32 left_word_saved[SOUND_MAX_DELAY_BUFFER];
static int saved_ptr;

static int mixed_on, mixed_stereo_size, mixed_mul1, mixed_mul2;

/* Always put the right word before the left word.  */

STATIC_INLINE void put_sound_word_right (uae_u32 w)
{
    if (mixed_on) {
	right_word_saved[saved_ptr] = w;
	return;
    }

    PUT_SOUND_WORD_RIGHT (w);
}

STATIC_INLINE void put_sound_word_left (uae_u32 w)
{
    if (mixed_on) {
	uae_u32 rold, lold, rnew, lnew, tmp;

	left_word_saved[saved_ptr] = w;
	lnew = w - SOUND16_BASE_VAL;
	rnew = right_word_saved[saved_ptr] - SOUND16_BASE_VAL;

	saved_ptr = (saved_ptr + 1) & mixed_stereo_size;

	lold = left_word_saved[saved_ptr] - SOUND16_BASE_VAL;
	tmp = (rnew * mixed_mul2 + lold * mixed_mul1) / MIXED_STEREO_SCALE;
	tmp += SOUND16_BASE_VAL;
	PUT_SOUND_WORD_RIGHT (tmp);

	rold = right_word_saved[saved_ptr] - SOUND16_BASE_VAL;
	w = (lnew * mixed_mul2 + rold * mixed_mul1) / MIXED_STEREO_SCALE;
    }
    PUT_SOUND_WORD_LEFT (w);
}

#define DO_CHANNEL(v, c) do { (v) &= audio_channel[c].adk_mask; data += v; } while (0);

static void anti_prehandler (unsigned long best_evtime)
{
    int i, output;
    struct audio_channel_data *acd;

    /* Handle accumulator antialiasiation */
    for (i = 0; i < 4; i++) {
	acd = &audio_channel[i];
	output = (acd->current_sample * acd->vol) & acd->adk_mask;
	acd->sample_accum += output * best_evtime;
	acd->sample_accum_time += best_evtime;
    }
}

STATIC_INLINE void samplexx_anti_handler (int *datasp)
{
    int i;
    for (i = 0; i < 4; i++) {
	datasp[i] = audio_channel[i].sample_accum_time ? (audio_channel[i].sample_accum / audio_channel[i].sample_accum_time) : 0;
	audio_channel[i].sample_accum = 0;
	audio_channel[i].sample_accum_time = 0;

    }
}

static void sinc_prehandler (unsigned long best_evtime)
{
    int i, j, output;
    struct audio_channel_data *acd;

    for (i = 0; i < 4; i++) {
	acd = &audio_channel[i];
	output = (acd->current_sample * acd->vol) & acd->adk_mask;

	/* age the sinc queue and truncate it when necessary */
	for (j = 0; j < acd->sinc_queue_length; j += 1) {
	    acd->sinc_queue[j].age += best_evtime;
	    if (acd->sinc_queue[j].age >= SINC_QUEUE_MAX_AGE) {
		acd->sinc_queue_length = j;
		break;
	    }
	}
	/* if output state changes, record the state change and also
	 * write data into sinc queue for mixing in the BLEP */
	if (acd->sinc_output_state != output) {
	    if (acd->sinc_queue_length > SINC_QUEUE_LENGTH - 1) {
		write_log ("warning: sinc queue truncated. Last age: %d.\n",
			   acd->sinc_queue[SINC_QUEUE_LENGTH-1].age);
		acd->sinc_queue_length = SINC_QUEUE_LENGTH - 1;
	    }
	    /* make room for new and add the new value */
	    memmove (&acd->sinc_queue[1], &acd->sinc_queue[0],
		     sizeof(acd->sinc_queue[0]) * acd->sinc_queue_length);
	    acd->sinc_queue_length += 1;
	    acd->sinc_queue[0].age = best_evtime;
	    acd->sinc_queue[0].output = output - acd->sinc_output_state;
	    acd->sinc_output_state = output;
	}
    }
}


/* this interpolator performs BLEP mixing (bleps are shaped like integrated sinc
 * functions) with a type of BLEP that matches the filtering configuration. */
STATIC_INLINE void samplexx_sinc_handler (int *datasp)
{
    int i, n;
    int const *winsinc;

#if 1
    /* Amiga 500 filter model is default for now. Put n=2 for A1200. */
    n = 0;
    if (gui_ledstate & 1) /* power led */
	n += 1;
#else
    if (sound_use_filter_sinc) {
	n = (sound_use_filter_sinc == FILTER_MODEL_A500) ? 0 : 2;
	if (led_filter_on)
	    n += 1;
    } else {
	n = 4;
    }
#endif
    winsinc = winsinc_integral[n];

    for (i = 0; i < 4; i += 1) {
	int j, v;
	struct audio_channel_data *acd = &audio_channel[i];
	/* The sum rings with harmonic components up to infinity... */
	int sum = acd->sinc_output_state << 17;
	/* ...but we cancel them through mixing in BLEPs instead */
	for (j = 0; j < acd->sinc_queue_length; j += 1)
	    sum -= winsinc[acd->sinc_queue[j].age] * acd->sinc_queue[j].output;
	v = sum >> 17;
	if (v > 32767)
	    v = 32767;
	else if (v < -32768)
	    v = -32768;
	datasp[i] = v;
    }
}

static void sample16i_sinc_handler (void)
{
    int datas[4], data1;

    samplexx_sinc_handler (datas);
    data1 = datas[0] + datas[3] + datas[1] + datas[2];
    FINISH_DATA (data1, 16, 2);
    PUT_SOUND_WORD (data1);
    check_sound_buffers ();
}

void sample16_handler (void)
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
	FINISH_DATA (data, 16, 2);
	PUT_SOUND_WORD (data);
    }
    check_sound_buffers ();
}

/* This interpolator examines sample points when Paula switches the output
 * voltage and computes the average of Paula's output */
static void sample16i_anti_handler (void)
{
    int datas[4], data1;

    samplexx_anti_handler (datas);
    data1 = datas[0] + datas[3] + datas[1] + datas[2];
    FINISH_DATA (data1, 16, 2);
    PUT_SOUND_WORD (data1);
    check_sound_buffers ();
}

static void sample16i_rh_handler (void)
{
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

static void sample16i_crux_handler (void)
{
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
    check_sound_buffers ();
}

#ifdef HAVE_STEREO_SUPPORT
static void sample16si_anti_handler (void)
{
    int datas[4], data1, data2;

    samplexx_anti_handler (datas);
    data1 = datas[0] + datas[3];
    data2 = datas[1] + datas[2];
    FINISH_DATA (data1, 16, 1);
    put_sound_word_right (data1);
    FINISH_DATA (data2, 16, 1);
    put_sound_word_left (data2);
    check_sound_buffers ();
}

static void sample16si_sinc_handler (void)
{
    int datas[4], data1, data2;

    samplexx_sinc_handler (datas);
    data1 = datas[0] + datas[3];
    data2 = datas[1] + datas[2];
    FINISH_DATA (data1, 16, 1);
    put_sound_word_right (data1);
    FINISH_DATA (data2, 16, 1);
    put_sound_word_left (data2);
    check_sound_buffers ();
}

void sample16s_handler (void)
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
	FINISH_DATA (data, 16, 1);
	put_sound_word_right (data);
    }

    data1 += data2;
    {
	uae_u32 data = SBASEVAL16(1) + data1;
	FINISH_DATA (data, 16, 1);
	put_sound_word_left (data);
    }

    check_sound_buffers ();
}

static void sample16si_crux_handler (void)
{
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
    check_sound_buffers ();
}

static void sample16si_rh_handler (void)
{
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
    check_sound_buffers ();
}

#else
void sample16s_handler (void)
{
    sample16_handler ();
}
static void sample16si_crux_handler (void)
{
    sample16i_crux_handler ();
}
static void sample16si_rh_handler (void)
{
    sample16i_rh_handler ();
}
#endif

void switch_audio_interpol (void)
{
    if (currprefs.sound_interpol == 0) {
	changed_prefs.sound_interpol = 1;
	write_log ("Interpol on: rh\n");
    } else if (currprefs.sound_interpol == 1) {
	changed_prefs.sound_interpol = 2;
	write_log ("Interpol on: crux\n");
    } else if (currprefs.sound_interpol == 2) {
	changed_prefs.sound_interpol = 3;
	write_log ("Interpol on: sinc\n");
    } else if (currprefs.sound_interpol == 3) {
	changed_prefs.sound_interpol = 4;
	write_log ("Interpol on: anti\n");
    } else {
	changed_prefs.sound_interpol = 0;
	write_log ("Interpol off\n");
    }
    return;
}
 
void schedule_audio (void)
{
    unsigned long best = MAX_EV;
    int i;

    eventtab[ev_audio].active = 0;
    eventtab[ev_audio].oldcycles = get_cycles ();
    for (i = 0; i < 4; i++) {
	struct audio_channel_data *cdp = audio_channel + i;

	if (cdp->evtime != MAX_EV) {
	    if (best > cdp->evtime) {
		best = cdp->evtime;
		eventtab[ev_audio].active = 1;
	    }
	}
    }
    eventtab[ev_audio].evtime = get_cycles () + best;
}

/*
 * TODO: This function has been moved here from the audio back-end layer
 * since it was common to all.
 * Needs further cleaning up and a better name - or replacing entirely.
 */
void update_sound (unsigned int freq)
{
    if (obtainedfreq) {
	if (0 /*is_vsync ()*/) {
	    if (currprefs.ntscmode)
		scaled_sample_evtime = (unsigned long)(MAXHPOS_NTSC * MAXVPOS_NTSC * freq * CYCLE_UNIT + obtainedfreq - 1) / obtainedfreq;
	    else
		scaled_sample_evtime = (unsigned long)(MAXHPOS_PAL * MAXVPOS_PAL * freq * CYCLE_UNIT + obtainedfreq - 1) / obtainedfreq;
	} else {
	    scaled_sample_evtime = (unsigned long)(312.0 * 50 * CYCLE_UNIT / (obtainedfreq  / 227.0));
	}
    }
}

static void audio_handler (unsigned int nr)
{
    struct audio_channel_data *cdp = audio_channel + nr;

    cdp->evtime = MAX_EV;
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
	    cdp->evtime = MAX_EV;
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

static void audio_channel_enable_dma (struct audio_channel_data *cdp)
{
    if (cdp->evtime == MAX_EV) {
	cdp->state = 1;
	cdp->pt = cdp->lc;
	cdp->wper = cdp->per;
	cdp->wlen = cdp->len;
	cdp->data_written = 2;
	cdp->evtime = eventtab[ev_hsync].evtime - get_cycles ();
    }
}

static void audio_channel_disable_dma (struct audio_channel_data *cdp)
{
    if (cdp->state == 1 || cdp->state == 5) {
	cdp->state = 0;
	cdp->evtime = MAX_EV;
	cdp->last_sample = 0;
	cdp->current_sample = 0;
    }
}

void audio_reset (void)
{
    int i;
    struct audio_channel_data *cdp;

    if (savestate_state != STATE_RESTORE) {
	for (i = 0; i < 4; i++) {
	    cdp = &audio_channel[i];
	    memset (cdp, 0, sizeof *audio_channel);
	    cdp->per = PERIOD_MAX;
	    cdp->vol = 0;
	    cdp->evtime = MAX_EV;
	}
    } else
	for (i = 0; i < 4; i++)
	    audio_channel[i].dmaen = (dmacon & 0x200) && (dmacon & (1 << i));

#ifndef MULTIPLICATION_PROFITABLE
    for (i = 0; i < 4; i++)
	audio_channel[i].voltbl = sound_table[audio_channel[i].vol];
#endif

    last_cycles = get_cycles ();
    next_sample_evtime = scaled_sample_evtime;

    schedule_audio ();
    events_schedule ();
}

STATIC_INLINE int sound_prefs_changed (void)
{
    return (changed_prefs.produce_sound != currprefs.produce_sound
	    || changed_prefs.sound_stereo != currprefs.sound_stereo
	    || changed_prefs.sound_maxbsiz != currprefs.sound_maxbsiz
	    || changed_prefs.sound_freq != currprefs.sound_freq);
}

void check_prefs_changed_audio (void)
{
    int old_mixed_on = mixed_on;
    int old_mixed_size = mixed_stereo_size;
    int sep, delay;

    /* Some options we can just apply without reinitializing the sound
       backend.  */
    currprefs.sound_interpol = changed_prefs.sound_interpol;
    sep = currprefs.sound_stereo_separation = changed_prefs.sound_stereo_separation;
    delay = currprefs.sound_mixed_stereo_delay = changed_prefs.sound_mixed_stereo_delay;
    mixed_mul1 = MIXED_STEREO_SCALE / 2 - sep;
    mixed_mul2 = MIXED_STEREO_SCALE / 2 + sep;
    mixed_stereo_size = delay > 0 ? (1 << (delay - 1)) - 1 : 0;
    mixed_on = (sep > 0 && sep < MIXED_STEREO_MAX) || mixed_stereo_size > 0;
    if (mixed_on && old_mixed_size != mixed_stereo_size) {
	saved_ptr = 0;
	memset (right_word_saved, 0, sizeof right_word_saved);
    }

    if (sound_available && sound_prefs_changed ()) {
	if (currprefs.produce_sound >= 2)
	    close_sound ();

	currprefs.produce_sound = changed_prefs.produce_sound;
	currprefs.sound_stereo = changed_prefs.sound_stereo;
	currprefs.sound_freq = changed_prefs.sound_freq;
	currprefs.sound_maxbsiz = changed_prefs.sound_maxbsiz;
	if (currprefs.produce_sound >= 2) {
	    if (!init_audio ()) {
		if (! sound_available) {
		    write_log ("Sound is not supported.\n");
		} else {
		    write_log ("Sorry, can't initialize sound.\n");
		    currprefs.produce_sound = 0;
		    /* So we don't do this every frame */
		    changed_prefs.produce_sound = 0;
		}
	    }
	    next_sample_evtime = scaled_sample_evtime;
	    last_cycles = get_cycles () - 1;
	    compute_vsynctime ();
	}
	if (currprefs.produce_sound == 0) {
	    eventtab[ev_audio].active = 0;
	    events_schedule ();
	}
    }
    /* Select the right interpolation method.  */
    if (sample_handler == sample16_handler
	|| sample_handler == sample16i_crux_handler
	|| sample_handler == sample16i_rh_handler
	|| sample_handler == sample16i_sinc_handler
	|| sample_handler == sample16i_anti_handler)
    {
	sample_handler = (currprefs.sound_interpol == 0 ? sample16_handler
			  : currprefs.sound_interpol == 1 ? sample16i_rh_handler
			  : currprefs.sound_interpol == 2 ? sample16i_crux_handler
			  : currprefs.sound_interpol == 3 ? sample16i_sinc_handler
			  : sample16i_anti_handler);
    } else if (sample_handler == sample16s_handler
	       || sample_handler == sample16si_crux_handler
	       || sample_handler == sample16si_rh_handler
	       || sample_handler == sample16si_sinc_handler
	       || sample_handler == sample16si_anti_handler)
	sample_handler = (currprefs.sound_interpol == 0 ? sample16s_handler
			  : currprefs.sound_interpol == 1 ? sample16si_rh_handler
			  : currprefs.sound_interpol == 2 ? sample16si_crux_handler
			  : currprefs.sound_interpol == 3 ? sample16si_sinc_handler
			  : sample16si_anti_handler);
    sample_prehandler = NULL;
    if (currprefs.sound_interpol == 3) {
	sample_prehandler = sinc_prehandler;
    } else if (currprefs.sound_interpol == 4) {
	sample_prehandler = anti_prehandler;
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
	if (audio_channel[0].evtime != MAX_EV && best_evtime > audio_channel[0].evtime)
	    best_evtime = audio_channel[0].evtime;
	if (audio_channel[1].evtime != MAX_EV && best_evtime > audio_channel[1].evtime)
	    best_evtime = audio_channel[1].evtime;
	if (audio_channel[2].evtime != MAX_EV && best_evtime > audio_channel[2].evtime)
	    best_evtime = audio_channel[2].evtime;
	if (audio_channel[3].evtime != MAX_EV && best_evtime > audio_channel[3].evtime)
	    best_evtime = audio_channel[3].evtime;
	if (currprefs.produce_sound > 1 && best_evtime > next_sample_evtime)
	    best_evtime = next_sample_evtime;

	if (best_evtime > n_cycles)
	    break;

	if (audio_channel[0].evtime != MAX_EV)
	    audio_channel[0].evtime -= best_evtime;
	if (audio_channel[1].evtime != MAX_EV)
	    audio_channel[1].evtime -= best_evtime;
	if (audio_channel[2].evtime != MAX_EV)
	    audio_channel[2].evtime -= best_evtime;
	if (audio_channel[3].evtime != MAX_EV)
	    audio_channel[3].evtime -= best_evtime;
	n_cycles -= best_evtime;
	if (currprefs.produce_sound > 1) {
	    next_sample_evtime -= best_evtime;
	    if (sample_prehandler)
		sample_prehandler (best_evtime / CYCLE_UNIT);
	    if (next_sample_evtime == 0) {
		next_sample_evtime = scaled_sample_evtime;
		(*sample_handler) ();
	    }
	}
	if (audio_channel[0].evtime == 0)
	    audio_handler (0);
	if (audio_channel[1].evtime == 0)
	    audio_handler (1);
	if (audio_channel[2].evtime == 0)
	    audio_handler (2);
	if (audio_channel[3].evtime == 0)
	    audio_handler (3);
    }
    last_cycles = get_cycles () - n_cycles;
}

void update_audio_dmacon (void)
{
    unsigned int i;
    update_audio ();

    for (i = 0; i < 4; i++) {
	struct audio_channel_data *cdp = audio_channel + i;
	int chan_ena = (dmacon & 0x200) && (dmacon & (1<<i));
	if (cdp->dmaen == chan_ena)
	    continue;
	cdp->dmaen = chan_ena;
	if (cdp->dmaen)
	    audio_channel_enable_dma (cdp);
	else
	    audio_channel_disable_dma (cdp);
    }
    schedule_audio ();
}

void audio_evhandler (void)
{
    if (currprefs.produce_sound == 0)
	abort ();

    update_audio ();
    schedule_audio ();
}

void audio_hsync (int dmaaction)
{
    int nr;

    update_audio ();

    /* Sound data is fetched at the beginning of each line */
    for (nr = 0; nr < 4; nr++) {
	struct audio_channel_data *cdp = audio_channel + nr;

	if (cdp->data_written == 2) {
	    cdp->data_written = 0;
	    cdp->nextdat = chipmem_wget (cdp->pt);
	    cdp->pt += 2;
	    if (cdp->state == 2 || cdp->state == 3) {
		if (cdp->wlen == 1) {
		    cdp->pt = cdp->lc;
		    cdp->wlen = cdp->len;
		    cdp->intreq2 = 1;
		} else
		    cdp->wlen = (cdp->wlen - 1) & 0xFFFF;
	    }
	}
    }
}

void AUDxDAT (unsigned int nr, uae_u16 v)
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

void AUDxLCH (unsigned int nr, uae_u16 v)
{
    update_audio ();

    audio_channel[nr].lc = (audio_channel[nr].lc & 0xffff) | ((uae_u32)v << 16);
}

void AUDxLCL (unsigned int nr, uae_u16 v)
{
    update_audio ();

    audio_channel[nr].lc = (audio_channel[nr].lc & ~0xffff) | (v & 0xFFFE);
}

void AUDxPER (unsigned int nr, uae_u16 v)
{
    unsigned long per = v * CYCLE_UNIT;
    update_audio ();

    if (per == 0)
	per = PERIOD_MAX;

    if (per < maxhpos * CYCLE_UNIT / 2 && currprefs.produce_sound < 3)
	per = maxhpos * CYCLE_UNIT / 2;
    /* the sinc code registers paula output state changes, but has a finite
     * buffer in which to do so. Hence, we forbid very low values; this should
     * only limit the accurate rendering of supersonic sounds, which are
     * filtered away on the sinc output path anyway. */
    if (currprefs.produce_sound == 3 && sample_handler == sample16si_sinc_handler && per < MIN_ALLOWED_PERIOD * CYCLE_UNIT)
	per = MIN_ALLOWED_PERIOD * CYCLE_UNIT;

    if (audio_channel[nr].per == PERIOD_MAX && per != PERIOD_MAX
	&& audio_channel[nr].evtime != MAX_EV) {
	audio_channel[nr].evtime = CYCLE_UNIT;
	if (currprefs.produce_sound > 0) {
	    schedule_audio ();
	    events_schedule ();
	}
    }

    audio_channel[nr].per = per;
}

void AUDxLEN (unsigned int nr, uae_u16 v)
{
    update_audio ();
    audio_channel[nr].len = v;
}

void AUDxVOL (unsigned int nr, uae_u16 v)
{
    int v2 = v & 64 ? 63 : v & 63;

    update_audio ();

    audio_channel[nr].vol = v2;
#ifndef MULTIPLICATION_PROFITABLE
    audio_channel[nr].voltbl = sound_table[v2];
#endif
}

void update_adkmasks (void)
{
    unsigned long t;

    t = adkcon | (adkcon >> 4);
    audio_channel[0].adk_mask = (((t >> 0) & 1) - 1);
    audio_channel[1].adk_mask = (((t >> 1) & 1) - 1);
    audio_channel[2].adk_mask = (((t >> 2) & 1) - 1);
    audio_channel[3].adk_mask = (((t >> 3) & 1) - 1);
}

int init_audio (void)
{
    int result = init_sound ();
    update_sound (vblank_hz);
    return result;
}

/* audio save/restore code FIXME: not working correctly */
/* help needed */

const uae_u8 *restore_audio (int i, const uae_u8 *src)
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

uae_u8 *save_audio (int i, int *len)
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

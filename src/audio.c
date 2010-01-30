 /*
  * UAE - The Un*x Amiga Emulator
  *
  * Paula audio emulation
  *
  * Copyright 1995, 1996, 1997 Bernd Schmidt
  * Copyright 1996 Marcus Sundberg
  * Copyright 1996 Manfred Thole
  * Copyright 2005 Heikki Orsila
  * Copyright 2006 Toni Wilen
  *
  * new filter algorithm and anti&sinc interpolators by Antti S. Lankila
  */

#include "sysconfig.h"
#include "sysdeps.h"

#include <math.h>

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
    int age, output;
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
	    sound_table[j][i] = j * (uae_s8)i * 2;
}

#ifdef MULTIPLICATION_PROFITABLE
typedef uae_s8 sample8_t;
#define DO_CHANNEL_1(v, c) do { (v) *= audio_channel[c].vol; } while (0)
#define SBASEVAL16(logn) ((logn) == 1 ? SOUND16_BASE_VAL >> 1 : SOUND16_BASE_VAL)
#define FINISH_DATA(data, b, logn) do { if (14 - (b) + (logn) > 0) (data) >>= 14 - (b) + (logn); else (data) <<= (b) - 14 - (logn); } while (0);
#else
typedef uae_u8 sample8_t;
#define DO_CHANNEL_1(v, c) do { (v) = audio_channel[c].voltbl[(v)]; } while (0)
#define SBASEVAL16(logn) SOUND16_BASE_VAL
#define FINISH_DATA(data, b, logn)
#endif

static uae_u32 right_word_saved[SOUND_MAX_DELAY_BUFFER];
static uae_u32 left_word_saved[SOUND_MAX_DELAY_BUFFER];
static int saved_ptr;

static int mixed_on, mixed_stereo_size, mixed_mul1, mixed_mul2;
static int led_filter_forced, sound_use_filter, sound_use_filter_sinc, led_filter_on;

/* denormals are very small floating point numbers that force FPUs into slow
   mode. All lowpass filters using floats are suspectible to denormals unless
   a small offset is added to avoid very small floating point numbers. */
#define DENORMAL_OFFSET (1E-10)

static struct filter_state {
    float rc1, rc2, rc3, rc4, rc5;
} sound_filter_state[4];

static float a500e_filter1_a0;
static float a500e_filter2_a0;
static float filter_a0; /* a500 and a1200 use the same */

enum {
  FILTER_NONE = 0,
  FILTER_MODEL_A500,
  FILTER_MODEL_A1200
};

/* Amiga has two separate filtering circuits per channel, a static RC filter
 * on A500 and the LED filter. This code emulates both.
 *
 * The Amiga filtering circuitry depends on Amiga model. Older Amigas seem
 * to have a 6 dB/oct RC filter with cutoff frequency such that the -6 dB
 * point for filter is reached at 6 kHz, while newer Amigas have no filtering.
 *
 * The LED filter is complicated, and we are modelling it with a pair of
 * RC filters, the other providing a highboost. The LED starts to cut
 * into signal somewhere around 5-6 kHz, and there's some kind of highboost
 * in effect above 12 kHz. Better measurements are required.
 *
 * The current filtering should be accurate to 2 dB with the filter on,
 * and to 1 dB with the filter off.
*/

static int filter(int input, struct filter_state *fs)
{
    int o;
    float normal_output, led_output;

    input = (uae_s16)input;
    switch (sound_use_filter) {
    case FILTER_NONE:
	return input;
    case FILTER_MODEL_A500:
	fs->rc1 = a500e_filter1_a0 * input + (1 - a500e_filter1_a0) * fs->rc1 + DENORMAL_OFFSET;
	fs->rc2 = a500e_filter2_a0 * fs->rc1 + (1-a500e_filter2_a0) * fs->rc2;
	normal_output = fs->rc2;

	fs->rc3 = filter_a0 * normal_output + (1 - filter_a0) * fs->rc3;
	fs->rc4 = filter_a0 * fs->rc3       + (1 - filter_a0) * fs->rc4;
	fs->rc5 = filter_a0 * fs->rc4       + (1 - filter_a0) * fs->rc5;

	led_output = fs->rc5;
	break;

    case FILTER_MODEL_A1200:
	normal_output = input;

	fs->rc2 = filter_a0 * normal_output + (1 - filter_a0) * fs->rc2 + DENORMAL_OFFSET;
	fs->rc3 = filter_a0 * fs->rc2       + (1 - filter_a0) * fs->rc3;
	fs->rc4 = filter_a0 * fs->rc3       + (1 - filter_a0) * fs->rc4;

	led_output = fs->rc4;
	break;
    }

    if (led_filter_on)
	o = led_output;
    else
	o = normal_output;

    if (o > 32767)
	o = 32767;
    else if (o < -32768)
	o = -32768;

    return o;
}

/* This computes the 1st order low-pass filter term b0.
 * The a1 term is 1.0 - b0. The center frequency marks the -3 dB point. */
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
static float rc_calculate_a0 (int sample_rate, int cutoff_freq)
{
    float omega;
    /* The BLT correction formula below blows up if the cutoff is above nyquist. */
    if (cutoff_freq >= sample_rate / 2)
	return 1.0;

    omega = 2 * M_PI * cutoff_freq / sample_rate;
    /* Compensate for the bilinear transformation. This allows us to specify the
     * stop frequency more exactly, but the filter becomes less steep further
     * from stopband. */
    omega = tan (omega / 2) * 2;
    return 1 / (1 + 1 / omega);
}

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

    if (sound_use_filter_sinc) {
	n = (sound_use_filter_sinc == FILTER_MODEL_A500) ? 0 : 2;
	if (led_filter_on)
	    n += 1;
    } else {
	n = 4;
    }
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

static void sample16si_anti_handler (void)
{
    int datas[4], data1, data2;

    samplexx_anti_handler (datas);
    data1 = datas[0] + datas[3];
    data2 = datas[1] + datas[2];
    FINISH_DATA (data1, 16, 1);
    if (sound_use_filter)
	data1 = filter (data1, &sound_filter_state[0]);
    put_sound_word_right (data1);
    FINISH_DATA (data2, 16, 1);
    if (sound_use_filter)
	data2 = filter (data2, &sound_filter_state[1]);
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
	if (sound_use_filter)
	    data = filter (data, &sound_filter_state[0]);
	put_sound_word_right (data);
    }

    data1 += data2;
    {
	uae_u32 data = SBASEVAL16(1) + data1;
	FINISH_DATA (data, 16, 1);
	if (sound_use_filter)
	    data = filter (data, &sound_filter_state[1]);
	put_sound_word_left (data);
    }

    check_sound_buffers ();
}

void switch_audio_interpol (void)
{
    if (currprefs.sound_interpol == 0) {
	changed_prefs.sound_interpol = 1;
	write_log ("Resampler on: sinc\n");
    } else if (currprefs.sound_interpol == 1) {
	changed_prefs.sound_interpol = 2;
	write_log ("Resampler on: anti\n");
    } else {
	changed_prefs.sound_interpol = 0;
	write_log ("Resampler off\n");
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
	if (currprefs.ntscmode)
	    scaled_sample_evtime = (unsigned long)(MAXHPOS_NTSC * MAXVPOS_NTSC * freq * CYCLE_UNIT + obtainedfreq - 1) / obtainedfreq;
	else
	    scaled_sample_evtime = (unsigned long)(MAXHPOS_PAL * MAXVPOS_PAL * freq * CYCLE_UNIT + obtainedfreq - 1) / obtainedfreq;
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
	cdp->nextdat = chipmem_agnus_wget (cdp->pt);

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

	if ((INTREQR () & (0x80 << nr)) && !cdp->dmaen) {
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

    memset (sound_filter_state, 0, sizeof sound_filter_state);
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
    currprefs.sound_filter = changed_prefs.sound_filter;
    currprefs.sound_filter_type = changed_prefs.sound_filter_type;

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

    led_filter_forced = -1; // always off
    sound_use_filter = sound_use_filter_sinc = 0;
    if (currprefs.sound_filter != FILTER_SOUND_OFF) {
	if (currprefs.sound_filter == FILTER_SOUND_ON)
	    led_filter_forced = 1;
	if (currprefs.sound_filter == FILTER_SOUND_EMUL)
	    led_filter_forced = 0;
	if (currprefs.sound_filter_type == FILTER_SOUND_TYPE_A500)
	    sound_use_filter = FILTER_MODEL_A500;
	else if (currprefs.sound_filter_type == FILTER_SOUND_TYPE_A1200)
	    sound_use_filter = FILTER_MODEL_A1200;
    }
    a500e_filter1_a0 = rc_calculate_a0(currprefs.sound_freq, 6200);
    a500e_filter2_a0 = rc_calculate_a0(currprefs.sound_freq, 20000);
    filter_a0 = rc_calculate_a0(currprefs.sound_freq, 7000);
    led_filter_audio();

    /* Select the right interpolation method.  */
    if (sample_handler == sample16s_handler
	|| sample_handler == sample16si_sinc_handler
	|| sample_handler == sample16si_anti_handler)
    {
	sample_handler = (currprefs.sound_interpol == 0 ? sample16s_handler
			  : currprefs.sound_interpol == 1 ? sample16si_sinc_handler
			  : sample16si_anti_handler);
    }
    sample_prehandler = NULL;
    if (currprefs.sound_interpol == 1) {
	sound_use_filter_sinc = sound_use_filter;
	sound_use_filter = 0;
	sample_prehandler = sinc_prehandler;
    } else if (currprefs.sound_interpol == 2) {
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
	    cdp->nextdat = chipmem_agnus_wget (cdp->pt);
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

void AUDxDAT (int nr, uae_u16 v)
{
    struct audio_channel_data *cdp = audio_channel + nr;

    if (currprefs.produce_sound == 0)
	return;

    update_audio ();

    cdp->dat = v;
    if (cdp->state == 0 && !(INTREQR () & (0x80 << nr))) {
	cdp->state = 2;
	INTREQ (0x8000 | (0x80 << nr));
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

void led_filter_audio (void)
{
    led_filter_on = 0;
    if (led_filter_forced > 0 || (gui_data.powerled && led_filter_forced >= 0))
	led_filter_on = 1;
    gui_led (0, gui_data.powerled);
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

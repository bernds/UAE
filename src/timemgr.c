 /*
  * UAE - The Un*x Amiga Emulator
  *
  * Supporting functions for timekeeping in event.h 
  *
  * Copyright 1995-2007 Bernd Schmidt
  */

#include "sysconfig.h"
#include "sysdeps.h"

#include <ctype.h>
#include <assert.h>

#include "options.h"
#include "threaddep/thread.h"
#include "uae.h"
#include "gensound.h"
#include "sounddep/sound.h"
#include "events.h"
#include "memory.h"
#include "custom.h"

/* Events */

unsigned long int currcycle, nextevent;
struct ev eventtab[ev_max];

/* Time taken for one frame given the current display settings
   (NTSC vs. PAL).  */
frame_time_t vsynctime;
/* Timestamps of the next VSYNC event.  */
frame_time_t vsyncmintime;

int vsyncmintime_valid;

/* Set if gettimeofday has high enough resolution for our purposes.  */
int use_gtod = 0;

/* Adjusted in do_cycles to keep track of how many gettimeofday calls
   we can safely afford to skip.  */
unsigned long nr_gtod_to_skip = 50000;
/* An upper bound on the previous value, determined at runtime.  */
unsigned long max_gtod_to_skip = ULONG_MAX;

/* First time we measured in do_cycles when waiting for vsyncmintime.  */
frame_time_t first_measured_gtod;
/* Number of gettimeofday calls while waiting for vsyncmintime.
   Affected by nr_gtod_to_skip.  */
unsigned long nr_gtod_done;
/* A counter used internally by do_cycles to skip calls to gettimeofday.  */
unsigned long gtod_counter;

/* Set by the sound initialization code if the sound code can be used for
   letting the m68k get extra cycles.  */
int sync_with_sound;

/* The number of ticks in a frame_time_t per second.  */
unsigned long syncbase;

int is_lastline;
volatile int delaying_for_sound;

unsigned long gtod_resolution, gtod_secs;

void init_gtod (void)
{
    struct timeval tv1, tv2;
    gettimeofday (&tv1, NULL);
    do {
	gettimeofday (&tv2, NULL);
    } while (tv1.tv_sec == tv2.tv_sec
	     && tv1.tv_usec == tv2.tv_usec);

    gtod_resolution = (tv2.tv_usec - tv1.tv_usec + 1000000 * (tv2.tv_sec - tv1.tv_sec));
    if (gtod_resolution < 1000) {
	use_gtod = 1;
	syncbase = 1000000;
    }
}

void reset_frame_rate_hack (void)
{
    if (currprefs.m68k_speed != -1)
	return;

    if (! sync_with_sound && ! use_gtod) {
	currprefs.m68k_speed = 0;
	return;
    }

    vsyncmintime_valid = 0;
    is_lastline = 0;
    vsyncmintime = get_current_time(1);
    vsyncmintime += vsynctime;
    write_log ("Resetting frame rate hack\n");
}

void compute_vsynctime (void)
{
    vsynctime = syncbase / vblank_hz;
    if (use_gtod)
	vsynctime -= gtod_resolution < 100 ? 100 : gtod_resolution;
    if (currprefs.produce_sound > 1 && !sync_with_sound) {
	vsynctime = vsynctime * 19 / 20;
    }
}

void time_vsync (void)
{
    if (sync_with_sound) {
	/* We don't strictly need it, but keep vsyncmintime accurate.  */
	if (use_gtod) {
	    vsyncmintime = get_current_time(1) + vsynctime;
	    nr_gtod_done = 0;
	    vsyncmintime_valid = 1;
	}
	return;
    }

    if (vsyncmintime_valid) {
	if (nr_gtod_done > 50 && nr_gtod_to_skip < ULONG_MAX / 2) {
	    /* Try to reduce the number of gtod calls per frame - 50
	       should be enough, so whenever we exceed that number,
	       increase the number of gtod calls we skip - about 12%
	       each time.  */
	    nr_gtod_to_skip += nr_gtod_to_skip / 8;
	}
    }

    if (currprefs.m68k_speed == -1) {
	frame_time_t curr_time = get_current_time (1);
	/* If we got behind in one frame, try catching up by using the
	   previous vsyncmintime as a base.  If we get too far behind,
	   give up and reset.  */
	if (vsyncmintime_valid
	    && curr_time > vsyncmintime
	    && curr_time < vsyncmintime + vsynctime / 2)
	    vsyncmintime += vsynctime;
	else
	    vsyncmintime = curr_time + vsynctime;
	nr_gtod_done = 0;
	vsyncmintime_valid = 1;
    } else {
	/* No sound, and not using maximum CPU speed: delay until the frame
	   has taken 20ms.  */
	if (currprefs.produce_sound < 2 && vsyncmintime_valid && use_gtod) {
	    frame_time_t curr_time;
	    do
		curr_time = get_current_time (0);
	    while ((long int)(get_current_time (0) - vsyncmintime) < 0);
	}
	vsyncmintime = get_current_time (1) + vsynctime;
	vsyncmintime_valid = 1;
	nr_gtod_done = 0;
    }
}

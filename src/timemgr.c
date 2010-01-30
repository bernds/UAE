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

int rpt_available = 0;

int is_lastline;
volatile int blocking_on_sound;

void reset_frame_rate_hack (void)
{
    if (currprefs.m68k_speed != -1)
	return;

    if (! rpt_available) {
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
    if (currprefs.produce_sound > 1) {
	vsynctime = vsynctime * 19 / 20;
    }
}

void time_vsync (void)
{
    if (vsyncmintime_valid) {
	frame_time_t measured = vsyncmintime - first_measured_gtod;
	unsigned long ideal;
	if (nr_gtod_done) {
	    if (use_gtod && gtod_resolution < measured)
		measured -= gtod_resolution;
	    /* Interval between gettimeofday calls.  */
	    measured /= nr_gtod_done;

	    /* syncbase has the number of ticks in one second.
	       Hence, syncbase / 5000 ~= 0.2ms.  We want to have at least 100
	       gtod calls per frame.  */
	    if (measured < syncbase / 5000) {
		if (max_gtod_to_skip == ULONG_MAX)
		    max_gtod_to_skip = nr_gtod_to_skip;
		else
		    max_gtod_to_skip = max_gtod_to_skip * 19 / 20;
	    }
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
	if (vsyncmintime_valid
	    && (use_gtod
#ifdef RPT_WORKS_OK
		|| RPT_WORKS_OK
#endif
		))
	{
	    frame_time_t curr_time;
	    do
		curr_time = get_current_time (0);
	    while ((long int)(get_current_time (0) - vsyncmintime) < 0);
	}
	vsyncmintime = get_current_time (1) + vsynctime;
	vsyncmintime_valid = 1;
    }
}

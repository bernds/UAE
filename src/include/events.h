 /*
  * UAE - The Un*x Amiga Emulator
  *
  * Events
  * These are best for low-frequency events. Having too many of them,
  * or using them for events that occur too frequently, can cause massive
  * slowdown.
  *
  * Copyright 1995-1998 Bernd Schmidt
  */

typedef unsigned long frame_time_t;

extern frame_time_t vsynctime, vsyncmintime;
extern frame_time_t gtod_resolution;
extern int vsyncmintime_valid;

extern frame_time_t first_measured_gtod;
extern unsigned long gtod_secs;
extern unsigned long nr_gtod_to_skip;
extern unsigned long nr_gtod_done, gtod_counter;

extern int sync_with_sound;
extern int use_gtod;

extern unsigned long currcycle, nextevent;

extern int is_lastline;
extern volatile int delaying_for_sound;
extern int active_fs_packets;

typedef void (*evfunc)(void);

struct ev
{
    int active;
    unsigned long int evtime, oldcycles;
    evfunc handler;
};

enum {
    ev_hsync, ev_copper, ev_audio, ev_cia, ev_blitter, ev_disk,
    ev_max
};

extern struct ev eventtab[ev_max];

extern void reset_frame_rate_hack (void);
extern void compute_vsynctime (void);
extern void time_vsync (void);

extern void init_gtod (void);

STATIC_INLINE frame_time_t get_current_time (int redo_secs)
{
    struct timeval tv;
    gettimeofday (&tv, NULL);
    if (redo_secs)
	gtod_secs = tv.tv_sec;
    return tv.tv_usec + (tv.tv_sec - gtod_secs) * 1000000;
}

STATIC_INLINE void events_schedule (void)
{
    int i;

    unsigned long int mintime = ~0L;
    for (i = 0; i < ev_max; i++) {
	if (eventtab[i].active) {
	    unsigned long int eventtime = eventtab[i].evtime - currcycle;
	    if (eventtime < mintime)
		mintime = eventtime;
	}
    }
    nextevent = currcycle + mintime;
}

STATIC_INLINE void do_cycles_slow (unsigned long cycles_to_add)
{
    if (delaying_for_sound)
	return;

    if (is_lastline
	&& eventtab[ev_hsync].evtime - currcycle <= cycles_to_add)
    {
	static int n_skipped = 0;
	frame_time_t curr;

	if (gtod_counter++ < nr_gtod_to_skip)
	    return;
	gtod_counter = 0;
	curr = get_current_time (0);
		
	if ((long int)(curr - vsyncmintime) < 0) {
	    nr_gtod_done++;
	    return;
	}
#if 0
	printf ("skipped %d times\n", nr_gtod_done);
#endif
    }

    while ((nextevent - currcycle) <= cycles_to_add) {
	int i;
	cycles_to_add -= (nextevent - currcycle);
	currcycle = nextevent;

	for (i = 0; i < ev_max; i++) {
	    if (eventtab[i].active && eventtab[i].evtime == currcycle) {
		(*eventtab[i].handler)();
	    }
	}
	events_schedule();
    }
    currcycle += cycles_to_add;
}

/* This is a special-case function.  Normally, all events should lie in the
   future; they should only ever be active at the current cycle during
   do_cycles.  However, a snapshot is saved during do_cycles, and so when
   restoring it, we may have other events pending.  */
STATIC_INLINE void handle_active_events (void)
{
    int i;
    for (i = 0; i < ev_max; i++) {
	if (eventtab[i].active && eventtab[i].evtime == currcycle) {
	    (*eventtab[i].handler)();
	}
    }
}

STATIC_INLINE unsigned long get_cycles (void)
{
    return currcycle;
}

extern void init_eventtab (void);

#define do_cycles do_cycles_slow

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

#include "machdep/rpt.h"
extern frame_time_t vsynctime, vsyncmintime;
extern void reset_frame_rate_hack (void);
extern int rpt_available;

extern unsigned long int cycles, nextevent, is_lastline;
extern unsigned long int sample_evtime;
typedef void (*evfunc)(void);

struct ev
{
    int active;
    unsigned long int evtime, oldcycles;
    evfunc handler;
};

enum {
    ev_hsync, ev_copper, ev_cia, ev_blitter, ev_disk,
    ev_max
};

extern struct ev eventtab[ev_max];

STATIC_INLINE void events_schedule (void)
{
    int i;

    unsigned long int mintime = ~0L;
    for(i = 0; i < ev_max; i++) {
	if (eventtab[i].active) {
	    unsigned long int eventtime = eventtab[i].evtime - cycles;
	    if (eventtime < mintime)
		mintime = eventtime;
	}
    }
    nextevent = cycles + mintime;
}

STATIC_INLINE void do_cycles_slow (unsigned long cycles_to_add)
{
    if (is_lastline && eventtab[ev_hsync].evtime-cycles <= cycles_to_add
	&& (long int)(read_processor_time () - vsyncmintime) < 0)
	return;

    if ((nextevent - cycles) <= cycles_to_add) {
	for (; cycles_to_add != 0; cycles_to_add--) {
	    if (++cycles == nextevent) {
		int i;

		for (i = 0; i < ev_max; i++) {
		    if (eventtab[i].active && eventtab[i].evtime == cycles) {
			(*eventtab[i].handler)();
		    }
		}
		events_schedule();
	    }
	}
    }
    cycles += cycles_to_add;
}

STATIC_INLINE void do_cycles_fast (void)
{
    if (is_lastline && eventtab[ev_hsync].evtime-cycles <= 1
	&& (long int)(read_processor_time () - vsyncmintime) < 0)
	return;

    cycles++;
    if (nextevent == cycles) {
	int i;

	for (i = 0; i < ev_max; i++) {
	    if (eventtab[i].active && eventtab[i].evtime == cycles) {
		(*eventtab[i].handler) ();
	    }
	}
	events_schedule();
    }

}

#if /* M68K_SPEED == 1 */  0
#define do_cycles do_cycles_fast
#else
#define do_cycles do_cycles_slow
#endif

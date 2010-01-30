 /* 
  * UAE - The Un*x Amiga Emulator
  * 
  * Miscellaneous machine dependent support functions and definitions
  *
  * Copyright 1996 Bernd Schmidt
  */

#include "sysconfig.h"
#include "sysdeps.h"

#include "config.h"
#include "options.h"
#include "events.h"
#include "machdep/m68k.h"

struct flag_struct regflags;

static volatile frame_time_t last_time, best_time;
static volatile int loops_to_go;

void machdep_init (void)
{
    rpt_available = 1;
    fprintf(stderr, "Calibrating delay loop.. ");
    fflush(stderr);
    loops_to_go = 5;

    sync();
    last_time = read_processor_time();
    usleep(loops_to_go * 1000000);
    best_time = read_processor_time()-last_time;
    sync();

    vsynctime = best_time / (50*loops_to_go);
    fprintf(stderr,"ok - %d\n",best_time/loops_to_go);
    last_time = read_processor_time();
    best_time = read_processor_time()-last_time;
}



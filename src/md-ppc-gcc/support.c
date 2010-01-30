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

int fast_memcmp(const void *foo, const void *bar, int len)
{
	return memcmp(foo, bar, len);
}

int memcmpy(void *foo, const void *bar, int len)
{
	while ((len -= 4) >= 0)
		if(*((long*)foo)++ != *((long*)bar)++)
			goto diff4;
	len += 4;
    
	while (len--)
		if (*((char*)foo)++ != *((char*)bar)++) {
			((char*)foo)[-1] = ((char*)bar)[-1];
			goto diff1;
		}
	return 0;

diff4:
	((long*)foo)[-1] = ((long*)bar)[-1];
	while ((len -= 4) >= 0)
		*((long*)foo)++ = *((long*)bar)++;
	len += 4;

diff1:
	while (len--)
		*((char*)foo)++ = *((char*)bar)++;
	return 1;
}

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



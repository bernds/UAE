/*
 * synchronize system time with the host's time
 *
 * Compile with DCC (or SAS/C)
 *
 * Copyright 1997, 1999 Bernd Schmidt
 * Copyright 1999 Patrick Ohly
 */

#include <stdio.h>

#include <exec/devices.h>
#include <exec/interrupts.h>
#include <exec/nodes.h>
#include <exec/io.h>
#include <exec/memory.h>
#include <intuition/intuitionbase.h>
#include <intuition/preferences.h>
#include <devices/input.h>
#include <devices/inputevent.h>
#include <devices/timer.h>
#include <hardware/intbits.h>
#include <clib/alib_protos.h>
#include <clib/exec_protos.h>

#ifdef _DCC
int (*calltrap)(__d0 int, __a0 struct timeval *) = 0xF0FFA0;
#else
/* SAS */
__asm int (*calltrap)(register __d0 int, register __a0 struct timeval *) = (APTR)0xF0FFA0;
#endif

int main (int argc, char **argv)
{
    struct timerequest *timereq = 0;
    struct MsgPort *timeport;
    int result = calltrap (0, 0);

    if (result == 1)
	fprintf (stderr, "timehack already running.\n");
    else if (result == 2)
	fprintf (stderr, "timehack not supported with this version of UAE.\n");
    if (result != 0)
	exit (5);

    timeport = CreatePort (0, 0);
    if (timeport)
	timereq = CreateStdIO(timeport);

    if (timereq == 0)
	goto fail;

    if (OpenDevice ("timer.device", UNIT_VBLANK, timereq, 0) != 0)
	goto fail;

    SetTaskPri (FindTask (NULL), 20); /* same as input.device */
    for (;;) {
        struct timeval cur_sys_time;

        timereq->tr_node.io_Command = TR_GETSYSTIME;
        DoIO(timereq);
        cur_sys_time = timereq->tr_time;
        calltrap (1, &timereq->tr_time);
        if (timereq->tr_time.tv_secs > cur_sys_time.tv_secs
	    || (timereq->tr_time.tv_secs == cur_sys_time.tv_secs
		&& timereq->tr_time.tv_micro > cur_sys_time.tv_micro))
	{
            timereq->tr_node.io_Command = TR_SETSYSTIME;
            DoIO(timereq);
        }

        timereq->tr_time.tv_secs = 1;
	timereq->tr_time.tv_micro = 0;
        timereq->tr_node.io_Command = TR_ADDREQUEST;
	DoIO(timereq);
    }
    fail:
    fprintf (stderr, "Couldn't start timehack (that's bad!)\n");
    exit (5);
}

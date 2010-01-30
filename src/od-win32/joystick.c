/* 
 * UAE - The Un*x Amiga Emulator
 *
 * Win32 interface
 *
 * Copyright 1997 Mathias Ortmann
 */

#include <windows.h>
#include <ddraw.h>
#include <stdlib.h>
#include <stdarg.h>
#include <mmsystem.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <io.h>

#include "config.h"
#include "sysconfig.h"
#include "sysdeps.h"
#include "options.h"
#include "osdep/win32gui.h"
#include "include/memory.h"
#include "osdep/resource.h"

/* Joystick emulation, Win32 interface */
int nr_joysticks;

int joystickspresent[MAXJOYSTICKS];
static int unplugged[MAXJOYSTICKS];
static int warned[MAXJOYSTICKS];
static JOYCAPS jc[MAXJOYSTICKS];
static JOYINFOEX ji =
{sizeof (ji), JOY_RETURNBUTTONS | JOY_RETURNX | JOY_RETURNY};

void read_joystick (int nr, short *dir, int *button)
{
    int left = 0, right = 0, top = 0, bot = 0;
    int joyerr;

    *dir = *button = 0;
    if (!joystickspresent[nr])
	return;

    if (unplugged[nr]) {
	unplugged[nr]--;
	return;
    }
    switch (joyerr = joyGetPosEx (JOYSTICKID1 + nr, &ji)) {
    case JOYERR_NOERROR:
	warned[nr] = 0;
	break;
    case JOYERR_UNPLUGGED:
	if (!warned[nr]) {
	    write_log( "Joystick %d seems to be unplugged.\n", nr );
	    warned[nr] = 1;
	}
	unplugged[nr] = 200;
	return;
    default:
	write_log( "Error reading joystick %d: %d.\n", nr, joyerr );
	unplugged[nr] = 100;
	return;
    }

    if (ji.dwXpos < (jc[nr].wXmin + 2 * (jc[nr].wXmax - jc[nr].wXmin) / 5))
	left = 1;
    else if (ji.dwXpos > (jc[nr].wXmin + 3 * (jc[nr].wXmax - jc[nr].wXmin) / 5))
	right = 1;

    if (ji.dwYpos < (jc[nr].wYmin + 2 * (jc[nr].wYmax - jc[nr].wYmin) / 5))
	top = 1;
    else if (ji.dwYpos > (jc[nr].wYmin + 3 * (jc[nr].wYmax - jc[nr].wYmin) / 5))
	bot = 1;

    if (left)
	top = !top;
    if (right)
	bot = !bot;
    *dir = bot | (right << 1) | (top << 8) | (left << 9);
    *button = ji.dwButtons;
}

void init_joystick (void)
{
    int nr;
    int found = 0;

    for (nr = 0; nr < MAXJOYSTICKS; nr++) {
	if (joyGetDevCaps (JOYSTICKID1 + nr, jc + nr, sizeof (jc)) == JOYERR_NOERROR) {
	    write_log( "Joystick %d: %s with %d buttons\n", nr, jc[nr].szPname, jc[nr].wNumButtons );
	    joystickspresent[nr] = 1;
	    found++;
	}
    }
    nr_joysticks = found;
    write_log( "%d joystick(s) found.\n", found );
}

void close_joystick (void)
{
}

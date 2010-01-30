 /*
  * UAE - The Un*x Amiga Emulator
  *
  * Joystick emulation for DOS
  *
  * Copyright 1996, 1997 Gustavo Goedert
  */

#include "sysconfig.h"
#include "sysdeps.h"

#include <dos.h>

#include "config.h"
#include "options.h"
#include "memory.h"
#include "custom.h"
#include "joystick.h"

int nr_joysticks;
int HasJoy0 = 0, HasJoy1 = 0;
int minx0, maxx0, miny0, maxy0;
int minx1, maxx1, miny1, maxy1;

void read_joystick(int nr, unsigned int *dir, int *button) {
    int *minx, *maxx, *miny, *maxy;
    int left = 0, right = 0, top = 0, bot = 0;
    char JoyPort;
    int laps, JoyX, JoyY, XCenter, YCenter, XDelta, YDelta;

    *dir = 0;
    *button = 0;
    if (((nr == 0) && (HasJoy0 == 0)) || ((nr == 1) && (HasJoy1 == 0)))
	return;

    JoyX = 0;
    JoyY = 0;
    laps = 0;
    outportb(0x201, 0xff);
    if (nr == 0) {
	disable();
	do {
	    JoyPort = inportb(0x201);
	    if (JoyPort & 1)
		JoyX++;
	    if (JoyPort & 2)
		JoyY++;
	    laps++;
	} while(((JoyPort & 3) != 0) && (laps != 65535));
	enable();
	if (laps == 65535) {
	    HasJoy0 = 0;
	    return;
	}
	minx = &minx0;
	maxx = &maxx0;
	miny = &miny0;
	maxy = &maxy0;
    } else {
	disable();
	do {
	    JoyPort = inportb(0x201);
	    if (JoyPort & 4)
		JoyX++;
	    if (JoyPort & 8)
		JoyY++;
	    laps++;
	} while(((JoyPort & 12) != 0) && (laps != 65535));
	enable();
	if (laps == 65535) {
	    HasJoy1 = 0;
	    return;
	}
	minx = &minx1;
	maxx = &maxx1;
	miny = &miny1;
	maxy = &maxy1;
    }

    if (JoyX < (*minx)) *minx = JoyX;
    if (JoyY < (*miny)) *miny = JoyY;
    if (JoyX > (*maxx)) *maxx = JoyX;
    if (JoyY > (*maxy)) *maxy = JoyY;

    XCenter = ((*maxx)+(*minx))/2;
    YCenter = ((*maxy)+(*miny))/2;
    XDelta = ((*maxx)-(*minx))/3;
    YDelta = ((*maxy)-(*miny))/3;

    if (JoyX < (XCenter-XDelta))
	left = 1;
    if (JoyX > (XCenter+XDelta))
	right = 1;

    if (JoyY < (YCenter-YDelta))
	top = 1;
    if (JoyY > (YCenter+YDelta))
	bot = 1;

    if (left) top = !top;
    if (right) bot = !bot;
    *dir = bot | (right << 1) | (top << 8) | (left << 9);

    JoyPort = ~JoyPort;
    if (nr == 0) {
       if (JoyPort & 16) *button += 1;
       if (JoyPort & 32) *button += 2;
    } else {
	if (JoyPort & 64) *button += 1;
	if (JoyPort & 128) *button += 2;
    }
    if ((*button) == 3) *button = 4;
}

void init_joystick(void) {
    int laps, JoyX, JoyY;
    char JoyPort;

    JoyX = 0;
    JoyY = 0;
    laps = 0;
    disable();
    outportb(0x201, 0xff);
    do {
	JoyPort = inportb(0x201);
	if (JoyPort & 1)
	    JoyX++;
	if (JoyPort & 2)
	    JoyY++;
	laps++;
    } while(((JoyPort & 3) != 0) && (laps != 65535));
    enable();
    if (laps != 65535) {
	HasJoy0 = 1;
	minx0 = JoyX - 2*JoyX/3;
	maxx0 = JoyX + 2*JoyX/3;
	miny0 = JoyY - 2*JoyY/3;
	maxy0 = JoyY + 2*JoyY/3;
    }

    JoyX = 0;
    JoyY = 0;
    laps = 0;
    disable();
    outportb(0x201, 0xff);
    do {
	JoyPort = inportb(0x201);
	if (JoyPort & 4)
	    JoyX++;
	if (JoyPort & 8)
	    JoyY++;
	laps++;
    } while(((JoyPort & 12) != 0) && (laps != 65535));
    enable();
    if (laps != 65535) {
	HasJoy1 = 1;
	minx1 = JoyX - 2*JoyX/3;
	maxx1 = JoyX + 2*JoyX/3;
	miny1 = JoyY - 2*JoyY/3;
	maxy1 = JoyY + 2*JoyY/3;
    }

    nr_joysticks = HasJoy0 + HasJoy1;
}

void close_joystick(void) {
}

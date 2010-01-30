 /* 
  * UAE - The Un*x Amiga Emulator
  * 
  * Joystick emulation for Linux and BSD. They share too much code to
  * split this file.
  * 
  * Copyright 1997 Bernd Schmidt
  * Copyright 1998 Krister Walfridsson
  */

#include "sysconfig.h"
#include "sysdeps.h"

#include "config.h"
#include "options.h"
#include "memory.h"
#include "custom.h"
#include "joystick.h"
#include "SDL.h"

int nr_joysticks;

static SDL_Joystick *joy0, *joy1;

struct joy_range
{
    int minx, maxx, miny, maxy;
} range0, range1;

void read_joystick(int nr, unsigned int *dir, int *button)
{
    int x_axis, y_axis;
    int left = 0, right = 0, top = 0, bot = 0;
    int len, i, num;
    SDL_Joystick *joy = nr == 0 ? joy0 : joy1;
    struct joy_range *r = nr == 0 ? &range0 : &range1;

    *dir = 0;
    *button = 0;
    if (nr >= nr_joysticks)
    	return;

    SDL_JoystickUpdate ();

    x_axis = SDL_JoystickGetAxis (joy, 0);
    y_axis = SDL_JoystickGetAxis (joy, 1);

    if (x_axis < r->minx) r->minx = x_axis;
    if (y_axis < r->miny) r->miny = y_axis;
    if (x_axis > r->maxx) r->maxx = x_axis;
    if (y_axis > r->maxy) r->maxy = y_axis;
    
    if (x_axis < (r->minx + (r->maxx - r->minx)/3))
    	left = 1;
    else if (x_axis > (r->minx + 2*(r->maxx - r->minx)/3))
    	right = 1;

    if (y_axis < (r->miny + (r->maxy - r->miny)/3))
    	top = 1;
    else if (y_axis > (r->miny + 2*(r->maxy - r->miny)/3))
    	bot = 1;

    if (left) top = !top;
    if (right) bot = !bot;
    *dir = bot | (right << 1) | (top << 8) | (left << 9);
    num = SDL_JoystickNumButtons (joy);
    if (num > 16)
	num = 16;
    for (i = 0; i < num; i++)
	*button |= (SDL_JoystickGetButton (joy, i) & 1) << i;
}

void init_joystick(void)
{
    int i;
    nr_joysticks = SDL_NumJoysticks ();
    if (nr_joysticks > 0)
	joy0 = SDL_JoystickOpen (0);
    if (nr_joysticks > 1)
	joy1 = SDL_JoystickOpen (1);
    range0.minx = INT_MAX;
    range0.maxx = INT_MIN;
    range0.miny = INT_MAX;
    range0.maxy = INT_MIN;
    range1.minx = INT_MAX;
    range1.maxx = INT_MIN;
    range1.miny = INT_MAX;
    range1.maxy = INT_MIN;
}

void close_joystick(void)
{
    if (nr_joysticks > 0)
	SDL_JoystickClose (joy0);
    if (nr_joysticks > 1)
	SDL_JoystickClose (joy1);
}

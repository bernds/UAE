 /*
  * UAE - The Un*x Amiga Emulator
  *
  * Joystick emulation for Linux and BSD. They share too much code to
  * split this file.
  *
  * This uses the deprecated 0.x Linux joystick API.
  *
  * Copyright 1997 Bernd Schmidt
  * Copyright 1998 Krister Walfridsson
  * Copyright 2003 Richard Drummond
  */

#include "sysconfig.h"
#include "sysdeps.h"

#include "config.h"
#include "options.h"
#include "memory.h"
#include "custom.h"
#include "inputdevice.h"

#ifdef HAVE_MACHINE_JOYSTICK_H

/* The BSD way.  */

# include <machine/joystick.h>
typedef struct joystick uae_joystick_t;

#define JS_DEVNAME_PREFIX "joy"

#else

/* The Linux way.  */

/* There are too many different versions of <linux/joystick.h>.  Rather
 * than trying to work correctly with all of them, we duplicate the
 * necessary definitions here.  */
typedef struct
{
    int buttons;
    int x;
    int y;
} uae_joystick_t;

#define JS_DEVNAME_PREFIX "js"

#endif

/* Hard code these for the old joystick API */
#define MAX_BUTTONS  2
#define MAX_AXLES    2
#define FIRST_AXLE   0
#define FIRST_BUTTON 2

static int nr_joysticks;

static int js0, js1;

struct joy_range
{
    int minx, maxx, miny, maxy;
    int centrex, centrey;
} range0, range1;

static int get_joystick_num (void);

static void read_joy(int nr)
{
    uae_joystick_t buffer;
    int len;
    int fd = nr == 0 ? js0 : js1;
    struct joy_range *r = nr == 0 ? &range0 : &range1;

    if (nr >= nr_joysticks)
	return;

    len = read(fd, &buffer, sizeof(buffer));
    if (len != sizeof(buffer))
	return;

    /* According to old 0.x JS API, we don't know the range
     * or the centre for either axis, so we try to work these
     * out as we go along.
     *
     * Must be a better way to do this . . .
     */
    if (buffer.x < r->minx) r->minx = buffer.x;
    if (buffer.y < r->miny) r->miny = buffer.y;
    if (buffer.x > r->maxx) r->maxx = buffer.x;
    if (buffer.y > r->maxy) r->maxy = buffer.y;

    r->centrex = (r->maxx-r->minx)/2 + r->minx;
    r->centrey = (r->maxy-r->miny)/2 + r->miny;

    /* Translate these values to be centred on 0 and
     * feed 'em to the inputdevice system */
    setjoystickstate (nr, 0, buffer.x - r->centrex, r->centrex );
    setjoystickstate (nr, 1, buffer.y - r->centrey, r->centrey );

#ifdef HAVE_MACHINE_JOYSTICK_H
    setjoybuttonstate (nr, 0, buffer.b1);
    setjoybuttonstate (nr, 1, buffer.b2);
#else
    setjoybuttonstate (nr, 0, buffer.buttons & 1);
    setjoybuttonstate (nr, 1, buffer.buttons & 2);
#endif
}

static int init_joysticks(void)
{
    nr_joysticks = 0;
    js0 = -1; js1 = -1;

    if ((js0 = open("/dev/" JS_DEVNAME_PREFIX "0", O_RDONLY)) >= 0)
	nr_joysticks++;

    if ((js1 = open("/dev/" JS_DEVNAME_PREFIX "1", O_RDONLY)) >= 0)
	nr_joysticks++;

    write_log ("Found %d joysticks\n", nr_joysticks);

    range0.minx = INT_MAX;
    range0.maxx = INT_MIN;
    range0.miny = INT_MAX;
    range0.maxy = INT_MIN;
    range1.minx = INT_MAX;
    range1.maxx = INT_MIN;
    range1.miny = INT_MAX;
    range1.maxy = INT_MIN;
    range0.centrex = 0;
    range1.centrey = 0;
    return 1;
}

static void close_joysticks(void)
{
    if (js0 >= 0)
	close (js0);
    if (js1 >= 0)
	close (js1);
}

static int acquire_joy (int num, int flags)
{
    return 1;
}

static void unacquire_joy (int num)
{
}

static void read_joysticks (void)
{
    int i;
    for (i = 0; i < get_joystick_num(); i++)
	read_joy (i);
}

static int get_joystick_num (void)
{
    return nr_joysticks;
}

static char *get_joystick_name (int joy)
{
    static char name[100];
    sprintf (name, "%d: /dev/%s%d", joy + 1, JS_DEVNAME_PREFIX, joy);
    return name;
}

static int get_joystick_widget_num (int joy)
{
    return MAX_AXLES + MAX_BUTTONS;
}

static int get_joystick_widget_type (int joy, int num, char *name)
{
    if (num >= MAX_AXLES && num < MAX_AXLES+MAX_BUTTONS) {
	if (name)
	    sprintf (name, "Button %d", num + 1 - MAX_AXLES);
	return IDEV_WIDGET_BUTTON;
    } else if (num < MAX_AXLES) {
	if (name)
	    sprintf (name, "Axis %d", num + 1);
	return IDEV_WIDGET_AXIS;
    }
    return IDEV_WIDGET_NONE;
}

static int get_joystick_widget_first (int joy, int type)
{
    switch (type) {
	case IDEV_WIDGET_BUTTON:
	    return FIRST_BUTTON;
	case IDEV_WIDGET_AXIS:
	    return FIRST_AXLE;
    }

    return -1;
}

struct inputdevice_functions inputdevicefunc_joystick = {
    init_joysticks, close_joysticks, acquire_joy, unacquire_joy,
    read_joysticks, get_joystick_num, get_joystick_name,
    get_joystick_widget_num, get_joystick_widget_type,
    get_joystick_widget_first
};

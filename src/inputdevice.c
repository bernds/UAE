 /*
  * UAE - The Un*x Amiga Emulator
  *
  * Input devices
  *
  * Copyright 1995-2004 Bernd Schmidt
  * Copyright 1995 Alessandro Bissacco
  * Copyright 2000-2004 Toni Wilen
  */

#include "sysconfig.h"
#include "sysdeps.h"

#include <ctype.h>
#include <assert.h>

#include "config.h"
#include "options.h"
#include "threaddep/thread.h"
#include "uae.h"
#include "custom.h"
#include "xwin.h"
#include "memory.h"
#include "newcpu.h"
#include "drawing.h"
#include "picasso96.h"
#include "inputdevice.h"

/* Mouse and joystick emulation */

int buttonstate[3];
int mouse_x, mouse_y;
int joy0button, joy1button;
unsigned int joy0dir, joy1dir;

/* Mousehack stuff */

#define defstepx (1<<16)
#define defstepy (1<<16)
#define defxoffs 0
#define defyoffs 0

static const int docal = 60, xcaloff = 40, ycaloff = 20;
static const int calweight = 3;
static int lastsampledmx, lastsampledmy;
static int lastspr0x,lastspr0y,lastdiffx,lastdiffy,spr0pos,spr0ctl;
static int mstepx,mstepy,xoffs=defxoffs,yoffs=defyoffs;
static int sprvbfl;

int lastmx, lastmy;
int newmousecounters;
int ievent_alive = 0;

enum mousestate mousestate;

void mousehack_setdontcare (void)
{
    if (mousestate == dont_care_mouse)
	return;

    write_log ("Don't care mouse mode set\n");
    mousestate = dont_care_mouse;
    lastspr0x = lastmx; lastspr0y = lastmy;
    mstepx = defstepx; mstepy = defstepy;
}

void mousehack_setfollow (void)
{
    if (mousestate == follow_mouse)
	return;

    write_log ("Follow sprite mode set\n");
    mousestate = follow_mouse;
    lastdiffx = lastdiffy = 0;
    sprvbfl = 0;
    spr0ctl = spr0pos = 0;
    mstepx = defstepx; mstepy = defstepy;
}

uae_u32 mousehack_helper (void)
{
    int mousexpos, mouseypos;

#ifdef PICASSO96
    if (picasso_on) {
	picasso_clip_mouse (&lastmx, &lastmy);
	mousexpos = lastmx;
	mouseypos = lastmy;
    } else
#endif
    {
	/* @@@ This isn't completely right, it doesn't deal with virtual
	   screen sizes larger than physical very well.  */
	if (lastmy >= gfxvidinfo.height)
	    lastmy = gfxvidinfo.height - 1;
	if (lastmy < 0)
	    lastmy = 0;
	if (lastmx < 0)
	    lastmx = 0;
	if (lastmx >= gfxvidinfo.width)
	    lastmx = gfxvidinfo.width - 1;
	mouseypos = coord_native_to_amiga_y (lastmy) << 1;
	mousexpos = coord_native_to_amiga_x (lastmx);
    }

    switch (m68k_dreg (regs, 0)) {
    case 0:
	return ievent_alive ? -1 : needmousehack ();
    case 1:
	ievent_alive = 10;
	return mousexpos;
    case 2:
	return mouseypos;
    }
    return 0;
}

void togglemouse (void)
{
    switch (mousestate) {
     case dont_care_mouse: mousehack_setfollow (); break;
     case follow_mouse: mousehack_setdontcare (); break;
     default: break; /* Nnnnnghh! */
    }
}

STATIC_INLINE int adjust (int val)
{
    if (val > 127)
	return 127;
    else if (val < -127)
	return -127;
    return val;
}

void do_mouse_hack (void)
{
    int spr0x = ((spr0pos & 0xff) << 2) | ((spr0ctl & 1) << 1);
    int spr0y = ((spr0pos >> 8) | ((spr0ctl & 4) << 6)) << 1;
    int diffx, diffy;

    if (ievent_alive > 0) {
	mouse_x = mouse_y = 0;
	return;
    }
    switch (mousestate) {
    case normal_mouse:
	diffx = lastmx - lastsampledmx;
	diffy = lastmy - lastsampledmy;
	if (!newmousecounters) {
	    if (diffx > 127) diffx = 127;
	    if (diffx < -127) diffx = -127;
	    mouse_x += diffx;
	    if (diffy > 127) diffy = 127;
	    if (diffy < -127) diffy = -127;
	    mouse_y += diffy;
	}
	lastsampledmx += diffx; lastsampledmy += diffy;
	break;

    case dont_care_mouse:
	diffx = adjust (((lastmx - lastspr0x) * mstepx) >> 16);
	diffy = adjust (((lastmy - lastspr0y) * mstepy) >> 16);
	lastspr0x = lastmx; lastspr0y = lastmy;
	mouse_x += diffx; mouse_y += diffy;
	break;

    case follow_mouse:
	if (sprvbfl && sprvbfl-- > 1) {
	    int mousexpos, mouseypos;

	    if ((lastdiffx > docal || lastdiffx < -docal)
		&& lastspr0x != spr0x
		&& spr0x > plfstrt*4 + 34 + xcaloff
		&& spr0x < plfstop*4 - xcaloff)
	    {
		int val = (lastdiffx << 16) / (spr0x - lastspr0x);
		if (val >= 0x8000)
		    mstepx = (mstepx * (calweight - 1) + val) / calweight;
	    }
	    if ((lastdiffy > docal || lastdiffy < -docal)
		&& lastspr0y != spr0y
		&& spr0y > plffirstline + ycaloff
		&& spr0y < plflastline - ycaloff)
	    {
		int val = (lastdiffy << 16) / (spr0y - lastspr0y);
		if (val >= 0x8000)
		    mstepy = (mstepy * (calweight - 1) + val) / calweight;
	    }
	    if (lastmy >= gfxvidinfo.height)
		lastmy = gfxvidinfo.height-1;
	    mouseypos = coord_native_to_amiga_y (lastmy) << 1;
	    mousexpos = coord_native_to_amiga_x (lastmx);
	    diffx = adjust ((((mousexpos + xoffs - spr0x) & ~1) * mstepx) >> 16);
	    diffy = adjust ((((mouseypos + yoffs - spr0y) & ~1) * mstepy) >> 16);
	    lastspr0x = spr0x; lastspr0y = spr0y;
	    lastdiffx = diffx; lastdiffy = diffy;
	    mouse_x += diffx; mouse_y += diffy;
	}
	break;
	
    default:
	abort ();
    }
}

void mousehack_handle (int ctl, int pos)
{
    if (!sprvbfl && ((pos & 0xff) << 2) > 2 * DISPLAY_LEFT_SHIFT) {
	spr0ctl = ctl;
	spr0pos = pos;
	sprvbfl = 2;
    }
}

uae_u16 potgo_value;

void POTGO (uae_u16 v)
{
    potgo_value = v;
}

uae_u16 POTGOR (void)
{
    uae_u16 v = (potgo_value | (potgo_value >> 1)) & 0x5500;

    v |= (~potgo_value & 0xAA00) >> 1;

    if (JSEM_ISMOUSE (0, &currprefs)) {
	if (buttonstate[2])
	    v &= 0xFBFF;

	if (buttonstate[1])
	    v &= 0xFEFF;
    } else if (JSEM_ISJOY0 (0, &currprefs) || JSEM_ISJOY1 (0, &currprefs)) {
	if (joy0button & 2) v &= 0xfbff;
	if (joy0button & 4) v &= 0xfeff;
    }

    if (JSEM_ISJOY0 (1, &currprefs) || JSEM_ISJOY1 (1, &currprefs)) {
	if (joy1button & 2) v &= 0xbfff;
	if (joy1button & 4) v &= 0xefff;
    }

    return v;
}

uae_u16 POT0DAT (void)
{
    static uae_u16 cnt = 0;
    if (JSEM_ISMOUSE (0, &currprefs)) {
	if (buttonstate[2])
	    cnt = ((cnt + 1) & 0xFF) | (cnt & 0xFF00);
	if (buttonstate[1])
	    cnt += 0x100;
    }

    return cnt;
}
uae_u16 JOY0DAT (void)
{
    if (JSEM_ISMOUSE (0, &currprefs)) {
	do_mouse_hack ();
	return ((uae_u8)mouse_x) + ((uae_u16)mouse_y << 8);
    }
    return joy0dir;
}
uae_u16 JOY1DAT (void)
{
    if (JSEM_ISMOUSE (1, &currprefs)) {
	do_mouse_hack ();
	return ((uae_u8)mouse_x) + ((uae_u16)mouse_y << 8);
    }
    return joy1dir;
}
void JOYTEST (uae_u16 v)
{
    if (JSEM_ISMOUSE (0, &currprefs)) {
	mouse_x = v & 0xFC;
	mouse_y = (v >> 8) & 0xFC;
    }
}

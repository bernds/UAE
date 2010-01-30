/*
 * UAE - The Un*x Amiga Emulator
 *
 * Additional Win32 helper functions not calling any system routines
 *
 * (c) 1997 Mathias Ortmann
 */

#include "config.h"
#include "sysconfig.h"

#include <assert.h>
#include <ctype.h>
#include <signal.h>
#include <windows.h>

#include "sysdeps.h"
#include "uae.h"
#include "options.h"
#include "threaddep/penguin.h"
#include "include/memory.h"
#include "custom.h"
#include "readcpu.h"
#include "newcpu.h"
#include "keyboard.h"
#include "xwin.h"
#include "disk.h"
#include "keybuf.h"
#include "gui.h"

static int keystate[256];

static int helppressed (void)
{
    return GetAsyncKeyState (VK_END) & 0x8000;
}

static int shiftpressed (void)
{
    return GetAsyncKeyState (VK_SHIFT) & 0x8000 ? 1 : 0;
}

static int getcapslock (void)
{
    BYTE keyState[256];

    GetKeyboardState (keyState);
    return keyState[VK_CAPITAL] & 1;
}


/* there's a problem remaining with the semantics of the '#' key on U.S. keyboards */
static int scancode2amiga (int keycode, int scancode)
{
    switch (keycode & 0xff) {
     case VK_INSERT:
     case VK_LWIN:
	return AK_LAMI;
     case VK_HOME:
     case VK_RWIN:
	return AK_RAMI;
     case VK_DECIMAL:
	return 0x3c;
     case VK_SCROLL:
	return AK_inhibit;
     case VK_F12:
     case VK_APPS:
	return -2;
     case VK_PAUSE:
	return -3;
     case VK_DIVIDE:
	return AK_NPDIV;
     case VK_MULTIPLY:
	return AK_NPMUL;
     case VK_SUBTRACT:
	return AK_NPSUB;
     case VK_ADD:
	return AK_NPADD;
     case VK_NUMPAD0:
	return AK_NP0;
     case VK_NUMPAD1:
	return AK_NP1;
     case VK_NUMPAD2:
	return AK_NP2;
     case VK_NUMPAD3:
	return AK_NP3;
     case VK_NUMPAD4:
	return AK_NP4;
     case VK_NUMPAD5:
	return AK_NP5;
     case VK_NUMPAD6:
	return AK_NP6;
     case VK_NUMPAD7:
	return AK_NP7;
     case VK_NUMPAD8:
	return AK_NP8;
     case VK_NUMPAD9:
	return AK_NP9;
     case VK_UP:
	return AK_UP;
     case VK_DOWN:
	return AK_DN;
     case VK_LEFT:
	return AK_LF;
     case VK_RIGHT:
	return AK_RT;
     case VK_CONTROL:
	return scancode == 0x11d ? AK_RCTRL : AK_CTRL;
     case VK_RETURN:
	return keycode > 0xff ? 0x43 : 0x44;
     case VK_CAPITAL:
	return -4;
     case VK_F1:
	if (helppressed ())
	    return -10 - shiftpressed ();
	return 0x50;
     case VK_F2:
	if (helppressed ())
	    return -12 - shiftpressed ();
	return 0x51;
    case VK_F3:
	if (helppressed ())
	    return -14 - shiftpressed ();
	return 0x52;
    case VK_F4:
	if (helppressed ())
	    return -16 - shiftpressed ();
	return 0x53;
     case VK_F5:
	return 0x54;
     case VK_F6:
	return 0x55;
     case VK_F7:
	return 0x56;
     case VK_F8:
	return 0x57;
     case VK_F9:
	return 0x58;
     case VK_F10:
	return 0x59;
     case VK_NEXT:
	return 0x5f;
    }

    switch (scancode) {
     case 0x01:
	return 0x45;
     case 0x29:
	return 0x00;
     case 0x02:
	return 0x01;
     case 0x03:
	return 0x02;
     case 0x04:
	return 0x03;
     case 0x05:
	return 0x04;
     case 0x06:
	return 0x05;
     case 0x07:
	return 0x06;
     case 0x08:
	return 0x07;
     case 0x09:
	return 0x08;
     case 0x0a:
	return 0x09;
     case 0x0b:
	return 0x0a;
     case 0x0c:
	return 0x0b;
     case 0x0d:
	return 0x0c;
     case 0x57:
	return 0x0d;
     case 0x0e:
	return 0x41;
     case 0x0f:
	return 0x42;
     case 0x10:
	return 0x10;
     case 0x11:
	return 0x11;
     case 0x12:
	return 0x12;
     case 0x13:
	return 0x13;
     case 0x14:
	return 0x14;
     case 0x15:
	return 0x15;
     case 0x16:
	return 0x16;
     case 0x17:
	return 0x17;
     case 0x18:
	return 0x18;
     case 0x19:
	return 0x19;
     case 0x1a:
	return 0x1a;
     case 0x1b:
	return 0x1b;
	/* special handling for caps lock: return 0x63*/
     case 0x3a:
	return 0x62;
     case 0x1e:
	return 0x20;
     case 0x1f:
	return 0x21;
     case 0x20:
	return 0x22;
     case 0x21:
	return 0x23;
     case 0x22:
	return 0x24;
     case 0x23:
	return 0x25;
     case 0x24:
	return 0x26;
     case 0x25:
	return 0x27;
     case 0x26:
	return 0x28;
     case 0x27:
	return 0x29;
     case 0x28:
	return 0x2a;
     case 0x2b:
	return 0x2b;
     case 0x2a:
	return 0x60;
     case 0x56:
	return 0x30;
     case 0x2c:
	return 0x31;
     case 0x2d:
	return 0x32;
     case 0x2e:
	return 0x33;
     case 0x2f:
	return 0x34;
     case 0x30:
	return 0x35;
     case 0x31:
	return 0x36;
     case 0x32:
	return 0x37;
     case 0x33:
	return 0x38;
     case 0x34:
	return 0x39;
     case 0x35:
	return 0x3a;
     case 0x36:
	return 0x61;
     case 0x38:
	return 0x64;
     case 0x39:
	return 0x40;
     case 0x153:
	return 0x46;
     case 0x51:
	return 0x5f;
     case 0x52:
	return 0x66;
     case 0x47:
	return 0x67;
     case 0x4b:
	return 0x4f;
     case 0x50:
	return 0x4d;
     case 0x4d:
	return 0x4e;
     case 0x48:
	return 0x4c;
     case 0x4e:
	return 0x5e;
     case 0x4a:
	return 0x4a;
     case 0x37:
	return 0x5d;
    }

    return -1;
}

int my_kbd_handler (int keycode, int scancode, int newstate)
{
    int akey = scancode2amiga (keycode, scancode);

    if (akey < 0) {
	switch (akey) {
	 case -2:
	    if (newstate)
		DisplayGUI ();

	    return 0;

	 case -3:
#if 0
	    if (newstate)
		toggle_sound = 1;
#endif
	    return 0;
	 case -4:
	    if (newstate) {
		akey = 0x62;
		newstate = getcapslock ();
	    }
	    break;
	 default:
	    if (newstate && akey >= -17 && akey <= -10) {
		char tmpbuf[80];
		akey = -10 - akey;

		if (akey & 1) {
		    akey >>= 1;
		    disk_eject (akey);
		    return 0;
		}
		akey >>= 1;

		sprintf (tmpbuf, "Insert disk into drive df%d:...", akey);

		requestfname (tmpbuf, changed_prefs.df[akey]);

	    }
	    return 0;
	}
    }
    if (akey == AK_inhibit) {
	if (newstate)
	    inhibit_frame ^= 1;
	return 0;
    }
    if (keystate[akey] == newstate)
	return 0;
    keystate[akey] = newstate;

    if (newstate)
	record_key (akey << 1);
    else
	record_key ((akey << 1) + 1);

    /* "Affengriff" */
    if (keystate[AK_CTRL] && keystate[AK_LAMI] && keystate[AK_RAMI])
	uae_reset ();

    return 0;
}


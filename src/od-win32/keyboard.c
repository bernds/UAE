/*
 * UAE - The Un*x Amiga Emulator
 *
 * Additional Win32 helper functions not calling any system routines
 *
 * (c) 1997 Mathias Ortmann
 * (c) 1999-2001 Brian King
 * (c) 2000-2001 Bernd Roesch
 */

#include "sysconfig.h"

#include <assert.h>
#include <ctype.h>
#include <signal.h>
#include <windows.h>

#include "sysdeps.h"
#include "uae.h"
#include "options.h"
#include "memory.h"
#include "custom.h"
#include "newcpu.h"
#include "keyboard.h"
#include "xwin.h"
#include "drawing.h"
#include "disk.h"
#include "keybuf.h"
#include "gui.h"
#include "osdep/win32.h"
#include "osdep/win32gui.h"
#include "debug.h"

//#define DBG_KEYBD 1
static int keystate[AK_RCTRL+1];
static HKL keyboardlayoutid;
static WORD keyboardlangid;

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
#ifdef _WIN32_WCE
    return( GetKeyState( VK_CAPITAL ) & 1 );
#else
    GetKeyboardState (keyState);
    return keyState[VK_CAPITAL] & 1;
#endif
}

static char getascii( UINT vk, UINT scancode )
{
    BYTE keyState[256];
    char buffer[2];
    int result;
#ifndef _WIN32_WCE
    GetKeyboardState (keyState);
    result = ToAscii( vk, scancode, keyState, (LPWORD)buffer, 0 );
    if( result == 1 )
	result = buffer[0];
#endif
    return result;
}

/* there's a problem remaining with the semantics of the '#' key on U.S. keyboards */
static int scancode2amiga (int keycode, int scancode)
{
	static int firstscan=0;
	if (!firstscan)
	{
	    keyboardlayoutid=GetKeyboardLayout(0);
	    keyboardlangid=LOWORD(keyboardlayoutid);
	    firstscan=1;
	}
#if DEBUG_KBD
    write_log ( "keycode = 0x%x, scancode = 0x%x\n", keycode, scancode ); 
#endif

    if(keyboardlangid==MAKELANGID(LANG_GERMAN, SUBLANG_GERMAN))
    {
	if (keystate[AK_CTRL] &&
	    keystate[AK_RALT])
	{
	    switch (keycode & 0xff) 
	    {
	    case 0xdb:
		return AK_BACKSLASH;
	    case 0x51:          //@	     
		record_key ((AK_CTRL << 1)+1);
		return 0x10;
	    case 0xbb:          //~
		record_key ((AK_CTRL << 1)+1);
		return 0x1b;
		
	    }
	}
    }

#define USE_OLD_SHIFT_CODE

    switch (keycode & 0xff) 
    {
#ifndef USE_OLD_SHIFT_CODE
    case VK_SHIFT:
	{
	    SHORT lsh, rsh;
	    lsh = GetKeyState( VK_LSHIFT );
	    rsh = GetKeyState( VK_RSHIFT );
	    if( !keystate[AK_RSH] && (rsh & 0x8000))
		return AK_RSH;
	    if( keystate[AK_RSH] && !(rsh & 0x8000))
		return AK_RSH;
	    if( !keystate[AK_LSH] && (lsh & 0x8000))
		return AK_LSH;
	    if( keystate[AK_LSH] && !(lsh & 0x8000))
		return AK_LSH;
	}
#endif
     case VK_INSERT:
     case VK_LWIN:
	return AK_LAMI;
     case VK_HOME:
     case VK_RWIN:
	return AK_RAMI;
     case VK_MENU:
	 return scancode & KF_EXTENDED ? AK_RALT:AK_LALT;
     case VK_APPS: /* Map Windows "Menu" key to Right-Alt of Amiga, for Stefan Stuntz */
	 return AK_RALT;
     case VK_SCROLL:
	return AK_inhibit;
     case VK_F11:
#if 0
	 if( currprefs.win32_ctrl_F11_is_quit )
	 {
	    if (GetAsyncKeyState (VK_CONTROL)&0x8000)
		uae_quit();
	 }
#endif
	 return AK_BACKSLASH;
     case VK_F12:
	 if (GetAsyncKeyState (VK_CONTROL)&0x8000)
	     return -6;
	 if( shiftpressed() )
	     return -5;
	 else
	     return -2;
	 break;
     case VK_PAUSE:
	return -3;
     case VK_DECIMAL:
	return AK_NPDEL;
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
	return scancode & KF_EXTENDED ? AK_RCTRL : AK_CTRL;
     case VK_RETURN:
	return scancode & KF_EXTENDED ? AK_ENT : AK_RET;
     case VK_CAPITAL:
	return -4;
     case VK_F1:
	if (helppressed ())
	    return -10 - shiftpressed ();
	return AK_F1;
     case VK_F2:
	if (helppressed ())
	    return -12 - shiftpressed ();
	return AK_F2;
    case VK_F3:
	if (helppressed ())
	    return -14 - shiftpressed ();
	return AK_F3;
    case VK_F4:
	if (helppressed ())
	    return -16 - shiftpressed ();
	return AK_F4;
     case VK_F5:
	return AK_F5;
     case VK_F6:
	return AK_F6;
     case VK_F7:
	return AK_F7;
     case VK_F8:
	return AK_F8;
     case VK_F9:
	return AK_F9;
     case VK_F10:
	return AK_F10;
     case VK_NEXT:
	return AK_HELP;
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
     case 0x57: /* This is F11 */
	return AK_BACKSLASH;
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
	return AK_SEMICOLON;
     case 0x28:
	return AK_QUOTE;
     case 0x2b: /* This scancode is \ on US keyboards, but # on German ones - figure out which! */
	 if( getascii( keycode & 0xFF, scancode ) == '#' )
	     return AK_NUMBERSIGN;
	 else
	     return AK_BACKSLASH;
	 break;
#ifdef USE_OLD_SHIFT_CODE
     case 0x2a:
	return AK_LSH;
     case 0x36:
	return AK_RSH;
#endif
     case 0x56:
	return AK_LTGT;
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
     case 0x38:
	return 0x64;
     case 0x39:
	return 0x40;
     case 0x153:
	return 0x46;
     case 0x51:
	return 0x5f;
     case 0x52:
	return AK_LAMI;
     case 0x47:
	return AK_RAMI;
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

void clearallkeys()
{
    int i;
    for(i = 0; i <AK_CTRL + 1; i++)
	my_kbd_handler (0, i, 0);
    for(i = 0; i < AK_CTRL + 1; i++)
	my_kbd_handler (i, 0, 0);
    buttonstate[0] = 0;
    buttonstate[1] = 0;
    buttonstate[2] = 0;
}

int my_kbd_handler (int keycode, int scancode, int newstate)
{
    int akey = scancode2amiga (keycode, scancode);

#ifdef DEBUG_KBD
    write_log ( "kbd_handler: keycode 0x%x, scancode 0x%x, newstate %d\n", keycode, scancode, newstate );
#endif
    if (akey < 0) {
	
	switch (akey) {
#ifndef _WIN32_WCE
	 case -2:
	    if (newstate)
		{
		clearallkeys();
		WIN32GUI_DisplayGUI(-1);
		}
	    return 0;

	 case -3:
	    if (newstate)
		toggle_sound = 1;
	    return 0;
	 case -4:
	    if (newstate) {
		akey = 0x62;
		newstate = getcapslock ();
	    }
	    break;
     case -5:
	 if( newstate )
	 {
	    // Bring up GUI-based debugger here
	  if(currprefs.illegal_mem)
		   {   
	
		   
	     if(picasso_on)changed_prefs.gfx_pfullscreen=0;
		 else changed_prefs.gfx_afullscreen=0;

		 check_prefs_changed_gfx();
	 activate_debugger();
		 
	   
	 }
	    
		 }
	 return 0;
	 break;
	 case -6:
		 if(newstate)
		 {
		 if(picasso_on)changed_prefs.gfx_pfullscreen=!changed_prefs.gfx_pfullscreen;
		 else changed_prefs.gfx_afullscreen=!changed_prefs.gfx_afullscreen;

		 return 0;
		 }
		 break;
	 default:

	     if (newstate && akey >= -17 && akey <= -10) 
	     {
		 akey = -10 - akey;
		 
		 if (akey & 1) 
		 {
		     akey >>= 1;
		     disk_eject (akey);
		     return 0;
		 }
		 akey >>= 1;
		 
		 WIN32GUI_DisplayGUI( akey );
	     }
	     return 0;
#endif
	}
    }
    if (akey == AK_inhibit) {
	if (newstate)
	    toggle_inhibit_frame (IHF_SCROLLLOCK);
	return 0;
    }
    if( akey > AK_RCTRL )
	return 0;
    if (keystate[akey] == newstate)
	return 0;
    keystate[akey] = newstate;

    if (newstate)
	record_key (akey << 1);
    else
	record_key ((akey << 1) + 1);

    /* "Affengriff" */
	if(  (keystate[AK_CTRL] || keystate[AK_RCTRL] ) && keystate[AK_LAMI] && keystate[AK_RAMI])uae_reset();
    //if(  (keystate[AK_CTRL] || keystate[AK_RCTRL] ) && keystate[AK_RAMI])uae_reset();

    return 0;
}


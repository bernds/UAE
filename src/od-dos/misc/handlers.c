 /*
  * UAE - The Un*x Amiga Emulator
  *
  * DOS handlers.
  *
  * (c) 1997 Gustavo Goedert
  *
  * Led routines by Michael Sontheimer
  */

#include "sysconfig.h"
#include "sysdeps.h"

#include <dos.h>
#include <go32.h>
#include <dpmi.h>
#include <sys/farptr.h>

#include "config.h"
#include "options.h"
#include "threaddep/thread.h"
#include "memory.h"
#include "custom.h"
#include "readcpu.h"
#include "newcpu.h"
#include "keyboard.h"
#include "xwin.h"
#include "keybuf.h"
#include "disk.h"
#include "gui.h"
#include "tui.h"
#include "uae.h"
#include "picasso96.h"
#include "misc/handlers.h"
#include "misc/misc.h"

/* dummie Amiga Keys */
#define AK_Options	0x1fc
#define AK_SaveScreen1  0x1fd
#define AK_SaveScreen2  0x1fe
#define AK_TougleScreen 0x1ff

/* Event Flags */
#define EF_ResetCPU       0x01
#define EF_ChangeOptions  0x02
#define EF_EnterDebug     0x04
#define EF_SaveScreen     0x08
#define EF_TougleScreen   0x10
#define EF_TougleSound    0x20
#define EF_TougleCapsLock 0x40
#define EF_QuitProgram    0x80

extern int kpb_first, kpb_last;
extern int keybuf[256];

int Original_produce_sound;
int KeyBoardInstalled = 0, MouseInstalled = 0;
int EventFlags = 0;
int KeyState[512];
int buttonstate[3] = {0, 0, 0};
int lastmx, lastmy;
int newmousecounters = 0;
_go32_dpmi_seginfo OldKeyboardHandler, NewKeyboardHandler, NewMouseHandler;
_go32_dpmi_registers MouseCallbackRegs;
volatile int NumLockState = -1, CapsLockState = -1, ScrollLockState = -1;
UBYTE SavedLEDa;

int AmigaKeys[128] = {
    -1,             AK_ESC,        AK_1,           AK_2,           AK_3,      AK_4,     AK_5,            AK_6,
    AK_7,           AK_8,          AK_9,           AK_0,           AK_MINUS,  AK_EQUAL, AK_BS,           AK_TAB,
    AK_Q,           AK_W,          AK_E,           AK_R,           AK_T,      AK_Y,     AK_U,            AK_I,
    AK_O,           AK_P,          AK_LBRACKET,    AK_RBRACKET,    AK_RET,    AK_CTRL,  AK_A,            AK_S,
    AK_D,           AK_F,          AK_G,           AK_H,           AK_J,      AK_K,     AK_L,            AK_SEMICOLON,
    AK_QUOTE,       AK_NUMBERSIGN, AK_LSH,         AK_LTGT,        AK_Z,      AK_X,     AK_C,            AK_V,
    AK_B,           AK_N,          AK_M,           AK_COMMA,       AK_PERIOD, AK_SLASH, AK_RSH,          -1,
    AK_LALT,        AK_SPC,        AK_CAPSLOCK,    AK_F1,          AK_F2,     AK_F3,    AK_F4,           AK_F5,
    AK_F6,          AK_F7,         AK_F8,          AK_F9,          AK_F10,    -1,       AK_TougleScreen, AK_NP7,
    AK_NP8,         AK_NP9,        -1,             AK_NP4,         AK_NP5,    AK_NP6,   -1,              AK_NP1,
    AK_NP2,         AK_NP3,        AK_NP0,         -1,             -1,        -1,       -1,              AK_BACKSLASH,
    -1, 	    -1,            -1,             -1,             -1,        -1,       -1,              -1,
    -1,             AK_RCTRL,      AK_SaveScreen1, AK_SaveScreen2, AK_RALT,   -1,       AK_Options,	 AK_UP,
    AK_RAMI,        AK_LF,         AK_RT,          -1,   	   AK_DN,     AK_LAMI,  AK_HELP,         AK_DEL,
    -1,             -1,            -1,             -1,             -1,        -1,       -1,              -1,
    -1,             -1,            -1,             -1,             -1,        AK_LAMI,  AK_RAMI,         -1
};

unsigned char EscapeKeys[128] = {
    0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   96,  97,  0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   98,  0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   99,
    100, 0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   102,
    103, 104, 0,   105, 0,   106, 0,   107,
    108, 109, 110, 111, 0,   0,   0,   0,
    0,   0,   0,   125, 126, 127, 0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,
};

int InstallHandlers(void) {
    if (!KeyBoardInstalled)
	if (!InstallKeyboardHandler())
	    return(0);
    if (!MouseInstalled)
	if (!InstallMouseHandler())
	    return(0);
    SavedLEDa = InitLED();
    return(1);
}

void UninstallHandlers(void) {
    if (KeyBoardInstalled)
	UninstallKeyboardHandler();
    if (MouseInstalled)
	UninstallMouseHandler();
    RestoreLEDStatus(SavedLEDa);
}

void handle_events(void) {
    if (EventFlags) {
	if (EventFlags & EF_ResetCPU) {
	    m68k_reset();
	    EventFlags &= ~EF_ResetCPU;
	}
	if (EventFlags & EF_ChangeOptions) {
	    HandleOptions();
	    KeyState[AK_Options] = 0;
	    KeyState[AK_F1] = 0;
	    record_key ((AK_F1 << 1) | 1);
	    EventFlags &= ~EF_ChangeOptions;
	}
	if (EventFlags & EF_EnterDebug) {
	    EnterDebug();
	    EventFlags &= ~EF_EnterDebug;
	}
	if (EventFlags & EF_SaveScreen) {
	    SaveScreen();
	    EventFlags &= ~EF_SaveScreen;
	}
	if (EventFlags & EF_TougleScreen) {
	    inhibit_frame ^= 1;
	    EventFlags &= ~EF_TougleScreen;
	}
	if (EventFlags & EF_TougleSound) {
	    if (currprefs.produce_sound == 0)
		currprefs.produce_sound = Original_produce_sound;
	    else
		currprefs.produce_sound = 0;
	    EventFlags &= ~EF_TougleSound;
	}
	if (EventFlags & EF_TougleCapsLock) {
	    CapsLockLED(!CapsLockState);
	    EventFlags &= ~EF_TougleCapsLock;
	}
	if (EventFlags & EF_QuitProgram) {
	    regs.spcflags |= SPCFLAG_BRK;
	    quit_program = 1;
	    EventFlags &= ~EF_QuitProgram;
	}
    }
#ifdef PICASSO96
    PicassoRefresh();
#endif
}

static void KeyboardHandler(void) {
    static int IsEscape = 0, CapsLockCanChange = 1;
    int ScanCode, NewState, AmigaKey;

    ScanCode = inportb(0x60);

    if (ScanCode == 0xe0) {
	IsEscape = 1;
	outportb(0x20, 0x20);
	return;
    }

    /* PAUSE Key */
    if (IsEscape > 1) {
	IsEscape++;
	if (IsEscape == 7) {
	    EventFlags |= EF_TougleSound;
	    IsEscape = 0;
	}
	outportb(0x20, 0x20);
	return;
    }
    if (ScanCode == 0xe1) {
	IsEscape = 2;
	outportb(0x20, 0x20);
	return;
    }

    NewState = !(ScanCode & 0x80);
    ScanCode = ScanCode & 0x7f;
    if (IsEscape) {
	ScanCode = EscapeKeys[ScanCode];
	IsEscape = 0;
    }
    outportb(0x20, 0x20);

    AmigaKey = AmigaKeys[ScanCode];

    if (AmigaKey == -1)
	return;

    if (AmigaKey == AK_CAPSLOCK) {
	if (NewState) {
	    if (CapsLockCanChange) {
		NewState = !KeyState[AmigaKey];
		CapsLockCanChange = 0;
	    } else
	       return;
	} else {
	    CapsLockCanChange = 1;
	    return;
	}
	EventFlags |= EF_TougleCapsLock;
    }

    if (KeyState[AmigaKey] == NewState)
	return;
    KeyState[AmigaKey] = NewState;

    if (AmigaKey < AK_Options) {
	if (NewState)
	    record_key (AmigaKey << 1);
	else
	    record_key ((AmigaKey << 1) | 1);
    }

    if ((KeyState[AK_CTRL] && KeyState[AK_LAMI] && KeyState[AK_RAMI]) ||
	(KeyState[AK_RCTRL] && KeyState[AK_LAMI] && KeyState[AK_RAMI]))
	EventFlags |= EF_ResetCPU;
    if (KeyState[AK_Options]) {
	if (KeyState[AK_ESC])
	    EventFlags |= EF_QuitProgram;
	if (KeyState[AK_F1])
	    EventFlags |= EF_ChangeOptions;
	if (KeyState[AK_F2])
	    EventFlags |= EF_EnterDebug;
    }
    if (KeyState[AK_TougleScreen])
	EventFlags |= EF_TougleScreen;
    if (KeyState[AK_SaveScreen1] && KeyState[AK_SaveScreen2])
	EventFlags |= EF_SaveScreen;
}

int InstallKeyboardHandler(void) {
    int i;

    if (KeyBoardInstalled)
	return(1);
    _go32_dpmi_get_protected_mode_interrupt_vector(9, &OldKeyboardHandler);
    NewKeyboardHandler.pm_offset = (int)KeyboardHandler;
    if (_go32_dpmi_allocate_iret_wrapper(&NewKeyboardHandler) != 0) {
	printf ("Can't allocate keyboard iret_wrapper.\n");
	return(0);
    }
    if (_go32_dpmi_set_protected_mode_interrupt_vector(9, &NewKeyboardHandler) != 0) {
	printf ("Can't set protected mode interrupt vector.\n");
	return(0);
    }
    for(i = 0; i < 256; i++)
	KeyState[i] = 0;
    _go32_dpmi_lock_code(KeyboardHandler, (unsigned int)InstallKeyboardHandler-(unsigned int)KeyboardHandler);
    _go32_dpmi_lock_data(AmigaKeys, sizeof(AmigaKeys));
    _go32_dpmi_lock_data(EscapeKeys, sizeof(EscapeKeys));
    _go32_dpmi_lock_data(KeyState, sizeof(KeyState));
    _go32_dpmi_lock_data(&EventFlags, sizeof(EventFlags));
    _go32_dpmi_lock_data(&OldKeyboardHandler, sizeof(OldKeyboardHandler));
    _go32_dpmi_lock_data(&NewKeyboardHandler, sizeof(NewKeyboardHandler));
    _go32_dpmi_lock_code(record_key, (unsigned int)keybuf_init-(unsigned int)record_key);
    _go32_dpmi_lock_data(&kpb_first, sizeof(kpb_first));
    _go32_dpmi_lock_data(&kpb_last, sizeof(kpb_last));
    _go32_dpmi_lock_data(keybuf, sizeof(keybuf));
    KeyBoardInstalled = 1;
    return(1);
}

void UninstallKeyboardHandler(void) {
    if (!KeyBoardInstalled)
	return;
    _go32_dpmi_set_protected_mode_interrupt_vector(9, &OldKeyboardHandler);
    _go32_dpmi_free_iret_wrapper(&NewKeyboardHandler);
    KeyBoardInstalled = 0;
}

static void MouseHandler(_go32_dpmi_registers *MouseRegs) {
    lastmx = (short)MouseRegs->x.si;
    lastmy = (short)MouseRegs->x.di;
    buttonstate[0] = MouseRegs->x.bx & 1;
    buttonstate[1] = MouseRegs->x.bx & 4;
    buttonstate[2] = MouseRegs->x.bx & 2;
}

int InstallMouseHandler(void) {
    _go32_dpmi_registers IntRegs;

    if (MouseInstalled)
	return(1);
    IntRegs.x.ax = 0;
    IntRegs.x.ss = IntRegs.x.sp = IntRegs.x.flags = 0;
    _go32_dpmi_simulate_int(0x33, &IntRegs);
    if (IntRegs.x.ax==0) {
	printf ("Mouse driver not present.\n");
	return(0);
    }
    NewMouseHandler.pm_offset = (int)MouseHandler;
    if (_go32_dpmi_allocate_real_mode_callback_retf(&NewMouseHandler, &MouseCallbackRegs) != 0) {
	printf ("Can't allocate mouse callback_retf.\n");
	return(0);
    }
    IntRegs.x.ax = 0xc;
    IntRegs.x.cx = 0x7f;
    IntRegs.x.es = NewMouseHandler.rm_segment;
    IntRegs.x.dx = NewMouseHandler.rm_offset;
    IntRegs.x.ss = IntRegs.x.sp = IntRegs.x.flags = 0;
    buttonstate[0] = buttonstate[1] = buttonstate[2] = 0;
    lastmx = lastmy = 0;
    newmousecounters = 0;
    _go32_dpmi_simulate_int(0x33, &IntRegs);
    _go32_dpmi_lock_code(MouseHandler, (unsigned int)InstallMouseHandler-(unsigned int)MouseHandler);
    _go32_dpmi_lock_data(&lastmx, sizeof(lastmx));
    _go32_dpmi_lock_data(&lastmy, sizeof(lastmy));
    _go32_dpmi_lock_data(buttonstate, sizeof(buttonstate));
    _go32_dpmi_lock_data(&NewMouseHandler, sizeof(NewMouseHandler));
    _go32_dpmi_lock_data(&MouseCallbackRegs, sizeof(MouseCallbackRegs));
    MouseInstalled = 1;
    return(1);
}

void UninstallMouseHandler(void) {
    _go32_dpmi_registers IntRegs;

    if (!MouseInstalled)
	return;
    IntRegs.x.ax = 0xc;
    IntRegs.x.cx = 0x0;
    IntRegs.x.ss = IntRegs.x.sp = IntRegs.x.flags = 0;
    _go32_dpmi_simulate_int(0x33, &IntRegs);
    _go32_dpmi_free_real_mode_callback(&NewMouseHandler);
    MouseInstalled = 0;
}


void LED(int on) {

    ScrollLockLED(!on);

}

void NumLockLED(int on) {

    _go32_dpmi_registers regs;

    if( NumLockState != on )     // Avoid more int's than necessary
    {
	if( on )
	    _farnspokeb( (0x40<<4)+0x17, _farnspeekb( (0x40<<4)+0x17 ) | 32 );
	else
	    _farnspokeb( (0x40<<4)+0x17, _farnspeekb( (0x40<<4)+0x17 ) & ~32 );

	regs.x.ax = 0x0100;
	regs.x.ss=regs.x.sp=regs.x.flags=0;
	_go32_dpmi_simulate_int(0x16, &regs);

	NumLockState = on;
    }

}

void CapsLockLED(int on) {

    _go32_dpmi_registers regs;

    if( CapsLockState != on )     // Avoid more int's than necessary
    {
	if( on )
	    _farnspokeb( (0x40<<4)+0x17, _farnspeekb( (0x40<<4)+0x17 ) | 64 );
	else
	    _farnspokeb( (0x40<<4)+0x17, _farnspeekb( (0x40<<4)+0x17 ) & ~64 );

	regs.x.ax = 0x0100;
	regs.x.ss=regs.x.sp=regs.x.flags=0;
	_go32_dpmi_simulate_int(0x16, &regs);

	CapsLockState = on;
    }

}

void ScrollLockLED(int on) {

    _go32_dpmi_registers regs;

    if( ScrollLockState != on )     // Avoid more int's than necessary
    {
	if( on )
	    _farnspokeb( (0x40<<4)+0x17, _farnspeekb( (0x40<<4)+0x17 ) | 16 );
	else
	    _farnspokeb( (0x40<<4)+0x17, _farnspeekb( (0x40<<4)+0x17 ) & ~16 );

	regs.x.ax = 0x0100;
	regs.x.ss=regs.x.sp=regs.x.flags=0;
	_go32_dpmi_simulate_int(0x16, &regs);

	ScrollLockState = on;
    }

}

UBYTE InitLED(void) {

    UBYTE ubSavedLED;

    ubSavedLED = _farnspeekb( (0x40<<4)+0x17 );    // Save LED-status

    NumLockLED(0);
    CapsLockLED(0);
    ScrollLockLED(0);

    return(ubSavedLED);

}

void RestoreLEDStatus(UBYTE ubSavedLED) {

    _go32_dpmi_registers regs;

    _farnspokeb( (0x40<<4)+0x17, ubSavedLED );

    regs.x.ax = 0x0100;
    regs.x.ss=regs.x.sp=regs.x.flags=0;
    _go32_dpmi_simulate_int(0x16, &regs);

}

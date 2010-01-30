/* 
  * UAE - The Un*x Amiga Emulator
  * 
  * Win32-specific header file
  * 
  * (c) 1997-1999 Mathias Ortmann
  * (c) 1998-2001 Brian King
  */

#ifndef __WIN32_H__
#define __WIN32_H__

#define IHF_WINDOWHIDDEN 6
#define NORMAL_WINDOW_STYLE (WS_VISIBLE | WS_BORDER | WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU )

extern HMODULE hUIDLL;
extern HWND hAmigaWnd, hMainWnd;
extern RECT amigawin_rect;
extern int in_sizemove;
extern int manual_painting_needed;
extern int mouseactive;
#define WINUAERELEASE 3
#define WINUAEBETA 0
#define WINUAEBETASTR " Beta 4"

extern int my_kbd_handler (int, int, int);

void releasecapture (void);
int WIN32_RegisterClasses( void );
int WIN32_InitHtmlHelp( void );
int WIN32_InitLibraries( void );
int WIN32_CleanupLibraries( void );
void WIN32_MouseDefaults( void );
void WIN32_HandleRegistryStuff( void );
extern int currtime (void);
extern void setup_brkhandler (void);
extern void remove_brkhandler (void);

extern int checkkey (int vkey, long lParam);
extern void setmouseactive (int active);
extern uae_u32 OSDEP_minimize_uae(void);
extern int gunzip_hack (const char *src, const char *dst);

// globals
extern int bActive;
//extern int draw_all;

//extern int vpos;
//extern unsigned short lof;
extern int capslock;

extern int process_desired_pri;

extern int toggle_sound, bytesinbuf;

#define USE_ZFILE

void finishjob (void);
void DoSomeWeirdPrintingStuff (WPARAM wParam);

extern int sound_available;
extern int framecnt;
extern char prtname[];
extern char VersionStr[256];

struct win32_displaymode {
    struct win32_displaymode *next;
    uae_u32 width;
    uae_u32 height;
    uae_u32 refreshrate;
    /* Bit mask of RGBFF_xxx values.  */
    uae_u32 colormodes;
};

extern struct win32_displaymode *win32_displaymode_list;

/* For StatusBar when running in a Window */
#define NUM_PARTS 7
#define LED_WIDTH 50
#define DRIVE_WIDTH 24
#define FPS_WIDTH 60

extern HKEY hWinUAEKey;

extern int screen_is_picasso;

extern HINSTANCE hInst;

#ifndef _WIN32_WCE
#include "osdep/win32gui.h"
#include "osdep/resource.h"
#endif

#endif
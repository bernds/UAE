/*
 * UAE - The Un*x Amiga Emulator
 *
 * Win32 interface
 *
 * Copyright 1997-1998 Mathias Ortmann
 * Copyright 1997-1999 Brian King
 */

/* Uncomment this line if you want the logs time-stamped */
/* #define TIMESTAMP_LOGS */

#include "sysconfig.h"

#include <stdlib.h>
#include <stdarg.h>
#include <signal.h>

#include <windows.h>
#include <commctrl.h>
#include <commdlg.h>
#include <shellapi.h>
#include <zmouse.h>
#include <ddraw.h>
#include <dbt.h>
#include <math.h>

#include "sysdeps.h"
#include "options.h"
#include "gensound.h"
#include "sounddep/sound.h"
#include "uae.h"
#include "memory.h"
#include "custom.h"
#include "events.h"
#include "xwin.h"
#include "keyboard.h"
#include "keybuf.h"
#include "drawing.h"
#include "picasso96.h"
#include "bsdsocket.h"
#include "osdep/win32.h"
#include "osdep/win32gfx.h"
#include "osdep/win32gui.h"
#include "osdep/dxwrap.h"
#include "autoconf.h"
#include "gui.h"
#include "newcpu.h"
extern void WIN32GFX_WindowMove ( void );
extern void WIN32GFX_WindowSize ( void );
unsigned long *win32_stackbase; 
unsigned long *win32_freestack[42]; //EXTRA_STACK_SIZE

/* Comment out the following line if you don't want ZLIB.DLL support */
#undef USE_ZLIB_DLL

#ifdef USE_ZLIB_DLL
#include "zlib.h"
#endif

int useqpc = 0; /* Set to TRUE to use the QueryPerformanceCounter() function instead of rdtsc() */

static FILE *debugfile = NULL;

HINSTANCE hInst = NULL;
HMODULE hUIDLL = NULL;

HWND (WINAPI *pHtmlHelp)(HWND, LPCSTR, UINT, LPDWORD ) = NULL;

HWND hAmigaWnd, hMainWnd;
/*DWORD Keys; */
RECT amigawin_rect;

char VersionStr[256];

int in_sizemove = 0;
int manual_painting_needed = 0;
int customsize = 0;

int bActive;
int toggle_sound;

BOOL viewing_child = FALSE;

HKEY hWinUAEKey    = NULL;
COLORREF g_dwBackgroundColor  = RGB(10, 0, 10);

/* Keyboard emulation, Win32 helper routines. */
static LPARAM keysdown[256];
static short numkeysdown;
int checkkey (int vkey, LPARAM lParam)
{
    switch (vkey) {
    case VK_LWIN:
    case VK_RWIN:
     case VK_SHIFT:
     case VK_LSHIFT:
     case VK_RSHIFT:
     case VK_CONTROL:
     case VK_LCONTROL:
     case VK_RCONTROL:
     case VK_MENU:
     case VK_LMENU:
     case VK_RMENU:
	return GetKeyState (vkey) & 0x8000;
    }
    return GetAsyncKeyState (vkey) & 0x8000;
}

/* Mouse emulation, Win32 interface */
static int mousecx = 160, mousecy = 100, mousedx = 160, mousedy = 100;
static int mousecl = MAKELONG (160, 100);
int mouseactive;

void WIN32_MouseDefaults( void )
{
    mousecx = 160, mousecy = 100, mousedx = 160, mousedy = 100, mousecl = MAKELONG (160, 100);
}

void setmouseactive (int active)
{
    mousedx = (amigawin_rect.right - amigawin_rect.left) / 2;
    mousedy = (amigawin_rect.bottom - amigawin_rect.top) / 2;
    mousecl = MAKELONG (mousedx, mousedy);
    mousecx = amigawin_rect.left + mousedx;
    mousecy = amigawin_rect.top + mousedy;

    if (active == mouseactive)
	return;
    mouseactive = active;

    if (active) 
    {
#ifdef HARDWARE_SPRITE_EMULATION
	if( !WIN32GFX_IsPicassoScreen() )
#endif
	{
	    ShowCursor (FALSE);
	    SetCursorPos (mousecx, mousecy);
	}
	SetWindowText (hMainWnd ? hMainWnd : hAmigaWnd, "UAE/Win32 - [Mouse active - press Alt-Tab to cancel]");
	ClipCursor (&amigawin_rect);
    }
    else
    {
	ShowCursor (TRUE);
	SetWindowText (hMainWnd ? hMainWnd : hAmigaWnd, "UAE/Win32" );
	ClipCursor (NULL);
    }
}

static int hascapture = 0;

static void setcapture (void)
{
    if (hascapture)
	return;
    hascapture++;
    SetCapture (hAmigaWnd);
}

void releasecapture (void)
{
    if (!hascapture)
	return;
    hascapture--;
    ReleaseCapture ();
}

frame_time_t read_processor_time_cyrix (void)
{
    LARGE_INTEGER counter;
    QueryPerformanceCounter( &counter );
    return (frame_time_t)(counter.LowPart);
}

#include <setjmp.h>
jmp_buf catch_test;

static RETSIGTYPE illhandler(int foo)
{
    rpt_available = 0;
    longjmp(catch_test,1);
}

int have_rdtsc (void)
{
    rpt_available = 1;
    write_log ("Testing the RDTSC instruction ... ");
    signal (SIGILL, illhandler);
    if (setjmp (catch_test) == 0)
	read_processor_time ();
    signal (SIGILL, SIG_DFL);
    write_log ("done.\n");
    if (! rpt_available) {
	write_log ("Your processor does not support the RDTSC instruction.\n");
	return 0;
    }
    return 1;
}

static void figure_processor_speed (void)
{
    extern volatile frame_time_t vsynctime;
    extern unsigned long syncbase;
    frame_time_t clockrate;

#if defined __GNUC__
    if (! have_rdtsc ())
	useqpc = 1;
#else
    LARGE_INTEGER freq;

    __try
    {
	__asm{rdtsc};
    }
    __except( GetExceptionCode() == EXCEPTION_ILLEGAL_INSTRUCTION )
    {
	useqpc = 1;
    }
    if( QueryPerformanceFrequency( &freq ) )
    {
	if (freq.LowPart > 90000000) /* looks like CPU freq. */
	    write_log ( "CLOCKFREQ: QueryPerformanceFrequency() reports %d-MHz\n", freq.LowPart / 1000000 ); 
	else
	    write_log ( "CLOCKFREQ: QueryPerformanceFrequency() reports %.2f-MHz\n", (float)freq.LowPart / 1000000.0f );

	if( freq.LowPart < 1000000 )
	{
	    write_log ( "CLOCKFREQ: Weird value.  Using QueryPerformanceCounter() instead of RDTSC.\n" );
	    useqpc = 1;
	}
	rpt_available = 1;
    }
    else
    {
	write_log ( "CLOCKFREQ: No support for clock-rate stuff!\n" );
	rpt_available = 0;
    }
#endif
    SetPriorityClass( GetCurrentProcess(), REALTIME_PRIORITY_CLASS );
    clockrate = read_processor_time();
    Sleep( 1000 );
    clockrate = read_processor_time() - clockrate;
    SetPriorityClass( GetCurrentProcess(), NORMAL_PRIORITY_CLASS );

    write_log ( "CLOCKFREQ: Measured as %d-MHz\n", clockrate / 1000000 );
    syncbase = clockrate;
    vsynctime = syncbase / VBLANK_HZ_PAL; /* default to 50Hz */
}

static BOOL bDiskChanged = FALSE;

static long FAR PASCAL AmigaWindowProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hDC;
    short wheeldelta;
    static int store_xy = 0;
    BOOL minimized;
    LPMINMAXINFO lpmmi;
    RECT rect;

    switch( message ) 
    {
	case WM_PALETTECHANGED:
	    if( (HWND)wParam != hWnd )
	    {
		write_log ( "WM_PALETTECHANGED Request\n" );
		WIN32GFX_PaletteChange();
	    }
	break;
	case WM_ACTIVATEAPP:
	    if (bActive = wParam) 
	    {
		if( WIN32GFX_IsFullScreen() ) 
		{
		    SetCursor (NULL);
#ifndef HARDWARE_SPRITE_EMULATION
		    SetCursorPos (mousecx, mousecy);
#else
		    if( !WIN32GFX_IsPicassoScreen() )
			SetCursorPos (mousecx, mousecy);
#endif
		}
		my_kbd_handler (VK_CAPITAL, 0x3a, TRUE);
	    }
	    else
	    {
		if( !WIN32GFX_IsFullScreen() )
		    setmouseactive (0);
		else
		{
		    if( WIN32GFX_IsPicassoScreen() )
		    {
			WIN32GFX_DisablePicasso();
		    }
		}
		}
	    break;
	case WM_ACTIVATE:
	    minimized = HIWORD( wParam );
	    if (LOWORD (wParam) != WA_INACTIVE) 
	    {
		write_log ( "WinUAE now active via WM_ACTIVATE\n" );
		if( !minimized )
		{
		    if( currprefs.win32_iconified_nospeed )
		    {
			SetPriorityClass( GetCurrentProcess(), NORMAL_PRIORITY_CLASS );
		    }
		    if( currprefs.win32_iconified_nosound )
		    {
			resume_sound();
		    }

		    clear_inhibit_frame( IHF_WINDOWHIDDEN );
		}
		ShowWindow (hWnd, SW_RESTORE);
		if( WIN32GFX_IsPicassoScreen() )
		{
		    WIN32GFX_EnablePicasso();
		}
	    }
	    else
	    {
		write_log ( "WinUAE now inactive via WM_ACTIVATE\n" );
		if( minimized && !quit_program )
		{
		    if( currprefs.win32_iconified_nospeed )
		    {
			SetPriorityClass( GetCurrentProcess(), IDLE_PRIORITY_CLASS );
		    }
		    if( currprefs.win32_iconified_nosound )
		    {
			pause_sound ();
		    }
		    set_inhibit_frame( IHF_WINDOWHIDDEN );
		}
	    }
	    break;

	case WM_SETCURSOR:
		if( WIN32GFX_IsFullScreen() ) 
	    {
#ifdef HARDWARE_SPRITE_EMULATION
		    if( !WIN32GFX_IsPicassoScreen() )
#endif
		    SetCursor (NULL);
		    return TRUE;
		}
	    break;

	case WM_SYSCOMMAND:
	    switch( wParam & 0xFFF0 )
	    {
		case SC_MAXIMIZE:
		    WIN32GFX_ToggleFullScreen();
			return 0;
		break;
		case SC_MINIMIZE:
		    WIN32GFX_DisablePicasso();
		break;
	    }
	    break;

	case WM_KEYUP:
	case WM_SYSKEYUP:
		numkeysdown--;
		keysdown[wParam] = 0;
		my_kbd_handler (wParam, (lParam >> 16) & 0x1ff, FALSE);
	    return 0;
	break;

     case WM_KEYDOWN:
     case WM_SYSKEYDOWN:
	if (LOWORD (lParam) == 1) 
    {
	    if (numkeysdown) 
	{
		    int key;
		    numkeysdown = 0;

		    for (key = 256; key--;) 
	    {
			if (keysdown[key]) 
		{
				if (checkkey (key, lParam))
				    numkeysdown++;
				else 
		    {
				    my_kbd_handler (key, (keysdown[key] >> 16) & 0x1ff, FALSE);
				    keysdown[key] = 0;
				}
			}
		    }
	    }
	    if (!keysdown[wParam]) 
	{
		    keysdown[wParam] = lParam;
		    numkeysdown++;
	    }
	    numkeysdown++;
	    my_kbd_handler (wParam, (lParam >> 16) & 0x1ff, TRUE);
	}
	break;

     case WM_LBUTTONDBLCLK: // According to MSDN, having CS_DBLCLKS in your window-class
			    // means that the sequence is WM_LBUTTONDOWN, WM_LBUTTONUP,
			    // WM_LBUTTONDBLCLK, and WM_LBUTTONUP.
			    // So we need to make WM_LBUTTONDBLCLK act like WM_LBUTTONDOWN.
     case WM_LBUTTONDOWN:
	if (ievent_alive) {
	    setcapture ();
	    buttonstate[0] = 1;
	} else if (!WIN32GFX_IsFullScreen() && !mouseactive)
	    setmouseactive (1);
	else
	    buttonstate[0] = 1;
	break;

     case WM_LBUTTONUP:
	releasecapture ();
	buttonstate[0] = 0;
	break;


     case WM_MBUTTONDOWN:
	if (ievent_alive)
	    setcapture ();
	buttonstate[1] = 1;
	break;

     case WM_MBUTTONUP:
	releasecapture ();
	buttonstate[1] = 0;
	break;

     case WM_RBUTTONDOWN:
	if (ievent_alive)
	    setcapture ();
	buttonstate[2] = 1;
	break;

     case WM_RBUTTONUP:
	releasecapture ();
	buttonstate[2] = 0;
	break;

     case WM_VSCROLL:
	 write_log ( "WM_VSCROLL\n" );
	 if( LOWORD( wParam ) == SB_LINEDOWN )
	     record_key(0x7A<<1);
	 else if( LOWORD( wParam ) == SB_LINEUP )
	     record_key(0x7B<<1);
	 break;

     case WM_MOUSEWHEEL:
	 wheeldelta = HIWORD(wParam);
	 write_log ( "WM_MOUSEWHEEL with delta %d\n", wheeldelta );
	 if( wheeldelta > 0 )
	     record_key(0x7A<<1);
	 else if( wheeldelta < 0 )
	     record_key(0x7B<<1);
	 break;

     case WM_MOUSEMOVE:
#ifndef HARDWARE_SPRITE_EMULATION
	 if( ( mouseactive && !ievent_alive ) || 
	     WIN32GFX_IsFullScreen() ) 
#else
	 if( ( ( mouseactive && !ievent_alive ) || WIN32GFX_IsFullScreen() ) &&
	     !WIN32GFX_IsPicassoScreen() )
#endif
	 {
	     /*
	      * In this mode, the mouse pointer is always centered in the window,
	      * this is ensured by the SetCursorPos call below.
	      * We don't want to handle messages that result from such a SetCursorPos
	      * call (recursion!), so exit early if we see one.
	      */
	     if (lParam == mousecl)
		 break;
	     lastmx += (signed short) LOWORD (lParam) - mousedx;
	     lastmy += (signed short) HIWORD (lParam) - mousedy;
	     if (ievent_alive) 
	     {
		 if (lastmx < 0)
		     lastmx = 0;
		 if (lastmx > WIN32GFX_GetWidth() )
		     lastmx = WIN32GFX_GetWidth();
		 if (lastmy < 0)
		     lastmy = 0;
		 if (lastmy > WIN32GFX_GetHeight() )
		     lastmy = WIN32GFX_GetHeight();
	     }
	     SetCursorPos (mousecx, mousecy);
	     break;
	}
	lastmx = (signed short) LOWORD (lParam);
	lastmy = (signed short) HIWORD (lParam);
    break;

     case WM_PAINT:
	notice_screen_contents_lost ();

	hDC = BeginPaint (hWnd, &ps);
	/* Check to see if this WM_PAINT is coming while we've got the GUI visible */
	if( manual_painting_needed )
	{
	    /* Update the display area */
	    if( !WIN32GFX_IsFullScreen() )
	    {
		if( DirectDraw_GetLockableType() != overlay_surface )
		    DX_Blit( 0, 0, 0, 0, WIN32GFX_GetWidth(), WIN32GFX_GetHeight(), BLIT_SRC );
	    }
	    else
	    {
		DirectDraw_Blt( primary_surface, NULL, secondary_surface, NULL, DDBLT_WAIT, NULL );
	    }
	}
	EndPaint (hWnd, &ps);

	break;

     case WM_DROPFILES:
	if (DragQueryFile ((HDROP) wParam, (UINT) - 1, NULL, 0)) {
	    if (DragQueryFile ((HDROP) wParam, 0, NULL, 0) < 255)
		DragQueryFile ((HDROP) wParam, 0, changed_prefs.df[0], sizeof (changed_prefs.df[0]));
	}
	DragFinish ((HDROP) wParam);
	break;

     case WM_CAPTURECHANGED:
	if ((HWND)lParam != hAmigaWnd)
	    buttonstate[0] = buttonstate[1] = buttonstate[2] = 0;
	break;

     case WM_TIMER:
	finishjob ();
	break;

     case WM_CREATE:
	DragAcceptFiles (hWnd, TRUE);
	break;

     case WM_CLOSE:
	uae_quit ();
	return 0;

     case WM_WINDOWPOSCHANGED:
	if( GetWindowRect( hWnd, &amigawin_rect) )
	{
	    if( hMainWnd )
	    {
		if( hWinUAEKey && store_xy++)
		{
		    RegSetValueEx( hWinUAEKey, "xPos", 0, REG_DWORD, (LPBYTE)&amigawin_rect.left, sizeof( LONG ) );
		    RegSetValueEx( hWinUAEKey, "yPos", 0, REG_DWORD, (LPBYTE)&amigawin_rect.top, sizeof( LONG ) );
		}
	    }
	}
	break;

    case WM_MOVING:
    case WM_MOVE:
    WIN32GFX_WindowMove();
    return TRUE;

    case WM_SIZING:
    WIN32GFX_WindowSize();
    return TRUE;
    case WM_SIZE:
    WIN32GFX_WindowSize();
    return 0;

    case WM_GETMINMAXINFO:
    rect.left=0;
    rect.top=0;
    lpmmi=(LPMINMAXINFO)lParam;
    rect.right=320;
    rect.bottom=256;
    //AdjustWindowRectEx(&rect,WSTYLE,0,0);
    lpmmi->ptMinTrackSize.x=rect.right-rect.left;
    lpmmi->ptMinTrackSize.y=rect.bottom-rect.top;
    return 0;

    default:
#if 0
#ifdef BSDSOCKET_SUPPORTED
    if( message >= 0xB000 && message < 0xB000+MAXPENDINGASYNC*2 )
    {
#if DEBUG_SOCKETS
	write_log ( "sockmsg(0x%x, 0x%x, 0x%x)\n", message, wParam, lParam );
#endif
	sockmsg( message, wParam, lParam );
	return 0;
    }
#endif
#endif
    break;
    }

    return DefWindowProc (hWnd, message, wParam, lParam);
}

static long FAR PASCAL MainWindowProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    RECT rc;
    HDC hDC;

    switch (message) {
     case WM_LBUTTONDOWN:
     case WM_MOUSEWHEEL:
     case WM_MOUSEMOVE:
     case WM_ACTIVATEAPP:
     case WM_DROPFILES:
     case WM_ACTIVATE:
     case WM_SETCURSOR:
     case WM_SYSCOMMAND:
     case WM_KEYUP:
     case WM_SYSKEYUP:
     case WM_KEYDOWN:
     case WM_SYSKEYDOWN:
     case WM_LBUTTONUP:
     case WM_MBUTTONDOWN:
     case WM_MBUTTONUP:
     case WM_RBUTTONDOWN:
     case WM_RBUTTONUP:
     case WM_MOVING:
     case WM_MOVE:
     case WM_SIZING:
     case WM_SIZE:
     case WM_GETMINMAXINFO:
     case WM_CREATE:
     case WM_DESTROY:
     case WM_USER + 0x200:
     case WM_CLOSE:
     case WM_HELP:
     case WM_DEVICECHANGE:
	return AmigaWindowProc (hWnd, message, wParam, lParam);

     case WM_DISPLAYCHANGE:
	if (!WIN32GFX_IsFullScreen && (wParam + 7) / 8 != DirectDraw_GetBytesPerPixel() )
	    WIN32GFX_DisplayChangeRequested();
	break;

     case WM_ENTERSIZEMOVE:
	in_sizemove++;
	break;

     case WM_EXITSIZEMOVE:
	in_sizemove--;
	/* fall through */

     case WM_WINDOWPOSCHANGED:
	WIN32GFX_WindowMove();
	if( hAmigaWnd && GetWindowRect (hAmigaWnd, &amigawin_rect) )
	{
	    if (in_sizemove > 0)
		break;

	    if( !WIN32GFX_IsFullScreen() && hAmigaWnd )
	    {
		if( amigawin_rect.left & 3 ) 
		{
		    RECT rc2;
		    if( GetWindowRect( hMainWnd, &rc2 ) )
		    {
			MoveWindow (hMainWnd, rc2.left + 4 - amigawin_rect.left % 4, rc2.top,
				    rc2.right - rc2.left, rc2.bottom - rc2.top, TRUE);
		    }
		}
		//setmouseactive (0);
		return 0;
	    }
	}
	break;

     case WM_PAINT:
	hDC = BeginPaint (hWnd, &ps);
	GetClientRect (hWnd, &rc);
	DrawEdge (hDC, &rc, EDGE_SUNKEN, BF_RECT);
	EndPaint (hWnd, &ps);
	break;
     case WM_NCLBUTTONDBLCLK:
	if (wParam == HTCAPTION) {
	    WIN32GFX_ToggleFullScreen();
	    return 0;
	}
	break;
    default:
    break;

    }

    return DefWindowProc (hWnd, message, wParam, lParam);
}

void handle_events (void)
{
    MSG msg;

    while (PeekMessage (&msg, 0, 0, 0, PM_REMOVE)) {
	TranslateMessage (&msg);
	DispatchMessage (&msg);
    }
}

/* Console Win32 helper routines */
void activate_debugger ();

/* We're not a console-app anymore! */
void setup_brkhandler (void)
{
}

void remove_brkhandler (void)
{
}

int WIN32_RegisterClasses( void )
{
    WNDCLASS wc;
    HDC hDC = GetDC( NULL ); 

    if( GetDeviceCaps( hDC, NUMCOLORS ) != -1 ) 
	g_dwBackgroundColor = RGB( 255, 0, 255 );    
    ReleaseDC( NULL, hDC );

    wc.style = CS_BYTEALIGNCLIENT | CS_BYTEALIGNWINDOW | CS_DBLCLKS;
    wc.lpfnWndProc = AmigaWindowProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = 0;
    wc.hIcon = LoadIcon (GetModuleHandle (NULL), MAKEINTRESOURCE( IDI_APPICON ) );
    wc.hCursor = LoadCursor (NULL, IDC_ARROW);
    wc.lpszMenuName = 0;
    wc.lpszClassName = "AmigaPowah";
    wc.hbrBackground = CreateSolidBrush( g_dwBackgroundColor ); 
    if (!RegisterClass (&wc))
	return 0;

    wc.style = CS_BYTEALIGNCLIENT | CS_BYTEALIGNWINDOW | CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = MainWindowProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = 0;
    wc.hIcon = LoadIcon (GetModuleHandle (NULL), MAKEINTRESOURCE( IDI_APPICON ) );
    wc.hCursor = LoadCursor (NULL, IDC_ARROW);
    wc.hbrBackground = CreateSolidBrush( g_dwBackgroundColor ); 
    wc.lpszMenuName = 0;
    wc.lpszClassName = "PCsuxRox";
    if (!RegisterClass (&wc))
	return 0;
    return 1;
}

#ifdef __GNUC__
#undef WINAPI
#define WINAPI
#endif

static HINSTANCE hRichEdit = NULL, hHtmlHelp = NULL;

#ifdef USE_ZLIB_DLL
static HINSTANCE hZlib = NULL;
FARPROC	pgzread = NULL, pgzopen = NULL, pgzclose = NULL, pgzwrite = NULL, pgzerror = NULL;
#endif

int WIN32_CleanupLibraries( void )
{
    if (hRichEdit)
	FreeLibrary (hRichEdit);
    
    if( hHtmlHelp )
	FreeLibrary( hHtmlHelp );

    if( hUIDLL )
	FreeLibrary( hUIDLL );

#ifdef USE_ZLIB_DLL
    if( hZlib )
	FreeLibrary( hZlib );
#endif

    return 1;
}

/* HtmlHelp Initialization - optional component */
int WIN32_InitHtmlHelp( void )
{
    int result = 0;
    if( hHtmlHelp = LoadLibrary( "HHCTRL.OCX" ) )
    {
	pHtmlHelp = ( HWND(WINAPI *)(HWND, LPCSTR, UINT, LPDWORD ) )GetProcAddress( hHtmlHelp, "HtmlHelpA" );
	result = 1;
    }


    return result;
}

#if 0
#define TESTING_LANGUAGES
#define TEST_LANGID LANG_GERMAN
//#define TEST_LANGID LANG_FRENCH
//#define TEST_LANGID LANG_TURKISH
#endif

static HMODULE LoadGUI( void )
{
    HMODULE result = NULL;
    LPCTSTR dllname = NULL;
    LANGID language = GetUserDefaultLangID() & 0x3FF; // low 9-bits form the primary-language ID
#ifdef TESTING_LANGUAGES
    language = TEST_LANGID;
#endif

    switch( language )
    {
    case LANG_AFRIKAANS:
	dllname = "WinUAE_Afrikaans.dll";
	break;
    case LANG_ARABIC:
	dllname = "WinUAE_Arabic.dll";
	break;
    case LANG_ARMENIAN:
	dllname = "WinUAE_Armenian.dll";
	break;
    case LANG_ASSAMESE:
	dllname = "WinUAE_Assamese.dll";
	break;
    case LANG_AZERI:
	dllname = "WinUAE_Azeri.dll";
	break;
    case LANG_BASQUE:
	dllname = "WinUAE_Basque.dll";
	break;
    case LANG_BELARUSIAN:
	dllname = "WinUAE_Belarusian.dll";
	break;
    case LANG_BENGALI:
	dllname = "WinUAE_Bengali.dll";
	break;
    case LANG_BULGARIAN:
	dllname = "WinUAE_Bulgarian.dll";
	break;
    case LANG_CATALAN:
	dllname = "WinUAE_Catalan.dll";
	break;
    case LANG_CHINESE:
	dllname = "WinUAE_Chinese.dll";
	break;
    case LANG_CROATIAN:
	dllname = "WinUAE_CroatianSerbian.dll";
	break;
    case LANG_CZECH:
	dllname = "WinUAE_Czech.dll";
	break;
    case LANG_DANISH:
	dllname = "WinUAE_Danish.dll";
	break;
    case LANG_DUTCH:
	dllname = "WinUAE_Dutch.dll";
	break;
    case LANG_ESTONIAN:
	dllname = "WinUAE_Estonian.dll";
	break;
    case LANG_FAEROESE:
	dllname = "WinUAE_Faeroese.dll";
	break;
    case LANG_FARSI:
	dllname = "WinUAE_Farsi.dll";
	break;
    case LANG_FINNISH:
	dllname = "WinUAE_Finnish.dll";
	break;
    case LANG_FRENCH:
	dllname = "WinUAE_French.dll";
	break;
    case LANG_GEORGIAN:
	dllname = "WinUAE_Georgian.dll";
	break;
    case LANG_GERMAN:
	dllname = "WinUAE_German.dll";
	break;
    case LANG_GREEK:
	dllname = "WinUAE_Greek.dll";
	break;
    case LANG_GUJARATI:
	dllname = "WinUAE_Gujarati.dll";
	break;
    case LANG_HEBREW:
	dllname = "WinUAE_Hebrew.dll";
	break;
    case LANG_HINDI:
	dllname = "WinUAE_Hindi.dll";
	break;
    case LANG_HUNGARIAN:
	dllname = "WinUAE_Hungarian.dll";
	break;
    case LANG_ICELANDIC:
	dllname = "WinUAE_Icelandic.dll";
	break;
    case LANG_INDONESIAN:
	dllname = "WinUAE_Indonesian.dll";
	break;
    case LANG_ITALIAN:
	dllname = "WinUAE_Italian.dll";
	break;
    case LANG_JAPANESE:
	dllname = "WinUAE_Japanese.dll";
	break;
    case LANG_KANNADA:
	dllname = "WinUAE_Kannada.dll";
	break;
    case LANG_KASHMIRI:
	dllname = "WinUAE_Kashmiri.dll";
	break;
    case LANG_KAZAK:
	dllname = "WinUAE_Kazak.dll";
	break;
    case LANG_KONKANI:
	dllname = "WinUAE_Konkani.dll";
	break;
    case LANG_KOREAN:
	dllname = "WinUAE_Korean.dll";
	break;
    case LANG_LATVIAN:
	dllname = "WinUAE_Latvian.dll";
	break;
    case LANG_LITHUANIAN:
	dllname = "WinUAE_Lithuanian.dll";
	break;
    case LANG_MACEDONIAN:
	dllname = "WinUAE_Macedonian.dll";
	break;
    case LANG_MALAY:
	dllname = "WinUAE_Malay.dll";
	break;
    case LANG_MALAYALAM:
	dllname = "WinUAE_Malayalam.dll";
	break;
    case LANG_MANIPURI:
	dllname = "WinUAE_Manipuri.dll";
	break;
    case LANG_MARATHI:
	dllname = "WinUAE_Marathi.dll";
	break;
    case LANG_NEPALI:
	dllname = "WinUAE_Nepali.dll";
	break;
    case LANG_NORWEGIAN:
	dllname = "WinUAE_Norwegian.dll";
	break;
    case LANG_ORIYA:
	dllname = "WinUAE_Oriya.dll";
	break;
    case LANG_POLISH:
	dllname = "WinUAE_Polish.dll";
	break;
    case LANG_PORTUGUESE:
	dllname = "WinUAE_Portuguese.dll";
	break;
    case LANG_PUNJABI:
	dllname = "WinUAE_Punjabi.dll";
	break;
    case LANG_ROMANIAN:
	dllname = "WinUAE_Romanian.dll";
	break;
    case LANG_RUSSIAN:
	dllname = "WinUAE_Russian.dll";
	break;
    case LANG_SANSKRIT:
	dllname = "WinUAE_Sanskrit.dll";
	break;
    case LANG_SINDHI:
	dllname = "WinUAE_Sindhi.dll";
	break;
    case LANG_SLOVAK:
	dllname = "WinUAE_Slovak.dll";
	break;
    case LANG_SLOVENIAN:
	dllname = "WinUAE_Slovenian.dll";
	break;
    case LANG_SPANISH:
	dllname = "WinUAE_Spanish.dll";
	break;
    case LANG_SWAHILI:
	dllname = "WinUAE_Swahili.dll";
	break;
    case LANG_SWEDISH:
	dllname = "WinUAE_Swedish.dll";
	break;
    case LANG_TAMIL:
	dllname = "WinUAE_Tamil.dll";
	break;
    case LANG_TATAR:
	dllname = "WinUAE_Tatar.dll";
	break;
    case LANG_TELUGU:
	dllname = "WinUAE_Telugu.dll";
	break;
    case LANG_THAI:
	dllname = "WinUAE_Thai.dll";
	break;
    case LANG_TURKISH:
	dllname = "WinUAE_Turkish.dll";
	break;
    case LANG_UKRAINIAN:
	dllname = "WinUAE_Ukrainian.dll";
	break;
    case LANG_URDU:
	dllname = "WinUAE_Urdu.dll";
	break;
    case LANG_UZBEK:
	dllname = "WinUAE_Uzbek.dll";
	break;
    case LANG_VIETNAMESE:
	dllname = "WinUAE_Vietnamese.dll";
	break;
    case 0x400:
	dllname = "guidll.dll";
	break;
    }

    if( dllname )
    {
	TCHAR  szFilename[ MAX_PATH ];
	DWORD  dwVersionHandle, dwFileVersionInfoSize;
	LPVOID lpFileVersionData = NULL;
	BOOL   success = FALSE;
	result = LoadLibrary( dllname );
	if( result && GetModuleFileName( result, (LPTSTR)&szFilename, MAX_PATH ) )
	{
	    dwFileVersionInfoSize = GetFileVersionInfoSize( szFilename, &dwVersionHandle );
	    if( dwFileVersionInfoSize )
	    {
		if( lpFileVersionData = calloc( 1, dwFileVersionInfoSize ) )
		{
		    if( GetFileVersionInfo( szFilename, dwVersionHandle, dwFileVersionInfoSize, lpFileVersionData ) )
		    {
			VS_FIXEDFILEINFO *vsFileInfo = NULL;
			UINT uLen;
			if( VerQueryValue( lpFileVersionData, TEXT("\\"), (void **)&vsFileInfo, &uLen ) )
			{
			    if( vsFileInfo &&
				( HIWORD(vsFileInfo->dwProductVersionMS) == UAEMAJOR ) 
				&& ( LOWORD(vsFileInfo->dwProductVersionMS) == UAEMINOR ) 
				&& ( HIWORD(vsFileInfo->dwProductVersionLS) == UAESUBREV )
// Change this to an #if 1 when the WinUAE Release version (as opposed to UAE-core version) 
// requires a GUI-DLL change...
#if 0
				&& ( LOWORD(vsFileInfo->dwProductVersionLS) == WINUAERELEASE) 
#endif
				)
			    {
				success = TRUE;
			    }
			}
		    }
		    free( lpFileVersionData );
		}
	    }
	}
	if( result && !success )
	{
	    FreeLibrary( result );
	    result = NULL;
	}
    }

    return result;
}

#ifndef _WIN32_WCE
/* try to load COMDLG32 and DDRAW, initialize csDraw */
int WIN32_InitLibraries( void )
{
    int result = 1;
    /* Determine our processor speed and capabilities */
    figure_processor_speed();
    
    /* Make sure we do an InitCommonControls() to get some advanced controls */
    InitCommonControls();
    
    hRichEdit = LoadLibrary( "RICHED32.DLL" );
    
    hUIDLL = LoadGUI();

#ifdef USE_ZLIB_DLL
    hZlib = LoadLibrary( "ZLIB.DLL" );
    if( hZlib )
    {
	pgzread = GetProcAddress( hZlib, "gzread" );
	pgzopen = GetProcAddress( hZlib, "gzopen" );
	pgzclose = GetProcAddress( hZlib, "gzclose" );
	pgzwrite = GetProcAddress( hZlib, "gzwrite" );
	pgzerror = GetProcAddress( hZlib, "gzerror" );
	if( !pgzread || !pgzopen || !pgzclose || !pgzwrite || !pgzerror)
	{
	    FreeLibrary( hZlib );
	    hZlib = NULL;
	}
    }
#endif
    return result;
}
#endif

/* Mathias says: "this truly sucks, I'll include a native gunzip() routine soon" */

#ifdef USE_ZLIB_DLL
extern FARPROC	pgzread, pgzopen, pgzclose, pgzwrite, pgzerror;
#define GZOPEN( X, Y ) (gzFile)pgzopen( X, Y )
#define GZCLOSE( X ) (int)pgzclose( X )
#define GZREAD( X, Y, Z ) (int)pgzread( X, Y, Z )

#define ZLIB_BUFFER_SIZE 32767

/* gzip decompression via zlib */
static int zlib_gunzip( const char *src, char *dst )
{
    int result = 0, gzResult = 0;
    gzFile zSrc = NULL;
    FILE *fDst = NULL;
    size_t filesize = 0;
    uae_u8 *buffer = NULL;

    if( hZlib )
    {
	zSrc = GZOPEN( src, "rb" );
	fDst = fopen( dst, "wb" );
	buffer = xmalloc( ZLIB_BUFFER_SIZE + 1 );
	
	if( zSrc && fDst && buffer )
	{
	    DWORD dwWritten = 0;
	    result = 1;
	    do
	    {
		gzResult = GZREAD( zSrc, buffer, ZLIB_BUFFER_SIZE );
		if( gzResult > 0 )
		{
		    if( fwrite( buffer, gzResult, 1, fDst ) != 1 )
		    {
			result = 0;
			break;
		    }
		}
	    } while( gzResult == ZLIB_BUFFER_SIZE );
	}
	
	if( zSrc )
	{
	    GZCLOSE( zSrc );
	}
	
	if( fDst )
	{
	    fclose( fDst );
	}
    }
    return result;
}
#endif

static char *uncompress_error[2] = { "Error: You need zlib.dll to use .adz/.roz files!\n", 
				    "Error: You need xdms.exe (32 bit) to use .dms files!\n" };

int uncompress_hack( int type, const char *src, const char *dst)
{
    int result = 0;
    char fullname[1024];
    char buf[1024];
    char cmd[256];
    char *posn = NULL;
    STARTUPINFO si = {sizeof si};
    PROCESS_INFORMATION pi;

    strcpy( fullname, dst );

    if( type == 1 )
    {
#ifdef USE_ZLIB_DLL
	result = zlib_gunzip( src, fullname );
	if( !result )
	{
	    gui_message( uncompress_error[type-1] );
	}
	return result;
#else
	strcpy( cmd, "gzip.exe -f -d" );
	strcat( fullname, ".gz" );
#endif
    }
    else if( type == 2 )
    {
	strcpy( cmd, "xdms.exe u" );
	posn = strrchr( fullname, '.' );
	if( posn )
	{
	    *posn = 0;
	    strcat( fullname, ".dms" );
	}
    }

    if( CopyFile( src, fullname, FALSE ) ) 
    {
	sprintf (buf, "%s %s +%s", cmd, fullname, dst );
	si.dwFlags = STARTF_USESTDHANDLES;
	if( CreateProcess( NULL, buf, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi ) ) 
	{
	    WaitForSingleObject( pi.hProcess, INFINITE );
	    result = -1;
	}
	else
	{
	    gui_message( uncompress_error[type-1] );
	}
    }

    /* Special handling for broken xdms.exe */
    if( type == 2 )
    {
	DeleteFile( fullname );     /* Delete the uaeXX.dms file */
    }

    return result;
}

/* console window for debugging messages */
/* Brian: disable for release version if you want (TW) */

#define WRITE_LOG_BUF_SIZE 4096

static int consoleopen = 0;
HANDLE stdinput,stdoutput;

static void openconsole(void)
{
    if(consoleopen) return;
    AllocConsole();
    stdinput=GetStdHandle(STD_INPUT_HANDLE);
    stdoutput=GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleMode(stdinput,ENABLE_PROCESSED_INPUT|ENABLE_LINE_INPUT|ENABLE_ECHO_INPUT|ENABLE_PROCESSED_OUTPUT);
    consoleopen = 1;
}

/* console functions for debugger */

void console_out (const char *format,...)
{
    va_list parms;
    char buffer[WRITE_LOG_BUF_SIZE];
    DWORD temp;

    va_start (parms, format);
    _vsnprintf( buffer, WRITE_LOG_BUF_SIZE-1, format, parms );
    va_end (parms);
    openconsole();
    WriteConsole(stdoutput,buffer,strlen(buffer),&temp,0);
}

int console_get (char *out, int maxlen)
{
    DWORD len,totallen;

    totallen=0;
    while(maxlen>0) 
    {
	    ReadConsole(stdinput,out,1,&len,0);
	    if(*out == 13) break;
	    out++;
	    maxlen--;
	    totallen++;
    }
    *out=0;
    return totallen;
}

void console_flush (void)
{
}

/* GCC/EGCS wants this write_log in order to work from socket-land and to do traces */
#ifdef __GNUC__
void write_log (const char *format, ...)
{
    int result = 0;
    DWORD numwritten;
    char buffer[12];
    va_list parms;
    int count = 0;
    int *blah = NULL;

    if( debugfile )
    {
#if defined HAVE_GETTICKCOUNT && defined TIMESTAMP_LOGS
	{
	    sprintf( buffer, "%7d - ", GetTickCount() );
	    fprintf( debugfile, buffer );
	}
#endif
	va_start (parms, format);
	count = vfprintf( debugfile, format, parms );
	fflush( debugfile );
	if( count >= WRITE_LOG_BUF_SIZE-1 )
	{
	    fprintf( debugfile, "SHIT in write_log ()\n" );
	    fflush( debugfile );
	    *blah = 0; /* Access Violation here! */
	    abort();
	}
	else
	    result = count;
	va_end (parms);
    }
}
#else /* MSVC likes this one, and so do I */
int write_log ( const char *format, ... )
{
    int result = 0;
#ifdef _DEBUG
    DWORD numwritten;
#endif
    char buffer[ WRITE_LOG_BUF_SIZE ];
    va_list parms;
    int count = 0;
    int *blah = (int *)0xdeadbeef;

    va_start (parms, format);
    count = _vsnprintf( buffer, WRITE_LOG_BUF_SIZE-1, format, parms );
#if defined HAVE_GETTICKCOUNT && defined TIMESTAMP_LOGS
    {
	char buffme[WRITE_LOG_BUF_SIZE];
	sprintf( buffme, "%7d - %s", GetTickCount(), buffer );
	OutputDebugString( buffme );
	if( debugfile )
	    fprintf( debugfile, buffme );
	result = strlen( buffme );
    }
#else
    OutputDebugString( buffer );
    if( debugfile )
    {
	fprintf( debugfile, buffer );
	fflush( debugfile );
    }
    result = strlen( buffer );
#endif
#ifdef _DEBUG
    openconsole();
    WriteConsole(stdoutput,buffer,strlen(buffer),&numwritten,0);
#endif
    va_end (parms);
    return result;
}
#endif

int debuggable (void)
{
    return 0;
}

int needmousehack (void)
{
    if( WIN32GFX_IsFullScreen() || WIN32GFX_IsPicassoScreen() )
	return 0;
    else
	return 1;
}

void LED (int a)
{
}

void logging_init( void )
{
    char debugfilename[MAX_PATH];
    if (1) {
	sprintf( debugfilename, "%s\\winuaelog.txt", start_path );
	if( !debugfile )
	    debugfile = fopen( debugfilename, "wt" );
    }
    write_log ( "%s\n", VersionStr );
    write_log ("\n(c) 1995-2001 Bernd Schmidt   - Core UAE concept and implementation."
	       "\n(c) 1996-1999 Mathias Ortmann - Win32 port and bsdsocket support."
	       "\n(c) 1996-2001 Brian King      - Win32 port, Picasso96 RTG, and GUI."
	       "\n(c) 1998-2001 Toni Wilen      - AGA chipset, NTSC/PAL modes.\n"
	       "\n(c) 2000-2001 Bernd Meyer     - JIT engine.\n"
	       "\n(c) 2000-2001 Bernd Roesch    - MIDI input, many fixes.\n"
	       "\nPress F12 to show the Settings Dialog (GUI), Alt-F4 to quit."
	       "\nEnd+F1 changes floppy 0, End+F2 changes floppy 1, etc.\n"
	       "\nhttp://www.codepoet.com/UAE/\n\n");
}

void logging_cleanup( void )
{
    if( debugfile )
	fclose( debugfile );
}

static const char *sound_styles[] = { "waveout_looping", "waveout_dblbuff", "dsound_looping", "dsound_dblbuff", 0 };

void target_save_options (FILE *f, struct uae_prefs *p)
{
    fprintf (f, "win32.middle_mouse=%s\n", p->win32_middle_mouse ? "true" : "false");
    fprintf (f, "win32.logfile=%s\n", p->win32_logfile ? "true" : "false");
    fprintf (f, "win32.map_drives=%s\n", p->win32_automount_drives ? "true" : "false" );
    fprintf (f, "win32.serial_port=%s\n", p->use_serial ? p->sername : "none" );
    fprintf (f, "win32.parallel_port=%s\n", p->prtname[0] ? p->prtname : "none" );
    fprintf (f, "win32.iconified_nospeed=%s\n", p->win32_iconified_nospeed ? "true" : "false");
    fprintf (f, "win32.iconified_nosound=%s\n", p->win32_iconified_nosound ? "true" : "false");
    fprintf (f, "win32.no_overlay=%s\n", p->win32_no_overlay ? "true" : "false" );
}

int target_parse_option (struct uae_prefs *p, char *option, char *value)
{
    int result = (cfgfile_yesno (option, value, "middle_mouse", &p->win32_middle_mouse)
		  || cfgfile_yesno (option, value, "logfile", &p->win32_logfile)
		  || cfgfile_yesno (option, value, "no_overlay", &p->win32_no_overlay)
		  || cfgfile_yesno (option, value, "map_drives", &p->automount_drives)
		  || cfgfile_yesno (option, value, "iconified_nospeed", &p->win32_iconified_nospeed)
		  || cfgfile_yesno (option, value, "iconified_nosound", &p->win32_iconified_nosound)
		  || cfgfile_string (option, value, "serial_port", &p->sername[0], 256)
		  || cfgfile_string (option, value, "parallel_port", &p->prtname[0], 256));

    if (p->sername[0] == 'n')
	p->use_serial = 0;
    else
	p->use_serial = 1;
    
    return 0;
}

void WIN32_HandleRegistryStuff( void )
{
    RGBFTYPE colortype      = RGBFB_NONE;
    DWORD dwType            = REG_DWORD;
    DWORD dwDisplayInfoSize = sizeof( colortype );
    DWORD disposition;
    char path[MAX_PATH] = "";
    HKEY hWinUAEKeyLocal = NULL;

    /* Create/Open the hWinUAEKey which points to our config-info */
    if( RegCreateKeyEx( HKEY_CLASSES_ROOT, ".uae", 0, "", REG_OPTION_NON_VOLATILE,
			  KEY_ALL_ACCESS, NULL, &hWinUAEKey, &disposition ) == ERROR_SUCCESS )
    {
	// Regardless of opening the existing key, or creating a new key, we will write the .uae filename-extension
	// commands in.  This way, we're always up to date.

	/* Set our (default) sub-key to point to the "WinUAE" key, which we then create */
	RegSetValueEx( hWinUAEKey, "", 0, REG_SZ, (CONST BYTE *)"WinUAE", strlen( "WinUAE" ) + 1 );

	if( ( RegCreateKeyEx( HKEY_CLASSES_ROOT, "WinUAE\\shell\\Edit\\command", 0, "", REG_OPTION_NON_VOLATILE,
			      KEY_ALL_ACCESS, NULL, &hWinUAEKeyLocal, &disposition ) == ERROR_SUCCESS ) )
	{
	    /* Set our (default) sub-key to BE the "WinUAE" command for editing a configuration */
	    sprintf( path, "%s\\WinUAE.exe -f \"%%1\" -s use_gui=yes", start_path );
	    RegSetValueEx( hWinUAEKeyLocal, "", 0, REG_SZ, (CONST BYTE *)path, strlen( path ) + 1 );
	}
	RegCloseKey( hWinUAEKeyLocal );

	if( ( RegCreateKeyEx( HKEY_CLASSES_ROOT, "WinUAE\\shell\\Open\\command", 0, "", REG_OPTION_NON_VOLATILE,
			      KEY_ALL_ACCESS, NULL, &hWinUAEKeyLocal, &disposition ) == ERROR_SUCCESS ) )
	{
	    /* Set our (default) sub-key to BE the "WinUAE" command for launching a configuration */
	    sprintf( path, "%s\\WinUAE.exe -f \"%%1\"", start_path );
	    RegSetValueEx( hWinUAEKeyLocal, "", 0, REG_SZ, (CONST BYTE *)path, strlen( path ) + 1 );
	}
	RegCloseKey( hWinUAEKeyLocal );
    }
    else
    {
	char szMessage[ MAX_PATH ];
	WIN32GUI_LoadUIString( IDS_REGKEYCREATEFAILED, szMessage, MAX_PATH );
	gui_message( szMessage );
	hWinUAEKey = NULL;
    }
    RegCloseKey( hWinUAEKey );

    /* Create/Open the hWinUAEKey which points our config-info */
    if( RegCreateKeyEx( HKEY_CURRENT_USER, "Software\\CodePoet Computing\\WinUAE", 0, "", REG_OPTION_NON_VOLATILE,
			  KEY_ALL_ACCESS, NULL, &hWinUAEKey, &disposition ) == ERROR_SUCCESS )
    {
	if( disposition == REG_CREATED_NEW_KEY )
	{
	    /* Create and initialize all our sub-keys to the default values */
	    colortype = 0;
	    RegSetValueEx( hWinUAEKey, "DisplayInfo", 0, REG_DWORD, (CONST BYTE *)&colortype, sizeof( colortype ) );
	    RegSetValueEx( hWinUAEKey, "xPos", 0, REG_DWORD, (CONST BYTE *)&colortype, sizeof( colortype ) );
	    RegSetValueEx( hWinUAEKey, "yPos", 0, REG_DWORD, (CONST BYTE *)&colortype, sizeof( colortype ) );
	    RegSetValueEx( hWinUAEKey, "FloppyPath", 0, REG_SZ, (CONST BYTE *)start_path, strlen( start_path ) + 1 );
	    RegSetValueEx( hWinUAEKey, "KickstartPath", 0, REG_SZ, (CONST BYTE *)start_path, strlen( start_path ) + 1 );
	    RegSetValueEx( hWinUAEKey, "hdfPath", 0, REG_SZ, (CONST BYTE *)start_path, strlen( start_path ) + 1 );
	}
	// Set this even when we're opening an existing key, so that the version info is always up to date.
	RegSetValueEx( hWinUAEKey, "Version", 0, REG_SZ, (CONST BYTE *)VersionStr, strlen( VersionStr ) + 1 );
	
	RegQueryValueEx( hWinUAEKey, "DisplayInfo", 0, &dwType, (LPBYTE)&colortype, &dwDisplayInfoSize );
	if( colortype == 0 ) /* No color information stored in the registry yet */
	{
	    char szMessage[ 4096 ];
	    char szTitle[ MAX_PATH ];
	    WIN32GUI_LoadUIString( IDS_GFXCARDCHECK, szMessage, 4096 );
	    WIN32GUI_LoadUIString( IDS_GFXCARDTITLE, szTitle, MAX_PATH );
	    
	    if( MessageBox( NULL, szMessage, szTitle, 
		MB_YESNO | MB_ICONWARNING | MB_TASKMODAL | MB_SETFOREGROUND ) == IDYES )
	    {
		colortype = WIN32GFX_FigurePixelFormats(0);
		RegSetValueEx( hWinUAEKey, "DisplayInfo", 0, REG_DWORD, (CONST BYTE *)&colortype, sizeof( colortype ) );
	    }
	}
	if( colortype )
	{
	    /* Set the 16-bit pixel format for the appropriate modes */
	    WIN32GFX_FigurePixelFormats( colortype );
	}
    }
    else
    {
	char szMessage[ MAX_PATH ];
	WIN32GUI_LoadUIString( IDS_REGKEYCREATEFAILED, szMessage, MAX_PATH );
	gui_message( szMessage );
	hWinUAEKey = NULL;
    }
}

void machdep_init (void)
{
}

char *start_path = NULL;
char help_file[ MAX_PATH ];

int PASCAL WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine,
		    int nCmdShow)
{
    char *posn;
    HANDLE hMutex;
    OSVERSIONINFO osVersion;

#ifdef __GNUC__
    __asm__ ("leal -2300*1024(%%esp),%0" : "=r" (win32_stackbase) :);
#else
__asm{
    mov eax,esp
    sub eax,2300*1024
    mov win32_stackbase,eax
 }
#endif

    hInst = hInstance;
    hMutex = CreateMutex( NULL, FALSE, "WinUAE Instantiated" ); // To tell the installer we're running
    
    osVersion.dwOSVersionInfoSize = sizeof( OSVERSIONINFO );
    if( GetVersionEx( &osVersion ) )
    {
	if( ( osVersion.dwPlatformId == VER_PLATFORM_WIN32_NT ) &&
	    ( osVersion.dwMajorVersion <= 4 ) )
	{
	    /* WinUAE not supported on this version of Windows... */
	    char szWrongOSVersion[ MAX_PATH ];
	    WIN32GUI_LoadUIString( IDS_WRONGOSVERSION, szWrongOSVersion, MAX_PATH );
	    gui_message( szWrongOSVersion );
	    return FALSE;
	}
    }

    /* Get our executable's root-path */
    if( ( start_path = xmalloc( MAX_PATH ) ) )
    {
	GetModuleFileName( NULL, start_path, MAX_PATH );
	if( ( posn = strrchr( start_path, '\\' ) ) )
	    *posn = 0;
	sprintf( help_file, "%s\\WinUAE.chm", start_path );

	sprintf( VersionStr, "WinUAE %d.%d.%d, Release %d%s", UAEMAJOR, UAEMINOR, UAESUBREV, WINUAERELEASE, WINUAEBETA ? WINUAEBETASTR : "" );

	logging_init ();
	printf ("Hello, world\n");

	if( WIN32_RegisterClasses() && WIN32_InitLibraries() && DirectDraw_Start() )
	{
	    struct foo {
		DEVMODE actual_devmode;
		char overrun[8];
	    } devmode;

	    DWORD i = 0;

	    DirectDraw_EnumDisplayModes( 0, modesCallback );
	    
	    memset( &devmode, 0, sizeof(DEVMODE) + 8 );
	    devmode.actual_devmode.dmSize = sizeof(DEVMODE);
	    devmode.actual_devmode.dmDriverExtra = 8;
#define ENUM_CURRENT_SETTINGS ((DWORD)-1)
	    if( EnumDisplaySettings( NULL, ENUM_CURRENT_SETTINGS, (LPDEVMODE)&devmode ) )
	    {
		default_freq = devmode.actual_devmode.dmDisplayFrequency;
		write_log ( "Your Windows desktop refresh frequency is %d Hz\n", default_freq );
		if( default_freq >= 70 )
		    default_freq = 70;
		else
		    default_freq = 60;
	    }

	    WIN32_HandleRegistryStuff();
	    if( WIN32_InitHtmlHelp() == 0 )
	    {
		char szMessage[ MAX_PATH ];
		WIN32GUI_LoadUIString( IDS_NOHELP, szMessage, MAX_PATH );
		write_log ( szMessage );
	    }

	    DirectDraw_Release();
#ifdef __MINGW32__
	    real_main (_argc, _argv);
#else
	    real_main (__argc, __argv);
#endif
	}
	free( start_path );
    }
	
    WIN32_CleanupLibraries();
    _fcloseall();
    if( hWinUAEKey )
	RegCloseKey( hWinUAEKey );
    CloseHandle( hMutex );
    return FALSE;
}


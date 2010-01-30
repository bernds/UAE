/*
 * UAE - The Un*x Amiga Emulator
 *
 * Win32 interface
 *
 * Copyright 1997 Mathias Ortmann
 */

#include "config.h"
#include "sysconfig.h"

#include <stdlib.h>
#include <stdarg.h>
#include <signal.h>

/* No idea what this is for, but it doesn't link without it if we use
   QueryInterface.  */
#define INITGUID

#include <windows.h>
#include <ddraw.h>
#include <commctrl.h>
#include <commdlg.h>
#include <io.h>

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
#include "picasso96.h"

#include "osdep/win32.h"
#include "osdep/win32gui.h"
#include "resource.h"

#define IHF_WINDOWHIDDEN 6

HINSTANCE hInst;

static BOOL (WINAPI * pGetOpenFileNameA) (LPOPENFILENAME);
static HRESULT (WINAPI * pDirectDrawCreate) (GUID FAR *, LPDIRECTDRAW FAR *, IUnknown FAR *);
static HRESULT CALLBACK modesCallback (LPDDSURFACEDESC modeDesc, LPVOID context);

static int display_change_requested = 0;

int julian_mode;
int debug_logging = 3;

HWND hAmigaWnd, hMainWnd, hStatusWnd;
/*DWORD Keys; */
static RECT amigawin_rect;

char *start_path = NULL;

static LPDIRECTDRAW lpDD;
static LPDIRECTDRAW2 lpDD2;
static LPDIRECTDRAWSURFACE lpDDS;
static LPDIRECTDRAWCLIPPER lpDDC;
static LPDIRECTDRAWPALETTE lpDDP;
static DDSURFACEDESC current_surface;
static DDSURFACEDESC ddsd;

#define TITLETEXT PROGNAME " -- Amiga Display"

char VersionStr[256];

static int current_width, current_height, current_depth;
static int fullscreen, needs_fullscreen;
static int current_pixbytes;

static int in_sizemove;

static int screen_is_picasso = 0;

int customsize = 0;

int bActive;
int toggle_sound;

int process_desired_pri;

BOOL viewing_child = FALSE;
BOOL running_winnt = FALSE;

static char scrlinebuf[8192];	/* this is too large, but let's rather play on the safe side here */
static int scrindirect;

static void set_linemem (void)
{
    if (scrindirect)
	gfxvidinfo.linemem = scrlinebuf;
    else
	gfxvidinfo.linemem = 0;
}

/* Keyboard emulation, Win32 helper routines. */
static LPARAM keysdown[256];
static short numkeysdown;

int checkkey (int vkey, LPARAM lParam)
{
    switch (vkey) {
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

    if (active) {
	ShowCursor (FALSE);
	SetCursorPos (mousecx, mousecy);
	SetWindowText (hMainWnd ? hMainWnd : hAmigaWnd, TITLETEXT " [Mouse active - press Alt-Tab to cancel]");
	ClipCursor (&amigawin_rect);
    } else {
	ShowCursor (TRUE);
	SetWindowText (hMainWnd ? hMainWnd : hAmigaWnd, TITLETEXT);
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

static __inline__ void releasecapture (void)
{
    if (!hascapture)
	return;
    hascapture--;
    ReleaseCapture ();
}

static void illhandler(int foo)
{
    rpt_available = 0;
}

static void figure_processor_speed (void)
{
    frame_time_t best_time;
    int i;

    rpt_available = 1;
    signal (SIGILL, illhandler);
    read_processor_time ();
    signal (SIGILL, SIG_DFL);
    if (! rpt_available) {
	fprintf (stderr, "Your processor does not support the RDTSC instruction.\n");
	return;
    }
    fprintf (stderr, "Calibrating delay loop.. ");
    fflush (stderr);
    best_time = (frame_time_t)-1;
    for (i = 0; i < 5; i++) {
	frame_time_t t = read_processor_time ();
	Sleep (1001);
	t = 2 * read_processor_time () - t;
	Sleep (1);
	t -= read_processor_time ();
	if (t < best_time)
	    best_time = t;
    }
    fprintf (stderr, "ok - %.2f BogoMIPS\n",
	     ((double)best_time / 1000000), best_time);
    vsynctime = best_time / 50;
}

/* DirectDraw stuff */
static char *getddrname (HRESULT ddrval)
{
    switch (ddrval) {
     case DDERR_ALREADYINITIALIZED:
	return "This object is already initialized.";
     case DDERR_CANNOTATTACHSURFACE:
	return "Cannot attach surface.";
     case DDERR_CANNOTDETACHSURFACE:
	return "Cannot detach surface.";
     case DDERR_CURRENTLYNOTAVAIL:
	return "Support unavailable.";
     case DDERR_EXCEPTION:
	return "Unexpected exception.";
     case DDERR_GENERIC:
	return "Undefined";	/* THIS MAKES SENSE!  FUCKING M$ */
     case DDERR_HEIGHTALIGN:
	return "Height needs to be aligned.";
     case DDERR_INCOMPATIBLEPRIMARY:
	return "New params doesn't match existing primary surface.";
     case DDERR_INVALIDCAPS:
	return "Device doesn't have requested capabilities.";
     case DDERR_INVALIDCLIPLIST:
	return "Provided clip-list not supported.";
     case DDERR_INVALIDMODE:
	return "Mode not supported.";
     case DDERR_INVALIDOBJECT:
	return "Invalid object.";
     case DDERR_INVALIDPARAMS:
	return "Invalid params.";
     case DDERR_INVALIDPIXELFORMAT:
	return "Device doesn't support requested pixel format.";
     case DDERR_INVALIDRECT:
	return "Invalid RECT.";
     case DDERR_LOCKEDSURFACES:
	return "Surface locked.";
     case DDERR_NO3D:
	return "No 3d capabilities.";
     case DDERR_NOALPHAHW:
	return "No alpha h/w.";
     case DDERR_NOCLIPLIST:
	return "No clip-list.";
     case DDERR_NOCOLORCONVHW:
	return "No colour-conversion h/w.";
     case DDERR_NOCOOPERATIVELEVELSET:
	return "No cooperative-level set.";
     case DDERR_NOCOLORKEY:
	return "No colour-key.";
     case DDERR_NOCOLORKEYHW:
	return "No colour-key hardware.";
     case DDERR_NODIRECTDRAWSUPPORT:
	return "No DirectDraw support with this display driver!";
     case DDERR_NOEXCLUSIVEMODE:
	return "Exlusive-mode needed but not set yet.";
     case DDERR_NOFLIPHW:
        return "No flipping hardware.";
     case DDERR_NOGDI:
        return "No GDI present.";
     case DDERR_NOMIRRORHW:
        return "No mirror hardware.";
     case DDERR_NOTFOUND:
        return "Requested item not found.";
     case DDERR_NOOVERLAYHW:
        return "No overlay hardware.";
     case DDERR_NORASTEROPHW:
        return "No raster-op hardware.";
     case DDERR_NOROTATIONHW:
        return "No rotation hardware.";
     case DDERR_NOSTRETCHHW:
        return "No stretch hardware.";
     case DDERR_NOT4BITCOLOR:
        return "Not a 4-bit colour palette.";
     case DDERR_NOT4BITCOLORINDEX:
        return "Not a 4-bit colour-index.";
     case DDERR_NOT8BITCOLOR:
        return "Not an 8-bit colour palette.";
     case DDERR_NOTEXTUREHW:
        return "No texture hardware.";
     case DDERR_BLTFASTCANTCLIP:
	return "Cannot use BLTFAST with Clipper attached to surface.";
     case DDERR_CANTCREATEDC:
	return "Cannot create DC Device Context.";
     case DDERR_CANTDUPLICATE:
	return "Cannot duplicate.";
     case DDERR_CANTLOCKSURFACE:
	return "Access to surface refused (no DCI Provider).";
     case DDERR_CANTPAGELOCK:
	return "PageLock failure.";
     case DDERR_CANTPAGEUNLOCK:
	return "PageUnlock failure.";
     case DDERR_CLIPPERISUSINGHWND:
	return "Can't set a clip-list for a Clipper which is attached to an HWND.";
     case DDERR_COLORKEYNOTSET:
	return "No source colour-key provided.";
     case DDERR_DCALREADYCREATED:
	return "Surface already has a Device Context.";
     case DDERR_DIRECTDRAWALREADYCREATED:
	return "DirectDraw already bound to this process.";
     case DDERR_EXCLUSIVEMODEALREADYSET:
	return "Already in exclusive mode.";
     case DDERR_HWNDALREADYSET:
	return "HWND already set for cooperative level.";
     case DDERR_HWNDSUBCLASSED:
	return "HWND has been subclassed.";
     case DDERR_IMPLICITLYCREATED:
	return "Can't restore an implicitly created surface.";
     case DDERR_INVALIDDIRECTDRAWGUID:
	return "Invalid GUID.";
     case DDERR_INVALIDPOSITION:
	return "Overlay position illegal.";
     case DDERR_INVALIDSURFACETYPE:
	return "Wrong type of surface.";
     case DDERR_NOBLTHW:
	return "No blit h/w.";
     case DDERR_NOCLIPPERATTACHED:
	return "No Clipper attached.";

     case DDERR_NOTLOCKED:
	return "Not locked.";
     case DDERR_NOTPAGELOCKED:
	return "Not page-locked.";
     case DDERR_NOTPALETTIZED:
	return "Not palette-based.";

     case DDERR_OUTOFCAPS:
	return "out of caps";
     case DDERR_OUTOFMEMORY:
	return "Out of memory.";
     case DDERR_OUTOFVIDEOMEMORY:
	return "out of video memory.";
     case DDERR_PALETTEBUSY:
	return "Palette busy.";
     case DDERR_PRIMARYSURFACEALREADYEXISTS:
	return "Already a primary surface.";

     case DDERR_SURFACEBUSY:
	return "Surface busy.";
	/*case DDERR_SURFACEOBSCURED:     return "Surface is obscured."; */
     case DDERR_SURFACELOST:
	return "Surface lost.";

     case DDERR_UNSUPPORTED:
	return "Unsupported.";
     case DDERR_UNSUPPORTEDFORMAT:
	return "Unsupported format.";

     case DDERR_WASSTILLDRAWING:
	return "Was still drawing.";

     case DDERR_WRONGMODE:
	return "Wrong Mode.";
    
    }
    return "";
}

static int lockcnt = 0;

static int do_surfacelock (void)
{
    HRESULT ddrval = IDirectDrawSurface_Lock (lpDDS, NULL, &ddsd, DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT, NULL);
    if (ddrval != DD_OK) {
	if (ddrval == DDERR_SURFACELOST)
	    ddrval = IDirectDrawSurface_Restore (lpDDS);
	else if (ddrval != DDERR_SURFACEBUSY)
	    write_log ("lpDDS->Lock() failed - %s (%d)\n", getddrname (ddrval), (unsigned short) ddrval);
	return 0;
    }
    lockcnt++;
    return 1;
}

void unlockscr (void)
{
    lockcnt--;
    IDirectDrawSurface_Unlock (lpDDS, ddsd.lpSurface);
}

/* Return one of the pixel formats declared in picasso96.h if the surface
 * is usable for us, or RGBFB_NONE if it is not usable.  */
static RGBFTYPE surface_pixelformat (DDSURFACEDESC *surface)
{
    DDPIXELFORMAT *pfp = &surface->ddpfPixelFormat;
    DWORD r, g, b;

    if ((surface->dwFlags & (DDSD_PIXELFORMAT | DDSD_WIDTH | DDSD_HEIGHT | DDSD_REFRESHRATE)) 
	!= (DDSD_PIXELFORMAT | DDSD_WIDTH | DDSD_HEIGHT | DDSD_REFRESHRATE))
	return RGBFB_NONE;

    if ((pfp->dwFlags & DDPF_RGB) == 0)
	return RGBFB_NONE;

    r = pfp->dwRBitMask;
    g = pfp->dwGBitMask;
    b = pfp->dwBBitMask;
    switch (pfp->dwRGBBitCount) {
     case 8:
	if ((pfp->dwFlags & DDPF_PALETTEINDEXED8) != 0)
	    return RGBFB_CHUNKY;
	break;

     case 16:
	if (r == 0xF800 && g == 0x07E0 && b == 0x001F)
	    return RGBFB_R5G6B5PC;
	if (r == 0x7C00 && g == 0x03E0 && b == 0x001F)
	    return RGBFB_R5G5B5PC;
	if (b == 0xF800 && g == 0x07E0 && r == 0x001F)
	    return RGBFB_B5G6R5PC;
	if (b == 0x7C00 && g == 0x03E0 && r == 0x001F)
	    return RGBFB_B5G5R5PC;
	/* This happens under NT - with r == b == g == 0 !!! */
	printf ("Unknown 16 bit format %d %d %d\n", r, g, b);
	break;

     case 24:
	if (r == 0xFF0000 && g == 0x00FF00 && b == 0x0000FF)
	    return RGBFB_B8G8R8;
	if (r == 0x0000FF && g == 0x00FF00 && b == 0xFF0000)
	    return RGBFB_R8G8B8;
	break;

     case 32:
	if (r == 0x00FF0000 && g == 0x0000FF00 && b == 0x000000FF)
	    return RGBFB_B8G8R8A8;
	if (r == 0x000000FF && g == 0x0000FF00 && b == 0x00FF0000)
	    return RGBFB_R8G8B8A8;
	if (r == 0xFF000000 && g == 0x00FF0000 && b == 0x0000FF00)
	    return RGBFB_A8B8G8R8;
	if (r == 0x0000FF00 && g == 0x00FF0000 && b == 0xFF000000)
	    return RGBFB_A8R8G8B8;
	break;
	
     default:
	printf ("Unknown %d bit format %d %d %d\n", pfp->dwRGBBitCount, r, g, b);
	break;
    }
    return RGBFB_NONE;
}

static int rgbformat_bits (RGBFTYPE t)
{
    unsigned long f = 1 << t;
    return ((f & RGBMASK_8BIT) != 0 ? 8
	    : (f & RGBMASK_15BIT) != 0 ? 15
	    : (f & RGBMASK_16BIT) != 0 ? 16
	    : (f & RGBMASK_24BIT) != 0 ? 24
	    : (f & RGBMASK_32BIT) != 0 ? 32
	    : 0);
}

static void release_ddraw (void)
{
#if 0
    if (lpDD2) {
	IDirectDraw2_RestoreDisplayMode (lpDD2);
	IDirectDraw2_SetCooperativeLevel (lpDD2, hAmigaWnd, DDSCL_NORMAL);
    }
#else
    if (lpDD) {
	IDirectDraw_RestoreDisplayMode (lpDD);
	IDirectDraw_SetCooperativeLevel (lpDD, hAmigaWnd, DDSCL_NORMAL);
    }
#endif

    if (lpDDC)
	IDirectDrawClipper_Release (lpDDC);
    if (lpDDS)
	IDirectDrawSurface_Release (lpDDS);
    if (lpDDP)
	IDirectDrawPalette_Release (lpDDP);
    if (lpDD2)
	IDirectDraw2_Release (lpDD2);
    if (lpDD)
	IDirectDraw_Release (lpDD);
    lpDDC = 0;
    lpDDS = 0;
    lpDDP = 0;
    lpDD2 = 0;
    lpDD = 0;
}


static int start_ddraw (void)
{
    HRESULT ddrval;

    ddrval = (*pDirectDrawCreate) (NULL, &lpDD, NULL);
    if (ddrval != DD_OK)
	goto oops;

#if 0
    ddrval = IDirectDraw_QueryInterface (lpDD, &IID_IDirectDraw2, (LPVOID *)&lpDD2);
    if (ddrval != DD_OK)
	goto oops;
#endif

    current_surface.dwSize = sizeof current_surface;
    ddrval = IDirectDraw_GetDisplayMode (lpDD, &current_surface);
    if (ddrval != DD_OK)
	goto oops;

    return 1;

  oops:
    write_log ("DirectDraw initialization failed with %s/%d\n", getddrname (ddrval), ddrval);
    release_ddraw ();
    return 0;
}

static int set_ddraw (int width, int height, int wantfull, int bits,
		      LPPALETTEENTRY pal)
{
    HRESULT ddrval;

    bits = (bits + 7) & ~7;
    
    ddrval = IDirectDraw_SetCooperativeLevel (lpDD, hAmigaWnd,
					      (wantfull
					       ? DDSCL_ALLOWREBOOT | DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN
					       : DDSCL_NORMAL));
    if (ddrval != DD_OK)
	goto oops;

    ddrval = IDirectDraw_CreateClipper (lpDD, 0, &lpDDC, NULL);
    if (ddrval != DD_OK)
	goto oops;

    ddrval = IDirectDrawClipper_SetHWnd (lpDDC, 0, hAmigaWnd);
    if (ddrval != DD_OK)
	goto oops;

    if (wantfull) {
	/* Hmmm...
	 * The "bits" parameter is somewhat suspicious.  What if a graphics
	 * card supports A8R8G8B8 and A8B8G8R8 modes? */
	printf ("Trying %dx%d, %d\n", width, height, bits);
	ddrval = IDirectDraw_SetDisplayMode (lpDD, width, height, bits);
	if (ddrval != DD_OK)
	    goto oops;
	printf ("ok\n");

	current_surface.dwSize = sizeof current_surface;
	ddrval = IDirectDraw_GetDisplayMode (lpDD, &current_surface);
	if (ddrval != DD_OK)
	    goto oops;
    }

    ddsd.dwSize = sizeof (ddsd);
    ddsd.dwFlags = DDSD_CAPS;
    ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;

    ddrval = IDirectDraw_CreateSurface (lpDD, &ddsd, &lpDDS, NULL);
    if (ddrval != DD_OK)
	goto oops;

    if (! do_surfacelock ())
	return 0;
    unlockscr ();

    current_pixbytes = (ddsd.ddpfPixelFormat.dwRGBBitCount + 7) >> 3;

    if (current_pixbytes == 1) {
	ddrval = IDirectDraw_CreatePalette (lpDD, DDPCAPS_8BIT | DDPCAPS_ALLOW256,
					    pal,
					    &lpDDP, NULL);
	if (ddrval != DD_OK)
	    goto oops;

	ddrval = IDirectDrawSurface2_SetPalette (lpDDS, lpDDP);
	if (ddrval != DD_OK)
	    goto oops;
    }

    return 1;

  oops:
    write_log ("DirectDraw initialization failed with %s/%d\n", getddrname (ddrval), ddrval);
    return 0;
}

/* Color management */

static xcolnr xcol8[4096];
static PALETTEENTRY colors256[256];
static int ncols256 = 0;

static int red_bits, green_bits, blue_bits;
static int red_shift, green_shift, blue_shift;

static int get_color (int r, int g, int b, xcolnr * cnp)
{
    if (ncols256 == 256)
	return 0;
    colors256[ncols256].peRed = r * 0x11;
    colors256[ncols256].peGreen = g * 0x11;
    colors256[ncols256].peBlue = b * 0x11;
    colors256[ncols256].peFlags = 0;
    *cnp = ncols256;
    ncols256++;
    return 1;
}

static void init_colors (void)
{
    int i;

    if (ncols256 == 0) {
	alloc_colors256 (get_color);
	memcpy (xcol8, xcolors, sizeof xcol8);
    }

    /* init colors */

    switch (current_pixbytes) {
    case 1:
	memcpy (xcolors, xcol8, sizeof xcolors);
	if (lpDDP != 0) {
	    HRESULT ddrval = IDirectDrawPalette_SetEntries (lpDDP, 0, 0, 256, colors256);
	    if (ddrval != DD_OK)
		write_log ("DX_SetPalette() failed with %s/%d\n", getddrname (ddrval), ddrval);
	}
	break;

    case 2:
    case 3:
    case 4:
	red_bits = bits_in_mask (ddsd.ddpfPixelFormat.dwRBitMask);
	green_bits = bits_in_mask (ddsd.ddpfPixelFormat.dwGBitMask);
	blue_bits = bits_in_mask (ddsd.ddpfPixelFormat.dwBBitMask);
	red_shift = mask_shift (ddsd.ddpfPixelFormat.dwRBitMask);
	green_shift = mask_shift (ddsd.ddpfPixelFormat.dwGBitMask);
	blue_shift = mask_shift (ddsd.ddpfPixelFormat.dwBBitMask);

	alloc_colors64k (red_bits, green_bits, blue_bits, red_shift,
			 green_shift, blue_shift);
	break;
    }
    switch (gfxvidinfo.pixbytes) {
     case 2:
	for (i = 0; i < 4096; i++)
	    xcolors[i] = xcolors[i] * 0x00010001;
	gfxvidinfo.can_double = 1;
	break;
     case 1:
	for (i = 0; i < 4096; i++)
	    xcolors[i] = xcolors[i] * 0x01010101;
	gfxvidinfo.can_double = 1;
	break;
     default:
	gfxvidinfo.can_double = 0;
	break;
    }
}

static void close_windows (void)
{
    gfxvidinfo.bufmem = 0;
    gfxvidinfo.linemem = 0;

    releasecapture ();
    setmouseactive (0);
    ClipCursor (NULL);
    release_ddraw ();
    dsound_newwindow (0);

    if (hAmigaWnd)
	DestroyWindow (hAmigaWnd);
    if (hStatusWnd)
	DestroyWindow (hStatusWnd);
    if (hMainWnd)
	DestroyWindow (hMainWnd);

    hMainWnd = 0;
    hStatusWnd = 0;
    hAmigaWnd = 0;
}

void toggle_fullscreen (void)
{
    if (needs_fullscreen)
	return;

    display_change_requested = 1;
    if (screen_is_picasso)
	currprefs.gfx_pfullscreen ^= 1;
    else
	currprefs.gfx_afullscreen ^= 1;    
}

static long FAR PASCAL AmigaWindowProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hDC;

    switch (message) {
     case WM_ACTIVATEAPP:
	if (bActive = wParam) {
	    if (fullscreen) {
		SetCursor (NULL);
		SetCursorPos (mousecx, mousecy);
	    }
	    my_kbd_handler (VK_CAPITAL, 0x3a, TRUE);
	} else {
	    if (!fullscreen)
		setmouseactive (0);
	}
	break;

     case WM_ACTIVATE:
	if (LOWORD (wParam) != WA_INACTIVE) {
	    ShowWindow (hWnd, SW_RESTORE);
	}
	break;

     case WM_SETCURSOR:
	if (fullscreen) {
	    SetCursor (NULL);
	    return TRUE;
	}
	break;

     case WM_SYSCOMMAND:
	if (wParam == SC_ZOOM) {
	    toggle_fullscreen ();
	    return 0;
	}
	break;

     case WM_KEYUP:
     case WM_SYSKEYUP:
	numkeysdown--;
	keysdown[wParam] = 0;
	my_kbd_handler (wParam, (lParam >> 16) & 0x1ff, FALSE);
	break;

     case WM_KEYDOWN:
     case WM_SYSKEYDOWN:
	if (LOWORD (lParam) == 1) {
	    if (numkeysdown) {
		int key;
		numkeysdown = 0;

		for (key = 256; key--;) {
		    if (keysdown[key]) {
			if (checkkey (key, lParam))
			    numkeysdown++;
			else {
			    my_kbd_handler (key, (keysdown[key] >> 16) & 0x1ff, FALSE);
			    keysdown[key] = 0;
			}
		    }
		}
	    }
	    if (!keysdown[wParam]) {
		keysdown[wParam] = lParam;
		numkeysdown++;
	    }
	    numkeysdown++;
	    my_kbd_handler (wParam, (lParam >> 16) & 0x1ff, TRUE);
	}
	break;

     case WM_LBUTTONDOWN:
	if (ievent_alive) {
	    setcapture ();
	    buttonstate[0] = 1;
	} else if (!fullscreen && !mouseactive)
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

     case WM_MOUSEMOVE:
	if ((mouseactive && !ievent_alive) || fullscreen) {
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
	    if (ievent_alive) {
		if (lastmx < 0)
		    lastmx = 0;
		if (lastmx > current_width)
		    lastmx = current_width;
		if (lastmy < 0)
		    lastmy = 0;
		if (lastmy > current_height)
		    lastmy = current_height;

	    }
	    SetCursorPos (mousecx, mousecy);
	    break;
	}
	lastmx = (signed short) LOWORD (lParam);
	lastmy = (signed short) HIWORD (lParam);
	break;

     case WM_PAINT:
	clear_inhibit_frame (IHF_WINDOWHIDDEN);
	hDC = BeginPaint (hWnd, &ps);
	EndPaint (hWnd, &ps);
	notice_screen_contents_lost ();
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

     case WM_USER + 0x200:
	DoSomeWeirdPrintingStuff (wParam);
	break;

     case WM_CREATE:
	DragAcceptFiles (hWnd, TRUE);
	break;

     case WM_CLOSE:
	uae_quit ();
	return 0;

     case WM_WINDOWPOSCHANGED:
	GetWindowRect (hAmigaWnd, &amigawin_rect);
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
     case WM_MOUSEMOVE:
     case WM_ACTIVATEAPP:
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
     case WM_DROPFILES:
     case WM_CREATE:
     case WM_DESTROY:
     case WM_USER + 0x200:
     case WM_CLOSE:
	return AmigaWindowProc (hWnd, message, wParam, lParam);

     case WM_DISPLAYCHANGE:
	if (!fullscreen && (wParam + 7) / 8 != current_pixbytes)
	    display_change_requested = 1;
	break;
	
     case WM_ENTERSIZEMOVE:
	in_sizemove++;
	break;

     case WM_EXITSIZEMOVE:
	in_sizemove--;

	/* fall through */

     case WM_WINDOWPOSCHANGED:
	GetWindowRect (hAmigaWnd, &amigawin_rect);

	if (in_sizemove > 0)
	    break;

	if (!fullscreen && hAmigaWnd) {
	    if (amigawin_rect.left & 3) {
		RECT rc2;
		GetWindowRect (hMainWnd, &rc2);
		if (1 /*!mon || rc2.left + 4 < GetSystemMetrics (SM_CXSCREEN) */ ) {
		    MoveWindow (hMainWnd, rc2.left + 4 - amigawin_rect.left % 4, rc2.top,
				rc2.right - rc2.left, rc2.bottom - rc2.top, TRUE);
		}
	    }

	    setmouseactive (0);
	    return 0;
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
	    toggle_fullscreen ();
	    return 0;
	}
	break;
    }

    return DefWindowProc (hWnd, message, wParam, lParam);
}

static HANDLE debugfile;

/* Console Win32 helper routines */
void activate_debugger ();

static BOOL __stdcall ctrlchandler (DWORD type)
{
    SetConsoleCtrlHandler ((PHANDLER_ROUTINE) ctrlchandler, FALSE);

    if (type == CTRL_C_EVENT) {
	activate_debugger ();
	return TRUE;
    }
    return FALSE;
}

void setup_brkhandler (void)
{
    SetConsoleCtrlHandler ((PHANDLER_ROUTINE) ctrlchandler, TRUE);
}

void remove_brkhandler (void)
{
    SetConsoleCtrlHandler ((PHANDLER_ROUTINE) ctrlchandler, FALSE);
}

static int register_classes (void)
{
    WNDCLASS wc;

    wc.style = 0;
    wc.lpfnWndProc = AmigaWindowProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = 0;
    wc.hIcon = LoadIcon (GetModuleHandle (NULL), IDI_APPICON);
    wc.hCursor = LoadCursor (NULL, IDC_ARROW);
    wc.hbrBackground = GetStockObject (BLACK_BRUSH);
    wc.lpszMenuName = 0;
    wc.lpszClassName = "AmigaPowah";
    if (!RegisterClass (&wc))
	return 0;

    wc.style = 0;
    wc.lpfnWndProc = MainWindowProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = 0;
    wc.hIcon = LoadIcon (GetModuleHandle (NULL), IDI_APPICON);
    wc.hCursor = LoadCursor (NULL, IDC_ARROW);
    wc.hbrBackground = GetStockObject (BLACK_BRUSH);
    wc.lpszMenuName = 0;
    wc.lpszClassName = "PCsuxRox";
    if (!RegisterClass (&wc))
	return 0;
    return 1;
}

struct win32_displaymode *win32_displaymode_list;

static HRESULT CALLBACK modesCallback (LPDDSURFACEDESC modeDesc, LPVOID context)
{
    struct win32_displaymode **dmpp;
    RGBFTYPE colortype;

    colortype = surface_pixelformat (modeDesc);
    if (colortype == RGBFB_NONE)
	return DDENUMRET_OK;

    dmpp = &win32_displaymode_list;
    while (*dmpp != 0) {
	if ((*dmpp)->width == modeDesc->dwWidth
	    && (*dmpp)->height == modeDesc->dwHeight
	    && (*dmpp)->refreshrate == modeDesc->dwRefreshRate)
	    break;
	dmpp = &(*dmpp)->next;
    }

    if (*dmpp == 0) {
	*dmpp = (struct win32_displaymode *)malloc (sizeof **dmpp);
	(*dmpp)->next = 0;
	(*dmpp)->width = modeDesc->dwWidth;
	(*dmpp)->height = modeDesc->dwHeight;
	(*dmpp)->refreshrate = modeDesc->dwRefreshRate;
	(*dmpp)->colormodes = 0;
    }
    (*dmpp)->colormodes |= 1 << colortype;
    return DDENUMRET_OK;
}

static int our_possible_depths[] = { 8, 15, 16, 24, 32 };

static void figure_pixel_formats (void)
{
    HWND tmpw;
    HRESULT ddrval;
    struct win32_displaymode *dm;

    tmpw = CreateWindowEx (WS_EX_TOPMOST,
			   "AmigaPowah", PROGNAME,
			   WS_VISIBLE | WS_POPUP,
			   CW_USEDEFAULT, CW_USEDEFAULT,
			   GetSystemMetrics (SM_CXSCREEN),
			   GetSystemMetrics (SM_CYSCREEN),
			   0, NULL, 0, NULL);

    ddrval = IDirectDraw_SetCooperativeLevel (lpDD, tmpw,
					      DDSCL_ALLOWREBOOT | DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN);
    if (ddrval != DD_OK) {
	printf ("error %s\n", getddrname (ddrval));
	goto out;
    }

    for (dm = win32_displaymode_list; dm != 0; dm = dm->next) {
	int i;

	printf ("Attempting %dx%d: ", dm->width, dm->height);
	dm->colormodes = 0;

	for (i = 0; i < 5; i++) {
	    RGBFTYPE colortype;
	    ddrval = IDirectDraw_SetDisplayMode (lpDD, dm->width, dm->height, our_possible_depths[i]);
	    if (ddrval != DD_OK)
		continue;

	    current_surface.dwSize = sizeof current_surface;
	    ddrval = IDirectDraw_GetDisplayMode (lpDD, &current_surface);
	    if (ddrval != DD_OK)
		continue;
	    colortype = surface_pixelformat (&current_surface);
	    if (colortype != RGBFB_NONE) {
		printf ("%d ", our_possible_depths[i]);
		dm->colormodes |= 1 << colortype;
	    }
	}
	printf ("(%08lx)\n", dm->colormodes);
    }
    out:
    DestroyWindow (tmpw);
}

#define NORMAL_WINDOW_STYLE (WS_VISIBLE | WS_BORDER | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX)

#define NUM_PARTS 6
#define LED_WIDTH 50
#define DRIVE_WIDTH 16

static int create_windows (void)
{
    if (!fullscreen) {
	RECT rc;
	HLOCAL hloc;
	LPINT lpParts;

	rc.left = 0;
	rc.top = 0;
	rc.right = current_width;
	rc.bottom = current_height + GetSystemMetrics (SM_CYMENU);
	rc.right += 4;
	rc.bottom += 4;
	AdjustWindowRect (&rc, NORMAL_WINDOW_STYLE, FALSE);
	printf ("(%d %d) (%d %d)\n", rc.left, rc.top, rc.right, rc.bottom);

	hMainWnd = CreateWindowEx (WS_EX_ACCEPTFILES, "PCsuxRox", TITLETEXT,
				   NORMAL_WINDOW_STYLE, CW_USEDEFAULT, CW_USEDEFAULT,
				   rc.right - rc.left, rc.bottom - rc.top,
				   NULL, NULL, 0, NULL);

	if (! hMainWnd)
	    return 0;
	hStatusWnd = CreateStatusWindow (WS_CHILD | WS_VISIBLE, "", hMainWnd, 1);
	if (hStatusWnd) {
	    GetClientRect (hMainWnd, &rc);
	    /* Allocate an array for holding the right edge coordinates. */
	    hloc = LocalAlloc (LHND, sizeof (int) * NUM_PARTS);
	    if (hloc) {
		lpParts = LocalLock (hloc);

		/* Calculate the right edge coordinate for each part, and copy the coords
		 * to the array.  */
		lpParts[0] = rc.right - (DRIVE_WIDTH * 4) - LED_WIDTH - 2;
		lpParts[1] = lpParts[0] + LED_WIDTH;
		lpParts[2] = lpParts[1] + DRIVE_WIDTH;
		lpParts[3] = lpParts[2] + DRIVE_WIDTH;
		lpParts[4] = lpParts[3] + DRIVE_WIDTH;
		lpParts[5] = lpParts[4] + DRIVE_WIDTH;

		/* Create the six parts */
		SendMessage (hStatusWnd, SB_SETPARTS, (WPARAM) NUM_PARTS, (LPARAM) lpParts);

		LocalUnlock (hloc);
		LocalFree (hloc);
	    }
	}
    } else
	hMainWnd = NULL;

    hAmigaWnd = CreateWindowEx (fullscreen ? WS_EX_TOPMOST : WS_EX_ACCEPTFILES,
				"AmigaPowah", PROGNAME,
				hMainWnd ? WS_VISIBLE | WS_CHILD : WS_VISIBLE | WS_POPUP,
				hMainWnd ? 2 : CW_USEDEFAULT, hMainWnd ? 2 : CW_USEDEFAULT,
				fullscreen ? GetSystemMetrics (SM_CXSCREEN) : current_width,
				fullscreen ? GetSystemMetrics (SM_CYSCREEN) : current_height,
				hMainWnd, NULL, 0, NULL);

    
    if (! hAmigaWnd) {
	if (hMainWnd)
	    DestroyWindow (hMainWnd);
	return 0;
    }

    if (hMainWnd)
	UpdateWindow (hMainWnd);
    if (hAmigaWnd)
	UpdateWindow (hAmigaWnd);

    return 1;
}

/* DirectX will fail with "Mode not supported" if we try to switch to a full
 * screen mode that doesn't match one of the dimensions we got during enumeration.
 * So try to find a best match for the given resolution in our list.  */
static int adjust_screenmode (int *pwidth, int *pheight, int *ppixbits)
{
    struct win32_displaymode *best;
    uae_u32 selected_mask = (*ppixbits == 8 ? RGBMASK_8BIT
			     : *ppixbits == 15 ? RGBMASK_15BIT
			     : *ppixbits == 16 ? RGBMASK_16BIT
			     : *ppixbits == 24 ? RGBMASK_24BIT
			     : RGBMASK_32BIT);
    int pass;
    
    for (pass = 0; pass < 2; pass++) {
	struct win32_displaymode *dm;
	uae_u32 mask = (pass == 0
			? selected_mask
			: RGBMASK_8BIT | RGBMASK_16BIT | RGBMASK_24BIT | RGBMASK_32BIT);

	best = win32_displaymode_list;
	dm = best->next;

	while (dm != 0) {
	    if ((dm->colormodes & mask) != 0) {
		if (dm->width <= best->width && dm->height <= best->height
		    && dm->width >= *pwidth && dm->height >= *pheight)
		    best = dm;
		if (dm->width >= best->width && dm->height >= best->height
		    && dm->width <= *pwidth && dm->height <= *pheight)
		    best = dm;
	    }
	    dm = dm->next;
	}
	if (best->width == *pwidth && best->height == *pheight)
	    break;
    }
    *pwidth = best->width;
    *pheight = best->height;
    if ((best->colormodes & selected_mask) != 0)
	return 1;
    if (best->colormodes & RGBMASK_8BIT)
	*ppixbits = 8;
    else if (best->colormodes & RGBMASK_16BIT)
	*ppixbits = 16;
    else if (best->colormodes & RGBMASK_32BIT)
	*ppixbits = 32;
    else if (best->colormodes & RGBMASK_24BIT)
	*ppixbits = 24;

    return 1;
}

static BOOL doInit (void)
{
    if (! create_windows ())
	goto oops;

    if (screen_is_picasso) {
	if (! set_ddraw (current_width, current_height, fullscreen, current_depth,
			(LPPALETTEENTRY) & picasso96_state.CLUT))
	    goto oops;
	picasso_vidinfo.rowbytes = ddsd.lPitch;
	picasso_vidinfo.pixbytes = current_pixbytes;
	picasso_vidinfo.rgbformat = surface_pixelformat (&current_surface);
    } else {
	if (fullscreen)
	    if (! adjust_screenmode (&current_width, &current_height, &current_depth))
		abort ();

	if (! set_ddraw (current_width, current_height, fullscreen, current_depth, colors256))
	    goto oops;
	gfxvidinfo.bufmem = 0;
	gfxvidinfo.linemem = 0;
	gfxvidinfo.maxblocklines = 0;
	gfxvidinfo.pixbytes = current_pixbytes;
	gfxvidinfo.width = current_width;
	gfxvidinfo.height = current_height;
	gfxvidinfo.rowbytes = ddsd.lPitch;
    }

    if (fullscreen) {
	scrindirect = 0;
	gfxvidinfo.linemem = 0;
	mousecx = 160, mousecy = 100, mousedx = 160, mousedy = 100, mousecl = MAKELONG (160, 100);
    }
    if (! do_surfacelock ())
	goto oops;
    unlockscr ();

    if ((ddsd.ddpfPixelFormat.dwFlags & (DDPF_RGB | DDPF_PALETTEINDEXED8 | DDPF_RGBTOYUV)) != 0) {
	write_log ("%s mode (bits: %d, pixbytes: %d)\n", fullscreen ? "Full screen" : "Window",
		   ddsd.ddpfPixelFormat.dwRGBBitCount, current_pixbytes);
    } else {
	write_log ("Error: Unsupported pixel format - use a different screen mode\n");
	goto oops;
    }

    init_colors ();

    if (! fullscreen)
	MainWindowProc (0, WM_WINDOWPOSCHANGED, 0, 0);
    dsound_newwindow (hAmigaWnd);
    return 1;

  oops:
    release_ddraw ();
    if (hMainWnd)
	DestroyWindow (hMainWnd);
    if (hAmigaWnd)
	DestroyWindow (hAmigaWnd);
    return 0;
}

struct myRGNDATA {
    RGNDATAHEADER rdh;
    RECT rects[640];		/* fixed buffers suck, but this is _very_ unlikely to overflow */
} ClipList = { { sizeof (ClipList), RDH_RECTANGLES, 0, 640 * sizeof (RECT) } };

/* this is the way the display line is put to screen
 * if the display is not 16 bits deep or the window is not fully visible */
static void clipped_linetoscr (char *dst, char *src, int y)
{
    LPRECT lpRect = ClipList.rects;
    int i;

    switch (current_pixbytes) {
     case 1:
	for (i = ClipList.rdh.nCount; i--; lpRect++) {
	    if (y >= lpRect->top && y < lpRect->bottom)
		memcpy (dst + lpRect->left, src + lpRect->left, lpRect->right);
	}
	break;

     case 2:
	for (i = ClipList.rdh.nCount; i--; lpRect++) {
	    if (y >= lpRect->top && y < lpRect->bottom)
		memcpy (dst + lpRect->left * 2, src + lpRect->left * 2, lpRect->right * 2);
	}
	break;

     case 3:
	for (i = ClipList.rdh.nCount; i--; lpRect++) {
	    if (y >= lpRect->top && y < lpRect->bottom)
		memcpy (dst + lpRect->left * 3, src + lpRect->left * 3, lpRect->right * 3);
	}
	break;

     case 4:
	for (i = ClipList.rdh.nCount; i--; lpRect++) {
	    if (y >= lpRect->top && y < lpRect->bottom)
		memcpy (dst + lpRect->left * 4, src + lpRect->left * 4, lpRect->right * 4);
	}
	break;
    }
}

void flush_line (int lineno)
{
    if (scrindirect)
	clipped_linetoscr (gfxvidinfo.bufmem + lineno * gfxvidinfo.rowbytes,
			   scrlinebuf, lineno);
}

void flush_block (int a, int b)
{
}

void flush_screen (int a, int b)
{
}

static uae_u8 *dolock (void)
{
    char *surface = NULL, *oldsurface;
    DWORD tmp;
    LPRECT lpRect;

    if (! do_surfacelock ())
	return 0;

    surface = ddsd.lpSurface;
    oldsurface = gfxvidinfo.bufmem;
    if (! fullscreen) {
	surface += amigawin_rect.top * ddsd.lPitch + current_pixbytes * amigawin_rect.left;
    }
    gfxvidinfo.bufmem = surface;
    if (surface != oldsurface && ! screen_is_picasso) {
	write_log ("Need to init_row_map\n");
	init_row_map ();
    }
    scrindirect = 0;

    if (fullscreen) {
	set_linemem ();
	clear_inhibit_frame (IHF_WINDOWHIDDEN);
	return surface;
    }
    tmp = sizeof (ClipList.rects);

    /* This is the VERY instruction that drags other threads (input/file system) down when in windowed
     * mode - WHY can't Microsoft implement the IsClipListChanged() method as documented? ARGH! */
    if (IDirectDrawClipper_GetClipList (lpDDC, NULL, (LPRGNDATA) & ClipList, &tmp) == DD_OK) {
	lpRect = ClipList.rects;

	if (!ClipList.rdh.nCount) {
	    set_inhibit_frame (IHF_WINDOWHIDDEN);
	    unlockscr ();
	    return 0;
	}
	if (ClipList.rdh.nCount != 1
	    || lpRect->right - lpRect->left != current_width
	    || lpRect->bottom - lpRect->top != current_height)
	{
	    scrindirect = 1;
	    for (tmp = ClipList.rdh.nCount; tmp--; lpRect++) {
		lpRect->left -= amigawin_rect.left;
		lpRect->right -= amigawin_rect.left;
		lpRect->top -= amigawin_rect.top;
		lpRect->bottom -= amigawin_rect.top;

		lpRect->right -= lpRect->left;
	    }
	}
    }
    set_linemem ();
    clear_inhibit_frame (IHF_WINDOWHIDDEN);
    return surface;
}

int lockscr (void)
{
    return dolock () != 0;
}

uae_u8 *gfx_lock_picasso (void)
{
    return dolock ();
}

void gfx_unlock_picasso (void)
{
    unlockscr ();
}

static int open_windows (void)
{
    char *fs_warning = 0;
    RGBFTYPE colortype;

    current_pixbytes = 0;

    in_sizemove = 0;
    fixup_prefs_dimensions (&currprefs);

    if (! start_ddraw ())
	return 0;

    colortype = surface_pixelformat (&current_surface);
    printf ("Ct: %08lx, picasso_vidinfo.selected_rgbformat %08lx\n", (unsigned long)colortype,
	    picasso_vidinfo.selected_rgbformat);

    if (screen_is_picasso) {
	fullscreen = currprefs.gfx_pfullscreen;
	current_width = picasso_vidinfo.width;
	current_height = picasso_vidinfo.height;
	current_depth = rgbformat_bits (picasso_vidinfo.selected_rgbformat);
    } else {
	fullscreen = currprefs.gfx_afullscreen;
	current_width = currprefs.gfx_width;
	current_height = currprefs.gfx_height;
	current_depth = (currprefs.color_mode == 0 ? 8
			 : currprefs.color_mode == 1 ? 15
			 : currprefs.color_mode == 2 ? 16
			 : currprefs.color_mode == 3 ? 8
			 : currprefs.color_mode == 4 ? 8 : 32);
    }

    needs_fullscreen = 0;
    if (colortype == RGBFB_NONE) {
	needs_fullscreen = 1;
	fs_warning = "the desktop is running in an unknown color mode.";
    } else if (colortype == RGBFB_CLUT) {
	needs_fullscreen = 1;
	fs_warning = "the desktop is running in 8 bit color depth, which UAE can't use in windowed mode.";
    } else if (current_width > current_surface.dwWidth || current_height > current_surface.dwHeight) {
	needs_fullscreen = 1;
	fs_warning = "the desktop is too small for the specified window size.";
    } else if (screen_is_picasso
	       && picasso_vidinfo.selected_rgbformat != RGBFB_CHUNKY
	       && picasso_vidinfo.selected_rgbformat != colortype)
    {
	needs_fullscreen = 1;
	fs_warning = "you selected a Picasso display with a color depth different from that of the desktop.";
    }

    if (needs_fullscreen && ! fullscreen) {
	char tmpstr[300];
	fullscreen = needs_fullscreen;
	/* Temporarily drop the DirectDraw stuff.  This is necessary, otherwise
	 * WinNT will just return 1 for the message box without ever displaying 
	 * it on the screen.  */
	release_ddraw ();
	sprintf (tmpstr, "The selected screen mode can't be displayed in a window, because %s\n"
		 "Switching to full-screen display.", fs_warning);
	MessageBox (0, tmpstr, "UAE", MB_ICONEXCLAMATION | MB_OK);
	start_ddraw ();
    }
    
    if (! fullscreen)
	current_depth = rgbformat_bits (colortype);
    
    if (! doInit ())
	return 0;

    return 1;
}

void handle_events (void)
{
    MSG msg;

    while (PeekMessage (&msg, 0, 0, 0, PM_REMOVE)) {
	TranslateMessage (&msg);
	DispatchMessage (&msg);
    }
}

int check_prefs_changed_gfx (void)
{
    if (display_change_requested) {
	display_change_requested = 0;
	close_windows ();
	open_windows ();
	return 1;
    }
    return 0;
}

/* this truly sucks, I'll include a native gunzip() routine soon */
int gunzip_hack (const char *src, const char *dst)
{
    char buf[1024];
    STARTUPINFO si =
    {sizeof si};
    PROCESS_INFORMATION pi;

    strcpy (buf, dst);
    strcat (buf, ".gz");

    if (CopyFile (src, buf, FALSE)) {
	sprintf (buf, "gzip -f -d \"%s.gz\"", dst);
	si.dwFlags = STARTF_USESTDHANDLES;
	if (CreateProcess (NULL, buf, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi)) {
	    WaitForSingleObject (pi.hProcess, INFINITE);
	    return -1;
	} else {
	    write_log ("Error: You need gzip.exe (32 bit) to use .adz/.roz files!\n");
	}
    }
    return 0;
}

static OPENFILENAME ofn =
{
    sizeof (OPENFILENAME),
    NULL, NULL, "Amiga Disk Files\000*.adf;*.adz\000",
    NULL, 0, 0, 0, 256, NULL, 0, "",
    0,
    OFN_NOCHANGEDIR | OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY,
    0,
    0,
    "adf",
    0,
    NULL,
    NULL
};

int requestfname (char *title, char *name)
{
    char *result;

    ofn.hwndOwner = hAmigaWnd;
    ofn.lpstrTitle = title;
    ofn.lpstrFile = name;

    if (pGetOpenFileNameA == 0)
	return 0;

    if (!fullscreen)
	setmouseactive (0);

    if (title)
	result = (char *) ((*pGetOpenFileNameA) (&ofn));
    else {
	GetSettings (0);
    }

    if (!fullscreen || !mouseactive)
	SetCursor (NULL);

    notice_screen_contents_lost ();

    if (result)
	return 1;

    return 0;
}

int DisplayGUI (void)
{
#ifdef PICASSO96
    BITMAPINFO bminfo =
    {
	{
	    sizeof (BITMAPINFOHEADER),
	    0, 0,
	    1, 8, BI_RGB, 0, 0, 0, 0, 0,
	},
	0
    };
#endif

    bminfo.bmiHeader.biWidth = current_width;
    bminfo.bmiHeader.biHeight = -current_height;
#if 0
    /* ??????? */
    bminfo.bmiColors[0] = &picasso96_state.CLUT;
#endif
    if (pGetOpenFileNameA) {
	if (lpDDP)
	    IDirectDrawSurface_SetPalette (lpDDS, NULL);

	if (!fullscreen)
	    setmouseactive (FALSE);
	GetSettings (0);

	if (lpDDP)
	    IDirectDrawSurface2_SetPalette (lpDDS, lpDDP);

	if (!fullscreen || !mouseactive)
	    SetCursor (NULL);
    }
    return 0;
}

#ifdef __GNUC__
#undef WINAPI
#define WINAPI
#endif

static HINSTANCE hDDraw = NULL, hComDlg32 = NULL, hRichEdit = NULL;

static int cleanuplibs (void)
{
    if (hRichEdit)
	FreeLibrary (hRichEdit);
    if (hDDraw)
	FreeLibrary (hDDraw);
    if (hComDlg32)
	FreeLibrary (hComDlg32);
    return 1;
}

/* try to load COMDLG32 and DDRAW, initialize csDraw, try to obtain the system clock frequency
 * from the registry, try to find out if we are running on a Pentium */
static int initlibs (void)
{
    OSVERSIONINFO osinfo;
    /* Figure out which Win32 OS we're running under */
    osinfo.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
    if (GetVersionEx (&osinfo)) {
	if (osinfo.dwPlatformId == VER_PLATFORM_WIN32_NT)
	    running_winnt = TRUE;
	else
	    running_winnt = FALSE;
    }
    figure_processor_speed ();
    /* Make sure we do an InitCommonControls() to get some advanced controls */
    InitCommonControls ();

    if (hComDlg32 = LoadLibrary ("COMDLG32.DLL")) {
	pGetOpenFileNameA = (BOOL (WINAPI *) (LPOPENFILENAME)) GetProcAddress (hComDlg32, "GetOpenFileNameA");
    } else
	/* System administrator? ROFL! -- Bernd */
	write_log ("COMDLG32.DLL not available. Please contact your system administrator.\n");

    /* LoadLibrary the richedit control stuff */
    if ((hRichEdit = LoadLibrary ("RICHED32.DLL")) == NULL) {
	write_log ("RICHED32.DLL not available. Please contact your system administrator.\n");
    }
    hDDraw = LoadLibrary ("DDRAW.DLL");
    if (hDDraw == 0) {
	write_log ("You have to install DirectX on your system before you can use UAE.\n"
		   "Refer to the documentation for further details.\n");
	return 0;
    }
    pDirectDrawCreate = (HRESULT (WINAPI *) (GUID FAR *, LPDIRECTDRAW FAR *, IUnknown FAR *)) GetProcAddress (hDDraw, "DirectDrawCreate");

    process_desired_pri = IDLE_PRIORITY_CLASS;

    return 1;
}

void write_log (const char *format,...)
{
    DWORD numwritten;
    char buffer[512];
    va_list parms;

    va_start (parms, format);
    vsprintf (buffer, format, parms);
    va_end (parms);

    fprintf (stderr, "%s", buffer);

    WriteFile (debugfile, buffer, strlen (buffer), &numwritten, NULL);
}

int debuggable (void)
{
    return 1;
}

int needmousehack (void)
{
    return 1;
}

void LED (int a)
{
}

int DX_FillResolutions (uae_u16 * ppixel_format)
{
    struct win32_displaymode *dm;
    int count = 0;

    *ppixel_format = 0;
    for (dm = win32_displaymode_list; dm != 0; dm = dm->next) {
	*ppixel_format |= dm->colormodes;
	if (dm->colormodes & RGBMASK_8BIT) {
	    DisplayModes[count].res.width = dm->width;
	    DisplayModes[count].res.height = dm->height;
	    DisplayModes[count].refresh = 75;
	    DisplayModes[count].depth = 1;
	    count++;
	}
	if (count >= MAX_PICASSO_MODES)
	    break;
	if (dm->colormodes & (RGBMASK_16BIT | RGBMASK_15BIT)) {
	    DisplayModes[count].res.width = dm->width;
	    DisplayModes[count].res.height = dm->height;
	    DisplayModes[count].refresh = 75;
	    DisplayModes[count].depth = 2;
	    count++;
	}
	if (count >= MAX_PICASSO_MODES)
	    break;
	if (dm->colormodes & RGBMASK_24BIT) {
	    DisplayModes[count].res.width = dm->width;
	    DisplayModes[count].res.height = dm->height;
	    DisplayModes[count].refresh = 75;
	    DisplayModes[count].depth = 3;
	    count++;
	}
	if (count >= MAX_PICASSO_MODES)
	    break;
	if (dm->colormodes & RGBMASK_32BIT) {
	    DisplayModes[count].res.width = dm->width;
	    DisplayModes[count].res.height = dm->height;
	    DisplayModes[count].refresh = 75;
	    DisplayModes[count].depth = 4;
	    count++;
	}
	if (count >= MAX_PICASSO_MODES)
	    break;
    }
    return count;
}

void DX_SetPalette (int start, int count)
{
    HRESULT ddrval;

    if (! screen_is_picasso || picasso96_state.RGBFormat != RGBFB_CHUNKY)
	return;

    if (picasso_vidinfo.pixbytes != 1) {
	printf ("DX Setpalette emulation\n");
	/* This is the case when we're emulating a 256 color display.  */
	while (count-- > 0) {
	    int r = picasso96_state.CLUT[start].Red;
	    int g = picasso96_state.CLUT[start].Green;
	    int b = picasso96_state.CLUT[start].Blue;
	    picasso_vidinfo.clut[start++] = (doMask256 (r, red_bits, red_shift)
					     | doMask256 (g, green_bits, green_shift)
					     | doMask256 (b, blue_bits, blue_shift));
	}
	return;
    }

    /* Set our DirectX palette here */
    if (lpDDP && current_pixbytes == 1) {
	/* For now, until I figure this out, just set the entire range of CLUT values */
	ddrval = IDirectDrawPalette_SetEntries (lpDDP, 0, start, count, (LPPALETTEENTRY) & (picasso96_state.CLUT[start]));
	if (ddrval != DD_OK)
	    write_log ("DX_SetPalette() failed with %s/%d\n", getddrname (ddrval), ddrval);
	else
	    printf ("DX_SetPalette OK\n");
    } else
	write_log ("ERROR - DX_SetPalette() doesn't have palette, or isn't Chunky mode.\n");
}

void DX_Invalidate (int first, int last)
{
}

int DX_BitsPerCannon (void)
{
    return 8;
}

int DX_FillRect (uaecptr addr, uae_u16 X, uae_u16 Y, uae_u16 Width, uae_u16 Height, uae_u32 Pen, uae_u8 Bpp)
{
    return 0;
}

void gfx_set_picasso_state (int on)
{
    if (screen_is_picasso == on)
	return;

    screen_is_picasso = on;
    close_windows ();
    open_windows ();
    DX_SetPalette (0, 256);
}

void gfx_set_picasso_modeinfo (int w, int h, int depth, int rgbfmt)
{
    depth >>= 3;
    if (picasso_vidinfo.width == w
	&& picasso_vidinfo.height == h
	&& picasso_vidinfo.depth == depth
	&& picasso_vidinfo.selected_rgbformat == rgbfmt)
	return;

    picasso_vidinfo.selected_rgbformat = rgbfmt;
    picasso_vidinfo.width = w;
    picasso_vidinfo.height = h;
    picasso_vidinfo.depth = depth;
    picasso_vidinfo.extra_mem = 1;

    if (screen_is_picasso) {
	close_windows ();
	open_windows ();
	DX_SetPalette (0, 256);
    }
}

int graphics_init (void)
{
    SetPriorityClass (GetCurrentProcess (), process_desired_pri);

    return open_windows ();
}

int graphics_setup (void)
{
    char *posn;
    int i;

    debugfile = CreateFile ("outfile", GENERIC_WRITE, 0, 0, CREATE_ALWAYS,
			    0, NULL);

    /* Get our executable's root-path */
    start_path = xmalloc (MAX_PATH);
    GetModuleFileName (0, start_path, MAX_PATH);
    if (posn = strrchr (start_path, '\\'))
	*posn = 0;

    write_log ("UAE " UAEWINVERSION " Win32/DirectX, release " UAEWINRELEASE "\n");
    strcpy (VersionStr, PROGNAME);
    write_log ("\n(c) 1995-1997 Bernd Schmidt   - Core UAE concept and implementation."
	       "\n(c) 1996-1997 Mathias Ortmann - Win32 port and enhancements."
	       "\n(c) 1996-1997 Brian King      - Picasso96 and AHI support, GUI.\n"
	       "\nPress F12 to show the Settings Dialog (GUI), Alt-F4 to quit.\n"
	       "\nhttp://www.informatik.tu-muenchen.de/~ortmann/uae/\n\n");

    if (! initlibs ())
	return 0;

    if (! register_classes ())
	return 0;

    if (! start_ddraw ())
	return 0;
    IDirectDraw_EnumDisplayModes (lpDD, 0, NULL, NULL, modesCallback);
#if 0
    figure_pixel_formats ();
#endif
    release_ddraw ();

    return 1;
}

void machdep_init (void)
{
}

void graphics_leave (void)
{
    close_windows ();
    dumpcustom ();
    cleanuplibs ();
}

#if defined NO_MAIN_IN_MAIN_C
int PASCAL WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine,
		    int nCmdShow)
{
    hInst = hInstance;

    printf ("WinMain called\n");

    /*initgfxspecs(); */
#ifdef __CYGWIN32__
    {
	char *token, *tmpbuf;
	int my_argc, i;
	char ** my_argv;

	tmpbuf = my_strdup (lpCmdLine);
	my_argc = 1;
	if (strtok (tmpbuf, "\n ") != NULL) {
	    do {
		my_argc++;
	    } while (strtok (NULL, "\n ") != NULL);
	}
	free (tmpbuf);

	tmpbuf = my_strdup (lpCmdLine);

	my_argv = (char **)malloc ((1 + my_argc) * sizeof (char **));
	my_argv[0] = "uae.exe";

	token = strtok (tmpbuf, "\n ");
	i = 1;
	while (token != NULL) {
	    my_argv[i++] = my_strdup (token);
	    token = strtok (NULL, "\n ");
	}
	my_argv[my_argc] = NULL;

	real_main (my_argc, my_argv);
    }
#elif defined __MINGW32__
    real_main (_argc, _argv);
#else
    real_main (__argc, __argv);
#endif
    return FALSE;
}
#endif

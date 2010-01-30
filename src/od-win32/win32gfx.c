/*
 * UAE - The Un*x Amiga Emulator
 *
 * Win32 Drawing and DirectX interface
 *
 * Copyright 1997-1998 Mathias Ortmann
 * Copyright 1997-2000 Brian King
 */

#include "sysconfig.h"

#include <stdlib.h>
#include <stdarg.h>

#include <signal.h>
#include <io.h>

#include <windows.h>
#include <commctrl.h>
#include <stdio.h>

#include "sysdeps.h"
#include "options.h"
#include "gensound.h"
#include "uae.h"
#include "memory.h"
#include "custom.h"
#include "events.h"
#include "xwin.h"
#include "keyboard.h"
#include "drawing.h"
#include "picasso96.h"
#include "osdep/win32.h"
#include "osdep/win32gui.h"
#include "osdep/win32gfx.h"
#include "sounddep/sound.h"

/* Local globals */
static uae_u32 current_width, current_height, current_depth;
static int fullscreen = 0; /* fullscreen mode */
static int window_width = 900, window_height = 720, window_depth; /* target resolution */
static int usedirect = 0; /* direct to dx surface (fullscreen or overlay) */
static int overlay = 1; /* use overlay surface */
static int needs_direct; /* is overlay or fullscreen mode required */
static int display_change_requested = 0;
static int mapping_is_mainscreen = 0;
static BOOL bInitDone = FALSE; //?????JGI

int screen_is_picasso = 0;

static BOOL bJustClosedWithActiveMouse = FALSE;

int WIN32GFX_IsPicassoScreen( void )
{
    return screen_is_picasso;
}

void WIN32GFX_DisablePicasso( void )
{
    picasso_requested_on = 0;
    picasso_on = 0;
}

void WIN32GFX_EnablePicasso( void )
{
    picasso_requested_on = 1;
}

void WIN32GFX_DisplayChangeRequested( void )
{
    display_change_requested = 1;
}

int WIN32GFX_IsFullScreen( void )
{
    return fullscreen;
}

int WIN32GFX_GetWidth( void )
{
    return current_width;
}

int WIN32GFX_GetHeight( void )
{
    return current_height;
}

#ifdef _WIN32_WCE
int nr_joysticks = 1;

void init_joystick( void )
{
}

void read_joystick (int nr, unsigned int *dir, int *button)
{
    *dir = *button = 0;

    if( nr < nr_joysticks )
    {

    }
}

void close_joystick( void )
{
}

int check_prefs_changed_gfx (void)
{
    return 0;
}

int graphics_init( void )
{
    return 1;
}

int graphics_setup( void )
{
    return 1;
}

void graphics_leave( void )
{
}

void unlockscr( void )
{
}

int lockscr( void )
{
    return 0;
}

void flush_screen( int a, int b )
{
}

void flush_block( int a, int b )
{
}

void flush_line( int a )
{
}

void WIN32GFX_PaletteChange( void )
{
}

#else
#include "osdep/dxwrap.h"

#ifdef __GNUC__
int __cdecl _fcloseall( void );
#endif

/* Local Globals */
static UINT current_pixbytes;
static LPPALETTEENTRY current_palette = NULL;

static BOOL doInit (void);
static void close_windows (void);

uae_u32 default_freq = 0;

HWND hStatusWnd = NULL;
HINSTANCE hDDraw = NULL;

/* For the DX_Invalidate() and gfx_unlock_picasso() functions */
static int p96_double_buffer_first, p96_double_buffer_last, p96_double_buffer_needs_flushing = 0;

static char scrlinebuf[4096];	/* this is too large, but let's rather play on the safe side here */

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

static int set_ddraw (int width, int height, int wantfull, int wantoverlay, int bits, LPPALETTEENTRY pal)
{
    HRESULT ddrval;

    bits = (bits + 7) & ~7;
    
    ddrval = DirectDraw_SetCooperativeLevel( hAmigaWnd, wantfull );
    if (ddrval != DD_OK)
	goto oops;

    if (wantfull) 
    {
	write_log ( "set_ddraw: Trying %dx%d, %d bits\n", width, height, bits );
	ddrval = DirectDraw_SetDisplayMode( width, height, bits, 0 );
	if (ddrval != DD_OK)
	{
	    write_log ( "set_ddraw: Couldn't SetDisplayMode()\n" );
	    goto oops;
	}

	ddrval = DirectDraw_GetDisplayMode();
	if (ddrval != DD_OK)
	{
	    write_log ( "set_ddraw: Couldn't GetDisplayMode()\n" );
	    goto oops;
	}
    }

    ddrval = DirectDraw_CreateClipper();
    if (ddrval != DD_OK)
    {
	write_log ( "set_ddraw: No clipping support\n" );
	goto oops;
    }

    ddrval = DirectDraw_CreateSurface( width, height );
    if( ddrval != DD_OK )
    {
	write_log ( "set_ddraw: Couldn't CreateSurface() for primary because %s.\n", DirectDraw_ErrorString( ddrval ) );
	goto oops;
    }

    if( wantoverlay )
    {
	if( !currprefs.win32_no_overlay && ( DirectDraw_GetPrimaryBitCount() != bits ) )
	{
	    ddrval = DirectDraw_CreateOverlaySurface( width, height, bits );
	    if( ddrval != DD_OK )
	    {
		write_log ( "set_ddraw: Couldn't CreateOverlaySurface(%d,%d,%d) because %s.\n", width, height, bits, DirectDraw_ErrorString( ddrval ) );
		overlay = 0;
	    }
	}
	else
	{
	    overlay = 0;
	}
    }

    DirectDraw_ClearSurfaces();

    if( !DirectDraw_DetermineLocking( wantfull ) )
    {
	write_log ( "set_ddraw: Couldn't determine locking.\n" );
	goto oops;
    }

    ddrval = DirectDraw_SetClipper( hAmigaWnd );

    if (ddrval != DD_OK)
    {
	write_log ( "set_ddraw: Couldn't SetHWnd()\n" );
	    goto oops;
    }

    current_pixbytes = DirectDraw_GetBytesPerPixel();

    write_log ( "set_ddraw() called, and is %dx%d@%d-bytes\n", width, height, current_pixbytes );

    if (current_pixbytes == 1) {
	current_palette = pal;
	ddrval = DirectDraw_CreatePalette( pal );
	if (ddrval != DD_OK)
	{
	    write_log ( "set_ddraw: Couldn't CreatePalette()\n" );
		goto oops;
	}
    }

    return 1;

  oops:
    if( wantfull )
	DirectDraw_SetCooperativeLevel( hAmigaWnd, 0 ); /* No full-screen, so that people can see our gui_message() */
    gui_message("set_ddraw(): DirectDraw initialization failed with %s/%d\n", DirectDraw_ErrorString( ddrval ), ddrval);
    return 0;
}

struct win32_displaymode *win32_displaymode_list;

HRESULT CALLBACK modesCallback( LPDDSURFACEDESC2 modeDesc, LPVOID context )
{
    struct win32_displaymode **dmpp;
    RGBFTYPE colortype;

    colortype = DirectDraw_GetSurfacePixelFormat( modeDesc );
    if (colortype == RGBFB_NONE || colortype == RGBFB_R8G8B8 || colortype == RGBFB_B8G8R8 )
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
	*dmpp = (struct win32_displaymode *)xmalloc (sizeof **dmpp);
	(*dmpp)->next = 0;
	(*dmpp)->width = modeDesc->dwWidth;
	(*dmpp)->height = modeDesc->dwHeight;
	(*dmpp)->refreshrate = modeDesc->dwRefreshRate;
	(*dmpp)->colormodes = 0;
    }
    (*dmpp)->colormodes |= 1 << colortype;
    return DDENUMRET_OK;
}

static void cleanup_modes( void )
{
    struct win32_displaymode *temp = NULL, *dmpp = win32_displaymode_list;

    while( dmpp )
    {
	temp = dmpp;
	dmpp = dmpp->next;
	free( temp );
    }
}

static int our_possible_depths[] = { 8, 15, 16, 24, 32 };

RGBFTYPE WIN32GFX_FigurePixelFormats( RGBFTYPE colortype )
{
    HRESULT ddrval;
    struct win32_displaymode *dm;
    int got_16bit_mode = 0;
    int window_created = 0;

    if( colortype == 0 ) /* Need to query a 16-bit display mode for its pixel-format.  Do this by opening such a screen */
    {
	hAmigaWnd = CreateWindowEx (WS_EX_TOPMOST,
			       "AmigaPowah", VersionStr,
			       WS_VISIBLE | WS_POPUP,
			       CW_USEDEFAULT, CW_USEDEFAULT,
			       1,//GetSystemMetrics (SM_CXSCREEN),
			       1,//GetSystemMetrics (SM_CYSCREEN),
			       0, NULL, 0, NULL);
	if( hAmigaWnd )
	{
	    window_created = 1;
	    ddrval = DirectDraw_SetCooperativeLevel( hAmigaWnd, TRUE ); /* TRUE indicates full-screen */
	    if( ddrval != DD_OK )
	    {
		write_log ( "WIN32GFX_FigurePixelFormats: ERROR -  %s\n", DirectDraw_ErrorString(ddrval) );
		gui_message( "WIN32GFX_FigurePixelFormats: ERROR - %s\n", DirectDraw_ErrorString(ddrval) );
		goto out;
	    }
	}
	else
	{
	    write_log ( "WIN32GFX_FigurePixelFormats: ERROR - test-window could not be created.\n" );
	    gui_message( "WIN32GFX_FigurePixelFormats: ERROR - test-window could not be created.\n" );
	}
    }
    else
    {
	got_16bit_mode = 1;
    }

    for (dm = win32_displaymode_list; dm != 0; dm = dm->next) 
    {
	if (!got_16bit_mode)
	{
	    write_log ("figure_pixel_formats: Attempting %dx%d: ", dm->width, dm->height);

	    ddrval = DirectDraw_SetDisplayMode( dm->width, dm->height, 16, 0 ); /* 0 for default freq */
	    if (ddrval != DD_OK)
		continue;

	    ddrval = DirectDraw_GetDisplayMode();
	    if (ddrval != DD_OK)
		continue;

	    colortype = DirectDraw_GetPixelFormat();
	    if (colortype != RGBFB_NONE) 
	    {
		write_log ("%d ", our_possible_depths[2]);
	    
		/* Clear the 16-bit information, and get the real stuff! */
		dm->colormodes &= ~(RGBFF_R5G6B5PC|RGBFF_R5G5B5PC|RGBFF_R5G6B5|RGBFF_R5G5B5|RGBFF_B5G6R5PC|RGBFF_B5G5R5PC);
		dm->colormodes |= 1 << colortype;
		got_16bit_mode = 1;
		write_log ( "Got real 16-bit colour-depth information: 0x%x\n", colortype );
	    }
	}
	else if (dm->colormodes & (RGBFF_R5G6B5PC|RGBFF_R5G5B5PC|RGBFF_R5G6B5|RGBFF_R5G5B5|RGBFF_B5G6R5PC|RGBFF_B5G5R5PC) ) 
	{
	    /* Clear the 16-bit information, and set the real stuff! */
	    dm->colormodes &= ~(RGBFF_R5G6B5PC|RGBFF_R5G5B5PC|RGBFF_R5G6B5|RGBFF_R5G5B5|RGBFF_B5G6R5PC|RGBFF_B5G5R5PC);
	    dm->colormodes |= 1 << colortype;
	}
    }
    out:
    if( window_created )
    {
	Sleep( 2000 );
	DestroyWindow( hAmigaWnd );
	hAmigaWnd = NULL;
    }
    return colortype;
}

/* DirectX will fail with "Mode not supported" if we try to switch to a full
 * screen mode that doesn't match one of the dimensions we got during enumeration.
 * So try to find a best match for the given resolution in our list.  */
int WIN32GFX_AdjustScreenmode( uae_u32 *pwidth, uae_u32 *pheight, uae_u32 *ppixbits )
{
    struct win32_displaymode *best;
    uae_u32 selected_mask = (*ppixbits == 8 ? RGBMASK_8BIT
			     : *ppixbits == 15 ? RGBMASK_15BIT
			     : *ppixbits == 16 ? RGBMASK_16BIT
			     : *ppixbits == 24 ? RGBMASK_24BIT
			     : RGBMASK_32BIT);
    int pass, i = 0, index = 0;
    
    for (pass = 0; pass < 2; pass++) 
    {
	struct win32_displaymode *dm;
	uae_u32 mask = (pass == 0
			? selected_mask
			: RGBMASK_8BIT | RGBMASK_15BIT | RGBMASK_16BIT | RGBMASK_24BIT | RGBMASK_32BIT); /* %%% - BERND, were you missing 15-bit here??? */
	i = 0;
	index = 0;

	best = win32_displaymode_list;
	dm = best->next;

	while (dm != 0) 
	{
	    if ((dm->colormodes & mask) != 0) 
	    {
		if (dm->width <= best->width && dm->height <= best->height
		    && dm->width >= *pwidth && dm->height >= *pheight)
		{
		    best = dm;
		    index = i;
		}
		if (dm->width >= best->width && dm->height >= best->height
		    && dm->width <= *pwidth && dm->height <= *pheight)
		{
		    best = dm;
		    index = i;
		}
	    }
	    dm = dm->next;
	    i++;
	}
	if (best->width == *pwidth && best->height == *pheight)
	{
	    selected_mask = mask; /* %%% - BERND, I added this - does it make sense?  Otherwise, I'd specify a 16-bit display-mode for my
				     Workbench (using -H 2, but SHOULD have been -H 1), and end up with an 8-bit mode instead*/
	    break;
	}
    }
    *pwidth = best->width;
    *pheight = best->height;
    if( best->colormodes & selected_mask )
	return index;

    /* Ordering here is done such that 16-bit is preferred, followed by 15-bit, 8-bit, 32-bit and 24-bit */
    if (best->colormodes & RGBMASK_16BIT)
	*ppixbits = 16;
    else if (best->colormodes & RGBMASK_15BIT) /* %%% - BERND, this possibility was missing? */
	*ppixbits = 15;
    else if (best->colormodes & RGBMASK_8BIT)
	*ppixbits = 8;
    else if (best->colormodes & RGBMASK_32BIT)
	*ppixbits = 32;
    else if (best->colormodes & RGBMASK_24BIT)
	*ppixbits = 24;
    else
	index = -1;

    return index;
}

void flush_line( int lineno )
{

}

void flush_block (int a, int b)
{

}

void flush_screen (int a, int b)
{
    if( DirectDraw_GetLockableType() == secondary_surface )
    {
	if( fullscreen )
	{
	    if( DX_Flip() == 0 )
	    {
		DX_Blit( 0, a, 0, a, current_width, b - a + 1, BLIT_SRC );
	    }
	}
	else
	{
	    DX_Blit( 0, a, 0, a, current_width, b - a + 1, BLIT_SRC );
	}
    }
}

static uae_u8 *dolock (void)
{
    char *surface = NULL, *oldsurface;

    if( !DirectDraw_SurfaceLock( lockable_surface ) )
	return 0;

    surface = DirectDraw_GetSurfacePointer();
    oldsurface = gfxvidinfo.bufmem;
    gfxvidinfo.bufmem = surface;
    if (surface != oldsurface && ! screen_is_picasso) 
    {
	write_log ("Need to init_row_map\n");
	init_row_map ();
    }

    clear_inhibit_frame (IHF_WINDOWHIDDEN);
    return surface;
}

int lockscr( void )
{
    return dolock() != 0;
}

uae_u8 *gfx_lock_picasso (void)
{
    return dolock ();
}

void gfx_unlock_picasso (void)
{
    DirectDraw_SurfaceUnlock();
    if( p96_double_buffer_needs_flushing )
    {
	/* Here, our flush_block() will deal with a offscreen-plain (back-buffer) to visible-surface (front-buffer) */
	if( DirectDraw_GetLockableType() == secondary_surface )
	{
	    BOOL relock = FALSE;
	    if( DirectDraw_IsLocked() )
	    {
		relock = TRUE;
		unlockscr();
	    }
	    DX_Blit( 0, p96_double_buffer_first, 
		     0, p96_double_buffer_first, 
		     current_width, p96_double_buffer_last - p96_double_buffer_first + 1, 
		     BLIT_SRC );
	    if( relock )
	    {
		lockscr();
	    }
	}
	p96_double_buffer_needs_flushing = 0;
    }
}

static void close_hwnds( void )
{
    if (hStatusWnd)
    {
	ShowWindow( hStatusWnd, SW_HIDE );
	DestroyWindow (hStatusWnd);
    }
    if (hAmigaWnd)
    {
	ShowWindow( hAmigaWnd, SW_HIDE );
	DestroyWindow (hAmigaWnd);
    }
    if (hMainWnd)
    {
	ShowWindow( hMainWnd, SW_HIDE );
	DestroyWindow (hMainWnd);
    }

    hMainWnd = 0;
    hStatusWnd = 0;
    hAmigaWnd = 0;
}

static int open_windows (void)
{
    static BOOL bFirstTime = TRUE;
    char tmpstr[300];

    char *fs_warning = 0;
    RGBFTYPE colortype;

    current_pixbytes = 0;

    in_sizemove = 0;
    fixup_prefs_dimensions (&currprefs);

    if( !DirectDraw_Start() )
	return 0;
    if( DirectDraw_GetDisplayMode() != DD_OK )
	return 0;
    colortype = DirectDraw_GetPixelFormat();
    write_log ("Ct: %08lx, picasso_vidinfo.selected_rgbformat %08lx\n", (unsigned long)colortype,
	    picasso_vidinfo.selected_rgbformat);

    if (screen_is_picasso) {
	fullscreen = currprefs.gfx_pfullscreen;
	window_width = current_width = picasso_vidinfo.width;
	window_height = current_height = picasso_vidinfo.height;
	window_depth = current_depth = rgbformat_bits (picasso_vidinfo.selected_rgbformat);
    } else {
	fullscreen = currprefs.gfx_afullscreen;
	window_width = current_width = currprefs.gfx_width;
	window_height = current_height = currprefs.gfx_height;
	window_depth = current_depth = (currprefs.color_mode == 0 ? 8
			 : currprefs.color_mode == 1 ? 15
			 : currprefs.color_mode == 2 ? 16
			 : currprefs.color_mode == 3 ? 8
			 : currprefs.color_mode == 4 ? 8 : 32);
    }

    //If screen depth is equal to the desired window_depth then no overlay is needed.
    if( DirectDraw_GetSurfaceBitCount() == window_depth )
	overlay = 0;

    if (overlay) {
	needs_direct = 1;
    } else {
	needs_direct = 0;
    }

    if (colortype == RGBFB_NONE && !overlay) {
	needs_direct = 1;
	fs_warning = "the desktop is running in an unknown color mode.";
    } else if (colortype == RGBFB_CLUT && !overlay) {
	needs_direct = 1;
	fs_warning = "the desktop is running in 8 bit color depth, which UAE can't use in windowed mode.";
    } else if ( !fullscreen && ( current_width >= DirectDraw_CurrentWidth() || current_height >= DirectDraw_CurrentHeight() ) ) {
	needs_direct = 1;
	fs_warning = "the desktop is too small for the specified window size.";
	overlay = 0; // we're going to go full-screen
    } else if (screen_is_picasso && !fullscreen &&
	      ( picasso_vidinfo.selected_rgbformat != RGBFB_CHUNKY ) &&
	      ( picasso_vidinfo.selected_rgbformat != colortype ) &&
		overlay )
    {
	needs_direct = 1;
	fs_warning = "you selected a Picasso display with a color depth different from that of the desktop and an overlay was unavailable.";
	if( !doInit() )
	{
	    overlay = 0; // we're going to go full-screen
	    // doInit in windowed-mode modifies the width/height params, so 
	    // restore our mucked-up width and height values for full-screen.
	    window_width = current_width = picasso_vidinfo.width;
	    window_height = current_height = picasso_vidinfo.height;
	}
	DirectDraw_Release();
	close_hwnds();
	if( !DirectDraw_Start() )
	    return 0;
    }

    if( needs_direct && ! usedirect ) 
	usedirect = needs_direct;

    if( fs_warning && !overlay )
    {
	// Temporarily drop the DirectDraw stuff
	DirectDraw_Release();
	sprintf (tmpstr, "The selected screen mode can't be displayed in a window, because %s\n"
		 "Switching to full-screen display.", fs_warning);
	MessageBox (0, tmpstr, "WinUAE", MB_ICONEXCLAMATION | MB_OK);
	DirectDraw_Start();
	fullscreen = 1;
    }
    
    if (! usedirect)
	window_depth = rgbformat_bits (colortype);

    if( current_depth == 24 )
    {
	if( screen_is_picasso )
	    fs_warning = "you've selected a Picasso display with a 24-bit depth, which WinUAE no longer supports.";
	else
	    fs_warning = "the desktop is running in 24-bit color depth, which WinUAE no longer supports.";
	/* Temporarily drop the DirectDraw stuff.  This is necessary, otherwise
	 * WinNT will just return 1 for the message box without ever displaying 
	 * it on the screen.  */
	DirectDraw_Release();
	sprintf (tmpstr, "WinUAE cannot run because %s", fs_warning);
	MessageBox (0, tmpstr, "WinUAE", MB_ICONEXCLAMATION | MB_OK);
	return 0;
    }
    
    if (! doInit ())
	return 0;

    if(!WIN32GFX_IsFullScreen() && ( bFirstTime || bJustClosedWithActiveMouse ) )
    {
	bFirstTime = FALSE;
	bJustClosedWithActiveMouse = FALSE;
	setmouseactive (1);
    }

    return 1;
}

int check_prefs_changed_gfx (void)
{
    if (display_change_requested || 
	( currprefs.gfx_afullscreen != changed_prefs.gfx_afullscreen ) ||
	( currprefs.gfx_pfullscreen != changed_prefs.gfx_pfullscreen ) )
    {
	display_change_requested = 0;
	currprefs.gfx_afullscreen = changed_prefs.gfx_afullscreen;
	currprefs.gfx_pfullscreen = changed_prefs.gfx_pfullscreen;
	close_windows ();
	open_windows ();
	return 1;
    }
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
    HRESULT ddrval;

    if (ncols256 == 0) {
	alloc_colors256 (get_color);
	memcpy (xcol8, xcolors, sizeof xcol8);
    }

    /* init colors */

    switch( current_pixbytes )
    {
	case 1:
	    memcpy (xcolors, xcol8, sizeof xcolors);
	    ddrval = DirectDraw_SetPaletteEntries( 0, 256, colors256 );
	    if (ddrval != DD_OK)
		gui_message ("DX_SetPalette() failed with %s/%d\n", DirectDraw_ErrorString (ddrval), ddrval);
	break;

	case 2:
	case 3:
	case 4:
	    red_bits = bits_in_mask( DirectDraw_GetPixelFormatBitMask( red_mask ) );
	    green_bits = bits_in_mask( DirectDraw_GetPixelFormatBitMask( green_mask ) );
	    blue_bits = bits_in_mask( DirectDraw_GetPixelFormatBitMask( blue_mask ) );
	    red_shift = mask_shift( DirectDraw_GetPixelFormatBitMask( red_mask ) );
	    green_shift = mask_shift( DirectDraw_GetPixelFormatBitMask( green_mask ) );
	    blue_shift = mask_shift( DirectDraw_GetPixelFormatBitMask( blue_mask ) );

	    alloc_colors64k (red_bits, green_bits, blue_bits, red_shift,
			     green_shift, blue_shift);
	break;
    }
    switch (gfxvidinfo.pixbytes) 
    {
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
	    DisplayModes[count].refresh = default_freq;
	    DisplayModes[count].depth = 1;
	    count++;
	}
	if (count >= MAX_PICASSO_MODES)
	    break;
	if (dm->colormodes & (RGBMASK_16BIT | RGBMASK_15BIT)) {
	    DisplayModes[count].res.width = dm->width;
	    DisplayModes[count].res.height = dm->height;
	    DisplayModes[count].refresh = default_freq;
	    DisplayModes[count].depth = 2;
	    count++;
	}
	if (count >= MAX_PICASSO_MODES)
	    break;
	if (dm->colormodes & RGBMASK_24BIT) {
	    DisplayModes[count].res.width = dm->width;
	    DisplayModes[count].res.height = dm->height;
	    DisplayModes[count].refresh = default_freq;
	    DisplayModes[count].depth = 3;
	    count++;
	}
	if (count >= MAX_PICASSO_MODES)
	    break;
	if (dm->colormodes & RGBMASK_32BIT) {
	    DisplayModes[count].res.width = dm->width;
	    DisplayModes[count].res.height = dm->height;
	    DisplayModes[count].refresh = default_freq;
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

    if( !screen_is_picasso )
	return;

    if( picasso96_state.RGBFormat != RGBFB_CHUNKY )
    {
	/* notice_screen_contents_lost(); */
	return;
    }

    if (picasso_vidinfo.pixbytes != 1) 
    {
	/* write_log ("DX Setpalette emulation\n"); */
	/* This is the case when we're emulating a 256 color display.  */
	while (count-- > 0) 
	{
	    int r = picasso96_state.CLUT[start].Red;
	    int g = picasso96_state.CLUT[start].Green;
	    int b = picasso96_state.CLUT[start].Blue;
	    picasso_vidinfo.clut[start++] = (doMask256 (r, red_bits, red_shift)
		| doMask256 (g, green_bits, green_shift)
		| doMask256 (b, blue_bits, blue_shift));
	}
	notice_screen_contents_lost();
	return;
    }

    /* Set our DirectX palette here */
    if( current_pixbytes == 1 )
    {
	    ddrval = DirectDraw_SetPaletteEntries( start, count, (LPPALETTEENTRY)&(picasso96_state.CLUT[start] ) );
	    if (ddrval != DD_OK)
		gui_message("DX_SetPalette() failed with %s/%d\n", DirectDraw_ErrorString (ddrval), ddrval);
    }
    else
    {
	    write_log ("ERROR - DX_SetPalette() pixbytes %d\n", current_pixbytes );
    }
}

void DX_Invalidate (int first, int last)
{
    p96_double_buffer_first = first;
    if( (unsigned)last >= picasso_vidinfo.height )
	last = picasso_vidinfo.height - 1;
    p96_double_buffer_last  = last;
    p96_double_buffer_needs_flushing = 1;
}

int DX_BitsPerCannon (void)
{
    return 8;
}

static COLORREF BuildColorRef( int color, RGBFTYPE pixelformat )
{
    COLORREF result;

    /* Do special case first */
    if( pixelformat == RGBFB_CHUNKY )
	result = color;
    else
	result = do_get_mem_long( &color );
    return result;
#if 0
    int r,g,b;
    write_log ( "DX_Blit() called to fill with color of 0x%x, rgbtype of 0x%x\n", color, pixelformat );

    switch( pixelformat )
    {
	case RGBFB_R5G6B5PC:
	    r = color & 0xF800 >> 11;
	    g = color & 0x07E0 >> 5;
	    b = color & 0x001F;
	break;
	case RGBFB_R5G5B5PC:
	    r = color & 0x7C00 >> 10;
	    g = color & 0x03E0 >> 5;
	    b = color & 0x001F;
	break;
	case RGBFB_B5G6R5PC:
	    r = color & 0x001F;
	    g = color & 0x07E0 >> 5;
	    b = color & 0xF800 >> 11;
	break;
	case RGBFB_B5G5R5PC:
	    r = color & 0x001F;
	    g = color & 0x03E0 >> 5;
	    b = color & 0x7C00 >> 10;
	break;
	case RGBFB_B8G8R8:
	    r = color & 0x00FF0000 >> 16;
	    g = color & 0x0000FF00 >> 8;
	    b = color & 0x000000FF;
	break;
	case RGBFB_A8B8G8R8:
	    r = color & 0xFF000000 >> 24;
	    g = color & 0x00FF0000 >> 16;
	    b = color & 0x0000FF00 >> 8;
	break;
	case RGBFB_R8G8B8:
	    r = color & 0x000000FF;
	    g = color & 0x0000FF00 >> 8;
	    b = color & 0x00FF0000 >> 16;
	break;
	case RGBFB_A8R8G8B8:
	    r = color & 0x0000FF00 >> 8;
	    g = color & 0x00FF0000 >> 16;
	    b = color & 0xFF000000 >> 24;
	break;
	default:
	    write_log ( "Uknown 0x%x pixel-format\n", pixelformat );
	break;
    }
    result = RGB(r,g,b);
    write_log ( "R = 0x%02x, G = 0x%02x, B = 0x%02x - result = 0x%08x\n", r, g, b, result );
    return result;
#endif
}

/* This is a general purpose DirectDrawSurface filling routine.  It can fill within primary surface.
 * Definitions:
 * - primary is the displayed (visible) surface in VRAM, which may have an associated offscreen surface (or back-buffer)
 */
int DX_Fill( int dstx, int dsty, int width, int height, uae_u32 color, RGBFTYPE rgbtype )
{
    int result = 0;
    RECT dstrect;
    RECT srcrect;
    DDBLTFX ddbltfx;
    memset( &ddbltfx, 0, sizeof( ddbltfx ) );
    ddbltfx.dwFillColor = BuildColorRef( color, rgbtype );
    ddbltfx.dwSize = sizeof( ddbltfx );

    /* Set up our source rectangle.  This NEVER needs to be adjusted for windowed display, since the
     * source is ALWAYS in an offscreen buffer, or we're in full-screen mode. */
    SetRect( &srcrect, dstx, dsty, dstx+width, dsty+height );

    /* Set up our destination rectangle, and adjust for blit to windowed display (if necessary ) */
    SetRect( &dstrect, dstx, dsty, dstx+width, dsty+height );
    if( !fullscreen && !overlay )
	OffsetRect( &dstrect, amigawin_rect.left, amigawin_rect.top );

    /* Render our fill to the visible (primary) surface */
    if( ( result = DirectDraw_Blt( primary_surface, &dstrect, invalid_surface, NULL, DDBLT_WAIT | DDBLT_COLORFILL, &ddbltfx ) ) )
    {
	if( DirectDraw_GetLockableType() == secondary_surface )
	{
	    /* We've colour-filled the visible, but still need to colour-fill the offscreen */
	    result = DirectDraw_Blt( secondary_surface, &srcrect, invalid_surface, NULL, DDBLT_WAIT | DDBLT_COLORFILL, &ddbltfx );
	}
    }
    return result;
}

/* This is a general purpose DirectDrawSurface blitting routine.  It can blit within primary surface
 * Definitions:
 * - primary is the displayed (visible) surface in VRAM, which may have an associated offscreen surface (or back-buffer)
 */

DDBLTFX fx = { sizeof( DDBLTFX ) };

DWORD BLIT_OPCODE_TRANSLATION[ BLIT_LAST ] =
{
    BLACKNESS,  /* BLIT_FALSE */
    NOTSRCERASE,/* BLIT_NOR */
    -1,         /* BLIT_ONLYDST NOT SUPPORTED */
    NOTSRCCOPY, /* BLIT_NOTSRC */
    SRCERASE,   /* BLIT_ONLYSRC */
    DSTINVERT,  /* BLIT_NOTDST */
    SRCINVERT,  /* BLIT_EOR */
    -1,         /* BLIT_NAND NOT SUPPORTED */
    SRCAND,     /* BLIT_AND */
    -1,         /* BLIT_NEOR NOT SUPPORTED */
    -1,         /* NO-OP */
    MERGEPAINT, /* BLIT_NOTONLYSRC */
    SRCCOPY,    /* BLIT_SRC */
    -1,         /* BLIT_NOTONLYDST NOT SUPPORTED */
    SRCPAINT,   /* BLIT_OR */
    WHITENESS   /* BLIT_TRUE */
};

// This function is only called for full-screen Amiga screen-modes, and simply flips
// the front and back buffers.  Additionally, because the emulation is not always drawing
// complete frames, we also need to update the back-buffer with the new contents we just
// flipped to.  Thus, after our flip, we blit.
int DX_Flip( void )
{
    int result = 0;

    result = DirectDraw_Flip();
    if( result )
    {
	result = DirectDraw_BltFast( secondary_surface, 0, 0, primary_surface, NULL );
    }
    return result;
}

int DX_Blit( int srcx, int srcy, int dstx, int dsty, int width, int height, BLIT_OPCODE opcode )
{
    int result = 0;
    RECT dstrect;
    RECT srcrect;
    DWORD dwROP = BLIT_OPCODE_TRANSLATION[ opcode ];

    /* Set up our source rectangle.  This NEVER needs to be adjusted for windowed display, since the
     * source is ALWAYS in an offscreen buffer, or we're in full-screen mode. */
    SetRect( &srcrect, srcx, srcy, srcx+width, srcy+height );

    /* Set up our destination rectangle, and adjust for blit to windowed display (if necessary ) */
    SetRect( &dstrect, dstx, dsty, dstx+width, dsty+height );
    
    if( !fullscreen && !overlay )
	OffsetRect( &dstrect, amigawin_rect.left, amigawin_rect.top );

    if( dwROP == -1 )
    {
	/* Unsupported blit opcode! */
	return 0;
    }
    else
    {
	fx.dwROP = dwROP;
    }

    /* Render our blit within the primary surface */
    result = DirectDraw_Blt( primary_surface, &dstrect, DirectDraw_GetLockableType(), &srcrect, DDBLT_WAIT | DDBLT_ROP, &fx );

    if( !result )
    {
	BLIT_OPCODE_TRANSLATION[ opcode ] = -1;
    }
    else if( DirectDraw_GetLockableType() == secondary_surface )
    {
	/* We've just blitted from the offscreen to the visible, but still need to blit from offscreen to offscreen
	 * NOTE: reset our destination rectangle again if its been modified above... */
	if( ( srcx != dstx ) || ( srcy != dsty ) )
	{
	    if( !fullscreen )
		SetRect( &dstrect, dstx, dsty, dstx+width, dsty+height );
	    result = DirectDraw_Blt( secondary_surface, &dstrect, secondary_surface, &srcrect, DDBLT_WAIT | DDBLT_ROP, &fx );
	}
    }

    return result;
}

void DX_WaitVerticalSync( void )
{
    DirectDraw_WaitForVerticalBlank( DDWAITVB_BLOCKBEGIN );
}

uae_u32 DX_ShowCursor( uae_u32 activate )
{
    uae_u32 result = 0;
    if( ShowCursor( activate ) > 0 )
	result = 1;
    return result;
}

uae_u32 DX_MoveCursor( uae_u32 x, uae_u32 y )
{
    uae_u32 result = 0;

    // We may need to adjust the x,y values for our window-offset
    if( !fullscreen )
    {
	RECT rect;
	if( GetWindowRect( hAmigaWnd, &rect ) )
	{
	    x = rect.left + x;
	    y = rect.top + y;
	}
    }
    if( SetCursorPos( x, y ) )
	result = 1;
    return result;
}

static void open_screen( void )
{
    close_windows ();
    if( open_windows() )
	DX_SetPalette (0, 256);
    else
	abort();
}

void gfx_set_picasso_state( int on )
{
    if (screen_is_picasso == on)
	return;

    screen_is_picasso = on;
    open_screen();
}

void gfx_set_picasso_modeinfo( int w, int h, int depth, int rgbfmt )
{
    depth >>= 3;
    if( ( picasso_vidinfo.width == w ) &&
	    ( picasso_vidinfo.height == h ) &&
	    ( picasso_vidinfo.depth == depth ) &&
	    ( picasso_vidinfo.selected_rgbformat == rgbfmt) )
	return;

    picasso_vidinfo.selected_rgbformat = rgbfmt;
    picasso_vidinfo.width = w;
    picasso_vidinfo.height = h;
    picasso_vidinfo.depth = depth;
    picasso_vidinfo.extra_mem = 1;

    if( screen_is_picasso ) 
    {
	open_screen();
    }
}

int graphics_init (void)
{
    return open_windows ();
}

int graphics_setup (void)
{
    if( !DirectDraw_Start() )
	return 0;

    DirectDraw_Release();

    InitPicasso96();
    
    return 1;
}

void graphics_leave (void)
{
    close_windows ();
    dumpcustom ();
    cleanup_modes ();
}

uae_u32 OSDEP_minimize_uae( void )
{
    return ShowWindow( hAmigaWnd, SW_MINIMIZE );
}

static void close_windows (void)
{
    gfxvidinfo.bufmem = 0;

    releasecapture ();
    setmouseactive (0);
    ClipCursor (NULL);
    DirectDraw_Release();
    close_hwnds();
    if( mouseactive )
	bJustClosedWithActiveMouse = TRUE;
    { 
	int i;
	for( i=0;i<AK_CTRL+1;i++)my_kbd_handler (0,i,0);
	for( i=0;i<AK_CTRL+1;i++)my_kbd_handler (i,0,0);
	buttonstate[0] = 0;
	buttonstate[1] = 0;
	buttonstate[2] = 0;
    }
    bInitDone = FALSE; //?????JGI
    overlay = 1; // Go back to desiring overlay support
}

void WIN32GFX_ToggleFullScreen( void )
{
    if (needs_direct && !overlay)
	return;

    display_change_requested = 1;
    if (screen_is_picasso)
	currprefs.gfx_pfullscreen ^= 1;
    else
	currprefs.gfx_afullscreen ^= 1;    
}

static int create_windows (void)
{
    if (!fullscreen) 
    {
	RECT rc;
	LONG stored_x = 1, stored_y = GetSystemMetrics( SM_CYMENU ) + GetSystemMetrics( SM_CYBORDER );
	DWORD regkeytype;
	DWORD regkeysize = sizeof(LONG);
	HLOCAL hloc;
	    LPINT lpParts;

	RegQueryValueEx( hWinUAEKey, "xPos", 0, &regkeytype, (LPBYTE)&stored_x, &regkeysize );
	RegQueryValueEx( hWinUAEKey, "yPos", 0, &regkeytype, (LPBYTE)&stored_y, &regkeysize );
	if( stored_x < 1 )
	    stored_x = 1;
	if( stored_y < ( GetSystemMetrics( SM_CYMENU ) + GetSystemMetrics( SM_CYBORDER ) ) )
	    stored_y = GetSystemMetrics( SM_CYMENU ) + GetSystemMetrics( SM_CYBORDER );

	if( stored_x > GetSystemMetrics( SM_CXFULLSCREEN ) )
	    rc.left = 1;
	else
	    rc.left = stored_x;
	
	if( stored_y > GetSystemMetrics( SM_CYFULLSCREEN ) )
	    rc.top = 1;
	else
	    rc.top = stored_y;

	rc.right = rc.left + current_width;
	rc.bottom = rc.top + current_height + GetSystemMetrics( SM_CYMENU ) + GetSystemMetrics( SM_CYBORDER )*3;

	AdjustWindowRect (&rc, NORMAL_WINDOW_STYLE, FALSE);

	hMainWnd = CreateWindowEx( picasso_on ? WS_EX_ACCEPTFILES : WS_EX_ACCEPTFILES | WS_EX_APPWINDOW, "PCsuxRox", "WinUAE",
				       NORMAL_WINDOW_STYLE, rc.left, rc.top,
				       rc.right - rc.left, rc.bottom - rc.top,
				       NULL, NULL, 0, NULL);

	if (! hMainWnd)
	    return 0;
	hStatusWnd = CreateStatusWindow (WS_CHILD | WS_VISIBLE, "", hMainWnd, 1);
	if (hStatusWnd) 
	{
	    GetClientRect (hMainWnd, &rc);
	    /* Allocate an array for holding the right edge coordinates. */
	    hloc = LocalAlloc (LHND, sizeof (int) * NUM_PARTS);
	    if (hloc) 
	    {
		lpParts = LocalLock (hloc);

		/* Calculate the right edge coordinate for each part, and copy the coords
		 * to the array.  */
		lpParts[0] = rc.right - (DRIVE_WIDTH * 4) - LED_WIDTH - FPS_WIDTH - 2;
		lpParts[1] = lpParts[0] + FPS_WIDTH;
		lpParts[2] = lpParts[1] + LED_WIDTH;
		lpParts[3] = lpParts[2] + DRIVE_WIDTH;
		lpParts[4] = lpParts[3] + DRIVE_WIDTH;
		lpParts[5] = lpParts[4] + DRIVE_WIDTH;
		lpParts[6] = lpParts[5] + DRIVE_WIDTH;

		/* Create the seven parts */
		SendMessage (hStatusWnd, SB_SETPARTS, (WPARAM) NUM_PARTS, (LPARAM) lpParts);

		LocalUnlock (hloc);
		LocalFree (hloc);
	    }
	}
    }
    else
	hMainWnd = NULL;

    hAmigaWnd = CreateWindowEx (fullscreen ? WS_EX_TOPMOST : WS_EX_ACCEPTFILES | WS_EX_APPWINDOW,
				"AmigaPowah", "WinUAE",
				hMainWnd ? WS_VISIBLE | WS_CHILD : WS_VISIBLE | WS_POPUP,
				hMainWnd ? 0 : CW_USEDEFAULT, hMainWnd ? 0 : CW_USEDEFAULT,
				fullscreen ? GetSystemMetrics (SM_CXSCREEN) : current_width,
				fullscreen ? GetSystemMetrics (SM_CYSCREEN) : current_height,
				hMainWnd, NULL, 0, NULL);
    
    if (! hAmigaWnd) 
    {
	close_hwnds();
	return 0;
    }


    if (hMainWnd)
	UpdateWindow( hMainWnd );
    if (hAmigaWnd)
	UpdateWindow (hAmigaWnd);

    return 1;
}

static void setoverlay(void)
{
    RECT sr, dr, statusr;
    POINT p = {0,0};
    int maxwidth, maxheight;

    GetClientRect(hMainWnd, &dr);
    // adjust the dest-rect to avoid the status-bar
    if( hStatusWnd )
    {
	if( GetWindowRect( hStatusWnd, &statusr ) )
	    dr.bottom = dr.bottom - ( statusr.bottom - statusr.top );
    }

    ClientToScreen( hMainWnd, &p );
    dr.left = p.x;
    dr.top = p.y;
    dr.right += p.x;
    dr.bottom += p.y;    

    sr.left = 0; sr.top = 0;
    sr.right = current_width; sr.bottom = current_height;

    // Adjust our dst-rect to match the dimensions of our src-rect
    if( dr.right - dr.left > sr.right - sr.left )
    {
	dr.right = dr.left + sr.right - sr.left;
    }
    if( dr.bottom - dr.top > sr.bottom - sr.top )
    {
	dr.bottom = dr.top + sr.bottom - sr.top;
    }

    maxwidth = GetSystemMetrics(SM_CXSCREEN);
    if (dr.right > maxwidth) {
	sr.right = current_width - (dr.right - maxwidth);
	dr.right = maxwidth;
    }
    maxheight = GetSystemMetrics(SM_CYSCREEN);
    if (dr.bottom > maxheight) {
	sr.bottom = current_height - (dr.bottom - maxheight);
	dr.bottom = maxheight;
    }
    if (dr.left < 0) {
	sr.left = -dr.left;
	dr.left = 0;
    }
    if (dr.top < 0) {
	sr.top = -dr.top;
	dr.top = 0;
    }
    DirectDraw_UpdateOverlay(sr, dr);
}

static BOOL doInit (void)
{
    if (! create_windows ())
	goto oops;

    if( screen_is_picasso ) 
    {
	if (! set_ddraw (current_width, current_height, fullscreen, overlay, window_depth,
			(LPPALETTEENTRY) & picasso96_state.CLUT))
	    goto oops;
	picasso_vidinfo.rowbytes = DirectDraw_GetSurfacePitch();
	picasso_vidinfo.pixbytes = DirectDraw_GetBytesPerPixel();
	picasso_vidinfo.rgbformat = DirectDraw_GetPixelFormat();
    }
    else
    {
	if (fullscreen)
	{
#if 0
	    write_log ( "Calling adjust_screenmode with %d,%d,%d\n", window_width, window_height, window_depth );
	    if( WIN32GFX_AdjustScreenmode( &window_width, &window_height, &window_depth ) < 0 )
		abort ();
	    write_log ( "Finished adjust_screenmode with %d,%d,%d\n", window_width, window_height, window_depth );
#endif
	}
	if (! set_ddraw (current_width, current_height, fullscreen, overlay, window_depth, colors256))
	    goto oops;
	gfxvidinfo.bufmem = 0;
	gfxvidinfo.linemem = 0;
	gfxvidinfo.emergmem = scrlinebuf; // memcpy from system-memory to video-memory
	gfxvidinfo.pixbytes = current_pixbytes;
	gfxvidinfo.width = current_width;
	gfxvidinfo.height = current_height;
	gfxvidinfo.maxblocklines = 0; // flush_screen actually does everything
	gfxvidinfo.rowbytes = DirectDraw_GetSurfacePitch();
    }

    if( fullscreen ) 
    {
	WIN32_MouseDefaults();
    }
    if( !DirectDraw_SurfaceLock( lockable_surface ) )
	goto oops;
    DirectDraw_SurfaceUnlock();

    if( ( DirectDraw_GetPixelFormatFlags() & (DDPF_RGB | DDPF_PALETTEINDEXED8 | DDPF_RGBTOYUV ) ) ) 
    {
	write_log ( "%s mode (bits: %d, pixbytes: %d)\n", fullscreen ? "Full screen" : "Window",
		   DirectDraw_GetSurfaceBitCount(), current_pixbytes );
    }
    else
    {
	char szMessage[ MAX_PATH ];
	WIN32GUI_LoadUIString( IDS_UNSUPPORTEDPIXELFORMAT, szMessage, MAX_PATH );
	gui_message( szMessage);
	goto oops;
    }

    init_colors ();

    if (overlay)
	setoverlay ();

#if 0 // This crashes WinXP, because that last param should be an LPWINDOWPOS...
    if (! fullscreen)
	SendMessage( hMainWnd, WM_WINDOWPOSCHANGED, 0, 0);
#endif

    bInitDone = TRUE;
    return 1;

oops:
    DirectDraw_Release();
    close_hwnds();
    return 0;
}


void WIN32GFX_PaletteChange( void )
{
    DirectDraw_SetPalette( 1 ); /* Remove current palette */
    DirectDraw_SetPalette( 0 ); /* Set our real palette */
}

void WIN32GFX_ClearPalette( void )
{
    DirectDraw_SetPalette( 1 ); /* Remove palette */
}

void WIN32GFX_SetPalette( void )
{
    DirectDraw_SetPalette( 0 ); /* Set palette */
}
#endif
void WIN32GFX_WindowMove ( void )
{
    if (overlay && hMainWnd && bInitDone) setoverlay();
}

void WIN32GFX_WindowSize ( void )
{
    RECT r;

    //?????JGI (to delete): if (!overlay) return;
    if (!hMainWnd) return;
    if(!GetClientRect (hMainWnd, &r)) return;
    window_width = r.right - r.left;
    window_height = r.bottom - r.top;
    if (overlay && bInitDone) setoverlay();//????? JGI
}

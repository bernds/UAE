 /*
  * UAE - The Un*x Amiga Emulator
  *
  * SDL graphics support
  *
  * Copyright 2001 Bernd Lachner (EMail: dev@lachner-net.de)
  * Copyright 2003-2006 Richard Drummond
  *
  * Partialy based on the UAE X interface (xwin.c)
  *
  * Copyright 1995, 1996 Bernd Schmidt
  * Copyright 1996 Ed Hanway, Andre Beck, Samuel Devulder, Bruno Coste
  * Copyright 1998 Marcus Sundberg
  * DGA support by Kai Kollmorgen
  * X11/DGA merge, hotkeys and grabmouse by Marcus Sundberg
  */

#include "sysconfig.h"
#include "sysdeps.h"

#include <signal.h>
#include <SDL.h>
#include <SDL_endian.h>

#include "options.h"
#include "uae.h"
#include "xwin.h"
#include "custom.h"
#include "drawing.h"
#include "keyboard.h"
#include "keybuf.h"
#include "gui.h"
#include "debug.h"
#include "picasso96.h"
#include "inputdevice.h"
#include "hotkeys.h"
#include "sdlgfx.h"

/* Uncomment for debugging output */
//#define DEBUG
#ifdef DEBUG
#define DEBUG_LOG write_log
#else
#define DEBUG_LOG(...) do ; while(0)
#endif

static SDL_Surface *display;
static SDL_Surface *screen;

/* Standard P96 screen modes */
#define MAX_SCREEN_MODES 12
static int x_size_table[MAX_SCREEN_MODES] = { 320, 320, 320, 320, 640, 640, 640, 800, 1024, 1152, 1280 };
static int y_size_table[MAX_SCREEN_MODES] = { 200, 240, 256, 400, 350, 480, 512, 600, 768,  864,  1024 };

static int red_bits, green_bits, blue_bits;
static int red_shift, green_shift, blue_shift;

#ifdef PICASSO96
static char picasso_invalid_lines[1201];
static int picasso_has_invalid_lines;
static int picasso_invalid_start, picasso_invalid_stop;
static int picasso_maxw = 0, picasso_maxh = 0;
#endif

static int bitdepth, bit_unit;
static int current_width, current_height;

/* If we have to lock the SDL surface, then we remember the address
 * of its pixel data - and recalculate the row maps only when this
 * address changes */
static void *old_pixels;

static SDL_Color arSDLColors[256];
#ifdef PICASSO96
static SDL_Color p96Colors[256];
#endif
static int ncolors;

static int fullscreen, prev_fullscreen;
static int mousegrab;

static int is_hwsurface;

static int have_rawkeys;

/* This isn't supported yet.
 * gui_handle_events() needs to be reworked fist
 */
int pause_emulation;

#ifdef __cplusplus
static RETSIGTYPE sigbrkhandler(...)
#else
static RETSIGTYPE sigbrkhandler (int foo)
#endif
{
  activate_debugger();
#if !defined(__unix) || defined(__NeXT__)
  signal (SIGINT, sigbrkhandler);
#endif
}

void setup_brkhandler (void)
{
#if defined(__unix) && !defined(__NeXT__)
  struct sigaction sa;
  sa.sa_handler = sigbrkhandler;
  sa.sa_flags = 0;
#ifdef SA_RESTART
  sa.sa_flags = SA_RESTART;
#endif
  sigemptyset (&sa.sa_mask);
  sigaction (SIGINT, &sa, NULL);
#else
  signal (SIGINT, sigbrkhandler);
#endif
}

/*
 * What graphics platform are we running on . . .?
 *
 * Yes, SDL is supposed to abstract away from the underlying
 * platform, but we need to know this to be able to map raw keys
 * and to work around any platform-specific quirks . . .
 */
int get_sdlgfx_type (void)
{
    char name[16] = "";
    static int driver = SDLGFX_DRIVER_UNKNOWN;
    static int search_done = 0;

    if (!search_done) {
	if (SDL_VideoDriverName (name, sizeof name)) {
	    if (strcmp (name, "x11")==0)
		driver = SDLGFX_DRIVER_X11;
	    else if (strcmp (name, "dga") == 0)
		driver = SDLGFX_DRIVER_DGA;
	    else if (strcmp (name, "svgalib") == 0)
		driver = SDLGFX_DRIVER_SVGALIB;
	    else if (strcmp (name, "fbcon") == 0)
		driver = SDLGFX_DRIVER_FBCON;
	    else if (strcmp (name, "directfb") == 0)
		driver = SDLGFX_DRIVER_DIRECTFB;
	    else if (strcmp (name, "Quartz") == 0)
		driver = SDLGFX_DRIVER_QUARTZ;
	    else if (strcmp (name, "bwindow") == 0)
		driver = SDLGFX_DRIVER_BWINDOW;
	    else if (strcmp (name, "CGX") == 0)
		driver = SDLGFX_DRIVER_CYBERGFX;
	    else if (strcmp (name, "OS4") == 0)
		driver = SDLGFX_DRIVER_AMIGAOS4;
	}
	search_done = 1;

	DEBUG_LOG ("SDL video driver: %s\n", name);
    }
    return driver;
}

STATIC_INLINE unsigned long bitsInMask (unsigned long mask)
{
    /* count bits in mask */
    unsigned long n = 0;
    while (mask) {
	n += mask & 1;
	mask >>= 1;
    }
    return n;
}

STATIC_INLINE unsigned long maskShift (unsigned long mask)
{
    /* determine how far mask is shifted */
    unsigned long n = 0;
    while (!(mask & 1)) {
	n++;
	mask >>= 1;
    }
    return n;
}

static int get_color (int r, int g, int b, xcolnr *cnp)
{
    DEBUG_LOG ("Function: get_color\n");

    arSDLColors[ncolors].r = r << 4;
    arSDLColors[ncolors].g = g << 4;
    arSDLColors[ncolors].b = b << 4;
    *cnp = ncolors++;
    return 1;
}

static int init_colors (void)
{
    int i;

    DEBUG_LOG ("Function: init_colors\n");

    if (bitdepth > 8) {
	red_bits    = bitsInMask (display->format->Rmask);
	green_bits  = bitsInMask (display->format->Gmask);
	blue_bits   = bitsInMask (display->format->Bmask);
	red_shift   = maskShift (display->format->Rmask);
	green_shift = maskShift (display->format->Gmask);
	blue_shift  = maskShift (display->format->Bmask);

	alloc_colors64k (red_bits, green_bits, blue_bits, red_shift, green_shift, blue_shift);
    } else {
	alloc_colors256 (get_color);
	SDL_SetColors (screen, arSDLColors, 0, 256);
    }

    return 1;
}


/*
 * Test whether the screen mode <width>x<height>x<depth> is
 * available. If not, find a supported screen mode which best
 * matches.
 */
static int find_best_mode (int *width, int *height, int depth, int *try_fs)
{
    int found = 0;

    DEBUG_LOG ("Function: find_best_mode(%d,%d,%d)\n", *width, *height, depth);

  restart:
    /* First test whether the specified mode is supported */
    found = SDL_VideoModeOK (*width, *height, depth, *try_fs ? SDL_FULLSCREEN : 0);

    if (!found && n_fullscreen_modes > 0) {
	/* The specified mode wasn't available, so we'll try and find
	 * a supported resolution which best matches it.
	 */
	int i;
	write_log ("SDLGFX: Requested mode (%dx%d%d) not available.\n", *width, *height, depth);

	for (i = 0; i < n_fullscreen_modes; i--) {
	    if (gfx_fullscreen_modes[i].w >= *width && gfx_fullscreen_modes[i].h >= *height)
		break;
	}

	/* If we didn't find a mode, use the largest supported mode */
	if (i == n_fullscreen_modes)
	    i = n_fullscreen_modes - 1;

	*width  = gfx_fullscreen_modes[i].w;
	*height = gfx_fullscreen_modes[i].h;
	found = 1;

	write_log ("SDLGFX: Using mode (%dx%d)\n", *width, *height);
    }
    if (!found && *try_fs) {
	*try_fs = 0;
	goto restart;
    }
    return found;
}

#ifdef PICASSO96
/*
 * Map an SDL pixel format to a P96 pixel format
 */
static int get_p96_pixel_format (const struct SDL_PixelFormat *fmt)
{
    if (fmt->BitsPerPixel == 8)
	return RGBFB_CLUT;

#ifdef WORDS_BIGENDIAN
    if (fmt->BitsPerPixel == 24) {
	if (fmt->Rmask == 0x00FF0000 && fmt->Gmask == 0x0000FF00 && fmt->Bmask == 0x000000FF)
	    return RGBFB_R8G8B8;
	if (fmt->Rmask == 0x000000FF && fmt->Gmask == 0x0000FF00 && fmt->Bmask == 0x00FF0000)
	    return RGBFB_B8G8R8;
    } else if (fmt->BitsPerPixel == 32) {
	if (fmt->Rmask == 0xFF000000 && fmt->Gmask == 0x00FF0000 && fmt->Bmask == 0x0000FF00)
	    return RGBFB_R8G8B8A8;
	if (fmt->Rmask == 0x00FF0000 && fmt->Gmask == 0x0000FF00 && fmt->Bmask == 0x000000FF)
	    return RGBFB_A8R8G8B8;
	if (fmt->Bmask == 0x00FF0000 && fmt->Gmask == 0x0000FF00 && fmt->Rmask == 0x000000FF)
	    return RGBFB_A8B8G8R8;
	if (fmt->Bmask == 0xFF000000 && fmt->Gmask == 0x00FF0000 && fmt->Rmask == 0x0000FF00)
	    return RGBFB_B8G8R8A8;
    } else if (fmt->BitsPerPixel == 16) {
	if (get_sdlgfx_type () == SDLGFX_DRIVER_QUARTZ) {
	    /* The MacOS X port of SDL lies about it's default pixel format
	     * for high-colour display. It's always R5G5B5. */
	    return RGBFB_R5G5B5;
	} else {
	    if (fmt->Rmask == 0xf800 && fmt->Gmask == 0x07e0 && fmt->Bmask == 0x001f)
		return RGBFB_R5G6B5;
	    if (fmt->Rmask == 0x7C00 && fmt->Gmask == 0x03e0 && fmt->Bmask == 0x001f)
		return RGBFB_R5G5B5;
	}
    } else if (fmt->BitsPerPixel == 15) {
	if (fmt->Rmask == 0x7C00 && fmt->Gmask == 0x03e0 && fmt->Bmask == 0x001f)
	    return RGBFB_R5G5B5;
    }
#else
    if (fmt->BitsPerPixel == 24) {
	if (fmt->Rmask == 0x00FF0000 && fmt->Gmask == 0x0000FF00 && fmt->Bmask == 0x000000FF)
	    return RGBFB_B8G8R8;
	if (fmt->Rmask == 0x000000FF && fmt->Gmask == 0x0000FF00 && fmt->Bmask == 0x00FF0000)
	    return RGBFB_R8G8B8;
    } else if (fmt->BitsPerPixel == 32) {
	if (fmt->Rmask == 0xFF000000 && fmt->Gmask == 0x00FF0000 && fmt->Bmask == 0x0000FF00)
	    return RGBFB_A8B8G8R8;
	if (fmt->Rmask == 0x00FF0000 && fmt->Gmask == 0x0000FF00 && fmt->Bmask == 0x000000FF)
	    return RGBFB_B8G8R8A8;
	if (fmt->Bmask == 0x00FF0000 && fmt->Gmask == 0x0000FF00 && fmt->Rmask == 0x000000FF)
	    return RGBFB_R8G8B8A8;
	if (fmt->Bmask == 0xFF000000 && fmt->Gmask == 0x00FF0000 && fmt->Rmask == 0x0000FF00)
	    return RGBFB_A8R8G8B8;
    } else if (fmt->BitsPerPixel == 16) {
	if (fmt->Rmask == 0xf800 && fmt->Gmask == 0x07e0 && fmt->Bmask == 0x001f)
	    return RGBFB_R5G6B5PC;
	if (fmt->Rmask == 0x7C00 && fmt->Gmask == 0x03e0 && fmt->Bmask == 0x001f)
	    return RGBFB_R5G5B5PC;
    } else if (fmt->BitsPerPixel == 15) {
	if (fmt->Rmask == 0x7C00 && fmt->Gmask == 0x03e0 && fmt->Bmask == 0x001f)
	    return RGBFB_R5G5B5PC;
    }
#endif

    return RGBFB_NONE;
}
#endif

/*
 * Build list of full-screen screen-modes supported by SDL
 * with the specified pixel format.
 *
 * Returns a count of the number of supported modes, -1 if any mode is supported,
 * or 0 if there are no modes with this pixel format.
 */
static void find_screen_modes (struct SDL_PixelFormat *vfmt)
{
    long count = 0;
    SDL_Rect **modes = SDL_ListModes (vfmt, SDL_FULLSCREEN | SDL_HWSURFACE);

    if (modes != 0 && modes != (SDL_Rect**)-1) {
	unsigned int i;
	int w = -1;
	int h = -1;

	/* Filter list of modes SDL gave us and ignore duplicates */
	for (i = 0; modes[i]; i++)
	    if (modes[i]->w != w || modes[i]->h != h) {
		count++;
		h = modes[i]->h;
		w = modes[i]->w;
	    }

	gfx_fullscreen_modes = malloc (sizeof (struct uae_rect) * count);
	n_fullscreen_modes = count;

	w = h = -1;
	for (i = count = 0; count < n_fullscreen_modes; i++) {
	    if (modes[i]->w != w || modes[i]->h != h) {
		/* Fill the array in reverse, as SDL gives us a list of
		   screenmodes sorted largest to smallest.  */
		gfx_fullscreen_modes[n_fullscreen_modes - 1 - count].w = w = modes[i]->w;
		gfx_fullscreen_modes[n_fullscreen_modes - 1 - count].h = h = modes[i]->h;
		count++;

		write_log ("SDLGFX: Found screenmode: %dx%d.\n", w, h);
	    }
	}
    }
}

/**
 ** Buffer methods for SDL surfaces that must be locked
 **/

int lockscr (void)
{
    int success = 0;

    if (!SDL_MUSTLOCK (screen))
	return 1;

    if (SDL_LockSurface (display) == 0) {
	gfxvidinfo.bufmem   = display->pixels;
	gfxvidinfo.rowbytes = display->pitch;

	if (display->pixels != old_pixels) {
	   /* If the address of the pixel data has
	    * changed, recalculate the row maps
	    */
	    init_row_map ();
	    old_pixels = display->pixels;
	}
	success = 1;
    }
    return success;
}

void unlockscr (void)
{
    if (SDL_MUSTLOCK (screen))
	SDL_UnlockSurface (display);
}

void flush_block (int first_line, int last_line)
{
    //    DEBUG_LOG ("Function: flush_block %d %d\n", first_line, last_line);

    SDL_UpdateRect (display, 0, first_line, current_width, last_line - first_line + 1);
}

void flush_screen (int first_line, int last_line)
{
    if (! (screen->flags & SDL_DOUBLEBUF))
	return;

    SDL_BlitSurface (display,0,screen,0);

    SDL_Flip (screen);
}

void flush_line (int line)
{
}

int graphics_setup (void)
{
    int result = 0;

    if (SDL_InitSubSystem (SDL_INIT_VIDEO) == 0) {
	const SDL_version   *version = SDL_Linked_Version ();
	const SDL_VideoInfo *info    = SDL_GetVideoInfo ();

	write_log ("SDLGFX: Initialized.\n");
	write_log ("SDLGFX: Using SDL version %d.%d.%d.\n", version->major, version->minor, version->patch);

	/* Find default display depth */
	bitdepth = info->vfmt->BitsPerPixel;
	bit_unit = info->vfmt->BytesPerPixel * 8;

	write_log ("SDLGFX: Display is %d bits deep.\n", bitdepth);

	/* Build list of screenmodes */
	find_screen_modes (info->vfmt);

	result = 1;
    } else
	write_log ("SDLGFX: initialization failed - %s\n", SDL_GetError());

    return result;
}

int graphics_subinit (void)
{
    Uint32 uiSDLVidModFlags = 0;

    DEBUG_LOG ("Function: graphics_subinit\n");

    if (screen_is_picasso) {
	// Set height, width for Picasso gfx
	current_width  = picasso_vidinfo.width;
	current_height = picasso_vidinfo.height;
	fullscreen = currprefs.gfx_pfullscreen;
	curr_gfx = 0;
    } else {
	// Set height, width for Amiga gfx
	fullscreen = currprefs.gfx_afullscreen;
	if (fullscreen)
	    curr_gfx = &currprefs.gfx_f;
	else
	    curr_gfx = &currprefs.gfx_w;

	current_width  = curr_gfx->width;
	current_height = curr_gfx->height;
    }
    find_best_mode (&current_width, &current_height, bitdepth, &fullscreen);
    if (!screen_is_picasso) {
	gfxvidinfo.width  = current_width;
	gfxvidinfo.height = current_height;
    }

    if (!fullscreen && prev_fullscreen) {
	struct uae_rect *biggest = gfx_fullscreen_modes + n_fullscreen_modes - 1;
	/* Try switching back to the previous screenmode.  */
	SDL_SetVideoMode (biggest->w, biggest->h, bitdepth, SDL_FULLSCREEN);
    }

    if (bitdepth == 8)
	uiSDLVidModFlags |= SDL_HWPALETTE;
    if (fullscreen) {
	uiSDLVidModFlags |= SDL_FULLSCREEN | SDL_HWSURFACE;
	if(!screen_is_picasso && 0 /*currprefs.gfx_vsync */)
	    uiSDLVidModFlags |= SDL_DOUBLEBUF;
    }

    DEBUG_LOG ("Resolution: %d x %d x %d (FS: %d)\n", current_width, current_height, bitdepth, fullscreen);

    screen = SDL_SetVideoMode (current_width, current_height, bitdepth, uiSDLVidModFlags);

    if (screen == NULL) {
#if 0
	gui_message ("Unable to set video mode: %s\n", SDL_GetError ());
#endif
	return 0;
    } else {
	/* Just in case we didn't get exactly what we asked for . . . */
	fullscreen   = ((screen->flags & SDL_FULLSCREEN) == SDL_FULLSCREEN);
	is_hwsurface = ((screen->flags & SDL_HWSURFACE)  == SDL_HWSURFACE);

	/* Are these values what we expected? */
#	ifdef PICASSO96
	    DEBUG_LOG ("P96 screen?    : %d\n", screen_is_picasso);
#	endif
	DEBUG_LOG ("Fullscreen?    : %d\n", fullscreen);
	DEBUG_LOG ("Mouse grabbed? : %d\n", mousegrab);
	DEBUG_LOG ("HW surface?    : %d\n", is_hwsurface);
	DEBUG_LOG ("Must lock?     : %d\n", SDL_MUSTLOCK (screen));
	DEBUG_LOG ("Bytes per Pixel: %d\n", screen->format->BytesPerPixel);
	DEBUG_LOG ("Bytes per Line : %d\n", screen->pitch);

	/* Set up buffer methods */

	if (screen->flags & SDL_DOUBLEBUF) {
	    display = SDL_CreateRGBSurface(SDL_HWSURFACE, screen->w, screen->h, screen->format->BitsPerPixel,
					  screen->format->Rmask, screen->format->Gmask, screen->format->Bmask, 0);

	} else {
	    display = screen;
	}

	/* Set UAE window title and icon name */
	SDL_WM_SetCaption (PACKAGE_NAME, PACKAGE_NAME);

	/* Mouse is now always grabbed when full-screen - to work around
	 * problems with full-screen mouse input in some SDL implementations */
	if (fullscreen)
	    SDL_WM_GrabInput (SDL_GRAB_ON);
	else
	    SDL_WM_GrabInput (mousegrab ? SDL_GRAB_ON : SDL_GRAB_OFF);

	/* Hide mouse cursor */
	SDL_ShowCursor (/*currprefs.hide_cursor || */fullscreen || mousegrab ? SDL_DISABLE : SDL_ENABLE);

	inputdevice_release_all_keys ();
	reset_hotkeys ();

	bit_unit = display->format->BytesPerPixel * 8;

#ifdef PICASSO96
	if (!screen_is_picasso) {
#endif
	    /* Initialize structure for Amiga video modes */
	    if (is_hwsurface) {
		SDL_LockSurface (display);
		gfxvidinfo.bufmem	 = 0;
		gfxvidinfo.emergmem	 = malloc (display->pitch);
		gfxvidinfo.maxblocklines = 0;
		SDL_UnlockSurface (display);
	    }

	    if (!is_hwsurface) {
		gfxvidinfo.bufmem	 = display->pixels;
		gfxvidinfo.emergmem	 = 0;
		gfxvidinfo.maxblocklines = gfxvidinfo.height;
	    }
	    gfxvidinfo.linemem		= 0;
	    gfxvidinfo.pixbytes		= bit_unit >> 3;
	    gfxvidinfo.rowbytes		= display->pitch;


	    SDL_SetColors (display, arSDLColors, 0, 256);

	    /* Force recalculation of row maps - if we're locking */
	    old_pixels = (void *)-1;
#ifdef PICASSO96
	} else {
	    /* Initialize structure for Picasso96 video modes */
	    picasso_vidinfo.pixbytes    = bit_unit >> 3;
	    picasso_vidinfo.rowbytes	= display->pitch;
	    picasso_vidinfo.extra_mem	= 1;
	    picasso_vidinfo.depth	= bitdepth;
	    picasso_has_invalid_lines	= 0;
	    picasso_invalid_start	= picasso_vidinfo.height + 1;
	    picasso_invalid_stop	= -1;

	    memset (picasso_invalid_lines, 0, sizeof picasso_invalid_lines);
	}
#endif
    }

    return 1;
}

int graphics_init (void)
{
    int success = 0;

    DEBUG_LOG ("Function: graphics_init\n");

    if (currprefs.color_mode > 5) {
	write_log ("Bad color mode selected. Using default.\n");
	currprefs.color_mode = 0;
    }

#ifdef PICASSO96
    screen_is_picasso = 0;
#endif
    mousegrab = 0;

    fixup_prefs_dimensions (&currprefs.gfx_w, gfx_windowed_modes, n_windowed_modes);
    fixup_prefs_dimensions (&currprefs.gfx_f, gfx_fullscreen_modes, n_fullscreen_modes);

    if (graphics_subinit ()) {
	if (init_colors ()) {
	    success = 1;
	}
    }
    return success;
}

void graphics_subshutdown (int final)
{
    DEBUG_LOG ("Function: graphics_subshutdown\n");

    if (display && display != screen)
	SDL_FreeSurface (display);

    /* XRandR requires that clients sometimes have to retry a request for
       changing screen size, which SDL doesn't do.  Work around the problem
       by reinitializing SDL.  */
    SDL_QuitSubSystem (SDL_INIT_VIDEO);
    SDL_InitSubSystem (SDL_INIT_VIDEO);

    prev_fullscreen = fullscreen;

    if (final && prev_fullscreen) {
	struct uae_rect *biggest = gfx_fullscreen_modes + n_fullscreen_modes - 1;
	/* Try switching back to the previous screenmode.  */
	SDL_SetVideoMode (biggest->w, biggest->h, bitdepth, SDL_FULLSCREEN);
    }

    display = screen = 0;

    if (gfxvidinfo.emergmem) {
	free (gfxvidinfo.emergmem);
	gfxvidinfo.emergmem = 0;
    }
}

void graphics_leave (void)
{
    DEBUG_LOG ("Function: graphics_leave\n");

    graphics_subshutdown (1);
    SDL_QuitSubSystem (SDL_INIT_VIDEO);
    dumpcustom ();
}

static int refresh_necessary = 0;

void handle_events (void)
{
    SDL_Event rEvent;

    gui_handle_events ();

    while (SDL_PollEvent (&rEvent)) {
	switch (rEvent.type) {
	case SDL_QUIT:
	    DEBUG_LOG ("Event: quit\n");
	    uae_quit ();
	    break;

	case SDL_MOUSEBUTTONDOWN:
	case SDL_MOUSEBUTTONUP: {
	    int state = (rEvent.type == SDL_MOUSEBUTTONDOWN);
	    int buttonno = -1;

	    DEBUG_LOG ("Event: mouse button %d %s\n", rEvent.button.button, state ? "down" : "up");

	    switch (rEvent.button.button) {
	    case SDL_BUTTON_LEFT:      buttonno = 0; break;
	    case SDL_BUTTON_MIDDLE:    buttonno = 2; break;
	    case SDL_BUTTON_RIGHT:     buttonno = 1; break;
#ifdef SDL_BUTTON_WHEELUP
	    case SDL_BUTTON_WHEELUP:   if (state) record_key (0x7a << 1); break;
	    case SDL_BUTTON_WHEELDOWN: if (state) record_key (0x7b << 1); break;
#endif
	    }
	    if (buttonno >= 0)
		setmousebuttonstate (0, buttonno, rEvent.type == SDL_MOUSEBUTTONDOWN ? 1:0);
	    break;
	}

	case SDL_KEYUP:
	case SDL_KEYDOWN: {
	    int state = (rEvent.type == SDL_KEYDOWN);
	    int keycode;
	    int ievent;

	    keycode = rEvent.key.keysym.sym;

	    DEBUG_LOG ("Event: key %d %s\n", keycode, state ? "down" : "up");

	    if ((ievent = match_hotkey_sequence (keycode, state))) {
		DEBUG_LOG ("Hotkey event: %d\n", ievent);
		handle_hotkey_event (ievent, state);
	    } else {
		inputdevice_do_keyboard (keysym2amiga (keycode), state);
	    }
	    break;
	}

	case SDL_MOUSEMOTION:
	    //DEBUG_LOG ("Event: mouse motion\n");

	    if (!fullscreen && !mousegrab) {
		setmousestate (0, 0,rEvent.motion.x, 1);
		setmousestate (0, 1,rEvent.motion.y, 1);
	    } else {
		setmousestate (0, 0, rEvent.motion.xrel, 0);
		setmousestate (0, 1, rEvent.motion.yrel, 0);
	    }
	    break;

	case SDL_ACTIVEEVENT:
	    if (rEvent.active.state & SDL_APPINPUTFOCUS && !rEvent.active.gain) {
		DEBUG_LOG ("Lost input focus\n");
		inputdevice_release_all_keys ();
		reset_hotkeys ();
	    }
	    break;
	} /* end switch() */
    } /* end while() */

#ifdef PICASSO96
    if (screen_is_picasso && refresh_necessary) {
	SDL_UpdateRect (screen, 0, 0, picasso_vidinfo.width, picasso_vidinfo.height);
	refresh_necessary = 0;
	memset (picasso_invalid_lines, 0, sizeof picasso_invalid_lines);
    } else if (screen_is_picasso && picasso_has_invalid_lines) {
	int i;
	int strt = -1;
	picasso_invalid_lines[picasso_vidinfo.height] = 0;
	for (i = picasso_invalid_start; i < picasso_invalid_stop + 2; i++) {
	    if (picasso_invalid_lines[i]) {
		picasso_invalid_lines[i] = 0;
		if (strt != -1)
		    continue;
		strt = i;
	    } else {
		if (strt == -1)
		    continue;
		SDL_UpdateRect (screen, 0, strt, picasso_vidinfo.width, i - strt);
		strt = -1;
	    }
	}
	if (strt != -1)
	    abort ();
    }
    picasso_has_invalid_lines = 0;
    picasso_invalid_start = picasso_vidinfo.height + 1;
    picasso_invalid_stop = -1;
#endif
}

static void switch_keymaps (void)
{
    set_default_hotkeys (get_default_cooked_hotkeys ());
}

int debuggable (void)
{
    return 1;
}

int needmousehack (void)
{
    return 1;
}

int mousehack_allowed (void)
{
    return 1;
}

void LED (int on)
{
}

#ifdef PICASSO96

void DX_Invalidate (int first, int last)
{
    DEBUG_LOG ("Function: DX_Invalidate %i - %i\n", first, last);

    if (is_hwsurface)
	return;

    if (first > last)
	return;

    picasso_has_invalid_lines = 1;
    if (first < picasso_invalid_start)
	picasso_invalid_start = first;
    if (last > picasso_invalid_stop)
	picasso_invalid_stop = last;

    while (first <= last) {
	picasso_invalid_lines[first] = 1;
	first++;
    }
}

int DX_BitsPerCannon (void)
{
    return 8;
}

static int palette_update_start = 256;
static int palette_update_end   = 0;

void DX_SetPalette (int start, int count)
{
    DEBUG_LOG ("Function: DX_SetPalette_real\n");

    if (! screen_is_picasso || picasso96_state.RGBFormat != RGBFB_CHUNKY)
	return;

    if (picasso_vidinfo.pixbytes != 1) {
	/* This is the case when we're emulating a 256 color display. */
	while (count-- > 0) {
	    int r = picasso96_state.CLUT[start].Red;
	    int g = picasso96_state.CLUT[start].Green;
	    int b = picasso96_state.CLUT[start].Blue;
	    picasso_vidinfo.clut[start++] =
				 (doMask256 (r, red_bits, red_shift)
				| doMask256 (g, green_bits, green_shift)
				| doMask256 (b, blue_bits, blue_shift));
	}
    } else {
	int i;
	for (i = start; i < start+count && i < 256;  i++) {
	    p96Colors[i].r = picasso96_state.CLUT[i].Red;
	    p96Colors[i].g = picasso96_state.CLUT[i].Green;
	    p96Colors[i].b = picasso96_state.CLUT[i].Blue;
	}
	SDL_SetColors (screen, &p96Colors[start], start, count);
    }
}

#if 0
void DX_SetPalette_vsync(void)
{
    if (palette_update_end > palette_update_start) {
	DX_SetPalette (palette_update_start,
				palette_update_end - palette_update_start);
    palette_update_end   = 0;
    palette_update_start = 0;
  }
}

int DX_Fill (int dstx, int dsty, int width, int height, uae_u32 color, RGBFTYPE rgbtype)
{
    int result = 0;
    SDL_Rect rect = {dstx, dsty, width, height};

    DEBUG_LOG ("DX_Fill (x:%d y:%d w:%d h:%d color=%08x)\n", dstx, dsty, width, height, color);

    if (SDL_FillRect (screen, &rect, color) == 0) {
	DX_Invalidate (dsty, dsty + height - 1);
	result = 1;
    }
    return result;
}

int DX_Blit (int srcx, int srcy, int dstx, int dsty, int width, int height, BLIT_OPCODE opcode)
{
    int result = 0;
    SDL_Rect src_rect  = {srcx, srcy, width, height};
    SDL_Rect dest_rect = {dstx, dsty, 0, 0};

    DEBUG_LOG ("DX_Blit (sx:%d sy:%d dx:%d dy:%d w:%d h:%d op:%d)\n",
	       srcx, srcy, dstx, dsty, width, height, opcode);

    if (opcode == BLIT_SRC && SDL_BlitSurface (screen, &src_rect, screen, &dest_rect) == 0) {
	DX_Invalidate (dsty, dsty + height - 1);
	result = 1;
    }
    return result;
}
#endif

/*
 * Add a screenmode to the emulated P96 display database
 */
static void add_p96_mode (int width, int height, int emulate_chunky, int *count)
{
    unsigned int i;

    for (i = 0; i <= (emulate_chunky ? 1 : 0); i++) {
	if (*count < MAX_PICASSO_MODES) {
	    DisplayModes[*count].res.width  = width;
	    DisplayModes[*count].res.height = height;
	    DisplayModes[*count].depth      = (i == 1) ? 1 : bit_unit >> 3;
	    DisplayModes[*count].refresh    = 75;
	    (*count)++;

	    write_log ("SDLGFX: Added P96 mode: %dx%dx%d\n", width, height, (i == 1) ? 8 : bitdepth);
	}
    }
    return;
}

int DX_FillResolutions (uae_u16 *ppixel_format)
{
    int i;
    int count = 0;
    int emulate_chunky = 0;

    DEBUG_LOG ("Function: DX_FillResolutions\n");

    /* Find supported pixel formats */
    picasso_vidinfo.rgbformat = get_p96_pixel_format (SDL_GetVideoInfo()->vfmt);

    *ppixel_format = 1 << picasso_vidinfo.rgbformat;
    if (bit_unit == 16 || bit_unit == 32) {
	*ppixel_format |= RGBFF_CHUNKY;
	emulate_chunky = 1;
    }

    /* Check list of standard P96 screenmodes */
    for (i = 0; i < MAX_SCREEN_MODES; i++) {
	if (SDL_VideoModeOK (x_size_table[i], y_size_table[i], bitdepth,
			     SDL_HWSURFACE | SDL_FULLSCREEN)) {
	    add_p96_mode (x_size_table[i], y_size_table[i], emulate_chunky, &count);
	}
    }

    /* Check list of supported SDL screenmodes */
    for (i = 0; i < n_fullscreen_modes; i++) {
	int j;
	int found = 0;
	for (j = 0; j < MAX_SCREEN_MODES - 1; j++) {
	    if (gfx_fullscreen_modes[i].w == x_size_table[j] &&
		gfx_fullscreen_modes[i].h == y_size_table[j])
	    {
		found = 1;
		break;
	    }
	}

	/* If SDL mode is not a standard P96 mode (and thus already added to the
	 * list, above) then add it */
	if (!found)
	    add_p96_mode (gfx_fullscreen_modes[i].w, gfx_fullscreen_modes[i].h,
			  emulate_chunky, &count);
    }

    return count;
}

uae_u8 *gfx_lock_picasso (void)
{
    DEBUG_LOG ("Function: gfx_lock_picasso\n");

    if (SDL_MUSTLOCK (screen))
	SDL_LockSurface (screen);
    picasso_vidinfo.rowbytes = screen->pitch;
    return screen->pixels;
}

void gfx_unlock_picasso (void)
{
    DEBUG_LOG ("Function: gfx_unlock_picasso\n");

    if (SDL_MUSTLOCK (screen))
	SDL_UnlockSurface (screen);
}
#endif /* PICASSO96 */

void toggle_fullscreen (void)
{
    changed_prefs.gfx_afullscreen = changed_prefs.gfx_pfullscreen = !fullscreen;

    DEBUG_LOG ("ToggleFullScreen: %d\n", fullscreen );
};

void toggle_mousegrab (void)
{
    if (!fullscreen) {
	if (SDL_WM_GrabInput (SDL_GRAB_QUERY) == SDL_GRAB_OFF) {
	    if (SDL_WM_GrabInput (SDL_GRAB_ON) == SDL_GRAB_ON) {
		SDL_WarpMouse (0, 0);
		mousegrab = 1;
		SDL_ShowCursor (SDL_DISABLE);
	    }
	} else {
	    if (SDL_WM_GrabInput (SDL_GRAB_OFF) == SDL_GRAB_OFF) {
		mousegrab = 0;
		SDL_ShowCursor (SDL_DISABLE);
	    }
	}
    }
}

/*
 * Mouse inputdevice functions
 */

/* Hardwire for 3 axes and 3 buttons - although SDL doesn't
 * currently support a Z-axis as such. Mousewheel events are supplied
 * as buttons 4 and 5
 */
#define MAX_BUTTONS	3
#define MAX_AXES	3
#define FIRST_AXIS	0
#define FIRST_BUTTON	MAX_AXES

static int init_mouse (void)
{
   return 1;
}

static void close_mouse (void)
{
   return;
}

static int acquire_mouse (unsigned int num, int flags)
{
   /* SDL supports only one mouse */
   return 1;
}

static void unacquire_mouse (unsigned int num)
{
   return;
}

static int get_mouse_num (void)
{
    return 1;
}

static const char *get_mouse_name (unsigned int mouse)
{
    return "Default mouse";
}

static int get_mouse_widget_num (unsigned int mouse)
{
    return MAX_AXES + MAX_BUTTONS;
}

static int get_mouse_widget_first (unsigned int mouse, int type)
{
    switch (type) {
	case IDEV_WIDGET_BUTTON:
	    return FIRST_BUTTON;
	case IDEV_WIDGET_AXIS:
	    return FIRST_AXIS;
    }
    return -1;
}

static int get_mouse_widget_type (unsigned int mouse, unsigned int num, char *name, uae_u32 *code)
{
    if (num >= MAX_AXES && num < MAX_AXES + MAX_BUTTONS) {
	if (name)
	    sprintf (name, "Button %d", num + 1 + MAX_AXES);
	return IDEV_WIDGET_BUTTON;
    } else if (num < MAX_AXES) {
	if (name)
	    sprintf (name, "Axis %d", num + 1);
	return IDEV_WIDGET_AXIS;
    }
    return IDEV_WIDGET_NONE;
}

static void read_mouse (void)
{
    /* We handle mouse input in handle_events() */
}

struct inputdevice_functions inputdevicefunc_mouse = {
    init_mouse,
    close_mouse,
    acquire_mouse,
    unacquire_mouse,
    read_mouse,
    get_mouse_num,
    get_mouse_name,
    get_mouse_widget_num,
    get_mouse_widget_type,
    get_mouse_widget_first
};

/*
 * Keyboard inputdevice functions
 */
static int get_kb_num (void)
{
    /* SDL supports only one keyboard */
    return 1;
}

static const char *get_kb_name (unsigned int kb)
{
    return "Default keyboard";
}

static int get_kb_widget_num (unsigned int kb)
{
    return 255; // fix me
}

static int get_kb_widget_first (unsigned int kb, int type)
{
    return 0;
}

static int get_kb_widget_type (unsigned int kb, unsigned int num, char *name, uae_u32 *code)
{
    // fix me
    *code = num;
    return IDEV_WIDGET_KEY;
}

static int init_kb (void)
{
    struct uae_input_device_kbr_default *keymap = 0;

    /* See if we support raw keys on this platform */
    if ((keymap = get_default_raw_keymap (get_sdlgfx_type ())) != 0) {
	inputdevice_setkeytranslation (keymap);
	have_rawkeys = 1;
    }
    switch_keymaps ();

    return 1;
}

static void close_kb (void)
{
}

static int keyhack (int scancode, int pressed, int num)
{
    return scancode;
}

static void read_kb (void)
{
}

static int acquire_kb (unsigned int num, int flags)
{
    return 1;
}

static void unacquire_kb (unsigned int num)
{
}

struct inputdevice_functions inputdevicefunc_keyboard =
{
    init_kb,
    close_kb,
    acquire_kb,
    unacquire_kb,
    read_kb,
    get_kb_num,
    get_kb_name,
    get_kb_widget_num,
    get_kb_widget_type,
    get_kb_widget_first
};

//static int capslockstate;

int getcapslockstate (void)
{
// TODO
//    return capslockstate;
    return 0;
}
void setcapslockstate (int state)
{
// TODO
//    capslockstate = state;
}


/*
 * Default inputdevice config for SDL mouse
 */
void input_get_default_mouse (struct uae_input_device *uid)
{
    /* SDL supports only one mouse */
    uid[0].eventid[ID_AXIS_OFFSET + 0][0]   = INPUTEVENT_MOUSE1_HORIZ;
    uid[0].eventid[ID_AXIS_OFFSET + 1][0]   = INPUTEVENT_MOUSE1_VERT;
    uid[0].eventid[ID_AXIS_OFFSET + 2][0]   = INPUTEVENT_MOUSE1_WHEEL;
    uid[0].eventid[ID_BUTTON_OFFSET + 0][0] = INPUTEVENT_JOY1_FIRE_BUTTON;
    uid[0].eventid[ID_BUTTON_OFFSET + 1][0] = INPUTEVENT_JOY1_2ND_BUTTON;
    uid[0].eventid[ID_BUTTON_OFFSET + 2][0] = INPUTEVENT_JOY1_3RD_BUTTON;
    uid[0].enabled = 1;
}

/*
 * Handle gfx specific cfgfile options
 */

void target_save_options (FILE *f, const struct uae_prefs *p)
{
}

int target_parse_option (struct uae_prefs *p, const char *option, const char *value)
{
    return 0;
}

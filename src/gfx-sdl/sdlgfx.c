 /*
  * UAE - The Un*x Amiga Emulator
  *
  * SDL interface
  *
  * Copyright 2001 Bernd Lachner (EMail: dev@lachner-net.de)
  * Copyright 2003 Richard Drummond
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

#include <unistd.h>
#include <signal.h>

#include <SDL.h>
#include <SDL_endian.h>

#include "config.h"
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

/* Uncomment for debugging output */
//#define DEBUG
#ifdef DEBUG
#define DEBUG_LOG write_log
#else
#define DEBUG_LOG(...) do ; while(0)
#endif

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

/* SDL variable for output surface */
static SDL_Surface *prSDLScreen = NULL;

/* Possible screen modes (x and y resolutions) */
#define MAX_SCREEN_MODES 12
static int x_size_table[MAX_SCREEN_MODES] = { 320, 320, 320, 320, 640, 640, 640, 800, 1024, 1152, 1280, 1280 };
static int y_size_table[MAX_SCREEN_MODES] = { 200, 240, 256, 400, 350, 480, 512, 600, 768,  864,  960,  1024 };

static int red_bits, green_bits, blue_bits;
static int red_shift, green_shift, blue_shift;

static int screen_is_picasso;
static char picasso_invalid_lines[1201];
static int picasso_has_invalid_lines;
static int picasso_invalid_start, picasso_invalid_stop;
static int picasso_maxw = 0, picasso_maxh = 0;

static int bitdepth, bit_unit;
static int current_width, current_height;

static SDL_Color arSDLColors[256];
static int ncolors = 0;

static int keystate[256];

static int fullscreen;
static int mousegrab;

static void togglefullscreen(void);
static void handle_mousegrab(void);
static void handle_inhibit(void);
static void framerate_up(void);
static void framerate_down(void);
static void handle_interpol (void);

struct SDLHotKey
{
    SDLKey aHotKeys[2];
    void (*pfHandler)(void);
    long aPressedKeys[2];
};

#ifdef USE_F11_FOR_HOTKEYS
/* This is a hack which will be cleaned up later - Rich.
 * The F12 key doesn't seem to work under MacOS X (at least on the two systems
 * I've tried): key up/down events are only generated when the key is released,
 * so it's no use as a hot-key modifier. Use F11 instead.
 */
static struct SDLHotKey arHotKeys[] =
{
    {{ SDLK_F11, SDLK_s},	 togglefullscreen,	{0, 0} },
    {{ SDLK_F11, SDLK_q},	 uae_quit,		{0, 0} },
    {{ SDLK_F11, SDLK_m},	 togglemouse,		{0, 0} },
    {{ SDLK_F11, SDLK_g},	 handle_mousegrab,	{0, 0} },
    {{ SDLK_F11, SDLK_i},	 handle_inhibit,	{0, 0} },
    {{ SDLK_F11, SDLK_p},	 handle_interpol,	{0, 0} },
    {{ SDLK_F11, SDLK_KP_PLUS},	 framerate_up,		{0, 0} },
    {{ SDLK_F11, SDLK_KP_MINUS}, framerate_down,	{0, 0} },
    {{ 0, 0 }, 			 NULL, 			{0, 0} }  /* List must be terminated */
};
#else
static struct SDLHotKey arHotKeys[] =
{
    {{ SDLK_F12, SDLK_s},	 togglefullscreen,	{0, 0} },
    {{ SDLK_F12, SDLK_q},	 uae_quit,		{0, 0} },
    {{ SDLK_F12, SDLK_m},	 togglemouse,		{0, 0} },
    {{ SDLK_F12, SDLK_g},	 handle_mousegrab,	{0, 0} },
    {{ SDLK_F12, SDLK_i},	 handle_inhibit,	{0, 0} },
    {{ SDLK_F12, SDLK_p},	 handle_interpol,	{0, 0} },
    {{ SDLK_F12, SDLK_KP_PLUS},	 framerate_up,		{0, 0} },
    {{ SDLK_F12, SDLK_KP_MINUS}, framerate_down,	{0, 0} },
    {{ 0, 0 }, 			 NULL, 			{0, 0} }  /* List must be terminated */
};
#endif

void flush_line (int y)
{
    DEBUG_LOG ("Function: flush_line\n");

    /* Not implemented for SDL output */
}

void flush_block (int ystart, int ystop)
{
    DEBUG_LOG ("Function: flush_block %d %d\n", ystart, ystop);

    SDL_UnlockSurface (prSDLScreen);
    SDL_UpdateRect (prSDLScreen, 0, ystart, current_width, ystop - ystart + 1);
    SDL_LockSurface (prSDLScreen);
}

void flush_screen (int ystart, int ystop)
{
    DEBUG_LOG ("Function: flush_screen\n");

#if 0
    SDL_UpdateRect(prSDLScreen, 0, 0, current_width, current_height);
#endif
}

void flush_clear_screen (void)
{
   DEBUG_LOG ("Function: flush_clear_screen\n");

   /* Not implemented yet. It probably needs to be - Rich */
}


STATIC_INLINE int bitsInMask (unsigned long mask)
{
    /* count bits in mask */
    int n = 0;
    while (mask) {
	n += mask & 1;
	mask >>= 1;
    }
    return n;
}

STATIC_INLINE int maskShift (unsigned long mask)
{
    /* determine how far mask is shifted */
    int n = 0;
    while (!(mask & 1)) {
	n++;
	mask >>= 1;
    }
    return n;
}

static int get_color (int r, int g, int b, xcolnr *cnp)
{
    DEBUG_LOG ("Function: get_color\n");

    *cnp = SDL_MapRGB (prSDLScreen->format, r, g, b);
    arSDLColors[ncolors].r = r;
    arSDLColors[ncolors].g = g;
    arSDLColors[ncolors].b = b;

    ncolors++;
    return 1;
}

static int init_colors (void)
{
    int i;

    DEBUG_LOG ("Function: init_colors\n");

    if (bitdepth > 8) {
	red_bits    = bitsInMask (prSDLScreen->format->Rmask);
	green_bits  = bitsInMask (prSDLScreen->format->Gmask);
	blue_bits   = bitsInMask (prSDLScreen->format->Bmask);
	red_shift   = maskShift (prSDLScreen->format->Rmask);
	green_shift = maskShift (prSDLScreen->format->Gmask);
	blue_shift  = maskShift (prSDLScreen->format->Bmask);
	alloc_colors64k (red_bits, green_bits, blue_bits, red_shift, green_shift, blue_shift, 0, 0, 0);
    } else {
	alloc_colors256 (get_color);
	SDL_SetColors (prSDLScreen, arSDLColors, 0, 256);
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

    if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
	switch (gfxvidinfo.pixbytes) {
	    case 4:
		for (i = 0; i < 4096; i++)
		    SDL_Swap32 (xcolors[i]);
		break;
	    case 2:
		for (i = 0; i < 4096; i++)
		    SDL_Swap16 (xcolors[i]);
		break;
	}
    }

    return 1;
}

/*
 * Find the colour depth of the display
 */
static int get_display_depth (void)
{
    const SDL_VideoInfo *vid_info;
    int depth = 0;

    DEBUG_LOG ("Function: get_display_depth()\n");

    if ((vid_info = SDL_GetVideoInfo())) {
        depth = vid_info->vfmt->BitsPerPixel;

	/* Don't trust the answer if it's 16 bits; the display
	 * could actually be 15 bits deep. We'll count the bits
	 * ourselves */
	if (depth == 16)
	    depth = bitsInMask (vid_info->vfmt->Rmask) +
	            bitsInMask (vid_info->vfmt->Gmask) +
	            bitsInMask (vid_info->vfmt->Bmask);

	    DEBUG_LOG ("Display is %d bits deep\n", depth);
    }
    return depth;
}

/*
 * Test whether the screen mode <width>x<height>x<depth> is
 * available. If not, find a standard screen mode which best
 * matches.
 */
static int find_best_mode (int *width, int *height, int depth)
{
    int found = 0;

    DEBUG_LOG ("Function: find_best_mode(%d,%d,%d)\n", *width, *height, depth);

    /* First test whether the specified mode is supported */
    found = SDL_VideoModeOK (*width, *height, depth, SDL_SWSURFACE);

    if (!found) {
	/* The specified mode wasn't available, so we'll try and find
	 * a standard resolution which best matches it.
	 * Note: this should rarely be necessary.
	 */
        int i;
        DEBUG_LOG ("Requested mode not available\n");

	for (i = 0; i < MAX_SCREEN_MODES && !found; i++) {
	    if (x_size_table[i] < *width || y_size_table[i] < *height)
		continue; /* too small - try next mode */
	    found = SDL_VideoModeOK (x_size_table[i], y_size_table[i], bitdepth, SDL_SWSURFACE);
	    if (found) {
		*width  = x_size_table[i];
		*height = y_size_table[i];

		DEBUG_LOG ("Using mode: %dx%d\n", *width, *height);
	    }
	}
    }
    return found;
}

int graphics_setup (void)
{
    DEBUG_LOG ("Function: graphics_setup\n");

    return (SDL_InitSubSystem(SDL_INIT_VIDEO)==0);
}

static int graphics_subinit (void)
{
    Uint32 uiSDLVidModFlags;

    DEBUG_LOG ("Function: graphics_subinit\n");

    uiSDLVidModFlags = SDL_SWSURFACE;
    if (bitdepth == 8)
	uiSDLVidModFlags |= SDL_HWPALETTE;
    if (fullscreen)
	uiSDLVidModFlags |= SDL_FULLSCREEN;

    write_log ("Resolution: %d x %d x %d\n", current_width, current_height, bitdepth);

    prSDLScreen = SDL_SetVideoMode (current_width, current_height, bitdepth, uiSDLVidModFlags);

    /* Are these values what we expected? */
    DEBUG_LOG ("P96 screen?   : %d\n", screen_is_picasso);
    DEBUG_LOG ("Fullscreen    : %d\n", fullscreen);
    DEBUG_LOG ("Mouse grabbed?: %d\n", SDL_WM_GrabInput (SDL_GRAB_QUERY));

    if (prSDLScreen == NULL) {
	write_log ("Unable to set video mode: %s\n", SDL_GetError ());
	return 0;
    } else {
	DEBUG_LOG ("Bytes per Pixel: %d\n", prSDLScreen->format->BytesPerPixel);
	DEBUG_LOG ("Bytes per Line:  %d\n", prSDLScreen->pitch);

	/* CLear surface */
	SDL_LockSurface (prSDLScreen);
	memset (prSDLScreen->pixels, 0, current_width * current_height * prSDLScreen->format->BytesPerPixel);
	SDL_UnlockSurface (prSDLScreen);
	SDL_UpdateRect (prSDLScreen, 0, 0, current_width, current_height);

	/* Set UAE window title and icon name */
	SDL_WM_SetCaption ("UAE","UAE");

	/* Hide mouse cursor */
	SDL_ShowCursor (SDL_DISABLE);

	if (!screen_is_picasso) {
	    /* Initialize structure for Amiga video modes */
	    gfxvidinfo.bufmem = prSDLScreen->pixels;
	    gfxvidinfo.linemem = 0;
	    gfxvidinfo.emergmem = 0;
	    gfxvidinfo.pixbytes = prSDLScreen->format->BytesPerPixel;
	    bit_unit = prSDLScreen->format->BytesPerPixel * 8;
	    gfxvidinfo.rowbytes = prSDLScreen->pitch;
	    gfxvidinfo.maxblocklines = 100;
	    gfxvidinfo.can_double = 0;
	} else {
	    /* Initialize structure for Picasso96 video modes */
	    picasso_vidinfo.rowbytes = current_width * gfxvidinfo.pixbytes;
	    picasso_vidinfo.extra_mem = 1;
	    picasso_vidinfo.depth = bitdepth;
	    picasso_has_invalid_lines = 0;
	    picasso_invalid_start = picasso_vidinfo.height + 1;
	    picasso_invalid_stop = -1;
	    memset (picasso_invalid_lines, 0, sizeof picasso_invalid_lines);
	}
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

    screen_is_picasso = 0;
    fullscreen = currprefs.gfx_afullscreen;
    mousegrab = 0;

    fixup_prefs_dimensions (&currprefs);

    current_width  = currprefs.gfx_width_win;
    current_height = currprefs.gfx_height_win;
    bitdepth       = get_display_depth();

    if (find_best_mode (&current_width, &current_height, bitdepth)) {
        gfxvidinfo.width  = current_width;
        gfxvidinfo.height = current_height;

	if (graphics_subinit ()) {
	    if (init_colors ()) {
		int i;
        	for (i = 0; i < 256; i++)
		    keystate[i] = 0;

		success = 1;
	    }
	}
    }

    return success;
}

static void graphics_subshutdown (void)
{
    DEBUG_LOG ("Function: graphics_subshutdown\n");

    SDL_FreeSurface (prSDLScreen);
}

void graphics_leave (void)
{
    DEBUG_LOG ("Function: graphics_leave\n");

    graphics_subshutdown ();
    dumpcustom ();
}

/* Decode KeySyms. This function knows about all keys that are common
 * between different keyboard languages. */
static int kc_decode (SDL_keysym *prKeySym)
{
    switch (prKeySym->sym) {
	case SDLK_b: return AK_B;
	case SDLK_c: return AK_C;
	case SDLK_d: return AK_D;
	case SDLK_e: return AK_E;
	case SDLK_f: return AK_F;
	case SDLK_g: return AK_G;
	case SDLK_h: return AK_H;
	case SDLK_i: return AK_I;
	case SDLK_j: return AK_J;
	case SDLK_k: return AK_K;
	case SDLK_l: return AK_L;
	case SDLK_n: return AK_N;
	case SDLK_o: return AK_O;
	case SDLK_p: return AK_P;
	case SDLK_r: return AK_R;
	case SDLK_s: return AK_S;
	case SDLK_t: return AK_T;
	case SDLK_u: return AK_U;
	case SDLK_v: return AK_V;
	case SDLK_x: return AK_X;

	case SDLK_0: return AK_0;
	case SDLK_1: return AK_1;
	case SDLK_2: return AK_2;
	case SDLK_3: return AK_3;
	case SDLK_4: return AK_4;
	case SDLK_5: return AK_5;
	case SDLK_6: return AK_6;
	case SDLK_7: return AK_7;
	case SDLK_8: return AK_8;
	case SDLK_9: return AK_9;

	case SDLK_KP0: return AK_NP0;
	case SDLK_KP1: return AK_NP1;
	case SDLK_KP2: return AK_NP2;
	case SDLK_KP3: return AK_NP3;
	case SDLK_KP4: return AK_NP4;
	case SDLK_KP5: return AK_NP5;
	case SDLK_KP6: return AK_NP6;
	case SDLK_KP7: return AK_NP7;
	case SDLK_KP8: return AK_NP8;
	case SDLK_KP9: return AK_NP9;
	case SDLK_KP_DIVIDE: return AK_NPDIV;
	case SDLK_KP_MULTIPLY: return AK_NPMUL;
	case SDLK_KP_MINUS: return AK_NPSUB;
	case SDLK_KP_PLUS: return AK_NPADD;
	case SDLK_KP_PERIOD: return AK_NPDEL;
	case SDLK_KP_ENTER: return AK_ENT;

	case SDLK_F1: return AK_F1;
	case SDLK_F2: return AK_F2;
	case SDLK_F3: return AK_F3;
	case SDLK_F4: return AK_F4;
	case SDLK_F5: return AK_F5;
	case SDLK_F6: return AK_F6;
	case SDLK_F7: return AK_F7;
	case SDLK_F8: return AK_F8;
	case SDLK_F9: return AK_F9;
	case SDLK_F10: return AK_F10;

	case SDLK_BACKSPACE: return AK_BS;
	case SDLK_DELETE: return AK_DEL;
	case SDLK_LCTRL: return AK_CTRL;
	case SDLK_RCTRL: return AK_RCTRL;
	case SDLK_TAB: return AK_TAB;
	case SDLK_LALT: return AK_LALT;
	case SDLK_RALT: return AK_RALT;
	case SDLK_RMETA: return AK_RAMI;
	case SDLK_LMETA: return AK_LAMI;
	case SDLK_RETURN: return AK_RET;
	case SDLK_SPACE: return AK_SPC;
	case SDLK_LSHIFT: return AK_LSH;
	case SDLK_RSHIFT: return AK_RSH;
	case SDLK_ESCAPE: return AK_ESC;

	case SDLK_INSERT: return AK_HELP;
	case SDLK_HOME: return AK_NPLPAREN;
	case SDLK_END: return AK_NPRPAREN;
	case SDLK_CAPSLOCK: return AK_CAPSLOCK;

	case SDLK_UP: return AK_UP;
	case SDLK_DOWN: return AK_DN;
	case SDLK_LEFT: return AK_LF;
	case SDLK_RIGHT: return AK_RT;

	case SDLK_PAGEUP: return AK_RAMI;          /* PgUp mapped to right amiga */
	case SDLK_PAGEDOWN: return AK_LAMI;        /* PgDn mapped to left amiga */

	/* This should enable mapping of Windows keys
	 * to Amiga keys - Rich */
	case SDLK_LSUPER: return AK_LAMI;
	case SDLK_RSUPER: return AK_RAMI;

	default: return -1;
    }
}

static int decode_fr (SDL_keysym *prKeySym)
{
    switch(prKeySym->sym) {
	/* FR specific */
	case SDLK_a: return AK_Q;
	case SDLK_m: return AK_SEMICOLON;
    case SDLK_q: return AK_A;
    case SDLK_y: return AK_Y;
    case SDLK_w: return AK_Z;
    case SDLK_z: return AK_W;
    case SDLK_LEFTBRACKET: return AK_LBRACKET;
    case SDLK_RIGHTBRACKET: return AK_RBRACKET;
    case SDLK_COMMA: return AK_M;
    case SDLK_LESS: case SDLK_GREATER: return AK_LTGT;
    case SDLK_PERIOD: case SDLK_SEMICOLON: return AK_COMMA;
    case SDLK_RIGHTPAREN: return AK_MINUS;
    case SDLK_EQUALS: return AK_SLASH;
    case SDLK_HASH: return AK_NUMBERSIGN;
    case SDLK_SLASH: return AK_PERIOD;
    case SDLK_MINUS: return AK_EQUAL;
    case SDLK_BACKSLASH: return AK_BACKSLASH;
    default: return -1;
    }
}

static int decode_us (SDL_keysym *prKeySym)
{
    switch(prKeySym->sym)
    {
	/* US specific */
	case SDLK_a: return AK_A;
	case SDLK_m: return AK_M;
	case SDLK_q: return AK_Q;
	case SDLK_y: return AK_Y;
	case SDLK_w: return AK_W;
	case SDLK_z: return AK_Z;
	case SDLK_LEFTBRACKET: return AK_LBRACKET;
	case SDLK_RIGHTBRACKET: return AK_RBRACKET;
	case SDLK_COMMA: return AK_COMMA;
	case SDLK_PERIOD: return AK_PERIOD;
	case SDLK_SLASH: return AK_SLASH;
	case SDLK_SEMICOLON: return AK_SEMICOLON;
	case SDLK_MINUS: return AK_MINUS;
	case SDLK_EQUALS: return AK_EQUAL;
	case SDLK_QUOTE: return AK_QUOTE;
	case SDLK_BACKQUOTE: return AK_BACKQUOTE;
	case SDLK_BACKSLASH: return AK_BACKSLASH;
	default: return -1;
    }
}

static int decode_de (SDL_keysym *prKeySym)
{
    switch(prKeySym->sym) {
	/* DE specific */
	case SDLK_a: return AK_A;
	case SDLK_m: return AK_M;
	case SDLK_q: return AK_Q;
	case SDLK_w: return AK_W;
	case SDLK_y: return AK_Z;
	case SDLK_z: return AK_Y;
	/* German umlaut oe */
	case SDLK_WORLD_86: return AK_SEMICOLON;
	/* German umlaut ae */
	case SDLK_WORLD_68: return AK_QUOTE;
	/* German umlaut ue */
	case SDLK_WORLD_92: return AK_LBRACKET;
	case SDLK_PLUS: case SDLK_ASTERISK: return AK_RBRACKET;
	case SDLK_COMMA: return AK_COMMA;
	case SDLK_PERIOD: return AK_PERIOD;
	case SDLK_LESS: case SDLK_GREATER: return AK_LTGT;
	case SDLK_HASH: return AK_NUMBERSIGN;
	/* German sharp s */
	case SDLK_WORLD_63: return AK_MINUS;
	case SDLK_QUOTE: return AK_EQUAL;
	case SDLK_CARET: return AK_BACKQUOTE;
	case SDLK_MINUS: return AK_SLASH;
	default: return -1;
    }
}

static int decode_dk (SDL_keysym *prKeySym)
{
    switch(prKeySym->sym) {
	/* Partial mapping of DK-specific keys
	 * SDL has no keysyms for dead keys: diaeresis, acute, circumflex */
	case SDLK_a: return AK_A;
	case SDLK_m: return AK_M;
	case SDLK_q: return AK_Q;
	case SDLK_w: return AK_W;
	case SDLK_y: return AK_Y;
	case SDLK_z: return AK_Z;
	/* Danish AE */
	case SDLK_WORLD_88: return AK_SEMICOLON;
	/* Danish o oblique */
	case SDLK_WORLD_68: return AK_QUOTE;
	/* Danish A ring */
	case SDLK_WORLD_69: return AK_LBRACKET;
	/* one half - SDL has no 'section'? */
	case SDLK_WORLD_70: return AK_BACKQUOTE;
	case SDLK_COMMA: return AK_COMMA;
	case SDLK_PERIOD: return AK_PERIOD;
	case SDLK_LESS: case SDLK_GREATER: return AK_LTGT;
	case SDLK_HASH: return AK_NUMBERSIGN;
	case SDLK_PLUS: return AK_MINUS;
	case SDLK_MINUS: return AK_SLASH;
	default: return -1;
    }
}

static int decode_se (SDL_keysym *prKeySym)
{
    switch(prKeySym->sym) {
	/* SE specific */
	case SDLK_a: return AK_A;
	case SDLK_m: return AK_M;
	case SDLK_q: return AK_Q;
	case SDLK_w: return AK_W;
	case SDLK_y: return AK_Y;
	case SDLK_z: return AK_Z;
	case SDLK_WORLD_86: return AK_SEMICOLON;
	case SDLK_WORLD_68: return AK_QUOTE;
	case SDLK_WORLD_69: return AK_LBRACKET;
	case SDLK_COMMA: return AK_COMMA;
	case SDLK_PERIOD: return AK_PERIOD;
	case SDLK_MINUS: return AK_SLASH;
	case SDLK_LESS: case SDLK_GREATER: return AK_LTGT;
	case SDLK_PLUS: case SDLK_QUESTION: return AK_EQUAL;
	case SDLK_AT: case SDLK_WORLD_29: return AK_BACKQUOTE;
	case SDLK_CARET: return AK_RBRACKET;
	case SDLK_BACKSLASH: return AK_MINUS;
	case SDLK_HASH: return AK_NUMBERSIGN;
	default: return -1;
    }
}

static int decode_it (SDL_keysym *prKeySym)
{
    switch(prKeySym->sym) {
	/* IT specific */
	case SDLK_a: return AK_A;
	case SDLK_m: return AK_M;
	case SDLK_q: return AK_Q;
	case SDLK_w: return AK_W;
	case SDLK_y: return AK_Y;
	case SDLK_z: return AK_Z;
	case SDLK_WORLD_82: return AK_SEMICOLON;
	case SDLK_WORLD_64: return AK_QUOTE;
	case SDLK_WORLD_72: return AK_LBRACKET;
	case SDLK_PLUS: case SDLK_ASTERISK: return AK_RBRACKET;
	case SDLK_COMMA: return AK_COMMA;
	case SDLK_PERIOD: return AK_PERIOD;
	case SDLK_LESS: case SDLK_GREATER: return AK_LTGT;
	case SDLK_BACKSLASH: return AK_BACKQUOTE;
	case SDLK_QUOTE: return AK_MINUS;
	case SDLK_WORLD_76: return AK_EQUAL;
	case SDLK_MINUS: return AK_SLASH;
	case SDLK_HASH: return AK_NUMBERSIGN;
	default: return -1;
    }
}

static int decode_es (SDL_keysym *prKeySym)
{
    switch(prKeySym->sym) {
	/* ES specific */
	case SDLK_a: return AK_A;
	case SDLK_m: return AK_M;
	case SDLK_q: return AK_Q;
	case SDLK_w: return AK_W;
	case SDLK_y: return AK_Y;
	case SDLK_z: return AK_Z;
	case SDLK_WORLD_81: return AK_SEMICOLON;
	case SDLK_PLUS: case SDLK_ASTERISK: return AK_RBRACKET;
	case SDLK_COMMA: return AK_COMMA;
	case SDLK_PERIOD: return AK_PERIOD;
	case SDLK_LESS: case SDLK_GREATER: return AK_LTGT;
	case SDLK_BACKSLASH: return AK_BACKQUOTE;
	case SDLK_QUOTE: return AK_MINUS;
	case SDLK_WORLD_76: return AK_EQUAL;
	case SDLK_MINUS: return AK_SLASH;
	case SDLK_HASH: return AK_NUMBERSIGN;
	default: return -1;
    }
}

static int keycode2amiga(SDL_keysym *prKeySym)
{
    int iAmigaKeycode = kc_decode (prKeySym);

    if (iAmigaKeycode == -1) {
	switch (currprefs.keyboard_lang) {
	    case KBD_LANG_FR:
		return decode_fr (prKeySym);
	    case KBD_LANG_US:
		return decode_us (prKeySym);
	    case KBD_LANG_DE:
		return decode_de (prKeySym);
	    case KBD_LANG_DK:
		return decode_dk (prKeySym);
	    case KBD_LANG_SE:
		return decode_se (prKeySym);
	    case KBD_LANG_IT:
		return decode_it (prKeySym);
	    case KBD_LANG_ES:
		return decode_es (prKeySym);
	    default:
		return -1;
	}
    }
    return iAmigaKeycode;
}

static int refresh_necessary = 0;

void handle_events (void)
{
    SDL_Event rEvent;
    int iAmigaKeyCode;
    int i, j;
    int iIsHotKey = 0;
    int buttonno = 0;

//    DEBUG_LOG ("Function: handle_events\n");

    gui_handle_events ();

    while (SDL_PollEvent (&rEvent)) {
	switch (rEvent.type) {
	    case SDL_QUIT:
		DEBUG_LOG ("Event: quit\n");
		uae_quit();
		break;

	    case SDL_KEYDOWN:
		DEBUG_LOG ("Event: key down\n");

		/* Check for hotkey sequence */
		i = 0;
		while (arHotKeys[i].pfHandler != NULL) {
		    if (rEvent.key.keysym.sym == arHotKeys[i].aHotKeys[0]) {
			arHotKeys[i].aPressedKeys[0] = 1;
			iIsHotKey = 1;
		    }
		    if (arHotKeys[i].aPressedKeys[0] == 1 &&
			rEvent.key.keysym.sym == arHotKeys[i].aHotKeys[1]) {
			arHotKeys[i].aPressedKeys[1] = 1;
			arHotKeys[i].pfHandler();
			iIsHotKey = 1;
		    }
		    i++;
		}
		if (iIsHotKey == 0) {
		    /* No hotkey sequence */
		    iAmigaKeyCode = keycode2amiga (&(rEvent.key.keysym));
		    if (iAmigaKeyCode >= 0) {
			if (!keystate[iAmigaKeyCode]) {
			    keystate[iAmigaKeyCode] = 1;
			    record_key (iAmigaKeyCode << 1);
			}
		    }
		}
		break;

	    case SDL_KEYUP:
		DEBUG_LOG ("Event: key up\n");

		/* Check for hotkey sequence */
		i = 0;
		while (arHotKeys[i].pfHandler != NULL) {
		    for (j = 0; j < 2; j++) {
			if (rEvent.key.keysym.sym == arHotKeys[i].aHotKeys[j] &&
					arHotKeys[i].aPressedKeys[j] == 1) {
			    arHotKeys[i].aPressedKeys[j] = 0;
			    iIsHotKey = 1;
			}
		    }
		    i++;
		}
		if (iIsHotKey == 0) {
		    iAmigaKeyCode = keycode2amiga (&(rEvent.key.keysym));
		    if (iAmigaKeyCode >= 0) {
			keystate[iAmigaKeyCode] = 0;
			record_key ((iAmigaKeyCode << 1) | 1);
		    }
		}
	        break;

	    case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
		if (rEvent.type == SDL_MOUSEBUTTONDOWN)
		    DEBUG_LOG ("Event: mouse button down\n");
		else
		    DEBUG_LOG ("Event: mouse button up\n");

		switch (rEvent.button.button) {
		    case SDL_BUTTON_LEFT:   buttonno = 0; break;
		    case SDL_BUTTON_MIDDLE: buttonno = 2; break;
		    case SDL_BUTTON_RIGHT:  buttonno = 1; break;
		    default: buttonno = -1;
		}
		if (buttonno >= 0)
		    setmousebuttonstate (0, buttonno, rEvent.type == SDL_MOUSEBUTTONDOWN ? 1:0);
		break;

	    case SDL_MOUSEMOTION:
		DEBUG_LOG ("Event: mouse motion\n");

		if (!fullscreen && !mousegrab) {
		    setmousestate (0, 0,rEvent.motion.x, 1);
		    setmousestate (0, 1,rEvent.motion.y, 1);
		} else {
		    setmousestate (0, 0, rEvent.motion.xrel, 0);
		    setmousestate (0, 1, rEvent.motion.yrel, 0);
		}
		break;
	} /* end switch() */
    } /* end while() */

#if defined PICASSO96
    if (screen_is_picasso && refresh_necessary) {
	SDL_UpdateRect (prSDLScreen, 0, 0, picasso_vidinfo.width, picasso_vidinfo.height);
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
		SDL_UpdateRect (prSDLScreen, 0, strt, picasso_vidinfo.width, i - strt);
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

    /* Handle UAE reset */
    if ((keystate[AK_CTRL] || keystate[AK_RCTRL]) && keystate[AK_LAMI] && keystate[AK_RAMI])
	uae_reset (0);
}

int check_prefs_changed_gfx (void)
{
    if (changed_prefs.gfx_width_win  != currprefs.gfx_width_win
     || changed_prefs.gfx_height_win != currprefs.gfx_height_win
     || changed_prefs.gfx_width_fs   != currprefs.gfx_width_fs
     || changed_prefs.gfx_height_fs  != currprefs.gfx_height_fs) {
	fixup_prefs_dimensions (&changed_prefs);
    }
    else if (changed_prefs.gfx_lores          == currprefs.gfx_lores
	  && changed_prefs.gfx_linedbl        == currprefs.gfx_linedbl
	  && changed_prefs.gfx_correct_aspect == currprefs.gfx_correct_aspect
	  && changed_prefs.gfx_xcenter        == currprefs.gfx_xcenter
	  && changed_prefs.gfx_ycenter        == currprefs.gfx_ycenter
/*	  && changed_prefs.gfx_afullscreen    == currprefs.gfx_afullscreen
	  && changed_prefs.gfx_pfullscreen    == currprefs.gfx_pfullscreen */ // Won't work yet.
    ) {
	return 0;
    }

    DEBUG_LOG ("Function: check_prefs_changed_gfx\n");

    graphics_subshutdown ();

    currprefs.gfx_width_win	 = changed_prefs.gfx_width_win;
    currprefs.gfx_height_win	 = changed_prefs.gfx_height_win;
    currprefs.gfx_width_fs	 = changed_prefs.gfx_width_fs;
    currprefs.gfx_height_fs	 = changed_prefs.gfx_height_fs;
    currprefs.gfx_lores		 = changed_prefs.gfx_lores;
    currprefs.gfx_linedbl	 = changed_prefs.gfx_linedbl;
    currprefs.gfx_correct_aspect = changed_prefs.gfx_correct_aspect;
    currprefs.gfx_xcenter	 = changed_prefs.gfx_xcenter;
    currprefs.gfx_ycenter	 = changed_prefs.gfx_ycenter;
/*    currprefs.gfx_afullscreen	 = changed_prefs.gfx_afullscreen;
    currprefs.gfx_pfullscreen	 = changed_prefs.gfx_pfullscreen; */

    gui_update_gfx ();
    graphics_subinit ();

    /* Redundant? This is done in the caller
    notice_screen_contents_lost ();
    init_row_map (); */
    if (screen_is_picasso)
	picasso_enablescreen (1);

    return 0;
}

int debuggable (void)
{
    return 1;
}

int needmousehack (void)
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
    }
}

void DX_SetPalette_vsync(void)
{
    if (palette_update_end > palette_update_start) {
	DX_SetPalette (palette_update_start,
				palette_update_end - palette_update_start);
    palette_update_end   = 0;
    palette_update_start = 0;
  }
}

int DX_FillResolutions (uae_u16 *ppixel_format)
{
    int i, count = 0;
    int w = 0;
    int h = 0;
    int emulate_chunky = 0;

    DEBUG_LOG ("Function: DX_FillResolutions\n");

    /* In the new scheme of things, this function is called *before* graphics_init.
     * Hence, we need to find the display depth ourselves - Rich */
    bitdepth = get_display_depth ();
    bit_unit = (bitdepth + 1) & 0xF8;

    /* Find out, which is the highest resolution the SDL can offer */
    for (i = MAX_SCREEN_MODES-1; i>=0; i--) {
	if ( SDL_VideoModeOK (x_size_table[i], y_size_table[i],
						bitdepth, SDL_SWSURFACE)) {
	    w = x_size_table[i];
	    h = y_size_table[i];
	    break;
	}
    }

    DEBUG_LOG ("Max. Picasso screen size: %d x %d\n", w, h);

#ifdef WORDS_BIGENDIAN
    picasso_vidinfo.rgbformat = (bit_unit == 8 ? RGBFB_CHUNKY
				: bitdepth == 15 && bit_unit == 16 ? RGBFB_R5G5B5
				: bitdepth == 16 && bit_unit == 16 ? RGBFB_R5G6B5
				: bit_unit == 24 ? RGBFB_B8G8R8
				: bit_unit == 32 ? RGBFB_A8R8G8B8
				: RGBFB_NONE);
#else
    picasso_vidinfo.rgbformat = (bit_unit == 8 ? RGBFB_CHUNKY
				: bitdepth == 15 && bit_unit == 16 ? RGBFB_R5G5B5PC
				: bitdepth == 16 && bit_unit == 16 ? RGBFB_R5G6B5PC
				: bit_unit == 24 ? RGBFB_B8G8R8
				: bit_unit == 32 ? RGBFB_B8G8R8A8
				: RGBFB_NONE);
#endif

    *ppixel_format = 1 << picasso_vidinfo.rgbformat;
    if (bit_unit == 16 || bit_unit == 32) {
	*ppixel_format |= RGBFF_CHUNKY;
	emulate_chunky = 1;
    }

    for (i = 0; i < MAX_SCREEN_MODES && count < MAX_PICASSO_MODES; i++) {
	int j;
	for (j = 0; j <= emulate_chunky && count < MAX_PICASSO_MODES; j++) {
	    if (x_size_table[i] <= w && y_size_table[i] <= h) {
		if (x_size_table[i] > picasso_maxw)
		    picasso_maxw = x_size_table[i];
		if (y_size_table[i] > picasso_maxh)
		    picasso_maxh = y_size_table[i];
		DisplayModes[count].res.width = x_size_table[i];
		DisplayModes[count].res.height = y_size_table[i];
		DisplayModes[count].depth = j == 1 ? 1 : bit_unit >> 3;
		DisplayModes[count].refresh = 75;

 		DEBUG_LOG ("Picasso resolution %d x %d @ %d allowed\n",
			DisplayModes[count].res.width,
			DisplayModes[count].res.height,
			DisplayModes[count].depth);

		count++;
	    }
	}
    }
    DEBUG_LOG("Max. Picasso screen size: %d x %d\n", picasso_maxw, picasso_maxh);

    return count;
}

static void set_window_for_picasso (void)
{
    DEBUG_LOG ("Function: set_window_for_picasso\n");

    if (current_width == picasso_vidinfo.width && current_height == picasso_vidinfo.height)
	return;

    graphics_subshutdown();
    current_width  = picasso_vidinfo.width;
    current_height = picasso_vidinfo.height;
    graphics_subinit();
}

void gfx_set_picasso_modeinfo (int w, int h, int depth, int rgbfmt)
{
    DEBUG_LOG ("Function: gfx_set_picasso_modeinfo w: %i h: %i depth: %i rgbfmt: %i\n", w, h, depth, rgbfmt);

    picasso_vidinfo.width = w;
    picasso_vidinfo.height = h;
    picasso_vidinfo.depth = depth;
    picasso_vidinfo.pixbytes = bit_unit >> 3;
    if (screen_is_picasso)
	set_window_for_picasso();
}

void gfx_set_picasso_baseaddr (uaecptr a)
{
}

void gfx_set_picasso_state (int on)
{
    DEBUG_LOG ("Function: gfx_set_picasso_state: %d\n", on);

    if (on == screen_is_picasso)
	return;

    graphics_subshutdown ();
    screen_is_picasso = on;

    if (on) {
	// Set height, width for Picasso gfx
	current_width  = picasso_vidinfo.width;
	current_height = picasso_vidinfo.height;
	graphics_subinit ();
    } else {
	// Set height, width for Amiga gfx
	current_width  = gfxvidinfo.width;
	current_height = gfxvidinfo.height;
	graphics_subinit ();
	reset_drawing ();
    }

    if (on)
	DX_SetPalette (0, 256);
}

uae_u8 *gfx_lock_picasso (void)
{
    DEBUG_LOG ("Function: gfx_lock_picasso\n");

    SDL_LockSurface (prSDLScreen);
    return prSDLScreen->pixels;
}

void gfx_unlock_picasso (void)
{
    DEBUG_LOG ("Function: gfx_unlock_picasso\n");

    SDL_UnlockSurface (prSDLScreen);
}
#endif

int lockscr (void)
{
    DEBUG_LOG ("Function: lockscr\n");

    SDL_LockSurface (prSDLScreen);
    return 1;
}

void unlockscr (void)
{
    DEBUG_LOG ("Function: unlockscr\n");

    SDL_UnlockSurface (prSDLScreen);
}

static void togglefullscreen (void)
{
    /* FIXME: Add support for separate full-screen/windowed sizes */
    fullscreen = (fullscreen+1) & 1;

    if (!SDL_WM_ToggleFullScreen (prSDLScreen)) {
	/* If SDL_WM_ToggleFullScreen isn't supported, do things the
	 * hard way. Close down the window/screen and open a new one */
	graphics_subshutdown ();
	graphics_subinit ();
        if (!screen_is_picasso)
	  reset_drawing();
	notice_screen_contents_lost ();
    }
    DEBUG_LOG ("ToggleFullScreen: %d\n", fullscreen );
};

static void handle_mousegrab (void)
{
    if (SDL_WM_GrabInput (SDL_GRAB_QUERY) == SDL_GRAB_OFF) {
	SDL_WM_GrabInput (SDL_GRAB_ON);
	SDL_WarpMouse (0, 0);
        mousegrab = 1;
    } else {
	SDL_WM_GrabInput (SDL_GRAB_OFF);
        mousegrab = 0;
    }
}

static void handle_inhibit (void)
{
    toggle_inhibit_frame (IHF_SCROLLLOCK);
}

static void handle_interpol (void)
{
    if (currprefs.sound_interpol == 0) {
	currprefs.sound_interpol = 1;
	write_log ("Interpol on: rh\n");
    } else if (currprefs.sound_interpol == 1) {
	currprefs.sound_interpol = 2;
	write_log ("Interpol on: crux\n");
    } else {
	currprefs.sound_interpol = 0;
	write_log ("Interpol off\n");
    }
}

static void framerate_up (void)
{
    if (currprefs.gfx_framerate < 20)
	changed_prefs.gfx_framerate = currprefs.gfx_framerate + 1;
}

static void framerate_down (void)
{
    if (currprefs.gfx_framerate > 1)
	changed_prefs.gfx_framerate = currprefs.gfx_framerate - 1;
}

void target_save_options (FILE *f, struct uae_prefs *p)
{
}

int target_parse_option (struct uae_prefs *p, char *option, char *value)
{
    return 0;
}

 /*
  * UAE - The Un*x Amiga Emulator
  *
  * X interface
  *
  * Copyright 1995, 1996 Bernd Schmidt
  * Copyright 1996 Ed Hanway, Andre Beck, Samuel Devulder, Bruno Coste
  * Copyright 1998 Marcus Sundberg
  * DGA support by Kai Kollmorgen
  * X11/DGA merge, hotkeys and grabmouse by Marcus Sundberg
  * Copyright 2003-2004 Richard Drummond
  */

#include "sysconfig.h"
#include "sysdeps.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <X11/cursorfont.h>

#include <ctype.h>
#include <signal.h>

#include "options.h"
#include "threaddep/thread.h"
#include "uae.h"
#include "memory.h"
#include "xwin.h"
#include "custom.h"
#include "drawing.h"
#include "newcpu.h"
#include "keyboard.h"
#include "keybuf.h"
#include "gui.h"
#include "debug.h"
#include "picasso96.h"
#include "inputdevice.h"
#include "hotkeys.h"

#ifdef __cplusplus
#define VI_CLASS c_class
#else
#define VI_CLASS class
#endif

#ifdef USE_DGA_EXTENSION

#ifdef USE_VIDMODE_EXTENSION
#include <X11/extensions/xf86vmode.h>
#define VidMode_MINMAJOR 0
#define VidMode_MINMINOR 0
#endif

#include <X11/extensions/xf86dga.h>
#define DGA_MINMAJOR 0
#define DGA_MINMINOR 0

#endif /* USE_DGA_EXTENSION */

#if SHM_SUPPORT_LINKS == 1

#include <sys/ipc.h>
#include <sys/shm.h>
#include <X11/extensions/XShm.h>

#define DO_PUTIMAGE(IMG, SRCX, SRCY, DSTX, DSTY, WIDTH, HEIGHT) \
    do { \
	if (currprefs.x11_use_mitshm && shmavail) \
	     XShmPutImage (display, mywin, mygc, (IMG), (SRCX), (SRCY), (DSTX), (DSTY), (WIDTH), (HEIGHT), 0); \
	else \
	    XPutImage (display, mywin, mygc, (IMG), (SRCX), (SRCY), (DSTX), (DSTY), (WIDTH), (HEIGHT)); \
    } while (0)
#else
#define DO_PUTIMAGE(IMG, SRCX, SRCY, DSTX, DSTY, WIDTH, HEIGHT) \
    XPutImage (display, mywin, mygc, (IMG), (SRCX), (SRCY), (DSTX), (DSTY), (WIDTH), (HEIGHT))
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

struct disp_info {
    XImage *ximg;
    char *image_mem;
#if SHM_SUPPORT_LINKS == 1
    XShmSegmentInfo shminfo;
#endif
};

static Display *display;
static int screen;
static Window rootwin, mywin;
static Atom delete_win;

static GC mygc;
static XColor black, white;
static Colormap cmap, cmap2;
static int red_bits, green_bits, blue_bits;
static int red_shift, green_shift, blue_shift;

/* Kludge-O-Matic.
 * Unfortunately the X server loses colormap changes in DGA mode. Switching
 * back and forth between two identical colormaps fixes this problem.  */
static int dga_colormap_installed;

static int need_dither;

static char picasso_invalid_lines[1201];
static int picasso_has_invalid_lines;
static int picasso_invalid_start, picasso_invalid_stop;
static int picasso_maxw = 0, picasso_maxh = 0;

static int autorepeatoff = 0;
static struct disp_info ami_dinfo, pic_dinfo;
static Visual *vis;
static XVisualInfo visualInfo;
static int bitdepth, bit_unit;
static Cursor blankCursor, xhairCursor;
static int cursorOn;
static int inverse_byte_order = 0;

static int current_width, current_height;

static int x11_init_ok;
static int dgaavail = 0, vidmodeavail = 0, shmavail = 0;
static int dgamode;
static int grabbed;

void toggle_mousegrab (void);
void framerate_up (void);
void framerate_down (void);
int xkeysym2amiga (int);
struct uae_hotkeyseq *get_x11_default_hotkeys (void);

int pause_emulation;

static int oldx, oldy;
static int inwindow;

#define EVENTMASK (KeyPressMask|KeyReleaseMask|ButtonPressMask \
		   |ButtonReleaseMask|PointerMotionMask \
		   |FocusChangeMask|EnterWindowMask \
		   |ExposureMask |LeaveWindowMask)
#define DGA_EVENTMASK (KeyPressMask|KeyReleaseMask|ButtonPressMask \
		       |ButtonReleaseMask|PointerMotionMask)

#if SHM_SUPPORT_LINKS == 1
/* Hack to detect shm-failure, probably due to displaying on a
 * remote server. */
static int shmerror;
static int (*oldshmerrorhandler) (Display *, XErrorEvent *);

static int shmerrorhandler (Display *dsp, XErrorEvent *ev)
{
    if (ev->error_code == BadAccess)
	shmerror=1;
    else
	(*oldshmerrorhandler) (dsp, ev);
    return 0;
}
#endif

static void get_image (int w, int h, struct disp_info *dispi)
{
    XImage *new_img;
    char *p;

#if SHM_SUPPORT_LINKS == 1
    if (currprefs.x11_use_mitshm && shmavail) {
	XShmSegmentInfo *shminfo = &dispi->shminfo;

	new_img = XShmCreateImage (display, vis, bitdepth, ZPixmap, 0, shminfo, w, h);

	shminfo->shmid = shmget (IPC_PRIVATE, h * new_img->bytes_per_line,
				 IPC_CREAT | 0777);
	shminfo->shmaddr = new_img->data = (char *)shmat (shminfo->shmid, 0, 0);
	dispi->image_mem = new_img->data;
	shminfo->readOnly = False;
	/* Try to let the Xserver attach */
	shmerror = 0;
	oldshmerrorhandler = XSetErrorHandler (shmerrorhandler);
	XShmAttach (display, shminfo);
	XSync (display, 0);
	XSetErrorHandler (oldshmerrorhandler);
	if (shmerror) {
	    shmdt (shminfo->shmaddr);
	    XDestroyImage (new_img);
	    shminfo->shmid = -1;
	    shmavail = 0;
	    write_log ("MIT-SHM extension failed, trying fallback.\n");
	} else {
	    /* now deleting means making it temporary */
	    shmctl (shminfo->shmid, IPC_RMID, 0);
	    dispi->ximg = new_img;
	    write_log ("Using MIT-SHM extension.\n");
	    return;
	}
    }
#endif

    /* Question for people who know about X: Could we allocate the buffer
     * after creating the image and then do new_img->data = buffer, as above in
     * the SHM case?
     */
    write_log ("Using normal image buffer.\n");
    p = (char *)xmalloc (h * w * ((bit_unit + 7) / 8)); /* ??? */
    new_img = XCreateImage (display, vis, bitdepth, ZPixmap, 0, p,
			    w, h, 32, 0);
    if (new_img->bytes_per_line != w * ((bit_unit + 7) / 8))
	write_log ("Possible bug here... graphics may look strange.\n");

    dispi->image_mem = p;
    dispi->ximg = new_img;
}

#ifdef USE_VIDMODE_EXTENSION
static XF86VidModeModeInfo **allmodes;
static int vidmodecount;

static int sortfn (const void *a, const void *b)
{
    XF86VidModeInfo **ppa = a, *ppb = b;
    XF86VidModeInfo *pa = *ppa, *pb = *ppb;
    if (pa->hdisplay != pb->hdisplay)
	return pa->hdisplay - pb->hdisplay;
    return pa->vdisplay - pb->vdisplay;
}

static int get_vidmodes (void)
{
    int i;
    
    if (!XF86VidModeGetAllModeLines (display, screen, &vidmodecount, &allmodes))
	return 0;

    qsort (allmodes, vidmodecount, sizeof *allmode, sortfn);

    gfx_fullscreen_modes = sizeof (struct uae_rect) * vidmodecount;
    n_fullscreen_modes = vidmodecount;
    for (i = 0; i < vidmodecount; i++) {
	gfx_fullscreen_modes[i].w = allmodes[i].hdisplay;
	gfx_fullscreen_modes[i].h = allmodes[i].vdisplay;
    }
}
#endif

#ifdef USE_DGA_EXTENSION

static int fb_bank, fb_banks, fb_mem;
static char *fb_addr;
static int fb_width;

static void switch_to_best_mode (void)
{
    Screen *scr = ScreenOfDisplay (display, screen);
    int w = WidthOfScreen (scr);
    int h = HeightOfScreen (scr);
    int d = DefaultDepthOfScreen (scr);
#ifdef USE_VIDMODE_EXTENSION
    int i, best;
    if (vidmodeavail) {
	best = 0;
	for (i = 1; i < vidmodecount; i++) {
	    if (allmodes[i]->hdisplay >= current_width
		&& allmodes[i]->vdisplay >= current_height
		&& allmodes[i]->hdisplay <= allmodes[best]->hdisplay
		&& allmodes[i]->vdisplay <= allmodes[best]->vdisplay)
		best = i;
	}
	write_log ("entering DGA mode: %dx%d (%d, %d)\n",
		allmodes[best]->hdisplay, allmodes[best]->vdisplay,
		current_width, current_height);
	XF86VidModeSwitchToMode (display, screen, allmodes[best]);
	XF86VidModeSetViewPort (display, screen, 0, 0);
    }
#endif
    XMoveWindow (display, mywin, 0, 0);
    XWarpPointer (display, None, rootwin, 0, 0, 0, 0, 0, 0);
    XF86DGADirectVideo (display, screen, XF86DGADirectGraphics | XF86DGADirectMouse | XF86DGADirectKeyb);
    XF86DGASetViewPort (display, screen, 0, 0);
    memset (fb_addr, 0, (w * h) * (d / 8));
}

static void enter_dga_mode (void)
{
    XRaiseWindow (display, mywin);

    /* We want all the key presses */
    XGrabKeyboard (display, rootwin, 1, GrabModeAsync,
		   GrabModeAsync,  CurrentTime);

    /* and all the mouse moves */
    XGrabPointer (display, rootwin, 1, PointerMotionMask | ButtonPressMask | ButtonReleaseMask,
		  GrabModeAsync, GrabModeAsync, None,  None, CurrentTime);

    switch_to_best_mode ();

    gfxvidinfo.rowbytes = fb_width*gfxvidinfo.pixbytes;
    gfxvidinfo.bufmem = fb_addr;
    gfxvidinfo.linemem = 0;
    gfxvidinfo.emergmem = malloc (gfxvidinfo.rowbytes);
    gfxvidinfo.maxblocklines = 10000;
}

static void leave_dga_mode (void)
{
    XF86DGADirectVideo (display, screen, 0);
    XUngrabPointer (display, CurrentTime);
    XUngrabKeyboard (display, CurrentTime);
#ifdef USE_VIDMODE_EXTENSION
    if (vidmodeavail)
	XF86VidModeSwitchToMode (display, screen, allmodes[0]);
#endif
}
#endif

static char *oldpixbuf;

void flush_line (int y)
{
    char *linebuf = gfxvidinfo.linemem;
    int xs, xe;
    int len;

    if (linebuf == NULL)
	linebuf = y*gfxvidinfo.rowbytes + gfxvidinfo.bufmem;

#ifdef USE_DGA_EXTENSION
    if (dgamode && need_dither) {
	DitherLine ((unsigned char *)(fb_addr + fb_width*y),
		    (uae_u16 *)linebuf, 0, y, gfxvidinfo.width, bit_unit);
	return;
    }
#endif
    xs = 0;
    xe = gfxvidinfo.width - 1;

    if (currprefs.x11_use_low_bandwidth) {
	char *src, *dst;
	switch (gfxvidinfo.pixbytes) {
	 case 4:
	    {
		uae_u32 *newp = (uae_u32 *)linebuf;
		uae_u32 *oldp = (uae_u32 *)((uae_u8 *)ami_dinfo.image_mem + y*ami_dinfo.ximg->bytes_per_line);
		while (newp[xs] == oldp[xs]) {
		    if (xs == xe)
			return;
		    xs++;
		}
		while (newp[xe] == oldp[xe]) xe--;

		dst = (char *)(oldp + xs); src = (char *)(newp + xs);
	    }
	    break;
	 case 2:
	    {
		uae_u16 *newp = (uae_u16 *)linebuf;
		uae_u16 *oldp = (uae_u16 *)((uae_u8 *)ami_dinfo.image_mem + y*ami_dinfo.ximg->bytes_per_line);
		while (newp[xs] == oldp[xs]) {
		    if (xs == xe)
			return;
		    xs++;
		}
		while (newp[xe] == oldp[xe]) xe--;

		dst = (char *)(oldp + xs); src = (char *)(newp + xs);
	    }
	    break;
	 case 1:
	    {
		uae_u8 *newp = (uae_u8 *)linebuf;
		uae_u8 *oldp = (uae_u8 *)((uae_u8 *)ami_dinfo.image_mem + y*ami_dinfo.ximg->bytes_per_line);
		while (newp[xs] == oldp[xs]) {
		    if (xs == xe)
			return;
		    xs++;
		}
		while (newp[xe] == oldp[xe]) xe--;

		dst = (char *)(oldp + xs); src = (char *)(newp + xs);
	    }
	    break;

	 default:
	    abort ();
	    break;
	}

	len = xe - xs + 1;
	memcpy (dst, src, len * gfxvidinfo.pixbytes);
    } else if (need_dither) {
	uae_u8 *target = (uae_u8 *)ami_dinfo.image_mem + ami_dinfo.ximg->bytes_per_line * y;
	len = gfxvidinfo.width;
	DitherLine (target, (uae_u16 *)linebuf, 0, y, gfxvidinfo.width, bit_unit);
    } else {
	write_log ("Bug!\n");
	abort();
    }

    DO_PUTIMAGE (ami_dinfo.ximg, xs, y, xs, y, len, 1);
}

void flush_block (int ystart, int ystop)
{
    if (dgamode)
	return;

    DO_PUTIMAGE (ami_dinfo.ximg, 0, ystart, 0, ystart, gfxvidinfo.width, ystop - ystart + 1);
}

void flush_screen (int ystart, int ystop)
{
    if (dgamode)
	return;

#if SHM_SUPPORT_LINKS == 1
    if (currprefs.x11_use_mitshm && shmavail)
	XSync (display, 0);
#endif
}

void flush_clear_screen (void)
{
    flush_screen(0,0);
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

static unsigned long pixel_return[256];
static XColor parsed_xcolors[256];
static int ncolors = 0;

static int blackval = 32767;
static int whiteval = 0;

static int get_color (int r, int g, int b, xcolnr *cnp)
{
    XColor *col = parsed_xcolors + ncolors;
    char str[10];

    sprintf (str, "rgb:%x/%x/%x", r, g, b);
    XParseColor (display, cmap, str, col);
    *cnp = col->pixel = pixel_return[ncolors];
    XStoreColor (display, cmap, col);
    XStoreColor (display, cmap2, col);

    if (r + g + b < blackval)
	blackval = r + g + b, black = *col;
    if (r + g + b > whiteval)
	whiteval = r + g + b, white = *col;

    ncolors++;
    return 1;
}

static int init_colors (void)
{
    int i;

    if (visualInfo.VI_CLASS == TrueColor) {
	red_bits = bitsInMask (visualInfo.red_mask);
	green_bits = bitsInMask (visualInfo.green_mask);
	blue_bits = bitsInMask (visualInfo.blue_mask);
	red_shift = maskShift (visualInfo.red_mask);
	green_shift = maskShift (visualInfo.green_mask);
	blue_shift = maskShift (visualInfo.blue_mask);
    }

    if (need_dither) {
	if (bitdepth == 1)
	    setup_greydither (1, get_color);
	else
	    setup_dither (bitdepth, get_color);
    } else {
	if (bitdepth != 8 && bitdepth != 12 && bitdepth != 15
	    && bitdepth != 16 && bitdepth != 24) {
	    write_log ("Unsupported bit depth (%d)\n", bitdepth);
	    return 0;
	}

	switch (visualInfo.VI_CLASS) {
	 case TrueColor:
	    alloc_colors64k (red_bits, green_bits, blue_bits, red_shift,
			     green_shift, blue_shift);

	    XParseColor (display, cmap, "#000000", &black);
	    if (! XAllocColor (display, cmap, &black))
		write_log ("Whoops??\n");
	    XParseColor (display, cmap, "#ffffff", &white);
	    if (! XAllocColor (display, cmap, &white))
		write_log ("Whoops??\n");
	    break;

	 case GrayScale:
	 case PseudoColor:
	    alloc_colors256 (get_color);
	    break;

	 default:
	    write_log ("Unsupported visual class (%d)\n", visualInfo.VI_CLASS);
	    return 0;
	}
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
    if (inverse_byte_order)
	switch (gfxvidinfo.pixbytes) {
	 case 4:
	    for(i = 0; i < 4096; i++)
		xcolors[i] = ((((xcolors[i]>>0)&255) << 24)
			      | (((xcolors[i]>>8)&255) << 16)
			      | (((xcolors[i]>>16)&255) << 8)
			      | (((xcolors[i]>>24)&255) << 0));
	    break;
	 case 2:
	    for (i = 0; i < 4096; i++)
		xcolors[i] = (xcolors[i]>>8) | ((xcolors[i]&255)<<8);
	    break;
	 default:
	    break;
	}
    return 1;
}

static int dga_available (void)
{
#ifdef USE_DGA_EXTENSION
    int MajorVersion, MinorVersion;
    int EventBase, ErrorBase;

    if (! XF86DGAQueryVersion (display, &MajorVersion, &MinorVersion)) {
	write_log ("Unable to query video extension version\n");
	return 0;
    }
    if (! XF86DGAQueryExtension (display, &EventBase, &ErrorBase)) {
	write_log ("Unable to query video extension information\n");
	return 0;
    }
    /* Fail if the extension version in the server is too old */
    if (MajorVersion < DGA_MINMAJOR
	|| (MajorVersion == DGA_MINMAJOR && MinorVersion < DGA_MINMINOR))
    {
	write_log (
		 "Xserver is running an old XFree86-DGA version"
		 " (%d.%d)\n", MajorVersion, MinorVersion);
	write_log ("Minimum required version is %d.%d\n",
		 DGA_MINMAJOR, DGA_MINMINOR);
	return 0;
    }
    if (geteuid () != 0) {
	write_log ("UAE is not running as root, DGA extension disabled.\n");
	return 0;
    }
    if (! XF86DGAGetVideo (display, screen, &fb_addr, &fb_width, &fb_bank, &fb_mem)
	|| fb_bank < fb_mem)
    {
	write_log ("Problems with DGA - disabling DGA extension.\n");
	return 0;
    }
    write_log ("DGA extension: addr:%X, width %d, bank size %d mem size %d\n",
	     fb_addr, fb_width, fb_bank, fb_mem);

    return 1;
#else
    return 0;
#endif
}

static int vid_mode_available (void)
{
#ifdef USE_VIDMODE_EXTENSION
    int MajorVersion, MinorVersion;
    int EventBase, ErrorBase;

    if (! dgaavail)
	return 0;
    if (! XF86VidModeQueryVersion (display, &MajorVersion, &MinorVersion)) {
	write_log ("Unable to query video extension version\n");
	return 0;
    }
    if (! XF86VidModeQueryExtension (display, &EventBase, &ErrorBase)) {
	write_log ("Unable to query video extension information\n");
	return 0;
    }
    if (MajorVersion < VidMode_MINMAJOR
	|| (MajorVersion == VidMode_MINMAJOR && MinorVersion < VidMode_MINMINOR)) {
	/* Fail if the extension version in the server is too old */
	write_log ("Xserver is running an old XFree86-VidMode version (%d.%d)\n",
		 MajorVersion, MinorVersion);
	write_log ("Minimum required version is %d.%d\n",
		 VidMode_MINMAJOR, VidMode_MINMINOR);
	return 0;
    }
    if (! get_vidmodes ()) {
	write_log ("Error getting video mode information\n");
	return 0;
    }
    return 1;
#else
    return 0;
#endif
}

static int shm_available (void)
{
#if SHM_SUPPORT_LINKS == 1
    if (XShmQueryExtension (display))
	return 1;
#endif
    return 0;
}

int graphics_setup (void)
{
    char *display_name = 0;

    display = XOpenDisplay (display_name);
    if (display == 0)  {
	write_log ("Can't connect to X server %s\n", XDisplayName (display_name));
	return 0;
    }

    shmavail = shm_available ();
    dgaavail = dga_available ();
    vidmodeavail = vid_mode_available ();

    {
	int local_byte_order;
	int x = 0x04030201;
	char *y=(char*)&x;

	local_byte_order = y[0] == 0x04 ? MSBFirst : LSBFirst;
	if (ImageByteOrder(display) != local_byte_order)
	    inverse_byte_order = 1;
    }

    return 1;
}

static void lock_window_size (void)
{
    XSizeHints hint;

    hint.flags  = PMinSize | PMaxSize;
    hint.min_width = current_width;
    hint.min_height = current_height;
    hint.max_width = current_width;
    hint.max_height = current_height;
    XSetWMNormalHints (display, mywin, &hint);
}

static void init_dispinfo (struct disp_info *disp)
{
#if SHM_SUPPORT_LINKS == 1
    disp->shminfo.shmid = -1;
#endif
    disp->ximg = 0;
}

static void reset_cursor (void)
{
    if (! dgamode) {
	if (! currprefs.x11_hide_cursor)
	    XDefineCursor (display, mywin, xhairCursor);
	else
	    XDefineCursor (display, mywin, blankCursor);
	cursorOn = 1;
    }
}

int graphics_subinit (void)
{
    int i, j;
    XSetWindowAttributes wattr;
    XClassHint classhint;
    XWMHints *hints;
    unsigned long valuemask;

    if (screen_is_picasso) {
	// Set height, width for Picasso gfx
	current_width  = picasso_vidinfo.width;
	current_height = picasso_vidinfo.height;
	dgamode = currprefs.gfx_pfullscreen && dgaavail;
	curr_gfx = 0;
    } else {
	// Set height, width for Amiga gfx
	dgamode = currprefs.gfx_afullscreen && dgaavail;
	if (dgamode)
	    curr_gfx = &currprefs.gfx_f;
	else
	    curr_gfx = &currprefs.gfx_w;
	    
	current_width  = curr_gfx->width;
	current_height = curr_gfx->height;
    }
    if (!screen_is_picasso) {
	gfxvidinfo.width  = current_width;
	gfxvidinfo.height = current_height;
    }

    wattr.background_pixel = /*black.pixel*/0;
    wattr.backing_store = Always;
    wattr.backing_planes = bitdepth;
    wattr.border_pixmap = None;
    wattr.border_pixel = /*black.pixel*/0;
    wattr.colormap = cmap;
    valuemask = (CWEventMask | CWBackPixel | CWBorderPixel
		 | CWBackingStore | CWBackingPlanes | CWColormap);

    if (dgamode) {
	wattr.event_mask = DGA_EVENTMASK;
	wattr.override_redirect = 1;
	valuemask |= CWOverrideRedirect;
    } else
	wattr.event_mask = EVENTMASK;

    XSync (display, 0);
    delete_win = XInternAtom(display, "WM_DELETE_WINDOW", False);
    mywin = XCreateWindow (display, rootwin, 0, 0, current_width, current_height,
			   0, bitdepth, InputOutput, vis, valuemask, &wattr);
    XSetWMProtocols (display, mywin, &delete_win, 1);
    XSync (display, 0);

    XStoreName (display, mywin, "UAE");
    XSetIconName (display, mywin, "UAE Screen");

    /* set class hint */
    classhint.res_name = "UAE";
    classhint.res_class = "UAEScreen";
    XSetClassHint (display, mywin, &classhint);

    hints = XAllocWMHints();
    /* Set window group leader to self to become an application
     * that can be hidden by e.g. WindowMaker.
     * Would be more useful if we could find out what the
     * (optional) GTK+ window ID is :-/ */
    hints->window_group = mywin;
    hints->flags = WindowGroupHint;
    XSetWMHints (display, mywin, hints);
    XFree (hints);

    XMapRaised (display, mywin);
    XSync (display, 0);
    mygc = XCreateGC (display, mywin, 0, 0);

    if (dgamode) {
#ifdef USE_DGA_EXTENSION
	enter_dga_mode ();
	/*setuid(getuid());*/
	picasso_vidinfo.rowbytes = fb_width * picasso_vidinfo.pixbytes;
#endif
    } else {
	get_image (current_width, current_height, &ami_dinfo);
	if (screen_is_picasso) {
	    get_image (current_width, current_height, &pic_dinfo);
	    picasso_vidinfo.rowbytes = pic_dinfo.ximg->bytes_per_line;
	}
    }

    picasso_vidinfo.extra_mem = 1;

    if (need_dither) {
	gfxvidinfo.maxblocklines = 0;
	gfxvidinfo.rowbytes = gfxvidinfo.pixbytes * current_width;
	gfxvidinfo.linemem = (char *)malloc (gfxvidinfo.rowbytes);
    } else if (! dgamode) {
	gfxvidinfo.emergmem = 0;
	gfxvidinfo.linemem = 0;
	gfxvidinfo.bufmem = ami_dinfo.image_mem;
	gfxvidinfo.rowbytes = ami_dinfo.ximg->bytes_per_line;
	if (currprefs.x11_use_low_bandwidth) {
	    gfxvidinfo.maxblocklines = 0;
	    gfxvidinfo.rowbytes = ami_dinfo.ximg->bytes_per_line;
	    gfxvidinfo.linemem = (char *)malloc (gfxvidinfo.rowbytes);
	} else {
	    gfxvidinfo.maxblocklines = 100; /* whatever... */
	}
    }

    if (visualInfo.VI_CLASS != TrueColor && ! screen_is_picasso) {
	int i;
	for (i = 0; i < 256; i++)
	    XStoreColor (display, cmap, parsed_xcolors + i);
    }

#ifdef USE_DGA_EXTENSION
    if (dgamode) {
	dga_colormap_installed = 0;
	XF86DGAInstallColormap (display, screen, cmap2);
	XF86DGAInstallColormap (display, screen, cmap);
    }
#endif

    reset_cursor ();

    if (screen_is_picasso) {
	picasso_has_invalid_lines = 0;
	picasso_invalid_start = picasso_vidinfo.height + 1;
	picasso_invalid_stop = -1;
	memset (picasso_invalid_lines, 0, sizeof picasso_invalid_lines);
    }

    inwindow = 0;
    inputdevice_release_all_keys ();
    reset_hotkeys ();

    XWarpPointer (display, None, mywin, 0, 0, 0, 0,
		  current_width / 2, current_height / 2);
    return 1;
}

static int get_best_visual (XVisualInfo *vi)
{
    screen = XDefaultScreen (display);
    rootwin = XRootWindow (display, screen);

    /* try for a 12 bit visual first, then a 16 bit, then a 24 bit, then 8 bit */
    if (XMatchVisualInfo (display, screen, 12, TrueColor, vi)) {
    } else if (XMatchVisualInfo (display, screen, 15, TrueColor, vi)) {
    } else if (XMatchVisualInfo (display, screen, 16, TrueColor, vi)) {
    } else if (XMatchVisualInfo (display, screen, 24, TrueColor, vi)) {
    } else if (XMatchVisualInfo (display, screen, 8, PseudoColor, vi)) {
	/* for our HP boxes */
    } else if (XMatchVisualInfo (display, screen, 8, GrayScale, vi)) {
    } else if (XMatchVisualInfo (display, screen, 4, PseudoColor, vi)) {
	/* VGA16 server. Argh. */
    } else if (XMatchVisualInfo (display, screen, 1, StaticGray, vi)) {
	/* Mono server. Yuk */
    } else {
	write_log ("Can't obtain appropriate X visual.\n");
	return 0;
    }
    return 1;
}

static int get_visual_bit_unit (XVisualInfo *vi, int bitdepth)
{
    int bit_unit = 0;
    XPixmapFormatValues *xpfvs;
    int i,j;

    /* We now have the bitdepth of the display, but that doesn't tell us yet
     * how many bits to use per pixel. The VGA16 server has a bitdepth of 4,
     * but uses 1 byte per pixel. */
    xpfvs = XListPixmapFormats (display, &i);
    for (j = 0; j < i && xpfvs[j].depth != bitdepth; j++)
	;
    if (j < i)
	bit_unit = xpfvs[j].bits_per_pixel;
    XFree (xpfvs);
    if (j == i) {
	write_log ("Your X server is feeling ill.\n");
    }

    return bit_unit;
}

int graphics_init (void)
{
    int i,j;
    XPixmapFormatValues *xpfvs;

    if (currprefs.x11_use_mitshm && ! shmavail) {
	write_log ("MIT-SHM extension not supported by X server.\n");
    }
    if (currprefs.color_mode > 5)
	write_log ("Bad color mode selected. Using default.\n"), currprefs.color_mode = 0;

    x11_init_ok = 0;
    need_dither = 0;
    screen_is_picasso = 0;
    dgamode = 0;

    init_dispinfo (&ami_dinfo);
    init_dispinfo (&pic_dinfo);

    screen = XDefaultScreen (display);
    rootwin = XRootWindow (display, screen);

    if (!get_best_visual (&visualInfo)) return 0;

    vis = visualInfo.visual;
    bitdepth = visualInfo.depth;

    if (!(bit_unit = get_visual_bit_unit (&visualInfo, bitdepth))) return 0;

    write_log ("Using %d bit visual, %d bits per pixel\n", bitdepth, bit_unit);

    fixup_prefs_dimensions (&currprefs.gfx_w, gfx_windowed_modes, n_windowed_modes);
    fixup_prefs_dimensions (&currprefs.gfx_f, gfx_fullscreen_modes, n_fullscreen_modes);

    cmap = XCreateColormap (display, rootwin, vis, AllocNone);
    cmap2 = XCreateColormap (display, rootwin, vis, AllocNone);
    if (visualInfo.VI_CLASS == GrayScale || visualInfo.VI_CLASS == PseudoColor) {
	XAllocColorCells (display, cmap, 0, 0, 0, pixel_return, 1 << bitdepth);
	XAllocColorCells (display, cmap2, 0, 0, 0, pixel_return, 1 << bitdepth);
    }

    if (bitdepth < 8 || (bitdepth == 8 && currprefs.color_mode == 3)) {
	gfxvidinfo.pixbytes = 2;
	currprefs.x11_use_low_bandwidth = 0;
	need_dither = 1;
	picasso_vidinfo.pixbytes = 1 /* ??? */;
    } else {
	gfxvidinfo.pixbytes = bit_unit >> 3;
	picasso_vidinfo.pixbytes = bit_unit >> 3;
    }

    if (! init_colors ())
	return 0;

    blankCursor = XCreatePixmapCursor (display,
				       XCreatePixmap (display, rootwin, 1, 1, 1),
				       XCreatePixmap (display, rootwin, 1, 1, 1),
				       &black, &white, 0, 0);
    xhairCursor = XCreateFontCursor (display, XC_crosshair);

    graphics_subinit ();

    grabbed = 0;

    return x11_init_ok = 1;
}

static void destroy_dinfo (struct disp_info *dinfo)
{
    if (dinfo->ximg == NULL)
	return;
#if SHM_SUPPORT_LINKS == 1
    if (dinfo->shminfo.shmid != -1)
	shmdt (dinfo->shminfo.shmaddr);
    dinfo->shminfo.shmid = -1;
#endif
    XDestroyImage (dinfo->ximg);
    dinfo->ximg = NULL;
}

void graphics_subshutdown (int final)
{
    XSync (display, 0);
#ifdef USE_DGA_EXTENSION
    if (dgamode)
	leave_dga_mode ();
#endif

    destroy_dinfo (&ami_dinfo);
    destroy_dinfo (&pic_dinfo);

    if (mywin) {
	XDestroyWindow (display, mywin);
	mywin = 0;
    }

    if (gfxvidinfo.linemem != NULL)
	free (gfxvidinfo.linemem);
    if (gfxvidinfo.emergmem != NULL)
	free (gfxvidinfo.emergmem);
}

void graphics_leave (void)
{
    if (! x11_init_ok)
	return;

    graphics_subshutdown (1);

    if (autorepeatoff)
	XAutoRepeatOn (display);

    XFlush (display);
    XSync (display, 0);

    XFreeColormap (display, cmap);
    XFreeColormap (display, cmap2);

#if 0
    XCloseDisplay (display);
#endif
    dumpcustom ();
}

static struct timeval lastMotionTime;

static int refresh_necessary = 0;

void handle_events (void)
{
    gui_handle_events ();

    for (;;) {
	XEvent event;
#if 0
	if (! XCheckMaskEvent (display, eventmask, &event))
	    break;
#endif
	if (! XPending (display))
	    break;

	XNextEvent (display, &event);

	switch (event.type) {
	 case KeyPress:
	 case KeyRelease: {
	    int state = (event.type == KeyPress);
	    KeySym keysym;
	    int index = 0;
	    int ievent, amiga_keycode;
	    do {
		keysym = XLookupKeysym ((XKeyEvent *)&event, index);
		if ((ievent = match_hotkey_sequence (keysym, state))) {
		    handle_hotkey_event (ievent, state);
		    break;
		} else
		    if ((amiga_keycode = xkeysym2amiga (keysym)) >= 0) {
			inputdevice_do_keyboard (amiga_keycode, state);
			break;
		    }
		index++;
	    } while (keysym != NoSymbol);
	    break;
	 }
	 case ButtonPress:
	 case ButtonRelease: {
	    int state = (event.type == ButtonPress);
	    int buttonno = -1;
	    switch ((int)((XButtonEvent *)&event)->button) {
		case 1:  buttonno = 0; break;
		case 2:  buttonno = 2; break;
		case 3:  buttonno = 1; break;
		/* buttons 4 and 5 report mousewheel events */
		case 4:  if (state) record_key (0x7a << 1); break;
		case 5:  if (state) record_key (0x7b << 1); break;
	    }
	    if (buttonno >=0)
		setmousebuttonstate(0, buttonno, state);
	    break;
	 }
	 case MotionNotify:
	    if (dgamode) {
		int tx = ((XMotionEvent *)&event)->x_root;
		int ty = ((XMotionEvent *)&event)->y_root;
		setmousestate (0, 0, tx, 0);
		setmousestate (0, 1, ty, 0);
	    } else if (grabbed) {
		int realmove = 0;
		int tx, ty,ttx,tty;

		tx = ((XMotionEvent *)&event)->x;
		ty = ((XMotionEvent *)&event)->y;
		if (! event.xmotion.send_event) {
		    setmousestate (0, 0, tx - oldx, 0);
		    setmousestate (0, 1, ty - oldy, 0);
		    realmove = 1;
#undef ABS
#define ABS(a) (((a)<0) ? -(a) : (a) )
		    if (ABS(current_width / 2 - tx) > 3 * current_width / 8
			|| ABS(current_height / 2 - ty) > 3 * current_height / 8)
		    {
#undef ABS
			XEvent event;
			ttx = current_width / 2;
			tty = current_height / 2;
			event.type = MotionNotify;
			event.xmotion.display = display;
			event.xmotion.window = mywin;
			event.xmotion.x = ttx;
			event.xmotion.y = tty;
			XSendEvent (display, mywin, False,
				    PointerMotionMask, &event);
			XWarpPointer (display, None, mywin, 0, 0, 0, 0, ttx, tty);
		    }
		} else {
		    tx = event.xmotion.x;
		    ty = event.xmotion.y;
		}
		oldx = tx;
		oldy = ty;
	    } else if (inwindow) {
		int tx = ((XMotionEvent *)&event)->x;
		int ty = ((XMotionEvent *)&event)->y;
		setmousestate(0, 0, tx, 1);
		setmousestate(0, 1, ty, 1);
		if (! cursorOn && !currprefs.x11_hide_cursor) {
		    XDefineCursor(display, mywin, xhairCursor);
		    cursorOn = 1;
		}
		gettimeofday(&lastMotionTime, NULL);
	    }
	    break;
	 case EnterNotify:
	    {
		int tx = ((XCrossingEvent *)&event)->x;
		int ty = ((XCrossingEvent *)&event)->y;
		setmousestate (0, 0, tx, 1);
		setmousestate (0, 1, ty, 1);
	    }
	    inwindow = 1;
	    break;
	 case LeaveNotify:
	    inwindow = 0;
	    break;
	 case FocusIn:
	    if (! autorepeatoff)
		XAutoRepeatOff (display);
	    autorepeatoff = 1;
	    break;
	 case FocusOut:
	    if (autorepeatoff)
		XAutoRepeatOn (display);
	    autorepeatoff = 0;
	    break;
	 case Expose:
	    refresh_necessary = 1;
	    break;
	 case ClientMessage:
	    if (((Atom)event.xclient.data.l[0]) == delete_win) {
		uae_quit ();
	    }
	    break;
	}
    }

#if defined PICASSO96
    if (! dgamode) {
	if (screen_is_picasso && refresh_necessary) {
	    DO_PUTIMAGE (pic_dinfo.ximg, 0, 0, 0, 0,
			 picasso_vidinfo.width, picasso_vidinfo.height);
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
		    DO_PUTIMAGE (pic_dinfo.ximg, 0, strt, 0, strt,
				 picasso_vidinfo.width, i - strt);
		    strt = -1;
		}
	    }
	    if (strt != -1)
		abort ();
	}
    }
    picasso_has_invalid_lines = 0;
    picasso_invalid_start = picasso_vidinfo.height + 1;
    picasso_invalid_stop = -1;
#endif

    if (! dgamode) {
	if (! screen_is_picasso && refresh_necessary) {
	    DO_PUTIMAGE (ami_dinfo.ximg, 0, 0, 0, 0, current_width, current_height);
	    refresh_necessary = 0;
	}
	if (cursorOn && !currprefs.x11_hide_cursor) {
	    struct timeval now;
	    int diff;
	    gettimeofday(&now, NULL);
	    diff = (now.tv_sec - lastMotionTime.tv_sec) * 1000000 +
		(now.tv_usec - lastMotionTime.tv_usec);
	    if (diff > 1000000) {
		XDefineCursor (display, mywin, blankCursor);
		cursorOn = 0;
	    }
	}
    }
}

int debuggable (void)
{
    return 1;
}

int needmousehack (void)
{
    if (dgamode || grabbed)
	return 0;
    else
	return 1;
}

int mousehack_allowed (void)
{
    return 1;
}

void LED (int on)
{
#if 0 /* Maybe that is responsible for the joystick emulation problems on SunOS? */
    static int last_on = -1;
    XKeyboardControl control;

    if (last_on == on)
	return;
    last_on = on;
    control.led = 1; /* implementation defined */
    control.led_mode = on ? LedModeOn : LedModeOff;
    XChangeKeyboardControl(display, KBLed | KBLedMode, &control);
#endif
}

#ifdef PICASSO96

void DX_Invalidate (int first, int last)
{
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

static int palette_update_start=256;
static int palette_update_end=0;

static void DX_SetPalette_real (int start, int count)
{
    if (! screen_is_picasso || picasso96_state.RGBFormat != RGBFB_CHUNKY)
	return;

    if (picasso_vidinfo.pixbytes != 1) {
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

    while (count-- > 0) {
	XColor col = parsed_xcolors[start];
	col.red = picasso96_state.CLUT[start].Red * 0x0101;
	col.green = picasso96_state.CLUT[start].Green * 0x0101;
	col.blue = picasso96_state.CLUT[start].Blue * 0x0101;
	XStoreColor (display, cmap, &col);
	XStoreColor (display, cmap2, &col);
	start++;
    }
#ifdef USE_DGA_EXTENSION
    if (dgamode) {
	dga_colormap_installed ^= 1;
	if (dga_colormap_installed == 1)
	    XF86DGAInstallColormap (display, screen, cmap2);
	else
	    XF86DGAInstallColormap (display, screen, cmap);
    }
#endif
}
void DX_SetPalette (int start, int count)
{
   DX_SetPalette_real (start, count);
}

static void DX_SetPalette_delayed (int start, int count)
{
    if (bit_unit!=8) {
	DX_SetPalette_real(start,count);
	return;
    }
    if (start<palette_update_start)
	palette_update_start=start;
    if (start+count>palette_update_end)
	palette_update_end=start+count;
}

void DX_SetPalette_vsync(void)
{
  if (palette_update_end>palette_update_start) {
    DX_SetPalette_real(palette_update_start,
		       palette_update_end-palette_update_start);
    palette_update_end=0;
    palette_update_start=256;
  }
}

int DX_Fill (int dstx, int dsty, int width, int height, uae_u32 color, RGBFTYPE rgbtype)
{
    /* not implemented yet */
    return 0;
}

int DX_Blit (int srcx, int srcy, int dstx, int dsty, int width, int height, BLIT_OPCODE opcode)
{
    /* not implemented yet */
    return 0;
}

#define MAX_SCREEN_MODES 12

static int x_size_table[MAX_SCREEN_MODES] = { 320, 320, 320, 320, 640, 640, 640, 800, 1024, 1152, 1280, 1280 };
static int y_size_table[MAX_SCREEN_MODES] = { 200, 240, 256, 400, 350, 480, 512, 600, 768,  864,  960,  1024 };

int DX_FillResolutions (uae_u16 *ppixel_format)
{
    Screen *scr = ScreenOfDisplay (display, screen);
    int i, count = 0;
    int w = WidthOfScreen (scr);
    int h = HeightOfScreen (scr);
    int emulate_chunky = 0;

    /* we now need to find display depth first */
    XVisualInfo vi;
    if (!get_best_visual (&vi)) return 0;
    bitdepth = vi.depth;
    bit_unit = get_visual_bit_unit (&vi, bitdepth);

    if (ImageByteOrder (display) == LSBFirst) {
    picasso_vidinfo.rgbformat = (bit_unit == 8 ? RGBFB_CHUNKY
				 : bitdepth == 15 && bit_unit == 16 ? RGBFB_R5G5B5PC
				 : bitdepth == 16 && bit_unit == 16 ? RGBFB_R5G6B5PC
				 : bit_unit == 24 ? RGBFB_B8G8R8
				 : bit_unit == 32 ? RGBFB_B8G8R8A8
				 : RGBFB_NONE);
    } else {
    picasso_vidinfo.rgbformat = (bit_unit == 8 ? RGBFB_CHUNKY
				 : bitdepth == 15 && bit_unit == 16 ? RGBFB_R5G5B5
				 : bitdepth == 16 && bit_unit == 16 ? RGBFB_R5G6B5
				 : bit_unit == 24 ? RGBFB_R8G8B8
				 : bit_unit == 32 ? RGBFB_A8R8G8B8
				 : RGBFB_NONE);
    }

    *ppixel_format = 1 << picasso_vidinfo.rgbformat;
    if (vi.VI_CLASS == TrueColor && (bit_unit == 16 || bit_unit == 32))
	*ppixel_format |= RGBFF_CHUNKY, emulate_chunky = 1;

#if defined USE_DGA_EXTENSION && defined USE_VIDMODE_EXTENSION
    if (dgaavail && vidmodeavail) {
	for (i = 0; i < vidmodecount && count < MAX_PICASSO_MODES; i++) {
	    int j;
	    for (j = 0; j <= emulate_chunky && count < MAX_PICASSO_MODES; j++) {
		DisplayModes[count].res.width = allmodes[i]->hdisplay;
		DisplayModes[count].res.height = allmodes[i]->vdisplay;
		DisplayModes[count].depth = j == 1 ? 1 : bit_unit >> 3;
		DisplayModes[count].refresh = 75;
		count++;
	    }
	}
    } else
#endif
    {
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
		    count++;
		}
	    }
	}
    }

    return count;
}

uae_u8 *gfx_lock_picasso (void)
{
#ifdef USE_DGA_EXTENSION
    if (dgamode)
	return fb_addr;
    else
#endif
	return pic_dinfo.ximg->data;
}

void gfx_unlock_picasso (void)
{
}
#endif

int lockscr (void)
{
    return 1;
}

void unlockscr (void)
{
}

void toggle_mousegrab (void)
{
    if (grabbed) {
	XUngrabPointer (display, CurrentTime);
	XUndefineCursor (display, mywin);
	reset_cursor ();
	grabbed = 0;
    } else if (! dgamode) {
	XGrabPointer (display, mywin, 1, 0, GrabModeAsync, GrabModeAsync,
		      mywin, blankCursor, CurrentTime);
	oldx = current_width / 2;
	oldy = current_height / 2;
	XWarpPointer (display, None, mywin, 0, 0, 0, 0, oldx, oldy);
	grabbed = 1;
    }
}

void framerate_up (void)
{
    if (currprefs.gfx_framerate < 20)
	changed_prefs.gfx_framerate = currprefs.gfx_framerate + 1;
}

void framerate_down (void)
{
    if (currprefs.gfx_framerate > 1)
	changed_prefs.gfx_framerate = currprefs.gfx_framerate - 1;
}

int is_fullscreen (void)
{
#ifdef USE_DGA_EXTENSION
    return dgamode;
#else
    return 0;
#endif
}

void toggle_fullscreen (void)
{
#ifdef USE_DGA_EXTENSION
    changed_prefs.gfx_afullscreen = changed_prefs.gfx_pfullscreen = !dgamode;
#endif
}

void screenshot (int type)
{
    write_log ("Screenshot not implemented yet\n");
}

/*
 * Mouse inputdevice functions
 */

/* Hardwire for 3 axes and 3 buttons
 * There is no 3rd axis as such - mousewheel events are
 * supplied by X on buttons 4 and 5.
 */
#define MAX_BUTTONS     3
#define MAX_AXES        3
#define FIRST_AXIS      0
#define FIRST_BUTTON    MAX_AXES

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
    return 0;
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
    init_mouse, close_mouse, acquire_mouse, unacquire_mouse, read_mouse,
    get_mouse_num, get_mouse_name,
    get_mouse_widget_num, get_mouse_widget_type,
    get_mouse_widget_first
};

/*
 * Keyboard inputdevice functions
 */
static int get_kb_num (void)
{
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

static int keyhack (int scancode, int pressed, int num)
{
    return scancode;
}

static void read_kb (void)
{
}
static int init_kb (void)
{
    set_default_hotkeys (get_x11_default_hotkeys());
    return 1;
}

static void close_kb (void)
{
}

static int acquire_kb (unsigned int num, int flags)
{
    return 1;
}

static void unacquire_kb (unsigned int num)
{
}

/*
 * Default inputdevice config for X11 mouse
 */
void input_get_default_mouse (struct uae_input_device *uid)
{
    /* Supports only one mouse */
    uid[0].eventid[ID_AXIS_OFFSET + 0][0]   = INPUTEVENT_MOUSE1_HORIZ;
    uid[0].eventid[ID_AXIS_OFFSET + 1][0]   = INPUTEVENT_MOUSE1_VERT;
    uid[0].eventid[ID_AXIS_OFFSET + 2][0]   = INPUTEVENT_MOUSE1_WHEEL;
    uid[0].eventid[ID_BUTTON_OFFSET + 0][0] = INPUTEVENT_JOY1_FIRE_BUTTON;
    uid[0].eventid[ID_BUTTON_OFFSET + 1][0] = INPUTEVENT_JOY1_2ND_BUTTON;
    uid[0].eventid[ID_BUTTON_OFFSET + 2][0] = INPUTEVENT_JOY1_3RD_BUTTON;
    uid[0].enabled = 1;
}

struct inputdevice_functions inputdevicefunc_keyboard =
{
    init_kb, close_kb, acquire_kb, unacquire_kb,
    read_kb, get_kb_num, get_kb_name, get_kb_widget_num,
    get_kb_widget_type, get_kb_widget_first
};

int getcapslockstate (void)
{
    return 0;
}

void setcapslockstate (int state)
{
}

/*
 * Handle gfx cfgfile options
 */
void target_save_options (FILE *f, const struct uae_prefs *p)
{
    fprintf (f, "x11.low_bandwidth=%s\n", p->x11_use_low_bandwidth ? "true" : "false");
    fprintf (f, "x11.use_mitshm=%s\n", p->x11_use_mitshm ? "true" : "false");
    fprintf (f, "x11.hide_cursor=%s\n", p->x11_hide_cursor ? "true" : "false");
}

int target_parse_option (struct uae_prefs *p, const char *option, const char *value)
{
    return (cfgfile_yesno (option, value, "low_bandwidth", &p->x11_use_low_bandwidth)
	    || cfgfile_yesno (option, value, "use_mitshm", &p->x11_use_mitshm)
	    || cfgfile_yesno (option, value, "hide_cursor", &p->x11_hide_cursor));
}

void target_default_options (struct uae_prefs *p)
{
    p->x11_use_low_bandwidth = 0;
    p->x11_use_mitshm = 1;
    p->x11_hide_cursor = 1;
}

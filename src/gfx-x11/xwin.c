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
  * Copyright 2003 Richard Drummond
  */

#include "sysconfig.h"
#include "sysdeps.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <X11/cursorfont.h>

#include <ctype.h>
#include <signal.h>

#include "config.h"
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

static int screen_is_picasso;
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

/* Keyboard and mouse */

static int keystate[256];

static int oldx, oldy;
static int grabbed;

struct uae_hotkeys {
    KeySym syms[4];
    void (*handler)(void);
    int retval;
    int mask;
};

static void handle_modeswitch (void);
static void handle_mousegrab (void);
static void handle_inhibit (void);
static void framerate_up (void);
static void framerate_down (void);
static void handle_interpol (void);

static struct uae_hotkeys hotkeys[] = {
#ifdef USE_DGA_EXTENSION
    {{ XK_F12, XK_s, 0 }, handle_modeswitch, -1, 0 },
#endif
    {{ XK_F12, XK_q, 0 }, uae_quit, -1, 0 },
    {{ XK_F12, XK_m, 0 }, togglemouse, -1, 0 },
    {{ XK_F12, XK_g, 0 }, handle_mousegrab, -1, 0 },
    {{ XK_F12, XK_i, 0 }, handle_inhibit, -1, 0 },
    {{ XK_F12, XK_p, 0 }, handle_interpol, -1, 0 },
    {{ XK_F12, XK_KP_Add, 0 }, framerate_up, -1, 0 },
    {{ XK_F12, XK_KP_Subtract, 0 }, framerate_down, -1, 0 },
    {{ XK_Scroll_Lock, 0 }, handle_inhibit, -1, 0 },
    {{ 0 }, NULL, -1, 0 } /* List must be terminated */
};

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

static int get_vidmodes (void)
{
    return XF86VidModeGetAllModeLines (display, screen, &vidmodecount, &allmodes);
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
	len = currprefs.gfx_width_win;
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
			     green_shift, blue_shift, 0, 0, 0);

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

static void graphics_subinit (void)
{
    int i, j;
    XSetWindowAttributes wattr;
    XClassHint classhint;
    XWMHints *hints;
    unsigned long valuemask;

    dgamode = screen_is_picasso ? currprefs.gfx_pfullscreen : currprefs.gfx_afullscreen;
    dgamode = dgamode && dgaavail;

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
    XSetClassHint(display, mywin, &classhint);

    hints = XAllocWMHints();
    /* Set window group leader to self to become an application
     * that can be hidden by e.g. WindowMaker.
     * Would be more useful if we could find out what the
     * (optional) GTK+ window ID is :-/ */
    hints->window_group = mywin;
    hints->flags = WindowGroupHint;
    XSetWMHints(display, mywin, hints);

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
	gfxvidinfo.rowbytes = gfxvidinfo.pixbytes * currprefs.gfx_width_win;
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

    if (! dgamode) {
	if (! currprefs.x11_hide_cursor)
	    XDefineCursor (display, mywin, xhairCursor);
	else
	    XDefineCursor (display, mywin, blankCursor);
	cursorOn = 1;
    }

    if (screen_is_picasso) {
	picasso_has_invalid_lines = 0;
	picasso_invalid_start = picasso_vidinfo.height + 1;
	picasso_invalid_stop = -1;
	memset (picasso_invalid_lines, 0, sizeof picasso_invalid_lines);
    }

    inwindow = 0;
    for (i = 0; hotkeys[i].syms[0] != 0; i++) {
	hotkeys[i].mask = 0;
	for (j = 0; hotkeys[i].syms[j] != 0; j++)
	    hotkeys[i].mask |= (1 << j);
    }
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

    /* try for a 12 bit visual first, then a 16 bit, then a 24 bit, then 8 bit */
    if (XMatchVisualInfo (display, screen, 12, TrueColor, &visualInfo)) {
    } else if (XMatchVisualInfo (display, screen, 15, TrueColor, &visualInfo)) {
    } else if (XMatchVisualInfo (display, screen, 16, TrueColor, &visualInfo)) {
    } else if (XMatchVisualInfo (display, screen, 24, TrueColor, &visualInfo)) {
    } else if (XMatchVisualInfo (display, screen, 8, PseudoColor, &visualInfo)) {
	/* for our HP boxes */
    } else if (XMatchVisualInfo (display, screen, 8, GrayScale, &visualInfo)) {
    } else if (XMatchVisualInfo (display, screen, 4, PseudoColor, &visualInfo)) {
	/* VGA16 server. Argh. */
    } else if (XMatchVisualInfo (display, screen, 1, StaticGray, &visualInfo)) {
	/* Mono server. Yuk */
    } else {
	write_log ("Can't obtain appropriate X visual.\n");
	return 0;
    }
    vis = visualInfo.visual;
    bitdepth = visualInfo.depth;

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
	return 0;
    }

    write_log ("Using %d bit visual, %d bits per pixel\n", bitdepth, bit_unit);

    fixup_prefs_dimensions (&currprefs);

    gfxvidinfo.width = currprefs.gfx_width_win;
    gfxvidinfo.height = currprefs.gfx_height_win;
    current_width = currprefs.gfx_width_win;
    current_height = currprefs.gfx_height_win;

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
    } else {
	gfxvidinfo.pixbytes = bit_unit >> 3;
    }

    if (! init_colors ())
	return 0;

    blankCursor = XCreatePixmapCursor (display,
				       XCreatePixmap (display, rootwin, 1, 1, 1),
				       XCreatePixmap (display, rootwin, 1, 1, 1),
				       &black, &white, 0, 0);
    xhairCursor = XCreateFontCursor (display, XC_crosshair);

    graphics_subinit ();

    for (i = 0; i < 256; i++)
	keystate[i] = 0;
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

static void graphics_subshutdown (void)
{
    XSync (display, 0);
#ifdef USE_DGA_EXTENSION
    if (dgamode)
	leave_dga_mode ();
#endif

    destroy_dinfo (&ami_dinfo);
    destroy_dinfo (&pic_dinfo);

    XDestroyWindow (display, mywin);

    if (gfxvidinfo.linemem != NULL)
	free (gfxvidinfo.linemem);
    if (gfxvidinfo.emergmem != NULL)
	free (gfxvidinfo.emergmem);
}

void graphics_leave (void)
{
    if (! x11_init_ok)
	return;

    graphics_subshutdown ();

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

/* Decode KeySyms. This function knows about all keys that are common
 * between different keyboard languages. */
static int kc_decode (KeySym ks)
{
    switch (ks) {
     case XK_B: case XK_b: return AK_B;
     case XK_C: case XK_c: return AK_C;
     case XK_D: case XK_d: return AK_D;
     case XK_E: case XK_e: return AK_E;
     case XK_F: case XK_f: return AK_F;
     case XK_G: case XK_g: return AK_G;
     case XK_H: case XK_h: return AK_H;
     case XK_I: case XK_i: return AK_I;
     case XK_J: case XK_j: return AK_J;
     case XK_K: case XK_k: return AK_K;
     case XK_L: case XK_l: return AK_L;
     case XK_N: case XK_n: return AK_N;
     case XK_O: case XK_o: return AK_O;
     case XK_P: case XK_p: return AK_P;
     case XK_R: case XK_r: return AK_R;
     case XK_S: case XK_s: return AK_S;
     case XK_T: case XK_t: return AK_T;
     case XK_U: case XK_u: return AK_U;
     case XK_V: case XK_v: return AK_V;
     case XK_X: case XK_x: return AK_X;

     case XK_0: return AK_0;
     case XK_1: return AK_1;
     case XK_2: return AK_2;
     case XK_3: return AK_3;
     case XK_4: return AK_4;
     case XK_5: return AK_5;
     case XK_6: return AK_6;
     case XK_7: return AK_7;
     case XK_8: return AK_8;
     case XK_9: return AK_9;

	/* You never know which Keysyms might be missing on some workstation
	 * This #ifdef should be enough. */
#if defined(XK_KP_Prior) && defined(XK_KP_Left) && defined(XK_KP_Insert) && defined (XK_KP_End)
     case XK_KP_0: case XK_KP_Insert: return AK_NP0;
     case XK_KP_1: case XK_KP_End: return AK_NP1;
     case XK_KP_2: case XK_KP_Down: return AK_NP2;
     case XK_KP_3: case XK_KP_Next: return AK_NP3;
     case XK_KP_4: case XK_KP_Left: return AK_NP4;
     case XK_KP_5: case XK_KP_Begin: return AK_NP5;
     case XK_KP_6: case XK_KP_Right: return AK_NP6;
     case XK_KP_7: case XK_KP_Home: return AK_NP7;
     case XK_KP_8: case XK_KP_Up: return AK_NP8;
     case XK_KP_9: case XK_KP_Prior: return AK_NP9;
#else
     case XK_KP_0: return AK_NP0;
     case XK_KP_1: return AK_NP1;
     case XK_KP_2: return AK_NP2;
     case XK_KP_3: return AK_NP3;
     case XK_KP_4: return AK_NP4;
     case XK_KP_5: return AK_NP5;
     case XK_KP_6: return AK_NP6;
     case XK_KP_7: return AK_NP7;
     case XK_KP_8: return AK_NP8;
     case XK_KP_9: return AK_NP9;
#endif
     case XK_KP_Divide: return AK_NPDIV;
     case XK_KP_Multiply: return AK_NPMUL;
     case XK_KP_Subtract: return AK_NPSUB;
     case XK_KP_Add: return AK_NPADD;
     case XK_KP_Decimal: return AK_NPDEL;
     case XK_KP_Enter: return AK_ENT;

     case XK_F1: return AK_F1;
     case XK_F2: return AK_F2;
     case XK_F3: return AK_F3;
     case XK_F4: return AK_F4;
     case XK_F5: return AK_F5;
     case XK_F6: return AK_F6;
     case XK_F7: return AK_F7;
     case XK_F8: return AK_F8;
     case XK_F9: return AK_F9;
     case XK_F10: return AK_F10;

     case XK_BackSpace: return AK_BS;
     case XK_Delete: return AK_DEL;
     case XK_Control_L: return AK_CTRL;
     case XK_Control_R: return AK_RCTRL;
     case XK_Tab: return AK_TAB;
     case XK_Alt_L: return AK_LALT;
     case XK_Alt_R: return AK_RALT;
     case XK_Meta_R: case XK_Hyper_R: return AK_RAMI;
     case XK_Meta_L: case XK_Hyper_L: return AK_LAMI;
     case XK_Return: return AK_RET;
     case XK_space: return AK_SPC;
     case XK_Shift_L: return AK_LSH;
     case XK_Shift_R: return AK_RSH;
     case XK_Escape: return AK_ESC;

     case XK_Insert: return AK_HELP;
     case XK_Home: return AK_NPLPAREN;
     case XK_End: return AK_NPRPAREN;
     case XK_Caps_Lock: return AK_CAPSLOCK;

     case XK_Up: return AK_UP;
     case XK_Down: return AK_DN;
     case XK_Left: return AK_LF;
     case XK_Right: return AK_RT;

#if 0
     case XK_F11: return AK_BACKSLASH;
#else
     case XK_F11: frametime = 0; timeframes = 0; bogusframe = 1; break;
#endif
#ifdef XK_Page_Up /* These are missing occasionally */
     case XK_Page_Up: return AK_RAMI;          /* PgUp mapped to right amiga */
     case XK_Page_Down: return AK_LAMI;        /* PgDn mapped to left amiga */
#endif
       
#ifdef XK_Super_L
     case XK_Super_L: return AK_LAMI;
     case XK_Super_R: return AK_RAMI;
#endif
    }
    return -1;
}

static int decode_fr (KeySym ks)
{
    switch(ks) {        /* FR specific */
     case XK_A: case XK_a: return AK_Q;
     case XK_M: case XK_m: return AK_SEMICOLON;
     case XK_Q: case XK_q: return AK_A;
     case XK_Y: case XK_y: return AK_Y;
     case XK_W: case XK_w: return AK_Z;
     case XK_Z: case XK_z: return AK_W;
#if 0
     case XK_bracketleft: return AK_LBRACKET;
     case XK_bracketright: return AK_RBRACKET;
     case XK_comma: return AK_M;
     case XK_less: case XK_greater: return AK_LTGT;
     case XK_period: return AK_COMMA;
     case XK_parenright: return AK_MINUS;
     case XK_equal: return AK_SLASH;
     case XK_numbersign: return AK_NUMBERSIGN;
     case XK_slash: return AK_PERIOD;
     case XK_minus: return AK_EQUAL;
     case XK_backslash: return AK_BACKSLASH;
#else
     /* not sure for this one: my X 3.3 server doesn't handle this key always
      * correctly... But anyway, on new french keyboards, no more bracket key
      * at this place. 
      */
     case XK_dead_circumflex:
     case XK_dead_diaeresis: return AK_LBRACKET;
     case XK_dollar:
     case XK_sterling: return AK_RBRACKET;
     case XK_comma: case XK_question: return AK_M;
     case XK_less: case XK_greater: return AK_LTGT;
     case XK_semicolon: case XK_period: return AK_COMMA;
     case XK_parenright: case XK_degree: return AK_MINUS;
     case XK_equal: case XK_plus: return AK_SLASH;
     case XK_numbersign: return AK_NUMBERSIGN;
     case XK_colon: case XK_slash: return AK_PERIOD;
     case XK_minus: case XK_6: return AK_6;
     case XK_ugrave: case XK_percent: return AK_QUOTE;
     /* found a spare key - I hope it deserves this place. */
     case XK_asterisk: case XK_mu: return AK_BACKSLASH;
     case XK_exclam: case XK_section: return AK_EQUAL;
     case XK_twosuperior: case XK_asciitilde: return AK_BACKQUOTE;
     case XK_Multi_key: return AK_RAMI;
     case XK_Mode_switch: return AK_RALT;
#endif
    }

    return -1;
}

static int decode_us (KeySym ks)
{
    switch(ks) {	/* US specific */
     case XK_A: case XK_a: return AK_A;
     case XK_M: case XK_m: return AK_M;
     case XK_Q: case XK_q: return AK_Q;
     case XK_Y: case XK_y: return AK_Y;
     case XK_W: case XK_w: return AK_W;
     case XK_Z: case XK_z: return AK_Z;
     case XK_bracketleft: return AK_LBRACKET;
     case XK_bracketright: return AK_RBRACKET;
     case XK_comma: return AK_COMMA;
     case XK_period: return AK_PERIOD;
     case XK_slash: return AK_SLASH;
     case XK_semicolon: return AK_SEMICOLON;
     case XK_minus: return AK_MINUS;
     case XK_equal: return AK_EQUAL;
	/* this doesn't work: */
     case XK_quoteright: return AK_QUOTE;
     case XK_quoteleft: return AK_BACKQUOTE;
     case XK_backslash: return AK_BACKSLASH;
    }

    return -1;
}

static int decode_de (KeySym ks)
{
    switch(ks) {
	/* DE specific */
     case XK_A: case XK_a: return AK_A;
     case XK_M: case XK_m: return AK_M;
     case XK_Q: case XK_q: return AK_Q;
     case XK_W: case XK_w: return AK_W;
     case XK_Y: case XK_y: return AK_Z;
     case XK_Z: case XK_z: return AK_Y;
     case XK_Odiaeresis: case XK_odiaeresis: return AK_SEMICOLON;
     case XK_Adiaeresis: case XK_adiaeresis: return AK_QUOTE;
     case XK_Udiaeresis: case XK_udiaeresis: return AK_LBRACKET;
     case XK_plus: case XK_asterisk: return AK_RBRACKET;
     case XK_comma: return AK_COMMA;
     case XK_period: return AK_PERIOD;
     case XK_less: case XK_greater: return AK_LTGT;
     case XK_numbersign: return AK_NUMBERSIGN;
     case XK_ssharp: return AK_MINUS;
     case XK_apostrophe: return AK_EQUAL;
     case XK_asciicircum: return AK_BACKQUOTE;
     case XK_minus: return AK_SLASH;
    }

    return -1;
}

static int decode_dk (KeySym ks)
{
    switch(ks) {
	/* DK specific */
    case XK_A: case XK_a: return AK_A;
    case XK_M: case XK_m: return AK_M;
    case XK_Q: case XK_q: return AK_Q;
    case XK_W: case XK_w: return AK_W;
    case XK_Y: case XK_y: return AK_Y;
    case XK_Z: case XK_z: return AK_Z;
    case XK_AE: case XK_ae: return AK_SEMICOLON;
    case XK_Ooblique: case XK_oslash: return AK_QUOTE;
    case XK_Aring: case XK_aring: return AK_LBRACKET;
    case XK_apostrophe: case XK_asterisk: return AK_NUMBERSIGN;
    case XK_dead_diaeresis: case XK_dead_circumflex: return AK_RBRACKET;
    case XK_dead_acute: case XK_dead_grave: return AK_BACKSLASH;
    case XK_onehalf: case XK_section: return AK_BACKQUOTE;
    case XK_comma: return AK_COMMA;
    case XK_period: return AK_PERIOD;
    case XK_less: case XK_greater: return AK_LTGT;
    case XK_numbersign: return AK_NUMBERSIGN;
    case XK_plus: return AK_MINUS;
    case XK_asciicircum: return AK_BACKQUOTE;
    case XK_minus: return AK_SLASH;
    }

    return -1;
}

static int decode_se (KeySym ks)
{
    switch(ks) {
	/* SE specific */
     case XK_A: case XK_a: return AK_A;
     case XK_M: case XK_m: return AK_M;
     case XK_Q: case XK_q: return AK_Q;
     case XK_W: case XK_w: return AK_W;
     case XK_Y: case XK_y: return AK_Y;
     case XK_Z: case XK_z: return AK_Z;
     case XK_Odiaeresis: case XK_odiaeresis: return AK_SEMICOLON;
     case XK_Adiaeresis: case XK_adiaeresis: return AK_QUOTE;
     case XK_Aring: case XK_aring: return AK_LBRACKET;
     case XK_comma: return AK_COMMA;
     case XK_period: return AK_PERIOD;
     case XK_minus: return AK_SLASH;
     case XK_less: case XK_greater: return AK_LTGT;
     case XK_plus: case XK_question: return AK_EQUAL;
     case XK_at: case XK_onehalf: return AK_BACKQUOTE;
     case XK_asciitilde: case XK_asciicircum: return AK_RBRACKET;
     case XK_backslash: case XK_bar: return AK_MINUS;

     case XK_numbersign: return AK_NUMBERSIGN;
    }

    return -1;
 }

static int decode_it (KeySym ks)
{
    switch(ks) {
	/* IT specific */
     case XK_A: case XK_a: return AK_A;
     case XK_M: case XK_m: return AK_M;
     case XK_Q: case XK_q: return AK_Q;
     case XK_W: case XK_w: return AK_W;
     case XK_Y: case XK_y: return AK_Y;
     case XK_Z: case XK_z: return AK_Z;
     case XK_Ograve: case XK_ograve: return AK_SEMICOLON;
     case XK_Agrave: case XK_agrave: return AK_QUOTE;
     case XK_Egrave: case XK_egrave: return AK_LBRACKET;
     case XK_plus: case XK_asterisk: return AK_RBRACKET;
     case XK_comma: return AK_COMMA;
     case XK_period: return AK_PERIOD;
     case XK_less: case XK_greater: return AK_LTGT;
     case XK_backslash: case XK_bar: return AK_BACKQUOTE;
     case XK_apostrophe: return AK_MINUS;
     case XK_Igrave: case XK_igrave: return AK_EQUAL;
     case XK_minus: return AK_SLASH;
     case XK_numbersign: return AK_NUMBERSIGN;
    }

    return -1;
}

static int decode_es (KeySym ks)
{
    switch(ks) {
	/* ES specific */
     case XK_A: case XK_a: return AK_A;
     case XK_M: case XK_m: return AK_M;
     case XK_Q: case XK_q: return AK_Q;
     case XK_W: case XK_w: return AK_W;
     case XK_Y: case XK_y: return AK_Y;
     case XK_Z: case XK_z: return AK_Z;
     case XK_ntilde: case XK_Ntilde: return AK_SEMICOLON;
#ifdef XK_dead_acute
     case XK_dead_acute: case XK_dead_diaeresis: return AK_QUOTE;
     case XK_dead_grave: case XK_dead_circumflex: return AK_LBRACKET;
#endif
     case XK_plus: case XK_asterisk: return AK_RBRACKET;
     case XK_comma: return AK_COMMA;
     case XK_period: return AK_PERIOD;
     case XK_less: case XK_greater: return AK_LTGT;
     case XK_backslash: case XK_bar: return AK_BACKQUOTE;
     case XK_apostrophe: return AK_MINUS;
     case XK_Igrave: case XK_igrave: return AK_EQUAL;
     case XK_minus: return AK_SLASH;
     case XK_numbersign: return AK_NUMBERSIGN;
    }

    return -1;
}

static int keycode2amiga (XKeyEvent *event)
{
    KeySym ks;
    int as;
    int index = 0;

    do {
	int hkreturn = -1, returnnow = 0;
	ks = XLookupKeysym (event, index);
	if (event->type == KeyPress) {
	    int i, j;
	    for (i = 0; hotkeys[i].syms[0] != 0; i++) {
		for (j = 0; hotkeys[i].syms[j] != 0; j++) {
		    if (ks == hotkeys[i].syms[j]) {
			hotkeys[i].mask &= ~(1 << j);
			if (hotkeys[i].mask == 0) {
			    returnnow = 1;
			    hkreturn = hotkeys[i].retval;
			}
		    }
		}
	    }
	} else {
	    int i, j;
	    for (i = 0; hotkeys[i].syms[0] != 0; i++) {
		for (j = 0; hotkeys[i].syms[j] != 0; j++) {
		    if (ks == hotkeys[i].syms[j]) {
			hotkeys[i].mask |= (1 << j);
		    }
		}
	    }
	}
	if (returnnow)
	    return -2;
	as = kc_decode (ks);

	if (as == -1) {
	    switch (currprefs.keyboard_lang) {
	    case KBD_LANG_FR:
		as = decode_fr (ks);
		break;

	    case KBD_LANG_US:
		as = decode_us (ks);
		break;

	     case KBD_LANG_DE:
		as = decode_de (ks);
		break;

	     case KBD_LANG_DK:
		as = decode_dk (ks);
		break;

	     case KBD_LANG_SE:
		as = decode_se (ks);
		break;

	     case KBD_LANG_IT:
		as = decode_it (ks);
		break;

	     case KBD_LANG_ES:
		as = decode_es (ks);
		break;

	     default:
		as = -1;
		break;
	    }
	}
	if (-1 != as)
		return as;
	index++;
    } while (ks != NoSymbol);
    return -1;
}

static struct timeval lastMotionTime;

static int refresh_necessary = 0;

void handle_events (void)
{
    gui_handle_events ();

    for (;;) {
	XEvent event;
        int buttonno;
#if 0
	if (! XCheckMaskEvent (display, eventmask, &event))
	    break;
#endif
	if (! XPending (display))
	    break;

	XNextEvent (display, &event);

	switch (event.type) {
	 case KeyPress: {
	     int i;
	     int kc = keycode2amiga ((XKeyEvent *)&event);

	     if (kc == -2) {
		 for (i = 0; hotkeys[i].syms[0] != 0; i++) {
		     if (hotkeys[i].mask == 0) {
			 if (hotkeys[i].handler != NULL)
			     hotkeys[i].handler();
		     }
		 }
		 break;
	     }

	     if (kc == -1)
		 break;
	     if (! keystate[kc]) {
		 keystate[kc] = 1;
		 record_key (kc << 1);
	     }
	     break;
	 }
	 case KeyRelease: {
	     int kc = keycode2amiga ((XKeyEvent *)&event);
	     if (kc < 0)
		 break;
	     keystate[kc] = 0;
	     record_key ((kc << 1) | 1);
	     break;
	 }
	 case ButtonPress:
	 case ButtonRelease:
	    switch ((int)((XButtonEvent *)&event)->button) {
		case 1:  buttonno = 0; break;
		case 2:  buttonno = 2; break;
		case 3:  buttonno = 1; break;
		default: buttonno = -1;
	    }
            if (buttonno >=0)
		setmousebuttonstate(0, buttonno, event.type == ButtonPress ? 1:0);

	    break;
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
		    setmousestate( 0,0,tx-oldx,0);
		    setmousestate( 0,1,ty-oldy,0);
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
		    tx=event.xmotion.x;
		    ty=event.xmotion.y;
		}
		oldx = tx;
		oldy = ty;
	    } else if (inwindow) {
		int tx = ((XMotionEvent *)&event)->x;
		int ty = ((XMotionEvent *)&event)->y;
		setmousestate(0,0,tx,1);
		setmousestate(0,1,ty,1);
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
		setmousestate(0,0,tx,1);
		setmousestate(0,1,ty,1);
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
            if (event.xclient.data.l[0]==delete_win) {
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
	    DO_PUTIMAGE (ami_dinfo.ximg, 0, 0, 0, 0, currprefs.gfx_width_fs, currprefs.gfx_height_fs);
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
    /* "Affengriff" */
    if ((keystate[AK_CTRL] || keystate[AK_RCTRL]) && keystate[AK_LAMI] && keystate[AK_RAMI])
	uae_reset ( 0 );
}

int check_prefs_changed_gfx (void)
{
    if (changed_prefs.gfx_width_win != currprefs.gfx_width_win
	|| changed_prefs.gfx_height_win != currprefs.gfx_height_win)
	fixup_prefs_dimensions (&changed_prefs);

    if (changed_prefs.gfx_width_win == currprefs.gfx_width_win
	&& changed_prefs.gfx_height_win == currprefs.gfx_height_win
	&& changed_prefs.gfx_lores == currprefs.gfx_lores
	&& changed_prefs.gfx_linedbl == currprefs.gfx_linedbl
	&& changed_prefs.gfx_correct_aspect == currprefs.gfx_correct_aspect
	&& changed_prefs.gfx_xcenter == currprefs.gfx_xcenter
	&& changed_prefs.gfx_ycenter == currprefs.gfx_ycenter
	&& changed_prefs.gfx_afullscreen == currprefs.gfx_afullscreen
	&& changed_prefs.gfx_pfullscreen == currprefs.gfx_pfullscreen)
	return 0;

    graphics_subshutdown ();
    currprefs.gfx_width_win = changed_prefs.gfx_width_win;
    currprefs.gfx_height_win = changed_prefs.gfx_height_win;
    currprefs.gfx_lores = changed_prefs.gfx_lores;
    currprefs.gfx_linedbl = changed_prefs.gfx_linedbl;
    currprefs.gfx_correct_aspect = changed_prefs.gfx_correct_aspect;
    currprefs.gfx_xcenter = changed_prefs.gfx_xcenter;
    currprefs.gfx_ycenter = changed_prefs.gfx_ycenter;
    currprefs.gfx_afullscreen = changed_prefs.gfx_afullscreen;
    currprefs.gfx_pfullscreen = changed_prefs.gfx_pfullscreen;

    gui_update_gfx ();

    graphics_subinit ();

    if (! inwindow)
	XWarpPointer (display, None, mywin, 0, 0, 0, 0,
		      current_width / 2, current_height / 2);

    notice_screen_contents_lost ();
    init_row_map ();
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
    if (dgamode || grabbed)
	return 0;
    else
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

void DX_SetPalette_real (int start, int count)
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


void DX_SetPalette_delayed (int start, int count)
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

    picasso_vidinfo.rgbformat = (bit_unit == 8 ? RGBFB_CHUNKY
				 : bitdepth == 15 && bit_unit == 16 ? RGBFB_R5G5B5PC
				 : bitdepth == 16 && bit_unit == 16 ? RGBFB_R5G6B5PC
				 : bit_unit == 24 ? RGBFB_B8G8R8
				 : bit_unit == 32 ? RGBFB_B8G8R8A8
				 : RGBFB_NONE);

    *ppixel_format = 1 << picasso_vidinfo.rgbformat;
    if (visualInfo.VI_CLASS == TrueColor && (bit_unit == 16 || bit_unit == 32))
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

static void set_window_for_picasso (void)
{
    if (current_width == picasso_vidinfo.width && current_height == picasso_vidinfo.height)
	return;

    current_width = picasso_vidinfo.width;
    current_height = picasso_vidinfo.height;
    XResizeWindow (display, mywin, current_width, current_height);
#if defined USE_DGA_EXTENSION && defined USE_VIDMODE_EXTENSION
    if (dgamode && vidmodeavail)
	switch_to_best_mode ();
#endif
}

void gfx_set_picasso_modeinfo (int w, int h, int depth, int rgbfmt)
{
    picasso_vidinfo.width = w;
    picasso_vidinfo.height = h;
    picasso_vidinfo.depth = depth;
    picasso_vidinfo.pixbytes = bit_unit >> 3;

    if (screen_is_picasso)
	set_window_for_picasso ();
}

void gfx_set_picasso_baseaddr (uaecptr a)
{
}

void gfx_set_picasso_state (int on)
{
    if (on == screen_is_picasso)
	return;
    graphics_subshutdown ();
    screen_is_picasso = on;
    if (on) {
	current_width = picasso_vidinfo.width;
	current_height = picasso_vidinfo.height;
    } else {
	current_width = gfxvidinfo.width;
	current_height = gfxvidinfo.height;
    }
    graphics_subinit ();
    if (on)
	DX_SetPalette_real (0, 256);
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

static void handle_mousegrab (void)
{
    if (grabbed) {
	XUngrabPointer (display, CurrentTime);
//	XUndefineCursor (display, mywin);
	grabbed = 0;
        printf("Ungrabbed mouse\n");
    } else if (! dgamode) {
	XGrabPointer (display, mywin, 1, 0, GrabModeAsync, GrabModeAsync,
		      mywin, blankCursor, CurrentTime);
	oldx = current_width / 2;
	oldy = current_height / 2;
	XWarpPointer (display, None, mywin, 0, 0, 0, 0, oldx, oldy);
	printf("Grabbed mouse\n");
        grabbed = 1;       
    }
}

static void handle_inhibit (void)
{
    toggle_inhibit_frame (IHF_SCROLLLOCK);
}

#include "gensound.h"
#include "sounddep/sound.h"
#include "audio.h"

static void handle_interpol (void)
{
    if (currprefs.sound_interpol == 0) {
	currprefs.sound_interpol = 1;
	printf ("Interpol on: rh\n");
    }
    else if (currprefs.sound_interpol == 1) {
	currprefs.sound_interpol = 2;
	printf ("Interpol on: crux\n");
    }
    else {
	currprefs.sound_interpol = 0;
	printf ("Interpol off\n");
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

static void handle_modeswitch (void)
{
    changed_prefs.gfx_afullscreen = changed_prefs.gfx_pfullscreen = !dgamode;
}

void target_save_options (FILE *f, struct uae_prefs *p)
{
    fprintf (f, "x11.low_bandwidth=%s\n", p->x11_use_low_bandwidth ? "true" : "false");
    fprintf (f, "x11.use_mitshm=%s\n", p->x11_use_mitshm ? "true" : "false");
    fprintf (f, "x11.hide_cursor=%s\n", p->x11_hide_cursor ? "true" : "false");
}

int target_parse_option (struct uae_prefs *p, char *option, char *value)
{
    return (cfgfile_yesno (option, value, "low_bandwidth", &p->x11_use_low_bandwidth)
	    || cfgfile_yesno (option, value, "use_mitshm", &p->x11_use_mitshm)
	    || cfgfile_yesno (option, value, "hide_cursor", &p->x11_hide_cursor));
}

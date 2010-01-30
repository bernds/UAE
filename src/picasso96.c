/*
 * UAE - The U*nix Amiga Emulator
 *
 * Picasso96 Support Module
 *
 * Copyright 1997 Brian King <Brian_King@Mitel.com, Brian_King@Cloanto.com>
 *
 * Theory of operation:
 * On the Amiga side, a Picasso card consists mainly of a memory area that
 * contains the frame buffer.  On the UAE side, we allocate a block of memory
 * that will hold the frame buffer.  This block is in normal memory, it is
 * never directly on the graphics card.  All graphics operations, which are
 * mainly reads and writes into this block and a few basic operations like
 * filling a rectangle, operate on this block of memory.
 * Since the memory is not on the graphics card, some work must be done to
 * synchronize the display with the data in the Picasso frame buffer.  There
 * are various ways to do this.  One possibility is to allocate a second
 * buffer of the same size, and perform all write operations twice.  Since
 * we never read from the second buffer, it can actually be placed in video
 * memory.  The X11 driver could be made to use the Picasso frame buffer as
 * the data buffer of an XImage, which could then be XPutImage()d from time
 * to time.  Another possibility is to translate all Picasso accesses into
 * Xlib (or GDI, or whatever your graphics system is) calls.  This possibility
 * is a bit tricky, since there is a risk of generating very many single pixel
 * accesses which may be rather slow.
 *
 * TODO:
 * - add panning capability
 * - we want to add a manual switch to override SetSwitch for hardware banging
 *   programs started from a Picasso workbench.
 */

#include "sysconfig.h"
#include "sysdeps.h"

#include "config.h"
#include "options.h"
#include "threaddep/thread.h"
#include "uae.h"
#include "memory.h"
#include "custom.h"
#include "newcpu.h"
#include "xwin.h"
#include "picasso96.h"

#ifdef PICASSO96

/*#define P96TRACING_ENABLED */
#ifdef P96TRACING_ENABLED
#define P96TRACE(x)	do { write_log x; } while(0)
#else
#define P96TRACE(x)
#endif

static uae_u32 gfxmem_lget (uaecptr) REGPARAM;
static uae_u32 gfxmem_wget (uaecptr) REGPARAM;
static uae_u32 gfxmem_bget (uaecptr) REGPARAM;
static void gfxmem_lput (uaecptr, uae_u32) REGPARAM;
static void gfxmem_wput (uaecptr, uae_u32) REGPARAM;
static void gfxmem_bput (uaecptr, uae_u32) REGPARAM;
static int gfxmem_check (uaecptr addr, uae_u32 size) REGPARAM;
static uae_u8 *gfxmem_xlate (uaecptr addr) REGPARAM;

static void write_gfx_long (uaecptr addr, uae_u32 value);
static void write_gfx_word (uaecptr addr, uae_u16 value);
static void write_gfx_byte (uaecptr addr, uae_u8 value);

static uae_u8 all_ones_bitmap, all_zeros_bitmap;

struct picasso96_state_struct picasso96_state;
struct picasso_vidbuf_description picasso_vidinfo;

/* These are the maximum resolutions... They are filled in by GetSupportedResolutions() */
/* have to fill this in, otherwise problems occur
 * @@@ ??? what problems?
 */
struct ScreenResolution planar = { 320, 240 };
struct ScreenResolution chunky = { 640, 480 };
struct ScreenResolution hicolour = { 640, 480 };
struct ScreenResolution truecolour = { 640, 480 };
struct ScreenResolution alphacolour = { 640, 480 };

uae_u16 picasso96_pixel_format = RGBFF_CHUNKY;

struct PicassoResolution DisplayModes[MAX_PICASSO_MODES];

static int mode_count = 0;

static uae_u32 p2ctab[256][2];

/*
 * Debugging dumps
 */

static void DumpModeInfoStructure (uaecptr amigamodeinfoptr)
{
    write_log ("ModeInfo Structure Dump:\n");
    write_log ("  Node.ln_Succ  = 0x%x\n", get_long (amigamodeinfoptr));
    write_log ("  Node.ln_Pred  = 0x%x\n", get_long (amigamodeinfoptr + 4));
    write_log ("  Node.ln_Type  = 0x%x\n", get_byte (amigamodeinfoptr + 8));
    write_log ("  Node.ln_Pri   = %d\n", get_byte (amigamodeinfoptr + 9));
    /*write_log ("  Node.ln_Name  = %s\n", uaememptr->Node.ln_Name); */
    write_log ("  OpenCount     = %d\n", get_word (amigamodeinfoptr + PSSO_ModeInfo_OpenCount));
    write_log ("  Active        = %d\n", get_byte (amigamodeinfoptr + PSSO_ModeInfo_Active));
    write_log ("  Width         = %d\n", get_word (amigamodeinfoptr + PSSO_ModeInfo_Width));
    write_log ("  Height        = %d\n", get_word (amigamodeinfoptr + PSSO_ModeInfo_Height));
    write_log ("  Depth         = %d\n", get_byte (amigamodeinfoptr + PSSO_ModeInfo_Depth));
    write_log ("  Flags         = %d\n", get_byte (amigamodeinfoptr + PSSO_ModeInfo_Flags));
    write_log ("  HorTotal      = %d\n", get_word (amigamodeinfoptr + PSSO_ModeInfo_HorTotal));
    write_log ("  HorBlankSize  = %d\n", get_word (amigamodeinfoptr + PSSO_ModeInfo_HorBlankSize));
    write_log ("  HorSyncStart  = %d\n", get_word (amigamodeinfoptr + PSSO_ModeInfo_HorSyncStart));
    write_log ("  HorSyncSize   = %d\n", get_word (amigamodeinfoptr + PSSO_ModeInfo_HorSyncSize));
    write_log ("  HorSyncSkew   = %d\n", get_byte (amigamodeinfoptr + PSSO_ModeInfo_HorSyncSkew));
    write_log ("  HorEnableSkew = %d\n", get_byte (amigamodeinfoptr + PSSO_ModeInfo_HorEnableSkew));
    write_log ("  VerTotal      = %d\n", get_word (amigamodeinfoptr + PSSO_ModeInfo_VerTotal));
    write_log ("  VerBlankSize  = %d\n", get_word (amigamodeinfoptr + PSSO_ModeInfo_VerBlankSize));
    write_log ("  VerSyncStart  = %d\n", get_word (amigamodeinfoptr + PSSO_ModeInfo_VerSyncStart));
    write_log ("  VerSyncSize   = %d\n", get_word (amigamodeinfoptr + PSSO_ModeInfo_VerSyncSize));
    write_log ("  Clock         = %d\n", get_byte (amigamodeinfoptr + PSSO_ModeInfo_first_union));
    write_log ("  ClockDivide   = %d\n", get_byte (amigamodeinfoptr + PSSO_ModeInfo_second_union));
    write_log ("  PixelClock    = %d\n", get_long (amigamodeinfoptr + PSSO_ModeInfo_PixelClock));
}

static void DumpLibResolutionStructure (uaecptr amigalibresptr)
{
    int i;
    uaecptr amigamodeinfoptr;
    struct LibResolution *uaememptr = (struct LibResolution *) get_mem_bank (amigalibresptr).xlateaddr (amigalibresptr);

    return;

    write_log ("LibResolution Structure Dump:\n");

    if (get_long (amigalibresptr + PSSO_LibResolution_DisplayID) == 0xFFFFFFFF) {
	write_log ("  Finished With LibResolutions...\n");
    } else {
	write_log ("  Name      = %s\n", uaememptr->P96ID);
	write_log ("  DisplayID = 0x%x\n", get_long (amigalibresptr + PSSO_LibResolution_DisplayID));
	write_log ("  Width     = %d\n", get_word (amigalibresptr + PSSO_LibResolution_Width));
	write_log ("  Height    = %d\n", get_word (amigalibresptr + PSSO_LibResolution_Height));
	write_log ("  Flags     = %d\n", get_word (amigalibresptr + PSSO_LibResolution_Flags));
	for (i = 0; i < MAXMODES; i++) {
	    amigamodeinfoptr = get_long (amigalibresptr + PSSO_LibResolution_Modes + i * 4);
	    write_log ("  ModeInfo[%d] = 0x%x\n", i, amigamodeinfoptr);
	    if (amigamodeinfoptr)
		DumpModeInfoStructure (amigamodeinfoptr);
	}
	write_log ("  BoardInfo = 0x%x\n", get_long (amigalibresptr + PSSO_LibResolution_BoardInfo));
    }
}

static char binary_byte[9];

static char *BuildBinaryString (uae_u8 value)
{
    int i;
    for (i = 0; i < 8; i++) {
	binary_byte[i] = (value & (1 << (7 - i))) ? '#' : '.';
    }
    binary_byte[8] = '\0';
    return binary_byte;
}

static void DumpPattern (struct Pattern *patt)
{
    uae_u8 *mem;
    int row, col;
    for (row = 0; row < (1 << patt->Size); row++) {
	mem = patt->Memory + row * 2;
	for (col = 0; col < 2; col++) {
	    write_log ("%s", BuildBinaryString (*mem++));
	}
	write_log ("\n");
    }
}

static void DumpTemplate (struct Template *tmp, uae_u16 w, uae_u16 h)
{
    uae_u8 *mem = tmp->Memory;
    int row, col, width;
    width = (w + 7) >> 3;
    write_log ("xoffset = %d, bpr = %d\n", tmp->XOffset, tmp->BytesPerRow);
    for (row = 0; row < h; row++) {
	mem = tmp->Memory + row * tmp->BytesPerRow;
	for (col = 0; col < width; col++) {
	    write_log ("%s", BuildBinaryString (*mem++));
	}
	write_log ("\n");
    }
}

int picasso_nr_resolutions (void)
{
    return mode_count;
}

static void ShowSupportedResolutions (void)
{
    int i;

    return;

    for (i = 0; i < mode_count; i++)
	write_log ("%s\n", DisplayModes[i].name);
}

static uae_u8 GetBytesPerPixel (uae_u32 RGBfmt)
{
    switch (RGBfmt) {
    case RGBFB_CLUT:
	return 1;

    case RGBFB_A8R8G8B8:
    case RGBFB_A8B8G8R8:
    case RGBFB_R8G8B8A8:
    case RGBFB_B8G8R8A8:
	return 4;

    case RGBFB_B8G8R8:
    case RGBFB_R8G8B8:
	return 3;

    case RGBFB_R5G5B5:
    case RGBFB_R5G6B5:
    case RGBFB_R5G6B5PC:
    case RGBFB_R5G5B5PC:
    case RGBFB_B5G6R5PC:
    case RGBFB_B5G5R5PC:
	return 2;
    default:
	write_log ("ERROR - GetBytesPerPixel() was unsuccessful with 0x%x?!\n", RGBfmt);
	return 0;
    }
}

/*
 * Amiga <-> native structure conversion functions
 */

static int CopyRenderInfoStructureA2U (uaecptr amigamemptr, struct RenderInfo *ri)
{
    uaecptr memp = get_long (amigamemptr + PSSO_RenderInfo_Memory);

    if (valid_address (memp, PSSO_RenderInfo_sizeof)) {
	ri->Memory = get_real_address (memp);
	ri->BytesPerRow = get_word (amigamemptr + PSSO_RenderInfo_BytesPerRow);
	ri->RGBFormat = get_long (amigamemptr + PSSO_RenderInfo_RGBFormat);
	return 1;
    }
    write_log ("ERROR - Invalid RenderInfo memory area...\n");
    return 0;
}

static int CopyPatternStructureA2U (uaecptr amigamemptr, struct Pattern *pattern)
{
    uaecptr memp = get_long (amigamemptr + PSSO_Pattern_Memory);
    if (valid_address (memp, PSSO_Pattern_sizeof)) {
	pattern->Memory = get_real_address (memp);
	pattern->XOffset = get_word (amigamemptr + PSSO_Pattern_XOffset);
	pattern->YOffset = get_word (amigamemptr + PSSO_Pattern_YOffset);
	pattern->FgPen = get_long (amigamemptr + PSSO_Pattern_FgPen);
	pattern->BgPen = get_long (amigamemptr + PSSO_Pattern_BgPen);
	pattern->Size = get_byte (amigamemptr + PSSO_Pattern_Size);
	pattern->DrawMode = get_byte (amigamemptr + PSSO_Pattern_DrawMode);
	return 1;
    }
    write_log ("ERROR - Invalid Pattern memory area...\n");
    return 0;
}

static void CopyColorIndexMappingA2U (uaecptr amigamemptr, struct ColorIndexMapping *cim)
{
    int i;
    cim->ColorMask = get_long (amigamemptr);
    for (i = 0; i < 256; i++, amigamemptr += 4)
	cim->Colors[i] = get_long (amigamemptr + 4);
}

static int CopyBitMapStructureA2U (uaecptr amigamemptr, struct BitMap *bm)
{
    int i;

    bm->BytesPerRow = get_word (amigamemptr + PSSO_BitMap_BytesPerRow);
    bm->Rows = get_word (amigamemptr + PSSO_BitMap_Rows);
    bm->Flags = get_byte (amigamemptr + PSSO_BitMap_Flags);
    bm->Depth = get_byte (amigamemptr + PSSO_BitMap_Depth);

    for (i = 0; i < bm->Depth; i++) {
	uaecptr plane = get_long (amigamemptr + PSSO_BitMap_Planes + i * 4);
	switch (plane) {
	case 0:
	    bm->Planes[i] = &all_zeros_bitmap;
	    break;
	case 0xFFFFFFFF:
	    bm->Planes[i] = &all_ones_bitmap;
	    break;
	default:
	    if (valid_address (plane, bm->BytesPerRow * bm->Rows))
		bm->Planes[i] = get_real_address (plane);
	    else
		return 0;
	    break;
	}
    }
    return 1;
}

static int CopyTemplateStructureA2U (uaecptr amigamemptr, struct Template *tmpl)
{
    uaecptr memp = get_long (amigamemptr + PSSO_Template_Memory);

    if (valid_address (memp, 1 /* FIXME */ )) {
	tmpl->Memory = get_real_address (memp);
	tmpl->BytesPerRow = get_word (amigamemptr + PSSO_Template_BytesPerRow);
	tmpl->XOffset = get_byte (amigamemptr + PSSO_Template_XOffset);
	tmpl->DrawMode = get_byte (amigamemptr + PSSO_Template_DrawMode);
	tmpl->FgPen = get_long (amigamemptr + PSSO_Template_FgPen);
	tmpl->BgPen = get_long (amigamemptr + PSSO_Template_BgPen);
	return 1;
    }
    write_log ("ERROR - Invalid Template memory area...\n");
    return 0;
}

static void CopyLibResolutionStructureU2A (struct LibResolution *libres, uaecptr amigamemptr)
{
    char *uaememptr = 0;
    int i;

    uaememptr = gfxmem_xlate (amigamemptr);	/* I know that amigamemptr is inside my gfxmem chunk, so I can just do the xlate() */
    memset (uaememptr, 0, PSSO_LibResolution_sizeof);	/* zero out our LibResolution structure */
    strcpy (uaememptr + PSSO_LibResolution_P96ID, libres->P96ID);
    put_long (amigamemptr + PSSO_LibResolution_DisplayID, libres->DisplayID);
    put_word (amigamemptr + PSSO_LibResolution_Width, libres->Width);
    put_word (amigamemptr + PSSO_LibResolution_Height, libres->Height);
    put_word (amigamemptr + PSSO_LibResolution_Flags, libres->Flags);
    for (i = 0; i < MAXMODES; i++)
	put_long (amigamemptr + PSSO_LibResolution_Modes + i * 4, libres->Modes[i]);
#if 0
    put_long (amigamemptr, libres->Node.ln_Succ);
    put_long (amigamemptr + 4, libres->Node.ln_Pred);
    put_byte (amigamemptr + 8, libres->Node.ln_Type);
    put_byte (amigamemptr + 9, libres->Node.ln_Pri);
#endif
    put_long (amigamemptr + 10, amigamemptr + PSSO_LibResolution_P96ID);
    put_long (amigamemptr + PSSO_LibResolution_BoardInfo, libres->BoardInfo);
}

/* list is Amiga address of list, in correct endian format for UAE
 * node is Amiga address of node, in correct endian format for UAE */
static void AmigaListAddTail (uaecptr list, uaecptr node)
{
    uaecptr amigamemptr = 0;

    if (get_long (list + 8) == list) {
	/* Empty list - set it up */
	put_long (list, node);	/* point the lh_Head to our new node */
	put_long (list + 4, 0);	/* set the lh_Tail to NULL */
	put_long (list + 8, node);	/* point the lh_TailPred to our new node */

	/* Adjust the new node - don't rely on it being zeroed out */
	put_long (node, 0);	/* ln_Succ */
	put_long (node + 4, 0);	/* ln_Pred */
    } else {
	amigamemptr = get_long (list + 8);	/* get the lh_TailPred contents */

	put_long (list + 8, node);	/* point the lh_TailPred to our new node */

	/* Adjust the previous lh_TailPred node */
	put_long (amigamemptr, node);	/* point the ln_Succ to our new node */

	/* Adjust the new node - don't rely on it being zeroed out */
	put_long (node, 0);	/* ln_Succ */
	put_long (node + 4, amigamemptr);	/* ln_Pred */
    }
}

/*
 * Functions to perform an action on the real screen
 */

/*
 * Fill a rectangle on the screen.  src points to the start of a line of the
 * filled rectangle in the frame buffer; it can be used as a memcpy source if
 * there is no OS specific function to fill the rectangle.
 */

static void do_fillrect (uae_u8 * src, int x, int y, int width, int height)
{
    uae_u8 *dst;
    /* Try OS specific fillrect function here; and return if successful.  */

    DX_Invalidate (y, y + height - 1);
    if (!picasso_vidinfo.extra_mem)
	return;

    width *= picasso96_state.BytesPerPixel;
    dst = gfx_lock_picasso ();
    if (!dst)
	goto out;

    dst += y * picasso_vidinfo.rowbytes + x * picasso_vidinfo.pixbytes;
    if (picasso_vidinfo.rgbformat == picasso96_state.RGBFormat) {
	while (height-- > 0) {
	    memcpy (dst, src, width);
	    dst += picasso_vidinfo.rowbytes;
	}
    } else {
	int psiz = GetBytesPerPixel (picasso_vidinfo.rgbformat);
	if (picasso96_state.RGBFormat != RGBFB_CHUNKY)
	    abort ();

	while (height-- > 0) {
	    int i;
	    switch (psiz) {
	    case 2:
		for (i = 0; i < width; i++)
		    *((uae_u16 *) dst + i) = picasso_vidinfo.clut[src[i]];
		break;
	    case 4:
		for (i = 0; i < width; i++)
		    *((uae_u32 *) dst + i) = picasso_vidinfo.clut[src[i]];
		break;
	    default:
		abort ();
	    }
	    dst += picasso_vidinfo.rowbytes;
	}
    }
  out:
    gfx_unlock_picasso ();
}

/*
 * This routine modifies the real screen buffer after a blit has been
 * performed in the save area. If can_do_blit is nonzero, the blit can
 * be performed within the real screen buffer; otherwise, this routine
 * must do it by hand using the data in the save area, pointed to by
 * srcp.
 */

static void do_blit (uae_u8 * srcp, unsigned long src_rowbytes,
		     int srcx, int srcy, int dstx, int dsty, int width, int height, int can_do_blit)
{
    uae_u8 *dstp;
    /* If this RenderInfo points at something else than the currently visible
     * screen, we must ignore the blit.  */
    if (can_do_blit) {
	/*
	 * Call OS blitting function that can do it in video memory.
	 * Should return if it was successful
	 */
    }

    DX_Invalidate (dsty, dsty + height - 1);
    if (!picasso_vidinfo.extra_mem)
	return;

    dstp = gfx_lock_picasso ();
    if (dstp == 0)
	goto out;

    /* The areas can't overlap: the source is always in the Picasso frame buffer,
     * and the destination is a different buffer owned by the graphics code.  */

    dstp += dsty * picasso_vidinfo.rowbytes + dstx * picasso_vidinfo.pixbytes;
    if (picasso_vidinfo.rgbformat == picasso96_state.RGBFormat) {
	width *= picasso96_state.BytesPerPixel;
	while (height-- > 0) {
	    memcpy (dstp, srcp, width);
	    srcp += src_rowbytes;
	    dstp += picasso_vidinfo.rowbytes;
	}
    } else {
	int psiz = GetBytesPerPixel (picasso_vidinfo.rgbformat);
	if (picasso96_state.RGBFormat != RGBFB_CHUNKY)
	    abort ();

	while (height-- > 0) {
	    int i;
	    switch (psiz) {
	    case 2:
		for (i = 0; i < width; i++)
		    *((uae_u16 *) dstp + i) = picasso_vidinfo.clut[srcp[i]];
		break;
	    case 4:
		for (i = 0; i < width; i++)
		    *((uae_u32 *) dstp + i) = picasso_vidinfo.clut[srcp[i]];
		break;
	    default:
		abort ();
	    }
	    srcp += src_rowbytes;
	    dstp += picasso_vidinfo.rowbytes;
	}
    }
  out:
    gfx_unlock_picasso ();
}

/*
 * Invert a rectangle on the screen.  src and src_rowbytes describe the
 * inverted rectangle in the frame buffer, so that do_blit can be used if
 * there is no OS specific function to fill the rectangle.
 */

static void do_invertrect (uae_u8 * src, unsigned long src_rowbytes, int x, int y, int width, int height)
{
    /* TODO: Try OS specific invertrect function here; and return if successful.  */

    do_blit (src, src_rowbytes, 0, 0, x, y, width, height, 0);
}

static uaecptr wgfx_linestart;
static uaecptr wgfx_lineend;
static uaecptr wgfx_min, wgfx_max;
static unsigned long wgfx_y;

static void wgfx_do_flushline (void)
{
    uae_u8 *src, *dstp;

    DX_Invalidate (wgfx_y, wgfx_y);
    if (!picasso_vidinfo.extra_mem)
	goto out;

    dstp = gfx_lock_picasso ();
    if (dstp == 0)
	goto out;
    /*printf("flushing %d (%x %x %x)\n", wgfx_y, wgfx_linestart, wgfx_min, wgfx_max); */
    src = gfxmemory + wgfx_min;

    if (picasso_vidinfo.rgbformat == picasso96_state.RGBFormat) {
	dstp += wgfx_y * picasso_vidinfo.rowbytes + wgfx_min - wgfx_linestart;
	memcpy (dstp, src, wgfx_max - wgfx_min);
    } else {
	int width = wgfx_max - wgfx_min;
	int i;
	int psiz = GetBytesPerPixel (picasso_vidinfo.rgbformat);

	if (picasso96_state.RGBFormat != RGBFB_CHUNKY)
	    abort ();

	dstp += wgfx_y * picasso_vidinfo.rowbytes + (wgfx_min - wgfx_linestart) * psiz;
	switch (psiz) {
	case 2:
	    for (i = 0; i < width; i++)
		*((uae_u16 *) dstp + i) = picasso_vidinfo.clut[src[i]];
	    break;
	case 4:
	    for (i = 0; i < width; i++)
		*((uae_u32 *) dstp + i) = picasso_vidinfo.clut[src[i]];
	    break;
	default:
	    abort ();
	}
    }

  out:
    gfx_unlock_picasso ();
    wgfx_linestart = 0xFFFFFFFF;
}

STATIC_INLINE void wgfx_flushline (void)
{
    if (wgfx_linestart == 0xFFFFFFFF || !picasso_on)
	return;
    wgfx_do_flushline ();
}

static int renderinfo_is_current_screen (struct RenderInfo *ri)
{
    if (!picasso_on)
	return 0;
    if (ri->Memory != gfxmemory + (picasso96_state.Address - gfxmem_start))
	return 0;

    return 1;
}

/* Clear our screen, since we've got a new Picasso screen-mode, and refresh with the proper contents
 * This is called on several occasions:
 * 1. Amiga-->Picasso transition, via SetSwitch()
 * 2. Picasso-->Picasso transition, via SetPanning().
 * 3. whenever the graphics code notifies us that the screen contents have been lost.
 */
void picasso_refresh (void)
{
    struct RenderInfo ri;

    if (!picasso_on)
	return;

    /* Make sure that the first time we show a Picasso video mode, we don't blit any crap.
     * We can do this by checking if we have an Address yet.  */
    if (picasso96_state.Address) {
	/* blit the stuff from our static frame-buffer to the gfx-card */
	uae_u8 *ptr = gfxmemory + (picasso96_state.Address - gfxmem_start);
	ri.BytesPerRow = picasso96_state.BytesPerRow;
	ri.RGBFormat = picasso96_state.RGBFormat;
	do_blit (ptr, picasso96_state.BytesPerRow, 0, 0, 0, 0, picasso96_state.Width, picasso96_state.Height, 0);
    } else
	write_log ("ERROR - picasso_refresh() can't refresh!\n");
}

/*
 * BOOL FindCard(struct BoardInfo *bi);       and
 *
 * FindCard is called in the first stage of the board initialisation and
 * configuration and is used to look if there is a free and unconfigured
 * board of the type the driver is capable of managing. If it finds one,
 * it immediately reserves it for use by Picasso96, usually by clearing
 * the CDB_CONFIGME bit in the flags field of the ConfigDev struct of
 * this expansion card. But this is only a common example, a driver can
 * do whatever it wants to mark this card as used by the driver. This
 * mechanism is intended to ensure that a board is only configured and
 * used by one driver. FindBoard also usually fills some fields of the
 * BoardInfo struct supplied by the caller, the rtg.library, for example
 * the MemoryBase, MemorySize and RegisterBase fields.
 */
uae_u32 picasso_FindCard (void)
{
    uaecptr AmigaBoardInfo = m68k_areg (regs, 0);
    /* NOTES: See BoardInfo struct definition in Picasso96 dev info */

    if (allocated_gfxmem && !picasso96_state.CardFound) {
	/* Fill in MemoryBase, MemorySize */
	put_long (AmigaBoardInfo + PSSO_BoardInfo_MemoryBase, gfxmem_start);
	/* size of memory, minus a 32K chunk: 16K for pattern bitmaps, 16K for resolution list */
	put_long (AmigaBoardInfo + PSSO_BoardInfo_MemorySize, allocated_gfxmem - 32768);

	picasso96_state.CardFound = 1;	/* mark our "card" as being found */
	return -1;
    } else
	return 0;
}

static void FillBoardInfo (uaecptr amigamemptr, struct LibResolution *res, struct PicassoResolution *dm)
{
    char *uaememptr;
    switch (dm->depth) {
    case 1:
	res->Modes[CHUNKY] = amigamemptr;
	break;
    case 2:
	res->Modes[HICOLOR] = amigamemptr;
	break;
    case 3:
	res->Modes[TRUECOLOR] = amigamemptr;
	break;
    default:
	res->Modes[TRUEALPHA] = amigamemptr;
	break;
    }
    uaememptr = gfxmem_xlate (amigamemptr);	/* I know that amigamemptr is inside my gfxmem chunk, so I can just do the xlate() */
    memset (uaememptr, 0, PSSO_ModeInfo_sizeof);	/* zero out our ModeInfo struct */

    put_word (amigamemptr + PSSO_ModeInfo_Width, dm->res.width);
    put_word (amigamemptr + PSSO_ModeInfo_Height, dm->res.height);
    put_byte (amigamemptr + PSSO_ModeInfo_Depth, dm->depth * 8);
    put_byte (amigamemptr + PSSO_ModeInfo_Flags, 0);
    put_word (amigamemptr + PSSO_ModeInfo_HorTotal, dm->res.width);
    put_word (amigamemptr + PSSO_ModeInfo_HorBlankSize, 0);
    put_word (amigamemptr + PSSO_ModeInfo_HorSyncStart, 0);
    put_word (amigamemptr + PSSO_ModeInfo_HorSyncSize, 0);
    put_byte (amigamemptr + PSSO_ModeInfo_HorSyncSkew, 0);
    put_byte (amigamemptr + PSSO_ModeInfo_HorEnableSkew, 0);

    put_word (amigamemptr + PSSO_ModeInfo_VerTotal, dm->res.height);
    put_word (amigamemptr + PSSO_ModeInfo_VerBlankSize, 0);
    put_word (amigamemptr + PSSO_ModeInfo_VerSyncStart, 0);
    put_word (amigamemptr + PSSO_ModeInfo_VerSyncSize, 0);

    put_byte (amigamemptr + PSSO_ModeInfo_first_union, 98);
    put_byte (amigamemptr + PSSO_ModeInfo_second_union, 14);

    put_long (amigamemptr + PSSO_ModeInfo_PixelClock, dm->res.width * dm->res.height * dm->refresh);
}

static uae_u32 AssignModeID (int i, int count)
{
    if (DisplayModes[i].res.width == 320 && DisplayModes[i].res.height == 200)
	return 0x50001000;
    else if (DisplayModes[i].res.width == 320 && DisplayModes[i].res.height == 240)
	return 0x50011000;
    else if (DisplayModes[i].res.width == 640 && DisplayModes[i].res.height == 400)
	return 0x50021000;
    else if (DisplayModes[i].res.width == 640 && DisplayModes[i].res.height == 480)
	return 0x50031000;
    else if (DisplayModes[i].res.width == 800 && DisplayModes[i].res.height == 600)
	return 0x50041000;
    else if (DisplayModes[i].res.width == 1024 && DisplayModes[i].res.height == 768)
	return 0x50051000;
    else if (DisplayModes[i].res.width == 1152 && DisplayModes[i].res.height == 864)
	return 0x50061000;
    else if (DisplayModes[i].res.width == 1280 && DisplayModes[i].res.height == 1024)
	return 0x50071000;
    else if (DisplayModes[i].res.width == 1600 && DisplayModes[i].res.height == 1280)
	return 0x50081000;

    return 0x50091000 + count * 0x10000;
}

/****************************************
* InitCard()
*
* a2: BoardInfo structure ptr - Amiga-based address in Intel endian-format
*
* Job - fill in the following structure members:
* gbi_RGBFormats: the pixel formats that the host-OS of UAE supports
*     If UAE is running in a window, it should ONLY report the pixel format of the host-OS desktop
*     If UAE is running full-screen, it should report ALL pixel formats that the host-OS can handle in full-screen
*     NOTE: If full-screen, and the user toggles to windowed-mode, all hell will break loose visually.  Must inform
*           user that they're doing something stupid (unless their desktop and full-screen colour modes match).
* gbi_SoftSpriteFlags: should be the same as above for now, until actual cursor support is added
* gbi_BitsPerCannon: could be 6 or 8 or ???, depending on the host-OS gfx-card
* gbi_MaxHorResolution: fill this in for all modes (even if you don't support them)
* gbi_MaxVerResolution: fill this in for all modes (even if you don't support them)
*/
uae_u32 picasso_InitCard (void)
{
    struct LibResolution res;
    int i;
    int ModeInfoStructureCount = 1, LibResolutionStructureCount = 0;
    uaecptr amigamemptr = 0;
    uaecptr AmigaBoardInfo = m68k_areg (regs, 2);
    put_word (AmigaBoardInfo + PSSO_BoardInfo_BitsPerCannon, DX_BitsPerCannon ());
    put_word (AmigaBoardInfo + PSSO_BoardInfo_RGBFormats, picasso96_pixel_format);
    put_word (AmigaBoardInfo + PSSO_BoardInfo_SoftSpriteFlags, picasso96_pixel_format);
    put_word (AmigaBoardInfo + PSSO_BoardInfo_MaxHorResolution + 0, planar.width);
    put_word (AmigaBoardInfo + PSSO_BoardInfo_MaxHorResolution + 2, chunky.width);
    put_word (AmigaBoardInfo + PSSO_BoardInfo_MaxHorResolution + 4, hicolour.width);
    put_word (AmigaBoardInfo + PSSO_BoardInfo_MaxHorResolution + 6, truecolour.width);
    put_word (AmigaBoardInfo + PSSO_BoardInfo_MaxHorResolution + 8, alphacolour.width);
    put_word (AmigaBoardInfo + PSSO_BoardInfo_MaxVerResolution + 0, planar.height);
    put_word (AmigaBoardInfo + PSSO_BoardInfo_MaxVerResolution + 2, chunky.height);
    put_word (AmigaBoardInfo + PSSO_BoardInfo_MaxVerResolution + 4, hicolour.height);
    put_word (AmigaBoardInfo + PSSO_BoardInfo_MaxVerResolution + 6, truecolour.height);
    put_word (AmigaBoardInfo + PSSO_BoardInfo_MaxVerResolution + 8, alphacolour.height);

    for (i = 0; i < mode_count;) {
	int j = i;
	/* Add a LibResolution structure to the ResolutionsList MinList in our BoardInfo */
	res.DisplayID = AssignModeID (i, LibResolutionStructureCount);
	res.BoardInfo = AmigaBoardInfo;
	res.Width = DisplayModes[i].res.width;
	res.Height = DisplayModes[i].res.height;
	res.Flags = P96F_PUBLIC;
	res.P96ID[0] = 'P';
	res.P96ID[1] = '9';
	res.P96ID[2] = '6';
	res.P96ID[3] = '-';
	res.P96ID[4] = '0';
	res.P96ID[5] = ':';
	strcpy (res.Name, "uaegfx:");
	strncat (res.Name, DisplayModes[i].name, strchr (DisplayModes[i].name, ',') - DisplayModes[i].name);
	res.Modes[PLANAR] = 0;
	res.Modes[CHUNKY] = 0;
	res.Modes[HICOLOR] = 0;
	res.Modes[TRUECOLOR] = 0;
	res.Modes[TRUEALPHA] = 0;

	do {
	    /* Handle this display mode's depth */
	    amigamemptr = gfxmem_start + allocated_gfxmem - (PSSO_ModeInfo_sizeof * ModeInfoStructureCount++);
	    FillBoardInfo (amigamemptr, &res, &DisplayModes[i]);
	    i++;
	} while (i < mode_count
		 && DisplayModes[i].res.width == DisplayModes[j].res.width
		 && DisplayModes[i].res.height == DisplayModes[j].res.height);

	amigamemptr = gfxmem_start + allocated_gfxmem - 16384 + (PSSO_LibResolution_sizeof * LibResolutionStructureCount++);
	CopyLibResolutionStructureU2A (&res, amigamemptr);
	DumpLibResolutionStructure (amigamemptr);
	AmigaListAddTail (AmigaBoardInfo + PSSO_BoardInfo_ResolutionsList, amigamemptr);
    }

    return 0;
}

extern int x_size, y_size;

/*
 * SetSwitch:
 * a0:	struct BoardInfo
 * d0.w:	BOOL state
 * this function should set a board switch to let the Amiga signal pass
 * through when supplied with a 0 in d0 and to show the board signal if
 * a 1 is passed in d0. You should remember the current state of the
 * switch to avoid unneeded switching. If your board has no switch, then
 * simply supply a function that does nothing except a RTS.
 *
 * NOTE: Return the opposite of the switch-state. BDK
*/
uae_u32 picasso_SetSwitch (void)
{
    uae_u16 flag = m68k_dreg (regs, 0) & 0xFFFF;

    /* Do not switch immediately.  Tell the custom chip emulation about the
     * desired state, and wait for custom.c to call picasso_enablescreen
     * whenever it is ready to change the screen state.  */
    picasso_requested_on = !!flag;
#if 0
    write_log ("SetSwitch() - trying to show %s screen\n", flag ? "picasso96" : "amiga");
#endif
    /* Put old switch-state in D0 */
    return !flag;
}

void picasso_enablescreen (int on)
{
    wgfx_linestart = 0xFFFFFFFF;
    picasso_refresh ();
#if 0
    write_log ("SetSwitch() - showing %s screen\n", on ? "picasso96" : "amiga");
#endif
}

static int first_color_changed = 256;
static int last_color_changed = -1;

void picasso_handle_vsync (void)
{
    if (first_color_changed < last_color_changed) {
	DX_SetPalette (first_color_changed, last_color_changed);
	/* If we're emulating a CLUT mode, we need to redraw the entire screen.  */
	if (picasso_vidinfo.rgbformat != picasso96_state.RGBFormat)
	    picasso_refresh ();
    }

    first_color_changed = 256;
    last_color_changed = -1;
}

/*
 * SetColorArray:
 * a0: struct BoardInfo
 * d0.w: startindex
 * d1.w: count
 * when this function is called, your driver has to fetch "count" color
 * values starting at "startindex" from the CLUT field of the BoardInfo
 * structure and write them to the hardware. The color values are always
 * between 0 and 255 for each component regardless of the number of bits
 * per cannon your board has. So you might have to shift the colors
 * before writing them to the hardware.
 */
uae_u32 picasso_SetColorArray (void)
{
    /* Fill in some static UAE related structure about this new CLUT setting
     * We need this for CLUT-based displays, and for mapping CLUT to hi/true colour */
    uae_u16 start = m68k_dreg (regs, 0);
    uae_u16 count = m68k_dreg (regs, 1);
    int i;
    uaecptr boardinfo = m68k_areg (regs, 0);
    uaecptr clut = boardinfo + PSSO_BoardInfo_CLUT + start * 3;
    int changed = 0;

    for (i = start; i < start + count; i++) {
	int r = get_byte (clut);
	int g = get_byte (clut + 1);
	int b = get_byte (clut + 2);

	changed |= (picasso96_state.CLUT[i].Red != r || picasso96_state.CLUT[i].Green != g || picasso96_state.CLUT[i].Blue != b);

	picasso96_state.CLUT[i].Red = r;
	picasso96_state.CLUT[i].Green = g;
	picasso96_state.CLUT[i].Blue = b;
	clut += 3;
    }
    if (changed) {
	if (start < first_color_changed)
	    first_color_changed = start;
	if (start + count > last_color_changed)
	    last_color_changed = start + count;
    }
    /*write_log ("SetColorArray(%d,%d)\n", start, count); */
    return 1;
}

/*
 * SetDAC:
 * a0: struct BoardInfo
 * d7: RGBFTYPE RGBFormat
 * This function is called whenever the RGB format of the display changes,
 * e.g. from chunky to TrueColor. Usually, all you have to do is to set
 * the RAMDAC of your board accordingly.
 */
uae_u32 picasso_SetDAC (void)
{
    /* Fill in some static UAE related structure about this new DAC setting
     * Lets us keep track of what pixel format the Amiga is thinking about in our frame-buffer */

    write_log ("SetDAC()\n");
    return 1;
}

static int set_gc_called = 0;
static int set_panning_called = 0;

static void init_picasso_screen (void)
{
    if (!set_gc_called)
	return;

    if (set_panning_called)
	picasso96_state.Extent = picasso96_state.Address + (picasso96_state.BytesPerRow * picasso96_state.Height);

    gfx_set_picasso_modeinfo (picasso96_state.Width, picasso96_state.Height, picasso96_state.GC_Depth, picasso96_state.RGBFormat);
    DX_SetPalette (0, 256);

    wgfx_linestart = 0xFFFFFFFF;
    picasso_refresh ();
}

/*
 * SetGC:
 * a0: struct BoardInfo
 * a1: struct ModeInfo
 * d0: BOOL Border
 * This function is called whenever another ModeInfo has to be set. This
 * function simply sets up the CRTC and TS registers to generate the
 * timing used for that screen mode. You should not set the DAC, clocks
 * or linear start adress. They will be set when appropriate by their
 * own functions.
 */
uae_u32 picasso_SetGC (void)
{
    /* Fill in some static UAE related structure about this new ModeInfo setting */
    uaecptr modeinfo = m68k_areg (regs, 1);

    picasso96_state.Width = get_word (modeinfo + PSSO_ModeInfo_Width);
    picasso96_state.VirtualWidth = picasso96_state.Width;	/* in case SetPanning doesn't get called */

    picasso96_state.Height = get_word (modeinfo + PSSO_ModeInfo_Height);
    picasso96_state.VirtualHeight = picasso96_state.Height;

    picasso96_state.GC_Depth = get_byte (modeinfo + PSSO_ModeInfo_Depth);
    picasso96_state.GC_Flags = get_byte (modeinfo + PSSO_ModeInfo_Flags);

    write_log ("SetGC(%d,%d,%d)\n", picasso96_state.Width, picasso96_state.Height, picasso96_state.GC_Depth);

    set_gc_called = 1;		/* @@@ when do we need to reset this? */
    init_picasso_screen ();
    return 1;
}

/*
 * SetPanning:
 * a0: struct BoardInfo
 * a1: UBYTE *Memory
 * d0: uae_u16 Width
 * d1: WORD XOffset
 * d2: WORD YOffset
 * d7: RGBFTYPE RGBFormat
 * This function sets the view origin of a display which might also be
 * overscanned. In register a1 you get the start address of the screen
 * bitmap on the Amiga side. You will have to subtract the starting
 * address of the board memory from that value to get the memory start
 * offset within the board. Then you get the offset in pixels of the
 * left upper edge of the visible part of an overscanned display. From
 * these values you will have to calculate the LinearStartingAddress
 * fields of the CRTC registers.

 * NOTE: SetPanning() can be used to know when a Picasso96 screen is
 * being opened.  Better to do the appropriate clearing of the
 * background here than in SetSwitch() derived functions,
 * because SetSwitch() is not called for subsequent Picasso screens.
 */
uae_u32 picasso_SetPanning (void)
{
    uae_u16 Width = m68k_dreg (regs, 0);
    uaecptr start_of_screen = m68k_areg (regs, 1);

    picasso96_state.Address = start_of_screen;	/* Amiga-side address */
    picasso96_state.XOffset = (uae_s16) m68k_dreg (regs, 1);
    picasso96_state.YOffset = (uae_s16) m68k_dreg (regs, 2);
    picasso96_state.VirtualWidth = Width;
    picasso96_state.RGBFormat = m68k_dreg (regs, 7);
    picasso96_state.BytesPerPixel = GetBytesPerPixel (picasso96_state.RGBFormat);
    picasso96_state.BytesPerRow = Width * picasso96_state.BytesPerPixel;

    set_panning_called = 1;
    write_log ("SetPanning(%d, %d, %d) Start 0x%x, BPR %d\n",
	       Width, picasso96_state.XOffset, picasso96_state.YOffset, start_of_screen, picasso96_state.BytesPerRow);

    init_picasso_screen ();

    return 1;
}

static void do_xor8 (uae_u8 * ptr, long len, uae_u32 val)
{
    int i;
#if 0 && defined ALIGN_POINTER_TO32
    int align_adjust = ALIGN_POINTER_TO32 (ptr);
    int len2;

    len -= align_adjust;
    while (align_adjust) {
	*ptr ^= val;
	ptr++;
	align_adjust--;
    }
    len2 = len >> 2;
    len -= len2 << 2;
    for (i = 0; i < len2; i++, ptr += 4) {
	*(uae_u32 *) ptr ^= val;
    }
    while (len) {
	*ptr ^= val;
	ptr++;
	len--;
    }
    return;
#endif
    for (i = 0; i < len; i++, ptr++) {
	do_put_mem_byte (ptr, do_get_mem_byte (ptr) ^ val);
    }
}

/*
 * InvertRect:
 * 
 * Inputs:
 * a0:struct BoardInfo *bi
 * a1:struct RenderInfo *ri
 * d0.w:X
 * d1.w:Y
 * d2.w:Width
 * d3.w:Height
 * d4.l:Mask
 * d7.l:RGBFormat
 * 
 * This function is used to invert a rectangular area on the board. It is called by BltBitMap,
 * BltPattern and BltTemplate.
 */
uae_u32 picasso_InvertRect (void)
{
    uaecptr renderinfo = m68k_areg (regs, 1);
    unsigned long X = (uae_u16) m68k_dreg (regs, 0);
    unsigned long Y = (uae_u16) m68k_dreg (regs, 1);
    unsigned long Width = (uae_u16) m68k_dreg (regs, 2);
    unsigned long Height = (uae_u16) m68k_dreg (regs, 3);
    uae_u32 mask = m68k_dreg (regs, 4);
    int Bpp = GetBytesPerPixel (m68k_dreg (regs, 7));
    uae_u32 xorval;
    unsigned int lines;
    struct RenderInfo ri;
    uae_u8 *uae_mem, *rectstart;
    unsigned long width_in_bytes;

    wgfx_flushline ();

    if (!CopyRenderInfoStructureA2U (renderinfo, &ri))
	return 0;

    /*write_log ("InvertRect %d %lx\n", Bpp, (long)mask); */

    /* ??? Brian? mask used to be 32 bit, but it appears that only 8 bit
     * values are passed to this function.  This code here seems to work
     * much better... */
    if (mask != 0xFF && Bpp > 8) {
	write_log ("InvertRect: not obeying mask 0x%x properly with Bpp %d.\n", mask, Bpp);
	mask = 0xFF;
    }
    if ((mask & ~0xFF) != 0) {
	write_log ("InvertRect: mask has high bits set!\n");
    }
    xorval = 0x01010101 * (mask & 0xFF);
    width_in_bytes = Bpp * Width;
    rectstart = uae_mem = ri.Memory + Y * ri.BytesPerRow + X * Bpp;

    for (lines = 0; lines < Height; lines++, uae_mem += ri.BytesPerRow)
	do_xor8 (uae_mem, width_in_bytes, xorval);

    if (renderinfo_is_current_screen (&ri)) {
	if (mask == 0xFF)
	    do_invertrect (rectstart, ri.BytesPerRow, X, Y, Width, Height);
	else
	    do_blit (rectstart, ri.BytesPerRow, 0, 0, X, Y, Width, Height, 0);
    }

    return 1;			/* 1 if supported, 0 otherwise */
}

/***********************************************************
FillRect:
***********************************************************
* a0: 	struct BoardInfo *
* a1:	struct RenderInfo *
* d0: 	WORD X
* d1: 	WORD Y
* d2: 	WORD Width
* d3: 	WORD Height
* d4:	uae_u32 Pen
* d5:	UBYTE Mask
* d7:	uae_u32 RGBFormat
***********************************************************/
uae_u32 picasso_FillRect (void)
{
    uaecptr renderinfo = m68k_areg (regs, 1);
    unsigned long X = (uae_u16) m68k_dreg (regs, 0);
    unsigned long Y = (uae_u16) m68k_dreg (regs, 1);
    unsigned long Width = (uae_u16) m68k_dreg (regs, 2);
    unsigned long Height = (uae_u16) m68k_dreg (regs, 3);
    uae_u32 Pen = m68k_dreg (regs, 4);
    uae_u8 Mask = (uae_u8) m68k_dreg (regs, 5);
    uae_u32 RGBFormat = m68k_dreg (regs, 7);

    uae_u8 *src, *dst;
    uae_u8 *start, *oldstart;
    unsigned long lines, cols;
    int Bpp;
    struct RenderInfo ri;

    wgfx_flushline ();

    if (!CopyRenderInfoStructureA2U (renderinfo, &ri) || Y == 0xFFFF)
	return 0;

    if (ri.RGBFormat != RGBFormat)
	write_log ("Weird Stuff!\n");

    Bpp = GetBytesPerPixel (RGBFormat);

    /*write_log ("FillRect(%d, %d, %d, %d) Pen 0x%x BPP %d BPR %d Mask 0x%x\n",
       X, Y, Width, Height, Pen, Bpp, ri.BytesPerRow, Mask); */

    if (Mask == 0xFF) {
	/* Do our virtual frame-buffer memory.  First, we do a single line fill by hand */
	oldstart = start = ri.Memory + Y * ri.BytesPerRow + X * Bpp;
	switch (Bpp) {
	case 1:
	    memset (start, Pen, Width);
	    break;
	case 2:
	    for (cols = 0; cols < Width; cols++) {
		do_put_mem_word ((uae_u16 *) start, Pen);
		start += 2;
	    }
	    break;
	case 3:
	    for (cols = 0; cols < Width; cols++) {
		do_put_mem_byte (start, Pen & 0x000000FF);
		start++;
		*(uae_u16 *) (start) = (Pen & 0x00FFFF00) >> 8;
		start += 2;
	    }
	    break;
	case 4:
	    for (cols = 0; cols < Width; cols++) {
		/**start = Pen; */
		do_put_mem_long ((uae_u32 *) start, Pen);
		start += 4;
	    }
	    break;
	}
	src = oldstart;
	dst = src + ri.BytesPerRow;
	/* next, we do the remaining line fills via memcpy() */
	for (lines = 0; lines < (Height - 1); lines++, dst += ri.BytesPerRow)
	    memcpy (dst, src, Width * Bpp);

	if (renderinfo_is_current_screen (&ri))
	    do_fillrect (src, X, Y, Width, Height);

	return 1;
    }

    /* We get here only if Mask != 0xFF */
    if (Bpp != 1) {
	write_log ("Picasso: mask != 0xFF in truecolor mode!\n");
	return 0;
    }
    Pen &= Mask;
    Mask = ~Mask;

    oldstart = ri.Memory + Y * ri.BytesPerRow + X * Bpp;
    {
	uae_u8 *start = oldstart;
	uae_u8 *end = start + Height * ri.BytesPerRow;
	for (; start != end; start += ri.BytesPerRow) {
	    uae_u8 *p = start;
	    unsigned long cols;
	    for (cols = 0; cols < Width; cols++) {
		uae_u32 tmpval = do_get_mem_byte (p + cols) & Mask;
		do_put_mem_byte (p + cols, Pen | tmpval);
	    }
	}
    }
    if (renderinfo_is_current_screen (&ri))
	do_blit (oldstart, ri.BytesPerRow, 0, 0, X, Y, Width, Height, 0);

    return 1;
}

/*
 * BlitRect() is a generic (any chunky pixel format) rectangle copier
 * NOTE: If dstri is NULL, then we're only dealing with one RenderInfo area, and called from picasso_BlitRect()
 */
static void BlitRect (struct RenderInfo *ri, struct RenderInfo *dstri,
		      unsigned long srcx, unsigned long srcy, unsigned long dstx, unsigned long dsty,
		      unsigned long width, unsigned long height, uae_u8 mask)
{
    uae_u8 *src, *dst, *tmp, *tmp2, *tmp3;
    unsigned long lines;
    uae_u8 Bpp = GetBytesPerPixel (ri->RGBFormat);
    uae_u8 *blitsrc;
    unsigned long total_width = width * Bpp;
    unsigned long linewidth = (total_width + 15) & ~15;
    int cant_blit = 1;

    /*
     * If we have no destination RenderInfo, then we're dealing with a single-buffer action, called
     * from picasso_BlitRect().  The code up to the DX_xxxxx() functions deals with the frame-buffer,
     * while the DX_ functions actually deal with the visible screen.
     *
     * If we have a destination RenderInfo, then we've been called from picasso_BlitRectNoMaskComplete()
     * and we need to put the results on the screen from the frame-buffer.
     */
    if (dstri == NULL) {
	dstri = ri;
	cant_blit = 0;
    }

    /* Do our virtual frame-buffer memory first */
    src = ri->Memory + srcx * Bpp + srcy * ri->BytesPerRow;
    dst = dstri->Memory + dstx * Bpp + dsty * dstri->BytesPerRow;
    blitsrc = dst;
    if (mask != 0xFF && Bpp > 1)
	write_log ("ERROR - not obeying BlitRect() mask 0x%x properly with Bpp %d.\n", mask, Bpp);

    if (mask == 0xFF || Bpp > 1) {
	/* handle normal case efficiently */
	if (ri->Memory == dstri->Memory && dsty == srcy) {
	    unsigned long i;
	    for (i = 0; i < height; i++, src += ri->BytesPerRow, dst += dstri->BytesPerRow)
		memmove (dst, src, total_width);
	} else if (dsty < srcy) {
	    unsigned long i;
	    for (i = 0; i < height; i++, src += ri->BytesPerRow, dst += dstri->BytesPerRow)
		memcpy (dst, src, total_width);
	} else {
	    unsigned long i;
	    src += (height - 1) * ri->BytesPerRow;
	    dst += (height - 1) * dstri->BytesPerRow;
	    for (i = 0; i < height; i++, src -= ri->BytesPerRow, dst -= dstri->BytesPerRow)
		memcpy (dst, src, total_width);
	}
	if (renderinfo_is_current_screen (dstri))
	    do_blit (blitsrc, dstri->BytesPerRow, srcx, srcy, dstx, dsty, width, height, !cant_blit);
	return;
    }

    tmp3 = tmp2 = tmp = xmalloc (linewidth * height);	/* allocate enough memory for the src-rect */
    if (!tmp)
	return;

    /* copy the src-rect into our temporary buffer space */
    for (lines = 0; lines < height; lines++, src += ri->BytesPerRow, tmp2 += linewidth) {
	memcpy (tmp2, src, total_width);
    }

    /* copy the temporary buffer to the destination */
    for (lines = 0; lines < height; lines++, dst += dstri->BytesPerRow, tmp += linewidth) {
	unsigned long cols;
	for (cols = 0; cols < width; cols++) {
	    dst[cols] &= ~mask;
	    dst[cols] |= tmp[cols] & mask;
	}
    }
    if (renderinfo_is_current_screen (dstri))
	do_blit (blitsrc, dstri->BytesPerRow, srcx, srcy, dstx, dsty, width, height, 0);
    /* free the temp-buf */
    free (tmp3);

}

/***********************************************************
BlitRect:
***********************************************************
* a0: 	struct BoardInfo
* a1:	struct RenderInfo
* d0: 	WORD SrcX
* d1: 	WORD SrcY
* d2: 	WORD DstX
* d3: 	WORD DstY
* d4:   WORD Width
* d5:   WORD Height
* d6:	UBYTE Mask
* d7:	uae_u32 RGBFormat
***********************************************************/
uae_u32 picasso_BlitRect (void)
{
    uaecptr renderinfo = m68k_areg (regs, 1);
    unsigned long srcx = (uae_u16) m68k_dreg (regs, 0);
    unsigned long srcy = (uae_u16) m68k_dreg (regs, 1);
    unsigned long dstx = (uae_u16) m68k_dreg (regs, 2);
    unsigned long dsty = (uae_u16) m68k_dreg (regs, 3);
    unsigned long width = (uae_u16) m68k_dreg (regs, 4);
    unsigned long height = (uae_u16) m68k_dreg (regs, 5);
    uae_u8 Mask = (uae_u8) m68k_dreg (regs, 6);

    struct RenderInfo ri;

    wgfx_flushline ();

    if (!CopyRenderInfoStructureA2U (renderinfo, &ri))
	return 0;

    BlitRect (&ri, NULL, srcx, srcy, dstx, dsty, width, height, Mask);
    /*write_log ("BlitRect(%d, %d, %d, %d, %d, %d, 0x%x)\n", srcx, srcy, dstx, dsty, width, height, Mask); */

    return 1;
}

/***********************************************************
BlitRectNoMaskComplete:
***********************************************************
* a0: 	struct BoardInfo
* a1:	struct RenderInfo (src)
* a2:   struct RenderInfo (dst)
* d0: 	WORD SrcX
* d1: 	WORD SrcY
* d2: 	WORD DstX
* d3: 	WORD DstY
* d4:   WORD Width
* d5:   WORD Height
* d6:	UBYTE OpCode
* d7:	uae_u32 RGBFormat
* NOTE: MUST return 0 in D0 if we're not handling this operation
*       because the RGBFormat or opcode aren't supported.
*       OTHERWISE return 1
***********************************************************/
uae_u32 picasso_BlitRectNoMaskComplete (void)
{
    uaecptr srcri = m68k_areg (regs, 1);
    uaecptr dstri = m68k_areg (regs, 2);
    unsigned long srcx = (uae_u16) m68k_dreg (regs, 0);
    unsigned long srcy = (uae_u16) m68k_dreg (regs, 1);
    unsigned long dstx = (uae_u16) m68k_dreg (regs, 2);
    unsigned long dsty = (uae_u16) m68k_dreg (regs, 3);
    unsigned long width = (uae_u16) m68k_dreg (regs, 4);
    unsigned long height = (uae_u16) m68k_dreg (regs, 5);
    uae_u8 OpCode = m68k_dreg (regs, 6);
    uae_u32 RGBFmt = m68k_dreg (regs, 7);
    struct RenderInfo src_ri, dst_ri;

    wgfx_flushline ();

    if (!CopyRenderInfoStructureA2U (srcri, &src_ri)
	|| !CopyRenderInfoStructureA2U (dstri, &dst_ri))
	return 0;

    /*write_log ("BlitRectNoMaskComplete() op 0x%2x, Bpp %d, xy(%4d,%4d) --> xy(%4d,%4d), wh(%4d,%4d)\n",
       OpCode, Bpp, srcx, srcy, dstx, dsty, width, height); */
    /*write_log ("-- src mem 0x%x BPR %d, dst mem 0x%x BPR %d, screen-mem 0x%x - 0x%x\n",
       src_ri.Memory, src_ri.BytesPerRow, dst_ri.Memory, dst_ri.BytesPerRow, picasso96_state.Address, picasso96_state.Extent); */

    switch (OpCode) {
    case 0x0C:
	BlitRect (&src_ri, &dst_ri, srcx, srcy, dstx, dsty, width, height, 0xFF);
	return 1;

    default:
	/* FOR NOW! */
	return 0;
    }
}

/* This utility function is used both by BlitTemplate() and BlitPattern() */
STATIC_INLINE void PixelWrite1 (uae_u8 * mem, int bits, uae_u32 fgpen, uae_u32 mask)
{
    if (mask != 0xFF)
	fgpen = (fgpen & mask) | (do_get_mem_byte (mem + bits) & ~mask);
    do_put_mem_byte (mem + bits, fgpen);
}

STATIC_INLINE void PixelWrite2 (uae_u8 * mem, int bits, uae_u32 fgpen)
{
    do_put_mem_word (((uae_u16 *) mem) + bits, fgpen);
}

STATIC_INLINE void PixelWrite3 (uae_u8 * mem, int bits, uae_u32 fgpen)
{
    do_put_mem_byte (mem + bits * 3, fgpen & 0x000000FF);
    *(uae_u16 *) (mem + bits * 3 + 1) = (fgpen & 0x00FFFF00) >> 8;
}

STATIC_INLINE void PixelWrite4 (uae_u8 * mem, int bits, uae_u32 fgpen)
{
    do_put_mem_long (((uae_u32 *) mem) + bits, fgpen);
}

STATIC_INLINE void PixelWrite (uae_u8 * mem, int bits, uae_u32 fgpen, uae_u8 Bpp, uae_u32 mask)
{
    switch (Bpp) {
    case 1:
	if (mask != 0xFF)
	    fgpen = (fgpen & mask) | (do_get_mem_byte (mem + bits) & ~mask);
	do_put_mem_byte (mem + bits, fgpen);
	break;
    case 2:
	do_put_mem_word (((uae_u16 *) mem) + bits, fgpen);
	break;
    case 3:
	do_put_mem_byte (mem + bits * 3, fgpen & 0x000000FF);
	*(uae_u16 *) (mem + bits * 3 + 1) = (fgpen & 0x00FFFF00) >> 8;
	break;
    case 4:
	do_put_mem_long (((uae_u32 *) mem) + bits, fgpen);
	break;
    }
}

/*
 * BlitPattern:
 * 
 * Synopsis:BlitPattern(bi, ri, pattern, X, Y, Width, Height, Mask, RGBFormat);
 * Inputs:
 * a0:struct BoardInfo *bi
 * a1:struct RenderInfo *ri
 * a2:struct Pattern *pattern
 * d0.w:X
 * d1.w:Y
 * d2.w:Width
 * d3.w:Height
 * d4.w:Mask
 * d7.l:RGBFormat
 * 
 * This function is used to paint a pattern on the board memory using the blitter. It is called by
 * BltPattern, if a AreaPtrn is used with positive AreaPtSz. The pattern consists of a b/w image
 * using a single plane of image data which will be expanded repeatedly to the destination RGBFormat
 * using ForeGround and BackGround pens as well as draw modes. The width of the pattern data is
 * always 16 pixels (one word) and the height is calculated as 2^Size. The data must be shifted up
 * and to the left by XOffset and YOffset pixels at the beginning.
 */
uae_u32 picasso_BlitPattern (void)
{
    uaecptr rinf = m68k_areg (regs, 1);
    uaecptr pinf = m68k_areg (regs, 2);
    unsigned long X = (uae_u16) m68k_dreg (regs, 0);
    unsigned long Y = (uae_u16) m68k_dreg (regs, 1);
    unsigned long W = (uae_u16) m68k_dreg (regs, 2);
    unsigned long H = (uae_u16) m68k_dreg (regs, 3);
    uae_u8 Mask = (uae_u8) m68k_dreg (regs, 4);
    uae_u32 RGBFmt = m68k_dreg (regs, 7);

    uae_u8 Bpp = GetBytesPerPixel (RGBFmt);
    int inversion = 0;
    struct RenderInfo ri;
    struct Pattern pattern;
    unsigned long rows;
    uae_u32 fgpen;
    uae_u8 *uae_mem, *frame_buffer_UAM;
    int xshift;
    unsigned long ysize_mask;

    wgfx_flushline ();

    if (!CopyRenderInfoStructureA2U (rinf, &ri)
	|| !CopyPatternStructureA2U (pinf, &pattern))
	return 0;

    Bpp = GetBytesPerPixel (ri.RGBFormat);
    uae_mem = ri.Memory + Y * ri.BytesPerRow + X * Bpp;	/* offset with address */

    if (pattern.DrawMode & INVERS)
	inversion = 1;

    pattern.DrawMode &= 0x03;
    if (Mask != 0xFF) {
	if (Bpp > 1)
	    write_log ("ERROR - not obeying BlitPattern() mask 0x%x properly with Bpp %d.\n", Mask, Bpp);
	else if (pattern.DrawMode == COMP) {
	    write_log ("ERROR - Unsupported Mask value 0x%x with COMP Draw in BlitPattern(), using fallback method.\n", Mask);
	    return 0;
	}
    }

    /* write_log ("BlitPattern() xy(%d,%d), wh(%d,%d) draw 0x%x, off(%d,%d), ph %d\n",
       X, Y, W, H, pattern.DrawMode, pattern.XOffset, pattern.YOffset, 1 << pattern.Size); */
#ifdef _DEBUG
    DumpPattern (&pattern);
#endif
    ysize_mask = (1 << pattern.Size) - 1;
    xshift = pattern.XOffset & 15;

    for (rows = 0; rows < H; rows++, uae_mem += ri.BytesPerRow) {
	unsigned long prow = (rows + pattern.YOffset) & ysize_mask;
	unsigned int d = do_get_mem_word (((uae_u16 *) pattern.Memory) + prow);
	uae_u8 *uae_mem2 = uae_mem;
	unsigned long cols;

	if (xshift != 0)
	    d = (d << xshift) | (d >> (16 - xshift));

	for (cols = 0; cols < W; cols += 16, uae_mem2 += Bpp << 4) {
	    long bits;
	    long max = W - cols;
	    unsigned int data = d;

	    if (max > 16)
		max = 16;

	    for (bits = 0; bits < max; bits++) {
		int bit_set = data & 0x8000;
		data <<= 1;
		switch (pattern.DrawMode) {
		case JAM1:
		    if (inversion)
			bit_set = !bit_set;
		    if (bit_set)
			PixelWrite (uae_mem2, bits, pattern.FgPen, Bpp, Mask);
		    break;
		case JAM2:
		    if (inversion)
			bit_set = !bit_set;
		    if (bit_set)
			PixelWrite (uae_mem2, bits, pattern.FgPen, Bpp, Mask);
		    else
			PixelWrite (uae_mem2, bits, pattern.BgPen, Bpp, Mask);
		    break;
		case COMP:
		    if (bit_set) {
			fgpen = pattern.FgPen;

			switch (Bpp) {
			case 1:
			    {
				uae_u8 *addr = uae_mem2 + bits;
				do_put_mem_byte (addr, do_get_mem_byte (addr) ^ fgpen);
			    }
			    break;
			case 2:
			    {
				uae_u16 *addr = ((uae_u16 *) uae_mem2) + bits;
				do_put_mem_word (addr, do_get_mem_word (addr) ^ fgpen);
			    }
			    break;
			case 3:
			    {
				uae_u32 *addr = (uae_u32 *) (uae_mem2 + bits * 3);
				do_put_mem_long (addr, do_get_mem_long (addr) ^ (fgpen & 0x00FFFFFF));
			    }
			    break;
			case 4:
			    {
				uae_u32 *addr = ((uae_u32 *) uae_mem2) + bits;
				do_put_mem_long (addr, do_get_mem_long (addr) ^ fgpen);
			    }
			    break;
			}
		    }
		    break;
		}
	    }
	}
    }

    frame_buffer_UAM = ri.Memory + X * picasso96_state.BytesPerPixel + Y * ri.BytesPerRow;
    if (renderinfo_is_current_screen (&ri))
	do_blit (frame_buffer_UAM, ri.BytesPerRow, 0, 0, X, Y, W, H, 0);

    return 1;
}

/*************************************************
BlitTemplate:
**************************************************
* Synopsis: BlitTemplate(bi, ri, template, X, Y, Width, Height, Mask, RGBFormat);
* a0: struct BoardInfo *bi
* a1: struct RenderInfo *ri
* a2: struct Template *template
* d0.w: X
* d1.w: Y
* d2.w: Width
* d3.w: Height
* d4.w: Mask
* d7.l: RGBFormat
*
* This function is used to paint a template on the board memory using the blitter.
* It is called by BltPattern and BltTemplate. The template consists of a b/w image
* using a single plane of image data which will be expanded to the destination RGBFormat
* using ForeGround and BackGround pens as well as draw modes.
***********************************************************************************/
uae_u32 picasso_BlitTemplate (void)
{
    uae_u8 inversion = 0;
    uaecptr rinf = m68k_areg (regs, 1);
    uaecptr tmpl = m68k_areg (regs, 2);
    unsigned long X = (uae_u16) m68k_dreg (regs, 0);
    unsigned long Y = (uae_u16) m68k_dreg (regs, 1);
    unsigned long W = (uae_u16) m68k_dreg (regs, 2);
    unsigned long H = (uae_u16) m68k_dreg (regs, 3);
    uae_u16 Mask = (uae_u16) m68k_dreg (regs, 4);
    struct Template tmp;
    struct RenderInfo ri;
    unsigned long rows;
    int bitoffset;
    uae_u32 fgpen;
    uae_u8 *uae_mem, Bpp, *frame_buffer_UAM;
    uae_u8 *tmpl_base;

    wgfx_flushline ();

    if (!CopyRenderInfoStructureA2U (rinf, &ri)
	|| !CopyTemplateStructureA2U (tmpl, &tmp))
	return 0;

    Bpp = GetBytesPerPixel (ri.RGBFormat);
    uae_mem = ri.Memory + Y * ri.BytesPerRow + X * Bpp;	/* offset into address */

    if (tmp.DrawMode & INVERS)
	inversion = 1;

    tmp.DrawMode &= 0x03;
    if (Mask != 0xFF) {
	if (Bpp > 1)
	    write_log ("ERROR - not obeying BlitTemplate() mask 0x%x properly with Bpp %d.\n", Mask, Bpp);
	else if (tmp.DrawMode == COMP) {
	    write_log ("ERROR - Unsupported Mask value 0x%x with COMP Draw in BlitTemplate(), using fallback method.\n", Mask);
	    return 0;
	}
    }

    /*write_log ("BlitTemplate() xy(%d,%d), wh(%d,%d) draw 0x%x fg 0x%x bg 0x%x \n",
       X, Y, W, H, tmp.DrawMode, tmp.FgPen, tmp.BgPen); */

    bitoffset = tmp.XOffset % 8;

#ifdef _DEBUG
    DumpTemplate (&tmp, W, H);
#endif

    tmpl_base = tmp.Memory + tmp.XOffset / 8;

    for (rows = 0; rows < H; rows++, uae_mem += ri.BytesPerRow, tmpl_base += tmp.BytesPerRow) {
	unsigned long cols;
	uae_u8 *tmpl_mem = tmpl_base;
	uae_u8 *uae_mem2 = uae_mem;
	unsigned int data = *tmpl_mem;

	for (cols = 0; cols < W; cols += 8, uae_mem2 += Bpp << 3) {
	    unsigned int byte;
	    long bits;
	    long max = W - cols;

	    if (max > 8)
		max = 8;

	    data <<= 8;
	    data |= *++tmpl_mem;

	    byte = data >> (8 - bitoffset);

	    for (bits = 0; bits < max; bits++) {
		int bit_set = (byte & 0x80);
		byte <<= 1;
		switch (tmp.DrawMode) {
		case JAM1:
		    if (inversion)
			bit_set = !bit_set;
		    if (bit_set) {
			fgpen = tmp.FgPen;
			PixelWrite (uae_mem2, bits, fgpen, Bpp, Mask);
		    }
		    break;
		case JAM2:
		    if (inversion)
			bit_set = !bit_set;
		    fgpen = tmp.BgPen;
		    if (bit_set)
			fgpen = tmp.FgPen;

		    PixelWrite (uae_mem2, bits, fgpen, Bpp, Mask);
		    break;
		case COMP:
		    if (bit_set) {
			fgpen = tmp.FgPen;

			switch (Bpp) {
			case 1:
			    {
				uae_u8 *addr = uae_mem2 + bits;
				do_put_mem_byte (addr, do_get_mem_byte (addr) ^ fgpen);
			    }
			    break;
			case 2:
			    {
				uae_u16 *addr = ((uae_u16 *) uae_mem2) + bits;
				do_put_mem_word (addr, do_get_mem_word (addr) ^ fgpen);
			    }
			    break;
			case 3:
			    {
				uae_u32 *addr = (uae_u32 *) (uae_mem2 + bits * 3);
				do_put_mem_long (addr, do_get_mem_long (addr) ^ (fgpen & 0x00FFFFFF));
			    }
			    break;
			case 4:
			    {
				uae_u32 *addr = ((uae_u32 *) uae_mem2) + bits;
				do_put_mem_long (addr, do_get_mem_long (addr) ^ fgpen);
			    }
			    break;
			}
		    }
		    break;
		}
	    }
	}
    }

    frame_buffer_UAM = ri.Memory + X * picasso96_state.BytesPerPixel + Y * ri.BytesPerRow;
    if (renderinfo_is_current_screen (&ri))
	do_blit (frame_buffer_UAM, ri.BytesPerRow, 0, 0, X, Y, W, H, 0);

    return 1;
}

/*
 * CalculateBytesPerRow:
 * a0: 	struct BoardInfo
 * d0: 	uae_u16 Width
 * d7:	RGBFTYPE RGBFormat
 * This function calculates the amount of bytes needed for a line of
 * "Width" pixels in the given RGBFormat.
 */
uae_u32 picasso_CalculateBytesPerRow (void)
{
    uae_u16 width = m68k_dreg (regs, 0);
    uae_u32 type = m68k_dreg (regs, 7);

    width = GetBytesPerPixel (type) * width;
    /*write_log ("CalculateBytesPerRow() = %d\n",width); */

    return width;
}

/*
 * SetDisplay:
 * a0:	struct BoardInfo
 * d0:	BOOL state
 * This function enables and disables the video display.
 * 
 * NOTE: return the opposite of the state
 */
uae_u32 picasso_SetDisplay (void)
{
    uae_u32 state = m68k_dreg (regs, 0);
    write_log ("SetDisplay(%d)\n", state);
    return !state;
}

/*
 * WaitVerticalSync:
 * a0:	struct BoardInfo
 * This function waits for the next horizontal retrace.
 */
uae_u32 picasso_WaitVerticalSync (void)
{
    /*write_log ("WaitVerticalSync()\n"); */
    return 1;
}

/* NOTE: Watch for those planeptrs of 0x00000000 and 0xFFFFFFFF for all zero / all one bitmaps !!!! */
static void PlanarToChunky (struct RenderInfo *ri, struct BitMap *bm,
			    unsigned long srcx, unsigned long srcy,
			    unsigned long dstx, unsigned long dsty, unsigned long width, unsigned long height, uae_u8 mask)
{
    int j;

    uae_u8 *PLANAR[8], *image = ri->Memory + dstx * GetBytesPerPixel (ri->RGBFormat) + dsty * ri->BytesPerRow;
    int Depth = bm->Depth;
    unsigned long rows, bitoffset = srcx & 7;
    long eol_offset;

    /* if (mask != 0xFF) 
       write_log ("P2C - pixel-width = %d, bit-offset = %d\n", width, bitoffset); */

    /* Set up our bm->Planes[] pointers to the right horizontal offset */
    for (j = 0; j < Depth; j++) {
	uae_u8 *p = bm->Planes[j];
	if (p != &all_zeros_bitmap && p != &all_ones_bitmap)
	    p += srcx / 8 + srcy * bm->BytesPerRow;
	PLANAR[j] = p;
	if ((mask & (1 << j)) == 0)
	    PLANAR[j] = &all_zeros_bitmap;
    }
    eol_offset = (long) bm->BytesPerRow - (long) ((width + 7) >> 3);
    for (rows = 0; rows < height; rows++, image += ri->BytesPerRow) {
	unsigned long cols;

	for (cols = 0; cols < width; cols += 8) {
	    int k;
	    uae_u32 a = 0, b = 0;
	    unsigned int msk = 0xFF;
	    long tmp = cols + 8 - width;
	    if (tmp > 0) {
		msk <<= tmp;
		b = do_get_mem_long ((uae_u32 *) (image + cols + 4));
		if (tmp < 4)
		    b &= 0xFFFFFFFF >> (32 - tmp * 8);
		else if (tmp > 4) {
		    a = do_get_mem_long ((uae_u32 *) (image + cols));
		    a &= 0xFFFFFFFF >> (64 - tmp * 8);
		}
	    }
	    for (k = 0; k < Depth; k++) {
		unsigned int data;
		if (PLANAR[k] == &all_zeros_bitmap)
		    data = 0;
		else if (PLANAR[k] == &all_ones_bitmap)
		    data = 0xFF;
		else {
		    data = (uae_u8) (do_get_mem_word ((uae_u16 *) PLANAR[k]) >> (8 - bitoffset));
		    PLANAR[k]++;
		}
		data &= msk;
		a |= p2ctab[data][0] << k;
		b |= p2ctab[data][1] << k;
	    }
	    do_put_mem_long ((uae_u32 *) (image + cols), a);
	    do_put_mem_long ((uae_u32 *) (image + cols + 4), b);
	}
	for (j = 0; j < Depth; j++) {
	    if (PLANAR[j] != &all_zeros_bitmap && PLANAR[j] != &all_ones_bitmap) {
		PLANAR[j] += eol_offset;
	    }
	}
    }
}

/*
 * BlitPlanar2Chunky:
 * a0: struct BoardInfo *bi
 * a1: struct BitMap *bm - source containing planar information and assorted details
 * a2: struct RenderInfo *ri - dest area and its details
 * d0.w: SrcX
 * d1.w: SrcY
 * d2.w: DstX
 * d3.w: DstY
 * d4.w: SizeX
 * d5.w: SizeY
 * d6.b: MinTerm - uh oh!
 * d7.b: Mask - uh oh!
 *
 * This function is currently used to blit from planar bitmaps within system memory to chunky bitmaps
 * on the board. Watch out for plane pointers that are 0x00000000 (represents a plane with all bits "0")
 * or 0xffffffff (represents a plane with all bits "1").
 */
uae_u32 picasso_BlitPlanar2Chunky (void)
{
    uaecptr bm = m68k_areg (regs, 1);
    uaecptr ri = m68k_areg (regs, 2);
    unsigned long srcx = (uae_u16) m68k_dreg (regs, 0);
    unsigned long srcy = (uae_u16) m68k_dreg (regs, 1);
    unsigned long dstx = (uae_u16) m68k_dreg (regs, 2);
    unsigned long dsty = (uae_u16) m68k_dreg (regs, 3);
    unsigned long width = (uae_u16) m68k_dreg (regs, 4);
    unsigned long height = (uae_u16) m68k_dreg (regs, 5);
    uae_u8 minterm = m68k_dreg (regs, 6) & 0xFF;
    uae_u8 mask = m68k_dreg (regs, 7) & 0xFF;
    struct RenderInfo local_ri;
    struct BitMap local_bm;

    wgfx_flushline ();

    if (minterm != 0x0C) {
	write_log ("ERROR - BlitPlanar2Chunky() has minterm 0x%x, which I don't handle. Using fall-back routine.\n", minterm);
	return 0;
    }
    if (!CopyRenderInfoStructureA2U (ri, &local_ri)
	|| !CopyBitMapStructureA2U (bm, &local_bm))
	return 0;

    /*write_log ("BlitPlanar2Chunky(%d, %d, %d, %d, %d, %d) Minterm 0x%x, Mask 0x%x, Depth %d\n",
       srcx, srcy, dstx, dsty, width, height, minterm, mask, local_bm.Depth);
       write_log ("P2C - BitMap has %d BPR, %d rows\n", local_bm.BytesPerRow, local_bm.Rows); */
    PlanarToChunky (&local_ri, &local_bm, srcx, srcy, dstx, dsty, width, height, mask);
    if (renderinfo_is_current_screen (&local_ri))
	do_blit (local_ri.Memory + dstx * GetBytesPerPixel (local_ri.RGBFormat) + dsty * local_ri.BytesPerRow,
		 local_ri.BytesPerRow, 0, 0, dstx, dsty, width, height, 0);

    return 1;
}

/* NOTE: Watch for those planeptrs of 0x00000000 and 0xFFFFFFFF for all zero / all one bitmaps !!!! */
static void PlanarToDirect (struct RenderInfo *ri, struct BitMap *bm,
			    unsigned long srcx, unsigned long srcy,
			    unsigned long dstx, unsigned long dsty,
			    unsigned long width, unsigned long height, uae_u8 mask, struct ColorIndexMapping *cim)
{
    int j;
    int bpp = GetBytesPerPixel (ri->RGBFormat);
    uae_u8 *PLANAR[8];
    uae_u8 *image = ri->Memory + dstx * bpp + dsty * ri->BytesPerRow;
    int Depth = bm->Depth;
    unsigned long rows;
    long eol_offset;

    /* Set up our bm->Planes[] pointers to the right horizontal offset */
    for (j = 0; j < Depth; j++) {
	uae_u8 *p = bm->Planes[j];
	if (p != &all_zeros_bitmap && p != &all_ones_bitmap)
	    p += srcx / 8 + srcy * bm->BytesPerRow;
	PLANAR[j] = p;
	if ((mask & (1 << j)) == 0)
	    PLANAR[j] = &all_zeros_bitmap;
    }

    eol_offset = (long) bm->BytesPerRow - (long) ((width + (srcx & 7)) >> 3);
    for (rows = 0; rows < height; rows++, image += ri->BytesPerRow) {
	unsigned long cols;
	uae_u8 *image2 = image;
	unsigned int bitoffs = 7 - (srcx & 7);
	int i;

	for (cols = 0; cols < width; cols++) {
	    int v = 0, k;
	    for (k = 0; k < Depth; k++) {
		if (PLANAR[k] == &all_ones_bitmap)
		    v |= 1 << k;
		else if (PLANAR[k] != &all_zeros_bitmap) {
		    v |= ((*PLANAR[k] >> bitoffs) & 1) << k;
		}
	    }

	    switch (bpp) {
	    case 2:
		do_put_mem_word ((uae_u16 *) image2, cim->Colors[v]);
		image2 += 2;
		break;
	    case 3:
		do_put_mem_byte (image2++, cim->Colors[v] & 0x000000FF);
		do_put_mem_word ((uae_u16 *) image2, (cim->Colors[v] & 0x00FFFF00) >> 8);
		image2 += 2;
		break;
	    case 4:
		do_put_mem_long ((uae_u32 *) image2, cim->Colors[v]);
		image2 += 4;
		break;
	    }
	    bitoffs--;
	    bitoffs &= 7;
	    if (bitoffs == 7) {
		int k;
		for (k = 0; k < Depth; k++) {
		    if (PLANAR[k] != &all_zeros_bitmap && PLANAR[k] != &all_ones_bitmap) {
			PLANAR[k]++;
		    }
		}
	    }
	}

	for (i = 0; i < Depth; i++) {
	    if (PLANAR[i] != &all_zeros_bitmap && PLANAR[i] != &all_ones_bitmap) {
		PLANAR[i] += eol_offset;
	    }
	}
    }
}

/*
 * BlitPlanar2Direct: 
 * 
 * Synopsis:
 * BlitPlanar2Direct(bi, bm, ri, cim, SrcX, SrcY, DstX, DstY, SizeX, SizeY, MinTerm, Mask);
 * Inputs:
 * a0:struct BoardInfo *bi
 * a1:struct BitMap *bm
 * a2:struct RenderInfo *ri
 * a3:struct ColorIndexMapping *cmi
 * d0.w:SrcX
 * d1.w:SrcY
 * d2.w:DstX
 * d3.w:DstY
 * d4.w:SizeX
 * d5.w:SizeY
 * d6.b:MinTerm
 * d7.b:Mask
 * 
 * This function is currently used to blit from planar bitmaps within system memory to direct color
 * bitmaps (15, 16, 24 or 32 bit) on the board. Watch out for plane pointers that are 0x00000000 (represents
 * a plane with all bits "0") or 0xffffffff (represents a plane with all bits "1"). The ColorIndexMapping is
 * used to map the color index of each pixel formed by the bits in the bitmap's planes to a direct color value
 * which is written to the destination RenderInfo. The color mask and all colors within the mapping are words,
 * triple bytes or longwords respectively similar to the color values used in FillRect(), BlitPattern() or
 * BlitTemplate(). 
 */
uae_u32 picasso_BlitPlanar2Direct (void)
{
    uaecptr bm = m68k_areg (regs, 1);
    uaecptr ri = m68k_areg (regs, 2);
    uaecptr cim = m68k_areg (regs, 3);
    unsigned long srcx = (uae_u16) m68k_dreg (regs, 0);
    unsigned long srcy = (uae_u16) m68k_dreg (regs, 1);
    unsigned long dstx = (uae_u16) m68k_dreg (regs, 2);
    unsigned long dsty = (uae_u16) m68k_dreg (regs, 3);
    unsigned long width = (uae_u16) m68k_dreg (regs, 4);
    unsigned long height = (uae_u16) m68k_dreg (regs, 5);
    uae_u8 minterm = m68k_dreg (regs, 6);
    uae_u8 Mask = m68k_dreg (regs, 7);
    struct RenderInfo local_ri;
    struct BitMap local_bm;
    struct ColorIndexMapping local_cim;

    wgfx_flushline ();

    if (minterm != 0x0C) {
	write_log ("ERROR - BlitPlanar2Direct() has op-code 0x%x, which I don't handle. Using fall-back routine.\n", minterm);
	return 0;
    }
    if (Mask != 0xFF) {
	write_log ("ERROR - Unsupported Mask value 0x%x in BlitPlanar2Direct(), using fallback method.\n", Mask);
	return 0;
    }
    if (!CopyRenderInfoStructureA2U (ri, &local_ri)
	|| !CopyBitMapStructureA2U (bm, &local_bm))
	return 0;

    CopyColorIndexMappingA2U (cim, &local_cim);
    /* write_log ("BlitPlanar2Direct(%d, %d, %d, %d, %d, %d) Minterm 0x%x, Mask 0x%x, Depth %d\n",
       srcx, srcy, dstx, dsty, width, height, minterm, Mask, local_bm.Depth); */
    PlanarToDirect (&local_ri, &local_bm, srcx, srcy, dstx, dsty, width, height, Mask, &local_cim);
    if (renderinfo_is_current_screen (&local_ri))
	do_blit (local_ri.Memory + dstx * GetBytesPerPixel (local_ri.RGBFormat) + dsty * local_ri.BytesPerRow,
		 local_ri.BytesPerRow, 0, 0, dstx, dsty, width, height, 0);
    return 1;
}

/* @@@ - Work to be done here!
 *
 * The address is the offset into our Picasso96 frame-buffer (pointed to by gfxmem_start)
 * where the value was put.
 *
 * Porting work: on some machines you may not need these functions, ie. if the memory for the
 * Picasso96 frame-buffer is directly viewable or directly blittable.  On Win32 with DirectX,
 * this is not the case.  So I provide some write-through functions (as per Mathias' orders!)
 */
static void write_gfx_long (uaecptr addr, uae_u32 value)
{
    uaecptr oldaddr = addr;
    int x, xbytes, y;
    uae_u8 *dst;

    if (!picasso_on)
	return;

    /*
     * Several writes to successive memory locations are a common access pattern.
     * Try to optimize it.
     */
    if (addr >= wgfx_linestart && addr + 4 <= wgfx_lineend) {
	if (addr < wgfx_min)
	    wgfx_min = addr;
	if (addr + 4 > wgfx_max)
	    wgfx_max = addr + 4;
	return;
    } else
	wgfx_flushline ();

    addr += gfxmem_start;
    /* Check to see if this needs to be written through to the display, or was it an "offscreen" area? */
    if (addr < picasso96_state.Address || addr + 4 > picasso96_state.Extent)
	return;

    addr -= picasso96_state.Address;
    y = addr / picasso96_state.BytesPerRow;

#if 0
    DX_Invalidate (y, y);
    if (!picasso_vidinfo.extra_mem)
	return;

    xbytes = addr - y * picasso96_state.BytesPerRow;
    x = xbytes / picasso96_state.BytesPerPixel;

    if (x < picasso96_state.Width && y < picasso96_state.Height) {
	dst = gfx_lock_picasso ();
	if (dst) {
	    do_put_mem_long ((uae_u32 *) (dst + y * picasso_vidinfo.rowbytes + xbytes), value);
	    gfx_unlock_picasso ();
	}
    }
#else
    if (y >= picasso96_state.Height)
	return;
    wgfx_linestart = picasso96_state.Address - gfxmem_start + y * picasso96_state.BytesPerRow;
    wgfx_lineend = wgfx_linestart + picasso96_state.BytesPerRow;
    wgfx_y = y;
    wgfx_min = oldaddr;
    wgfx_max = oldaddr + 4;
#endif
}

static void write_gfx_word (uaecptr addr, uae_u16 value)
{
    uaecptr oldaddr = addr;
    int x, xbytes, y;
    uae_u8 *dst;

    if (!picasso_on)
	return;

    /*
     * Several writes to successive memory locations are a common access pattern.
     * Try to optimize it.
     */
    if (addr >= wgfx_linestart && addr + 2 <= wgfx_lineend) {
	if (addr < wgfx_min)
	    wgfx_min = addr;
	if (addr + 2 > wgfx_max)
	    wgfx_max = addr + 2;
	return;
    } else
	wgfx_flushline ();

    addr += gfxmem_start;
    /* Check to see if this needs to be written through to the display, or was it an "offscreen" area? */
    if (addr < picasso96_state.Address || addr + 2 > picasso96_state.Extent)
	return;

    addr -= picasso96_state.Address;
    y = addr / picasso96_state.BytesPerRow;

#if 0
    DX_Invalidate (y, y);
    if (!picasso_vidinfo.extra_mem)
	return;

    xbytes = addr - y * picasso96_state.BytesPerRow;
    x = xbytes / picasso96_state.BytesPerPixel;

    if (x < picasso96_state.Width && y < picasso96_state.Height) {
	dst = gfx_lock_picasso ();
	if (dst) {
	    do_put_mem_word ((uae_u16 *) (dst + y * picasso_vidinfo.rowbytes + xbytes), value);
	    gfx_unlock_picasso ();
	}
    }
#else
    if (y >= picasso96_state.Height)
	return;
    wgfx_linestart = picasso96_state.Address - gfxmem_start + y * picasso96_state.BytesPerRow;
    wgfx_lineend = wgfx_linestart + picasso96_state.BytesPerRow;
    wgfx_y = y;
    wgfx_min = oldaddr;
    wgfx_max = oldaddr + 2;
#endif
}

static void write_gfx_byte (uaecptr addr, uae_u8 value)
{
    uaecptr oldaddr = addr;
    int x, xbytes, y;
    uae_u8 *dst;

    if (!picasso_on)
	return;

    /*
     * Several writes to successive memory locations are a common access pattern.
     * Try to optimize it.
     */
    if (addr >= wgfx_linestart && addr + 4 <= wgfx_lineend) {
	if (addr < wgfx_min)
	    wgfx_min = addr;
	if (addr + 1 > wgfx_max)
	    wgfx_max = addr + 1;
	return;
    } else
	wgfx_flushline ();

    addr += gfxmem_start;
    /* Check to see if this needs to be written through to the display, or was it an "offscreen" area? */
    if (addr < picasso96_state.Address || addr + 1 > picasso96_state.Extent)
	return;

    addr -= picasso96_state.Address;
    y = addr / picasso96_state.BytesPerRow;

#if 0
    DX_Invalidate (y, y);
    if (!picasso_vidinfo.extra_mem)
	return;

    xbytes = addr - y * picasso96_state.BytesPerRow;
    x = xbytes / picasso96_state.BytesPerPixel;

    if (x < picasso96_state.Width && y < picasso96_state.Height) {
	dst = gfx_lock_picasso ();
	if (dst) {
	    *(uae_u8 *) (dst + y * picasso_vidinfo.rowbytes + xbytes) = value;
	    gfx_unlock_picasso ();
	}
    }
#else
    if (y >= picasso96_state.Height)
	return;
    wgfx_linestart = picasso96_state.Address - gfxmem_start + y * picasso96_state.BytesPerRow;
    wgfx_lineend = wgfx_linestart + picasso96_state.BytesPerRow;
    wgfx_y = y;
    wgfx_min = oldaddr;
    wgfx_max = oldaddr + 1;
#endif
}

static uae_u32 REGPARAM2 gfxmem_lget (uaecptr addr)
{
    uae_u32 *m;
    addr -= gfxmem_start & gfxmem_mask;
    addr &= gfxmem_mask;
    m = (uae_u32 *) (gfxmemory + addr);
    return do_get_mem_long (m);
}

static uae_u32 REGPARAM2 gfxmem_wget (uaecptr addr)
{
    uae_u16 *m;
    addr -= gfxmem_start & gfxmem_mask;
    addr &= gfxmem_mask;
    m = (uae_u16 *) (gfxmemory + addr);
    return do_get_mem_word (m);
}

static uae_u32 REGPARAM2 gfxmem_bget (uaecptr addr)
{
    addr -= gfxmem_start & gfxmem_mask;
    addr &= gfxmem_mask;
    return gfxmemory[addr];
}

static void REGPARAM2 gfxmem_lput (uaecptr addr, uae_u32 l)
{
    uae_u32 *m;
    addr -= gfxmem_start & gfxmem_mask;
    addr &= gfxmem_mask;
    m = (uae_u32 *) (gfxmemory + addr);
    do_put_mem_long (m, l);

    /* write the long-word to our displayable memory */
    write_gfx_long (addr, l);
}

static void REGPARAM2 gfxmem_wput (uaecptr addr, uae_u32 w)
{
    uae_u16 *m;
    addr -= gfxmem_start & gfxmem_mask;
    addr &= gfxmem_mask;
    m = (uae_u16 *) (gfxmemory + addr);
    do_put_mem_word (m, (uae_u16) w);

    /* write the word to our displayable memory */
    write_gfx_word (addr, (uae_u16) w);
}

static void REGPARAM2 gfxmem_bput (uaecptr addr, uae_u32 b)
{
    addr -= gfxmem_start & gfxmem_mask;
    addr &= gfxmem_mask;
    gfxmemory[addr] = b;

    /* write the byte to our displayable memory */
    write_gfx_byte (addr, (uae_u8) b);
}

static int REGPARAM2 gfxmem_check (uaecptr addr, uae_u32 size)
{
    addr -= gfxmem_start & gfxmem_mask;
    addr &= gfxmem_mask;
    return (addr + size) < allocated_gfxmem;
}

static uae_u8 REGPARAM2 *gfxmem_xlate (uaecptr addr)
{
    addr -= gfxmem_start & gfxmem_mask;
    addr &= gfxmem_mask;
    return gfxmemory + addr;
}

addrbank gfxmem_bank = {
    gfxmem_lget, gfxmem_wget, gfxmem_bget,
    gfxmem_lput, gfxmem_wput, gfxmem_bput,
    gfxmem_xlate, gfxmem_check, NULL
};

int picasso_display_mode_index (uae_u32 x, uae_u32 y, uae_u32 d)
{
    int i;
    for (i = 0; i < mode_count; i++) {
        if (DisplayModes[i].res.width == x
	    && DisplayModes[i].res.height == y
	    && DisplayModes[i].depth == d)
            break;
    }
    if (i == mode_count)
        i = -1;
    return i;
}

static int resolution_compare (const void *a, const void *b)
{
    struct PicassoResolution *ma = (struct PicassoResolution *) a;
    struct PicassoResolution *mb = (struct PicassoResolution *) b;
    if (ma->res.width > mb->res.width)
	return -1;
    if (ma->res.width < mb->res.width)
	return 1;
    if (ma->res.height > mb->res.height)
	return -1;
    if (ma->res.height < mb->res.height)
	return 1;
    return ma->depth - mb->depth;
}

/* Call this function first, near the beginning of code flow
 * NOTE: Don't stuff it in InitGraphics() which seems reasonable...
 * Instead, put it in customreset() for safe-keeping.  */
void InitPicasso96 (void)
{
    static int first_time = 1;

    memset (&picasso96_state, 0, sizeof (struct picasso96_state_struct));

    if (first_time) {
	int i;

	for (i = 0; i < 256; i++) {
	    p2ctab[i][0] = (((i & 128) ? 0x01000000 : 0)
			    | ((i & 64) ? 0x010000 : 0)
			    | ((i & 32) ? 0x0100 : 0)
			    | ((i & 16) ? 0x01 : 0));
	    p2ctab[i][1] = (((i & 8) ? 0x01000000 : 0)
			    | ((i & 4) ? 0x010000 : 0)
			    | ((i & 2) ? 0x0100 : 0)
			    | ((i & 1) ? 0x01 : 0));
	}
	mode_count = DX_FillResolutions (&picasso96_pixel_format);
	qsort (DisplayModes, mode_count, sizeof (struct PicassoResolution), resolution_compare);

	for (i = 0; i < mode_count; i++) {
	    sprintf (DisplayModes[i].name, "%dx%d, %d-bit, %d Hz",
		     DisplayModes[i].res.width, DisplayModes[i].res.height, DisplayModes[i].depth * 8, DisplayModes[i].refresh);
	    switch (DisplayModes[i].depth) {
	    case 1:
		if (DisplayModes[i].res.width > chunky.width)
		    chunky.width = DisplayModes[i].res.width;
		if (DisplayModes[i].res.height > chunky.height)
		    chunky.height = DisplayModes[i].res.height;
		break;
	    case 2:
		if (DisplayModes[i].res.width > hicolour.width)
		    hicolour.width = DisplayModes[i].res.width;
		if (DisplayModes[i].res.height > hicolour.height)
		    hicolour.height = DisplayModes[i].res.height;
		break;
	    case 3:
		if (DisplayModes[i].res.width > truecolour.width)
		    truecolour.width = DisplayModes[i].res.width;
		if (DisplayModes[i].res.height > truecolour.height)
		    truecolour.height = DisplayModes[i].res.height;
		break;
	    case 4:
		if (DisplayModes[i].res.width > alphacolour.width)
		    alphacolour.width = DisplayModes[i].res.width;
		if (DisplayModes[i].res.height > alphacolour.height)
		    alphacolour.height = DisplayModes[i].res.height;
		break;
	    }
	}
	ShowSupportedResolutions ();

	first_time = 0;
    }
}

#endif

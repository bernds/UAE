 /*
  * UAE - The Un*x Amiga Emulator
  *
  * DOS video interface.
  *
  * (c) 1997 Gustavo Goedert
  *
  * Originaly based on svga.c -- (c) 1995 Bernd Schmidt
  */

#include "sysconfig.h"
#include "sysdeps.h"

#include <dos.h>
#include <go32.h>
#include <dpmi.h>
#include <sys/exceptn.h>
#include <crt0.h>
#include <signal.h>

#include "config.h"
#include "options.h"
#include "threaddep/thread.h"
#include "memory.h"
#include "custom.h"
#include "xwin.h"
#include "disk.h"
#include "uae.h"
#include "debug.h"
#include "picasso96.h"
#include "misc/handlers.h"
#include "misc/misc.h"
#include "video/vbe.h"

int GetColor(int r, int g, int b, xcolnr *cnp);
void InitColors(void);
void SetWindowForPicasso(void);
void SetWindowForAmiga(void);
uae_u8 *lockscr (void);
void unlockscr (void);

extern int debugging;

xcolnr xcolors[4096];
struct vidbuf_description gfxvidinfo;
int NeedDither;
UBYTE DitherBuf[1000];
int ColorsAllocated;
char *LinearMem = NULL;
int ModeNumber = -1, UseLinear = 0;
int PicassoModeNumber = -1, PicassoUseLinear = 0;
int ScreenIsPicasso = 0;
char PicassoInvalidLines[1200];

struct bstring *video_mode_menu = NULL;

void setup_brkhandler(void) {
}

void flush_line(int y) {
    int target_y = y;
    char *addr;

    if (debugging)
	return;
    if (LinearMem == NULL) {
	addr = gfxvidinfo.bufmem + y*gfxvidinfo.rowbytes;
	if (target_y < CurrentMode.ModeHeight && target_y >= 0) {
	    if (NeedDither) {
		DitherLine(DitherBuf, (UWORD *)addr, 0, y, gfxvidinfo.width, CurrentMode.BitsPerPixel);
		addr = DitherBuf;
	    }
	    CurrentMode.PutLine(addr, target_y);
	}
    } else if (NeedDither && target_y >= 0) {
	addr = gfxvidinfo.bufmem + y*gfxvidinfo.rowbytes;
	DitherLine(LinearMem + CurrentMode.LogicalLineLength * target_y, (UWORD *)addr, 0, y, gfxvidinfo.width, CurrentMode.BitsPerPixel);
    }
}

void flush_block(int a, int b) {
    abort();
}

void flush_screen(int a, int b) {
}

int GetColor(int r, int g, int b, xcolnr *cnp) {
    if ((CurrentMode.ModeType == T16) && (ColorsAllocated == 16))
	return -1;
    else if (ColorsAllocated == 256)
	return -1;
    *cnp = ColorsAllocated;
    DelayedSetPalette(ColorsAllocated, doMask(r, 6, 0), doMask(g, 6, 0), doMask(b, 6, 0));
    ColorsAllocated++;
    return 1;
}

void InitColors(void) {
    int i;

    gfxvidinfo.can_double = 0;
    if (NeedDither)
	setup_dither(CurrentMode.BitsPerColor, GetColor);
    else {
	ColorsAllocated = 0;
	if (gfxvidinfo.pixbytes == 1)
	    alloc_colors256(GetColor);
	else
	    alloc_colors64k(CurrentMode.RedSize,
			    CurrentMode.GreenSize,
			    CurrentMode.BlueSize,
			    CurrentMode.RedPosition,
			    CurrentMode.GreenPosition,
			    CurrentMode.BluePosition);
	if (gfxvidinfo.pixbytes == 1) {
	    for (i = 0; i < 4096; i++)
		xcolors[i] = xcolors[i] * 0x01010101;
	    gfxvidinfo.can_double = 1;
	} else if (gfxvidinfo.pixbytes == 2) {
	    for (i = 0; i < 4096; i++)
		xcolors[i] = xcolors[i] * 0x00010001;
	    gfxvidinfo.can_double = 1;
	}
   }
}

struct gfxstring *gfxmenu;
int numgfxitems = 0;

struct gfxstring{
    char data[32];
    int val;
};

int gfxmenusort(const void *a, const void *b) {
   struct gfxstring *ga, *gb;

   ga = (struct gfxstring *) a;
   gb = (struct gfxstring *) b;
   if (ModeList[ga->val].ModeWidth < ModeList[gb->val].ModeWidth)
      return(-1);
   else if (ModeList[ga->val].ModeWidth > ModeList[gb->val].ModeWidth)
      return(1);
   else if (ModeList[ga->val].ModeHeight < ModeList[gb->val].ModeHeight)
      return(-1);
   else if (ModeList[ga->val].ModeHeight > ModeList[gb->val].ModeHeight)
      return(1);
   else if (ModeList[ga->val].ModeType < ModeList[gb->val].ModeType)
      return(-1);
   else if (ModeList[ga->val].ModeType > ModeList[gb->val].ModeType)
      return(1);
   return(0);
}

int graphics_setup(void) {
    int i, j, hasres;
    struct gfxstring gfxmenuitem;

    gfxmenu = (struct gfxstring *)calloc(NumberOfModes, sizeof(struct gfxstring));

    for (i = 0; i < NumberOfModes; i++) {
	hasres = 0;
	for (j = 0; j < numgfxitems; j++)
	    if ((ModeList[gfxmenu[j].val].ModeType == ModeList[i].ModeType) &&
		(ModeList[gfxmenu[j].val].ModeWidth == ModeList[i].ModeWidth) &&
		(ModeList[gfxmenu[j].val].ModeHeight == ModeList[i].ModeHeight))
		hasres = 1;
	if (!hasres) {
	    sprintf(gfxmenu[numgfxitems].data, "%dx%d ", ModeList[i].ModeWidth, ModeList[i].ModeHeight);
	    switch (ModeList[i].ModeType) {
		case T16:
		    strcat(gfxmenu[numgfxitems].data, "16 colors");
		    break;
		case T256:
		    strcat(gfxmenu[numgfxitems].data, "256 colors");
		    break;
		case T32K:
		    strcat(gfxmenu[numgfxitems].data, "32k colors");
		    break;
		case T64K:
		    strcat(gfxmenu[numgfxitems].data, "64k colors");
		    break;
		case T16M:
		    strcat(gfxmenu[numgfxitems].data, "16M colors");
		    break;
	    }
	    gfxmenu[numgfxitems].val = i;
	    numgfxitems++;
	}
    }
    qsort((void *)gfxmenu, numgfxitems, sizeof(struct gfxstring), gfxmenusort);

    video_mode_menu = (struct bstring *)malloc(sizeof (struct bstring)*(numgfxitems+1));
    for (i = 0; i < numgfxitems; i++) {
	video_mode_menu[i].val = -1;
	video_mode_menu[i].data = strdup(gfxmenu[i].data);
    }
    video_mode_menu[numgfxitems].val = -3;
    video_mode_menu[numgfxitems].data = NULL;

    return 1;
}

void vidmode_menu_selected(int a) {
    currprefs.gfx_width = ModeList[gfxmenu[a].val].ModeWidth;
    currprefs.gfx_height = ModeList[gfxmenu[a].val].ModeHeight;
    switch (ModeList[gfxmenu[a].val].ModeType) {
	case T16:
	    currprefs.color_mode = 4;
	    break;
	case T256:
	    currprefs.color_mode = 0;
	    break;
	case T32K:
	    currprefs.color_mode = 1;
	    break;
	case T64K:
	    currprefs.color_mode = 2;
	    break;
	case T16M:
	    currprefs.color_mode = 5;
	    break;
    }
    currprefs.gfx_lores=currprefs.gfx_width<640;
    currprefs.gfx_xcenter=2;
    currprefs.gfx_ycenter=2;
    currprefs.gfx_linedbl=0;
    if (currprefs.gfx_height > 285)
	currprefs.gfx_linedbl = 1;
    currprefs.gfx_correct_aspect = 1;
    if (currprefs.gfx_height > 570)
	currprefs.gfx_correct_aspect = 0;
}

int graphics_init(void) {
    T_ModeType WantedType;
    int i;

    /* save produce_sound state */
    Original_produce_sound = currprefs.produce_sound;

    switch (currprefs.color_mode) {
	case 0: WantedType=T256; NeedDither=0; break;
	case 1: WantedType=T32K; NeedDither=0; break;
	case 2: WantedType=T64K; NeedDither=0; break;
	case 3: WantedType=T256; NeedDither=1; break;
	case 4: WantedType=T16;  NeedDither=1; break;
	case 5: WantedType=T16M; NeedDither=0; break;
	default:
	printf("Invalid color mode.\n");
	return 0;
    }

    if (!InitGfxLib()) {
	printf("Can't init GfxLib.\n");
	return 0;
    }

    if (!currprefs.no_xhair)
	for (i = 0; i < NumberOfModes; i++) {
	    if ((ModeList[i].ModeType == WantedType) &&
		(ModeList[i].ModeWidth == currprefs.gfx_width) &&
		(ModeList[i].ModeHeight == currprefs.gfx_height) &&
		 ModeList[i].HasLinear) {
		ModeNumber = i;
		UseLinear = 1;
		break;
	    }
	}
    if (ModeNumber == -1)
	for (i = 0; i < NumberOfModes; i++) {
	    if ((ModeList[i].ModeType == WantedType) &&
		(ModeList[i].ModeWidth == currprefs.gfx_width) &&
		(ModeList[i].ModeHeight == currprefs.gfx_height)) {
		if (ModeList[i].HasWindow || ((!ModeList[i].HasLinear) && (!ModeList[i].HasWindow))) {
		    ModeNumber = i;
		    break;
		}
	    }
	}

    if (ModeNumber == -1) {
	printf("Sorry, this combination of color and video mode is not supported.\n");
	return 0;
    }

    if (!SetMode(ModeNumber, UseLinear, 1)) {
	printf("Can't start graphics mode.\n");
	return 0;
    }

    if (!NeedDither)
	gfxvidinfo.pixbytes = CurrentMode.BitsPerPixel>>3;
    else
	gfxvidinfo.pixbytes = 2;

    if (UseLinear) {
	printf("Using linear framebuffer.\n");
	if (CanMapBuffer) {
	    LinearMem = CurrentMode.MappedAddress;
	    if (LinearMem != NULL)
		printf("Mapping linear framebuffer into emulator memory.\n");
	    else {
		printf("Can't map linear framebuffer.\n");
		return 0;
	    }
	}
    }

    gfxvidinfo.maxblocklines = 0;

    gfxvidinfo.width = CurrentMode.ModeWidth;
    gfxvidinfo.height = CurrentMode.ModeHeight;
    gfxvidinfo.linemem = 0;

    InitColors();
    if ((gfxvidinfo.pixbytes == 1) || NeedDither)
	LoadPalette();

    if (UseLinear && CanMapBuffer && (!NeedDither) && (LinearMem != NULL)) {
	gfxvidinfo.bufmem = LinearMem;
	gfxvidinfo.rowbytes = CurrentMode.LogicalLineLength;
    } else {
	gfxvidinfo.rowbytes = (CurrentMode.ModeWidth * gfxvidinfo.pixbytes + 3) & ~3;
	gfxvidinfo.bufmem = malloc(gfxvidinfo.rowbytes * CurrentMode.ModeHeight);
	if (gfxvidinfo.bufmem == NULL) {
	    printf("Can't allocate framebuffer.\n");
	    return 0;
	}
    }

    memset(gfxvidinfo.bufmem, 0, gfxvidinfo.rowbytes * CurrentMode.ModeHeight);
    for(i=0; i<gfxvidinfo.height; i++)
	flush_line(i);

    /* set up handlers */
    if (!InstallHandlers()) {
	printf("Can't install all handlers.\n");
	return(0);
    }

    return 1;
}

void graphics_leave(void) {
    UninstallHandlers();
    CloseGfxLib();
    dumpcustom();
    /* restore produce_sound state */
    currprefs.produce_sound = Original_produce_sound;
}

int debuggable(void) {
    return(1);
}

int needmousehack(void) {
    return(0);
}

void write_log(const char *buf) {
    printf(buf);
}

#ifdef PICASSO96

void PicassoRefresh(void) {
    if (ScreenIsPicasso && !picasso_vidinfo.extra_mem) {
	int i;
	char *addr = gfxmemory + (picasso96_state.Address - gfxmem_start);
	for (i = 0; i < picasso_vidinfo.height; i++, addr += picasso96_state.BytesPerRow) {
	    if (!PicassoInvalidLines[i])
		continue;
	    PicassoInvalidLines[i] = 0;
	    CurrentMode.PutLine(addr, i);
	}
    }
}

void DX_Invalidate (int first, int last) {
    if (!picasso_vidinfo.extra_mem) {
	do {
	    PicassoInvalidLines[first] = 1;
	    first++;
	} while (first <= last);
    }
}

int DX_BitsPerCannon (void) {
    return 8;
}

void DX_SetPalette(int start, int count) {
    if (!ScreenIsPicasso || picasso_vidinfo.pixbytes != 1)
	return;

    while (count-- > 0) {
	DelayedSetPalette(start, picasso96_state.CLUT[start].Red * 63 / 255,
				 picasso96_state.CLUT[start].Green * 63 / 255,
				 picasso96_state.CLUT[start].Blue * 63 / 255);
	start++;
    }
    LoadPalette();
}

int DX_FillResolutions (uae_u16 *ppixel_format) {
    int i, count = 0;
    uae_u16 format = 0;

    for (i = 0; i < NumberOfModes; i++) {
	if (ModeList[i].ModeType != T16) {
	    DisplayModes[count].res.width = ModeList[i].ModeWidth;
	    DisplayModes[count].res.height = ModeList[i].ModeHeight;
	    DisplayModes[count].depth = ModeList[i].BitsPerPixel>>3;
	    DisplayModes[count].refresh = 75;
	    switch (DisplayModes[count].depth) {
		case 1: format |= RGBFF_CHUNKY; break;
		case 2: format |= RGBFF_R5G6B5PC; break;
		case 3: format |= RGBFF_B8G8R8; break;
		case 4: format |= RGBFF_B8G8R8A8; break;
	    }
	    count++;
	}
    }

    *ppixel_format = format;
    return count;
}

void SetWindowForPicasso(void) {
    if (!SetMode(PicassoModeNumber, PicassoUseLinear, 0)) {
	printf("Can't start graphics mode.\n");
	abort();
    }
    if (PicassoUseLinear && (CurrentMode.MappedAddress != NULL)) {
	LinearMem = CurrentMode.MappedAddress;
	picasso_vidinfo.extra_mem = 1;
    } else
	picasso_vidinfo.extra_mem = 0;
    DX_SetPalette (0, 256);
}

void SetWindowForAmiga(void) {
    if (!SetMode(ModeNumber, UseLinear, 0)) {
	printf("Can't start graphics mode.\n");
	abort();
    }
    if (UseLinear && (CurrentMode.MappedAddress != NULL) && (!NeedDither)) {
	LinearMem = CurrentMode.MappedAddress;
	gfxvidinfo.bufmem = LinearMem;
    }
    if ((gfxvidinfo.pixbytes == 1) || NeedDither)
	LoadPalette();
}

void gfx_set_picasso_state (int on) {
    if (on == ScreenIsPicasso)
	return;
    ScreenIsPicasso = on;
    if (on)
	SetWindowForPicasso();
    else
	SetWindowForAmiga();
}

void gfx_set_picasso_modeinfo (int w, int h, int depth) {
    T_ModeType WantedType;
    int i;

    PicassoModeNumber = -1;
    PicassoUseLinear = 0;
    switch (depth) {
	case 8: WantedType=T256; break;
	case 16: WantedType=T64K; break;
	case 24:
	case 32: WantedType=T16M; break;
	default:
	printf("Invalid Picasso mode.\n");
	abort();
    }

    if (!currprefs.no_xhair)
	for (i = 0; i < NumberOfModes; i++) {
	    if ((ModeList[i].ModeType == WantedType) &&
		(ModeList[i].ModeWidth == w) &&
		(ModeList[i].ModeHeight == h) &&
		 ModeList[i].HasLinear) {
		PicassoModeNumber = i;
		PicassoUseLinear = 1;
		break;
	    }
	}
    if (PicassoModeNumber == -1)
	for (i = 0; i < NumberOfModes; i++) {
	    if ((ModeList[i].ModeType == WantedType) &&
		(ModeList[i].ModeWidth == w) &&
		(ModeList[i].ModeHeight == h)) {
		if (ModeList[i].HasWindow || ((!ModeList[i].HasLinear) && (!ModeList[i].HasWindow))) {
		    PicassoModeNumber = i;
		    break;
		}
	    }
	}

    if (PicassoModeNumber == -1) {
	printf("Invalid Picasso mode.\n");
	abort ();
    }

    picasso_vidinfo.width = w;
    picasso_vidinfo.height = h;
    picasso_vidinfo.depth = depth;
    picasso_vidinfo.pixbytes = depth>>3;
    picasso_vidinfo.rowbytes = ModeList[PicassoModeNumber].LogicalLineLength;
    if (ScreenIsPicasso)
	SetWindowForPicasso ();
}

uae_u8 *lockscr (void) {
    return LinearMem;
}

void unlockscr (void) {
}

#endif

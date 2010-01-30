 /*
  * UAE - The Un*x Amiga Emulator
  *
  * GGI - Interface
  *
  * Always remember: "GGI - The right thing to do[tm]"
  *
  * Copyright 1997 Christian Schmitt <schmitt@freiburg.linux.de>,
  *                Stefan Reinauer <stepan@freiburg.linux.de>
  */

#include "sysconfig.h"
#include "sysdeps.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <ggi/libggi.h>
#include <ggi/libggi_db.h>
#include <ggi/events.h>
#include <string.h>

#include "config.h"
#include "options.h"
#include "threaddep/penguin.h"
#include "uae.h"
#include "memory.h"
#include "custom.h"
#include "readcpu.h"
#include "newcpu.h"
#include "keyboard.h"
#include "xwin.h"
#include "keybuf.h"
#include "gui.h"
#include "picasso96.h"

static ggi_visual_t vis;
static ggi_graphtype amiga_modetype;
static unsigned char pack[256];
static uae_u8 dither_buf[1000];
int buttonstate[3] = {0, 0, 0};
int lastmx,lastmy,newmousecounters=0;
static int keystate[256];

int need_dither,bpp;
void *linear=NULL;

static int screen_is_picasso;
static char picasso_invalid_lines[1200];
static ggi_graphtype picasso_modetype;

struct bstring *video_mode_menu = NULL;

/* We don't need a brkhandler with GGI */
void setup_brkhandler(void)
{
}

#define MAX_SCREEN_MODES 6
#define MAX_GGI_COLOR_MODES 4

int x_size_table[MAX_SCREEN_MODES]={320,640,640,800,1024,1152};
int y_size_table[MAX_SCREEN_MODES]={200,400,480,600,768,864};
ggi_graphtype color_table[MAX_GGI_COLOR_MODES]=
  {GT_4BIT,GT_8BIT,GT_16BIT,GT_32BIT};

static struct ggi_graphmodes{
  int x;
  int y;
  int col;
  struct ggi_graphmodes *next;
} got_modes;


int graphics_setup(void)
{
    struct ggi_graphmodes *dummy;
    int count=0;
    char buf[80];
    int i,j,c=0;

    ggiInit();

    if((vis=ggiOpen(NULL))==NULL) {
	fprintf(stderr,"Can't open Visual\n");
	return 0;
    }
    
    for(i=0;i<MAX_SCREEN_MODES;i++) {
	for(j=0;j<MAX_GGI_COLOR_MODES;j++) {
	    if (ggiCheckGraphMode(vis, x_size_table[i], y_size_table[i],
				  x_size_table[i], y_size_table[i],
				  color_table[j],NULL,NULL) != 0)
	      continue;

	    dummy=(struct ggi_graphmodes *)malloc(sizeof(struct ggi_graphmodes));
	    dummy->x=x_size_table[i];
	    dummy->y=y_size_table[i];
	    switch(color_table[j]) {
	     case GT_4BIT  : c=4;break;
	     case GT_8BIT  : c=8;break;
	     case GT_16BIT : c=16;break;
	     case GT_32BIT : c=32;break;
	     default       : break; /* Ups */
	    }
	    dummy->col=c;
	    dummy->next=got_modes.next;
	    got_modes.next=dummy;
	    count++;
	}
    }
    count++;
    video_mode_menu = (struct bstring *)malloc(sizeof (struct bstring)*count);
    memset(video_mode_menu, 0, sizeof (struct bstring)*count);
    dummy=got_modes.next;
    for (i = 0; i < count - 1; i++) {
	sprintf(buf, "%3dx%d, %2d Bit", dummy->x, dummy->y,dummy->col);
	video_mode_menu[i].val = -1;
	video_mode_menu[i].data = strdup(buf);
	dummy = dummy->next;
    }
    video_mode_menu[count-1].val = -3;
    video_mode_menu[count-1].data = NULL;
    return 1;
}

void vidmode_menu_selected(int m)
{
    int i;
    struct ggi_graphmodes *dummy=got_modes.next;
    for(i=1;i<=m;i++) dummy=dummy->next;
    currprefs.gfx_width = dummy->x;
    currprefs.gfx_height = dummy->y;
    currprefs.color_mode = dummy->col == 32 ? 5 : dummy->col == 16 ? 2 : 0;
}

static int colors_allocated;
static ggi_color map[256];
static ggi_color vga_map[256];

static void restore_vga_colors (void)
{
    int i;
    if (gfxvidinfo.pixbytes != 1)
	return;
    memcpy (map, vga_map, sizeof map);
    ggiSetPaletteVec(vis, 0, colors_allocated, map);
}

static int get_color(int r, int g, int b, xcolnr *cnp)
{
    if(colors_allocated == 255) 
	return -1;
    *cnp = colors_allocated;
    map[colors_allocated].r = doMask (r,16,0);
    map[colors_allocated].g = doMask (g,16,0);
    map[colors_allocated].b = doMask (b,16,0);

    colors_allocated++;
    return 1;
}

static void init_colors(void)
{
    if (need_dither) {
	setup_dither (8, get_color);
    } else {
	int i;
	colors_allocated = 0;
	gfxvidinfo.can_double = 0;
	switch (bpp){
	 case 8:
	    alloc_colors256 (get_color);
	    ggiSetPaletteVec (vis, 0, colors_allocated, map);
	    memcpy (vga_map, map, sizeof vga_map);
	    for (i = 0; i < 4096;i++)
		xcolors[i] *= 0x01010101;
	    gfxvidinfo.can_double=1;
	    break;
	 case 16:
	    alloc_colors64k (5, 6, 5, 11, 5, 0);
	    for(i = 0; i < 4096; i++)
		xcolors[i] *= 0x00010001;
	    gfxvidinfo.can_double=1;
	    break;
	 case 32:
	    alloc_colors64k(8, 8, 8, 16, 8, 0);
	    break;
	 default:
	    break;
	}
    }
}

static int set_amiga_mode (void)
{
    int sx = gfxvidinfo.width;
    int sy = gfxvidinfo.height;
    ggi_directbuffer_t gbuf=NULL;
    ggi_pixellinearbuffer *plb;   

    if (ggiSetGraphMode (vis, sx, sy, sx, sy, amiga_modetype) != 0) {
	fprintf(stderr,"Can't set graphics mode\n");
	return 0;
    }
    ggiDBGetBuffer(vis,&gbuf);
    plb=ggiDBGetPLB(gbuf);
    linear = plb->write;
    return 1;
}

int graphics_init (void) /* See how simple it is? :-> */
{
    int err,i;

    switch(currprefs.color_mode) {
     case  0:
	amiga_modetype = GT_8BIT;
	need_dither = 0;
	break;
     case  1:
     case  2:
	amiga_modetype = GT_16BIT;
	need_dither = 0;
	break;
     case  3:
	amiga_modetype = GT_8BIT;
	need_dither = 1;
	break;
     case  4:
	amiga_modetype = GT_4BIT;
	need_dither = 1;
	break;
     default:
	fprintf(stderr, "Bad color mode selected. Using default.\n");
	currprefs.color_mode = 0;
	amiga_modetype = GT_8BIT;
	break;
    }

    gfxvidinfo.width = currprefs.gfx_width;
    gfxvidinfo.height = currprefs.gfx_height;
    
    if (! set_amiga_mode ())
	return 0;

    bpp = ggiGetBPP (vis);

    gfxvidinfo.pixbytes = bpp >> 3;
    gfxvidinfo.rowbytes = gfxvidinfo.pixbytes * currprefs.gfx_width;
    if (linear != NULL)
	gfxvidinfo.bufmem = linear;
    else
	gfxvidinfo.bufmem = (char *)malloc (gfxvidinfo.rowbytes * currprefs.gfx_height);
    gfxvidinfo.maxblocklines = 0;
    memset (gfxvidinfo.bufmem, 0, gfxvidinfo.rowbytes * currprefs.gfx_height);
    
    screen_is_picasso = 0;
    
    for (i = 0; i < 256; i++) {
       map[i].r=i;
       map[i].g=i;
       map[i].b=i;
    }
    ggiPackColors(vis, pack, map, 256);
    init_colors ();

    buttonstate[0] = buttonstate[1] = buttonstate[2] = 0;
    for (i = 0; i < 256; i++)
	keystate[i] = 0;
    lastmx = lastmy = 0;
    return 1;
}

void graphics_leave(void)
{
    ggiClose (vis);
    ggiExit ();
}

void flush_line(int y)
{
    int target_y = y;
    int bit_unit=8;
    int i=0;
    char *addr;

    if (linear != NULL && !need_dither)
	return;

    addr = gfxvidinfo.bufmem + y*gfxvidinfo.rowbytes;
    if (target_y < 400 && target_y >= 0) {
	if (need_dither) {
	    if (linear == NULL){
		DitherLine (dither_buf, (uae_u16 *)addr, 0, y,
			    gfxvidinfo.width, bit_unit);
		addr = dither_buf;
		ggiPutHLine (vis, 0, target_y, gfxvidinfo.width, addr);
	    } else {
		DitherLine (linear + gfxvidinfo.rowbytes*y, (uae_u16 *)addr, 0, y,
			    gfxvidinfo.width, bit_unit);
	    }
	    return;
	}
	if(linear==NULL)
	    ggiPutHLine(vis, 0, target_y, gfxvidinfo.width, addr);
    }
}

void flush_block(int a, int b){};
void flush_screen(int a, int b){};

#define KEY_NOTHING 0x00
#define AK_QUIT 0xf00

/* We should not need to have different versions for different keyboard */
static short decodekey[128]={
   KEY_NOTHING,	/* 0 */
   AK_ESC,
   AK_1, AK_2, AK_3, AK_4, AK_5, AK_6, AK_7, AK_8, AK_9, AK_0,
   AK_MINUS,	/* 12 */
   AK_EQUAL,
   AK_BS,
   AK_TAB,	/* 0x0f */
   AK_Q, AK_W, AK_E, AK_R, AK_T,
   AK_Y,	/* 0x15  Attention: language specific !!!! */
   AK_U, AK_I, AK_O, AK_P, AK_LBRACKET,
   AK_RBRACKET,	/* 0x1b */
   AK_RET,
   AK_CTRL,
   AK_A, AK_S, AK_D, AK_F, AK_G, AK_H, AK_J, AK_K, AK_L,
   AK_SEMICOLON,	/* 0x27 */
   AK_QUOTE,	/* &auml; 0x28 */
   KEY_NOTHING,
   AK_LSH,	/* 0x2a */
   AK_LTGT,	/* < 0x2b Attention: language specific !!!! */
   AK_Z,	/* 0x2c Attention: language specific !!!! */
   AK_X, AK_C, AK_V, AK_B, AK_N, AK_M, AK_COMMA, AK_PERIOD,
   AK_SLASH,	/* 0x35 */
   AK_RSH,	/* 0x36 */
   AK_NPMUL,
   AK_LALT,
   AK_SPC,
   AK_CAPSLOCK,	/* 0x3a */
   AK_F1, AK_F2, AK_F3, AK_F4, AK_F5, AK_F6, AK_F7, AK_F8, AK_F9,
   AK_F10,	/* 0x44 */
   KEY_NOTHING,	/* NumLock */
   KEY_NOTHING,	/* "Rollen" */
   AK_NP7,	/* 0x47 */
   AK_NP8,
   AK_NP9,
   AK_NPSUB,	/* 0x4a */
   AK_NP4,
   AK_NP5,
   AK_NP6,
   AK_NPADD,	/* 0x4e */
   AK_NP1,
   AK_NP2,
   AK_NP3,
   AK_NP0,	/* 0x52 */
   AK_NPDEL,
   KEY_NOTHING,	/* ??? */
   KEY_NOTHING,	/* ??? */
   KEY_NOTHING,	/* ??? */
   AK_BS,	/* 0x57 --> F11 */
   AK_QUIT,	/* 0x58 --> F12 */
   KEY_NOTHING,	/* ??? */
   KEY_NOTHING,	/* ??? */
   KEY_NOTHING,	/* ??? */
   KEY_NOTHING,	/* ??? */
   KEY_NOTHING,	/* ??? */
   KEY_NOTHING,	/* ??? */
   KEY_NOTHING,	/* ??? */
   AK_ENT,	/* 0x60 */
   AK_RCTRL,	/* 0x61 */
   AK_NPDIV,
   KEY_NOTHING,
   AK_RALT,	/* 0x64 */
   KEY_NOTHING,	/* ??? */
   AK_NP7,	/* Pos1 */
   AK_UP,	/* 0x67 */
   AK_LAMI,	/* 0x68  --> PgUp, */
   AK_LF,	/* 0x69 */
   AK_RT,	/* 0x6a */
   AK_NP1,	/* End */
   AK_DN,	/* 0x6c */
   AK_RAMI,	/* --> PgDown */
   AK_HELP,	/* 0x6e --> insert */
   AK_DEL,
   KEY_NOTHING,
   KEY_NOTHING,
   KEY_NOTHING,
   KEY_NOTHING,
   KEY_NOTHING,
   KEY_NOTHING,
   KEY_NOTHING,
   KEY_NOTHING,	/* 0x77 --> Pause */
   KEY_NOTHING,
   KEY_NOTHING,
   KEY_NOTHING,
   KEY_NOTHING,
   KEY_NOTHING,
   AK_LAMI,	/* 0x7d	--> WindOoze left  */
   AK_RAMI,	/*	--> WindOoze right */
   AK_CTRL	/* 0x7f	--> WindOoze Menu  */
};

void handle_events (void)
{
    struct timeval t={0,1};
    ggi_event ev;
    ggi_event_mask mask;
    int akey;
    uint32 state=0,button=0;
    while(ggiEventPoll(vis,emPtrButton|emPtrRelative|emKey,&t)) {
	ggiEventRead(vis,&ev,emPtrButton|emPtrRelative|emKey);
	switch(ev.any.type){
         case evPtrButtonPress:
	 case evPtrButtonRelease: 
	    state=ev.pbutton.state;
	    button=ev.pbutton.button;
	    switch(button) {
	     case 1 : buttonstate[0]=state&button;break;
	     case 2 : buttonstate[2]=(state&button)>>1;break;
	     case 4 : buttonstate[1]=(state&button)>>2;break;
	     default: break;
	    }
	    break;
	 case evPtrRelative:
	    lastmx+=ev.pmove.x;
	    lastmy+=ev.pmove.y;
	    break;
 	 case evKeyPress:
	 case evKeyRelease: 
	    if(ev.key.sym==0) continue;
	    if(ev.key.code>128) return;
	    akey=decodekey[ev.key.code];
	    if(akey==0xf00) uae_quit();
	    if(keystate[AK_CTRL] && keystate[AK_LAMI] && keystate[AK_RAMI])
		m68k_reset();
	    switch(ev.key.type) {
	     case 6            : keystate[akey]=0;
		record_key((akey << 1) | 1);
		break;
	     case 5            : if(akey==KEY_NOTHING) break;
		if(!keystate[akey]) {
		    keystate[akey]=1;
		    record_key(akey << 1);
		}
		break;
	     default           : break;
	    }
	  default:  
/*	    printf("Ugh\n");*/
	}
    }
    
    

#ifdef PICASSO96
    if (screen_is_picasso && !picasso_vidinfo.extra_mem) {
	int i;
	char *addr = gfxmemory + (picasso96_state.Address - gfxmem_start);
	for (i = 0; i < picasso_vidinfo.height; i++, addr += picasso96_state.BytesPerRow) {
	    if (!picasso_invalid_lines[i])
		continue;
	    picasso_invalid_lines[i] = 0;
	    ggiPutHLine (vis, 0, i, gfxvidinfo.width, addr);
	}
    }
#endif

    return;
}

int check_prefs_changed_gfx (void)
{
    return 0;
}

int debuggable(void)
{
    return 0;
}

int needmousehack(void)
{
    return 0;
}

/* This is not implemented yet as GGI is not that far */
void LED(int on)
{
}

#ifdef PICASSO96

void DX_Invalidate (int first, int last)
{
    do {
	picasso_invalid_lines[first] = 1;
	first++;
    } while (first <= last);
}

int DX_BitsPerCannon (void)
{
    return 8;
}

void DX_SetPalette(int start, int count)
{
    int i;

    if (!screen_is_picasso || picasso_vidinfo.pixbytes != 1)
	return;

    fprintf (stderr, "DX_SetPalette\n");

    for (i = 0; i < count; i++) {
	map[start + i].r = picasso96_state.CLUT[start + i].Red * 0x0101;
	map[start + i].g = picasso96_state.CLUT[start + i].Green * 0x0101;
	map[start + i].b = picasso96_state.CLUT[start + i].Blue * 0x0101;
    }
    ggiSetPaletteVec (vis, start, count, map + start);
}

int DX_FillResolutions (uae_u16 *ppixel_format)
{
    int i, count = 0;
    uae_u16 format = 0;

    for (i = 0; i < MAX_SCREEN_MODES; i++) {
	if (ggiCheckGraphMode (vis, x_size_table[i], y_size_table[i],
			       x_size_table[i], y_size_table[i],
			       GT_8BIT,NULL,NULL) == 0) {
	    DisplayModes[count].res.width = x_size_table[i];
	    DisplayModes[count].res.height = y_size_table[i];
	    DisplayModes[count].depth = 1;
	    DisplayModes[count].refresh = 75;
	    count++;
	    format |= RGBFF_CHUNKY;
	}
	if (ggiCheckGraphMode (vis, x_size_table[i], y_size_table[i],
			       x_size_table[i], y_size_table[i],
			       GT_16BIT,NULL,NULL) == 0) {
	    DisplayModes[count].res.width = x_size_table[i];
	    DisplayModes[count].res.height = y_size_table[i];
	    DisplayModes[count].depth = 2;
	    DisplayModes[count].refresh = 75;
	    count++;
	    format |= RGBFF_R5G6B5PC;
	}
	if (ggiCheckGraphMode (vis, x_size_table[i], y_size_table[i],
			       x_size_table[i], y_size_table[i],
			       GT_32BIT,NULL,NULL) == 0) {
	    DisplayModes[count].res.width = x_size_table[i];
	    DisplayModes[count].res.height = y_size_table[i];
	    DisplayModes[count].depth = 4;
	    DisplayModes[count].refresh = 75;
	    count++;
	    format |= RGBFF_B8G8R8A8;
	}
    }

    *ppixel_format = format;
    return count;
}

static void set_window_for_picasso (void)
{
    int sx, sy;
    ggi_directbuffer_t gbuf=NULL;
    ggi_pixellinearbuffer *plb;
    
    sx = picasso_vidinfo.width;
    sy = picasso_vidinfo.height;
    fprintf (stderr, "picasso mode %d %d %d!\n", sx, sy, picasso_modetype);
    if (ggiSetGraphMode (vis, sx, sy, sx, sy, picasso_modetype) != 0) {
	fprintf (stderr,"Can't set graphics mode\n");
	abort ();
    }
    fprintf (stderr, "picasso mode ok!\n");

    ggiDBGetBuffer(vis,&gbuf);
    plb=ggiDBGetPLB(gbuf);
    linear = plb->write;

    if (linear != NULL)
	picasso_vidinfo.extra_mem = 1;
    else
	picasso_vidinfo.extra_mem = 0;
    fprintf (stderr, "em: %d\n", picasso_vidinfo.extra_mem);
    DX_SetPalette (0, 256);
}

static void set_window_for_amiga (void)
{
    set_amiga_mode ();
    if (linear != NULL && !need_dither)
	gfxvidinfo.bufmem = linear;

    restore_vga_colors ();
}

void gfx_set_picasso_modeinfo (int w, int h, int depth)
{
    fprintf (stderr, "set_picasso_modeinfo\n");
    picasso_vidinfo.width = w;
    picasso_vidinfo.height = h;
    picasso_vidinfo.depth = depth;
    picasso_vidinfo.pixbytes = depth >> 3;
    picasso_vidinfo.rowbytes = w * picasso_vidinfo.pixbytes;
    picasso_modetype = (depth == 8 ? GT_8BIT
			: depth == 16 ? GT_16BIT
			: GT_32BIT);
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
    fprintf (stderr, "set_picasso_state\n");
    screen_is_picasso = on;
    if (on)
	set_window_for_picasso ();
    else
	set_window_for_amiga ();
}

uae_u8 *gfx_lock_picasso (void)
{
    return linear;
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




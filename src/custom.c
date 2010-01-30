 /*
  * UAE - The Un*x Amiga Emulator
  *
  * Custom chip emulation
  *
  * Copyright 1995-1998 Bernd Schmidt
  * Copyright 1995 Alessandro Bissacco
  */

#include "sysconfig.h"
#include "sysdeps.h"

#include <ctype.h>
#include <assert.h>

#include "config.h"
#include "options.h"
#include "threaddep/penguin.h"
#include "uae.h"
#include "gensound.h"
#include "sounddep/sound.h"
#include "events.h"
#include "memory.h"
#include "custom.h"
#include "readcpu.h"
#include "newcpu.h"
#include "cia.h"
#include "disk.h"
#include "blitter.h"
#include "xwin.h"
#include "joystick.h"
#include "audio.h"
#include "keybuf.h"
#include "serial.h"
#include "osemu.h"
#include "autoconf.h"
#include "gui.h"
#include "picasso96.h"
#include "drawing.h"

static unsigned int n_consecutive_skipped = 0;
static unsigned int total_skipped = 0;

#define SPRITE_COLLISIONS

/* Mouse and joystick emulation */

int buttonstate[3];
static int mouse_x, mouse_y;
int joy0button, joy1button;
unsigned int joy0dir, joy1dir;

/* Events */

unsigned long int cycles, nextevent, is_lastline;
static int rpt_did_reset;
struct ev eventtab[ev_max];

frame_time_t vsynctime, vsyncmintime;

static int vpos;
static uae_u16 lof;
static int next_lineno;
static enum nln_how nextline_how;
static int lof_changed = 0;

static const int dskdelay = 2; /* FIXME: ??? */

static uae_u32 sprtaba[256],sprtabb[256];

/*
 * Hardware registers of all sorts.
 */

static void custom_wput_1 (int, uaecptr, uae_u32) REGPARAM;

static uae_u16 cregs[256];

uae_u16 intena,intreq;
uae_u16 dmacon;
uae_u16 adkcon; /* used by audio code */

static uae_u32 cop1lc,cop2lc,copcon;
 
int maxhpos = MAXHPOS_PAL;
int maxvpos = MAXVPOS_PAL;
int minfirstline = MINFIRSTLINE_PAL;
int vblank_endline = VBLANK_ENDLINE_PAL;
int vblank_hz = VBLANK_HZ_PAL;
unsigned long syncbase;
static int fmode;
static unsigned int beamcon0, new_beamcon0;
static int ntscmode = 0;

#define MAX_SPRITES 32

/* This is but an educated guess. It seems to be correct, but this stuff
 * isn't documented well. */
enum sprstate { SPR_stop, SPR_restart, SPR_waiting_start, SPR_waiting_stop };
static enum sprstate sprst[8];
static int spron[8];
static uaecptr sprpt[8];
static int sprxpos[8], sprvstart[8], sprvstop[8];

static unsigned int sprdata[MAX_SPRITES], sprdatb[MAX_SPRITES], sprctl[MAX_SPRITES], sprpos[MAX_SPRITES];
static int sprarmed[MAX_SPRITES], sprite_last_drawn_at[MAX_SPRITES];
static int last_sprite_point, nr_armed;

static uae_u32 bpl1dat, bpl2dat, bpl3dat, bpl4dat, bpl5dat, bpl6dat, bpl7dat, bpl8dat;
static uae_s16 bpl1mod, bpl2mod;

static uaecptr bplpt[8];
#ifndef SMART_UPDATE
static char *real_bplpt[8];
#endif

/*static int blitcount[256];  blitter debug */

static struct color_entry current_colors;
static unsigned int bplcon0, bplcon1, bplcon2, bplcon3, bplcon4;
static int nr_planes_from_bplcon0, corrected_nr_planes_from_bplcon0;
static unsigned int diwstrt, diwstop, diwhigh;
static int diwhigh_written;
static unsigned int ddfstrt, ddfstop;

static uae_u32 dskpt;
static uae_u16 dsklen, dsksync;
static int dsklength, syncfound;

/* The display and data fetch windows */

enum diw_states
{
    DIW_waiting_start, DIW_waiting_stop
};

static int plffirstline, plflastline, plfstrt, plfstop, plflinelen;
int diwfirstword, diwlastword;
static enum diw_states diwstate, hdiwstate;

/* Sprite collisions */
uae_u16 clxdat, clxcon;
int clx_sprmask;

enum copper_states {
    COP_stop,
    COP_rdelay1, COP_read1, COP_read2,
    COP_bltwait,
    COP_wait1, COP_wait1b, COP_wait2,
};

struct copper {
    /* The current instruction words.  */
    unsigned int i1, i2;
    enum copper_states state;
    /* Instruction pointer.  */
    uaecptr ip;
    int hpos, vpos, count;
    unsigned int ignore_next;
    unsigned int do_move;
    enum diw_states vdiw;
};

static struct copper cop_state;

static void prepare_copper_1 (void);

static int dskdmaen;

/*
 * Statistics
 */

/* Used also by bebox.cpp */
unsigned long int msecs = 0, frametime = 0, lastframetime = 0, timeframes = 0;
static unsigned long int seconds_base;
int bogusframe;


static int current_change_set;

#ifdef OS_WITHOUT_MEMORY_MANAGEMENT
/* sam: Those arrays uses around 7Mb of BSS... That seems  */
/* too much for AmigaDOS (uae crashes as soon as one loads */
/* it. So I use a different strategy here (realloc the     */
/* arrays when needed. That strategy might be usefull for  */
/* computer with low memory.                               */
struct sprite_draw  *sprite_positions[2];
struct color_change *color_changes[2];
struct delay_change *delay_changes;
static int max_sprite_draw = 400;
static int delta_sprite_draw = 0;
static int max_color_change = 400;
static int delta_color_change = 0;
static int max_delay_change = 100;
static int delta_delay_change = 0;
#else
struct sprite_draw sprite_positions[2][MAX_REG_CHANGE];
struct color_change color_changes[2][MAX_REG_CHANGE];
/* We don't remember those across frames, that would be too much effort.
 * We simply redraw the line whenever we see one of these. */
struct delay_change delay_changes[MAX_REG_CHANGE];
#endif

struct decision line_decisions[2 * (MAXVPOS + 1) + 1];
struct draw_info line_drawinfo[2][2 * (MAXVPOS + 1) + 1];
struct color_entry color_tables[2][(MAXVPOS + 1) * 2];

struct sprite_draw *curr_sprite_positions, *prev_sprite_positions;
struct color_change *curr_color_changes, *prev_color_changes;
struct draw_info *curr_drawinfo, *prev_drawinfo;
struct color_entry *curr_color_tables, *prev_color_tables;

static int next_color_change, next_sprite_draw, next_delay_change;
static int next_color_entry, remembered_color_entry;
static int color_src_match, color_dest_match, color_compare_result;

/* These few are only needed during/at the end of the scanline, and don't
 * have to be remembered. */
static int decided_bpl1mod, decided_bpl2mod, decided_nr_planes, decided_res;

static char thisline_changed;


#ifdef SMART_UPDATE
#define MARK_LINE_CHANGED do { thisline_changed = 1; } while (0)
#else
#define MARK_LINE_CHANGED do { ; } while (0)
#endif

static struct decision thisline_decision;
static int modulos_added, plane_decided, color_decided, very_broken_program;

/*
 * helper functions
 */

int rpt_available = 0;

void reset_frame_rate_hack (void)
{
    if (currprefs.m68k_speed != -1)
	return;

    if (! rpt_available) {
	currprefs.m68k_speed = 0;
	return;
    }

    rpt_did_reset = 1;
    is_lastline = 0;
    vsyncmintime = read_processor_time() + vsynctime;
    write_log ("Resetting frame rate hack\n");
}

STATIC_INLINE void prepare_copper (void)
{
    if (cop_state.vpos > vpos
	|| cop_state.state == COP_stop)
    {
	eventtab[ev_copper].active = 0;
	return;
    }
    prepare_copper_1 ();
}

void check_prefs_changed_custom (void)
{
    currprefs.gfx_framerate = changed_prefs.gfx_framerate;
    /* Not really the right place... */
    if (currprefs.jport0 != changed_prefs.jport0
	|| currprefs.jport1 != changed_prefs.jport1) {
	currprefs.jport0 = changed_prefs.jport0;
	currprefs.jport1 = changed_prefs.jport1;
	joystick_setting_changed ();
    }
    currprefs.immediate_blits = changed_prefs.immediate_blits;
    currprefs.blits_32bit_enabled = changed_prefs.blits_32bit_enabled;
	
}

STATIC_INLINE void setclr (uae_u16 *p, uae_u16 val)
{
    if (val & 0x8000)
	*p |= val & 0x7FFF;
    else
	*p &= ~val;
}

__inline__ int current_hpos (void)
{
    return cycles - eventtab[ev_hsync].oldcycles;
}

STATIC_INLINE uae_u8 *pfield_xlateptr (uaecptr plpt, int bytecount)
{
    if (!chipmem_bank.check (plpt, bytecount)) {
	static int count = 0;
	if (!count)
	    count++, write_log ("Warning: Bad playfield pointer\n");
	return NULL;
    }
    return chipmem_bank.xlateaddr (plpt);
}

STATIC_INLINE void docols (struct color_entry *colentry)
{
#if AGA_CHIPSET == 0
    int i;
    for (i = 0; i < 32; i++) {
	int v = colentry->color_regs[i];
	if (v < 0 || v > 4095)
	  continue;
	colentry->acolors[i] = xcolors[v];
    }
#endif
}

void notice_new_xcolors (void)
{
    int i;

    docols(&current_colors);
/*    docols(&colors_for_drawing);*/
    for (i = 0; i < (MAXVPOS + 1)*2; i++) {
	docols(color_tables[0]+i);
	docols(color_tables[1]+i);
    }
}

static void do_sprites (int currvp, int currhp);

static void remember_ctable (void)
{
    if (remembered_color_entry == -1) {
	/* The colors changed since we last recorded a color map. Record a
	 * new one. */
	memcpy (curr_color_tables + next_color_entry, &current_colors, sizeof current_colors);
	remembered_color_entry = next_color_entry++;
    }
    thisline_decision.ctable = remembered_color_entry;
    if (color_src_match == -1 || color_dest_match != remembered_color_entry
	|| line_decisions[next_lineno].ctable != color_src_match)
    {
	/* The remembered comparison didn't help us - need to compare again. */
	int oldctable = line_decisions[next_lineno].ctable;
	int changed = 0;

	if (oldctable == -1) {
	    changed = 1;
	    color_src_match = color_dest_match = -1;
	} else {
	    color_compare_result = memcmp (&prev_color_tables[oldctable].color_regs,
					   &current_colors.color_regs,
					   sizeof current_colors.color_regs) != 0;
	    if (color_compare_result)
		changed = 1;
	    color_src_match = oldctable;
	    color_dest_match = remembered_color_entry;
	}
	thisline_changed |= changed;
    } else {
	/* We know the result of the comparison */
	if (color_compare_result)
	    thisline_changed = 1;
    }
}

static void remember_ctable_for_border (void)
{
    remember_ctable ();
}

/* Called to determine the state of the horizontal display window state
 * machine at the current position. It might have changed since we last
 * checked.  */
static void decide_diw (int hpos)
{
    if (hdiwstate == DIW_waiting_start && thisline_decision.diwfirstword == -1
	&& PIXEL_XPOS (hpos) >= diwfirstword)
    {
	thisline_decision.diwfirstword = diwfirstword;
	hdiwstate = DIW_waiting_stop;
	/* Decide playfield delays only at DIW start, because they don't matter before and
	 * some programs change them after DDF start but before DIW start. */
	thisline_decision.bplcon1 = bplcon1;
	if (thisline_decision.diwfirstword != line_decisions[next_lineno].diwfirstword)
	    MARK_LINE_CHANGED;
	thisline_decision.diwlastword = -1;
    }
    if (hdiwstate == DIW_waiting_stop && thisline_decision.diwlastword == -1
	&& PIXEL_XPOS (hpos) >= diwlastword)
    {
	thisline_decision.diwlastword = diwlastword;
	hdiwstate = DIW_waiting_start;
	if (thisline_decision.diwlastword != line_decisions[next_lineno].diwlastword)
	    MARK_LINE_CHANGED;
    }
}

/* Called when we know that the line is not in the border and we want to draw
 * it.  The data fetch starting position and length are passed as parameters. */
STATIC_INLINE void decide_as_playfield (int startpos, int len)
{
    thisline_decision.which = 1;

    /* The latter condition might be able to happen in interlaced frames. */
    if (vpos >= minfirstline && (thisframe_first_drawn_line == -1 || vpos < thisframe_first_drawn_line))
	thisframe_first_drawn_line = vpos;
    thisframe_last_drawn_line = vpos;

    thisline_decision.plfstrt = startpos;
    thisline_decision.plflinelen = len;

    /* These are for comparison. */
    thisline_decision.bplcon0 = bplcon0;
    thisline_decision.bplcon2 = bplcon2;
#if AGA_CHIPSET == 1
    thisline_decision.bplcon4 = bplcon4;
#endif

#ifdef SMART_UPDATE
    if (line_decisions[next_lineno].plfstrt != thisline_decision.plfstrt
	|| line_decisions[next_lineno].plflinelen != thisline_decision.plflinelen
	|| line_decisions[next_lineno].bplcon0 != thisline_decision.bplcon0
	|| line_decisions[next_lineno].bplcon2 != thisline_decision.bplcon2
#if AGA_CHIPSET == 1
	|| line_decisions[next_lineno].bplcon4 != thisline_decision.bplcon4
#endif
	)
#endif /* SMART_UPDATE */
	thisline_changed = 1;
}

/* Called when we already decided whether the line is playfield or border,
 * but are no longer sure that this was such a good idea. This can make a
 * difference only for very badly written software.
 * 
 * We try to handle two cases:
 * a) bitplane DMA gets turned off during a line which was decided as plane
 * b) bitplane DMA turns on for all or some planes, either after the line was
 *    decided as border, or after we made an incorrect nr_planes decision.
 * 
 * Examples of programs which need this: Majic 12 "Ray of Hope 2" demo
 * (final large scrolltext) and Masterblazer (menu screen).
 * 
 * There's no hope of catching all cases; we'd need a cycle based emulation
 * for that. I'm getting increasingly convinced that that would be a good
 * idea.
 */
static int broken_plane_sub[8];

static void init_broken_program (void)
{
    int i;
    if (very_broken_program)
	return;
    very_broken_program = 1;
    for (i = 0; i < 8; i++)
	broken_plane_sub[i] = i >= decided_nr_planes || thisline_decision.which != 1 ? -1 : 0;
}

static void adjust_broken_program (int hpos)
{
    int tmp1, tmp2;
    int i;
    if (decided_res == RES_HIRES) {
	tmp1 = hpos & 3;
	tmp2 = hpos & ~3;
    } else if (decided_res == RES_SUPERHIRES) {
	tmp1 = hpos & 1;
	tmp2 = hpos & ~1;
    } else {
	tmp1 = hpos & 7;
	tmp2 = hpos & ~7;
    }
    for (i = 0; i < decided_nr_planes; i++) {
	if (broken_plane_sub[i] != -1)
	    continue;
	/* FIXME: superhires, AGA playfields */
	/* Actually, I don't want to support this crap for AGA if we can avoid it.  */
	if (decided_res == RES_HIRES) {
	    broken_plane_sub[i] = (tmp2 - thisline_decision.plfstrt) / 4 * 2;
	    switch (i) {
	     case 3: broken_plane_sub[i] += 2; break; /* @@@ break was missing */
	     case 2: broken_plane_sub[i] += (tmp1 >= 3 ? 2 : 0); break;
	     case 1: broken_plane_sub[i] += (tmp1 >= 2 ? 2 : 0); break;
	     case 0: break;
	    }
	} else {
	    broken_plane_sub[i] = (tmp2 - thisline_decision.plfstrt) / 8 * 2;
	    switch (i) {
	     case 5: broken_plane_sub[i] += (tmp1 >= 3 ? 2 : 0); break;
	     case 4: broken_plane_sub[i] += (tmp1 >= 7 ? 2 : 0); break;
	     case 3: broken_plane_sub[i] += (tmp1 >= 2 ? 2 : 0); break;
	     case 2: broken_plane_sub[i] += (tmp1 >= 6 ? 2 : 0); break;
	     case 1: broken_plane_sub[i] += (tmp1 >= 4 ? 2 : 0); break;
	     case 0: break;
	    }
	}
    }
}

STATIC_INLINE void set_decided_res (void)
{
    decided_res = RES_LORES;
    if (bplcon0 & 0x8000)
	decided_res = RES_HIRES;
    if (bplcon0 & 0x40)
	decided_res = RES_SUPERHIRES;
}

STATIC_INLINE void post_decide_line (int hpos)
{
    if (thisline_decision.which == 1
	&& hpos < thisline_decision.plfstrt + thisline_decision.plflinelen
	&& (GET_PLANES (bplcon0) == 0 || !dmaen (DMA_BITPLANE)))
    {
	/* This is getting gross... */
	thisline_decision.plflinelen = hpos - thisline_decision.plfstrt;
	thisline_decision.plflinelen &= 7;
	if (thisline_decision.plflinelen == 0)
	    thisline_decision.which = -1;
	/* ... especially THIS! */
	modulos_added = 1;
	return;
    }

    if (diwstate == DIW_waiting_start
	|| GET_PLANES (bplcon0) == 0
	|| !dmaen (DMA_BITPLANE)
	|| hpos >= thisline_decision.plfstrt + thisline_decision.plflinelen)
	return;

    if (thisline_decision.which == 1
	&& hpos > thisline_decision.plfstrt
	&& decided_nr_planes < nr_planes_from_bplcon0)
    {
	set_decided_res ();
    	init_broken_program ();
	decided_nr_planes = nr_planes_from_bplcon0;
	thisline_decision.bplcon0 &= 0xFEF;
	thisline_decision.bplcon0 |= bplcon0 & 0xF010;
	adjust_broken_program (hpos);
	MARK_LINE_CHANGED; /* Play safe. */
	return;
    }
    
    if (thisline_decision.which != -1)
	return;

#if 0 /* Can't warn because Kickstart 1.3 does it at reset time ;-) */
    {
	static int warned = 0;
	if (!warned) {
	    write_log ("That program you are running is _really_ broken.\n");
	    warned = 1;
	}
    }
#endif
    init_broken_program ();

    decided_nr_planes = nr_planes_from_bplcon0;
    thisline_decision.bplcon0 &= 0xFEF;
    thisline_decision.bplcon0 |= bplcon0 & 0xF010;

    MARK_LINE_CHANGED; /* Play safe. */
    decide_as_playfield (plfstrt, plflinelen);
    adjust_broken_program (hpos);
}

static void decide_line_1 (int hpos)
{
    do_sprites (vpos, hpos);

    /* Surprisingly enough, this seems to be correct here - putting this into
     * decide_diw() results in garbage. */
    if (diwstate == DIW_waiting_start && vpos == plffirstline) {
	diwstate = DIW_waiting_stop;
    }
    if (diwstate == DIW_waiting_stop && vpos == plflastline) {
	diwstate = DIW_waiting_start;
    }

    if (framecnt != 0) {
/*	thisline_decision.which = -2; This doesn't do anything but hurt, I think. */
	return;
    }

    if (!dmaen(DMA_BITPLANE) || diwstate == DIW_waiting_start || nr_planes_from_bplcon0 == 0) {
	/* We don't want to draw this one. */
	thisline_decision.which = -1;
	thisline_decision.plfstrt = plfstrt;
	thisline_decision.plflinelen = plflinelen;
	return;
    }

    set_decided_res ();
    decided_nr_planes = nr_planes_from_bplcon0;
#if 0
    /* The blitter gets slower if there's high bitplane activity.
     * @@@ but the values must be different. FIXME */
    if (bltstate != BLT_done
	&& ((decided_res == RES_HIRES && decided_nr_planes > 2)
	    || (decided_res == RES_LORES && decided_nr_planes > 4)))
    {
	int pl = decided_nr_planes;
	unsigned int n = (eventtab[ev_blitter].evtime-cycles);
	if (n > plflinelen)
	    n = plflinelen;
	n >>= 1;
	if (decided_res == RES_HIRES)
	    pl <<= 1;
	if (pl == 8)
	    eventtab[ev_blitter].evtime += plflinelen;
	else if (pl == 6)
	    eventtab[ev_blitter].evtime += n*2;
	else if (pl == 5)
	    eventtab[ev_blitter].evtime += n*3/2;
	events_schedule();
    }
#endif
    decide_as_playfield (plfstrt, plflinelen);
}

/* Main entry point for deciding how to draw a line. May either do
 * nothing, decide the line as border, or decide the line as playfield. */
STATIC_INLINE void decide_line (int hpos)
{
    if (thisline_decision.which == 0 && hpos >= plfstrt)
	decide_line_1 (hpos);
}

/* Called when a color is about to be changed (write to a color register),
 * but the new color has not been entered into the table yet. */
static void record_color_change (int hpos, int regno, unsigned long value)
{
    /* Early positions don't appear on-screen. */
    if (framecnt != 0 || vpos < minfirstline || hpos < 0x18
	/*|| currprefs.emul_accuracy == 0*/)
	return;

    decide_diw (hpos);
    decide_line (hpos);

    /* See if we can record the color table, but have not done so yet.
     * @@@ There might be a minimal performance loss in case someone changes
     * a color exactly at the start of the DIW. I don't think it can actually
     * fail to work even in this case, but I'm not 100% sure.
     * @@@ There might be a slightly larger performance loss if we're drawing
     * this line as border... especially if there are changes in colors != 0
     * we might end up setting line_changed for no reason. FIXME */
    if (thisline_decision.diwfirstword >= 0
	&& thisline_decision.which != 0)
    {
	if (thisline_decision.ctable == -1)
	    remember_ctable ();
    }

    /* Changes outside the DIW can be ignored if the modified color is not the
     * background color, or if the accuracy is < 2. */
    if ((regno != 0 || currprefs.emul_accuracy < 2)
	&& (diwstate == DIW_waiting_start || thisline_decision.diwfirstword < 0
	    || thisline_decision.diwlastword >= 0
	    || thisline_decision.which == 0
	    || (thisline_decision.which == 1 && hpos >= thisline_decision.plfstrt + thisline_decision.plflinelen)))
	return;

    /* If the border is changed the first time before the DIW, record the
     * original starting border value. */
    if (regno == 0 && thisline_decision.color0 == 0xFFFFFFFFul && thisline_decision.diwfirstword < 0) {
	thisline_decision.color0 = current_colors.color_regs[0];
	if (line_decisions[next_lineno].color0 != value)
	    thisline_changed = 1;
    }
    /* Anything else gets recorded in the color_changes table. */
#ifdef OS_WITHOUT_MEMORY_MANAGEMENT
    if (next_color_change >= max_color_change) {
	++delta_color_change;
	return;
    }
#endif
    curr_color_changes[next_color_change].linepos = hpos;
    curr_color_changes[next_color_change].regno = regno;
    curr_color_changes[next_color_change++].value = value;
}

/* Stupid hack to get some Sanity demos working. */
static void decide_delay (int hpos)
{
    static int warned;

    /* Don't do anything if we're outside the DIW. */
    if (thisline_decision.diwfirstword == -1 || thisline_decision.diwlastword > 0)
	return;
    decide_line (hpos);
    /* Half-hearted attempt to get things right even when post_decide_line() changes
     * the decision afterwards. */
    if (thisline_decision.which != 1) {
	thisline_decision.bplcon1 = bplcon1;
	return;
    }
    /* @@@ Could check here for DDF stopping earlier than DIW */

#ifdef OS_WITHOUT_MEMORY_MANAGEMENT
    if (next_delay_change >= max_delay_change) {
	++delta_delay_change;
	return;
    }
#endif
    delay_changes[next_delay_change].linepos = hpos;
    delay_changes[next_delay_change++].value = bplcon1;
    if (!warned) {
	warned = 1;
	write_log ("Program is torturing BPLCON1.\n");
    }
}

/*
 * The decision which bitplane pointers to use is not made at plfstrt, since
 * data fetch does not start for all planes at this point. Therefore, we wait
 * for the end of the ddf area or the first write to any of the bitplane
 * pointer registers, whichever comes last, before we decide which plane pointers
 * to use.
 * Call decide_line() before this function.
 */
static void decide_plane (int hpos)
{
    int i, bytecount;

    if (framecnt != 0 || plane_decided)
	return;

    if (decided_nr_planes == -1 /* Still undecided */
	|| hpos < thisline_decision.plfstrt + thisline_decision.plflinelen)
	return;

    plane_decided = 1;

    bytecount = plflinelen / RES_SHIFT (decided_res) * 2;

    if (bytecount > MAX_WORDS_PER_LINE * 2) {
	/* Can't happen. */
	static int warned = 0;
	if (!warned)
	    write_log ("Mysterious bug in decide_plane(). Please report.\n");
	warned = 1;
	bytecount = 0;
    }
    if (!very_broken_program) {
	uae_u8 *dataptr = line_data[next_lineno];
	for (i = 0; i < decided_nr_planes; i++, dataptr += MAX_WORDS_PER_LINE*2) {
	    uaecptr pt = bplpt[i];
	    uae_u8 *real_ptr = pfield_xlateptr(pt, bytecount);
	    if (real_ptr == NULL)
		real_ptr = pfield_xlateptr(0, 0);
#ifdef SMART_UPDATE
#if 1
	    if (thisline_changed)
		memcpy (dataptr, real_ptr, bytecount);
	    else
#endif
		thisline_changed |= memcmpy (dataptr, real_ptr, bytecount);
#else
	    real_bplpt[i] = real_ptr;
#endif
	}
    } else {
	uae_u8 *dataptr = line_data[next_lineno];
	for (i = 0; i < decided_nr_planes; i++, dataptr += MAX_WORDS_PER_LINE*2) {
	    uaecptr pt = bplpt[i];
	    uae_u8 *real_ptr;

	    pt -= broken_plane_sub[i];
	    real_ptr = pfield_xlateptr (pt, bytecount);
	    if (real_ptr == NULL)
		real_ptr = pfield_xlateptr(0, 0);
#ifdef SMART_UPDATE
	    if (!thisline_changed)
		thisline_changed |= memcmpy (dataptr, real_ptr, bytecount);
	    else
		memcpy (dataptr, real_ptr, bytecount);
#else
	    real_bplpt[i] = real_ptr;
#endif
	}
    }
}

/*
 * Called from the BPLxMOD routines, after a new value has been written.
 * This routine decides whether the new value is already relevant for the
 * current line.
 */
static void decide_modulos (int hpos)
{
    /* All this effort just for the Sanity WOC demo... */
    decide_line (hpos);
    if (decided_nr_planes != -1 && hpos >= thisline_decision.plfstrt + thisline_decision.plflinelen)
	return;
    decided_bpl1mod = bpl1mod;
    decided_bpl2mod = bpl2mod;
}

/*
 * Add the modulos to the bitplane pointers if data fetch is already
 * finished for the current line.
 * Call decide_plane() before calling this, so that we won't use the
 * new values of the plane pointers for the current line.
 */
static void do_modulos (int hpos)
{
    /* decided_nr_planes is != -1 if this line should be drawn by the
     * display hardware, regardless of whether it fits on the emulated screen.
     */
    if (decided_nr_planes != -1 && plane_decided && !modulos_added
	&& hpos >= thisline_decision.plfstrt + thisline_decision.plflinelen)
    {
	int bytecount = thisline_decision.plflinelen / RES_SHIFT (decided_res) * 2;
	int add1 = bytecount + decided_bpl1mod;
	int add2 = bytecount + decided_bpl2mod;

	if (!very_broken_program) {
	    switch (decided_nr_planes) {
	     case 8: bplpt[7] += add2;
	     case 7: bplpt[6] += add1;
	     case 6: bplpt[5] += add2;
	     case 5: bplpt[4] += add1;
	     case 4: bplpt[3] += add2;
	     case 3: bplpt[2] += add1;
	     case 2: bplpt[1] += add2;
	     case 1: bplpt[0] += add1;
	    }
	} else switch (decided_nr_planes) {
	     case 8: bplpt[7] += add2 - broken_plane_sub[7];
	     case 7: bplpt[6] += add1 - broken_plane_sub[6];
	     case 6: bplpt[5] += add2 - broken_plane_sub[5];
	     case 5: bplpt[4] += add1 - broken_plane_sub[4];
	     case 4: bplpt[3] += add2 - broken_plane_sub[3];
	     case 3: bplpt[2] += add1 - broken_plane_sub[2];
	     case 2: bplpt[1] += add2 - broken_plane_sub[1];
	     case 1: bplpt[0] += add1 - broken_plane_sub[0];
	}

	modulos_added = 1;
    }
}

STATIC_INLINE void record_sprite (int spr, int sprxp)
{
    int pos = next_sprite_draw;
    unsigned int data, datb;

#ifdef OS_WITHOUT_MEMORY_MANAGEMENT
    if(pos >= max_sprite_draw) {
	++delta_sprite_draw;
	return;
    }
#endif
    /* XXX FIXME, this isn't very clever, but it might do */
    for (;;) {
	if (pos == curr_drawinfo[next_lineno].first_sprite_draw)
	    break;
	if (curr_sprite_positions[pos-1].linepos < sprxp)
	    break;
	if (curr_sprite_positions[pos-1].linepos == sprxp
	    && curr_sprite_positions[pos-1].num > spr)
	    break;
	printf("Foo\n");
	pos--;
    }
    if (pos != next_sprite_draw) {
	int pos2 = next_sprite_draw;
	while (pos2 != pos) {
	    curr_sprite_positions[pos2] = curr_sprite_positions[pos2-1];
	    pos2--;
	}
    }
    curr_sprite_positions[pos].linepos = sprxp;
    curr_sprite_positions[pos].num = spr;
    curr_sprite_positions[pos].ctl = sprctl[spr];
    data = sprdata[spr];
    datb = sprdatb[spr];
    curr_sprite_positions[pos].datab = ((sprtaba[data & 0xFF] << 16) | sprtaba[data >> 8]
					| (sprtabb[datb & 0xFF] << 16) | sprtabb[datb >> 8]);
    next_sprite_draw++;
}

static void decide_sprites (int hpos)
{
    int nrs[MAX_SPRITES], posns[MAX_SPRITES];
    int count, i;
    int point = PIXEL_XPOS (hpos);

    if (framecnt != 0 || hpos < 0x14 || nr_armed == 0 || point == last_sprite_point)
	return;

    decide_diw (hpos);
    decide_line (hpos);

    if (thisline_decision.which != 1)
	return;

    count = 0;
    for (i = 0; i < 8; i++) {
	int sprxp = sprxpos[i];
	int j, bestp;

	if (!sprarmed[i] || sprxp < 0 || sprxp > point || last_sprite_point >= sprxp)
	    continue;
	if ((thisline_decision.diwfirstword >= 0 && sprxp + sprite_width < thisline_decision.diwfirstword)
	    || (thisline_decision.diwlastword >= 0 && sprxp > thisline_decision.diwlastword))
	    continue;

	for (bestp = 0; bestp < count; bestp++) {
	    if (posns[bestp] > sprxp)
		break;
	    if (posns[bestp] == sprxp && nrs[bestp] < i)
		break;
	}
	for (j = count; j > bestp; j--) {
	    posns[j] = posns[j-1];
	    nrs[j] = nrs[j-1];
	}
	posns[j] = sprxp;
	nrs[j] = i;
	count++;
    }
    for (i = 0; i < count; i++)
	record_sprite (nrs[i], posns[i]);
    last_sprite_point = point;
}

/* End of a horizontal scan line. Finish off all decisions that were not
 * made yet. */
static void finish_decisions (void)
{
    struct draw_info *dip;
    struct draw_info *dip_old;
    struct decision *dp;
    int changed;
    int hpos = current_hpos ();

    if (framecnt != 0)
	return;

    decide_diw (hpos);
    if (thisline_decision.which == 0)
	decide_line_1 (hpos);

    /* Large DIWSTOP values can cause the stop position never to be
     * reached, so the state machine always stays in the same state and
     * there's a more-or-less full-screen DIW. */
    if (hdiwstate == DIW_waiting_stop) {
	thisline_decision.diwlastword = max_diwlastword;
	if (thisline_decision.diwlastword != line_decisions[next_lineno].diwlastword)
	    MARK_LINE_CHANGED;
    }

    if (line_decisions[next_lineno].which != thisline_decision.which)
	thisline_changed = 1;
    decide_plane (hpos);

    dip = curr_drawinfo + next_lineno;
    dip_old = prev_drawinfo + next_lineno;
    dp = line_decisions + next_lineno;
    changed = thisline_changed;

    if (thisline_decision.which == 1) {
	record_diw_line (diwfirstword, diwlastword);

	decide_sprites (hpos);

	if (thisline_decision.bplcon1 != line_decisions[next_lineno].bplcon1)
	    changed = 1;
    }

    dip->last_color_change = next_color_change;
    dip->last_delay_change = next_delay_change;
    dip->last_sprite_draw = next_sprite_draw;

    if (thisline_decision.ctable == -1) {
	if (thisline_decision.which == 1)
	    remember_ctable ();
	else
	    remember_ctable_for_border ();
    }
    if (thisline_decision.which == -1 && thisline_decision.color0 == 0xFFFFFFFFul)
	thisline_decision.color0 = current_colors.color_regs[0];

    dip->nr_color_changes = next_color_change - dip->first_color_change;
    dip->nr_sprites = next_sprite_draw - dip->first_sprite_draw;

    if (dip->first_delay_change != dip->last_delay_change)
	changed = 1;
    if (!changed
	&& (dip->nr_color_changes != dip_old->nr_color_changes
	    || (dip->nr_color_changes > 0
		&& memcmp (curr_color_changes + dip->first_color_change,
			   prev_color_changes + dip_old->first_color_change,
			   dip->nr_color_changes * sizeof *curr_color_changes) != 0)))
	changed = 1;
    if (!changed && thisline_decision.which == 1
	&& (dip->nr_sprites != dip_old->nr_sprites
	    || (dip->nr_sprites > 0
		&& memcmp (curr_sprite_positions + dip->first_sprite_draw,
			   prev_sprite_positions + dip_old->first_sprite_draw,
			   dip->nr_sprites * sizeof *curr_sprite_positions) != 0)))
	changed = 1;

    if (changed) {
	thisline_changed = 1;
	*dp = thisline_decision;
    } else
	/* The only one that may differ: */
	dp->ctable = thisline_decision.ctable;
}

/* Set the state of all decisions to "undecided" for a new scanline. */
static void reset_decisions (void)
{
    if (framecnt != 0)
	return;
    thisline_decision.which = 0;
    decided_bpl1mod = bpl1mod;
    decided_bpl2mod = bpl2mod;
    decided_nr_planes = -1;

    /* decided_res shouldn't be touched before it's initialized by decide_line(). */
    thisline_decision.diwfirstword = -1;
    thisline_decision.diwlastword = -2;
    if (hdiwstate == DIW_waiting_stop) {
	thisline_decision.diwfirstword = PIXEL_XPOS (DISPLAY_LEFT_SHIFT/2);
	if (thisline_decision.diwfirstword != line_decisions[next_lineno].diwfirstword)
	    MARK_LINE_CHANGED;
    }
    thisline_decision.ctable = -1;
    thisline_decision.color0 = 0xFFFFFFFFul;

    thisline_changed = 0;
    curr_drawinfo[next_lineno].first_color_change = next_color_change;
    curr_drawinfo[next_lineno].first_delay_change = next_delay_change;
    curr_drawinfo[next_lineno].first_sprite_draw = next_sprite_draw;

    /* memset(sprite_last_drawn_at, 0, sizeof sprite_last_drawn_at); */
    last_sprite_point = 0;
    modulos_added = 0;
    plane_decided = 0;
    color_decided = 0;
    very_broken_program = 0;
}

/* Initialize the decision array, once before the emulator really starts. */
static void init_decisions (void)
{
    size_t i;
    for (i = 0; i < sizeof line_decisions / sizeof *line_decisions; i++) {
	line_decisions[i].which = -2;
    }
}

/* set PAL or NTSC timing variables */

static void init_hz (void)
{
    int isntsc;

    beamcon0 = new_beamcon0;
    init_decisions ();

    isntsc = beamcon0 & 0x20 ? 0 : 1;
    if (!isntsc) {
	maxvpos = MAXVPOS_PAL;
	maxhpos = MAXHPOS_PAL;
	minfirstline = MINFIRSTLINE_PAL;
	vblank_endline = VBLANK_ENDLINE_PAL;
	vblank_hz = VBLANK_HZ_PAL;
    } else {
	maxvpos = MAXVPOS_NTSC;
	maxhpos = MAXHPOS_NTSC;
	minfirstline = MINFIRSTLINE_NTSC;
	vblank_endline = VBLANK_ENDLINE_NTSC;
	vblank_hz = VBLANK_HZ_NTSC;
    }
    vsynctime = syncbase / vblank_hz;

    write_log ("Using %s timing\n", isntsc ? "NTSC" : "PAL");
}

/* Calculate display window and data fetch values from the corresponding
 * hardware registers. */
static void calcdiw (void)
{
    int mask, add, mask2, add2, plfold;

    int hstrt = diwstrt & 0xFF;
    int hstop = diwstop & 0xFF;
    int vstrt = diwstrt >> 8;
    int vstop = diwstop >> 8;

    if (diwhigh_written) {
	hstrt |= ((diwhigh >> 5) & 1) << 8;
	hstop |= ((diwhigh >> 13) & 1) << 8;
	vstrt |= (diwhigh & 7) << 8;
	vstop |= ((diwhigh >> 8) & 7) << 8;
    } else {
	hstop += 0x100;
	if ((vstop & 0x80) == 0)
	    vstop |= 0x100;
    }

    diwfirstword = coord_hw_to_native_x (hstrt - 1);
    diwlastword = coord_hw_to_native_x (hstop - 1);

    if (diwlastword > max_diwlastword)
	diwlastword = max_diwlastword;
    if (diwfirstword < 0)
	diwfirstword = 0;
    if (diwlastword < 0)
	diwlastword = 0;

    plffirstline = vstrt;
    plflastline = vstop;

#if 0
    /* This happens far too often. */
    if (plffirstline < minfirstline) {
	fprintf(stderr, "Warning: Playfield begins before line %d!\n", minfirstline);
	plffirstline = minfirstline;
    }
#endif

#if 0 /* Turrican does this */
    if (plflastline > 313) {
	fprintf (stderr, "Warning: Playfield out of range!\n");
	plflastline = 313;
    }
#endif

    plfstrt = ddfstrt;
    plfstop = ddfstop;
    /* @@@ Toni... these ones might be wrong for AGA?  */
    if (plfstrt < 0x18) plfstrt = 0x18;
    if (plfstop < 0x18) plfstop = 0x18;
    if (plfstop > 0xD8) plfstop = 0xD8;

    /* ! If the masking operation is changed, the pfield_doline code could break
     * on some systems (alignment) */
    /* This actually seems to be correct now, at least the non-AGA stuff... */

    if ((fmode & 3) == 0) {
	/* FMODE=0 */
	mask = ~7;
	add = 15;
	mask2 = ~3;
	add2 = 0;
    } else if ((fmode & 3) == 3) {
	/* FMODE=3 */
	if(bplcon0 & 0x8000) {
	    mask = ~15;
	    add = 31;
	    mask2 = ~7;
	    add2 = 4;
	} else {
	    mask = ~31;
	    add = 63;
	    mask2 =  ~15;
	    add2 = 8;
	}
    } else {
	/* FMODE=1/2 */
	if(bplcon0 & 0x8000) {
	    mask = ~15;
	    add = 31;
	    mask2 = ~15;
	    add2 = 4;
	} else { 
	    mask = ~15;
	    add = 31;
	    mask2 = ~15;
	    add2 = 8;

	}
    }
    /* ugh.. This works in every demo and game I have tested, but can't be correct,
     * it looks too complex and stupid, does anybody know how this really works? (TW)
     */
    plfold = plfstrt;
    plfstrt &= mask2;
    plfstrt += add2;
    /* @@@ This one is different from the pre-AGA version.  It shouldn't make
     * a difference in OCS modes, though.  */
    plfstop += plfstrt - plfold;
    plfstop &= mask2;
    plfstop += add2;
    plflinelen = (plfstop-plfstrt+add) & mask;

    if (plfstrt > plfstop)
	plfstrt = plfstop;

#if 0
    if (plflinelen > 100 && (bplcon0 & 0x8000))
	write_log("vpos: %d fmode: %d, hires %d strt1 %d stop1 %d strt2 %d stop2 %d,plflinelen %d\n",
		  vpos,fmode&3,(bplcon0&0x8000)?1:0,ddfstrt,ddfstop,plfstrt,plfstop,plflinelen);
#endif
}

/*
 * lores,fmode=3 24 184 = 192
 * hires,fmode=3 40 200 = 176
 * hires,fmode=3 40 216 = 192
 * hires,fmode=1 56 200 = 160
 * hires,fmode=1 56 208 = 160
 * lores,fmode=1 48 200 = 176
 * lores,fmode=1 72 168 = 112 
*/

/* Mousehack stuff */

#define defstepx (1<<16)
#define defstepy (1<<16)
#define defxoffs 0
#define defyoffs 0

static const int docal = 60, xcaloff = 40, ycaloff = 20;
static const int calweight = 3;
static int lastsampledmx, lastsampledmy;
static int lastspr0x,lastspr0y,lastdiffx,lastdiffy,spr0pos,spr0ctl;
static int mstepx,mstepy,xoffs=defxoffs,yoffs=defyoffs;
static int sprvbfl;

int lastmx, lastmy;
int newmousecounters;
int ievent_alive = 0;

static enum { unknown_mouse, normal_mouse, dont_care_mouse, follow_mouse } mousestate;

static void mousehack_setdontcare (void)
{
    if (mousestate == dont_care_mouse)
	return;

    write_log ("Don't care mouse mode set\n");
    mousestate = dont_care_mouse;
    lastspr0x = lastmx; lastspr0y = lastmy;
    mstepx = defstepx; mstepy = defstepy;
}

static void mousehack_setfollow (void)
{
    if (mousestate == follow_mouse)
	return;

    write_log ("Follow sprite mode set\n");
    mousestate = follow_mouse;
    lastdiffx = lastdiffy = 0;
    sprvbfl = 0;
    spr0ctl = spr0pos = 0;
    mstepx = defstepx; mstepy = defstepy;
}

static uae_u32 mousehack_helper (void)
{
    int mousexpos, mouseypos;

#ifdef PICASSO96
    if (picasso_on) {
	mousexpos = lastmx - picasso96_state.XOffset;
	mouseypos = lastmy - picasso96_state.YOffset;
    } else
#endif
    {
	if (lastmy >= gfxvidinfo.height)
	    lastmy = gfxvidinfo.height - 1;
	mouseypos = coord_native_to_amiga_y (lastmy) << 1;
	mousexpos = coord_native_to_amiga_x (lastmx);
    }

    switch (m68k_dreg (regs, 0)) {
     case 0:
	return ievent_alive ? -1 : needmousehack ();
     case 1:
	ievent_alive = 10;
	return mousexpos;
     case 2:
	return mouseypos;
    }
    return 0;
}

void togglemouse (void)
{
    switch (mousestate) {
     case dont_care_mouse: mousehack_setfollow (); break;
     case follow_mouse: mousehack_setdontcare (); break;
     default: break; /* Nnnnnghh! */
    }
}

STATIC_INLINE int adjust (int val)
{
    if (val > 127)
	return 127;
    else if (val < -127)
	return -127;
    return val;
}

static void do_mouse_hack (void)
{
    int spr0x = ((spr0pos & 0xff) << 2) | ((spr0ctl & 1) << 1);
    int spr0y = ((spr0pos >> 8) | ((spr0ctl & 4) << 6)) << 1;
    int diffx, diffy;

    if (ievent_alive > 0) {
	mouse_x = mouse_y = 0;
	return;
    }
    switch (mousestate) {
     case normal_mouse:
	diffx = lastmx - lastsampledmx;
	diffy = lastmy - lastsampledmy;
	if (!newmousecounters) {
	    if (diffx > 127) diffx = 127;
	    if (diffx < -127) diffx = -127;
	    mouse_x += diffx;
	    if (diffy > 127) diffy = 127;
	    if (diffy < -127) diffy = -127;
	    mouse_y += diffy;
	}
	lastsampledmx += diffx; lastsampledmy += diffy;
	break;

     case dont_care_mouse:
	diffx = adjust (((lastmx - lastspr0x) * mstepx) >> 16);
	diffy = adjust (((lastmy - lastspr0y) * mstepy) >> 16);
	lastspr0x = lastmx; lastspr0y = lastmy;
	mouse_x += diffx; mouse_y += diffy;
	break;

     case follow_mouse:
	if (sprvbfl && sprvbfl-- > 1) {
	    int mousexpos, mouseypos;

	    if ((lastdiffx > docal || lastdiffx < -docal)
		&& lastspr0x != spr0x
		&& spr0x > plfstrt*4 + 34 + xcaloff
		&& spr0x < plfstop*4 - xcaloff)
	    {
		int val = (lastdiffx << 16) / (spr0x - lastspr0x);
		if (val >= 0x8000)
		    mstepx = (mstepx * (calweight - 1) + val) / calweight;
	    }
	    if ((lastdiffy > docal || lastdiffy < -docal)
		&& lastspr0y != spr0y
		&& spr0y > plffirstline + ycaloff
		&& spr0y < plflastline - ycaloff)
	    {
		int val = (lastdiffy << 16) / (spr0y - lastspr0y);
		if (val >= 0x8000)
		    mstepy = (mstepy * (calweight - 1) + val) / calweight;
	    }
	    if (lastmy >= gfxvidinfo.height)
		lastmy = gfxvidinfo.height-1;
	    mouseypos = coord_native_to_amiga_y (lastmy) << 1;
	    mousexpos = coord_native_to_amiga_x (lastmx);
	    diffx = adjust ((((mousexpos + xoffs - spr0x) & ~1) * mstepx) >> 16);
	    diffy = adjust ((((mouseypos + yoffs - spr0y) & ~1) * mstepy) >> 16);
	    lastspr0x = spr0x; lastspr0y = spr0y;
	    lastdiffx = diffx; lastdiffy = diffy;
	    mouse_x += diffx; mouse_y += diffy;
	}
	break;
	
     default:
	abort ();
    }
}

static int timehack_alive = 0;

static uae_u32 timehack_helper (void)
{
#ifdef HAVE_GETTIMEOFDAY
    struct timeval tv;
    if (m68k_dreg (regs, 0) == 0)
	return timehack_alive;

    timehack_alive = 10;

    gettimeofday (&tv, NULL);
    put_long (m68k_areg (regs, 0), tv.tv_sec - (((365 * 8 + 2) * 24 - 2) * 60 * 60));
    put_long (m68k_areg (regs, 0) + 4, tv.tv_usec);
    return 0;
#else
    return 2;
#endif
}

 /*
  * register functions
  */
STATIC_INLINE uae_u16 DENISEID (void)
{
    if (currprefs.chipset_mask & CSMASK_AGA)
	return 0xF8;
    if (currprefs.chipset_mask & CSMASK_ECS_DENISE)
	return 0xFC;
    return 0xFFFF;
}
STATIC_INLINE uae_u16 DMACONR (void)
{
    return (dmacon | (bltstate==BLT_done ? 0 : 0x4000)
	    | (blt_info.blitzero ? 0x2000 : 0));
}
STATIC_INLINE uae_u16 INTENAR (void)
{
    return intena;
}
uae_u16 INTREQR (void)
{
    return intreq | (currprefs.use_serial ? 0x0001 : 0);
}
STATIC_INLINE uae_u16 ADKCONR (void)
{
    return adkcon;
}
STATIC_INLINE uae_u16 VPOSR (void)
{
    unsigned int csbit = ntscmode ? 0x1000 : 0;
    csbit |= (currprefs.chipset_mask & CSMASK_AGA) ? 0x2300 : 0;
    csbit |= (currprefs.chipset_mask & CSMASK_ECS_AGNUS) ? 0x2000 : 0;
    return (vpos >> 8) | lof | csbit;
}
static void VPOSW (uae_u16 v)
{
    if (lof != (v & 0x8000))
	lof_changed = 1;
    lof = v & 0x8000;
    /*
     * This register is much more fun on a real Amiga. You can program
     * refresh rates with it ;) But I won't emulate this...
     */
}

STATIC_INLINE uae_u16 VHPOSR (void)
{
    return (vpos << 8) | current_hpos();
}

STATIC_INLINE void COP1LCH (uae_u16 v) { cop1lc = (cop1lc & 0xffff) | ((uae_u32)v << 16); }
STATIC_INLINE void COP1LCL (uae_u16 v) { cop1lc = (cop1lc & ~0xffff) | (v & 0xfffe); }
STATIC_INLINE void COP2LCH (uae_u16 v) { cop2lc = (cop2lc & 0xffff) | ((uae_u32)v << 16); }
STATIC_INLINE void COP2LCL (uae_u16 v) { cop2lc = (cop2lc & ~0xffff) | (v & 0xfffe); }

static void COPJMP1 (uae_u16 a)
{
    cop_state.ip = cop1lc;
    cop_state.do_move = 0;
    cop_state.ignore_next = 0;
    cop_state.state = COP_read1;
    cop_state.vpos = vpos;
    cop_state.hpos = current_hpos () & ~1;
    cop_state.count = current_hpos () & ~1;
    prepare_copper ();
    if (eventtab[ev_copper].evtime == cycles && eventtab[ev_copper].active)
	abort ();
    events_schedule ();
}

static void COPJMP2 (uae_u16 a)
{
    cop_state.ip = cop2lc;
    cop_state.do_move = 0;
    cop_state.ignore_next = 0;
    cop_state.state = COP_read1;
    cop_state.vpos = vpos;
    cop_state.hpos = current_hpos () & ~1;
    cop_state.count = current_hpos () & ~1;
    prepare_copper ();
    if (eventtab[ev_copper].evtime == cycles && eventtab[ev_copper].active)
	abort ();
    events_schedule ();
}

STATIC_INLINE void COPCON (uae_u16 a)
{
    copcon = a;
}

static void DMACON (uae_u16 v)
{
    int i, need_resched = 0;

    uae_u16 oldcon = dmacon;

    decide_line (current_hpos ());
    setclr(&dmacon,v); dmacon &= 0x1FFF;
    /* ??? post_decide_line (); */

    /* FIXME? Maybe we need to think a bit more about the master DMA enable
     * bit in these cases. */
    if ((dmacon & DMA_COPPER) > (oldcon & DMA_COPPER)) {
	cop_state.ip = cop1lc;
	cop_state.do_move = 0;
	cop_state.ignore_next = 0;
	cop_state.state = COP_read1;
	cop_state.vpos = vpos;
	cop_state.hpos = current_hpos () & ~1;
	cop_state.count = current_hpos () & ~1;
	prepare_copper ();
	if (eventtab[ev_copper].evtime == cycles && eventtab[ev_copper].active)
	    abort ();
	need_resched = 1;
    }
    if ((dmacon & DMA_DISK) > (oldcon & DMA_DISK)) {
	DISK_reset_cycles ();
    }
	
    if ((dmacon & DMA_BLITPRI) > (oldcon & DMA_BLITPRI) && bltstate != BLT_done) {
	static int count = 0;
	if (!count) {
	    count = 1;
	    write_log ("warning: program is doing blitpri hacks.\n");
	}
	regs.spcflags |= SPCFLAG_BLTNASTY;
    }

    update_audio ();

    for (i = 0; i < 4; i++) {
	struct audio_channel_data *cdp = audio_channel + i;

	cdp->dmaen = (dmacon & 0x200) && (dmacon & (1<<i));
	if (cdp->dmaen) {
	    if (cdp->state == 0) {
		cdp->state = 1;
		cdp->pt = cdp->lc;
		cdp->wper = cdp->per;
		cdp->wlen = cdp->len;
		cdp->data_written = 2;
		cdp->evtime = eventtab[ev_hsync].evtime - cycles;
	    }
	} else {
	    if (cdp->state == 1 || cdp->state == 5) {
		cdp->state = 0;
		cdp->last_sample = 0;
		cdp->current_sample = 0;
	    }
	}
    }

    if (need_resched)
	events_schedule();
}

/*static int trace_intena = 0;*/

STATIC_INLINE void INTENA (uae_u16 v)
{
/*    if (trace_intena)
	fprintf (stderr, "INTENA: %04x\n", v);*/
    setclr(&intena,v); regs.spcflags |= SPCFLAG_INT;
}
void INTREQ (uae_u16 v)
{
    setclr(&intreq,v);
    regs.spcflags |= SPCFLAG_INT;
    if (( v & 0x8800) == 0x0800)
	serdat &= 0xbfff;
}

static void ADKCON (uae_u16 v)
{
    unsigned long t;

    update_audio ();

    setclr (&adkcon,v);
    t = adkcon | (adkcon >> 4);
    audio_channel[0].adk_mask = (((t >> 0) & 1) - 1);
    audio_channel[1].adk_mask = (((t >> 1) & 1) - 1);
    audio_channel[2].adk_mask = (((t >> 2) & 1) - 1);
    audio_channel[3].adk_mask = (((t >> 3) & 1) - 1);
}

static void BEAMCON0 (uae_u16 v)
{
    new_beamcon0 = v & 0x20;
}

static void BPLPTH (int hpos, uae_u16 v, int num)
{
    decide_line (hpos);
    decide_plane (hpos);
    do_modulos (hpos);
    bplpt[num] = (bplpt[num] & 0xffff) | ((uae_u32)v << 16);
}
static void BPLPTL (int hpos, uae_u16 v, int num)
{
    decide_line (hpos);
    decide_plane (hpos);
    do_modulos (hpos);
    bplpt[num] = (bplpt[num] & ~0xffff) | (v & 0xfffe);
}

static void BPLCON0 (int hpos, uae_u16 v)
{
    if (! (currprefs.chipset_mask & CSMASK_AGA)) {
	v &= 0xFF0E;
	/* The Sanity WOC demo needs this at one place (at the end of the "Party Effect")
	 * Disable bitplane DMA if someone tries to do more than 4 Hires bitplanes. */
	if ((v & 0xF000) > 0xC000)
	    v &= 0xFFF;
	/* Don't want 7 lores planes either. */
	if ((v & 0x8000) == 0 && (v & 0x7000) == 0x7000)
	    v &= 0xEFFF;
    }
    if (bplcon0 == v)
	return;
    decide_line (hpos);
    bplcon0 = v;
    nr_planes_from_bplcon0 = GET_PLANES (v);

    if (currprefs.chipset_mask & CSMASK_AGA)
	/* It's not clear how the copper timings are affected by the number
	 * of bitplanes on AGA machines */
	corrected_nr_planes_from_bplcon0 = 4;
    else
	corrected_nr_planes_from_bplcon0 = nr_planes_from_bplcon0 << (bplcon0 & 0x8000 ? 1 : 0);

    post_decide_line (hpos);
}
STATIC_INLINE void BPLCON1 (int hpos, uae_u16 v)
{
    if (bplcon1 == v)
	return;
    decide_diw (hpos);
    bplcon1 = v;
    decide_delay (hpos);
}
STATIC_INLINE void BPLCON2 (int hpos, uae_u16 v)
{
    if (bplcon2 == v)
	return;
    decide_line (hpos);
    bplcon2 = v;
}
STATIC_INLINE void BPLCON3 (int hpos, uae_u16 v)
{
    if (bplcon3 == v)
	return;
    decide_line (hpos);
    bplcon3 = v;
}
STATIC_INLINE void BPLCON4 (int hpos, uae_u16 v)
{
    if (! (currprefs.chipset_mask & CSMASK_AGA))
	return;
    if (bplcon4 == v)
	return;
    decide_line (hpos);
    bplcon4 = v;
}

static void BPL1MOD (int hpos, uae_u16 v)
{
    v &= ~1;
    if ((uae_s16)bpl1mod == (uae_s16)v)
	return;
    bpl1mod = v;
    decide_modulos (hpos);
}

static void BPL2MOD (int hpos, uae_u16 v)
{
    v &= ~1;
    if ((uae_s16)bpl2mod == (uae_s16)v)
	return;
    bpl2mod = v;
    decide_modulos (hpos);
}

/* We could do as well without those... */
STATIC_INLINE void BPL1DAT (uae_u16 v) { bpl1dat = v; }
STATIC_INLINE void BPL2DAT (uae_u16 v) { bpl2dat = v; }
STATIC_INLINE void BPL3DAT (uae_u16 v) { bpl3dat = v; }
STATIC_INLINE void BPL4DAT (uae_u16 v) { bpl4dat = v; }
STATIC_INLINE void BPL5DAT (uae_u16 v) { bpl5dat = v; }
STATIC_INLINE void BPL6DAT (uae_u16 v) { bpl6dat = v; }
STATIC_INLINE void BPL7DAT (uae_u16 v) { bpl7dat = v; }
STATIC_INLINE void BPL8DAT (uae_u16 v) { bpl8dat = v; }

static void DIWSTRT (int hpos, uae_u16 v)
{
    if (diwstrt == v && ! diwhigh_written)
	return;
    decide_line (hpos);
    diwhigh_written = 0;
    diwstrt = v;
    calcdiw ();
}

static void DIWSTOP (int hpos, uae_u16 v)
{
    if (diwstop == v && ! diwhigh_written)
	return;
    decide_line (hpos);
    diwhigh_written = 0;
    diwstop = v;
    calcdiw ();
}

static void DIWHIGH (int hpos, uae_u16 v)
{
    if (! (currprefs.chipset_mask & CSMASK_ECS_DENISE))
	return;
    if (diwhigh_written && diwhigh == v)
	return;
    decide_line (hpos);
    diwhigh_written = 1;
    diwhigh = v;
    calcdiw ();
}

static void DDFSTRT (int hpos, uae_u16 v)
{
    v &= (currprefs.chipset_mask & CSMASK_AGA) ? 0x1FC : 0xFC;
    if (ddfstrt == v)
	return;
    decide_line (hpos);
    ddfstrt = v;
    calcdiw ();
}
static void DDFSTOP (int hpos, uae_u16 v)
{
    v &= (currprefs.chipset_mask & CSMASK_AGA) ? 0x1FC : 0xFC;
    if (ddfstop == v)
	return;
    decide_line (hpos);
    ddfstop = v;
    calcdiw ();
}

static void FMODE (uae_u16 v)
{
    if (! (currprefs.chipset_mask & CSMASK_AGA))
	return;
    fmode = v;
    calcdiw ();
}

static void BLTADAT (uae_u16 v)
{
    maybe_blit();

    blt_info.bltadat = v;
}
/*
 * "Loading data shifts it immediately" says the HRM. Well, that may
 * be true for BLTBDAT, but not for BLTADAT - it appears the A data must be
 * loaded for every word so that AFWM and ALWM can be applied.
 */
static void BLTBDAT (uae_u16 v)
{
    maybe_blit();

    if (bltcon1 & 2)
	blt_info.bltbhold = v << (bltcon1 >> 12);
    else
	blt_info.bltbhold = v >> (bltcon1 >> 12);
    blt_info.bltbdat = v;
}
static void BLTCDAT (uae_u16 v) { maybe_blit (); blt_info.bltcdat = v; }

static void BLTAMOD (uae_u16 v) { maybe_blit (); blt_info.bltamod = (uae_s16)(v & 0xFFFE); }
static void BLTBMOD (uae_u16 v) { maybe_blit (); blt_info.bltbmod = (uae_s16)(v & 0xFFFE); }
static void BLTCMOD (uae_u16 v) { maybe_blit (); blt_info.bltcmod = (uae_s16)(v & 0xFFFE); }
static void BLTDMOD (uae_u16 v) { maybe_blit (); blt_info.bltdmod = (uae_s16)(v & 0xFFFE); }

static void BLTCON0 (uae_u16 v) { maybe_blit (); bltcon0 = v; blinea_shift = v >> 12; }
/* The next category is "Most useless hardware register".
 * And the winner is... */
static void BLTCON0L (uae_u16 v)
{
    if (! (currprefs.chipset_mask & CSMASK_ECS_AGNUS))
	return;
    maybe_blit (); bltcon0 = (bltcon0 & 0xFF00) | (v & 0xFF);
}
static void BLTCON1 (uae_u16 v) { maybe_blit (); bltcon1 = v; }

static void BLTAFWM (uae_u16 v) { maybe_blit (); blt_info.bltafwm = v; }
static void BLTALWM (uae_u16 v) { maybe_blit (); blt_info.bltalwm = v; }

static void BLTAPTH (uae_u16 v) { maybe_blit (); bltapt = (bltapt & 0xffff) | ((uae_u32)v << 16); }
static void BLTAPTL (uae_u16 v) { maybe_blit (); bltapt = (bltapt & ~0xffff) | (v & 0xFFFE); }
static void BLTBPTH (uae_u16 v) { maybe_blit (); bltbpt = (bltbpt & 0xffff) | ((uae_u32)v << 16); }
static void BLTBPTL (uae_u16 v) { maybe_blit (); bltbpt = (bltbpt & ~0xffff) | (v & 0xFFFE); }
static void BLTCPTH (uae_u16 v) { maybe_blit (); bltcpt = (bltcpt & 0xffff) | ((uae_u32)v << 16); }
static void BLTCPTL (uae_u16 v) { maybe_blit (); bltcpt = (bltcpt & ~0xffff) | (v & 0xFFFE); }
static void BLTDPTH (uae_u16 v) { maybe_blit (); bltdpt = (bltdpt & 0xffff) | ((uae_u32)v << 16); }
static void BLTDPTL (uae_u16 v) { maybe_blit (); bltdpt = (bltdpt & ~0xffff) | (v & 0xFFFE); }

static void BLTSIZE (uae_u16 v)
{
    bltsize = v;

    maybe_blit ();

    blt_info.vblitsize = bltsize >> 6;
    blt_info.hblitsize = bltsize & 0x3F;
    if (!blt_info.vblitsize) blt_info.vblitsize = 1024;
    if (!blt_info.hblitsize) blt_info.hblitsize = 64;

    bltstate = BLT_init;
    do_blitter ();
}

static void BLTSIZV (uae_u16 v)
{
    if (! (currprefs.chipset_mask & CSMASK_ECS_AGNUS))
	return;
    maybe_blit ();
    oldvblts = v & 0x7FFF;
}

static void BLTSIZH (uae_u16 v)
{
    if (! (currprefs.chipset_mask & CSMASK_ECS_AGNUS))
	return;
    maybe_blit ();
    blt_info.hblitsize = v & 0x7FF;
    blt_info.vblitsize = oldvblts;
    if (!blt_info.vblitsize) blt_info.vblitsize = 32768;
    if (!blt_info.hblitsize) blt_info.hblitsize = 0x800;
    bltstate = BLT_init;
    do_blitter ();
}

STATIC_INLINE void SPRxCTL_1 (uae_u16 v, int num)
{
    int sprxp;
    sprctl[num] = v;
    nr_armed -= sprarmed[num];
    sprarmed[num] = 0;
    if (sprpos[num] == 0 && v == 0) {
	sprst[num] = SPR_stop;
	spron[num] = 0;
    } else
	sprst[num] = SPR_waiting_start;

    sprxp = coord_hw_to_native_x ((sprpos[num] & 0xFF) * 2 + (v & 1));
    sprxpos[num] = sprxp;
    sprvstart[num] = (sprpos[num] >> 8) | ((sprctl[num] << 6) & 0x100);
    sprvstop[num] = (sprctl[num] >> 8) | ((sprctl[num] << 7) & 0x100);
}
STATIC_INLINE void SPRxPOS_1 (uae_u16 v, int num)
{
    int sprxp;
    sprpos[num] = v;
    sprxp = coord_hw_to_native_x ((v & 0xFF) * 2 + (sprctl[num] & 1));
    sprxpos[num] = sprxp;
    sprvstart[num] = (sprpos[num] >> 8) | ((sprctl[num] << 6) & 0x100);
}
STATIC_INLINE void SPRxDATA_1 (uae_u16 v, int num)
{
    sprdata[num] = v;
    nr_armed += 1 - sprarmed[num];
    sprarmed[num] = 1;
}
STATIC_INLINE void SPRxDATB_1 (uae_u16 v, int num)
{
    sprdatb[num] = v;
}
static void SPRxDATA (int hpos, uae_u16 v, int num) { decide_sprites (hpos); SPRxDATA_1 (v, num); }
static void SPRxDATB (int hpos, uae_u16 v, int num) { decide_sprites (hpos); SPRxDATB_1 (v, num); }
static void SPRxCTL (int hpos, uae_u16 v, int num) { decide_sprites (hpos); SPRxCTL_1 (v, num); }
static void SPRxPOS (int hpos, uae_u16 v, int num) { decide_sprites (hpos); SPRxPOS_1 (v, num); }
static void SPRxPTH (int hpos, uae_u16 v, int num)
{
    decide_sprites (hpos);
    sprpt[num] &= 0xffff;
    sprpt[num] |= (uae_u32)v << 16;

    if (sprst[num] == SPR_stop || vpos < vblank_endline)
	sprst[num] = SPR_restart;
    spron[num] = 1;
}
static void SPRxPTL (int hpos, uae_u16 v, int num)
{
    decide_sprites (hpos);
    sprpt[num] &= ~0xffff;
    sprpt[num] |= v;

    if (sprst[num] == SPR_stop || vpos < vblank_endline)
	sprst[num] = SPR_restart;
    spron[num] = 1;
}
static void CLXCON (uae_u16 v)
{
    clxcon = v;
    clx_sprmask = (((v >> 15) << 7) | ((v >> 14) << 5) | ((v >> 13) << 3) | ((v >> 12) << 1) | 0x55);
}
static uae_u16 CLXDAT (void)
{
    uae_u16 v = clxdat;
    clxdat = 0;
    return v;
}
static void COLOR (int hpos, uae_u16 v, int num)
{

    v &= 0xFFF;
#if AGA_CHIPSET == 1
    {
	/* XXX Broken */
	int r,g,b;
	int cr,cg,cb;
	int colreg;
	uae_u32 cval;

	colreg = ((bplcon3 >> 13) & 7) * 32 + num;
	r = (v & 0xF00) >> 8;
	g = (v & 0xF0) >> 4;
	b = (v & 0xF) >> 0;
	cr = current_colors.color_regs[colreg] >> 16;
	cg = (current_colors.color_regs[colreg] >> 8) & 0xFF;
	cb = current_colors.color_regs[colreg] & 0xFF;

	if (bplcon3 & 0x200) {
	    cr &= 0xF0; cr |= r;
	    cg &= 0xF0; cg |= g;
	    cb &= 0xF0; cb |= b;
	} else {
	    cr = r + (r << 4);
	    cg = g + (g << 4);
	    cb = b + (b << 4);
	}
	cval = (cr << 16) | (cg << 8) | cb;
	if (cval == current_colors.color_regs[colreg])
	    return;

	/* Call this with the old table still intact. */
	record_color_change (hpos, colreg, v);
	remembered_color_entry = -1;
	current_colors.color_regs[colreg] = cval;
/*	current_colors.acolors[colreg] = xcolors[v];*/
    }
#else
    {
	if (current_colors.color_regs[num] == v)
	    return;
	/* Call this with the old table still intact. */
	record_color_change (hpos, num, v);
	remembered_color_entry = -1;
	current_colors.color_regs[num] = v;
	current_colors.acolors[num] = xcolors[v];
    }
#endif
}

static void DSKSYNC (uae_u16 v) { dsksync = v; }
static void DSKDAT (uae_u16 v) { write_log ("DSKDAT written. Not good.\n"); }
static void DSKPTH (uae_u16 v) { dskpt = (dskpt & 0xffff) | ((uae_u32)v << 16); }
static void DSKPTL (uae_u16 v) { dskpt = (dskpt & ~0xffff) | (v); }

static void DSKLEN (uae_u16 v)
{
    if (v & 0x8000) {
	dskdmaen = dskdmaen == 1 ? 2 : 1;
    } else {
	dskdmaen = 0;
	if (eventtab[ev_diskblk].active)
	    write_log ("warning: Disk DMA aborted!\n");
	eventtab[ev_diskblk].active = 0;
	events_schedule();
    }
    dsklen = dsklength = v; dsklength &= 0x3fff;
    if (dskdmaen == 2 && dsksync != 0x4489 && (adkcon & 0x400)) {
	write_log ("Non-standard sync: %04x len: %x\n", dsksync, dsklength);
    }
    if (dskdmaen <= 1)
	return;
    
    if (dsklen & 0x4000) {
	eventtab[ev_diskblk].active = 1;
	eventtab[ev_diskblk].oldcycles = cycles;
	eventtab[ev_diskblk].evtime = 40 + cycles; /* ??? */
	events_schedule();
	dskdmaen = 3;
    } else {
	 DISK_StartRead ();
	 syncfound = !(adkcon & 0x400);
    }
}

static int update_disk_reads (void)
{
    int retval = 0;
    /* If we get called from DSKBYTR or DSKDATR, we may not actually be
       reading from disk, so skip the DMA update.  */
    if (dskdmaen != 2) {
	DISK_update_reads (0, 0, 0, 0);
	return 0;
    }
    if (dsklength == 0)
	write_log ("Bug in disk code: dsklength == 0\n");
    else
	retval = DISK_update_reads (&dskpt, &dsklength, &syncfound, dsksync);

    if (dsklength == 0) {
	dskdmaen = -1;
	/* DISKBLK */
	INTREQ (0x8002);
    }
    return retval;
}

static void disksync_handler (void)
{
    uae_u16 mfm, byte;
    /* If no DMA, something weird happened.  It's not clear what to do
       about it.  */
    if (! dmaen (0x10) || dskdmaen != 2)
	return;

    /* Likewise... */
    if (! update_disk_reads ())
	return;
    DISK_GetData (&mfm, &byte);

    if (dsksync == mfm)
	INTREQ (0x9000);
    eventtab[ev_disksync].active = 0;
}

static uae_u16 DSKBYTR (void)
{
    uae_u16 v = (dsklen >> 1) & 0x6000;
    uae_u16 mfm, byte;
    if (update_disk_reads ())
	v |= 0x8000;
    DISK_GetData (&mfm, &byte);
    v |= byte;
    if (dsksync == mfm)
	v |= 0x1000;
    return v;
}

static uae_u16 DSKDATR (void)
{
    uae_u16 mfm, byte;
    update_disk_reads ();
    DISK_GetData (&mfm, &byte);
    return mfm;
}

static uae_u16 potgo_value;

static void POTGO (uae_u16 v)
{
    potgo_value = v;
}

static uae_u16 POTGOR (void)
{
    uae_u16 v = (potgo_value | (potgo_value << 1)) & 0xAA00;
    v |= v >> 1;

    if (JSEM_ISMOUSE (0, &currprefs)) {
	if (buttonstate[2])
	    v &= 0xFBFF;

	if (buttonstate[1])
	    v &= 0xFEFF;
    } else if (JSEM_ISJOY0 (0, &currprefs) || JSEM_ISJOY1 (0, &currprefs)) {
	if (joy0button & 2) v &= 0xfbff;
	if (joy0button & 4) v &= 0xfeff;
    }

    if (JSEM_ISJOY0 (1, &currprefs) || JSEM_ISJOY1 (1, &currprefs)) {
	if (joy1button & 2) v &= 0xbfff;
	if (joy1button & 4) v &= 0xefff;
    }

    return v;
}

static uae_u16 POT0DAT (void)
{
    static uae_u16 cnt = 0;
    if (JSEM_ISMOUSE (0, &currprefs)) {
	if (buttonstate[2])
	    cnt = ((cnt + 1) & 0xFF) | (cnt & 0xFF00);
	if (buttonstate[1])
	    cnt += 0x100;
    }

    return cnt;
}
static uae_u16 JOY0DAT (void)
{
    if (JSEM_ISMOUSE (0, &currprefs)) {
	do_mouse_hack ();
	return ((uae_u8)mouse_x) + ((uae_u16)mouse_y << 8);
    }
    return joy0dir;
}
static uae_u16 JOY1DAT (void)
{
    if (JSEM_ISMOUSE (1, &currprefs)) {
	do_mouse_hack ();
	return ((uae_u8)mouse_x) + ((uae_u16)mouse_y << 8);
    }
    return joy1dir;
}
static void JOYTEST (uae_u16 v)
{
    if (JSEM_ISMOUSE (0, &currprefs)) {
	mouse_x = v & 0xFC;
	mouse_y = (v >> 8) & 0xFC;
    }
}

/*
 * Here starts the copper code. Can you believe it used to be worse?
 */

STATIC_INLINE void copper_adjust_diw (struct copper *cst)
{
    if (cst->vdiw == DIW_waiting_start && vpos == plffirstline)
	cst->vdiw = DIW_waiting_stop;
    if (cst->vdiw == DIW_waiting_stop && vpos == plflastline)
	cst->vdiw = DIW_waiting_start;
}

/* Determine which cycles are available for the copper in a display
 * with a agiven number of planes.  */
static int cycles_for_plane[9][8] = {
    { 0, -1, 0, -1, 0, -1, 0, -1 },
    { 0, -1, 0, -1, 0, -1, 0, -1 },
    { 0, -1, 0, -1, 0, -1, 0, -1 },
    { 0, -1, 0, -1, 0, -1, 0, -1 },
    { 0, -1, 0, -1, 0, -1, 0, -1 },
    { 0, -1, 1, -1, 0, -1, 0, -1 },
    { 0, -1, 1, -1, 0, -1, 1, -1 },
    { 1, -1, 1, -1, 1, -1, 1, -1 },
    { 1, -1, 1, -1, 1, -1, 1, -1 }
};

static unsigned int waitmasktab[256];

#define COP_OFFSET 4

STATIC_INLINE int copper_in_playfield (enum diw_states diw, int hpos)
{
    hpos -= COP_OFFSET;
    return diw == DIW_waiting_stop && hpos >= plfstrt && hpos < plfstrt + plflinelen;
}

STATIC_INLINE int copper_cant_read (enum diw_states diw, int hpos, int planes)
{
    int t;

    if (hpos >= ((maxhpos - 2) & ~1))
	return 1;

    if (currprefs.chipset_mask & CSMASK_AGA)
	/* FIXME */
	return 0;

    if (! copper_in_playfield (diw, hpos))
	return 0;

    t = cycles_for_plane[planes][hpos & 7];
#if 0
    if (t == -1)
	abort ();
#endif
    return t;
}

static void update_copper_1 (int until_hpos)
{
    unsigned int vp, hp, vcmp, hcmp;
    int c_hpos = cop_state.hpos;
    int c_vpos = cop_state.vpos;

    vp = vpos & (((cop_state.i2 >> 8) & 0x7F) | 0x80);
    hp = cop_state.count & (cop_state.i2 & 0xFE);
    vcmp = ((cop_state.i1 & (cop_state.i2 | 0x8000)) >> 8);
    hcmp = (cop_state.i1 & cop_state.i2 & 0xFE);

    if (cop_state.state == COP_wait2)
	cop_state.state = COP_wait1b;

    for (;;) {
	if (c_hpos == (maxhpos & ~1)) {
	    c_hpos = 0;
	    cop_state.count = 0;
	    c_vpos++;
	}

	if (! dmaen (DMA_COPPER))
	    cop_state.state = COP_stop;

	if (c_vpos > vpos
	    || c_hpos > until_hpos
	    || cop_state.state == COP_stop)
	    break;

	if (c_hpos - COP_OFFSET == plfstrt)
	    copper_adjust_diw (&cop_state);

	switch (cop_state.state) {
	 case COP_rdelay1:
	    cop_state.state = COP_read1;
	    break;

	 case COP_read1:
	    if (copper_cant_read (cop_state.vdiw, c_hpos, corrected_nr_planes_from_bplcon0))
		break;

	    if (cop_state.do_move) {
		cop_state.do_move = 0;
		if (cop_state.i1 < (copcon & 2 ? ((currprefs.chipset_mask & CSMASK_AGA) ? 0 : 0x40u) : 0x80u)) {
		    cop_state.state = COP_stop;
		    break;
		}
		switch (cop_state.i1) {
		 case 0x088:
		    cop_state.ip = cop1lc;
		    cop_state.state = COP_rdelay1;
		    break;
		 case 0x08A:
		    cop_state.ip = cop2lc;
		    cop_state.state = COP_rdelay1;
		    break;
		 default:
		    custom_wput_1 (c_hpos, cop_state.i1, cop_state.i2);
		    break;
		}
		if (cop_state.state != COP_read1)
		    break;
	    }
	    cop_state.i1 = chipmem_bank.wget (cop_state.ip);
	    cop_state.ip += 2;
	    cop_state.state = COP_read2;
	    break;

	 case COP_read2:
	    if (copper_cant_read (cop_state.vdiw, c_hpos, corrected_nr_planes_from_bplcon0))
		break;
	    cop_state.i2 = chipmem_bank.wget (cop_state.ip);
	    cop_state.ip += 2;
	    if (cop_state.ignore_next) {
		cop_state.ignore_next = 0;
		cop_state.state = COP_read1;
		break;
	    }
	    if ((cop_state.i1 & 1) == 0) {
		cop_state.state = COP_read1;
		cop_state.do_move = 1;
	    } else {
		vp = vpos & (((cop_state.i2 >> 8) & 0x7F) | 0x80);
		hp = cop_state.count & (cop_state.i2 & 0xFE);
		vcmp = ((cop_state.i1 & (cop_state.i2 | 0x8000)) >> 8);
		hcmp = (cop_state.i1 & cop_state.i2 & 0xFE);

		if ((cop_state.i2 & 1) == 0) {
		    cop_state.state = COP_wait1;
		    if (cop_state.i1 == 0xFFFF && cop_state.i2 == 0xFFFE)
			cop_state.state = COP_stop;
		} else {
		    /* Skip instruction.  */
		    if ((vp > vcmp || (vp == vcmp && hp >= hcmp))
			&& ((cop_state.i2 & 0x8000) != 0 || ! (DMACONR() & 0x4000)))
			cop_state.ignore_next = 1;
		    cop_state.state = COP_read1;
		}
	    }
	    break;

	 case COP_wait1:
	    if (copper_cant_read (cop_state.vdiw, c_hpos, corrected_nr_planes_from_bplcon0))
		break;

	    /* fall through */

	 case COP_wait1b:
	    {
		int do_wait2 = cop_state.state == COP_wait1b;
		cop_state.state = COP_wait2;
		if (vp == vcmp && corrected_nr_planes_from_bplcon0 < 8) {
		    int t = cop_state.count + 2;
		    int next_count = (t & waitmasktab[cop_state.i2 & 0xfe]) | hcmp;
		    int nexthpos;
		    if (next_count < t)
			next_count = t;
		    nexthpos = c_hpos + next_count - cop_state.count;
		    if (nexthpos < (maxhpos & ~1)) {
			if (c_hpos - COP_OFFSET < plfstrt && nexthpos - COP_OFFSET >= plfstrt)
			    copper_adjust_diw (&cop_state);
			c_hpos = nexthpos;
			cop_state.count = next_count;
			do_wait2 = 1;
		    }
		}
		if (! do_wait2)
		    break;
	    }

	    /* fall through */

	 case COP_wait2:
	    if (vp < vcmp) {
		if (c_hpos - COP_OFFSET < plfstrt)
		    copper_adjust_diw (&cop_state);

		c_vpos++;
		c_hpos = 0;
		cop_state.count = 0;
		continue;
	    }
	    hp = cop_state.count & (cop_state.i2 & 0xFE);
	    if (vp == vcmp && hp < hcmp)
		break;
	    /* Now we know that the comparisons were successful.  */
	    if ((cop_state.i2 & 0x8000) == 0x8000 || ! (DMACONR() & 0x4000))
		cop_state.state = COP_read1;
	    else {
		/* We need to wait for the blitter.  It won't stop while
		 * we're in update_copper, so we _could_ as well proceed to
		 * until_hpos in one big step.  There are some tricky
		 * issues to be considered, though, so use the slow method
		 * for now.  */
		cop_state.state = COP_bltwait;
	    }
	    break;

	 default:
	    /* Delay cycles.  */
	    break;	    
	}

	c_hpos += 2;
	if (corrected_nr_planes_from_bplcon0 < 8 || ! copper_in_playfield (cop_state.vdiw, c_hpos))
	    cop_state.count += 2;
    }
    cop_state.hpos = c_hpos;
    cop_state.vpos = c_vpos;
}

STATIC_INLINE void update_copper (int until_hpos)
{
    if (cop_state.vpos > vpos
	|| cop_state.hpos > until_hpos
	|| cop_state.state == COP_stop)
	return;
    update_copper_1 (until_hpos);
}

static int dangerous_reg (int reg)
{
    /* Safe:
     * Bitplane pointers, control registers, modulos and data.
     * Sprite pointers, control registers, and data.
     * Color registers.  */
    if (reg >= 0xE0 && reg < 0x1C0)
	return 0;
    return 1;
}

static void prepare_copper_1 (void)
{
    struct copper cst = cop_state;
    unsigned int vp, hp, vcmp, hcmp;

    vp = vpos & (((cop_state.i2 >> 8) & 0x7F) | 0x80);
    hp = cop_state.count & (cop_state.i2 & 0xFE);
    vcmp = ((cop_state.i1 & (cop_state.i2 | 0x8000)) >> 8);
    hcmp = (cop_state.i1 & cop_state.i2 & 0xFE);

    if (cst.state == COP_wait2)
	cst.state = COP_wait1b;

    for (;;) {
	if (cst.hpos == (maxhpos & ~1)) {
	    cst.hpos = 0;
	    cst.count = 0;
	    cst.vpos++;
	}

	if (! dmaen (DMA_COPPER))
	    cst.state = COP_stop;

	if (cst.vpos > vpos
	    || cst.state == COP_stop)
	{
	    eventtab[ev_copper].active = 0;
	    return;
	}

	if (cst.hpos - COP_OFFSET == plfstrt)
	    copper_adjust_diw (&cst);

	switch (cst.state) {
	 case COP_rdelay1:
	    cst.state = COP_read1;
	    break;

	 case COP_read1:
	    if (copper_cant_read (cst.vdiw, cst.hpos, corrected_nr_planes_from_bplcon0))
		break;

	    if (cst.do_move) {
		cst.do_move = 0;
		if (cst.i1 < (copcon & 2 ? ((currprefs.chipset_mask & CSMASK_AGA) ? 0 : 0x40u) : 0x80u)) {
		    cst.state = COP_stop;
		    eventtab[ev_copper].active = 0;
		    return;
		} else if (dangerous_reg (cst.i1)) {
		    eventtab[ev_copper].active = 1;
		    eventtab[ev_copper].oldcycles = cycles;
		    eventtab[ev_copper].evtime = cycles + cst.hpos - current_hpos () + (cst.vpos - cop_state.vpos) * maxhpos;
		    return;
		}
	    }

	    cst.i1 = chipmem_bank.wget (cst.ip);
	    cst.ip += 2;
	    cst.state = COP_read2;
	    break;

	 case COP_read2:
	    if (copper_cant_read (cst.vdiw, cst.hpos, corrected_nr_planes_from_bplcon0))
		break;
	    cst.i2 = chipmem_bank.wget (cst.ip);
	    cst.ip += 2;
	    if (cst.ignore_next) {
		cst.ignore_next = 0;
		cst.state = COP_read1;
		break;
	    }
	    if ((cst.i1 & 1) == 0) {
		cst.state = COP_read1;
		cst.do_move = 1;
	    } else {
		vp = vpos & (((cst.i2 >> 8) & 0x7F) | 0x80);
		hp = cst.count & (cst.i2 & 0xFE);
		vcmp = ((cst.i1 & (cst.i2 | 0x8000)) >> 8);
		hcmp = (cst.i1 & cst.i2 & 0xFE);

		if ((cst.i2 & 1) == 0) {
		    cst.state = COP_wait1;
		    if (cst.i1 == 0xFFFF && cst.i2 == 0xFFFE)
			cst.state = COP_stop;
		} else {
		    /* Skip instruction.  */
		    if ((vp > vcmp || (vp == vcmp && hp >= hcmp))
			&& ((cst.i2 & 0x8000) != 0 || ! (DMACONR() & 0x4000)))
			cst.ignore_next = 1;
		    cst.state = COP_read1;
		}
	    }
	    break;

	 case COP_wait1:
	    if (copper_cant_read (cst.vdiw, cst.hpos, corrected_nr_planes_from_bplcon0))
		break;

	    /* fall through */

	 case COP_wait1b:
	    {
		int do_wait2 = cst.state == COP_wait1b;
		cst.state = COP_wait2;
		if (vp == vcmp && corrected_nr_planes_from_bplcon0 < 8) {
		    int t = cst.count + 2;
		    int next_count = (t & waitmasktab[cst.i2 & 0xfe]) | hcmp;
		    int nexthpos;
		    if (next_count < t)
			next_count = t;
		    nexthpos = cst.hpos + next_count - cst.count;
		    if (nexthpos < (maxhpos & ~1)) {
			if (cst.hpos - COP_OFFSET < plfstrt && nexthpos - COP_OFFSET >= plfstrt)
			    copper_adjust_diw (&cst);
			cst.hpos = nexthpos;
			cst.count = next_count;
			do_wait2 = 1;
		    }
		}
		if (! do_wait2)
		    break;
	    }

	    /* fall through */
	    
	 case COP_wait2:
	    if (vp < vcmp) {
		if (cst.hpos - COP_OFFSET < plfstrt)
		    copper_adjust_diw (&cst);

		cst.vpos++;
		cst.count = 0;
		cst.hpos = 0;
		continue;
	    }
	    hp = cst.count & (cst.i2 & 0xFE);
	    if (vp == vcmp && hp < hcmp)
		break;
#if 0
	    if (nexthpos != -1)
		if (nexthpos != cst.count)
		    printf ("ERROR\n");
	    nexthpos = -1;
#endif
	    /* Now we know that the comparisons were successful.  */
	    if ((cst.i2 & 0x8000) == 0x8000 || ! (DMACONR() & 0x4000))
		cst.state = COP_read1;
	    else {
		/* We need to wait for the blitter.  It won't stop while
		 * we're in update_copper, so we _could_ as well proceed to
		 * until_hpos in one big step.  There are some tricky
		 * issues to be considered, though, so use the slow method
		 * for now.  */
		cst.state = COP_bltwait;
	    }
	    break;

	 default:
	    /* Delay cycles.  */
	    break;	    
	}

	cst.hpos += 2;
	if (corrected_nr_planes_from_bplcon0 < 8 || ! copper_in_playfield (cst.vdiw, cst.hpos))
	    cst.count += 2;
    }
}

void blitter_done_notify (void)
{
    if (cop_state.state != COP_bltwait)
	return;
    update_copper (current_hpos ());
    cop_state.state = COP_wait2;
    eventtab[ev_copper].active = 1;
    eventtab[ev_copper].oldcycles = cycles;
    eventtab[ev_copper].evtime = (cycles + 2) & ~1;
    events_schedule ();
}

static void do_copper (void)
{
    int hpos = current_hpos ();
    update_copper (hpos);
    prepare_copper ();
    if (eventtab[ev_copper].evtime == cycles && eventtab[ev_copper].active)
	abort ();
}

static void diskblk_handler (void)
{
    regs.spcflags |= SPCFLAG_DISK;
    eventtab[ev_diskblk].active = 0;
}

void do_disk (void)
{
    if (dskdmaen != 3 && (regs.spcflags & SPCFLAG_DISK)) {
	static int warned = 0;
	if (!warned)
	    warned++, write_log ("BUG!\n");
	return;
    }
    if (dmaen (0x10)){
	if (dsklen & 0x4000) {
	    if (!chipmem_bank.check (dskpt, 2*dsklength)) {
		write_log ("warning: Bad disk write DMA pointer\n");
	    } else {
		uae_u8 *mfmp = get_real_address (dskpt);
		int i;
		DISK_InitWrite();

		for (i = 0; i < dsklength; i++) {
		    uae_u16 d = (*mfmp << 8) + *(mfmp+1);
		    mfmwrite[i] = d;
		    mfmp += 2;
		}
		DISK_WriteData(dsklength);
	    }
	}
	regs.spcflags &= ~SPCFLAG_DISK;
	INTREQ(0x9002);
	dskdmaen = -1;
    }
}

static void do_sprites (int currvp, int currhp)
{
    int i;
    int maxspr;

#if 0
    if (currvp == 0)
	return;
#else
    /* I don't know whether this is right. Some programs write the sprite pointers
     * directly at the start of the copper list. With the currvp==0 check, the
     * first two words of data are read on the second line in the frame. The problem
     * occurs when the program jumps to another copperlist a few lines further down
     * which _also_ writes the sprite pointer registers. This means that a) writing
     * to the sprite pointers sets the state to SPR_restart; or b) that sprite DMA
     * is disabled until the end of the vertical blanking interval. The HRM
     * isn't clear - it says that the vertical sprite position can be set to any
     * value, but this wouldn't be the first mistake... */
    /* Update: I modified one of the programs to write the sprite pointers the
     * second time only _after_ the VBlank interval, and it showed the same behaviour
     * as it did unmodified under UAE with the above check. This indicates that the
     * solution below is correct. */
    if (currvp < vblank_endline)
	return;
#endif

    /* The graph in the HRM, p. 195 seems to indicate that sprite 0 is
     * fetched at cycle 0x14 and thus can't be disabled by bitplane DMA. */
    maxspr = currhp/4 - 0x14/4;
    if (maxspr < 0)
	return;
    if (maxspr > 8)
	maxspr = 8;

    for (i = 0; i < maxspr; i++) {
	int fetch = 0;

	if (spron[i] == 0)
	    continue;

	if (sprst[i] == SPR_restart) {
	    fetch = 2;
	    spron[i] = 1;
	} else if ((sprst[i] == SPR_waiting_start && sprvstart[i] == vpos) || sprst[i] == SPR_waiting_stop) {
	    fetch = 1;
	    sprst[i] = SPR_waiting_stop;
	}
	if (sprst[i] == SPR_waiting_stop && sprvstop[i] == vpos) {
	    fetch = 2;
	    sprst[i] = SPR_waiting_start;
	}

	if (fetch && dmaen(DMA_SPRITE)) {
	    uae_u16 data1 = chipmem_bank.wget(sprpt[i]);
	    uae_u16 data2 = chipmem_bank.wget(sprpt[i]+2);
	    sprpt[i] += 4;

	    if (fetch == 1) {
		/* Hack for X mouse auto-calibration */
		if (i == 0 && !sprvbfl && ((sprpos[0] & 0xff) << 2) > 2 * DISPLAY_LEFT_SHIFT) {
		    spr0ctl=sprctl[0];
		    spr0pos=sprpos[0];
		    sprvbfl=2;
		}
		SPRxDATB_1(data2, i);
		SPRxDATA_1(data1, i);
	    } else {
		SPRxPOS_1(data1, i);
		SPRxCTL_1(data2, i);
	    }
	}
    }
}

static void init_sprites (void)
{
    int i;

    for (i = 0; i < 8; i++) {
	/* ???? */
	sprst[i] = SPR_stop;
	spron[i] = 0;
    }

    memset (sprpos, 0, sizeof sprpos);
    memset (sprctl, 0, sizeof sprctl);
}

static void adjust_array_sizes (void)
{
#ifdef OS_WITHOUT_MEMORY_MANAGEMENT
    if (delta_sprite_draw) {
	void *p1,*p2;
	int mcc = max_sprite_draw + 200 + delta_sprite_draw;
	delta_sprite_draw = 0;
	p1 = realloc (sprite_positions[0], mcc * sizeof (struct sprite_draw));
	p2 = realloc (sprite_positions[1], mcc * sizeof (struct sprite_draw));
	if (p1) sprite_positions[0] = p1;
	if (p2) sprite_positions[1] = p2;
	if (p1 && p2) {
	    fprintf (stderr, "new max_sprite_draw=%d\n",mcc);
	    max_sprite_draw = mcc;
	}
    }
    if (delta_color_change) {
	void *p1,*p2;
	int mcc = max_color_change + 200 + delta_color_change;
	delta_color_change = 0;
	p1 = realloc (color_changes[0], mcc * sizeof (struct color_change));
	p2 = realloc (color_changes[1], mcc * sizeof (struct color_change));
	if (p1) color_changes[0] = p1;
	if (p2) color_changes[1] = p2;
	if (p1 && p2) {
	    fprintf (stderr, "new max_color_change=%d\n",mcc);
	    max_color_change = mcc;
	}
    }
    if (delta_delay_change) {
	void *p;
	int mcc = max_delay_change + 200 + delta_delay_change;
	delta_delay_change = 0;
	p = realloc (delay_changes, mcc * sizeof (struct delay_change));
	if (p) {
	    fprintf (stderr, "new max_delay_change=%d\n",mcc);
	    delay_changes = p;
	    max_delay_change = mcc;
	}
    }
#endif
}

static void init_hardware_frame (void)
{
    next_lineno = 0;
    nextline_how = nln_normal;
    diwstate = DIW_waiting_start;
    hdiwstate = DIW_waiting_start;
}

void init_hardware_for_drawing_frame (void)
{
    adjust_array_sizes ();

    next_color_change = 0;
    next_delay_change = 0;
    next_sprite_draw = 0;

    next_color_entry = 0;
    remembered_color_entry = -1;
    prev_sprite_positions = sprite_positions[current_change_set];
    curr_sprite_positions = sprite_positions[current_change_set ^ 1];
    prev_color_changes = color_changes[current_change_set];
    curr_color_changes = color_changes[current_change_set ^ 1];
    prev_color_tables = color_tables[current_change_set];
    curr_color_tables = color_tables[current_change_set ^ 1];

    prev_drawinfo = line_drawinfo[current_change_set];
    curr_drawinfo = line_drawinfo[current_change_set ^ 1];
    current_change_set ^= 1;

    color_src_match = color_dest_match = -1;
}

static void vsync_handler (void)
{
    if (currprefs.m68k_speed == -1) {
	frame_time_t curr_time = read_processor_time ();
	vsyncmintime += vsynctime;
	/* @@@ Mathias? How do you think we should do this? */
	/* If we are too far behind, or we just did a reset, adjust the
	 * needed time. */
	if ((long int)(curr_time - vsyncmintime) > 0 || rpt_did_reset)
	    vsyncmintime = curr_time + vsynctime;
	rpt_did_reset = 0;
    }

    handle_events ();

    getjoystate (0, &joy0dir, &joy0button);
    getjoystate (1, &joy1dir, &joy1button);

    INTREQ (0x8020);
    if (bplcon0 & 4)
	lof ^= 0x8000;

    vsync_handle_redraw (lof, lof_changed);
    if (quit_program > 0)
	return;

    {
	static int cnt = 0;
	if (cnt == 0) {
	    /* resolution_check_change (); */
	    DISK_check_change ();
	    cnt = 5;
	}
	cnt--;
    }

    /* For now, let's only allow this to change at vsync time.  It gets too
     * hairy otherwise.  */
    if (beamcon0 != new_beamcon0)
	init_hz ();

    lof_changed = 0;

    cop_state.ip = cop1lc;
    cop_state.state = COP_read1;
    cop_state.vpos = 0;
    cop_state.hpos = 0;
    cop_state.count = 0;
    cop_state.do_move = 0;
    cop_state.ignore_next = 0;
    cop_state.vdiw = DIW_waiting_start;

    init_hardware_frame ();

#ifdef HAVE_GETTIMEOFDAY
    {
	struct timeval tv;
	unsigned long int newtime;

	gettimeofday(&tv,NULL);
	newtime = (tv.tv_sec-seconds_base) * 1000 + tv.tv_usec / 1000;

	if (!bogusframe) {
	    lastframetime = newtime - msecs;

#if 0 /* This doesn't appear to work too well yet... later.  */
	    if (n_consecutive_skipped > currprefs.sound_pri_cutoff
		|| lastframetime < currprefs.sound_pri_time)
	    {
		n_consecutive_skipped = 0;
		clear_inhibit_frame (IHF_SOUNDADJUST);
	    } else {
		n_consecutive_skipped++;
		set_inhibit_frame (IHF_SOUNDADJUST);
		total_skipped++;
	    }
#endif

	    frametime += lastframetime;
	    timeframes++;

	    if ((timeframes & 127) == 0)
		gui_fps (1000 * timeframes / frametime);
	}
	msecs = newtime;
	bogusframe = 0;
    }
#endif
    if (ievent_alive > 0)
	ievent_alive--;
    if (timehack_alive > 0)
	timehack_alive--;
    CIA_vsync_handler();
}

static void hsync_handler (void)
{
    update_copper (maxhpos);

    finish_decisions ();
    do_modulos (current_hpos ());

    hsync_record_line_state (next_lineno, nextline_how, thisline_changed);

    eventtab[ev_hsync].evtime += cycles - eventtab[ev_hsync].oldcycles;
    eventtab[ev_hsync].oldcycles = cycles;
    CIA_hsync_handler ();

    if (dskdmaen == 2 && dmaen (0x10)) {
	update_disk_reads ();
	DISK_search_sync (maxhpos, dsksync);
    }

    if (currprefs.produce_sound > 0) {
	int nr;

	update_audio ();

	/* Sound data is fetched at the beginning of each line */
	for (nr = 0; nr < 4; nr++) {
	    struct audio_channel_data *cdp = audio_channel + nr;

	    if (cdp->data_written == 2) {
		cdp->data_written = 0;
		cdp->nextdat = chipmem_bank.wget(cdp->pt);
		cdp->pt += 2;
		if (cdp->state == 2 || cdp->state == 3) {
		    if (cdp->wlen == 1) {
			cdp->pt = cdp->lc;
			cdp->wlen = cdp->len;
			cdp->intreq2 = 1;
		    } else
			cdp->wlen--;
		}
	    }
	}
    }

    hardware_line_completed (next_lineno);

    if (++vpos == (maxvpos + (lof != 0))) {
	vpos = 0;
	vsync_handler();
    }

    is_lastline = vpos + 1 == maxvpos + (lof != 0) && currprefs.m68k_speed == -1 && ! rpt_did_reset;

    if ((bplcon0 & 4) && currprefs.gfx_linedbl)
	notice_interlace_seen ();

    if (framecnt == 0) {
	int lineno = vpos;
	nextline_how = nln_normal;
	if (currprefs.gfx_linedbl) {
	    lineno *= 2;
	    nextline_how = currprefs.gfx_linedbl == 1 ? nln_doubled : nln_nblack;
	    if (bplcon0 & 4) {
		if (!lof) {
		    lineno++;
		    nextline_how = nln_lower;
		} else {
		    nextline_how = nln_upper;
		}
	    }
	}
	next_lineno = lineno;
	reset_decisions ();
    }
    if (uae_int_requested) {
	set_uae_int_flag ();
	INTREQ (0xA000);
    }
    update_copper (0);
    prepare_copper ();
    if (eventtab[ev_copper].evtime == cycles && eventtab[ev_copper].active)
	abort ();
}

static void init_eventtab (void)
{
    int i;

    for(i = 0; i < ev_max; i++) {
	eventtab[i].active = 0;
	eventtab[i].oldcycles = 0;
    }

    eventtab[ev_cia].handler = CIA_handler;
    eventtab[ev_copper].handler = do_copper;
    eventtab[ev_hsync].handler = hsync_handler;
    eventtab[ev_hsync].evtime = maxhpos + cycles;
    eventtab[ev_hsync].active = 1;

    eventtab[ev_blitter].handler = blitter_handler;
    eventtab[ev_blitter].active = 0;
    eventtab[ev_diskblk].handler = diskblk_handler;
    eventtab[ev_diskblk].active = 0;
    eventtab[ev_diskindex].handler = diskindex_handler;
    eventtab[ev_diskindex].active = 0;
    eventtab[ev_disksync].handler = disksync_handler;
    eventtab[ev_disksync].active = 0;

    events_schedule ();
}

void customreset (void)
{
    int i;
#ifdef HAVE_GETTIMEOFDAY
    struct timeval tv;
#endif

    for (i = 0; i < sizeof current_colors.color_regs / sizeof *current_colors.color_regs; i++)
	current_colors.color_regs[i] = -1;

    expamem_reset ();

    CIA_reset ();
    cycles = 0;
    regs.spcflags &= SPCFLAG_BRK;

    vpos = 0;
    lof = 0;

    if (needmousehack ()) {
#if 0
	mousehack_setfollow();
#else
	mousehack_setdontcare();
#endif
    } else {
	mousestate = normal_mouse;
    }
    ievent_alive = 0;
    timehack_alive = 0;

    clx_sprmask = 0xFF;
    clxdat = 0;

    memset (sprarmed, 0, sizeof sprarmed);
    nr_armed = 0;

    dmacon = intena = 0;
    bltstate = BLT_done;
    cop_state.state = COP_stop;
    diwstate = DIW_waiting_start;
    hdiwstate = DIW_waiting_start;
    copcon = 0;
    dskdmaen = 0;
    cycles = 0;

    bplcon4 = 0x11; /* Get AGA chipset into ECS compatibility mode */
    bplcon3 = 0xC00;

    new_beamcon0 = ntscmode ? 0x00 : 0x20;
    init_hz ();

    audio_reset ();

    init_eventtab ();

    init_sprites ();

    init_hardware_frame ();
    reset_drawing ();

    reset_decisions ();

#ifdef HAVE_GETTIMEOFDAY
    gettimeofday (&tv, NULL);
    seconds_base = tv.tv_sec;
    bogusframe = 1;
#endif
}

void dumpcustom (void)
{
    write_log ("DMACON: %x INTENA: %x INTREQ: %x VPOS: %x HPOS: %x\n", DMACONR(),
	       (unsigned int)intena, (unsigned int)intreq, (unsigned int)vpos, (unsigned int)current_hpos());
    write_log ("COP1LC: %08lx, COP2LC: %08lx\n", (unsigned long)cop1lc, (unsigned long)cop2lc);
    if (timeframes) {
	write_log ("Average frame time: %d ms [frames: %d time: %d]\n",
		   frametime / timeframes, timeframes, frametime);
	if (total_skipped)
	    write_log ("Skipped frames: %d\n", total_skipped);
    }
    dump_audio_bench ();
    /*for (i=0; i<256; i++) if (blitcount[i]) fprintf (stderr, "minterm %x = %d\n",i,blitcount[i]);  blitter debug */
}

int intlev (void)
{
    uae_u16 imask = intreq & intena;
    if (imask && (intena & 0x4000)){
	if (imask & 0x2000) return 6;
	if (imask & 0x1800) return 5;
	if (imask & 0x0780) return 4;
	if (imask & 0x0070) return 3;
	if (imask & 0x0008) return 2;
	if (imask & 0x0007) return 1;
    }
    return -1;
}

static void gen_custom_tables (void)
{
    int i;
    for (i = 0; i < 256; i++) {
	unsigned int j;
	sprtaba[i] = ((((i >> 7) & 1) << 0)
		      | (((i >> 6) & 1) << 2)
		      | (((i >> 5) & 1) << 4)
		      | (((i >> 4) & 1) << 6)
		      | (((i >> 3) & 1) << 8)
		      | (((i >> 2) & 1) << 10)
		      | (((i >> 1) & 1) << 12)
		      | (((i >> 0) & 1) << 14));
	sprtabb[i] = sprtaba[i] * 2;
	for (j = 0; j < 511; j = (j << 1) | 1)
	    if ((i & ~j) == 0)
		waitmasktab[i] = ~j;
    }
}

void custom_init (void)
{
    uaecptr pos;

#ifdef OS_WITHOUT_MEMORY_MANAGEMENT
    int num;

    for (num = 0; num < 2; num++) {
       sprite_positions[num] = xmalloc (max_sprite_draw * sizeof (struct sprite_draw));
       color_changes[num] = xmalloc (max_color_change * sizeof (struct color_change));
    }
    delay_changes = xmalloc (max_delay_change * sizeof (struct delay_change));
#endif

    pos = here ();

    org (0xF0FF70);
    calltrap (deftrap (mousehack_helper));
    dw (RTS);

    org (0xF0FFA0);
    calltrap (deftrap (timehack_helper));
    dw (RTS);

    org (pos);

    init_decisions ();

    gen_custom_tables ();
    build_blitfilltable ();

    drawing_init ();

    mousestate = unknown_mouse;

    if (needmousehack ())
	mousehack_setfollow();
}

/* Custom chip memory bank */

static uae_u32 custom_lget (uaecptr) REGPARAM;
static uae_u32 custom_wget (uaecptr) REGPARAM;
static uae_u32 custom_bget (uaecptr) REGPARAM;
static void custom_lput (uaecptr, uae_u32) REGPARAM;
static void custom_wput (uaecptr, uae_u32) REGPARAM;
static void custom_bput (uaecptr, uae_u32) REGPARAM;

addrbank custom_bank = {
    custom_lget, custom_wget, custom_bget,
    custom_lput, custom_wput, custom_bput,
    default_xlate, default_check
};

uae_u32 REGPARAM2 custom_wget (uaecptr addr)
{
    switch (addr & 0x1FE) {
     case 0x002: return DMACONR();
     case 0x004: return VPOSR();
     case 0x006: return VHPOSR();

     case 0x008: return DSKDATR();

     case 0x00A: return JOY0DAT();
     case 0x00C: return JOY1DAT();
     case 0x00E: return CLXDAT();
     case 0x010: return ADKCONR();

     case 0x012: return POT0DAT();
     case 0x016: return POTGOR();
     case 0x018: return SERDATR();
     case 0x01A: return DSKBYTR();
     case 0x01C: return INTENAR();
     case 0x01E: return INTREQR();
     case 0x07C: return DENISEID();
     default:
	custom_wput(addr,0);
	return 0xffff;
    }
}

uae_u32 REGPARAM2 custom_bget (uaecptr addr)
{
    return custom_wget(addr & 0xfffe) >> (addr & 1 ? 0 : 8);
}

uae_u32 REGPARAM2 custom_lget (uaecptr addr)
{
    return ((uae_u32)custom_wget(addr & 0xfffe) << 16) | custom_wget((addr+2) & 0xfffe);
}

void REGPARAM2 custom_wput_1 (int hpos, uaecptr addr, uae_u32 value)
{
    addr &= 0x1FE;
    switch (addr) {
     case 0x020: DSKPTH (value); break;
     case 0x022: DSKPTL (value); break;
     case 0x024: DSKLEN (value); break;
     case 0x026: DSKDAT (value); break;

     case 0x02A: VPOSW (value); break;
     case 0x2E:  COPCON (value); break;
     case 0x030: SERDAT (value); break;
     case 0x032: SERPER (value); break;
     case 0x34: POTGO (value); break;
     case 0x040: BLTCON0 (value); break;
     case 0x042: BLTCON1 (value); break;

     case 0x044: BLTAFWM (value); break;
     case 0x046: BLTALWM (value); break;

     case 0x050: BLTAPTH (value); break;
     case 0x052: BLTAPTL (value); break;
     case 0x04C: BLTBPTH (value); break;
     case 0x04E: BLTBPTL (value); break;
     case 0x048: BLTCPTH (value); break;
     case 0x04A: BLTCPTL (value); break;
     case 0x054: BLTDPTH (value); break;
     case 0x056: BLTDPTL (value); break;

     case 0x058: BLTSIZE (value); break;

     case 0x064: BLTAMOD (value); break;
     case 0x062: BLTBMOD (value); break;
     case 0x060: BLTCMOD (value); break;
     case 0x066: BLTDMOD (value); break;

     case 0x070: BLTCDAT (value); break;
     case 0x072: BLTBDAT (value); break;
     case 0x074: BLTADAT (value); break;

     case 0x07E: DSKSYNC (value); break;

     case 0x080: COP1LCH (value); break;
     case 0x082: COP1LCL (value); break;
     case 0x084: COP2LCH (value); break;
     case 0x086: COP2LCL (value); break;

     case 0x088: COPJMP1 (value); break;
     case 0x08A: COPJMP2 (value); break;

     case 0x08E: DIWSTRT (hpos, value); break;
     case 0x090: DIWSTOP (hpos, value); break;
     case 0x092: DDFSTRT (hpos, value); break;
     case 0x094: DDFSTOP (hpos, value); break;

     case 0x096: DMACON (value); break;
     case 0x098: CLXCON (value); break;
     case 0x09A: INTENA (value); break;
     case 0x09C: INTREQ (value); break;
     case 0x09E: ADKCON (value); break;

     case 0x0A0: AUDxLCH (0, value); break;
     case 0x0A2: AUDxLCL (0, value); break;
     case 0x0A4: AUDxLEN (0, value); break;
     case 0x0A6: AUDxPER (0, value); break;
     case 0x0A8: AUDxVOL (0, value); break;
     case 0x0AA: AUDxDAT (0, value); break;

     case 0x0B0: AUDxLCH (1, value); break;
     case 0x0B2: AUDxLCL (1, value); break;
     case 0x0B4: AUDxLEN (1, value); break;
     case 0x0B6: AUDxPER (1, value); break;
     case 0x0B8: AUDxVOL (1, value); break;
     case 0x0BA: AUDxDAT (1, value); break;

     case 0x0C0: AUDxLCH (2, value); break;
     case 0x0C2: AUDxLCL (2, value); break;
     case 0x0C4: AUDxLEN (2, value); break;
     case 0x0C6: AUDxPER (2, value); break;
     case 0x0C8: AUDxVOL (2, value); break;
     case 0x0CA: AUDxDAT (2, value); break;

     case 0x0D0: AUDxLCH (3, value); break;
     case 0x0D2: AUDxLCL (3, value); break;
     case 0x0D4: AUDxLEN (3, value); break;
     case 0x0D6: AUDxPER (3, value); break;
     case 0x0D8: AUDxVOL (3, value); break;
     case 0x0DA: AUDxDAT (3, value); break;

     case 0x0E0: BPLPTH (hpos, value, 0); break;
     case 0x0E2: BPLPTL (hpos, value, 0); break;
     case 0x0E4: BPLPTH (hpos, value, 1); break;
     case 0x0E6: BPLPTL (hpos, value, 1); break;
     case 0x0E8: BPLPTH (hpos, value, 2); break;
     case 0x0EA: BPLPTL (hpos, value, 2); break;
     case 0x0EC: BPLPTH (hpos, value, 3); break;
     case 0x0EE: BPLPTL (hpos, value, 3); break;
     case 0x0F0: BPLPTH (hpos, value, 4); break;
     case 0x0F2: BPLPTL (hpos, value, 4); break;
     case 0x0F4: BPLPTH (hpos, value, 5); break;
     case 0x0F6: BPLPTL (hpos, value, 5); break;
     case 0x0F8: BPLPTH (hpos, value, 6); break;
     case 0x0FA: BPLPTL (hpos, value, 6); break;
     case 0x0FC: BPLPTH (hpos, value, 7); break;
     case 0x0FE: BPLPTL (hpos, value, 7); break;

     case 0x100: BPLCON0 (hpos, value); break;
     case 0x102: BPLCON1 (hpos, value); break;
     case 0x104: BPLCON2 (hpos, value); break;
     case 0x106: BPLCON3 (hpos, value); break;

     case 0x108: BPL1MOD (hpos, value); break;
     case 0x10A: BPL2MOD (hpos, value); break;

     case 0x110: BPL1DAT (value); break;
     case 0x112: BPL2DAT (value); break;
     case 0x114: BPL3DAT (value); break;
     case 0x116: BPL4DAT (value); break;
     case 0x118: BPL5DAT (value); break;
     case 0x11A: BPL6DAT (value); break;
     case 0x11C: BPL7DAT (value); break;
     case 0x11E: BPL8DAT (value); break;

     case 0x180: case 0x182: case 0x184: case 0x186: case 0x188: case 0x18A:
     case 0x18C: case 0x18E: case 0x190: case 0x192: case 0x194: case 0x196:
     case 0x198: case 0x19A: case 0x19C: case 0x19E: case 0x1A0: case 0x1A2:
     case 0x1A4: case 0x1A6: case 0x1A8: case 0x1AA: case 0x1AC: case 0x1AE:
     case 0x1B0: case 0x1B2: case 0x1B4: case 0x1B6: case 0x1B8: case 0x1BA:
     case 0x1BC: case 0x1BE:
	COLOR (hpos, value & 0xFFF, (addr & 0x3E) / 2);
	break;
     case 0x120: case 0x124: case 0x128: case 0x12C:
     case 0x130: case 0x134: case 0x138: case 0x13C:
	SPRxPTH (hpos, value, (addr - 0x120) / 4);
	break;
     case 0x122: case 0x126: case 0x12A: case 0x12E:
     case 0x132: case 0x136: case 0x13A: case 0x13E:
	SPRxPTL (hpos, value, (addr - 0x122) / 4);
	break;
     case 0x140: case 0x148: case 0x150: case 0x158:
     case 0x160: case 0x168: case 0x170: case 0x178:
	SPRxPOS (hpos, value, (addr - 0x140) / 8);
	break;
     case 0x142: case 0x14A: case 0x152: case 0x15A:
     case 0x162: case 0x16A: case 0x172: case 0x17A:
	SPRxCTL (hpos, value, (addr - 0x142) / 8);
	break;
     case 0x144: case 0x14C: case 0x154: case 0x15C:
     case 0x164: case 0x16C: case 0x174: case 0x17C:
	SPRxDATA (hpos, value, (addr - 0x144) / 8);
	break;
     case 0x146: case 0x14E: case 0x156: case 0x15E:
     case 0x166: case 0x16E: case 0x176: case 0x17E:
	SPRxDATB (hpos, value, (addr - 0x146) / 8);
	break;

     case 0x36: JOYTEST (value); break;
     case 0x5A: BLTCON0L (value); break;
     case 0x5C: BLTSIZV (value); break;
     case 0x5E: BLTSIZH (value); break;
     case 0x1E4: DIWHIGH (hpos, value); break;
     case 0x10C: BPLCON4 (hpos, value); break;
     case 0x1FC: FMODE (value); break;
    }
}

void REGPARAM2 custom_wput (uaecptr addr, uae_u32 value)
{
    int hpos = current_hpos ();
    update_copper (hpos);
    custom_wput_1 (hpos, addr, value);
}

void REGPARAM2 custom_bput (uaecptr addr, uae_u32 value)
{
    static int warned = 0;
    /* Is this correct now? (There are people who bput things to the upper byte of AUDxVOL). */
    uae_u16 rval = (value << 8) | (value & 0xFF);
    custom_wput(addr, rval);
    if (!warned)
	write_log ("Byte put to custom register.\n"), warned++;
}

void REGPARAM2 custom_lput(uaecptr addr, uae_u32 value)
{
    custom_wput(addr & 0xfffe, value >> 16);
    custom_wput((addr+2) & 0xfffe, (uae_u16)value);
}

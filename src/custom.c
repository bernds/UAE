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

unsigned long int currcycle, nextevent, is_lastline;
static int rpt_did_reset;
struct ev eventtab[ev_max];

frame_time_t vsynctime, vsyncmintime;

static int vpos;
static uae_u16 lof;
static int next_lineno;
static enum nln_how nextline_how;
static int lof_changed = 0;

static uae_u32 sprtaba[256],sprtabb[256];
static uae_u32 sprite_ab_merge[256];
/* Tables for collision detection.  */
static uae_u32 sprclx[16], clxmask[16];

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

#define MAX_SPRITES 32

/* This is but an educated guess. It seems to be correct, but this stuff
 * isn't documented well. */
enum sprstate { SPR_stop, SPR_restart, SPR_waiting_start, SPR_waiting_stop };

struct sprite {
    uaecptr pt;
    int on;
    int xpos;
    int vstart;
    int vstop;
    int armed;
    enum sprstate state;
};

static struct sprite spr[8];

static int sprite_width;

static int sprite_vblank_endline = VBLANK_ENDLINE_NTSC + 2;

static unsigned int sprdata[MAX_SPRITES], sprdatb[MAX_SPRITES], sprctl[MAX_SPRITES], sprpos[MAX_SPRITES];
static int sprite_last_drawn_at[MAX_SPRITES];
static int last_sprite_point, nr_armed;

static uae_u32 bpl1dat, bpl2dat, bpl3dat, bpl4dat, bpl5dat, bpl6dat, bpl7dat, bpl8dat;
static uae_s16 bpl1mod, bpl2mod;

static uaecptr bplpt[8];
uae_u8 *real_bplpt[8];

/*static int blitcount[256];  blitter debug */

static struct color_entry current_colors;
static unsigned int bplcon0, bplcon1, bplcon2, bplcon3, bplcon4;
static int nr_planes_from_bplcon0, corrected_nr_planes_from_bplcon0;
static unsigned int diwstrt, diwstop, diwhigh;
static int diwhigh_written;
static unsigned int ddfstrt, ddfstop;

/* The display and data fetch windows */

enum diw_states
{
    DIW_waiting_start, DIW_waiting_stop
};

static int plffirstline, plflastline;
static int plfstrt, plfstop;
static int last_diw_pix_hpos, last_ddf_pix_hpos, last_decide_line_hpos;
static int last_fetch_hpos, last_sprite_hpos;
int diwfirstword, diwlastword;
static enum diw_states diwstate, hdiwstate;

/* Sprite collisions */
static unsigned int clxdat, clxcon, clxcon2, clxcon_bpl_enable, clxcon_bpl_match;
static int clx_sprmask;

enum copper_states {
    COP_stop,
    COP_read1, COP_read2,
    COP_bltwait,
    COP_wait,
    COP_wait1
};

struct copper {
    /* The current instruction words.  */
    unsigned int i1, i2;
    enum copper_states state;
    /* Instruction pointer.  */
    uaecptr ip;
    int hpos, vpos;
    unsigned int ignore_next;
    int vcmp, hcmp;
};

static struct copper cop_state;
static int copper_enabled_thisline;
static int cop_min_waittime;

/*
 * Statistics
 */

/* Used also by bebox.cpp */
unsigned long int msecs = 0, frametime = 0, lastframetime = 0, timeframes = 0;
static unsigned long int seconds_base;
int bogusframe;
int n_frames;

static int current_change_set;

#ifdef OS_WITHOUT_MEMORY_MANAGEMENT
/* sam: Those arrays uses around 7Mb of BSS... That seems  */
/* too much for AmigaDOS (uae crashes as soon as one loads */
/* it. So I use a different strategy here (realloc the     */
/* arrays when needed. That strategy might be usefull for  */
/* computer with low memory.                               */
struct sprite_entry  *sprite_entries[2];
struct color_change *color_changes[2];
static int max_sprite_entry = 400;
static int delta_sprite_entry = 0;
static int max_color_change = 400;
static int delta_color_change = 0;
#else
struct sprite_entry sprite_entries[2][MAX_SPR_PIXELS / 16];
struct color_change color_changes[2][MAX_REG_CHANGE];
#endif

struct decision line_decisions[2 * (MAXVPOS + 1) + 1];
struct draw_info line_drawinfo[2][2 * (MAXVPOS + 1) + 1];
struct color_entry color_tables[2][(MAXVPOS + 1) * 2];

static int next_sprite_entry = 0;
static int prev_next_sprite_entry;
static int next_sprite_forced = 1;

struct sprite_entry *curr_sprite_entries, *prev_sprite_entries;
struct color_change *curr_color_changes, *prev_color_changes;
struct draw_info *curr_drawinfo, *prev_drawinfo;
struct color_entry *curr_color_tables, *prev_color_tables;

static int next_color_change;
static int next_color_entry, remembered_color_entry;
static int color_src_match, color_dest_match, color_compare_result;

/* These few are only needed during/at the end of the scanline, and don't
 * have to be remembered. */
static int decided_bpl1mod, decided_bpl2mod, decided_nr_planes;

static uae_u32 thisline_changed;

#ifdef SMART_UPDATE
#define MARK_LINE_CHANGED do { thisline_changed = 1; } while (0)
#else
#define MARK_LINE_CHANGED do { ; } while (0)
#endif

static struct decision thisline_decision;
static int modulos_added, plane_decided, color_decided;
static int passed_plfstop, fetch_cycle;

enum fetchstate {
    fetch_not_started,
    fetch_started,
    fetch_was_plane0
} fetch_state;

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
    return (get_cycles () - eventtab[ev_hsync].oldcycles) / CYCLE_UNIT;
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
    int i;

    if (currprefs.chipset_mask & CSMASK_AGA) {
        for (i = 0; i < 256; i++) {
	    int v = color_reg_get (colentry, i);
	    if (v < 0 || v > 16777215)
		continue;
	    colentry->acolors[i] = CONVERT_RGB (v);
	}
    } else {
        for (i = 0; i < 32; i++) {
	    int v = color_reg_get (colentry, i);
	    if (v < 0 || v > 4095)
		continue;
	    colentry->acolors[i] = xcolors[v];
	}
    }
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
	color_reg_cpy (curr_color_tables + next_color_entry, &current_colors);
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
	    color_compare_result = color_reg_cmp (&prev_color_tables[oldctable], &current_colors) != 0;
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
    int pix_hpos = coord_diw_to_window_x (hpos * 2);
    if (hdiwstate == DIW_waiting_start && thisline_decision.diwfirstword == -1
	&& pix_hpos >= diwfirstword && last_diw_pix_hpos < diwfirstword)
    {
	thisline_decision.diwfirstword = diwfirstword < 0 ? 0 : diwfirstword;
	hdiwstate = DIW_waiting_stop;
	thisline_decision.diwlastword = -1;
    }
    if (hdiwstate == DIW_waiting_stop && thisline_decision.diwlastword == -1
	&& pix_hpos >= diwlastword && last_diw_pix_hpos < diwlastword)
    {
	thisline_decision.diwlastword = diwlastword < 0 ? 0 : diwlastword;
	hdiwstate = DIW_waiting_start;
    }
    last_diw_pix_hpos = pix_hpos;
}

/* The HRM says 0xD8, but that can't work... */
#define HARD_DDF_STOP (0xD4)

static void finish_playfield_line (void)
{
    /* The latter condition might be able to happen in interlaced frames. */
    if (vpos >= minfirstline && (thisframe_first_drawn_line == -1 || vpos < thisframe_first_drawn_line))
	thisframe_first_drawn_line = vpos;
    thisframe_last_drawn_line = vpos;

    if (dmaen (DMA_BITPLANE))
	switch (nr_planes_from_bplcon0) {
	case 8: bplpt[7] += bpl2mod;
	case 7: bplpt[6] += bpl1mod;
	case 6: bplpt[5] += bpl2mod;
	case 5: bplpt[4] += bpl1mod;
	case 4: bplpt[3] += bpl2mod;
	case 3: bplpt[2] += bpl1mod;
	case 2: bplpt[1] += bpl2mod;
	case 1: bplpt[0] += bpl1mod;
	}

    /* These are for comparison. */
    thisline_decision.bplcon0 = bplcon0;
    thisline_decision.bplcon2 = bplcon2;
    thisline_decision.bplcon3 = bplcon3;
    thisline_decision.bplcon4 = bplcon4;

#ifdef SMART_UPDATE
    if (line_decisions[next_lineno].plflinelen != thisline_decision.plflinelen
	|| line_decisions[next_lineno].plfleft != thisline_decision.plfleft
	|| line_decisions[next_lineno].bplcon0 != thisline_decision.bplcon0
	|| line_decisions[next_lineno].bplcon2 != thisline_decision.bplcon2
	|| line_decisions[next_lineno].bplcon3 != thisline_decision.bplcon3
	|| line_decisions[next_lineno].bplcon4 != thisline_decision.bplcon4
	)
#endif /* SMART_UPDATE */
	thisline_changed = 1;
}

static int fetchunits[] = { 8,8,8,0, 16,8,8,0, 32,16,8,0 };
static int fetchstarts[] = { 3,2,1,0, 4,3,2,0, 5,4,3,0 };
static int fm_maxplanes[] = { 3,2,1,0, 3,3,2,0, 3,3,3,0 }; 

static int fetchmode, fetchunit, fetchunit_mask;
static int fetchstart, fetchstart_shift, fetchstart_mask;
static int fm_maxplane, fm_maxplane_shift;

static int estimated_last_fetch_cycle;

static void estimate_last_fetch_cycle (int hpos)
{
    int fetchunit = fetchunits[fetchmode * 4 + GET_RES (bplcon0)];

    if (! passed_plfstop) {
	int stop = plfstop < hpos || plfstop > HARD_DDF_STOP ? HARD_DDF_STOP : plfstop;
	/* We know that fetching is up-to-date up until hpos, so we can use fetch_cycle.  */
	int fetch_cycle_at_stop = fetch_cycle + (stop - hpos);
	int starting_last_block_at = (fetch_cycle_at_stop + fetchunit - 1) & ~(fetchunit - 1);

	estimated_last_fetch_cycle = hpos + (starting_last_block_at - fetch_cycle) + fetchunit;
    } else {
	int starting_last_block_at = (fetch_cycle + fetchunit - 1) & ~(fetchunit - 1);
	if (passed_plfstop == 2)
	    starting_last_block_at -= fetchunit;

	estimated_last_fetch_cycle = hpos + (starting_last_block_at - fetch_cycle) + fetchunit;
    }
}

static uae_u32 outword[MAX_PLANES];
static int out_nbits, out_offs;
static uae_u32 todisplay[MAX_PLANES][4];
static uae_u32 fetched[MAX_PLANES];
static uae_u32 fetched_aga0[MAX_PLANES];
static uae_u32 fetched_aga1[MAX_PLANES];

/* Expansions from bplcon0/bplcon1.  */
static int toscr_res, toscr_delay1, toscr_delay2, toscr_nr_planes, fetchwidth;

/* The number of bits left from the last fetched words.  
   This is an optimization - conceptually, we have to make sure the result is
   the same as if toscr is called in each clock cycle.  However, to speed this
   up, we accumulate display data; this variable keeps track of how much. 
   Thus, once we do call toscr_nbits (which happens at least every 16 bits),
   we can do more work at once.  */
static int toscr_nbits;

static int delayoffset;

STATIC_INLINE void compute_delay_offset (int hpos)
{
    /* this fixes most horizontal scrolling jerkyness but can't be correct */
    delayoffset = ((hpos - 0x18) & fetchstart_mask) << 1;
    delayoffset &= ~7;
    delayoffset &= 31;
}

STATIC_INLINE void maybe_first_bpl1dat (int hpos)
{
    if (thisline_decision.plfleft == -1) {
	thisline_decision.plfleft = hpos;
	compute_delay_offset (hpos);
    }
}

STATIC_INLINE void fetch (int nr, int fm)
{
    if (nr >= toscr_nr_planes)
	return;
    switch (fm) {
    case 0:
	fetched[nr] = chipmem_wget (bplpt[nr]);
	bplpt[nr] += 2;
	break;
    case 1:
	fetched_aga0[nr] = chipmem_lget (bplpt[nr]);
	bplpt[nr] += 4;
	break;
    case 2:
	fetched_aga1[nr] = chipmem_lget (bplpt[nr]);
	fetched_aga0[nr] = chipmem_lget (bplpt[nr] + 4);
	bplpt[nr] += 8;
	break;
    }
    if (nr == 0)
	fetch_state = fetch_was_plane0;
}

static void expand_fmodes(void)
{
    int res = GET_RES(bplcon0);
    int fm = fetchmode;
    fetchunit = fetchunits[fm * 4 + res];
    fetchunit_mask = fetchunit - 1;
    fetchstart_shift = fetchstarts[fm * 4 + res];
    fetchstart = 1 << fetchstart_shift;
    fetchstart_mask = fetchstart - 1;
    fm_maxplane_shift = fm_maxplanes[fm * 4 + res];
    fm_maxplane = 1 << fm_maxplane_shift;
}

/* Expand bplcon0/bplcon1 into the toscr_xxx variables.  */
static void compute_toscr_delay (int hpos)
{
    int delay1 = (bplcon1 & 0x0f) | ((bplcon1 & 0x0c00) >> 6);
    int delay2 = ((bplcon1 >> 4) & 0x0f) | (((bplcon1 >> 4) & 0x0c00) >> 6);
    int delaymask;
    int fetchwidth = 16 << fetchmode;

    toscr_res = GET_RES (bplcon0);
    delay1 += delayoffset;
    delay2 += delayoffset;
    delaymask = (fetchwidth - 1) >> toscr_res;
    toscr_delay1 = (delay1 & delaymask) << toscr_res;
    toscr_delay2 = (delay2 & delaymask) << toscr_res;

    toscr_nr_planes = GET_PLANES (bplcon0);
}

static void clear_fetchbuffer (uae_u32 *ptr, int nwords)
{
    int i;

    if (! thisline_changed)
	for (i = 0; i < nwords; i++)
	    if (ptr[i]) {
		thisline_changed = 1;
		break;
	    }

    memset (ptr, 0, nwords * 4);
}

static void update_toscr_planes (void)
{
    if (toscr_nr_planes > thisline_decision.nr_planes) {
	int j;
	for (j = thisline_decision.nr_planes; j < toscr_nr_planes; j++)
	    clear_fetchbuffer ((uae_u32 *)(line_data[next_lineno] + 2 * MAX_WORDS_PER_LINE * j), out_offs);
#if 0
	if (thisline_decision.nr_planes > 0)
	    printf ("Planes from %d to %d\n", thisline_decision.nr_planes, toscr_nr_planes);
#endif
	thisline_decision.nr_planes = toscr_nr_planes;
    }
}

STATIC_INLINE void toscr_3_ecs (int nbits)
{
    int delay1 = toscr_delay1;
    int delay2 = toscr_delay2;
    int i;
    uae_u32 mask = 0xFFFF >> (16 - nbits);

    for (i = 0; i < toscr_nr_planes; i += 2) {
	outword[i] <<= nbits;
	outword[i] |= (todisplay[i][0] >> (16 - nbits + delay1)) & mask;
	todisplay[i][0] <<= nbits;
    }
    for (i = 1; i < toscr_nr_planes; i += 2) {
	outword[i] <<= nbits;
	outword[i] |= (todisplay[i][0] >> (16 - nbits + delay2)) & mask;
	todisplay[i][0] <<= nbits;
    }
}

STATIC_INLINE void shift32plus (uae_u32 *p, int n)
{
    uae_u32 t = p[1];
    t <<= n;
    t |= p[0] >> (32 - n);
    p[1] = t;
}

STATIC_INLINE void aga_shift (uae_u32 *p, int n, int fm)
{
    if (fm == 2) {
	shift32plus (p + 2, n);
	shift32plus (p + 1, n);
    }
    shift32plus (p + 0, n);
    p[0] <<= n;
}

STATIC_INLINE void toscr_3_aga (int nbits, int fm)
{
    int delay1 = toscr_delay1;
    int delay2 = toscr_delay2;
    int i;
    uae_u32 mask = 0xFFFF >> (16 - nbits);

    {
	int offs = (16 << fm) - nbits + delay1;
	int off1 = offs >> 5;
	if (off1 == 3)
	    off1 = 2;
	offs -= off1 * 32;
	for (i = 0; i < toscr_nr_planes; i += 2) {
	    uae_u32 t0 = todisplay[i][off1];
	    uae_u32 t1 = todisplay[i][off1 + 1];
	    uae_u64 t = (((uae_u64)t1) << 32) | t0;
	    outword[i] <<= nbits;
	    outword[i] |= (t >> offs) & mask;
	    aga_shift (todisplay[i], nbits, fm);
	}
    }
    {
	int offs = (16 << fm) - nbits + delay2;
	int off1 = offs >> 5;
	if (off1 == 3)
	    off1 = 2;
	offs -= off1 * 32;
	for (i = 1; i < toscr_nr_planes; i += 2) {
	    uae_u32 t0 = todisplay[i][off1];
	    uae_u32 t1 = todisplay[i][off1 + 1];
	    uae_u64 t = (((uae_u64)t1) << 32) | t0;
	    outword[i] <<= nbits;
	    outword[i] |= (t >> offs) & mask;
	    aga_shift (todisplay[i], nbits, fm);
	}
    }
}

static void toscr_2_0 (int nbits) { toscr_3_ecs (nbits); }
static void toscr_2_1 (int nbits) { toscr_3_aga (nbits, 1); }
static void toscr_2_2 (int nbits) { toscr_3_aga (nbits, 2); }

STATIC_INLINE void toscr_1 (int nbits, int fm)
{
    switch (fm) {
    case 0:
	toscr_2_0 (nbits);
	break;
    case 1:
	toscr_2_1 (nbits);
	break;
    case 2:
	toscr_2_2 (nbits);
	break;
    }

    out_nbits += nbits;
    if (out_nbits == 32) {
	int i;
	uae_u8 *dataptr = line_data[next_lineno] + out_offs * 4;
	/* Don't use toscr_nr_planes here; if the plane count drops during the
	   line we still want the data to be correct for the full number of planes
	   over the full width of the line.  */
	for (i = 0; i < thisline_decision.nr_planes; i++) {
	    uae_u32 *dataptr32 = (uae_u32 *)dataptr;
	    if (*dataptr32 != outword[i])
		thisline_changed = 1;
	    *dataptr32 = outword[i];
	    dataptr += MAX_WORDS_PER_LINE * 2;
	}
	out_offs++;
	out_nbits = 0;
    }
}

static void toscr_fm0 (int);
static void toscr_fm1 (int);
static void toscr_fm2 (int);

STATIC_INLINE void toscr (int nbits, int fm)
{
    switch (fm) {
    case 0: toscr_fm0 (nbits); break;
    case 1: toscr_fm1 (nbits); break;
    case 2: toscr_fm2 (nbits); break;
    }
}

STATIC_INLINE void toscr_0 (int nbits, int fm)
{
    int t;

    if (nbits > 16) {
	toscr (16, fm);
	nbits -= 16;
    }

    t = 32 - out_nbits;
    if (t < nbits) {
	toscr_1 (t, fm);
	nbits -= t;
    }
    toscr_1 (nbits, fm);
}

static void toscr_fm0 (int nbits) { toscr_0 (nbits, 0); }
static void toscr_fm1 (int nbits) { toscr_0 (nbits, 1); }
static void toscr_fm2 (int nbits) { toscr_0 (nbits, 2); }

static int flush_plane_data (int fm)
{
    int i = 0;
    int fetchwidth = 16 << fm;

    if (out_nbits <= 16) {
	i += 16;
	toscr_1 (16, fm);
    }
    if (out_nbits != 0) {
	i += 32 - out_nbits;
	toscr_1 (32 - out_nbits, fm);
    }
    i += 32;

    toscr_1 (16, fm);
    toscr_1 (16, fm);
    return i >> (1 + toscr_res);
}

STATIC_INLINE void flush_display (int fm)
{
    if (toscr_nbits > 0 && thisline_decision.plfleft != -1)
	toscr (toscr_nbits, fm);
    toscr_nbits = 0;
}

/* Called when all planes have been fetched, i.e. when a new block
   of data is available to be displayed.  The data in fetched[] is
   moved into todisplay[].  */
STATIC_INLINE void beginning_of_plane_block (int pos, int dma, int fm)
{
    int i;

    flush_display (fm);

    if (fm == 0)
	for (i = 0; i < MAX_PLANES; i++) 
	    todisplay[i][0] |= fetched[i];
    else
	for (i = 0; i < MAX_PLANES; i++) {
	    if (fm == 2)
		todisplay[i][1] = fetched_aga1[i];
	    todisplay[i][0] = fetched_aga0[i];
	}

    maybe_first_bpl1dat (pos);
}

#define SPEEDUP

#ifdef SPEEDUP

/* The usual inlining tricks - don't touch unless you know what you are doing.  */
STATIC_INLINE void long_fetch_ecs (int plane, int nwords, int weird_number_of_bits, int dma)
{
    uae_u16 *real_pt = (uae_u16 *)pfield_xlateptr (bplpt[plane], nwords * 2);
    int delay = ((plane & 1) ? toscr_delay2 : toscr_delay1);
    int tmp_nbits = out_nbits;
    uae_u32 shiftbuffer = todisplay[plane][0];
    uae_u32 outval = outword[plane];
    uae_u32 fetchval = fetched[plane];
    uae_u32 *dataptr = (uae_u32 *)(line_data[next_lineno] + 2 * plane * MAX_WORDS_PER_LINE + 4 * out_offs);

    if (dma)
	bplpt[plane] += nwords * 2;

    if (real_pt == 0)
	/* @@@ Don't do this, fall back on chipmem_wget instead.  */
	return;

    while (nwords > 0) {
	int bits_left = 32 - tmp_nbits;
	uae_u32 t;

	shiftbuffer |= fetchval;

	t = (shiftbuffer >> delay) & 0xFFFF;

	if (weird_number_of_bits && bits_left < 16) {
	    outval <<= bits_left;
	    outval |= t >> (16 - bits_left);
	    thisline_changed |= *dataptr ^ outval;
	    *dataptr++ = outval;

	    outval = t;
	    tmp_nbits = 16 - bits_left;
	    shiftbuffer <<= 16;
	} else {
	    outval = (outval << 16) | t;
	    shiftbuffer <<= 16;
	    tmp_nbits += 16;
	    if (tmp_nbits == 32) {
		thisline_changed |= *dataptr ^ outval;
		*dataptr++ = outval;
		tmp_nbits = 0;
	    }
	}
	nwords--;
	if (dma) {
	    fetchval = do_get_mem_word (real_pt);
	    real_pt++;
	}
    }
    fetched[plane] = fetchval;
    todisplay[plane][0] = shiftbuffer;
    outword[plane] = outval;
}

STATIC_INLINE void long_fetch_aga (int plane, int nwords, int weird_number_of_bits, int fm, int dma)
{
    uae_u32 *real_pt = (uae_u32 *)pfield_xlateptr (bplpt[plane], nwords * 2);
    int delay = ((plane & 1) ? toscr_delay2 : toscr_delay1);
    int tmp_nbits = out_nbits;
    uae_u32 *shiftbuffer = todisplay[plane];
    uae_u32 outval = outword[plane];
    uae_u32 fetchval0 = fetched_aga0[plane];
    uae_u32 fetchval1 = fetched_aga1[plane];
    uae_u32 *dataptr = (uae_u32 *)(line_data[next_lineno] + 2 * plane * MAX_WORDS_PER_LINE + 4 * out_offs);
    int offs = (16 << fm) - 16 + delay;
    int off1 = offs >> 5;
    if (off1 == 3)
	off1 = 2;
    offs -= off1 * 32;

    if (dma)
	bplpt[plane] += nwords * 2;

    if (real_pt == 0)
	/* @@@ Don't do this, fall back on chipmem_wget instead.  */
	return;

    while (nwords > 0) {
	int i;

	shiftbuffer[0] = fetchval0;
	if (fm == 2)
	    shiftbuffer[1] = fetchval1;

	for (i = 0; i < (1 << fm); i++) {
	    int bits_left = 32 - tmp_nbits;

	    uae_u32 t0 = shiftbuffer[off1];
	    uae_u32 t1 = shiftbuffer[off1 + 1];
	    uae_u64 t = (((uae_u64)t1) << 32) | t0;

	    t0 = (t >> offs) & 0xFFFF;

	    if (weird_number_of_bits && bits_left < 16) {
		outval <<= bits_left;
		outval |= t0 >> (16 - bits_left);

		thisline_changed |= *dataptr ^ outval;
		*dataptr++ = outval;

		outval = t0;
		tmp_nbits = 16 - bits_left;
		aga_shift (shiftbuffer, 16, fm);
	    } else {
		outval = (outval << 16) | t0;
		aga_shift (shiftbuffer, 16, fm);
		tmp_nbits += 16;
		if (tmp_nbits == 32) {
		    thisline_changed |= *dataptr ^ outval;
		    *dataptr++ = outval;
		    tmp_nbits = 0;
		}
	    }
	}

	nwords -= 1 << fm;

	if (dma) {
	    if (fm == 1)
		fetchval0 = do_get_mem_long (real_pt);
	    else {
		fetchval1 = do_get_mem_long (real_pt);
		fetchval0 = do_get_mem_long (real_pt + 1);
	    }
	    real_pt += fm;
	}
    }
    fetched_aga0[plane] = fetchval0;
    fetched_aga1[plane] = fetchval1;
    outword[plane] = outval;
}

static void long_fetch_ecs_0 (int hpos, int nwords, int dma) { long_fetch_ecs (hpos, nwords, 0, dma); }
static void long_fetch_ecs_1 (int hpos, int nwords, int dma) { long_fetch_ecs (hpos, nwords, 1, dma); }
static void long_fetch_aga_1_0 (int hpos, int nwords, int dma) { long_fetch_aga (hpos, nwords,  0, 1, dma); }
static void long_fetch_aga_1_1 (int hpos, int nwords, int dma) { long_fetch_aga (hpos, nwords,  1, 1, dma); }
static void long_fetch_aga_2_0 (int hpos, int nwords, int dma) { long_fetch_aga (hpos, nwords,  0, 2, dma); }
static void long_fetch_aga_2_1 (int hpos, int nwords, int dma) { long_fetch_aga (hpos, nwords,  1, 2, dma); }

static void do_long_fetch (int hpos, int nwords, int dma, int fm)
{
    int added;
    int i;

    flush_display (fm);
    switch (fm) {
    case 0:
	if (out_nbits & 15) {
	    for (i = 0; i < toscr_nr_planes; i++)
		long_fetch_ecs_1 (i, nwords, dma);
	} else {
	    for (i = 0; i < toscr_nr_planes; i++)
		long_fetch_ecs_0 (i, nwords, dma);
	}
	break;
    case 1:
	if (out_nbits & 15) {
	    for (i = 0; i < toscr_nr_planes; i++)
		long_fetch_aga_1_1 (i, nwords, dma);
	} else {
	    for (i = 0; i < toscr_nr_planes; i++)
		long_fetch_aga_1_0 (i, nwords, dma);
	}
	break;
    case 2:
	if (out_nbits & 15) {
	    for (i = 0; i < toscr_nr_planes; i++)
		long_fetch_aga_2_1 (i, nwords, dma);
	} else {
	    for (i = 0; i < toscr_nr_planes; i++)
		long_fetch_aga_2_0 (i, nwords, dma);
	}
	break;
    }

    out_nbits += nwords * 16;
    out_offs += out_nbits >> 5;
    out_nbits &= 31;

    if (dma && toscr_nr_planes > 0)
	fetch_state = fetch_was_plane0;
}

#endif

/* make sure fetch that goes beyond maxhpos is finished */
static void finish_final_fetch (int i, int fm)
{
    passed_plfstop = 3;
    i += flush_plane_data (fm);
    thisline_decision.plfright = i;
    thisline_decision.plflinelen = out_offs;
    thisline_decision.bplres = toscr_res;
    finish_playfield_line ();
}

STATIC_INLINE int one_fetch_cycle_0 (int i, int ddfstop_to_test, int dma, int fm)
{
    if (! passed_plfstop && i == ddfstop_to_test)
	passed_plfstop = 1;

    if ((fetch_cycle & fetchunit_mask) == 0) {
	if (passed_plfstop == 2) {
	    finish_final_fetch (i, fm);
	    return 1;
	}
	if (passed_plfstop)
	    passed_plfstop++;
    }
    if (dma) {
	/* fetchstart_mask can be larger than fm_maxplane if FMODE > 0.  This means
	   that the remaining cycles are idle; we'll fall through the whole switch
	   without doing anything.  */
	int cycle_start = fetch_cycle & fetchstart_mask;
	switch (fm_maxplane) {
	case 8:
	    switch (cycle_start) {
	    case 0: fetch (7, fm); break;
	    case 1: fetch (3, fm); break;
	    case 2: fetch (5, fm); break;
	    case 3: fetch (1, fm); break;
	    case 4: fetch (6, fm); break;
	    case 5: fetch (2, fm); break;
	    case 6: fetch (4, fm); break;
	    case 7: fetch (0, fm); break;
	    }
	    break;
	case 4:
	    switch (cycle_start) {
	    case 0: fetch (3, fm); break;
	    case 1: fetch (1, fm); break;
	    case 2: fetch (2, fm); break;
	    case 3: fetch (0, fm); break;
	    }
	    break;
	case 2:
	    switch (cycle_start) {
	    case 0: fetch (1, fm); break;
	    case 1: fetch (0, fm); break;
	    }
	    break;
	}
    }
    fetch_cycle++;
    toscr_nbits += 2 << toscr_res;
    return 0;
}

static int one_fetch_cycle_fm0 (int i, int ddfstop_to_test, int dma) { return one_fetch_cycle_0 (i, ddfstop_to_test, dma, 0); }
static int one_fetch_cycle_fm1 (int i, int ddfstop_to_test, int dma) { return one_fetch_cycle_0 (i, ddfstop_to_test, dma, 1); }
static int one_fetch_cycle_fm2 (int i, int ddfstop_to_test, int dma) { return one_fetch_cycle_0 (i, ddfstop_to_test, dma, 2); }

STATIC_INLINE int one_fetch_cycle (int i, int ddfstop_to_test, int dma, int fm)
{
    switch (fm) {
    case 0: return one_fetch_cycle_fm0 (i, ddfstop_to_test, dma);
    case 1: return one_fetch_cycle_fm1 (i, ddfstop_to_test, dma);
    case 2: return one_fetch_cycle_fm2 (i, ddfstop_to_test, dma);
    default: abort ();
    }
}

STATIC_INLINE void update_fetch (int until, int fm)
{
    int pos;
    int dma = dmaen (DMA_BITPLANE);

    int ddfstop_to_test;

    if (framecnt != 0 || passed_plfstop == 3)
	return;

    /* We need an explicit test against HARD_DDF_STOP here to guard against
       programs that move the DDFSTOP before our current position before we
       reach it.  */
    ddfstop_to_test = HARD_DDF_STOP;
    if (ddfstop >= last_fetch_hpos && ddfstop < HARD_DDF_STOP)
	ddfstop_to_test = ddfstop;

    compute_toscr_delay (last_fetch_hpos);
    update_toscr_planes ();

    pos = last_fetch_hpos;

    /* First, a loop that prepares us for the speedup code.  We want to enter
       the SPEEDUP case with fetch_state == fetch_was_plane0, and then unroll
       whole blocks, so that we end on the same fetch_state again.  */
    for (; ; pos++) {
	if (pos == until) {
	    if (until >= maxhpos && passed_plfstop == 2) {
		finish_final_fetch (pos, fm);
		return;
	    }
	    flush_display (fm);
	    return;
	}

	if (fetch_state == fetch_was_plane0)
	    break;

	fetch_state = fetch_started;
	if (one_fetch_cycle (pos, ddfstop_to_test, dma, fm))
	    return;

	if (toscr_nbits == 16)
	    flush_display (fm);
	if (toscr_nbits > 16)
	    abort ();
    }

#ifdef SPEEDUP
    /* Unrolled version of the for loop below.  */
    if (! passed_plfstop
	&& dma
	&& (pos & fetchstart_mask) == (fm_maxplane & fetchstart_mask)
# if 0
	/* @@@ We handle this case, but the code would be simpler if we
	 * disallowed it - it may even be possible to guarantee that
	 * this condition never is false.  Later.  */
	&& (out_nbits & 15) == 0
# endif
	&& toscr_nr_planes == thisline_decision.nr_planes)
    {
	int offs = (pos - fetch_cycle) & 7;
	int ddf2 = ((ddfstop_to_test - offs + fetchunit - 1) & ~fetchunit_mask) + offs;
	int ddf3 = ddf2 + fetchunit;
	int stop = until < ddf2 ? until : until < ddf3 ? ddf2 : ddf3;
	int count;

	count = stop - pos;

	if (count >= fetchstart) {
	    count &= ~fetchstart_mask;

	    if (thisline_decision.plfleft == -1)
		compute_delay_offset (pos);

	    do_long_fetch (pos, count >> (3 - toscr_res), dma, fm);

	    /* This must come _after_ do_long_fetch so as not to confuse flush_display
	       into thinking the first fetch has produced any output worth emitting to
	       the screen.  But the calculation of delay_offset must happen _before_.  */
	    maybe_first_bpl1dat (pos);

	    if (pos <= ddfstop_to_test && pos + count > ddfstop_to_test)
		passed_plfstop = 1;
	    if (pos <= ddfstop_to_test && pos + count > ddf2)
		passed_plfstop = 2;
	    pos += count;
	    fetch_cycle += count;
	}
    }
#endif
    for (; pos < until; pos++) {
	if (fetch_state == fetch_was_plane0)
	    beginning_of_plane_block (pos, dma, fm);
	fetch_state = fetch_started;

	if (one_fetch_cycle (pos, ddfstop_to_test, dma, fm))
	    return;

	if (toscr_nbits == 16)
	    flush_display (fm);
	if (toscr_nbits > 16)
	    abort ();
    }
    if (until >= maxhpos && passed_plfstop == 2) {
	finish_final_fetch (pos, fm);
	return;
    }
    flush_display (fm);
}

static void update_fetch_0 (int hpos) { update_fetch (hpos, 0); }
static void update_fetch_1 (int hpos) { update_fetch (hpos, 1); }
static void update_fetch_2 (int hpos) { update_fetch (hpos, 2); }

STATIC_INLINE void decide_fetch (int hpos)
{
    if (fetch_state != fetch_not_started && hpos > last_fetch_hpos) {
	switch (fetchmode) {
	case 0: update_fetch_0 (hpos); break;
	case 1: update_fetch_1 (hpos); break;
	case 2: update_fetch_2 (hpos); break;
	default: abort ();
	}
    }
    last_fetch_hpos = hpos;
}

/* This function is responsible for turning on datafetch if necessary.  */
STATIC_INLINE void decide_line (int hpos)
{
    if (fetch_state != fetch_not_started)
	return;

    /* Test if we passed the start of the DDF window.  */
    if (last_decide_line_hpos < plfstrt && hpos >= plfstrt) {
	/* First, take care of the vertical DIW.  Surprisingly enough, this seems to be
	   correct here - putting this into decide_diw() results in garbage.  */
	if (diwstate == DIW_waiting_start && vpos == plffirstline) {
	    diwstate = DIW_waiting_stop;
	}
	if (diwstate == DIW_waiting_stop && vpos == plflastline) {
	    diwstate = DIW_waiting_start;
	}

	if (diwstate == DIW_waiting_stop) {
	    fetch_state = fetch_started;
	    fetch_cycle = 0;
	    last_fetch_hpos = plfstrt;
	    out_nbits = 0;
	    out_offs = 0;
	    toscr_nbits = 0;

	    compute_toscr_delay (last_fetch_hpos);

	    /* If someone already wrote BPL1DAT, clear the area between that point and
	       the real fetch start.  */
	    if (framecnt == 0) {
		if (thisline_decision.plfleft != -1) {
		    out_nbits = (plfstrt - thisline_decision.plfleft) << (1 + toscr_res);
		    out_offs = out_nbits >> 5;
		    out_nbits &= 31;
		}
		update_toscr_planes ();
	    }
	    estimate_last_fetch_cycle (plfstrt);
	    last_decide_line_hpos = hpos;
	    do_sprites (vpos, plfstrt);
	    return;
	}
    }

    if (last_decide_line_hpos < 0x34)
	do_sprites (vpos, hpos);

    last_decide_line_hpos = hpos;
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

    if (thisline_decision.ctable == -1)
	remember_ctable ();

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

/* Sprite-to-sprite collisions are taken care of in record_sprite.  This one does
   playfield/sprite collisions.
   That's the theory.  In practice this doesn't work yet.  I also suspect this code
   is way too slow.  */
static void do_sprite_collisions (void)
{
    int nr_sprites = curr_drawinfo[next_lineno].nr_sprites;
    int first = curr_drawinfo[next_lineno].first_sprite_entry;
    int i;
    unsigned int collision_mask = clxmask[clxcon >> 12];
    int plf_first_pixel = thisline_decision.plfleft * 2 + DIW_DDF_OFFSET;

    for (i = 0; i < nr_sprites; i++) {
	struct sprite_entry *e = curr_sprite_entries + first + i;
	int j;
	int minpos = e->pos;
	int maxpos = e->max;

	if (maxpos > thisline_decision.diwlastword)
	    maxpos = thisline_decision.diwlastword;
	if (maxpos > thisline_decision.plfright * 2 + DIW_DDF_OFFSET)
	    maxpos = thisline_decision.plfright * 2 + DIW_DDF_OFFSET;
	if (minpos < thisline_decision.diwfirstword)
	    minpos = thisline_decision.diwfirstword;
	if (minpos < plf_first_pixel)
	    minpos = plf_first_pixel;

	for (j = minpos; j < maxpos; j++) {
	    int sprpix = spixels[e->first_pixel + j - e->pos] & collision_mask;
	    int k;

	    if (sprpix == 0)
		continue;

	    sprpix = sprite_ab_merge[sprpix & 255] | (sprite_ab_merge[sprpix >> 8] << 2);
	    sprpix <<= 1;

	    /* Loop over number of playfields.  */
	    for (k = 0; k < 2; k++) {
		int l;
		int match = 1;
		int planes = ((currprefs.chipset_mask & CSMASK_AGA) ? 8 : 6);

		for (l = k; match && l < planes; l += 2)
		    if (clxcon_bpl_enable & (1 << l)) {
			int t = 0;
			if (l < thisline_decision.nr_planes) {
			    int offs = j - plf_first_pixel;
			    uae_u32 *ldata = (uae_u32 *)(line_data[next_lineno] + 2 * l * MAX_WORDS_PER_LINE);
			    uae_u32 word = ldata[offs >> 5];
			    t = (word >> (31 - (offs & 31))) & 1;
			}
			if (t != ((clxcon_bpl_match >> l) & 1))
			    match = 0;
		    }
		if (match)
		    clxdat |= sprpix;
		sprpix <<= 4;
	    }
	}
    }
}

/* DATAB contains the sprite data; 16 pixels in two-bit packets.  Bits 0/1
   determine the color of the leftmost pixel, bits 2/3 the color of the next
   etc.
   This function assumes that for all sprites in a given line, SPRXP either
   stays equal or increases between successive calls.  */
static void record_sprite (int line, int num, int sprxp, uae_u32 datab, unsigned int ctl)
{
    struct sprite_entry *e = curr_sprite_entries + next_sprite_entry;
    int i;
    int word_offs;
    uae_u16 *buf;
    uae_u32 collision_mask;
    int collision_enabled;

    if (currprefs.gfx_lores == 0)
	sprxp >>= 1;

    /* Try to coalesce entries if they aren't too far apart.  */
    if (! next_sprite_forced && e[-1].max + 16 >= sprxp)
	e--;
    else {
	next_sprite_entry++;
	e->pos = sprxp;
	e->has_attached = 0;
    }
    if (sprxp < e->pos)
	abort ();
    e->max = sprxp + 16;
    e[1].first_pixel = e->first_pixel + ((e->max - e->pos + 3) & ~3);
    next_sprite_forced = 0;

    collision_mask = clxmask[clxcon >> 12];
    word_offs = e->first_pixel + sprxp - e->pos;
    buf = spixels + word_offs;
    while (datab) {
	unsigned int tmp = *buf;
	unsigned int col = (datab & 3) << (2 * num);
	tmp |= col;
	*buf++ = tmp;
	datab >>= 2;
	tmp &= collision_mask;
	if (tmp) {
	    unsigned int shrunk_tmp = sprite_ab_merge[tmp & 255] | (sprite_ab_merge[tmp >> 8] << 2);
	    clxdat |= sprclx[shrunk_tmp];
	}
    }

    /* We have 8 bits per pixel in spixstate, two for every sprite pair.  The
       low order bit records whether the attach bit was set for this pair.  */

    if (ctl & (num << 7) & 0x80) {
	uae_u32 state = 0x01010101 << (num - 1);
	uae_u32 *stbuf = spixstate.words + (word_offs >> 2);
	uae_u8 *stb1 = spixstate.bytes + word_offs;	
	stb1[0] |= state;
	stb1[1] |= state;
	stb1[2] |= state;
	stb1[3] |= state;
	stb1[4] |= state;
	stb1[5] |= state;
	stb1[6] |= state;
	stb1[7] |= state;
	stb1[8] |= state;
	stb1[9] |= state;
	stb1[10] |= state;
	stb1[11] |= state;
	stb1[12] |= state;
	stb1[13] |= state;
	stb1[14] |= state;
	stb1[15] |= state;
	e->has_attached = 1;
    }
}

static void decide_sprites (int hpos)
{
    int nrs[MAX_SPRITES], posns[MAX_SPRITES];
    int count, i;
    int point = coord_hw_to_window_x (hpos * 2);
    int width = 16 << lores_shift;

    if (framecnt != 0 || hpos < 0x14 || nr_armed == 0 || point == last_sprite_point)
	return;

    decide_diw (hpos);
    decide_line (hpos);

#if 0
    /* This tries to detect whether the line is border, but that doesn't work, it's too early.  */
    if (thisline_decision.plfleft == -1)
	return;
#endif
    count = 0;
    for (i = 0; i < 8; i++) {
	int sprxp = spr[i].xpos;
	int j, bestp;

	if (! spr[i].armed || sprxp < 0 || sprxp <= last_sprite_point || sprxp > point)
	    continue;
	if ((thisline_decision.diwfirstword >= 0 && sprxp + width < thisline_decision.diwfirstword)
	    || (thisline_decision.diwlastword >= 0 && sprxp > thisline_decision.diwlastword))
	    continue;

	/* Sort the sprites in order of ascending X position before recording them.  */
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
    for (i = 0; i < count; i++) {
	int nr = nrs[i];    
	unsigned int data = sprdata[nr];
	unsigned int datb = sprdatb[nr];
	uae_u32 datab = ((sprtaba[data & 0xFF] << 16) | sprtaba[data >> 8]
			 | (sprtabb[datb & 0xFF] << 16) | sprtabb[datb >> 8]);
	record_sprite (next_lineno, nr, spr[nr].xpos, datab, sprctl[nr]);
    }
    last_sprite_point = point;
}

STATIC_INLINE int sprites_differ (struct draw_info *dip, struct draw_info *dip_old)
{
    struct sprite_entry *this_first = curr_sprite_entries + dip->first_sprite_entry;
    struct sprite_entry *this_last = curr_sprite_entries + dip->last_sprite_entry;
    struct sprite_entry *prev_first = prev_sprite_entries + dip_old->first_sprite_entry;
    int npixels;
    int i;

    if (dip->nr_sprites != dip_old->nr_sprites)
	return 1;
    
    if (dip->nr_sprites == 0)
	return 0;

    for (i = 0; i < dip->nr_sprites; i++)
	if (this_first[i].pos != prev_first[i].pos
	    || this_first[i].max != prev_first[i].max
	    || this_first[i].has_attached != prev_first[i].has_attached)
	    return 1;

    npixels = this_last->first_pixel + (this_last->max - this_last->pos) - this_first->first_pixel;
    if (memcmp (spixels + this_first->first_pixel, spixels + prev_first->first_pixel,
		npixels * sizeof (uae_u16)) != 0)
	return 1;
    if (memcmp (spixstate.bytes + this_first->first_pixel, spixstate.bytes + prev_first->first_pixel, npixels) != 0)
	return 1;
    return 0;
}

STATIC_INLINE int color_changes_differ (struct draw_info *dip, struct draw_info *dip_old)
{
    if (dip->nr_color_changes != dip_old->nr_color_changes)
	return 1;
    
    if (dip->nr_color_changes == 0)
	return 0;
    if (memcmp (curr_color_changes + dip->first_color_change,
		prev_color_changes + dip_old->first_color_change,
		dip->nr_color_changes * sizeof *curr_color_changes) != 0)
	return 1;
    return 0;
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
    decide_line (hpos);
    decide_fetch (hpos);

    if (thisline_decision.plfleft != -1 && thisline_decision.plflinelen == -1) {
	if (fetch_state != fetch_not_started)
	    abort ();
	thisline_decision.plfright = thisline_decision.plfleft;
	thisline_decision.plflinelen = 0;
	thisline_decision.bplres = RES_LORES;
    }

    /* Large DIWSTOP values can cause the stop position never to be
     * reached, so the state machine always stays in the same state and
     * there's a more-or-less full-screen DIW. */
    if (hdiwstate == DIW_waiting_stop || thisline_decision.diwlastword > max_diwlastword)
	thisline_decision.diwlastword = max_diwlastword;

    if (thisline_decision.diwfirstword != line_decisions[next_lineno].diwfirstword)
	MARK_LINE_CHANGED;
    if (thisline_decision.diwlastword != line_decisions[next_lineno].diwlastword)
	MARK_LINE_CHANGED;

    dip = curr_drawinfo + next_lineno;
    dip_old = prev_drawinfo + next_lineno;
    dp = line_decisions + next_lineno;
    changed = thisline_changed;

    if (thisline_decision.plfleft != -1) {
	record_diw_line (thisline_decision.diwfirstword, thisline_decision.diwlastword);

	decide_sprites (hpos);
    }

    dip->last_sprite_entry = next_sprite_entry;
    dip->last_color_change = next_color_change;

    if (thisline_decision.ctable == -1) {
	if (thisline_decision.plfleft == -1)
	    remember_ctable_for_border ();
	else
	    remember_ctable ();
    }

    dip->nr_color_changes = next_color_change - dip->first_color_change;
    dip->nr_sprites = next_sprite_entry - dip->first_sprite_entry;

    if (thisline_decision.plfleft != line_decisions[next_lineno].plfleft)
	changed = 1;
    if (! changed && color_changes_differ (dip, dip_old))
	changed = 1;
    if (!changed && thisline_decision.plfleft != -1 && sprites_differ (dip, dip_old))
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

    thisline_decision.nr_planes = 0;

    decided_bpl1mod = bpl1mod;
    decided_bpl2mod = bpl2mod;

    thisline_decision.plfleft = -1;
    thisline_decision.plflinelen = -1;

    /* decided_res shouldn't be touched before it's initialized by decide_line(). */
    thisline_decision.diwfirstword = -1;
    thisline_decision.diwlastword = -2;
    if (hdiwstate == DIW_waiting_stop) {
	thisline_decision.diwfirstword = 0;
	if (thisline_decision.diwfirstword != line_decisions[next_lineno].diwfirstword)
	    MARK_LINE_CHANGED;
    }
    thisline_decision.ctable = -1;

    thisline_changed = 0;
    curr_drawinfo[next_lineno].first_color_change = next_color_change;
    curr_drawinfo[next_lineno].first_sprite_entry = next_sprite_entry;
    next_sprite_forced = 1;

    /* memset(sprite_last_drawn_at, 0, sizeof sprite_last_drawn_at); */
    last_sprite_point = 0;
    modulos_added = 0;
    plane_decided = 0;
    color_decided = 0;
    fetch_state = fetch_not_started;
    passed_plfstop = 0;

    memset (todisplay, 0, sizeof todisplay);
    memset (fetched, 0, sizeof fetched);
    memset (fetched_aga0, 0, sizeof fetched_aga0);
    memset (fetched_aga1, 0, sizeof fetched_aga1);
    memset (outword, 0, sizeof outword);

    last_decide_line_hpos = -1;
    last_diw_pix_hpos = -1;
    last_ddf_pix_hpos = -1;
    last_sprite_hpos = -1;
    last_fetch_hpos = -1;
}

/* set PAL or NTSC timing variables */

static void init_hz (void)
{
    int isntsc;

    beamcon0 = new_beamcon0;

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

static void calcdiw (void)
{
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

    diwfirstword = coord_diw_to_window_x (hstrt);
    diwlastword = coord_diw_to_window_x (hstop);
    if (diwfirstword < 0)
	diwfirstword = 0;

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
    if (plfstrt < 0x18)
	plfstrt = 0x18;
}

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
    return intreq /* | (currprefs.use_serial ? 0x0001 : 0) */;
}
STATIC_INLINE uae_u16 ADKCONR (void)
{
    return adkcon;
}
STATIC_INLINE uae_u16 VPOSR (void)
{
    unsigned int csbit = currprefs.ntscmode ? 0x1000 : 0;
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
    return (vpos << 8) | current_hpos ();
}

STATIC_INLINE void COP1LCH (uae_u16 v) { cop1lc = (cop1lc & 0xffff) | ((uae_u32)v << 16); }
STATIC_INLINE void COP1LCL (uae_u16 v) { cop1lc = (cop1lc & ~0xffff) | (v & 0xfffe); }
STATIC_INLINE void COP2LCH (uae_u16 v) { cop2lc = (cop2lc & 0xffff) | ((uae_u32)v << 16); }
STATIC_INLINE void COP2LCL (uae_u16 v) { cop2lc = (cop2lc & ~0xffff) | (v & 0xfffe); }

static void start_copper (void)
{
    int was_active = eventtab[ev_copper].active;
    eventtab[ev_copper].active = 0;
    if (was_active)
	events_schedule ();

    cop_state.ignore_next = 0;
    cop_state.state = COP_read1;
    cop_state.vpos = vpos;
    cop_state.hpos = current_hpos () & ~1;

    if (dmaen (DMA_COPPER)) {
	copper_enabled_thisline = 1;
	set_special (SPCFLAG_COPPER);
    }
}

static void COPJMP1 (uae_u16 a)
{
    cop_state.ip = cop1lc;
    start_copper ();
}

static void COPJMP2 (uae_u16 a)
{
    cop_state.ip = cop2lc;
    start_copper ();
}

STATIC_INLINE void COPCON (uae_u16 a)
{
    copcon = a;
}

static void DMACON (int hpos, uae_u16 v)
{
    int i;

    uae_u16 oldcon = dmacon;

    decide_line (hpos);
    decide_fetch (hpos);

    setclr (&dmacon, v);
    dmacon &= 0x1FFF;

    /* FIXME? Maybe we need to think a bit more about the master DMA enable
     * bit in these cases. */
    if ((dmacon & DMA_COPPER) != (oldcon & DMA_COPPER)) {
	eventtab[ev_copper].active = 0;
    }
    if ((dmacon & DMA_COPPER) > (oldcon & DMA_COPPER)) {
	cop_state.ip = cop1lc;
	cop_state.ignore_next = 0;
	cop_state.state = COP_read1;
	cop_state.vpos = vpos;
	cop_state.hpos = hpos & ~1;
	copper_enabled_thisline = 1;
	set_special (SPCFLAG_COPPER);
    }
    if (! (dmacon & DMA_COPPER)) {
	copper_enabled_thisline = 0;
	unset_special (SPCFLAG_COPPER);
	cop_state.state = COP_stop;
    }

    if ((dmacon & DMA_BLITPRI) > (oldcon & DMA_BLITPRI) && bltstate != BLT_done) {
	static int count = 0;
	if (!count) {
	    count = 1;
	    write_log ("warning: program is doing blitpri hacks.\n");
	}
	set_special (SPCFLAG_BLTNASTY);
    }
    if ((dmacon & (DMA_BLITPRI | DMA_BLITTER | DMA_MASTER)) != (DMA_BLITPRI | DMA_BLITTER | DMA_MASTER))
	unset_special (SPCFLAG_BLTNASTY);

    if (currprefs.produce_sound > 0) {
	update_audio ();

	for (i = 0; i < 4; i++) {
	    struct audio_channel_data *cdp = audio_channel + i;
	    int chan_ena = (dmacon & 0x200) && (dmacon & (1<<i));
	    if (cdp->dmaen == chan_ena)
		continue;
	    cdp->dmaen = chan_ena;
	    if (cdp->dmaen) {
		if (cdp->state == 0) {
		    cdp->state = 1;
		    cdp->pt = cdp->lc;
		    cdp->wper = cdp->per;
		    cdp->wlen = cdp->len;
		    cdp->data_written = 2;
		    cdp->evtime = eventtab[ev_hsync].evtime - get_cycles ();
		}
	    } else {
		if (cdp->state == 1 || cdp->state == 5) {
		    cdp->state = 0;
		    cdp->last_sample = 0;
		    cdp->current_sample = 0;
		}
	    }
	}
	schedule_audio ();
    }
    events_schedule();
}

/*static int trace_intena = 0;*/

STATIC_INLINE void INTENA (uae_u16 v)
{
/*    if (trace_intena)
	fprintf (stderr, "INTENA: %04x\n", v);*/
    setclr (&intena,v);
    set_special (SPCFLAG_INT);
}
void INTREQ (uae_u16 v)
{
    setclr(&intreq,v);
    set_special (SPCFLAG_INT);
    if ((v & 0x8800) == 0x0800)
	serdat &= 0xbfff;
}

static void ADKCON (uae_u16 v)
{
    unsigned long t;

    if (currprefs.produce_sound > 0)
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
    decide_fetch (hpos);
    bplpt[num] = (bplpt[num] & 0xffff) | ((uae_u32)v << 16);
}
static void BPLPTL (int hpos, uae_u16 v, int num)
{
    decide_line (hpos);
    decide_fetch (hpos);
    bplpt[num] = (bplpt[num] & ~0xffff) | (v & 0xfffe);
}

static int maxplanes_ocs[]={ 6,4,0,0 };
static int maxplanes_ecs[]={ 6,4,2,0 };
static int maxplanes_aga[]={ 8,4,2,0, 8,8,4,0, 8,8,8,0 };

static void BPLCON0 (int hpos, uae_u16 v)
{
    int *planes;

    if (currprefs.chipset_mask & CSMASK_AGA) {
	planes = maxplanes_aga;
    } else if (! (currprefs.chipset_mask & CSMASK_ECS_DENISE)) {
	planes = maxplanes_ocs;
	v &= ~(0x0001|0x0010|0x0020|0x0040|0x0080);
    } else {
	planes = maxplanes_ecs;
	v &= ~(0x0001|0x0010|0x0020|0x0080);
    }
    /* Disable bitplane DMA if planes > maxplanes.  This is needed e.g. by the
       Sanity WOC demo (at the "Party Effect").  */
    if (GET_PLANES(v) > planes[fetchmode*4 + GET_RES (v)])
	v &= ~0x7040;

    if (bplcon0 == v)
	return;
    decide_line (hpos);
    decide_fetch (hpos);

    bplcon0 = v;
    nr_planes_from_bplcon0 = GET_PLANES (v);

    if (currprefs.chipset_mask & CSMASK_AGA)
	/* It's not clear how the copper timings are affected by the number
	 * of bitplanes on AGA machines */
	corrected_nr_planes_from_bplcon0 = 4;
    else
	corrected_nr_planes_from_bplcon0 = nr_planes_from_bplcon0 << GET_RES (bplcon0);
    expand_fmodes ();
}

STATIC_INLINE void BPLCON1 (int hpos, uae_u16 v)
{
    if (bplcon1 == v)
	return;
    decide_line (hpos);
    decide_fetch (hpos);
    bplcon1 = v;
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
    if (! (currprefs.chipset_mask & CSMASK_AGA))
	return;
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
    decide_line (hpos);
    decide_fetch (hpos);
    bpl1mod = v;
}

static void BPL2MOD (int hpos, uae_u16 v)
{
    v &= ~1;
    if ((uae_s16)bpl2mod == (uae_s16)v)
	return;
    decide_line (hpos);
    decide_fetch (hpos);
    bpl2mod = v;
}

STATIC_INLINE void BPL1DAT (int hpos, uae_u16 v)
{
    decide_line (hpos);
    bpl1dat = v;

    {
	static int count = 0;
	if (count++ > 1000) {
	    count = 0;
	    write_log ("BPL1DAT %d\n", hpos);
	}
    }

    maybe_first_bpl1dat (hpos);
}
/* We could do as well without those... */
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
    v &= 0xFC;
    if (ddfstrt == v)
	return;
    decide_line (hpos);
    ddfstrt = v;
    calcdiw ();
    if (ddfstop > 0xD4 && (ddfstrt & 4) == 4)
	write_log ("WARNING! Very strange DDF values.\n");
}
static void DDFSTOP (int hpos, uae_u16 v)
{
    v &= 0xFC;
    if (ddfstop == v)
	return;
    decide_line (hpos);
    decide_fetch (hpos);
    ddfstop = v;
    calcdiw ();
    if (fetch_state != fetch_not_started)
	estimate_last_fetch_cycle (hpos);
    if (ddfstop > 0xD4 && (ddfstrt & 4) == 4)
	write_log ("WARNING! Very strange DDF values.\n");
}

static void FMODE (uae_u16 v)
{
    if (! (currprefs.chipset_mask & CSMASK_AGA))
	v = 0;

    fmode = v;
    sprite_width = GET_SPRITEWIDTH (fmode);
    switch (fmode & 3) {
    case 0:
	fetchmode = 0;
	break;
    case 1:
    case 2:
	fetchmode = 1;
	break;
    case 3:
	fetchmode = 2;
	break;
    }
    expand_fmodes ();
}

static void BLTADAT (uae_u16 v)
{
    maybe_blit ();

    blt_info.bltadat = v;
}
/*
 * "Loading data shifts it immediately" says the HRM. Well, that may
 * be true for BLTBDAT, but not for BLTADAT - it appears the A data must be
 * loaded for every word so that AFWM and ALWM can be applied.
 */
static void BLTBDAT (uae_u16 v)
{
    maybe_blit ();

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
    nr_armed -= spr[num].armed;
    spr[num].armed = 0;
    if (sprpos[num] == 0 && v == 0) {
	spr[num].state = SPR_stop;
	spr[num].on = 0;
    } else
	spr[num].state = SPR_waiting_start;

    sprxp = coord_hw_to_window_x ((sprpos[num] & 0xFF) * 2 + (v & 1) + DIW_DDF_OFFSET);
    spr[num].xpos = sprxp;
    spr[num].vstart = (sprpos[num] >> 8) | ((sprctl[num] << 6) & 0x100);
    spr[num].vstop = (sprctl[num] >> 8) | ((sprctl[num] << 7) & 0x100);
}
STATIC_INLINE void SPRxPOS_1 (uae_u16 v, int num)
{
    int sprxp;
    sprpos[num] = v;
    sprxp = coord_hw_to_window_x ((v & 0xFF) * 2 + (sprctl[num] & 1) + DIW_DDF_OFFSET);
    spr[num].xpos = sprxp;
    spr[num].vstart = (sprpos[num] >> 8) | ((sprctl[num] << 6) & 0x100);
}
STATIC_INLINE void SPRxDATA_1 (uae_u16 v, int num)
{
    sprdata[num] = v;
    nr_armed += 1 - spr[num].armed;
    spr[num].armed = 1;
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
    spr[num].pt &= 0xffff;
    spr[num].pt |= (uae_u32)v << 16;

    if (spr[num].state == SPR_stop || vpos < sprite_vblank_endline)
	spr[num].state = SPR_restart;
    spr[num].on = 1;
}
static void SPRxPTL (int hpos, uae_u16 v, int num)
{
    decide_sprites (hpos);
    spr[num].pt &= ~0xffff;
    spr[num].pt |= v;

    if (spr[num].state == SPR_stop || vpos < sprite_vblank_endline)
	spr[num].state = SPR_restart;
    spr[num].on = 1;
}
static void CLXCON (uae_u16 v)
{
    clxcon = v;
    clxcon_bpl_enable = (v >> 6) & 63;
    clxcon_bpl_match = v & 63;
    clx_sprmask = ((((v >> 15) & 1) << 7) | (((v >> 14) & 1) << 5) | (((v >> 13) & 1) << 3) | (((v >> 12) & 1) << 1) | 0x55);
}
static void CLXCON2 (uae_u16 v)
{
    if (!(currprefs.chipset_mask & CSMASK_AGA))
	return;
    clxcon2 = v;
    clxcon_bpl_enable |= v & (0x40|0x80);
    clxcon_bpl_match |= (v & (0x01|0x02)) << 6;
 }
static uae_u16 CLXDAT (void)
{
    uae_u16 v = clxdat;
    clxdat = 0;
    return v;
}

static uae_u16 COLOR_READ (int num)
{
    int cr, cg, cb, colreg;
    uae_u16 cval;

    if (!(currprefs.chipset_mask & CSMASK_AGA) || !(bplcon2 & 0x0100))
	return 0xffff;

    colreg = ((bplcon3 >> 13) & 7) * 32 + num;
    cr = current_colors.color_regs_aga[colreg] >> 16;
    cg = (current_colors.color_regs_aga[colreg] >> 8) & 0xFF;
    cb = current_colors.color_regs_aga[colreg] & 0xFF;
    if (bplcon3 & 0x200)
	cval = ((cr & 15) << 12) | ((cg & 15) << 4) | ((cb & 15) << 0);
    else
	cval = ((cr >> 4) << 12) | ((cg >> 4) << 4) | ((cb >> 4) << 0);
    return cval;
}

static void COLOR_WRITE (int hpos, uae_u16 v, int num)
{
    v &= 0xFFF;
    if (currprefs.chipset_mask & CSMASK_AGA) {
	int r,g,b;
	int cr,cg,cb;
	int colreg;
	uae_u32 cval;

	/* writing is disabled when RDRAM=1 */
	if (bplcon2 & 0x0100)
	    return;

	colreg = ((bplcon3 >> 13) & 7) * 32 + num;
	r = (v & 0xF00) >> 8;
	g = (v & 0xF0) >> 4;
	b = (v & 0xF) >> 0;
	cr = current_colors.color_regs_aga[colreg] >> 16;
	cg = (current_colors.color_regs_aga[colreg] >> 8) & 0xFF;
	cb = current_colors.color_regs_aga[colreg] & 0xFF;

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
	if (cval == current_colors.color_regs_aga[colreg])
	    return;

	/* Call this with the old table still intact. */
	record_color_change (hpos, colreg, cval);
	remembered_color_entry = -1;
	current_colors.color_regs_aga[colreg] = cval;
	current_colors.acolors[colreg] = CONVERT_RGB (cval);
   } else {
	if (current_colors.color_regs_ecs[num] == v)
	    return;
	/* Call this with the old table still intact. */
	record_color_change (hpos, num, v);
	remembered_color_entry = -1;
	current_colors.color_regs_ecs[num] = v;
	current_colors.acolors[num] = xcolors[v];
    }
}

static uae_u16 potgo_value;

static void POTGO (uae_u16 v)
{
    potgo_value = v;
}

static uae_u16 POTGOR (void)
{
    uae_u16 v = (potgo_value | (potgo_value >> 1)) & 0x5500;

    v |= (~potgo_value & 0xAA00) >> 1;

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

/* Determine which cycles are available for the copper in a display
 * with a agiven number of planes.  */
static int cycles_for_plane[9][8] = {
    { 0, -1, 0, -1, 0, -1, 0, -1 },
    { 0, -1, 0, -1, 0, -1, 0, -1 },
    { 0, -1, 0, -1, 0, -1, 0, -1 },
    { 0, -1, 0, -1, 0, -1, 0, -1 },
    { 0, -1, 0, -1, 0, -1, 0, -1 },
    { 0, -1, 0, -1, 0, -1, 1, -1 }, /* */
    { 0, -1, 1, -1, 0, -1, 1, -1 },
    { 1, -1, 1, -1, 1, -1, 1, -1 },
    { 1, -1, 1, -1, 1, -1, 1, -1 }
};

static unsigned int waitmasktab[256];

STATIC_INLINE int copper_cant_read (int hpos, int planes)
{
    int t;

    /* @@@ */
    if (hpos >= (maxhpos & ~1))
	return 1;

    if (currprefs.chipset_mask & CSMASK_AGA)
	/* FIXME */
	return 0;

    if (fetch_state == fetch_not_started || passed_plfstop == 3
	|| hpos > estimated_last_fetch_cycle)
	return 0;

    t = cycles_for_plane[planes][(hpos + fetch_cycle - last_fetch_hpos) & 7];
#if 0
    if (t == -1)
	abort ();
#endif
    return t;
}

STATIC_INLINE int dangerous_reg (int reg)
{
    /* Safe:
     * Bitplane pointers, control registers, modulos and data.
     * Sprite pointers, control registers, and data.
     * Color registers.  */
    if (reg >= 0xE0 && reg < 0x1C0)
	return 0;
    return 1;
}

#define FAST_COPPER 0

static void update_copper (int until_hpos)
{
    int vp = vpos & (((cop_state.i2 >> 8) & 0x7F) | 0x80);
    int c_hpos = cop_state.hpos;

    if (eventtab[ev_copper].active)
	abort ();

    if (cop_state.state == COP_wait && vp < cop_state.vcmp)
	abort ();

    until_hpos &= ~1;

    if (until_hpos > (maxhpos & ~1))
	until_hpos = maxhpos & ~1;

    until_hpos += 2;
    for (;;) {
	int old_hpos = c_hpos;
	int hp;

	if (c_hpos >= until_hpos)
	    break;

	/* So we know about the fetch state.  */
	decide_line (c_hpos);

	c_hpos += 2;
	if (copper_cant_read (old_hpos, corrected_nr_planes_from_bplcon0))
	    continue;

	switch (cop_state.state) {
	case COP_read1:
	    cop_state.i1 = chipmem_bank.wget (cop_state.ip);
	    cop_state.ip += 2;
	    cop_state.state = COP_read2;
	    break;

	case COP_read2:
	    cop_state.i2 = chipmem_bank.wget (cop_state.ip);
	    cop_state.ip += 2;
	    cop_state.state = COP_read1;
	    if (cop_state.ignore_next) {
		cop_state.ignore_next = 0;
		break;
	    }
	    /* Perform moves immediately.  */
	    if ((cop_state.i1 & 1) == 0) {
		unsigned int address = cop_state.i1 & 0x1FE;
		if (address < (copcon & 2 ? ((currprefs.chipset_mask & CSMASK_AGA) ? 0 : 0x40u) : 0x80u)) {
		    cop_state.state = COP_stop;	
		    copper_enabled_thisline = 0;
		    unset_special (SPCFLAG_COPPER);
		    goto out;
		}
		if (address == 0x88) {
		    cop_state.ip = cop1lc;
		} else if (address == 0x8A) {
		    cop_state.ip = cop2lc;
		} else
		    custom_wput_1 (old_hpos, address, cop_state.i2);
		/* That could have turned off the copper...  */
		if (! copper_enabled_thisline)
		    goto out;
		break;
	    }

	    cop_state.vcmp = (cop_state.i1 & (cop_state.i2 | 0x8000)) >> 8;
	    cop_state.hcmp = (cop_state.i1 & cop_state.i2 & 0xFE);
	    if ((cop_state.i2 & 1) == 1) {
		/* Skip instruction.  */
		vp = vpos & (((cop_state.i2 >> 8) & 0x7F) | 0x80);
		hp = old_hpos & (cop_state.i2 & 0xFE);

		if ((vp > cop_state.vcmp || (vp == cop_state.vcmp && hp >= cop_state.hcmp))
		    && ((cop_state.i2 & 0x8000) != 0 || ! (DMACONR() & 0x4000)))
		    cop_state.ignore_next = 1;
		break;
	    }
	    cop_state.state = COP_wait;

	    vp = vpos & (((cop_state.i2 >> 8) & 0x7F) | 0x80);
	    hp = old_hpos & (cop_state.i2 & 0xFE);

	    if (cop_state.i1 == 0xFFFF && cop_state.i2 == 0xFFFE) {
		cop_state.state = COP_stop;
		copper_enabled_thisline = 0;
		unset_special (SPCFLAG_COPPER);
		goto out;
	    }
	    if (vp < cop_state.vcmp) {
		copper_enabled_thisline = 0;
		unset_special (SPCFLAG_COPPER);
		goto out;
	    }
	    if (vp > cop_state.vcmp)
		break;

	    /* Only enable shortcuts if there's no masking going on.  */
	    if (FAST_COPPER && (cop_state.i2 & 0xFE) == 0xFE) {
		/* Compute cycles remaining until c_hpos is past until_hpos
		   (i.e. until we'd normally break out of the loop).  */
		int time_remaining = until_hpos - c_hpos;

		if (time_remaining < 0)
		    abort ();

		/* Compute minimum number of cycles to wait once the copper is at c_hpos.  */
		cop_min_waittime = cop_state.hcmp - hp - 2;

		if (cop_min_waittime <= 0)
		    break;

		/* Does this still leave us before until_hpos?  */
		if (cop_min_waittime <= time_remaining) {
		    c_hpos += cop_min_waittime;
		    break;
		}

		/* This wait will use up all the time up to until_hpos, and then some.  */
		c_hpos += time_remaining;
		cop_min_waittime -= time_remaining;

		if (cop_min_waittime >= 8) {
		    unset_special (SPCFLAG_COPPER);
		    eventtab[ev_copper].active = 1;
		    eventtab[ev_copper].oldcycles = get_cycles ();
		    eventtab[ev_copper].evtime = get_cycles () + cop_min_waittime * CYCLE_UNIT;
		    /* until_hpos is larger than hpos; add a correction for this adjustment.  */
		    cop_min_waittime -= 2;
		    events_schedule ();
		    goto out;
		}
	    }
	    break;

	case COP_wait:
	    if (vp < cop_state.vcmp)
		abort ();

	    hp = old_hpos & (cop_state.i2 & 0xFE);
	    if (vp == cop_state.vcmp && hp < cop_state.hcmp)
		break;

	    /* Now we know that the comparisons were successful.  We might still
	       have to wait for the blitter though.  */
	    if ((cop_state.i2 & 0x8000) == 0 && (DMACONR() & 0x4000)) {
		/* We need to wait for the blitter.  */
		cop_state.state = COP_bltwait;
		copper_enabled_thisline = 0;
		unset_special (SPCFLAG_COPPER);
		goto out;
	    }

	    cop_state.state = COP_wait1;
	    break;

	case COP_wait1:
	    cop_state.state = COP_read1;
	    break;

	default:
	    abort ();
	}
    }

  out:
    cop_state.hpos = c_hpos;
#if FAST_COPPER
    /* The future, Conan?  */
    if (cop_state.state == COP_read1 || cop_state.state == COP_read2) {
	int ip = cop_state.ip;
	int word = cop_state.i1;
	int cycle_count;

	if (eventtab[ev_copper].active /* || ! (regs.spcflags & SPCFLAG_COPPER) */)
	    abort ();

	if (cop_state.state == COP_read2) {
	    ip += 2;
	    c_hpos += 2;
	    goto inner;
	}
	while (c_hpos < (maxhpos & ~1)) {
	    word = chipmem_bank.wget (ip);
	    ip += 4;
	    c_hpos += 4;
	  inner:
	    if ((word & 1) || dangerous_reg (word))
		break;
	}
	cycle_count = c_hpos - cop_state.hpos;
	if (cycle_count >= 8) {
	    unset_special (SPCFLAG_COPPER);
	    eventtab[ev_copper].active = 1;
	    eventtab[ev_copper].oldcycles = get_cycles ();
	    eventtab[ev_copper].evtime = get_cycles () + cycle_count * CYCLE_UNIT;
	    events_schedule ();
	}
    }
#endif
}

static void compute_spcflag_copper (void)
{
    copper_enabled_thisline = 0;
    unset_special (SPCFLAG_COPPER);
    if (! dmaen (DMA_COPPER) || cop_state.state == COP_stop || cop_state.state == COP_bltwait)
	return;

    if (cop_state.state == COP_wait) {
	int vp = vpos & (((cop_state.i2 >> 8) & 0x7F) | 0x80);

	if (vp < cop_state.vcmp)
	    return;
	copper_enabled_thisline = 1;

	if (FAST_COPPER && vp == cop_state.vcmp) {
	    int hp = cop_state.hpos & (cop_state.i2 & 0xFE);
	    cop_min_waittime = cop_state.hcmp - hp - 2;

	    /* If possible, compute a minimum waiting time, and set the event
	       timer if it's sufficiently large to be worthwhile.  */
	    if ((cop_state.i2 & 0xFE) == 0xFE && cop_min_waittime >= 8) {
		eventtab[ev_copper].active = 1;
		eventtab[ev_copper].oldcycles = get_cycles ();
		eventtab[ev_copper].evtime = get_cycles () + cop_min_waittime * CYCLE_UNIT;
		events_schedule ();
		return;
	    }
	}
    }

    copper_enabled_thisline = 1;
    set_special (SPCFLAG_COPPER);
}

static void copper_handler (void)
{
    /* This will take effect immediately, within the same cycle.  */
    set_special (SPCFLAG_COPPER);

    if (! copper_enabled_thisline)
	abort ();

    if (cop_state.state == COP_wait) {
	cop_state.hpos += cop_min_waittime;
	if (cop_state.hpos > current_hpos ())
	    abort ();
    }

    eventtab[ev_copper].active = 0;
}

void blitter_done_notify (void)
{
    if (cop_state.state != COP_bltwait)
	return;

    copper_enabled_thisline = 1;
    set_special (SPCFLAG_COPPER);
    cop_state.state = COP_wait;
}

void do_copper (void)
{
    int hpos = current_hpos ();
    update_copper (hpos);
}

static void sync_copper_with_cpu (int hpos)
{
    /* Need to let the copper advance to the current position, but only if it
       isn't in a waiting state.  */
    if (eventtab[ev_copper].active) {
	if (cop_state.state != COP_wait) {
	    eventtab[ev_copper].active = 0;
	    events_schedule ();
	    set_special (SPCFLAG_COPPER);
	}
    }
    if (copper_enabled_thisline && ! eventtab[ev_copper].active)
	update_copper (hpos);
}

static void do_sprites (int currvp, int hpos)
{
    int i;
    int maxspr, minspr;

    /* I don't know whether this is right. Some programs write the sprite pointers
     * directly at the start of the copper list. With the test against currvp, the
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
    /* Another update: seems like we have to use the NTSC value here (see Sanity Turmoil
     * demo).  */
    /* Maximum for Sanity Turmoil: 27.
       Minimum for Sanity Arte: 22.  */
    if (currvp < sprite_vblank_endline)
	return;

    /* The graph in the HRM, p. 195 seems to indicate that sprite 0 is
     * fetched at cycle 0x14 and thus can't be disabled by bitplane DMA. */
    maxspr = hpos / 4 - 0x14 / 4;
    minspr = last_sprite_hpos / 4 - 0x14 / 4;

    if (minspr > 8 || maxspr < 0)
	return;

    if (maxspr > 8)
	maxspr = 8;
    if (minspr < 0)
        minspr = 0;

    for (i = minspr; i < maxspr; i++) {
	int fetch = 0;

	if (spr[i].on == 0)
	    continue;

	if (spr[i].state == SPR_restart) {
	    fetch = 2;
	    spr[i].on = 1;
	} else if ((spr[i].state == SPR_waiting_start && spr[i].vstart == vpos) || spr[i].state == SPR_waiting_stop) {
	    fetch = 1;
	    spr[i].state = SPR_waiting_stop;
	}
	if ((spr[i].state == SPR_waiting_stop || spr[i].state == SPR_waiting_start) && spr[i].vstop == vpos) {
	    fetch = 2;
	    spr[i].state = SPR_waiting_start;
	}

	if (fetch && dmaen (DMA_SPRITE)) {
	    uae_u16 data1 = chipmem_bank.wget (spr[i].pt);
	    uae_u16 data2 = chipmem_bank.wget (spr[i].pt + 2);
	    spr[i].pt += 4;

	    if (fetch == 1) {
		/* Hack for X mouse auto-calibration */
		if (i == 0 && !sprvbfl && ((sprpos[0] & 0xff) << 2) > 2 * DISPLAY_LEFT_SHIFT) {
		    spr0ctl = sprctl[0];
		    spr0pos = sprpos[0];
		    sprvbfl = 2;
		}
		SPRxDATB_1 (data2, i);
		SPRxDATA_1 (data1, i);
	    } else {
		SPRxPOS_1 (data1, i);
		SPRxCTL_1 (data2, i);
	    }
	}
    }
    last_sprite_hpos = hpos;
}

static void init_sprites (void)
{
    int i;

    for (i = 0; i < 8; i++) {
	/* ???? */
	spr[i].state = SPR_stop;
	spr[i].on = 0;
    }

    memset (sprpos, 0, sizeof sprpos);
    memset (sprctl, 0, sizeof sprctl);
}

static void adjust_array_sizes (void)
{
#ifdef OS_WITHOUT_MEMORY_MANAGEMENT
    if (delta_sprite_entry) {
	void *p1,*p2;
	int mcc = max_sprite_entry + 50 + delta_sprite_entry;
	delta_sprite_entry = 0;
	p1 = realloc (sprite_entries[0], mcc * sizeof (struct sprite_entry));
	p2 = realloc (sprite_entries[1], mcc * sizeof (struct sprite_entry));
	if (p1) sprite_entries[0] = p1;
	if (p2) sprite_entries[1] = p2;
	if (p1 && p2) {
	    fprintf (stderr, "new max_sprite_entry=%d\n",mcc);
	    max_sprite_entry = mcc;
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

    /* Avoid this code in the first frame after a customreset.  */
    if (prev_sprite_entries) {
	int first_pixel = prev_sprite_entries[0].first_pixel;
	int npixels = prev_sprite_entries[prev_next_sprite_entry].first_pixel - first_pixel;
	memset (spixels + first_pixel, 0, npixels * sizeof *spixels);
	memset (spixstate.bytes + first_pixel, 0, npixels * sizeof *spixstate.bytes);
    }
    prev_next_sprite_entry = next_sprite_entry;

    next_color_change = 0;
    next_sprite_entry = 0;
    next_color_entry = 0;
    remembered_color_entry = -1;

    prev_sprite_entries = sprite_entries[current_change_set];
    curr_sprite_entries = sprite_entries[current_change_set ^ 1];
    prev_color_changes = color_changes[current_change_set];
    curr_color_changes = color_changes[current_change_set ^ 1];
    prev_color_tables = color_tables[current_change_set];
    curr_color_tables = color_tables[current_change_set ^ 1];

    prev_drawinfo = line_drawinfo[current_change_set];
    curr_drawinfo = line_drawinfo[current_change_set ^ 1];
    current_change_set ^= 1;

    color_src_match = color_dest_match = -1;

    /* Use both halves of the array in alternating fashion.  */
    curr_sprite_entries[0].first_pixel = current_change_set * MAX_SPR_PIXELS;
    next_sprite_forced = 1;
}

static void vsync_handler (void)
{
    n_frames++;

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
    cop_state.ignore_next = 0;

    init_hardware_frame ();

#ifdef HAVE_GETTIMEOFDAY
    {
	struct timeval tv;
	unsigned long int newtime;

	gettimeofday (&tv,NULL);
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
    int copper_was_active = eventtab[ev_copper].active;
    if (copper_was_active) {
	/* Could happen if horizontal wait position is too large.  */
	eventtab[ev_copper].active = 0;
	/* If this was a sequence of moves, we have to call update_copper if
	   we don't want to lose them.  */
	if (cop_state.state != COP_wait) {
	    copper_was_active = 0;
	    set_special (SPCFLAG_COPPER);
	}
    }
    if (copper_enabled_thisline && ! copper_was_active)
	update_copper (maxhpos);

    finish_decisions ();
    if (currprefs.collision_level > 0)
	do_sprite_collisions ();

    hsync_record_line_state (next_lineno, nextline_how, thisline_changed);

    eventtab[ev_hsync].evtime += get_cycles () - eventtab[ev_hsync].oldcycles;
    eventtab[ev_hsync].oldcycles = get_cycles ();
    CIA_hsync_handler ();
    DISK_update ();

    if (currprefs.produce_sound > 0) {
	int nr;

	update_audio ();

	/* Sound data is fetched at the beginning of each line */
	for (nr = 0; nr < 6; nr++) {
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
    /* See if there's a chance of a copper wait ending this line.  */
    cop_state.hpos = 0;
    compute_spcflag_copper ();
}

static void init_eventtab (void)
{
    int i;

    for(i = 0; i < ev_max; i++) {
	eventtab[i].active = 0;
	eventtab[i].oldcycles = 0;
    }

    eventtab[ev_cia].handler = CIA_handler;
    eventtab[ev_hsync].handler = hsync_handler;
    eventtab[ev_hsync].evtime = maxhpos * CYCLE_UNIT + get_cycles ();
    eventtab[ev_hsync].active = 1;

    eventtab[ev_copper].handler = copper_handler;
    eventtab[ev_copper].active = 0;
    eventtab[ev_blitter].handler = blitter_handler;
    eventtab[ev_blitter].active = 0;
    eventtab[ev_disk].handler = DISK_handler;
    eventtab[ev_disk].active = 0;
    eventtab[ev_audio].handler = audio_evhandler;
    eventtab[ev_audio].active = 0;
    events_schedule ();
}

void customreset (void)
{
    int i;
    int zero = 0;
#ifdef HAVE_GETTIMEOFDAY
    struct timeval tv;
#endif

    if ((currprefs.chipset_mask & CSMASK_AGA) == 0) {
	for (i = 0; i < 32; i++) {
	    current_colors.color_regs_ecs[i] = 0;
	    current_colors.acolors[i] = xcolors[0];
	}
    } else {
        for (i = 0; i < 256; i++) {
	    current_colors.color_regs_aga[i] = 0;
	    current_colors.acolors[i] = CONVERT_RGB (zero);
	}
    }

    n_frames = 0;

    expamem_reset ();

    DISK_reset ();
    CIA_reset ();
    currcycle = 0;
    unset_special (~(SPCFLAG_BRK | SPCFLAG_MODE_CHANGE));

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

    curr_sprite_entries = 0;
    prev_sprite_entries = 0;
    sprite_entries[0][0].first_pixel = 0;
    sprite_entries[1][0].first_pixel = MAX_SPR_PIXELS;
    sprite_entries[0][1].first_pixel = 0;
    sprite_entries[1][1].first_pixel = MAX_SPR_PIXELS;
    memset (spixels, 0, sizeof spixels);
    memset (&spixstate, 0, sizeof spixstate);
    
    /* Clear the armed flags of all sprites.  */
    memset (spr, 0, sizeof spr);
    nr_armed = 0;

    dmacon = intena = 0;
    bltstate = BLT_done;
    cop_state.state = COP_stop;
    diwstate = DIW_waiting_start;
    hdiwstate = DIW_waiting_start;
    copcon = 0;
    DSKLEN (0, 0);
    currcycle = 0;

    bplcon4 = 0x11; /* Get AGA chipset into ECS compatibility mode */
    bplcon3 = 0xC00;
    fmode = 0;
    fetchmode = 0;
    sprite_width = 16;

    new_beamcon0 = currprefs.ntscmode ? 0x00 : 0x20;
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
    write_log ("DIWSTRT: %04x DIWSTOP: %04x DDFSTRT: %04x DDFSTOP: %04x\n",
	       (unsigned int)diwstrt, (unsigned int)diwstop, (unsigned int)ddfstrt, (unsigned int)ddfstop);
    if (timeframes) {
	write_log ("Average frame time: %d ms [frames: %d time: %d]\n",
		   frametime / timeframes, timeframes, frametime);
	if (total_skipped)
	    write_log ("Skipped frames: %d\n", total_skipped);
    }
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
	sprite_ab_merge[i] = (((i & 15) ? 1 : 0)
			      | ((i & 240) ? 2 : 0));
	
	for (j = 0; j < 511; j = (j << 1) | 1)
	    if ((i & ~j) == 0)
		waitmasktab[i] = ~j;
    }
    for (i = 0; i < 16; i++) {
	clxmask[i] = (((i & 1) ? 0xF : 0x3)
		      | ((i & 2) ? 0xF0 : 0x30)
		      | ((i & 4) ? 0xF00 : 0x300)
		      | ((i & 8) ? 0xF000 : 0x3000));
	sprclx[i] = (((i & 0x3) == 0x3 ? 1 : 0)
		     | ((i & 0x5) == 0x5 ? 2 : 0)
		     | ((i & 0x9) == 0x9 ? 4 : 0)
		     | ((i & 0x6) == 0x6 ? 8 : 0)
		     | ((i & 0xA) == 0xA ? 16 : 0)
		     | ((i & 0xC) == 0xC ? 32 : 0)) << 9;
    }
}

void custom_init (void)
{
    uaecptr pos;

#ifdef OS_WITHOUT_MEMORY_MANAGEMENT
    int num;

    for (num = 0; num < 2; num++) {
       sprite_entries[num] = xmalloc (max_sprite_entry * sizeof (struct sprite_entry));
       color_changes[num] = xmalloc (max_color_change * sizeof (struct color_change));
    }
#endif

    pos = here ();

    org (0xF0FF70);
    calltrap (deftrap (mousehack_helper));
    dw (RTS);

    org (0xF0FFA0);
    calltrap (deftrap (timehack_helper));
    dw (RTS);

    org (pos);

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

STATIC_INLINE uae_u32 REGPARAM2 custom_wget_1 (uaecptr addr)
{
    special_mem |= S_READ;
    switch (addr & 0x1FE) {
     case 0x002: return DMACONR ();
     case 0x004: return VPOSR ();
     case 0x006: return VHPOSR ();

     case 0x008: return DSKDATR (current_hpos ());

     case 0x00A: return JOY0DAT ();
     case 0x00C: return JOY1DAT ();
     case 0x00E: return CLXDAT ();
     case 0x010: return ADKCONR ();

     case 0x012: return POT0DAT ();
     case 0x016: return POTGOR ();
     case 0x018: return SERDATR ();
     case 0x01A: return DSKBYTR (current_hpos ());
     case 0x01C: return INTENAR ();
     case 0x01E: return INTREQR ();
     case 0x07C: return DENISEID ();

     case 0x180: case 0x182: case 0x184: case 0x186: case 0x188: case 0x18A:
     case 0x18C: case 0x18E: case 0x190: case 0x192: case 0x194: case 0x196:
     case 0x198: case 0x19A: case 0x19C: case 0x19E: case 0x1A0: case 0x1A2:
     case 0x1A4: case 0x1A6: case 0x1A8: case 0x1AA: case 0x1AC: case 0x1AE:
     case 0x1B0: case 0x1B2: case 0x1B4: case 0x1B6: case 0x1B8: case 0x1BA:
     case 0x1BC: case 0x1BE:
	return COLOR_READ ((addr & 0x3E) / 2);
	break;

     default:
	custom_wput (addr, 0);
	return 0xffff;
    }
}

uae_u32 REGPARAM2 custom_wget (uaecptr addr)
{
    sync_copper_with_cpu (current_hpos ());
    return custom_wget_1 (addr);
}

uae_u32 REGPARAM2 custom_bget (uaecptr addr)
{
    special_mem |= S_READ;
    return custom_wget (addr & 0xfffe) >> (addr & 1 ? 0 : 8);
}

uae_u32 REGPARAM2 custom_lget (uaecptr addr)
{
    special_mem |= S_READ;
    return ((uae_u32)custom_wget (addr & 0xfffe) << 16) | custom_wget ((addr + 2) & 0xfffe);
}

void REGPARAM2 custom_wput_1 (int hpos, uaecptr addr, uae_u32 value)
{
    addr &= 0x1FE;
    switch (addr) {
     case 0x020: DSKPTH (value); break;
     case 0x022: DSKPTL (value); break;
     case 0x024: DSKLEN (value, hpos); break;
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

     case 0x096: DMACON (hpos, value); break;
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

     case 0x110: BPL1DAT (hpos, value); break;
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
	COLOR_WRITE (hpos, value & 0xFFF, (addr & 0x3E) / 2);
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
    special_mem |= S_WRITE;

    sync_copper_with_cpu (hpos);
    custom_wput_1 (hpos, addr, value);
}

void REGPARAM2 custom_bput (uaecptr addr, uae_u32 value)
{
    static int warned = 0;
    /* Is this correct now? (There are people who bput things to the upper byte of AUDxVOL). */
    uae_u16 rval = (value << 8) | (value & 0xFF);
    special_mem |= S_WRITE;
    custom_wput (addr, rval);
    if (!warned)
	write_log ("Byte put to custom register.\n"), warned++;
}

void REGPARAM2 custom_lput(uaecptr addr, uae_u32 value)
{
    special_mem |= S_WRITE;
    custom_wput (addr & 0xfffe, value >> 16);
    custom_wput ((addr + 2) & 0xfffe, (uae_u16)value);
}

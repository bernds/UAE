/*
 * Data used for communication between custom.c and drawing.c.
 * 
 * Copyright 1996-1998 Bernd Schmidt
 */

#define SMART_UPDATE 1

#ifdef SUPPORT_PENGUINS
#undef SMART_UPDATE
#define SMART_UPDATE 1
#endif

/*
 * This doesn't work very well
 */

#undef EMULATE_AGA

#ifndef EMULATE_AGA
#define AGA_CHIPSET 0
#else
#define AGA_CHIPSET 1
#endif

#if AGA_CHIPSET == 1
#define MAX_PLANES 8
#else
#define MAX_PLANES 6
#endif

#define RES_LORES 0
#define RES_HIRES 1
#define RES_SUPERHIRES 2

/* calculate shift depending on resolution (replaced "decided_hires ? 4 : 8") (TW) */
#define RES_SHIFT(res) ((res) == RES_LORES ? 8 : (res) == RES_HIRES ? 4 : 2)
#define RES_ADJUST(res) ((res) == RES_LORES ? 8 : (res) == RES_HIRES ? 4 : 2)

/* We ignore that many lores pixels at the start of the display. These are
 * invisible anyway due to hardware DDF limits. */
#define DISPLAY_LEFT_SHIFT 0x38
#define PIXEL_XPOS(HPOS) (((HPOS)*2 - DISPLAY_LEFT_SHIFT) << lores_shift)

/* @@@ Is maxhpos + 4 - 1 correct? (4 less isn't enough) */
#define max_diwlastword (PIXEL_XPOS(maxhpos + 4 - 1))

extern int lores_factor, lores_shift, sprite_width;

STATIC_INLINE int coord_hw_to_native_x (int x)
{
    x -= DISPLAY_LEFT_SHIFT;
    return x << lores_shift;
}

extern int framecnt;

struct color_entry {
#if AGA_CHIPSET == 0
    /* Color values in two formats: 12 bit Amiga RGB (color_regs), and
     * the native color value; both for each Amiga hardware color register. 
     */
    /* X86.S expects acolors at the start of the structure. */
    xcolnr acolors[32];
    uae_u16 color_regs[32];
#else
    uae_u32 color_regs[256];
#endif
};

/*
 * The idea behind this code is that at some point during each horizontal
 * line, we decide how to draw this line. There are many more-or-less
 * independent decisions, each of which can be taken at a different horizontal
 * position.
 * Sprites, color changes and bitplane delay changes are handled specially:
 * There isn't a single decision, but a list of structures containing
 * information on how to draw the line.
 */

struct color_change {
    int linepos;
    int regno;
    unsigned long value;
};

struct sprite_draw {
    int linepos;
    int num;
    int ctl;
    uae_u32 datab;
};

struct delay_change {
    int linepos;
    unsigned int value;
};

/* Way too much... */
#define MAX_REG_CHANGE ((MAXVPOS + 1) * 2 * MAXHPOS)

#ifdef OS_WITHOUT_MEMORY_MANAGEMENT
extern struct sprite_draw  *sprite_positions[2];
extern struct color_change *color_changes[2];
extern struct delay_change *delay_changes;
#else
extern struct sprite_draw sprite_positions[2][MAX_REG_CHANGE];
extern struct color_change color_changes[2][MAX_REG_CHANGE];
/* We don't remember those across frames, that would be too much effort.
 * We simply redraw the line whenever we see one of these. */
extern struct delay_change delay_changes[MAX_REG_CHANGE];
#endif

extern struct color_entry color_tables[2][(MAXVPOS+1) * 2];
extern struct color_entry *curr_color_tables, *prev_color_tables;

extern struct sprite_draw *curr_sprite_positions, *prev_sprite_positions;
extern struct color_change *curr_color_changes, *prev_color_changes;
extern struct draw_info *curr_drawinfo, *prev_drawinfo;

/* struct decision contains things we save across drawing frames for
 * comparison (smart update stuff). */
struct decision {
    unsigned long color0;
    int which;
    int plfstrt, plflinelen;
    /* Display window: native coordinates, depend on lores state.  */
    int diwfirstword, diwlastword;
    int ctable;

    uae_u16 bplcon0, bplcon1, bplcon2;
#if 0 /* We don't need this. */
    uae_u16 bplcon3;
#endif
#if AGA_CHIPSET == 1
    uae_u16 bplcon4;
#endif
};

/* Compute the number of bitplanes from a value written to BPLCON0  */
#define GET_PLANES(x) ((((x) >> 12) & 7) | (((x) & 0x10) >> 1))

/* Anything related to changes in hw registers during the DDF for one
 * line. */
struct draw_info {
    int first_sprite_draw, last_sprite_draw;
    int first_color_change, last_color_change;
    int first_delay_change, last_delay_change;
    int nr_color_changes, nr_sprites;
};

extern struct decision line_decisions[2 * (MAXVPOS+1) + 1];
extern struct draw_info line_drawinfo[2][2 * (MAXVPOS+1) + 1];

extern uae_u8 line_data[(MAXVPOS+1) * 2][MAX_PLANES * MAX_WORDS_PER_LINE * 2];

/* Functions in drawing.c.  */
extern int coord_native_to_amiga_y (int);
extern int coord_native_to_amiga_x (int);

extern void record_diw_line (int first, int last);
extern void hardware_line_completed (int lineno);

/* Determine how to draw a scan line.  */
enum nln_how {
    /* All lines on a non-doubled display. */
    nln_normal,
    /* Non-interlace, doubled display.  */
    nln_doubled,
    /* Interlace, doubled display, upper line.  */
    nln_upper,
    /* Interlace, doubled display, lower line.  */
    nln_lower,
    /* This line normal, next one black.  */
    nln_nblack
};

extern void hsync_record_line_state (int lineno, enum nln_how, int changed);
extern void vsync_handle_redraw (int long_frame, int lof_changed);
extern void init_hardware_for_drawing_frame (void);
extern void finish_drawing_frame (void);
extern void reset_drawing (void);
extern void drawing_init (void);
extern void notice_interlace_seen (void);

/* Finally, stuff that shouldn't really be shared.  */

extern int thisframe_first_drawn_line, thisframe_last_drawn_line;
extern uae_u16 clxdat, clxcon;
extern int clx_sprmask;
extern int diwfirstword,diwlastword;

#define IHF_SCROLLLOCK 0
#define IHF_QUIT_PROGRAM 1
#define IHF_PICASSO 2
#define IHF_SOUNDADJUST 3

extern int inhibit_frame;

STATIC_INLINE void set_inhibit_frame (int bit)
{
    inhibit_frame |= 1 << bit;
}
STATIC_INLINE void clear_inhibit_frame (int bit)
{
    inhibit_frame &= ~(1 << bit);
}
STATIC_INLINE void toggle_inhibit_frame (int bit)
{
    inhibit_frame ^= ~(1 << bit);
}

 /*
  * UAE - The Un*x Amiga Emulator
  *
  * Stuff
  *
  * Copyright 1995, 1996 Ed Hanway
  * Copyright 1995-2001 Bernd Schmidt
  */

#define UAEMAJOR 0
#define UAEMINOR 8
#define UAESUBREV 22

typedef enum { KBD_LANG_US, KBD_LANG_DK, KBD_LANG_DE, KBD_LANG_SE, KBD_LANG_FR, KBD_LANG_IT, KBD_LANG_ES } KbdLang;

extern long int version;

struct uaedev_mount_info;

struct strlist {
    struct strlist *next;
    char *str;
};

struct uae_prefs {
    struct strlist *unknown_lines;

    char description[256];
    char info[256];

    int illegal_mem;
    int no_xhair;
    int use_serial;
    int serial_demand;
    int parallel_demand;
    int use_gfxlib;
    int socket_emu;

    int start_debugger;
    int start_gui;

    int jport0;
    int jport1;
    KbdLang keyboard_lang;
    int allow_save;
    int emul_accuracy;
    int test_drawing_speed;

    int produce_sound;
    int stereo;
    int mixed_stereo;
    int sound_bits;
    int sound_freq;
    int sound_minbsiz;
    int sound_maxbsiz;
    int sound_pri_time;
    int sound_pri_cutoff;
    int sound_interpol;

    int gfx_framerate;
    int gfx_width;
    int gfx_height;
    int gfx_lores;
    int gfx_linedbl;
    int gfx_correct_aspect;
    int gfx_afullscreen;
    int gfx_pfullscreen;
    int gfx_xcenter;
    int gfx_ycenter;
    int color_mode;

    int blits_32bit_enabled;
    int immediate_blits;
    unsigned int chipset_mask;
    int ntscmode;
    int collision_level;
    int fast_copper;

    char df[4][256];
    char romfile[256];
    char romextfile[256];
    char keyfile[256];
    char prtname[256];
    char sername[256];

    char path_floppy[256];
    char path_hardfile[256];
    char path_rom[256];

    int m68k_speed;
    int cpu_level;
    int cpu_compatible;
    int address_space_24;

    uae_u32 z3fastmem_size;
    uae_u32 fastmem_size;
    uae_u32 chipmem_size;
    uae_u32 bogomem_size;
    uae_u32 a3000mem_size;
    uae_u32 gfxmem_size;

    int kickshifter;

    struct uaedev_mount_info *mountinfo;

    int nr_floppies;

    /* Target specific options */
    int x11_use_low_bandwidth;
    int x11_use_mitshm;
    int x11_use_dgamode;
    int x11_hide_cursor;
    int svga_no_linear;
    int win32_middle_mouse;
    int win32_logfile;
    int win32_iconified_nospeed;
    int win32_iconified_nosound;
    int win32_no_overlay; /* If this is set, we won't try and use any RGB overlays */
    int win32_automount_drives;

    int curses_reverse_video;
};

/* Contains the filename of .uaerc */
extern char optionsfile[];
extern void save_options (FILE *, struct uae_prefs *);

extern void default_prefs (struct uae_prefs *);
extern void discard_prefs (struct uae_prefs *);

extern int parse_cmdline_option (char, char *);

extern int cfgfile_yesno (char *option, char *value, char *name, int *location);
extern int cfgfile_intval (char *option, char *value, char *name, int *location, int scale);
extern int cfgfile_strval (char *option, char *value, char *name, int *location, const char *table[], int more);
extern int cfgfile_string (char *option, char *value, char *name, char *location, int maxsz);
extern char *cfgfile_subst_path (const char *path, const char *subst, const char *file);

extern int target_parse_option (struct uae_prefs *, char *option, char *value);
extern void target_save_options (FILE *, struct uae_prefs *);

extern int cfgfile_load (struct uae_prefs *, const char *filename);
extern int cfgfile_save (struct uae_prefs *, const char *filename);
extern void cfgfile_parse_line (struct uae_prefs *p, char *);
extern int cfgfile_parse_option (struct uae_prefs *p, char *option, char *value);
extern int cfgfile_get_description (const char *filename, char *description);
extern void cfgfile_show_usage (void);

extern void fixup_prefs_dimensions (struct uae_prefs *prefs);

extern void check_prefs_changed_custom (void);
extern void check_prefs_changed_cpu (void);
extern void check_prefs_changed_audio (void);
extern int check_prefs_changed_gfx (void);

#define JSEM_DECODEVAL(n,v) ((n) == 0 ? (v)->jport0 : (v)->jport1)
/* Determine how port n is configured */
#define JSEM_ISJOY0(n,v) (JSEM_DECODEVAL(n,v) == 0)
#define JSEM_ISJOY1(n,v) (JSEM_DECODEVAL(n,v) == 1)
#define JSEM_ISMOUSE(n,v) (JSEM_DECODEVAL(n,v) == 2)
#define JSEM_ISNUMPAD(n,v) (JSEM_DECODEVAL(n,v) == 3)
#define JSEM_ISCURSOR(n,v) (JSEM_DECODEVAL(n,v) == 4)
#define JSEM_ISSOMEWHEREELSE(n,v) (JSEM_DECODEVAL(n,v) == 5)
extern const char *gameport_state (int n);

extern struct uae_prefs currprefs, changed_prefs;

extern void machdep_init (void);

/* AIX doesn't think it is Unix. Neither do I. */
#if defined(_ALL_SOURCE) || defined(_AIX)
#undef __unix
#define __unix
#endif

#define MAX_COLOR_MODES 5

/* #define NEED_TO_DEBUG_BADLY */

#if !defined(USER_PROGRAMS_BEHAVE)
#define USER_PROGRAMS_BEHAVE 0
#endif

/* Some memsets which know that they can safely overwrite some more memory
 * at both ends and use that knowledge to align the pointers. */

#define QUADRUPLIFY(c) (((c) | ((c) << 8)) | (((c) | ((c) << 8)) << 16))

/* When you call this routine, bear in mind that it rounds the bounds and
 * may need some padding for the array. */

#define fuzzy_memset(p, c, o, l) fuzzy_memset_1 ((p), QUADRUPLIFY (c), (o) & ~3, ((l) + 4) >> 2)
STATIC_INLINE void fuzzy_memset_1 (void *p, uae_u32 c, int offset, int len)
{
    uae_u32 *p2 = (uae_u32 *)((char *)p + offset);
    int a = len & 7;
    len >>= 3;
    switch (a) {
     case 7: p2--; goto l1;
     case 6: p2-=2; goto l2;
     case 5: p2-=3; goto l3;
     case 4: p2-=4; goto l4;
     case 3: p2-=5; goto l5;
     case 2: p2-=6; goto l6;
     case 1: p2-=7; goto l7;
     case 0: if (!--len) return; break;
    }

    for (;;) {
	p2[0] = c;
	l1:
	p2[1] = c;
	l2:
	p2[2] = c;
	l3:
	p2[3] = c;
	l4:
	p2[4] = c;
	l5:
	p2[5] = c;
	l6:
	p2[6] = c;
	l7:
	p2[7] = c;

	if (!len)
	    break;
	len--;
	p2 += 8;
    }
}

/* This one knows it will never be asked to clear more than 32 bytes.  Make sure you call this with a
   constant for the length.  */
#define fuzzy_memset_le32(p, c, o, l) fuzzy_memset_le32_1 ((p), QUADRUPLIFY (c), (o) & ~3, ((l) + 7) >> 2)
STATIC_INLINE void fuzzy_memset_le32_1 (void *p, uae_u32 c, int offset, int len)
{
    uae_u32 *p2 = (uae_u32 *)((char *)p + offset);

    switch (len) {
     case 9: p2[0] = c; p2[1] = c; p2[2] = c; p2[3] = c; p2[4] = c; p2[5] = c; p2[6] = c; p2[7] = c; p2[8] = c; break;
     case 8: p2[0] = c; p2[1] = c; p2[2] = c; p2[3] = c; p2[4] = c; p2[5] = c; p2[6] = c; p2[7] = c; break;
     case 7: p2[0] = c; p2[1] = c; p2[2] = c; p2[3] = c; p2[4] = c; p2[5] = c; p2[6] = c; break;
     case 6: p2[0] = c; p2[1] = c; p2[2] = c; p2[3] = c; p2[4] = c; p2[5] = c; break;
     case 5: p2[0] = c; p2[1] = c; p2[2] = c; p2[3] = c; p2[4] = c; break;
     case 4: p2[0] = c; p2[1] = c; p2[2] = c; p2[3] = c; break;
     case 3: p2[0] = c; p2[1] = c; p2[2] = c; break;
     case 2: p2[0] = c; p2[1] = c; break;
     case 1: p2[0] = c; break;
     case 0: break;
     default: printf("Hit the programmer.\n"); break;
    }
}

#if defined(AMIGA) && defined(__GNUC__)
#include "od-amiga/amiga-kludges.h"
#endif

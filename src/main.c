 /*
  * UAE - The Un*x Amiga Emulator
  *
  * Main program
  *
  * Copyright 1995 Ed Hanway
  * Copyright 1995, 1996, 1997 Bernd Schmidt
  */

#include "sysconfig.h"
#include "sysdeps.h"
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
#include "serial.h"
#include "readcpu.h"
#include "newcpu.h"
#include "disk.h"
#include "debug.h"
#include "xwin.h"
#include "joystick.h"
#include "keybuf.h"
#include "gui.h"
#include "zfile.h"
#include "autoconf.h"
#include "osemu.h"
#include "osdep/exectasks.h"
#include "compiler.h"
#include "picasso96.h"
#include "bsdsocket.h"
#include "uaeexe.h"
#include "native2amiga.h"
#include "scsidev.h"

long int version = 256*65536L*UAEMAJOR + 65536L*UAEMINOR + UAESUBREV;

struct uae_prefs currprefs, changed_prefs;

int no_gui = 0;
int joystickpresent = 0;
int cloanto_rom = 0;

char warning_buffer[256];

char optionsfile[256];

/* If you want to pipe printer output to a file, put something like
 * "cat >>printerfile.tmp" above.
 * The printer support was only tested with the driver "PostScript" on
 * Amiga side, using apsfilter for linux to print ps-data.
 *
 * Under DOS it ought to be -p LPT1: or -p PRN: but you'll need a
 * PostScript printer or ghostscript -=SR=-
 */

/* People must provide their own name for this */
char sername[256] = "";

/* Slightly stupid place for this... */
/* ncurses.c might use quite a few of those. */
char *colormodes[] = { "256 colors", "32768 colors", "65536 colors",
    "256 colors dithered", "16 colors dithered", "16 million colors",
    "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
    "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
    "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
    "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
    "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""
};

void discard_prefs (struct uae_prefs *p)
{
    struct strlist **ps = &p->unknown_lines;
    while (*ps) {
	struct strlist *s = *ps;
	*ps = s->next;
	free (s->str);
	free (s);
    }
    free_mountinfo (p->mountinfo);
}

void default_prefs (struct uae_prefs *p)
{
    strcpy (p->description, "UAE default configuration");

    p->start_gui = 1;
    p->start_debugger = 0;

    p->unknown_lines = 0;
    /* Note to porters: please don't change any of these options! UAE is supposed
     * to behave identically on all platforms if possible. */
    p->illegal_mem = 0;
    p->no_xhair = 0;
    p->use_serial = 0;
    p->serial_demand = 0;
    p->parallel_demand = 0;
    p->automount_uaedev = 1;

    p->jport0 = 2;
    p->jport1 = 0;
    p->keyboard_lang = KBD_LANG_US;
    p->emul_accuracy = 2;
    p->test_drawing_speed = 0;

    p->produce_sound = 0;
    p->stereo = 0;
    p->sound_bits = DEFAULT_SOUND_BITS;
    p->sound_freq = DEFAULT_SOUND_FREQ;
    p->sound_minbsiz = DEFAULT_SOUND_MINB;
    p->sound_maxbsiz = DEFAULT_SOUND_MAXB;
    p->sound_interpol = 0;

    p->gfx_framerate = 1;
    p->gfx_width = 800;
    p->gfx_height = 600;
    p->gfx_lores = 0;
    p->gfx_linedbl = 0;
    p->gfx_afullscreen = 0;
    p->gfx_pfullscreen = 0;
    p->gfx_correct_aspect = 0;
    p->gfx_xcenter = 0;
    p->gfx_ycenter = 0;
    p->color_mode = 0;

    p->x11_use_low_bandwidth = 0;
    p->x11_use_mitshm = 0;
    p->x11_hide_cursor = 1;

    p->svga_no_linear = 0;

    p->win32_middle_mouse = 0;
    p->win32_sound_style = 0;
    p->win32_sound_tweak = 0;
    p->win32_logfile = 0;
    p->win32_iconified_nospeed = 0;
    p->win32_iconified_nosound = 0;

    p->immediate_blits = 0;
    p->blits_32bit_enabled = 0;

    strcpy (p->df[0], "df0.adf");
    strcpy (p->df[1], "df1.adf");
    strcpy (p->df[2], "df2.adf");
    strcpy (p->df[3], "df3.adf");

    strcpy (p->romfile, "kick.rom");
    strcpy (p->keyfile, "");
    strcpy (p->prtname, DEFPRTNAME);

    strcpy (p->path_rom, "./");
    strcpy (p->path_floppy, "./");
    strcpy (p->path_hardfile, "./");

    p->m68k_speed = 4;
    p->cpu_level = 2;
    p->cpu_compatible = 0;
    p->address_space_24 = 0;

    p->fastmem_size = 0x00000000;
    p->a3000mem_size = 0x00000000;
    p->z3fastmem_size = 0x00000000;
    p->chipmem_size = 0x00200000;
    p->bogomem_size = 0x00000000;
    p->gfxmem_size = 0x00000000;

    p->mountinfo = alloc_mountinfo ();
}

void fixup_prefs_dimensions (struct uae_prefs *prefs)
{
    if (prefs->gfx_width < 320)
	prefs->gfx_width = 320;
    if (prefs->gfx_height < 200)
	prefs->gfx_height = 200;
    if (prefs->gfx_height > 300 && ! prefs->gfx_linedbl)
	prefs->gfx_height = 300;
    if (prefs->gfx_height > 600)
	prefs->gfx_height = 600;

    prefs->gfx_width += 7; /* X86.S wants multiples of 4 bytes, might be 8 in the future. */
    prefs->gfx_width &= ~7;
}

static void fix_options (void)
{
    int err = 0;

    if ((currprefs.chipmem_size & (currprefs.chipmem_size - 1)) != 0
	|| currprefs.chipmem_size < 0x80000
	|| currprefs.chipmem_size > 0x800000)
    {
	currprefs.chipmem_size = 0x200000;
	fprintf (stderr, "Unsupported chipmem size!\n");
	err = 1;
    }
    if ((currprefs.fastmem_size & (currprefs.fastmem_size - 1)) != 0
	|| (currprefs.fastmem_size != 0 && (currprefs.fastmem_size < 0x100000 || currprefs.fastmem_size > 0x800000)))
    {
	currprefs.fastmem_size = 0;
	fprintf (stderr, "Unsupported fastmem size!\n");
	err = 1;
    }
    if ((currprefs.gfxmem_size & (currprefs.gfxmem_size - 1)) != 0
	|| (currprefs.gfxmem_size != 0 && (currprefs.gfxmem_size < 0x100000 || currprefs.gfxmem_size > 0x800000)))
    {
	currprefs.gfxmem_size = 0;
	fprintf (stderr, "Unsupported graphics card memory size!\n");
	err = 1;
    }
    if ((currprefs.z3fastmem_size & (currprefs.z3fastmem_size - 1)) != 0
	|| (currprefs.z3fastmem_size != 0 && (currprefs.z3fastmem_size < 0x100000 || currprefs.z3fastmem_size > 0x4000000)))
    {
	currprefs.z3fastmem_size = 0;
	fprintf (stderr, "Unsupported Zorro III fastmem size!\n");
	err = 1;
    }
    if (currprefs.address_space_24 && (currprefs.gfxmem_size != 0 || currprefs.z3fastmem_size != 0)) {
	currprefs.z3fastmem_size = currprefs.gfxmem_size = 0;
	fprintf (stderr, "Can't use a graphics card or Zorro III fastmem when using a 24 bit\n"
		 "address space - sorry.\n");
    }
    if ((currprefs.bogomem_size & (currprefs.bogomem_size - 1)) != 0
	|| (currprefs.bogomem_size != 0 && (currprefs.bogomem_size < 0x80000 || currprefs.bogomem_size > 0x100000)))
    {
	currprefs.bogomem_size = 0;
	fprintf (stderr, "Unsupported bogomem size!\n");
	err = 1;
    }

    if (currprefs.chipmem_size > 0x200000 && currprefs.fastmem_size != 0) {
	fprintf (stderr, "You can't use fastmem and more than 2MB chip at the same time!\n");
	currprefs.fastmem_size = 0;
	err = 1;
    }
    if (currprefs.m68k_speed < -1 || currprefs.m68k_speed > 20) {
	fprintf (stderr, "Bad value for -w parameter: must be -1, 0, or within 1..20.\n");
	currprefs.m68k_speed = 4;
	err = 1;
    }
    if (currprefs.produce_sound < 0 || currprefs.produce_sound > 3) {
	fprintf (stderr, "Bad value for -S parameter: enable value must be within 0..3\n");
	currprefs.produce_sound = 0;
	err = 1;
    }
    if (currprefs.cpu_level < 2 && currprefs.z3fastmem_size > 0) {
	fprintf (stderr, "Z3 fast memory can't be used with a 68000/68010 emulation. It\n"
		 "requires a 68020 emulation. Turning off Z3 fast memory.\n");
	currprefs.z3fastmem_size = 0;
	err = 1;
    }
    if (currprefs.gfxmem_size > 0 && (currprefs.cpu_level < 2 || currprefs.address_space_24)) {
	fprintf (stderr, "Picasso96 can't be used with a 68000/68010 or 68EC020 emulation. It\n"
		 "requires a 68020 emulation. Turning off Picasso96.\n");
	currprefs.gfxmem_size = 0;
	err = 1;
    }
#ifndef BSDSOCKET_SUPPORTED
    if (currprefs.socket_emu) {
	fprintf (stderr, "Compile-time option of BSDSOCKET_SUPPORTED was not enabled.  You can't use bsd-socket emulation.\n");
	currprefs.socket_emu = 0;
	err = 1;
    }
#endif

    if (err)
	fprintf (stderr, "Please use \"uae -h\" to get usage information.\n");
}

int quit_program = 0;

void uae_reset (void)
{
    if (quit_program == 0)
	quit_program = -2;
}

void uae_quit (void)
{
    if (quit_program != -1)
	quit_program = -1;
}

const char *gameport_state (int nr)
{
    if (JSEM_ISJOY0 (nr, &currprefs) && nr_joysticks > 0)
	return "using joystick #0";
    else if (JSEM_ISJOY1 (nr, &currprefs) && nr_joysticks > 1)
	return "using joystick #1";
    else if (JSEM_ISMOUSE (nr, &currprefs))
	return "using mouse";
    else if (JSEM_ISNUMPAD (nr, &currprefs))
	return "using numeric pad as joystick";
    else if (JSEM_ISCURSOR (nr, &currprefs))
	return "using cursor keys as joystick";
    else if (JSEM_ISSOMEWHEREELSE (nr, &currprefs))
	return "using T/F/H/B/Alt as joystick";

    return "not connected";
}

#ifndef DONT_PARSE_CMDLINE

void usage (void)
{
}

void parse_cmdline (int argc, char **argv)
{
    int i;
    for (i = 1; i < argc; i++) {
	if (strncmp (argv[i], "-config=", 8) == 0)
	    cfgfile_load (&currprefs, argv[i] + 8);
	/* Check for new-style "-f xxx" argument, where xxx is config-file */
	else if (strcmp (argv[i], "-f") == 0) {
	    if (i + 1 == argc)
		fprintf (stderr, "Missing argument for '-f' option.\n");
	    else
		cfgfile_load (&currprefs, argv[++i]);
	} else if (strcmp (argv[i], "-s") == 0) {
	    if (i + 1 == argc)
		fprintf (stderr, "Missing argument for '-s' option.\n");
	    else
		cfgfile_parse_line (&currprefs, argv[++i]);
	} else if (strcmp (argv[i], "-h") == 0 || strcmp (argv[i], "-help") == 0) {
	    usage ();
	    exit (0);
	} else {
	    if (argv[i][0] == '-' && argv[i][1] != '\0') {
		const char *arg = argv[i] + 2;
		if (*arg == '\0')
		    arg = i + 1 < argc ? argv[i + 1] : 0;
		i += parse_cmdline_option (argv[i][1], arg);
	    }
	}
    }
}
#endif

static void parse_cmdline_and_init_file (int argc, char **argv)
{
    char *home;

    strcpy (optionsfile, "");

#ifdef OPTIONS_IN_HOME
    home = getenv ("HOME");
    if (home != NULL && strlen (home) < 240)
    {
	strcpy (optionsfile, home);
	strcat (optionsfile, "/");
    }
#endif

    strcat (optionsfile, OPTIONSFILENAME);

    if (! cfgfile_load (&currprefs, optionsfile)) {
#ifdef OPTIONS_IN_HOME
	/* sam: if not found in $HOME then look in current directory */
	strcpy (optionsfile, OPTIONSFILENAME);
	cfgfile_load (&currprefs, optionsfile);
    }
#endif

    parse_cmdline (argc, argv);
}

void write_log_standard (const char *fmt, ...)
{
    va_list ap;
    va_start (ap, fmt);
#ifdef HAVE_VFPRINTF
    vfprintf (stderr, fmt, ap);
#else
    /* Technique stolen from GCC.  */
    {
	int x1, x2, x3, x4, x5, x6, x7, x8;
	x1 = va_arg (ap, int);
	x2 = va_arg (ap, int);
	x3 = va_arg (ap, int);
	x4 = va_arg (ap, int);
	x5 = va_arg (ap, int);
	x6 = va_arg (ap, int);
	x7 = va_arg (ap, int);
	x8 = va_arg (ap, int);
	fprintf (stderr, fmt, x1, x2, x3, x4, x5, x6, x7, x8);
    }
#endif
}

void reset_all_systems (void)
{
    bsdlib_reset ();
    filesys_reset ();
    filesys_start_threads ();
    scsidev_reset ();
    scsidev_start_threads ();
}

/* Okay, this stuff looks strange, but it is here to encourage people who
 * port UAE to re-use as much of this code as possible. Functions that you
 * should be using are do_start_program() and do_leave_program(), as well
 * as real_main(). Some OSes don't call main() (which is braindamaged IMHO,
 * but unfortunately very common), so you need to call real_main() from
 * whatever entry point you have. You may want to write your own versions
 * of start_program() and leave_program() if you need to do anything special.
 * Add #ifdefs around these as appropriate.
 */

void do_start_program (void)
{
    /* Do a reset on startup. Whether this is elegant is debatable. */
    quit_program = 2;
    m68k_go (1);
}

void do_leave_program (void)
{
    graphics_leave ();
    close_joystick ();
    close_sound ();
    dump_counts ();
    serial_exit ();
    zfile_exit ();
    if (! no_gui)
	gui_exit ();
}

void start_program (void)
{
    do_start_program ();
}

void leave_program (void)
{
    do_leave_program ();
}

void real_main (int argc, char **argv)
{
    FILE *hf;

    default_prefs (&currprefs);
    
    if (! graphics_setup ()) {
	exit (1);
    }

    rtarea_init ();
    hardfile_install ();
    scsidev_install ();

    parse_cmdline_and_init_file (argc, argv);

    machdep_init ();

    if (! setup_sound ()) {
	fprintf (stderr, "Sound driver unavailable: Sound output disabled\n");
	currprefs.produce_sound = 0;
    }
    init_joystick ();

    no_gui = ! currprefs.start_gui;
    if (! no_gui) {
	int err = gui_init ();
	if (err == -1) {
	    fprintf (stderr, "Failed to initialize the GUI\n");
	} else if (err == -2) {
	    exit (0);
	}
    }
    if (sound_available && currprefs.produce_sound > 1 && ! init_sound ()) {
	fprintf (stderr, "Sound driver unavailable: Sound output disabled\n");
	currprefs.produce_sound = 0;
    }

    fix_options ();
    changed_prefs = currprefs;

    /* Install resident module to get 8MB chipmem, if requested */
    rtarea_setup ();

    keybuf_init (); /* Must come after init_joystick */

    expansion_init ();
    memory_init ();

    filesys_install ();
    gfxlib_install ();
    emulib_install ();
    uaeexe_install ();
    native2amiga_install ();

    custom_init (); /* Must come after memory_init */
    serial_init ();
    DISK_init ();

    reset_frame_rate_hack ();
    init_m68k(); /* must come after reset_frame_rate_hack (); */

    compiler_init ();
    gui_update ();

    if (graphics_init ()) {
	setup_brkhandler ();
	if (currprefs.start_debugger && debuggable ())
	    activate_debugger ();

	start_program ();
    }
    leave_program ();
}

#ifndef NO_MAIN_IN_MAIN_C
int main (int argc, char **argv)
{
    real_main (argc, argv);
    return 0;
}
#endif

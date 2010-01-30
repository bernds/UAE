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

#include "options.h"
#include "threaddep/thread.h"
#include "uae.h"
#include "gensound.h"
#include "audio.h"
#include "sounddep/sound.h"
#include "events.h"
#include "memory.h"
#include "custom.h"
#include "serial.h"
#include "newcpu.h"
#include "disk.h"
#include "debug.h"
#include "xwin.h"
#include "inputdevice.h"
#include "keybuf.h"
#include "gui.h"
#include "zfile.h"
#include "autoconf.h"
#include "filesys.h"
#include "osemu.h"
#include "picasso96.h"
#include "traps.h"
#include "bsdsocket.h"
#include "uaeexe.h"
#include "native2amiga.h"
#include "scsidev.h"
#include "romlist.h"

#ifdef USE_SDL
#include "SDL.h"
#endif

struct uae_prefs currprefs, changed_prefs;
struct gfx_params *curr_gfx;

int no_gui = 0;
int joystickpresent = 0;
int cloanto_rom = 0;

struct gui_info gui_data;

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

struct uae_rect *gfx_fullscreen_modes, *gfx_windowed_modes;
int n_fullscreen_modes, n_windowed_modes;

static struct uae_rect default_windowed_modes[] =
{
    { 320, 256 },
    { 400, 300 },
    { 640, 512 },
    { 800, 600 }
};

struct config_list *predef_configs;
int n_predef_configs;
int predef_configs_space;

static int sortfn (const void *a, const void *b)
{
    struct config_list *pa = (struct config_list *)a;
    struct config_list *pb = (struct config_list *)b;
    return strcmp (pa->sortstr, pb->sortstr);
}

static void scan_configs (const char *path)
{
    DIR *dir;
    int pathlen = strlen (path);
    int bufsz = pathlen + 256;
    char *buffer;
    uae_u8 *data;

    predef_configs_space = 20;
    predef_configs = malloc (sizeof (struct config_list) * 20);
    n_predef_configs = 0;

    dir = opendir (path);
    if (!dir)
	return;

    buffer = malloc (bufsz);
    if (!buffer)
	goto out;
    data = malloc (1024 * 1024);
    if (!data)
	goto out1;

    strcpy (buffer, path);
    buffer[pathlen++] = '/';
    buffer[pathlen] = '\0';
    for (;;) {
	struct uae_prefs p;
	struct zfile *f;
	struct dirent *ent = readdir (dir);
	int len;
	struct romdata *rd;
	long size;

	if (!ent)
	    break;

	len = strlen (ent->d_name);
	if (len + pathlen + 1 >= bufsz) {
	    bufsz = len + pathlen + 200;
	    buffer = realloc (buffer, bufsz);
	    if (!buffer) {
		free (data);
		goto out;
	    }
	}
	strcpy (buffer + pathlen, ent->d_name);

	if (n_predef_configs >= predef_configs_space) {
	    predef_configs_space += 20;
	    predef_configs = realloc (predef_configs,
				      sizeof (struct config_list) * predef_configs_space);
	}
	default_prefs (&p);
	strcpy (p.description, "");
	strcpy (p.sortstr, "");
	cfgfile_load (&p, buffer);
	if (strlen (p.description) > 0) {
	    predef_configs[n_predef_configs].filename = strdup (buffer);
	    predef_configs[n_predef_configs].description = strdup (p.description);
	    predef_configs[n_predef_configs++].sortstr = strdup (p.sortstr);
	}
    }
    qsort (predef_configs, n_predef_configs, sizeof *predef_configs, sortfn);
    free (data);
  out1:
    free (buffer);
  out:
    closedir (dir);
}

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
    p->leds_on_screen = 1;

    p->unknown_lines = 0;
    /* Note to porters: please don't change any of these options! UAE is supposed
     * to behave identically on all platforms if possible. */
    p->illegal_mem = 0;
    p->use_serial = 0;
    p->serial_demand = 0;
    p->parallel_demand = 0;

    p->jport0 = JSEM_MICE;
    p->jport1 = JSEM_JOYS;

    p->keyboard_lang = KBD_LANG_US;
    p->emul_accuracy = 2;
    p->test_drawing_speed = 0;

    p->produce_sound = 3;
    p->sound_stereo = 0;
    p->sound_stereo_separation = 7;
    p->sound_mixed_stereo_delay = 0;
    p->sound_freq = DEFAULT_SOUND_FREQ;
    p->sound_maxbsiz = DEFAULT_SOUND_MAXB;
    p->sound_interpol = 0;
    p->sound_filter = FILTER_SOUND_OFF;
    p->sound_filter_type = FILTER_SOUND_TYPE_A500;

    p->gfx_framerate = 1;
    p->gfx_w.width = 800;
    p->gfx_w.height = 600;
    p->gfx_w.lores = 0;
    p->gfx_w.linedbl = 2;
    p->gfx_w.correct_aspect = 0;
    p->gfx_w.xcenter = 0;
    p->gfx_w.ycenter = 0;
    p->gfx_f = p->gfx_w;
    p->gfx_afullscreen = 0;
    p->gfx_pfullscreen = 0;
    p->color_mode = 0;

    p->x11_use_low_bandwidth = 0;
    p->x11_use_mitshm = 0;
    p->x11_hide_cursor = 1;

    p->svga_no_linear = 0;

    p->curses_reverse_video = 0;

    p->win32_middle_mouse = 0;
    p->win32_logfile = 0;
    p->win32_iconified_nospeed = 0;
    p->win32_iconified_nosound = 0;
    p->win32_no_overlay = 0;

    p->immediate_blits = 0;
    p->collision_level = 1;
 
    p->chipset_mask = CSMASK_ECS_AGNUS;
    p->cs_rtc = 2;
    p->cs_a1000ram = 0;
    p->cs_fatgaryrev = -1;
    p->cs_ramseyrev = -1;
    p->cs_ide = 0;

    strcpy (p->df[0], "df0.adf");
    strcpy (p->df[1], "df1.adf");
    strcpy (p->df[2], "df2.adf");
    strcpy (p->df[3], "df3.adf");

    strcpy (p->romfile, "kick.rom");
    strcpy (p->keyfile, "");
    strcpy (p->prtname, DEFPRTNAME);
    p->rom_crc32 = 0;

    strcpy (p->path_rom, "./");
    strcpy (p->path_floppy, "./");
    strcpy (p->path_hardfile, "./");

    strcpy (p->prtname, "");
    strcpy (p->sername, "");

    p->nr_floppies = 2;
    p->dfxtype[0] = DRV_35_DD;
    p->dfxtype[1] = DRV_35_DD;
    p->dfxtype[2] = DRV_NONE;
    p->dfxtype[3] = DRV_NONE;

    p->m68k_speed = 0;
    p->cpu_model = 68020;
    p->fpu_model = 0;
    p->address_space_24 = 0;

    p->fastmem_size = 0x00000000;
    p->mbresmem_low_size = 0x00000000;
    p->mbresmem_high_size = 0x00000000;
    p->z3fastmem_size = 0x00000000;
    p->chipmem_size = 0x00200000;
    p->bogomem_size = 0x00000000;
    p->gfxmem_size = 0x00000000;

    p->mountinfo = alloc_mountinfo ();
    inputdevice_default_prefs (p);
    p->bootrom = 1;
}

int fixup_prefs_dimensions (struct gfx_params *p, struct uae_rect *modes, int n_modes)
{
    int i;
    if (p->width < 320)
	p->width = 320;
    if (p->height < 200)
	p->height = 200;
    for (i = 0; i < n_modes; i++) {
	if ((modes[i].w >= p->width && modes[i].h >= p->height)
	    || i == n_modes - 1)
	{
	    p->width = modes[i].w;
	    p->height = modes[i].h;
	    p->lores = p->width <= 512;
	    if (p->height > 300 && !p->linedbl)
		p->linedbl = 1;
	    return i;
	}
    }
}

void fixup_cpu (struct uae_prefs *p)
{
}

void fixup_sound (struct uae_prefs *p)
{
    if (p->sound_stereo_separation < 0)
	p->sound_stereo_separation = 0;
    if (p->sound_stereo_separation > MIXED_STEREO_MAX)
	p->sound_stereo_separation = MIXED_STEREO_MAX;
}

static void fixup_prefs (struct uae_prefs *p)
{
    int err = 0;

    if ((p->chipmem_size & (p->chipmem_size - 1)) != 0
	|| p->chipmem_size < 0x80000
	|| p->chipmem_size > 0x800000)
    {
	p->chipmem_size = 0x200000;
	write_log ("Unsupported chipmem size!\n");
	err = 1;
    }
    if ((p->fastmem_size & (p->fastmem_size - 1)) != 0
	|| (p->fastmem_size != 0 && (p->fastmem_size < 0x100000 || p->fastmem_size > 0x800000)))
    {
	p->fastmem_size = 0;
	write_log ("Unsupported fastmem size!\n");
	err = 1;
    }
    if ((p->gfxmem_size & (p->gfxmem_size - 1)) != 0
	|| (p->gfxmem_size != 0 && (p->gfxmem_size < 0x100000 || p->gfxmem_size > 0x2000000)))
    {
	write_log ("Unsupported graphics card memory size %lx!\n", p->gfxmem_size);
	p->gfxmem_size = 0;
	err = 1;
    }
    if ((p->z3fastmem_size & (p->z3fastmem_size - 1)) != 0
	|| (p->z3fastmem_size != 0 && (p->z3fastmem_size < 0x100000 || p->z3fastmem_size > 0x4000000)))
    {
	p->z3fastmem_size = 0;
	write_log ("Unsupported Zorro III fastmem size!\n");
	err = 1;
    }
    if (p->address_space_24 && (p->gfxmem_size != 0 || p->z3fastmem_size != 0)) {
	p->z3fastmem_size = p->gfxmem_size = 0;
	write_log ("Can't use a graphics card or Zorro III fastmem when using a 24 bit\n"
		 "address space - sorry.\n");
    }
    if ((p->bogomem_size & (p->bogomem_size - 1)) != 0
	|| (p->bogomem_size != 0 && (p->bogomem_size < 0x80000 || p->bogomem_size > 0x100000)))
    {
	p->bogomem_size = 0;
	write_log ("Unsupported bogomem size!\n");
	err = 1;
    }

    if (p->chipmem_size > 0x200000 && p->fastmem_size != 0) {
	write_log ("You can't use fastmem and more than 2MB chip at the same time!\n");
	p->fastmem_size = 0;
	err = 1;
    }
#if 0
    if (p->m68k_speed < -1 || p->m68k_speed > 20) {
	write_log ("Bad value for -w parameter: must be -1, 0, or within 1..20.\n");
	p->m68k_speed = 4;
	err = 1;
    }
#endif

    if (p->produce_sound < 0 || p->produce_sound > 3) {
	write_log ("Bad value for -S parameter: enable value must be within 0..3\n");
	p->produce_sound = 0;
	err = 1;
    }
    if (p->cpu_model < 68020 && p->z3fastmem_size > 0) {
	write_log ("Z3 fast memory can't be used with a 68000/68010 emulation. It\n"
		 "requires a 68020 emulation. Turning off Z3 fast memory.\n");
	p->z3fastmem_size = 0;
	err = 1;
    }
    if (p->gfxmem_size > 0 && (p->cpu_model < 68020 || p->address_space_24)) {
	write_log ("Picasso96 can't be used with a 68000/68010 or 68EC020 emulation. It\n"
		 "requires a 68020 emulation. Turning off Picasso96.\n");
	p->gfxmem_size = 0;
	err = 1;
    }
#ifndef BSDSOCKET
    if (p->socket_emu) {
	write_log ("Compile-time option of BSDSOCKET_SUPPORTED was not enabled.  You can't use bsd-socket emulation.\n");
	p->socket_emu = 0;
	err = 1;
    }
#endif

    if (p->nr_floppies < 1 || p->nr_floppies > 4) {
	write_log ("Invalid number of floppies.  Using 4.\n");
	p->nr_floppies = 4;
	err = 1;
    }
    if (p->collision_level < 0 || p->collision_level > 3) {
	write_log ("Invalid collision support level.  Using 1.\n");
	p->collision_level = 1;
	err = 1;
    }

    fixup_sound (p);

    if (err)
	write_log ("Please use \"uae -h\" to get usage information.\n");
}

int quit_program = 0;

void uae_reset (int hardreset)
{
    if (quit_program == 0) {
	quit_program = -2;
	if (hardreset)
	    quit_program = -3;
    }
}

void uae_quit (void)
{
    if (quit_program != -1)
	quit_program = -1;
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
		write_log ("Missing argument for '-f' option.\n");
	    else
		cfgfile_load (&currprefs, argv[++i]);
	} else if (strcmp (argv[i], "-s") == 0) {
	    if (i + 1 == argc)
		write_log ("Missing argument for '-s' option.\n");
	    else
		cfgfile_parse_line (&currprefs, argv[++i]);
	} else if (strcmp (argv[i], "-h") == 0 || strcmp (argv[i], "-help") == 0) {
	    usage ();
	    exit (0);
	} else {
	    if (argv[i][0] == '-' && argv[i][1] != '\0') {
		const char *arg = argv[i] + 2;
		int extra_arg = *arg == '\0';
		if (extra_arg)
		    arg = i + 1 < argc ? argv[i + 1] : 0;
		if (parse_cmdline_option (&currprefs, argv[i][1], arg) && extra_arg)
		    i++;
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
	/* If not found in $HOME then look in current directory.  However,
	 * don't use the optionsfile variable, so that we will save changes
	 * to the home directory.  */
	char pwd_optionsfile[256];
	strcpy (optionsfile, OPTIONSFILENAME);
	cfgfile_load (&currprefs, optionsfile);
#endif
    }

    parse_cmdline (argc, argv);
}

void reset_all_systems (void)
{
    init_eventtab ();

    memory_reset ();
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
    if (quit_program == -1)
	return;
    inputdevice_updateconfig (&currprefs);

    /* Do a reset on startup. Whether this is elegant is debatable. */
    if (quit_program >= 0)
	quit_program = 2;
    m68k_go (1);
}

void do_leave_program (void)
{
    graphics_leave ();
    inputdevice_close ();
    close_sound ();
    dump_counts ();
    serial_exit ();
    zfile_exit ();
    if (! no_gui)
	gui_exit ();
#ifdef USE_SDL
    SDL_Quit ();
#endif
    expansion_cleanup ();
    memory_cleanup ();
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

#ifdef USE_SDL
    SDL_Init (SDL_INIT_EVERYTHING | SDL_INIT_NOPARACHUTE);
#endif

    default_prefs (&currprefs);

#ifdef SYSTEM_CFGDIR
    scan_configs (SYSTEM_CFGDIR);
#endif

    /* Can be overriden in graphics_setup, although there's not much of a
       point.  Fullscreen modes are filled in by graphics_setup.  */
    gfx_windowed_modes = default_windowed_modes;
    n_windowed_modes = sizeof default_windowed_modes / sizeof *default_windowed_modes;

    if (! graphics_setup ()) {
	exit (1);
    }

    rtarea_init ();
    hardfile_install ();
    scsidev_install ();

    parse_cmdline_and_init_file (argc, argv);

    machdep_init ();
    init_gtod ();

    if (! setup_sound ()) {
	write_log ("Sound driver unavailable: Sound output disabled\n");
	currprefs.produce_sound = 0;
    }
    inputdevice_init ();

    changed_prefs = currprefs;
    no_gui = ! currprefs.start_gui;
    if (! no_gui) {
	int err = gui_init (1);
	currprefs = changed_prefs;
	if (err == -1) {
	    write_log ("Failed to initialize the GUI\n");
	} else if (err == -2) {
	    exit (0);
	}
    }
    if (sound_available && currprefs.produce_sound > 1 && ! init_audio ()) {
	write_log ("Sound driver unavailable: Sound output disabled\n");
	currprefs.produce_sound = 0;
    }

    fixup_prefs (&currprefs);
    changed_prefs = currprefs;

#ifdef SYSTEM_ROMDIR
    scan_roms (SYSTEM_ROMDIR, ROMLOC_SYSTEM);
#endif
    scan_roms (currprefs.path_rom, ROMLOC_USER);
    /* Install resident module to get 8MB chipmem, if requested */
    rtarea_setup ();

    keybuf_init (); /* Must come after init_joystick */

    expansion_init ();
    memory_init ();

    filesys_install ();
    bsdlib_install ();
    emulib_install ();
    uaeexe_install ();
    native2amiga_install ();

    custom_init (); /* Must come after memory_init */
    serial_init ();
    DISK_init ();

    reset_frame_rate_hack ();
    init_m68k(); /* must come after reset_frame_rate_hack (); */

    gui_update ();

    if (graphics_init ()) {
	setup_brkhandler ();
	if (currprefs.start_debugger && debuggable ())
	    activate_debugger ();

	start_program ();
    }
    leave_program ();
}

void uae_abort (const char *msg)
{
    write_log ("%s", msg);
    abort ();
}

#ifndef NO_MAIN_IN_MAIN_C
int main (int argc, char **argv)
{
    real_main (argc, argv);
    return 0;
}
#endif

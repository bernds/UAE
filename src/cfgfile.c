 /*
  * UAE - The Un*x Amiga Emulator
  *
  * Config file handling
  * This still needs some thought before it's complete...
  *
  * Copyright 1998 Brian King, Bernd Schmidt
  */

#include "sysconfig.h"
#include "sysdeps.h"

#include <ctype.h>

#include "options.h"
#include "threaddep/thread.h"
#include "uae.h"
#include "autoconf.h"
#include "events.h"
#include "custom.h"
#include "audio.h"
#include "inputdevice.h"

#define cfgfile_write fprintf

/* @@@ need to get rid of this... just cut part of the manual and print that
 * as a help text.  */
struct cfg_lines
{
    const char *config_label, *config_help;
};

static struct cfg_lines opttable[] =
{
    {"help", "Prints this help" },
    {"config_description", "" },
    {"use_gui", "Enable the GUI?  If no, then goes straight to emulator" },
    {"use_debugger", "Enable the debugger?" },
    {"cpu_speed", "can be max, real, or a number between 1 and 20" },
    {"cpu_type", "Can be 68000, 68010, 68020, 68020/68881" },
    {"cpu_24bit_addressing", "must be set to 'no' in order for Z3mem or P96mem to work" },
    {"log_illegal_mem", "print illegal memory access by Amiga software?" },
    {"fastmem_size", "Size in megabytes of fast-memory" },
    {"chipmem_size", "Size in megabytes of chip-memory" },
    {"bogomem_size", "Size in megabytes of bogo-memory at 0xC00000" },
    {"a3000mem_size", "Size in megabytes of A3000 memory" },
    {"gfxcard_size", "Size in megabytes of Picasso96 graphics-card memory" },
    {"z3mem_size", "Size in megabytes of Zorro-III expansion memory" },
    {"gfx_test_speed", "Test graphics speed?" },
    {"framerate", "Print every nth frame" },
    {"gfx_width", "Screen width" },
    {"gfx_height", "Screen height" },
    {"gfx_lores", "Treat display as lo-res?" },
    {"gfx_linemode", "Can be none, double, or scanlines" },
    {"gfx_fullscreen_amiga", "Amiga screens are fullscreen?" },
    {"gfx_fullscreen_picasso", "Picasso screens are fullscreen?" },
    {"gfx_correct_aspect", "Correct aspect ratio?" },
    {"gfx_center_horizontal", "Center display horizontally?" },
    {"gfx_center_vertical", "Center display vertically?" },
    {"gfx_colour_mode", "" },
    {"32bit_blits", "Enable 32 bit blitter emulation" },
    {"immediate_blits", "Perform blits immediately" },
    {"show_leds", "LED display" },
    {"sound_output", "" },
    {"sound_frequency", "" },
    {"sound_channels", "" },
    {"sound_max_buff", "" },
    {"parallel_on_demand", "" },
    {"serial_on_demand", "" },
    {"joyport0", "" },
    {"joyport1", "" },
    {"kickstart_rom_file", "Kickstart ROM image, (C) Copyright Amiga, Inc." },
    {"kickstart_ext_rom_file", "Extended Kickstart ROM image, (C) Copyright Amiga, Inc." },
    {"kickstart_key_file", "Key-file for encrypted ROM images (from Cloanto's Amiga Forever)" },
    {"floppy0", "Diskfile for drive 0" },
    {"floppy1", "Diskfile for drive 1" },
    {"floppy2", "Diskfile for drive 2" },
    {"floppy3", "Diskfile for drive 3" },
    {"hardfile", "access,sectors, surfaces, reserved, blocksize, path format" },
    {"filesystem", "access,'Amiga volume-name':'host directory path' - where 'access' can be 'read-only' or 'read-write'" }
};

static const char *guimode1[] = { "no", "yes", "nowait", 0 };
static const char *guimode2[] = { "false", "true", "nowait", 0 };
static const char *guimode3[] = { "0", "1", "nowait", 0 };
static const char *csmode[] = { "ocs", "ecs_agnus", "ecs_denise", "ecs", "aga", 0 };
static const char *linemode1[] = { "none", "double", "scanlines", 0 };
static const char *linemode2[] = { "n", "d", "s", 0 };
static const char *speedmode[] = { "max", "real", 0 };
static const char *cpumode[] = {
    "68000", "68010",
    "68ec020", "68ec020/68881", "68ec020/68882",
    "68020", "68020/68881", "68020/68882",
    "68030", "68030/68881", "68030/68882",
    "68040", "68060", 0
};
static int mode_to_cpumodel[] = {
    68000, 68010,
    68020, 68020, 68020,
    68020, 68020, 68020,
    68030, 68030, 68030,
    68040, 68060
};
static int mode_to_fpumodel[] = {
    0, 0,
    0, 68881, 68882,
    0, 68881, 68882,
    0, 68881, 68882,
    68040, 68060
};
static const char *colormode1[] = { "8bit", "15bit", "16bit", "8bit_dither", "4bit_dither", "32bit", 0 };
static const char *colormode2[] = { "8", "15", "16", "8d", "4d", "32", 0 };
static const char *soundmode[] = { "none", "interrupts", "normal", "exact", 0 };
static const char *centermode1[] = { "none", "simple", "smart", 0 };
static const char *centermode2[] = { "false", "true", "smart", 0 };
static const char *stereomode[] = { "mono", "stereo", "mixed", 0 };
static const char *interpolmode[] = { "none", "rh", "crux", "sinc", 0 };
static const char *collmode[] = { "none", "sprites", "playfields", "full", 0 };

static const char *obsolete[] = {
    "accuracy", "gfx_opengl", "gfx_32bit_blits", "32bit_blits",
    "gfx_immediate_blits", "gfx_ntsc", "win32", "gfx_filter_bits",
    "sound_pri_cutoff", "sound_pri_time", "sound_min_buff", "sound_bits",
    "gfx_test_speed", "gfxlib_replacement", "enforcer", "catweasel_io",
    "fast_copper", "sound_adjust", "serial_hardware_dtrdsr", "cpu_compatible",
    0
};

#define UNEXPANDED "$(FILE_PATH)"

static int match_string (const char *table[], const char *str)
{
    int i;
    for (i = 0; table[i] != 0; i++)
	if (strcasecmp (table[i], str) == 0)
	    return i;
    return -1;
}

char *cfgfile_subst_path (const char *path, const char *subst, const char *file)
{
    /* @@@ use strcasecmp for some targets.  */
    if (strlen (path) > 0 && strncmp (file, path, strlen (path)) == 0) {
	int l;
	char *p = xmalloc (strlen (file) + strlen (subst) + 2);
	strcpy (p, subst);
	l = strlen (p);
	while (l > 0 && p[l - 1] == '/')
	    p[--l] = '\0';
	l = strlen (path);
	while (file[l] == '/')
	    l++;
	strcat (p, "/"); strcat (p, file + l);
	return p;
    }
    return my_strdup (file);
}

static void write_gfx_params (FILE *f, struct gfx_params *p, char *suffix)
{
    cfgfile_write (f, "gfx_width_%s=%d\n", suffix, p->width);
    cfgfile_write (f, "gfx_height_%s=%d\n", suffix, p->height);
    cfgfile_write (f, "gfx_lores_%s=%s\n", suffix, p->lores ? "true" : "false");
    cfgfile_write (f, "gfx_linemode_%s=%s\n", suffix, linemode1[p->linedbl]);
    cfgfile_write (f, "gfx_correct_aspect_%s=%s\n", suffix, p->correct_aspect ? "true" : "false");
    cfgfile_write (f, "gfx_center_horizontal_%s=%s\n", suffix, centermode1[p->xcenter]);
    cfgfile_write (f, "gfx_center_vertical_%s=%s\n", suffix, centermode1[p->ycenter]);
}

void save_options (FILE *f, struct uae_prefs *p)
{
    struct strlist *sl;
    char *str;
    int i;

    cfgfile_write (f, "config_description=%s\n", p->description);

    for (sl = p->unknown_lines; sl; sl = sl->next)
	cfgfile_write (f, "%s\n", sl->str);

    cfgfile_write (f, "%s.rom_path=%s\n", TARGET_NAME, p->path_rom);
    cfgfile_write (f, "%s.floppy_path=%s\n", TARGET_NAME, p->path_floppy);
    cfgfile_write (f, "%s.hardfile_path=%s\n", TARGET_NAME, p->path_hardfile);

    target_save_options (f, p);

    cfgfile_write (f, "use_gui=%s\n", guimode1[p->start_gui]);
    cfgfile_write (f, "use_debugger=%s\n", p->start_debugger ? "true" : "false");
    str = cfgfile_subst_path (p->path_rom, UNEXPANDED, p->romfile);
    cfgfile_write (f, "kickstart_rom_file=%s\n", str);
    free (str);
    str = cfgfile_subst_path (p->path_rom, UNEXPANDED, p->romextfile);
    cfgfile_write (f, "kickstart_ext_rom_file=%s\n", str);
    free (str);
    str = cfgfile_subst_path (p->path_rom, UNEXPANDED, p->keyfile);
    cfgfile_write (f, "kickstart_key_file=%s\n", str);
    free (str);
    cfgfile_write (f, "kickshifter=%s\n", p->kickshifter ? "true" : "false");

    for (i = 0; i < 4; i++) {
	str = cfgfile_subst_path (p->path_floppy, UNEXPANDED, p->df[i]);
	cfgfile_write (f, "floppy%d=%s\n", i, str);
	free (str);
    }
    cfgfile_write (f, "nr_floppies=%d\n", p->nr_floppies);
    cfgfile_write (f, "parallel_on_demand=%s\n", p->parallel_demand ? "true" : "false");
    cfgfile_write (f, "serial_on_demand=%s\n", p->serial_demand ? "true" : "false");

    cfgfile_write (f, "sound_output=%s\n", soundmode[p->produce_sound]);
    cfgfile_write (f, "sound_channels=%s\n", stereomode[p->sound_stereo]);
    cfgfile_write (f, "sound_stereo_separation=%d\n", p->sound_stereo_separation);
    cfgfile_write (f, "sound_stereo_mixing_delay=%d\n", p->sound_mixed_stereo_delay >= 0 ? p->sound_mixed_stereo_delay : 0);
    cfgfile_write (f, "sound_max_buff=%d\n", p->sound_maxbsiz);
    cfgfile_write (f, "sound_frequency=%d\n", p->sound_freq);
    cfgfile_write (f, "sound_interpol=%s\n", interpolmode[p->sound_interpol]);

    for (i = 0; i < 2; i++) {
	int v = i == 0 ? p->jport0 : p->jport1;
	char tmp1[100], tmp2[50];
	if (v < JSEM_JOYS) {
	    sprintf (tmp2, "kbd%d", v + 1);
	} else if (v < JSEM_MICE) {
	    sprintf (tmp2, "joy%d", v - JSEM_JOYS);
	} else {
	    strcpy (tmp2, "mouse");
	    if (v - JSEM_MICE > 0)
		sprintf (tmp2, "mouse%d", v - JSEM_MICE);
	}
	sprintf (tmp1, "joyport%d=%s\n", i, tmp2);
	cfgfile_write (f, tmp1);
    }

    cfgfile_write (f, "bsdsocket_emu=%s\n", p->socket_emu ? "true" : "false");

    cfgfile_write (f, "gfx_framerate=%d\n", p->gfx_framerate);
    write_gfx_params (f, &p->gfx_w, "windowed");
    write_gfx_params (f, &p->gfx_f, "fullscreen");
    cfgfile_write (f, "gfx_fullscreen_amiga=%s\n", p->gfx_afullscreen ? "true" : "false");
    cfgfile_write (f, "gfx_fullscreen_picasso=%s\n", p->gfx_pfullscreen ? "true" : "false");
    cfgfile_write (f, "gfx_colour_mode=%s\n", colormode1[p->color_mode]);

    cfgfile_write (f, "immediate_blits=%s\n", p->immediate_blits ? "true" : "false");
    cfgfile_write (f, "ntsc=%s\n", p->ntscmode ? "true" : "false");
    cfgfile_write (f, "show_leds=%s\n", p->leds_on_screen ? "true" : "false");
    if (p->chipset_mask & CSMASK_AGA)
	cfgfile_write (f, "chipset=aga\n");
    else if ((p->chipset_mask & CSMASK_ECS_AGNUS) && (p->chipset_mask & CSMASK_ECS_AGNUS))
	cfgfile_write (f, "chipset=ecs\n");
    else if (p->chipset_mask & CSMASK_ECS_AGNUS)
	cfgfile_write (f, "chipset=ecs_agnus\n");
    else if (p->chipset_mask & CSMASK_ECS_DENISE)
	cfgfile_write (f, "chipset=ecs_denise\n");
    else
	cfgfile_write (f, "chipset=ocs\n");
    cfgfile_write (f, "collision_level=%s\n", collmode[p->collision_level]);

    cfgfile_write (f, "fastmem_size=%d\n", p->fastmem_size / 0x100000);
    cfgfile_write (f, "a3000mem_size=%d\n", p->a3000mem_size / 0x100000);
    cfgfile_write (f, "z3mem_size=%d\n", p->z3fastmem_size / 0x100000);
    cfgfile_write (f, "bogomem_size=%d\n", p->bogomem_size / 0x40000);
    cfgfile_write (f, "gfxcard_size=%d\n", p->gfxmem_size / 0x100000);
    cfgfile_write (f, "chipmem_size=%d\n", p->chipmem_size / 0x80000);

    if (p->m68k_speed > 0)
	cfgfile_write (f, "finegrain_cpu_speed=%d\n", p->m68k_speed);
    else
	cfgfile_write (f, "cpu_speed=%s\n", p->m68k_speed == -1 ? "max" : "real");

    for (i = 0; cpumode[i] != NULL; i++)
	if (mode_to_cpumodel[i] == p->cpu_model && mode_to_fpumodel[i] == p->fpu_model)
	    cfgfile_write (f, "cpu_type=%s\n", cpumode[i]);

    cfgfile_write (f, "log_illegal_mem=%s\n", p->illegal_mem ? "true" : "false");

    cfgfile_write (f, "kbd_lang=%s\n", (p->keyboard_lang == KBD_LANG_DE ? "de"
				  : p->keyboard_lang == KBD_LANG_DK ? "dk"
				  : p->keyboard_lang == KBD_LANG_ES ? "es"
				  : p->keyboard_lang == KBD_LANG_US ? "us"
				  : p->keyboard_lang == KBD_LANG_SE ? "se"
				  : p->keyboard_lang == KBD_LANG_FR ? "fr"
				  : p->keyboard_lang == KBD_LANG_IT ? "it"
				  : "FOO"));

    write_filesys_config (p->mountinfo, UNEXPANDED, p->path_hardfile, f);

    /* Don't write gfxlib/gfx_test_speed options.  */
}

int cfgfile_yesno (const char *option, const char *value, const char *name, int *location)
{
    if (strcmp (option, name) != 0)
	return 0;
    if (strcasecmp (value, "yes") == 0 || strcasecmp (value, "y") == 0
	|| strcasecmp (value, "true") == 0 || strcasecmp (value, "t") == 0)
	*location = 1;
    else if (strcasecmp (value, "no") == 0 || strcasecmp (value, "n") == 0
	|| strcasecmp (value, "false") == 0 || strcasecmp (value, "f") == 0)
	*location = 0;
    else
	write_log ("Option `%s' requires a value of either `yes' or `no'.\n", option);
    return 1;
}

int cfgfile_intval (const char *option, const char *value, const char *name, int *location, int scale)
{
    int base = 10;
    char *endptr;
    if (strcmp (option, name) != 0)
	return 0;
    /* I guess octal isn't popular enough to worry about here...  */
    if (value[0] == '0' && value[1] == 'x')
	value += 2, base = 16;
    *location = strtol (value, &endptr, base) * scale;

    if (*endptr != '\0' || *value == '\0')
	write_log ("Option `%s' requires a numeric argument.\n", option);
    return 1;
}

int cfgfile_strval (const char *option, const char *value, const char *name, int *location, const char *table[], int more)
{
    int val;
    if (strcmp (option, name) != 0)
	return 0;
    val = match_string (table, value);
    if (val == -1) {
	if (more)
	    return 0;

	write_log ("Unknown value for option `%s'.\n", option);
	return 1;
    }
    *location = val;
    return 1;
}

int cfgfile_string (const char *option, const char *value, const char *name, char *location, int maxsz)
{
    if (strcmp (option, name) != 0)
	return 0;
    strncpy (location, value, maxsz - 1);
    location[maxsz - 1] = '\0';
    return 1;
}

static int getintval (char **p, int *result, int delim)
{
    char *value = *p;
    int base = 10;
    char *endptr;
    char *p2 = strchr (*p, delim);

    if (p2 == 0)
	return 0;

    *p2++ = '\0';

    if (value[0] == '0' && value[1] == 'x')
	value += 2, base = 16;
    *result = strtol (value, &endptr, base);
    *p = p2;

    if (*endptr != '\0' || *value == '\0')
	return 0;

    return 1;
}

static void set_chipset_mask (struct uae_prefs *p, int val)
{
    p->chipset_mask = (val == 0 ? 0
		       : val == 1 ? CSMASK_ECS_AGNUS
		       : val == 2 ? CSMASK_ECS_DENISE
		       : val == 3 ? CSMASK_ECS_DENISE | CSMASK_ECS_AGNUS
		       : CSMASK_AGA | CSMASK_ECS_DENISE | CSMASK_ECS_AGNUS);
}

static int cfgfile_parse_host (struct uae_prefs *p, char *option, char *value)
{
    int tmpval;
    int dummy;
    char *section = 0;
    char *tmpp;

    if (memcmp (option, "input.", 6) == 0) {
	read_inputdevice_config (p, option, value);
	return 1;
    }

    for (tmpp = option; *tmpp != '\0'; tmpp++)
	if (isupper (*tmpp))
	    *tmpp = tolower (*tmpp);
    tmpp = strchr (option, '.');
    if (tmpp) {
	section = option;
	option = tmpp + 1;
	*tmpp = '\0';
	if (strcmp (section, TARGET_NAME) == 0) {
	    /* We special case the various path options here.  */
	    if (cfgfile_string (option, value, "rom_path", p->path_rom, 256)
		|| cfgfile_string (option, value, "floppy_path", p->path_floppy, 256)
		|| cfgfile_string (option, value, "hardfile_path", p->path_hardfile, 256))
		return 1;
	    return target_parse_option (p, option, value);
	}
	return 0;
    }

    if (cfgfile_yesno (option, value, "use_debugger", &p->start_debugger)
	|| cfgfile_yesno (option, value, "bsdsocket_emu", &p->socket_emu)
	|| (cfgfile_yesno (option, value, "gfx_lores", &p->gfx_w.lores)
	    && cfgfile_yesno (option, value, "gfx_lores", &p->gfx_f.lores))
	|| cfgfile_yesno (option, value, "gfx_lores_windowed", &p->gfx_w.lores)
	|| cfgfile_yesno (option, value, "gfx_lores_fullscreen", &p->gfx_f.lores)
	|| (cfgfile_yesno (option, value, "gfx_correct_aspect", &p->gfx_w.correct_aspect)
	    && cfgfile_yesno (option, value, "gfx_correct_aspect", &p->gfx_f.correct_aspect))
	|| cfgfile_yesno (option, value, "gfx_correct_aspect_windowed", &p->gfx_w.correct_aspect)
	|| cfgfile_yesno (option, value, "gfx_correct_aspect_fullscreen", &p->gfx_f.correct_aspect)
	|| cfgfile_yesno (option, value, "gfx_fullscreen_amiga", &p->gfx_afullscreen)
	|| cfgfile_yesno (option, value, "gfx_fullscreen_picasso", &p->gfx_pfullscreen)
	|| cfgfile_yesno (option, value, "log_illegal_mem", &p->illegal_mem))
	return 1;
    if (cfgfile_intval (option, value, "sound_max_buff", &p->sound_maxbsiz, 1)
	|| cfgfile_intval (option, value, "sound_frequency", &p->sound_freq, 1)
	|| cfgfile_intval (option, value, "sound_stereo_separation", &p->sound_stereo_separation, 1)
	|| cfgfile_intval (option, value, "sound_stereo_mixing_delay", &p->sound_mixed_stereo_delay, 1)

	|| cfgfile_intval (option, value, "gfx_framerate", &p->gfx_framerate, 1)
	|| (cfgfile_intval (option, value, "gfx_width", &p->gfx_w.width, 1)
	    && cfgfile_intval (option, value, "gfx_width", &p->gfx_f.width, 1))
	|| cfgfile_intval (option, value, "gfx_width_windowed", &p->gfx_w.width, 1)
	|| cfgfile_intval (option, value, "gfx_width_fullscreen", &p->gfx_f.width, 1)
	|| (cfgfile_intval (option, value, "gfx_height", &p->gfx_w.height, 1)
	    && cfgfile_intval (option, value, "gfx_height", &p->gfx_f.height, 1))
	|| cfgfile_intval (option, value, "gfx_height_windowed", &p->gfx_w.height, 1)
	|| cfgfile_intval (option, value, "gfx_height_fullscreen", &p->gfx_f.height, 1))
	return 1;
    if (cfgfile_strval (option, value, "sound_output", &p->produce_sound, soundmode, 0)
	|| cfgfile_strval (option, value, "sound_interpol", &p->sound_interpol, interpolmode, 0)
	|| cfgfile_strval (option, value, "use_gui", &p->start_gui, guimode1, 1)
	|| cfgfile_strval (option, value, "use_gui", &p->start_gui, guimode2, 1)
	|| cfgfile_strval (option, value, "use_gui", &p->start_gui, guimode3, 0)
	|| (cfgfile_strval (option, value, "gfx_linemode", &p->gfx_w.linedbl, linemode1, 1)
	    && cfgfile_strval (option, value, "gfx_linemode", &p->gfx_f.linedbl, linemode1, 1))
	|| (cfgfile_strval (option, value, "gfx_linemode", &p->gfx_w.linedbl, linemode2, 0)
	    && cfgfile_strval (option, value, "gfx_linemode", &p->gfx_f.linedbl, linemode2, 0))
	|| cfgfile_strval (option, value, "gfx_linemode_windowed", &p->gfx_w.linedbl, linemode1, 1)
	|| cfgfile_strval (option, value, "gfx_linemode_windowed", &p->gfx_w.linedbl, linemode2, 0)
	|| cfgfile_strval (option, value, "gfx_linemode_fullscreen", &p->gfx_f.linedbl, linemode1, 1)
	|| cfgfile_strval (option, value, "gfx_linemode_fullscreen", &p->gfx_f.linedbl, linemode2, 0)
	|| (cfgfile_strval (option, value, "gfx_center_horizontal", &p->gfx_w.xcenter, centermode1, 1)
	    && cfgfile_strval (option, value, "gfx_center_horizontal", &p->gfx_f.xcenter, centermode1, 1))
	|| (cfgfile_strval (option, value, "gfx_center_horizontal", &p->gfx_w.xcenter, centermode2, 0)
	    && cfgfile_strval (option, value, "gfx_center_horizontal", &p->gfx_f.xcenter, centermode2, 0))
	|| cfgfile_strval (option, value, "gfx_center_horizontal_windowed", &p->gfx_w.xcenter, centermode1, 1)
	|| cfgfile_strval (option, value, "gfx_center_horizontal_windowed", &p->gfx_w.xcenter, centermode2, 0)
	|| cfgfile_strval (option, value, "gfx_center_horizontal_fullscreen", &p->gfx_f.xcenter, centermode1, 1)
	|| cfgfile_strval (option, value, "gfx_center_horizontal_fullscreen", &p->gfx_f.xcenter, centermode2, 0)
	|| (cfgfile_strval (option, value, "gfx_center_vertical", &p->gfx_w.ycenter, centermode1, 1)
	    && cfgfile_strval (option, value, "gfx_center_vertical", &p->gfx_f.ycenter, centermode1, 1))
	|| (cfgfile_strval (option, value, "gfx_center_vertical", &p->gfx_w.ycenter, centermode2, 0)
	    && cfgfile_strval (option, value, "gfx_center_vertical", &p->gfx_f.ycenter, centermode2, 0))
	|| cfgfile_strval (option, value, "gfx_center_vertical_windowed", &p->gfx_w.ycenter, centermode1, 1)
	|| cfgfile_strval (option, value, "gfx_center_vertical_windowed", &p->gfx_w.ycenter, centermode2, 0)
	|| cfgfile_strval (option, value, "gfx_center_vertical_fullscreen", &p->gfx_f.ycenter, centermode1, 1)
	|| cfgfile_strval (option, value, "gfx_center_vertical_fullscreen", &p->gfx_f.ycenter, centermode2, 0)
	|| cfgfile_strval (option, value, "gfx_colour_mode", &p->color_mode, colormode1, 1)
	|| cfgfile_strval (option, value, "gfx_colour_mode", &p->color_mode, colormode2, 0)
	|| cfgfile_strval (option, value, "gfx_color_mode", &p->color_mode, colormode1, 1)
	|| cfgfile_strval (option, value, "gfx_color_mode", &p->color_mode, colormode2, 0))
	return 1;

    if (strcmp (option, "joyport0") == 0 || strcmp (option, "joyport1") == 0) {
	int port = strcmp (option, "joyport0") == 0 ? 0 : 1;
	int start = -1;
	char *pp = 0;
	if (strncmp (value, "kbd", 3) == 0) {
	    start = JSEM_KBDLAYOUT;
	    pp = value + 3;
	} else if (strncmp (value, "joy", 3) == 0) {
	    start = JSEM_JOYS;
	    pp = value + 3;
	} else if (strncmp (value, "mouse", 5) == 0) {
	    start = JSEM_MICE;
	    pp = value + 5;
	}
	if (pp) {
	    int v = atol (pp);
	    if (start >= 0) {
		if (start == JSEM_KBDLAYOUT)
		    v--;
		if (v >= 0) {
		    start += v;
		    if (port)
			p->jport1 = start;
		    else
			p->jport0 = start;
		}
	    }
	}
	return 1;
    }

    if (cfgfile_string (option, value, "config_description", p->description, 256))
	return 1;

    /* Tricky ones... */
    if (cfgfile_strval (option, value, "sound_channels", &p->sound_stereo, stereomode, 1)) {
	if (p->sound_stereo == SND_NONE) { /* "mixed stereo" compatibility hack */
	    p->sound_stereo = SND_STEREO;
	    p->sound_mixed_stereo_delay = 5;
	    p->sound_stereo_separation = 7;
	}
	return 1;
    }

    if (strcmp (option, "kbd_lang") == 0) {
	KbdLang l;
	if ((l = KBD_LANG_DE, strcasecmp (value, "de") == 0)
	    || (l = KBD_LANG_DK, strcasecmp (value, "dk") == 0)
	    || (l = KBD_LANG_SE, strcasecmp (value, "se") == 0)
	    || (l = KBD_LANG_US, strcasecmp (value, "us") == 0)
	    || (l = KBD_LANG_FR, strcasecmp (value, "fr") == 0)
	    || (l = KBD_LANG_IT, strcasecmp (value, "it") == 0)
	    || (l = KBD_LANG_ES, strcasecmp (value, "es") == 0))
	    p->keyboard_lang = l;
	else
	    write_log ("Unknown keyboard language\n");
	return 1;
    }

    if (strcmp (option, "filesystem") == 0
	|| strcmp (option, "hardfile") == 0)
    {
	int secs, heads, reserved, bs, ro;
	char *aname, *root;
	char *tmpp = strchr (value, ',');
	char *str;
	if (tmpp == 0)
	    goto invalid_fs;

	*tmpp++ = '\0';
	if (strcmp (value, "0") == 0 || strcasecmp (value, "ro") == 0
	    || strcasecmp (value, "readonly") == 0
	    || strcasecmp (value, "read-only") == 0)
	    ro = 1;
	else if (strcmp (value, "1") == 0 || strcasecmp (value, "rw") == 0
		 || strcasecmp (value, "readwrite") == 0
		 || strcasecmp (value, "read-write") == 0)
	    ro = 0;
	else
	    goto invalid_fs;
	secs = 0; heads = 0; reserved = 0; bs = 0;

	value = tmpp;
	if (strcmp (option, "filesystem") == 0) {
	    tmpp = strchr (value, ':');
	    if (tmpp == 0)
		goto invalid_fs;
	    *tmpp++ = '\0';
	    aname = value;
	    root = tmpp;
	} else {
	    if (! getintval (&value, &secs, ',')
		|| ! getintval (&value, &heads, ',')
		|| ! getintval (&value, &reserved, ',')
		|| ! getintval (&value, &bs, ','))
		goto invalid_fs;
	    root = value;
	    aname = 0;
	}
	str = cfgfile_subst_path (UNEXPANDED, p->path_hardfile, root);
	tmpp = add_filesys_unit (p->mountinfo, aname, str, ro, secs,
				 heads, reserved, bs);
	free (str);
	if (tmpp)
	    write_log ("Error: %s\n", tmpp);
	return 1;

      invalid_fs:
	write_log ("Invalid filesystem/hardfile specification.\n");
	return 1;
    }

    return 0;
}

static int cfgfile_parse_hardware (struct uae_prefs *p, char *option, char *value)
{
    int tmpval, dummy, i;

    if (cfgfile_yesno (option, value, "immediate_blits", &p->immediate_blits)
	|| cfgfile_yesno (option, value, "kickshifter", &p->kickshifter)
	|| cfgfile_yesno (option, value, "ntsc", &p->ntscmode)
	|| cfgfile_yesno (option, value, "cpu_24bit_addressing", &p->address_space_24)
	|| cfgfile_yesno (option, value, "parallel_on_demand", &p->parallel_demand)
	|| cfgfile_yesno (option, value, "serial_on_demand", &p->serial_demand))
	return 1;
    
    if (cfgfile_intval (option, value, "fastmem_size", &p->fastmem_size, 0x100000)
	|| cfgfile_intval (option, value, "a3000mem_size", &p->a3000mem_size, 0x100000)
	|| cfgfile_intval (option, value, "z3mem_size", &p->z3fastmem_size, 0x100000)
	|| cfgfile_intval (option, value, "bogomem_size", &p->bogomem_size, 0x40000)
	|| cfgfile_intval (option, value, "gfxcard_size", &p->gfxmem_size, 0x100000)
	|| cfgfile_intval (option, value, "chipmem_size", &p->chipmem_size, 0x80000)
	|| cfgfile_intval (option, value, "nr_floppies", &p->nr_floppies, 1))
	return 1;

    if (cfgfile_strval (option, value, "collision_level", &p->collision_level, collmode, 0))
	return 1;
    
    if (cfgfile_string (option, value, "floppy0", p->df[0], 256)
	|| cfgfile_string (option, value, "floppy1", p->df[1], 256)
	|| cfgfile_string (option, value, "floppy2", p->df[2], 256)
	|| cfgfile_string (option, value, "floppy3", p->df[3], 256)
	|| cfgfile_string (option, value, "kickstart_rom_file", p->romfile, 256)
	|| cfgfile_string (option, value, "kickstart_ext_rom_file", p->romextfile, 256)
	|| cfgfile_string (option, value, "kickstart_key_file", p->keyfile, 256))
	return 1;

    if (cfgfile_strval (option, value, "chipset", &tmpval, csmode, 0)) {
	set_chipset_mask (p, tmpval);
	return 1;
    }

    if (cfgfile_strval (option, value, "cpu_type", &tmpval, cpumode, 0)) {
	p->fpu_model = 0;
	p->address_space_24 = 0;
	p->cpu_model = 68000;
	switch (tmpval) {
	case 0:
	    p->cpu_model = 68000;
	    break;
	case 1:
	    p->cpu_model = 68010;
	    break;
	case 2: case 3: case 4:
	case 5: case 6: case 7:
	    p->cpu_model = 68020;
	    p->address_space_24 = tmpval < 5;
	    p->fpu_model = (tmpval == 3 || tmpval == 6 ? 68881
			    : tmpval == 4 || tmpval == 7 ? 68882
			    : 0);
	    break;
	case 8: case 9: case 10:
	    p->cpu_model = 68030;
	    p->fpu_model = (tmpval == 9 ? 68881 : tmpval == 10 ? 68882 : 0);
	    break;
	case 11:
	    p->cpu_model = 68040;
	    p->fpu_model = 68040;
	    break;
	case 12:
	    p->cpu_model = 68060;
	    p->fpu_model = 68060;
	    break;
	}
	printf ("%d %d\n", p->cpu_model, p->fpu_model);
	return 1;
    }

    if (cfgfile_strval (option, value, "cpu_speed", &p->m68k_speed, speedmode, 1)
	/* Broken earlier versions used to write this out as a string.  */
	|| cfgfile_strval (option, value, "finegraincpu_speed", &p->m68k_speed, speedmode, 1))
    {
	p->m68k_speed--;
	return 1;
    }

    if (cfgfile_intval (option, value, "cpu_speed", &p->m68k_speed, 1)) {
	p->m68k_speed *= CYCLE_UNIT;
	return 1;
    }

    if (cfgfile_intval (option, value, "finegrain_cpu_speed", &p->m68k_speed, 1)) {
	if (OFFICIAL_CYCLE_UNIT > CYCLE_UNIT) {
	    int factor = OFFICIAL_CYCLE_UNIT / CYCLE_UNIT;
	    p->m68k_speed = (p->m68k_speed + factor - 1) / factor;
	}
	return 1;
    }

    return 0;
}

int cfgfile_parse_option (struct uae_prefs *p, char *option, char *value)
{
    if (cfgfile_parse_hardware (p, option, value))
	return 1;
    if (cfgfile_parse_host (p, option, value))
	return 1;

    return 0;
}

void cfgfile_parse_line (struct uae_prefs *p, char *line)
{
    int i;
    char *orig_line = my_strdup (line);
    char *line1, *line2;

    line1 = line;
    line2 = strchr (line, '=');
    if (! line2) {
	write_log ("CFGFILE: line was incomplete with only %s\n", line1);
	return;
    }

    *line2++ = '\0';

    /* Get rid of whitespace.  */
    i = strlen (line2);
    while (i > 0 && (line2[i - 1] == '\t' || line2[i - 1] == ' '
		     || line2[i - 1] == '\r' || line2[i - 1] == '\n'))
	line2[--i] = '\0';
    line2 += strspn (line2, "\t \r\n");
    i = strlen (line);
    while (i > 0 && (line[i - 1] == '\t' || line[i - 1] == ' '
		     || line[i - 1] == '\r' || line[i - 1] == '\n'))
	line[--i] = '\0';
    line += strspn (line, "\t \r\n");

    if (! cfgfile_parse_option (p, line, line2)) {
	struct strlist *u = xmalloc (sizeof (struct strlist));
	u->str = orig_line;
	u->next = p->unknown_lines;
	p->unknown_lines = u;
    } else
	free (orig_line);
}

static void subst (char *p, char *f, int n)
{
    char *str = cfgfile_subst_path (UNEXPANDED, p, f);
    strncpy (f, str, n - 1);
    f[n - 1] = '\0';
    free (str);
}

int cfgfile_load (struct uae_prefs *p, const char *filename)
{
    int i;

    FILE *fh;
    char line[256];

    fh = fopen (filename, "r");
    if (! fh)
	return 0;

    while (fgets (line, 256, fh) != 0) {
	int len = strlen (line);
	/* Delete trailing whitespace.  */
	while (len > 0 && strcspn (line + len - 1, "\t \r\n") == 0) {
	    line[--len] = '\0';
	}
	if (strlen (line) > 0)
	    cfgfile_parse_line (p, line);
    }
    fclose (fh);

    for (i = 0; i < 4; i++)
	subst (p->path_floppy, p->df[i], sizeof p->df[i]);
    subst (p->path_rom, p->romfile, sizeof p->romfile);
    subst (p->path_rom, p->romextfile, sizeof p->romextfile);
    subst (p->path_rom, p->keyfile, sizeof p->keyfile);

    return 1;
}

int cfgfile_save (struct uae_prefs *p, const char *filename)
{
    FILE *fh = fopen (filename, "w");
    if (! fh)
	return 0;

    save_options (fh, p);
    fclose (fh);
    return 1;
}

int cfgfile_get_description (const char *filename, char *description)
{
    int result = 0;
    struct uae_prefs p;
    default_prefs (&p);
    strcpy (p.description, "");
    if (cfgfile_load (&p, filename)) {
	result = 1;
	strcpy (description, p.description);
    }
    discard_prefs (&p);
    return result;
}

void cfgfile_show_usage (void)
{
    int i;
    fprintf (stderr, "UAE Configuration Help:\n" \
	       "=======================\n");
    for (i = 0; i < sizeof opttable / sizeof *opttable; i++)
	fprintf (stderr, "%s: %s\n", opttable[i].config_label, opttable[i].config_help);
}

/* This implements the old commandline option parsing.  I've re-added this
   because the new way of doing things is painful for me (it requires me
   to type a couple hundred characters when invoking UAE).  The following
   is far less annoying to use.  */
static void parse_gfx_specs (struct uae_prefs *p, char *spec)
{
    char *x0 = my_strdup (spec);
    char *x1, *x2;

    x1 = strchr (x0, ':');
    if (x1 == 0)
	goto argh;
    x2 = strchr (x1+1, ':');
    if (x2 == 0)
	goto argh;
    *x1++ = 0; *x2++ = 0;

    p->gfx_w.width = atoi (x0);
    p->gfx_w.height = atoi (x1);
    p->gfx_w.lores = strchr (x2, 'l') != 0;
    p->gfx_w.xcenter = strchr (x2, 'x') != 0 ? 1 : strchr (x2, 'X') != 0 ? 2 : 0;
    p->gfx_w.ycenter = strchr (x2, 'y') != 0 ? 1 : strchr (x2, 'Y') != 0 ? 2 : 0;
    p->gfx_w.linedbl = strchr (x2, 'd') != 0;
    p->gfx_w.linedbl += 2 * (strchr (x2, 'D') != 0);
    p->gfx_w.correct_aspect = strchr (x2, 'c') != 0;

    p->gfx_f = p->gfx_w;

    p->gfx_afullscreen = strchr (x2, 'a') != 0;
    p->gfx_pfullscreen = strchr (x2, 'p') != 0;

    if (p->gfx_w.linedbl == 3) {
	fprintf (stderr, "You can't use both 'd' and 'D' modifiers in the display mode specification.\n");
    }

    free (x0);
    return;

    argh:
    fprintf (stderr, "Bad display mode specification.\n");
    fprintf (stderr, "The format to use is: \"width:height:modifiers\"\n");
    fprintf (stderr, "Type \"uae -h\" for detailed help.\n");
    free (x0);
}

static void parse_sound_spec (struct uae_prefs *p, char *spec)
{
    char *x0 = my_strdup (spec);
    char *x1, *x2 = NULL, *x3 = NULL, *x4 = NULL;

    x1 = strchr (x0, ':');
    if (x1 != NULL) {
	*x1++ = '\0';
	x2 = strchr (x1 + 1, ':');
	if (x2 != NULL) {
	    *x2++ = '\0';
	    x3 = strchr (x2 + 1, ':');
	    if (x3 != NULL) {
		*x3++ = '\0';
		x4 = strchr (x3 + 1, ':');
	    }
	}
    }
    p->produce_sound = atoi (x0);
    if (x1) {
	p->sound_mixed_stereo_delay = 0;
	if (*x1 == 'S') {
	    p->sound_stereo = SND_STEREO;
	    p->sound_stereo_separation = 7;
	} else if (*x1 == 's')
	    p->sound_stereo = SND_STEREO;
	else
	    p->sound_stereo = SND_MONO;
    }
    /* x2 used to be bits, now fixed at 16 bit.  */
    if (x3)
	p->sound_freq = atoi (x3);
    if (x4)
	p->sound_maxbsiz = atoi (x4);
    free (x0);
}


static void parse_joy_spec (struct uae_prefs *p, char *spec)
{
    int v0 = 2, v1 = 0;
    if (strlen(spec) != 2)
	goto bad;

    switch (spec[0]) {
     case '0': v0 = JSEM_JOYS; break;
     case '1': v0 = JSEM_JOYS + 1; break;
     case 'M': case 'm': v0 = JSEM_MICE; break;
     case 'A': case 'a': v0 = JSEM_KBDLAYOUT; break;
     case 'B': case 'b': v0 = JSEM_KBDLAYOUT + 1; break;
     case 'C': case 'c': v0 = JSEM_KBDLAYOUT + 2; break;
     default: goto bad;
    }

    switch (spec[1]) {
     case '0': v1 = JSEM_JOYS; break;
     case '1': v1 = JSEM_JOYS + 1; break;
     case 'M': case 'm': v1 = JSEM_MICE; break;
     case 'A': case 'a': v1 = JSEM_KBDLAYOUT; break;
     case 'B': case 'b': v1 = JSEM_KBDLAYOUT + 1; break;
     case 'C': case 'c': v1 = JSEM_KBDLAYOUT + 2; break;
     default: goto bad;
    }
    if (v0 == v1)
	goto bad;
    /* Let's scare Pascal programmers */
    if (0)
bad:
    write_log ("Bad joystick mode specification. Use -J xy, where x and y\n"
	     "can be 0 for joystick 0, 1 for joystick 1, M for mouse, and\n"
	     "a, b or c for different keyboard settings.\n");

    p->jport0 = v0;
    p->jport1 = v1;
}

static void parse_filesys_spec (struct uae_prefs *p, int readonly, char *spec)
{
    char buf[256];
    char *s2;

    strncpy (buf, spec, 255); buf[255] = 0;
    s2 = strchr (buf, ':');
    if (s2) {
	*s2++ = '\0';
#ifdef __DOS__
	{
	    char *tmp;

	    while ((tmp = strchr (s2, '\\')))
		*tmp = '/';
	}
#endif
	s2 = add_filesys_unit (p->mountinfo, buf, s2, readonly, 0, 0, 0, 0);
	if (s2)
	    fprintf (stderr, "%s\n", s2);
    } else {
	fprintf (stderr, "Usage: [-m | -M] VOLNAME:mount_point\n");
    }
}

static void parse_hardfile_spec (struct uae_prefs *p, char *spec)
{
    char *x0 = my_strdup (spec);
    char *x1, *x2, *x3, *x4;

    x1 = strchr (x0, ':');
    if (x1 == NULL)
	goto argh;
    *x1++ = '\0';
    x2 = strchr (x1 + 1, ':');
    if (x2 == NULL)
	goto argh;
    *x2++ = '\0';
    x3 = strchr (x2 + 1, ':');
    if (x3 == NULL)
	goto argh;
    *x3++ = '\0';
    x4 = strchr (x3 + 1, ':');
    if (x4 == NULL)
	goto argh;
    *x4++ = '\0';
    x4 = add_filesys_unit (p->mountinfo, 0, x4, 0, atoi (x0), atoi (x1), atoi (x2), atoi (x3));
    if (x4)
	fprintf (stderr, "%s\n", x4);

    free (x0);
    return;

 argh:
    free (x0);
    fprintf (stderr, "Bad hardfile parameter specified - type \"uae -h\" for help.\n");
    return;
}

static void parse_cpu_specs (struct uae_prefs *p, char *spec)
{
    int lvl;
    if (*spec < '0' || *spec > '4') {
	fprintf (stderr, "CPU parameter string must begin with '0', '1', '2', '3' or '4'.\n");
	return;
    }

    lvl = *spec++ - '0';
    p->fpu_model = 0;
    p->cpu_model = lvl * 10 + 68000;
    p->address_space_24 = p->cpu_model < 68020;
    while (*spec != '\0') {
	switch (*spec) {
	case 'a':
	    if (p->cpu_model < 68020)
		fprintf (stderr, "In 68000/68010 emulation, the address space is always 24 bit.\n");
	    else if (p->cpu_model >= 68040)
		fprintf (stderr, "In 68040 emulation, the address space is always 32 bit.\n");
	    else
		p->address_space_24 = 1;
	    break;
	case 'c':
	    fprintf (stderr, "Obsolete CPU type modifier 'c'.\n");
	    break;
	default:
	    fprintf (stderr, "Bad CPU parameter specified - type \"uae -h\" for help.\n");
	    break;
	}
	spec++;
    }
}

/* Returns the number of args used up (0 or 1).  */
int parse_cmdline_option (struct uae_prefs *p, char c, char *arg)
{
    const char arg_required[] = "0123rKpImWSAJwNCZUFcblOdHRv";

    if (strchr (arg_required, c) && ! arg) {
	fprintf (stderr, "Missing argument for option `-%c'!\n", c);
	return 0;
    }

    switch (c) {
    case 'h': usage (); exit (0);

    case '0': strncpy (p->df[0], arg, 255); p->df[0][255] = 0; break;
    case '1': strncpy (p->df[1], arg, 255); p->df[1][255] = 0; break;
    case '2': strncpy (p->df[2], arg, 255); p->df[2][255] = 0; break;
    case '3': strncpy (p->df[3], arg, 255); p->df[3][255] = 0; break;
    case 'r': strncpy (p->romfile, arg, 255); p->romfile[255] = 0; break;
    case 'K': strncpy (p->keyfile, arg, 255); p->keyfile[255] = 0; break;
    case 'p': strncpy (p->prtname, arg, 255); p->prtname[255] = 0; break;
	/*     case 'I': strncpy (p->sername, arg, 255); p->sername[255] = 0; p->use_serial = 1; break; */
    case 'm': case 'M': parse_filesys_spec (p, c == 'M', arg); break;
    case 'W': parse_hardfile_spec (p, arg); break;
    case 'S': parse_sound_spec (p, arg); break;
    case 'R': p->gfx_framerate = atoi (arg); break;
    case 'i': p->illegal_mem = 1; break;
    case 'J': parse_joy_spec (p, arg); break;

    case 't': p->test_drawing_speed = 1; break;
    case 'L': p->x11_use_low_bandwidth = 1; break;
    case 'T': p->x11_use_mitshm = 1; break;
    case 'w': p->m68k_speed = atoi (arg); break;

    case 'G': p->start_gui = 0; break;
    case 'D': p->start_debugger = 1; break;

    case 'n':
	if (strchr (arg, '3') != 0)
	    p->blits_32bit_enabled = 1;
	if (strchr (arg, 'i') != 0)
	    p->immediate_blits = 1;
	break;

    case 'v':
	set_chipset_mask (p, atoi (arg));
	break;

    case 'C':
	parse_cpu_specs (p, arg);
	break;

    case 'Z':
	p->z3fastmem_size = atoi (arg) * 0x100000;
	break;

    case 'U':
	p->gfxmem_size = atoi (arg) * 0x100000;
	break;

    case 'F':
	p->fastmem_size = atoi (arg) * 0x100000;
	break;

    case 'b':
	p->bogomem_size = atoi (arg) * 0x40000;
	break;

    case 'c':
	p->chipmem_size = atoi (arg) * 0x80000;
	break;

    case 'l':
	if (0 == strcasecmp (arg, "de"))
	    p->keyboard_lang = KBD_LANG_DE;
	else if (0 == strcasecmp (arg, "dk"))
	    p->keyboard_lang = KBD_LANG_DK;
	else if (0 == strcasecmp (arg, "us"))
	    p->keyboard_lang = KBD_LANG_US;
	else if (0 == strcasecmp (arg, "se"))
	    p->keyboard_lang = KBD_LANG_SE;
	else if (0 == strcasecmp (arg, "fr"))
	    p->keyboard_lang = KBD_LANG_FR;
	else if (0 == strcasecmp (arg, "it"))
	    p->keyboard_lang = KBD_LANG_IT;
	else if (0 == strcasecmp (arg, "es"))
	    p->keyboard_lang = KBD_LANG_ES;
	break;

    case 'O': parse_gfx_specs (p, arg); break;
    case 'd':
	if (strchr (arg, 'S') != NULL || strchr (arg, 's')) {
	    write_log ("  Serial on demand.\n");
	    p->serial_demand = 1;
	}
	if (strchr (arg, 'P') != NULL || strchr (arg, 'p')) {
	    write_log ("  Parallel on demand.\n");
	    p->parallel_demand = 1;
	}

	break;

    case 'H':
	p->color_mode = atoi (arg);
	if (p->color_mode < 0) {
	    fprintf (stderr, "Bad color mode selected. Using default.\n");
	    p->color_mode = 0;
	}
	break;
    default:
	fprintf (stderr, "Unknown option `-%c'!\n", c);
	break;
    }
    return !! strchr (arg_required, c);
}

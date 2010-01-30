/*
 * UAE - the Un*x Amiga Emulator
 *
 * Yet Another User Interface for the X11 version
 *
 * Copyright 1997, 1998 Bernd Schmidt
 * Copyright 1998 Michael Krause
 *
 * The Tk GUI doesn't work.
 * The X Forms Library isn't available as source, and there aren't any
 * binaries compiled against glibc
 *
 * So let's try this...
 */

#include "sysconfig.h"
#include "sysdeps.h"

#include "options.h"
#include "uae.h"
#include "memory.h"
#include "custom.h"
#include "audio.h"
#include "gui.h"
#include "newcpu.h"
#include "autoconf.h"
#include "filesys.h"
#include "threaddep/thread.h"
#include "sounddep/sound.h"
#include "savestate.h"
#include "debug.h"
#include "inputdevice.h"
#include "romlist.h"
#include "zfile.h"

#include <gtk/gtk.h>
#include <gdk/gdk.h>

#include "util.h"

static int gui_active, gui_available;

static GtkWidget *gui_window;

static GtkWidget *pause_uae_widget, *snap_save_widget, *snap_load_widget;

static GtkWidget *chipsize_widget[5];
static GtkWidget *bogosize_widget[4];
static GtkWidget *fastsize_widget[5];
static GtkWidget *z3size_widget[10];
static GtkWidget *p96size_widget[7];
static GtkWidget *rom_text_widget, *rom_path_widget, *key_text_widget;
static GtkWidget *romlist_widget, *romsel_button;
static int romlist_nr_entries;

static GtkWidget *disk_insert_widget[4], *disk_eject_widget[4], *disk_text_widget[4];
static GtkWidget *dftype_widget[4];
static GtkWidget *disk_type_widget[4];

static GtkAdjustment *cpuspeed_adj;
static GtkWidget *cpuspeed_widgets[4], *cpuspeed_scale;
static GtkWidget *cpu_widget[7], *fpu_widget[5];
static GtkWidget *sound_widget[4], *sound_ch_widget[3], *sound_interpol_widget[3];
static GtkWidget *sound_filter_widget[5];
static GtkAdjustment *stereo_sep_adj, *stereo_delay_adj;
static GtkWidget *stereo_sep_scale, *stereo_delay_scale;

static GtkWidget *coll_widget[4], *cslevel_widget[4], *ntsc_widget[2];
static GtkWidget *mbtype_widget, *mbtype_labelled;
static GtkWidget *fcop_widget;

static GtkAdjustment *framerate_adj;
static GtkWidget *bimm_widget, *b32_widget, *afscr_widget, *pfscr_widget;

struct scrmode_widgets
{
    GtkWidget *frame, *modelist;
    GtkWidget *hcenter, *vcenter, *aspect, *linedbl, *leds;
    GtkWidget *hc_label, *vc_label, *ld_label;
};

static struct scrmode_widgets gw_windowed, gw_fullscreen;

static GtkWidget *joy_widget[2][6], *legacy_widget, *kbdlang_widget;

static GtkWidget *led_widgets[5];
static GdkColor led_on[5], led_off[5];
static unsigned int prevledstate;

static GtkListStore *hd_store;
static GtkWidget *hdlist_widget, *bootrom_widget;
static GtkWidget *hdchange_button, *hddel_button;
static GtkTreeSelection *hd_selection;

static GtkWidget *preset_list, *preset_button;
static GtkTreeSelection *preset_selection;

static GtkWidget *req68020, *req68ec020, *req68030, *reqa4000, *reqa1000;
static int rom_req;

static GtkWidget *lab_info;
static GtkWidget *notebook;

GtkWidget *disk_selector;

static uae_sem_t gui_sem;

/* Copies of certain changed_prefs elements, local to the GUI thread and
   protected by the gui_sem.  */
static struct gfx_params gfx_w, gfx_f;
static char *gui_snapname, *gui_romname, *gui_keyname;
static char *new_disk_string[4];


static smp_comm_pipe to_gui_pipe, from_gui_pipe;

/* Set to ignore the widget callbacks while we're in the process of updating
   them with new data.  */
static int ignore_gui_changes;

/*
 * Messages sent to GUI from UAE via to_gui_pipe
 */
enum gui_commands {
    GUICMD_SHOW,                 // Show yourself
    GUICMD_UPDATE,               // Refresh your state from changed preferences
    GUICMD_DISKCHANGE,           // Hey! A disk has been changed. Do something!
    GUICMD_MSGBOX,               // Display a message box for me, please
    GUICMD_NEW_ROMLIST,          // The ROM list has been updated.
    GUICMD_FLOPPYDLG,            // Open a floppy insert dialog
    GUICMD_FULLSCREEN,           // Fullscreen mode was toggled; update checkboxes
    GUICMD_PAUSE,                // We're now paused, in case you didn't notice
    GUICMD_UNPAUSE               // We're now running.
};

static uae_sem_t gui_sem, gui_init_sem, gui_quit_sem; /* gui_sem protects the DFx fields */

static volatile int quit_gui = 0;

/* Message box functions.  */


static GtkWidget *make_labelled_button (guchar *label, GtkAccelGroup *accel_group)
{
    return gtk_button_new_with_mnemonic (label);
}

static int find_current_toggle (GtkWidget **widgets, int count)
{
    int i;
    for (i = 0; i < count; i++)
	if (GTK_TOGGLE_BUTTON (*widgets++)->active)
	    return i;
    write_log ("GTKUI: Can't happen!\n");
    return -1;
}

/*
 * make_message_box()
 *
 * This does the actual work of constructing the message dialog.
 *
 * title   - displayed in the dialog's titlebar
 * message - the message itself
 * modal   - whether the dialog should block input to the rest of the GUI
 * sem     - semaphore used for signalling that the dialog's finished
 *
 * TODO: Make that semaphore go away. We shouldn't need to know about it here.
 */

static void make_message_box (const char *title, const char *message, int modal)
{
    GtkWidget *dialog;
    GtkWidget *vbox, *hbox;
    GtkWidget *label;

    if (title == NULL || strlen (title) == 0)
	title = PACKAGE_NAME " information";
    dialog = gtk_dialog_new_with_buttons (title, NULL,
					  (modal ? GTK_DIALOG_MODAL : 0),
					  GTK_STOCK_OK, GTK_RESPONSE_ACCEPT,
					  NULL);

    gtk_window_set_position (GTK_WINDOW (dialog), GTK_WIN_POS_MOUSE);

    vbox = GTK_DIALOG (dialog)->vbox;

    hbox = gtk_hbox_new (FALSE, 0);
    gtk_box_pack_start (GTK_BOX (vbox), hbox, TRUE, TRUE, 20);
    gtk_widget_show (hbox);

    label = gtk_label_new (message);
    gtk_widget_show (label);
    gtk_box_pack_start (GTK_BOX (hbox), label, TRUE, TRUE, 20);
    gtk_label_set_line_wrap (GTK_LABEL (label), TRUE);

    gtk_dialog_run (GTK_DIALOG (dialog));

    gtk_widget_destroy (dialog);
}

static void mkmsgbox (const char *title, const char *message)
{
    make_message_box (title, message, 1);
}

/*
 * handle_message_box_request()
 *
 * This is called from the GUI's context in repsonse to do_message_box()
 * to actually create the dialog box
 */
static void handle_message_box_request (smp_comm_pipe *msg_pipe)
{
    const char *title = (const char *) read_comm_pipe_pvoid_blocking (msg_pipe);
    const char *msg = (const char *) read_comm_pipe_pvoid_blocking (msg_pipe);
    int modal = read_comm_pipe_int_blocking (msg_pipe);
    uae_sem_t *msg_quit_sem = (uae_sem_t *) read_comm_pipe_pvoid_blocking (msg_pipe);

    make_message_box (title, msg, modal);
    uae_sem_post (msg_quit_sem);
}

/*
 * do_message_box()
 *
 * This makes up for GTK's lack of a function for creating simple message dialogs.
 * It can be called from any context. gui_init() must have been called at some point
 * previously.
 *
 * title   - will be displayed in the dialog's titlebar (or NULL for default)
 * message - the message itself
 * modal   - should the dialog block input to the rest of the GUI
 * wait    - should the dialog wait until the user has acknowledged it
 */
static void do_message_box (const guchar *title, const guchar *message, gboolean modal, gboolean wait)
{
    uae_sem_t msg_quit_sem;

    // If we a need reply, then this semaphore which will be used
    // to signal us when the dialog has been exited.
    uae_sem_init (&msg_quit_sem, 0, 0);

    write_comm_pipe_int   (&to_gui_pipe, GUICMD_MSGBOX, 0);
    write_comm_pipe_pvoid (&to_gui_pipe, (void *) title, 0);
    write_comm_pipe_pvoid (&to_gui_pipe, (void *) message, 0);
    write_comm_pipe_int   (&to_gui_pipe, (int) modal, 0);
    write_comm_pipe_pvoid (&to_gui_pipe, wait?&msg_quit_sem:NULL, 1);

    if (wait)
	uae_sem_wait (&msg_quit_sem);
}

void gui_message (const char *format, ...)
{
    char msg[2048];
    va_list parms;

    va_start (parms,format);
    vsprintf ( msg, format, parms);
    va_end (parms);

    if (gui_available)
	do_message_box (NULL, msg, TRUE, TRUE);

    write_log (msg);
}

/* Config save/load.  */

static void save_config (void)
{
    FILE *f;
    char tmp[257];

    /* Backup the options file.  */
    strcpy (tmp, optionsfile);
    strcat (tmp, "~");
    rename (optionsfile, tmp);

    f = fopen (optionsfile, "w");
    if (f == NULL) {
	write_log ("Error saving options file!\n");
	return;
    }
    save_options (f, &changed_prefs);
    fclose (f);
}

static int nr_for_led (GtkWidget *led)
{
    int i;
    i = 0;
    while (led_widgets[i] != led)
	i++;
    return i;
}

static void enable_disk_buttons (int enable)
{
    int i;
    for (i = 0; i < 4; i++) {
	drive_type t = changed_prefs.dfxtype[i];
	gtk_widget_set_sensitive (disk_insert_widget[i], enable && t != DRV_NONE);
	gtk_widget_set_sensitive (disk_eject_widget[i], enable && t != DRV_NONE);
	gtk_widget_set_sensitive (dftype_widget[i], enable);
    }
}

static void set_disk_state (void)
{
    int i;
    ignore_gui_changes++;
    for (i = 0; i < 4; i++) {
	drive_type t = changed_prefs.dfxtype[i];
	int n;
	if (i == 0)
	    n = t == DRV_35_DD ? 0 : 1;
	else
	    n = t == DRV_35_DD ? 1 : t == DRV_35_HD ? 2 : 0;
	gtk_combo_box_set_active (GTK_COMBO_BOX (dftype_widget[i]), n);
    }
    ignore_gui_changes--;
}

static void set_romlist_state (void)
{
    int i;
    int active_entry = 0;

    for (i = 1; i < romlist_nr_entries; i++)
	gtk_combo_box_remove_text (GTK_COMBO_BOX (romlist_widget), 1);
    romlist_nr_entries = 1;

    for (i = 0;; i++) {
	struct romlist *rl = romlist_from_idx (i, ROMTYPE_KICK, 1);
	if (!rl)
	    break;
	if ((changed_prefs.rom_crc32 != 0 && rl->rd->crc32 == changed_prefs.rom_crc32)
	    || (changed_prefs.rom_crc32 == 0 && strcmp (rl->path, changed_prefs.romfile) == 0))
	    active_entry = romlist_nr_entries;
	gtk_combo_box_append_text (GTK_COMBO_BOX (romlist_widget), rl->rd->name);
	romlist_nr_entries++;
    }
    gtk_combo_box_set_active (GTK_COMBO_BOX (romlist_widget), active_entry);
    gtk_widget_set_sensitive (romsel_button, active_entry == 0);
    gtk_widget_set_sensitive (rom_text_widget, active_entry == 0);
}

static void set_cpu_state (void)
{
    gtk_widget_set_sensitive (cpuspeed_scale, changed_prefs.m68k_speed > 0);

    gtk_widget_set_sensitive (fpu_widget[0], changed_prefs.cpu_model != 68040 && changed_prefs.cpu_model != 68060);
    gtk_widget_set_sensitive (fpu_widget[1], changed_prefs.cpu_model == 68020 || changed_prefs.cpu_model == 68030);
    gtk_widget_set_sensitive (fpu_widget[2], changed_prefs.cpu_model == 68020 || changed_prefs.cpu_model == 68030);
    gtk_widget_set_sensitive (fpu_widget[3], changed_prefs.cpu_model == 68040);
    gtk_widget_set_sensitive (fpu_widget[4], changed_prefs.cpu_model == 68060);
}

static void set_cpu_widget (void)
{
    int nr = (changed_prefs.cpu_model == 68000 ? 0
	      : changed_prefs.cpu_model == 68010 ? 1
	      : changed_prefs.cpu_model == 68020 ? 2
	      : changed_prefs.cpu_model == 68030 ? 4
	      : changed_prefs.cpu_model == 68040 ? 5
	      : 6);

    if (nr == 2 && !changed_prefs.address_space_24)
	nr++;
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (cpu_widget[nr]), TRUE);

    nr = (changed_prefs.fpu_model == 68881 ? 1
	  : changed_prefs.fpu_model == 68882 ? 2
	  : changed_prefs.cpu_model == 68040 ? 3
	  : changed_prefs.cpu_model == 68060 ? 4
	  : 0);
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (fpu_widget[nr]), TRUE);
    nr = changed_prefs.m68k_speed + 1 < 3 ? changed_prefs.m68k_speed + 1 : 2;
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (cpuspeed_widgets[nr]), TRUE);
}

/* The idea behind this is to keep entries in aspect/centering boxes even
   if a screen mode is selected where they are unnecessary, but set the
   prefs to sane values.  */
static void adjust_gfx_prefs (struct gfx_params *gp, struct scrmode_widgets *w)
{
    int need_hcenter, need_vcenter, need_aspect, need_linedbl;
    need_hcenter = gp->width < 400 || (gp->width > 512 && gp->width < 800);
    need_vcenter = gp->height <= 256 || (gp->height > 300 && gp->height <= 512);
    need_linedbl = gp->height >= 400;
    need_aspect = ((gp->height < 256 || (gp->height > 300 && gp->height < 512))
		   && gp->height * 5 / 4 < gp->width);

    gtk_widget_set_sensitive (w->vc_label, need_vcenter);
    gtk_widget_set_sensitive (w->hc_label, need_hcenter);
    gtk_widget_set_sensitive (w->aspect, need_aspect);
    gtk_widget_set_sensitive (w->ld_label, need_linedbl);

    if (!need_linedbl)
	gp->linedbl = 0;
    if (!need_aspect)
	gp->correct_aspect = FALSE;
    if (!need_vcenter)
	gp->ycenter = 0;
    if (!need_hcenter)
	gp->xcenter = 0;
}

static void set_gfx_mode_state (struct gfx_params *gp, struct scrmode_widgets *w)
{
    ignore_gui_changes++;

    gtk_combo_box_set_active (GTK_COMBO_BOX (w->linedbl), gp->linedbl ? gp->linedbl - 1 : 1);
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (w->aspect), gp->correct_aspect);
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (w->leds), gp->leds_on_screen);

    gtk_combo_box_set_active (GTK_COMBO_BOX (w->hcenter), gp->xcenter);
    gtk_combo_box_set_active (GTK_COMBO_BOX (w->vcenter), gp->ycenter);
    ignore_gui_changes--;
}

static void set_fullscreen_state (void)
{
    ignore_gui_changes++;
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (afscr_widget), changed_prefs.gfx_afullscreen != 0);
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (pfscr_widget), changed_prefs.gfx_pfullscreen != 0);
    ignore_gui_changes--;
}

static void set_gfx_state (void)
{
    int t;

    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (bimm_widget), changed_prefs.immediate_blits != 0);

    ignore_gui_changes++;
    uae_sem_wait (&gui_sem);
    t = fixup_prefs_dimensions (&gfx_w, gfx_windowed_modes, n_windowed_modes);
    gtk_combo_box_set_active (GTK_COMBO_BOX (gw_windowed.modelist), t);
    t = fixup_prefs_dimensions (&gfx_f, gfx_fullscreen_modes, n_fullscreen_modes);
    gtk_combo_box_set_active (GTK_COMBO_BOX (gw_fullscreen.modelist), t);
    ignore_gui_changes--;

    adjust_gfx_prefs (&gfx_w, &gw_windowed);
    adjust_gfx_prefs (&gfx_f, &gw_fullscreen);
    uae_sem_post (&gui_sem);

    set_gfx_mode_state (&gfx_w, &gw_windowed);
    set_gfx_mode_state (&gfx_f, &gw_fullscreen);

    set_fullscreen_state ();
#if 0
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (b32_widget), changed_prefs.blits_32bit_enabled != 0);
#endif
}

/* The intended behaviour here is that on startup, and when loading a state
   file, we examine the prefs to see if they match a given motherboard, and
   initialize the combo box accordingly.  When the user changes it, default
   settings are automatically chosen unless the user sets "Generic", in which
   case he has control over all the options.  */
static void discover_cstype (void)
{
    int t0;
    int cstype = cstype_from_prefs (&changed_prefs);

    t0 = (cstype == CP_GENERIC ? 0
	  : cstype == CP_A1000 ? 1
	  : cstype == CP_A500 || cstype == CP_A500P ? 2
	  : cstype == CP_A600 ? 3
	  : cstype == CP_A1200 ? 4
	  : cstype == CP_A3000 ? 5
	  : 6);

    ignore_gui_changes++;
    gtk_combo_box_set_active (GTK_COMBO_BOX (mbtype_widget), t0);
    ignore_gui_changes--;
}

static void set_chipset_state (void)
{
    int t0 = 0;

    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (coll_widget[changed_prefs.collision_level]), TRUE);
    if (changed_prefs.chipset_mask & CSMASK_AGA)
	t0 = 3;
    else if (changed_prefs.chipset_mask & CSMASK_ECS_DENISE)
	t0 = 2;
    else if (changed_prefs.chipset_mask & CSMASK_ECS_AGNUS)
	t0 = 1;
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (cslevel_widget[t0]), TRUE);
}

static void set_sound_state (void)
{
    int n;

    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (sound_widget[changed_prefs.produce_sound]), 1);
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (sound_interpol_widget[changed_prefs.sound_interpol]), 1);
    n = (changed_prefs.sound_filter == FILTER_SOUND_OFF ? 0
	 : ((changed_prefs.sound_filter == FILTER_SOUND_EMUL ? 1 : 2)
	    + (changed_prefs.sound_filter_type == FILTER_SOUND_TYPE_A500 ? 0 : 2)));
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (sound_filter_widget[n]), 1);
}

static void set_mem_state (void)
{
    int bogo2m_ok;
    int t, t2, i;

    for (i = 0; i < 10; i++)
	gtk_widget_set_sensitive (z3size_widget[i], ! changed_prefs.address_space_24);
    for (i = 0; i < 7; i++)
	gtk_widget_set_sensitive (p96size_widget[i],
				  changed_prefs.bootrom && ! changed_prefs.address_space_24);

    bogo2m_ok = !((changed_prefs.chipset_mask & CSMASK_AGA) || changed_prefs.cpu_model >= 68020);
    gtk_widget_set_sensitive (bogosize_widget[3], bogo2m_ok);
			      
    t = 0;
    t2 = changed_prefs.chipmem_size;
    while (t < 4 && t2 > 0x80000)
	t++, t2 >>= 1;
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (chipsize_widget[t]), 1);

    t = 0;
    t2 = changed_prefs.bogomem_size;
    if (t2 > 0x100000 && !bogo2m_ok)
	t2 = 0x100000;
    changed_prefs.bogomem_size = t2;
    while (t < 3 && t2 >= 0x80000)
	t++, t2 >>= 1;
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (bogosize_widget[t]), 1);

    t = 0;
    t2 = changed_prefs.fastmem_size;
    while (t < 4 && t2 >= 0x100000)
	t++, t2 >>= 1;
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (fastsize_widget[t]), 1);

    t = 0;
    t2 = changed_prefs.z3fastmem_size;
    while (t < 9 && t2 >= 0x100000)
	t++, t2 >>= 1;
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (z3size_widget[t]), 1);

    t = 0;
    t2 = changed_prefs.gfxmem_size;
    while (t < 6 && t2 >= 0x100000)
	t++, t2 >>= 1;
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (p96size_widget[t]), 1);

    gtk_label_set_text (GTK_LABEL (rom_path_widget), changed_prefs.path_rom);
    gtk_label_set_text (GTK_LABEL (rom_text_widget), changed_prefs.romfile);
    gtk_label_set_text (GTK_LABEL (key_text_widget), changed_prefs.keyfile);
}

static void set_joy_state (void)
{
    int joy_count = inputdevice_get_device_total (IDTYPE_JOYSTICK);
    int j0j = jsem_isjoy (0, &changed_prefs);
    int j0m = jsem_ismouse (0, &changed_prefs);
    int j0k = jsem_iskbdjoy (0, &changed_prefs);
    int j1j = jsem_isjoy (1, &changed_prefs);
    int j1m = jsem_ismouse (1, &changed_prefs);
    int j1k = jsem_iskbdjoy (1, &changed_prefs);
    int j0t = (j0j == 0 || j0j == 1 ? j0j
	       : j0m == 0 ? 2
	       : j0k + 3);
    int j1t = (j1j == 0 || j1j == 1 ? j1j
	       : j1m == 0 ? 2
	       : j1k + 3);
    int i;

    if (j0t == j1t) {
	/* Can't happen */
	j0t++;
	j0t %= 6;
    }

    for (i = 0; i < 6; i++) {
	int available = i >= 2 || joy_count > i;
	if (changed_prefs.input_selected_setting != 0)
	    available = 0;
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (joy_widget[0][i]), j0t == i);
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (joy_widget[1][i]), j1t == i);
	gtk_widget_set_sensitive (joy_widget[0][i], available && j1t != i);
	gtk_widget_set_sensitive (joy_widget[1][i], available && j0t != i);
    }

    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (legacy_widget),
				  changed_prefs.input_selected_setting == 0);

    gtk_combo_box_set_active (GTK_COMBO_BOX (kbdlang_widget),
			      changed_prefs.keyboard_lang);
}

static void set_hd_state (void)
{
    char texts[9][256];
    int nr = nr_units (currprefs.mountinfo);
    int i;

    gtk_list_store_clear (hd_store);
    for (i = 0; i < nr; i++) {
	GtkTreeIter iter;
	int secspertrack, surfaces, reserved, blocksize, size;
	int cylinders, readonly, pri;
	char *volname, *rootdir, *devname;
	char *failure;

	/* We always use currprefs.mountinfo for the GUI.  The filesystem
	   code makes a private copy which is updated every reset.  */
	failure = get_filesys_unit (currprefs.mountinfo, i,
				    &devname, &volname, &rootdir, &readonly,
				    &secspertrack, &surfaces, &reserved,
				    &cylinders, &size, &blocksize, &pri);

	if (is_hardfile (currprefs.mountinfo, i)) {
	    sprintf (texts[0], "DH%d", i );
	    sprintf (texts[3], "%d", surfaces);
	    sprintf (texts[4], "%d", cylinders);
	    sprintf (texts[5], "%d", secspertrack);
	    sprintf (texts[6], "%d", reserved);
	    sprintf (texts[7], "%d", size);
	    sprintf (texts[8], "%d", blocksize);
	} else {
	    strcpy (texts[0], volname);
	    strcpy (texts[3], "n/a");
	    strcpy (texts[4], "n/a");
	    strcpy (texts[5], "n/a");
	    strcpy (texts[6], "n/a");
	    strcpy (texts[7], "n/a");
	    strcpy (texts[8], "n/a");
	}
	strcpy (texts[1], rootdir);
	gtk_list_store_append (hd_store, &iter);

	gtk_list_store_set (hd_store, &iter,
			    0, i,
			    1, texts[0],
			    2, texts[1],
			    3, readonly,
			    4, texts[3],
			    5, texts[4],
			    6, texts[5],
			    7, texts[6],
			    8, texts[7],
			    9, texts[8],
			    10, pri,
			    -1);
	if (volname)
	    free (volname);
	if (devname)
	    free (devname);
	if (rootdir)
	    free (rootdir);
    }
    gtk_widget_set_sensitive (hdchange_button, FALSE);
    gtk_widget_set_sensitive (hddel_button, FALSE);

    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (bootrom_widget),
				  changed_prefs.bootrom != 0);
}

static void rom_requirements_change (void)
{
    int i, min_widget = 0;

    gtk_widget_hide (req68020);
    gtk_widget_hide (req68ec020);
    gtk_widget_hide (req68030);
    gtk_widget_hide (reqa1000);
    gtk_widget_hide (reqa4000);
    if ((rom_req & ROMREQ_CPUMASK) == ROMREQ_68EC020) {
	min_widget = 2;
	gtk_widget_show (req68ec020);
    } else if ((rom_req & ROMREQ_CPUMASK) == ROMREQ_68020) {
	min_widget = 3;
	gtk_widget_show (req68020);
    } else if ((rom_req & ROMREQ_CPUMASK) == ROMREQ_68030) {
	min_widget = 4;
	gtk_widget_show (req68030);
    }
    if (rom_req & ROMREQ_A4000MB) {
	gtk_widget_show (reqa4000);
	gtk_combo_box_set_active (GTK_COMBO_BOX (mbtype_widget), 6);
	gtk_widget_set_sensitive (mbtype_labelled, 0);
    } else if (rom_req & ROMREQ_A1000) {
	gtk_widget_show (reqa1000);
	gtk_combo_box_set_active (GTK_COMBO_BOX (mbtype_widget), 1);
	gtk_widget_set_sensitive (mbtype_labelled, 0);
    } else
	gtk_widget_set_sensitive (mbtype_labelled, 1);

    i = find_current_toggle (cpu_widget, 7);
    if (i < min_widget)
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (cpu_widget[min_widget]), TRUE);

    for (i = 0; i < 7; i++)
	gtk_widget_set_sensitive (cpu_widget[i], i >= min_widget);
}

static void draw_led (int nr)
{
    GtkWidget *thing = led_widgets[nr];
    GdkWindow *window = thing->window;
    GdkGC *gc = gdk_gc_new (window);
    GdkColor *col;

    if (gui_ledstate & (1 << nr))
	col = led_on + nr;
    else
	col = led_off + nr;
    gdk_gc_set_foreground (gc, col);
    gdk_draw_rectangle (window, gc, 1, 0, 0, -1, -1);
    gdk_gc_destroy (gc);
}

static void set_widgets_from_config (void)
{
    gfx_w = changed_prefs.gfx_w;
    gfx_f = changed_prefs.gfx_f;
    set_disk_state ();
    enable_disk_buttons (1);
    set_romlist_state ();
    set_cpu_widget ();
    set_cpu_state ();
    set_gfx_state ();
    set_joy_state ();
    set_sound_state ();
    set_mem_state ();
    set_hd_state ();
    discover_cstype ();
    set_chipset_state ();
    rom_requirements_change ();
}

static int my_idle (void)
{
    unsigned int leds = gui_ledstate;
    int i;

    if (quit_gui) {	
	gui_active = 0;
	gtk_main_quit ();
	goto out;
    }

    while (comm_pipe_has_data (&to_gui_pipe)) {
	int cmd = read_comm_pipe_int_blocking (&to_gui_pipe);
	int n;
	switch (cmd) {
	case GUICMD_DISKCHANGE:
	    n = read_comm_pipe_int_blocking (&to_gui_pipe);
	    gtk_label_set_text (GTK_LABEL (disk_text_widget[n]), currprefs.df[n]);
	    break;
	case GUICMD_SHOW:
	    gtk_widget_show (gui_window);
	    uae_sem_post (&gui_init_sem);
	    break;
	case GUICMD_UPDATE:
	    /* Initialization.  */
	    set_widgets_from_config ();

	    gtk_widget_show (gui_window);
	    uae_sem_post (&gui_init_sem);
	    gui_active = 1;
	    break;
	case GUICMD_PAUSE:
	    /* Set Pause-Button active */
	    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (pause_uae_widget), TRUE);
	    break;
	case GUICMD_UNPAUSE:
	    /* Set Pause-Button inactive */
	    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (pause_uae_widget), FALSE);
	    break;
	case GUICMD_NEW_ROMLIST:
	    set_romlist_state ();
	    break;
	case GUICMD_FULLSCREEN:
	    set_fullscreen_state ();
	    break;
	case GUICMD_MSGBOX:
	    handle_message_box_request(&to_gui_pipe);
	    break;
	}
    }

    if (gui_active) {
	int n = gtk_notebook_get_current_page (GTK_NOTEBOOK (notebook)) == 0 ? 5 : 1;
	for (i = 0; i < n; i++) {
	    unsigned int mask = 1 << i;
	    unsigned int on = leds & mask;

	    if (on == (prevledstate & mask))
		continue;

	    /*	printf(": %d %d\n", i, on);*/
	    draw_led (i);
	}
	prevledstate = leds;
    }
  out:
    return 1;
}

static gint gui_delete_event (GtkWidget *widget, GdkEvent *event, gpointer data)
{
    gui_active = 0;
    if (disk_selector)
	gtk_widget_destroy (disk_selector);
    disk_selector = 0;
    gtk_widget_hide (gui_window);
    return TRUE;
}

static void dftype_changed (GtkWidget *w, gpointer data)
{
    int i;

    if (ignore_gui_changes)
	return;
    for (i = 0; i < 4; i++) {
	int which = gtk_combo_box_get_active (GTK_COMBO_BOX (dftype_widget[i]));
	if (i == 0)
	    which++;
	changed_prefs.dfxtype[i] = (which == 0 ? DRV_NONE : which == 1 ? DRV_35_DD : DRV_35_HD);
    }
    enable_disk_buttons (1);
}

static void kbdlang_changed (GtkWidget *w, gpointer data)
{
    int which = gtk_combo_box_get_active (GTK_COMBO_BOX (kbdlang_widget));
    changed_prefs.keyboard_lang = which;
}

static void joy_changed (void)
{
    int j0t, j1t;
    if (! gui_active)
	return;

    j0t = find_current_toggle (joy_widget[0], 6);
    j1t = find_current_toggle (joy_widget[1], 6);
    changed_prefs.jport0 = (j0t == 0 || j0t == 1 ? JSEM_JOYS + j0t
			    : j0t == 2 ? JSEM_MICE
			    : JSEM_KBDLAYOUT + j0t - 3);
    changed_prefs.jport1 = (j1t == 0 || j1t == 1 ? JSEM_JOYS + j1t
			    : j1t == 2 ? JSEM_MICE
			    : JSEM_KBDLAYOUT + j1t - 3);

    changed_prefs.input_selected_setting = GTK_TOGGLE_BUTTON (legacy_widget)->active ? 0 : 1;

    if (changed_prefs.input_selected_setting != currprefs.input_selected_setting
	|| changed_prefs.jport0 != currprefs.jport0
	|| changed_prefs.jport1 != currprefs.jport1)
	inputdevice_config_change ();

    set_joy_state ();
}

static void coll_changed (void)
{
    changed_prefs.collision_level = find_current_toggle (coll_widget, 4);
}

static void cslevel_changed (void)
{
    int t = find_current_toggle (cslevel_widget, 4);
    int t1 = 0;
    if (t > 0)
	t1 |= CSMASK_ECS_AGNUS;
    if (t > 1)
	t1 |= CSMASK_ECS_DENISE;
    if (t > 2)
	t1 |= CSMASK_AGA;
    changed_prefs.chipset_mask = t1;
    set_mem_state ();
}

static void custom_changed (void)
{
    if (ignore_gui_changes)
	return;
    changed_prefs.gfx_framerate = framerate_adj->value;
    changed_prefs.immediate_blits = GTK_TOGGLE_BUTTON (bimm_widget)->active;
    changed_prefs.gfx_afullscreen = GTK_TOGGLE_BUTTON (afscr_widget)->active;
    changed_prefs.gfx_pfullscreen = GTK_TOGGLE_BUTTON (pfscr_widget)->active;
#if 0
    changed_prefs.blits_32bit_enabled = GTK_TOGGLE_BUTTON (b32_widget)->active;
#endif
}

static void leds_changed (void)
{
    int w_leds = GTK_TOGGLE_BUTTON (gw_windowed.leds)->active;
    int f_leds = GTK_TOGGLE_BUTTON (gw_fullscreen.leds)->active;
    gfx_w.leds_on_screen = w_leds;
    gfx_f.leds_on_screen = f_leds;
    write_comm_pipe_int (&from_gui_pipe, 11, 1);
}

static void aspect_changed (void)
{
    int w_aspect = GTK_TOGGLE_BUTTON (gw_windowed.aspect)->active;
    int f_aspect = GTK_TOGGLE_BUTTON (gw_fullscreen.aspect)->active;
    int w_ldbl = gtk_combo_box_get_active (GTK_COMBO_BOX (gw_windowed.linedbl)) + 1;
    int f_ldbl = gtk_combo_box_get_active (GTK_COMBO_BOX (gw_fullscreen.linedbl)) + 1;
    int w_hc = gtk_combo_box_get_active (GTK_COMBO_BOX (gw_windowed.hcenter));
    int f_hc = gtk_combo_box_get_active (GTK_COMBO_BOX (gw_fullscreen.hcenter));
    int w_vc = gtk_combo_box_get_active (GTK_COMBO_BOX (gw_windowed.vcenter));
    int f_vc = gtk_combo_box_get_active (GTK_COMBO_BOX (gw_fullscreen.vcenter));

    if (ignore_gui_changes)
	return;

    uae_sem_wait (&gui_sem);
    gfx_w.ycenter = w_vc;
    gfx_w.xcenter = w_hc;
    gfx_w.correct_aspect = w_aspect;
    gfx_w.linedbl = w_ldbl;
    gfx_f.ycenter = f_vc;
    gfx_f.xcenter = f_hc;
    gfx_f.correct_aspect = f_aspect;
    gfx_f.linedbl = f_ldbl;

    adjust_gfx_prefs (&gfx_w, &gw_windowed);
    adjust_gfx_prefs (&gfx_f, &gw_fullscreen);
    uae_sem_post (&gui_sem);
    write_comm_pipe_int (&from_gui_pipe, 11, 1);
}

static void screenmode_changed (void)
{
    int w = gtk_combo_box_get_active (GTK_COMBO_BOX (gw_windowed.modelist));
    int f = gtk_combo_box_get_active (GTK_COMBO_BOX (gw_fullscreen.modelist));
    int need_set_w = 0, need_set_f = 0;

    if (ignore_gui_changes)
	return;

    uae_sem_wait (&gui_sem);
    if (w != -1) {
	need_set_w = (gfx_w.width != gfx_windowed_modes[w].w
		      || gfx_w.height != gfx_windowed_modes[w].h);
	gfx_w.width = gfx_windowed_modes[w].w;
	gfx_w.height = gfx_windowed_modes[w].h;
    }
    if (f != -1) {
	need_set_f = (gfx_f.width != gfx_fullscreen_modes[f].w
		      || gfx_f.height != gfx_fullscreen_modes[f].h);
	gfx_f.width = gfx_fullscreen_modes[f].w;
	gfx_f.height = gfx_fullscreen_modes[f].h;
    }
    if (need_set_w) {
	fixup_prefs_dimensions (&gfx_w, gfx_windowed_modes, n_windowed_modes);
    }
    if (need_set_f) {
	fixup_prefs_dimensions (&gfx_f, gfx_fullscreen_modes, n_fullscreen_modes);
    }
    uae_sem_post (&gui_sem);

    if (!need_set_w && !need_set_f)
	return;

    /* We may have switched to a screenmode that has centering or other
       modifiers enabled; re-set the members of the prefs structure
       according to the new mode.  See comment before adjust_gfx_prefs.

       aspect_changed also sends the message to the main thread.  */
    aspect_changed ();
}

static void cpuspeed_changed (void)
{
    int which = find_current_toggle (cpuspeed_widgets, 3);
    changed_prefs.m68k_speed = (which == 0 ? -1
				: which == 1 ? 0
				: cpuspeed_adj->value);
    set_cpu_state ();
}

static void cputype_changed (void)
{
    int i, oldcl;
    int whichtoggle;
    if (! gui_active)
	return;

    whichtoggle = find_current_toggle (cpu_widget, 7);
    changed_prefs.address_space_24 = whichtoggle < 3;
    if (whichtoggle >= 3)
	whichtoggle--;

    if (whichtoggle == 5)
	whichtoggle = 6;
    changed_prefs.cpu_model = 68000 + 10 * whichtoggle;

    if (changed_prefs.cpu_model == 68060) {
	changed_prefs.fpu_model = 68060;
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (fpu_widget[4]), 1);
    } else if (changed_prefs.cpu_model == 68040) {
	changed_prefs.fpu_model = 68040;
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (fpu_widget[3]), 1);
    } else if ((changed_prefs.cpu_model != 68020 && changed_prefs.cpu_model != 68030)
	       || changed_prefs.fpu_model == 68040
	       || changed_prefs.fpu_model == 68060) {
	changed_prefs.fpu_model = 0;
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (fpu_widget[0]), 1);
    } else {
	whichtoggle = find_current_toggle (fpu_widget, 5);
	changed_prefs.fpu_model = (whichtoggle == 0 ? 0
				   : whichtoggle == 1 ? 68881
				   : whichtoggle == 2 ? 68882
				   : whichtoggle == 3 ? 68040
				   : 68060);
    }

    set_cpu_state ();
    /* May influence bogomem size and Z3/GFXcard memory.  */
    set_mem_state();
}

static void chipsize_changed (void)
{
    int t = find_current_toggle (chipsize_widget, 5);
    changed_prefs.chipmem_size = 0x80000 << t;
    for (t = 0; t < 5; t++)
	gtk_widget_set_sensitive (fastsize_widget[t], changed_prefs.chipmem_size <= 0x200000);
    if (changed_prefs.chipmem_size > 0x200000) {
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (fastsize_widget[0]), 1);
	changed_prefs.fastmem_size = 0;
    }
}

static void bogosize_changed (void)
{
    int t = find_current_toggle (bogosize_widget, 4);
    changed_prefs.bogomem_size = (0x40000 << t) & ~0x40000;
}

static void fastsize_changed (void)
{
    int t = find_current_toggle (fastsize_widget, 5);
    changed_prefs.fastmem_size = (0x80000 << t) & ~0x80000;
}

static void z3size_changed (void)
{
    int t = find_current_toggle (z3size_widget, 10);
    changed_prefs.z3fastmem_size = (0x80000 << t) & ~0x80000;
}

static void p96size_changed (void)
{
    int t = find_current_toggle (p96size_widget, 7);
    changed_prefs.gfxmem_size = (0x80000 << t) & ~0x80000;
}

static void rom_combo_changed (void)
{
    int t = gtk_combo_box_get_active (GTK_COMBO_BOX (romlist_widget));
    gtk_widget_set_sensitive (romsel_button, t == 0);
    gtk_widget_set_sensitive (rom_text_widget, t == 0);

    rom_req = 0;

    if (t == 0) {
	changed_prefs.rom_crc32 = 0;
    } else {
	struct romlist *rl = romlist_from_idx (t - 1, ROMTYPE_KICK, 1);
	if (rl) {
	    changed_prefs.rom_crc32 = rl->rd->crc32;
	    rom_req = rl->rd->cpu;
	}
    }
    rom_requirements_change ();
}

static void sound_changed (void)
{
    int n;

    changed_prefs.produce_sound = find_current_toggle (sound_widget, 4);
    changed_prefs.sound_interpol = find_current_toggle (sound_interpol_widget, 3);
    n = find_current_toggle (sound_filter_widget, 5);
    if (n == 0)
	changed_prefs.sound_filter = FILTER_SOUND_OFF;
    else {
	changed_prefs.sound_filter = (n & 1) ? FILTER_SOUND_EMUL : FILTER_SOUND_ON;
	changed_prefs.sound_filter_type = (n > 2 ? FILTER_SOUND_TYPE_A1200 : FILTER_SOUND_TYPE_A500);
    }
    changed_prefs.sound_mixed_stereo_delay = stereo_delay_adj->value;
    changed_prefs.sound_stereo_separation = stereo_sep_adj->value;
}

static void bootrom_changed (void)
{
    changed_prefs.bootrom = GTK_TOGGLE_BUTTON (bootrom_widget)->active ? 1 : 0;
    set_mem_state ();
}

static void ntsc_changed (void)
{
    changed_prefs.ntscmode = find_current_toggle (ntsc_widget, 2);
}

static void mbtype_changed (void)
{
    int needs_aga, allows_ocs, allows_ecs;
    int n, cstype;

    if (ignore_gui_changes)
	return;

    n = gtk_combo_box_get_active (GTK_COMBO_BOX (mbtype_widget));
    cstype = (n == 0 ? CP_GENERIC
	      : n == 1 ? CP_A1000
	      : n == 2 ? CP_A500
	      : n == 3 ? CP_A600
	      : n == 4 ? CP_A1200
	      : n == 5 ? CP_A3000
	      : CP_A4000);
    if (cstype != CP_GENERIC)
	built_in_chipset_prefs (&changed_prefs, cstype);

    needs_aga = cstype == CP_A1200 || cstype == CP_A4000 || cstype == CP_A4000T || cstype == CP_CD32;
    allows_ocs = cstype == CP_A1000 || cstype == CP_A500 || cstype == CP_A2000;
    allows_ecs = !needs_aga && cstype != CP_A1000;

    gtk_widget_set_sensitive (cslevel_widget[0], cstype == CP_GENERIC || allows_ocs);
    gtk_widget_set_sensitive (cslevel_widget[1], cstype == CP_GENERIC || allows_ecs);
    gtk_widget_set_sensitive (cslevel_widget[2], cstype == CP_GENERIC || allows_ecs);
    gtk_widget_set_sensitive (cslevel_widget[3], cstype == CP_GENERIC || needs_aga);

    if (cstype != CP_GENERIC) {
	if (needs_aga)
	    changed_prefs.chipset_mask = CSMASK_AGA | CSMASK_FULL_ECS;
	else if (! allows_ecs)
	    changed_prefs.chipset_mask = 0;
    }
    set_chipset_state ();
}

static void did_reset (void)
{
    if (quit_gui)
	return;

    write_comm_pipe_int (&from_gui_pipe, 2, 1);
}

static void did_debug (void)
{
    if (quit_gui)
	return;

    write_comm_pipe_int (&from_gui_pipe, 3, 1);
}

static void did_quit (void)
{
    if (quit_gui)
	return;

    write_comm_pipe_int (&from_gui_pipe, 4, 1);
}

static void did_eject (GtkWidget *w, gpointer data_p)
{
    int data = GPOINTER_TO_INT (data_p);
    if (quit_gui)
	return;

    write_comm_pipe_int (&from_gui_pipe, 0, 0);
    write_comm_pipe_int (&from_gui_pipe, data, 1);
    gtk_label_set_text (GTK_LABEL (disk_text_widget[data]), "");
}

static void pause_uae (void)
{
    if (quit_gui)
	return;

    write_comm_pipe_int (&from_gui_pipe, GTK_TOGGLE_BUTTON (pause_uae_widget)->active ? 5 : 6, 1);
    if (! GTK_TOGGLE_BUTTON (pause_uae_widget)->active)
	gtk_widget_hide (lab_info);
}

static void end_pause_uae (void)
{
    write_comm_pipe_int (&to_gui_pipe, GUICMD_UNPAUSE, 1);
}

static char fsbuffer[100];

static GtkWidget *make_file_selector (const char *title,
				      GtkFileChooserAction action)
{
    GtkWidget *p = gtk_file_chooser_dialog_new (title, GTK_WINDOW (gui_window), action,
						GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
						action == GTK_FILE_CHOOSER_ACTION_SAVE ? GTK_STOCK_SAVE : GTK_STOCK_OPEN,
						GTK_RESPONSE_ACCEPT,
						NULL);

    return p;
}

static void filesel_set_path (GtkWidget *p, const char *path)
{
    size_t len = strlen (path);
    if (len > 0 && ! access (path, R_OK)) {
	char *tmp = xmalloc (len + 2);
	strcpy (tmp, path);
	strcat (tmp, "/");
	gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (p), tmp);
    }
}

static void make_disk_selector (void)
{
    if (disk_selector)
	return;
    disk_selector = make_file_selector ("Select a disk image file to mount",
					GTK_FILE_CHOOSER_ACTION_OPEN);
    filesel_set_path (disk_selector, currprefs.path_floppy);
}

static void did_insert (GtkWidget *w, gpointer data)
{
    int n = GPOINTER_TO_INT (data);

    make_disk_selector ();
    sprintf (fsbuffer, "Select a disk image file for DF%d", n);
    gtk_window_set_title (GTK_WINDOW (disk_selector), fsbuffer);

    if (gtk_dialog_run (GTK_DIALOG (disk_selector)) == GTK_RESPONSE_ACCEPT) {
	char *filename;

	filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (disk_selector));

	uae_sem_wait (&gui_sem);
	if (new_disk_string[n] != 0)
	    free (new_disk_string[n]);
	new_disk_string[n] = strdup (filename);
	uae_sem_post (&gui_sem);
	write_comm_pipe_int (&from_gui_pipe, 1, 0);
	write_comm_pipe_int (&from_gui_pipe, n, 1);
	gtk_label_set_text (GTK_LABEL (disk_text_widget[n]), filename);

	g_free (filename);
    }
    gtk_widget_hide (disk_selector);
}

static gint driveled_event (GtkWidget *thing, GdkEvent *event)
{
    int lednr = nr_for_led (thing);

    switch (event->type) {
     case GDK_MAP:
	draw_led (lednr);
	break;
     case GDK_EXPOSE:
	draw_led (lednr);
	break;
     default:
	break;
    }

  return 0;
}

static GtkWidget *snap_selector;

static void did_loadstate (void)
{
    GtkWidget *dialog;

    dialog = make_file_selector ("Select a state file to restore", GTK_FILE_CHOOSER_ACTION_OPEN);

    if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT) {
	char *filename;

	filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));

	uae_sem_wait (&gui_sem);
	gui_snapname = strdup (filename);
	uae_sem_post (&gui_sem);
	write_comm_pipe_int (&from_gui_pipe, 7, 0);
	write_comm_pipe_int (&from_gui_pipe, STATE_DORESTORE, 1);

	g_free (filename);
    }

    gtk_widget_destroy (dialog);
}

static void did_savestate (void)
{
    GtkWidget *dialog;

    dialog = make_file_selector ("Select a filename for the state file",
				 GTK_FILE_CHOOSER_ACTION_SAVE);

    if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT) {
	char *filename;

	filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));

	uae_sem_wait (&gui_sem);
	gui_snapname = strdup (filename);
	uae_sem_post (&gui_sem);
	write_comm_pipe_int (&from_gui_pipe, 7, 0);
	write_comm_pipe_int (&from_gui_pipe, STATE_DOSAVE, 1);

	g_free (filename);
    }

    gtk_widget_destroy (dialog);
}

static void did_rompathchange (GtkWidget *w, gpointer data)
{
    GtkWidget *pathsel;
    pathsel = make_file_selector ("Select a ROM path",
				  GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER);
    filesel_set_path (pathsel, currprefs.path_rom);
    if (gtk_dialog_run (GTK_DIALOG (pathsel)) == GTK_RESPONSE_ACCEPT) {
	char *filename;

	filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (pathsel));

	uae_sem_wait (&gui_sem);
	gui_romname = strdup (filename);
	uae_sem_post (&gui_sem);
	write_comm_pipe_int (&from_gui_pipe, 10, 0);
	gtk_label_set_text (GTK_LABEL (rom_path_widget), gui_romname);

	g_free (filename);
    }

    gtk_widget_destroy (pathsel);
}

static void did_romchange (GtkWidget *w, gpointer data)
{
    GtkWidget *romsel;

    romsel = make_file_selector ("Select a ROM file",
				 GTK_FILE_CHOOSER_ACTION_OPEN);
    filesel_set_path (romsel, currprefs.path_rom);
    if (gtk_dialog_run (GTK_DIALOG (romsel)) == GTK_RESPONSE_ACCEPT) {
	char *filename;

	filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (romsel));
	rom_req = 0;
	rom_requirements_change ();

	uae_sem_wait (&gui_sem);
	gui_romname = strdup (filename);
	uae_sem_post (&gui_sem);
	write_comm_pipe_int (&from_gui_pipe, 8, 0);
	gtk_label_set_text (GTK_LABEL (rom_text_widget), gui_romname);

	g_free (filename);
    }

    gtk_widget_destroy (romsel);
}

static void did_keychange (GtkWidget *w, gpointer data)
{
    GtkWidget *keysel;

    keysel = make_file_selector ("Select a Kickstart key file",
				 GTK_FILE_CHOOSER_ACTION_OPEN);
    filesel_set_path (keysel, currprefs.path_rom);

    if (gtk_dialog_run (GTK_DIALOG (keysel)) == GTK_RESPONSE_ACCEPT) {
	char *filename;

	filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (keysel));
	uae_sem_wait (&gui_sem);
	gui_keyname = strdup (filename);
	uae_sem_post (&gui_sem);
	write_comm_pipe_int (&from_gui_pipe, 9, 0);
	gtk_label_set_text (GTK_LABEL (key_text_widget), gui_keyname);

	g_free (filename);
    }
    
    gtk_widget_destroy (keysel);
}

static void add_empty_vbox (GtkWidget *tobox)
{
    GtkWidget *thing = gtk_vbox_new (FALSE, 0);
    gtk_widget_show (thing);
    gtk_box_pack_start (GTK_BOX (tobox), thing, TRUE, TRUE, 0);
}

static void add_centered_to_vbox (GtkWidget *vbox, GtkWidget *w, int vexpand)
{
    GtkWidget *hbox = gtk_hbox_new (TRUE, 0);
    gtk_widget_show (hbox);
    gtk_box_pack_start (GTK_BOX (hbox), w, TRUE, FALSE, 0);
    gtk_box_pack_start (GTK_BOX (vbox), hbox, vexpand, vexpand, 0);
}

static GtkWidget *make_labelled_widget (const char *str, GtkWidget *thing, int expand)
{
    GtkWidget *label = gtk_label_new (str);
    GtkWidget *hbox2 = gtk_hbox_new (FALSE, 4);

    gtk_widget_show (label);
    gtk_widget_show (thing);

    gtk_box_pack_start (GTK_BOX (hbox2), label, FALSE, TRUE, 0);
    gtk_box_pack_start (GTK_BOX (hbox2), thing, expand, TRUE, 0);

    return hbox2;
}

static GtkWidget *add_labelled_widget_centered (const char *str, GtkWidget *thing, GtkWidget *vbox)
{
    GtkWidget *w = make_labelled_widget (str, thing, FALSE);
    gtk_widget_show (w);
    add_centered_to_vbox (vbox, w, 0);
    return w;
}

static int make_radio_group (const char **labels, GtkWidget *tobox,
			      GtkWidget **saveptr, gint t1, gint t2,
			      void (*sigfunc) (void), int count, GSList *group)
{
    int t = 0;

    while (*labels && (count == -1 || count-- > 0)) {
	GtkWidget *thing = gtk_radio_button_new_with_label (group, *labels++);
	group = gtk_radio_button_group (GTK_RADIO_BUTTON (thing));

	*saveptr++ = thing;
	gtk_widget_show (thing);
	gtk_box_pack_start (GTK_BOX (tobox), thing, t1, t2, 0);
	gtk_signal_connect (GTK_OBJECT (thing), "clicked", (GtkSignalFunc) sigfunc, NULL);
	t++;
    }
    return t;
}

static GtkWidget *make_radio_group_box (const char *title, const char **labels,
					GtkWidget **saveptr, int horiz,
					void (*sigfunc) (void), GtkWidget **boxp)
{
    GtkWidget *frame, *newbox;

    frame = gtk_frame_new (title);
    newbox = (horiz ? gtk_hbox_new : gtk_vbox_new) (FALSE, 4);
    gtk_widget_show (newbox);
    gtk_container_set_border_width (GTK_CONTAINER (newbox), 4);
    gtk_container_add (GTK_CONTAINER (frame), newbox);
    make_radio_group (labels, newbox, saveptr, horiz, !horiz, sigfunc, -1, NULL);
    if (boxp)
	*boxp = newbox;
    return frame;
}

static GtkWidget *make_radio_group_box_1 (const char *title, const char **labels,
					  GtkWidget **saveptr, int horiz,
					  void (*sigfunc) (void), int elts_per_column)
{
    GtkWidget *frame, *newbox;
    GtkWidget *column;
    GSList *group = 0;

    frame = gtk_frame_new (title);
    column = (horiz ? gtk_vbox_new : gtk_hbox_new) (FALSE, 4);
    gtk_container_add (GTK_CONTAINER (frame), column);
    gtk_widget_show (column);

    while (*labels) {
	int count;
	newbox = (horiz ? gtk_hbox_new : gtk_vbox_new) (FALSE, 4);
	gtk_widget_show (newbox);
	gtk_container_set_border_width (GTK_CONTAINER (newbox), 4);
	gtk_container_add (GTK_CONTAINER (column), newbox);
	count = make_radio_group (labels, newbox, saveptr, horiz, !horiz, sigfunc, elts_per_column, group);
	labels += count;
	saveptr += count;
	group = gtk_radio_button_group (GTK_RADIO_BUTTON (saveptr[-1]));
    }
    return frame;
}

static GtkWidget *make_led (int nr)
{
    GtkWidget *subframe, *the_led, *thing;
    GdkColormap *colormap;

    the_led = gtk_vbox_new (FALSE, 0);
    gtk_widget_show (the_led);

    thing = gtk_preview_new (GTK_PREVIEW_COLOR);
    gtk_box_pack_start (GTK_BOX (the_led), thing, TRUE, TRUE, 0);
    gtk_widget_show (thing);

    subframe = gtk_frame_new (NULL);
    gtk_box_pack_start (GTK_BOX (the_led), subframe, TRUE, TRUE, 0);
    gtk_widget_show (subframe);

    thing = gtk_drawing_area_new ();
    gtk_drawing_area_size (GTK_DRAWING_AREA (thing), 20, 5);
    gtk_widget_set_events (thing, GDK_EXPOSURE_MASK);
    gtk_container_add (GTK_CONTAINER (subframe), thing);
    colormap = gtk_widget_get_colormap (thing);
    led_on[nr].red = nr == 0 ? 0xEEEE : 0xCCCC;
    led_on[nr].green = nr == 0 ? 0: 0xFFFF;
    led_on[nr].blue = 0;
    led_on[nr].pixel = 0;
    led_off[nr].red = 0;
    led_off[nr].green = 0;
    led_off[nr].blue = 0;
    led_off[nr].pixel = 0;
    gdk_color_alloc (colormap, led_on + nr);
    gdk_color_alloc (colormap, led_off + nr);
    led_widgets[nr] = thing;
    gtk_signal_connect (GTK_OBJECT (thing), "event",
			(GtkSignalFunc) driveled_event, (gpointer) thing);
    gtk_widget_show (thing);

    thing = gtk_preview_new (GTK_PREVIEW_COLOR);
    gtk_box_pack_start (GTK_BOX (the_led), thing, TRUE, TRUE, 0);
    gtk_widget_show (thing);

    return the_led;
}

static GtkWidget *make_file_container (const char *title, GtkWidget *vbox, int vertical)
{
    GtkWidget *thing = gtk_frame_new (title);
    GtkWidget *file_vbox = vertical ? gtk_vbox_new (FALSE, 4) : gtk_hbox_new (FALSE, 4);

    gtk_container_set_border_width (GTK_CONTAINER (file_vbox), 4);
    gtk_container_add (GTK_CONTAINER (thing), file_vbox);
    gtk_box_pack_start (GTK_BOX (vbox), thing, FALSE, TRUE, 0);
    gtk_widget_show (file_vbox);
    gtk_widget_show (thing);

    return file_vbox;
}

static GtkWidget *make_file_widget (GtkWidget *buttonbox)
{
    GtkWidget *thing, *subthing;
    GtkWidget *subframe = gtk_frame_new (NULL);

    gtk_frame_set_shadow_type (GTK_FRAME (subframe), GTK_SHADOW_ETCHED_OUT);
    gtk_box_pack_start (GTK_BOX (buttonbox), subframe, TRUE, TRUE, 0);
    gtk_widget_show (subframe);
    subthing = gtk_vbox_new (FALSE, 0);
    gtk_widget_show (subthing);
    gtk_container_add (GTK_CONTAINER (subframe), subthing);
    thing = gtk_label_new ("");
    gtk_widget_show (thing);
    gtk_box_pack_start (GTK_BOX (subthing), thing, TRUE, TRUE, 0);

    return thing;
}

static void make_floppy_disks (GtkWidget *vbox)
{
    GtkWidget *thing, *subthing, *subframe, *buttonbox, *file_vbox;
    char buf[5];
    int i;

    add_empty_vbox (vbox);

    for (i = 0; i < 4; i++) {
	/* Frame with an hbox and the "DFx:" title */
	sprintf (buf, "DF%d:", i);
	file_vbox = make_file_container (buf, vbox, 1);

	buttonbox = gtk_hbox_new (FALSE, 4);
	gtk_box_pack_start (GTK_BOX (file_vbox), buttonbox, FALSE, TRUE, 0);
	gtk_widget_show (buttonbox);

	/* LED */
	subthing = make_led (i + 1);
	gtk_box_pack_start (GTK_BOX (buttonbox), subthing, FALSE, TRUE, 0);

	/* Current file display */
	disk_text_widget[i] = make_file_widget (buttonbox);

	/* Now, the buttons.  */
	thing = gtk_button_new_with_label ("Eject");
	gtk_box_pack_start (GTK_BOX (buttonbox), thing, FALSE, TRUE, 0);
	gtk_widget_show (thing);
	disk_eject_widget[i] = thing;
	gtk_signal_connect (GTK_OBJECT (thing), "clicked", (GtkSignalFunc) did_eject,
			    GINT_TO_POINTER (i));

	thing = gtk_button_new_with_label ("Insert");
	gtk_box_pack_start (GTK_BOX (buttonbox), thing, FALSE, TRUE, 0);
	gtk_widget_show (thing);
	disk_insert_widget[i] = thing;
	gtk_signal_connect (GTK_OBJECT (thing), "clicked", (GtkSignalFunc) did_insert,
			    GINT_TO_POINTER (i));

	buttonbox = gtk_hbox_new (FALSE, 4);
	gtk_box_pack_start (GTK_BOX (file_vbox), buttonbox, FALSE, FALSE, 0);
	gtk_widget_show (buttonbox);

	if (i == 0)
	    thing = make_chooser (2, "3.5 DD", "3.5 HD");
	else
	    thing = make_chooser (3, "Disabled", "3.5 DD", "3.5 HD");
	gtk_signal_connect (GTK_OBJECT (thing), "changed", (GtkSignalFunc) dftype_changed,
			    GINT_TO_POINTER (i));
	dftype_widget[i] = thing;

	thing = make_labelled_widget ("Type:", thing, 0);
	gtk_box_pack_end (GTK_BOX (buttonbox), thing, FALSE, FALSE, 0);
	gtk_widget_show (thing);
    }

    add_empty_vbox (vbox);
}

static GtkWidget *make_cpu_speed_sel (void)
{
    int t;
    static const char *labels[] = {
	"Optimize for host CPU speed", "Approximate 68000/7MHz speed", "Adjustable",
	NULL
    };
    GtkWidget *frame, *newbox;

    frame = gtk_frame_new ("CPU speed");
    newbox = gtk_vbox_new (FALSE, 4);
    gtk_widget_show (newbox);
    gtk_container_set_border_width (GTK_CONTAINER (newbox), 4);
    gtk_container_add (GTK_CONTAINER (frame), newbox);
    make_radio_group (labels, newbox, cpuspeed_widgets, 0, 1, cpuspeed_changed, -1, NULL);

    t = currprefs.m68k_speed > 0 ? currprefs.m68k_speed : 4 * CYCLE_UNIT;
    cpuspeed_adj = GTK_ADJUSTMENT (gtk_adjustment_new (t, 1.0, 5120.0, 1.0, 1.0, 1.0));
    gtk_signal_connect (GTK_OBJECT (cpuspeed_adj), "value_changed",
			GTK_SIGNAL_FUNC (cpuspeed_changed), NULL);

    cpuspeed_scale = gtk_hscale_new (cpuspeed_adj);
    gtk_range_set_update_policy (GTK_RANGE (cpuspeed_scale), GTK_UPDATE_DELAYED);
    gtk_scale_set_digits (GTK_SCALE (cpuspeed_scale), 0);
    gtk_scale_set_value_pos (GTK_SCALE (cpuspeed_scale), GTK_POS_RIGHT);
    cpuspeed_scale = make_labelled_widget ("Cycles/insn:", cpuspeed_scale, TRUE);
    gtk_widget_show (cpuspeed_scale);
    gtk_box_pack_start (GTK_BOX (newbox), cpuspeed_scale, FALSE, FALSE, 0);

    return frame;
}

static void make_cpu_widgets (GtkWidget *vbox)
{
    int i;
    GtkWidget *newbox, *hbox, *frame;
    GtkWidget *thing, *label;
    static const char *cpulabels[] = {
	"68000", "68010", "68EC020", "68020", "68030", "68040", "68060",
	NULL
    };
    static const char *fpulabels[] = {
	"None", "68881", "68882", "68040", "68060",
	NULL
    };

    add_empty_vbox (vbox);

    hbox = gtk_hbox_new (FALSE, 0);
    add_empty_vbox (hbox);

    newbox = make_radio_group_box ("CPU type", cpulabels, cpu_widget, 0, cputype_changed, NULL);
    gtk_widget_show (newbox);
    gtk_box_pack_start (GTK_BOX (hbox), newbox, FALSE, FALSE, 0);

    newbox = make_radio_group_box ("FPU type", fpulabels, fpu_widget, 0, cputype_changed, NULL);
    gtk_widget_show (newbox);
    gtk_box_pack_start (GTK_BOX (hbox), newbox, FALSE, FALSE, 0);

    newbox = make_cpu_speed_sel ();
    gtk_widget_show (newbox);
    gtk_box_pack_start (GTK_BOX (hbox), newbox, FALSE, FALSE, 0);

    add_empty_vbox (hbox);
    gtk_widget_show (hbox);
    gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);

    req68ec020 = gtk_label_new ("The selected ROM requires at least a 68ec020.");
    add_centered_to_vbox (vbox, req68ec020, 0);

    req68020 = gtk_label_new ("The selected ROM requires at least a 68020.");
    add_centered_to_vbox (vbox, req68020, 0);

    req68030 = gtk_label_new ("The selected ROM requires at least a 68030.");
    add_centered_to_vbox (vbox, req68030, 0);

    add_empty_vbox (vbox);

    label = gtk_label_new ("CPU type settings take effect after the next reset.");
    gtk_widget_show (label);
    add_centered_to_vbox (vbox, label, 0);
}

static GtkWidget *make_screenmode_box (const char *title, struct scrmode_widgets *mw,
				       struct uae_rect *modes, int n_modes)
{
    GtkWidget *vbox;
    int i;

    mw->frame = gtk_frame_new (title);
    gtk_widget_show (mw->frame);

    vbox = gtk_vbox_new (FALSE, 10);
    gtk_widget_show (vbox);
    gtk_container_add (GTK_CONTAINER (mw->frame), vbox);
    gtk_container_set_border_width (GTK_CONTAINER (vbox), 4);

    mw->modelist = gtk_combo_box_new_text ();
    gtk_widget_show (mw->modelist);
    gtk_box_pack_start (GTK_BOX (vbox), mw->modelist, FALSE, TRUE, 0);

    for (i = 0; i < n_modes; i++) {
	char t[200];
	sprintf (t, "%dx%d", modes[i].w, modes[i].h);
	gtk_combo_box_append_text (GTK_COMBO_BOX (mw->modelist), t);
    }
    if (n_modes == 0)
	gtk_widget_set_sensitive (mw->frame, FALSE);

    mw->hcenter = make_chooser (3, "Off", "Simple", "Smart");
    mw->hc_label = make_labelled_widget ("Horizontal bounds detect:", mw->hcenter, TRUE);
    gtk_widget_show (mw->hc_label);
    gtk_box_pack_start (GTK_BOX (vbox), mw->hc_label, FALSE, TRUE, 0);
    
    mw->vcenter = make_chooser (3, "Off", "Simple", "Smart");
    mw->vc_label = make_labelled_widget ("Vertical bounds detect:", mw->vcenter, TRUE);
    gtk_widget_show (mw->vc_label);
    gtk_box_pack_start (GTK_BOX (vbox), mw->vc_label, FALSE, TRUE, 0);

    mw->linedbl = make_chooser (2, "Double", "Scanlines");
    mw->ld_label = make_labelled_widget ("Line doubling:", mw->linedbl, TRUE);
    gtk_widget_show (mw->ld_label);
    gtk_box_pack_start (GTK_BOX (vbox), mw->ld_label, FALSE, TRUE, 0);

    mw->aspect = gtk_check_button_new_with_label ("Correct aspect ratio");
    gtk_widget_show (mw->aspect);
    gtk_box_pack_start (GTK_BOX (vbox), mw->aspect, FALSE, TRUE, 0);

    mw->leds = gtk_check_button_new_with_label ("Show LEDs on screen");
    gtk_widget_show (mw->leds);
    gtk_box_pack_start (GTK_BOX (vbox), mw->leds, FALSE, TRUE, 0);

    gtk_signal_connect (GTK_OBJECT (mw->leds), "clicked",
			(GtkSignalFunc) leds_changed, NULL);
    gtk_signal_connect (GTK_OBJECT (mw->aspect), "clicked",
			(GtkSignalFunc) aspect_changed, NULL);
    gtk_signal_connect (GTK_OBJECT (mw->linedbl), "changed",
			(GtkSignalFunc) aspect_changed, NULL);
    gtk_signal_connect (GTK_OBJECT (mw->hcenter), "changed",
			(GtkSignalFunc) aspect_changed, NULL);
    gtk_signal_connect (GTK_OBJECT (mw->vcenter), "changed",
			(GtkSignalFunc) aspect_changed, NULL);
    gtk_signal_connect (GTK_OBJECT (mw->modelist), "changed",
			(GtkSignalFunc) screenmode_changed, NULL);

    return mw->frame;
}

static void make_gfx_widgets (GtkWidget *vbox)
{
    GtkWidget *thing, *frame, *newbox, *hbox;

    add_empty_vbox (vbox);

    hbox = gtk_hbox_new (FALSE, 10);
    gtk_widget_show (hbox);
    add_centered_to_vbox (vbox, hbox, 0);

    thing = make_screenmode_box ("Fullscreen mode", &gw_fullscreen, gfx_fullscreen_modes, n_fullscreen_modes);
    gtk_box_pack_start (GTK_BOX (hbox), thing, FALSE, FALSE, 0);
    thing = make_screenmode_box ("Windowed mode", &gw_windowed, gfx_windowed_modes, n_windowed_modes);
    gtk_box_pack_start (GTK_BOX (hbox), thing, FALSE, FALSE, 0);
    
    hbox = gtk_hbox_new (FALSE, 10);
    gtk_widget_show (hbox);
    add_centered_to_vbox (vbox, hbox, 0);

    frame = gtk_frame_new ("Miscellaneous");
    gtk_box_pack_start (GTK_BOX (hbox), frame, FALSE, FALSE, 0);

    gtk_widget_show (frame);
    newbox = gtk_vbox_new (FALSE, 4);
    gtk_widget_show (newbox);
    gtk_container_set_border_width (GTK_CONTAINER (newbox), 4);
    gtk_container_add (GTK_CONTAINER (frame), newbox);

    framerate_adj = GTK_ADJUSTMENT (gtk_adjustment_new (currprefs.gfx_framerate, 1, 21, 1, 1, 1));
    gtk_signal_connect (GTK_OBJECT (framerate_adj), "value_changed",
			GTK_SIGNAL_FUNC (custom_changed), NULL);

    thing = gtk_hscale_new (framerate_adj);
    gtk_range_set_update_policy (GTK_RANGE (thing), GTK_UPDATE_DELAYED);
    gtk_scale_set_digits (GTK_SCALE (thing), 0);
    gtk_scale_set_value_pos (GTK_SCALE (thing), GTK_POS_RIGHT);
    thing = make_labelled_widget ("Framerate:", thing, TRUE);
    gtk_widget_show (thing);
    gtk_box_pack_start (GTK_BOX (newbox), thing, FALSE, FALSE, 0);

    b32_widget = gtk_check_button_new_with_label ("32 bit blitter");
    add_centered_to_vbox (newbox, b32_widget, 0);
#if 0
    gtk_widget_show (b32_widget);
#endif
    bimm_widget = gtk_check_button_new_with_label ("Immediate blits");
    gtk_box_pack_start (GTK_BOX (newbox), bimm_widget, FALSE, FALSE, 0);
    gtk_widget_show (bimm_widget);

    afscr_widget = gtk_check_button_new_with_label ("Amiga modes fullscreen");
    gtk_box_pack_start (GTK_BOX (newbox), afscr_widget, FALSE, FALSE, 0);
    gtk_widget_show (afscr_widget);
    pfscr_widget = gtk_check_button_new_with_label ("Picasso modes fullscreen");
    gtk_box_pack_start (GTK_BOX (newbox), pfscr_widget, FALSE, FALSE, 0);
    gtk_widget_show (pfscr_widget);
    add_empty_vbox (vbox);

    gtk_signal_connect (GTK_OBJECT (bimm_widget), "clicked",
			(GtkSignalFunc) custom_changed, NULL);
    gtk_signal_connect (GTK_OBJECT (afscr_widget), "clicked",
			(GtkSignalFunc) custom_changed, NULL);
    gtk_signal_connect (GTK_OBJECT (pfscr_widget), "clicked",
			(GtkSignalFunc) custom_changed, NULL);
#if 0
    gtk_signal_connect (GTK_OBJECT (b32_widget), "clicked",
			(GtkSignalFunc) custom_changed, NULL);
#endif
}

static void make_chipset_widgets (GtkWidget *vbox)
{
    GtkWidget *newbox, *hbox, *thing, *tmpbox;
    static const char *colllabels[] = {
	"None (fastest)", "Sprites only", "Sprites & playfields", "Full (very slow)",
	NULL
    };
    static const char *cslevellabels[] = {
	"OCS", "ECS Agnus", "Full ECS", "AGA", NULL
    };
    static const char *ntsclabels[] = {
	"PAL (European)", "NTSC (American)", NULL
    };

    add_empty_vbox (vbox);

    mbtype_widget = make_chooser (7, "Generic", "A1000", "A500/A2000", "A600", "A1200", "A3000", "A4000");
    gtk_widget_show (mbtype_widget);
    gtk_signal_connect (GTK_OBJECT (mbtype_widget), "changed", (GtkSignalFunc) mbtype_changed,
			NULL);
    mbtype_labelled = make_labelled_widget ("Motherboard type:", mbtype_widget, TRUE);
    gtk_widget_show (mbtype_labelled);
    add_centered_to_vbox (vbox, mbtype_labelled, 0);

    hbox = gtk_hbox_new (FALSE, 10);
    gtk_widget_show (hbox);
    add_centered_to_vbox (vbox, hbox, 0);

    newbox = make_radio_group_box ("Sprite collisions", colllabels, coll_widget, 0, coll_changed, NULL);
    gtk_widget_show (newbox);
    gtk_box_pack_start (GTK_BOX (hbox), newbox, FALSE, TRUE, 0);

    newbox = make_radio_group_box ("Chipset", cslevellabels, cslevel_widget, 0, cslevel_changed, NULL);
    gtk_widget_show (newbox);
    gtk_box_pack_start (GTK_BOX (hbox), newbox, FALSE, TRUE, 0);

    newbox = make_radio_group_box ("Chipset video output", ntsclabels, ntsc_widget, 0, ntsc_changed, NULL);
    gtk_widget_show (newbox);
    gtk_box_pack_start (GTK_BOX (hbox), newbox, FALSE, TRUE, 0);

    reqa4000 = gtk_label_new ("The selected ROM requires at least an A4000.");
    add_centered_to_vbox (vbox, reqa4000, 0);

    reqa1000 = gtk_label_new ("The selected ROM requires an A1000 motherboard.");
    add_centered_to_vbox (vbox, reqa1000, 0);

    add_empty_vbox (vbox);

    thing = gtk_label_new ("Chipset type settings take effect after the next reset.");
    gtk_widget_show (thing);
    add_centered_to_vbox (vbox, thing, 0);
}

static void make_sound_widgets (GtkWidget *vbox)
{
    GtkWidget *frame, *newbox;
    int t;
    GtkWidget *hbox, *inner_vbox, *outer_hbox;
    static const char *soundlabels1[] = {
	"None", "No output", "Normal", "Accurate",
	NULL
    }, *soundlabels2[] = {
	"None", "Sinc", "Anti",
	NULL
    }, *soundlabels3[] = {
	"Off", "A500 (power LED)", "A500 (always on)",
	"A1200 (power LED)", "A1200 (always on)",
	NULL
    };

    add_empty_vbox (vbox);

    outer_hbox = gtk_hbox_new (FALSE, 0);
    gtk_widget_show (outer_hbox);
    add_centered_to_vbox (vbox, outer_hbox, 0);
    
    inner_vbox = gtk_vbox_new (FALSE, 0);
    gtk_widget_show (inner_vbox);
    gtk_box_pack_start (GTK_BOX (outer_hbox), inner_vbox, FALSE, TRUE, 0);

    hbox = gtk_hbox_new (FALSE, 0);
    add_empty_vbox (hbox);

    newbox = make_radio_group_box ("Mode", soundlabels1, sound_widget, 0, sound_changed, NULL);
    gtk_widget_show (newbox);
    gtk_box_pack_start (GTK_BOX (hbox), newbox, FALSE, FALSE, 0);

    newbox = make_radio_group_box ("Resampler", soundlabels2, sound_interpol_widget, 0, sound_changed, NULL);
    gtk_widget_show (newbox);
    gtk_box_pack_start (GTK_BOX (hbox), newbox, FALSE, TRUE, 0);

    newbox = make_radio_group_box ("Filter", soundlabels3, sound_filter_widget, 0, sound_changed, NULL);
    gtk_widget_show (newbox);
    gtk_box_pack_start (GTK_BOX (hbox), newbox, FALSE, TRUE, 0);

    add_empty_vbox (hbox);
    gtk_widget_show (hbox);
    gtk_box_pack_start (GTK_BOX (inner_vbox), hbox, FALSE, FALSE, 0);

    t = currprefs.sound_stereo_separation;
    if (t == 0)
	t = MIXED_STEREO_MAX;
    stereo_sep_adj = GTK_ADJUSTMENT (gtk_adjustment_new (t, 1, MIXED_STEREO_MAX, 1, 1, 0));
    gtk_signal_connect (GTK_OBJECT (stereo_sep_adj), "value_changed",
			GTK_SIGNAL_FUNC (sound_changed), NULL);

    stereo_sep_scale = gtk_hscale_new (stereo_sep_adj);
    gtk_range_set_update_policy (GTK_RANGE (stereo_sep_scale), GTK_UPDATE_DELAYED);
    gtk_scale_set_digits (GTK_SCALE (stereo_sep_scale), 0);
    gtk_scale_set_value_pos (GTK_SCALE (stereo_sep_scale), GTK_POS_RIGHT);
    stereo_sep_scale = make_labelled_widget ("Stereo separation:", stereo_sep_scale, TRUE);
    gtk_widget_show (stereo_sep_scale);
    gtk_box_pack_start (GTK_BOX (inner_vbox), stereo_sep_scale, FALSE, FALSE, 0);

    t = currprefs.sound_mixed_stereo_delay;
    stereo_delay_adj = GTK_ADJUSTMENT (gtk_adjustment_new (t, 0, SOUND_MAX_LOG_DELAY, 1, 1, 1));
    gtk_signal_connect (GTK_OBJECT (stereo_delay_adj), "value_changed",
			GTK_SIGNAL_FUNC (sound_changed), NULL);

    stereo_delay_scale = gtk_hscale_new (stereo_delay_adj);
    gtk_range_set_update_policy (GTK_RANGE (stereo_delay_scale), GTK_UPDATE_DELAYED);
    gtk_scale_set_digits (GTK_SCALE (stereo_delay_scale), 0);
    gtk_scale_set_value_pos (GTK_SCALE (stereo_delay_scale), GTK_POS_RIGHT);
    stereo_delay_scale = make_labelled_widget ("Stereo mixing delay:", stereo_delay_scale, TRUE);
    gtk_widget_show (stereo_delay_scale);
    gtk_box_pack_start (GTK_BOX (inner_vbox), stereo_delay_scale, FALSE, FALSE, 0);

    add_empty_vbox (vbox);
}

static void make_rom_widgets (GtkWidget *dvbox)
{
    GtkWidget *hbox = gtk_hbox_new (FALSE, 10);
    GtkWidget *label, *frame;

    rom_req = 0;

    add_empty_vbox (dvbox);

    {
	GtkWidget *buttonbox = make_file_container ("ROM search path:", dvbox, 0);
	GtkWidget *thing = gtk_button_new_with_label ("Change");

	/* Current file display */
	rom_path_widget = make_file_widget (buttonbox);

	gtk_box_pack_start (GTK_BOX (buttonbox), thing, FALSE, TRUE, 0);
	gtk_widget_show (thing);
	gtk_signal_connect (GTK_OBJECT (thing), "clicked", (GtkSignalFunc) did_rompathchange, 0);
    }

    {
	GtkWidget *buttonbox = make_file_container ("Kickstart ROM file:", dvbox, 1);
	GtkWidget *hbox, *thing;

	romlist_widget = gtk_combo_box_new_text ();
	gtk_widget_show (romlist_widget);
	gtk_box_pack_start (GTK_BOX (buttonbox), romlist_widget, FALSE, TRUE, 0);
	gtk_combo_box_append_text (GTK_COMBO_BOX (romlist_widget),
				   "Manually select a ROM file");
	romlist_nr_entries = 1;
	gtk_signal_connect (GTK_OBJECT (romlist_widget), "changed", (GtkSignalFunc) rom_combo_changed, 0);

	hbox = gtk_hbox_new (FALSE, 0);
	gtk_box_pack_start (GTK_BOX (buttonbox), hbox, FALSE, TRUE, 0);
	gtk_widget_show (hbox);

	/* Current file display */
	rom_text_widget = make_file_widget (hbox);

	romsel_button = gtk_button_new_with_label ("Change");
	gtk_box_pack_start (GTK_BOX (hbox), romsel_button, FALSE, TRUE, 0);
	gtk_widget_show (romsel_button);
	gtk_signal_connect (GTK_OBJECT (romsel_button), "clicked", (GtkSignalFunc) did_romchange, 0);
    }

    {
	GtkWidget *buttonbox = make_file_container ("ROM key file for Cloanto Amiga Forever:", dvbox, 0);
	GtkWidget *thing = gtk_button_new_with_label ("Change");

	/* Current file display */
	key_text_widget = make_file_widget (buttonbox);

	gtk_box_pack_start (GTK_BOX (buttonbox), thing, FALSE, TRUE, 0);
	gtk_widget_show (thing);
	gtk_signal_connect (GTK_OBJECT (thing), "clicked", (GtkSignalFunc) did_keychange, 0);
    }

    label = gtk_label_new ("These settings take effect after the next reset.");
    gtk_widget_show (label);
    add_centered_to_vbox (dvbox, label, 0);

    add_empty_vbox (dvbox);
}

static void make_ram_widgets (GtkWidget *dvbox)
{
    GtkWidget *hbox = gtk_hbox_new (FALSE, 10);
    GtkWidget *label, *frame;

    static const char *chiplabels[] = {
	"512 KB", "1 MB", "2 MB", "4 MB", "8 MB", NULL
    };
    static const char *bogolabels[] = {
	"None", "512 KB", "1 MB", "1.8 MB", NULL
    };
    static const char *fastlabels[] = {
	"None", "1 MB", "2 MB", "4 MB", "8 MB", NULL
    };
    static const char *z3labels[] = {
	"None", "1 MB", "2 MB", "4 MB", "8 MB",
	"16 MB", "32 MB", "64 MB", "128 MB", "256 MB",
	NULL
    };
    static const char *p96labels[] = {
	"None", "1 MB", "2 MB", "4 MB", "8 MB", "16 MB", "32 MB", NULL
    };

    add_empty_vbox (dvbox);

    gtk_widget_show (hbox);
    add_centered_to_vbox (dvbox, hbox, 0);

    add_empty_vbox (dvbox);

    label = gtk_label_new ("These settings take effect after the next reset.");
    gtk_widget_show (label);
    add_centered_to_vbox (dvbox, label, 0);

    frame = make_radio_group_box ("Chip Mem", chiplabels, chipsize_widget, 0, chipsize_changed, NULL);
    gtk_widget_show (frame);
    gtk_box_pack_start (GTK_BOX (hbox), frame, FALSE, TRUE, 0);

    frame = make_radio_group_box ("Slow Mem", bogolabels, bogosize_widget, 0, bogosize_changed, NULL);
    gtk_widget_show (frame);
    gtk_box_pack_start (GTK_BOX (hbox), frame, FALSE, TRUE, 0);

    frame = make_radio_group_box ("Fast Mem", fastlabels, fastsize_widget, 0, fastsize_changed, NULL);
    gtk_widget_show (frame);
    gtk_box_pack_start (GTK_BOX (hbox), frame, FALSE, TRUE, 0);

    frame = make_radio_group_box_1 ("Z3 Mem", z3labels, z3size_widget, 0, z3size_changed, 5);
    gtk_widget_show (frame);
    gtk_box_pack_start (GTK_BOX (hbox), frame, FALSE, TRUE, 0);

    frame = make_radio_group_box_1 ("P96 RAM", p96labels, p96size_widget, 0, p96size_changed, 4);
    gtk_widget_show (frame);
    gtk_box_pack_start (GTK_BOX (hbox), frame, FALSE, TRUE, 0);
}

static void make_joy_widgets (GtkWidget *dvbox)
{
    int i;
    GtkWidget *hbox = gtk_hbox_new (FALSE, 10);
    GtkWidget *thing;
    static const char *joylabels[] = {
	"Joystick 0", "Joystick 1", "Mouse", "Numeric pad",
	"Cursor keys/Right Ctrl", "T/F/H/B/Left Alt",
	NULL
    };

    add_empty_vbox (dvbox);
    gtk_widget_show (hbox);
    add_centered_to_vbox (dvbox, hbox, 0);

    legacy_widget = gtk_check_button_new_with_label ("Use legacy configuration");
    add_centered_to_vbox (dvbox, legacy_widget, 0);
    gtk_widget_show (legacy_widget);
    gtk_signal_connect (GTK_OBJECT (legacy_widget), "clicked",
			(GtkSignalFunc) joy_changed, NULL);

    for (i = 0; i < 2; i++) {
	GtkWidget *vbox, *frame;
	char buffer[20];
	int j;

	sprintf (buffer, "Port %d", i);
	frame = make_radio_group_box (buffer, joylabels, joy_widget[i], 0, joy_changed, NULL);
	gtk_widget_show (frame);
	gtk_box_pack_start (GTK_BOX (hbox), frame, FALSE, TRUE, 0);
    }

    thing = make_chooser (7, "English (QWERTY)", "Danish", "Deutsch (QWERTZ)", "Swedish",
			  "Francais (AZERTY)", "Italian", "Spanish");
    gtk_signal_connect (GTK_OBJECT (thing), "changed", (GtkSignalFunc) kbdlang_changed,
			GINT_TO_POINTER (i));
    kbdlang_widget = thing;
    thing = add_labelled_widget_centered ("Keyboard language:", thing, dvbox);

    add_empty_vbox (dvbox);
}

static GtkWidget *leftalign (GtkWidget *thing)
{
    GtkWidget *hbox = gtk_hbox_new (FALSE, 0);
    gtk_box_pack_start (GTK_BOX (hbox), thing, FALSE, FALSE, 0);
    return hbox;
}

static GtkWidget *create_dirdlg (const char *title, int row)
{
    GtkWidget *vbox, *pathsel, *dirdlg;
    GtkWidget *path_entry, *volname_entry, *pri_entry;

    dirdlg = gtk_dialog_new_with_buttons (title, GTK_WINDOW (gui_window),
					  GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
					  GTK_STOCK_OK, GTK_RESPONSE_ACCEPT,
					  GTK_STOCK_CANCEL, GTK_RESPONSE_REJECT,
					  NULL);

    gtk_window_set_position (GTK_WINDOW (dirdlg), GTK_WIN_POS_MOUSE);

    vbox = GTK_DIALOG (dirdlg)->vbox;

    pathsel = make_file_selector ("Select a path to mount",
				  GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER);

    gtkutil_add_table (GTK_WIDGET (vbox),
		       leftalign (gtk_label_new ("Path:")), 1, 1, GTK_FILL,
		       leftalign (path_entry = gtk_file_chooser_button_new_with_dialog (pathsel)), 2, 1, GTK_FILL,
		       GTKUTIL_ROW_END,
		       leftalign (gtk_label_new ("Volume name:")), 1, 1, GTK_FILL,
		       leftalign (volname_entry = gtk_entry_new_with_max_length (32)), 2, 1, GTK_FILL,
		       GTKUTIL_ROW_END,
		       leftalign (gtk_label_new ("Boot priority:")), 1, 1, GTK_FILL,
		       leftalign (pri_entry = gtk_entry_new_with_max_length (2)), 2, 1, GTK_FILL,
		       GTKUTIL_TABLE_END);

    if (row != -1) {
	char *failure;
	char t[20];
	int secspertrack, surfaces, reserved, blocksize, size;
	int cylinders, readonly, pri;
	char *volname, *rootdir, *devname;

	failure = get_filesys_unit (currprefs.mountinfo, row,
				    &devname, &volname, &rootdir, &readonly,
				    &secspertrack, &surfaces, &reserved,
				    &cylinders, &size, &blocksize, &pri);
	if (failure) {
	    mkmsgbox ("Error", failure);
	    goto out;
	}
	gtk_entry_set_text (GTK_ENTRY (volname_entry), volname);
	gtk_file_chooser_set_filename (GTK_FILE_CHOOSER (path_entry), rootdir);
	sprintf (t, "%d", pri);
	gtk_entry_set_text (GTK_ENTRY (pri_entry), t);
	if (volname)
	    free (volname);
	if (devname)
	    free (devname);
	if (rootdir)
	    free (rootdir);
    }

  retry:
    if (gtk_dialog_run (GTK_DIALOG (dirdlg)) == GTK_RESPONSE_ACCEPT) {
	const char *failure;
	const char *pathname = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (path_entry));
	const char *volname = gtk_entry_get_text (GTK_ENTRY (volname_entry));
	const char *bprstr = gtk_entry_get_text (GTK_ENTRY (pri_entry));
	char fixed_volname[256];
	int n, bpr;

	if (pathname == NULL || strlen (pathname) == 0 || access (pathname, R_OK) != 0) {
	    mkmsgbox ("Error", "You must select a valid path.");
	    goto retry;
	}
	n = volname == NULL ? 0 : strlen (volname);
	if (n > 255) {
	    mkmsgbox ("Error", "Volume name too long.");
	    goto retry;
	}
	/* Strip colons from the end.  */
	if (n > 0) {
	    strcpy (fixed_volname, volname);
	    if (fixed_volname[n - 1] == ':') {
		fixed_volname[n - 1] = '\0';
		n--;
	    }
	}

	if (n == 0) {
	    mkmsgbox ("Error", "You must enter a volume name.");
	    goto retry;
	}

	if (strlen (bprstr) == 0) {
	    mkmsgbox ("Error", "You must enter a valid number of surfaces.");
	    goto retry;
	}
	bpr = atoi (bprstr);

	if (row == -1) {
	    failure = add_filesys_unit (currprefs.mountinfo, NULL, fixed_volname, pathname,
					0, 0, 0, 0, 0, bpr);
	} else {
	    failure = set_filesys_unit (currprefs.mountinfo, row, NULL, fixed_volname, pathname,
					0, 0, 0, 0, 0, bpr);
	}
	if (failure) {
	    mkmsgbox ("Error", failure);
	    goto retry;
	}
	set_hd_state ();
    }

  out:
    gtk_widget_destroy (dirdlg);
    return 0;
}

static void create_hdfdlg (const char *title, int row)
{
    GtkWidget *vbox, *pathsel;
    GtkWidget *hdfdlg;
    static GtkWidget *bpt_entry, *surfaces_entry, *pri_entry;
    static GtkWidget *blocksize_entry, *reserved_entry, *path_entry;

    int readonly = 0;

    hdfdlg = gtk_dialog_new_with_buttons (title, GTK_WINDOW (gui_window),
					  GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
					  GTK_STOCK_OK, GTK_RESPONSE_ACCEPT,
					  GTK_STOCK_CANCEL, GTK_RESPONSE_REJECT,
					  NULL);

    gtk_window_set_position (GTK_WINDOW (hdfdlg), GTK_WIN_POS_MOUSE);

    pathsel = make_file_selector ("Select a path to mount",
				  GTK_FILE_CHOOSER_ACTION_OPEN);

    vbox = GTK_DIALOG (hdfdlg)->vbox;

    gtkutil_add_table (GTK_WIDGET (vbox),
		       leftalign (gtk_label_new ("File:")), 1, 1, GTK_FILL,
		       leftalign (path_entry = gtk_file_chooser_button_new_with_dialog (pathsel)), 2, 1, GTK_FILL,
		       GTKUTIL_ROW_END,
		       leftalign (gtk_label_new ("Reserved blocks:")), 1, 1, GTK_FILL,
		       leftalign (reserved_entry = gtk_entry_new_with_max_length (6)), 2, 1, GTK_FILL,
		       GTKUTIL_ROW_END,
		       leftalign (gtk_label_new ("Surfaces:")), 1, 1, GTK_FILL,
		       leftalign (surfaces_entry = gtk_entry_new_with_max_length (2)), 2, 1, GTK_FILL,
		       GTKUTIL_ROW_END,
		       leftalign (gtk_label_new ("Blocksize:")), 1, 1, GTK_FILL,
		       leftalign (blocksize_entry = gtk_entry_new_with_max_length (6)), 2, 1, GTK_FILL,
		       GTKUTIL_ROW_END,		       
		       leftalign (gtk_label_new ("Blocks per track:")), 1, 1, GTK_FILL,
		       leftalign (bpt_entry = gtk_entry_new_with_max_length (6)), 2, 1, GTK_FILL,
		       GTKUTIL_ROW_END,
		       leftalign (gtk_label_new ("Boot priority:")), 1, 1, GTK_FILL,
		       leftalign (pri_entry = gtk_entry_new_with_max_length (2)), 2, 1, GTK_FILL,

		       GTKUTIL_TABLE_END);

    gtk_file_chooser_button_set_width_chars (GTK_FILE_CHOOSER_BUTTON (path_entry), 40);

    if (row != -1) {
	char *failure;
	char *devname, *volname, *rootdir;
	int secspertrack, surfaces, reserved, blocksize, size;
	int cylinders, pri;
	char t[20];

	failure = get_filesys_unit (currprefs.mountinfo, row,
				    &devname, &volname, &rootdir, &readonly,
				    &secspertrack, &surfaces, &reserved,
				    &cylinders, &size, &blocksize, &pri);
	if (failure) {
	    mkmsgbox ("Error", failure);
	    goto out;
	}
	gtk_file_chooser_set_filename (GTK_FILE_CHOOSER (path_entry), rootdir);
	sprintf (t, "%d", blocksize);
	gtk_entry_set_text (GTK_ENTRY (blocksize_entry), t);
	sprintf (t, "%d", secspertrack);
	gtk_entry_set_text (GTK_ENTRY (bpt_entry), t);
	sprintf (t, "%d", surfaces);
	gtk_entry_set_text (GTK_ENTRY (surfaces_entry), t);
	sprintf (t, "%d", reserved);
	gtk_entry_set_text (GTK_ENTRY (reserved_entry), t);
	sprintf (t, "%d", pri);
	gtk_entry_set_text (GTK_ENTRY (pri_entry), t);
	if (volname)
	    free (volname);
	if (devname)
	    free (devname);
	if (rootdir)
	    free (rootdir);
    }

  retry:
    if (gtk_dialog_run (GTK_DIALOG (hdfdlg)) == GTK_RESPONSE_ACCEPT) {
	const char *failure;
	const char *new_filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (path_entry));
	const char *bptstr = gtk_entry_get_text (GTK_ENTRY (bpt_entry));
	const char *sfstr = gtk_entry_get_text (GTK_ENTRY (surfaces_entry));
	const char *bsstr = gtk_entry_get_text (GTK_ENTRY (blocksize_entry));
	const char *rsrvstr = gtk_entry_get_text (GTK_ENTRY (reserved_entry));
	const char *bprstr = gtk_entry_get_text (GTK_ENTRY (pri_entry));
	int n, sf, bpt, bs, rsrv, bpr;

	if (new_filename == NULL || strlen (new_filename) == 0) {
	    mkmsgbox ("Error", "You must enter a valid filename.");
	    goto retry;
	}

	if (strlen (bptstr) == 0 || (bpt = atoi (bptstr)) == 0) {
	    mkmsgbox ("Error", "You must enter a valid number of blocks per track.");
	    goto retry;
	}
	if (strlen (rsrvstr) == 0) {
	    mkmsgbox ("Error", "You must enter the number of reserved blocks.");
	    goto retry;
	}
	rsrv = atoi (rsrvstr);
	if (strlen (sfstr) == 0 || (sf = atoi (sfstr)) == 0) {
	    mkmsgbox ("Error", "You must enter a valid number of surfaces.");
	    goto retry;
	}
	if (strlen (bsstr) == 0 || (bs = atoi (bsstr)) == 0) {
	    mkmsgbox ("Error", "You must enter a valid number of surfaces.");
	    goto retry;
	}
	if (strlen (bprstr) == 0) {
	    mkmsgbox ("Error", "You must enter a valid number of surfaces.");
	    goto retry;
	}
	bpr = atoi (bprstr);

	if (row == -1) {
	    failure = add_filesys_unit (currprefs.mountinfo, NULL, NULL, new_filename,
					readonly, bpt, sf, rsrv, bs, bpr);
	} else {
	    failure = set_filesys_unit (currprefs.mountinfo, row, NULL, NULL, new_filename,
					readonly, bpt, sf, rsrv, bs, bpr);
	}
	if (failure) {
	    mkmsgbox ("Error", failure);
	    goto retry;
	}
	set_hd_state ();
    }
  out:
    gtk_widget_destroy (hdfdlg);
    return;
}

static void did_newdir (void)
{
    create_dirdlg ("Add a new mounted directory", -1);
}

static void did_newhdf (void)
{
    create_hdfdlg ("Add a new hardfile", -1);
}

static void did_newadf (void)
{
    make_disk_selector ();

    if (gtk_dialog_run (GTK_DIALOG (disk_selector)) == GTK_RESPONSE_ACCEPT) {
	char *filename;
	struct zfile *f;
	filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (disk_selector));

	f = zfile_open (filename, "rb");
	if (f) {
	    const char *failure;
	    char buf[512];
	    long size;
	    zfile_fread (buf, 1, 512, f);
	    zfile_fseek (f, 0, SEEK_END);
	    size = zfile_ftell (f);
	    if (buf[0] != 'D' || buf[1] != 'O' || buf[2] != 'S'
		|| (buf[3] != 0 && buf[3] != 1)
		|| size != 901120)
	    {
		mkmsgbox ("Error", "Not an ADF diskfile.");
	    } else {
		failure = add_filesys_unit (currprefs.mountinfo, NULL, NULL, filename,
					    0, 11, 2, 2, 512, -2);
		if (failure)
		    mkmsgbox ("Error", failure);
		else
		    set_hd_state ();
	    }
	    zfile_fclose (f);
	}

	g_free (filename);
    }

    gtk_widget_hide (disk_selector);
}

static void did_hdchange (void)
{
    GtkTreeIter iter;
    GtkTreeModel *model;
    int idx;

    if (gtk_tree_selection_get_selected (hd_selection, &model, &iter))
	gtk_tree_model_get (model, &iter, 0, &idx, -1);
    else
	return;

    if (is_hardfile (currprefs.mountinfo, idx)) {
	create_hdfdlg ("Change a hardfile", idx);
    } else {
	create_dirdlg ("Change a mounted directory", idx);
   }
}
static void did_hddel (void)
{
    GtkTreeIter iter;
    GtkTreeModel *model;
    int idx;

    if (gtk_tree_selection_get_selected (hd_selection, &model, &iter))
	gtk_tree_model_get (model, &iter, 0, &idx, -1);
    else
	return;

    kill_filesys_unit (currprefs.mountinfo, idx);
    set_hd_state ();
}

static void hdsel_changed (GtkWidget *widget, gint row, gint column, GdkEventButton *bevent,
			   gpointer user_data)
{
    GtkTreeIter iter;
    GtkTreeModel *model;

    if (gtk_tree_selection_get_selected (hd_selection, &model, &iter)) {
	gtk_widget_set_sensitive (hdchange_button, TRUE);
	gtk_widget_set_sensitive (hddel_button, TRUE);
    } else {
	gtk_widget_set_sensitive (hdchange_button, FALSE);
	gtk_widget_set_sensitive (hddel_button, FALSE);
    }
}

static void hdunselect (GtkWidget *widget, gint row, gint column, GdkEventButton *bevent,
			gpointer user_data)
{
    gtk_widget_set_sensitive (hdchange_button, FALSE);
    gtk_widget_set_sensitive (hddel_button, FALSE);
}

static void preset_changed (GtkTreeSelection *selection, gpointer data)
{
    GtkTreeIter iter;
    GtkTreeModel *model;

    if (gtk_tree_selection_get_selected (selection, &model, &iter))
	gtk_widget_set_sensitive (preset_button, TRUE);
    else
	gtk_widget_set_sensitive (preset_button, FALSE);
}

static void did_set_preset (void)
{
    GtkTreeIter iter;
    GtkTreeModel *model;
    gchar *filename;

    if (gtk_tree_selection_get_selected (preset_selection, &model, &iter)) {
	gtk_tree_model_get (model, &iter, 1, &filename, -1);

	if (filename) {
	    cfgfile_load (&changed_prefs, filename);
	    set_widgets_from_config ();
	}
	g_free (filename);
    }
}

static GtkWidget *make_buttons_1 (const char *label, GtkWidget *box, void (*sigfunc) (void),
				  GtkWidget *(*create)(const char *label), int expand)
{
    GtkWidget *thing = create (label);
    gtk_widget_show (thing);
    gtk_signal_connect (GTK_OBJECT (thing), "clicked", (GtkSignalFunc) sigfunc, NULL);
    gtk_box_pack_start (GTK_BOX (box), thing, expand, expand, 0);

    return thing;
}
#define make_buttons(label, box, sigfunc, create) make_buttons_1(label, box, sigfunc, create, 1)
#define make_button(label, box, sigfunc) make_buttons(label, box, sigfunc, gtk_button_new_with_label)

static void new_text_cell_renderer (GtkWidget *treeview, const char *title, int colnr,
				    int expand, int ellipsize)
{
    GtkCellRenderer *renderer = gtk_cell_renderer_text_new ();
    GtkTreeViewColumn *column;
    column = gtk_tree_view_column_new_with_attributes (title, renderer,
						       "text", colnr, NULL);
    gtk_tree_view_column_set_expand (column, expand);
    gtk_tree_view_append_column (GTK_TREE_VIEW (treeview), column);
    if (ellipsize)
	g_object_set (renderer,
		      "ellipsize", PANGO_ELLIPSIZE_START,
		      "ellipsize-set", TRUE,
		      NULL);
}

static void make_hd_widgets (GtkWidget *dvbox)
{
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;
    GtkAdjustment *adj;
    GtkWidget *scrolled, *buttonbox, *vbox, *hbox;
    int i;

    hd_store = gtk_list_store_new (11,
				   G_TYPE_INT,
				   G_TYPE_STRING, G_TYPE_STRING,
				   G_TYPE_BOOLEAN,
				   G_TYPE_STRING, G_TYPE_STRING,
				   G_TYPE_STRING, G_TYPE_STRING,
				   G_TYPE_STRING, G_TYPE_STRING,
				   G_TYPE_INT);

#if 0
    add_empty_vbox (dvbox);
#endif

    hbox = gtk_hbox_new (FALSE, 0);
    gtk_box_pack_start (GTK_BOX (dvbox), hbox, TRUE, TRUE, 20);
    gtk_widget_show (hbox);

    vbox = gtk_vbox_new (FALSE, 4);
    gtk_box_pack_start (GTK_BOX (hbox), vbox, TRUE, TRUE, 0);
    gtk_widget_show (vbox);

    hdlist_widget = gtk_tree_view_new_with_model (GTK_TREE_MODEL (hd_store));

    new_text_cell_renderer (hdlist_widget, "Volume", 1, 0, 0);
    new_text_cell_renderer (hdlist_widget, "Path/File", 2, 1, 1);
    
    renderer = gtk_cell_renderer_toggle_new ();
    column = gtk_tree_view_column_new_with_attributes ("R/O", renderer,
						       "active", 3, NULL);
    gtk_tree_view_column_set_expand (column, 0);
    gtk_tree_view_append_column (GTK_TREE_VIEW (hdlist_widget), column);

    new_text_cell_renderer (hdlist_widget, "Heads", 4, 0, 0);
    new_text_cell_renderer (hdlist_widget, "Cyl.", 5, 0, 0);
    new_text_cell_renderer (hdlist_widget, "Sec.", 6, 0, 0);
    new_text_cell_renderer (hdlist_widget, "Rsrvd", 7, 0, 0);
    new_text_cell_renderer (hdlist_widget, "Size", 8, 0, 0);
    new_text_cell_renderer (hdlist_widget, "Blksize", 9, 0, 0);
    new_text_cell_renderer (hdlist_widget, "Pri", 10, 0, 0);

    gtk_widget_show (hdlist_widget);

    scrolled = gtk_scrolled_window_new (NULL, NULL);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled),
				    GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
    gtk_box_pack_start (GTK_BOX (vbox), scrolled, TRUE, TRUE, 0);
    gtk_widget_show (scrolled);

    gtk_container_add (GTK_CONTAINER (scrolled), hdlist_widget);

    hd_selection = gtk_tree_view_get_selection (GTK_TREE_VIEW (hdlist_widget));
    gtk_tree_selection_set_mode (hd_selection, GTK_SELECTION_SINGLE);
    g_signal_connect (G_OBJECT (hd_selection), "changed",
		      G_CALLBACK (hdsel_changed), NULL);

    /* The buttons */
    buttonbox = gtk_hbox_new (FALSE, 4);
    gtk_widget_show (buttonbox);
    gtk_box_pack_start (GTK_BOX (vbox), buttonbox, FALSE, FALSE, 0);
    make_button ("New filesystem...", buttonbox, did_newdir);
    make_button ("New hardfile...", buttonbox, did_newhdf);
    make_button ("Mount floppy as HD...", buttonbox, did_newadf);

    buttonbox = gtk_hbox_new (FALSE, 4);
    gtk_widget_show (buttonbox);
    gtk_box_pack_start (GTK_BOX (vbox), buttonbox, FALSE, FALSE, 0);
    hdchange_button = make_button ("Change...", buttonbox, did_hdchange);
    hddel_button = make_button ("Delete", buttonbox, did_hddel);

    bootrom_widget = gtk_check_button_new_with_label ("Enable bootrom - needed for filesystem, Picasso96 and better mouse support;\ndisable only in case of (unlikely) compatibility issues");
    add_centered_to_vbox (dvbox, bootrom_widget, 0);
    gtk_widget_show (bootrom_widget);
    gtk_signal_connect (GTK_OBJECT (bootrom_widget), "clicked",
			(GtkSignalFunc) bootrom_changed, NULL);
#if 0
    add_empty_vbox (dvbox);
#endif
}

static void make_presets (GtkWidget *dvbox)
{
    GtkListStore *store = gtk_list_store_new (2, G_TYPE_STRING, G_TYPE_STRING);
    GtkCellRenderer *renderer = gtk_cell_renderer_text_new ();
    GtkTreeViewColumn *column;
    GtkWidget *scrolled, *buttonbox, *vbox, *hbox;
    int i;

    add_empty_vbox (dvbox);

    hbox = gtk_hbox_new (FALSE, 0);
    gtk_box_pack_start (GTK_BOX (dvbox), hbox, TRUE, TRUE, 0);
    gtk_widget_show (hbox);

    add_empty_vbox (hbox);

    vbox = gtk_vbox_new (FALSE, 4);
    gtk_box_pack_start (GTK_BOX (hbox), vbox, TRUE, TRUE, 0);
    gtk_widget_show (vbox);

    for (i = 0; i < n_predef_configs; i++) {
	GtkTreeIter iter;
	gtk_list_store_append (store, &iter);

	gtk_list_store_set (store, &iter,
			    0, predef_configs[i].description,
			    1, predef_configs[i].filename,
			    -1);
    }
    preset_list = gtk_tree_view_new_with_model (GTK_TREE_MODEL (store));
    gtk_widget_show (preset_list);

    new_text_cell_renderer (preset_list, "Hardware configuration presets", 0, 1, 0);

    scrolled = gtk_scrolled_window_new (NULL, NULL);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled),
				    GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
    gtk_box_pack_start (GTK_BOX (vbox), scrolled, TRUE, TRUE, 0);
    gtk_widget_show (scrolled);

    gtk_container_add (GTK_CONTAINER (scrolled), preset_list);

    preset_selection = gtk_tree_view_get_selection (GTK_TREE_VIEW (preset_list));
    gtk_tree_selection_set_mode (preset_selection, GTK_SELECTION_SINGLE);
    g_signal_connect (G_OBJECT (preset_selection), "changed",
		      G_CALLBACK (preset_changed),
		      NULL);

    /* The buttons */
    buttonbox = gtk_hbox_new (FALSE, 4);
    gtk_widget_show (buttonbox);
    gtk_box_pack_start (GTK_BOX (vbox), buttonbox, FALSE, FALSE, 0);
    preset_button = make_buttons_1 ("Set configuration to chosen preset", buttonbox, did_set_preset,
				    gtk_button_new_with_label, 0);

    add_empty_vbox (hbox);

    add_empty_vbox (dvbox);
}

static void make_about_widgets (GtkWidget *dvbox)
{
    GtkWidget *lab_version;
    GtkStyle *style;
    GdkFont *font;
    char t[20];

    add_empty_vbox (dvbox);

    sprintf (t, "%s %s", PACKAGE_NAME, PACKAGE_VERSION);
    lab_version = gtk_label_new (t);
    lab_info = gtk_label_new ("Choose your settings, then deselect the Pause button to start!");

    style = gtk_style_copy (GTK_WIDGET (lab_version)->style);
    pango_font_description_free (style->font_desc);
    style->font_desc = pango_font_description_from_string ("Sans 24");
    gtk_widget_set_style (lab_version, style);

    add_centered_to_vbox (dvbox, lab_version, 0);
    add_centered_to_vbox (dvbox, lab_info, 0);
    gtk_widget_show (lab_version);
    if (currprefs.start_gui == 1)
	gtk_widget_show (lab_info);

    add_empty_vbox (dvbox);
}

static void create_guidlg (void)
{
    GtkWidget *window;
    GtkWidget *buttonbox, *vbox, *hbox;
    GtkWidget *thing;
    unsigned int i;
    int argc = 1;
    char *a[] = {"UAE"};
    char **argv = a;
    static const struct _pages {
	const char *title;
	void (*createfunc)(GtkWidget *);
    } pages[] = {
	/* ??? If this isn't the first page, there are errors in draw_led.  */
	{ "Floppies", make_floppy_disks },
	{ "ROM", make_rom_widgets },
	{ "RAM", make_ram_widgets },
	{ "CPU", make_cpu_widgets },
	{ "Graphics", make_gfx_widgets },
	{ "Chipset", make_chipset_widgets },
	{ "Sound", make_sound_widgets },
	{ "Input devices", make_joy_widgets },
	{ "Harddisks", make_hd_widgets },
	{ "Presets", make_presets },
	{ "About", make_about_widgets }
    };

    gtk_init (&argc, &argv);
    gtk_rc_parse ("uaegtkrc");

    gui_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title (GTK_WINDOW (gui_window), "UAE control");

    gtk_signal_connect (GTK_OBJECT (gui_window), "delete_event",
			GTK_SIGNAL_FUNC (gui_delete_event), NULL);

    vbox = gtk_vbox_new (FALSE, 4);
    gtk_container_add (GTK_CONTAINER (gui_window), vbox);
    gtk_container_set_border_width (GTK_CONTAINER (gui_window), 10);

    /* First line - buttons and power LED */
    hbox = gtk_hbox_new (FALSE, 10);
    gtk_widget_show (hbox);
    gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, TRUE, 0);

    /* The buttons */
    buttonbox = gtk_hbox_new (TRUE, 4);
    gtk_widget_show (buttonbox);
    gtk_box_pack_start (GTK_BOX (hbox), buttonbox, TRUE, TRUE, 0);
    make_button ("Reset", buttonbox, did_reset);
    make_button ("Debug", buttonbox, did_debug);
    make_button ("Quit", buttonbox, did_quit);
    make_button ("Save config", buttonbox, save_config);
    pause_uae_widget = make_buttons ("Pause", buttonbox, pause_uae, gtk_toggle_button_new_with_label);

    /* The LED */
    thing = make_led (0);
    thing = make_labelled_widget ("Power:", thing, FALSE);
    gtk_widget_show (thing);
    gtk_box_pack_start (GTK_BOX (hbox), thing, FALSE, TRUE, 0);

    /* More buttons */
    buttonbox = gtk_hbox_new (TRUE, 4);
    gtk_widget_show (buttonbox);
    gtk_box_pack_start (GTK_BOX (vbox), buttonbox, TRUE, TRUE, 0);
    snap_save_widget = make_button ("Save state", buttonbox, did_savestate);
    snap_load_widget = make_button ("Load state", buttonbox, did_loadstate);

    /* Place a separator below those buttons.  */
    thing = gtk_hseparator_new ();
    gtk_box_pack_start (GTK_BOX (vbox), thing, FALSE, TRUE, 0);
    gtk_widget_show (thing);

    /* Now the notebook */
    notebook = gtk_notebook_new ();
    gtk_box_pack_start (GTK_BOX (vbox), notebook, TRUE, TRUE, 0);
    gtk_widget_show (notebook);

    for (i = 0; i < sizeof pages / sizeof (struct _pages); i++) {
	thing = gtk_vbox_new (FALSE, 4);
	gtk_widget_show (thing);
	gtk_container_set_border_width (GTK_CONTAINER (thing), 10);
	pages[i].createfunc (thing);
	gtk_notebook_append_page (GTK_NOTEBOOK (notebook), thing, gtk_label_new (pages[i].title));
    }

    /* Put "about" screen first.  */
    gtk_notebook_set_page (GTK_NOTEBOOK (notebook), i - 1);
    enable_disk_buttons (1);

    gtk_widget_show (vbox);

    gtk_timeout_add (1000, (GtkFunction)my_idle, 0);
}

static void *gtk_gui_thread (void *dummy)
{
    create_guidlg ();
    gtk_main ();

    gui_available = 0;
    uae_sem_post (&gui_quit_sem);
    return 0;
}

void gui_fps (int x)
{
}

void gui_display (int shortcut)
{
    if (shortcut == -1 && !gui_active) {
	gui_init (0);
    }
}

void gui_led (int num, int on)
{
    if (no_gui)
	return;

/*    if (num == 0)
	return;
    printf("LED %d %d\n", num, on);
    write_comm_pipe_int (&to_gui_pipe, 1, 0);
    write_comm_pipe_int (&to_gui_pipe, num == 0 ? 4 : num - 1, 0);
    write_comm_pipe_int (&to_gui_pipe, on, 1);
    printf("#LED %d %d\n", num, on);*/
}

void gui_filename (int num, const char *name)
{
    if (no_gui)
	return;

    write_comm_pipe_int (&to_gui_pipe, GUICMD_DISKCHANGE, 0);
    write_comm_pipe_int (&to_gui_pipe, num, 1);

/*    gui_update ();*/
}

void gui_handle_events (void)
{
    int pause_uae = FALSE;

    if (no_gui)
	return;

    do {
	while (pause_uae || comm_pipe_has_data (&from_gui_pipe)) {
	    int cmd = read_comm_pipe_int_blocking (&from_gui_pipe);
	    int n;
	    switch (cmd) {
	    case 0:
		n = read_comm_pipe_int_blocking (&from_gui_pipe);
		changed_prefs.df[n][0] = '\0';
		break;
	    case 1:
		n = read_comm_pipe_int_blocking (&from_gui_pipe);
		uae_sem_wait (&gui_sem);
		strncpy (changed_prefs.df[n], new_disk_string[n], 255);
		free (new_disk_string[n]);
		new_disk_string[n] = 0;
		changed_prefs.df[n][255] = '\0';
		uae_sem_post (&gui_sem);
		break;
	    case 2:
		uae_reset (1);
		end_pause_uae ();
		break;
	    case 3:
		activate_debugger ();
		end_pause_uae ();
		break;
	    case 4:
		uae_quit ();
		end_pause_uae ();
		break;
	    case 5:
		pause_uae = TRUE;
		break;
	    case 6:
		pause_uae = FALSE;
		break;
	    case 7:
		printf ("STATESAVE\n");
		savestate_state = read_comm_pipe_int_blocking (&from_gui_pipe);
		uae_sem_wait (&gui_sem);
		savestate_filename = gui_snapname;
		uae_sem_post (&gui_sem);
		break;
	    case 8:
		uae_sem_wait (&gui_sem);
		strncpy (changed_prefs.romfile, gui_romname, 255);
		changed_prefs.romfile[255] = '\0';
		free (gui_romname);
		uae_sem_post (&gui_sem);
		break;
	    case 9:
		uae_sem_wait (&gui_sem);
		strncpy (changed_prefs.keyfile, gui_keyname, 255);
		changed_prefs.keyfile[255] = '\0';
		free (gui_keyname);
		uae_sem_post (&gui_sem);
		break;
	    case 10:
		uae_sem_wait (&gui_sem);
		strncpy (changed_prefs.path_rom, gui_romname, 255);
		changed_prefs.path_rom[255] = '\0';
		free (gui_romname);
		uae_sem_post (&gui_sem);
		break;
	    case 11:
		uae_sem_wait (&gui_sem);
		changed_prefs.gfx_w = gfx_w;
		changed_prefs.gfx_f = gfx_f;
		uae_sem_post (&gui_sem);
		break;
	    }
	}
    } while (pause_uae);
}

void gui_update_gfx (void)
{
    if (no_gui)
	return;

    write_comm_pipe_int (&to_gui_pipe, GUICMD_FULLSCREEN, 1);
}

int gui_update (void)
{
    if (!gui_available)
	return 0;

    write_comm_pipe_int (&to_gui_pipe, GUICMD_UPDATE, 1);
    uae_sem_wait (&gui_init_sem);
    return 0;
}

static void gui_show (void)
{
    if (!gui_available)
	return;

    write_comm_pipe_int (&to_gui_pipe, GUICMD_SHOW, 1);
    uae_sem_wait (&gui_init_sem);
    return;
}

void gui_romlist_changed (void)
{
    if (!gui_available)
	return;

    write_comm_pipe_int (&to_gui_pipe, GUICMD_NEW_ROMLIST, 1);
    return;
}

void gui_exit (void)
{
    if (!gui_available)
	return;

    quit_gui = 1;
    uae_sem_wait (&gui_quit_sem);
}

void gui_lock (void)
{
    if (!gui_available)
	return;
    uae_sem_wait (&gui_sem);
}

void gui_unlock (void)
{
    if (!gui_available)
	return;

    uae_sem_post (&gui_sem);
}

int gui_init (int at_start)
{
    uae_thread_id tid;

    if (gui_available) {
	if (!gui_active) {
	    gui_update ();
	}
	return 0;
    }

    gui_available = 1;
    if (at_start) {
	init_comm_pipe (&to_gui_pipe, 20, 1);
	init_comm_pipe (&from_gui_pipe, 20, 1);
	uae_sem_init (&gui_sem, 0, 1);
	uae_sem_init (&gui_init_sem, 0, 0);
	uae_sem_init (&gui_quit_sem, 0, 0);

	uae_start_thread (gtk_gui_thread, NULL, &tid);
    }
    gui_update ();

    if (at_start && currprefs.start_gui == 1) {
	write_comm_pipe_int (&to_gui_pipe, GUICMD_PAUSE, 1);
	write_comm_pipe_int (&from_gui_pipe, 5, 1);
	/* Handle events until Pause is unchecked.  */
	gui_handle_events ();
	/* Quit requested?  */
	if (quit_program == -1) {
	    gui_exit ();
	    return -2;
	}
    }

    return 1;
}

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

#include "config.h"
#include "options.h"
#include "uae.h"
#include "memory.h"
#include "custom.h"
#include "readcpu.h"
#include "gui.h"
#include "newcpu.h"
#include "threaddep/penguin.h"
#include "sounddep/sound.h"

#include <gtk/gtk.h>

/* One of the 1.1.6 "features" is a gratuitous name change */
#ifndef HAVE_GTK_FEATURES_1_1_6
#define gtk_container_set_border_width gtk_container_border_width
#endif
/* Likewise for 1.1.8.  */
#ifndef HAVE_GTK_FEATURES_1_1_8
#define gtk_label_set_text gtk_label_set
#endif
/* This is beginning to suck... */
#ifndef HAVE_GTK_FEATURES_1_1_13
#define gtk_toggle_button_set_active gtk_toggle_button_set_state
#endif

static int gui_active;

static GtkWidget *pause_uae_widget;

static GtkWidget *disk_insert_widget[4], *disk_eject_widget[4], *disk_text_widget[4];
static char *new_disk_string[4];

static GtkAdjustment *cpuspeed_adj;
static GtkWidget *cpuspeed_widgets[4], *cpuspeed_scale;
static GtkWidget *cpu_widget[4], *a24m_widget, *ccpu_widget;
static GtkWidget *sound_widget[4], *sound_bits_widget[2], *sound_freq_widget[3], *sound_ch_widget[2];

static GtkAdjustment *framerate_adj;
static GtkWidget *bimm_widget, *b32_widget;

static GtkWidget *joy_widget[2][6];

static GtkWidget *led_widgets[5];
static GdkColor led_on[5], led_off[5];
static unsigned int prevledstate;

static smp_comm_pipe to_gui_pipe, from_gui_pipe;
static uae_sem_t gui_sem, gui_quit_sem; /* gui_sem protects the DFx fields */

static volatile int quit_gui = 0, quitted_gui = 0;

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
	fprintf (stderr, "Error saving options file!\n");
	return;
    }
    save_options (f, &currprefs);
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
	gtk_widget_set_sensitive (disk_insert_widget[i], enable);
	gtk_widget_set_sensitive (disk_eject_widget[i], enable);
    }
}

static void set_cpu_state (void)
{
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (a24m_widget), changed_prefs.address_space_24 != 0);
    gtk_widget_set_sensitive (a24m_widget, changed_prefs.cpu_level > 1);
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (ccpu_widget), changed_prefs.cpu_compatible != 0);
    gtk_widget_set_sensitive (ccpu_widget, changed_prefs.cpu_level == 0);
    gtk_widget_set_sensitive (cpuspeed_scale, changed_prefs.m68k_speed > 0);
}

static void set_cpu_widget (void)
{
    int nr = changed_prefs.cpu_level;
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (cpu_widget[nr]), TRUE);
    nr = currprefs.m68k_speed + 1 < 3 ? currprefs.m68k_speed + 1 : 2;
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (cpuspeed_widgets[nr]), TRUE);
}

static void set_gfx_state (void)
{
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (bimm_widget), currprefs.immediate_blits != 0);
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (b32_widget), currprefs.blits_32bit_enabled != 0);
}

static void set_sound_state (void)
{
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (sound_widget[currprefs.produce_sound]), 1);
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (sound_ch_widget[currprefs.stereo]), 1);
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (sound_bits_widget[currprefs.sound_bits == 16]), 1);
}

static void set_joy_state (void)
{
    int j0t = changed_prefs.jport0;
    int j1t = changed_prefs.jport1;
    int i;

    if (j0t == j1t) {
	/* Can't happen */
	j0t++;
	j0t %= 6;
    }
    for (i = 0; i < 6; i++) {
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (joy_widget[0][i]), j0t == i);
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (joy_widget[1][i]), j1t == i);
	gtk_widget_set_sensitive (joy_widget[0][i], j1t != i);
	gtk_widget_set_sensitive (joy_widget[1][i], j0t != i);
    }
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

static int my_idle (void)
{
    unsigned int leds = gui_ledstate;
    int i;

    if (quit_gui) {
	/*printf("Foo...\n");*/
	gtk_main_quit ();
	goto out;
    }
    while (comm_pipe_has_data (&to_gui_pipe)) {
	int cmd = read_comm_pipe_int_blocking (&to_gui_pipe);
	int n;
	/*printf ("cmd %d\n", cmd);*/
	switch (cmd) {
	 case 0:
	    n = read_comm_pipe_int_blocking (&to_gui_pipe);
	    gtk_label_set_text (GTK_LABEL (disk_text_widget[n]), currprefs.df[n]);
	    break;
	 case 1:
	    /* Initialization.  */
	    set_cpu_widget ();
	    set_cpu_state ();
	    set_gfx_state ();
	    set_joy_state ();
	    set_sound_state ();

	    gui_active = 1;
	    break;
	}
    }
    
    for (i = 0; i < 5; i++) {
	unsigned int mask = 1 << i;
	unsigned int on = leds & mask;

	if (on == (prevledstate & mask))
	    continue;

/*	printf(": %d %d\n", i, on);*/
	draw_led (i);
    }
    prevledstate = leds;
out:
    return 1;
}

static int find_current_toggle (GtkWidget **widgets, int count)
{
    int i;
    for (i = 0; i < count; i++)
	if (GTK_TOGGLE_BUTTON (*widgets++)->active)
	    return i;
    fprintf (stderr, "GTKUI: Can't happen!\n");
    return -1;
}

static void joy_changed (void)
{
    if (! gui_active)
	return;

    changed_prefs.jport0 = find_current_toggle (joy_widget[0], 6);
    changed_prefs.jport1 = find_current_toggle (joy_widget[1], 6);
    set_joy_state ();
}

static void custom_changed (void)
{
    changed_prefs.gfx_framerate = framerate_adj->value;
    changed_prefs.blits_32bit_enabled = GTK_TOGGLE_BUTTON (b32_widget)->active;
    changed_prefs.immediate_blits = GTK_TOGGLE_BUTTON (bimm_widget)->active;
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
    if (! gui_active)
	return;

    oldcl = changed_prefs.cpu_level;

    changed_prefs.cpu_level = find_current_toggle (cpu_widget, 4);
    changed_prefs.cpu_compatible = GTK_TOGGLE_BUTTON (ccpu_widget)->active;
    changed_prefs.address_space_24 = GTK_TOGGLE_BUTTON (a24m_widget)->active;

    if (changed_prefs.cpu_level != 0)
	changed_prefs.cpu_compatible = 0;
    /* 68000/68010 always have a 24 bit address space.  */
    if (changed_prefs.cpu_level < 2)
	changed_prefs.address_space_24 = 1;
    /* Changing from 68000/68010 to 68020 should set a sane default.  */
    else if (oldcl < 2)
	changed_prefs.address_space_24 = 0;
    
    set_cpu_state ();
}

static void sound_changed (void)
{
    changed_prefs.produce_sound = find_current_toggle (sound_widget, 4);
    changed_prefs.stereo = find_current_toggle (sound_ch_widget, 2);
    changed_prefs.sound_bits = (find_current_toggle (sound_bits_widget, 2) + 1) * 8;
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

static void did_eject (GtkWidget *w, gpointer data)
{
    if (quit_gui)
	return;
    
    write_comm_pipe_int (&from_gui_pipe, 0, 0);
    write_comm_pipe_int (&from_gui_pipe, (int)data, 1);
}

static void pause_uae (GtkWidget *widget, gpointer data)
{
    if (quit_gui)
	return;

    write_comm_pipe_int (&from_gui_pipe, GTK_TOGGLE_BUTTON (widget)->active ? 5 : 6, 1);  
}

static void end_pause_uae (void)
{
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (pause_uae_widget), FALSE);
}
    
static int filesel_active = -1;
static GtkWidget *selector;

static void did_close_insert (GtkObject *o, GtkWidget *w)
{
    filesel_active = -1;
    enable_disk_buttons (1);
    gtk_widget_hide (selector);
}

static void did_cancel_insert (GtkObject *o)
{
    filesel_active = -1;
    enable_disk_buttons (1);
    gtk_widget_hide (selector);
}

static void did_insert_select (GtkObject *o)
{
    char *s = gtk_file_selection_get_filename (GTK_FILE_SELECTION (selector));
    printf ("%s\n", s);
    if (quit_gui)
	return;

    uae_sem_wait (&gui_sem);
    if (new_disk_string[filesel_active] != 0)
	free (new_disk_string[filesel_active]);
    new_disk_string[filesel_active] = strdup (s);
    uae_sem_post (&gui_sem);
    write_comm_pipe_int (&from_gui_pipe, 1, 0);
    write_comm_pipe_int (&from_gui_pipe, filesel_active, 1);
    filesel_active = -1;
    enable_disk_buttons (1);
    gtk_widget_hide (selector);
}

static char fsbuffer[100];

static void did_insert (GtkWidget *w, gpointer data)
{
    int n = (int)data;
    if (filesel_active != -1)
	return;
    filesel_active = n;
    enable_disk_buttons (0);

    sprintf (fsbuffer, "Select a disk image file for DF%d", n);
    gtk_window_set_title (GTK_WINDOW (selector), fsbuffer);

    /*printf("%p\n", selector);*/
    gtk_widget_show (selector);
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

static void add_empty_vbox (GtkWidget *tobox)
{
    GtkWidget *thing = gtk_vbox_new (FALSE, 0);
    gtk_widget_show (thing);
    gtk_box_pack_start (GTK_BOX (tobox), thing, TRUE, TRUE, 0);
}

static void add_empty_hbox (GtkWidget *tobox)
{
    GtkWidget *thing = gtk_hbox_new (FALSE, 0);
    gtk_widget_show (thing);
    gtk_box_pack_start (GTK_BOX (tobox), thing, TRUE, TRUE, 0);
}

static void add_centered_to_vbox (GtkWidget *vbox, GtkWidget *w)
{
    GtkWidget *hbox = gtk_hbox_new (TRUE, 0);
    gtk_widget_show (hbox);
    gtk_box_pack_start (GTK_BOX (hbox), w, TRUE, FALSE, 0);
    gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, TRUE, 0);
}

static GtkWidget *make_labelled_widget (const char *str, GtkWidget *thing)
{
    GtkWidget *label = gtk_label_new (str);
    GtkWidget *hbox2 = gtk_hbox_new (FALSE, 4);

    gtk_widget_show (label);
    gtk_widget_show (thing);

    gtk_box_pack_start (GTK_BOX (hbox2), label, FALSE, TRUE, 0);
    gtk_box_pack_start (GTK_BOX (hbox2), thing, FALSE, TRUE, 0);

    return hbox2;
}

static GtkWidget *add_labelled_widget_centered (const char *str, GtkWidget *thing, GtkWidget *vbox)
{
    GtkWidget *w = make_labelled_widget (str, thing);
    gtk_widget_show (w);
    add_centered_to_vbox (vbox, w);
    return w;
}

static void make_radio_group (const char **labels, GtkWidget *tobox,
			      GtkWidget **saveptr, gint t1, gint t2,
			      void (*sigfunc) (void))
{
    GtkWidget *thing = NULL;

    while (*labels) {
	thing = gtk_radio_button_new_with_label ((thing
						  ? gtk_radio_button_group (GTK_RADIO_BUTTON (thing))
						  : 0),
						 *labels++);
	*saveptr++ = thing;
	gtk_widget_show (thing);
	gtk_box_pack_start (GTK_BOX (tobox), thing, t1, t2, 0);
	gtk_signal_connect (GTK_OBJECT (thing), "clicked", (GtkSignalFunc) sigfunc, NULL);
    }
}

static GtkWidget *make_radio_group_box (const char *title, const char **labels,
					GtkWidget **saveptr, int horiz,
					void (*sigfunc) (void))
{
    GtkWidget *frame, *newbox;

    frame = gtk_frame_new (title);
    newbox = (horiz ? gtk_hbox_new : gtk_vbox_new) (FALSE, 4);
    gtk_widget_show (newbox);
    gtk_container_set_border_width (GTK_CONTAINER (newbox), 4);
    gtk_container_add (GTK_CONTAINER (frame), newbox);
    make_radio_group (labels, newbox, saveptr, horiz, !horiz, sigfunc);
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

static void make_floppy_disks (GtkWidget *vbox)
{
    GtkWidget *thing, *subthing, *subframe, *buttonbox;
    char buf[5];
    int i;

    add_empty_vbox (vbox);

    for (i = 0; i < 4; i++) {
	/* Frame with an hbox and the "DFx:" title */
	sprintf (buf, "DF%d:", i);
	thing = gtk_frame_new (buf);
	buttonbox = gtk_hbox_new (FALSE, 4);
	gtk_container_set_border_width (GTK_CONTAINER (buttonbox), 4);
	gtk_container_add (GTK_CONTAINER (thing), buttonbox);
        gtk_box_pack_start (GTK_BOX (vbox), thing, FALSE, TRUE, 0);
	gtk_widget_show (buttonbox);
	gtk_widget_show (thing);

	/* LED */
	subthing = make_led (i + 1);
	gtk_box_pack_start (GTK_BOX (buttonbox), subthing, FALSE, TRUE, 0);

	/* Current file display */
	subframe = gtk_frame_new (NULL);
	gtk_frame_set_shadow_type (GTK_FRAME (subframe), GTK_SHADOW_ETCHED_OUT);
	gtk_box_pack_start (GTK_BOX (buttonbox), subframe, TRUE, TRUE, 0);
	gtk_widget_show (subframe);
	subthing = gtk_vbox_new (FALSE, 0);
	gtk_widget_show (subthing);
	gtk_container_add (GTK_CONTAINER (subframe), subthing);
	thing = gtk_label_new ("");
	disk_text_widget[i] = thing;
	gtk_widget_show (thing);
	gtk_box_pack_start (GTK_BOX (subthing), thing, TRUE, TRUE, 0);

	/* Now, the buttons.  */
	thing = gtk_button_new_with_label ("Eject");
	gtk_box_pack_start (GTK_BOX (buttonbox), thing, FALSE, TRUE, 0);
	gtk_widget_show (thing);
	disk_eject_widget[i] = thing;
	gtk_signal_connect (GTK_OBJECT (thing), "clicked", (GtkSignalFunc) did_eject, (gpointer) i);

	thing = gtk_button_new_with_label ("Insert");
	gtk_box_pack_start (GTK_BOX (buttonbox), thing, FALSE, TRUE, 0);
	gtk_widget_show (thing);
	disk_insert_widget[i] = thing;
	gtk_signal_connect (GTK_OBJECT (thing), "clicked", (GtkSignalFunc) did_insert, (gpointer) i);
    }

    add_empty_vbox (vbox);
}

static GtkWidget *make_cpu_speed_sel (void)
{
    static const char *labels[] = {
	"Optimize for host CPU speed","Approximate 68000/7MHz speed", "Adjustable",
	NULL
    };
    GtkWidget *frame, *newbox;

    frame = gtk_frame_new ("CPU speed");
    newbox = gtk_vbox_new (FALSE, 4);
    gtk_widget_show (newbox);
    gtk_container_set_border_width (GTK_CONTAINER (newbox), 4);
    gtk_container_add (GTK_CONTAINER (frame), newbox);
    make_radio_group (labels, newbox, cpuspeed_widgets, 0, 1, cpuspeed_changed);

    cpuspeed_adj = GTK_ADJUSTMENT (gtk_adjustment_new (currprefs.m68k_speed, 1.0, 10.0, 1.0, 1.0, 1.0));
    gtk_signal_connect (GTK_OBJECT (cpuspeed_adj), "value_changed",
			GTK_SIGNAL_FUNC (cpuspeed_changed), NULL);

    cpuspeed_scale = gtk_hscale_new (cpuspeed_adj);
    gtk_range_set_update_policy (GTK_RANGE (cpuspeed_scale), GTK_UPDATE_DELAYED);
    gtk_scale_set_digits (GTK_SCALE (cpuspeed_scale), 0);
    gtk_scale_set_value_pos (GTK_SCALE (cpuspeed_scale), GTK_POS_RIGHT);
    cpuspeed_scale = add_labelled_widget_centered ("Cycles per instruction:", cpuspeed_scale, newbox);

    return frame;
}

static void make_cpu_widgets (GtkWidget *vbox)
{
    int i;
    GtkWidget *newbox, *hbox, *frame;
    GtkWidget *thing;
    static const char *radiolabels[] = {
	"68000", "68010", "68020", "68020+68881",
	NULL
    };

    add_empty_vbox (vbox);

    hbox = gtk_hbox_new (FALSE, 0);
    add_empty_vbox (hbox);

    newbox = make_radio_group_box ("CPU type", radiolabels, cpu_widget, 0, cputype_changed);
    gtk_widget_show (newbox);
    gtk_box_pack_start (GTK_BOX (hbox), newbox, FALSE, FALSE, 0);

    newbox = make_cpu_speed_sel ();
    gtk_widget_show (newbox);
    gtk_box_pack_start (GTK_BOX (hbox), newbox, FALSE, FALSE, 0);

    add_empty_vbox (hbox);
    gtk_widget_show (hbox); 
    gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);

    frame = gtk_frame_new ("CPU flags");
    add_centered_to_vbox (vbox, frame);
    gtk_widget_show (frame);
    newbox = gtk_vbox_new (FALSE, 4);
    gtk_widget_show (newbox);
    gtk_container_set_border_width (GTK_CONTAINER (newbox), 4);
    gtk_container_add (GTK_CONTAINER (frame), newbox);

    a24m_widget = gtk_check_button_new_with_label ("24 bit address space");
    add_centered_to_vbox (newbox, a24m_widget);
    gtk_widget_show (a24m_widget);
    ccpu_widget = gtk_check_button_new_with_label ("Slow but compatible");
    add_centered_to_vbox (newbox, ccpu_widget);
    gtk_widget_show (ccpu_widget);
    
    add_empty_vbox (vbox);

    gtk_signal_connect (GTK_OBJECT (ccpu_widget), "clicked",
			(GtkSignalFunc) cputype_changed, NULL);
    gtk_signal_connect (GTK_OBJECT (a24m_widget), "clicked",
			(GtkSignalFunc) cputype_changed, NULL);    
}

static void make_gfx_widgets (GtkWidget *vbox)
{
    GtkWidget *thing;

    add_empty_vbox (vbox);

    framerate_adj = GTK_ADJUSTMENT (gtk_adjustment_new (currprefs.gfx_framerate, 1.0, 21.0, 1.0, 1.0, 1.0));
    gtk_signal_connect (GTK_OBJECT (framerate_adj), "value_changed",
			GTK_SIGNAL_FUNC (custom_changed), NULL);

    thing = gtk_hscale_new (framerate_adj);
    gtk_range_set_update_policy (GTK_RANGE (thing), GTK_UPDATE_DELAYED);
    gtk_scale_set_digits (GTK_SCALE (thing), 0);
    gtk_scale_set_value_pos (GTK_SCALE (thing), GTK_POS_RIGHT);
    add_labelled_widget_centered ("Framerate:", thing, vbox);

    b32_widget = gtk_check_button_new_with_label ("32 bit blitter");
    add_centered_to_vbox (vbox, b32_widget);
    gtk_widget_show (b32_widget);

    bimm_widget = gtk_check_button_new_with_label ("Immediate blits");
    add_centered_to_vbox (vbox, bimm_widget);
    gtk_widget_show (bimm_widget);

    add_empty_vbox (vbox);

    gtk_signal_connect (GTK_OBJECT (bimm_widget), "clicked",
			(GtkSignalFunc) custom_changed, NULL);
    gtk_signal_connect (GTK_OBJECT (b32_widget), "clicked",
			(GtkSignalFunc) custom_changed, NULL);
}

static void make_sound_widgets (GtkWidget *vbox)
{
    GtkWidget *frame, *newbox;
    int i;
    GtkWidget *hbox;
    static const char *soundlabels1[] = {
	"None", "No output", "Normal", "Accurate",
	NULL
    }, *soundlabels2[] = {
	"8 bit", "16 bit",
	NULL
    }, *soundlabels3[] = {
	"Mono", "Stereo",
	NULL
    };

    add_empty_vbox (vbox);

    newbox = make_radio_group_box ("Mode", soundlabels1, sound_widget, 1, sound_changed);
    gtk_widget_show (newbox);
    add_centered_to_vbox (vbox, newbox);

    hbox = gtk_hbox_new (FALSE, 10);
    gtk_widget_show (hbox);
    add_centered_to_vbox (vbox, hbox);
    newbox = make_radio_group_box ("Channels", soundlabels3, sound_ch_widget, 1, sound_changed);
    gtk_widget_show (newbox);
    gtk_box_pack_start (GTK_BOX (hbox), newbox, FALSE, TRUE, 0);
    newbox = make_radio_group_box ("Resolution", soundlabels2, sound_bits_widget, 1, sound_changed);
    gtk_widget_show (newbox);
    gtk_box_pack_start (GTK_BOX (hbox), newbox, FALSE, TRUE, 0);

    add_empty_vbox (vbox);
}

static void make_joy_widgets (GtkWidget *dvbox)
{
    int i;
    GtkWidget *hbox = gtk_hbox_new (FALSE, 10);
    static const char *joylabels[] = {
	"Joystick 0", "Joystick 1", "Mouse", "Numeric pad",
	"Cursor keys/Right Ctrl", "T/F/H/B/Left Alt",
	NULL
    };

    add_empty_vbox (dvbox);
    gtk_widget_show (hbox);
    add_centered_to_vbox (dvbox, hbox);

    for (i = 0; i < 2; i++) {
	GtkWidget *vbox, *frame;
	GtkWidget *thing;
	char buffer[20];
	int j;

	sprintf (buffer, "Port %d", i);
	frame = make_radio_group_box (buffer, joylabels, joy_widget[i], 0, joy_changed);
	gtk_widget_show (frame);
	gtk_box_pack_start (GTK_BOX (hbox), frame, FALSE, TRUE, 0);
    }

    add_empty_vbox (dvbox);
}

static GtkWidget *make_buttons (const char *label, GtkWidget *box, void (*sigfunc) (void), GtkWidget *(*create)(const char *label))
{
    GtkWidget *thing = create (label);
    gtk_widget_show (thing);
    gtk_signal_connect (GTK_OBJECT (thing), "clicked", (GtkSignalFunc) sigfunc, NULL);
    gtk_box_pack_start (GTK_BOX (box), thing, TRUE, TRUE, 0);

    return thing;
}
#define make_button(label, box, sigfunc) make_buttons(label, box, sigfunc, gtk_button_new_with_label)

static void *gtk_penguin (void *dummy)
{
    GtkWidget *window, *notebook;
    GtkWidget *buttonbox, *vbox, *hbox;
    GtkWidget *thing;
    int i;
    int argc = 1;
    char *a[] = {"UAE"};
    char **argv = a;
    static const struct _pages {
	const char *title;
	void (*createfunc)(GtkWidget *);
    } pages[] = {
	{ "Floppy disks", make_floppy_disks },
	{ "CPU emulation", make_cpu_widgets },
	{ "Graphics", make_gfx_widgets },
	{ "Sound", make_sound_widgets },
	{ "Game ports", make_joy_widgets }
    };

    gtk_init (&argc, &argv);
    gtk_rc_parse ("uaegtkrc");

    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title (GTK_WINDOW (window), "UAE control");

    vbox = gtk_vbox_new (FALSE, 4);
    gtk_container_add (GTK_CONTAINER (window), vbox);
    gtk_container_set_border_width (GTK_CONTAINER (window), 10);

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
    thing = make_labelled_widget ("Power:", thing);
    gtk_widget_show (thing);
    gtk_box_pack_start (GTK_BOX (hbox), thing, FALSE, TRUE, 0);

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

    enable_disk_buttons (1);

    gtk_widget_show (vbox);
    gtk_widget_show (window);

    /* We're going to need that later.  */
    selector = gtk_file_selection_new ("");
    gtk_signal_connect (GTK_OBJECT (selector), "destroy",
			(GtkSignalFunc) did_close_insert, selector);

    gtk_signal_connect_object (GTK_OBJECT (GTK_FILE_SELECTION (selector)->ok_button),
			       "clicked", (GtkSignalFunc) did_insert_select,
			       GTK_OBJECT (selector));
    gtk_signal_connect_object (GTK_OBJECT (GTK_FILE_SELECTION (selector)->cancel_button),
			       "clicked", (GtkSignalFunc) did_cancel_insert,
			       GTK_OBJECT (selector));
    filesel_active = -1;

    gtk_timeout_add (1000, (GtkFunction)my_idle, 0);
    gtk_main ();
    
    quitted_gui = 1;
    uae_sem_post (&gui_quit_sem);
    return 0;
}

void gui_changesettings(void)
{
    
}

int gui_init (void)
{
    penguin_id tid;

    gui_active = 0;
    
    init_comm_pipe (&to_gui_pipe, 20, 1);
    init_comm_pipe (&from_gui_pipe, 20, 1);
    uae_sem_init (&gui_sem, 0, 1);
    uae_sem_init (&gui_quit_sem, 0, 0);
    start_penguin (gtk_penguin, NULL, &tid);
    return 1;
}

int gui_update (void)
{
    if (no_gui)
	return 0;

    write_comm_pipe_int (&to_gui_pipe, 1, 1);    
    return 0;
}

void gui_exit (void)
{
    if (no_gui)
	return;

    quit_gui = 1;
    uae_sem_wait (&gui_quit_sem);
}

void gui_fps (int x)
{
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

void gui_filename(int num, const char *name)
{
    if (no_gui)
	return;

    write_comm_pipe_int (&to_gui_pipe, 0, 0);
    write_comm_pipe_int (&to_gui_pipe, num, 1);

/*    gui_update ();*/
}

void gui_handle_events(void)
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
                changed_prefs.df[n][255] = '\0';
                uae_sem_post (&gui_sem);
                break;
             case 2:
                uae_reset ();
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
            }
        }
    } while (pause_uae);
}

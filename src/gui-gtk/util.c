/*
 * util.c
 *
 * Copyright 2003-2004 Richard Drummond
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>

#include "util.h"
#include "chooserwidget.h"

/*
 * Some utility functions to make building a GTK+ GUI easier
 * and more compact, to hide differences between GTK1.x and GTK2.x
 * and to help maintain consistency
 */

/*
 * Create a list of signals and add them to a GTK+ class
 */
void gtkutil_add_signals_to_class (GtkObjectClass *class, guint func_offset, guint *signals, ...)
{
   va_list signames;
   const char *name;
   unsigned int count = 0;

   va_start (signames, signals);
   name = va_arg (signames, const char *);

   while (name) {
#if GTK_MAJOR_VERSION >= 2
	signals[count] = g_signal_new (name, G_TYPE_FROM_CLASS (class), G_SIGNAL_RUN_FIRST | G_SIGNAL_ACTION,
				func_offset, NULL, NULL, g_cclosure_marshal_VOID__VOID, G_TYPE_NONE, 0);
#else
	signals[count] = gtk_signal_new (name, GTK_RUN_FIRST, class->type, func_offset,
					gtk_signal_default_marshaller, GTK_TYPE_NONE, 0);
#endif
	count++;
	name = va_arg (signames, const char *);
   };

#if GTK_MAJOR_VERSION < 2
    gtk_object_class_add_signals (class, signals, count);
#endif
}


GtkWidget *make_chooser (int count, ...)
{
    GtkWidget *chooser;
    va_list   choices;
    int       i;

    chooser = gtk_combo_box_new_text ();

    va_start (choices, count);
    for (i = 0; i < count; i++)
	gtk_combo_box_append_text (GTK_COMBO_BOX (chooser), va_arg (choices, char *));

    gtk_widget_show (chooser);
    return chooser;
}

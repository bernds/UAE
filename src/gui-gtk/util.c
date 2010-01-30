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

/*
 * Some utility functions to make building a GTK+ GUI easier
 * and more compact, to hide differences between GTK1.x and GTK2.x
 * and to help maintain consistency
 */

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

/*
 * Add some padding to a vbox or hbox which will consume
 * space when the box is resized to larger than default size
 */
void add_box_padding (GtkWidget *box)
{
    GtkWidget *vbox;

    vbox = gtk_vbox_new (FALSE, 0);
    gtk_widget_show (vbox);
    gtk_box_pack_start (GTK_BOX (box), vbox, TRUE, TRUE, 0);
}

/*
 * Add some padding to a table which will consime space when
 * when the table is resized to larger than default size
 */
void add_table_padding (GtkWidget *table, int x, int y)
{
    GtkWidget *vbox;
    vbox = gtk_vbox_new (FALSE, 0);
    gtk_widget_show (vbox);
    gtk_table_attach (GTK_TABLE (table), vbox, x, x+1, y, y+1,
		     (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
		     (GtkAttachOptions) (GTK_EXPAND | GTK_FILL), 0, 0);
}

/*
 * Add a widget to a table with some sensible defaults
 *
 * <x,y> is the table position to insert the widget
 * <width> is the the number of columns the widget will take up
 * <xflags> are the attachment flags that will apply horizontally
 */
void add_to_table(GtkWidget *table, GtkWidget *widget, int x, int y, int width, int xflags)
{
  gtk_table_attach (GTK_TABLE (table), widget, x, x+width, y, y+1,
		   (GtkAttachOptions) (xflags),
		   (GtkAttachOptions) (0), 0, 0);
}


/*
 * Super-duper, handy table creation tool!
 *
 * Creates a table and add a list of widgets to it.
 */
GtkWidget *gtkutil_add_table (GtkWidget *container, ...)
{
    va_list contents;
    GtkWidget *widget;
    GtkWidget *table;
    int row, max_col;
    int col, width;
    int flags;

    table = gtk_table_new (3, 3, FALSE);
    gtk_container_set_border_width (GTK_CONTAINER (table), TABLE_BORDER_WIDTH);
    gtk_table_set_row_spacings (GTK_TABLE (table), TABLE_ROW_SPACING);
    gtk_table_set_col_spacings (GTK_TABLE (table), TABLE_COL_SPACING);
    gtk_container_add (GTK_CONTAINER (container), table);

    va_start (contents, container);
    widget = va_arg (contents, GtkWidget *);
    row = 1;
    max_col = 1;

    while (widget != GTKUTIL_TABLE_END) {
	if (widget == GTKUTIL_ROW_END) {
	    row += 2;
	} else {
	    col = va_arg (contents, gint);
	    if (col > max_col) max_col = col;
	    width = va_arg (contents, gint);
	    flags = va_arg (contents, gint);

	    gtk_table_attach (GTK_TABLE (table), widget, col, col+width, row, row+1,
			      (GtkAttachOptions) (flags), (GtkAttachOptions) (0), 0, 0);
	}
	widget = va_arg (contents, GtkWidget *);
    }

    gtk_table_resize (GTK_TABLE (table), row, max_col + 2);
    add_table_padding (table, 0, 0);
    add_table_padding (table, max_col + 1, row - 1);

    gtk_widget_show_all (table);

    return table;
}

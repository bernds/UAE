/*
 * util.h
 *
 * Copyright 2003-2004 Richard Drummond
 */

/*
 * Default spacing of widgets used in the
 * utility functions below
 */
#define DEFAULT_BORDER_WIDTH  8
#define PANEL_BORDER_WIDTH    DEFAULT_BORDER_WIDTH
#define TABLE_BORDER_WIDTH    DEFAULT_BORDER_WIDTH
#define TABLE_COL_SPACING     8
#define TABLE_ROW_SPACING     4

#define GTKUTIL_TABLE_END	((GtkWidget *)0)
#define GTKUTIL_ROW_END		((GtkWidget *)1)

extern GtkWidget *make_chooser (int count, ...);
extern GtkWidget *gtkutil_add_table (GtkWidget *container, ...);

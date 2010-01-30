/*
 * util.h
 *
 * Copyright 2003-2004 Richard Drummond
 */

void gtkutil_add_signals_to_class (GtkObjectClass *class, guint func_offset, guint *signals, ...);

extern GtkWidget *make_chooser (int count, ...);

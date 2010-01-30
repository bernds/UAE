 /*
  * UAE - The Un*x Amiga Emulator
  *
  * GUI interface (to be done).
  * Calls AREXX interface.
  *
  * Copyright 1996 Bernd Schmidt, Samuel Devulder
  */

#include "sysconfig.h"
#include "sysdeps.h"

#include "config.h"
#include "options.h"
#include "gui.h"

/****************************************************************************/

extern void rexx_led(int led, int on);          /* ami-rexx.c */
extern void rexx_filename(int num, const char *name);
extern void rexx_handle_events(void);
extern void main_window_led(int led, int on);   /* ami-win.c */

/****************************************************************************/

int gui_init(void)
{
    return 0;
}

/****************************************************************************/

void gui_exit(void)
{
}

/****************************************************************************/

int gui_update(void)
{
 return 0;
}

/****************************************************************************/

void gui_led(int led, int on)
{
    main_window_led(led, on);
    rexx_led(led, on);
}

/****************************************************************************/

void gui_filename(int num, const char *name)
{
    rexx_filename(num, name);
}

/****************************************************************************/

void gui_handle_events(void)
{
    rexx_handle_events();
}

/****************************************************************************/


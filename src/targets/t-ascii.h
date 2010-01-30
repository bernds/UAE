 /*
  * UAE - The Un*x Amiga Emulator
  *
  * Target specific stuff, ncurses version
  *
  * Copyright 1997 Bernd Schmidt
  */

#define TARGET_NAME "curses"

#define OPTIONSFILENAME ".uaerc"
#define OPTIONS_IN_HOME

#define DEFPRTNAME "lpr"
#define DEFSERNAME "/dev/ttyS1"

#define TARGET_SPECIAL_OPTIONS \
    { "x",	  "  -x           : Reverse video (white on black)\n" },

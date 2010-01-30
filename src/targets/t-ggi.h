 /*
  * UAE - The Un*x Amiga Emulator
  *
  * Target specific stuff, GGI version
  *
  * Copyright 1997 Stefan Reinauer
  */

#define UNSUPPORTED_OPTION_l

#ifndef USING_CURSES_UI
#define UNSUPPORTED_OPTION_G
#endif

#define OPTIONSFILENAME ".uaerc"
#define OPTIONS_IN_HOME

#define DEFPRTNAME "lpr"
#define DEFSERNAME "/dev/ttyS1"

#define PICASSO96_SUPPORTED

#define write_log write_log_standard

 /*
  * UAE - The Un*x Amiga Emulator
  *
  * Target specific stuff, DOS version
  *
  * Copyright 1997 Bernd Schmidt, Gustavo Goedert
  */

#define TARGET_SPECIAL_OPTIONS \
    { "x",	  "  -x           : Don't use linear frame buffer even if it is available\n" }, \

#define UNSUPPORTED_OPTION_l

#define OPTIONSFILENAME "uae.rc"

#define DEFPRTNAME "PRN:"
#define DEFSERNAME "AUX:"

#define PICASSO96_SUPPORTED

#define write_log write_log_standard

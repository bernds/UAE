 /*
  * UAE - The Un*x Amiga Emulator
  *
  * Target specific stuff, BeOS version
  *
  * Copyright 1997 Bernd Schmidt
  */

#define TARGET_NAME "beos"

#define OPTIONSFILENAME "~/config/settings/uaerc"

#define DEFPRTNAME "lpr"
#define DEFSERNAME "/dev/ports/serial1"

#define NO_MAIN_IN_MAIN_C

#define write_log write_log_standard

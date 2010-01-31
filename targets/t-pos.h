 /*
  * UAE - The Un*x Amiga Emulator
  *
  * Target specific stuff, p.OS version
  *
  * Copyright 1997 Bernd Schmidt
  */

#define UNSUPPORTED_OPTION_l

#define OPTIONSFILENAME ".uaerc"
#define OPTIONS_IN_HOME

#define TARGET_SPECIAL_OPTIONS \
    { "L",        "  -L           : Only update needed part of the window\n" },
#define COLOR_MODE_HELP_STRING \
    "Valid color modes (see -H) are:\n" \
    "     0 => 8 bit output, gray  (no dithering);\n"\
    "     1 => 8 bit output, color (no dithering);\n"\
    "     2 => 8 bit output, gray  (dithering);\n"\
    "     3 => truecolor output.\n\n"

#define DEFSERNAME "ser:"
#define DEFPRTNAME "par:"

#define write_log write_log_standard

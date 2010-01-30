 /*
  * E-UAE - The portable Amiga Emulator
  *
  * Support for traps
  *
  * Copyright Richard Drummond 2005
  *
  * Based on code:
  * Copyright 1995, 1996 Bernd Schmidt
  * Copyright 1996 Ed Hanway
  */

#ifndef TRAPS_H
#define TRAPS_H

#define TRAPFLAG_NO_REGSAVE  1
#define TRAPFLAG_NO_RETVAL   2
#define TRAPFLAG_EXTRA_STACK 4
#define TRAPFLAG_DORET       8

/*
 * A function which handles a 68k trap
 */
typedef uae_u32 (*TrapFunction) (void);

extern int deftrap2 (TrapFunction func, int mode, const char *str);
extern int deftrap (TrapFunction);

extern uae_u32 CallLib (uaecptr base, uae_s16 offset);
extern void call_calltrap (int nr) REGPARAM;

extern uaecptr libemu_InstallFunction (TrapFunction, uaecptr, int, const char *);
extern uaecptr libemu_InstallFunctionFlags (TrapFunction, uaecptr, int, int, const char *);

#endif

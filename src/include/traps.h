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
#define TRACE_TRAPS
#ifndef TRAPS_H
#define TRAPS_H

/*
 * Data passed to a trap handler.  Opaque to callees, but can be passed
 * to CallLib.
 */
typedef struct TrapContext TrapContext;

#define TRAPFLAG_NO_REGSAVE  1
#define TRAPFLAG_NO_RETVAL   2
#define TRAPFLAG_EXTRA_STACK 4
#define TRAPFLAG_DORET       8

/*
 * A function which handles a 68k trap
 */
typedef uae_u32 (*TrapHandler) (TrapContext *);


/*
 * Interface with 68k interpreter
 */
extern void m68k_handle_trap (unsigned int trap_num);

unsigned int define_trap (TrapHandler handler_func, int flags, const char *name);

/*
 * Call a 68k Library function from an extended trap
 */
extern uae_u32 CallLib (TrapContext *context, uaecptr library_base, uae_s16 func_offset);

/*
 * Initialization
 */
void init_traps (void);
void init_extended_traps (void);

#define deftrap(f) define_trap((f), 0, "")
#ifdef TRACE_TRAPS
# define deftrap2(f, mode, str) define_trap((f), (mode), (str))
#else
# define deftrap2(f, mode, str) define_trap((f), (mode), "")
#endif

#endif

/* Declare here so we don't have to pull in traps.h from everywhere that wants
 * inputdevice.h.  */
extern uae_u32 mousehack_helper (TrapContext *);

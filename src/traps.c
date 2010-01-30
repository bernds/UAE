 /*
  * E-UAE - The portable Amiga Emulator
  *
  * Support for traps
  *
  * Copyright Richard Drummond 2005
  *
  * Inspired by code from UAE:
  * Copyright 1995, 1996 Bernd Schmidt
  * Copyright 1996 Ed Hanway
  */

#include "sysconfig.h"
#include "sysdeps.h"

#include "options.h"
#include "memory.h"
#include "custom.h"
#include "newcpu.h"
#include "threaddep/thread.h"
#include "autoconf.h"
#include "traps.h"

/* We'll need a lot of these. */
#define MAX_TRAPS 4096
static TrapFunction traps[MAX_TRAPS];
static int trapmode[MAX_TRAPS];
static const char *trapstr[MAX_TRAPS];
static uaecptr trapoldfunc[MAX_TRAPS];

static int max_trap = 0;

/* Stack management */

/* The mechanism for doing m68k calls from native code is as follows:
 *
 * m68k code executes, stack is main
 * calltrap to execute_fn_on_extra_stack. new stack is allocated
 * do_stack_magic is called for the first time
 * current context is saved with setjmp [0]
 *  transfer_control is done
 *   native code executes on new stack
 *   native code calls call_m68k
 *   setjmp saves current context [1]
 *  longjmp back to execute_fn_on_extra_stack [0]
 * pointer to new stack is saved on m68k stack. m68k return address set
 * to RTAREA_BASE + 0xFF00. m68k PC set to called function
 * m68k function executes, stack is main
 * m68k function returns to RTAREA_BASE + 0xFF00
 * calltrap to m68k_mode_return
 * do_stack_magic is called again
 * current context is saved again with setjmp [0]
 *  this time, transfer_control is _not_ done, instead a longjmp[1]
 *  to the previously saved context
 *   native code executes again on temp stack
 *   native function returns to stack_stub
 *  longjmp[0] back to old context
 * back again!
 *
 * A bearded man enters the room, carrying a bowl of spaghetti.
 */

/* This _shouldn't_ crash with a stack size of 4096, but it does...
 * might be a bug */
#ifndef EXTRA_STACK_SIZE
#define EXTRA_STACK_SIZE 65536
#endif

static void *extra_stack_list = NULL;

static void *get_extra_stack (void)
{
    void *s = extra_stack_list;
    if (s)
	extra_stack_list = *(void **)s;
    if (!s)
	s = xmalloc (EXTRA_STACK_SIZE);
    return s;
}

static void free_extra_stack (void *s)
{
    *(void **)s = extra_stack_list;
    extra_stack_list = s;
}

static void stack_stub (void *s, TrapFunction f, uae_u32 *retval)
{
#ifdef CAN_DO_STACK_MAGIC
    /*printf("in stack_stub: %p %p %p %x\n", s, f, retval, (int)*retval);*/
    *retval = f ();
    /*write_log ("returning from stack_stub\n");*/
    longjmp (((jmp_buf *)s)[0], 1);
#endif
}

static void *current_extra_stack = NULL;
static uaecptr m68k_calladdr;

static void do_stack_magic (TrapFunction f, void *s, int has_retval)
{
#ifdef CAN_DO_STACK_MAGIC
    uaecptr a7;
    jmp_buf *j = (jmp_buf *)s;
    switch (setjmp (j[0])) {
     case 0:
	/* Returning directly */
	current_extra_stack = s;
	if (has_retval == -1) {
	    /*write_log ("finishing m68k mode return\n");*/
	    longjmp (j[1], 1);
	}
	/*write_log ("calling native function\n");*/
	transfer_control (s, EXTRA_STACK_SIZE, stack_stub, f, has_retval);
	/* not reached */
	abort ();

     case 1:
	/*write_log ("native function complete\n");*/
	/* Returning normally. */
	if (stack_has_retval (s, EXTRA_STACK_SIZE))
	    m68k_dreg (regs, 0) = get_retval_from_stack (s, EXTRA_STACK_SIZE);
	free_extra_stack (s);
	break;

     case 2:
	/* Returning to do a m68k call. We're now back on the main stack. */
	a7 = m68k_areg(regs, 7) -= (sizeof (void *) + 7) & ~3;
	/* Save stack to restore */
	*((void **)get_real_address (a7 + 4)) = s;
	/* Save special return address: this address contains a
	 * calltrap that will longjmp to the right stack. */
	put_long (m68k_areg (regs, 7), RTAREA_BASE + 0xFF00);
	m68k_setpc (m68k_calladdr);
	fill_prefetch_slow ();
	/*write_log ("native function calls m68k\n");*/
	break;
    }
    current_extra_stack = 0;
#endif
}

static uae_u32 execute_fn_on_extra_stack (TrapFunction f, int has_retval)
{
#ifdef CAN_DO_STACK_MAGIC
    void *s = get_extra_stack ();
    do_stack_magic (f, s, has_retval);
#endif
    return 0;
}

uae_u32 m68k_mode_return (void)
{
#ifdef CAN_DO_STACK_MAGIC
    uaecptr a7 = m68k_areg(regs, 7);
    void *s = *(void **)get_real_address(a7);
    m68k_areg(regs, 7) += (sizeof (void *) + 3) & ~3;
    /*write_log ("doing m68k mode return\n");*/
    do_stack_magic (NULL, s, -1);
#endif
    return 0;
}

static uae_u32 call_m68k (uaecptr addr, int saveregs)
{
    volatile uae_u32 retval = 0;
    volatile int do_save = saveregs;
    if (current_extra_stack == NULL)
	abort();
#ifdef CAN_DO_STACK_MAGIC
    {
	volatile struct regstruct saved_regs;
	jmp_buf *j = (jmp_buf *)current_extra_stack;

	if (do_save)
	    saved_regs = regs;
	m68k_calladdr = addr;
	switch (setjmp(j[1])) {
	 case 0:
	    /*write_log ("doing call\n");*/
	    /* Returning directly: now switch to main stack and do the call */
	    longjmp (j[0], 2);
	 case 1:
	    /*write_log ("returning from call\n");*/
	    retval = m68k_dreg (regs, 0);
	    if (do_save)
		regs = saved_regs;
	    /* Returning after the call. */
	    break;
	}
    }
#endif
    return retval;
}

uae_u32 CallLib (uaecptr base, uae_s16 offset)
{
    int i;
    uaecptr olda6 = m68k_areg(regs, 6);
    uae_u32 retval;
#if 0
    for (i = 0; i < n_libpatches; i++) {
	if (libpatches[i].libbase == base && libpatches[i].functions[-offset/6] != NULL)
	    return (*libpatches[i].functions[-offset/6])();
    }
#endif
    m68k_areg(regs, 6) = base;
    retval = call_m68k(base + offset, 1);
    m68k_areg(regs, 6) = olda6;
    return retval;
}

static int trace_traps = 1;

void REGPARAM2 call_calltrap(int func)
{
    uae_u32 retval = 0;
    int has_retval = (trapmode[func] & TRAPFLAG_NO_RETVAL) == 0;
    int implicit_rts = (trapmode[func] & TRAPFLAG_DORET) != 0;

    if (*trapstr[func] != 0 && trace_traps)
	write_log ("TRAP: %s\n", trapstr[func]);

    /* For monitoring only? */
    if (traps[func] == NULL) {
	m68k_setpc(trapoldfunc[func]);
	fill_prefetch_slow ();
	return;
    }

    if (func < max_trap) {
	if (trapmode[func] & TRAPFLAG_EXTRA_STACK) {
	    execute_fn_on_extra_stack(traps[func], has_retval);
	    return;
	}
	retval = (*traps[func])();
    } else
	write_log ("illegal emulator trap\n");

    if (has_retval)
	m68k_dreg(regs, 0) = retval;
    if (implicit_rts) {
	m68k_do_rts ();
	fill_prefetch_slow ();
    }
}

static volatile int four = 4;
uaecptr libemu_InstallFunctionFlags (TrapFunction f, uaecptr libbase, int offset,
				     int flags, const char *tracename)
{
    int i;
    uaecptr retval;
    uaecptr execbase = get_long (four);
    int trnum;
    uaecptr addr = here();
    calltrap (trnum = deftrap2 (f, flags, tracename));
    dw (RTS);
    
    m68k_areg(regs, 1) = libbase;
    m68k_areg(regs, 0) = offset;
    m68k_dreg(regs, 0) = addr;
    retval = CallLib (execbase, -420);
    
    trapoldfunc[trnum] = retval;
#if 0
    for (i = 0; i < n_libpatches; i++) {
	if (libpatches[i].libbase == libbase)
	    break;
    }
    if (i == n_libpatches) {
	int j;
	libpatches[i].libbase = libbase;
	for (j = 0; j < 300; j++)
	    libpatches[i].functions[j] = NULL;
	n_libpatches++;
    }
    libpatches[i].functions[-offset/6] = f;
#endif
    return retval;
}

int deftrap2 (TrapFunction func, int mode, const char *str)
{
    int num = max_trap++;
    traps[num] = func;
    trapstr[num] = str;
    trapmode[num] = mode;
    return num;
}

int deftrap (TrapFunction func)
{
    return deftrap2 (func, 0, "");
}

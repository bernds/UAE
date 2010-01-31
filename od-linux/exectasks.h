 /*
  * UAE - The Un*x Amiga Emulator
  *
  * Stack magic definitions for autoconf.c
  *
  * Copyright 1997 Bernd Schmidt
  */

#include <setjmp.h>

#undef CAN_DO_STACK_MAGIC

#if defined __GNUC__ && defined __GNUC_MINOR__
#if defined __i386__ && (__GNUC__ > 2 || __GNUC_MINOR__ >= 7)
#define CAN_DO_STACK_MAGIC
#endif
#endif

#ifdef CAN_DO_STACK_MAGIC
static inline void transfer_control(void *, int, void *, void *, int) __attribute__((noreturn));
static inline void transfer_control(void *s, int size, void *pc, void *f, int has_retval)
{
    unsigned long *stacktop = (unsigned long *)((char *)s + size - 20);
    stacktop[0] = 0xC0DEDBAD; /* return address */
    stacktop[1] = (int)s; /* function arg 1: stack */
    stacktop[2] = (int)f; /* function arg 2: trap function */
    stacktop[3] = (int)stacktop; /* function arg 3: return value address */
    stacktop[4] = has_retval;
    __asm__ __volatile__ ("movl %0,%%esp\n\tpushl %1\n\tret\n" : : "r" (stacktop), "r" (pc) : "memory");
    /* Not reached. */
    abort();
}

static inline uae_u32 get_retval_from_stack (void *s, int size)
{
    return *(uae_u32 *)((char *)s + size - 20);
}

static inline int stack_has_retval (void *s, int size)
{
    return *(int *)((char *)s + size - 4);
}

#define USE_EXECLIB

#else

#endif


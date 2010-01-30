/*
  * UAE - The Un*x Amiga Emulator
  *
  * Definitions for accessing cycle counters on a given machine, if possible.
  *
  * Copyright 1998 Bernd Schmidt
  */

typedef unsigned long frame_time_t;

static __inline__ frame_time_t read_processor_time (void)
{
    frame_time_t b;
/*    __asm__ __volatile__ ("mfspr %0,269" : "=r" (a) :  );*/
    __asm__ __volatile__ ("mfspr %0,268" : "=r" (b) :  );
    return b;
}



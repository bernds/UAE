/*
  * UAE - The Un*x Amiga Emulator
  *
  * Definitions for accessing cycle counters on a given machine, if possible.
  *
  * Copyright 1997, 1998 Bernd Schmidt
  */

typedef unsigned long frame_time_t;

static inline frame_time_t read_processor_time (void)
{
    frame_time_t foo;
    int dummy;
    /* Don't assume the assembler knows rdtsc */
    __asm__ __volatile__ (".byte 0x0f,0x31" : "=a" (foo), "=d" (dummy) :);
    return foo;
}

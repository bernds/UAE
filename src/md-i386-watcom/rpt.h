/*
  * UAE - The Un*x Amiga Emulator
  *
  * Definitions for accessing cycle counters on a given machine, if possible.
  *
  * Copyright 1998 Bernd Schmidt
  */

typedef unsigned __int64 frame_time_t;

extern frame_time_t read_processor_time (void);
#pragma aux read_processor_time = \
 "rdtsc" \
 value [eax edx];


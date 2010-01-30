 /*
  * UAE - The Un*x Amiga Emulator
  *
  * Prototypes for general sound related functions
  * This use to be called sound.h, but that causes confusion
  *
  * Copyright 1997 Bernd Schmidt
  */

extern int sound_available;

extern void (*sample_handler) (void);
extern unsigned long sample_evtime;

/* Determine if we can produce any sound at all.  This can be only a guess;
 * if unsure, say yes.  Any call to init_sound may change the value.  */
extern int setup_sound (void);

extern int init_sound (void);
extern void close_sound (void);

extern void sample16_handler (void);
extern void sample16i_rh_handler (void);
extern void sample16i_crux_handler (void);
extern void sample8_handler (void);
extern void sample16s_handler (void);
extern void sample16si_rh_handler (void);
extern void sample16si_crux_handler (void);
extern void sample8s_handler (void);
extern void sample_ulaw_handler (void);
extern void init_sound_table16 (void);
extern void init_sound_table8 (void);


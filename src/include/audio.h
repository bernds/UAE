 /*
  * UAE - The Un*x Amiga Emulator
  *
  * Sound emulation stuff
  *
  * Copyright 1995, 1996, 1997 Bernd Schmidt
  */

#define PERIOD_MAX ULONG_MAX

extern void AUDxDAT (unsigned int nr, uae_u16 value);
extern void AUDxVOL (unsigned int nr, uae_u16 value);
extern void AUDxPER (unsigned int nr, uae_u16 value);
extern void AUDxLCH (unsigned int nr, uae_u16 value);
extern void AUDxLCL (unsigned int nr, uae_u16 value);
extern void AUDxLEN (unsigned int nr, uae_u16 value);

extern int init_audio (void);
extern void ahi_install (void);
extern void audio_reset (void);
extern void update_audio (void);
extern void schedule_audio (void);
extern void audio_evhandler (void);
extern void audio_hsync (int);
extern void update_adkmasks (void);
extern void update_sound (unsigned int freq);

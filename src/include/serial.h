 /*
  * UAE - The Un*x Amiga Emulator
  *
  * Serial Line Emulation
  *
  * Copyright 1996, 1997 Stefan Reinauer <stepan@linux.de>
  * Copyright 1997 Christian Schmitt <schmitt@freiburg.linux.de>
  */

extern void serial_init(void);
extern void serial_exit(void);
extern void serial_dtr_off(void);

extern uae_u16 SERDATR(void);
extern int   SERDATS(void);
extern void  SERPER(uae_u16 w);
extern void  SERDAT(uae_u16 w);

extern uae_u16 serial_writestatus(int, int);
extern int serial_readstatus (void);
extern uae_u16 serdat;

extern int doreadser, serstat;

extern void serial_flush_buffer(void);

 /*
  * UAE - The Un*x Amiga Emulator
  *
  * Autoconfig device support
  *
  * (c) 1996 Ed Hanway
  */


extern uae_u32 addr (int);
extern void db (uae_u8);
extern void dw (uae_u16);
extern void dl (uae_u32);
extern uae_u32 ds (char *);
extern void calltrap (uae_u32);
extern void org (uae_u32);
extern uae_u32 here (void);
extern uae_u32 m68k_mode_return (void);
extern void align (int);

extern volatile int uae_int_requested;
extern void set_uae_int_flag (void);

#define RTS 0x4e75
#define RTE 0x4e73

extern uaecptr EXPANSION_explibname, EXPANSION_doslibname, EXPANSION_uaeversion;
extern uaecptr EXPANSION_explibbase, EXPANSION_uaedevname, EXPANSION_haveV36;
extern uaecptr EXPANSION_bootcode, EXPANSION_nullfunc;

extern uaecptr ROM_filesys_resname, ROM_filesys_resid;
extern uaecptr ROM_filesys_diagentry;
extern uaecptr ROM_hardfile_resname, ROM_hardfile_resid;
extern uaecptr ROM_hardfile_init;
extern uaecptr filesys_initcode;

extern void hardfile_install (void);
extern void emulib_install (void);
extern void expansion_init (void);
extern void expansion_clear (void);
extern void expansion_cleanup (void);

#define RTAREA_BASE 0xF00000

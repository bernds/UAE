 /*
  * UAE - The Un*x Amiga Emulator
  *
  * Save/restore emulator state
  *
  * (c) 1999-2001 Toni Wilen
  */


/* functions to save byte,word or long word
 * independent of CPU's endianess */

extern void save_u64_func (uae_u8 **, uae_u64);
extern void save_u32_func (uae_u8 **, uae_u32);
extern void save_u16_func (uae_u8 **, uae_u16);
extern void save_u8_func (uae_u8 **, uae_u8);

extern uae_u64 restore_u64_func (const uae_u8 **);
extern uae_u32 restore_u32_func (const uae_u8 **);
extern uae_u16 restore_u16_func (const uae_u8 **);
extern uae_u8 restore_u8_func (const uae_u8 **);

extern void save_string_func (uae_u8 **, const char*);
extern char *restore_string_func (const uae_u8 **);

#define save_u64(x) save_u64_func (&dst, (x))
#define save_u32(x) save_u32_func (&dst, (x))
#define save_u16(x) save_u16_func (&dst, (x))
#define save_u8(x) save_u8_func (&dst, (x))

#define restore_u64() restore_u64_func (&src)
#define restore_u32() restore_u32_func (&src)
#define restore_u16() restore_u16_func (&src)
#define restore_u8() restore_u8_func (&src)

#define save_string(x) save_string_func (&dst, (x))
#define restore_string() restore_string_func (&src)

/* save, restore and initialize routines for Amiga's subsystems */

extern const uae_u8 *restore_cpu (const uae_u8 *);
extern uae_u8 *save_cpu (int *, uae_u8 *);

extern const uae_u8 *restore_fpu (const uae_u8 *);
extern uae_u8 *save_fpu (int *, uae_u8 *);

extern const uae_u8 *restore_disk (int, const uae_u8 *);
extern uae_u8 *save_disk (int, int *, uae_u8 *);
extern const uae_u8 *restore_floppy (const uae_u8 *src);
extern uae_u8 *save_floppy (int *len, uae_u8 *);
extern void DISK_save_custom  (uae_u32 *pdskpt, uae_u16 *pdsklen, uae_u16 *pdsksync, uae_u16 *pdskdatr, uae_u16 *pdskbytr);
extern void DISK_restore_custom  (uae_u32 pdskpt, uae_u16 pdsklength, uae_u16 pdskdatr, uae_u16 pdskbytr);

extern const uae_u8 *restore_custom (const uae_u8 *);
extern uae_u8 *save_custom (int *, uae_u8 *, int);

extern const uae_u8 *restore_custom_sprite (int num, const uae_u8 *src);
extern uae_u8 *save_custom_sprite (int num, int *len);

extern const uae_u8 *restore_custom_agacolors (const uae_u8 *src);
extern uae_u8 *save_custom_agacolors (int *len, uae_u8 *);

extern const uae_u8 *restore_custom_blitter (const uae_u8 *src);
extern uae_u8 *save_custom_blitter (int *len);

extern const uae_u8 *restore_audio (int, const uae_u8 *);
extern uae_u8 *save_audio (int, int *);

extern const uae_u8 *restore_cia (int, const uae_u8 *);
extern uae_u8 *save_cia (int, int *, uae_u8 *);

extern const uae_u8 *restore_expansion (const uae_u8 *);
extern uae_u8 *save_expansion (int *, uae_u8 *);

extern void restore_cram (int, long);
extern void restore_bram (int, long);
extern void restore_fram (int, long);
extern void restore_zram (int, long);
extern void restore_a3000lram (int, size_t);
extern void restore_a3000hram (int, size_t);

extern void restore_ram (size_t, uae_u8 *);

extern uae_u8 *save_cram (int *);
extern uae_u8 *save_bram (int *);
extern uae_u8 *save_fram (int *);
extern uae_u8 *save_zram (int *);
extern uae_u8 *save_a3000lram (int *);
extern uae_u8 *save_a3000hram (int *);

extern const uae_u8 *restore_rom (const uae_u8 *);
extern uae_u8 *save_rom (int, int *, uae_u8 *);

extern void save_state (const char *filename, const char *description);
extern void restore_state (const char *filename);
extern void savestate_restore_finish (void);

extern void custom_save_state (void);

#define STATE_SAVE 1
#define STATE_RESTORE 2
#define STATE_DOSAVE 4
#define STATE_DORESTORE 8

extern int savestate_state;
extern char *savestate_filename;
extern struct zfile *savestate_file;

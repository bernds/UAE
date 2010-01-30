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

extern int nr_units (struct uaedev_mount_info *mountinfo);
extern int is_hardfile (struct uaedev_mount_info *mountinfo, int unit_no);
extern char *set_filesys_unit (struct uaedev_mount_info *mountinfo, int,
			       char *volname, char *rootdir, int readonly,
			       int secs, int surfaces, int reserved,
			       int blocksize);
extern char *add_filesys_unit (struct uaedev_mount_info *mountinfo,
			       char *volname, char *rootdir, int readonly,
			       int secs, int surfaces, int reserved,
			       int blocksize);
extern char *get_filesys_unit (struct uaedev_mount_info *mountinfo, int nr,
			       char **volname, char **rootdir, int *readonly,
			       int *secspertrack, int *surfaces, int *reserved,
			       int *cylinders, int *size, int *blocksize);
extern int kill_filesys_unit (struct uaedev_mount_info *mountinfo, int);
extern int move_filesys_unit (struct uaedev_mount_info *mountinfo, int nr, int to);
extern int sprintf_filesys_unit (struct uaedev_mount_info *mountinfo, char *buffer, int num);
extern void write_filesys_config (struct uaedev_mount_info *mountinfo, const char *unexpanded,
				  const char *defaultpath, FILE *f);

extern struct uaedev_mount_info *alloc_mountinfo (void);
extern struct uaedev_mount_info *dup_mountinfo (struct uaedev_mount_info *);
extern void free_mountinfo (struct uaedev_mount_info *);

extern void filesys_reset (void);
extern void filesys_prepare_reset (void);
extern void filesys_start_threads (void);

extern void filesys_install (void);
extern void filesys_install_code (void);
extern void filesys_store_devinfo (uae_u8 *);
extern void hardfile_install (void);
extern void emulib_install (void);
extern void expansion_init (void);
extern void expansion_cleanup (void);

#define RTAREA_BASE 0xF00000

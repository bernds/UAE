 /*
  * UAE - The Un*x Amiga Emulator
  *
  * Unix file system handler for AmigaDOS
  *
  * Copyright 1997 Bernd Schmidt
  */

struct hardfiledata {
    uae_u64 size;
    uae_u64 offset;
    int nrcyls;
    int secspertrack;
    int surfaces;
    int reservedblocks;
    int blocksize;
    FILE *fd;

    /* geometry from possible RDSK block */
    unsigned int cylinders;
    unsigned int sectors;
    unsigned int heads;
};

#define FILESYS_VIRTUAL 0
#define FILESYS_HARDFILE 1
#define FILESYS_HARDFILE_RDB 2
#define FILESYS_HARDDRIVE 3

#define FILESYS_MAX_BLOCKSIZE 2048

struct uaedev_mount_info;

extern struct hardfiledata *get_hardfile_data (int nr);

extern int nr_units (struct uaedev_mount_info *mountinfo);
extern int hardfile_fs_type (struct uaedev_mount_info *mountinfo, int unit_no);

extern char *set_filesys_unit (struct uaedev_mount_info *mountinfo, int,
			       const char *devname, const char *volname, const char *rootdir,
			       int readonly, int secs, int surfaces, int reserved,
			       int blocksize, int bootpri);
extern char *add_filesys_unit (struct uaedev_mount_info *mountinfo,
			       const char *devname, const char *volname, const char *rootdir,
			       int readonly, int secs, int surfaces, int reserved,
			       int blocksize, int bootpri);
extern char *get_filesys_unit (struct uaedev_mount_info *mountinfo, int nr,
			       char **devname, char **volname, char **rootdir,
			       int *readonly, int *secspertrack, int *surfaces, int *reserved,
			       int *cylinders, int *size, int *blocksize, int *bootpri);
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

extern int hdf_read (struct hardfiledata *hfd, void *buffer, uae_u64 offset, int len);
extern int hdf_write (struct hardfiledata *hfd, void *buffer, uae_u64 offset, int len);

 /*
  * UAE - The Un*x Amiga Emulator
  *
  * routines to handle compressed file automatically
  *
  * (c) 1996 Samuel Devulder
  */

struct zfile;
extern int is_zlib;

extern struct zfile *zfile_fopen (const char *, const char *);
extern struct zfile *zfile_fopen_empty (const char *name, int size);
extern int zfile_exists (const char *name);
extern void zfile_fclose (struct zfile *);
extern int zfile_fseek (struct zfile *z, long offset, int mode);
extern long zfile_ftell (struct zfile *z);
extern size_t zfile_fread  (void *b, size_t l1, size_t l2, struct zfile *z);
extern size_t zfile_fwrite  (void *b, size_t l1, size_t l2, struct zfile *z);
extern void zfile_exit (void);
extern int execute_command (char *);
extern int zfile_iscompressed (struct zfile *z);
extern int zfile_zcompress (struct zfile *dst, void *src, int size);
extern int zfile_zuncompress (void *dst, int dstsize, struct zfile *src, int srcsize);


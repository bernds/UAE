 /*
  * UAE - The Un*x Amiga Emulator
  *
  * routines to handle compressed file automatically
  *
  * (c) 1996 Samuel Devulder
  */

struct zfile;

extern struct zfile *zfile_open (const char *, const char *);
extern int zfile_fclose (struct zfile *);
extern int zfile_fseek (struct zfile *z, long offset, int mode);
extern long zfile_ftell (struct zfile *z);
extern size_t zfile_fread (void *b, size_t l1, size_t l2, struct zfile *z);
extern size_t zfile_fwrite (void *b, size_t l1, size_t l2, struct zfile *z);
extern void zfile_exit (void);

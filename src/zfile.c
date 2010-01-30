 /*
  * UAE - The Un*x Amiga Emulator
  *
  * routines to handle compressed file automatically
  *
  * (c) 1996 Samuel Devulder, Tim Gunn
  */

#include "sysconfig.h"
#include "sysdeps.h"

#include "options.h"
#include "zfile.h"

#ifdef AMIGA
extern char *amiga_dev_path;   /* dev: */
extern char *ixemul_dev_path;  /* /dev/ */
extern int readdevice (const char *, char *);
#endif

struct zfile
{
    struct zfile *next;
    struct zfile **pprev;
    FILE *f;
    char name[L_tmpnam];
};

static struct zfile *zlist = 0;

/*
 * called on exit ()
 */
void zfile_exit (void)
{
    struct zfile *l;

    while ((l = zlist)) {
	zlist = l->next;
	fclose (l->f);
	unlink (l->name); /* sam: in case unlink () after fopen () fails */
	free (l);
    }
}

/*
 * fclose () but for a compressed file
 */
int zfile_fclose (struct zfile *f)
{
    int ret;

    if (f->next)
	f->next->pprev = f->pprev;
    (*f->pprev) = f->next;
    
    ret = fclose (f->f);
    unlink (f->name);

    free (f);

    return ret;
}

int zfile_fseek (struct zfile *z, long offset, int mode)
{
    return fseek (z->f, offset, mode);
}

long zfile_ftell (struct zfile *z)
{
    return ftell (z->f);
}

size_t zfile_fread (void *b, size_t l1, size_t l2, struct zfile *z)
{
    return fread (b, l1, l2, z->f);
}

size_t zfile_fwrite (void *b, size_t l1, size_t l2, struct zfile *z)
{
    return fwrite (b, l1, l2, z->f);
}

/*
 * gzip decompression
 */
static int gunzip (const char *decompress, const char *src, const char *dst)
{
    char cmd[1024];
    char *ext = strrchr (src, '.');
    if (!dst)
	return 1;
#if defined (AMIGA)
    sprintf (cmd, "%s -c -d -S %s \"%s\" > \"%s\"", decompress, ext, src, dst);
#else
    sprintf (cmd, "%s -c -d \"%s\" > \"%s\"", decompress, src, dst);
#endif
    return !system (cmd);
}

/*
 * bzip/bzip2 decompression
 */
static int bunzip (const char *decompress, const char *src, const char *dst)
{
    char cmd[1024];
    if (!dst)
	return 1;
    sprintf (cmd, "%s -c -d \"%s\" > \"%s\"", decompress, src, dst);
    return !system (cmd);
}

/*
 * lha decompression
 */
static int lha (const char *src, const char *dst)
{
    char cmd[1024];
    if (!dst)
	return 1;
#if defined (AMIGA)
    sprintf (cmd, "lha -q -N p %s >%s", src, dst);
#else
    sprintf (cmd, "lha pq %s >%s", src, dst);
#endif
    return !system (cmd);
}

/*
 * (pk)unzip decompression
 */
static int unzip (const char *src, const char *dst)
{
    char cmd[1024];
    if (!dst)
	return 1;
#if defined AMIGA || defined __unix
    sprintf (cmd, "unzip -p %s '*.adf' >%s", src, dst);
    return !system (cmd);
#endif
}

/*
 * decompresses the file (or check if dest is null)
 */
static int uncompress (const char *name, char *dest)
{
    char *ext = strrchr (name, '.');
    char nam[1024];

    if (ext != NULL && access (name, 0) >= 0) {
	ext++;
	if (strcasecmp (ext, "z") == 0
	    || strcasecmp (ext, "gz") == 0
	    || strcasecmp (ext, "adz") == 0
	    || strcasecmp (ext, "roz") == 0)
	    return gunzip ("gzip", name, dest);
	if (strcasecmp (ext, "bz") == 0)
	    return bunzip ("bzip", name, dest);
	if (strcasecmp (ext, "bz2") == 0)
	    return bunzip ("bzip2", name, dest);

#ifndef __DOS__
	if (strcasecmp (ext, "lha") == 0
	    || strcasecmp (ext, "lzh") == 0)
	    return lha (name, dest);
	if (strcasecmp (ext, "zip") == 0)
	     return unzip (name, dest);
#endif
    }

    if (access (strcat (strcpy (nam, name), ".z"), 0) >= 0
	|| access (strcat (strcpy (nam, name), ".Z"), 0) >= 0
	|| access (strcat (strcpy (nam, name), ".gz"), 0) >= 0
	|| access (strcat (strcpy (nam, name), ".GZ"), 0) >= 0
	|| access (strcat (strcpy (nam, name), ".adz"), 0) >= 0
	|| access (strcat (strcpy (nam, name), ".roz"), 0) >= 0)
	return gunzip ("gzip", nam, dest);

    if (access (strcat (strcpy (nam, name), ".bz"), 0) >= 0
	|| access (strcat (strcpy (nam, name), ".BZ"), 0) >= 0)
	return bunzip ("bzip", nam, dest);

    if (access (strcat (strcpy (nam, name), ".bz2"), 0) >= 0
	|| access (strcat (strcpy (nam, name), ".BZ2"), 0) >= 0)
	return bunzip ("bzip2", nam, dest);

#ifndef __DOS__
    if (access (strcat (strcpy (nam, name), ".lha"), 0) >= 0
	|| access (strcat (strcpy (nam, name), ".LHA"), 0) >= 0
	|| access (strcat (strcpy (nam, name), ".lzh"), 0) >= 0
	|| access (strcat (strcpy (nam, name), ".LZH"), 0) >= 0)
	return lha (nam, dest);

    if (access (strcat (strcpy (nam, name),".zip"),0) >= 0
	|| access (strcat (strcpy (nam, name),".ZIP"),0) >= 0)
       return unzip (nam, dest);
#endif

#if defined (AMIGA)
    /* sam: must be before real access to work */
    if (!strnicmp (name, ixemul_dev_path, strlen (ixemul_dev_path)))
	return readdevice (name + strlen (ixemul_dev_path), dest);
    if (!strnicmp (name, amiga_dev_path, strlen (amiga_dev_path)))
	return readdevice (name + strlen (amiga_dev_path), dest);
#endif

    return 0;
}

/*
 * fopen () for a compressed file
 */
struct zfile *zfile_open (const char *name, const char *mode)
{
    struct zfile *l = malloc (sizeof *l);
    int fd = 0;

    if (! l)
	return NULL;

    strcpy (l->name, "");

    if (! uncompress (name, NULL))
	l->f = fopen (name, mode);
    else {
	tmpnam (l->name);
	fd = creat (l->name, S_IRUSR | S_IWUSR);
	if (fd < 0)
	    return NULL;

	if (! uncompress (name, l->name)) {
	    close (fd);
	    unlink (l->name);
	    free (l);
	    return NULL;
	}
	l->f = fopen (l->name, mode);
	close (fd);

    }
    if (l->f == NULL) {
	if (strlen (l->name) > 0)
	    unlink (l->name);
	free (l);
	return NULL;
    }

    l->pprev = &zlist;
    l->next = zlist;
    if (l->next)
	l->next->pprev = &l->next;
    zlist = l;

    return l;
}

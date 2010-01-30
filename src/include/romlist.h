#define ROMTYPE_KICK 1
#define ROMTYPE_KICKCD32 2
#define ROMTYPE_EXTCD32 4
#define ROMTYPE_EXTCDTV 8
#define ROMTYPE_A2091BOOT 16
#define ROMTYPE_A4091BOOT 32
#define ROMTYPE_AR 64
#define ROMTYPE_SUPERIV 128
#define ROMTYPE_KEY 256
#define ROMTYPE_ARCADIABIOS 512
#define ROMTYPE_ARCADIAGAME 1024
#define ROMTYPE_HRTMON 2048

struct romheader {
    char *name;
    int id;
};

/* Mask to determine CPU type required by the ROM.  */
#define ROMREQ_CPUMASK 3

#define ROMREQ_68EC020 1
#define ROMREQ_68020 2
#define ROMREQ_68030 3
#define ROMREQ_A4000MB 4
#define ROMREQ_A4000 (ROMREQ_A4000MB | ROMREQ_68020)
#define ROMREQ_A1000 8

struct romdata {
    char *name;
    int ver, rev;
    int subver, subrev;
    char *model;
    uae_u32 size;
    int id;
    int cpu;
    int cloanto;
    int type;
    int title;
    uae_u32 crc32;
    uae_u32 sha1[5];
    char *configname;
};

/* Used to tag a romlist elt with the location where it was found.
   That way, we can quickly remove everything from a user path when
   that changes, leaving the system ROMs intact.  */
#define ROMLOC_SYSTEM 1
#define ROMLOC_USER 2

struct romlist {
    char *path;
    struct romdata *rd;
    int loc;
};

extern struct romdata *getromdatabycrc (uae_u32 crc32);
extern struct romdata *getromdatabydata (uae_u8 *rom, int size);
extern struct romdata *getromdatabyid (int id);
extern struct romdata *getarcadiarombyname (char *name);
extern void getromname (struct romdata*, char*);
extern struct romdata *getromdatabyname (char*);
extern void romlist_add (char *path, struct romdata *rd, int loc);
extern char *romlist_get (struct romdata *rd);
extern void romlist_clear (int mask);
extern int decode_cloanto_rom (uae_u8 *, int, int, int);
extern struct romlist *romlist_from_idx (int idx, int type, int need_crc32);

extern void scan_roms (const char *path, int loc);

 /*
  * UAE - The Un*x Amiga Emulator
  *
  * Memory management
  *
  * (c) 1995 Bernd Schmidt
  */

#include "sysconfig.h"
#include "sysdeps.h"

#include "config.h"
#include "options.h"
#include "uae.h"
#include "memory.h"
#include "ersatz.h"
#include "zfile.h"
#include "custom.h"
#include "events.h"
#include "newcpu.h"
#include "autoconf.h"
#include "savestate.h"
#include "crc32.h"

#ifdef USE_MAPPED_MEMORY
#include <sys/mman.h>
#endif

/* Set by each memory handler that does not simply access real memory.  */
int special_mem;

int ersatzkickfile = 0;

uae_u32 allocated_chipmem;
uae_u32 allocated_fastmem;
uae_u32 allocated_bogomem;
uae_u32 allocated_gfxmem;
uae_u32 allocated_z3fastmem;
uae_u32 allocated_a3000mem;

static long chip_filepos;
static long bogo_filepos;
static long rom_filepos;

/* ROM management.  */
struct romlist {
    char *path;
    struct romdata *rd;
};

static struct romlist *rl;
static int romlist_cnt;

void romlist_add (char *path, struct romdata *rd)
{
    struct romlist *rl2;

    romlist_cnt++;
    rl = realloc (rl, sizeof (struct romlist) * romlist_cnt);
    rl2 = rl + romlist_cnt - 1;
    rl2->path = my_strdup (path);
    rl2->rd = rd;
}

char *romlist_get (struct romdata *rd)
{
    int i;

    if (!rd)
	return 0;
    for (i = 0; i < romlist_cnt; i++) {
	if (rl[i].rd == rd)
	    return rl[i].path;
    }
    return 0;
}

void romlist_clear (void)
{
    free (rl);
    rl = 0;
    romlist_cnt = 0;
}

static struct romdata roms[] = {
    { "Cloanto Amiga Forever ROM key", 0, 0, 0x869ae1b1, 2069, 0, 0, 1, ROMTYPE_KEY },

    { "Kickstart v1.0 (A1000)(NTSC)", 0, 0, 0x299790ff, 262144, 1, 0, 0, ROMTYPE_KICK },
    { "Kickstart v1.1 (A1000)(NTSC)", 31, 34, 0xd060572a, 262144, 2, 0, 0, ROMTYPE_KICK },
    { "Kickstart v1.1 (A1000)(PAL)", 31, 34, 0xec86dae2, 262144, 3, 0, 0, ROMTYPE_KICK },
    { "Kickstart v1.2 (A1000)", 33, 166, 0x9ed783d0, 262144, 4, 0, 0, ROMTYPE_KICK },
    { "Kickstart v1.2 (A500,A1000,A2000)", 33, 180, 0xa6ce1636, 262144, 5, 0, 0, ROMTYPE_KICK },
    { "Kickstart v1.3 (A500,A1000,A2000)", 34, 5, 0xc4f0f55f, 262144, 6, 60, 0, ROMTYPE_KICK },
    { "Kickstart v1.3 (A3000)", 34, 5, 0xe0f37258, 262144, 32, 0, 0, ROMTYPE_KICK },

    { "Kickstart v2.04 (A500+)", 37, 175, 0xc3bdb240, 524288, 7, 0, 0, ROMTYPE_KICK },
    { "Kickstart v2.05 (A600)", 37, 299, 0x83028fb5, 524288, 8, 0, 0, ROMTYPE_KICK },
    { "Kickstart v2.05 (A600HD)", 37, 300, 0x64466c2a, 524288, 9, 0, 0, ROMTYPE_KICK },
    { "Kickstart v2.05 (A600HD)", 37, 350, 0x43b0df7b, 524288, 10, 0, 0, ROMTYPE_KICK },

    { "Kickstart v3.0 (A1200)", 39, 106, 0x6c9b07d2, 524288, 11, 0, 0, ROMTYPE_KICK },
    { "Kickstart v3.0 (A4000)", 39, 106, 0x9e6ac152, 524288, 12, 2, 0, ROMTYPE_KICK },
    { "Kickstart v3.1 (A4000)", 40, 70, 0x2b4566f1, 524288, 13, 2, 0, ROMTYPE_KICK },
    { "Kickstart v3.1 (A500,A600,A2000)", 40, 63, 0xfc24ae0d, 524288, 14, 0, 0, ROMTYPE_KICK },
    { "Kickstart v3.1 (A1200)", 40, 68, 0x1483a091, 524288, 15, 1, 0, ROMTYPE_KICK },
    { "Kickstart v3.1 (A4000)(Cloanto)", 40, 68, 0x43b6dd22, 524288, 31, 2, 1, ROMTYPE_KICK },
    { "Kickstart v3.1 (A4000)", 40, 68, 0xd6bae334, 524288, 16, 2, 0, ROMTYPE_KICK },
    { "Kickstart v3.1 (A4000T)", 40, 70, 0x75932c3a, 524288, 17, 2, 0, ROMTYPE_KICK },

    { "CD32 Kickstart v3.1", 40, 60, 0x1e62d4a5, 524288, 18, 1, 0, ROMTYPE_KICKCD32 },
    { "CD32 Extended", 40, 60, 0x87746be2, 524288, 19, 1, 0, ROMTYPE_EXTCD32 },

    { "CDTV Extended v1.00", 0, 0, 0x42baa124, 262144, 20, 0, 0, ROMTYPE_EXTCDTV },
    { "CDTV Extended v2.30", 0, 0, 0x30b54232, 262144, 21, 0, 0, ROMTYPE_EXTCDTV },
    { "CDTV Extended v2.07", 0, 0, 0xceae68d2, 262144, 22, 0, 0, ROMTYPE_EXTCDTV },

    { "A1000 Bootstrap", 0, 0, 0x62f11c04, 8192, 23, 0, 0, ROMTYPE_KICK },
    { "A1000 Bootstrap", 0, 0, 0x0b1ad2d0, 65536, 24, 0, 0, ROMTYPE_KICK },

    { "Action Replay Mk I v1.50", 0, 0, 0xd4ce0675, 65536, 25, 0, 0, ROMTYPE_AR },
    { "Action Replay Mk II v2.05", 0, 0, 0x1287301f , 131072, 26, 0, 0, ROMTYPE_AR },
    { "Action Replay Mk II v2.12", 0, 0, 0x804d0361 , 131072, 27, 0, 0, ROMTYPE_AR },
    { "Action Replay Mk II v2.14", 0, 0, 0x49650e4f, 131072, 28, 0, 0, ROMTYPE_AR },
    { "Action Replay Mk III v3.09", 0, 0, 0x0ed9b5aa, 262144, 29, 0, 0, ROMTYPE_AR },
    { "Action Replay Mk III v3.17", 0, 0, 0xc8a16406, 262144, 30, 0, 0, ROMTYPE_AR },

    { "SportTime Table Hockey\0ar_airh", 0, 0, 0, 0, 33, 0, 0, ROMTYPE_ARCADIA },
    { "SportTime Bowling\0ar_bowl", 0, 0, 0, 0, 34, 0, 0, ROMTYPE_ARCADIA },
    { "World Darts\0ar_dart", 0, 0, 0, 0, 35, 0, 0, ROMTYPE_ARCADIA },
    { "Magic Johnson's Fast Break\0ar_fast", 0, 0, 0, 0, 36, 0, 0, ROMTYPE_ARCADIA },
    { "Leader Board Golf\0ar_ldrb", 0, 0, 0, 0, 37, 0, 0, ROMTYPE_ARCADIA },
    { "Leader Board Golf (alt)\0ar_ldrba", 0, 0, 0, 0, 38, 0, 0, ROMTYPE_ARCADIA },
    { "Ninja Mission\0ar_ninj", 0, 0, 0, 0, 39, 0, 0, ROMTYPE_ARCADIA },
    { "Road Wars\0ar_rdwr", 0, 0, 0, 0, 40, 0, 0, ROMTYPE_ARCADIA },
    { "Sidewinder\0ar_sdwr", 0, 0, 0, 0, 41, 0, 0, ROMTYPE_ARCADIA },
    { "Cool Spot\0ar_spot", 0, 0, 0, 0, 42, 0, 0, ROMTYPE_ARCADIA },
    { "Space Ranger\0ar_sprg", 0, 0, 0, 0, 43, 0, 0, ROMTYPE_ARCADIA },
    { "Xenon\0ar_xeon", 0, 0, 0, 0, 44, 0, 0, ROMTYPE_ARCADIA },

    { NULL, 0, 0, 0, 0, 0, 0, 0, 0 }
};

static void decode_cloanto_rom_do (uae_u8 *mem, int size, int real_size, uae_u8 *key, int keysize)
{
    long cnt, t;
    for (t = cnt = 0; cnt < size; cnt++, t = (t + 1) % keysize)  {
	mem[cnt] ^= key[t];
	if (real_size == cnt + 1)
	    t = keysize - 1;
    }
}

static int decode_cloanto_rom (uae_u8 *mem, int size, int real_size)
{
    FILE *keyf;
    uae_u8 *p;
    int keysize;

    if (strlen (currprefs.keyfile) == 0) {
	write_log ("No filename given for ROM key file and ROM image is an encrypted \"Amiga Forever\" ROM file.\n");
	return 0;
    }
    keyf = zfile_open (currprefs.keyfile, "rb");
    if (keyf == 0) {
	write_log ("Could not find specified ROM key-file.\n");
	return 0;
    }

    p = (uae_u8 *) xmalloc (524288);
    keysize = fread (p, 1, 524288, keyf);
    decode_cloanto_rom_do (mem, size, real_size, p, keysize);
    fclose (keyf);
    free (p);
    return 1;
}

struct romdata *getromdatabyname (char *name)
{
    char tmp[1024];
    int i = 0;
    while (roms[i].name) {
	getromname (&roms[i], tmp);
	if (!strcmp (tmp, name) || !strcmp (roms[i].name, name))
	    return &roms[i];
	i++;
    }
    return 0;
}

struct romdata *getromdatabyid (int id)
{
    int i = 0;
    while (roms[i].name) {
	if (id == roms[i].id)
	    return &roms[i];
	i++;
    }
    return 0;
}

struct romdata *getromdatabycrc (uae_u32 crc32)
{
    int i = 0;
    while (roms[i].name) {
	if (crc32 == roms[i].crc32)
	    return &roms[i];
	i++;
    }
    return 0;
}

struct romdata *getromdatabydata (uae_u8 *rom, int size)
{
    int i;
    uae_u32 crc32a, crc32b, crc32c;
    uae_u8 tmp[4];
    uae_u8 *tmpbuf = NULL;

    if (size > 11 && !memcmp (rom, "AMIROMTYPE1", 11)) {
	uae_u8 *tmpbuf = xmalloc (size);
	int tmpsize = size - 11;
	memcpy (tmpbuf, rom + 11, tmpsize);
	decode_cloanto_rom (tmpbuf, tmpsize, tmpsize);
	rom = tmpbuf;
	size = tmpsize;
    }
    crc32a = get_crc32 (rom, size);
    crc32b = get_crc32 (rom, size / 2);
     /* ignore AR IO-port range until we have full dump */
    memcpy (tmp, rom, 4);
    memset (rom, 0, 4);
    crc32c = get_crc32 (rom, size);
    memcpy (rom, tmp, 4);
    i = 0;
    while (roms[i].name) {
	if (roms[i].crc32) {
	    if (crc32a == roms[i].crc32 || crc32b == roms[i].crc32)
		return &roms[i];
	    if (crc32c == roms[i].crc32 && roms[i].type == ROMTYPE_AR)
		return &roms[i];
	}
	i++;
    }
    free (tmpbuf);
    return 0;
}

void getromname (struct romdata *rd, char *name)
{
    name[0] = 0;
    if (rd->type == ROMTYPE_ARCADIA)
	strcat (name, "Arcadia ");
    strcat (name, rd->name);
    if (rd->revision)
	sprintf (name + strlen (name), " rev %d.%d", rd->version, rd->revision);
    if (rd->size > 0)
	sprintf (name + strlen (name), " (%dk)", (rd->size + 1023) / 1024);
}

addrbank *mem_banks[65536];

/* This has two functions. It either holds a host address that, when added
   to the 68k address, gives the host address corresponding to that 68k
   address (in which case the value in this array is even), OR it holds the
   same value as mem_banks, for those banks that have baseaddr==0. In that
   case, bit 0 is set (the memory access routines will take care of it).  */

uae_u8 *baseaddr[65536];

#ifdef NO_INLINE_MEMORY_ACCESS
__inline__ uae_u32 longget (uaecptr addr)
{
    return call_mem_get_func (get_mem_bank (addr).lget, addr);
}
__inline__ uae_u32 wordget (uaecptr addr)
{
    return call_mem_get_func (get_mem_bank (addr).wget, addr);
}
__inline__ uae_u32 byteget (uaecptr addr)
{
    return call_mem_get_func (get_mem_bank (addr).bget, addr);
}
__inline__ void longput (uaecptr addr, uae_u32 l)
{
    call_mem_put_func (get_mem_bank (addr).lput, addr, l);
}
__inline__ void wordput (uaecptr addr, uae_u32 w)
{
    call_mem_put_func (get_mem_bank (addr).wput, addr, w);
}
__inline__ void byteput (uaecptr addr, uae_u32 b)
{
    call_mem_put_func (get_mem_bank (addr).bput, addr, b);
}
#endif

uae_u32 chipmem_mask, kickmem_mask, extendedkickmem_mask, bogomem_mask, a3000mem_mask;

static int illegal_count;
/* A dummy bank that only contains zeros */

static uae_u32 dummy_lget (uaecptr) REGPARAM;
static uae_u32 dummy_wget (uaecptr) REGPARAM;
static uae_u32 dummy_bget (uaecptr) REGPARAM;
static void dummy_lput (uaecptr, uae_u32) REGPARAM;
static void dummy_wput (uaecptr, uae_u32) REGPARAM;
static void dummy_bput (uaecptr, uae_u32) REGPARAM;
static int dummy_check (uaecptr addr, uae_u32 size) REGPARAM;

uae_u32 REGPARAM2 dummy_lget (uaecptr addr)
{
    special_mem |= S_READ;
    if (currprefs.illegal_mem) {
	if (illegal_count < 20) {
	    illegal_count++;
	    write_log ("Illegal lget at %08lx\n", addr);
	}
    }

    return 0xFFFFFFFF;
}

uae_u32 REGPARAM2 dummy_wget (uaecptr addr)
{
    special_mem |= S_READ;
    if (currprefs.illegal_mem) {
	if (illegal_count < 20) {
	    illegal_count++;
	    write_log ("Illegal wget at %08lx\n", addr);
	}
    }

    return 0xFFFF;
}

uae_u32 REGPARAM2 dummy_bget (uaecptr addr)
{
    special_mem |= S_READ;
    if (currprefs.illegal_mem) {
	if (illegal_count < 20) {
	    illegal_count++;
	    write_log ("Illegal bget at %08lx\n", addr);
	}
    }

    return 0xFF;
}

void REGPARAM2 dummy_lput (uaecptr addr, uae_u32 l)
{
    special_mem |= S_WRITE;
    if (currprefs.illegal_mem) {
	if (illegal_count < 20) {
	    illegal_count++;
	    write_log ("Illegal lput at %08lx\n", addr);
	}
    }
}
void REGPARAM2 dummy_wput (uaecptr addr, uae_u32 w)
{
    special_mem |= S_WRITE;
    if (currprefs.illegal_mem) {
	if (illegal_count < 20) {
	    illegal_count++;
	    write_log ("Illegal wput at %08lx\n", addr);
	}
    }
}
void REGPARAM2 dummy_bput (uaecptr addr, uae_u32 b)
{
    special_mem |= S_WRITE;
    if (currprefs.illegal_mem) {
	if (illegal_count < 20) {
	    illegal_count++;
	    write_log ("Illegal bput at %08lx\n", addr);
	}
    }
}

int REGPARAM2 dummy_check (uaecptr addr, uae_u32 size)
{
    special_mem |= S_READ;
    if (currprefs.illegal_mem) {
	if (illegal_count < 20) {
	    illegal_count++;
	    write_log ("Illegal check at %08lx\n", addr);
	}
    }

    return 0;
}

/* A3000 "motherboard resources" bank.  */
static uae_u32 mbres_lget (uaecptr) REGPARAM;
static uae_u32 mbres_wget (uaecptr) REGPARAM;
static uae_u32 mbres_bget (uaecptr) REGPARAM;
static void mbres_lput (uaecptr, uae_u32) REGPARAM;
static void mbres_wput (uaecptr, uae_u32) REGPARAM;
static void mbres_bput (uaecptr, uae_u32) REGPARAM;
static int mbres_check (uaecptr addr, uae_u32 size) REGPARAM;

static int mbres_val = 0;

uae_u32 REGPARAM2 mbres_lget (uaecptr addr)
{
    special_mem |= S_READ;
    if (currprefs.illegal_mem)
	write_log ("Illegal lget at %08lx\n", addr);

    return 0;
}

uae_u32 REGPARAM2 mbres_wget (uaecptr addr)
{
    special_mem |= S_READ;
    if (currprefs.illegal_mem)
	write_log ("Illegal wget at %08lx\n", addr);

    return 0;
}

uae_u32 REGPARAM2 mbres_bget (uaecptr addr)
{
    special_mem |= S_READ;
    if (currprefs.illegal_mem)
	write_log ("Illegal bget at %08lx\n", addr);

    return (addr & 0xFFFF) == 3 ? mbres_val : 0;
}

void REGPARAM2 mbres_lput (uaecptr addr, uae_u32 l)
{
    special_mem |= S_WRITE;
    if (currprefs.illegal_mem)
	write_log ("Illegal lput at %08lx\n", addr);
}
void REGPARAM2 mbres_wput (uaecptr addr, uae_u32 w)
{
    special_mem |= S_WRITE;
    if (currprefs.illegal_mem)
	write_log ("Illegal wput at %08lx\n", addr);
}
void REGPARAM2 mbres_bput (uaecptr addr, uae_u32 b)
{
    special_mem |= S_WRITE;
    if (currprefs.illegal_mem)
	write_log ("Illegal bput at %08lx\n", addr);

    if ((addr & 0xFFFF) == 3)
	mbres_val = b;
}

int REGPARAM2 mbres_check (uaecptr addr, uae_u32 size)
{
    if (currprefs.illegal_mem)
	write_log ("Illegal check at %08lx\n", addr);

    return 0;
}

/* Chip memory */

uae_u8 *chipmemory;

static int chipmem_check (uaecptr addr, uae_u32 size) REGPARAM;
static uae_u8 *chipmem_xlate (uaecptr addr) REGPARAM;

uae_u32 REGPARAM2 chipmem_lget (uaecptr addr)
{
    uae_u32 *m;

    addr -= chipmem_start & chipmem_mask;
    addr &= chipmem_mask;
    m = (uae_u32 *)(chipmemory + addr);
    return do_get_mem_long (m);
}

uae_u32 REGPARAM2 chipmem_wget (uaecptr addr)
{
    uae_u16 *m;

    addr -= chipmem_start & chipmem_mask;
    addr &= chipmem_mask;
    m = (uae_u16 *)(chipmemory + addr);
    return do_get_mem_word (m);
}

uae_u32 REGPARAM2 chipmem_bget (uaecptr addr)
{
    addr -= chipmem_start & chipmem_mask;
    addr &= chipmem_mask;
    return chipmemory[addr];
}

void REGPARAM2 chipmem_lput (uaecptr addr, uae_u32 l)
{
    uae_u32 *m;

    addr -= chipmem_start & chipmem_mask;
    addr &= chipmem_mask;
    m = (uae_u32 *)(chipmemory + addr);
    do_put_mem_long (m, l);
}

void REGPARAM2 chipmem_wput (uaecptr addr, uae_u32 w)
{
    uae_u16 *m;

    addr -= chipmem_start & chipmem_mask;
    addr &= chipmem_mask;
    m = (uae_u16 *)(chipmemory + addr);
    do_put_mem_word (m, w);
}

void REGPARAM2 chipmem_bput (uaecptr addr, uae_u32 b)
{
    addr -= chipmem_start & chipmem_mask;
    addr &= chipmem_mask;
    chipmemory[addr] = b;
}

int REGPARAM2 chipmem_check (uaecptr addr, uae_u32 size)
{
    addr -= chipmem_start & chipmem_mask;
    addr &= chipmem_mask;
    return (addr + size) <= allocated_chipmem;
}

uae_u8 REGPARAM2 *chipmem_xlate (uaecptr addr)
{
    addr -= chipmem_start & chipmem_mask;
    addr &= chipmem_mask;
    return chipmemory + addr;
}

/* Slow memory */

static uae_u8 *bogomemory;

static uae_u32 bogomem_lget (uaecptr) REGPARAM;
static uae_u32 bogomem_wget (uaecptr) REGPARAM;
static uae_u32 bogomem_bget (uaecptr) REGPARAM;
static void bogomem_lput (uaecptr, uae_u32) REGPARAM;
static void bogomem_wput (uaecptr, uae_u32) REGPARAM;
static void bogomem_bput (uaecptr, uae_u32) REGPARAM;
static int bogomem_check (uaecptr addr, uae_u32 size) REGPARAM;
static uae_u8 *bogomem_xlate (uaecptr addr) REGPARAM;

uae_u32 REGPARAM2 bogomem_lget (uaecptr addr)
{
    uae_u32 *m;
    addr -= bogomem_start & bogomem_mask;
    addr &= bogomem_mask;
    m = (uae_u32 *)(bogomemory + addr);
    return do_get_mem_long (m);
}

uae_u32 REGPARAM2 bogomem_wget (uaecptr addr)
{
    uae_u16 *m;
    addr -= bogomem_start & bogomem_mask;
    addr &= bogomem_mask;
    m = (uae_u16 *)(bogomemory + addr);
    return do_get_mem_word (m);
}

uae_u32 REGPARAM2 bogomem_bget (uaecptr addr)
{
    addr -= bogomem_start & bogomem_mask;
    addr &= bogomem_mask;
    return bogomemory[addr];
}

void REGPARAM2 bogomem_lput (uaecptr addr, uae_u32 l)
{
    uae_u32 *m;
    addr -= bogomem_start & bogomem_mask;
    addr &= bogomem_mask;
    m = (uae_u32 *)(bogomemory + addr);
    do_put_mem_long (m, l);
}

void REGPARAM2 bogomem_wput (uaecptr addr, uae_u32 w)
{
    uae_u16 *m;
    addr -= bogomem_start & bogomem_mask;
    addr &= bogomem_mask;
    m = (uae_u16 *)(bogomemory + addr);
    do_put_mem_word (m, w);
}

void REGPARAM2 bogomem_bput (uaecptr addr, uae_u32 b)
{
    addr -= bogomem_start & bogomem_mask;
    addr &= bogomem_mask;
    bogomemory[addr] = b;
}

int REGPARAM2 bogomem_check (uaecptr addr, uae_u32 size)
{
    addr -= bogomem_start & bogomem_mask;
    addr &= bogomem_mask;
    return (addr + size) <= allocated_bogomem;
}

uae_u8 REGPARAM2 *bogomem_xlate (uaecptr addr)
{
    addr -= bogomem_start & bogomem_mask;
    addr &= bogomem_mask;
    return bogomemory + addr;
}

/* A3000 motherboard fast memory */

static uae_u8 *a3000memory;

static uae_u32 a3000mem_lget (uaecptr) REGPARAM;
static uae_u32 a3000mem_wget (uaecptr) REGPARAM;
static uae_u32 a3000mem_bget (uaecptr) REGPARAM;
static void a3000mem_lput (uaecptr, uae_u32) REGPARAM;
static void a3000mem_wput (uaecptr, uae_u32) REGPARAM;
static void a3000mem_bput (uaecptr, uae_u32) REGPARAM;
static int a3000mem_check (uaecptr addr, uae_u32 size) REGPARAM;
static uae_u8 *a3000mem_xlate (uaecptr addr) REGPARAM;

uae_u32 REGPARAM2 a3000mem_lget (uaecptr addr)
{
    uae_u32 *m;
    addr -= a3000mem_start & a3000mem_mask;
    addr &= a3000mem_mask;
    m = (uae_u32 *)(a3000memory + addr);
    return do_get_mem_long (m);
}

uae_u32 REGPARAM2 a3000mem_wget (uaecptr addr)
{
    uae_u16 *m;
    addr -= a3000mem_start & a3000mem_mask;
    addr &= a3000mem_mask;
    m = (uae_u16 *)(a3000memory + addr);
    return do_get_mem_word (m);
}

uae_u32 REGPARAM2 a3000mem_bget (uaecptr addr)
{
    addr -= a3000mem_start & a3000mem_mask;
    addr &= a3000mem_mask;
    return a3000memory[addr];
}

void REGPARAM2 a3000mem_lput (uaecptr addr, uae_u32 l)
{
    uae_u32 *m;
    addr -= a3000mem_start & a3000mem_mask;
    addr &= a3000mem_mask;
    m = (uae_u32 *)(a3000memory + addr);
    do_put_mem_long (m, l);
}

void REGPARAM2 a3000mem_wput (uaecptr addr, uae_u32 w)
{
    uae_u16 *m;
    addr -= a3000mem_start & a3000mem_mask;
    addr &= a3000mem_mask;
    m = (uae_u16 *)(a3000memory + addr);
    do_put_mem_word (m, w);
}

void REGPARAM2 a3000mem_bput (uaecptr addr, uae_u32 b)
{
    addr -= a3000mem_start & a3000mem_mask;
    addr &= a3000mem_mask;
    a3000memory[addr] = b;
}

int REGPARAM2 a3000mem_check (uaecptr addr, uae_u32 size)
{
    addr -= a3000mem_start & a3000mem_mask;
    addr &= a3000mem_mask;
    return (addr + size) <= allocated_a3000mem;
}

uae_u8 REGPARAM2 *a3000mem_xlate (uaecptr addr)
{
    addr -= a3000mem_start & a3000mem_mask;
    addr &= a3000mem_mask;
    return a3000memory + addr;
}

/* Kick memory */

uae_u8 *kickmemory;

/*
 * A1000 kickstart RAM handling
 *
 * RESET instruction unhides boot ROM and disables write protection
 * write access to boot ROM hides boot ROM and enables write protection
 *
 */
static int a1000_kickstart_mode;
static uae_u8 *a1000_bootrom;
static void a1000_handle_kickstart (int mode)
{
    if (mode == 0) {
	a1000_kickstart_mode = 0;
	memcpy (kickmemory, kickmemory + 262144, 262144);
    } else {
	a1000_kickstart_mode = 1;
	memset (kickmemory, 0, 262144);
	memcpy (kickmemory, a1000_bootrom, 8192);
	memcpy (kickmemory + 131072, a1000_bootrom, 8192);
    }
}

static uae_u32 kickmem_lget (uaecptr) REGPARAM;
static uae_u32 kickmem_wget (uaecptr) REGPARAM;
static uae_u32 kickmem_bget (uaecptr) REGPARAM;
static void kickmem_lput (uaecptr, uae_u32) REGPARAM;
static void kickmem_wput (uaecptr, uae_u32) REGPARAM;
static void kickmem_bput (uaecptr, uae_u32) REGPARAM;
static int kickmem_check (uaecptr addr, uae_u32 size) REGPARAM;
static uae_u8 *kickmem_xlate (uaecptr addr) REGPARAM;

uae_u32 REGPARAM2 kickmem_lget (uaecptr addr)
{
    uae_u32 *m;
    addr -= kickmem_start & kickmem_mask;
    addr &= kickmem_mask;
    m = (uae_u32 *)(kickmemory + addr);
    return do_get_mem_long (m);
}

uae_u32 REGPARAM2 kickmem_wget (uaecptr addr)
{
    uae_u16 *m;
    addr -= kickmem_start & kickmem_mask;
    addr &= kickmem_mask;
    m = (uae_u16 *)(kickmemory + addr);
    return do_get_mem_word (m);
}

uae_u32 REGPARAM2 kickmem_bget (uaecptr addr)
{
    addr -= kickmem_start & kickmem_mask;
    addr &= kickmem_mask;
    return kickmemory[addr];
}

void REGPARAM2 kickmem_lput (uaecptr addr, uae_u32 b)
{
    uae_u32 *m;
    if (a1000_kickstart_mode) {
	if (addr >= 0xfc0000) {
	    addr -= kickmem_start & kickmem_mask;
	    addr &= kickmem_mask;
	    m = (uae_u32 *)(kickmemory + addr);
	    do_put_mem_long (m, b);
	    return;
	} else
	    a1000_handle_kickstart (0);
    } else if (currprefs.illegal_mem)
	write_log ("Illegal kickmem lput at %08lx\n", addr);
}

void REGPARAM2 kickmem_wput (uaecptr addr, uae_u32 b)
{
    uae_u16 *m;
    if (a1000_kickstart_mode) {
	if (addr >= 0xfc0000) {
	    addr -= kickmem_start & kickmem_mask;
	    addr &= kickmem_mask;
	    m = (uae_u16 *)(kickmemory + addr);
	    do_put_mem_word (m, b);
	    return;
	} else
	    a1000_handle_kickstart (0);
    } else if (currprefs.illegal_mem)
	write_log ("Illegal kickmem wput at %08lx\n", addr);
}

void REGPARAM2 kickmem_bput (uaecptr addr, uae_u32 b)
{
    if (a1000_kickstart_mode) {
	if (addr >= 0xfc0000) {
	    addr -= kickmem_start & kickmem_mask;
	    addr &= kickmem_mask;
	    kickmemory[addr] = b;
	    return;
	} else
	    a1000_handle_kickstart (0);
    } else if (currprefs.illegal_mem)
	write_log ("Illegal kickmem lput at %08lx\n", addr);
}

int REGPARAM2 kickmem_check (uaecptr addr, uae_u32 size)
{
    addr -= kickmem_start & kickmem_mask;
    addr &= kickmem_mask;
    return (addr + size) <= kickmem_size;
}

uae_u8 REGPARAM2 *kickmem_xlate (uaecptr addr)
{
    addr -= kickmem_start & kickmem_mask;
    addr &= kickmem_mask;
    return kickmemory + addr;
}

/* CD32/CDTV extended kick memory */

uae_u8 *extendedkickmemory;
static int extendedkickmem_size;
static uae_u32 extendedkickmem_start;

#define EXTENDED_ROM_CD32 1
#define EXTENDED_ROM_CDTV 2

static int extromtype (void)
{
    switch (extendedkickmem_size) {
    case 524288:
	return EXTENDED_ROM_CD32;
    case 262144:
	return EXTENDED_ROM_CDTV;
    }
    return 0;
}

static uae_u32 extendedkickmem_lget (uaecptr) REGPARAM;
static uae_u32 extendedkickmem_wget (uaecptr) REGPARAM;
static uae_u32 extendedkickmem_bget (uaecptr) REGPARAM;
static void extendedkickmem_lput (uaecptr, uae_u32) REGPARAM;
static void extendedkickmem_wput (uaecptr, uae_u32) REGPARAM;
static void extendedkickmem_bput (uaecptr, uae_u32) REGPARAM;
static int extendedkickmem_check (uaecptr addr, uae_u32 size) REGPARAM;
static uae_u8 *extendedkickmem_xlate (uaecptr addr) REGPARAM;

uae_u32 REGPARAM2 extendedkickmem_lget (uaecptr addr)
{
    uae_u32 *m;
    addr -= extendedkickmem_start & extendedkickmem_mask;
    addr &= extendedkickmem_mask;
    m = (uae_u32 *)(extendedkickmemory + addr);
    return do_get_mem_long (m);
}

uae_u32 REGPARAM2 extendedkickmem_wget (uaecptr addr)
{
    uae_u16 *m;
    addr -= extendedkickmem_start & extendedkickmem_mask;
    addr &= extendedkickmem_mask;
    m = (uae_u16 *)(extendedkickmemory + addr);
    return do_get_mem_word (m);
}

uae_u32 REGPARAM2 extendedkickmem_bget (uaecptr addr)
{
    addr -= extendedkickmem_start & extendedkickmem_mask;
    addr &= extendedkickmem_mask;
    return extendedkickmemory[addr];
}

void REGPARAM2 extendedkickmem_lput (uaecptr addr, uae_u32 b)
{
    if (currprefs.illegal_mem)
	write_log ("Illegal extendedkickmem lput at %08lx\n", addr);
}

void REGPARAM2 extendedkickmem_wput (uaecptr addr, uae_u32 b)
{
    if (currprefs.illegal_mem)
	write_log ("Illegal extendedkickmem wput at %08lx\n", addr);
}

void REGPARAM2 extendedkickmem_bput (uaecptr addr, uae_u32 b)
{
    if (currprefs.illegal_mem)
	write_log ("Illegal extendedkickmem lput at %08lx\n", addr);
}

int REGPARAM2 extendedkickmem_check (uaecptr addr, uae_u32 size)
{
    addr -= extendedkickmem_start & extendedkickmem_mask;
    addr &= extendedkickmem_mask;
    return (addr + size) <= extendedkickmem_size;
}

uae_u8 REGPARAM2 *extendedkickmem_xlate (uaecptr addr)
{
    addr -= extendedkickmem_start & extendedkickmem_mask;
    addr &= extendedkickmem_mask;
    return extendedkickmemory + addr;
}

/* Default memory access functions */

int REGPARAM2 default_check (uaecptr a, uae_u32 b)
{
    return 0;
}

uae_u8 REGPARAM2 *default_xlate (uaecptr a)
{
    write_log ("Your Amiga program just did something terribly stupid\n");
    uae_reset (1);
    return kickmem_xlate (get_long (0xF80000));	/* So we don't crash. */
}

/* Address banks */

addrbank dummy_bank = {
    dummy_lget, dummy_wget, dummy_bget,
    dummy_lput, dummy_wput, dummy_bput,
    default_xlate, dummy_check, NULL
};

addrbank mbres_bank = {
    mbres_lget, mbres_wget, mbres_bget,
    mbres_lput, mbres_wput, mbres_bput,
    default_xlate, mbres_check, NULL
};

addrbank chipmem_bank = {
    chipmem_lget, chipmem_wget, chipmem_bget,
    chipmem_lput, chipmem_wput, chipmem_bput,
    chipmem_xlate, chipmem_check, NULL
};

addrbank bogomem_bank = {
    bogomem_lget, bogomem_wget, bogomem_bget,
    bogomem_lput, bogomem_wput, bogomem_bput,
    bogomem_xlate, bogomem_check, NULL
};

addrbank a3000mem_bank = {
    a3000mem_lget, a3000mem_wget, a3000mem_bget,
    a3000mem_lput, a3000mem_wput, a3000mem_bput,
    a3000mem_xlate, a3000mem_check, NULL
};

addrbank kickmem_bank = {
    kickmem_lget, kickmem_wget, kickmem_bget,
    kickmem_lput, kickmem_wput, kickmem_bput,
    kickmem_xlate, kickmem_check, NULL
};

addrbank extendedkickmem_bank = {
    extendedkickmem_lget, extendedkickmem_wget, extendedkickmem_bget,
    extendedkickmem_lput, extendedkickmem_wput, extendedkickmem_bput,
    extendedkickmem_xlate, extendedkickmem_check, NULL
};

static int kickstart_checksum (uae_u8 *mem, int size)
{
    uae_u32 cksum = 0, prevck = 0;
    int i;
    for (i = 0; i < size; i += 4) {
	uae_u32 data = mem[i] * 65536 * 256 + mem[i + 1] * 65536 + mem[i + 2] * 256 + mem[i + 3];
	cksum += data;
	if (cksum < prevck)
	    cksum++;
	prevck = cksum;
    }
    if (cksum != 0xFFFFFFFFul) {
	write_log ("Kickstart checksum incorrect. You probably have a corrupted ROM image.\n");
    }
    return 0;
}

static int read_kickstart (FILE *f, uae_u8 *mem, int size, int dochecksum, int *cloanto_rom)
{
    unsigned char buffer[20];
    int i, cr = 0;

    if (cloanto_rom)
	*cloanto_rom = 0;
    i = fread (buffer, 1, 11, f);
    if (strncmp ((char *) buffer, "AMIROMTYPE1", 11) != 0) {
	fseek (f, 0, SEEK_SET);
    } else {
	cr = 1;
    }

    i = fread (mem, 1, size, f);
    if (i == 8192) {
	a1000_bootrom = malloc (8192);
	memcpy (a1000_bootrom, kickmemory, 8192);
	a1000_handle_kickstart (1);
    } else if (i == size / 2) {
	memcpy (mem + size / 2, mem, i);
    } else if (i != size) {
	write_log ("Error while reading Kickstart.\n");
	zfile_close (f);
	return 0;
    }
    zfile_close (f);

    if (cr)
	decode_cloanto_rom (mem, size, i);
    if (dochecksum && i >= 262144)
	kickstart_checksum (mem, size);
    if (cloanto_rom)
	*cloanto_rom = cr;
    return 1;
}

static int load_extendedkickstart (void)
{
    FILE *f;
    int size;

    if (strlen (currprefs.romextfile) == 0)
	return 0;
    f = zfile_open (currprefs.romextfile, "rb");
    if (!f) {
	write_log ("No extended Kickstart ROM found");
	return 0;
    }

    fseek (f, 0, SEEK_END);
    size = ftell (f);
    if (size > 300000)
	extendedkickmem_size = 524288;
    else
	extendedkickmem_size = 262144;
    fseek (f, 0, SEEK_SET);

    switch (extromtype ()) {
    case EXTENDED_ROM_CDTV:
	extendedkickmemory = (uae_u8 *) mapped_malloc (extendedkickmem_size, "rom_f0");
	extendedkickmem_bank.baseaddr = (uae_u8 *) extendedkickmemory;
	break;
    case EXTENDED_ROM_CD32:
	extendedkickmemory = (uae_u8 *) mapped_malloc (extendedkickmem_size, "rom_e0");
	extendedkickmem_bank.baseaddr = (uae_u8 *) extendedkickmemory;
	break;
    }
    read_kickstart (f, extendedkickmemory, 524288, 0, 0);
    fclose (f);
    return 1;
}


static int load_kickstart (void)
{
    FILE *f = zfile_open (currprefs.romfile, "rb");

    if (f == NULL) {
#if defined(AMIGA)||defined(__POS__)
#define USE_UAE_ERSATZ "USE_UAE_ERSATZ"
	if (!getenv (USE_UAE_ERSATZ)) {
	    write_log ("Using current ROM. (create ENV:%s to " "use uae's ROM replacement)\n", USE_UAE_ERSATZ);
	    memcpy (kickmemory, (char *) 0x1000000 - kickmem_size, kickmem_size);
	    kickstart_checksum (kickmemory, kickmem_size);
	    goto chk_sum;
	}
#endif
	return 0;
    }

    if (!read_kickstart (f, kickmemory, kickmem_size, 1, &cloanto_rom))
	return 0;

#if defined(AMIGA)
  chk_sum:
#endif

    if (currprefs.kickshifter) {
	/* Patch Kickstart ROM for ShapeShifter - from Christian Bauer.
	   Changes 'lea $400,a0' to 'lea $2000,a0' for ShapeShifter compatability.
	   NOTE: lea is 0x41f8, so we should do this better with a search for
	   0x41f80400 --> 0x41f82000
	*/
	if (kickmemory[0x24a] == 0x04 && kickmemory[0x24b] == 0x00) {	/* Kick 3.0 */
	    kickmemory[0x24a] = 0x20;
	    kickmemory[0x7ffea] -= 0x1c;
	    write_log ("Kickstart KickShifted\n");
	} else if (kickmemory[0x26e] == 0x04 && kickmemory[0x26f] == 0x00) {	/* Kick 3.1 */
	    kickmemory[0x26e] = 0x20;
	    kickmemory[0x7ffea] -= 0x1c;
	    write_log ("Kickstart KickShifted\n");
	} else if (kickmemory[0x24e] == 0x04 && kickmemory[0x24f] == 0x00) {	/* Kick 2.04 */
	    kickmemory[0x24e] = 0x20;
	    kickmemory[0x7ffea] -= 0x1c;
	    write_log ("Kickstart KickShifted\n");
	}
    }
    return 1;
}

char *address_space, *good_address_map;
int good_address_fd;

#ifndef NATMEM_OFFSET

uae_u8 *mapped_malloc (size_t s, char *file)
{
    return calloc (s, 1);
}

void mapped_free (uae_u8 *p)
{
    free (p);
}
#else

#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/mman.h>

shmpiece *shm_start = NULL;
int canbang = 1;

static void dumplist (void)
{
    shmpiece *x = shm_start;
    printf ("Start Dump:\n");
    while (x) {
	printf ("  this=%p, Native %p, id %d, prev=%p, next=%p, size=0x%08x\n",
		x, x->native_address, x->id, x->prev, x->next, x->size);
	x = x->next;
    }
    printf ("End Dump:\n");
}

static shmpiece *find_shmpiece (uae_u8 *base)
{
    shmpiece *x = shm_start;

    while (x && x->native_address != base)
	x = x->next;
    if (!x) {
	printf ("NATMEM: Failure to find mapping at %p\n", base);
	dumplist ();
	canbang = 0;
	return 0;
    }
    return x;
}

static void delete_shmmaps (uae_u32 start, uae_u32 size)
{
    if (!canbang)
	return;

    while (size) {
	uae_u8 *base = mem_banks[bankindex (start)]->baseaddr;
	if (base) {
	    shmpiece *x;
	    base = ((uae_u8 *) NATMEM_OFFSET) + start;

	    x = find_shmpiece (base);
	    if (!x)
		return;

	    if (x->size > size) {
		printf ("NATMEM: Failure to delete mapping at %08x(size %08x, delsize %08x)\n", start, x->size, size);
		dumplist ();
		canbang = 0;
		return;
	    }
	    shmdt (x->native_address);
	    size -= x->size;
	    start += x->size;
	    if (x->next)
		x->next->prev = x->prev;	/* remove this one from the list */
	    if (x->prev)
		x->prev->next = x->next;
	    else
		shm_start = x->next;
	    free (x);
	} else {
	    size -= 0x10000;
	    start += 0x10000;
	}
    }
}

static void add_shmmaps (uae_u32 start, addrbank *what)
{
    shmpiece *x = shm_start;
    shmpiece *y;
    uae_u8 *base = what->baseaddr;

    if (!canbang)
	return;
    if (!base)
	return;

    x = find_shmpiece (base);
    if (!x)
	return;
    y = malloc (sizeof (shmpiece));
    *y = *x;
    base = ((uae_u8 *) NATMEM_OFFSET) + start;
    y->native_address = shmat (y->id, base, 0);
    if (y->native_address == (void *) -1) {
	printf ("NATMEM: Failure to map existing at %08x(%p)\n", start, base);
	perror ("shmat");
	dumplist ();
	canbang = 0;
	return;
    }
    y->next = shm_start;
    y->prev = NULL;
    if (y->next)
	y->next->prev = y;
    shm_start = y;
}

uae_u8 *mapped_malloc (size_t s, char *file)
{
    int id;
    void *answer;
    shmpiece *x;

    if (!canbang)
	return malloc (s);

    id = shmget (IPC_PRIVATE, s, 0x1ff, file);
    if (id == 1) {
	canbang = 0;
	return mapped_malloc (s, file);
    }
    answer = shmat (id, 0, 0);
    shmctl (id, IPC_RMID, NULL);
    if (answer != (void *) -1) {
	x = malloc (sizeof (shmpiece));
	x->native_address = answer;
	x->id = id;
	x->size = s;
	x->next = shm_start;
	x->prev = NULL;
	if (x->next)
	    x->next->prev = x;
	shm_start = x;

	return answer;
    }
    canbang = 0;
    return mapped_malloc (s, file);
}

void mapped_free (uae_u8 *base)
{
    shmpiece *x = find_shmpiece (base);
    if (!x)
	abort ();
    shmdt (x->native_address);
}

#endif

static void init_mem_banks (void)
{
    int i;
    for (i = 0; i < 65536; i++)
	put_mem_bank (i << 16, &dummy_bank, 0);
}

static void allocate_memory (void)
{
    if (allocated_chipmem != currprefs.chipmem_size) {
	if (chipmemory)
	    mapped_free (chipmemory);
	chipmemory = 0;

	allocated_chipmem = currprefs.chipmem_size;
	chipmem_mask = allocated_chipmem - 1;

	chipmemory = mapped_malloc (allocated_chipmem, "chip");
	if (chipmemory == 0) {
	    write_log ("Fatal error: out of memory for chipmem.\n");
	    allocated_chipmem = 0;
	} else
	    do_put_mem_long ((uae_u32 *)(chipmemory + 4), 0);
    }

    if (allocated_bogomem != currprefs.bogomem_size) {
	if (bogomemory)
	    mapped_free (bogomemory);
	bogomemory = 0;

	allocated_bogomem = currprefs.bogomem_size;
	bogomem_mask = allocated_bogomem - 1;

	if (allocated_bogomem) {
	    bogomemory = mapped_malloc (allocated_bogomem, "bogo");
	    if (bogomemory == 0) {
		write_log ("Out of memory for bogomem.\n");
		allocated_bogomem = 0;
	    }
	}
    }
    if (allocated_a3000mem != currprefs.a3000mem_size) {
	if (a3000memory)
	    mapped_free (a3000memory);
	a3000memory = 0;

	allocated_a3000mem = currprefs.a3000mem_size;
	a3000mem_mask = allocated_a3000mem - 1;

	if (allocated_a3000mem) {
	    a3000memory = mapped_malloc (allocated_a3000mem, "a3000");
	    if (a3000memory == 0) {
		write_log ("Out of memory for a3000mem.\n");
		allocated_a3000mem = 0;
	    }
	}
    }

    if (savestate_state == STATE_RESTORE) {
	fseek (savestate_file, chip_filepos, SEEK_SET);
	fread (chipmemory, 1, allocated_chipmem, savestate_file);
	if (allocated_bogomem > 0) {
	    fseek (savestate_file, bogo_filepos, SEEK_SET);
	    fread (bogomemory, 1, allocated_bogomem, savestate_file);
	}
    }
    chipmem_bank.baseaddr = chipmemory;
    bogomem_bank.baseaddr = bogomemory;
}

void memory_reset (void)
{
    int i, custom_start;

#ifdef NATMEM_OFFSET
    delete_shmmaps (0, 0xFFFF0000);
#endif
    init_mem_banks ();

    currprefs.chipmem_size = changed_prefs.chipmem_size;
    currprefs.bogomem_size = changed_prefs.bogomem_size;
    currprefs.a3000mem_size = changed_prefs.a3000mem_size;

    allocate_memory ();

    if (strcmp (currprefs.romfile, changed_prefs.romfile) != 0 || strcmp (currprefs.keyfile, changed_prefs.keyfile) != 0) {
	ersatzkickfile = 0;
	memcpy (currprefs.romfile, changed_prefs.romfile, sizeof currprefs.romfile);
	memcpy (currprefs.keyfile, changed_prefs.keyfile, sizeof currprefs.keyfile);
	/* Clear out whatever data remains across a reset.  */
	memset (chipmemory, 0, allocated_chipmem);
	if (!load_kickstart ()) {
	    init_ersatz_rom (kickmemory);
	    ersatzkickfile = 1;
	}
    }
    /* Map the chipmem into all of the lower 8MB */
    i = allocated_chipmem > 0x200000 ? (allocated_chipmem >> 16) : 32;
    map_banks (&chipmem_bank, 0x00, i, allocated_chipmem);

    custom_start = 0xC0;

    map_banks (&custom_bank, custom_start, 0xE0 - custom_start, 0);
    map_banks (&cia_bank, 0xA0, 32, 0);
    map_banks (&clock_bank, 0xDC, 1, 0);

    /* @@@ Does anyone have a clue what should be in the 0x200000 - 0xA00000
     * range on an Amiga without expansion memory?  */
    custom_start = allocated_chipmem >> 16;
    if (custom_start < 0x20)
	custom_start = 0x20;
    map_banks (&dummy_bank, custom_start, 0xA0 - custom_start, 0);
    /*map_banks (&mbres_bank, 0xDE, 1); */

    if (bogomemory != 0) {
	int t = allocated_bogomem >> 16;
	if (t > 0x1C)
	    t = 0x1C;
	map_banks (&bogomem_bank, 0xC0, t, allocated_bogomem);
    }
    if (a3000memory != 0)
	map_banks (&a3000mem_bank, a3000mem_start >> 16, allocated_a3000mem >> 16, allocated_a3000mem);

    map_banks (&rtarea_bank, RTAREA_BASE >> 16, 1, 0);

    map_banks (&kickmem_bank, 0xF8, 8, 0);
    if (a1000_bootrom)
	a1000_handle_kickstart (1);
    map_banks (&expamem_bank, 0xE8, 1, 0);

    switch (extromtype ()) {
    case EXTENDED_ROM_CDTV:
	map_banks (&extendedkickmem_bank, 0xF0, 4, 0);
	break;
    case EXTENDED_ROM_CD32:
	map_banks (&extendedkickmem_bank, 0xE0, 8, 0);
	break;
    default:
	if (cloanto_rom)
	    map_banks (&kickmem_bank, 0xE0, 8, 0);
    }
}

void memory_init (void)
{
    allocated_chipmem = 0;
    allocated_bogomem = 0;
    allocated_a3000mem = 0;
    kickmemory = 0;
    extendedkickmemory = 0;
    chipmemory = 0;
    a3000memory = 0;
    bogomemory = 0;

    kickmemory = mapped_malloc (kickmem_size, "kick");
    kickmem_bank.baseaddr = kickmemory;

    load_extendedkickstart ();
    if (!load_kickstart ()) {
	init_ersatz_rom (kickmemory);
	ersatzkickfile = 1;
    }

    init_mem_banks ();
    memory_reset ();

    kickmem_mask = kickmem_size - 1;
    extendedkickmem_mask = extendedkickmem_size - 1;
}

void memory_cleanup (void)
{
    if (a3000memory)
	mapped_free (a3000memory);
    if (bogomemory)
	mapped_free (bogomemory);
    if (kickmemory)
	mapped_free (kickmemory);
    if (a1000_bootrom)
	free (a1000_bootrom);
    if (chipmemory)
	mapped_free (chipmemory);

    a3000memory = 0;
    bogomemory = 0;
    kickmemory = 0;
    a1000_bootrom = 0;
    chipmemory = 0;
}

void map_banks (addrbank *bank, int start, int size, int realsize)
{
    int bnr;
    unsigned long int hioffs = 0, endhioffs = 0x100;
    addrbank *orgbank = bank;
    uae_u32 realstart = start;

    flush_icache (1);		/* Sure don't want to keep any old mappings around! */
#ifdef NATMEM_OFFSET
    delete_shmmaps (start << 16, size << 16);
#endif

    if (!realsize)
	realsize = size << 16;

    if ((size << 16) < realsize) {
	write_log ("Please report to bmeyer@cs.monash.edu.au, and mention:\n");
	write_log ("Broken mapping, size=%x, realsize=%x\n", size, realsize);
	write_log ("Start is %x\n", start);
	write_log ("Reducing memory sizes, especially chipmem, may fix this problem\n");
	abort ();
    }

    if (start >= 0x100) {
	int real_left = 0;
	for (bnr = start; bnr < start + size; bnr++) {
	    if (!real_left) {
		realstart = bnr;
		real_left = realsize >> 16;
#ifdef NATMEM_OFFSET
		add_shmmaps (realstart << 16, bank);
#endif
	    }
	    put_mem_bank (bnr << 16, bank, realstart << 16);
	    real_left--;
	}
	return;
    }
    if (currprefs.address_space_24)
	endhioffs = 0x10000;
    for (hioffs = 0; hioffs < endhioffs; hioffs += 0x100) {
	int real_left = 0;
	for (bnr = start; bnr < start + size; bnr++) {
	    if (!real_left) {
		realstart = bnr + hioffs;
		real_left = realsize >> 16;
#ifdef NATMEM_OFFSET
		add_shmmaps (realstart << 16, bank);
#endif
	    }
	    put_mem_bank ((bnr + hioffs) << 16, bank, realstart << 16);
	    real_left--;
	}
    }
}


/* memory save/restore code */

uae_u8 *save_cram (int *len)
{
    *len = allocated_chipmem;
    return chipmemory;
}

uae_u8 *save_bram (int *len)
{
    *len = allocated_bogomem;
    return bogomemory;
}

void restore_cram (int len, long filepos)
{
    chip_filepos = filepos;
    changed_prefs.chipmem_size = len;
}

void restore_bram (int len, long filepos)
{
    bogo_filepos = filepos;
    changed_prefs.bogomem_size = len;
}

uae_u8 *restore_rom (uae_u8 *src)
{
    uae_u32 crc32;
    int i;

    restore_u32 ();
    restore_u32 ();
    restore_u32 ();
    restore_u32 ();
    crc32 = restore_u32 ();

    for (i = 0; i < romlist_cnt; i++) {
	if (rl[i].rd->crc32 == crc32 && crc32) {
	    strncpy (changed_prefs.romfile, rl[i].path, 255);
	    break;
	}
    }

    return src;
}

uae_u8 *save_rom (int first, int *len, uae_u8 *dstptr)
{
    static int count;
    uae_u8 *dst, *dstbak;
    uae_u8 *mem_real_start;
    int mem_start, mem_size, mem_type, i, saverom;

    saverom = 0;
    if (first)
	count = 0;
    for (;;) {
	mem_type = count;
	switch (count) {
	case 0:		/* Kickstart ROM */
	    mem_start = 0xf80000;
	    mem_real_start = kickmemory;
	    mem_size = kickmem_size;
	    /* 256KB or 512KB ROM? */
	    for (i = 0; i < mem_size / 2 - 4; i++) {
		if (longget (i + mem_start) != longget (i + mem_start + mem_size / 2))
		    break;
	    }
	    if (i == mem_size / 2 - 4) {
		mem_size /= 2;
		mem_start += 262144;
	    }
	    mem_type = 0;
	    break;
	default:
	    return 0;
	}
	count++;
	if (mem_size)
	    break;
    }
    if (dstptr)
	dstbak = dst = dstptr;
    else
	dstbak = dst = malloc (4 + 4 + 4 + 4 + 4 + mem_size);
    save_u32 (mem_start);
    save_u32 (mem_size);
    save_u32 (mem_type);
    save_u32 (longget (mem_start + 12));	/* version+revision */
    save_u32 (get_crc32 (kickmemory, mem_size));
    sprintf (dst, "Kickstart %d.%d", wordget (mem_start + 12), wordget (mem_start + 14));
    dst += strlen (dst) + 1;
    if (saverom) {
	for (i = 0; i < mem_size; i++)
	    *dst++ = byteget (mem_start + i);
    }
    *len = dst - dstbak;
    return dstbak;
}

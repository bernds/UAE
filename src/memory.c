 /*
  * UAE - The Un*x Amiga Emulator
  *
  * Memory management
  *
  * (c) 1995 Bernd Schmidt
  */

#include "sysconfig.h"
#include "sysdeps.h"

#include "options.h"
#include "uae.h"
#include "memory.h"
#include "romlist.h"
#include "ersatz.h"
#include "zfile.h"
#include "custom.h"
#include "events.h"
#include "newcpu.h"
#include "autoconf.h"
#include "savestate.h"
#include "crc32.h"
#include "gui.h"

#ifdef USE_MAPPED_MEMORY
#include <sys/mman.h>
#endif

int ersatzkickfile = 0;

uae_u32 allocated_chipmem;
uae_u32 allocated_fastmem;
uae_u32 allocated_bogomem;
uae_u32 allocated_gfxmem;
uae_u32 allocated_z3fastmem;
uae_u32 allocated_a3000lmem;
uae_u32 allocated_a3000hmem;

static long chip_filepos, bogo_filepos, a3000lmem_filepos, a3000hmem_filepos;

/* Set if we notice during initialization that settings changed,
   and we must clear all memory to prevent bogus contents from confusing
   the Kickstart.  */
static int need_hardreset;

/* The address space setting used during the last reset.  */
static int last_address_space_24;

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

static uae_u32 chipmem_mask, chipmem_full_mask;
static uae_u32 kickmem_mask, extendedkickmem_mask, bogomem_mask;
static uae_u32 a3000lmem_mask, a3000hmem_mask;

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
    if (currprefs.illegal_mem) {
	if (illegal_count < 20) {
	    illegal_count++;
	    write_log ("Illegal lput at %08lx\n", addr);
	}
    }
}

void REGPARAM2 dummy_wput (uaecptr addr, uae_u32 w)
{
    if (currprefs.illegal_mem) {
	if (illegal_count < 20) {
	    illegal_count++;
	    write_log ("Illegal wput at %08lx\n", addr);
	}
    }
}

void REGPARAM2 dummy_bput (uaecptr addr, uae_u32 b)
{
    if (currprefs.illegal_mem) {
	if (illegal_count < 20) {
	    illegal_count++;
	    write_log ("Illegal bput at %08lx\n", addr);
	}
    }
}

int REGPARAM2 dummy_check (uaecptr addr, uae_u32 size)
{
    if (currprefs.illegal_mem) {
	if (illegal_count < 20) {
	    illegal_count++;
	    write_log ("Illegal check at %08lx\n", addr);
	}
    }

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

uae_u32 REGPARAM2 chipmem_agnus_wget (uaecptr addr)
{
    uae_u16 *m;

    addr &= chipmem_full_mask;
    m = (uae_u16 *)(chipmemory + addr);
    return do_get_mem_word (m);
}

void REGPARAM2 chipmem_agnus_wput (uaecptr addr, uae_u32 w)
{
    uae_u16 *m;

    addr &= chipmem_full_mask;
    if (addr >= allocated_chipmem)
	return;
    m = (uae_u16 *)(chipmemory + addr);
    do_put_mem_word (m, w);
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

/* A3000 motherboard fastmemory.  */
static uae_u8 *a3000lmemory, *a3000hmemory;
uae_u32 a3000lmem_start, a3000hmem_start;

static uae_u32 a3000lmem_lget (uaecptr) REGPARAM;
static uae_u32 a3000lmem_wget (uaecptr) REGPARAM;
static uae_u32 a3000lmem_bget (uaecptr) REGPARAM;
static void a3000lmem_lput (uaecptr, uae_u32) REGPARAM;
static void a3000lmem_wput (uaecptr, uae_u32) REGPARAM;
static void a3000lmem_bput (uaecptr, uae_u32) REGPARAM;
static int a3000lmem_check (uaecptr addr, uae_u32 size) REGPARAM;

static uae_u32 REGPARAM2 a3000lmem_lget (uaecptr addr)
{
    uae_u32 *m;
    addr &= a3000lmem_mask;
    m = (uae_u32 *)(a3000lmemory + addr);
    return do_get_mem_long (m);
}

static uae_u32 REGPARAM2 a3000lmem_wget (uaecptr addr)
{
    uae_u16 *m;
    addr &= a3000lmem_mask;
    m = (uae_u16 *)(a3000lmemory + addr);
    return do_get_mem_word (m);
}

static uae_u32 REGPARAM2 a3000lmem_bget (uaecptr addr)
{
    addr &= a3000lmem_mask;
    return a3000lmemory[addr];
}

static void REGPARAM2 a3000lmem_lput (uaecptr addr, uae_u32 l)
{
    uae_u32 *m;
    addr &= a3000lmem_mask;
    m = (uae_u32 *)(a3000lmemory + addr);
    do_put_mem_long (m, l);
}

static void REGPARAM2 a3000lmem_wput (uaecptr addr, uae_u32 w)
{
    uae_u16 *m;
    addr &= a3000lmem_mask;
    m = (uae_u16 *)(a3000lmemory + addr);
    do_put_mem_word (m, w);
}

static void REGPARAM2 a3000lmem_bput (uaecptr addr, uae_u32 b)
{
    addr &= a3000lmem_mask;
    a3000lmemory[addr] = b;
}

static int REGPARAM2 a3000lmem_check (uaecptr addr, uae_u32 size)
{
    addr &= a3000lmem_mask;
    return (addr + size) <= allocated_a3000lmem;
}

static uae_u8 *REGPARAM2 a3000lmem_xlate (uaecptr addr)
{
    addr &= a3000lmem_mask;
    return a3000lmemory + addr;
}

static uae_u32 a3000hmem_lget (uaecptr) REGPARAM;
static uae_u32 a3000hmem_wget (uaecptr) REGPARAM;
static uae_u32 a3000hmem_bget (uaecptr) REGPARAM;
static void a3000hmem_lput (uaecptr, uae_u32) REGPARAM;
static void a3000hmem_wput (uaecptr, uae_u32) REGPARAM;
static void a3000hmem_bput (uaecptr, uae_u32) REGPARAM;
static int a3000hmem_check (uaecptr addr, uae_u32 size) REGPARAM;
static uae_u8 *a3000hmem_xlate (uaecptr addr) REGPARAM;

static uae_u32 REGPARAM2 a3000hmem_lget (uaecptr addr)
{
    uae_u32 *m;
    addr &= a3000hmem_mask;
    m = (uae_u32 *)(a3000hmemory + addr);
    return do_get_mem_long (m);
}

static uae_u32 REGPARAM2 a3000hmem_wget (uaecptr addr)
{
    uae_u16 *m;
    addr &= a3000hmem_mask;
    m = (uae_u16 *)(a3000hmemory + addr);
    return do_get_mem_word (m);
}

static uae_u32 REGPARAM2 a3000hmem_bget (uaecptr addr)
{
    addr &= a3000hmem_mask;
    return a3000hmemory[addr];
}

static void REGPARAM2 a3000hmem_lput (uaecptr addr, uae_u32 l)
{
    uae_u32 *m;
    addr &= a3000hmem_mask;
    m = (uae_u32 *)(a3000hmemory + addr);
    do_put_mem_long (m, l);
}

static void REGPARAM2 a3000hmem_wput (uaecptr addr, uae_u32 w)
{
    uae_u16 *m;
    addr &= a3000hmem_mask;
    m = (uae_u16 *)(a3000hmemory + addr);
    do_put_mem_word (m, w);
}

static void REGPARAM2 a3000hmem_bput (uaecptr addr, uae_u32 b)
{
    addr &= a3000hmem_mask;
    a3000hmemory[addr] = b;
}

static int REGPARAM2 a3000hmem_check (uaecptr addr, uae_u32 size)
{
    addr &= a3000hmem_mask;
    return (addr + size) <= allocated_a3000hmem;
}

static uae_u8 *REGPARAM2 a3000hmem_xlate (uaecptr addr)
{
    addr &= a3000hmem_mask;
    return a3000hmemory + addr;
}

/* Kick memory */

uae_u8 *kickmemory;
uae_u16 kickstart_version;

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
    if (!a1000_bootrom)
	return;
    if (mode == 0) {
	a1000_kickstart_mode = 0;
	memcpy (kickmemory, kickmemory + 262144, 262144);
	kickstart_version = (kickmemory[262144 + 12] << 8) | kickmemory[262144 + 13];
    } else {
	a1000_kickstart_mode = 1;
	memcpy (kickmemory, a1000_bootrom, 262144);
	kickstart_version = 0;
    }
}

void a1000_reset (void)
{
    a1000_handle_kickstart (1);
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

static uae_u32 REGPARAM2 extendedkickmem_lget (uaecptr addr)
{
    uae_u32 *m;
    addr -= extendedkickmem_start & extendedkickmem_mask;
    addr &= extendedkickmem_mask;
    m = (uae_u32 *)(extendedkickmemory + addr);
    return do_get_mem_long (m);
}

static uae_u32 REGPARAM2 extendedkickmem_wget (uaecptr addr)
{
    uae_u16 *m;
    addr -= extendedkickmem_start & extendedkickmem_mask;
    addr &= extendedkickmem_mask;
    m = (uae_u16 *)(extendedkickmemory + addr);
    return do_get_mem_word (m);
}

static uae_u32 REGPARAM2 extendedkickmem_bget (uaecptr addr)
{
    addr -= extendedkickmem_start & extendedkickmem_mask;
    addr &= extendedkickmem_mask;
    return extendedkickmemory[addr];
}

static void REGPARAM2 extendedkickmem_lput (uaecptr addr, uae_u32 b)
{
    if (currprefs.illegal_mem)
	write_log ("Illegal extendedkickmem lput at %08lx\n", addr);
}

static void REGPARAM2 extendedkickmem_wput (uaecptr addr, uae_u32 b)
{
    if (currprefs.illegal_mem)
	write_log ("Illegal extendedkickmem wput at %08lx\n", addr);
}

static void REGPARAM2 extendedkickmem_bput (uaecptr addr, uae_u32 b)
{
    if (currprefs.illegal_mem)
	write_log ("Illegal extendedkickmem lput at %08lx\n", addr);
}

static int REGPARAM2 extendedkickmem_check (uaecptr addr, uae_u32 size)
{
    addr -= extendedkickmem_start & extendedkickmem_mask;
    addr &= extendedkickmem_mask;
    return (addr + size) <= extendedkickmem_size;
}

static uae_u8 REGPARAM2 *extendedkickmem_xlate (uaecptr addr)
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

    set_special (SPCFLAG_RESTORE_SANITY);
    /* rtarea_bank has an infinite loop in m68k space, which we use to avoid a
     * crash. */
    return rtarea_bank.xlateaddr (0xF0FFC0);
}

/* Address banks */

addrbank dummy_bank = {
    dummy_lget, dummy_wget, dummy_bget,
    dummy_lput, dummy_wput, dummy_bput,
    default_xlate, dummy_check, NULL, NULL
};

addrbank chipmem_bank = {
    chipmem_lget, chipmem_wget, chipmem_bget,
    chipmem_lput, chipmem_wput, chipmem_bput,
    chipmem_xlate, chipmem_check, NULL, "Chip memory"
};

addrbank bogomem_bank = {
    bogomem_lget, bogomem_wget, bogomem_bget,
    bogomem_lput, bogomem_wput, bogomem_bput,
    bogomem_xlate, bogomem_check, NULL, "Slow memory"
};

addrbank a3000lmem_bank = {
    a3000lmem_lget, a3000lmem_wget, a3000lmem_bget,
    a3000lmem_lput, a3000lmem_wput, a3000lmem_bput,
    a3000lmem_xlate, a3000lmem_check, NULL, "RAMSEY memory (low)"
};

addrbank a3000hmem_bank = {
    a3000hmem_lget, a3000hmem_wget, a3000hmem_bget,
    a3000hmem_lput, a3000hmem_wput, a3000hmem_bput,
    a3000hmem_xlate, a3000hmem_check, NULL, "RAMSEY memory (high)"
};

addrbank kickmem_bank = {
    kickmem_lget, kickmem_wget, kickmem_bget,
    kickmem_lput, kickmem_wput, kickmem_bput,
    kickmem_xlate, kickmem_check, NULL, "Kickstart ROM"
};

addrbank extendedkickmem_bank = {
    extendedkickmem_lget, extendedkickmem_wget, extendedkickmem_bget,
    extendedkickmem_lput, extendedkickmem_wput, extendedkickmem_bput,
    extendedkickmem_xlate, extendedkickmem_check, NULL, "Extended Kickstart ROM"
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

static const char kickstring[] = "exec.library";

static int read_kickstart (struct zfile *f, uae_u8 *mem, int size, int dochecksum, int *cloanto_rom)
{
    unsigned char buffer[20];
    int i, j, cr = 0;

    if (cloanto_rom)
	*cloanto_rom = 0;
    i = zfile_fread (buffer, 1, 11, f);
    if (strncmp ((char *) buffer, "AMIROMTYPE1", 11) != 0) {
	zfile_fseek (f, 0, SEEK_SET);
    } else {
	cr = 1;
    }

    if (cloanto_rom)
	*cloanto_rom = cr;

    i = zfile_fread (mem, 1, size, f);

    if (i != 8192 && i != 65536 && i != 131072 && i != 262144 && i != 524288 && i != 524288 * 2 && i != 524288 * 4) {
	gui_message ("Error while reading Kickstart ROM file.");
	return 0;
    }
    if (i == size / 2)
	memcpy (mem + size / 2, mem, size / 2);

    if (cr) {
	if (!decode_cloanto_rom (mem, size, i, 0))
	    return 0;
    }
    if (currprefs.cs_a1000ram) {
	int off = 0;
	a1000_bootrom = xcalloc (262144, 1);
	while (off + i < 262144) {
	    memcpy (a1000_bootrom + off, kickmemory, i);
	    off += i;
	}
	memset (kickmemory, 0, kickmem_size);
	a1000_handle_kickstart (1);
	dochecksum = 0;
	i = 524288;
    }

    zfile_fclose (f);

    for (j = 0; j < 256 && i >= 262144; j++) {
	if (!memcmp (mem + j, kickstring, strlen (kickstring) + 1))
	    break;
    }

    if (j == 256 || i < 262144)
	dochecksum = 0;
    if (dochecksum)
	kickstart_checksum (mem, size);

    return 1;
}

static int load_extendedkickstart (void)
{
    struct zfile *f;
    int size;

    if (strlen (currprefs.romextfile) == 0)
	return 0;
    f = zfile_open (currprefs.romextfile, "rb");
    if (!f) {
	write_log ("No extended Kickstart ROM found");
	return 0;
    }

    zfile_fseek (f, 0, SEEK_END);
    size = zfile_ftell (f);
    if (size > 300000)
	extendedkickmem_size = 524288;
    else
	extendedkickmem_size = 262144;
    zfile_fseek (f, 0, SEEK_SET);

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
    return 1;
}


static void kickstart_fix_checksum (uae_u8 *mem, int size)
{
    uae_u32 cksum = 0, prevck = 0;
    int i, ch = size == 524288 ? 0x7ffe8 : 0x3e;

    mem[ch] = 0;
    mem[ch + 1] = 0;
    mem[ch + 2] = 0;
    mem[ch + 3] = 0;
    for (i = 0; i < size; i+=4) {
	uae_u32 data = (mem[i] << 24) | (mem[i + 1] << 16) | (mem[i + 2] << 8) | mem[i + 3];
	cksum += data;
	if (cksum < prevck)
	    cksum++;
	prevck = cksum;
    }
    cksum ^= 0xffffffff;
    mem[ch++] = cksum >> 24;
    mem[ch++] = cksum >> 16;
    mem[ch++] = cksum >> 8;
    mem[ch++] = cksum >> 0;
}

static int patch_shapeshifter (uae_u8 *kickmemory)
{
    /* Patch Kickstart ROM for ShapeShifter - from Christian Bauer.
     * Changes 'lea $400,a0' and 'lea $1000,a0' to 'lea $3000,a0' for
     * ShapeShifter compatability.
    */
    int i, patched = 0;
    uae_u8 kickshift1[] = { 0x41, 0xf8, 0x04, 0x00 };
    uae_u8 kickshift2[] = { 0x41, 0xf8, 0x10, 0x00 };
    uae_u8 kickshift3[] = { 0x43, 0xf8, 0x04, 0x00 };

    for (i = 0x200; i < 0x300; i++) {
	if (!memcmp (kickmemory + i, kickshift1, sizeof (kickshift1)) ||
	!memcmp (kickmemory + i, kickshift2, sizeof (kickshift2)) ||
	!memcmp (kickmemory + i, kickshift3, sizeof (kickshift3))) {
	    kickmemory[i + 2] = 0x30;
	    write_log ("Kickstart KickShifted @%04.4X\n", i);
	    patched++;
	}
    }
    return patched;
}

/* disable incompatible drivers */
static int patch_residents (uae_u8 *kickmemory, int size)
{
    int i, j, patched = 0;
    char *residents[] = {
	"NCR scsi.device", 
	"scsi.device", "carddisk.device", "card.resource",
	NULL
    };
    uaecptr base = size == 524288 ? 0xf80000 : 0xfc0000;

    for (i = 0; i < size - 100; i++) {
	if (kickmemory[i] == 0x4a && kickmemory[i + 1] == 0xfc) {
	    uaecptr addr;
	    addr = (kickmemory[i + 2] << 24) | (kickmemory[i + 3] << 16) | (kickmemory[i + 4] << 8) | (kickmemory[i + 5] << 0);
	    if (addr != i + base)
		continue;
	    addr = (kickmemory[i + 14] << 24) | (kickmemory[i + 15] << 16) | (kickmemory[i + 16] << 8) | (kickmemory[i + 17] << 0);
	    if (addr >= base && addr < base + size) {
		j = 0;
		while (residents[j]) {
		    if (!memcmp (residents[j], kickmemory + addr - base, strlen (residents[j]) + 1)) {
			write_log ("KSPatcher: '%s' at %08.8X disabled\n", residents[j], i + base);
			kickmemory[i] = 0x4b; /* destroy RTC_MATCHWORD */
			patched++;
			break;
		    }
		    j++;
		}
	    }
	}
    }
    return patched;
}

static void patch_kick(void)
{
    int patched = 0;
    if (kickmem_size >= 524288 && currprefs.kickshifter)
	patched += patch_shapeshifter (kickmemory);
    patched += patch_residents (kickmemory, kickmem_size);
    if (extendedkickmemory) {
	patched += patch_residents (extendedkickmemory, extendedkickmem_size);
	if (patched)
	    kickstart_fix_checksum (extendedkickmemory, extendedkickmem_size);
    }
    if (patched)
	kickstart_fix_checksum (kickmemory, kickmem_size);
}

static int load_kickstart (void)
{
    struct zfile *f = NULL;
    int i;

    if (currprefs.rom_crc32) {
	for (i = 0;; i++) {
	    struct romlist *rl = romlist_from_idx (i, ROMTYPE_KICK, 1);
	    if (!rl)
		break;
	    if (rl->rd->crc32 == currprefs.rom_crc32) {
		f = zfile_open (rl->path, "rb");
		break;
	    }
	}
    } else
	f = zfile_open (currprefs.romfile, "rb");

    if (f == NULL) {
#if defined (AMIGA)
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

#if defined (AMIGA)
  chk_sum:
#endif
    kickstart_version = (kickmemory[12] << 8) | kickmemory[13];

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
		printf ("NATMEM: Failure to delete mapping at %08x (size %08x, delsize %08x)\n", start, x->size, size);
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
	printf ("NATMEM: Failure to map existing at %08x (%p)\n", start, base);
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
	int memsize;
	if (chipmemory)
	    mapped_free (chipmemory);
	chipmemory = 0;

	memsize = allocated_chipmem = currprefs.chipmem_size;
	chipmem_mask = allocated_chipmem - 1;

	if (memsize < 0x100000)
	    memsize = 0x100000;
	chipmemory = mapped_malloc (memsize, "chip");
	if (chipmemory == 0) {
	    write_log ("Fatal error: out of memory for chipmem.\n");
	    allocated_chipmem = 0;
	} else {
	    need_hardreset = 1;
	    if (memsize != allocated_chipmem)
		memset (chipmemory + allocated_chipmem, 0xff, memsize - allocated_chipmem);
	}
    }

    chipmem_full_mask = allocated_chipmem - 1;
    if ((currprefs.chipset_mask & CSMASK_ECS_AGNUS) && allocated_chipmem < 0x100000)
	chipmem_full_mask = 0x100000 - 1;

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
	need_hardreset = 1;
    }

    if (allocated_a3000lmem != currprefs.mbresmem_low_size) {
	if (a3000lmemory)
	    mapped_free (a3000lmemory);
	a3000lmemory = 0;

	allocated_a3000lmem = currprefs.mbresmem_low_size;
	a3000lmem_mask = allocated_a3000lmem - 1;
	a3000lmem_start = 0x08000000 - allocated_a3000lmem;
	if (allocated_a3000lmem) {
	    a3000lmemory = mapped_malloc (allocated_a3000lmem, "ramsey_low");
	    if (a3000lmemory == 0) {
		write_log ("Out of memory for a3000lowmem.\n");
		allocated_a3000lmem = 0;
	    }
	}
	need_hardreset = 1;
    }

    if (allocated_a3000hmem != currprefs.mbresmem_high_size) {
	if (a3000hmemory)
	    mapped_free (a3000hmemory);
	a3000hmemory = 0;

	allocated_a3000hmem = currprefs.mbresmem_high_size;
	a3000hmem_mask = allocated_a3000hmem - 1;
	a3000hmem_start = 0x08000000;
	if (allocated_a3000hmem) {
	    a3000hmemory = mapped_malloc (allocated_a3000hmem, "ramsey_high");
	    if (a3000hmemory == 0) {
		write_log ("Out of memory for a3000highmem.\n");
		allocated_a3000hmem = 0;
	    }
	}
	need_hardreset = 1;
    }

    if (savestate_state == STATE_RESTORE) {
	restore_ram (chip_filepos, chipmemory);
	if (allocated_bogomem > 0) {
	    restore_ram (bogo_filepos, bogomemory);
	}
	if (allocated_a3000lmem > 0)
	    restore_ram (a3000lmem_filepos, a3000lmemory);
	if (allocated_a3000hmem > 0)
	    restore_ram (a3000hmem_filepos, a3000hmemory);
    }
    chipmem_bank.baseaddr = chipmemory;
    bogomem_bank.baseaddr = bogomemory;
    a3000lmem_bank.baseaddr = a3000lmemory;
    a3000hmem_bank.baseaddr = a3000hmemory;
}

void map_overlay (int chip)
{
    int i = allocated_chipmem > 0x200000 ? (allocated_chipmem >> 16) : 32;
    addrbank *cb;

    cb = &chipmem_bank;
    if (chip)
	map_banks (cb, 0, i, allocated_chipmem);
    else
	map_banks (&kickmem_bank, 0, i, 0x80000);
    if (savestate_state != STATE_RESTORE)
	m68k_setpc (m68k_getpc ());
}

void memory_reset (void)
{
    int bnk, bnk_end;

#ifdef NATMEM_OFFSET
    delete_shmmaps (0, 0xFFFF0000);
#endif
    init_mem_banks ();

    currprefs.chipmem_size = changed_prefs.chipmem_size;
    currprefs.bogomem_size = changed_prefs.bogomem_size;
    currprefs.mbresmem_low_size = changed_prefs.mbresmem_low_size;
    currprefs.mbresmem_high_size = changed_prefs.mbresmem_high_size;
    currprefs.chipset_mask = changed_prefs.chipset_mask;
    currprefs.cs_a1000ram = changed_prefs.cs_a1000ram;
    currprefs.cs_fatgaryrev = changed_prefs.cs_fatgaryrev;
    currprefs.cs_ramseyrev = changed_prefs.cs_ramseyrev;
    currprefs.cs_ide = changed_prefs.cs_ide;
    currprefs.bootrom = changed_prefs.bootrom;

    need_hardreset = 0;
    /* Use changed_prefs, as m68k_reset is called later.  */
    if (last_address_space_24 != changed_prefs.address_space_24)
	need_hardreset = 1;

    last_address_space_24 = changed_prefs.address_space_24;
    allocate_memory ();

    if (currprefs.rom_crc32 != changed_prefs.rom_crc32
	|| strcmp (currprefs.romfile, changed_prefs.romfile) != 0
	|| strcmp (currprefs.keyfile, changed_prefs.keyfile) != 0)
    {
	ersatzkickfile = 0;
	a1000_handle_kickstart (0);
	free (a1000_bootrom);
	a1000_bootrom = 0;
	a1000_kickstart_mode = 0;

	currprefs.rom_crc32 = changed_prefs.rom_crc32;
	memcpy (currprefs.romfile, changed_prefs.romfile, sizeof currprefs.romfile);
	memcpy (currprefs.keyfile, changed_prefs.keyfile, sizeof currprefs.keyfile);
	need_hardreset = 1;
	if (!load_kickstart ()) {
	    init_ersatz_rom (kickmemory);
	    ersatzkickfile = 1;
	}
	patch_kick ();
    }

    map_banks (&custom_bank, 0xC0, 0xE0 - 0xC0, 0);
    map_banks (&cia_bank, 0xA0, 32, 0);
    if (!currprefs.cs_a1000ram)
	/* D80000 - DDFFFF not mapped (A1000 = custom chips) */
	map_banks (&dummy_bank, 0xD8, 6, 0);

    /* map "nothing" to 0x200000 - 0x9FFFFF (0xBEFFFF if PCMCIA or AGA) */
    bnk = allocated_chipmem >> 16;
    if (bnk < 0x20)
	bnk = 0x20;
    bnk_end = (currprefs.chipset_mask & CSMASK_AGA) ? 0xBF : 0xA0;
    map_banks (&dummy_bank, bnk, bnk_end - bnk, 0);
    if (currprefs.chipset_mask & CSMASK_AGA)
	map_banks (&dummy_bank, 0xc0, 0xd8 - 0xc0, 0);

    if (bogomemory != 0) {
	int t = allocated_bogomem >> 16;
	if (t > 0x1C)
	    t = 0x1C;
	if (t > 0x10 && ((currprefs.chipset_mask & CSMASK_AGA) || currprefs.cpu_model >= 68020))
	    t = 0x10;
	map_banks (&bogomem_bank, 0xC0, t, allocated_bogomem);
    }
    if (currprefs.cs_ide) {
	if (currprefs.cs_ide == 1) {
	    map_banks (&gayle_bank, 0xD8, 6, 0);
	    map_banks (&gayle2_bank, 0xDD, 2, 0);
	    // map_banks (&gayle_attr_bank, 0xA0, 8, 0); only if PCMCIA card inserted */
	}
	if (currprefs.cs_ide == 2) {
	    map_banks (&gayle_bank, 0xDD, 1, 0);
	}
	if (currprefs.cs_ide < 0) {
	    map_banks (&gayle_bank, 0xD8, 6, 0);
	    map_banks (&gayle_bank, 0xDD, 1, 0);
	}
    }
    if (currprefs.cs_rtc)
	map_banks (&clock_bank, 0xDC, 1, 0);
    if (currprefs.cs_fatgaryrev >= 0 || currprefs.cs_ramseyrev >= 0)
	map_banks (&mbres_bank, 0xDE, 1, 0);

    if (a3000lmemory != 0)
	map_banks (&a3000lmem_bank, a3000lmem_start >> 16, allocated_a3000lmem >> 16, 0);
    if (a3000hmemory != 0)
	map_banks (&a3000hmem_bank, a3000hmem_start >> 16, allocated_a3000hmem >> 16, 0);

    if (currprefs.bootrom) {
	uae_boot_rom = 1;
	map_banks (&rtarea_bank, RTAREA_BASE >> 16, 1, 0);
    } else
	uae_boot_rom = 0;

    map_banks (&kickmem_bank, 0xF8, 8, 0);

    a1000_reset ();
    map_banks (&expamem_bank, 0xE8, 1, 0);

    /* Map the chipmem into all of the lower 8MB */
    map_overlay (1);

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

    if (need_hardreset)
	memory_hardreset ();
}

void memory_init (void)
{
    allocated_chipmem = 0;
    allocated_bogomem = 0;
    kickmemory = 0;
    extendedkickmemory = 0;
    chipmemory = 0;
    allocated_a3000lmem = allocated_a3000hmem = 0;
    a3000lmemory = a3000hmemory = 0;
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
    if (a3000lmemory)
	mapped_free (a3000lmemory);
    if (a3000hmemory)
	mapped_free (a3000hmemory);
    if (bogomemory)
	mapped_free (bogomemory);
    if (kickmemory)
	mapped_free (kickmemory);
    if (a1000_bootrom)
	free (a1000_bootrom);
    if (chipmemory)
	mapped_free (chipmemory);

    a3000lmemory = a3000hmemory = 0;
    bogomemory = 0;
    kickmemory = 0;
    a1000_bootrom = 0;
    chipmemory = 0;
}

void memory_hardreset (void)
{
    if (savestate_state == STATE_RESTORE)
	return;
    if (chipmemory)
	memset (chipmemory, 0, allocated_chipmem);
    if (bogomemory)
	memset (bogomemory, 0, allocated_bogomem);
    if (a3000lmemory)
	memset (a3000lmemory, 0, allocated_a3000lmem);
    if (a3000hmemory)
	memset (a3000hmemory, 0, allocated_a3000hmem);
    expansion_clear ();
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
    /* Already in currprefs, since we get called after m68k_reset.  */
    if (last_address_space_24)
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

uae_u8 *save_a3000lram (int *len)
{
    *len = allocated_a3000lmem;
    return a3000lmemory;
}

uae_u8 *save_a3000hram (int *len)
{
    *len = allocated_a3000hmem;
    return a3000hmemory;
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

void restore_a3000lram (int len, size_t filepos)
{
    a3000lmem_filepos = filepos;
    changed_prefs.mbresmem_low_size = len;
}

void restore_a3000hram (int len, size_t filepos)
{
    a3000hmem_filepos = filepos;
    changed_prefs.mbresmem_high_size = len;
}

const uae_u8 *restore_rom (const uae_u8 *src)
{
    struct romdata *rom;
    uae_u32 crc32;
    char *romname;
    int i;

    restore_u32 ();
    restore_u32 ();
    restore_u32 ();
    restore_u32 ();
    crc32 = restore_u32 ();

    rom = getromdatabycrc (crc32);
    if (rom)
	changed_prefs.rom_crc32 = crc32;

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

/* memory helpers */

static int addr_valid (char *txt, uaecptr addr, uae_u32 len)
{
    addrbank *ab = &get_mem_bank (addr);
    if (ab == 0 || addr < 0x100
	|| len < 0 || len > 16777215 || !valid_address (addr, len)) {
	write_log ("corrupt %s pointer %x (%d) detected!\n", txt, addr, len);
	return 0;
    }
    return 1;
}

void memcpyha_safe (uaecptr dst, const uae_u8 *src, int size)
{
    if (!addr_valid ("memcpyha", dst, size))
	return;
    while (size--)
	put_byte (dst++, *src++);
}

void memcpyha (uaecptr dst, const uae_u8 *src, int size)
{
    while (size--)
	put_byte (dst++, *src++);
}

void memcpyah_safe (uae_u8 *dst, uaecptr src, int size)
{
    if (!addr_valid ("memcpyah", src, size))
	return;
    while (size--)
	*dst++ = get_byte (src++);
}

void memcpyah (uae_u8 *dst, uaecptr src, int size)
{
    while (size--)
	*dst++ = get_byte (src++);
}

char *strcpyah_safe (char *dst, uaecptr src)
{
    char *res = dst;
    uae_u8 b;
    do {
	if (!addr_valid ("strcpyah", src, 1))
	    return res;
	b = get_byte (src++);
	*dst++ = b;
    } while (b);
    return res;
}

uaecptr strcpyha_safe (uaecptr dst, const char *src)
{
    uaecptr res = dst;
    uae_u8 b;
    do {
	if (!addr_valid ("strcpyha", dst, 1))
	    return res;
	b = *src++;
	put_byte (dst++, b);
    } while (b);
    return res;
}

uae_u32 strncpyha (uae_u32 dst, const char *src, int size)
{
    uae_u32 res = dst;
    while (size--) {
	put_byte (dst++, *src);
	if (!*src++)
	    return res;
    }
    return res;
}

uae_u32 strncpyha_safe (uae_u32 dst, const char *src, int size)
{
    uae_u32 res = dst;
    while (size--) {
	if (!addr_valid ("strcpyha", dst, 1))
	    return res;
	put_byte (dst++, *src);
	if (!*src++)
	    return res;
    }
    return res;
}

 /*
  * UAE - The Un*x Amiga Emulator
  *
  * AutoConfig devices
  *
  * Copyright 1995, 1996 Bernd Schmidt
  * Copyright 1996 Ed Hanway
  */

#include "sysconfig.h"
#include "sysdeps.h"

#include "options.h"
#include "uae.h"
#include "memory.h"
#include "custom.h"
#include "newcpu.h"
#include "autoconf.h"
#include "traps.h"

/* Commonly used autoconfig strings */

uaecptr EXPANSION_explibname, EXPANSION_doslibname, EXPANSION_uaeversion;
uaecptr EXPANSION_uaedevname, EXPANSION_explibbase = 0, EXPANSION_haveV36;
uaecptr EXPANSION_bootcode, EXPANSION_nullfunc;
uaecptr EXPANSION_cddevice;

/* ROM tag area memory access */

uae_u8 *rtarea;

static uae_u32 rtarea_lget (uaecptr) REGPARAM;
static uae_u32 rtarea_wget (uaecptr) REGPARAM;
static uae_u32 rtarea_bget (uaecptr) REGPARAM;
static void rtarea_lput (uaecptr, uae_u32) REGPARAM;
static void rtarea_wput (uaecptr, uae_u32) REGPARAM;
static void rtarea_bput (uaecptr, uae_u32) REGPARAM;
static uae_u8 *rtarea_xlate (uaecptr) REGPARAM;

static uae_u8 REGPARAM2 *rtarea_xlate (uaecptr addr)
{
    addr &= 0xFFFF;
    return rtarea + addr;
}

static uae_u32 REGPARAM2 rtarea_lget (uaecptr addr)
{
    addr &= 0xFFFF;
    return (uae_u32)(rtarea_wget (addr) << 16) + rtarea_wget (addr+2);
}

static uae_u32 REGPARAM2 rtarea_wget (uaecptr addr)
{
    addr &= 0xFFFF;
    return (rtarea[addr]<<8) + rtarea[addr+1];
}

static uae_u32 REGPARAM2 rtarea_bget (uaecptr addr)
{
    addr &= 0xFFFF;
    return rtarea[addr];
}

static void REGPARAM2 rtarea_lput (uaecptr addr, uae_u32 value)
{
}

static void REGPARAM2 rtarea_wput (uaecptr addr, uae_u32 value)
{
}

static void REGPARAM2 rtarea_bput (uaecptr addr, uae_u32 value)
{
}

addrbank rtarea_bank = {
    rtarea_lget, rtarea_wget, rtarea_bget,
    rtarea_lput, rtarea_wput, rtarea_bput,
    rtarea_xlate, default_check, NULL
};

/* some quick & dirty code to fill in the rt area and save me a lot of
 * scratch paper
 */

static int rt_addr = 0;
static int rt_straddr = 0xFF00 - 2;

uae_u32 addr (int ptr)
{
    return (uae_u32)ptr + RTAREA_BASE;
}

void db (uae_u8 data)
{
    rtarea[rt_addr++] = data;
}

void dw (uae_u16 data)
{
    rtarea[rt_addr++] = data >> 8;
    rtarea[rt_addr++] = data;
}

void dl (uae_u32 data)
{
    rtarea[rt_addr++] = data >> 24;
    rtarea[rt_addr++] = data >> 16;
    rtarea[rt_addr++] = data >> 8;
    rtarea[rt_addr++] = data;
}

/* store strings starting at the end of the rt area and working
 * backward.  store pointer at current address
 */

uae_u32 ds (char *str)
{
    int len = strlen (str) + 1;

    rt_straddr -= len;
    strcpy ((char *)rtarea + rt_straddr, str);

    return addr (rt_straddr);
}

void calltrap (uae_u32 n)
{
    dw (0xA000 + n);
}

void org (uae_u32 a)
{
    rt_addr = a - RTAREA_BASE;
}

uae_u32 here (void)
{
    return addr (rt_addr);
}

void align (int b)
{
    rt_addr = (rt_addr + b - 1) & ~(b - 1);
}

static uae_u32 nullfunc (TrapContext *dummy)
{
    write_log ("Null function called\n");
    return 0;
}

static uae_u32 getchipmemsize (TrapContext *dummy)
{
    return allocated_chipmem;
}

static uae_u32 uae_puts (TrapContext *dummy)
{
    puts (get_real_address (m68k_areg (regs, 0)));
    return 0;
}

/* Used to keep the m68k emulation sane when we hit a call to
 * default_xlate.  */
static void m68k_infinite_loop (void)
{
    m68k_setpc (0xF0FFC0);
}

static void rtarea_init_mem (void)
{
    rtarea = mapped_malloc (0x10000, "rtarea");
    if (!rtarea) {
	write_log ("virtual memory exhausted (rtarea)!\n");
	abort ();
    }
    rtarea_bank.baseaddr = rtarea;
}


void rtarea_init (void)
{
    uae_u32 a;
    char uaever[100];

    rtarea_init_mem ();

    sprintf (uaever, "uae-%s", PACKAGE_VERSION);

    EXPANSION_uaeversion = ds (uaever);
    EXPANSION_explibname = ds ("expansion.library");
    EXPANSION_doslibname = ds ("dos.library");
    EXPANSION_uaedevname = ds ("uae.device");

    deftrap (NULL); /* Generic emulator trap */

    EXPANSION_nullfunc = here ();
    calltrap (deftrap (nullfunc));
    dw (RTS);

    a = here();
    /* Dummy trap - removing this breaks the filesys emulation. */
    org (RTAREA_BASE + 0xFF00);
    calltrap (deftrap2 (nullfunc, TRAPFLAG_NO_RETVAL, ""));

    org (RTAREA_BASE + 0xFF80);
    calltrap (deftrap2 (getchipmemsize, TRAPFLAG_DORET, ""));

    org (RTAREA_BASE + 0xFF10);
    calltrap (deftrap2 (uae_puts, TRAPFLAG_NO_RETVAL, ""));
    dw (RTS);

    org (RTAREA_BASE + 0xFFC0);
    calltrap (deftrap2 (m68k_infinite_loop, TRAPFLAG_NO_RETVAL, ""));

    org (a);

    filesys_install_code ();

    uae_boot_rom_size = here() - RTAREA_BASE;
    init_extended_traps ();
}

volatile int uae_int_requested = 0;

void set_uae_int_flag (void)
{
    rtarea[0xFFFB] = uae_int_requested;
}

void rtarea_setup (void)
{
}

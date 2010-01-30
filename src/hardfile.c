 /*
  * UAE - The Un*x Amiga Emulator
  *
  * Hardfile emulation
  *
  * Copyright 1995 Bernd Schmidt
  */

#include "sysconfig.h"
#include "sysdeps.h"

#include "options.h"
#include "memory.h"
#include "custom.h"
#include "newcpu.h"
#include "disk.h"
#include "traps.h"
#include "autoconf.h"
#include "execlib.h"
#include "filesys.h"

#define CMD_INVALID	0
#define CMD_RESET	1
#define CMD_READ	2
#define CMD_WRITE	3
#define CMD_UPDATE	4
#define CMD_CLEAR	5
#define CMD_STOP	6
#define CMD_START	7
#define CMD_FLUSH	8
#define CMD_MOTOR	9
#define CMD_SEEK	10
#define CMD_FORMAT	11
#define CMD_REMOVE	12
#define CMD_CHANGENUM	13
#define CMD_CHANGESTATE	14
#define CMD_PROTSTATUS	15
#define CMD_GETDRIVETYPE 18
#define CMD_GETNUMTRACKS 19
#define CMD_ADDCHANGEINT 20
#define CMD_REMCHANGEINT 21
#define CMD_GETGEOMETRY	22

/* Trackdisk64 support */
#define TD_READ64 24
#define TD_WRITE64 25
#define TD_SEEK64 26
#define TD_FORMAT64 27

/* New Style Devices (NSD) support */
#define DRIVE_NEWSTYLE 0x4E535459L   /* 'NSTY' */
#define NSCMD_DEVICEQUERY 0x4000
#define NSCMD_TD_READ64 0xc000
#define NSCMD_TD_WRITE64 0xc001
#define NSCMD_TD_SEEK64 0xc002
#define NSCMD_TD_FORMAT64 0xc003

static int opencount = 0;

static uae_u32 nscmd_cmd;

int hdf_read (struct hardfiledata *hfd, void *buffer, uae_u64 offset, int len)
{
    int result = 0;

    if (fseek (hfd->fd, offset, SEEK_SET) != 0)
	return 0;
    do {
	int t = fread (buffer, 1, len, hfd->fd);
	result += t;
	len -= t;
	if (t == 0)
	    return result;
    } while (len > 0);
    return result;
}

int hdf_write (struct hardfiledata *hfd, void *buffer, uae_u64 offset, int len)
{
    int result = 0;

    if (fseek (hfd->fd, offset, SEEK_SET) != 0)
	return 0;
    do {
	int t = fwrite (buffer, 1, len, hfd->fd);
	result += t;
	len -= t;
	if (t == 0)
	    return result;
    } while (len > 0);
    return result;
}

static uae_u64 cmd_readx (struct hardfiledata *hfd, uae_u8 *dataptr, uae_u64 offset, uae_u64 len)
{
    return hdf_read (hfd, dataptr, offset, len);
}
static uae_u64 cmd_read (struct hardfiledata *hfd, uaecptr dataptr, uae_u64 offset, uae_u64 len)
{
    addrbank *bank_data = &get_mem_bank (dataptr);
    if (!bank_data || !bank_data->check (dataptr, len))
	return 0;
    return cmd_readx (hfd, bank_data->xlateaddr (dataptr), offset, len);
}
static uae_u64 cmd_writex (struct hardfiledata *hfd, uae_u8 *dataptr, uae_u64 offset, uae_u64 len)
{
    return hdf_write (hfd, dataptr, offset, len);
}

static uae_u64 cmd_write (struct hardfiledata *hfd, uaecptr dataptr, uae_u64 offset, uae_u64 len)
{
    addrbank *bank_data = &get_mem_bank (dataptr);
    if (!bank_data || !bank_data->check (dataptr, len))
	return 0;
    return cmd_writex (hfd, bank_data->xlateaddr (dataptr), offset, len);
}

static uae_u32 hardfile_open (TrapContext *dummy)
{
    uaecptr tmp1 = m68k_areg (regs, 1); /* IOReq */

    /* Check unit number */
    if (get_hardfile_data (m68k_dreg (regs, 0))) {
	opencount++;
	put_word (m68k_areg (regs, 6)+32, get_word (m68k_areg (regs, 6)+32) + 1);
	put_long (tmp1 + 24, m68k_dreg (regs, 0)); /* io_Unit */
	put_byte (tmp1 + 31, 0); /* io_Error */
	put_byte (tmp1 + 8, 7); /* ln_type = NT_REPLYMSG */
	return 0;
    }

    put_long (tmp1 + 20, (uae_u32)-1);
    put_byte (tmp1 + 31, (uae_u8)-1);
    return (uae_u32)-1;
}

static uae_u32 hardfile_close (TrapContext *dummy)
{
    opencount--;
    put_word (m68k_areg (regs, 6) + 32, get_word (m68k_areg (regs, 6) + 32) - 1);

    return 0;
}

static uae_u32 hardfile_expunge (TrapContext *dummy)
{
    return 0; /* Simply ignore this one... */
}

static uae_u32 hardfile_beginio (TrapContext *dummy)
{
    uae_u32 request, len, dataptr, offset, actual = 0, cmd;
    uae_u32 retval = m68k_dreg (regs, 0);
    int unit;
    struct hardfiledata *hfd;

    request = m68k_areg (regs, 1);
    unit = get_long (request + 24);
#undef DEBUGME
#ifdef DEBUGME
    printf ("hardfile: unit = %d\n", unit);
    printf ("hardfile: request = %08lx\n", (unsigned long)request);
    printf ("hardfile: cmd  = %d\n", (int)get_word (request + 28));
#endif
    hfd = get_hardfile_data (unit);

    put_byte (request + 8, NT_MESSAGE);
    put_byte (request + 31, 0); /* no error yet */
    cmd = get_word (request + 28); /* io_Command */
/*    put_byte (request + 30, get_byte (request + 30) & ~1);*/
    dataptr = get_long (request + 40);

    switch (cmd) {
     case CMD_READ:
	if (dataptr & 1)
	    goto bad_command;
	offset = get_long (request + 44);
	if (offset & 511)
	    goto bad_command;
	len = get_long (request + 36); /* io_Length */
	if (len & 511)
	    goto bad_command;
	if (len + offset > (uae_u32)hfd->size)
	    goto bad_command;

	actual = (uae_u32)cmd_read (hfd, dataptr, offset, len);
	put_long (request + 32, actual);
	break;

     case CMD_WRITE:
     case CMD_FORMAT:
	if (dataptr & 1)
	    goto bad_command;
	offset = get_long (request + 44);
	if (offset & 511)
	    goto bad_command;
	len = get_long (request + 36); /* io_Length */
	if (len & 511)
	    goto bad_command;
	if (len + offset > (uae_u32)hfd->size)
	    goto bad_command;

	actual = (uae_u32)cmd_write (hfd, dataptr, offset, len);
	put_long (request + 32, actual); /* set io_Actual */
	break;

	bad_command:
	break;

     case NSCMD_DEVICEQUERY:
	put_long (dataptr + 4, 16); /* size */
	put_word (dataptr + 8, 5); /* NSDEVTYPE_TRACKDISK */
	put_word (dataptr + 10, 0);
	put_long (dataptr + 12, nscmd_cmd);
	put_long (request + 32, 16);
	break;

     case CMD_GETDRIVETYPE:
	printf ("Shouldn't happen\n");
	put_long (request + 32, 1); /* not exactly a 3.5" drive, but... */
	break;

     case CMD_GETNUMTRACKS:
	printf ("Shouldn't happen 2\n");
	put_long (request + 32, 0);
	break;

	/* Some commands that just do nothing and return zero */
     case CMD_UPDATE:
     case CMD_CLEAR:
     case 9: /* Motor */
     case 10: /* Seek */
     case 12: /* Remove */
     case 13: /* ChangeNum */
     case 14: /* ChangeStatus */
     case 15: /* ProtStatus */
     case 20: /* AddChangeInt */
     case 21: /* RemChangeInt */
	put_long (request + 32, 0); /* io_Actual */
	retval = 0;
	break;

     default:
	/* Command not understood. */
	put_byte (request + 31, (uae_u8)-3); /* io_Error */
	retval = 0;
	break;
    }
#if 0
    if ((get_byte (request + 30) & 1) == 0) {
	/* Not IOF_QUICK -- need to ReplyMsg */
	m68k_areg (regs, 1) = request;
	CallLib (get_long (4), -378);
    }
#endif
    return retval;
}

static uae_u32 hardfile_abortio (TrapContext *dummy)
{
    return (uae_u32)-3;
}

void hardfile_install (void)
{
    uae_u32 functable, datatable;
    uae_u32 initcode, openfunc, closefunc, expungefunc;
    uae_u32 beginiofunc, abortiofunc;

    ROM_hardfile_resname = ds ("uaehf.device");
    ROM_hardfile_resid = ds ("UAE hardfile.device 0.2");

    nscmd_cmd = here ();
    dw (CMD_READ);
    dw (CMD_WRITE);
    dw (CMD_FORMAT);
    dw (NSCMD_DEVICEQUERY);
    dw (CMD_GETDRIVETYPE);
    dw (CMD_CLEAR);
    dw (CMD_UPDATE);
    dw (CMD_MOTOR);
    dw (CMD_SEEK);
    dw (CMD_CHANGENUM);
    dw (CMD_CHANGESTATE);
    dw (CMD_PROTSTATUS);
    dw (CMD_ADDCHANGEINT);
    dw (CMD_REMCHANGEINT);
    dw (0);

    /* initcode */
#if 0
    initcode = here ();
    calltrap (deftrap (hardfile_init)); dw (RTS);
#else
    initcode = filesys_initcode;
#endif
    /* Open */
    openfunc = here ();
    calltrap (deftrap (hardfile_open)); dw (RTS);

    /* Close */
    closefunc = here ();
    calltrap (deftrap (hardfile_close)); dw (RTS);

    /* Expunge */
    expungefunc = here ();
    calltrap (deftrap (hardfile_expunge)); dw (RTS);

    /* BeginIO */
    beginiofunc = here ();
    calltrap (deftrap (hardfile_beginio));
    dw (0x48E7); dw (0x8002); /* movem.l d0/a6,-(a7) */
    dw (0x0829); dw (0); dw (30); /* btst #0,30(a1) */
    dw (0x6608); /* bne.b +8 */
    dw (0x2C78); dw (0x0004); /* move.l 4,a6 */
    dw (0x4EAE); dw (-378); /* jsr ReplyMsg(a6) */
    dw (0x4CDF); dw (0x4001); /* movem.l (a7)+,d0/a6 */
    dw (RTS);

    /* AbortIO */
    abortiofunc = here ();
    calltrap (deftrap (hardfile_abortio)); dw (RTS);

    /* FuncTable */
    functable = here ();
    dl (openfunc); /* Open */
    dl (closefunc); /* Close */
    dl (expungefunc); /* Expunge */
    dl (EXPANSION_nullfunc); /* Null */
    dl (beginiofunc); /* BeginIO */
    dl (abortiofunc); /* AbortIO */
    dl (0xFFFFFFFFul); /* end of table */

    /* DataTable */
    datatable = here ();
    dw (0xE000); /* INITBYTE */
    dw (0x0008); /* LN_TYPE */
    dw (0x0300); /* NT_DEVICE */
    dw (0xC000); /* INITLONG */
    dw (0x000A); /* LN_NAME */
    dl (ROM_hardfile_resname);
    dw (0xE000); /* INITBYTE */
    dw (0x000E); /* LIB_FLAGS */
    dw (0x0600); /* LIBF_SUMUSED | LIBF_CHANGED */
    dw (0xD000); /* INITWORD */
    dw (0x0014); /* LIB_VERSION */
    dw (0x0004); /* 0.4 */
    dw (0xD000);
    dw (0x0016); /* LIB_REVISION */
    dw (0x0000);
    dw (0xC000);
    dw (0x0018); /* LIB_IDSTRING */
    dl (ROM_hardfile_resid);
    dw (0x0000); /* end of table */

    ROM_hardfile_init = here ();
    dl (0x00000100); /* ??? */
    dl (functable);
    dl (datatable);
    dl (initcode);
}

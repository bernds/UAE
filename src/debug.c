 /*
  * UAE - The Un*x Amiga Emulator
  *
  * Debugger
  *
  * (c) 1995 Bernd Schmidt
  *
  */

#include "sysconfig.h"
#include "sysdeps.h"

#include <ctype.h>
#include <signal.h>

#include "config.h"
#include "options.h"
#include "threaddep/thread.h"
#include "uae.h"
#include "memory.h"
#include "custom.h"
#include "newcpu.h"
#include "debug.h"
#include "cia.h"
#include "xwin.h"
#include "gui.h"
#include "identify.h"

static int debugger_active = 0;
static uaecptr skipaddr;
static int do_skip;
int debugging = 0;

static FILE *logfile;

void activate_debugger (void)
{
    if (logfile)
	fclose (logfile);
    logfile = 0;
    do_skip = 0;
    if (debugger_active)
	return;
    debugger_active = 1;
    set_special (SPCFLAG_BRK);
    debugging = 1;
    /* use_debugger = 1; */
}

int firsthist = 0;
int lasthist = 0;
#ifdef NEED_TO_DEBUG_BADLY
struct regstruct history[MAX_HIST];
union flagu historyf[MAX_HIST];
#else
uaecptr history[MAX_HIST];
#endif

static void ignore_ws (char **c)
{
    while (**c && isspace(**c)) (*c)++;
}

static uae_u32 readhex (char **c)
{
    uae_u32 val = 0;
    char nc;

    ignore_ws (c);

    while (isxdigit(nc = **c)) {
	(*c)++;
	val *= 16;
	nc = toupper(nc);
	if (isdigit(nc)) {
	    val += nc - '0';
	} else {
	    val += nc - 'A' + 10;
	}
    }
    return val;
}

static uae_u32 readint (char **c)
{
    uae_u32 val = 0;
    char nc;
    int negative = 0;

    ignore_ws (c);

    if (**c == '-')
	negative = 1, (*c)++;
    while (isdigit(nc = **c)) {
	(*c)++;
	val *= 10;
	val += nc - '0';
    }
    return val * (negative ? -1 : 1);
}

static char next_char( char **c)
{
    ignore_ws (c);
    return *(*c)++;
}

static int more_params (char **c)
{
    ignore_ws (c);
    return (**c) != 0;
}

static void dumpmem (uaecptr addr, uaecptr *nxmem, int lines)
{
    broken_in = 0;
    for (;lines-- && !broken_in;) {
	int i;
	printf ("%08lx ", addr);
	for (i = 0; i < 16; i++) {
	    printf ("%04x ", get_word(addr)); addr += 2;
	}
	printf ("\n");
    }
    *nxmem = addr;
}

static void foundmod (uae_u32 ptr, char *type)
{
    char name[21];
    uae_u8 *ptr2 = chipmemory + ptr;
    int i,length;

    printf ("Found possible %s module at 0x%lx.\n", type, ptr);
    memcpy (name, ptr2, 20);
    name[20] = '\0';

    /* Browse playlist */
    length = 0;
    for (i = 0x3b8; i < 0x438; i++)
	if (ptr2[i] > length)
	    length = ptr2[i];

    length = (length+1)*1024 + 0x43c;

    /* Add sample lengths */
    ptr2 += 0x2A;
    for (i = 0; i < 31; i++, ptr2 += 30)
	length += 2*((ptr2[0]<<8)+ptr2[1]);
    
    printf ("Name \"%s\", Length 0x%lx bytes.\n", name, length);
}

static void modulesearch (void)
{
    uae_u8 *p = get_real_address (0);
    uae_u32 ptr;

    for (ptr = 0; ptr < allocated_chipmem - 40; ptr += 2, p += 2) {
	/* Check for Mahoney & Kaktus */
	/* Anyone got the format of old 15 Sample (SoundTracker)modules? */
	if (ptr >= 0x438 && p[0] == 'M' && p[1] == '.' && p[2] == 'K' && p[3] == '.')
	    foundmod (ptr - 0x438, "ProTracker (31 samples)");

	if (ptr >= 0x438 && p[0] == 'F' && p[1] == 'L' && p[2] == 'T' && p[3] == '4')
	    foundmod (ptr - 0x438, "Startrekker");

	if (strncmp ((char *)p, "SMOD", 4) == 0) {
	    printf ("Found possible FutureComposer 1.3 module at 0x%lx, length unknown.\n", ptr);
	}
	if (strncmp ((char *)p, "FC14", 4) == 0) {
	    printf ("Found possible FutureComposer 1.4 module at 0x%lx, length unknown.\n", ptr);
	}
	if (p[0] == 0x48 && p[1] == 0xe7 && p[4] == 0x61 && p[5] == 0
	    && p[8] == 0x4c && p[9] == 0xdf && p[12] == 0x4e && p[13] == 0x75
	    && p[14] == 0x48 && p[15] == 0xe7 && p[18] == 0x61 && p[19] == 0
	    && p[22] == 0x4c && p[23] == 0xdf && p[26] == 0x4e && p[27] == 0x75) {
	    printf ("Found possible Whittaker module at 0x%lx, length unknown.\n", ptr);
	}
	if (p[4] == 0x41 && p[5] == 0xFA) {
	    int i;

	    for (i = 0; i < 0x240; i += 2)
		if (p[i] == 0xE7 && p[i + 1] == 0x42 && p[i + 2] == 0x41 && p[i + 3] == 0xFA)
		    break;
	    if (i < 0x240) {
		uae_u8 *p2 = p + i + 4;
		for (i = 0; i < 0x30; i += 2)
		    if (p2[i] == 0xD1 && p2[i + 1] == 0xFA) {
			printf ("Found possible MarkII module at %lx, length unknown.\n", ptr);
		    }
	    }
	}
    }
}

static void dump_traps (void)
{
    int i;
    for (i = 0; trap_labels[i].name; i++) {
	printf("$%02x: %s\t $%08x\n", trap_labels[i].adr,
	       trap_labels[i].name, get_long (trap_labels[i].adr));
    }
}

static void dump_ints (void)
{
    int i;	
    for (i = 0; int_labels[i].name; i++) {
	printf ("$%02x: %s\t $%08x\n", int_labels[i].adr,
		int_labels[i].name, get_long (int_labels[i].adr));
    }
}

static void disassemble_wait (FILE *file, unsigned long insn)
{
    uae_u8 vp,hp,ve,he,bfd,v_mask,h_mask;

    vp = (insn & 0xff000000) >> 24;
    hp = (insn & 0x00fe0000) >> 16;
    ve = (insn & 0x00007f00) >> 8;
    he = (insn & 0x000000fe);
    bfd = insn & 0x00008000 >> 15;

    /* bit15 can never be masked out*/
    v_mask = vp & (ve | 0x80);
    h_mask = hp & he;
    if (v_mask > 0) {
	fprintf (file, "vpos ");
	if (ve != 0x7f) {
	    fprintf (file, "& 0x%02x ", ve);
	}
	fprintf (file, ">= 0x%02x", v_mask);
    }
    if (he > 0) {
	if (v_mask > 0) {
	    fprintf (file," and");
	}
	fprintf (file, " hpos ");
	if (he != 0xfe) {
	    fprintf (file, "& 0x%02x ", he);
	}
	fprintf (file, ">= 0x%02x", h_mask);
    } else {
	fprintf (file, ", ignore horizontal");
    }

    fprintf (file, ".\n                        \t; VP %02x, VE %02x; HP %02x, HE %02x; BFD %d\n",
	     vp, ve, hp, he, bfd);
}

/* simple decode copper by Mark Cox */
static void decode_copper_insn (FILE* file, unsigned long insn, unsigned long addr)
{
    uae_u32 insn_type = insn & 0x00010001;
    int hpos, vpos;
    char record[] = "          ";
    if (find_copper_record (addr, &hpos, &vpos)) {
	sprintf (record, " [%03x %03x]", vpos, hpos);
    }

    fprintf (file, "%08lx: %04lx %04lx%s\t; ", addr, insn >> 16, insn & 0xFFFF, record);

    switch (insn_type) {
    case 0x00010000: /* WAIT insn */
	fprintf (file, "Wait for ");
	disassemble_wait (file, insn);

	if (insn == 0xfffffffe)
	    fprintf (file, "                           \t; End of Copperlist\n");

	break;

    case 0x00010001: /* SKIP insn */
	fprintf (file, "Skip if ");
	disassemble_wait (file, insn);
	break;

    case 0x00000000:
    case 0x00000001: /* MOVE insn */
	fprintf (file, "%s := 0x%04lx\n",
		 custd[(insn & 0x01fe0000) >> 17].name,
		 insn & 0x0000ffff);

	if ((insn & 0xfe000000) != 0) {
	    fprintf (file, "                        \t;OCS Compatibility warning: bits 15-9 should be 0 for compatibility with OCS\n");
	}
	/* 01fe0000 register destination address
	   fe000000 should be 0 for compatibility (at least in ocs
	   0000ffff data to be put in register destination */
	break;

    default:
	abort ();
    }

}


static uaecptr decode_copperlist (FILE* file, uaecptr address, int nolines)
{
    uae_u32 insn;
    while (nolines-- > 0) {
	insn = get_long (address);
	decode_copper_insn (file, insn, address);
	address += 4;
    }
    return address;
    /* You may wonder why I don't stop this at the end of the copperlist?
     * Well, often nice things are hidden at the end and it is debatable the actual 
     * values that mean the end of the copperlist */
}


/* cheat-search by Holger Jakob */
static void cheatsearch (char **c)
{
    uae_u8 *p = get_real_address (0);
    static uae_u32 *vlist = NULL;
    uae_u32 ptr;
    uae_u32 val = 0;
    uae_u32 type = 0; /* not yet */
    uae_u32 count = 0;
    uae_u32 fcount = 0;
    uae_u32 full = 0;
    char nc;

    ignore_ws (c);

    while (isxdigit (nc = **c)) {
	(*c)++;
	val *= 10;
	nc = toupper (nc);
	if (isdigit (nc)) {
	    val += nc - '0';
	}
    }
    if (vlist == NULL) {
	vlist = malloc (256*4);
	if (vlist != 0) {
	    for (count = 0; count<255; count++)
		vlist[count] = 0;
	    count = 0;
	    for (ptr = 0; ptr < allocated_chipmem - 40; ptr += 2, p += 2) {
		if (ptr >= 0x438 && p[3] == (val & 0xff)
		    && p[2] == (val >> 8 & 0xff)
		    && p[1] == (val >> 16 & 0xff)
		    && p[0] == (val >> 24 & 0xff))
		{
		    if (count < 255) {
			vlist[count++]=ptr;
			printf ("%08x: %x%x%x%x\n",ptr,p[0],p[1],p[2],p[3]);
		    } else
			full = 1;
		}
	    }
	    printf ("Found %d possible addresses with %d\n",count,val);
	    printf ("Now continue with 'g' and use 'C' with a different value\n");
	}
    } else {
	for (count = 0; count<255; count++) {
	    if (p[vlist[count]+3] == (val & 0xff)
		&& p[vlist[count]+2] == (val>>8 & 0xff) 
		&& p[vlist[count]+1] == (val>>16 & 0xff)
		&& p[vlist[count]] == (val>>24 & 0xff))
	    {
		fcount++;
		printf ("%08x: %x%x%x%x\n", vlist[count], p[vlist[count]],
			p[vlist[count]+1], p[vlist[count]+2], p[vlist[count]+3]);
	    }
	}
	printf ("%d hits of %d found\n",fcount,val);
	free (vlist);
	vlist = NULL;
    }
}

static void writeintomem (char **c)
{
    uae_u8 *p = get_real_address (0);
    uae_u32 addr = 0;
    uae_u32 val = 0;
    char nc;

    ignore_ws(c);
    while (isxdigit(nc = **c)) {
	(*c)++;
	addr *= 16;
	nc = toupper(nc);
	if (isdigit(nc)) {
	    addr += nc - '0';
	} else {
	    addr += nc - 'A' + 10;
	}
    }
    ignore_ws(c);
    while (isxdigit(nc = **c)) {
	(*c)++;
	val *= 10;
	nc = toupper(nc);
	if (isdigit(nc)) {
	    val += nc - '0';
	}
    }

    if (addr < allocated_chipmem) {
      p[addr] = val>>24 & 0xff;
      p[addr+1] = val>>16 & 0xff;
      p[addr+2] = val>>8 & 0xff;
      p[addr+3] = val & 0xff;
      printf ("Wrote %d at %08x\n",val,addr);
    } else
      printf ("Invalid address %08x\n",addr);
}

static void show_exec_tasks (void)
{
    uaecptr execbase = get_long (4);
    uaecptr taskready = get_long (execbase + 406);
    uaecptr taskwait = get_long (execbase + 420);
    uaecptr node, end;
    printf ("execbase at 0x%08lx\n", (unsigned long) execbase);
    printf ("Current:\n");
    node = get_long (execbase + 276);
    printf ("%08lx: %08lx %s\n", node, 0, get_real_address (get_long (node + 10)));
    printf ("Ready:\n");
    node = get_long (taskready);
    end = get_long (taskready + 4);
    while (node) {
	printf ("%08lx: %08lx %s\n", node, 0, get_real_address (get_long (node + 10)));
	node = get_long (node);
    }
    printf ("Waiting:\n");
    node = get_long (taskwait);
    end = get_long (taskwait + 4);
    while (node) {
	printf ("%08lx: %08lx %s\n", node, 0, get_real_address (get_long (node + 10)));
	node = get_long (node);
    }
}

static int trace_same_insn_count;
static uae_u8 trace_insn_copy[10];
static struct regstruct trace_prev_regs;
void debug (void)
{
    char input[80];
    uaecptr nextpc,nxdis,nxmem,nxcopper;

    bogusframe = 1;

    if (do_skip && skipaddr == 0xC0DEDBAD) {
#if 0
	if (trace_same_insn_count > 0) {
	    if (memcmp (trace_insn_copy, regs.pc_p, 10) == 0
		&& memcmp (trace_prev_regs.regs, regs.regs, sizeof regs.regs) == 0)
	    {
		trace_same_insn_count++;
		return;
	    }
	}
	if (trace_same_insn_count > 1)
	    fprintf (logfile, "[ repeated %d times ]\n", trace_same_insn_count);
#endif
	m68k_dumpstate (logfile, &nextpc);
	trace_same_insn_count = 1;
	memcpy (trace_insn_copy, regs.pc_p, 10);
	memcpy (&trace_prev_regs, &regs, sizeof regs);
    }

    if (do_skip && (m68k_getpc() != skipaddr/* || regs.a[0] != 0x1e558*/)) {
	set_special (SPCFLAG_BRK);
	return;
    }
    do_skip = 0;

#ifdef NEED_TO_DEBUG_BADLY
    history[lasthist] = regs;
    historyf[lasthist] = regflags;
#else
    history[lasthist] = m68k_getpc();
#endif
    if (++lasthist == MAX_HIST) lasthist = 0;
    if (lasthist == firsthist) {
	if (++firsthist == MAX_HIST) firsthist = 0;
    }

    m68k_dumpstate (stdout, &nextpc);
    nxdis = nextpc; nxmem = nxcopper = 0;

    for (;;) {
	char cmd, *inptr;

	printf (">");
	fflush (stdout);
	if (fgets (input, 80, stdin) == 0)
	    return;
	inptr = input;
	cmd = next_char (&inptr);
	switch (cmd) {
	case 'c': dumpcia (); dumpdisk (); dumpcustom (); break;
	case 'i': dump_ints (); break;
	case 'e': dump_traps (); break;
	case 'r': m68k_dumpstate (stdout, &nextpc); break;
	case 'M': modulesearch (); break;
	case 'C': cheatsearch (&inptr); break; 
	case 'W': writeintomem (&inptr); break;
	case 'S':
	{
	    uae_u8 *memp;
	    uae_u32 src, len;
	    char *name;
	    FILE *fp;

	    if (!more_params (&inptr))
		goto S_argh;

	    name = inptr;
	    while (*inptr != '\0' && !isspace (*inptr))
		inptr++;
	    if (!isspace (*inptr))
		goto S_argh;

	    *inptr = '\0';
	    inptr++;
	    if (!more_params (&inptr))
		goto S_argh;
	    src = readhex (&inptr);
	    if (!more_params (&inptr))
		goto S_argh;
	    len = readhex (&inptr);
	    if (! valid_address (src, len)) {
		printf ("Invalid memory block\n");
		break;
	    }
	    memp = get_real_address (src);
	    fp = fopen (name, "w");
	    if (fp == NULL) {
		printf ("Couldn't open file\n");
		break;
	    }
	    if (fwrite (memp, 1, len, fp) != len) {
		printf ("Error writing file\n");
	    }
	    fclose (fp);
	    break;

	  S_argh:
	    printf ("S command needs more arguments!\n");
	    break;
	}
	case 'd':
	{
	    uae_u32 daddr;
	    int count;

	    if (more_params(&inptr))
		daddr = readhex(&inptr);
	    else
		daddr = nxdis;
	    if (more_params(&inptr))
		count = readhex(&inptr);
	    else
		count = 10;
	    m68k_disasm (stdout, daddr, &nxdis, count);
	}
	break;
	case 'T': show_exec_tasks (); break;
	case 't': set_special (SPCFLAG_BRK); return;
	case 'z':
	    skipaddr = nextpc;
	    do_skip = 1;
	    set_special (SPCFLAG_BRK);
	    return;

	case 'f':
	    skipaddr = readhex (&inptr);
	    do_skip = 1;
	    set_special (SPCFLAG_BRK);
	    if (skipaddr == 0xC0DEDBAD) {
	        trace_same_insn_count = 0;
		logfile = fopen ("uae.trace", "w");
		memcpy (trace_insn_copy, regs.pc_p, 10);
		memcpy (&trace_prev_regs, &regs, sizeof regs);
	    }
	    return;

	case 'q': uae_quit();
	    debugger_active = 0;
	    debugging = 0;
	    return;

	case 'g':
	    if (more_params (&inptr))
		m68k_setpc (readhex (&inptr));
	    fill_prefetch_0 ();
	    debugger_active = 0;
	    debugging = 0;
	    return;

	case 'H':
	{
	    int count;
	    int temp;
#ifdef NEED_TO_DEBUG_BADLY
	    struct regstruct save_regs = regs;
	    union flagu save_flags = regflags;
#endif

	    if (more_params(&inptr))
		count = readhex(&inptr);
	    else
		count = 10;
	    if (count < 0)
		break;
	    temp = lasthist;
	    while (count-- > 0 && temp != firsthist) {
		if (temp == 0) temp = MAX_HIST-1; else temp--;
	    }
	    while (temp != lasthist) {
#ifdef NEED_TO_DEBUG_BADLY
		regs = history[temp];
		regflags = historyf[temp];
		m68k_dumpstate (NULL);
#else
		m68k_disasm (stdout, history[temp], NULL, 1);
#endif
		if (++temp == MAX_HIST) temp = 0;
	    }
#ifdef NEED_TO_DEBUG_BADLY
	    regs = save_regs;
	    regflags = save_flags;
#endif
	}
	break;
	case 'm':
	{
	    uae_u32 maddr; int lines;
	    if (more_params(&inptr))
		maddr = readhex(&inptr);
	    else
		maddr = nxmem;
	    if (more_params(&inptr))
		lines = readhex(&inptr);
	    else
		lines = 16;
	    dumpmem(maddr, &nxmem, lines);
	}
	break;
	case 'o':
	{
	    uae_u32 maddr;
	    int lines;
 
	    if (more_params(&inptr)) {
		maddr = readhex(&inptr);
		if (maddr == 1 || maddr == 2)
		    maddr = get_copper_address (maddr);		
	    }
	    else
		maddr = nxcopper;

	    if (more_params (&inptr))
		lines = readhex (&inptr);
	    else
		lines = 10;

	    nxcopper = decode_copperlist (stdout, maddr, lines);
	    break;
	}
	case 'O':
	    if (more_params (&inptr)) {
		int plane = readint (&inptr);
		int offs = readint (&inptr);
		if (plane >= 0 && plane < 8)
		    bpl_off[plane] = offs;
	    } else {
		int i;
		for (i = 0; i < 8; i++)
		    printf ("Plane %d offset %d\n", i, bpl_off[i]);
	    }
	    break;
	case 'h':
	case '?':
	{
	    printf ("          HELP for UAE Debugger\n");
	    printf ("         -----------------------\n\n");
	    printf ("  g: <address>          Start execution at the current address or <address>\n");
	    printf ("  c:                    Dump state of the CIA and custom chips\n");
	    printf ("  r:                    Dump state of the CPU\n");
	    printf ("  m <address> <lines>:  Memory dump starting at <address>\n");
	    printf ("  d <address> <lines>:  Disassembly starting at <address>\n");
	    printf ("  t:                    Step one instruction\n");
	    printf ("  z:                    Step through one instruction - useful for JSR, DBRA etc\n");
	    printf ("  f <address>:          Step forward until PC == <address>\n");
	    printf ("  i:                    Dump contents of interrupt registers\n");
	    printf ("  e:                    Dump contents of trap vectors\n");
	    printf ("  o <1|2|addr> <lines>: View memory as Copper Instructions\n");
	    printf ("  O:                    Display bitplane offsets\n");
	    printf ("  O <plane> <offset>:   Offset a bitplane\n");
	    printf ("  H <count>:            Show PC history <count> instructions\n");
	    printf ("  M:                    Search for *Tracker sound modules\n");
	    printf ("  C <value>:            Search for values like energy or lifes in games\n");
	    printf ("  W <address> <value>:  Write into Amiga memory\n");
	    printf ("  S <file> <addr> <n>:  Save a block of Amiga memory\n");
	    printf ("  T:                    Show exec tasks and their PCs\n");
	    printf ("  h,?:                  Show this help page\n");
	    printf ("  q:                    Quit the emulator. You don't want to use this command.\n\n");
	}
	break;
	}
    }
}

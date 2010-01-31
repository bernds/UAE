#include "sysconfig.h"
#include "sysdeps.h"
#include "config.h"
#include "options.h"
#include "memory.h"
#include "custom.h"
#include "events.h"
#include "newcpu.h"
#include "cpu_prefetch.h"
#include "compiler.h"
#include "cputbl.h"
#define CPUFUNC(x) x##_ff
#define SET_CFLG_ALWAYS(x) SET_CFLG(x)
#define SET_NFLG_ALWAYS(x) SET_NFLG(x)
#ifdef NOFLAGS
#include "noflags.h"
#endif

#if !defined(PART_1) && !defined(PART_2) && !defined(PART_3) && !defined(PART_4) && !defined(PART_5) && !defined(PART_6) && !defined(PART_7) && !defined(PART_8)
#define PART_1 1
#define PART_2 1
#define PART_3 1
#define PART_4 1
#define PART_5 1
#define PART_6 1
#define PART_7 1
#define PART_8 1
#endif

#ifdef PART_1
/* OR.B #<data>.B,Dn */
unsigned long REGPARAM2 CPUFUNC(op_0000_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce_prefetch (4);
{	uae_s8 dst = m68k_dreg(regs, dstreg);
	src |= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xff) | ((src) & 0xff);
}}}	m68k_incpc (4);
return 0;
}
/* OR.B #<data>.B,(An) */
unsigned long REGPARAM2 CPUFUNC(op_0010_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce_prefetch (4);
{	uaecptr dsta = m68k_areg(regs, dstreg);
{	uae_s8 dst = get_byte_ce (dsta);
	src |= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	put_byte_ce (dsta,src);
}}}}	m68k_incpc (4);
return 0;
}
/* OR.B #<data>.B,(An)+ */
unsigned long REGPARAM2 CPUFUNC(op_0018_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce_prefetch (4);
{	uaecptr dsta = m68k_areg(regs, dstreg);
{	uae_s8 dst = get_byte_ce (dsta);
	m68k_areg(regs, dstreg) += areg_byteinc[dstreg];
	src |= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	put_byte_ce (dsta,src);
}}}}	m68k_incpc (4);
return 0;
}
/* OR.B #<data>.B,-(An) */
unsigned long REGPARAM2 CPUFUNC(op_0020_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce_prefetch (4);
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = m68k_areg(regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = get_byte_ce (dsta);
	m68k_areg (regs, dstreg) = dsta;
	src |= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	put_byte_ce (dsta,src);
}}}}	m68k_incpc (4);
return 0;
}
/* OR.B #<data>.B,(d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_0028_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce_prefetch (4);
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (6);
{	uae_s8 dst = get_byte_ce (dsta);
	src |= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	put_byte_ce (dsta,src);
}}}}	m68k_incpc (6);
return 0;
}
/* OR.B #<data>.B,(d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_0030_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce_prefetch (4);
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (6));
{	uae_s8 dst = get_byte_ce (dsta);
	src |= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	put_byte_ce (dsta,src);
}}}}	m68k_incpc (6);
return 0;
}
/* OR.B #<data>.B,(xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_0038_6)(uae_u32 opcode)
{
{{	uae_s8 src = (uae_u8)get_word_ce_prefetch (4);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (6);
{	uae_s8 dst = get_byte_ce (dsta);
	src |= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	put_byte_ce (dsta,src);
}}}}	m68k_incpc (6);
return 0;
}
/* OR.B #<data>.B,(xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_0039_6)(uae_u32 opcode)
{
{{	uae_s8 src = (uae_u8)get_word_ce_prefetch (4);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (6) << 16;
	dsta |= get_word_ce_prefetch (8);
{	uae_s8 dst = get_byte_ce (dsta);
	src |= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (10);
	put_byte_ce (dsta,src);
}}}}	m68k_incpc (8);
return 0;
}
/* ORSR.B #<data>.W */
unsigned long REGPARAM2 CPUFUNC(op_003c_6)(uae_u32 opcode)
{
{	MakeSR();
{	uae_s16 src = get_word_ce_prefetch (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	src &= 0xFF;
	do_cycles (8 * 256);
	regs.sr |= src;
	MakeFromSR();
}}	m68k_incpc (4);
return 0;
}
/* OR.W #<data>.W,Dn */
unsigned long REGPARAM2 CPUFUNC(op_0040_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	src |= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xffff) | ((src) & 0xffff);
}}}	m68k_incpc (4);
return 0;
}
/* OR.W #<data>.W,(An) */
unsigned long REGPARAM2 CPUFUNC(op_0050_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel3742;
	}
{{	uae_s16 dst = get_word_ce (dsta);
	src |= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	put_word_ce (dsta,src);
}}}}}	m68k_incpc (4);
endlabel3742: ;
return 0;
}
/* OR.W #<data>.W,(An)+ */
unsigned long REGPARAM2 CPUFUNC(op_0058_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel3743;
	}
{{	uae_s16 dst = get_word_ce (dsta);
	m68k_areg(regs, dstreg) += 2;
	src |= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	put_word_ce (dsta,src);
}}}}}	m68k_incpc (4);
endlabel3743: ;
return 0;
}
/* OR.W #<data>.W,-(An) */
unsigned long REGPARAM2 CPUFUNC(op_0060_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = m68k_areg(regs, dstreg) - 2;
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel3744;
	}
{{	uae_s16 dst = get_word_ce (dsta);
	m68k_areg (regs, dstreg) = dsta;
	src |= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	put_word_ce (dsta,src);
}}}}}	m68k_incpc (4);
endlabel3744: ;
return 0;
}
/* OR.W #<data>.W,(d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_0068_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (6);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel3745;
	}
{{	uae_s16 dst = get_word_ce (dsta);
	src |= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	put_word_ce (dsta,src);
}}}}}	m68k_incpc (6);
endlabel3745: ;
return 0;
}
/* OR.W #<data>.W,(d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_0070_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (6));
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel3746;
	}
{{	uae_s16 dst = get_word_ce (dsta);
	src |= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	put_word_ce (dsta,src);
}}}}}	m68k_incpc (6);
endlabel3746: ;
return 0;
}
/* OR.W #<data>.W,(xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_0078_6)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (6);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel3747;
	}
{{	uae_s16 dst = get_word_ce (dsta);
	src |= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	put_word_ce (dsta,src);
}}}}}	m68k_incpc (6);
endlabel3747: ;
return 0;
}
/* OR.W #<data>.W,(xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_0079_6)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (6) << 16;
	dsta |= get_word_ce_prefetch (8);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel3748;
	}
{{	uae_s16 dst = get_word_ce (dsta);
	src |= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (10);
	put_word_ce (dsta,src);
}}}}}	m68k_incpc (8);
endlabel3748: ;
return 0;
}
/* ORSR.W #<data>.W */
unsigned long REGPARAM2 CPUFUNC(op_007c_6)(uae_u32 opcode)
{
{if (!regs.s) { Exception(8,0); goto endlabel3749; }
{	MakeSR();
{	uae_s16 src = get_word_ce_prefetch (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	do_cycles (8 * 256);
	regs.sr |= src;
	MakeFromSR();
}}}	m68k_incpc (4);
endlabel3749: ;
return 0;
}
/* OR.L #<data>.L,Dn */
unsigned long REGPARAM2 CPUFUNC(op_0080_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	src |= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	do_cycles (4 * 256);
	m68k_dreg(regs, dstreg) = (src);
}}}	m68k_incpc (6);
return 0;
}
/* OR.L #<data>.L,(An) */
unsigned long REGPARAM2 CPUFUNC(op_0090_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel3751;
	}
{{	uae_s32 dst = get_word_ce (dsta) << 16; dst |= get_word_ce (dsta + 2);
	src |= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
}}}}}	m68k_incpc (6);
endlabel3751: ;
return 0;
}
/* OR.L #<data>.L,(An)+ */
unsigned long REGPARAM2 CPUFUNC(op_0098_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel3752;
	}
{{	uae_s32 dst = get_word_ce (dsta) << 16; dst |= get_word_ce (dsta + 2);
	m68k_areg(regs, dstreg) += 4;
	src |= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
}}}}}	m68k_incpc (6);
endlabel3752: ;
return 0;
}
/* OR.L #<data>.L,-(An) */
unsigned long REGPARAM2 CPUFUNC(op_00a0_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = m68k_areg(regs, dstreg) - 4;
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel3753;
	}
{{	uae_s32 dst = get_word_ce (dsta) << 16; dst |= get_word_ce (dsta + 2);
	m68k_areg (regs, dstreg) = dsta;
	src |= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
}}}}}	m68k_incpc (6);
endlabel3753: ;
return 0;
}
/* OR.L #<data>.L,(d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_00a8_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (8);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel3754;
	}
{{	uae_s32 dst = get_word_ce (dsta) << 16; dst |= get_word_ce (dsta + 2);
	src |= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (10);
	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
}}}}}	m68k_incpc (8);
endlabel3754: ;
return 0;
}
/* OR.L #<data>.L,(d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_00b0_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (8));
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel3755;
	}
{{	uae_s32 dst = get_word_ce (dsta) << 16; dst |= get_word_ce (dsta + 2);
	src |= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (10);
	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
}}}}}	m68k_incpc (8);
endlabel3755: ;
return 0;
}
/* OR.L #<data>.L,(xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_00b8_6)(uae_u32 opcode)
{
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (8);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel3756;
	}
{{	uae_s32 dst = get_word_ce (dsta) << 16; dst |= get_word_ce (dsta + 2);
	src |= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (10);
	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
}}}}}	m68k_incpc (8);
endlabel3756: ;
return 0;
}
/* OR.L #<data>.L,(xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_00b9_6)(uae_u32 opcode)
{
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (8) << 16;
	dsta |= get_word_ce_prefetch (10);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel3757;
	}
{{	uae_s32 dst = get_word_ce (dsta) << 16; dst |= get_word_ce (dsta + 2);
	src |= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (12);
	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
}}}}}	m68k_incpc (10);
endlabel3757: ;
return 0;
}
/* BTST.L Dn,Dn */
unsigned long REGPARAM2 CPUFUNC(op_0100_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	src &= 31;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	do_cycles (2 * 256);
}}}	m68k_incpc (2);
return 0;
}
/* MVPMR.W (d16,An),Dn */
unsigned long REGPARAM2 CPUFUNC(op_0108_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{	uaecptr memp = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_u16 val = (get_byte_ce (memp) << 8) + get_byte_ce (memp + 2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xffff) | ((val) & 0xffff);
}}	m68k_incpc (4);
return 0;
}
/* BTST.B Dn,(An) */
unsigned long REGPARAM2 CPUFUNC(op_0110_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = m68k_areg(regs, dstreg);
{	uae_s8 dst = get_byte_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
}}}}	m68k_incpc (2);
return 0;
}
/* BTST.B Dn,(An)+ */
unsigned long REGPARAM2 CPUFUNC(op_0118_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = m68k_areg(regs, dstreg);
{	uae_s8 dst = get_byte_ce (dsta);
	m68k_areg(regs, dstreg) += areg_byteinc[dstreg];
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
}}}}	m68k_incpc (2);
return 0;
}
/* BTST.B Dn,-(An) */
unsigned long REGPARAM2 CPUFUNC(op_0120_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = m68k_areg(regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = get_byte_ce (dsta);
	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
}}}}	m68k_incpc (2);
return 0;
}
/* BTST.B Dn,(d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_0128_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 dst = get_byte_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
}}}}	m68k_incpc (4);
return 0;
}
/* BTST.B Dn,(d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_0130_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (4));
{	uae_s8 dst = get_byte_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
}}}}	m68k_incpc (4);
return 0;
}
/* BTST.B Dn,(xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_0138_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 dst = get_byte_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
}}}}	m68k_incpc (4);
return 0;
}
/* BTST.B Dn,(xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_0139_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (4) << 16;
	dsta |= get_word_ce_prefetch (6);
{	uae_s8 dst = get_byte_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
}}}}	m68k_incpc (6);
return 0;
}
/* BTST.B Dn,(d16,PC) */
unsigned long REGPARAM2 CPUFUNC(op_013a_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = 2;
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = m68k_getpc () + 2;
	dsta += (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 dst = get_byte_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
}}}}	m68k_incpc (4);
return 0;
}
/* BTST.B Dn,(d8,PC,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_013b_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = 3;
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	uaecptr tmppc;
	uaecptr dsta;
	do_cycles (2 * 256);
	tmppc = m68k_getpc() + 2;
	dsta = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
{	uae_s8 dst = get_byte_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
}}}}	m68k_incpc (4);
return 0;
}
/* BTST.B Dn,#<data>.B */
unsigned long REGPARAM2 CPUFUNC(op_013c_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	uae_s8 dst = (uae_u8)get_word_ce_prefetch (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
}}}	m68k_incpc (4);
return 0;
}
/* BCHG.L Dn,Dn */
unsigned long REGPARAM2 CPUFUNC(op_0140_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	src &= 31;
	dst ^= (1 << src);
	SET_ZFLG (((uae_u32)dst & (1 << src)) >> src);
	do_cycles (4 * 256);
	m68k_dreg(regs, dstreg) = (dst);
}}}	m68k_incpc (2);
return 0;
}
/* MVPMR.L (d16,An),Dn */
unsigned long REGPARAM2 CPUFUNC(op_0148_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{	uaecptr memp = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_u32 val = (get_byte_ce (memp) << 24) + (get_byte_ce (memp + 2) << 16)
              + (get_byte_ce (memp + 4) << 8) + get_byte_ce (memp + 6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	m68k_dreg(regs, dstreg) = (val);
}}	m68k_incpc (4);
return 0;
}
/* BCHG.B Dn,(An) */
unsigned long REGPARAM2 CPUFUNC(op_0150_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = m68k_areg(regs, dstreg);
{	uae_s8 dst = get_byte_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	src &= 7;
	dst ^= (1 << src);
	SET_ZFLG (((uae_u32)dst & (1 << src)) >> src);
	put_byte_ce (dsta,dst);
}}}}	m68k_incpc (2);
return 0;
}
/* BCHG.B Dn,(An)+ */
unsigned long REGPARAM2 CPUFUNC(op_0158_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = m68k_areg(regs, dstreg);
{	uae_s8 dst = get_byte_ce (dsta);
	m68k_areg(regs, dstreg) += areg_byteinc[dstreg];
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	src &= 7;
	dst ^= (1 << src);
	SET_ZFLG (((uae_u32)dst & (1 << src)) >> src);
	put_byte_ce (dsta,dst);
}}}}	m68k_incpc (2);
return 0;
}
/* BCHG.B Dn,-(An) */
unsigned long REGPARAM2 CPUFUNC(op_0160_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = m68k_areg(regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = get_byte_ce (dsta);
	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	src &= 7;
	dst ^= (1 << src);
	SET_ZFLG (((uae_u32)dst & (1 << src)) >> src);
	put_byte_ce (dsta,dst);
}}}}	m68k_incpc (2);
return 0;
}
/* BCHG.B Dn,(d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_0168_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 dst = get_byte_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	src &= 7;
	dst ^= (1 << src);
	SET_ZFLG (((uae_u32)dst & (1 << src)) >> src);
	put_byte_ce (dsta,dst);
}}}}	m68k_incpc (4);
return 0;
}
/* BCHG.B Dn,(d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_0170_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (4));
{	uae_s8 dst = get_byte_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	src &= 7;
	dst ^= (1 << src);
	SET_ZFLG (((uae_u32)dst & (1 << src)) >> src);
	put_byte_ce (dsta,dst);
}}}}	m68k_incpc (4);
return 0;
}
/* BCHG.B Dn,(xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_0178_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 dst = get_byte_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	src &= 7;
	dst ^= (1 << src);
	SET_ZFLG (((uae_u32)dst & (1 << src)) >> src);
	put_byte_ce (dsta,dst);
}}}}	m68k_incpc (4);
return 0;
}
/* BCHG.B Dn,(xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_0179_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (4) << 16;
	dsta |= get_word_ce_prefetch (6);
{	uae_s8 dst = get_byte_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	src &= 7;
	dst ^= (1 << src);
	SET_ZFLG (((uae_u32)dst & (1 << src)) >> src);
	put_byte_ce (dsta,dst);
}}}}	m68k_incpc (6);
return 0;
}
/* BCHG.B Dn,(d16,PC) */
unsigned long REGPARAM2 CPUFUNC(op_017a_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = 2;
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = m68k_getpc () + 2;
	dsta += (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 dst = get_byte_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	src &= 7;
	dst ^= (1 << src);
	SET_ZFLG (((uae_u32)dst & (1 << src)) >> src);
	put_byte_ce (dsta,dst);
}}}}	m68k_incpc (4);
return 0;
}
/* BCHG.B Dn,(d8,PC,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_017b_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = 3;
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	uaecptr tmppc;
	uaecptr dsta;
	do_cycles (2 * 256);
	tmppc = m68k_getpc() + 2;
	dsta = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
{	uae_s8 dst = get_byte_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	src &= 7;
	dst ^= (1 << src);
	SET_ZFLG (((uae_u32)dst & (1 << src)) >> src);
	put_byte_ce (dsta,dst);
}}}}	m68k_incpc (4);
return 0;
}
/* BCLR.L Dn,Dn */
unsigned long REGPARAM2 CPUFUNC(op_0180_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	src &= 31;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst &= ~(1 << src);
	do_cycles (6 * 256);
	m68k_dreg(regs, dstreg) = (dst);
}}}	m68k_incpc (2);
return 0;
}
/* MVPRM.W Dn,(d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_0188_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg(regs, srcreg);
	uaecptr memp = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	put_byte_ce (memp, src >> 8); put_byte_ce (memp + 2, src);
}}	m68k_incpc (4);
return 0;
}
/* BCLR.B Dn,(An) */
unsigned long REGPARAM2 CPUFUNC(op_0190_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = m68k_areg(regs, dstreg);
{	uae_s8 dst = get_byte_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst &= ~(1 << src);
	put_byte_ce (dsta,dst);
}}}}	m68k_incpc (2);
return 0;
}
/* BCLR.B Dn,(An)+ */
unsigned long REGPARAM2 CPUFUNC(op_0198_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = m68k_areg(regs, dstreg);
{	uae_s8 dst = get_byte_ce (dsta);
	m68k_areg(regs, dstreg) += areg_byteinc[dstreg];
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst &= ~(1 << src);
	put_byte_ce (dsta,dst);
}}}}	m68k_incpc (2);
return 0;
}
/* BCLR.B Dn,-(An) */
unsigned long REGPARAM2 CPUFUNC(op_01a0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = m68k_areg(regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = get_byte_ce (dsta);
	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst &= ~(1 << src);
	put_byte_ce (dsta,dst);
}}}}	m68k_incpc (2);
return 0;
}
/* BCLR.B Dn,(d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_01a8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 dst = get_byte_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst &= ~(1 << src);
	put_byte_ce (dsta,dst);
}}}}	m68k_incpc (4);
return 0;
}
/* BCLR.B Dn,(d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_01b0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (4));
{	uae_s8 dst = get_byte_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst &= ~(1 << src);
	put_byte_ce (dsta,dst);
}}}}	m68k_incpc (4);
return 0;
}
/* BCLR.B Dn,(xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_01b8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 dst = get_byte_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst &= ~(1 << src);
	put_byte_ce (dsta,dst);
}}}}	m68k_incpc (4);
return 0;
}
/* BCLR.B Dn,(xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_01b9_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (4) << 16;
	dsta |= get_word_ce_prefetch (6);
{	uae_s8 dst = get_byte_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst &= ~(1 << src);
	put_byte_ce (dsta,dst);
}}}}	m68k_incpc (6);
return 0;
}
/* BCLR.B Dn,(d16,PC) */
unsigned long REGPARAM2 CPUFUNC(op_01ba_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = 2;
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = m68k_getpc () + 2;
	dsta += (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 dst = get_byte_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst &= ~(1 << src);
	put_byte_ce (dsta,dst);
}}}}	m68k_incpc (4);
return 0;
}
/* BCLR.B Dn,(d8,PC,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_01bb_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = 3;
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	uaecptr tmppc;
	uaecptr dsta;
	do_cycles (2 * 256);
	tmppc = m68k_getpc() + 2;
	dsta = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
{	uae_s8 dst = get_byte_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst &= ~(1 << src);
	put_byte_ce (dsta,dst);
}}}}	m68k_incpc (4);
return 0;
}
/* BSET.L Dn,Dn */
unsigned long REGPARAM2 CPUFUNC(op_01c0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	src &= 31;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst |= (1 << src);
	do_cycles (4 * 256);
	m68k_dreg(regs, dstreg) = (dst);
}}}	m68k_incpc (2);
return 0;
}
/* MVPRM.L Dn,(d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_01c8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg(regs, srcreg);
	uaecptr memp = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	put_byte_ce (memp, src >> 24); put_byte_ce (memp + 2, src >> 16);
	put_byte_ce (memp + 4, src >> 8); put_byte_ce (memp + 6, src);
}}	m68k_incpc (4);
return 0;
}
/* BSET.B Dn,(An) */
unsigned long REGPARAM2 CPUFUNC(op_01d0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = m68k_areg(regs, dstreg);
{	uae_s8 dst = get_byte_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst |= (1 << src);
	put_byte_ce (dsta,dst);
}}}}	m68k_incpc (2);
return 0;
}
/* BSET.B Dn,(An)+ */
unsigned long REGPARAM2 CPUFUNC(op_01d8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = m68k_areg(regs, dstreg);
{	uae_s8 dst = get_byte_ce (dsta);
	m68k_areg(regs, dstreg) += areg_byteinc[dstreg];
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst |= (1 << src);
	put_byte_ce (dsta,dst);
}}}}	m68k_incpc (2);
return 0;
}
/* BSET.B Dn,-(An) */
unsigned long REGPARAM2 CPUFUNC(op_01e0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = m68k_areg(regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = get_byte_ce (dsta);
	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst |= (1 << src);
	put_byte_ce (dsta,dst);
}}}}	m68k_incpc (2);
return 0;
}
/* BSET.B Dn,(d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_01e8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 dst = get_byte_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst |= (1 << src);
	put_byte_ce (dsta,dst);
}}}}	m68k_incpc (4);
return 0;
}
/* BSET.B Dn,(d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_01f0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (4));
{	uae_s8 dst = get_byte_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst |= (1 << src);
	put_byte_ce (dsta,dst);
}}}}	m68k_incpc (4);
return 0;
}
/* BSET.B Dn,(xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_01f8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 dst = get_byte_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst |= (1 << src);
	put_byte_ce (dsta,dst);
}}}}	m68k_incpc (4);
return 0;
}
/* BSET.B Dn,(xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_01f9_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (4) << 16;
	dsta |= get_word_ce_prefetch (6);
{	uae_s8 dst = get_byte_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst |= (1 << src);
	put_byte_ce (dsta,dst);
}}}}	m68k_incpc (6);
return 0;
}
/* BSET.B Dn,(d16,PC) */
unsigned long REGPARAM2 CPUFUNC(op_01fa_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = 2;
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = m68k_getpc () + 2;
	dsta += (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 dst = get_byte_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst |= (1 << src);
	put_byte_ce (dsta,dst);
}}}}	m68k_incpc (4);
return 0;
}
/* BSET.B Dn,(d8,PC,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_01fb_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = 3;
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	uaecptr tmppc;
	uaecptr dsta;
	do_cycles (2 * 256);
	tmppc = m68k_getpc() + 2;
	dsta = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
{	uae_s8 dst = get_byte_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst |= (1 << src);
	put_byte_ce (dsta,dst);
}}}}	m68k_incpc (4);
return 0;
}
/* AND.B #<data>.B,Dn */
unsigned long REGPARAM2 CPUFUNC(op_0200_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce_prefetch (4);
{	uae_s8 dst = m68k_dreg(regs, dstreg);
	src &= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xff) | ((src) & 0xff);
}}}	m68k_incpc (4);
return 0;
}
/* AND.B #<data>.B,(An) */
unsigned long REGPARAM2 CPUFUNC(op_0210_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce_prefetch (4);
{	uaecptr dsta = m68k_areg(regs, dstreg);
{	uae_s8 dst = get_byte_ce (dsta);
	src &= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	put_byte_ce (dsta,src);
}}}}	m68k_incpc (4);
return 0;
}
/* AND.B #<data>.B,(An)+ */
unsigned long REGPARAM2 CPUFUNC(op_0218_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce_prefetch (4);
{	uaecptr dsta = m68k_areg(regs, dstreg);
{	uae_s8 dst = get_byte_ce (dsta);
	m68k_areg(regs, dstreg) += areg_byteinc[dstreg];
	src &= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	put_byte_ce (dsta,src);
}}}}	m68k_incpc (4);
return 0;
}
/* AND.B #<data>.B,-(An) */
unsigned long REGPARAM2 CPUFUNC(op_0220_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce_prefetch (4);
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = m68k_areg(regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = get_byte_ce (dsta);
	m68k_areg (regs, dstreg) = dsta;
	src &= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	put_byte_ce (dsta,src);
}}}}	m68k_incpc (4);
return 0;
}
/* AND.B #<data>.B,(d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_0228_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce_prefetch (4);
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (6);
{	uae_s8 dst = get_byte_ce (dsta);
	src &= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	put_byte_ce (dsta,src);
}}}}	m68k_incpc (6);
return 0;
}
/* AND.B #<data>.B,(d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_0230_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce_prefetch (4);
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (6));
{	uae_s8 dst = get_byte_ce (dsta);
	src &= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	put_byte_ce (dsta,src);
}}}}	m68k_incpc (6);
return 0;
}
/* AND.B #<data>.B,(xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_0238_6)(uae_u32 opcode)
{
{{	uae_s8 src = (uae_u8)get_word_ce_prefetch (4);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (6);
{	uae_s8 dst = get_byte_ce (dsta);
	src &= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	put_byte_ce (dsta,src);
}}}}	m68k_incpc (6);
return 0;
}
/* AND.B #<data>.B,(xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_0239_6)(uae_u32 opcode)
{
{{	uae_s8 src = (uae_u8)get_word_ce_prefetch (4);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (6) << 16;
	dsta |= get_word_ce_prefetch (8);
{	uae_s8 dst = get_byte_ce (dsta);
	src &= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (10);
	put_byte_ce (dsta,src);
}}}}	m68k_incpc (8);
return 0;
}
/* ANDSR.B #<data>.W */
unsigned long REGPARAM2 CPUFUNC(op_023c_6)(uae_u32 opcode)
{
{	MakeSR();
{	uae_s16 src = get_word_ce_prefetch (4);
	src |= 0xFF00;
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	do_cycles (8 * 256);
	regs.sr &= src;
	MakeFromSR();
}}	m68k_incpc (4);
return 0;
}
/* AND.W #<data>.W,Dn */
unsigned long REGPARAM2 CPUFUNC(op_0240_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	src &= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xffff) | ((src) & 0xffff);
}}}	m68k_incpc (4);
return 0;
}
/* AND.W #<data>.W,(An) */
unsigned long REGPARAM2 CPUFUNC(op_0250_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel3813;
	}
{{	uae_s16 dst = get_word_ce (dsta);
	src &= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	put_word_ce (dsta,src);
}}}}}	m68k_incpc (4);
endlabel3813: ;
return 0;
}
/* AND.W #<data>.W,(An)+ */
unsigned long REGPARAM2 CPUFUNC(op_0258_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel3814;
	}
{{	uae_s16 dst = get_word_ce (dsta);
	m68k_areg(regs, dstreg) += 2;
	src &= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	put_word_ce (dsta,src);
}}}}}	m68k_incpc (4);
endlabel3814: ;
return 0;
}
/* AND.W #<data>.W,-(An) */
unsigned long REGPARAM2 CPUFUNC(op_0260_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = m68k_areg(regs, dstreg) - 2;
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel3815;
	}
{{	uae_s16 dst = get_word_ce (dsta);
	m68k_areg (regs, dstreg) = dsta;
	src &= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	put_word_ce (dsta,src);
}}}}}	m68k_incpc (4);
endlabel3815: ;
return 0;
}
/* AND.W #<data>.W,(d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_0268_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (6);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel3816;
	}
{{	uae_s16 dst = get_word_ce (dsta);
	src &= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	put_word_ce (dsta,src);
}}}}}	m68k_incpc (6);
endlabel3816: ;
return 0;
}
/* AND.W #<data>.W,(d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_0270_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (6));
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel3817;
	}
{{	uae_s16 dst = get_word_ce (dsta);
	src &= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	put_word_ce (dsta,src);
}}}}}	m68k_incpc (6);
endlabel3817: ;
return 0;
}
/* AND.W #<data>.W,(xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_0278_6)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (6);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel3818;
	}
{{	uae_s16 dst = get_word_ce (dsta);
	src &= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	put_word_ce (dsta,src);
}}}}}	m68k_incpc (6);
endlabel3818: ;
return 0;
}
/* AND.W #<data>.W,(xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_0279_6)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (6) << 16;
	dsta |= get_word_ce_prefetch (8);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel3819;
	}
{{	uae_s16 dst = get_word_ce (dsta);
	src &= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (10);
	put_word_ce (dsta,src);
}}}}}	m68k_incpc (8);
endlabel3819: ;
return 0;
}
/* ANDSR.W #<data>.W */
unsigned long REGPARAM2 CPUFUNC(op_027c_6)(uae_u32 opcode)
{
{if (!regs.s) { Exception(8,0); goto endlabel3820; }
{	MakeSR();
{	uae_s16 src = get_word_ce_prefetch (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	do_cycles (8 * 256);
	regs.sr &= src;
	MakeFromSR();
}}}	m68k_incpc (4);
endlabel3820: ;
return 0;
}
/* AND.L #<data>.L,Dn */
unsigned long REGPARAM2 CPUFUNC(op_0280_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	src &= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	do_cycles (2 * 256);
	m68k_dreg(regs, dstreg) = (src);
}}}	m68k_incpc (6);
return 0;
}
/* AND.L #<data>.L,(An) */
unsigned long REGPARAM2 CPUFUNC(op_0290_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel3822;
	}
{{	uae_s32 dst = get_word_ce (dsta) << 16; dst |= get_word_ce (dsta + 2);
	src &= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
}}}}}	m68k_incpc (6);
endlabel3822: ;
return 0;
}
/* AND.L #<data>.L,(An)+ */
unsigned long REGPARAM2 CPUFUNC(op_0298_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel3823;
	}
{{	uae_s32 dst = get_word_ce (dsta) << 16; dst |= get_word_ce (dsta + 2);
	m68k_areg(regs, dstreg) += 4;
	src &= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
}}}}}	m68k_incpc (6);
endlabel3823: ;
return 0;
}
/* AND.L #<data>.L,-(An) */
unsigned long REGPARAM2 CPUFUNC(op_02a0_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = m68k_areg(regs, dstreg) - 4;
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel3824;
	}
{{	uae_s32 dst = get_word_ce (dsta) << 16; dst |= get_word_ce (dsta + 2);
	m68k_areg (regs, dstreg) = dsta;
	src &= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
}}}}}	m68k_incpc (6);
endlabel3824: ;
return 0;
}
/* AND.L #<data>.L,(d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_02a8_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (8);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel3825;
	}
{{	uae_s32 dst = get_word_ce (dsta) << 16; dst |= get_word_ce (dsta + 2);
	src &= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (10);
	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
}}}}}	m68k_incpc (8);
endlabel3825: ;
return 0;
}
/* AND.L #<data>.L,(d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_02b0_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (8));
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel3826;
	}
{{	uae_s32 dst = get_word_ce (dsta) << 16; dst |= get_word_ce (dsta + 2);
	src &= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (10);
	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
}}}}}	m68k_incpc (8);
endlabel3826: ;
return 0;
}
/* AND.L #<data>.L,(xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_02b8_6)(uae_u32 opcode)
{
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (8);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel3827;
	}
{{	uae_s32 dst = get_word_ce (dsta) << 16; dst |= get_word_ce (dsta + 2);
	src &= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (10);
	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
}}}}}	m68k_incpc (8);
endlabel3827: ;
return 0;
}
/* AND.L #<data>.L,(xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_02b9_6)(uae_u32 opcode)
{
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (8) << 16;
	dsta |= get_word_ce_prefetch (10);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel3828;
	}
{{	uae_s32 dst = get_word_ce (dsta) << 16; dst |= get_word_ce (dsta + 2);
	src &= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (12);
	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
}}}}}	m68k_incpc (10);
endlabel3828: ;
return 0;
}
/* SUB.B #<data>.B,Dn */
unsigned long REGPARAM2 CPUFUNC(op_0400_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce_prefetch (4);
{	uae_s8 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xff) | ((newv) & 0xff);
}}}}}}	m68k_incpc (4);
return 0;
}
/* SUB.B #<data>.B,(An) */
unsigned long REGPARAM2 CPUFUNC(op_0410_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce_prefetch (4);
{	uaecptr dsta = m68k_areg(regs, dstreg);
{	uae_s8 dst = get_byte_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_byte_ce (dsta,newv);
}}}}}}}	m68k_incpc (4);
return 0;
}
/* SUB.B #<data>.B,(An)+ */
unsigned long REGPARAM2 CPUFUNC(op_0418_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce_prefetch (4);
{	uaecptr dsta = m68k_areg(regs, dstreg);
{	uae_s8 dst = get_byte_ce (dsta);
	m68k_areg(regs, dstreg) += areg_byteinc[dstreg];
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_byte_ce (dsta,newv);
}}}}}}}	m68k_incpc (4);
return 0;
}
/* SUB.B #<data>.B,-(An) */
unsigned long REGPARAM2 CPUFUNC(op_0420_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce_prefetch (4);
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = m68k_areg(regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = get_byte_ce (dsta);
	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_byte_ce (dsta,newv);
}}}}}}}	m68k_incpc (4);
return 0;
}
/* SUB.B #<data>.B,(d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_0428_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce_prefetch (4);
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (6);
{	uae_s8 dst = get_byte_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_byte_ce (dsta,newv);
}}}}}}}	m68k_incpc (6);
return 0;
}
/* SUB.B #<data>.B,(d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_0430_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce_prefetch (4);
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (6));
{	uae_s8 dst = get_byte_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_byte_ce (dsta,newv);
}}}}}}}	m68k_incpc (6);
return 0;
}
/* SUB.B #<data>.B,(xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_0438_6)(uae_u32 opcode)
{
{{	uae_s8 src = (uae_u8)get_word_ce_prefetch (4);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (6);
{	uae_s8 dst = get_byte_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_byte_ce (dsta,newv);
}}}}}}}	m68k_incpc (6);
return 0;
}
/* SUB.B #<data>.B,(xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_0439_6)(uae_u32 opcode)
{
{{	uae_s8 src = (uae_u8)get_word_ce_prefetch (4);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (6) << 16;
	dsta |= get_word_ce_prefetch (8);
{	uae_s8 dst = get_byte_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (10);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_byte_ce (dsta,newv);
}}}}}}}	m68k_incpc (8);
return 0;
}
/* SUB.W #<data>.W,Dn */
unsigned long REGPARAM2 CPUFUNC(op_0440_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xffff) | ((newv) & 0xffff);
}}}}}}	m68k_incpc (4);
return 0;
}
/* SUB.W #<data>.W,(An) */
unsigned long REGPARAM2 CPUFUNC(op_0450_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel3838;
	}
{{	uae_s16 dst = get_word_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_word_ce (dsta,newv);
}}}}}}}}	m68k_incpc (4);
endlabel3838: ;
return 0;
}
/* SUB.W #<data>.W,(An)+ */
unsigned long REGPARAM2 CPUFUNC(op_0458_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel3839;
	}
{{	uae_s16 dst = get_word_ce (dsta);
	m68k_areg(regs, dstreg) += 2;
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_word_ce (dsta,newv);
}}}}}}}}	m68k_incpc (4);
endlabel3839: ;
return 0;
}
/* SUB.W #<data>.W,-(An) */
unsigned long REGPARAM2 CPUFUNC(op_0460_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = m68k_areg(regs, dstreg) - 2;
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel3840;
	}
{{	uae_s16 dst = get_word_ce (dsta);
	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_word_ce (dsta,newv);
}}}}}}}}	m68k_incpc (4);
endlabel3840: ;
return 0;
}
/* SUB.W #<data>.W,(d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_0468_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (6);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel3841;
	}
{{	uae_s16 dst = get_word_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_word_ce (dsta,newv);
}}}}}}}}	m68k_incpc (6);
endlabel3841: ;
return 0;
}
/* SUB.W #<data>.W,(d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_0470_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (6));
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel3842;
	}
{{	uae_s16 dst = get_word_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_word_ce (dsta,newv);
}}}}}}}}	m68k_incpc (6);
endlabel3842: ;
return 0;
}
/* SUB.W #<data>.W,(xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_0478_6)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (6);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel3843;
	}
{{	uae_s16 dst = get_word_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_word_ce (dsta,newv);
}}}}}}}}	m68k_incpc (6);
endlabel3843: ;
return 0;
}
/* SUB.W #<data>.W,(xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_0479_6)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (6) << 16;
	dsta |= get_word_ce_prefetch (8);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel3844;
	}
{{	uae_s16 dst = get_word_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (10);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_word_ce (dsta,newv);
}}}}}}}}	m68k_incpc (8);
endlabel3844: ;
return 0;
}
/* SUB.L #<data>.L,Dn */
unsigned long REGPARAM2 CPUFUNC(op_0480_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	do_cycles (4 * 256);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	m68k_dreg(regs, dstreg) = (newv);
}}}}}}	m68k_incpc (6);
return 0;
}
/* SUB.L #<data>.L,(An) */
unsigned long REGPARAM2 CPUFUNC(op_0490_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel3846;
	}
{{	uae_s32 dst = get_word_ce (dsta) << 16; dst |= get_word_ce (dsta + 2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_word_ce (dsta, newv >> 16); put_word_ce (dsta + 2, newv);
}}}}}}}}	m68k_incpc (6);
endlabel3846: ;
return 0;
}
/* SUB.L #<data>.L,(An)+ */
unsigned long REGPARAM2 CPUFUNC(op_0498_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel3847;
	}
{{	uae_s32 dst = get_word_ce (dsta) << 16; dst |= get_word_ce (dsta + 2);
	m68k_areg(regs, dstreg) += 4;
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_word_ce (dsta, newv >> 16); put_word_ce (dsta + 2, newv);
}}}}}}}}	m68k_incpc (6);
endlabel3847: ;
return 0;
}
/* SUB.L #<data>.L,-(An) */
unsigned long REGPARAM2 CPUFUNC(op_04a0_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = m68k_areg(regs, dstreg) - 4;
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel3848;
	}
{{	uae_s32 dst = get_word_ce (dsta) << 16; dst |= get_word_ce (dsta + 2);
	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_word_ce (dsta, newv >> 16); put_word_ce (dsta + 2, newv);
}}}}}}}}	m68k_incpc (6);
endlabel3848: ;
return 0;
}
/* SUB.L #<data>.L,(d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_04a8_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (8);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel3849;
	}
{{	uae_s32 dst = get_word_ce (dsta) << 16; dst |= get_word_ce (dsta + 2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (10);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_word_ce (dsta, newv >> 16); put_word_ce (dsta + 2, newv);
}}}}}}}}	m68k_incpc (8);
endlabel3849: ;
return 0;
}
/* SUB.L #<data>.L,(d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_04b0_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (8));
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel3850;
	}
{{	uae_s32 dst = get_word_ce (dsta) << 16; dst |= get_word_ce (dsta + 2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (10);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_word_ce (dsta, newv >> 16); put_word_ce (dsta + 2, newv);
}}}}}}}}	m68k_incpc (8);
endlabel3850: ;
return 0;
}
/* SUB.L #<data>.L,(xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_04b8_6)(uae_u32 opcode)
{
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (8);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel3851;
	}
{{	uae_s32 dst = get_word_ce (dsta) << 16; dst |= get_word_ce (dsta + 2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (10);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_word_ce (dsta, newv >> 16); put_word_ce (dsta + 2, newv);
}}}}}}}}	m68k_incpc (8);
endlabel3851: ;
return 0;
}
/* SUB.L #<data>.L,(xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_04b9_6)(uae_u32 opcode)
{
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (8) << 16;
	dsta |= get_word_ce_prefetch (10);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel3852;
	}
{{	uae_s32 dst = get_word_ce (dsta) << 16; dst |= get_word_ce (dsta + 2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (12);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_word_ce (dsta, newv >> 16); put_word_ce (dsta + 2, newv);
}}}}}}}}	m68k_incpc (10);
endlabel3852: ;
return 0;
}
/* ADD.B #<data>.B,Dn */
unsigned long REGPARAM2 CPUFUNC(op_0600_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce_prefetch (4);
{	uae_s8 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xff) | ((newv) & 0xff);
}}}}}}	m68k_incpc (4);
return 0;
}
/* ADD.B #<data>.B,(An) */
unsigned long REGPARAM2 CPUFUNC(op_0610_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce_prefetch (4);
{	uaecptr dsta = m68k_areg(regs, dstreg);
{	uae_s8 dst = get_byte_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_byte_ce (dsta,newv);
}}}}}}}	m68k_incpc (4);
return 0;
}
/* ADD.B #<data>.B,(An)+ */
unsigned long REGPARAM2 CPUFUNC(op_0618_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce_prefetch (4);
{	uaecptr dsta = m68k_areg(regs, dstreg);
{	uae_s8 dst = get_byte_ce (dsta);
	m68k_areg(regs, dstreg) += areg_byteinc[dstreg];
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_byte_ce (dsta,newv);
}}}}}}}	m68k_incpc (4);
return 0;
}
/* ADD.B #<data>.B,-(An) */
unsigned long REGPARAM2 CPUFUNC(op_0620_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce_prefetch (4);
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = m68k_areg(regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = get_byte_ce (dsta);
	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_byte_ce (dsta,newv);
}}}}}}}	m68k_incpc (4);
return 0;
}
/* ADD.B #<data>.B,(d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_0628_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce_prefetch (4);
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (6);
{	uae_s8 dst = get_byte_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_byte_ce (dsta,newv);
}}}}}}}	m68k_incpc (6);
return 0;
}
/* ADD.B #<data>.B,(d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_0630_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce_prefetch (4);
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (6));
{	uae_s8 dst = get_byte_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_byte_ce (dsta,newv);
}}}}}}}	m68k_incpc (6);
return 0;
}
/* ADD.B #<data>.B,(xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_0638_6)(uae_u32 opcode)
{
{{	uae_s8 src = (uae_u8)get_word_ce_prefetch (4);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (6);
{	uae_s8 dst = get_byte_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_byte_ce (dsta,newv);
}}}}}}}	m68k_incpc (6);
return 0;
}
/* ADD.B #<data>.B,(xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_0639_6)(uae_u32 opcode)
{
{{	uae_s8 src = (uae_u8)get_word_ce_prefetch (4);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (6) << 16;
	dsta |= get_word_ce_prefetch (8);
{	uae_s8 dst = get_byte_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (10);
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_byte_ce (dsta,newv);
}}}}}}}	m68k_incpc (8);
return 0;
}
/* ADD.W #<data>.W,Dn */
unsigned long REGPARAM2 CPUFUNC(op_0640_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xffff) | ((newv) & 0xffff);
}}}}}}	m68k_incpc (4);
return 0;
}
/* ADD.W #<data>.W,(An) */
unsigned long REGPARAM2 CPUFUNC(op_0650_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel3862;
	}
{{	uae_s16 dst = get_word_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_word_ce (dsta,newv);
}}}}}}}}	m68k_incpc (4);
endlabel3862: ;
return 0;
}
/* ADD.W #<data>.W,(An)+ */
unsigned long REGPARAM2 CPUFUNC(op_0658_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel3863;
	}
{{	uae_s16 dst = get_word_ce (dsta);
	m68k_areg(regs, dstreg) += 2;
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_word_ce (dsta,newv);
}}}}}}}}	m68k_incpc (4);
endlabel3863: ;
return 0;
}
/* ADD.W #<data>.W,-(An) */
unsigned long REGPARAM2 CPUFUNC(op_0660_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = m68k_areg(regs, dstreg) - 2;
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel3864;
	}
{{	uae_s16 dst = get_word_ce (dsta);
	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_word_ce (dsta,newv);
}}}}}}}}	m68k_incpc (4);
endlabel3864: ;
return 0;
}
/* ADD.W #<data>.W,(d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_0668_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (6);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel3865;
	}
{{	uae_s16 dst = get_word_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_word_ce (dsta,newv);
}}}}}}}}	m68k_incpc (6);
endlabel3865: ;
return 0;
}
/* ADD.W #<data>.W,(d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_0670_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (6));
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel3866;
	}
{{	uae_s16 dst = get_word_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_word_ce (dsta,newv);
}}}}}}}}	m68k_incpc (6);
endlabel3866: ;
return 0;
}
/* ADD.W #<data>.W,(xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_0678_6)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (6);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel3867;
	}
{{	uae_s16 dst = get_word_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_word_ce (dsta,newv);
}}}}}}}}	m68k_incpc (6);
endlabel3867: ;
return 0;
}
/* ADD.W #<data>.W,(xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_0679_6)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (6) << 16;
	dsta |= get_word_ce_prefetch (8);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel3868;
	}
{{	uae_s16 dst = get_word_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (10);
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_word_ce (dsta,newv);
}}}}}}}}	m68k_incpc (8);
endlabel3868: ;
return 0;
}
/* ADD.L #<data>.L,Dn */
unsigned long REGPARAM2 CPUFUNC(op_0680_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	do_cycles (4 * 256);
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	m68k_dreg(regs, dstreg) = (newv);
}}}}}}	m68k_incpc (6);
return 0;
}
/* ADD.L #<data>.L,(An) */
unsigned long REGPARAM2 CPUFUNC(op_0690_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel3870;
	}
{{	uae_s32 dst = get_word_ce (dsta) << 16; dst |= get_word_ce (dsta + 2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_word_ce (dsta, newv >> 16); put_word_ce (dsta + 2, newv);
}}}}}}}}	m68k_incpc (6);
endlabel3870: ;
return 0;
}
/* ADD.L #<data>.L,(An)+ */
unsigned long REGPARAM2 CPUFUNC(op_0698_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel3871;
	}
{{	uae_s32 dst = get_word_ce (dsta) << 16; dst |= get_word_ce (dsta + 2);
	m68k_areg(regs, dstreg) += 4;
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_word_ce (dsta, newv >> 16); put_word_ce (dsta + 2, newv);
}}}}}}}}	m68k_incpc (6);
endlabel3871: ;
return 0;
}
/* ADD.L #<data>.L,-(An) */
unsigned long REGPARAM2 CPUFUNC(op_06a0_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = m68k_areg(regs, dstreg) - 4;
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel3872;
	}
{{	uae_s32 dst = get_word_ce (dsta) << 16; dst |= get_word_ce (dsta + 2);
	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_word_ce (dsta, newv >> 16); put_word_ce (dsta + 2, newv);
}}}}}}}}	m68k_incpc (6);
endlabel3872: ;
return 0;
}
/* ADD.L #<data>.L,(d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_06a8_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (8);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel3873;
	}
{{	uae_s32 dst = get_word_ce (dsta) << 16; dst |= get_word_ce (dsta + 2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (10);
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_word_ce (dsta, newv >> 16); put_word_ce (dsta + 2, newv);
}}}}}}}}	m68k_incpc (8);
endlabel3873: ;
return 0;
}
/* ADD.L #<data>.L,(d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_06b0_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (8));
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel3874;
	}
{{	uae_s32 dst = get_word_ce (dsta) << 16; dst |= get_word_ce (dsta + 2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (10);
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_word_ce (dsta, newv >> 16); put_word_ce (dsta + 2, newv);
}}}}}}}}	m68k_incpc (8);
endlabel3874: ;
return 0;
}
/* ADD.L #<data>.L,(xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_06b8_6)(uae_u32 opcode)
{
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (8);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel3875;
	}
{{	uae_s32 dst = get_word_ce (dsta) << 16; dst |= get_word_ce (dsta + 2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (10);
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_word_ce (dsta, newv >> 16); put_word_ce (dsta + 2, newv);
}}}}}}}}	m68k_incpc (8);
endlabel3875: ;
return 0;
}
/* ADD.L #<data>.L,(xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_06b9_6)(uae_u32 opcode)
{
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (8) << 16;
	dsta |= get_word_ce_prefetch (10);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel3876;
	}
{{	uae_s32 dst = get_word_ce (dsta) << 16; dst |= get_word_ce (dsta + 2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (12);
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_word_ce (dsta, newv >> 16); put_word_ce (dsta + 2, newv);
}}}}}}}}	m68k_incpc (10);
endlabel3876: ;
return 0;
}
/* BTST.L #<data>.W,Dn */
unsigned long REGPARAM2 CPUFUNC(op_0800_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	src &= 31;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	do_cycles (2 * 256);
}}}	m68k_incpc (4);
return 0;
}
/* BTST.B #<data>.W,(An) */
unsigned long REGPARAM2 CPUFUNC(op_0810_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta = m68k_areg(regs, dstreg);
{	uae_s8 dst = get_byte_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
}}}}	m68k_incpc (4);
return 0;
}
/* BTST.B #<data>.W,(An)+ */
unsigned long REGPARAM2 CPUFUNC(op_0818_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta = m68k_areg(regs, dstreg);
{	uae_s8 dst = get_byte_ce (dsta);
	m68k_areg(regs, dstreg) += areg_byteinc[dstreg];
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
}}}}	m68k_incpc (4);
return 0;
}
/* BTST.B #<data>.W,-(An) */
unsigned long REGPARAM2 CPUFUNC(op_0820_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = m68k_areg(regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = get_byte_ce (dsta);
	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
}}}}	m68k_incpc (4);
return 0;
}
/* BTST.B #<data>.W,(d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_0828_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (6);
{	uae_s8 dst = get_byte_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
}}}}	m68k_incpc (6);
return 0;
}
/* BTST.B #<data>.W,(d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_0830_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (6));
{	uae_s8 dst = get_byte_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
}}}}	m68k_incpc (6);
return 0;
}
/* BTST.B #<data>.W,(xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_0838_6)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (6);
{	uae_s8 dst = get_byte_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
}}}}	m68k_incpc (6);
return 0;
}
/* BTST.B #<data>.W,(xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_0839_6)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (6) << 16;
	dsta |= get_word_ce_prefetch (8);
{	uae_s8 dst = get_byte_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (10);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
}}}}	m68k_incpc (8);
return 0;
}
/* BTST.B #<data>.W,(d16,PC) */
unsigned long REGPARAM2 CPUFUNC(op_083a_6)(uae_u32 opcode)
{
	uae_u32 dstreg = 2;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta = m68k_getpc () + 4;
	dsta += (uae_s32)(uae_s16)get_word_ce_prefetch (6);
{	uae_s8 dst = get_byte_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
}}}}	m68k_incpc (6);
return 0;
}
/* BTST.B #<data>.W,(d8,PC,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_083b_6)(uae_u32 opcode)
{
	uae_u32 dstreg = 3;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr tmppc;
	uaecptr dsta;
	do_cycles (2 * 256);
	tmppc = m68k_getpc() + 4;
	dsta = get_disp_ea_000(tmppc, get_word_ce_prefetch (6));
{	uae_s8 dst = get_byte_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
}}}}	m68k_incpc (6);
return 0;
}
/* BTST.B #<data>.W,#<data>.B */
unsigned long REGPARAM2 CPUFUNC(op_083c_6)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uae_s8 dst = (uae_u8)get_word_ce_prefetch (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
}}}	m68k_incpc (6);
return 0;
}
/* BCHG.L #<data>.W,Dn */
unsigned long REGPARAM2 CPUFUNC(op_0840_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	src &= 31;
	dst ^= (1 << src);
	SET_ZFLG (((uae_u32)dst & (1 << src)) >> src);
	do_cycles (4 * 256);
	m68k_dreg(regs, dstreg) = (dst);
}}}	m68k_incpc (4);
return 0;
}
/* BCHG.B #<data>.W,(An) */
unsigned long REGPARAM2 CPUFUNC(op_0850_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta = m68k_areg(regs, dstreg);
{	uae_s8 dst = get_byte_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	src &= 7;
	dst ^= (1 << src);
	SET_ZFLG (((uae_u32)dst & (1 << src)) >> src);
	put_byte_ce (dsta,dst);
}}}}	m68k_incpc (4);
return 0;
}
/* BCHG.B #<data>.W,(An)+ */
unsigned long REGPARAM2 CPUFUNC(op_0858_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta = m68k_areg(regs, dstreg);
{	uae_s8 dst = get_byte_ce (dsta);
	m68k_areg(regs, dstreg) += areg_byteinc[dstreg];
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	src &= 7;
	dst ^= (1 << src);
	SET_ZFLG (((uae_u32)dst & (1 << src)) >> src);
	put_byte_ce (dsta,dst);
}}}}	m68k_incpc (4);
return 0;
}
/* BCHG.B #<data>.W,-(An) */
unsigned long REGPARAM2 CPUFUNC(op_0860_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = m68k_areg(regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = get_byte_ce (dsta);
	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	src &= 7;
	dst ^= (1 << src);
	SET_ZFLG (((uae_u32)dst & (1 << src)) >> src);
	put_byte_ce (dsta,dst);
}}}}	m68k_incpc (4);
return 0;
}
/* BCHG.B #<data>.W,(d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_0868_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (6);
{	uae_s8 dst = get_byte_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	src &= 7;
	dst ^= (1 << src);
	SET_ZFLG (((uae_u32)dst & (1 << src)) >> src);
	put_byte_ce (dsta,dst);
}}}}	m68k_incpc (6);
return 0;
}
/* BCHG.B #<data>.W,(d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_0870_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (6));
{	uae_s8 dst = get_byte_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	src &= 7;
	dst ^= (1 << src);
	SET_ZFLG (((uae_u32)dst & (1 << src)) >> src);
	put_byte_ce (dsta,dst);
}}}}	m68k_incpc (6);
return 0;
}
/* BCHG.B #<data>.W,(xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_0878_6)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (6);
{	uae_s8 dst = get_byte_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	src &= 7;
	dst ^= (1 << src);
	SET_ZFLG (((uae_u32)dst & (1 << src)) >> src);
	put_byte_ce (dsta,dst);
}}}}	m68k_incpc (6);
return 0;
}
/* BCHG.B #<data>.W,(xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_0879_6)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (6) << 16;
	dsta |= get_word_ce_prefetch (8);
{	uae_s8 dst = get_byte_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (10);
	src &= 7;
	dst ^= (1 << src);
	SET_ZFLG (((uae_u32)dst & (1 << src)) >> src);
	put_byte_ce (dsta,dst);
}}}}	m68k_incpc (8);
return 0;
}
/* BCHG.B #<data>.W,(d16,PC) */
unsigned long REGPARAM2 CPUFUNC(op_087a_6)(uae_u32 opcode)
{
	uae_u32 dstreg = 2;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta = m68k_getpc () + 4;
	dsta += (uae_s32)(uae_s16)get_word_ce_prefetch (6);
{	uae_s8 dst = get_byte_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	src &= 7;
	dst ^= (1 << src);
	SET_ZFLG (((uae_u32)dst & (1 << src)) >> src);
	put_byte_ce (dsta,dst);
}}}}	m68k_incpc (6);
return 0;
}
/* BCHG.B #<data>.W,(d8,PC,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_087b_6)(uae_u32 opcode)
{
	uae_u32 dstreg = 3;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr tmppc;
	uaecptr dsta;
	do_cycles (2 * 256);
	tmppc = m68k_getpc() + 4;
	dsta = get_disp_ea_000(tmppc, get_word_ce_prefetch (6));
{	uae_s8 dst = get_byte_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	src &= 7;
	dst ^= (1 << src);
	SET_ZFLG (((uae_u32)dst & (1 << src)) >> src);
	put_byte_ce (dsta,dst);
}}}}	m68k_incpc (6);
return 0;
}
/* BCLR.L #<data>.W,Dn */
unsigned long REGPARAM2 CPUFUNC(op_0880_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	src &= 31;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst &= ~(1 << src);
	do_cycles (6 * 256);
	m68k_dreg(regs, dstreg) = (dst);
}}}	m68k_incpc (4);
return 0;
}
/* BCLR.B #<data>.W,(An) */
unsigned long REGPARAM2 CPUFUNC(op_0890_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta = m68k_areg(regs, dstreg);
{	uae_s8 dst = get_byte_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst &= ~(1 << src);
	put_byte_ce (dsta,dst);
}}}}	m68k_incpc (4);
return 0;
}
/* BCLR.B #<data>.W,(An)+ */
unsigned long REGPARAM2 CPUFUNC(op_0898_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta = m68k_areg(regs, dstreg);
{	uae_s8 dst = get_byte_ce (dsta);
	m68k_areg(regs, dstreg) += areg_byteinc[dstreg];
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst &= ~(1 << src);
	put_byte_ce (dsta,dst);
}}}}	m68k_incpc (4);
return 0;
}
/* BCLR.B #<data>.W,-(An) */
unsigned long REGPARAM2 CPUFUNC(op_08a0_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = m68k_areg(regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = get_byte_ce (dsta);
	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst &= ~(1 << src);
	put_byte_ce (dsta,dst);
}}}}	m68k_incpc (4);
return 0;
}
/* BCLR.B #<data>.W,(d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_08a8_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (6);
{	uae_s8 dst = get_byte_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst &= ~(1 << src);
	put_byte_ce (dsta,dst);
}}}}	m68k_incpc (6);
return 0;
}
/* BCLR.B #<data>.W,(d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_08b0_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (6));
{	uae_s8 dst = get_byte_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst &= ~(1 << src);
	put_byte_ce (dsta,dst);
}}}}	m68k_incpc (6);
return 0;
}
/* BCLR.B #<data>.W,(xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_08b8_6)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (6);
{	uae_s8 dst = get_byte_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst &= ~(1 << src);
	put_byte_ce (dsta,dst);
}}}}	m68k_incpc (6);
return 0;
}
/* BCLR.B #<data>.W,(xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_08b9_6)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (6) << 16;
	dsta |= get_word_ce_prefetch (8);
{	uae_s8 dst = get_byte_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (10);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst &= ~(1 << src);
	put_byte_ce (dsta,dst);
}}}}	m68k_incpc (8);
return 0;
}
/* BCLR.B #<data>.W,(d16,PC) */
unsigned long REGPARAM2 CPUFUNC(op_08ba_6)(uae_u32 opcode)
{
	uae_u32 dstreg = 2;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta = m68k_getpc () + 4;
	dsta += (uae_s32)(uae_s16)get_word_ce_prefetch (6);
{	uae_s8 dst = get_byte_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst &= ~(1 << src);
	put_byte_ce (dsta,dst);
}}}}	m68k_incpc (6);
return 0;
}
/* BCLR.B #<data>.W,(d8,PC,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_08bb_6)(uae_u32 opcode)
{
	uae_u32 dstreg = 3;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr tmppc;
	uaecptr dsta;
	do_cycles (2 * 256);
	tmppc = m68k_getpc() + 4;
	dsta = get_disp_ea_000(tmppc, get_word_ce_prefetch (6));
{	uae_s8 dst = get_byte_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst &= ~(1 << src);
	put_byte_ce (dsta,dst);
}}}}	m68k_incpc (6);
return 0;
}
/* BSET.L #<data>.W,Dn */
unsigned long REGPARAM2 CPUFUNC(op_08c0_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	src &= 31;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst |= (1 << src);
	do_cycles (4 * 256);
	m68k_dreg(regs, dstreg) = (dst);
}}}	m68k_incpc (4);
return 0;
}
/* BSET.B #<data>.W,(An) */
unsigned long REGPARAM2 CPUFUNC(op_08d0_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta = m68k_areg(regs, dstreg);
{	uae_s8 dst = get_byte_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst |= (1 << src);
	put_byte_ce (dsta,dst);
}}}}	m68k_incpc (4);
return 0;
}
/* BSET.B #<data>.W,(An)+ */
unsigned long REGPARAM2 CPUFUNC(op_08d8_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta = m68k_areg(regs, dstreg);
{	uae_s8 dst = get_byte_ce (dsta);
	m68k_areg(regs, dstreg) += areg_byteinc[dstreg];
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst |= (1 << src);
	put_byte_ce (dsta,dst);
}}}}	m68k_incpc (4);
return 0;
}
/* BSET.B #<data>.W,-(An) */
unsigned long REGPARAM2 CPUFUNC(op_08e0_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = m68k_areg(regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = get_byte_ce (dsta);
	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst |= (1 << src);
	put_byte_ce (dsta,dst);
}}}}	m68k_incpc (4);
return 0;
}
/* BSET.B #<data>.W,(d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_08e8_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (6);
{	uae_s8 dst = get_byte_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst |= (1 << src);
	put_byte_ce (dsta,dst);
}}}}	m68k_incpc (6);
return 0;
}
/* BSET.B #<data>.W,(d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_08f0_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (6));
{	uae_s8 dst = get_byte_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst |= (1 << src);
	put_byte_ce (dsta,dst);
}}}}	m68k_incpc (6);
return 0;
}
/* BSET.B #<data>.W,(xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_08f8_6)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (6);
{	uae_s8 dst = get_byte_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst |= (1 << src);
	put_byte_ce (dsta,dst);
}}}}	m68k_incpc (6);
return 0;
}
/* BSET.B #<data>.W,(xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_08f9_6)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (6) << 16;
	dsta |= get_word_ce_prefetch (8);
{	uae_s8 dst = get_byte_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (10);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst |= (1 << src);
	put_byte_ce (dsta,dst);
}}}}	m68k_incpc (8);
return 0;
}
/* BSET.B #<data>.W,(d16,PC) */
unsigned long REGPARAM2 CPUFUNC(op_08fa_6)(uae_u32 opcode)
{
	uae_u32 dstreg = 2;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta = m68k_getpc () + 4;
	dsta += (uae_s32)(uae_s16)get_word_ce_prefetch (6);
{	uae_s8 dst = get_byte_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst |= (1 << src);
	put_byte_ce (dsta,dst);
}}}}	m68k_incpc (6);
return 0;
}
/* BSET.B #<data>.W,(d8,PC,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_08fb_6)(uae_u32 opcode)
{
	uae_u32 dstreg = 3;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr tmppc;
	uaecptr dsta;
	do_cycles (2 * 256);
	tmppc = m68k_getpc() + 4;
	dsta = get_disp_ea_000(tmppc, get_word_ce_prefetch (6));
{	uae_s8 dst = get_byte_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst |= (1 << src);
	put_byte_ce (dsta,dst);
}}}}	m68k_incpc (6);
return 0;
}
/* EOR.B #<data>.B,Dn */
unsigned long REGPARAM2 CPUFUNC(op_0a00_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce_prefetch (4);
{	uae_s8 dst = m68k_dreg(regs, dstreg);
	src ^= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xff) | ((src) & 0xff);
}}}	m68k_incpc (4);
return 0;
}
/* EOR.B #<data>.B,(An) */
unsigned long REGPARAM2 CPUFUNC(op_0a10_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce_prefetch (4);
{	uaecptr dsta = m68k_areg(regs, dstreg);
{	uae_s8 dst = get_byte_ce (dsta);
	src ^= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	put_byte_ce (dsta,src);
}}}}	m68k_incpc (4);
return 0;
}
/* EOR.B #<data>.B,(An)+ */
unsigned long REGPARAM2 CPUFUNC(op_0a18_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce_prefetch (4);
{	uaecptr dsta = m68k_areg(regs, dstreg);
{	uae_s8 dst = get_byte_ce (dsta);
	m68k_areg(regs, dstreg) += areg_byteinc[dstreg];
	src ^= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	put_byte_ce (dsta,src);
}}}}	m68k_incpc (4);
return 0;
}
/* EOR.B #<data>.B,-(An) */
unsigned long REGPARAM2 CPUFUNC(op_0a20_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce_prefetch (4);
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = m68k_areg(regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = get_byte_ce (dsta);
	m68k_areg (regs, dstreg) = dsta;
	src ^= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	put_byte_ce (dsta,src);
}}}}	m68k_incpc (4);
return 0;
}
/* EOR.B #<data>.B,(d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_0a28_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce_prefetch (4);
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (6);
{	uae_s8 dst = get_byte_ce (dsta);
	src ^= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	put_byte_ce (dsta,src);
}}}}	m68k_incpc (6);
return 0;
}
/* EOR.B #<data>.B,(d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_0a30_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce_prefetch (4);
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (6));
{	uae_s8 dst = get_byte_ce (dsta);
	src ^= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	put_byte_ce (dsta,src);
}}}}	m68k_incpc (6);
return 0;
}
/* EOR.B #<data>.B,(xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_0a38_6)(uae_u32 opcode)
{
{{	uae_s8 src = (uae_u8)get_word_ce_prefetch (4);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (6);
{	uae_s8 dst = get_byte_ce (dsta);
	src ^= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	put_byte_ce (dsta,src);
}}}}	m68k_incpc (6);
return 0;
}
/* EOR.B #<data>.B,(xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_0a39_6)(uae_u32 opcode)
{
{{	uae_s8 src = (uae_u8)get_word_ce_prefetch (4);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (6) << 16;
	dsta |= get_word_ce_prefetch (8);
{	uae_s8 dst = get_byte_ce (dsta);
	src ^= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (10);
	put_byte_ce (dsta,src);
}}}}	m68k_incpc (8);
return 0;
}
/* EORSR.B #<data>.W */
unsigned long REGPARAM2 CPUFUNC(op_0a3c_6)(uae_u32 opcode)
{
{	MakeSR();
{	uae_s16 src = get_word_ce_prefetch (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	src &= 0xFF;
	do_cycles (8 * 256);
	regs.sr ^= src;
	MakeFromSR();
}}	m68k_incpc (4);
return 0;
}
/* EOR.W #<data>.W,Dn */
unsigned long REGPARAM2 CPUFUNC(op_0a40_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	src ^= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xffff) | ((src) & 0xffff);
}}}	m68k_incpc (4);
return 0;
}
/* EOR.W #<data>.W,(An) */
unsigned long REGPARAM2 CPUFUNC(op_0a50_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel3928;
	}
{{	uae_s16 dst = get_word_ce (dsta);
	src ^= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	put_word_ce (dsta,src);
}}}}}	m68k_incpc (4);
endlabel3928: ;
return 0;
}
/* EOR.W #<data>.W,(An)+ */
unsigned long REGPARAM2 CPUFUNC(op_0a58_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel3929;
	}
{{	uae_s16 dst = get_word_ce (dsta);
	m68k_areg(regs, dstreg) += 2;
	src ^= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	put_word_ce (dsta,src);
}}}}}	m68k_incpc (4);
endlabel3929: ;
return 0;
}
/* EOR.W #<data>.W,-(An) */
unsigned long REGPARAM2 CPUFUNC(op_0a60_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = m68k_areg(regs, dstreg) - 2;
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel3930;
	}
{{	uae_s16 dst = get_word_ce (dsta);
	m68k_areg (regs, dstreg) = dsta;
	src ^= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	put_word_ce (dsta,src);
}}}}}	m68k_incpc (4);
endlabel3930: ;
return 0;
}
/* EOR.W #<data>.W,(d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_0a68_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (6);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel3931;
	}
{{	uae_s16 dst = get_word_ce (dsta);
	src ^= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	put_word_ce (dsta,src);
}}}}}	m68k_incpc (6);
endlabel3931: ;
return 0;
}
/* EOR.W #<data>.W,(d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_0a70_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (6));
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel3932;
	}
{{	uae_s16 dst = get_word_ce (dsta);
	src ^= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	put_word_ce (dsta,src);
}}}}}	m68k_incpc (6);
endlabel3932: ;
return 0;
}
/* EOR.W #<data>.W,(xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_0a78_6)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (6);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel3933;
	}
{{	uae_s16 dst = get_word_ce (dsta);
	src ^= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	put_word_ce (dsta,src);
}}}}}	m68k_incpc (6);
endlabel3933: ;
return 0;
}
/* EOR.W #<data>.W,(xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_0a79_6)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (6) << 16;
	dsta |= get_word_ce_prefetch (8);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel3934;
	}
{{	uae_s16 dst = get_word_ce (dsta);
	src ^= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (10);
	put_word_ce (dsta,src);
}}}}}	m68k_incpc (8);
endlabel3934: ;
return 0;
}
#endif

#ifdef PART_2
/* EORSR.W #<data>.W */
unsigned long REGPARAM2 CPUFUNC(op_0a7c_6)(uae_u32 opcode)
{
{if (!regs.s) { Exception(8,0); goto endlabel3935; }
{	MakeSR();
{	uae_s16 src = get_word_ce_prefetch (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	do_cycles (8 * 256);
	regs.sr ^= src;
	MakeFromSR();
}}}	m68k_incpc (4);
endlabel3935: ;
return 0;
}
/* EOR.L #<data>.L,Dn */
unsigned long REGPARAM2 CPUFUNC(op_0a80_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	src ^= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	do_cycles (4 * 256);
	m68k_dreg(regs, dstreg) = (src);
}}}	m68k_incpc (6);
return 0;
}
/* EOR.L #<data>.L,(An) */
unsigned long REGPARAM2 CPUFUNC(op_0a90_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel3937;
	}
{{	uae_s32 dst = get_word_ce (dsta) << 16; dst |= get_word_ce (dsta + 2);
	src ^= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
}}}}}	m68k_incpc (6);
endlabel3937: ;
return 0;
}
/* EOR.L #<data>.L,(An)+ */
unsigned long REGPARAM2 CPUFUNC(op_0a98_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel3938;
	}
{{	uae_s32 dst = get_word_ce (dsta) << 16; dst |= get_word_ce (dsta + 2);
	m68k_areg(regs, dstreg) += 4;
	src ^= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
}}}}}	m68k_incpc (6);
endlabel3938: ;
return 0;
}
/* EOR.L #<data>.L,-(An) */
unsigned long REGPARAM2 CPUFUNC(op_0aa0_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = m68k_areg(regs, dstreg) - 4;
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel3939;
	}
{{	uae_s32 dst = get_word_ce (dsta) << 16; dst |= get_word_ce (dsta + 2);
	m68k_areg (regs, dstreg) = dsta;
	src ^= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
}}}}}	m68k_incpc (6);
endlabel3939: ;
return 0;
}
/* EOR.L #<data>.L,(d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_0aa8_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (8);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel3940;
	}
{{	uae_s32 dst = get_word_ce (dsta) << 16; dst |= get_word_ce (dsta + 2);
	src ^= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (10);
	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
}}}}}	m68k_incpc (8);
endlabel3940: ;
return 0;
}
/* EOR.L #<data>.L,(d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_0ab0_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (8));
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel3941;
	}
{{	uae_s32 dst = get_word_ce (dsta) << 16; dst |= get_word_ce (dsta + 2);
	src ^= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (10);
	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
}}}}}	m68k_incpc (8);
endlabel3941: ;
return 0;
}
/* EOR.L #<data>.L,(xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_0ab8_6)(uae_u32 opcode)
{
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (8);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel3942;
	}
{{	uae_s32 dst = get_word_ce (dsta) << 16; dst |= get_word_ce (dsta + 2);
	src ^= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (10);
	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
}}}}}	m68k_incpc (8);
endlabel3942: ;
return 0;
}
/* EOR.L #<data>.L,(xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_0ab9_6)(uae_u32 opcode)
{
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (8) << 16;
	dsta |= get_word_ce_prefetch (10);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel3943;
	}
{{	uae_s32 dst = get_word_ce (dsta) << 16; dst |= get_word_ce (dsta + 2);
	src ^= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (12);
	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
}}}}}	m68k_incpc (10);
endlabel3943: ;
return 0;
}
/* CMP.B #<data>.B,Dn */
unsigned long REGPARAM2 CPUFUNC(op_0c00_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce_prefetch (4);
{	uae_s8 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}	m68k_incpc (4);
return 0;
}
/* CMP.B #<data>.B,(An) */
unsigned long REGPARAM2 CPUFUNC(op_0c10_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce_prefetch (4);
{	uaecptr dsta = m68k_areg(regs, dstreg);
{	uae_s8 dst = get_byte_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}	m68k_incpc (4);
return 0;
}
/* CMP.B #<data>.B,(An)+ */
unsigned long REGPARAM2 CPUFUNC(op_0c18_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce_prefetch (4);
{	uaecptr dsta = m68k_areg(regs, dstreg);
{	uae_s8 dst = get_byte_ce (dsta);
	m68k_areg(regs, dstreg) += areg_byteinc[dstreg];
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}	m68k_incpc (4);
return 0;
}
/* CMP.B #<data>.B,-(An) */
unsigned long REGPARAM2 CPUFUNC(op_0c20_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce_prefetch (4);
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = m68k_areg(regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = get_byte_ce (dsta);
	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}	m68k_incpc (4);
return 0;
}
/* CMP.B #<data>.B,(d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_0c28_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce_prefetch (4);
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (6);
{	uae_s8 dst = get_byte_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}	m68k_incpc (6);
return 0;
}
/* CMP.B #<data>.B,(d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_0c30_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce_prefetch (4);
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (6));
{	uae_s8 dst = get_byte_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}	m68k_incpc (6);
return 0;
}
/* CMP.B #<data>.B,(xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_0c38_6)(uae_u32 opcode)
{
{{	uae_s8 src = (uae_u8)get_word_ce_prefetch (4);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (6);
{	uae_s8 dst = get_byte_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}	m68k_incpc (6);
return 0;
}
/* CMP.B #<data>.B,(xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_0c39_6)(uae_u32 opcode)
{
{{	uae_s8 src = (uae_u8)get_word_ce_prefetch (4);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (6) << 16;
	dsta |= get_word_ce_prefetch (8);
{	uae_s8 dst = get_byte_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (10);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}	m68k_incpc (8);
return 0;
}
/* CMP.B #<data>.B,(d16,PC) */
unsigned long REGPARAM2 CPUFUNC(op_0c3a_6)(uae_u32 opcode)
{
	uae_u32 dstreg = 2;
{{	uae_s8 src = (uae_u8)get_word_ce_prefetch (4);
{	uaecptr dsta = m68k_getpc () + 4;
	dsta += (uae_s32)(uae_s16)get_word_ce_prefetch (6);
{	uae_s8 dst = get_byte_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}	m68k_incpc (6);
return 0;
}
/* CMP.B #<data>.B,(d8,PC,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_0c3b_6)(uae_u32 opcode)
{
	uae_u32 dstreg = 3;
{{	uae_s8 src = (uae_u8)get_word_ce_prefetch (4);
{	uaecptr tmppc;
	uaecptr dsta;
	do_cycles (2 * 256);
	tmppc = m68k_getpc() + 4;
	dsta = get_disp_ea_000(tmppc, get_word_ce_prefetch (6));
{	uae_s8 dst = get_byte_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}	m68k_incpc (6);
return 0;
}
/* CMP.W #<data>.W,Dn */
unsigned long REGPARAM2 CPUFUNC(op_0c40_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}	m68k_incpc (4);
return 0;
}
/* CMP.W #<data>.W,(An) */
unsigned long REGPARAM2 CPUFUNC(op_0c50_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel3955;
	}
{{	uae_s16 dst = get_word_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (4);
endlabel3955: ;
return 0;
}
/* CMP.W #<data>.W,(An)+ */
unsigned long REGPARAM2 CPUFUNC(op_0c58_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel3956;
	}
{{	uae_s16 dst = get_word_ce (dsta);
	m68k_areg(regs, dstreg) += 2;
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (4);
endlabel3956: ;
return 0;
}
/* CMP.W #<data>.W,-(An) */
unsigned long REGPARAM2 CPUFUNC(op_0c60_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = m68k_areg(regs, dstreg) - 2;
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel3957;
	}
{{	uae_s16 dst = get_word_ce (dsta);
	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (4);
endlabel3957: ;
return 0;
}
/* CMP.W #<data>.W,(d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_0c68_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (6);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel3958;
	}
{{	uae_s16 dst = get_word_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (6);
endlabel3958: ;
return 0;
}
/* CMP.W #<data>.W,(d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_0c70_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (6));
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel3959;
	}
{{	uae_s16 dst = get_word_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (6);
endlabel3959: ;
return 0;
}
/* CMP.W #<data>.W,(xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_0c78_6)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (6);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel3960;
	}
{{	uae_s16 dst = get_word_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (6);
endlabel3960: ;
return 0;
}
/* CMP.W #<data>.W,(xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_0c79_6)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (6) << 16;
	dsta |= get_word_ce_prefetch (8);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel3961;
	}
{{	uae_s16 dst = get_word_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (10);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (8);
endlabel3961: ;
return 0;
}
/* CMP.W #<data>.W,(d16,PC) */
unsigned long REGPARAM2 CPUFUNC(op_0c7a_6)(uae_u32 opcode)
{
	uae_u32 dstreg = 2;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta = m68k_getpc () + 4;
	dsta += (uae_s32)(uae_s16)get_word_ce_prefetch (6);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel3962;
	}
{{	uae_s16 dst = get_word_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (6);
endlabel3962: ;
return 0;
}
/* CMP.W #<data>.W,(d8,PC,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_0c7b_6)(uae_u32 opcode)
{
	uae_u32 dstreg = 3;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr tmppc;
	uaecptr dsta;
	do_cycles (2 * 256);
	tmppc = m68k_getpc() + 4;
	dsta = get_disp_ea_000(tmppc, get_word_ce_prefetch (6));
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel3963;
	}
{{	uae_s16 dst = get_word_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (6);
endlabel3963: ;
return 0;
}
/* CMP.L #<data>.L,Dn */
unsigned long REGPARAM2 CPUFUNC(op_0c80_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	do_cycles (2 * 256);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}	m68k_incpc (6);
return 0;
}
/* CMP.L #<data>.L,(An) */
unsigned long REGPARAM2 CPUFUNC(op_0c90_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel3965;
	}
{{	uae_s32 dst = get_word_ce (dsta) << 16; dst |= get_word_ce (dsta + 2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (6);
endlabel3965: ;
return 0;
}
/* CMP.L #<data>.L,(An)+ */
unsigned long REGPARAM2 CPUFUNC(op_0c98_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel3966;
	}
{{	uae_s32 dst = get_word_ce (dsta) << 16; dst |= get_word_ce (dsta + 2);
	m68k_areg(regs, dstreg) += 4;
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (6);
endlabel3966: ;
return 0;
}
/* CMP.L #<data>.L,-(An) */
unsigned long REGPARAM2 CPUFUNC(op_0ca0_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = m68k_areg(regs, dstreg) - 4;
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel3967;
	}
{{	uae_s32 dst = get_word_ce (dsta) << 16; dst |= get_word_ce (dsta + 2);
	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (6);
endlabel3967: ;
return 0;
}
/* CMP.L #<data>.L,(d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_0ca8_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (8);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel3968;
	}
{{	uae_s32 dst = get_word_ce (dsta) << 16; dst |= get_word_ce (dsta + 2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (10);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (8);
endlabel3968: ;
return 0;
}
/* CMP.L #<data>.L,(d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_0cb0_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (8));
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel3969;
	}
{{	uae_s32 dst = get_word_ce (dsta) << 16; dst |= get_word_ce (dsta + 2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (10);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (8);
endlabel3969: ;
return 0;
}
/* CMP.L #<data>.L,(xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_0cb8_6)(uae_u32 opcode)
{
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (8);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel3970;
	}
{{	uae_s32 dst = get_word_ce (dsta) << 16; dst |= get_word_ce (dsta + 2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (10);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (8);
endlabel3970: ;
return 0;
}
/* CMP.L #<data>.L,(xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_0cb9_6)(uae_u32 opcode)
{
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (8) << 16;
	dsta |= get_word_ce_prefetch (10);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel3971;
	}
{{	uae_s32 dst = get_word_ce (dsta) << 16; dst |= get_word_ce (dsta + 2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (12);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (10);
endlabel3971: ;
return 0;
}
/* CMP.L #<data>.L,(d16,PC) */
unsigned long REGPARAM2 CPUFUNC(op_0cba_6)(uae_u32 opcode)
{
	uae_u32 dstreg = 2;
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uaecptr dsta = m68k_getpc () + 6;
	dsta += (uae_s32)(uae_s16)get_word_ce_prefetch (8);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel3972;
	}
{{	uae_s32 dst = get_word_ce (dsta) << 16; dst |= get_word_ce (dsta + 2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (10);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (8);
endlabel3972: ;
return 0;
}
/* CMP.L #<data>.L,(d8,PC,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_0cbb_6)(uae_u32 opcode)
{
	uae_u32 dstreg = 3;
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uaecptr tmppc;
	uaecptr dsta;
	do_cycles (2 * 256);
	tmppc = m68k_getpc() + 6;
	dsta = get_disp_ea_000(tmppc, get_word_ce_prefetch (8));
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel3973;
	}
{{	uae_s32 dst = get_word_ce (dsta) << 16; dst |= get_word_ce (dsta + 2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (10);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (8);
endlabel3973: ;
return 0;
}
/* MOVE.B Dn,Dn */
unsigned long REGPARAM2 CPUFUNC(op_1000_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xff) | ((src) & 0xff);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}return 0;
}
/* MOVE.B (An),Dn */
unsigned long REGPARAM2 CPUFUNC(op_1010_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
{	uae_s8 src = get_byte_ce (srca);
{	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xff) | ((src) & 0xff);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}return 0;
}
/* MOVE.B (An)+,Dn */
unsigned long REGPARAM2 CPUFUNC(op_1018_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
{	uae_s8 src = get_byte_ce (srca);
	m68k_areg(regs, srcreg) += areg_byteinc[srcreg];
{	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xff) | ((src) & 0xff);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}return 0;
}
/* MOVE.B -(An),Dn */
unsigned long REGPARAM2 CPUFUNC(op_1020_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = get_byte_ce (srca);
	m68k_areg (regs, srcreg) = srca;
{	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xff) | ((src) & 0xff);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}return 0;
}
/* MOVE.B (d16,An),Dn */
unsigned long REGPARAM2 CPUFUNC(op_1028_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 src = get_byte_ce (srca);
{	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xff) | ((src) & 0xff);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}return 0;
}
/* MOVE.B (d8,An,Xn),Dn */
unsigned long REGPARAM2 CPUFUNC(op_1030_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
{	uae_s8 src = get_byte_ce (srca);
{	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xff) | ((src) & 0xff);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}return 0;
}
/* MOVE.B (xxx).W,Dn */
unsigned long REGPARAM2 CPUFUNC(op_1038_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 src = get_byte_ce (srca);
{	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xff) | ((src) & 0xff);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}return 0;
}
/* MOVE.B (xxx).L,Dn */
unsigned long REGPARAM2 CPUFUNC(op_1039_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
{	uae_s8 src = get_byte_ce (srca);
{	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xff) | ((src) & 0xff);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}return 0;
}
/* MOVE.B (d16,PC),Dn */
unsigned long REGPARAM2 CPUFUNC(op_103a_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 src = get_byte_ce (srca);
{	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xff) | ((src) & 0xff);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}return 0;
}
/* MOVE.B (d8,PC,Xn),Dn */
unsigned long REGPARAM2 CPUFUNC(op_103b_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	do_cycles (2 * 256);
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
{	uae_s8 src = get_byte_ce (srca);
{	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xff) | ((src) & 0xff);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}return 0;
}
/* MOVE.B #<data>.B,Dn */
unsigned long REGPARAM2 CPUFUNC(op_103c_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s8 src = (uae_u8)get_word_ce_prefetch (4);
{	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xff) | ((src) & 0xff);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}return 0;
}
/* MOVE.B Dn,(An) */
unsigned long REGPARAM2 CPUFUNC(op_1080_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	put_byte_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}return 0;
}
/* MOVE.B (An),(An) */
unsigned long REGPARAM2 CPUFUNC(op_1090_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
{	uae_s8 src = get_byte_ce (srca);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	put_byte_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}return 0;
}
/* MOVE.B (An)+,(An) */
unsigned long REGPARAM2 CPUFUNC(op_1098_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
{	uae_s8 src = get_byte_ce (srca);
	m68k_areg(regs, srcreg) += areg_byteinc[srcreg];
{	uaecptr dsta = m68k_areg(regs, dstreg);
	put_byte_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}return 0;
}
/* MOVE.B -(An),(An) */
unsigned long REGPARAM2 CPUFUNC(op_10a0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = get_byte_ce (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta = m68k_areg(regs, dstreg);
	put_byte_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}return 0;
}
/* MOVE.B (d16,An),(An) */
unsigned long REGPARAM2 CPUFUNC(op_10a8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 src = get_byte_ce (srca);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	put_byte_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}return 0;
}
/* MOVE.B (d8,An,Xn),(An) */
unsigned long REGPARAM2 CPUFUNC(op_10b0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
{	uae_s8 src = get_byte_ce (srca);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	put_byte_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}return 0;
}
/* MOVE.B (xxx).W,(An) */
unsigned long REGPARAM2 CPUFUNC(op_10b8_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 src = get_byte_ce (srca);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	put_byte_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}return 0;
}
/* MOVE.B (xxx).L,(An) */
unsigned long REGPARAM2 CPUFUNC(op_10b9_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
{	uae_s8 src = get_byte_ce (srca);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	put_byte_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}return 0;
}
/* MOVE.B (d16,PC),(An) */
unsigned long REGPARAM2 CPUFUNC(op_10ba_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 src = get_byte_ce (srca);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	put_byte_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}return 0;
}
/* MOVE.B (d8,PC,Xn),(An) */
unsigned long REGPARAM2 CPUFUNC(op_10bb_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	do_cycles (2 * 256);
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
{	uae_s8 src = get_byte_ce (srca);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	put_byte_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}return 0;
}
/* MOVE.B #<data>.B,(An) */
unsigned long REGPARAM2 CPUFUNC(op_10bc_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s8 src = (uae_u8)get_word_ce_prefetch (4);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	put_byte_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}return 0;
}
/* MOVE.B Dn,(An)+ */
unsigned long REGPARAM2 CPUFUNC(op_10c0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	m68k_areg(regs, dstreg) += areg_byteinc[dstreg];
	put_byte_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}return 0;
}
/* MOVE.B (An),(An)+ */
unsigned long REGPARAM2 CPUFUNC(op_10d0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
{	uae_s8 src = get_byte_ce (srca);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	m68k_areg(regs, dstreg) += areg_byteinc[dstreg];
	put_byte_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}return 0;
}
/* MOVE.B (An)+,(An)+ */
unsigned long REGPARAM2 CPUFUNC(op_10d8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
{	uae_s8 src = get_byte_ce (srca);
	m68k_areg(regs, srcreg) += areg_byteinc[srcreg];
{	uaecptr dsta = m68k_areg(regs, dstreg);
	m68k_areg(regs, dstreg) += areg_byteinc[dstreg];
	put_byte_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}return 0;
}
/* MOVE.B -(An),(An)+ */
unsigned long REGPARAM2 CPUFUNC(op_10e0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = get_byte_ce (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta = m68k_areg(regs, dstreg);
	m68k_areg(regs, dstreg) += areg_byteinc[dstreg];
	put_byte_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}return 0;
}
/* MOVE.B (d16,An),(An)+ */
unsigned long REGPARAM2 CPUFUNC(op_10e8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 src = get_byte_ce (srca);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	m68k_areg(regs, dstreg) += areg_byteinc[dstreg];
	put_byte_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}return 0;
}
/* MOVE.B (d8,An,Xn),(An)+ */
unsigned long REGPARAM2 CPUFUNC(op_10f0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
{	uae_s8 src = get_byte_ce (srca);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	m68k_areg(regs, dstreg) += areg_byteinc[dstreg];
	put_byte_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}return 0;
}
/* MOVE.B (xxx).W,(An)+ */
unsigned long REGPARAM2 CPUFUNC(op_10f8_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 src = get_byte_ce (srca);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	m68k_areg(regs, dstreg) += areg_byteinc[dstreg];
	put_byte_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}return 0;
}
/* MOVE.B (xxx).L,(An)+ */
unsigned long REGPARAM2 CPUFUNC(op_10f9_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
{	uae_s8 src = get_byte_ce (srca);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	m68k_areg(regs, dstreg) += areg_byteinc[dstreg];
	put_byte_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}return 0;
}
/* MOVE.B (d16,PC),(An)+ */
unsigned long REGPARAM2 CPUFUNC(op_10fa_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 src = get_byte_ce (srca);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	m68k_areg(regs, dstreg) += areg_byteinc[dstreg];
	put_byte_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}return 0;
}
/* MOVE.B (d8,PC,Xn),(An)+ */
unsigned long REGPARAM2 CPUFUNC(op_10fb_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	do_cycles (2 * 256);
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
{	uae_s8 src = get_byte_ce (srca);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	m68k_areg(regs, dstreg) += areg_byteinc[dstreg];
	put_byte_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}return 0;
}
/* MOVE.B #<data>.B,(An)+ */
unsigned long REGPARAM2 CPUFUNC(op_10fc_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s8 src = (uae_u8)get_word_ce_prefetch (4);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	m68k_areg(regs, dstreg) += areg_byteinc[dstreg];
	put_byte_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}return 0;
}
/* MOVE.B Dn,-(An) */
unsigned long REGPARAM2 CPUFUNC(op_1100_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - areg_byteinc[dstreg];
	m68k_areg (regs, dstreg) = dsta;
	put_byte_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}return 0;
}
/* MOVE.B (An),-(An) */
unsigned long REGPARAM2 CPUFUNC(op_1110_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
{	uae_s8 src = get_byte_ce (srca);
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - areg_byteinc[dstreg];
	m68k_areg (regs, dstreg) = dsta;
	put_byte_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}return 0;
}
/* MOVE.B (An)+,-(An) */
unsigned long REGPARAM2 CPUFUNC(op_1118_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
{	uae_s8 src = get_byte_ce (srca);
	m68k_areg(regs, srcreg) += areg_byteinc[srcreg];
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - areg_byteinc[dstreg];
	m68k_areg (regs, dstreg) = dsta;
	put_byte_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}return 0;
}
/* MOVE.B -(An),-(An) */
unsigned long REGPARAM2 CPUFUNC(op_1120_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = get_byte_ce (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - areg_byteinc[dstreg];
	m68k_areg (regs, dstreg) = dsta;
	put_byte_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}return 0;
}
/* MOVE.B (d16,An),-(An) */
unsigned long REGPARAM2 CPUFUNC(op_1128_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 src = get_byte_ce (srca);
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - areg_byteinc[dstreg];
	m68k_areg (regs, dstreg) = dsta;
	put_byte_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}return 0;
}
/* MOVE.B (d8,An,Xn),-(An) */
unsigned long REGPARAM2 CPUFUNC(op_1130_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
{	uae_s8 src = get_byte_ce (srca);
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - areg_byteinc[dstreg];
	m68k_areg (regs, dstreg) = dsta;
	put_byte_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}return 0;
}
/* MOVE.B (xxx).W,-(An) */
unsigned long REGPARAM2 CPUFUNC(op_1138_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 src = get_byte_ce (srca);
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - areg_byteinc[dstreg];
	m68k_areg (regs, dstreg) = dsta;
	put_byte_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}return 0;
}
/* MOVE.B (xxx).L,-(An) */
unsigned long REGPARAM2 CPUFUNC(op_1139_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
{	uae_s8 src = get_byte_ce (srca);
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - areg_byteinc[dstreg];
	m68k_areg (regs, dstreg) = dsta;
	put_byte_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}return 0;
}
/* MOVE.B (d16,PC),-(An) */
unsigned long REGPARAM2 CPUFUNC(op_113a_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 src = get_byte_ce (srca);
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - areg_byteinc[dstreg];
	m68k_areg (regs, dstreg) = dsta;
	put_byte_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}return 0;
}
/* MOVE.B (d8,PC,Xn),-(An) */
unsigned long REGPARAM2 CPUFUNC(op_113b_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	do_cycles (2 * 256);
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
{	uae_s8 src = get_byte_ce (srca);
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - areg_byteinc[dstreg];
	m68k_areg (regs, dstreg) = dsta;
	put_byte_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}return 0;
}
/* MOVE.B #<data>.B,-(An) */
unsigned long REGPARAM2 CPUFUNC(op_113c_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s8 src = (uae_u8)get_word_ce_prefetch (4);
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - areg_byteinc[dstreg];
	m68k_areg (regs, dstreg) = dsta;
	put_byte_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}return 0;
}
/* MOVE.B Dn,(d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_1140_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	put_byte_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}return 0;
}
/* MOVE.B (An),(d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_1150_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
{	uae_s8 src = get_byte_ce (srca);
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	put_byte_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}return 0;
}
/* MOVE.B (An)+,(d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_1158_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
{	uae_s8 src = get_byte_ce (srca);
	m68k_areg(regs, srcreg) += areg_byteinc[srcreg];
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	put_byte_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}return 0;
}
/* MOVE.B -(An),(d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_1160_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = get_byte_ce (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	put_byte_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}return 0;
}
/* MOVE.B (d16,An),(d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_1168_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 src = get_byte_ce (srca);
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (6);
	put_byte_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}return 0;
}
/* MOVE.B (d8,An,Xn),(d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_1170_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
{	uae_s8 src = get_byte_ce (srca);
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (6);
	put_byte_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}return 0;
}
/* MOVE.B (xxx).W,(d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_1178_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 src = get_byte_ce (srca);
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (6);
	put_byte_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}return 0;
}
/* MOVE.B (xxx).L,(d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_1179_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
{	uae_s8 src = get_byte_ce (srca);
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (8);
	put_byte_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (8);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}return 0;
}
/* MOVE.B (d16,PC),(d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_117a_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 src = get_byte_ce (srca);
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (6);
	put_byte_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}return 0;
}
/* MOVE.B (d8,PC,Xn),(d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_117b_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	do_cycles (2 * 256);
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
{	uae_s8 src = get_byte_ce (srca);
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (6);
	put_byte_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}return 0;
}
/* MOVE.B #<data>.B,(d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_117c_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s8 src = (uae_u8)get_word_ce_prefetch (4);
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (6);
	put_byte_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}return 0;
}
/* MOVE.B Dn,(d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_1180_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (4));
	put_byte_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}return 0;
}
/* MOVE.B (An),(d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_1190_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
{	uae_s8 src = get_byte_ce (srca);
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (4));
	put_byte_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}return 0;
}
/* MOVE.B (An)+,(d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_1198_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
{	uae_s8 src = get_byte_ce (srca);
	m68k_areg(regs, srcreg) += areg_byteinc[srcreg];
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (4));
	put_byte_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}return 0;
}
/* MOVE.B -(An),(d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_11a0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = get_byte_ce (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (4));
	put_byte_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}return 0;
}
/* MOVE.B (d16,An),(d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_11a8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 src = get_byte_ce (srca);
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (6));
	put_byte_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}return 0;
}
/* MOVE.B (d8,An,Xn),(d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_11b0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
{	uae_s8 src = get_byte_ce (srca);
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (6));
	put_byte_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}return 0;
}
/* MOVE.B (xxx).W,(d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_11b8_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 src = get_byte_ce (srca);
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (6));
	put_byte_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}return 0;
}
/* MOVE.B (xxx).L,(d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_11b9_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
{	uae_s8 src = get_byte_ce (srca);
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (8));
	put_byte_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (8);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}return 0;
}
/* MOVE.B (d16,PC),(d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_11ba_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 src = get_byte_ce (srca);
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (6));
	put_byte_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}return 0;
}
/* MOVE.B (d8,PC,Xn),(d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_11bb_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	do_cycles (2 * 256);
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
{	uae_s8 src = get_byte_ce (srca);
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (6));
	put_byte_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}return 0;
}
/* MOVE.B #<data>.B,(d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_11bc_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s8 src = (uae_u8)get_word_ce_prefetch (4);
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (6));
	put_byte_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}return 0;
}
/* MOVE.B Dn,(xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_11c0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	put_byte_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}return 0;
}
/* MOVE.B (An),(xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_11d0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
{	uae_s8 src = get_byte_ce (srca);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	put_byte_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}return 0;
}
/* MOVE.B (An)+,(xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_11d8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
{	uae_s8 src = get_byte_ce (srca);
	m68k_areg(regs, srcreg) += areg_byteinc[srcreg];
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	put_byte_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}return 0;
}
/* MOVE.B -(An),(xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_11e0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = get_byte_ce (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	put_byte_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}return 0;
}
/* MOVE.B (d16,An),(xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_11e8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 src = get_byte_ce (srca);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (6);
	put_byte_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}return 0;
}
/* MOVE.B (d8,An,Xn),(xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_11f0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
{	uae_s8 src = get_byte_ce (srca);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (6);
	put_byte_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}return 0;
}
/* MOVE.B (xxx).W,(xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_11f8_6)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 src = get_byte_ce (srca);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (6);
	put_byte_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}return 0;
}
/* MOVE.B (xxx).L,(xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_11f9_6)(uae_u32 opcode)
{
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
{	uae_s8 src = get_byte_ce (srca);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (8);
	put_byte_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (8);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}return 0;
}
/* MOVE.B (d16,PC),(xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_11fa_6)(uae_u32 opcode)
{
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 src = get_byte_ce (srca);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (6);
	put_byte_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}return 0;
}
/* MOVE.B (d8,PC,Xn),(xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_11fb_6)(uae_u32 opcode)
{
{{	uaecptr tmppc;
	uaecptr srca;
	do_cycles (2 * 256);
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
{	uae_s8 src = get_byte_ce (srca);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (6);
	put_byte_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}return 0;
}
/* MOVE.B #<data>.B,(xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_11fc_6)(uae_u32 opcode)
{
{{	uae_s8 src = (uae_u8)get_word_ce_prefetch (4);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (6);
	put_byte_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}return 0;
}
/* MOVE.B Dn,(xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_13c0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (4) << 16;
	dsta |= get_word_ce_prefetch (6);
	put_byte_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}return 0;
}
/* MOVE.B (An),(xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_13d0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
{	uae_s8 src = get_byte_ce (srca);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (4) << 16;
	dsta |= get_word_ce_prefetch (6);
	put_byte_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}return 0;
}
/* MOVE.B (An)+,(xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_13d8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
{	uae_s8 src = get_byte_ce (srca);
	m68k_areg(regs, srcreg) += areg_byteinc[srcreg];
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (4) << 16;
	dsta |= get_word_ce_prefetch (6);
	put_byte_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}return 0;
}
/* MOVE.B -(An),(xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_13e0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = get_byte_ce (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (4) << 16;
	dsta |= get_word_ce_prefetch (6);
	put_byte_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}return 0;
}
/* MOVE.B (d16,An),(xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_13e8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 src = get_byte_ce (srca);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (6) << 16;
	dsta |= get_word_ce_prefetch (8);
	put_byte_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (8);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}return 0;
}
/* MOVE.B (d8,An,Xn),(xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_13f0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
{	uae_s8 src = get_byte_ce (srca);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (6) << 16;
	dsta |= get_word_ce_prefetch (8);
	put_byte_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (8);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}return 0;
}
/* MOVE.B (xxx).W,(xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_13f8_6)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 src = get_byte_ce (srca);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (6) << 16;
	dsta |= get_word_ce_prefetch (8);
	put_byte_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (8);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}return 0;
}
/* MOVE.B (xxx).L,(xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_13f9_6)(uae_u32 opcode)
{
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
{	uae_s8 src = get_byte_ce (srca);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (8) << 16;
	dsta |= get_word_ce_prefetch (10);
	put_byte_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (10);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}return 0;
}
/* MOVE.B (d16,PC),(xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_13fa_6)(uae_u32 opcode)
{
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 src = get_byte_ce (srca);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (6) << 16;
	dsta |= get_word_ce_prefetch (8);
	put_byte_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (8);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}return 0;
}
/* MOVE.B (d8,PC,Xn),(xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_13fb_6)(uae_u32 opcode)
{
{{	uaecptr tmppc;
	uaecptr srca;
	do_cycles (2 * 256);
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
{	uae_s8 src = get_byte_ce (srca);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (6) << 16;
	dsta |= get_word_ce_prefetch (8);
	put_byte_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (8);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}return 0;
}
/* MOVE.B #<data>.B,(xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_13fc_6)(uae_u32 opcode)
{
{{	uae_s8 src = (uae_u8)get_word_ce_prefetch (4);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (6) << 16;
	dsta |= get_word_ce_prefetch (8);
	put_byte_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (8);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}return 0;
}
/* MOVE.L Dn,Dn */
unsigned long REGPARAM2 CPUFUNC(op_2000_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	m68k_dreg(regs, dstreg) = (src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}return 0;
}
/* MOVE.L An,Dn */
unsigned long REGPARAM2 CPUFUNC(op_2008_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_areg(regs, srcreg);
{	m68k_dreg(regs, dstreg) = (src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}return 0;
}
/* MOVE.L (An),Dn */
unsigned long REGPARAM2 CPUFUNC(op_2010_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4064;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	m68k_dreg(regs, dstreg) = (src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}endlabel4064: ;
return 0;
}
/* MOVE.L (An)+,Dn */
unsigned long REGPARAM2 CPUFUNC(op_2018_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4065;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	m68k_areg(regs, srcreg) += 4;
{	m68k_dreg(regs, dstreg) = (src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}endlabel4065: ;
return 0;
}
/* MOVE.L -(An),Dn */
unsigned long REGPARAM2 CPUFUNC(op_2020_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = m68k_areg(regs, srcreg) - 4;
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4066;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	m68k_areg (regs, srcreg) = srca;
{	m68k_dreg(regs, dstreg) = (src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}endlabel4066: ;
return 0;
}
/* MOVE.L (d16,An),Dn */
unsigned long REGPARAM2 CPUFUNC(op_2028_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4067;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	m68k_dreg(regs, dstreg) = (src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}endlabel4067: ;
return 0;
}
/* MOVE.L (d8,An,Xn),Dn */
unsigned long REGPARAM2 CPUFUNC(op_2030_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4068;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	m68k_dreg(regs, dstreg) = (src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}endlabel4068: ;
return 0;
}
/* MOVE.L (xxx).W,Dn */
unsigned long REGPARAM2 CPUFUNC(op_2038_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4069;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	m68k_dreg(regs, dstreg) = (src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}endlabel4069: ;
return 0;
}
/* MOVE.L (xxx).L,Dn */
unsigned long REGPARAM2 CPUFUNC(op_2039_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4070;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	m68k_dreg(regs, dstreg) = (src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}endlabel4070: ;
return 0;
}
/* MOVE.L (d16,PC),Dn */
unsigned long REGPARAM2 CPUFUNC(op_203a_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4071;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	m68k_dreg(regs, dstreg) = (src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}endlabel4071: ;
return 0;
}
/* MOVE.L (d8,PC,Xn),Dn */
unsigned long REGPARAM2 CPUFUNC(op_203b_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	do_cycles (2 * 256);
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4072;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	m68k_dreg(regs, dstreg) = (src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}endlabel4072: ;
return 0;
}
/* MOVE.L #<data>.L,Dn */
unsigned long REGPARAM2 CPUFUNC(op_203c_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	m68k_dreg(regs, dstreg) = (src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}return 0;
}
/* MOVEA.L Dn,An */
unsigned long REGPARAM2 CPUFUNC(op_2040_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uae_u32 val = src;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	m68k_areg(regs, dstreg) = (val);
}}}	m68k_incpc (2);
return 0;
}
/* MOVEA.L An,An */
unsigned long REGPARAM2 CPUFUNC(op_2048_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_areg(regs, srcreg);
{	uae_u32 val = src;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	m68k_areg(regs, dstreg) = (val);
}}}	m68k_incpc (2);
return 0;
}
/* MOVEA.L (An),An */
unsigned long REGPARAM2 CPUFUNC(op_2050_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4076;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_u32 val = src;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	m68k_areg(regs, dstreg) = (val);
}}}}}	m68k_incpc (2);
endlabel4076: ;
return 0;
}
/* MOVEA.L (An)+,An */
unsigned long REGPARAM2 CPUFUNC(op_2058_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4077;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	m68k_areg(regs, srcreg) += 4;
{	uae_u32 val = src;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	m68k_areg(regs, dstreg) = (val);
}}}}}	m68k_incpc (2);
endlabel4077: ;
return 0;
}
/* MOVEA.L -(An),An */
unsigned long REGPARAM2 CPUFUNC(op_2060_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = m68k_areg(regs, srcreg) - 4;
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4078;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	m68k_areg (regs, srcreg) = srca;
{	uae_u32 val = src;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	m68k_areg(regs, dstreg) = (val);
}}}}}	m68k_incpc (2);
endlabel4078: ;
return 0;
}
/* MOVEA.L (d16,An),An */
unsigned long REGPARAM2 CPUFUNC(op_2068_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4079;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_u32 val = src;
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	m68k_areg(regs, dstreg) = (val);
}}}}}	m68k_incpc (4);
endlabel4079: ;
return 0;
}
/* MOVEA.L (d8,An,Xn),An */
unsigned long REGPARAM2 CPUFUNC(op_2070_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4080;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_u32 val = src;
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	m68k_areg(regs, dstreg) = (val);
}}}}}	m68k_incpc (4);
endlabel4080: ;
return 0;
}
/* MOVEA.L (xxx).W,An */
unsigned long REGPARAM2 CPUFUNC(op_2078_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4081;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_u32 val = src;
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	m68k_areg(regs, dstreg) = (val);
}}}}}	m68k_incpc (4);
endlabel4081: ;
return 0;
}
/* MOVEA.L (xxx).L,An */
unsigned long REGPARAM2 CPUFUNC(op_2079_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4082;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_u32 val = src;
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	m68k_areg(regs, dstreg) = (val);
}}}}}	m68k_incpc (6);
endlabel4082: ;
return 0;
}
/* MOVEA.L (d16,PC),An */
unsigned long REGPARAM2 CPUFUNC(op_207a_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4083;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_u32 val = src;
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	m68k_areg(regs, dstreg) = (val);
}}}}}	m68k_incpc (4);
endlabel4083: ;
return 0;
}
/* MOVEA.L (d8,PC,Xn),An */
unsigned long REGPARAM2 CPUFUNC(op_207b_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	do_cycles (2 * 256);
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4084;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_u32 val = src;
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	m68k_areg(regs, dstreg) = (val);
}}}}}	m68k_incpc (4);
endlabel4084: ;
return 0;
}
/* MOVEA.L #<data>.L,An */
unsigned long REGPARAM2 CPUFUNC(op_207c_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uae_u32 val = src;
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	m68k_areg(regs, dstreg) = (val);
}}}	m68k_incpc (6);
return 0;
}
/* MOVE.L Dn,(An) */
unsigned long REGPARAM2 CPUFUNC(op_2080_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4086;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}endlabel4086: ;
return 0;
}
/* MOVE.L An,(An) */
unsigned long REGPARAM2 CPUFUNC(op_2088_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_areg(regs, srcreg);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4087;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}endlabel4087: ;
return 0;
}
/* MOVE.L (An),(An) */
unsigned long REGPARAM2 CPUFUNC(op_2090_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4088;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4088;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4088: ;
return 0;
}
/* MOVE.L (An)+,(An) */
unsigned long REGPARAM2 CPUFUNC(op_2098_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4089;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	m68k_areg(regs, srcreg) += 4;
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4089;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4089: ;
return 0;
}
/* MOVE.L -(An),(An) */
unsigned long REGPARAM2 CPUFUNC(op_20a0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = m68k_areg(regs, srcreg) - 4;
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4090;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4090;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4090: ;
return 0;
}
/* MOVE.L (d16,An),(An) */
unsigned long REGPARAM2 CPUFUNC(op_20a8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4091;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4091;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4091: ;
return 0;
}
/* MOVE.L (d8,An,Xn),(An) */
unsigned long REGPARAM2 CPUFUNC(op_20b0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4092;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4092;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4092: ;
return 0;
}
/* MOVE.L (xxx).W,(An) */
unsigned long REGPARAM2 CPUFUNC(op_20b8_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4093;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4093;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4093: ;
return 0;
}
/* MOVE.L (xxx).L,(An) */
unsigned long REGPARAM2 CPUFUNC(op_20b9_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4094;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel4094;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4094: ;
return 0;
}
/* MOVE.L (d16,PC),(An) */
unsigned long REGPARAM2 CPUFUNC(op_20ba_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4095;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4095;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4095: ;
return 0;
}
/* MOVE.L (d8,PC,Xn),(An) */
unsigned long REGPARAM2 CPUFUNC(op_20bb_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	do_cycles (2 * 256);
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4096;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4096;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4096: ;
return 0;
}
/* MOVE.L #<data>.L,(An) */
unsigned long REGPARAM2 CPUFUNC(op_20bc_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel4097;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}endlabel4097: ;
return 0;
}
/* MOVE.L Dn,(An)+ */
unsigned long REGPARAM2 CPUFUNC(op_20c0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4098;
	}
{	m68k_areg(regs, dstreg) += 4;
	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}endlabel4098: ;
return 0;
}
/* MOVE.L An,(An)+ */
unsigned long REGPARAM2 CPUFUNC(op_20c8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_areg(regs, srcreg);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4099;
	}
{	m68k_areg(regs, dstreg) += 4;
	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}endlabel4099: ;
return 0;
}
/* MOVE.L (An),(An)+ */
unsigned long REGPARAM2 CPUFUNC(op_20d0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4100;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4100;
	}
{	m68k_areg(regs, dstreg) += 4;
	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4100: ;
return 0;
}
/* MOVE.L (An)+,(An)+ */
unsigned long REGPARAM2 CPUFUNC(op_20d8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4101;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	m68k_areg(regs, srcreg) += 4;
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4101;
	}
{	m68k_areg(regs, dstreg) += 4;
	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4101: ;
return 0;
}
/* MOVE.L -(An),(An)+ */
unsigned long REGPARAM2 CPUFUNC(op_20e0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = m68k_areg(regs, srcreg) - 4;
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4102;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4102;
	}
{	m68k_areg(regs, dstreg) += 4;
	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4102: ;
return 0;
}
/* MOVE.L (d16,An),(An)+ */
unsigned long REGPARAM2 CPUFUNC(op_20e8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4103;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4103;
	}
{	m68k_areg(regs, dstreg) += 4;
	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4103: ;
return 0;
}
/* MOVE.L (d8,An,Xn),(An)+ */
unsigned long REGPARAM2 CPUFUNC(op_20f0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4104;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4104;
	}
{	m68k_areg(regs, dstreg) += 4;
	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4104: ;
return 0;
}
/* MOVE.L (xxx).W,(An)+ */
unsigned long REGPARAM2 CPUFUNC(op_20f8_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4105;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4105;
	}
{	m68k_areg(regs, dstreg) += 4;
	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4105: ;
return 0;
}
/* MOVE.L (xxx).L,(An)+ */
unsigned long REGPARAM2 CPUFUNC(op_20f9_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4106;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel4106;
	}
{	m68k_areg(regs, dstreg) += 4;
	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4106: ;
return 0;
}
/* MOVE.L (d16,PC),(An)+ */
unsigned long REGPARAM2 CPUFUNC(op_20fa_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4107;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4107;
	}
{	m68k_areg(regs, dstreg) += 4;
	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4107: ;
return 0;
}
/* MOVE.L (d8,PC,Xn),(An)+ */
unsigned long REGPARAM2 CPUFUNC(op_20fb_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	do_cycles (2 * 256);
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4108;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4108;
	}
{	m68k_areg(regs, dstreg) += 4;
	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4108: ;
return 0;
}
/* MOVE.L #<data>.L,(An)+ */
unsigned long REGPARAM2 CPUFUNC(op_20fc_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel4109;
	}
{	m68k_areg(regs, dstreg) += 4;
	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}endlabel4109: ;
return 0;
}
/* MOVE.L Dn,-(An) */
unsigned long REGPARAM2 CPUFUNC(op_2100_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - 4;
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4110;
	}
{	m68k_areg (regs, dstreg) = dsta;
	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}endlabel4110: ;
return 0;
}
/* MOVE.L An,-(An) */
unsigned long REGPARAM2 CPUFUNC(op_2108_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_areg(regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - 4;
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4111;
	}
{	m68k_areg (regs, dstreg) = dsta;
	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}endlabel4111: ;
return 0;
}
/* MOVE.L (An),-(An) */
unsigned long REGPARAM2 CPUFUNC(op_2110_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4112;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - 4;
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4112;
	}
{	m68k_areg (regs, dstreg) = dsta;
	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4112: ;
return 0;
}
/* MOVE.L (An)+,-(An) */
unsigned long REGPARAM2 CPUFUNC(op_2118_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4113;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	m68k_areg(regs, srcreg) += 4;
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - 4;
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4113;
	}
{	m68k_areg (regs, dstreg) = dsta;
	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4113: ;
return 0;
}
/* MOVE.L -(An),-(An) */
unsigned long REGPARAM2 CPUFUNC(op_2120_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = m68k_areg(regs, srcreg) - 4;
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4114;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - 4;
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4114;
	}
{	m68k_areg (regs, dstreg) = dsta;
	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4114: ;
return 0;
}
/* MOVE.L (d16,An),-(An) */
unsigned long REGPARAM2 CPUFUNC(op_2128_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4115;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - 4;
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4115;
	}
{	m68k_areg (regs, dstreg) = dsta;
	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4115: ;
return 0;
}
/* MOVE.L (d8,An,Xn),-(An) */
unsigned long REGPARAM2 CPUFUNC(op_2130_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4116;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - 4;
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4116;
	}
{	m68k_areg (regs, dstreg) = dsta;
	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4116: ;
return 0;
}
/* MOVE.L (xxx).W,-(An) */
unsigned long REGPARAM2 CPUFUNC(op_2138_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4117;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - 4;
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4117;
	}
{	m68k_areg (regs, dstreg) = dsta;
	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4117: ;
return 0;
}
/* MOVE.L (xxx).L,-(An) */
unsigned long REGPARAM2 CPUFUNC(op_2139_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4118;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - 4;
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel4118;
	}
{	m68k_areg (regs, dstreg) = dsta;
	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4118: ;
return 0;
}
/* MOVE.L (d16,PC),-(An) */
unsigned long REGPARAM2 CPUFUNC(op_213a_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4119;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - 4;
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4119;
	}
{	m68k_areg (regs, dstreg) = dsta;
	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4119: ;
return 0;
}
/* MOVE.L (d8,PC,Xn),-(An) */
unsigned long REGPARAM2 CPUFUNC(op_213b_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	do_cycles (2 * 256);
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4120;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - 4;
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4120;
	}
{	m68k_areg (regs, dstreg) = dsta;
	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4120: ;
return 0;
}
/* MOVE.L #<data>.L,-(An) */
unsigned long REGPARAM2 CPUFUNC(op_213c_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - 4;
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel4121;
	}
{	m68k_areg (regs, dstreg) = dsta;
	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}endlabel4121: ;
return 0;
}
/* MOVE.L Dn,(d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_2140_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4122;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}endlabel4122: ;
return 0;
}
/* MOVE.L An,(d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_2148_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_areg(regs, srcreg);
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4123;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}endlabel4123: ;
return 0;
}
#endif

#ifdef PART_3
/* MOVE.L (An),(d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_2150_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4124;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4124;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4124: ;
return 0;
}
/* MOVE.L (An)+,(d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_2158_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4125;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	m68k_areg(regs, srcreg) += 4;
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4125;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4125: ;
return 0;
}
/* MOVE.L -(An),(d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_2160_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = m68k_areg(regs, srcreg) - 4;
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4126;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4126;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4126: ;
return 0;
}
/* MOVE.L (d16,An),(d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_2168_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4127;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (6);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4127;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4127: ;
return 0;
}
/* MOVE.L (d8,An,Xn),(d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_2170_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4128;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (6);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4128;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4128: ;
return 0;
}
/* MOVE.L (xxx).W,(d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_2178_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4129;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (6);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4129;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4129: ;
return 0;
}
/* MOVE.L (xxx).L,(d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_2179_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4130;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (8);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel4130;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (8);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4130: ;
return 0;
}
/* MOVE.L (d16,PC),(d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_217a_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4131;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (6);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4131;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4131: ;
return 0;
}
/* MOVE.L (d8,PC,Xn),(d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_217b_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	do_cycles (2 * 256);
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4132;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (6);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4132;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4132: ;
return 0;
}
/* MOVE.L #<data>.L,(d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_217c_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (8);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel4133;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (8);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}endlabel4133: ;
return 0;
}
/* MOVE.L Dn,(d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_2180_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (4));
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4134;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}endlabel4134: ;
return 0;
}
/* MOVE.L An,(d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_2188_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_areg(regs, srcreg);
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (4));
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4135;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}endlabel4135: ;
return 0;
}
/* MOVE.L (An),(d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_2190_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4136;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (4));
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4136;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4136: ;
return 0;
}
/* MOVE.L (An)+,(d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_2198_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4137;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	m68k_areg(regs, srcreg) += 4;
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (4));
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4137;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4137: ;
return 0;
}
/* MOVE.L -(An),(d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_21a0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = m68k_areg(regs, srcreg) - 4;
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4138;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (4));
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4138;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4138: ;
return 0;
}
/* MOVE.L (d16,An),(d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_21a8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4139;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (6));
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4139;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4139: ;
return 0;
}
/* MOVE.L (d8,An,Xn),(d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_21b0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4140;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (6));
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4140;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4140: ;
return 0;
}
/* MOVE.L (xxx).W,(d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_21b8_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4141;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (6));
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4141;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4141: ;
return 0;
}
/* MOVE.L (xxx).L,(d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_21b9_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4142;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (8));
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel4142;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (8);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4142: ;
return 0;
}
/* MOVE.L (d16,PC),(d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_21ba_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4143;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (6));
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4143;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4143: ;
return 0;
}
/* MOVE.L (d8,PC,Xn),(d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_21bb_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	do_cycles (2 * 256);
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4144;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (6));
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4144;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4144: ;
return 0;
}
/* MOVE.L #<data>.L,(d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_21bc_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (8));
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel4145;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (8);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}endlabel4145: ;
return 0;
}
/* MOVE.L Dn,(xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_21c0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4146;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}endlabel4146: ;
return 0;
}
/* MOVE.L An,(xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_21c8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s32 src = m68k_areg(regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4147;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}endlabel4147: ;
return 0;
}
/* MOVE.L (An),(xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_21d0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4148;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4148;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4148: ;
return 0;
}
/* MOVE.L (An)+,(xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_21d8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4149;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	m68k_areg(regs, srcreg) += 4;
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4149;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4149: ;
return 0;
}
/* MOVE.L -(An),(xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_21e0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = m68k_areg(regs, srcreg) - 4;
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4150;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4150;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4150: ;
return 0;
}
/* MOVE.L (d16,An),(xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_21e8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4151;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (6);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4151;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4151: ;
return 0;
}
/* MOVE.L (d8,An,Xn),(xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_21f0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4152;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (6);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4152;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4152: ;
return 0;
}
/* MOVE.L (xxx).W,(xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_21f8_6)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4153;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (6);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4153;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4153: ;
return 0;
}
/* MOVE.L (xxx).L,(xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_21f9_6)(uae_u32 opcode)
{
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4154;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (8);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel4154;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (8);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4154: ;
return 0;
}
/* MOVE.L (d16,PC),(xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_21fa_6)(uae_u32 opcode)
{
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4155;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (6);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4155;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4155: ;
return 0;
}
/* MOVE.L (d8,PC,Xn),(xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_21fb_6)(uae_u32 opcode)
{
{{	uaecptr tmppc;
	uaecptr srca;
	do_cycles (2 * 256);
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4156;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (6);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4156;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4156: ;
return 0;
}
/* MOVE.L #<data>.L,(xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_21fc_6)(uae_u32 opcode)
{
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (8);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel4157;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (8);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}endlabel4157: ;
return 0;
}
/* MOVE.L Dn,(xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_23c0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (4) << 16;
	dsta |= get_word_ce_prefetch (6);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4158;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}endlabel4158: ;
return 0;
}
/* MOVE.L An,(xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_23c8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s32 src = m68k_areg(regs, srcreg);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (4) << 16;
	dsta |= get_word_ce_prefetch (6);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4159;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}endlabel4159: ;
return 0;
}
/* MOVE.L (An),(xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_23d0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4160;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (4) << 16;
	dsta |= get_word_ce_prefetch (6);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4160;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4160: ;
return 0;
}
/* MOVE.L (An)+,(xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_23d8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4161;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	m68k_areg(regs, srcreg) += 4;
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (4) << 16;
	dsta |= get_word_ce_prefetch (6);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4161;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4161: ;
return 0;
}
/* MOVE.L -(An),(xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_23e0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = m68k_areg(regs, srcreg) - 4;
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4162;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (4) << 16;
	dsta |= get_word_ce_prefetch (6);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4162;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4162: ;
return 0;
}
/* MOVE.L (d16,An),(xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_23e8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4163;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (6) << 16;
	dsta |= get_word_ce_prefetch (8);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4163;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (8);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4163: ;
return 0;
}
/* MOVE.L (d8,An,Xn),(xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_23f0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4164;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (6) << 16;
	dsta |= get_word_ce_prefetch (8);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4164;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (8);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4164: ;
return 0;
}
/* MOVE.L (xxx).W,(xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_23f8_6)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4165;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (6) << 16;
	dsta |= get_word_ce_prefetch (8);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4165;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (8);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4165: ;
return 0;
}
/* MOVE.L (xxx).L,(xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_23f9_6)(uae_u32 opcode)
{
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4166;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (8) << 16;
	dsta |= get_word_ce_prefetch (10);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel4166;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (10);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4166: ;
return 0;
}
/* MOVE.L (d16,PC),(xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_23fa_6)(uae_u32 opcode)
{
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4167;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (6) << 16;
	dsta |= get_word_ce_prefetch (8);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4167;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (8);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4167: ;
return 0;
}
/* MOVE.L (d8,PC,Xn),(xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_23fb_6)(uae_u32 opcode)
{
{{	uaecptr tmppc;
	uaecptr srca;
	do_cycles (2 * 256);
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4168;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (6) << 16;
	dsta |= get_word_ce_prefetch (8);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4168;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (8);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4168: ;
return 0;
}
/* MOVE.L #<data>.L,(xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_23fc_6)(uae_u32 opcode)
{
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (8) << 16;
	dsta |= get_word_ce_prefetch (10);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel4169;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (10);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}endlabel4169: ;
return 0;
}
/* MOVE.W Dn,Dn */
unsigned long REGPARAM2 CPUFUNC(op_3000_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xffff) | ((src) & 0xffff);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}return 0;
}
/* MOVE.W An,Dn */
unsigned long REGPARAM2 CPUFUNC(op_3008_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_areg(regs, srcreg);
{	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xffff) | ((src) & 0xffff);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}return 0;
}
/* MOVE.W (An),Dn */
unsigned long REGPARAM2 CPUFUNC(op_3010_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4172;
	}
{{	uae_s16 src = get_word_ce (srca);
{	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xffff) | ((src) & 0xffff);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}endlabel4172: ;
return 0;
}
/* MOVE.W (An)+,Dn */
unsigned long REGPARAM2 CPUFUNC(op_3018_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4173;
	}
{{	uae_s16 src = get_word_ce (srca);
	m68k_areg(regs, srcreg) += 2;
{	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xffff) | ((src) & 0xffff);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}endlabel4173: ;
return 0;
}
/* MOVE.W -(An),Dn */
unsigned long REGPARAM2 CPUFUNC(op_3020_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = m68k_areg(regs, srcreg) - 2;
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4174;
	}
{{	uae_s16 src = get_word_ce (srca);
	m68k_areg (regs, srcreg) = srca;
{	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xffff) | ((src) & 0xffff);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}endlabel4174: ;
return 0;
}
/* MOVE.W (d16,An),Dn */
unsigned long REGPARAM2 CPUFUNC(op_3028_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4175;
	}
{{	uae_s16 src = get_word_ce (srca);
{	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xffff) | ((src) & 0xffff);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}endlabel4175: ;
return 0;
}
/* MOVE.W (d8,An,Xn),Dn */
unsigned long REGPARAM2 CPUFUNC(op_3030_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4176;
	}
{{	uae_s16 src = get_word_ce (srca);
{	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xffff) | ((src) & 0xffff);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}endlabel4176: ;
return 0;
}
/* MOVE.W (xxx).W,Dn */
unsigned long REGPARAM2 CPUFUNC(op_3038_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4177;
	}
{{	uae_s16 src = get_word_ce (srca);
{	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xffff) | ((src) & 0xffff);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}endlabel4177: ;
return 0;
}
/* MOVE.W (xxx).L,Dn */
unsigned long REGPARAM2 CPUFUNC(op_3039_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4178;
	}
{{	uae_s16 src = get_word_ce (srca);
{	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xffff) | ((src) & 0xffff);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}endlabel4178: ;
return 0;
}
/* MOVE.W (d16,PC),Dn */
unsigned long REGPARAM2 CPUFUNC(op_303a_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4179;
	}
{{	uae_s16 src = get_word_ce (srca);
{	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xffff) | ((src) & 0xffff);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}endlabel4179: ;
return 0;
}
/* MOVE.W (d8,PC,Xn),Dn */
unsigned long REGPARAM2 CPUFUNC(op_303b_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	do_cycles (2 * 256);
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4180;
	}
{{	uae_s16 src = get_word_ce (srca);
{	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xffff) | ((src) & 0xffff);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}endlabel4180: ;
return 0;
}
/* MOVE.W #<data>.W,Dn */
unsigned long REGPARAM2 CPUFUNC(op_303c_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xffff) | ((src) & 0xffff);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}return 0;
}
/* MOVEA.W Dn,An */
unsigned long REGPARAM2 CPUFUNC(op_3040_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uae_u32 val = (uae_s32)(uae_s16)src;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	m68k_areg(regs, dstreg) = (val);
}}}	m68k_incpc (2);
return 0;
}
/* MOVEA.W An,An */
unsigned long REGPARAM2 CPUFUNC(op_3048_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_areg(regs, srcreg);
{	uae_u32 val = (uae_s32)(uae_s16)src;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	m68k_areg(regs, dstreg) = (val);
}}}	m68k_incpc (2);
return 0;
}
/* MOVEA.W (An),An */
unsigned long REGPARAM2 CPUFUNC(op_3050_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4184;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_u32 val = (uae_s32)(uae_s16)src;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	m68k_areg(regs, dstreg) = (val);
}}}}}	m68k_incpc (2);
endlabel4184: ;
return 0;
}
/* MOVEA.W (An)+,An */
unsigned long REGPARAM2 CPUFUNC(op_3058_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4185;
	}
{{	uae_s16 src = get_word_ce (srca);
	m68k_areg(regs, srcreg) += 2;
{	uae_u32 val = (uae_s32)(uae_s16)src;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	m68k_areg(regs, dstreg) = (val);
}}}}}	m68k_incpc (2);
endlabel4185: ;
return 0;
}
/* MOVEA.W -(An),An */
unsigned long REGPARAM2 CPUFUNC(op_3060_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = m68k_areg(regs, srcreg) - 2;
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4186;
	}
{{	uae_s16 src = get_word_ce (srca);
	m68k_areg (regs, srcreg) = srca;
{	uae_u32 val = (uae_s32)(uae_s16)src;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	m68k_areg(regs, dstreg) = (val);
}}}}}	m68k_incpc (2);
endlabel4186: ;
return 0;
}
/* MOVEA.W (d16,An),An */
unsigned long REGPARAM2 CPUFUNC(op_3068_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4187;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_u32 val = (uae_s32)(uae_s16)src;
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	m68k_areg(regs, dstreg) = (val);
}}}}}	m68k_incpc (4);
endlabel4187: ;
return 0;
}
/* MOVEA.W (d8,An,Xn),An */
unsigned long REGPARAM2 CPUFUNC(op_3070_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4188;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_u32 val = (uae_s32)(uae_s16)src;
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	m68k_areg(regs, dstreg) = (val);
}}}}}	m68k_incpc (4);
endlabel4188: ;
return 0;
}
/* MOVEA.W (xxx).W,An */
unsigned long REGPARAM2 CPUFUNC(op_3078_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4189;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_u32 val = (uae_s32)(uae_s16)src;
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	m68k_areg(regs, dstreg) = (val);
}}}}}	m68k_incpc (4);
endlabel4189: ;
return 0;
}
/* MOVEA.W (xxx).L,An */
unsigned long REGPARAM2 CPUFUNC(op_3079_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4190;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_u32 val = (uae_s32)(uae_s16)src;
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	m68k_areg(regs, dstreg) = (val);
}}}}}	m68k_incpc (6);
endlabel4190: ;
return 0;
}
/* MOVEA.W (d16,PC),An */
unsigned long REGPARAM2 CPUFUNC(op_307a_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4191;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_u32 val = (uae_s32)(uae_s16)src;
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	m68k_areg(regs, dstreg) = (val);
}}}}}	m68k_incpc (4);
endlabel4191: ;
return 0;
}
/* MOVEA.W (d8,PC,Xn),An */
unsigned long REGPARAM2 CPUFUNC(op_307b_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	do_cycles (2 * 256);
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4192;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_u32 val = (uae_s32)(uae_s16)src;
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	m68k_areg(regs, dstreg) = (val);
}}}}}	m68k_incpc (4);
endlabel4192: ;
return 0;
}
/* MOVEA.W #<data>.W,An */
unsigned long REGPARAM2 CPUFUNC(op_307c_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uae_u32 val = (uae_s32)(uae_s16)src;
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	m68k_areg(regs, dstreg) = (val);
}}}	m68k_incpc (4);
return 0;
}
/* MOVE.W Dn,(An) */
unsigned long REGPARAM2 CPUFUNC(op_3080_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4194;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}endlabel4194: ;
return 0;
}
/* MOVE.W An,(An) */
unsigned long REGPARAM2 CPUFUNC(op_3088_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_areg(regs, srcreg);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4195;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}endlabel4195: ;
return 0;
}
/* MOVE.W (An),(An) */
unsigned long REGPARAM2 CPUFUNC(op_3090_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4196;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4196;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4196: ;
return 0;
}
/* MOVE.W (An)+,(An) */
unsigned long REGPARAM2 CPUFUNC(op_3098_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4197;
	}
{{	uae_s16 src = get_word_ce (srca);
	m68k_areg(regs, srcreg) += 2;
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4197;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4197: ;
return 0;
}
/* MOVE.W -(An),(An) */
unsigned long REGPARAM2 CPUFUNC(op_30a0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = m68k_areg(regs, srcreg) - 2;
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4198;
	}
{{	uae_s16 src = get_word_ce (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4198;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4198: ;
return 0;
}
/* MOVE.W (d16,An),(An) */
unsigned long REGPARAM2 CPUFUNC(op_30a8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4199;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4199;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4199: ;
return 0;
}
/* MOVE.W (d8,An,Xn),(An) */
unsigned long REGPARAM2 CPUFUNC(op_30b0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4200;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4200;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4200: ;
return 0;
}
/* MOVE.W (xxx).W,(An) */
unsigned long REGPARAM2 CPUFUNC(op_30b8_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4201;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4201;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4201: ;
return 0;
}
/* MOVE.W (xxx).L,(An) */
unsigned long REGPARAM2 CPUFUNC(op_30b9_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4202;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel4202;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4202: ;
return 0;
}
/* MOVE.W (d16,PC),(An) */
unsigned long REGPARAM2 CPUFUNC(op_30ba_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4203;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4203;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4203: ;
return 0;
}
/* MOVE.W (d8,PC,Xn),(An) */
unsigned long REGPARAM2 CPUFUNC(op_30bb_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	do_cycles (2 * 256);
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4204;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4204;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4204: ;
return 0;
}
/* MOVE.W #<data>.W,(An) */
unsigned long REGPARAM2 CPUFUNC(op_30bc_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4205;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}endlabel4205: ;
return 0;
}
/* MOVE.W Dn,(An)+ */
unsigned long REGPARAM2 CPUFUNC(op_30c0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4206;
	}
{	m68k_areg(regs, dstreg) += 2;
	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}endlabel4206: ;
return 0;
}
/* MOVE.W An,(An)+ */
unsigned long REGPARAM2 CPUFUNC(op_30c8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_areg(regs, srcreg);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4207;
	}
{	m68k_areg(regs, dstreg) += 2;
	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}endlabel4207: ;
return 0;
}
/* MOVE.W (An),(An)+ */
unsigned long REGPARAM2 CPUFUNC(op_30d0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4208;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4208;
	}
{	m68k_areg(regs, dstreg) += 2;
	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4208: ;
return 0;
}
/* MOVE.W (An)+,(An)+ */
unsigned long REGPARAM2 CPUFUNC(op_30d8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4209;
	}
{{	uae_s16 src = get_word_ce (srca);
	m68k_areg(regs, srcreg) += 2;
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4209;
	}
{	m68k_areg(regs, dstreg) += 2;
	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4209: ;
return 0;
}
/* MOVE.W -(An),(An)+ */
unsigned long REGPARAM2 CPUFUNC(op_30e0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = m68k_areg(regs, srcreg) - 2;
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4210;
	}
{{	uae_s16 src = get_word_ce (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4210;
	}
{	m68k_areg(regs, dstreg) += 2;
	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4210: ;
return 0;
}
/* MOVE.W (d16,An),(An)+ */
unsigned long REGPARAM2 CPUFUNC(op_30e8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4211;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4211;
	}
{	m68k_areg(regs, dstreg) += 2;
	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4211: ;
return 0;
}
/* MOVE.W (d8,An,Xn),(An)+ */
unsigned long REGPARAM2 CPUFUNC(op_30f0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4212;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4212;
	}
{	m68k_areg(regs, dstreg) += 2;
	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4212: ;
return 0;
}
/* MOVE.W (xxx).W,(An)+ */
unsigned long REGPARAM2 CPUFUNC(op_30f8_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4213;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4213;
	}
{	m68k_areg(regs, dstreg) += 2;
	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4213: ;
return 0;
}
/* MOVE.W (xxx).L,(An)+ */
unsigned long REGPARAM2 CPUFUNC(op_30f9_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4214;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel4214;
	}
{	m68k_areg(regs, dstreg) += 2;
	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4214: ;
return 0;
}
/* MOVE.W (d16,PC),(An)+ */
unsigned long REGPARAM2 CPUFUNC(op_30fa_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4215;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4215;
	}
{	m68k_areg(regs, dstreg) += 2;
	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4215: ;
return 0;
}
/* MOVE.W (d8,PC,Xn),(An)+ */
unsigned long REGPARAM2 CPUFUNC(op_30fb_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	do_cycles (2 * 256);
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4216;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4216;
	}
{	m68k_areg(regs, dstreg) += 2;
	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4216: ;
return 0;
}
/* MOVE.W #<data>.W,(An)+ */
unsigned long REGPARAM2 CPUFUNC(op_30fc_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4217;
	}
{	m68k_areg(regs, dstreg) += 2;
	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}endlabel4217: ;
return 0;
}
/* MOVE.W Dn,-(An) */
unsigned long REGPARAM2 CPUFUNC(op_3100_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - 2;
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4218;
	}
{	m68k_areg (regs, dstreg) = dsta;
	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}endlabel4218: ;
return 0;
}
/* MOVE.W An,-(An) */
unsigned long REGPARAM2 CPUFUNC(op_3108_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_areg(regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - 2;
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4219;
	}
{	m68k_areg (regs, dstreg) = dsta;
	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}endlabel4219: ;
return 0;
}
/* MOVE.W (An),-(An) */
unsigned long REGPARAM2 CPUFUNC(op_3110_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4220;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - 2;
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4220;
	}
{	m68k_areg (regs, dstreg) = dsta;
	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4220: ;
return 0;
}
/* MOVE.W (An)+,-(An) */
unsigned long REGPARAM2 CPUFUNC(op_3118_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4221;
	}
{{	uae_s16 src = get_word_ce (srca);
	m68k_areg(regs, srcreg) += 2;
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - 2;
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4221;
	}
{	m68k_areg (regs, dstreg) = dsta;
	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4221: ;
return 0;
}
/* MOVE.W -(An),-(An) */
unsigned long REGPARAM2 CPUFUNC(op_3120_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = m68k_areg(regs, srcreg) - 2;
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4222;
	}
{{	uae_s16 src = get_word_ce (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - 2;
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4222;
	}
{	m68k_areg (regs, dstreg) = dsta;
	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4222: ;
return 0;
}
/* MOVE.W (d16,An),-(An) */
unsigned long REGPARAM2 CPUFUNC(op_3128_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4223;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - 2;
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4223;
	}
{	m68k_areg (regs, dstreg) = dsta;
	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4223: ;
return 0;
}
/* MOVE.W (d8,An,Xn),-(An) */
unsigned long REGPARAM2 CPUFUNC(op_3130_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4224;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - 2;
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4224;
	}
{	m68k_areg (regs, dstreg) = dsta;
	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4224: ;
return 0;
}
/* MOVE.W (xxx).W,-(An) */
unsigned long REGPARAM2 CPUFUNC(op_3138_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4225;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - 2;
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4225;
	}
{	m68k_areg (regs, dstreg) = dsta;
	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4225: ;
return 0;
}
/* MOVE.W (xxx).L,-(An) */
unsigned long REGPARAM2 CPUFUNC(op_3139_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4226;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - 2;
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel4226;
	}
{	m68k_areg (regs, dstreg) = dsta;
	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4226: ;
return 0;
}
/* MOVE.W (d16,PC),-(An) */
unsigned long REGPARAM2 CPUFUNC(op_313a_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4227;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - 2;
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4227;
	}
{	m68k_areg (regs, dstreg) = dsta;
	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4227: ;
return 0;
}
/* MOVE.W (d8,PC,Xn),-(An) */
unsigned long REGPARAM2 CPUFUNC(op_313b_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	do_cycles (2 * 256);
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4228;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - 2;
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4228;
	}
{	m68k_areg (regs, dstreg) = dsta;
	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4228: ;
return 0;
}
/* MOVE.W #<data>.W,-(An) */
unsigned long REGPARAM2 CPUFUNC(op_313c_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - 2;
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4229;
	}
{	m68k_areg (regs, dstreg) = dsta;
	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}endlabel4229: ;
return 0;
}
/* MOVE.W Dn,(d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_3140_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4230;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}endlabel4230: ;
return 0;
}
/* MOVE.W An,(d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_3148_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_areg(regs, srcreg);
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4231;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}endlabel4231: ;
return 0;
}
/* MOVE.W (An),(d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_3150_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4232;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4232;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4232: ;
return 0;
}
/* MOVE.W (An)+,(d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_3158_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4233;
	}
{{	uae_s16 src = get_word_ce (srca);
	m68k_areg(regs, srcreg) += 2;
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4233;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4233: ;
return 0;
}
/* MOVE.W -(An),(d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_3160_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = m68k_areg(regs, srcreg) - 2;
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4234;
	}
{{	uae_s16 src = get_word_ce (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4234;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4234: ;
return 0;
}
/* MOVE.W (d16,An),(d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_3168_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4235;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (6);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4235;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4235: ;
return 0;
}
/* MOVE.W (d8,An,Xn),(d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_3170_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4236;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (6);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4236;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4236: ;
return 0;
}
/* MOVE.W (xxx).W,(d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_3178_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4237;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (6);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4237;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4237: ;
return 0;
}
/* MOVE.W (xxx).L,(d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_3179_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4238;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (8);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel4238;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (8);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4238: ;
return 0;
}
/* MOVE.W (d16,PC),(d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_317a_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4239;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (6);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4239;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4239: ;
return 0;
}
/* MOVE.W (d8,PC,Xn),(d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_317b_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	do_cycles (2 * 256);
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4240;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (6);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4240;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4240: ;
return 0;
}
/* MOVE.W #<data>.W,(d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_317c_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (6);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4241;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}endlabel4241: ;
return 0;
}
/* MOVE.W Dn,(d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_3180_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (4));
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4242;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}endlabel4242: ;
return 0;
}
/* MOVE.W An,(d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_3188_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_areg(regs, srcreg);
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (4));
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4243;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}endlabel4243: ;
return 0;
}
/* MOVE.W (An),(d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_3190_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4244;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (4));
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4244;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4244: ;
return 0;
}
/* MOVE.W (An)+,(d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_3198_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4245;
	}
{{	uae_s16 src = get_word_ce (srca);
	m68k_areg(regs, srcreg) += 2;
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (4));
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4245;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4245: ;
return 0;
}
/* MOVE.W -(An),(d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_31a0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = m68k_areg(regs, srcreg) - 2;
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4246;
	}
{{	uae_s16 src = get_word_ce (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (4));
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4246;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4246: ;
return 0;
}
/* MOVE.W (d16,An),(d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_31a8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4247;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (6));
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4247;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4247: ;
return 0;
}
/* MOVE.W (d8,An,Xn),(d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_31b0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4248;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (6));
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4248;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4248: ;
return 0;
}
/* MOVE.W (xxx).W,(d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_31b8_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4249;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (6));
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4249;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4249: ;
return 0;
}
/* MOVE.W (xxx).L,(d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_31b9_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4250;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (8));
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel4250;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (8);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4250: ;
return 0;
}
/* MOVE.W (d16,PC),(d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_31ba_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4251;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (6));
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4251;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4251: ;
return 0;
}
/* MOVE.W (d8,PC,Xn),(d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_31bb_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	do_cycles (2 * 256);
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4252;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (6));
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4252;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4252: ;
return 0;
}
/* MOVE.W #<data>.W,(d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_31bc_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (6));
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4253;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}endlabel4253: ;
return 0;
}
/* MOVE.W Dn,(xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_31c0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4254;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}endlabel4254: ;
return 0;
}
/* MOVE.W An,(xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_31c8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_areg(regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4255;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}endlabel4255: ;
return 0;
}
/* MOVE.W (An),(xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_31d0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4256;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4256;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4256: ;
return 0;
}
/* MOVE.W (An)+,(xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_31d8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4257;
	}
{{	uae_s16 src = get_word_ce (srca);
	m68k_areg(regs, srcreg) += 2;
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4257;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4257: ;
return 0;
}
/* MOVE.W -(An),(xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_31e0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = m68k_areg(regs, srcreg) - 2;
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4258;
	}
{{	uae_s16 src = get_word_ce (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4258;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4258: ;
return 0;
}
/* MOVE.W (d16,An),(xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_31e8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4259;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (6);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4259;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4259: ;
return 0;
}
/* MOVE.W (d8,An,Xn),(xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_31f0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4260;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (6);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4260;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4260: ;
return 0;
}
/* MOVE.W (xxx).W,(xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_31f8_6)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4261;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (6);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4261;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4261: ;
return 0;
}
/* MOVE.W (xxx).L,(xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_31f9_6)(uae_u32 opcode)
{
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4262;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (8);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel4262;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (8);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4262: ;
return 0;
}
/* MOVE.W (d16,PC),(xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_31fa_6)(uae_u32 opcode)
{
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4263;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (6);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4263;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4263: ;
return 0;
}
/* MOVE.W (d8,PC,Xn),(xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_31fb_6)(uae_u32 opcode)
{
{{	uaecptr tmppc;
	uaecptr srca;
	do_cycles (2 * 256);
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4264;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (6);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4264;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4264: ;
return 0;
}
/* MOVE.W #<data>.W,(xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_31fc_6)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (6);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4265;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}endlabel4265: ;
return 0;
}
/* MOVE.W Dn,(xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_33c0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (4) << 16;
	dsta |= get_word_ce_prefetch (6);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4266;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}endlabel4266: ;
return 0;
}
/* MOVE.W An,(xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_33c8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_areg(regs, srcreg);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (4) << 16;
	dsta |= get_word_ce_prefetch (6);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4267;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}endlabel4267: ;
return 0;
}
/* MOVE.W (An),(xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_33d0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4268;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (4) << 16;
	dsta |= get_word_ce_prefetch (6);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4268;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4268: ;
return 0;
}
/* MOVE.W (An)+,(xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_33d8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4269;
	}
{{	uae_s16 src = get_word_ce (srca);
	m68k_areg(regs, srcreg) += 2;
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (4) << 16;
	dsta |= get_word_ce_prefetch (6);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4269;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4269: ;
return 0;
}
/* MOVE.W -(An),(xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_33e0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = m68k_areg(regs, srcreg) - 2;
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4270;
	}
{{	uae_s16 src = get_word_ce (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (4) << 16;
	dsta |= get_word_ce_prefetch (6);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4270;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4270: ;
return 0;
}
/* MOVE.W (d16,An),(xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_33e8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4271;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (6) << 16;
	dsta |= get_word_ce_prefetch (8);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4271;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (8);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4271: ;
return 0;
}
/* MOVE.W (d8,An,Xn),(xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_33f0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4272;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (6) << 16;
	dsta |= get_word_ce_prefetch (8);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4272;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (8);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4272: ;
return 0;
}
/* MOVE.W (xxx).W,(xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_33f8_6)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4273;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (6) << 16;
	dsta |= get_word_ce_prefetch (8);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4273;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (8);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4273: ;
return 0;
}
/* MOVE.W (xxx).L,(xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_33f9_6)(uae_u32 opcode)
{
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4274;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (8) << 16;
	dsta |= get_word_ce_prefetch (10);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel4274;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (10);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4274: ;
return 0;
}
/* MOVE.W (d16,PC),(xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_33fa_6)(uae_u32 opcode)
{
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4275;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (6) << 16;
	dsta |= get_word_ce_prefetch (8);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4275;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (8);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4275: ;
return 0;
}
/* MOVE.W (d8,PC,Xn),(xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_33fb_6)(uae_u32 opcode)
{
{{	uaecptr tmppc;
	uaecptr srca;
	do_cycles (2 * 256);
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4276;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (6) << 16;
	dsta |= get_word_ce_prefetch (8);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4276;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (8);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4276: ;
return 0;
}
/* MOVE.W #<data>.W,(xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_33fc_6)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (6) << 16;
	dsta |= get_word_ce_prefetch (8);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4277;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (8);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}endlabel4277: ;
return 0;
}
/* NEGX.B Dn */
unsigned long REGPARAM2 CPUFUNC(op_4000_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s8 src = m68k_dreg(regs, srcreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 newv = 0 - src - (GET_XFLG ? 1 : 0);
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(0)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_VFLG ((flgs ^ flgo) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
	COPY_CARRY;
	SET_ZFLG (GET_ZFLG & (((uae_s8)(newv)) == 0));
	SET_NFLG (((uae_s8)(newv)) < 0);
	m68k_dreg(regs, srcreg) = (m68k_dreg(regs, srcreg) & ~0xff) | ((newv) & 0xff);
}}}}	m68k_incpc (2);
return 0;
}
/* NEGX.B (An) */
unsigned long REGPARAM2 CPUFUNC(op_4010_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
{	uae_s8 src = get_byte_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 newv = 0 - src - (GET_XFLG ? 1 : 0);
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(0)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_VFLG ((flgs ^ flgo) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
	COPY_CARRY;
	SET_ZFLG (GET_ZFLG & (((uae_s8)(newv)) == 0));
	SET_NFLG (((uae_s8)(newv)) < 0);
	put_byte_ce (srca,newv);
}}}}}	m68k_incpc (2);
return 0;
}
/* NEGX.B (An)+ */
unsigned long REGPARAM2 CPUFUNC(op_4018_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
{	uae_s8 src = get_byte_ce (srca);
	m68k_areg(regs, srcreg) += areg_byteinc[srcreg];
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 newv = 0 - src - (GET_XFLG ? 1 : 0);
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(0)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_VFLG ((flgs ^ flgo) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
	COPY_CARRY;
	SET_ZFLG (GET_ZFLG & (((uae_s8)(newv)) == 0));
	SET_NFLG (((uae_s8)(newv)) < 0);
	put_byte_ce (srca,newv);
}}}}}	m68k_incpc (2);
return 0;
}
/* NEGX.B -(An) */
unsigned long REGPARAM2 CPUFUNC(op_4020_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = get_byte_ce (srca);
	m68k_areg (regs, srcreg) = srca;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 newv = 0 - src - (GET_XFLG ? 1 : 0);
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(0)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_VFLG ((flgs ^ flgo) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
	COPY_CARRY;
	SET_ZFLG (GET_ZFLG & (((uae_s8)(newv)) == 0));
	SET_NFLG (((uae_s8)(newv)) < 0);
	put_byte_ce (srca,newv);
}}}}}	m68k_incpc (2);
return 0;
}
/* NEGX.B (d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_4028_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 src = get_byte_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	uae_u32 newv = 0 - src - (GET_XFLG ? 1 : 0);
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(0)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_VFLG ((flgs ^ flgo) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
	COPY_CARRY;
	SET_ZFLG (GET_ZFLG & (((uae_s8)(newv)) == 0));
	SET_NFLG (((uae_s8)(newv)) < 0);
	put_byte_ce (srca,newv);
}}}}}	m68k_incpc (4);
return 0;
}
/* NEGX.B (d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_4030_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
{	uae_s8 src = get_byte_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	uae_u32 newv = 0 - src - (GET_XFLG ? 1 : 0);
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(0)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_VFLG ((flgs ^ flgo) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
	COPY_CARRY;
	SET_ZFLG (GET_ZFLG & (((uae_s8)(newv)) == 0));
	SET_NFLG (((uae_s8)(newv)) < 0);
	put_byte_ce (srca,newv);
}}}}}	m68k_incpc (4);
return 0;
}
/* NEGX.B (xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_4038_6)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 src = get_byte_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	uae_u32 newv = 0 - src - (GET_XFLG ? 1 : 0);
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(0)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_VFLG ((flgs ^ flgo) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
	COPY_CARRY;
	SET_ZFLG (GET_ZFLG & (((uae_s8)(newv)) == 0));
	SET_NFLG (((uae_s8)(newv)) < 0);
	put_byte_ce (srca,newv);
}}}}}	m68k_incpc (4);
return 0;
}
/* NEGX.B (xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_4039_6)(uae_u32 opcode)
{
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
{	uae_s8 src = get_byte_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
{	uae_u32 newv = 0 - src - (GET_XFLG ? 1 : 0);
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(0)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_VFLG ((flgs ^ flgo) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
	COPY_CARRY;
	SET_ZFLG (GET_ZFLG & (((uae_s8)(newv)) == 0));
	SET_NFLG (((uae_s8)(newv)) < 0);
	put_byte_ce (srca,newv);
}}}}}	m68k_incpc (6);
return 0;
}
/* NEGX.W Dn */
unsigned long REGPARAM2 CPUFUNC(op_4040_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_dreg(regs, srcreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 newv = 0 - src - (GET_XFLG ? 1 : 0);
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(0)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_VFLG ((flgs ^ flgo) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
	COPY_CARRY;
	SET_ZFLG (GET_ZFLG & (((uae_s16)(newv)) == 0));
	SET_NFLG (((uae_s16)(newv)) < 0);
	m68k_dreg(regs, srcreg) = (m68k_dreg(regs, srcreg) & ~0xffff) | ((newv) & 0xffff);
}}}}	m68k_incpc (2);
return 0;
}
/* NEGX.W (An) */
unsigned long REGPARAM2 CPUFUNC(op_4050_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4287;
	}
{{	uae_s16 src = get_word_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 newv = 0 - src - (GET_XFLG ? 1 : 0);
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(0)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_VFLG ((flgs ^ flgo) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
	COPY_CARRY;
	SET_ZFLG (GET_ZFLG & (((uae_s16)(newv)) == 0));
	SET_NFLG (((uae_s16)(newv)) < 0);
	put_word_ce (srca,newv);
}}}}}}	m68k_incpc (2);
endlabel4287: ;
return 0;
}
/* NEGX.W (An)+ */
unsigned long REGPARAM2 CPUFUNC(op_4058_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4288;
	}
{{	uae_s16 src = get_word_ce (srca);
	m68k_areg(regs, srcreg) += 2;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 newv = 0 - src - (GET_XFLG ? 1 : 0);
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(0)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_VFLG ((flgs ^ flgo) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
	COPY_CARRY;
	SET_ZFLG (GET_ZFLG & (((uae_s16)(newv)) == 0));
	SET_NFLG (((uae_s16)(newv)) < 0);
	put_word_ce (srca,newv);
}}}}}}	m68k_incpc (2);
endlabel4288: ;
return 0;
}
/* NEGX.W -(An) */
unsigned long REGPARAM2 CPUFUNC(op_4060_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = m68k_areg(regs, srcreg) - 2;
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4289;
	}
{{	uae_s16 src = get_word_ce (srca);
	m68k_areg (regs, srcreg) = srca;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 newv = 0 - src - (GET_XFLG ? 1 : 0);
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(0)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_VFLG ((flgs ^ flgo) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
	COPY_CARRY;
	SET_ZFLG (GET_ZFLG & (((uae_s16)(newv)) == 0));
	SET_NFLG (((uae_s16)(newv)) < 0);
	put_word_ce (srca,newv);
}}}}}}	m68k_incpc (2);
endlabel4289: ;
return 0;
}
/* NEGX.W (d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_4068_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4290;
	}
{{	uae_s16 src = get_word_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	uae_u32 newv = 0 - src - (GET_XFLG ? 1 : 0);
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(0)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_VFLG ((flgs ^ flgo) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
	COPY_CARRY;
	SET_ZFLG (GET_ZFLG & (((uae_s16)(newv)) == 0));
	SET_NFLG (((uae_s16)(newv)) < 0);
	put_word_ce (srca,newv);
}}}}}}	m68k_incpc (4);
endlabel4290: ;
return 0;
}
/* NEGX.W (d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_4070_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4291;
	}
{{	uae_s16 src = get_word_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	uae_u32 newv = 0 - src - (GET_XFLG ? 1 : 0);
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(0)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_VFLG ((flgs ^ flgo) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
	COPY_CARRY;
	SET_ZFLG (GET_ZFLG & (((uae_s16)(newv)) == 0));
	SET_NFLG (((uae_s16)(newv)) < 0);
	put_word_ce (srca,newv);
}}}}}}	m68k_incpc (4);
endlabel4291: ;
return 0;
}
/* NEGX.W (xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_4078_6)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4292;
	}
{{	uae_s16 src = get_word_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	uae_u32 newv = 0 - src - (GET_XFLG ? 1 : 0);
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(0)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_VFLG ((flgs ^ flgo) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
	COPY_CARRY;
	SET_ZFLG (GET_ZFLG & (((uae_s16)(newv)) == 0));
	SET_NFLG (((uae_s16)(newv)) < 0);
	put_word_ce (srca,newv);
}}}}}}	m68k_incpc (4);
endlabel4292: ;
return 0;
}
/* NEGX.W (xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_4079_6)(uae_u32 opcode)
{
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4293;
	}
{{	uae_s16 src = get_word_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
{	uae_u32 newv = 0 - src - (GET_XFLG ? 1 : 0);
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(0)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_VFLG ((flgs ^ flgo) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
	COPY_CARRY;
	SET_ZFLG (GET_ZFLG & (((uae_s16)(newv)) == 0));
	SET_NFLG (((uae_s16)(newv)) < 0);
	put_word_ce (srca,newv);
}}}}}}	m68k_incpc (6);
endlabel4293: ;
return 0;
}
/* NEGX.L Dn */
unsigned long REGPARAM2 CPUFUNC(op_4080_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s32 src = m68k_dreg(regs, srcreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 newv = 0 - src - (GET_XFLG ? 1 : 0);
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(0)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_VFLG ((flgs ^ flgo) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
	COPY_CARRY;
	SET_ZFLG (GET_ZFLG & (((uae_s32)(newv)) == 0));
	SET_NFLG (((uae_s32)(newv)) < 0);
	do_cycles (2 * 256);
	m68k_dreg(regs, srcreg) = (newv);
}}}}	m68k_incpc (2);
return 0;
}
/* NEGX.L (An) */
unsigned long REGPARAM2 CPUFUNC(op_4090_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4295;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 newv = 0 - src - (GET_XFLG ? 1 : 0);
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(0)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_VFLG ((flgs ^ flgo) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
	COPY_CARRY;
	SET_ZFLG (GET_ZFLG & (((uae_s32)(newv)) == 0));
	SET_NFLG (((uae_s32)(newv)) < 0);
	put_word_ce (srca, newv >> 16); put_word_ce (srca + 2, newv);
}}}}}}	m68k_incpc (2);
endlabel4295: ;
return 0;
}
/* NEGX.L (An)+ */
unsigned long REGPARAM2 CPUFUNC(op_4098_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4296;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	m68k_areg(regs, srcreg) += 4;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 newv = 0 - src - (GET_XFLG ? 1 : 0);
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(0)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_VFLG ((flgs ^ flgo) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
	COPY_CARRY;
	SET_ZFLG (GET_ZFLG & (((uae_s32)(newv)) == 0));
	SET_NFLG (((uae_s32)(newv)) < 0);
	put_word_ce (srca, newv >> 16); put_word_ce (srca + 2, newv);
}}}}}}	m68k_incpc (2);
endlabel4296: ;
return 0;
}
/* NEGX.L -(An) */
unsigned long REGPARAM2 CPUFUNC(op_40a0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = m68k_areg(regs, srcreg) - 4;
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4297;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	m68k_areg (regs, srcreg) = srca;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 newv = 0 - src - (GET_XFLG ? 1 : 0);
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(0)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_VFLG ((flgs ^ flgo) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
	COPY_CARRY;
	SET_ZFLG (GET_ZFLG & (((uae_s32)(newv)) == 0));
	SET_NFLG (((uae_s32)(newv)) < 0);
	put_word_ce (srca, newv >> 16); put_word_ce (srca + 2, newv);
}}}}}}	m68k_incpc (2);
endlabel4297: ;
return 0;
}
/* NEGX.L (d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_40a8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4298;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	uae_u32 newv = 0 - src - (GET_XFLG ? 1 : 0);
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(0)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_VFLG ((flgs ^ flgo) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
	COPY_CARRY;
	SET_ZFLG (GET_ZFLG & (((uae_s32)(newv)) == 0));
	SET_NFLG (((uae_s32)(newv)) < 0);
	put_word_ce (srca, newv >> 16); put_word_ce (srca + 2, newv);
}}}}}}	m68k_incpc (4);
endlabel4298: ;
return 0;
}
/* NEGX.L (d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_40b0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4299;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	uae_u32 newv = 0 - src - (GET_XFLG ? 1 : 0);
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(0)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_VFLG ((flgs ^ flgo) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
	COPY_CARRY;
	SET_ZFLG (GET_ZFLG & (((uae_s32)(newv)) == 0));
	SET_NFLG (((uae_s32)(newv)) < 0);
	put_word_ce (srca, newv >> 16); put_word_ce (srca + 2, newv);
}}}}}}	m68k_incpc (4);
endlabel4299: ;
return 0;
}
/* NEGX.L (xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_40b8_6)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4300;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	uae_u32 newv = 0 - src - (GET_XFLG ? 1 : 0);
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(0)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_VFLG ((flgs ^ flgo) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
	COPY_CARRY;
	SET_ZFLG (GET_ZFLG & (((uae_s32)(newv)) == 0));
	SET_NFLG (((uae_s32)(newv)) < 0);
	put_word_ce (srca, newv >> 16); put_word_ce (srca + 2, newv);
}}}}}}	m68k_incpc (4);
endlabel4300: ;
return 0;
}
/* NEGX.L (xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_40b9_6)(uae_u32 opcode)
{
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4301;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
{	uae_u32 newv = 0 - src - (GET_XFLG ? 1 : 0);
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(0)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_VFLG ((flgs ^ flgo) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
	COPY_CARRY;
	SET_ZFLG (GET_ZFLG & (((uae_s32)(newv)) == 0));
	SET_NFLG (((uae_s32)(newv)) < 0);
	put_word_ce (srca, newv >> 16); put_word_ce (srca + 2, newv);
}}}}}}	m68k_incpc (6);
endlabel4301: ;
return 0;
}
/* MVSR2.W Dn */
unsigned long REGPARAM2 CPUFUNC(op_40c0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	do_cycles (2 * 256);
	MakeSR();
	m68k_dreg(regs, srcreg) = (m68k_dreg(regs, srcreg) & ~0xffff) | ((regs.sr) & 0xffff);
}}	m68k_incpc (2);
return 0;
}
/* MVSR2.W (An) */
unsigned long REGPARAM2 CPUFUNC(op_40d0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4303;
	}
{	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	MakeSR();
	put_word_ce (srca,regs.sr);
}}}	m68k_incpc (2);
endlabel4303: ;
return 0;
}
/* MVSR2.W (An)+ */
unsigned long REGPARAM2 CPUFUNC(op_40d8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4304;
	}
{	m68k_areg(regs, srcreg) += 2;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	MakeSR();
	put_word_ce (srca,regs.sr);
}}}	m68k_incpc (2);
endlabel4304: ;
return 0;
}
/* MVSR2.W -(An) */
unsigned long REGPARAM2 CPUFUNC(op_40e0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = m68k_areg(regs, srcreg) - 2;
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4305;
	}
{	m68k_areg (regs, srcreg) = srca;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	MakeSR();
	put_word_ce (srca,regs.sr);
}}}	m68k_incpc (2);
endlabel4305: ;
return 0;
}
/* MVSR2.W (d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_40e8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4306;
	}
{	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	MakeSR();
	put_word_ce (srca,regs.sr);
}}}	m68k_incpc (4);
endlabel4306: ;
return 0;
}
/* MVSR2.W (d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_40f0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4307;
	}
{	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	MakeSR();
	put_word_ce (srca,regs.sr);
}}}	m68k_incpc (4);
endlabel4307: ;
return 0;
}
/* MVSR2.W (xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_40f8_6)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4308;
	}
{	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	MakeSR();
	put_word_ce (srca,regs.sr);
}}}	m68k_incpc (4);
endlabel4308: ;
return 0;
}
/* MVSR2.W (xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_40f9_6)(uae_u32 opcode)
{
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4309;
	}
{	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	MakeSR();
	put_word_ce (srca,regs.sr);
}}}	m68k_incpc (6);
endlabel4309: ;
return 0;
}
/* CHK.L Dn,Dn */
unsigned long REGPARAM2 CPUFUNC(op_4100_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{	uaecptr oldpc = m68k_getpc();
{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	do_cycles (6 * 256);
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6, oldpc);
		goto endlabel4310;
	} else if (dst > src) {
		SET_NFLG (0);
		Exception (6, oldpc);
		goto endlabel4310;
	}
}}}endlabel4310: ;
return 0;
}
/* CHK.L (An),Dn */
unsigned long REGPARAM2 CPUFUNC(op_4110_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{	uaecptr oldpc = m68k_getpc();
{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4311;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	do_cycles (6 * 256);
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6, oldpc);
		goto endlabel4311;
	} else if (dst > src) {
		SET_NFLG (0);
		Exception (6, oldpc);
		goto endlabel4311;
	}
}}}}}endlabel4311: ;
return 0;
}
/* CHK.L (An)+,Dn */
unsigned long REGPARAM2 CPUFUNC(op_4118_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{	uaecptr oldpc = m68k_getpc();
{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4312;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	m68k_areg(regs, srcreg) += 4;
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	do_cycles (6 * 256);
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6, oldpc);
		goto endlabel4312;
	} else if (dst > src) {
		SET_NFLG (0);
		Exception (6, oldpc);
		goto endlabel4312;
	}
}}}}}endlabel4312: ;
return 0;
}
/* CHK.L -(An),Dn */
unsigned long REGPARAM2 CPUFUNC(op_4120_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{	uaecptr oldpc = m68k_getpc();
{	uaecptr srca;
	do_cycles (2 * 256);
	srca = m68k_areg(regs, srcreg) - 4;
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4313;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	m68k_areg (regs, srcreg) = srca;
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	do_cycles (6 * 256);
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6, oldpc);
		goto endlabel4313;
	} else if (dst > src) {
		SET_NFLG (0);
		Exception (6, oldpc);
		goto endlabel4313;
	}
}}}}}endlabel4313: ;
return 0;
}
/* CHK.L (d16,An),Dn */
unsigned long REGPARAM2 CPUFUNC(op_4128_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{	uaecptr oldpc = m68k_getpc();
{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4314;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	do_cycles (6 * 256);
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6, oldpc);
		goto endlabel4314;
	} else if (dst > src) {
		SET_NFLG (0);
		Exception (6, oldpc);
		goto endlabel4314;
	}
}}}}}endlabel4314: ;
return 0;
}
/* CHK.L (d8,An,Xn),Dn */
unsigned long REGPARAM2 CPUFUNC(op_4130_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{	uaecptr oldpc = m68k_getpc();
{	uaecptr srca;
	do_cycles (2 * 256);
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4315;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	do_cycles (6 * 256);
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6, oldpc);
		goto endlabel4315;
	} else if (dst > src) {
		SET_NFLG (0);
		Exception (6, oldpc);
		goto endlabel4315;
	}
}}}}}endlabel4315: ;
return 0;
}
/* CHK.L (xxx).W,Dn */
unsigned long REGPARAM2 CPUFUNC(op_4138_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{	uaecptr oldpc = m68k_getpc();
{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4316;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	do_cycles (6 * 256);
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6, oldpc);
		goto endlabel4316;
	} else if (dst > src) {
		SET_NFLG (0);
		Exception (6, oldpc);
		goto endlabel4316;
	}
}}}}}endlabel4316: ;
return 0;
}
/* CHK.L (xxx).L,Dn */
unsigned long REGPARAM2 CPUFUNC(op_4139_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{	uaecptr oldpc = m68k_getpc();
{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4317;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	do_cycles (6 * 256);
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6, oldpc);
		goto endlabel4317;
	} else if (dst > src) {
		SET_NFLG (0);
		Exception (6, oldpc);
		goto endlabel4317;
	}
}}}}}endlabel4317: ;
return 0;
}
/* CHK.L (d16,PC),Dn */
unsigned long REGPARAM2 CPUFUNC(op_413a_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{	uaecptr oldpc = m68k_getpc();
{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4318;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	do_cycles (6 * 256);
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6, oldpc);
		goto endlabel4318;
	} else if (dst > src) {
		SET_NFLG (0);
		Exception (6, oldpc);
		goto endlabel4318;
	}
}}}}}endlabel4318: ;
return 0;
}
/* CHK.L (d8,PC,Xn),Dn */
unsigned long REGPARAM2 CPUFUNC(op_413b_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{	uaecptr oldpc = m68k_getpc();
{	uaecptr tmppc;
	uaecptr srca;
	do_cycles (2 * 256);
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4319;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	do_cycles (6 * 256);
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6, oldpc);
		goto endlabel4319;
	} else if (dst > src) {
		SET_NFLG (0);
		Exception (6, oldpc);
		goto endlabel4319;
	}
}}}}}endlabel4319: ;
return 0;
}
/* CHK.L #<data>.L,Dn */
unsigned long REGPARAM2 CPUFUNC(op_413c_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{	uaecptr oldpc = m68k_getpc();
{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	do_cycles (6 * 256);
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6, oldpc);
		goto endlabel4320;
	} else if (dst > src) {
		SET_NFLG (0);
		Exception (6, oldpc);
		goto endlabel4320;
	}
}}}endlabel4320: ;
return 0;
}
/* CHK.W Dn,Dn */
unsigned long REGPARAM2 CPUFUNC(op_4180_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{	uaecptr oldpc = m68k_getpc();
{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	do_cycles (6 * 256);
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6, oldpc);
		goto endlabel4321;
	} else if (dst > src) {
		SET_NFLG (0);
		Exception (6, oldpc);
		goto endlabel4321;
	}
}}}endlabel4321: ;
return 0;
}
/* CHK.W (An),Dn */
unsigned long REGPARAM2 CPUFUNC(op_4190_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{	uaecptr oldpc = m68k_getpc();
{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4322;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	do_cycles (6 * 256);
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6, oldpc);
		goto endlabel4322;
	} else if (dst > src) {
		SET_NFLG (0);
		Exception (6, oldpc);
		goto endlabel4322;
	}
}}}}}endlabel4322: ;
return 0;
}
/* CHK.W (An)+,Dn */
unsigned long REGPARAM2 CPUFUNC(op_4198_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{	uaecptr oldpc = m68k_getpc();
{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4323;
	}
{{	uae_s16 src = get_word_ce (srca);
	m68k_areg(regs, srcreg) += 2;
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	do_cycles (6 * 256);
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6, oldpc);
		goto endlabel4323;
	} else if (dst > src) {
		SET_NFLG (0);
		Exception (6, oldpc);
		goto endlabel4323;
	}
}}}}}endlabel4323: ;
return 0;
}
/* CHK.W -(An),Dn */
unsigned long REGPARAM2 CPUFUNC(op_41a0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{	uaecptr oldpc = m68k_getpc();
{	uaecptr srca;
	do_cycles (2 * 256);
	srca = m68k_areg(regs, srcreg) - 2;
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4324;
	}
{{	uae_s16 src = get_word_ce (srca);
	m68k_areg (regs, srcreg) = srca;
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	do_cycles (6 * 256);
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6, oldpc);
		goto endlabel4324;
	} else if (dst > src) {
		SET_NFLG (0);
		Exception (6, oldpc);
		goto endlabel4324;
	}
}}}}}endlabel4324: ;
return 0;
}
/* CHK.W (d16,An),Dn */
unsigned long REGPARAM2 CPUFUNC(op_41a8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{	uaecptr oldpc = m68k_getpc();
{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4325;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	do_cycles (6 * 256);
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6, oldpc);
		goto endlabel4325;
	} else if (dst > src) {
		SET_NFLG (0);
		Exception (6, oldpc);
		goto endlabel4325;
	}
}}}}}endlabel4325: ;
return 0;
}
/* CHK.W (d8,An,Xn),Dn */
unsigned long REGPARAM2 CPUFUNC(op_41b0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{	uaecptr oldpc = m68k_getpc();
{	uaecptr srca;
	do_cycles (2 * 256);
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4326;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	do_cycles (6 * 256);
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6, oldpc);
		goto endlabel4326;
	} else if (dst > src) {
		SET_NFLG (0);
		Exception (6, oldpc);
		goto endlabel4326;
	}
}}}}}endlabel4326: ;
return 0;
}
/* CHK.W (xxx).W,Dn */
unsigned long REGPARAM2 CPUFUNC(op_41b8_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{	uaecptr oldpc = m68k_getpc();
{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4327;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	do_cycles (6 * 256);
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6, oldpc);
		goto endlabel4327;
	} else if (dst > src) {
		SET_NFLG (0);
		Exception (6, oldpc);
		goto endlabel4327;
	}
}}}}}endlabel4327: ;
return 0;
}
/* CHK.W (xxx).L,Dn */
unsigned long REGPARAM2 CPUFUNC(op_41b9_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{	uaecptr oldpc = m68k_getpc();
{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4328;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	do_cycles (6 * 256);
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6, oldpc);
		goto endlabel4328;
	} else if (dst > src) {
		SET_NFLG (0);
		Exception (6, oldpc);
		goto endlabel4328;
	}
}}}}}endlabel4328: ;
return 0;
}
/* CHK.W (d16,PC),Dn */
unsigned long REGPARAM2 CPUFUNC(op_41ba_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{	uaecptr oldpc = m68k_getpc();
{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4329;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	do_cycles (6 * 256);
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6, oldpc);
		goto endlabel4329;
	} else if (dst > src) {
		SET_NFLG (0);
		Exception (6, oldpc);
		goto endlabel4329;
	}
}}}}}endlabel4329: ;
return 0;
}
/* CHK.W (d8,PC,Xn),Dn */
unsigned long REGPARAM2 CPUFUNC(op_41bb_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{	uaecptr oldpc = m68k_getpc();
{	uaecptr tmppc;
	uaecptr srca;
	do_cycles (2 * 256);
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4330;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	do_cycles (6 * 256);
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6, oldpc);
		goto endlabel4330;
	} else if (dst > src) {
		SET_NFLG (0);
		Exception (6, oldpc);
		goto endlabel4330;
	}
}}}}}endlabel4330: ;
return 0;
}
/* CHK.W #<data>.W,Dn */
unsigned long REGPARAM2 CPUFUNC(op_41bc_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{	uaecptr oldpc = m68k_getpc();
{	uae_s16 src = get_word_ce_prefetch (4);
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	do_cycles (6 * 256);
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6, oldpc);
		goto endlabel4331;
	} else if (dst > src) {
		SET_NFLG (0);
		Exception (6, oldpc);
		goto endlabel4331;
	}
}}}endlabel4331: ;
return 0;
}
/* LEA.L (An),An */
unsigned long REGPARAM2 CPUFUNC(op_41d0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
{	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	m68k_areg(regs, dstreg) = (srca);
}}}	m68k_incpc (2);
return 0;
}
/* LEA.L (d16,An),An */
unsigned long REGPARAM2 CPUFUNC(op_41e8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	m68k_areg(regs, dstreg) = (srca);
}}}	m68k_incpc (4);
return 0;
}
/* LEA.L (d8,An,Xn),An */
unsigned long REGPARAM2 CPUFUNC(op_41f0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
{	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	do_cycles (4 * 256);
	m68k_areg(regs, dstreg) = (srca);
}}}	m68k_incpc (4);
return 0;
}
/* LEA.L (xxx).W,An */
unsigned long REGPARAM2 CPUFUNC(op_41f8_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	m68k_areg(regs, dstreg) = (srca);
}}}	m68k_incpc (4);
return 0;
}
/* LEA.L (xxx).L,An */
unsigned long REGPARAM2 CPUFUNC(op_41f9_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
{	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	m68k_areg(regs, dstreg) = (srca);
}}}	m68k_incpc (6);
return 0;
}
/* LEA.L (d16,PC),An */
unsigned long REGPARAM2 CPUFUNC(op_41fa_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	m68k_areg(regs, dstreg) = (srca);
}}}	m68k_incpc (4);
return 0;
}
/* LEA.L (d8,PC,Xn),An */
unsigned long REGPARAM2 CPUFUNC(op_41fb_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
{	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	do_cycles (4 * 256);
	m68k_areg(regs, dstreg) = (srca);
}}}	m68k_incpc (4);
return 0;
}
/* CLR.B Dn */
unsigned long REGPARAM2 CPUFUNC(op_4200_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s8 src = m68k_dreg(regs, srcreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(0)) == 0);
	SET_NFLG (((uae_s8)(0)) < 0);
	m68k_dreg(regs, srcreg) = (m68k_dreg(regs, srcreg) & ~0xff) | ((0) & 0xff);
}}	m68k_incpc (2);
return 0;
}
/* CLR.B (An) */
unsigned long REGPARAM2 CPUFUNC(op_4210_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
{	uae_s8 src = get_byte_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(0)) == 0);
	SET_NFLG (((uae_s8)(0)) < 0);
	put_byte_ce (srca,0);
}}}	m68k_incpc (2);
return 0;
}
/* CLR.B (An)+ */
unsigned long REGPARAM2 CPUFUNC(op_4218_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
{	uae_s8 src = get_byte_ce (srca);
	m68k_areg(regs, srcreg) += areg_byteinc[srcreg];
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(0)) == 0);
	SET_NFLG (((uae_s8)(0)) < 0);
	put_byte_ce (srca,0);
}}}	m68k_incpc (2);
return 0;
}
/* CLR.B -(An) */
unsigned long REGPARAM2 CPUFUNC(op_4220_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = get_byte_ce (srca);
	m68k_areg (regs, srcreg) = srca;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(0)) == 0);
	SET_NFLG (((uae_s8)(0)) < 0);
	put_byte_ce (srca,0);
}}}	m68k_incpc (2);
return 0;
}
/* CLR.B (d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_4228_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 src = get_byte_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(0)) == 0);
	SET_NFLG (((uae_s8)(0)) < 0);
	put_byte_ce (srca,0);
}}}	m68k_incpc (4);
return 0;
}
/* CLR.B (d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_4230_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
{	uae_s8 src = get_byte_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(0)) == 0);
	SET_NFLG (((uae_s8)(0)) < 0);
	put_byte_ce (srca,0);
}}}	m68k_incpc (4);
return 0;
}
/* CLR.B (xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_4238_6)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 src = get_byte_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(0)) == 0);
	SET_NFLG (((uae_s8)(0)) < 0);
	put_byte_ce (srca,0);
}}}	m68k_incpc (4);
return 0;
}
/* CLR.B (xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_4239_6)(uae_u32 opcode)
{
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
{	uae_s8 src = get_byte_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(0)) == 0);
	SET_NFLG (((uae_s8)(0)) < 0);
	put_byte_ce (srca,0);
}}}	m68k_incpc (6);
return 0;
}
/* CLR.W Dn */
unsigned long REGPARAM2 CPUFUNC(op_4240_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_dreg(regs, srcreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(0)) == 0);
	SET_NFLG (((uae_s16)(0)) < 0);
	m68k_dreg(regs, srcreg) = (m68k_dreg(regs, srcreg) & ~0xffff) | ((0) & 0xffff);
}}	m68k_incpc (2);
return 0;
}
/* CLR.W (An) */
unsigned long REGPARAM2 CPUFUNC(op_4250_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4348;
	}
{{	uae_s16 src = get_word_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(0)) == 0);
	SET_NFLG (((uae_s16)(0)) < 0);
	put_word_ce (srca,0);
}}}}	m68k_incpc (2);
endlabel4348: ;
return 0;
}
/* CLR.W (An)+ */
unsigned long REGPARAM2 CPUFUNC(op_4258_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4349;
	}
{{	uae_s16 src = get_word_ce (srca);
	m68k_areg(regs, srcreg) += 2;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(0)) == 0);
	SET_NFLG (((uae_s16)(0)) < 0);
	put_word_ce (srca,0);
}}}}	m68k_incpc (2);
endlabel4349: ;
return 0;
}
/* CLR.W -(An) */
unsigned long REGPARAM2 CPUFUNC(op_4260_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = m68k_areg(regs, srcreg) - 2;
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4350;
	}
{{	uae_s16 src = get_word_ce (srca);
	m68k_areg (regs, srcreg) = srca;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(0)) == 0);
	SET_NFLG (((uae_s16)(0)) < 0);
	put_word_ce (srca,0);
}}}}	m68k_incpc (2);
endlabel4350: ;
return 0;
}
/* CLR.W (d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_4268_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4351;
	}
{{	uae_s16 src = get_word_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(0)) == 0);
	SET_NFLG (((uae_s16)(0)) < 0);
	put_word_ce (srca,0);
}}}}	m68k_incpc (4);
endlabel4351: ;
return 0;
}
/* CLR.W (d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_4270_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4352;
	}
{{	uae_s16 src = get_word_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(0)) == 0);
	SET_NFLG (((uae_s16)(0)) < 0);
	put_word_ce (srca,0);
}}}}	m68k_incpc (4);
endlabel4352: ;
return 0;
}
/* CLR.W (xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_4278_6)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4353;
	}
{{	uae_s16 src = get_word_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(0)) == 0);
	SET_NFLG (((uae_s16)(0)) < 0);
	put_word_ce (srca,0);
}}}}	m68k_incpc (4);
endlabel4353: ;
return 0;
}
/* CLR.W (xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_4279_6)(uae_u32 opcode)
{
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4354;
	}
{{	uae_s16 src = get_word_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(0)) == 0);
	SET_NFLG (((uae_s16)(0)) < 0);
	put_word_ce (srca,0);
}}}}	m68k_incpc (6);
endlabel4354: ;
return 0;
}
/* CLR.L Dn */
unsigned long REGPARAM2 CPUFUNC(op_4280_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s32 src = m68k_dreg(regs, srcreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(0)) == 0);
	SET_NFLG (((uae_s32)(0)) < 0);
	do_cycles (2 * 256);
	m68k_dreg(regs, srcreg) = (0);
}}	m68k_incpc (2);
return 0;
}
/* CLR.L (An) */
unsigned long REGPARAM2 CPUFUNC(op_4290_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4356;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(0)) == 0);
	SET_NFLG (((uae_s32)(0)) < 0);
	put_word_ce (srca, 0 >> 16); put_word_ce (srca + 2, 0);
}}}}	m68k_incpc (2);
endlabel4356: ;
return 0;
}
#endif

#ifdef PART_4
/* CLR.L (An)+ */
unsigned long REGPARAM2 CPUFUNC(op_4298_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4357;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	m68k_areg(regs, srcreg) += 4;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(0)) == 0);
	SET_NFLG (((uae_s32)(0)) < 0);
	put_word_ce (srca, 0 >> 16); put_word_ce (srca + 2, 0);
}}}}	m68k_incpc (2);
endlabel4357: ;
return 0;
}
/* CLR.L -(An) */
unsigned long REGPARAM2 CPUFUNC(op_42a0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = m68k_areg(regs, srcreg) - 4;
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4358;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	m68k_areg (regs, srcreg) = srca;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(0)) == 0);
	SET_NFLG (((uae_s32)(0)) < 0);
	put_word_ce (srca, 0 >> 16); put_word_ce (srca + 2, 0);
}}}}	m68k_incpc (2);
endlabel4358: ;
return 0;
}
/* CLR.L (d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_42a8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4359;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(0)) == 0);
	SET_NFLG (((uae_s32)(0)) < 0);
	put_word_ce (srca, 0 >> 16); put_word_ce (srca + 2, 0);
}}}}	m68k_incpc (4);
endlabel4359: ;
return 0;
}
/* CLR.L (d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_42b0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4360;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(0)) == 0);
	SET_NFLG (((uae_s32)(0)) < 0);
	put_word_ce (srca, 0 >> 16); put_word_ce (srca + 2, 0);
}}}}	m68k_incpc (4);
endlabel4360: ;
return 0;
}
/* CLR.L (xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_42b8_6)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4361;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(0)) == 0);
	SET_NFLG (((uae_s32)(0)) < 0);
	put_word_ce (srca, 0 >> 16); put_word_ce (srca + 2, 0);
}}}}	m68k_incpc (4);
endlabel4361: ;
return 0;
}
/* CLR.L (xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_42b9_6)(uae_u32 opcode)
{
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4362;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(0)) == 0);
	SET_NFLG (((uae_s32)(0)) < 0);
	put_word_ce (srca, 0 >> 16); put_word_ce (srca + 2, 0);
}}}}	m68k_incpc (6);
endlabel4362: ;
return 0;
}
/* NEG.B Dn */
unsigned long REGPARAM2 CPUFUNC(op_4400_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s8 src = m68k_dreg(regs, srcreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{{uae_u32 dst = ((uae_s8)(0)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(0)) < 0;
	int flgn = ((uae_s8)(dst)) < 0;
	SET_ZFLG (((uae_s8)(dst)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(0)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	m68k_dreg(regs, srcreg) = (m68k_dreg(regs, srcreg) & ~0xff) | ((dst) & 0xff);
}}}}}	m68k_incpc (2);
return 0;
}
/* NEG.B (An) */
unsigned long REGPARAM2 CPUFUNC(op_4410_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
{	uae_s8 src = get_byte_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{{uae_u32 dst = ((uae_s8)(0)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(0)) < 0;
	int flgn = ((uae_s8)(dst)) < 0;
	SET_ZFLG (((uae_s8)(dst)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(0)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_byte_ce (srca,dst);
}}}}}}	m68k_incpc (2);
return 0;
}
/* NEG.B (An)+ */
unsigned long REGPARAM2 CPUFUNC(op_4418_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
{	uae_s8 src = get_byte_ce (srca);
	m68k_areg(regs, srcreg) += areg_byteinc[srcreg];
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{{uae_u32 dst = ((uae_s8)(0)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(0)) < 0;
	int flgn = ((uae_s8)(dst)) < 0;
	SET_ZFLG (((uae_s8)(dst)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(0)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_byte_ce (srca,dst);
}}}}}}	m68k_incpc (2);
return 0;
}
/* NEG.B -(An) */
unsigned long REGPARAM2 CPUFUNC(op_4420_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = get_byte_ce (srca);
	m68k_areg (regs, srcreg) = srca;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{{uae_u32 dst = ((uae_s8)(0)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(0)) < 0;
	int flgn = ((uae_s8)(dst)) < 0;
	SET_ZFLG (((uae_s8)(dst)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(0)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_byte_ce (srca,dst);
}}}}}}	m68k_incpc (2);
return 0;
}
/* NEG.B (d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_4428_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 src = get_byte_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 dst = ((uae_s8)(0)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(0)) < 0;
	int flgn = ((uae_s8)(dst)) < 0;
	SET_ZFLG (((uae_s8)(dst)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(0)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_byte_ce (srca,dst);
}}}}}}	m68k_incpc (4);
return 0;
}
/* NEG.B (d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_4430_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
{	uae_s8 src = get_byte_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 dst = ((uae_s8)(0)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(0)) < 0;
	int flgn = ((uae_s8)(dst)) < 0;
	SET_ZFLG (((uae_s8)(dst)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(0)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_byte_ce (srca,dst);
}}}}}}	m68k_incpc (4);
return 0;
}
/* NEG.B (xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_4438_6)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 src = get_byte_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 dst = ((uae_s8)(0)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(0)) < 0;
	int flgn = ((uae_s8)(dst)) < 0;
	SET_ZFLG (((uae_s8)(dst)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(0)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_byte_ce (srca,dst);
}}}}}}	m68k_incpc (4);
return 0;
}
/* NEG.B (xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_4439_6)(uae_u32 opcode)
{
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
{	uae_s8 src = get_byte_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
{{uae_u32 dst = ((uae_s8)(0)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(0)) < 0;
	int flgn = ((uae_s8)(dst)) < 0;
	SET_ZFLG (((uae_s8)(dst)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(0)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_byte_ce (srca,dst);
}}}}}}	m68k_incpc (6);
return 0;
}
/* NEG.W Dn */
unsigned long REGPARAM2 CPUFUNC(op_4440_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_dreg(regs, srcreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{{uae_u32 dst = ((uae_s16)(0)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(0)) < 0;
	int flgn = ((uae_s16)(dst)) < 0;
	SET_ZFLG (((uae_s16)(dst)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(0)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	m68k_dreg(regs, srcreg) = (m68k_dreg(regs, srcreg) & ~0xffff) | ((dst) & 0xffff);
}}}}}	m68k_incpc (2);
return 0;
}
/* NEG.W (An) */
unsigned long REGPARAM2 CPUFUNC(op_4450_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4372;
	}
{{	uae_s16 src = get_word_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{{uae_u32 dst = ((uae_s16)(0)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(0)) < 0;
	int flgn = ((uae_s16)(dst)) < 0;
	SET_ZFLG (((uae_s16)(dst)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(0)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_word_ce (srca,dst);
}}}}}}}	m68k_incpc (2);
endlabel4372: ;
return 0;
}
/* NEG.W (An)+ */
unsigned long REGPARAM2 CPUFUNC(op_4458_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4373;
	}
{{	uae_s16 src = get_word_ce (srca);
	m68k_areg(regs, srcreg) += 2;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{{uae_u32 dst = ((uae_s16)(0)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(0)) < 0;
	int flgn = ((uae_s16)(dst)) < 0;
	SET_ZFLG (((uae_s16)(dst)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(0)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_word_ce (srca,dst);
}}}}}}}	m68k_incpc (2);
endlabel4373: ;
return 0;
}
/* NEG.W -(An) */
unsigned long REGPARAM2 CPUFUNC(op_4460_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = m68k_areg(regs, srcreg) - 2;
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4374;
	}
{{	uae_s16 src = get_word_ce (srca);
	m68k_areg (regs, srcreg) = srca;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{{uae_u32 dst = ((uae_s16)(0)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(0)) < 0;
	int flgn = ((uae_s16)(dst)) < 0;
	SET_ZFLG (((uae_s16)(dst)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(0)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_word_ce (srca,dst);
}}}}}}}	m68k_incpc (2);
endlabel4374: ;
return 0;
}
/* NEG.W (d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_4468_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4375;
	}
{{	uae_s16 src = get_word_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 dst = ((uae_s16)(0)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(0)) < 0;
	int flgn = ((uae_s16)(dst)) < 0;
	SET_ZFLG (((uae_s16)(dst)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(0)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_word_ce (srca,dst);
}}}}}}}	m68k_incpc (4);
endlabel4375: ;
return 0;
}
/* NEG.W (d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_4470_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4376;
	}
{{	uae_s16 src = get_word_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 dst = ((uae_s16)(0)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(0)) < 0;
	int flgn = ((uae_s16)(dst)) < 0;
	SET_ZFLG (((uae_s16)(dst)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(0)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_word_ce (srca,dst);
}}}}}}}	m68k_incpc (4);
endlabel4376: ;
return 0;
}
/* NEG.W (xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_4478_6)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4377;
	}
{{	uae_s16 src = get_word_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 dst = ((uae_s16)(0)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(0)) < 0;
	int flgn = ((uae_s16)(dst)) < 0;
	SET_ZFLG (((uae_s16)(dst)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(0)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_word_ce (srca,dst);
}}}}}}}	m68k_incpc (4);
endlabel4377: ;
return 0;
}
/* NEG.W (xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_4479_6)(uae_u32 opcode)
{
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4378;
	}
{{	uae_s16 src = get_word_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
{{uae_u32 dst = ((uae_s16)(0)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(0)) < 0;
	int flgn = ((uae_s16)(dst)) < 0;
	SET_ZFLG (((uae_s16)(dst)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(0)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_word_ce (srca,dst);
}}}}}}}	m68k_incpc (6);
endlabel4378: ;
return 0;
}
/* NEG.L Dn */
unsigned long REGPARAM2 CPUFUNC(op_4480_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s32 src = m68k_dreg(regs, srcreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{{uae_u32 dst = ((uae_s32)(0)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(0)) < 0;
	int flgn = ((uae_s32)(dst)) < 0;
	SET_ZFLG (((uae_s32)(dst)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(0)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	do_cycles (2 * 256);
	m68k_dreg(regs, srcreg) = (dst);
}}}}}	m68k_incpc (2);
return 0;
}
/* NEG.L (An) */
unsigned long REGPARAM2 CPUFUNC(op_4490_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4380;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{{uae_u32 dst = ((uae_s32)(0)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(0)) < 0;
	int flgn = ((uae_s32)(dst)) < 0;
	SET_ZFLG (((uae_s32)(dst)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(0)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_word_ce (srca, dst >> 16); put_word_ce (srca + 2, dst);
}}}}}}}	m68k_incpc (2);
endlabel4380: ;
return 0;
}
/* NEG.L (An)+ */
unsigned long REGPARAM2 CPUFUNC(op_4498_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4381;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	m68k_areg(regs, srcreg) += 4;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{{uae_u32 dst = ((uae_s32)(0)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(0)) < 0;
	int flgn = ((uae_s32)(dst)) < 0;
	SET_ZFLG (((uae_s32)(dst)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(0)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_word_ce (srca, dst >> 16); put_word_ce (srca + 2, dst);
}}}}}}}	m68k_incpc (2);
endlabel4381: ;
return 0;
}
/* NEG.L -(An) */
unsigned long REGPARAM2 CPUFUNC(op_44a0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = m68k_areg(regs, srcreg) - 4;
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4382;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	m68k_areg (regs, srcreg) = srca;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{{uae_u32 dst = ((uae_s32)(0)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(0)) < 0;
	int flgn = ((uae_s32)(dst)) < 0;
	SET_ZFLG (((uae_s32)(dst)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(0)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_word_ce (srca, dst >> 16); put_word_ce (srca + 2, dst);
}}}}}}}	m68k_incpc (2);
endlabel4382: ;
return 0;
}
/* NEG.L (d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_44a8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4383;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 dst = ((uae_s32)(0)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(0)) < 0;
	int flgn = ((uae_s32)(dst)) < 0;
	SET_ZFLG (((uae_s32)(dst)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(0)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_word_ce (srca, dst >> 16); put_word_ce (srca + 2, dst);
}}}}}}}	m68k_incpc (4);
endlabel4383: ;
return 0;
}
/* NEG.L (d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_44b0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4384;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 dst = ((uae_s32)(0)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(0)) < 0;
	int flgn = ((uae_s32)(dst)) < 0;
	SET_ZFLG (((uae_s32)(dst)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(0)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_word_ce (srca, dst >> 16); put_word_ce (srca + 2, dst);
}}}}}}}	m68k_incpc (4);
endlabel4384: ;
return 0;
}
/* NEG.L (xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_44b8_6)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4385;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 dst = ((uae_s32)(0)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(0)) < 0;
	int flgn = ((uae_s32)(dst)) < 0;
	SET_ZFLG (((uae_s32)(dst)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(0)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_word_ce (srca, dst >> 16); put_word_ce (srca + 2, dst);
}}}}}}}	m68k_incpc (4);
endlabel4385: ;
return 0;
}
/* NEG.L (xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_44b9_6)(uae_u32 opcode)
{
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4386;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
{{uae_u32 dst = ((uae_s32)(0)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(0)) < 0;
	int flgn = ((uae_s32)(dst)) < 0;
	SET_ZFLG (((uae_s32)(dst)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(0)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_word_ce (srca, dst >> 16); put_word_ce (srca + 2, dst);
}}}}}}}	m68k_incpc (6);
endlabel4386: ;
return 0;
}
/* MV2SR.B Dn */
unsigned long REGPARAM2 CPUFUNC(op_44c0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_dreg(regs, srcreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	do_cycles (8 * 256);
	MakeSR();
	regs.sr &= 0xFF00;
	regs.sr |= src & 0xFF;
	MakeFromSR();
}}	m68k_incpc (2);
return 0;
}
/* MV2SR.B (An) */
unsigned long REGPARAM2 CPUFUNC(op_44d0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4388;
	}
{{	uae_s16 src = get_word_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	do_cycles (8 * 256);
	MakeSR();
	regs.sr &= 0xFF00;
	regs.sr |= src & 0xFF;
	MakeFromSR();
}}}}	m68k_incpc (2);
endlabel4388: ;
return 0;
}
/* MV2SR.B (An)+ */
unsigned long REGPARAM2 CPUFUNC(op_44d8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4389;
	}
{{	uae_s16 src = get_word_ce (srca);
	m68k_areg(regs, srcreg) += 2;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	do_cycles (8 * 256);
	MakeSR();
	regs.sr &= 0xFF00;
	regs.sr |= src & 0xFF;
	MakeFromSR();
}}}}	m68k_incpc (2);
endlabel4389: ;
return 0;
}
/* MV2SR.B -(An) */
unsigned long REGPARAM2 CPUFUNC(op_44e0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = m68k_areg(regs, srcreg) - 2;
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4390;
	}
{{	uae_s16 src = get_word_ce (srca);
	m68k_areg (regs, srcreg) = srca;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	do_cycles (8 * 256);
	MakeSR();
	regs.sr &= 0xFF00;
	regs.sr |= src & 0xFF;
	MakeFromSR();
}}}}	m68k_incpc (2);
endlabel4390: ;
return 0;
}
/* MV2SR.B (d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_44e8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4391;
	}
{{	uae_s16 src = get_word_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	do_cycles (8 * 256);
	MakeSR();
	regs.sr &= 0xFF00;
	regs.sr |= src & 0xFF;
	MakeFromSR();
}}}}	m68k_incpc (4);
endlabel4391: ;
return 0;
}
/* MV2SR.B (d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_44f0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4392;
	}
{{	uae_s16 src = get_word_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	do_cycles (8 * 256);
	MakeSR();
	regs.sr &= 0xFF00;
	regs.sr |= src & 0xFF;
	MakeFromSR();
}}}}	m68k_incpc (4);
endlabel4392: ;
return 0;
}
/* MV2SR.B (xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_44f8_6)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4393;
	}
{{	uae_s16 src = get_word_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	do_cycles (8 * 256);
	MakeSR();
	regs.sr &= 0xFF00;
	regs.sr |= src & 0xFF;
	MakeFromSR();
}}}}	m68k_incpc (4);
endlabel4393: ;
return 0;
}
/* MV2SR.B (xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_44f9_6)(uae_u32 opcode)
{
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4394;
	}
{{	uae_s16 src = get_word_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	do_cycles (8 * 256);
	MakeSR();
	regs.sr &= 0xFF00;
	regs.sr |= src & 0xFF;
	MakeFromSR();
}}}}	m68k_incpc (6);
endlabel4394: ;
return 0;
}
/* MV2SR.B (d16,PC) */
unsigned long REGPARAM2 CPUFUNC(op_44fa_6)(uae_u32 opcode)
{
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4395;
	}
{{	uae_s16 src = get_word_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	do_cycles (8 * 256);
	MakeSR();
	regs.sr &= 0xFF00;
	regs.sr |= src & 0xFF;
	MakeFromSR();
}}}}	m68k_incpc (4);
endlabel4395: ;
return 0;
}
/* MV2SR.B (d8,PC,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_44fb_6)(uae_u32 opcode)
{
{{	uaecptr tmppc;
	uaecptr srca;
	do_cycles (2 * 256);
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4396;
	}
{{	uae_s16 src = get_word_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	do_cycles (8 * 256);
	MakeSR();
	regs.sr &= 0xFF00;
	regs.sr |= src & 0xFF;
	MakeFromSR();
}}}}	m68k_incpc (4);
endlabel4396: ;
return 0;
}
/* MV2SR.B #<data>.B */
unsigned long REGPARAM2 CPUFUNC(op_44fc_6)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce_prefetch (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	do_cycles (8 * 256);
	MakeSR();
	regs.sr &= 0xFF00;
	regs.sr |= src & 0xFF;
	MakeFromSR();
}}	m68k_incpc (4);
return 0;
}
/* NOT.B Dn */
unsigned long REGPARAM2 CPUFUNC(op_4600_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s8 src = m68k_dreg(regs, srcreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 dst = ~src;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(dst)) == 0);
	SET_NFLG (((uae_s8)(dst)) < 0);
	m68k_dreg(regs, srcreg) = (m68k_dreg(regs, srcreg) & ~0xff) | ((dst) & 0xff);
}}}	m68k_incpc (2);
return 0;
}
/* NOT.B (An) */
unsigned long REGPARAM2 CPUFUNC(op_4610_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
{	uae_s8 src = get_byte_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 dst = ~src;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(dst)) == 0);
	SET_NFLG (((uae_s8)(dst)) < 0);
	put_byte_ce (srca,dst);
}}}}	m68k_incpc (2);
return 0;
}
/* NOT.B (An)+ */
unsigned long REGPARAM2 CPUFUNC(op_4618_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
{	uae_s8 src = get_byte_ce (srca);
	m68k_areg(regs, srcreg) += areg_byteinc[srcreg];
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 dst = ~src;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(dst)) == 0);
	SET_NFLG (((uae_s8)(dst)) < 0);
	put_byte_ce (srca,dst);
}}}}	m68k_incpc (2);
return 0;
}
/* NOT.B -(An) */
unsigned long REGPARAM2 CPUFUNC(op_4620_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = get_byte_ce (srca);
	m68k_areg (regs, srcreg) = srca;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 dst = ~src;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(dst)) == 0);
	SET_NFLG (((uae_s8)(dst)) < 0);
	put_byte_ce (srca,dst);
}}}}	m68k_incpc (2);
return 0;
}
/* NOT.B (d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_4628_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 src = get_byte_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	uae_u32 dst = ~src;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(dst)) == 0);
	SET_NFLG (((uae_s8)(dst)) < 0);
	put_byte_ce (srca,dst);
}}}}	m68k_incpc (4);
return 0;
}
/* NOT.B (d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_4630_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
{	uae_s8 src = get_byte_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	uae_u32 dst = ~src;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(dst)) == 0);
	SET_NFLG (((uae_s8)(dst)) < 0);
	put_byte_ce (srca,dst);
}}}}	m68k_incpc (4);
return 0;
}
/* NOT.B (xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_4638_6)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 src = get_byte_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	uae_u32 dst = ~src;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(dst)) == 0);
	SET_NFLG (((uae_s8)(dst)) < 0);
	put_byte_ce (srca,dst);
}}}}	m68k_incpc (4);
return 0;
}
/* NOT.B (xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_4639_6)(uae_u32 opcode)
{
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
{	uae_s8 src = get_byte_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
{	uae_u32 dst = ~src;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(dst)) == 0);
	SET_NFLG (((uae_s8)(dst)) < 0);
	put_byte_ce (srca,dst);
}}}}	m68k_incpc (6);
return 0;
}
/* NOT.W Dn */
unsigned long REGPARAM2 CPUFUNC(op_4640_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_dreg(regs, srcreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 dst = ~src;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(dst)) == 0);
	SET_NFLG (((uae_s16)(dst)) < 0);
	m68k_dreg(regs, srcreg) = (m68k_dreg(regs, srcreg) & ~0xffff) | ((dst) & 0xffff);
}}}	m68k_incpc (2);
return 0;
}
/* NOT.W (An) */
unsigned long REGPARAM2 CPUFUNC(op_4650_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4407;
	}
{{	uae_s16 src = get_word_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 dst = ~src;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(dst)) == 0);
	SET_NFLG (((uae_s16)(dst)) < 0);
	put_word_ce (srca,dst);
}}}}}	m68k_incpc (2);
endlabel4407: ;
return 0;
}
/* NOT.W (An)+ */
unsigned long REGPARAM2 CPUFUNC(op_4658_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4408;
	}
{{	uae_s16 src = get_word_ce (srca);
	m68k_areg(regs, srcreg) += 2;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 dst = ~src;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(dst)) == 0);
	SET_NFLG (((uae_s16)(dst)) < 0);
	put_word_ce (srca,dst);
}}}}}	m68k_incpc (2);
endlabel4408: ;
return 0;
}
/* NOT.W -(An) */
unsigned long REGPARAM2 CPUFUNC(op_4660_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = m68k_areg(regs, srcreg) - 2;
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4409;
	}
{{	uae_s16 src = get_word_ce (srca);
	m68k_areg (regs, srcreg) = srca;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 dst = ~src;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(dst)) == 0);
	SET_NFLG (((uae_s16)(dst)) < 0);
	put_word_ce (srca,dst);
}}}}}	m68k_incpc (2);
endlabel4409: ;
return 0;
}
/* NOT.W (d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_4668_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4410;
	}
{{	uae_s16 src = get_word_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	uae_u32 dst = ~src;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(dst)) == 0);
	SET_NFLG (((uae_s16)(dst)) < 0);
	put_word_ce (srca,dst);
}}}}}	m68k_incpc (4);
endlabel4410: ;
return 0;
}
/* NOT.W (d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_4670_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4411;
	}
{{	uae_s16 src = get_word_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	uae_u32 dst = ~src;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(dst)) == 0);
	SET_NFLG (((uae_s16)(dst)) < 0);
	put_word_ce (srca,dst);
}}}}}	m68k_incpc (4);
endlabel4411: ;
return 0;
}
/* NOT.W (xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_4678_6)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4412;
	}
{{	uae_s16 src = get_word_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	uae_u32 dst = ~src;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(dst)) == 0);
	SET_NFLG (((uae_s16)(dst)) < 0);
	put_word_ce (srca,dst);
}}}}}	m68k_incpc (4);
endlabel4412: ;
return 0;
}
/* NOT.W (xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_4679_6)(uae_u32 opcode)
{
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4413;
	}
{{	uae_s16 src = get_word_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
{	uae_u32 dst = ~src;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(dst)) == 0);
	SET_NFLG (((uae_s16)(dst)) < 0);
	put_word_ce (srca,dst);
}}}}}	m68k_incpc (6);
endlabel4413: ;
return 0;
}
/* NOT.L Dn */
unsigned long REGPARAM2 CPUFUNC(op_4680_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s32 src = m68k_dreg(regs, srcreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 dst = ~src;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(dst)) == 0);
	SET_NFLG (((uae_s32)(dst)) < 0);
	do_cycles (2 * 256);
	m68k_dreg(regs, srcreg) = (dst);
}}}	m68k_incpc (2);
return 0;
}
/* NOT.L (An) */
unsigned long REGPARAM2 CPUFUNC(op_4690_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4415;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 dst = ~src;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(dst)) == 0);
	SET_NFLG (((uae_s32)(dst)) < 0);
	put_word_ce (srca, dst >> 16); put_word_ce (srca + 2, dst);
}}}}}	m68k_incpc (2);
endlabel4415: ;
return 0;
}
/* NOT.L (An)+ */
unsigned long REGPARAM2 CPUFUNC(op_4698_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4416;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	m68k_areg(regs, srcreg) += 4;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 dst = ~src;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(dst)) == 0);
	SET_NFLG (((uae_s32)(dst)) < 0);
	put_word_ce (srca, dst >> 16); put_word_ce (srca + 2, dst);
}}}}}	m68k_incpc (2);
endlabel4416: ;
return 0;
}
/* NOT.L -(An) */
unsigned long REGPARAM2 CPUFUNC(op_46a0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = m68k_areg(regs, srcreg) - 4;
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4417;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	m68k_areg (regs, srcreg) = srca;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 dst = ~src;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(dst)) == 0);
	SET_NFLG (((uae_s32)(dst)) < 0);
	put_word_ce (srca, dst >> 16); put_word_ce (srca + 2, dst);
}}}}}	m68k_incpc (2);
endlabel4417: ;
return 0;
}
/* NOT.L (d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_46a8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4418;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	uae_u32 dst = ~src;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(dst)) == 0);
	SET_NFLG (((uae_s32)(dst)) < 0);
	put_word_ce (srca, dst >> 16); put_word_ce (srca + 2, dst);
}}}}}	m68k_incpc (4);
endlabel4418: ;
return 0;
}
/* NOT.L (d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_46b0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4419;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	uae_u32 dst = ~src;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(dst)) == 0);
	SET_NFLG (((uae_s32)(dst)) < 0);
	put_word_ce (srca, dst >> 16); put_word_ce (srca + 2, dst);
}}}}}	m68k_incpc (4);
endlabel4419: ;
return 0;
}
/* NOT.L (xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_46b8_6)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4420;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	uae_u32 dst = ~src;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(dst)) == 0);
	SET_NFLG (((uae_s32)(dst)) < 0);
	put_word_ce (srca, dst >> 16); put_word_ce (srca + 2, dst);
}}}}}	m68k_incpc (4);
endlabel4420: ;
return 0;
}
/* NOT.L (xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_46b9_6)(uae_u32 opcode)
{
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4421;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
{	uae_u32 dst = ~src;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(dst)) == 0);
	SET_NFLG (((uae_s32)(dst)) < 0);
	put_word_ce (srca, dst >> 16); put_word_ce (srca + 2, dst);
}}}}}	m68k_incpc (6);
endlabel4421: ;
return 0;
}
/* MV2SR.W Dn */
unsigned long REGPARAM2 CPUFUNC(op_46c0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception(8,0); goto endlabel4422; }
{{	uae_s16 src = m68k_dreg(regs, srcreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	do_cycles (4 * 256);
	regs.sr = src;
	MakeFromSR();
}}}	m68k_incpc (2);
endlabel4422: ;
return 0;
}
/* MV2SR.W (An) */
unsigned long REGPARAM2 CPUFUNC(op_46d0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception(8,0); goto endlabel4423; }
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4423;
	}
{{	uae_s16 src = get_word_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	do_cycles (4 * 256);
	regs.sr = src;
	MakeFromSR();
}}}}}	m68k_incpc (2);
endlabel4423: ;
return 0;
}
/* MV2SR.W (An)+ */
unsigned long REGPARAM2 CPUFUNC(op_46d8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception(8,0); goto endlabel4424; }
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4424;
	}
{{	uae_s16 src = get_word_ce (srca);
	m68k_areg(regs, srcreg) += 2;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	do_cycles (4 * 256);
	regs.sr = src;
	MakeFromSR();
}}}}}	m68k_incpc (2);
endlabel4424: ;
return 0;
}
/* MV2SR.W -(An) */
unsigned long REGPARAM2 CPUFUNC(op_46e0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception(8,0); goto endlabel4425; }
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = m68k_areg(regs, srcreg) - 2;
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4425;
	}
{{	uae_s16 src = get_word_ce (srca);
	m68k_areg (regs, srcreg) = srca;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	do_cycles (4 * 256);
	regs.sr = src;
	MakeFromSR();
}}}}}	m68k_incpc (2);
endlabel4425: ;
return 0;
}
/* MV2SR.W (d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_46e8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception(8,0); goto endlabel4426; }
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4426;
	}
{{	uae_s16 src = get_word_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	do_cycles (4 * 256);
	regs.sr = src;
	MakeFromSR();
}}}}}	m68k_incpc (4);
endlabel4426: ;
return 0;
}
/* MV2SR.W (d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_46f0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception(8,0); goto endlabel4427; }
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4427;
	}
{{	uae_s16 src = get_word_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	do_cycles (4 * 256);
	regs.sr = src;
	MakeFromSR();
}}}}}	m68k_incpc (4);
endlabel4427: ;
return 0;
}
/* MV2SR.W (xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_46f8_6)(uae_u32 opcode)
{
{if (!regs.s) { Exception(8,0); goto endlabel4428; }
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4428;
	}
{{	uae_s16 src = get_word_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	do_cycles (4 * 256);
	regs.sr = src;
	MakeFromSR();
}}}}}	m68k_incpc (4);
endlabel4428: ;
return 0;
}
/* MV2SR.W (xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_46f9_6)(uae_u32 opcode)
{
{if (!regs.s) { Exception(8,0); goto endlabel4429; }
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4429;
	}
{{	uae_s16 src = get_word_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	do_cycles (4 * 256);
	regs.sr = src;
	MakeFromSR();
}}}}}	m68k_incpc (6);
endlabel4429: ;
return 0;
}
/* MV2SR.W (d16,PC) */
unsigned long REGPARAM2 CPUFUNC(op_46fa_6)(uae_u32 opcode)
{
{if (!regs.s) { Exception(8,0); goto endlabel4430; }
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4430;
	}
{{	uae_s16 src = get_word_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	do_cycles (4 * 256);
	regs.sr = src;
	MakeFromSR();
}}}}}	m68k_incpc (4);
endlabel4430: ;
return 0;
}
/* MV2SR.W (d8,PC,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_46fb_6)(uae_u32 opcode)
{
{if (!regs.s) { Exception(8,0); goto endlabel4431; }
{{	uaecptr tmppc;
	uaecptr srca;
	do_cycles (2 * 256);
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4431;
	}
{{	uae_s16 src = get_word_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	do_cycles (4 * 256);
	regs.sr = src;
	MakeFromSR();
}}}}}	m68k_incpc (4);
endlabel4431: ;
return 0;
}
/* MV2SR.W #<data>.W */
unsigned long REGPARAM2 CPUFUNC(op_46fc_6)(uae_u32 opcode)
{
{if (!regs.s) { Exception(8,0); goto endlabel4432; }
{{	uae_s16 src = get_word_ce_prefetch (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	do_cycles (4 * 256);
	regs.sr = src;
	MakeFromSR();
}}}	m68k_incpc (4);
endlabel4432: ;
return 0;
}
/* NBCD.B Dn */
unsigned long REGPARAM2 CPUFUNC(op_4800_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s8 src = m68k_dreg(regs, srcreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u16 newv_lo = - (src & 0xF) - (GET_XFLG ? 1 : 0);
	uae_u16 newv_hi = - (src & 0xF0);
	uae_u16 newv;
	int cflg;
	if (newv_lo > 9) { newv_lo -= 6; }
	newv = newv_hi + newv_lo;	cflg = (newv & 0x1F0) > 0x90;
	if (cflg) newv -= 0x60;
	SET_CFLG (cflg);
	COPY_CARRY;
	do_cycles (2 * 256);
	SET_ZFLG (GET_ZFLG & (((uae_s8)(newv)) == 0));
	SET_NFLG (((uae_s8)(newv)) < 0);
	m68k_dreg(regs, srcreg) = (m68k_dreg(regs, srcreg) & ~0xff) | ((newv) & 0xff);
}}}	m68k_incpc (2);
return 0;
}
/* NBCD.B (An) */
unsigned long REGPARAM2 CPUFUNC(op_4810_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
{	uae_s8 src = get_byte_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u16 newv_lo = - (src & 0xF) - (GET_XFLG ? 1 : 0);
	uae_u16 newv_hi = - (src & 0xF0);
	uae_u16 newv;
	int cflg;
	if (newv_lo > 9) { newv_lo -= 6; }
	newv = newv_hi + newv_lo;	cflg = (newv & 0x1F0) > 0x90;
	if (cflg) newv -= 0x60;
	SET_CFLG (cflg);
	COPY_CARRY;
	SET_ZFLG (GET_ZFLG & (((uae_s8)(newv)) == 0));
	SET_NFLG (((uae_s8)(newv)) < 0);
	put_byte_ce (srca,newv);
}}}}	m68k_incpc (2);
return 0;
}
/* NBCD.B (An)+ */
unsigned long REGPARAM2 CPUFUNC(op_4818_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
{	uae_s8 src = get_byte_ce (srca);
	m68k_areg(regs, srcreg) += areg_byteinc[srcreg];
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u16 newv_lo = - (src & 0xF) - (GET_XFLG ? 1 : 0);
	uae_u16 newv_hi = - (src & 0xF0);
	uae_u16 newv;
	int cflg;
	if (newv_lo > 9) { newv_lo -= 6; }
	newv = newv_hi + newv_lo;	cflg = (newv & 0x1F0) > 0x90;
	if (cflg) newv -= 0x60;
	SET_CFLG (cflg);
	COPY_CARRY;
	SET_ZFLG (GET_ZFLG & (((uae_s8)(newv)) == 0));
	SET_NFLG (((uae_s8)(newv)) < 0);
	put_byte_ce (srca,newv);
}}}}	m68k_incpc (2);
return 0;
}
/* NBCD.B -(An) */
unsigned long REGPARAM2 CPUFUNC(op_4820_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = get_byte_ce (srca);
	m68k_areg (regs, srcreg) = srca;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u16 newv_lo = - (src & 0xF) - (GET_XFLG ? 1 : 0);
	uae_u16 newv_hi = - (src & 0xF0);
	uae_u16 newv;
	int cflg;
	if (newv_lo > 9) { newv_lo -= 6; }
	newv = newv_hi + newv_lo;	cflg = (newv & 0x1F0) > 0x90;
	if (cflg) newv -= 0x60;
	SET_CFLG (cflg);
	COPY_CARRY;
	SET_ZFLG (GET_ZFLG & (((uae_s8)(newv)) == 0));
	SET_NFLG (((uae_s8)(newv)) < 0);
	put_byte_ce (srca,newv);
}}}}	m68k_incpc (2);
return 0;
}
/* NBCD.B (d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_4828_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 src = get_byte_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	uae_u16 newv_lo = - (src & 0xF) - (GET_XFLG ? 1 : 0);
	uae_u16 newv_hi = - (src & 0xF0);
	uae_u16 newv;
	int cflg;
	if (newv_lo > 9) { newv_lo -= 6; }
	newv = newv_hi + newv_lo;	cflg = (newv & 0x1F0) > 0x90;
	if (cflg) newv -= 0x60;
	SET_CFLG (cflg);
	COPY_CARRY;
	SET_ZFLG (GET_ZFLG & (((uae_s8)(newv)) == 0));
	SET_NFLG (((uae_s8)(newv)) < 0);
	put_byte_ce (srca,newv);
}}}}	m68k_incpc (4);
return 0;
}
/* NBCD.B (d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_4830_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
{	uae_s8 src = get_byte_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	uae_u16 newv_lo = - (src & 0xF) - (GET_XFLG ? 1 : 0);
	uae_u16 newv_hi = - (src & 0xF0);
	uae_u16 newv;
	int cflg;
	if (newv_lo > 9) { newv_lo -= 6; }
	newv = newv_hi + newv_lo;	cflg = (newv & 0x1F0) > 0x90;
	if (cflg) newv -= 0x60;
	SET_CFLG (cflg);
	COPY_CARRY;
	SET_ZFLG (GET_ZFLG & (((uae_s8)(newv)) == 0));
	SET_NFLG (((uae_s8)(newv)) < 0);
	put_byte_ce (srca,newv);
}}}}	m68k_incpc (4);
return 0;
}
/* NBCD.B (xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_4838_6)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 src = get_byte_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	uae_u16 newv_lo = - (src & 0xF) - (GET_XFLG ? 1 : 0);
	uae_u16 newv_hi = - (src & 0xF0);
	uae_u16 newv;
	int cflg;
	if (newv_lo > 9) { newv_lo -= 6; }
	newv = newv_hi + newv_lo;	cflg = (newv & 0x1F0) > 0x90;
	if (cflg) newv -= 0x60;
	SET_CFLG (cflg);
	COPY_CARRY;
	SET_ZFLG (GET_ZFLG & (((uae_s8)(newv)) == 0));
	SET_NFLG (((uae_s8)(newv)) < 0);
	put_byte_ce (srca,newv);
}}}}	m68k_incpc (4);
return 0;
}
/* NBCD.B (xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_4839_6)(uae_u32 opcode)
{
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
{	uae_s8 src = get_byte_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
{	uae_u16 newv_lo = - (src & 0xF) - (GET_XFLG ? 1 : 0);
	uae_u16 newv_hi = - (src & 0xF0);
	uae_u16 newv;
	int cflg;
	if (newv_lo > 9) { newv_lo -= 6; }
	newv = newv_hi + newv_lo;	cflg = (newv & 0x1F0) > 0x90;
	if (cflg) newv -= 0x60;
	SET_CFLG (cflg);
	COPY_CARRY;
	SET_ZFLG (GET_ZFLG & (((uae_s8)(newv)) == 0));
	SET_NFLG (((uae_s8)(newv)) < 0);
	put_byte_ce (srca,newv);
}}}}	m68k_incpc (6);
return 0;
}
/* SWAP.W Dn */
unsigned long REGPARAM2 CPUFUNC(op_4840_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s32 src = m68k_dreg(regs, srcreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 dst = ((src >> 16)&0xFFFF) | ((src&0xFFFF)<<16);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(dst)) == 0);
	SET_NFLG (((uae_s32)(dst)) < 0);
	m68k_dreg(regs, srcreg) = (dst);
}}}	m68k_incpc (2);
return 0;
}
/* PEA.L (An) */
unsigned long REGPARAM2 CPUFUNC(op_4850_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg(regs, 7) - 4;
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4442;
	}
{	m68k_areg (regs, 7) = dsta;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	put_word_ce (dsta, srca >> 16); put_word_ce (dsta + 2, srca);
}}}}	m68k_incpc (2);
endlabel4442: ;
return 0;
}
/* PEA.L (d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_4868_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uaecptr dsta;
	dsta = m68k_areg(regs, 7) - 4;
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4443;
	}
{	m68k_areg (regs, 7) = dsta;
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	put_word_ce (dsta, srca >> 16); put_word_ce (dsta + 2, srca);
}}}}	m68k_incpc (4);
endlabel4443: ;
return 0;
}
/* PEA.L (d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_4870_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
{	uaecptr dsta;
	dsta = m68k_areg(regs, 7) - 4;
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4444;
	}
{	m68k_areg (regs, 7) = dsta;
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	do_cycles (4 * 256);
	put_word_ce (dsta, srca >> 16); put_word_ce (dsta + 2, srca);
}}}}	m68k_incpc (4);
endlabel4444: ;
return 0;
}
/* PEA.L (xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_4878_6)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uaecptr dsta;
	dsta = m68k_areg(regs, 7) - 4;
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4445;
	}
{	m68k_areg (regs, 7) = dsta;
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	put_word_ce (dsta, srca >> 16); put_word_ce (dsta + 2, srca);
}}}}	m68k_incpc (4);
endlabel4445: ;
return 0;
}
/* PEA.L (xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_4879_6)(uae_u32 opcode)
{
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
{	uaecptr dsta;
	dsta = m68k_areg(regs, 7) - 4;
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel4446;
	}
{	m68k_areg (regs, 7) = dsta;
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	put_word_ce (dsta, srca >> 16); put_word_ce (dsta + 2, srca);
}}}}	m68k_incpc (6);
endlabel4446: ;
return 0;
}
/* PEA.L (d16,PC) */
unsigned long REGPARAM2 CPUFUNC(op_487a_6)(uae_u32 opcode)
{
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uaecptr dsta;
	dsta = m68k_areg(regs, 7) - 4;
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4447;
	}
{	m68k_areg (regs, 7) = dsta;
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	put_word_ce (dsta, srca >> 16); put_word_ce (dsta + 2, srca);
}}}}	m68k_incpc (4);
endlabel4447: ;
return 0;
}
/* PEA.L (d8,PC,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_487b_6)(uae_u32 opcode)
{
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
{	uaecptr dsta;
	dsta = m68k_areg(regs, 7) - 4;
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4448;
	}
{	m68k_areg (regs, 7) = dsta;
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	do_cycles (4 * 256);
	put_word_ce (dsta, srca >> 16); put_word_ce (dsta + 2, srca);
}}}}	m68k_incpc (4);
endlabel4448: ;
return 0;
}
/* EXT.W Dn */
unsigned long REGPARAM2 CPUFUNC(op_4880_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s32 src = m68k_dreg(regs, srcreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u16 dst = (uae_s16)(uae_s8)src;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(dst)) == 0);
	SET_NFLG (((uae_s16)(dst)) < 0);
	m68k_dreg(regs, srcreg) = (m68k_dreg(regs, srcreg) & ~0xffff) | ((dst) & 0xffff);
}}}	m68k_incpc (2);
return 0;
}
/* MVMLE.W #<data>.W,(An) */
unsigned long REGPARAM2 CPUFUNC(op_4890_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{	uae_u16 mask = get_word_ce_prefetch (4);
{	uaecptr srca = m68k_areg(regs, dstreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 4, srca);
		goto endlabel4450;
	}
{{	uae_u16 dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
	while (dmask) { put_word_ce (srca, m68k_dreg(regs, movem_index1[dmask])); srca += 2; dmask = movem_next[dmask]; }
	while (amask) { put_word_ce (srca, m68k_areg(regs, movem_index1[amask])); srca += 2; amask = movem_next[amask]; }
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
}}}}	m68k_incpc (4);
endlabel4450: ;
return 0;
}
/* MVMLE.W #<data>.W,-(An) */
unsigned long REGPARAM2 CPUFUNC(op_48a0_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{	uae_u16 mask = get_word_ce_prefetch (4);
{	uaecptr srca;
	srca = m68k_areg(regs, dstreg) - 0;
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 4, srca);
		goto endlabel4451;
	}
{{	uae_u16 amask = mask & 0xff, dmask = (mask >> 8) & 0xff;
	while (amask) { srca -= 2; put_word_ce (srca, m68k_areg(regs, movem_index2[amask])); amask = movem_next[amask]; }
	while (dmask) { srca -= 2; put_word_ce (srca, m68k_dreg(regs, movem_index2[dmask])); dmask = movem_next[dmask]; }
	m68k_areg(regs, dstreg) = srca;
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
}}}}	m68k_incpc (4);
endlabel4451: ;
return 0;
}
/* MVMLE.W #<data>.W,(d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_48a8_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{	uae_u16 mask = get_word_ce_prefetch (4);
{	uaecptr srca = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 4, srca);
		goto endlabel4452;
	}
{{	uae_u16 dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
	while (dmask) { put_word_ce (srca, m68k_dreg(regs, movem_index1[dmask])); srca += 2; dmask = movem_next[dmask]; }
	while (amask) { put_word_ce (srca, m68k_areg(regs, movem_index1[amask])); srca += 2; amask = movem_next[amask]; }
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
}}}}	m68k_incpc (6);
endlabel4452: ;
return 0;
}
/* MVMLE.W #<data>.W,(d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_48b0_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{	uae_u16 mask = get_word_ce_prefetch (4);
{	uaecptr srca;
	srca = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (6));
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 4, srca);
		goto endlabel4453;
	}
{	do_cycles (2 * 256);
{	uae_u16 dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
	while (dmask) { put_word_ce (srca, m68k_dreg(regs, movem_index1[dmask])); srca += 2; dmask = movem_next[dmask]; }
	while (amask) { put_word_ce (srca, m68k_areg(regs, movem_index1[amask])); srca += 2; amask = movem_next[amask]; }
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
}}}}	m68k_incpc (6);
endlabel4453: ;
return 0;
}
/* MVMLE.W #<data>.W,(xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_48b8_6)(uae_u32 opcode)
{
{	uae_u16 mask = get_word_ce_prefetch (4);
{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 4, srca);
		goto endlabel4454;
	}
{{	uae_u16 dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
	while (dmask) { put_word_ce (srca, m68k_dreg(regs, movem_index1[dmask])); srca += 2; dmask = movem_next[dmask]; }
	while (amask) { put_word_ce (srca, m68k_areg(regs, movem_index1[amask])); srca += 2; amask = movem_next[amask]; }
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
}}}}	m68k_incpc (6);
endlabel4454: ;
return 0;
}
/* MVMLE.W #<data>.W,(xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_48b9_6)(uae_u32 opcode)
{
{	uae_u16 mask = get_word_ce_prefetch (4);
{	uaecptr srca;
	srca = get_word_ce_prefetch (6) << 16;
	srca |= get_word_ce_prefetch (8);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 4, srca);
		goto endlabel4455;
	}
{{	uae_u16 dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
	while (dmask) { put_word_ce (srca, m68k_dreg(regs, movem_index1[dmask])); srca += 2; dmask = movem_next[dmask]; }
	while (amask) { put_word_ce (srca, m68k_areg(regs, movem_index1[amask])); srca += 2; amask = movem_next[amask]; }
	regs.ir = regs.irc;
	get_word_ce_prefetch (10);
}}}}	m68k_incpc (8);
endlabel4455: ;
return 0;
}
/* EXT.L Dn */
unsigned long REGPARAM2 CPUFUNC(op_48c0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s32 src = m68k_dreg(regs, srcreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 dst = (uae_s32)(uae_s16)src;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(dst)) == 0);
	SET_NFLG (((uae_s32)(dst)) < 0);
	m68k_dreg(regs, srcreg) = (dst);
}}}	m68k_incpc (2);
return 0;
}
/* MVMLE.L #<data>.W,(An) */
unsigned long REGPARAM2 CPUFUNC(op_48d0_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{	uae_u16 mask = get_word_ce_prefetch (4);
{	uaecptr srca = m68k_areg(regs, dstreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 4, srca);
		goto endlabel4457;
	}
{{	uae_u16 dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
	while (dmask) { put_word_ce (srca, m68k_dreg(regs, movem_index1[dmask]) >> 16); put_word_ce (srca + 2, m68k_dreg(regs, movem_index1[dmask])); srca += 4; dmask = movem_next[dmask]; }
	while (amask) { put_word_ce (srca, m68k_areg(regs, movem_index1[amask]) >> 16); put_word_ce (srca + 2, m68k_areg(regs, movem_index1[amask])); srca += 4; amask = movem_next[amask]; }
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
}}}}	m68k_incpc (4);
endlabel4457: ;
return 0;
}
/* MVMLE.L #<data>.W,-(An) */
unsigned long REGPARAM2 CPUFUNC(op_48e0_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{	uae_u16 mask = get_word_ce_prefetch (4);
{	uaecptr srca;
	srca = m68k_areg(regs, dstreg) - 0;
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 4, srca);
		goto endlabel4458;
	}
{{	uae_u16 amask = mask & 0xff, dmask = (mask >> 8) & 0xff;
	while (amask) { srca -= 4; put_word_ce (srca, m68k_areg(regs, movem_index2[amask]) >> 16); put_word_ce (srca + 2, m68k_areg(regs, movem_index2[amask])); amask = movem_next[amask]; }
	while (dmask) { srca -= 4; put_word_ce (srca, m68k_dreg(regs, movem_index2[dmask]) >> 16); put_word_ce (srca + 2, m68k_dreg(regs, movem_index2[dmask])); dmask = movem_next[dmask]; }
	m68k_areg(regs, dstreg) = srca;
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
}}}}	m68k_incpc (4);
endlabel4458: ;
return 0;
}
/* MVMLE.L #<data>.W,(d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_48e8_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{	uae_u16 mask = get_word_ce_prefetch (4);
{	uaecptr srca = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 4, srca);
		goto endlabel4459;
	}
{{	uae_u16 dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
	while (dmask) { put_word_ce (srca, m68k_dreg(regs, movem_index1[dmask]) >> 16); put_word_ce (srca + 2, m68k_dreg(regs, movem_index1[dmask])); srca += 4; dmask = movem_next[dmask]; }
	while (amask) { put_word_ce (srca, m68k_areg(regs, movem_index1[amask]) >> 16); put_word_ce (srca + 2, m68k_areg(regs, movem_index1[amask])); srca += 4; amask = movem_next[amask]; }
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
}}}}	m68k_incpc (6);
endlabel4459: ;
return 0;
}
/* MVMLE.L #<data>.W,(d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_48f0_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{	uae_u16 mask = get_word_ce_prefetch (4);
{	uaecptr srca;
	srca = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (6));
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 4, srca);
		goto endlabel4460;
	}
{	do_cycles (2 * 256);
{	uae_u16 dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
	while (dmask) { put_word_ce (srca, m68k_dreg(regs, movem_index1[dmask]) >> 16); put_word_ce (srca + 2, m68k_dreg(regs, movem_index1[dmask])); srca += 4; dmask = movem_next[dmask]; }
	while (amask) { put_word_ce (srca, m68k_areg(regs, movem_index1[amask]) >> 16); put_word_ce (srca + 2, m68k_areg(regs, movem_index1[amask])); srca += 4; amask = movem_next[amask]; }
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
}}}}	m68k_incpc (6);
endlabel4460: ;
return 0;
}
/* MVMLE.L #<data>.W,(xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_48f8_6)(uae_u32 opcode)
{
{	uae_u16 mask = get_word_ce_prefetch (4);
{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 4, srca);
		goto endlabel4461;
	}
{{	uae_u16 dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
	while (dmask) { put_word_ce (srca, m68k_dreg(regs, movem_index1[dmask]) >> 16); put_word_ce (srca + 2, m68k_dreg(regs, movem_index1[dmask])); srca += 4; dmask = movem_next[dmask]; }
	while (amask) { put_word_ce (srca, m68k_areg(regs, movem_index1[amask]) >> 16); put_word_ce (srca + 2, m68k_areg(regs, movem_index1[amask])); srca += 4; amask = movem_next[amask]; }
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
}}}}	m68k_incpc (6);
endlabel4461: ;
return 0;
}
/* MVMLE.L #<data>.W,(xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_48f9_6)(uae_u32 opcode)
{
{	uae_u16 mask = get_word_ce_prefetch (4);
{	uaecptr srca;
	srca = get_word_ce_prefetch (6) << 16;
	srca |= get_word_ce_prefetch (8);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 4, srca);
		goto endlabel4462;
	}
{{	uae_u16 dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
	while (dmask) { put_word_ce (srca, m68k_dreg(regs, movem_index1[dmask]) >> 16); put_word_ce (srca + 2, m68k_dreg(regs, movem_index1[dmask])); srca += 4; dmask = movem_next[dmask]; }
	while (amask) { put_word_ce (srca, m68k_areg(regs, movem_index1[amask]) >> 16); put_word_ce (srca + 2, m68k_areg(regs, movem_index1[amask])); srca += 4; amask = movem_next[amask]; }
	regs.ir = regs.irc;
	get_word_ce_prefetch (10);
}}}}	m68k_incpc (8);
endlabel4462: ;
return 0;
}
/* EXT.B Dn */
unsigned long REGPARAM2 CPUFUNC(op_49c0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s32 src = m68k_dreg(regs, srcreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 dst = (uae_s32)(uae_s8)src;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(dst)) == 0);
	SET_NFLG (((uae_s32)(dst)) < 0);
	m68k_dreg(regs, srcreg) = (dst);
}}}	m68k_incpc (2);
return 0;
}
/* TST.B Dn */
unsigned long REGPARAM2 CPUFUNC(op_4a00_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s8 src = m68k_dreg(regs, srcreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
}}	m68k_incpc (2);
return 0;
}
/* TST.B (An) */
unsigned long REGPARAM2 CPUFUNC(op_4a10_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
{	uae_s8 src = get_byte_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
}}}	m68k_incpc (2);
return 0;
}
/* TST.B (An)+ */
unsigned long REGPARAM2 CPUFUNC(op_4a18_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
{	uae_s8 src = get_byte_ce (srca);
	m68k_areg(regs, srcreg) += areg_byteinc[srcreg];
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
}}}	m68k_incpc (2);
return 0;
}
/* TST.B -(An) */
unsigned long REGPARAM2 CPUFUNC(op_4a20_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = get_byte_ce (srca);
	m68k_areg (regs, srcreg) = srca;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
}}}	m68k_incpc (2);
return 0;
}
/* TST.B (d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_4a28_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 src = get_byte_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
}}}	m68k_incpc (4);
return 0;
}
/* TST.B (d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_4a30_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
{	uae_s8 src = get_byte_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
}}}	m68k_incpc (4);
return 0;
}
/* TST.B (xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_4a38_6)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 src = get_byte_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
}}}	m68k_incpc (4);
return 0;
}
/* TST.B (xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_4a39_6)(uae_u32 opcode)
{
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
{	uae_s8 src = get_byte_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
}}}	m68k_incpc (6);
return 0;
}
/* TST.B (d16,PC) */
unsigned long REGPARAM2 CPUFUNC(op_4a3a_6)(uae_u32 opcode)
{
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 src = get_byte_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
}}}	m68k_incpc (4);
return 0;
}
/* TST.B (d8,PC,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_4a3b_6)(uae_u32 opcode)
{
{{	uaecptr tmppc;
	uaecptr srca;
	do_cycles (2 * 256);
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
{	uae_s8 src = get_byte_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
}}}	m68k_incpc (4);
return 0;
}
/* TST.B #<data>.B */
unsigned long REGPARAM2 CPUFUNC(op_4a3c_6)(uae_u32 opcode)
{
{{	uae_s8 src = (uae_u8)get_word_ce_prefetch (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
}}	m68k_incpc (4);
return 0;
}
/* TST.W Dn */
unsigned long REGPARAM2 CPUFUNC(op_4a40_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_dreg(regs, srcreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
}}	m68k_incpc (2);
return 0;
}
/* TST.W An */
unsigned long REGPARAM2 CPUFUNC(op_4a48_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_areg(regs, srcreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
}}	m68k_incpc (2);
return 0;
}
/* TST.W (An) */
unsigned long REGPARAM2 CPUFUNC(op_4a50_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4477;
	}
{{	uae_s16 src = get_word_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
}}}}	m68k_incpc (2);
endlabel4477: ;
return 0;
}
/* TST.W (An)+ */
unsigned long REGPARAM2 CPUFUNC(op_4a58_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4478;
	}
{{	uae_s16 src = get_word_ce (srca);
	m68k_areg(regs, srcreg) += 2;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
}}}}	m68k_incpc (2);
endlabel4478: ;
return 0;
}
/* TST.W -(An) */
unsigned long REGPARAM2 CPUFUNC(op_4a60_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = m68k_areg(regs, srcreg) - 2;
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4479;
	}
{{	uae_s16 src = get_word_ce (srca);
	m68k_areg (regs, srcreg) = srca;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
}}}}	m68k_incpc (2);
endlabel4479: ;
return 0;
}
/* TST.W (d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_4a68_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4480;
	}
{{	uae_s16 src = get_word_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
}}}}	m68k_incpc (4);
endlabel4480: ;
return 0;
}
/* TST.W (d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_4a70_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4481;
	}
{{	uae_s16 src = get_word_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
}}}}	m68k_incpc (4);
endlabel4481: ;
return 0;
}
/* TST.W (xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_4a78_6)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4482;
	}
{{	uae_s16 src = get_word_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
}}}}	m68k_incpc (4);
endlabel4482: ;
return 0;
}
/* TST.W (xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_4a79_6)(uae_u32 opcode)
{
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4483;
	}
{{	uae_s16 src = get_word_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
}}}}	m68k_incpc (6);
endlabel4483: ;
return 0;
}
/* TST.W (d16,PC) */
unsigned long REGPARAM2 CPUFUNC(op_4a7a_6)(uae_u32 opcode)
{
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4484;
	}
{{	uae_s16 src = get_word_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
}}}}	m68k_incpc (4);
endlabel4484: ;
return 0;
}
/* TST.W (d8,PC,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_4a7b_6)(uae_u32 opcode)
{
{{	uaecptr tmppc;
	uaecptr srca;
	do_cycles (2 * 256);
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4485;
	}
{{	uae_s16 src = get_word_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
}}}}	m68k_incpc (4);
endlabel4485: ;
return 0;
}
/* TST.W #<data>.W */
unsigned long REGPARAM2 CPUFUNC(op_4a7c_6)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce_prefetch (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
}}	m68k_incpc (4);
return 0;
}
/* TST.L Dn */
unsigned long REGPARAM2 CPUFUNC(op_4a80_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s32 src = m68k_dreg(regs, srcreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
}}	m68k_incpc (2);
return 0;
}
/* TST.L An */
unsigned long REGPARAM2 CPUFUNC(op_4a88_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s32 src = m68k_areg(regs, srcreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
}}	m68k_incpc (2);
return 0;
}
/* TST.L (An) */
unsigned long REGPARAM2 CPUFUNC(op_4a90_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4489;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
}}}}	m68k_incpc (2);
endlabel4489: ;
return 0;
}
/* TST.L (An)+ */
unsigned long REGPARAM2 CPUFUNC(op_4a98_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4490;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	m68k_areg(regs, srcreg) += 4;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
}}}}	m68k_incpc (2);
endlabel4490: ;
return 0;
}
/* TST.L -(An) */
unsigned long REGPARAM2 CPUFUNC(op_4aa0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = m68k_areg(regs, srcreg) - 4;
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4491;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	m68k_areg (regs, srcreg) = srca;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
}}}}	m68k_incpc (2);
endlabel4491: ;
return 0;
}
/* TST.L (d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_4aa8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4492;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
}}}}	m68k_incpc (4);
endlabel4492: ;
return 0;
}
/* TST.L (d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_4ab0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4493;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
}}}}	m68k_incpc (4);
endlabel4493: ;
return 0;
}
/* TST.L (xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_4ab8_6)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4494;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
}}}}	m68k_incpc (4);
endlabel4494: ;
return 0;
}
/* TST.L (xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_4ab9_6)(uae_u32 opcode)
{
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4495;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
}}}}	m68k_incpc (6);
endlabel4495: ;
return 0;
}
/* TST.L (d16,PC) */
unsigned long REGPARAM2 CPUFUNC(op_4aba_6)(uae_u32 opcode)
{
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4496;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
}}}}	m68k_incpc (4);
endlabel4496: ;
return 0;
}
/* TST.L (d8,PC,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_4abb_6)(uae_u32 opcode)
{
{{	uaecptr tmppc;
	uaecptr srca;
	do_cycles (2 * 256);
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4497;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
}}}}	m68k_incpc (4);
endlabel4497: ;
return 0;
}
/* TST.L #<data>.L */
unsigned long REGPARAM2 CPUFUNC(op_4abc_6)(uae_u32 opcode)
{
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
}}	m68k_incpc (6);
return 0;
}
/* TAS.B Dn */
unsigned long REGPARAM2 CPUFUNC(op_4ac0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s8 src = m68k_dreg(regs, srcreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	src |= 0x80;
	m68k_dreg(regs, srcreg) = (m68k_dreg(regs, srcreg) & ~0xff) | ((src) & 0xff);
}}	m68k_incpc (2);
return 0;
}
/* TAS.B (An) */
unsigned long REGPARAM2 CPUFUNC(op_4ad0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
{	uae_s8 src = get_byte_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	do_cycles (2 * 256);
	src |= 0x80;
	put_byte_ce (srca,src);
}}}	m68k_incpc (2);
return 0;
}
/* TAS.B (An)+ */
unsigned long REGPARAM2 CPUFUNC(op_4ad8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
{	uae_s8 src = get_byte_ce (srca);
	m68k_areg(regs, srcreg) += areg_byteinc[srcreg];
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	do_cycles (2 * 256);
	src |= 0x80;
	put_byte_ce (srca,src);
}}}	m68k_incpc (2);
return 0;
}
/* TAS.B -(An) */
unsigned long REGPARAM2 CPUFUNC(op_4ae0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = get_byte_ce (srca);
	m68k_areg (regs, srcreg) = srca;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	do_cycles (2 * 256);
	src |= 0x80;
	put_byte_ce (srca,src);
}}}	m68k_incpc (2);
return 0;
}
/* TAS.B (d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_4ae8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 src = get_byte_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	do_cycles (2 * 256);
	src |= 0x80;
	put_byte_ce (srca,src);
}}}	m68k_incpc (4);
return 0;
}
/* TAS.B (d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_4af0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
{	uae_s8 src = get_byte_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	do_cycles (2 * 256);
	src |= 0x80;
	put_byte_ce (srca,src);
}}}	m68k_incpc (4);
return 0;
}
/* TAS.B (xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_4af8_6)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 src = get_byte_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	do_cycles (2 * 256);
	src |= 0x80;
	put_byte_ce (srca,src);
}}}	m68k_incpc (4);
return 0;
}
/* TAS.B (xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_4af9_6)(uae_u32 opcode)
{
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
{	uae_s8 src = get_byte_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	do_cycles (2 * 256);
	src |= 0x80;
	put_byte_ce (srca,src);
}}}	m68k_incpc (6);
return 0;
}
/* MVMEL.W #<data>.W,(An) */
unsigned long REGPARAM2 CPUFUNC(op_4c90_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{	uae_u16 mask = get_word_ce_prefetch (4);
	unsigned int dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
	uae_u32 v;
{	uaecptr srca = m68k_areg(regs, dstreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 4, srca);
		goto endlabel4507;
	}
{{	while (dmask) { m68k_dreg(regs, movem_index1[dmask]) = (uae_s32)(uae_s16)get_word_ce(srca); srca += 2; dmask = movem_next[dmask]; }
	while (amask) { m68k_areg(regs, movem_index1[amask]) = (uae_s32)(uae_s16)get_word_ce(srca); srca += 2; amask = movem_next[amask]; }
	get_word_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
}}}}	m68k_incpc (4);
endlabel4507: ;
return 0;
}
/* MVMEL.W #<data>.W,(An)+ */
unsigned long REGPARAM2 CPUFUNC(op_4c98_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{	uae_u16 mask = get_word_ce_prefetch (4);
	unsigned int dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
	uae_u32 v;
{	uaecptr srca = m68k_areg(regs, dstreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 4, srca);
		goto endlabel4508;
	}
{{	while (dmask) { m68k_dreg(regs, movem_index1[dmask]) = (uae_s32)(uae_s16)get_word_ce(srca); srca += 2; dmask = movem_next[dmask]; }
	while (amask) { m68k_areg(regs, movem_index1[amask]) = (uae_s32)(uae_s16)get_word_ce(srca); srca += 2; amask = movem_next[amask]; }
	get_word_ce (srca);
	m68k_areg(regs, dstreg) = srca;
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
}}}}	m68k_incpc (4);
endlabel4508: ;
return 0;
}
/* MVMEL.W #<data>.W,(d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_4ca8_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{	uae_u16 mask = get_word_ce_prefetch (4);
	unsigned int dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
	uae_u32 v;
{	uaecptr srca = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 4, srca);
		goto endlabel4509;
	}
{{	while (dmask) { m68k_dreg(regs, movem_index1[dmask]) = (uae_s32)(uae_s16)get_word_ce(srca); srca += 2; dmask = movem_next[dmask]; }
	while (amask) { m68k_areg(regs, movem_index1[amask]) = (uae_s32)(uae_s16)get_word_ce(srca); srca += 2; amask = movem_next[amask]; }
	get_word_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
}}}}	m68k_incpc (6);
endlabel4509: ;
return 0;
}
/* MVMEL.W #<data>.W,(d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_4cb0_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{	uae_u16 mask = get_word_ce_prefetch (4);
	unsigned int dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
	uae_u32 v;
{	uaecptr srca;
	srca = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (6));
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 4, srca);
		goto endlabel4510;
	}
{	do_cycles (2 * 256);
{	while (dmask) { m68k_dreg(regs, movem_index1[dmask]) = (uae_s32)(uae_s16)get_word_ce(srca); srca += 2; dmask = movem_next[dmask]; }
	while (amask) { m68k_areg(regs, movem_index1[amask]) = (uae_s32)(uae_s16)get_word_ce(srca); srca += 2; amask = movem_next[amask]; }
	get_word_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
}}}}	m68k_incpc (6);
endlabel4510: ;
return 0;
}
/* MVMEL.W #<data>.W,(xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_4cb8_6)(uae_u32 opcode)
{
{	uae_u16 mask = get_word_ce_prefetch (4);
	unsigned int dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
	uae_u32 v;
{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 4, srca);
		goto endlabel4511;
	}
{{	while (dmask) { m68k_dreg(regs, movem_index1[dmask]) = (uae_s32)(uae_s16)get_word_ce(srca); srca += 2; dmask = movem_next[dmask]; }
	while (amask) { m68k_areg(regs, movem_index1[amask]) = (uae_s32)(uae_s16)get_word_ce(srca); srca += 2; amask = movem_next[amask]; }
	get_word_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
}}}}	m68k_incpc (6);
endlabel4511: ;
return 0;
}
/* MVMEL.W #<data>.W,(xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_4cb9_6)(uae_u32 opcode)
{
{	uae_u16 mask = get_word_ce_prefetch (4);
	unsigned int dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
	uae_u32 v;
{	uaecptr srca;
	srca = get_word_ce_prefetch (6) << 16;
	srca |= get_word_ce_prefetch (8);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 4, srca);
		goto endlabel4512;
	}
{{	while (dmask) { m68k_dreg(regs, movem_index1[dmask]) = (uae_s32)(uae_s16)get_word_ce(srca); srca += 2; dmask = movem_next[dmask]; }
	while (amask) { m68k_areg(regs, movem_index1[amask]) = (uae_s32)(uae_s16)get_word_ce(srca); srca += 2; amask = movem_next[amask]; }
	get_word_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (10);
}}}}	m68k_incpc (8);
endlabel4512: ;
return 0;
}
/* MVMEL.W #<data>.W,(d16,PC) */
unsigned long REGPARAM2 CPUFUNC(op_4cba_6)(uae_u32 opcode)
{
	uae_u32 dstreg = 2;
{	uae_u16 mask = get_word_ce_prefetch (4);
	unsigned int dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
	uae_u32 v;
{	uaecptr srca = m68k_getpc () + 4;
	srca += (uae_s32)(uae_s16)get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 4, srca);
		goto endlabel4513;
	}
{{	while (dmask) { m68k_dreg(regs, movem_index1[dmask]) = (uae_s32)(uae_s16)get_word_ce(srca); srca += 2; dmask = movem_next[dmask]; }
	while (amask) { m68k_areg(regs, movem_index1[amask]) = (uae_s32)(uae_s16)get_word_ce(srca); srca += 2; amask = movem_next[amask]; }
	get_word_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
}}}}	m68k_incpc (6);
endlabel4513: ;
return 0;
}
/* MVMEL.W #<data>.W,(d8,PC,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_4cbb_6)(uae_u32 opcode)
{
	uae_u32 dstreg = 3;
{	uae_u16 mask = get_word_ce_prefetch (4);
	unsigned int dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
	uae_u32 v;
{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc() + 4;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (6));
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 4, srca);
		goto endlabel4514;
	}
{	do_cycles (2 * 256);
{	while (dmask) { m68k_dreg(regs, movem_index1[dmask]) = (uae_s32)(uae_s16)get_word_ce(srca); srca += 2; dmask = movem_next[dmask]; }
	while (amask) { m68k_areg(regs, movem_index1[amask]) = (uae_s32)(uae_s16)get_word_ce(srca); srca += 2; amask = movem_next[amask]; }
	get_word_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
}}}}	m68k_incpc (6);
endlabel4514: ;
return 0;
}
/* MVMEL.L #<data>.W,(An) */
unsigned long REGPARAM2 CPUFUNC(op_4cd0_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{	uae_u16 mask = get_word_ce_prefetch (4);
	unsigned int dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
	uae_u32 v;
{	uaecptr srca = m68k_areg(regs, dstreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 4, srca);
		goto endlabel4515;
	}
{{	while (dmask) { v = get_word_ce(srca) << 16; v |= get_word_ce(srca + 2); m68k_dreg(regs, movem_index1[dmask]) = v; srca += 4; dmask = movem_next[dmask]; }
	while (amask) { v = get_word_ce(srca) << 16; v |= get_word_ce(srca + 2); m68k_areg(regs, movem_index1[amask]) = v; srca += 4; amask = movem_next[amask]; }
	get_word_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
}}}}	m68k_incpc (4);
endlabel4515: ;
return 0;
}
/* MVMEL.L #<data>.W,(An)+ */
unsigned long REGPARAM2 CPUFUNC(op_4cd8_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{	uae_u16 mask = get_word_ce_prefetch (4);
	unsigned int dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
	uae_u32 v;
{	uaecptr srca = m68k_areg(regs, dstreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 4, srca);
		goto endlabel4516;
	}
{{	while (dmask) { v = get_word_ce(srca) << 16; v |= get_word_ce(srca + 2); m68k_dreg(regs, movem_index1[dmask]) = v; srca += 4; dmask = movem_next[dmask]; }
	while (amask) { v = get_word_ce(srca) << 16; v |= get_word_ce(srca + 2); m68k_areg(regs, movem_index1[amask]) = v; srca += 4; amask = movem_next[amask]; }
	get_word_ce (srca);
	m68k_areg(regs, dstreg) = srca;
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
}}}}	m68k_incpc (4);
endlabel4516: ;
return 0;
}
/* MVMEL.L #<data>.W,(d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_4ce8_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{	uae_u16 mask = get_word_ce_prefetch (4);
	unsigned int dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
	uae_u32 v;
{	uaecptr srca = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 4, srca);
		goto endlabel4517;
	}
{{	while (dmask) { v = get_word_ce(srca) << 16; v |= get_word_ce(srca + 2); m68k_dreg(regs, movem_index1[dmask]) = v; srca += 4; dmask = movem_next[dmask]; }
	while (amask) { v = get_word_ce(srca) << 16; v |= get_word_ce(srca + 2); m68k_areg(regs, movem_index1[amask]) = v; srca += 4; amask = movem_next[amask]; }
	get_word_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
}}}}	m68k_incpc (6);
endlabel4517: ;
return 0;
}
/* MVMEL.L #<data>.W,(d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_4cf0_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{	uae_u16 mask = get_word_ce_prefetch (4);
	unsigned int dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
	uae_u32 v;
{	uaecptr srca;
	srca = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (6));
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 4, srca);
		goto endlabel4518;
	}
{	do_cycles (2 * 256);
{	while (dmask) { v = get_word_ce(srca) << 16; v |= get_word_ce(srca + 2); m68k_dreg(regs, movem_index1[dmask]) = v; srca += 4; dmask = movem_next[dmask]; }
	while (amask) { v = get_word_ce(srca) << 16; v |= get_word_ce(srca + 2); m68k_areg(regs, movem_index1[amask]) = v; srca += 4; amask = movem_next[amask]; }
	get_word_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
}}}}	m68k_incpc (6);
endlabel4518: ;
return 0;
}
/* MVMEL.L #<data>.W,(xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_4cf8_6)(uae_u32 opcode)
{
{	uae_u16 mask = get_word_ce_prefetch (4);
	unsigned int dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
	uae_u32 v;
{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 4, srca);
		goto endlabel4519;
	}
{{	while (dmask) { v = get_word_ce(srca) << 16; v |= get_word_ce(srca + 2); m68k_dreg(regs, movem_index1[dmask]) = v; srca += 4; dmask = movem_next[dmask]; }
	while (amask) { v = get_word_ce(srca) << 16; v |= get_word_ce(srca + 2); m68k_areg(regs, movem_index1[amask]) = v; srca += 4; amask = movem_next[amask]; }
	get_word_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
}}}}	m68k_incpc (6);
endlabel4519: ;
return 0;
}
/* MVMEL.L #<data>.W,(xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_4cf9_6)(uae_u32 opcode)
{
{	uae_u16 mask = get_word_ce_prefetch (4);
	unsigned int dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
	uae_u32 v;
{	uaecptr srca;
	srca = get_word_ce_prefetch (6) << 16;
	srca |= get_word_ce_prefetch (8);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 4, srca);
		goto endlabel4520;
	}
{{	while (dmask) { v = get_word_ce(srca) << 16; v |= get_word_ce(srca + 2); m68k_dreg(regs, movem_index1[dmask]) = v; srca += 4; dmask = movem_next[dmask]; }
	while (amask) { v = get_word_ce(srca) << 16; v |= get_word_ce(srca + 2); m68k_areg(regs, movem_index1[amask]) = v; srca += 4; amask = movem_next[amask]; }
	get_word_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (10);
}}}}	m68k_incpc (8);
endlabel4520: ;
return 0;
}
/* MVMEL.L #<data>.W,(d16,PC) */
unsigned long REGPARAM2 CPUFUNC(op_4cfa_6)(uae_u32 opcode)
{
	uae_u32 dstreg = 2;
{	uae_u16 mask = get_word_ce_prefetch (4);
	unsigned int dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
	uae_u32 v;
{	uaecptr srca = m68k_getpc () + 4;
	srca += (uae_s32)(uae_s16)get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 4, srca);
		goto endlabel4521;
	}
{{	while (dmask) { v = get_word_ce(srca) << 16; v |= get_word_ce(srca + 2); m68k_dreg(regs, movem_index1[dmask]) = v; srca += 4; dmask = movem_next[dmask]; }
	while (amask) { v = get_word_ce(srca) << 16; v |= get_word_ce(srca + 2); m68k_areg(regs, movem_index1[amask]) = v; srca += 4; amask = movem_next[amask]; }
	get_word_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
}}}}	m68k_incpc (6);
endlabel4521: ;
return 0;
}
/* MVMEL.L #<data>.W,(d8,PC,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_4cfb_6)(uae_u32 opcode)
{
	uae_u32 dstreg = 3;
{	uae_u16 mask = get_word_ce_prefetch (4);
	unsigned int dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
	uae_u32 v;
{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc() + 4;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (6));
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 4, srca);
		goto endlabel4522;
	}
{	do_cycles (2 * 256);
{	while (dmask) { v = get_word_ce(srca) << 16; v |= get_word_ce(srca + 2); m68k_dreg(regs, movem_index1[dmask]) = v; srca += 4; dmask = movem_next[dmask]; }
	while (amask) { v = get_word_ce(srca) << 16; v |= get_word_ce(srca + 2); m68k_areg(regs, movem_index1[amask]) = v; srca += 4; amask = movem_next[amask]; }
	get_word_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
}}}}	m68k_incpc (6);
endlabel4522: ;
return 0;
}
/* TRAP.L #<data> */
unsigned long REGPARAM2 CPUFUNC(op_4e40_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 15);
{{	uae_u32 src = srcreg;
	m68k_incpc (2);
	Exception (src + 32,0);
}}return 0;
}
/* LINK.W An,#<data>.W */
unsigned long REGPARAM2 CPUFUNC(op_4e50_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr olda;
	olda = m68k_areg(regs, 7) - 4;
	if (olda & 1) {
		exception3 (opcode, m68k_getpc() + 2, olda);
		goto endlabel4524;
	}
{	m68k_areg (regs, 7) = olda;
{	uae_s32 src = m68k_areg(regs, srcreg);
	put_word_ce (olda, src >> 16); put_word_ce (olda + 2, src);
	m68k_areg(regs, srcreg) = (m68k_areg(regs, 7));
{	uae_s16 offs = get_word_ce_prefetch (4);
	m68k_areg(regs, 7) += offs;
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
}}}}}	m68k_incpc (4);
endlabel4524: ;
return 0;
}
/* UNLK.L An */
unsigned long REGPARAM2 CPUFUNC(op_4e58_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s32 src = m68k_areg(regs, srcreg);
	m68k_areg(regs, 7) = src;
{	uaecptr olda = m68k_areg(regs, 7);
	if (olda & 1) {
		exception3 (opcode, m68k_getpc() + 2, olda);
		goto endlabel4525;
	}
{{	uae_s32 old = get_word_ce (olda) << 16; old |= get_word_ce (olda + 2);
	m68k_areg(regs, 7) += 4;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	m68k_areg(regs, srcreg) = (old);
}}}}}	m68k_incpc (2);
endlabel4525: ;
return 0;
}
/* MVR2USP.L An */
unsigned long REGPARAM2 CPUFUNC(op_4e60_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception(8,0); goto endlabel4526; }
{{	uae_s32 src = m68k_areg(regs, srcreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	regs.usp = src;
}}}	m68k_incpc (2);
endlabel4526: ;
return 0;
}
/* MVUSP2R.L An */
unsigned long REGPARAM2 CPUFUNC(op_4e68_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception(8,0); goto endlabel4527; }
{{	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	m68k_areg(regs, srcreg) = (regs.usp);
}}}	m68k_incpc (2);
endlabel4527: ;
return 0;
}
/* RESET.L  */
unsigned long REGPARAM2 CPUFUNC(op_4e70_6)(uae_u32 opcode)
{
{if (!regs.s) { Exception(8,0); goto endlabel4528; }
{	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	cpureset();
}}	m68k_incpc (2);
endlabel4528: ;
return 0;
}
/* NOP.L  */
unsigned long REGPARAM2 CPUFUNC(op_4e71_6)(uae_u32 opcode)
{
{	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
}	m68k_incpc (2);
return 0;
}
/* STOP.L #<data>.W */
unsigned long REGPARAM2 CPUFUNC(op_4e72_6)(uae_u32 opcode)
{
{if (!regs.s) { Exception(8,0); goto endlabel4530; }
{{	uae_s16 src = get_word_ce_prefetch (4);
	regs.sr = src;
	MakeFromSR();
	m68k_setstopped(1);
	m68k_incpc (4);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}endlabel4530: ;
return 0;
}
/* RTE.L  */
unsigned long REGPARAM2 CPUFUNC(op_4e73_6)(uae_u32 opcode)
{
{if (!regs.s) { Exception(8,0); goto endlabel4531; }
{{	uaecptr sra = m68k_areg(regs, 7);
	if (sra & 1) {
		exception3 (opcode, m68k_getpc() + 2, sra);
		goto endlabel4531;
	}
{{	uae_s16 sr = get_word_ce (sra);
	m68k_areg(regs, 7) += 2;
{	uaecptr pca = m68k_areg(regs, 7);
	if (pca & 1) {
		exception3 (opcode, m68k_getpc() + 2, pca);
		goto endlabel4531;
	}
{{	uae_s32 pc = get_word_ce (pca) << 16; pc |= get_word_ce (pca + 2);
	m68k_areg(regs, 7) += 4;
	regs.sr = sr; m68k_setpc_rte(pc);
	MakeFromSR();
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}}}endlabel4531: ;
return 0;
}
/* RTD.L #<data>.W */
unsigned long REGPARAM2 CPUFUNC(op_4e74_6)(uae_u32 opcode)
{
{	compiler_flush_jsr_stack();
{	uaecptr pca = m68k_areg(regs, 7);
	if (pca & 1) {
		exception3 (opcode, m68k_getpc() + 2, pca);
		goto endlabel4532;
	}
{{	uae_s32 pc = get_word_ce (pca) << 16; pc |= get_word_ce (pca + 2);
	m68k_areg(regs, 7) += 4;
{	uae_s16 offs = get_word_ce_prefetch (4);
	m68k_areg(regs, 7) += offs;
	m68k_setpc_rte(pc);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}endlabel4532: ;
return 0;
}
/* RTS.L  */
unsigned long REGPARAM2 CPUFUNC(op_4e75_6)(uae_u32 opcode)
{
{	m68k_do_rts_ce();
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}return 0;
}
/* TRAPV.L  */
unsigned long REGPARAM2 CPUFUNC(op_4e76_6)(uae_u32 opcode)
{
{	m68k_incpc (2);
	if (GET_VFLG) {
		Exception (7, m68k_getpc ());
		goto endlabel4534;
	}
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}endlabel4534: ;
return 0;
}
/* RTR.L  */
unsigned long REGPARAM2 CPUFUNC(op_4e77_6)(uae_u32 opcode)
{
{	compiler_flush_jsr_stack();
	MakeSR();
{	uaecptr sra = m68k_areg(regs, 7);
	if (sra & 1) {
		exception3 (opcode, m68k_getpc() + 2, sra);
		goto endlabel4535;
	}
{{	uae_s16 sr = get_word_ce (sra);
	m68k_areg(regs, 7) += 2;
{	uaecptr pca = m68k_areg(regs, 7);
	if (pca & 1) {
		exception3 (opcode, m68k_getpc() + 2, pca);
		goto endlabel4535;
	}
{{	uae_s32 pc = get_word_ce (pca) << 16; pc |= get_word_ce (pca + 2);
	m68k_areg(regs, 7) += 4;
	regs.sr &= 0xFF00; sr &= 0xFF;
	regs.sr |= sr; m68k_setpc(pc);
	MakeFromSR();
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}}endlabel4535: ;
return 0;
}
/* JSR.L (An) */
unsigned long REGPARAM2 CPUFUNC(op_4e90_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 6, srca);
		goto endlabel4536;
	}
	m68k_do_jsr_ce(m68k_getpc() + 2, srca);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4536: ;
return 0;
}
/* JSR.L (d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_4ea8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)regs.irc;
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 6, srca);
		goto endlabel4537;
	}
	m68k_do_jsr_ce(m68k_getpc() + 4, srca);
	do_cycles (2 * 256);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4537: ;
return 0;
}
/* JSR.L (d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_4eb0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), regs.irc);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 6, srca);
		goto endlabel4538;
	}
	m68k_do_jsr_ce(m68k_getpc() + 4, srca);
	do_cycles (2 * 256);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4538: ;
return 0;
}
/* JSR.L (xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_4eb8_6)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)regs.irc;
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 6, srca);
		goto endlabel4539;
	}
	m68k_do_jsr_ce(m68k_getpc() + 4, srca);
	do_cycles (2 * 256);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4539: ;
return 0;
}
/* JSR.L (xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_4eb9_6)(uae_u32 opcode)
{
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= regs.irc;
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 6, srca);
		goto endlabel4540;
	}
	m68k_do_jsr_ce(m68k_getpc() + 6, srca);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4540: ;
return 0;
}
/* JSR.L (d16,PC) */
unsigned long REGPARAM2 CPUFUNC(op_4eba_6)(uae_u32 opcode)
{
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)regs.irc;
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 6, srca);
		goto endlabel4541;
	}
	m68k_do_jsr_ce(m68k_getpc() + 4, srca);
	do_cycles (2 * 256);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4541: ;
return 0;
}
/* JSR.L (d8,PC,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_4ebb_6)(uae_u32 opcode)
{
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, regs.irc);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 6, srca);
		goto endlabel4542;
	}
	m68k_do_jsr_ce(m68k_getpc() + 4, srca);
	do_cycles (2 * 256);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4542: ;
return 0;
}
/* JMP.L (An) */
unsigned long REGPARAM2 CPUFUNC(op_4ed0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 6, srca);
		goto endlabel4543;
	}
	m68k_setpc(srca);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4543: ;
return 0;
}
/* JMP.L (d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_4ee8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)regs.irc;
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 6, srca);
		goto endlabel4544;
	}
	do_cycles (2 * 256);
	m68k_setpc(srca);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4544: ;
return 0;
}
/* JMP.L (d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_4ef0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), regs.irc);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 6, srca);
		goto endlabel4545;
	}
	do_cycles (2 * 256);
	m68k_setpc(srca);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4545: ;
return 0;
}
/* JMP.L (xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_4ef8_6)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)regs.irc;
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 6, srca);
		goto endlabel4546;
	}
	do_cycles (2 * 256);
	m68k_setpc(srca);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4546: ;
return 0;
}
/* JMP.L (xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_4ef9_6)(uae_u32 opcode)
{
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= regs.irc;
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 6, srca);
		goto endlabel4547;
	}
	m68k_setpc(srca);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4547: ;
return 0;
}
/* JMP.L (d16,PC) */
unsigned long REGPARAM2 CPUFUNC(op_4efa_6)(uae_u32 opcode)
{
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)regs.irc;
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 6, srca);
		goto endlabel4548;
	}
	do_cycles (2 * 256);
	m68k_setpc(srca);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4548: ;
return 0;
}
/* JMP.L (d8,PC,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_4efb_6)(uae_u32 opcode)
{
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, regs.irc);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 6, srca);
		goto endlabel4549;
	}
	do_cycles (2 * 256);
	m68k_setpc(srca);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4549: ;
return 0;
}
/* ADD.B #<data>,Dn */
unsigned long REGPARAM2 CPUFUNC(op_5000_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uae_s8 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xff) | ((newv) & 0xff);
}}}}}}	m68k_incpc (2);
return 0;
}
/* ADD.B #<data>,(An) */
unsigned long REGPARAM2 CPUFUNC(op_5010_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta = m68k_areg(regs, dstreg);
{	uae_s8 dst = get_byte_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_byte_ce (dsta,newv);
}}}}}}}	m68k_incpc (2);
return 0;
}
/* ADD.B #<data>,(An)+ */
unsigned long REGPARAM2 CPUFUNC(op_5018_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta = m68k_areg(regs, dstreg);
{	uae_s8 dst = get_byte_ce (dsta);
	m68k_areg(regs, dstreg) += areg_byteinc[dstreg];
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_byte_ce (dsta,newv);
}}}}}}}	m68k_incpc (2);
return 0;
}
/* ADD.B #<data>,-(An) */
unsigned long REGPARAM2 CPUFUNC(op_5020_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = m68k_areg(regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = get_byte_ce (dsta);
	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_byte_ce (dsta,newv);
}}}}}}}	m68k_incpc (2);
return 0;
}
/* ADD.B #<data>,(d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_5028_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 dst = get_byte_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_byte_ce (dsta,newv);
}}}}}}}	m68k_incpc (4);
return 0;
}
/* ADD.B #<data>,(d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_5030_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (4));
{	uae_s8 dst = get_byte_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_byte_ce (dsta,newv);
}}}}}}}	m68k_incpc (4);
return 0;
}
/* ADD.B #<data>,(xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_5038_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
{{	uae_u32 src = srcreg;
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 dst = get_byte_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_byte_ce (dsta,newv);
}}}}}}}	m68k_incpc (4);
return 0;
}
#endif

#ifdef PART_5
/* ADD.B #<data>,(xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_5039_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
{{	uae_u32 src = srcreg;
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (4) << 16;
	dsta |= get_word_ce_prefetch (6);
{	uae_s8 dst = get_byte_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_byte_ce (dsta,newv);
}}}}}}}	m68k_incpc (6);
return 0;
}
/* ADD.W #<data>,Dn */
unsigned long REGPARAM2 CPUFUNC(op_5040_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xffff) | ((newv) & 0xffff);
}}}}}}	m68k_incpc (2);
return 0;
}
/* ADDA.W #<data>,An */
unsigned long REGPARAM2 CPUFUNC(op_5048_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uae_s32 dst = m68k_areg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	do_cycles (2 * 256);
{	uae_u32 newv = dst + src;
	m68k_areg(regs, dstreg) = (newv);
}}}}	m68k_incpc (2);
return 0;
}
/* ADD.W #<data>,(An) */
unsigned long REGPARAM2 CPUFUNC(op_5050_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4560;
	}
{{	uae_s16 dst = get_word_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_word_ce (dsta,newv);
}}}}}}}}	m68k_incpc (2);
endlabel4560: ;
return 0;
}
/* ADD.W #<data>,(An)+ */
unsigned long REGPARAM2 CPUFUNC(op_5058_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4561;
	}
{{	uae_s16 dst = get_word_ce (dsta);
	m68k_areg(regs, dstreg) += 2;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_word_ce (dsta,newv);
}}}}}}}}	m68k_incpc (2);
endlabel4561: ;
return 0;
}
/* ADD.W #<data>,-(An) */
unsigned long REGPARAM2 CPUFUNC(op_5060_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = m68k_areg(regs, dstreg) - 2;
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4562;
	}
{{	uae_s16 dst = get_word_ce (dsta);
	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_word_ce (dsta,newv);
}}}}}}}}	m68k_incpc (2);
endlabel4562: ;
return 0;
}
/* ADD.W #<data>,(d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_5068_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4563;
	}
{{	uae_s16 dst = get_word_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_word_ce (dsta,newv);
}}}}}}}}	m68k_incpc (4);
endlabel4563: ;
return 0;
}
/* ADD.W #<data>,(d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_5070_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (4));
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4564;
	}
{{	uae_s16 dst = get_word_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_word_ce (dsta,newv);
}}}}}}}}	m68k_incpc (4);
endlabel4564: ;
return 0;
}
/* ADD.W #<data>,(xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_5078_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
{{	uae_u32 src = srcreg;
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4565;
	}
{{	uae_s16 dst = get_word_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_word_ce (dsta,newv);
}}}}}}}}	m68k_incpc (4);
endlabel4565: ;
return 0;
}
/* ADD.W #<data>,(xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_5079_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
{{	uae_u32 src = srcreg;
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (4) << 16;
	dsta |= get_word_ce_prefetch (6);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4566;
	}
{{	uae_s16 dst = get_word_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_word_ce (dsta,newv);
}}}}}}}}	m68k_incpc (6);
endlabel4566: ;
return 0;
}
/* ADD.L #<data>,Dn */
unsigned long REGPARAM2 CPUFUNC(op_5080_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	do_cycles (4 * 256);
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	m68k_dreg(regs, dstreg) = (newv);
}}}}}}	m68k_incpc (2);
return 0;
}
/* ADDA.L #<data>,An */
unsigned long REGPARAM2 CPUFUNC(op_5088_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uae_s32 dst = m68k_areg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	do_cycles (2 * 256);
{	uae_u32 newv = dst + src;
	m68k_areg(regs, dstreg) = (newv);
}}}}	m68k_incpc (2);
return 0;
}
/* ADD.L #<data>,(An) */
unsigned long REGPARAM2 CPUFUNC(op_5090_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4569;
	}
{{	uae_s32 dst = get_word_ce (dsta) << 16; dst |= get_word_ce (dsta + 2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_word_ce (dsta, newv >> 16); put_word_ce (dsta + 2, newv);
}}}}}}}}	m68k_incpc (2);
endlabel4569: ;
return 0;
}
/* ADD.L #<data>,(An)+ */
unsigned long REGPARAM2 CPUFUNC(op_5098_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4570;
	}
{{	uae_s32 dst = get_word_ce (dsta) << 16; dst |= get_word_ce (dsta + 2);
	m68k_areg(regs, dstreg) += 4;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_word_ce (dsta, newv >> 16); put_word_ce (dsta + 2, newv);
}}}}}}}}	m68k_incpc (2);
endlabel4570: ;
return 0;
}
/* ADD.L #<data>,-(An) */
unsigned long REGPARAM2 CPUFUNC(op_50a0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = m68k_areg(regs, dstreg) - 4;
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4571;
	}
{{	uae_s32 dst = get_word_ce (dsta) << 16; dst |= get_word_ce (dsta + 2);
	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_word_ce (dsta, newv >> 16); put_word_ce (dsta + 2, newv);
}}}}}}}}	m68k_incpc (2);
endlabel4571: ;
return 0;
}
/* ADD.L #<data>,(d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_50a8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4572;
	}
{{	uae_s32 dst = get_word_ce (dsta) << 16; dst |= get_word_ce (dsta + 2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_word_ce (dsta, newv >> 16); put_word_ce (dsta + 2, newv);
}}}}}}}}	m68k_incpc (4);
endlabel4572: ;
return 0;
}
/* ADD.L #<data>,(d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_50b0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (4));
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4573;
	}
{{	uae_s32 dst = get_word_ce (dsta) << 16; dst |= get_word_ce (dsta + 2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_word_ce (dsta, newv >> 16); put_word_ce (dsta + 2, newv);
}}}}}}}}	m68k_incpc (4);
endlabel4573: ;
return 0;
}
/* ADD.L #<data>,(xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_50b8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
{{	uae_u32 src = srcreg;
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4574;
	}
{{	uae_s32 dst = get_word_ce (dsta) << 16; dst |= get_word_ce (dsta + 2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_word_ce (dsta, newv >> 16); put_word_ce (dsta + 2, newv);
}}}}}}}}	m68k_incpc (4);
endlabel4574: ;
return 0;
}
/* ADD.L #<data>,(xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_50b9_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
{{	uae_u32 src = srcreg;
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (4) << 16;
	dsta |= get_word_ce_prefetch (6);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4575;
	}
{{	uae_s32 dst = get_word_ce (dsta) << 16; dst |= get_word_ce (dsta + 2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_word_ce (dsta, newv >> 16); put_word_ce (dsta + 2, newv);
}}}}}}}}	m68k_incpc (6);
endlabel4575: ;
return 0;
}
/* Scc.B Dn */
unsigned long REGPARAM2 CPUFUNC(op_50c0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(0) ? 0xff : 0;
	if (val)
		do_cycles (2 * 256);
	m68k_dreg(regs, srcreg) = (m68k_dreg(regs, srcreg) & ~0xff) | ((val) & 0xff);
}}}	m68k_incpc (2);
return 0;
}
/* DBcc.W Dn,#<data>.W */
unsigned long REGPARAM2 CPUFUNC(op_50c8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uae_s16 offs = regs.irc;
	uaecptr oldpc = m68k_getpc();
	do_cycles (2 * 256);
	if (!cctrue(0)) {
		m68k_incpc((uae_s32)offs + 2);
		get_word_ce_prefetch (0);
		m68k_dreg(regs, srcreg) = (m68k_dreg(regs, srcreg) & ~0xffff) | (((src-1)) & 0xffff);
		if (src) {
			if (offs & 1) {
				exception3 (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)offs + 2);
				goto endlabel4577;
			}
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
			return 0;
		}
	} else {
		do_cycles (2 * 256);
	}
	m68k_setpc (oldpc + 4);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}endlabel4577: ;
return 0;
}
/* Scc.B (An) */
unsigned long REGPARAM2 CPUFUNC(op_50d0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(0) ? 0xff : 0;
	put_byte_ce (srca,val);
}}}	m68k_incpc (2);
return 0;
}
/* Scc.B (An)+ */
unsigned long REGPARAM2 CPUFUNC(op_50d8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
	m68k_areg(regs, srcreg) += areg_byteinc[srcreg];
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(0) ? 0xff : 0;
	put_byte_ce (srca,val);
}}}	m68k_incpc (2);
return 0;
}
/* Scc.B -(An) */
unsigned long REGPARAM2 CPUFUNC(op_50e0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
	m68k_areg (regs, srcreg) = srca;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(0) ? 0xff : 0;
	put_byte_ce (srca,val);
}}}	m68k_incpc (2);
return 0;
}
/* Scc.B (d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_50e8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	int val = cctrue(0) ? 0xff : 0;
	put_byte_ce (srca,val);
}}}	m68k_incpc (4);
return 0;
}
/* Scc.B (d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_50f0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	int val = cctrue(0) ? 0xff : 0;
	put_byte_ce (srca,val);
}}}	m68k_incpc (4);
return 0;
}
/* Scc.B (xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_50f8_6)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	int val = cctrue(0) ? 0xff : 0;
	put_byte_ce (srca,val);
}}}	m68k_incpc (4);
return 0;
}
/* Scc.B (xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_50f9_6)(uae_u32 opcode)
{
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
{	int val = cctrue(0) ? 0xff : 0;
	put_byte_ce (srca,val);
}}}	m68k_incpc (6);
return 0;
}
/* SUB.B #<data>,Dn */
unsigned long REGPARAM2 CPUFUNC(op_5100_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uae_s8 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xff) | ((newv) & 0xff);
}}}}}}	m68k_incpc (2);
return 0;
}
/* SUB.B #<data>,(An) */
unsigned long REGPARAM2 CPUFUNC(op_5110_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta = m68k_areg(regs, dstreg);
{	uae_s8 dst = get_byte_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_byte_ce (dsta,newv);
}}}}}}}	m68k_incpc (2);
return 0;
}
/* SUB.B #<data>,(An)+ */
unsigned long REGPARAM2 CPUFUNC(op_5118_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta = m68k_areg(regs, dstreg);
{	uae_s8 dst = get_byte_ce (dsta);
	m68k_areg(regs, dstreg) += areg_byteinc[dstreg];
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_byte_ce (dsta,newv);
}}}}}}}	m68k_incpc (2);
return 0;
}
/* SUB.B #<data>,-(An) */
unsigned long REGPARAM2 CPUFUNC(op_5120_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = m68k_areg(regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = get_byte_ce (dsta);
	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_byte_ce (dsta,newv);
}}}}}}}	m68k_incpc (2);
return 0;
}
/* SUB.B #<data>,(d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_5128_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 dst = get_byte_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_byte_ce (dsta,newv);
}}}}}}}	m68k_incpc (4);
return 0;
}
/* SUB.B #<data>,(d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_5130_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (4));
{	uae_s8 dst = get_byte_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_byte_ce (dsta,newv);
}}}}}}}	m68k_incpc (4);
return 0;
}
/* SUB.B #<data>,(xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_5138_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
{{	uae_u32 src = srcreg;
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 dst = get_byte_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_byte_ce (dsta,newv);
}}}}}}}	m68k_incpc (4);
return 0;
}
/* SUB.B #<data>,(xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_5139_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
{{	uae_u32 src = srcreg;
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (4) << 16;
	dsta |= get_word_ce_prefetch (6);
{	uae_s8 dst = get_byte_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_byte_ce (dsta,newv);
}}}}}}}	m68k_incpc (6);
return 0;
}
/* SUB.W #<data>,Dn */
unsigned long REGPARAM2 CPUFUNC(op_5140_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xffff) | ((newv) & 0xffff);
}}}}}}	m68k_incpc (2);
return 0;
}
/* SUBA.W #<data>,An */
unsigned long REGPARAM2 CPUFUNC(op_5148_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uae_s32 dst = m68k_areg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	do_cycles (2 * 256);
{	uae_u32 newv = dst - src;
	m68k_areg(regs, dstreg) = (newv);
}}}}	m68k_incpc (2);
return 0;
}
/* SUB.W #<data>,(An) */
unsigned long REGPARAM2 CPUFUNC(op_5150_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4595;
	}
{{	uae_s16 dst = get_word_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_word_ce (dsta,newv);
}}}}}}}}	m68k_incpc (2);
endlabel4595: ;
return 0;
}
/* SUB.W #<data>,(An)+ */
unsigned long REGPARAM2 CPUFUNC(op_5158_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4596;
	}
{{	uae_s16 dst = get_word_ce (dsta);
	m68k_areg(regs, dstreg) += 2;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_word_ce (dsta,newv);
}}}}}}}}	m68k_incpc (2);
endlabel4596: ;
return 0;
}
/* SUB.W #<data>,-(An) */
unsigned long REGPARAM2 CPUFUNC(op_5160_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = m68k_areg(regs, dstreg) - 2;
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4597;
	}
{{	uae_s16 dst = get_word_ce (dsta);
	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_word_ce (dsta,newv);
}}}}}}}}	m68k_incpc (2);
endlabel4597: ;
return 0;
}
/* SUB.W #<data>,(d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_5168_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4598;
	}
{{	uae_s16 dst = get_word_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_word_ce (dsta,newv);
}}}}}}}}	m68k_incpc (4);
endlabel4598: ;
return 0;
}
/* SUB.W #<data>,(d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_5170_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (4));
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4599;
	}
{{	uae_s16 dst = get_word_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_word_ce (dsta,newv);
}}}}}}}}	m68k_incpc (4);
endlabel4599: ;
return 0;
}
/* SUB.W #<data>,(xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_5178_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
{{	uae_u32 src = srcreg;
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4600;
	}
{{	uae_s16 dst = get_word_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_word_ce (dsta,newv);
}}}}}}}}	m68k_incpc (4);
endlabel4600: ;
return 0;
}
/* SUB.W #<data>,(xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_5179_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
{{	uae_u32 src = srcreg;
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (4) << 16;
	dsta |= get_word_ce_prefetch (6);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4601;
	}
{{	uae_s16 dst = get_word_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_word_ce (dsta,newv);
}}}}}}}}	m68k_incpc (6);
endlabel4601: ;
return 0;
}
/* SUB.L #<data>,Dn */
unsigned long REGPARAM2 CPUFUNC(op_5180_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	do_cycles (4 * 256);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	m68k_dreg(regs, dstreg) = (newv);
}}}}}}	m68k_incpc (2);
return 0;
}
/* SUBA.L #<data>,An */
unsigned long REGPARAM2 CPUFUNC(op_5188_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uae_s32 dst = m68k_areg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	do_cycles (2 * 256);
{	uae_u32 newv = dst - src;
	m68k_areg(regs, dstreg) = (newv);
}}}}	m68k_incpc (2);
return 0;
}
/* SUB.L #<data>,(An) */
unsigned long REGPARAM2 CPUFUNC(op_5190_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4604;
	}
{{	uae_s32 dst = get_word_ce (dsta) << 16; dst |= get_word_ce (dsta + 2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_word_ce (dsta, newv >> 16); put_word_ce (dsta + 2, newv);
}}}}}}}}	m68k_incpc (2);
endlabel4604: ;
return 0;
}
/* SUB.L #<data>,(An)+ */
unsigned long REGPARAM2 CPUFUNC(op_5198_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4605;
	}
{{	uae_s32 dst = get_word_ce (dsta) << 16; dst |= get_word_ce (dsta + 2);
	m68k_areg(regs, dstreg) += 4;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_word_ce (dsta, newv >> 16); put_word_ce (dsta + 2, newv);
}}}}}}}}	m68k_incpc (2);
endlabel4605: ;
return 0;
}
/* SUB.L #<data>,-(An) */
unsigned long REGPARAM2 CPUFUNC(op_51a0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = m68k_areg(regs, dstreg) - 4;
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4606;
	}
{{	uae_s32 dst = get_word_ce (dsta) << 16; dst |= get_word_ce (dsta + 2);
	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_word_ce (dsta, newv >> 16); put_word_ce (dsta + 2, newv);
}}}}}}}}	m68k_incpc (2);
endlabel4606: ;
return 0;
}
/* SUB.L #<data>,(d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_51a8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4607;
	}
{{	uae_s32 dst = get_word_ce (dsta) << 16; dst |= get_word_ce (dsta + 2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_word_ce (dsta, newv >> 16); put_word_ce (dsta + 2, newv);
}}}}}}}}	m68k_incpc (4);
endlabel4607: ;
return 0;
}
/* SUB.L #<data>,(d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_51b0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (4));
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4608;
	}
{{	uae_s32 dst = get_word_ce (dsta) << 16; dst |= get_word_ce (dsta + 2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_word_ce (dsta, newv >> 16); put_word_ce (dsta + 2, newv);
}}}}}}}}	m68k_incpc (4);
endlabel4608: ;
return 0;
}
/* SUB.L #<data>,(xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_51b8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
{{	uae_u32 src = srcreg;
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4609;
	}
{{	uae_s32 dst = get_word_ce (dsta) << 16; dst |= get_word_ce (dsta + 2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_word_ce (dsta, newv >> 16); put_word_ce (dsta + 2, newv);
}}}}}}}}	m68k_incpc (4);
endlabel4609: ;
return 0;
}
/* SUB.L #<data>,(xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_51b9_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
{{	uae_u32 src = srcreg;
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (4) << 16;
	dsta |= get_word_ce_prefetch (6);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4610;
	}
{{	uae_s32 dst = get_word_ce (dsta) << 16; dst |= get_word_ce (dsta + 2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_word_ce (dsta, newv >> 16); put_word_ce (dsta + 2, newv);
}}}}}}}}	m68k_incpc (6);
endlabel4610: ;
return 0;
}
/* Scc.B Dn */
unsigned long REGPARAM2 CPUFUNC(op_51c0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(1) ? 0xff : 0;
	if (val)
		do_cycles (2 * 256);
	m68k_dreg(regs, srcreg) = (m68k_dreg(regs, srcreg) & ~0xff) | ((val) & 0xff);
}}}	m68k_incpc (2);
return 0;
}
/* DBcc.W Dn,#<data>.W */
unsigned long REGPARAM2 CPUFUNC(op_51c8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uae_s16 offs = regs.irc;
	uaecptr oldpc = m68k_getpc();
	do_cycles (2 * 256);
	if (!cctrue(1)) {
		m68k_incpc((uae_s32)offs + 2);
		get_word_ce_prefetch (0);
		m68k_dreg(regs, srcreg) = (m68k_dreg(regs, srcreg) & ~0xffff) | (((src-1)) & 0xffff);
		if (src) {
			if (offs & 1) {
				exception3 (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)offs + 2);
				goto endlabel4612;
			}
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
			return 0;
		}
	} else {
		do_cycles (2 * 256);
	}
	m68k_setpc (oldpc + 4);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}endlabel4612: ;
return 0;
}
/* Scc.B (An) */
unsigned long REGPARAM2 CPUFUNC(op_51d0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(1) ? 0xff : 0;
	put_byte_ce (srca,val);
}}}	m68k_incpc (2);
return 0;
}
/* Scc.B (An)+ */
unsigned long REGPARAM2 CPUFUNC(op_51d8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
	m68k_areg(regs, srcreg) += areg_byteinc[srcreg];
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(1) ? 0xff : 0;
	put_byte_ce (srca,val);
}}}	m68k_incpc (2);
return 0;
}
/* Scc.B -(An) */
unsigned long REGPARAM2 CPUFUNC(op_51e0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
	m68k_areg (regs, srcreg) = srca;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(1) ? 0xff : 0;
	put_byte_ce (srca,val);
}}}	m68k_incpc (2);
return 0;
}
/* Scc.B (d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_51e8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	int val = cctrue(1) ? 0xff : 0;
	put_byte_ce (srca,val);
}}}	m68k_incpc (4);
return 0;
}
/* Scc.B (d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_51f0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	int val = cctrue(1) ? 0xff : 0;
	put_byte_ce (srca,val);
}}}	m68k_incpc (4);
return 0;
}
/* Scc.B (xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_51f8_6)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	int val = cctrue(1) ? 0xff : 0;
	put_byte_ce (srca,val);
}}}	m68k_incpc (4);
return 0;
}
/* Scc.B (xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_51f9_6)(uae_u32 opcode)
{
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
{	int val = cctrue(1) ? 0xff : 0;
	put_byte_ce (srca,val);
}}}	m68k_incpc (6);
return 0;
}
/* Scc.B Dn */
unsigned long REGPARAM2 CPUFUNC(op_52c0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(2) ? 0xff : 0;
	if (val)
		do_cycles (2 * 256);
	m68k_dreg(regs, srcreg) = (m68k_dreg(regs, srcreg) & ~0xff) | ((val) & 0xff);
}}}	m68k_incpc (2);
return 0;
}
/* DBcc.W Dn,#<data>.W */
unsigned long REGPARAM2 CPUFUNC(op_52c8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uae_s16 offs = regs.irc;
	uaecptr oldpc = m68k_getpc();
	do_cycles (2 * 256);
	if (!cctrue(2)) {
		m68k_incpc((uae_s32)offs + 2);
		get_word_ce_prefetch (0);
		m68k_dreg(regs, srcreg) = (m68k_dreg(regs, srcreg) & ~0xffff) | (((src-1)) & 0xffff);
		if (src) {
			if (offs & 1) {
				exception3 (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)offs + 2);
				goto endlabel4621;
			}
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
			return 0;
		}
	} else {
		do_cycles (2 * 256);
	}
	m68k_setpc (oldpc + 4);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}endlabel4621: ;
return 0;
}
/* Scc.B (An) */
unsigned long REGPARAM2 CPUFUNC(op_52d0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(2) ? 0xff : 0;
	put_byte_ce (srca,val);
}}}	m68k_incpc (2);
return 0;
}
/* Scc.B (An)+ */
unsigned long REGPARAM2 CPUFUNC(op_52d8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
	m68k_areg(regs, srcreg) += areg_byteinc[srcreg];
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(2) ? 0xff : 0;
	put_byte_ce (srca,val);
}}}	m68k_incpc (2);
return 0;
}
/* Scc.B -(An) */
unsigned long REGPARAM2 CPUFUNC(op_52e0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
	m68k_areg (regs, srcreg) = srca;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(2) ? 0xff : 0;
	put_byte_ce (srca,val);
}}}	m68k_incpc (2);
return 0;
}
/* Scc.B (d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_52e8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	int val = cctrue(2) ? 0xff : 0;
	put_byte_ce (srca,val);
}}}	m68k_incpc (4);
return 0;
}
/* Scc.B (d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_52f0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	int val = cctrue(2) ? 0xff : 0;
	put_byte_ce (srca,val);
}}}	m68k_incpc (4);
return 0;
}
/* Scc.B (xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_52f8_6)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	int val = cctrue(2) ? 0xff : 0;
	put_byte_ce (srca,val);
}}}	m68k_incpc (4);
return 0;
}
/* Scc.B (xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_52f9_6)(uae_u32 opcode)
{
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
{	int val = cctrue(2) ? 0xff : 0;
	put_byte_ce (srca,val);
}}}	m68k_incpc (6);
return 0;
}
/* Scc.B Dn */
unsigned long REGPARAM2 CPUFUNC(op_53c0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(3) ? 0xff : 0;
	if (val)
		do_cycles (2 * 256);
	m68k_dreg(regs, srcreg) = (m68k_dreg(regs, srcreg) & ~0xff) | ((val) & 0xff);
}}}	m68k_incpc (2);
return 0;
}
/* DBcc.W Dn,#<data>.W */
unsigned long REGPARAM2 CPUFUNC(op_53c8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uae_s16 offs = regs.irc;
	uaecptr oldpc = m68k_getpc();
	do_cycles (2 * 256);
	if (!cctrue(3)) {
		m68k_incpc((uae_s32)offs + 2);
		get_word_ce_prefetch (0);
		m68k_dreg(regs, srcreg) = (m68k_dreg(regs, srcreg) & ~0xffff) | (((src-1)) & 0xffff);
		if (src) {
			if (offs & 1) {
				exception3 (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)offs + 2);
				goto endlabel4630;
			}
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
			return 0;
		}
	} else {
		do_cycles (2 * 256);
	}
	m68k_setpc (oldpc + 4);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}endlabel4630: ;
return 0;
}
/* Scc.B (An) */
unsigned long REGPARAM2 CPUFUNC(op_53d0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(3) ? 0xff : 0;
	put_byte_ce (srca,val);
}}}	m68k_incpc (2);
return 0;
}
/* Scc.B (An)+ */
unsigned long REGPARAM2 CPUFUNC(op_53d8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
	m68k_areg(regs, srcreg) += areg_byteinc[srcreg];
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(3) ? 0xff : 0;
	put_byte_ce (srca,val);
}}}	m68k_incpc (2);
return 0;
}
/* Scc.B -(An) */
unsigned long REGPARAM2 CPUFUNC(op_53e0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
	m68k_areg (regs, srcreg) = srca;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(3) ? 0xff : 0;
	put_byte_ce (srca,val);
}}}	m68k_incpc (2);
return 0;
}
/* Scc.B (d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_53e8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	int val = cctrue(3) ? 0xff : 0;
	put_byte_ce (srca,val);
}}}	m68k_incpc (4);
return 0;
}
/* Scc.B (d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_53f0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	int val = cctrue(3) ? 0xff : 0;
	put_byte_ce (srca,val);
}}}	m68k_incpc (4);
return 0;
}
/* Scc.B (xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_53f8_6)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	int val = cctrue(3) ? 0xff : 0;
	put_byte_ce (srca,val);
}}}	m68k_incpc (4);
return 0;
}
/* Scc.B (xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_53f9_6)(uae_u32 opcode)
{
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
{	int val = cctrue(3) ? 0xff : 0;
	put_byte_ce (srca,val);
}}}	m68k_incpc (6);
return 0;
}
/* Scc.B Dn */
unsigned long REGPARAM2 CPUFUNC(op_54c0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(4) ? 0xff : 0;
	if (val)
		do_cycles (2 * 256);
	m68k_dreg(regs, srcreg) = (m68k_dreg(regs, srcreg) & ~0xff) | ((val) & 0xff);
}}}	m68k_incpc (2);
return 0;
}
/* DBcc.W Dn,#<data>.W */
unsigned long REGPARAM2 CPUFUNC(op_54c8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uae_s16 offs = regs.irc;
	uaecptr oldpc = m68k_getpc();
	do_cycles (2 * 256);
	if (!cctrue(4)) {
		m68k_incpc((uae_s32)offs + 2);
		get_word_ce_prefetch (0);
		m68k_dreg(regs, srcreg) = (m68k_dreg(regs, srcreg) & ~0xffff) | (((src-1)) & 0xffff);
		if (src) {
			if (offs & 1) {
				exception3 (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)offs + 2);
				goto endlabel4639;
			}
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
			return 0;
		}
	} else {
		do_cycles (2 * 256);
	}
	m68k_setpc (oldpc + 4);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}endlabel4639: ;
return 0;
}
/* Scc.B (An) */
unsigned long REGPARAM2 CPUFUNC(op_54d0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(4) ? 0xff : 0;
	put_byte_ce (srca,val);
}}}	m68k_incpc (2);
return 0;
}
/* Scc.B (An)+ */
unsigned long REGPARAM2 CPUFUNC(op_54d8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
	m68k_areg(regs, srcreg) += areg_byteinc[srcreg];
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(4) ? 0xff : 0;
	put_byte_ce (srca,val);
}}}	m68k_incpc (2);
return 0;
}
/* Scc.B -(An) */
unsigned long REGPARAM2 CPUFUNC(op_54e0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
	m68k_areg (regs, srcreg) = srca;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(4) ? 0xff : 0;
	put_byte_ce (srca,val);
}}}	m68k_incpc (2);
return 0;
}
/* Scc.B (d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_54e8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	int val = cctrue(4) ? 0xff : 0;
	put_byte_ce (srca,val);
}}}	m68k_incpc (4);
return 0;
}
/* Scc.B (d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_54f0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	int val = cctrue(4) ? 0xff : 0;
	put_byte_ce (srca,val);
}}}	m68k_incpc (4);
return 0;
}
/* Scc.B (xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_54f8_6)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	int val = cctrue(4) ? 0xff : 0;
	put_byte_ce (srca,val);
}}}	m68k_incpc (4);
return 0;
}
/* Scc.B (xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_54f9_6)(uae_u32 opcode)
{
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
{	int val = cctrue(4) ? 0xff : 0;
	put_byte_ce (srca,val);
}}}	m68k_incpc (6);
return 0;
}
/* Scc.B Dn */
unsigned long REGPARAM2 CPUFUNC(op_55c0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(5) ? 0xff : 0;
	if (val)
		do_cycles (2 * 256);
	m68k_dreg(regs, srcreg) = (m68k_dreg(regs, srcreg) & ~0xff) | ((val) & 0xff);
}}}	m68k_incpc (2);
return 0;
}
/* DBcc.W Dn,#<data>.W */
unsigned long REGPARAM2 CPUFUNC(op_55c8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uae_s16 offs = regs.irc;
	uaecptr oldpc = m68k_getpc();
	do_cycles (2 * 256);
	if (!cctrue(5)) {
		m68k_incpc((uae_s32)offs + 2);
		get_word_ce_prefetch (0);
		m68k_dreg(regs, srcreg) = (m68k_dreg(regs, srcreg) & ~0xffff) | (((src-1)) & 0xffff);
		if (src) {
			if (offs & 1) {
				exception3 (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)offs + 2);
				goto endlabel4648;
			}
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
			return 0;
		}
	} else {
		do_cycles (2 * 256);
	}
	m68k_setpc (oldpc + 4);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}endlabel4648: ;
return 0;
}
/* Scc.B (An) */
unsigned long REGPARAM2 CPUFUNC(op_55d0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(5) ? 0xff : 0;
	put_byte_ce (srca,val);
}}}	m68k_incpc (2);
return 0;
}
/* Scc.B (An)+ */
unsigned long REGPARAM2 CPUFUNC(op_55d8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
	m68k_areg(regs, srcreg) += areg_byteinc[srcreg];
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(5) ? 0xff : 0;
	put_byte_ce (srca,val);
}}}	m68k_incpc (2);
return 0;
}
/* Scc.B -(An) */
unsigned long REGPARAM2 CPUFUNC(op_55e0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
	m68k_areg (regs, srcreg) = srca;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(5) ? 0xff : 0;
	put_byte_ce (srca,val);
}}}	m68k_incpc (2);
return 0;
}
/* Scc.B (d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_55e8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	int val = cctrue(5) ? 0xff : 0;
	put_byte_ce (srca,val);
}}}	m68k_incpc (4);
return 0;
}
/* Scc.B (d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_55f0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	int val = cctrue(5) ? 0xff : 0;
	put_byte_ce (srca,val);
}}}	m68k_incpc (4);
return 0;
}
/* Scc.B (xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_55f8_6)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	int val = cctrue(5) ? 0xff : 0;
	put_byte_ce (srca,val);
}}}	m68k_incpc (4);
return 0;
}
/* Scc.B (xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_55f9_6)(uae_u32 opcode)
{
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
{	int val = cctrue(5) ? 0xff : 0;
	put_byte_ce (srca,val);
}}}	m68k_incpc (6);
return 0;
}
/* Scc.B Dn */
unsigned long REGPARAM2 CPUFUNC(op_56c0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(6) ? 0xff : 0;
	if (val)
		do_cycles (2 * 256);
	m68k_dreg(regs, srcreg) = (m68k_dreg(regs, srcreg) & ~0xff) | ((val) & 0xff);
}}}	m68k_incpc (2);
return 0;
}
/* DBcc.W Dn,#<data>.W */
unsigned long REGPARAM2 CPUFUNC(op_56c8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uae_s16 offs = regs.irc;
	uaecptr oldpc = m68k_getpc();
	do_cycles (2 * 256);
	if (!cctrue(6)) {
		m68k_incpc((uae_s32)offs + 2);
		get_word_ce_prefetch (0);
		m68k_dreg(regs, srcreg) = (m68k_dreg(regs, srcreg) & ~0xffff) | (((src-1)) & 0xffff);
		if (src) {
			if (offs & 1) {
				exception3 (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)offs + 2);
				goto endlabel4657;
			}
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
			return 0;
		}
	} else {
		do_cycles (2 * 256);
	}
	m68k_setpc (oldpc + 4);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}endlabel4657: ;
return 0;
}
/* Scc.B (An) */
unsigned long REGPARAM2 CPUFUNC(op_56d0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(6) ? 0xff : 0;
	put_byte_ce (srca,val);
}}}	m68k_incpc (2);
return 0;
}
/* Scc.B (An)+ */
unsigned long REGPARAM2 CPUFUNC(op_56d8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
	m68k_areg(regs, srcreg) += areg_byteinc[srcreg];
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(6) ? 0xff : 0;
	put_byte_ce (srca,val);
}}}	m68k_incpc (2);
return 0;
}
/* Scc.B -(An) */
unsigned long REGPARAM2 CPUFUNC(op_56e0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
	m68k_areg (regs, srcreg) = srca;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(6) ? 0xff : 0;
	put_byte_ce (srca,val);
}}}	m68k_incpc (2);
return 0;
}
/* Scc.B (d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_56e8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	int val = cctrue(6) ? 0xff : 0;
	put_byte_ce (srca,val);
}}}	m68k_incpc (4);
return 0;
}
/* Scc.B (d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_56f0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	int val = cctrue(6) ? 0xff : 0;
	put_byte_ce (srca,val);
}}}	m68k_incpc (4);
return 0;
}
/* Scc.B (xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_56f8_6)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	int val = cctrue(6) ? 0xff : 0;
	put_byte_ce (srca,val);
}}}	m68k_incpc (4);
return 0;
}
/* Scc.B (xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_56f9_6)(uae_u32 opcode)
{
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
{	int val = cctrue(6) ? 0xff : 0;
	put_byte_ce (srca,val);
}}}	m68k_incpc (6);
return 0;
}
/* Scc.B Dn */
unsigned long REGPARAM2 CPUFUNC(op_57c0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(7) ? 0xff : 0;
	if (val)
		do_cycles (2 * 256);
	m68k_dreg(regs, srcreg) = (m68k_dreg(regs, srcreg) & ~0xff) | ((val) & 0xff);
}}}	m68k_incpc (2);
return 0;
}
/* DBcc.W Dn,#<data>.W */
unsigned long REGPARAM2 CPUFUNC(op_57c8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uae_s16 offs = regs.irc;
	uaecptr oldpc = m68k_getpc();
	do_cycles (2 * 256);
	if (!cctrue(7)) {
		m68k_incpc((uae_s32)offs + 2);
		get_word_ce_prefetch (0);
		m68k_dreg(regs, srcreg) = (m68k_dreg(regs, srcreg) & ~0xffff) | (((src-1)) & 0xffff);
		if (src) {
			if (offs & 1) {
				exception3 (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)offs + 2);
				goto endlabel4666;
			}
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
			return 0;
		}
	} else {
		do_cycles (2 * 256);
	}
	m68k_setpc (oldpc + 4);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}endlabel4666: ;
return 0;
}
/* Scc.B (An) */
unsigned long REGPARAM2 CPUFUNC(op_57d0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(7) ? 0xff : 0;
	put_byte_ce (srca,val);
}}}	m68k_incpc (2);
return 0;
}
/* Scc.B (An)+ */
unsigned long REGPARAM2 CPUFUNC(op_57d8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
	m68k_areg(regs, srcreg) += areg_byteinc[srcreg];
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(7) ? 0xff : 0;
	put_byte_ce (srca,val);
}}}	m68k_incpc (2);
return 0;
}
/* Scc.B -(An) */
unsigned long REGPARAM2 CPUFUNC(op_57e0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
	m68k_areg (regs, srcreg) = srca;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(7) ? 0xff : 0;
	put_byte_ce (srca,val);
}}}	m68k_incpc (2);
return 0;
}
/* Scc.B (d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_57e8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	int val = cctrue(7) ? 0xff : 0;
	put_byte_ce (srca,val);
}}}	m68k_incpc (4);
return 0;
}
/* Scc.B (d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_57f0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	int val = cctrue(7) ? 0xff : 0;
	put_byte_ce (srca,val);
}}}	m68k_incpc (4);
return 0;
}
/* Scc.B (xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_57f8_6)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	int val = cctrue(7) ? 0xff : 0;
	put_byte_ce (srca,val);
}}}	m68k_incpc (4);
return 0;
}
/* Scc.B (xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_57f9_6)(uae_u32 opcode)
{
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
{	int val = cctrue(7) ? 0xff : 0;
	put_byte_ce (srca,val);
}}}	m68k_incpc (6);
return 0;
}
/* Scc.B Dn */
unsigned long REGPARAM2 CPUFUNC(op_58c0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(8) ? 0xff : 0;
	if (val)
		do_cycles (2 * 256);
	m68k_dreg(regs, srcreg) = (m68k_dreg(regs, srcreg) & ~0xff) | ((val) & 0xff);
}}}	m68k_incpc (2);
return 0;
}
/* DBcc.W Dn,#<data>.W */
unsigned long REGPARAM2 CPUFUNC(op_58c8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uae_s16 offs = regs.irc;
	uaecptr oldpc = m68k_getpc();
	do_cycles (2 * 256);
	if (!cctrue(8)) {
		m68k_incpc((uae_s32)offs + 2);
		get_word_ce_prefetch (0);
		m68k_dreg(regs, srcreg) = (m68k_dreg(regs, srcreg) & ~0xffff) | (((src-1)) & 0xffff);
		if (src) {
			if (offs & 1) {
				exception3 (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)offs + 2);
				goto endlabel4675;
			}
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
			return 0;
		}
	} else {
		do_cycles (2 * 256);
	}
	m68k_setpc (oldpc + 4);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}endlabel4675: ;
return 0;
}
/* Scc.B (An) */
unsigned long REGPARAM2 CPUFUNC(op_58d0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(8) ? 0xff : 0;
	put_byte_ce (srca,val);
}}}	m68k_incpc (2);
return 0;
}
/* Scc.B (An)+ */
unsigned long REGPARAM2 CPUFUNC(op_58d8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
	m68k_areg(regs, srcreg) += areg_byteinc[srcreg];
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(8) ? 0xff : 0;
	put_byte_ce (srca,val);
}}}	m68k_incpc (2);
return 0;
}
/* Scc.B -(An) */
unsigned long REGPARAM2 CPUFUNC(op_58e0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
	m68k_areg (regs, srcreg) = srca;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(8) ? 0xff : 0;
	put_byte_ce (srca,val);
}}}	m68k_incpc (2);
return 0;
}
/* Scc.B (d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_58e8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	int val = cctrue(8) ? 0xff : 0;
	put_byte_ce (srca,val);
}}}	m68k_incpc (4);
return 0;
}
/* Scc.B (d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_58f0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	int val = cctrue(8) ? 0xff : 0;
	put_byte_ce (srca,val);
}}}	m68k_incpc (4);
return 0;
}
/* Scc.B (xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_58f8_6)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	int val = cctrue(8) ? 0xff : 0;
	put_byte_ce (srca,val);
}}}	m68k_incpc (4);
return 0;
}
/* Scc.B (xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_58f9_6)(uae_u32 opcode)
{
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
{	int val = cctrue(8) ? 0xff : 0;
	put_byte_ce (srca,val);
}}}	m68k_incpc (6);
return 0;
}
/* Scc.B Dn */
unsigned long REGPARAM2 CPUFUNC(op_59c0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(9) ? 0xff : 0;
	if (val)
		do_cycles (2 * 256);
	m68k_dreg(regs, srcreg) = (m68k_dreg(regs, srcreg) & ~0xff) | ((val) & 0xff);
}}}	m68k_incpc (2);
return 0;
}
/* DBcc.W Dn,#<data>.W */
unsigned long REGPARAM2 CPUFUNC(op_59c8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uae_s16 offs = regs.irc;
	uaecptr oldpc = m68k_getpc();
	do_cycles (2 * 256);
	if (!cctrue(9)) {
		m68k_incpc((uae_s32)offs + 2);
		get_word_ce_prefetch (0);
		m68k_dreg(regs, srcreg) = (m68k_dreg(regs, srcreg) & ~0xffff) | (((src-1)) & 0xffff);
		if (src) {
			if (offs & 1) {
				exception3 (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)offs + 2);
				goto endlabel4684;
			}
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
			return 0;
		}
	} else {
		do_cycles (2 * 256);
	}
	m68k_setpc (oldpc + 4);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}endlabel4684: ;
return 0;
}
/* Scc.B (An) */
unsigned long REGPARAM2 CPUFUNC(op_59d0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(9) ? 0xff : 0;
	put_byte_ce (srca,val);
}}}	m68k_incpc (2);
return 0;
}
/* Scc.B (An)+ */
unsigned long REGPARAM2 CPUFUNC(op_59d8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
	m68k_areg(regs, srcreg) += areg_byteinc[srcreg];
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(9) ? 0xff : 0;
	put_byte_ce (srca,val);
}}}	m68k_incpc (2);
return 0;
}
/* Scc.B -(An) */
unsigned long REGPARAM2 CPUFUNC(op_59e0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
	m68k_areg (regs, srcreg) = srca;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(9) ? 0xff : 0;
	put_byte_ce (srca,val);
}}}	m68k_incpc (2);
return 0;
}
/* Scc.B (d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_59e8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	int val = cctrue(9) ? 0xff : 0;
	put_byte_ce (srca,val);
}}}	m68k_incpc (4);
return 0;
}
/* Scc.B (d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_59f0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	int val = cctrue(9) ? 0xff : 0;
	put_byte_ce (srca,val);
}}}	m68k_incpc (4);
return 0;
}
/* Scc.B (xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_59f8_6)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	int val = cctrue(9) ? 0xff : 0;
	put_byte_ce (srca,val);
}}}	m68k_incpc (4);
return 0;
}
/* Scc.B (xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_59f9_6)(uae_u32 opcode)
{
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
{	int val = cctrue(9) ? 0xff : 0;
	put_byte_ce (srca,val);
}}}	m68k_incpc (6);
return 0;
}
/* Scc.B Dn */
unsigned long REGPARAM2 CPUFUNC(op_5ac0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(10) ? 0xff : 0;
	if (val)
		do_cycles (2 * 256);
	m68k_dreg(regs, srcreg) = (m68k_dreg(regs, srcreg) & ~0xff) | ((val) & 0xff);
}}}	m68k_incpc (2);
return 0;
}
/* DBcc.W Dn,#<data>.W */
unsigned long REGPARAM2 CPUFUNC(op_5ac8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uae_s16 offs = regs.irc;
	uaecptr oldpc = m68k_getpc();
	do_cycles (2 * 256);
	if (!cctrue(10)) {
		m68k_incpc((uae_s32)offs + 2);
		get_word_ce_prefetch (0);
		m68k_dreg(regs, srcreg) = (m68k_dreg(regs, srcreg) & ~0xffff) | (((src-1)) & 0xffff);
		if (src) {
			if (offs & 1) {
				exception3 (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)offs + 2);
				goto endlabel4693;
			}
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
			return 0;
		}
	} else {
		do_cycles (2 * 256);
	}
	m68k_setpc (oldpc + 4);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}endlabel4693: ;
return 0;
}
/* Scc.B (An) */
unsigned long REGPARAM2 CPUFUNC(op_5ad0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(10) ? 0xff : 0;
	put_byte_ce (srca,val);
}}}	m68k_incpc (2);
return 0;
}
/* Scc.B (An)+ */
unsigned long REGPARAM2 CPUFUNC(op_5ad8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
	m68k_areg(regs, srcreg) += areg_byteinc[srcreg];
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(10) ? 0xff : 0;
	put_byte_ce (srca,val);
}}}	m68k_incpc (2);
return 0;
}
/* Scc.B -(An) */
unsigned long REGPARAM2 CPUFUNC(op_5ae0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
	m68k_areg (regs, srcreg) = srca;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(10) ? 0xff : 0;
	put_byte_ce (srca,val);
}}}	m68k_incpc (2);
return 0;
}
/* Scc.B (d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_5ae8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	int val = cctrue(10) ? 0xff : 0;
	put_byte_ce (srca,val);
}}}	m68k_incpc (4);
return 0;
}
/* Scc.B (d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_5af0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	int val = cctrue(10) ? 0xff : 0;
	put_byte_ce (srca,val);
}}}	m68k_incpc (4);
return 0;
}
/* Scc.B (xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_5af8_6)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	int val = cctrue(10) ? 0xff : 0;
	put_byte_ce (srca,val);
}}}	m68k_incpc (4);
return 0;
}
/* Scc.B (xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_5af9_6)(uae_u32 opcode)
{
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
{	int val = cctrue(10) ? 0xff : 0;
	put_byte_ce (srca,val);
}}}	m68k_incpc (6);
return 0;
}
/* Scc.B Dn */
unsigned long REGPARAM2 CPUFUNC(op_5bc0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(11) ? 0xff : 0;
	if (val)
		do_cycles (2 * 256);
	m68k_dreg(regs, srcreg) = (m68k_dreg(regs, srcreg) & ~0xff) | ((val) & 0xff);
}}}	m68k_incpc (2);
return 0;
}
/* DBcc.W Dn,#<data>.W */
unsigned long REGPARAM2 CPUFUNC(op_5bc8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uae_s16 offs = regs.irc;
	uaecptr oldpc = m68k_getpc();
	do_cycles (2 * 256);
	if (!cctrue(11)) {
		m68k_incpc((uae_s32)offs + 2);
		get_word_ce_prefetch (0);
		m68k_dreg(regs, srcreg) = (m68k_dreg(regs, srcreg) & ~0xffff) | (((src-1)) & 0xffff);
		if (src) {
			if (offs & 1) {
				exception3 (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)offs + 2);
				goto endlabel4702;
			}
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
			return 0;
		}
	} else {
		do_cycles (2 * 256);
	}
	m68k_setpc (oldpc + 4);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}endlabel4702: ;
return 0;
}
/* Scc.B (An) */
unsigned long REGPARAM2 CPUFUNC(op_5bd0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(11) ? 0xff : 0;
	put_byte_ce (srca,val);
}}}	m68k_incpc (2);
return 0;
}
/* Scc.B (An)+ */
unsigned long REGPARAM2 CPUFUNC(op_5bd8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
	m68k_areg(regs, srcreg) += areg_byteinc[srcreg];
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(11) ? 0xff : 0;
	put_byte_ce (srca,val);
}}}	m68k_incpc (2);
return 0;
}
/* Scc.B -(An) */
unsigned long REGPARAM2 CPUFUNC(op_5be0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
	m68k_areg (regs, srcreg) = srca;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(11) ? 0xff : 0;
	put_byte_ce (srca,val);
}}}	m68k_incpc (2);
return 0;
}
/* Scc.B (d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_5be8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	int val = cctrue(11) ? 0xff : 0;
	put_byte_ce (srca,val);
}}}	m68k_incpc (4);
return 0;
}
/* Scc.B (d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_5bf0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	int val = cctrue(11) ? 0xff : 0;
	put_byte_ce (srca,val);
}}}	m68k_incpc (4);
return 0;
}
/* Scc.B (xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_5bf8_6)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	int val = cctrue(11) ? 0xff : 0;
	put_byte_ce (srca,val);
}}}	m68k_incpc (4);
return 0;
}
/* Scc.B (xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_5bf9_6)(uae_u32 opcode)
{
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
{	int val = cctrue(11) ? 0xff : 0;
	put_byte_ce (srca,val);
}}}	m68k_incpc (6);
return 0;
}
/* Scc.B Dn */
unsigned long REGPARAM2 CPUFUNC(op_5cc0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(12) ? 0xff : 0;
	if (val)
		do_cycles (2 * 256);
	m68k_dreg(regs, srcreg) = (m68k_dreg(regs, srcreg) & ~0xff) | ((val) & 0xff);
}}}	m68k_incpc (2);
return 0;
}
/* DBcc.W Dn,#<data>.W */
unsigned long REGPARAM2 CPUFUNC(op_5cc8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uae_s16 offs = regs.irc;
	uaecptr oldpc = m68k_getpc();
	do_cycles (2 * 256);
	if (!cctrue(12)) {
		m68k_incpc((uae_s32)offs + 2);
		get_word_ce_prefetch (0);
		m68k_dreg(regs, srcreg) = (m68k_dreg(regs, srcreg) & ~0xffff) | (((src-1)) & 0xffff);
		if (src) {
			if (offs & 1) {
				exception3 (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)offs + 2);
				goto endlabel4711;
			}
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
			return 0;
		}
	} else {
		do_cycles (2 * 256);
	}
	m68k_setpc (oldpc + 4);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}endlabel4711: ;
return 0;
}
/* Scc.B (An) */
unsigned long REGPARAM2 CPUFUNC(op_5cd0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(12) ? 0xff : 0;
	put_byte_ce (srca,val);
}}}	m68k_incpc (2);
return 0;
}
/* Scc.B (An)+ */
unsigned long REGPARAM2 CPUFUNC(op_5cd8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
	m68k_areg(regs, srcreg) += areg_byteinc[srcreg];
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(12) ? 0xff : 0;
	put_byte_ce (srca,val);
}}}	m68k_incpc (2);
return 0;
}
/* Scc.B -(An) */
unsigned long REGPARAM2 CPUFUNC(op_5ce0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
	m68k_areg (regs, srcreg) = srca;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(12) ? 0xff : 0;
	put_byte_ce (srca,val);
}}}	m68k_incpc (2);
return 0;
}
/* Scc.B (d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_5ce8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	int val = cctrue(12) ? 0xff : 0;
	put_byte_ce (srca,val);
}}}	m68k_incpc (4);
return 0;
}
/* Scc.B (d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_5cf0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	int val = cctrue(12) ? 0xff : 0;
	put_byte_ce (srca,val);
}}}	m68k_incpc (4);
return 0;
}
/* Scc.B (xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_5cf8_6)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	int val = cctrue(12) ? 0xff : 0;
	put_byte_ce (srca,val);
}}}	m68k_incpc (4);
return 0;
}
/* Scc.B (xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_5cf9_6)(uae_u32 opcode)
{
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
{	int val = cctrue(12) ? 0xff : 0;
	put_byte_ce (srca,val);
}}}	m68k_incpc (6);
return 0;
}
/* Scc.B Dn */
unsigned long REGPARAM2 CPUFUNC(op_5dc0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(13) ? 0xff : 0;
	if (val)
		do_cycles (2 * 256);
	m68k_dreg(regs, srcreg) = (m68k_dreg(regs, srcreg) & ~0xff) | ((val) & 0xff);
}}}	m68k_incpc (2);
return 0;
}
/* DBcc.W Dn,#<data>.W */
unsigned long REGPARAM2 CPUFUNC(op_5dc8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uae_s16 offs = regs.irc;
	uaecptr oldpc = m68k_getpc();
	do_cycles (2 * 256);
	if (!cctrue(13)) {
		m68k_incpc((uae_s32)offs + 2);
		get_word_ce_prefetch (0);
		m68k_dreg(regs, srcreg) = (m68k_dreg(regs, srcreg) & ~0xffff) | (((src-1)) & 0xffff);
		if (src) {
			if (offs & 1) {
				exception3 (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)offs + 2);
				goto endlabel4720;
			}
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
			return 0;
		}
	} else {
		do_cycles (2 * 256);
	}
	m68k_setpc (oldpc + 4);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}endlabel4720: ;
return 0;
}
/* Scc.B (An) */
unsigned long REGPARAM2 CPUFUNC(op_5dd0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(13) ? 0xff : 0;
	put_byte_ce (srca,val);
}}}	m68k_incpc (2);
return 0;
}
/* Scc.B (An)+ */
unsigned long REGPARAM2 CPUFUNC(op_5dd8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
	m68k_areg(regs, srcreg) += areg_byteinc[srcreg];
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(13) ? 0xff : 0;
	put_byte_ce (srca,val);
}}}	m68k_incpc (2);
return 0;
}
/* Scc.B -(An) */
unsigned long REGPARAM2 CPUFUNC(op_5de0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
	m68k_areg (regs, srcreg) = srca;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(13) ? 0xff : 0;
	put_byte_ce (srca,val);
}}}	m68k_incpc (2);
return 0;
}
/* Scc.B (d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_5de8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	int val = cctrue(13) ? 0xff : 0;
	put_byte_ce (srca,val);
}}}	m68k_incpc (4);
return 0;
}
/* Scc.B (d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_5df0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	int val = cctrue(13) ? 0xff : 0;
	put_byte_ce (srca,val);
}}}	m68k_incpc (4);
return 0;
}
/* Scc.B (xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_5df8_6)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	int val = cctrue(13) ? 0xff : 0;
	put_byte_ce (srca,val);
}}}	m68k_incpc (4);
return 0;
}
/* Scc.B (xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_5df9_6)(uae_u32 opcode)
{
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
{	int val = cctrue(13) ? 0xff : 0;
	put_byte_ce (srca,val);
}}}	m68k_incpc (6);
return 0;
}
/* Scc.B Dn */
unsigned long REGPARAM2 CPUFUNC(op_5ec0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(14) ? 0xff : 0;
	if (val)
		do_cycles (2 * 256);
	m68k_dreg(regs, srcreg) = (m68k_dreg(regs, srcreg) & ~0xff) | ((val) & 0xff);
}}}	m68k_incpc (2);
return 0;
}
/* DBcc.W Dn,#<data>.W */
unsigned long REGPARAM2 CPUFUNC(op_5ec8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uae_s16 offs = regs.irc;
	uaecptr oldpc = m68k_getpc();
	do_cycles (2 * 256);
	if (!cctrue(14)) {
		m68k_incpc((uae_s32)offs + 2);
		get_word_ce_prefetch (0);
		m68k_dreg(regs, srcreg) = (m68k_dreg(regs, srcreg) & ~0xffff) | (((src-1)) & 0xffff);
		if (src) {
			if (offs & 1) {
				exception3 (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)offs + 2);
				goto endlabel4729;
			}
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
			return 0;
		}
	} else {
		do_cycles (2 * 256);
	}
	m68k_setpc (oldpc + 4);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}endlabel4729: ;
return 0;
}
/* Scc.B (An) */
unsigned long REGPARAM2 CPUFUNC(op_5ed0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(14) ? 0xff : 0;
	put_byte_ce (srca,val);
}}}	m68k_incpc (2);
return 0;
}
/* Scc.B (An)+ */
unsigned long REGPARAM2 CPUFUNC(op_5ed8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
	m68k_areg(regs, srcreg) += areg_byteinc[srcreg];
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(14) ? 0xff : 0;
	put_byte_ce (srca,val);
}}}	m68k_incpc (2);
return 0;
}
/* Scc.B -(An) */
unsigned long REGPARAM2 CPUFUNC(op_5ee0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
	m68k_areg (regs, srcreg) = srca;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(14) ? 0xff : 0;
	put_byte_ce (srca,val);
}}}	m68k_incpc (2);
return 0;
}
/* Scc.B (d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_5ee8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	int val = cctrue(14) ? 0xff : 0;
	put_byte_ce (srca,val);
}}}	m68k_incpc (4);
return 0;
}
/* Scc.B (d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_5ef0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	int val = cctrue(14) ? 0xff : 0;
	put_byte_ce (srca,val);
}}}	m68k_incpc (4);
return 0;
}
/* Scc.B (xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_5ef8_6)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	int val = cctrue(14) ? 0xff : 0;
	put_byte_ce (srca,val);
}}}	m68k_incpc (4);
return 0;
}
/* Scc.B (xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_5ef9_6)(uae_u32 opcode)
{
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
{	int val = cctrue(14) ? 0xff : 0;
	put_byte_ce (srca,val);
}}}	m68k_incpc (6);
return 0;
}
/* Scc.B Dn */
unsigned long REGPARAM2 CPUFUNC(op_5fc0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(15) ? 0xff : 0;
	if (val)
		do_cycles (2 * 256);
	m68k_dreg(regs, srcreg) = (m68k_dreg(regs, srcreg) & ~0xff) | ((val) & 0xff);
}}}	m68k_incpc (2);
return 0;
}
/* DBcc.W Dn,#<data>.W */
unsigned long REGPARAM2 CPUFUNC(op_5fc8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uae_s16 offs = regs.irc;
	uaecptr oldpc = m68k_getpc();
	do_cycles (2 * 256);
	if (!cctrue(15)) {
		m68k_incpc((uae_s32)offs + 2);
		get_word_ce_prefetch (0);
		m68k_dreg(regs, srcreg) = (m68k_dreg(regs, srcreg) & ~0xffff) | (((src-1)) & 0xffff);
		if (src) {
			if (offs & 1) {
				exception3 (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)offs + 2);
				goto endlabel4738;
			}
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
			return 0;
		}
	} else {
		do_cycles (2 * 256);
	}
	m68k_setpc (oldpc + 4);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}endlabel4738: ;
return 0;
}
/* Scc.B (An) */
unsigned long REGPARAM2 CPUFUNC(op_5fd0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(15) ? 0xff : 0;
	put_byte_ce (srca,val);
}}}	m68k_incpc (2);
return 0;
}
/* Scc.B (An)+ */
unsigned long REGPARAM2 CPUFUNC(op_5fd8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
	m68k_areg(regs, srcreg) += areg_byteinc[srcreg];
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(15) ? 0xff : 0;
	put_byte_ce (srca,val);
}}}	m68k_incpc (2);
return 0;
}
/* Scc.B -(An) */
unsigned long REGPARAM2 CPUFUNC(op_5fe0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
	m68k_areg (regs, srcreg) = srca;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(15) ? 0xff : 0;
	put_byte_ce (srca,val);
}}}	m68k_incpc (2);
return 0;
}
/* Scc.B (d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_5fe8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	int val = cctrue(15) ? 0xff : 0;
	put_byte_ce (srca,val);
}}}	m68k_incpc (4);
return 0;
}
/* Scc.B (d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_5ff0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	int val = cctrue(15) ? 0xff : 0;
	put_byte_ce (srca,val);
}}}	m68k_incpc (4);
return 0;
}
/* Scc.B (xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_5ff8_6)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	int val = cctrue(15) ? 0xff : 0;
	put_byte_ce (srca,val);
}}}	m68k_incpc (4);
return 0;
}
#endif

#ifdef PART_6
/* Scc.B (xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_5ff9_6)(uae_u32 opcode)
{
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
{	int val = cctrue(15) ? 0xff : 0;
	put_byte_ce (srca,val);
}}}	m68k_incpc (6);
return 0;
}
/* Bcc.W #<data>.W */
unsigned long REGPARAM2 CPUFUNC(op_6000_6)(uae_u32 opcode)
{
{{	uae_s16 src = regs.irc;
	do_cycles (2 * 256);
	if (!cctrue(0)) goto didnt_jump;
	if (src & 1) {
		exception3 (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)src);
		goto endlabel4746;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	return 0;
didnt_jump:;
	m68k_incpc (4);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4746: ;
return 0;
}
/* Bcc.B #<data> */
unsigned long REGPARAM2 CPUFUNC(op_6001_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
{{	uae_u32 src = srcreg;
	do_cycles (2 * 256);
	if (!cctrue(0)) goto didnt_jump;
	if (src & 1) {
		exception3 (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)src);
		goto endlabel4747;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	return 0;
didnt_jump:;
	m68k_incpc (2);
	do_cycles (2 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4747: ;
return 0;
}
/* Bcc.L #<data>.L */
unsigned long REGPARAM2 CPUFUNC(op_60ff_6)(uae_u32 opcode)
{
{	do_cycles (2 * 256);
	if (cctrue(0)) {
		exception3 (opcode, m68k_getpc() + 2, m68k_getpc() + 1);
		goto endlabel4748;
	}
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	goto endlabel4748;
{	uae_s32 src;
	src = get_word_ce_prefetch (2) << 16;
	src |= regs.irc;
	do_cycles (2 * 256);
	if (!cctrue(0)) goto didnt_jump;
	if (src & 1) {
		exception3 (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)src);
		goto endlabel4748;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	return 0;
didnt_jump:;
	m68k_incpc (4);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4748: ;
return 0;
}
/* BSR.W #<data>.W */
unsigned long REGPARAM2 CPUFUNC(op_6100_6)(uae_u32 opcode)
{
{	uae_s32 s;
{	uae_s16 src = regs.irc;
	s = (uae_s32)src + 2;
	if (src & 1) {
		exception3 (opcode, m68k_getpc() + 2, m68k_getpc() + s);
		goto endlabel4749;
	}
	do_cycles (2 * 256);
	m68k_do_bsr_ce (m68k_getpc() + 4, s);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4749: ;
return 0;
}
/* BSR.B #<data> */
unsigned long REGPARAM2 CPUFUNC(op_6101_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
{	uae_s32 s;
{	uae_u32 src = srcreg;
	s = (uae_s32)src + 2;
	if (src & 1) {
		exception3 (opcode, m68k_getpc() + 2, m68k_getpc() + s);
		goto endlabel4750;
	}
	do_cycles (2 * 256);
	m68k_do_bsr_ce (m68k_getpc() + 2, s);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4750: ;
return 0;
}
/* BSR.L #<data>.L */
unsigned long REGPARAM2 CPUFUNC(op_61ff_6)(uae_u32 opcode)
{
{	uae_s32 s;
{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= regs.irc;
	s = (uae_s32)src + 2;
	if (src & 1) {
		exception3 (opcode, m68k_getpc() + 2, m68k_getpc() + s);
		goto endlabel4751;
	}
	do_cycles (2 * 256);
	m68k_do_bsr_ce (m68k_getpc() + 6, s);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4751: ;
return 0;
}
/* Bcc.W #<data>.W */
unsigned long REGPARAM2 CPUFUNC(op_6200_6)(uae_u32 opcode)
{
{{	uae_s16 src = regs.irc;
	do_cycles (2 * 256);
	if (!cctrue(2)) goto didnt_jump;
	if (src & 1) {
		exception3 (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)src);
		goto endlabel4752;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	return 0;
didnt_jump:;
	m68k_incpc (4);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4752: ;
return 0;
}
/* Bcc.B #<data> */
unsigned long REGPARAM2 CPUFUNC(op_6201_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
{{	uae_u32 src = srcreg;
	do_cycles (2 * 256);
	if (!cctrue(2)) goto didnt_jump;
	if (src & 1) {
		exception3 (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)src);
		goto endlabel4753;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	return 0;
didnt_jump:;
	m68k_incpc (2);
	do_cycles (2 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4753: ;
return 0;
}
/* Bcc.L #<data>.L */
unsigned long REGPARAM2 CPUFUNC(op_62ff_6)(uae_u32 opcode)
{
{	do_cycles (2 * 256);
	if (cctrue(2)) {
		exception3 (opcode, m68k_getpc() + 2, m68k_getpc() + 1);
		goto endlabel4754;
	}
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	goto endlabel4754;
{	uae_s32 src;
	src = get_word_ce_prefetch (2) << 16;
	src |= regs.irc;
	do_cycles (2 * 256);
	if (!cctrue(2)) goto didnt_jump;
	if (src & 1) {
		exception3 (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)src);
		goto endlabel4754;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	return 0;
didnt_jump:;
	m68k_incpc (4);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4754: ;
return 0;
}
/* Bcc.W #<data>.W */
unsigned long REGPARAM2 CPUFUNC(op_6300_6)(uae_u32 opcode)
{
{{	uae_s16 src = regs.irc;
	do_cycles (2 * 256);
	if (!cctrue(3)) goto didnt_jump;
	if (src & 1) {
		exception3 (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)src);
		goto endlabel4755;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	return 0;
didnt_jump:;
	m68k_incpc (4);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4755: ;
return 0;
}
/* Bcc.B #<data> */
unsigned long REGPARAM2 CPUFUNC(op_6301_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
{{	uae_u32 src = srcreg;
	do_cycles (2 * 256);
	if (!cctrue(3)) goto didnt_jump;
	if (src & 1) {
		exception3 (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)src);
		goto endlabel4756;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	return 0;
didnt_jump:;
	m68k_incpc (2);
	do_cycles (2 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4756: ;
return 0;
}
/* Bcc.L #<data>.L */
unsigned long REGPARAM2 CPUFUNC(op_63ff_6)(uae_u32 opcode)
{
{	do_cycles (2 * 256);
	if (cctrue(3)) {
		exception3 (opcode, m68k_getpc() + 2, m68k_getpc() + 1);
		goto endlabel4757;
	}
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	goto endlabel4757;
{	uae_s32 src;
	src = get_word_ce_prefetch (2) << 16;
	src |= regs.irc;
	do_cycles (2 * 256);
	if (!cctrue(3)) goto didnt_jump;
	if (src & 1) {
		exception3 (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)src);
		goto endlabel4757;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	return 0;
didnt_jump:;
	m68k_incpc (4);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4757: ;
return 0;
}
/* Bcc.W #<data>.W */
unsigned long REGPARAM2 CPUFUNC(op_6400_6)(uae_u32 opcode)
{
{{	uae_s16 src = regs.irc;
	do_cycles (2 * 256);
	if (!cctrue(4)) goto didnt_jump;
	if (src & 1) {
		exception3 (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)src);
		goto endlabel4758;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	return 0;
didnt_jump:;
	m68k_incpc (4);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4758: ;
return 0;
}
/* Bcc.B #<data> */
unsigned long REGPARAM2 CPUFUNC(op_6401_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
{{	uae_u32 src = srcreg;
	do_cycles (2 * 256);
	if (!cctrue(4)) goto didnt_jump;
	if (src & 1) {
		exception3 (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)src);
		goto endlabel4759;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	return 0;
didnt_jump:;
	m68k_incpc (2);
	do_cycles (2 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4759: ;
return 0;
}
/* Bcc.L #<data>.L */
unsigned long REGPARAM2 CPUFUNC(op_64ff_6)(uae_u32 opcode)
{
{	do_cycles (2 * 256);
	if (cctrue(4)) {
		exception3 (opcode, m68k_getpc() + 2, m68k_getpc() + 1);
		goto endlabel4760;
	}
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	goto endlabel4760;
{	uae_s32 src;
	src = get_word_ce_prefetch (2) << 16;
	src |= regs.irc;
	do_cycles (2 * 256);
	if (!cctrue(4)) goto didnt_jump;
	if (src & 1) {
		exception3 (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)src);
		goto endlabel4760;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	return 0;
didnt_jump:;
	m68k_incpc (4);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4760: ;
return 0;
}
/* Bcc.W #<data>.W */
unsigned long REGPARAM2 CPUFUNC(op_6500_6)(uae_u32 opcode)
{
{{	uae_s16 src = regs.irc;
	do_cycles (2 * 256);
	if (!cctrue(5)) goto didnt_jump;
	if (src & 1) {
		exception3 (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)src);
		goto endlabel4761;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	return 0;
didnt_jump:;
	m68k_incpc (4);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4761: ;
return 0;
}
/* Bcc.B #<data> */
unsigned long REGPARAM2 CPUFUNC(op_6501_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
{{	uae_u32 src = srcreg;
	do_cycles (2 * 256);
	if (!cctrue(5)) goto didnt_jump;
	if (src & 1) {
		exception3 (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)src);
		goto endlabel4762;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	return 0;
didnt_jump:;
	m68k_incpc (2);
	do_cycles (2 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4762: ;
return 0;
}
/* Bcc.L #<data>.L */
unsigned long REGPARAM2 CPUFUNC(op_65ff_6)(uae_u32 opcode)
{
{	do_cycles (2 * 256);
	if (cctrue(5)) {
		exception3 (opcode, m68k_getpc() + 2, m68k_getpc() + 1);
		goto endlabel4763;
	}
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	goto endlabel4763;
{	uae_s32 src;
	src = get_word_ce_prefetch (2) << 16;
	src |= regs.irc;
	do_cycles (2 * 256);
	if (!cctrue(5)) goto didnt_jump;
	if (src & 1) {
		exception3 (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)src);
		goto endlabel4763;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	return 0;
didnt_jump:;
	m68k_incpc (4);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4763: ;
return 0;
}
/* Bcc.W #<data>.W */
unsigned long REGPARAM2 CPUFUNC(op_6600_6)(uae_u32 opcode)
{
{{	uae_s16 src = regs.irc;
	do_cycles (2 * 256);
	if (!cctrue(6)) goto didnt_jump;
	if (src & 1) {
		exception3 (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)src);
		goto endlabel4764;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	return 0;
didnt_jump:;
	m68k_incpc (4);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4764: ;
return 0;
}
/* Bcc.B #<data> */
unsigned long REGPARAM2 CPUFUNC(op_6601_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
{{	uae_u32 src = srcreg;
	do_cycles (2 * 256);
	if (!cctrue(6)) goto didnt_jump;
	if (src & 1) {
		exception3 (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)src);
		goto endlabel4765;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	return 0;
didnt_jump:;
	m68k_incpc (2);
	do_cycles (2 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4765: ;
return 0;
}
/* Bcc.L #<data>.L */
unsigned long REGPARAM2 CPUFUNC(op_66ff_6)(uae_u32 opcode)
{
{	do_cycles (2 * 256);
	if (cctrue(6)) {
		exception3 (opcode, m68k_getpc() + 2, m68k_getpc() + 1);
		goto endlabel4766;
	}
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	goto endlabel4766;
{	uae_s32 src;
	src = get_word_ce_prefetch (2) << 16;
	src |= regs.irc;
	do_cycles (2 * 256);
	if (!cctrue(6)) goto didnt_jump;
	if (src & 1) {
		exception3 (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)src);
		goto endlabel4766;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	return 0;
didnt_jump:;
	m68k_incpc (4);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4766: ;
return 0;
}
/* Bcc.W #<data>.W */
unsigned long REGPARAM2 CPUFUNC(op_6700_6)(uae_u32 opcode)
{
{{	uae_s16 src = regs.irc;
	do_cycles (2 * 256);
	if (!cctrue(7)) goto didnt_jump;
	if (src & 1) {
		exception3 (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)src);
		goto endlabel4767;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	return 0;
didnt_jump:;
	m68k_incpc (4);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4767: ;
return 0;
}
/* Bcc.B #<data> */
unsigned long REGPARAM2 CPUFUNC(op_6701_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
{{	uae_u32 src = srcreg;
	do_cycles (2 * 256);
	if (!cctrue(7)) goto didnt_jump;
	if (src & 1) {
		exception3 (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)src);
		goto endlabel4768;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	return 0;
didnt_jump:;
	m68k_incpc (2);
	do_cycles (2 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4768: ;
return 0;
}
/* Bcc.L #<data>.L */
unsigned long REGPARAM2 CPUFUNC(op_67ff_6)(uae_u32 opcode)
{
{	do_cycles (2 * 256);
	if (cctrue(7)) {
		exception3 (opcode, m68k_getpc() + 2, m68k_getpc() + 1);
		goto endlabel4769;
	}
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	goto endlabel4769;
{	uae_s32 src;
	src = get_word_ce_prefetch (2) << 16;
	src |= regs.irc;
	do_cycles (2 * 256);
	if (!cctrue(7)) goto didnt_jump;
	if (src & 1) {
		exception3 (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)src);
		goto endlabel4769;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	return 0;
didnt_jump:;
	m68k_incpc (4);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4769: ;
return 0;
}
/* Bcc.W #<data>.W */
unsigned long REGPARAM2 CPUFUNC(op_6800_6)(uae_u32 opcode)
{
{{	uae_s16 src = regs.irc;
	do_cycles (2 * 256);
	if (!cctrue(8)) goto didnt_jump;
	if (src & 1) {
		exception3 (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)src);
		goto endlabel4770;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	return 0;
didnt_jump:;
	m68k_incpc (4);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4770: ;
return 0;
}
/* Bcc.B #<data> */
unsigned long REGPARAM2 CPUFUNC(op_6801_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
{{	uae_u32 src = srcreg;
	do_cycles (2 * 256);
	if (!cctrue(8)) goto didnt_jump;
	if (src & 1) {
		exception3 (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)src);
		goto endlabel4771;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	return 0;
didnt_jump:;
	m68k_incpc (2);
	do_cycles (2 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4771: ;
return 0;
}
/* Bcc.L #<data>.L */
unsigned long REGPARAM2 CPUFUNC(op_68ff_6)(uae_u32 opcode)
{
{	do_cycles (2 * 256);
	if (cctrue(8)) {
		exception3 (opcode, m68k_getpc() + 2, m68k_getpc() + 1);
		goto endlabel4772;
	}
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	goto endlabel4772;
{	uae_s32 src;
	src = get_word_ce_prefetch (2) << 16;
	src |= regs.irc;
	do_cycles (2 * 256);
	if (!cctrue(8)) goto didnt_jump;
	if (src & 1) {
		exception3 (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)src);
		goto endlabel4772;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	return 0;
didnt_jump:;
	m68k_incpc (4);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4772: ;
return 0;
}
/* Bcc.W #<data>.W */
unsigned long REGPARAM2 CPUFUNC(op_6900_6)(uae_u32 opcode)
{
{{	uae_s16 src = regs.irc;
	do_cycles (2 * 256);
	if (!cctrue(9)) goto didnt_jump;
	if (src & 1) {
		exception3 (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)src);
		goto endlabel4773;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	return 0;
didnt_jump:;
	m68k_incpc (4);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4773: ;
return 0;
}
/* Bcc.B #<data> */
unsigned long REGPARAM2 CPUFUNC(op_6901_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
{{	uae_u32 src = srcreg;
	do_cycles (2 * 256);
	if (!cctrue(9)) goto didnt_jump;
	if (src & 1) {
		exception3 (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)src);
		goto endlabel4774;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	return 0;
didnt_jump:;
	m68k_incpc (2);
	do_cycles (2 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4774: ;
return 0;
}
/* Bcc.L #<data>.L */
unsigned long REGPARAM2 CPUFUNC(op_69ff_6)(uae_u32 opcode)
{
{	do_cycles (2 * 256);
	if (cctrue(9)) {
		exception3 (opcode, m68k_getpc() + 2, m68k_getpc() + 1);
		goto endlabel4775;
	}
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	goto endlabel4775;
{	uae_s32 src;
	src = get_word_ce_prefetch (2) << 16;
	src |= regs.irc;
	do_cycles (2 * 256);
	if (!cctrue(9)) goto didnt_jump;
	if (src & 1) {
		exception3 (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)src);
		goto endlabel4775;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	return 0;
didnt_jump:;
	m68k_incpc (4);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4775: ;
return 0;
}
/* Bcc.W #<data>.W */
unsigned long REGPARAM2 CPUFUNC(op_6a00_6)(uae_u32 opcode)
{
{{	uae_s16 src = regs.irc;
	do_cycles (2 * 256);
	if (!cctrue(10)) goto didnt_jump;
	if (src & 1) {
		exception3 (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)src);
		goto endlabel4776;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	return 0;
didnt_jump:;
	m68k_incpc (4);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4776: ;
return 0;
}
/* Bcc.B #<data> */
unsigned long REGPARAM2 CPUFUNC(op_6a01_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
{{	uae_u32 src = srcreg;
	do_cycles (2 * 256);
	if (!cctrue(10)) goto didnt_jump;
	if (src & 1) {
		exception3 (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)src);
		goto endlabel4777;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	return 0;
didnt_jump:;
	m68k_incpc (2);
	do_cycles (2 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4777: ;
return 0;
}
/* Bcc.L #<data>.L */
unsigned long REGPARAM2 CPUFUNC(op_6aff_6)(uae_u32 opcode)
{
{	do_cycles (2 * 256);
	if (cctrue(10)) {
		exception3 (opcode, m68k_getpc() + 2, m68k_getpc() + 1);
		goto endlabel4778;
	}
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	goto endlabel4778;
{	uae_s32 src;
	src = get_word_ce_prefetch (2) << 16;
	src |= regs.irc;
	do_cycles (2 * 256);
	if (!cctrue(10)) goto didnt_jump;
	if (src & 1) {
		exception3 (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)src);
		goto endlabel4778;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	return 0;
didnt_jump:;
	m68k_incpc (4);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4778: ;
return 0;
}
/* Bcc.W #<data>.W */
unsigned long REGPARAM2 CPUFUNC(op_6b00_6)(uae_u32 opcode)
{
{{	uae_s16 src = regs.irc;
	do_cycles (2 * 256);
	if (!cctrue(11)) goto didnt_jump;
	if (src & 1) {
		exception3 (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)src);
		goto endlabel4779;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	return 0;
didnt_jump:;
	m68k_incpc (4);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4779: ;
return 0;
}
/* Bcc.B #<data> */
unsigned long REGPARAM2 CPUFUNC(op_6b01_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
{{	uae_u32 src = srcreg;
	do_cycles (2 * 256);
	if (!cctrue(11)) goto didnt_jump;
	if (src & 1) {
		exception3 (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)src);
		goto endlabel4780;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	return 0;
didnt_jump:;
	m68k_incpc (2);
	do_cycles (2 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4780: ;
return 0;
}
/* Bcc.L #<data>.L */
unsigned long REGPARAM2 CPUFUNC(op_6bff_6)(uae_u32 opcode)
{
{	do_cycles (2 * 256);
	if (cctrue(11)) {
		exception3 (opcode, m68k_getpc() + 2, m68k_getpc() + 1);
		goto endlabel4781;
	}
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	goto endlabel4781;
{	uae_s32 src;
	src = get_word_ce_prefetch (2) << 16;
	src |= regs.irc;
	do_cycles (2 * 256);
	if (!cctrue(11)) goto didnt_jump;
	if (src & 1) {
		exception3 (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)src);
		goto endlabel4781;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	return 0;
didnt_jump:;
	m68k_incpc (4);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4781: ;
return 0;
}
/* Bcc.W #<data>.W */
unsigned long REGPARAM2 CPUFUNC(op_6c00_6)(uae_u32 opcode)
{
{{	uae_s16 src = regs.irc;
	do_cycles (2 * 256);
	if (!cctrue(12)) goto didnt_jump;
	if (src & 1) {
		exception3 (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)src);
		goto endlabel4782;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	return 0;
didnt_jump:;
	m68k_incpc (4);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4782: ;
return 0;
}
/* Bcc.B #<data> */
unsigned long REGPARAM2 CPUFUNC(op_6c01_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
{{	uae_u32 src = srcreg;
	do_cycles (2 * 256);
	if (!cctrue(12)) goto didnt_jump;
	if (src & 1) {
		exception3 (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)src);
		goto endlabel4783;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	return 0;
didnt_jump:;
	m68k_incpc (2);
	do_cycles (2 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4783: ;
return 0;
}
/* Bcc.L #<data>.L */
unsigned long REGPARAM2 CPUFUNC(op_6cff_6)(uae_u32 opcode)
{
{	do_cycles (2 * 256);
	if (cctrue(12)) {
		exception3 (opcode, m68k_getpc() + 2, m68k_getpc() + 1);
		goto endlabel4784;
	}
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	goto endlabel4784;
{	uae_s32 src;
	src = get_word_ce_prefetch (2) << 16;
	src |= regs.irc;
	do_cycles (2 * 256);
	if (!cctrue(12)) goto didnt_jump;
	if (src & 1) {
		exception3 (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)src);
		goto endlabel4784;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	return 0;
didnt_jump:;
	m68k_incpc (4);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4784: ;
return 0;
}
/* Bcc.W #<data>.W */
unsigned long REGPARAM2 CPUFUNC(op_6d00_6)(uae_u32 opcode)
{
{{	uae_s16 src = regs.irc;
	do_cycles (2 * 256);
	if (!cctrue(13)) goto didnt_jump;
	if (src & 1) {
		exception3 (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)src);
		goto endlabel4785;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	return 0;
didnt_jump:;
	m68k_incpc (4);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4785: ;
return 0;
}
/* Bcc.B #<data> */
unsigned long REGPARAM2 CPUFUNC(op_6d01_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
{{	uae_u32 src = srcreg;
	do_cycles (2 * 256);
	if (!cctrue(13)) goto didnt_jump;
	if (src & 1) {
		exception3 (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)src);
		goto endlabel4786;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	return 0;
didnt_jump:;
	m68k_incpc (2);
	do_cycles (2 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4786: ;
return 0;
}
/* Bcc.L #<data>.L */
unsigned long REGPARAM2 CPUFUNC(op_6dff_6)(uae_u32 opcode)
{
{	do_cycles (2 * 256);
	if (cctrue(13)) {
		exception3 (opcode, m68k_getpc() + 2, m68k_getpc() + 1);
		goto endlabel4787;
	}
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	goto endlabel4787;
{	uae_s32 src;
	src = get_word_ce_prefetch (2) << 16;
	src |= regs.irc;
	do_cycles (2 * 256);
	if (!cctrue(13)) goto didnt_jump;
	if (src & 1) {
		exception3 (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)src);
		goto endlabel4787;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	return 0;
didnt_jump:;
	m68k_incpc (4);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4787: ;
return 0;
}
/* Bcc.W #<data>.W */
unsigned long REGPARAM2 CPUFUNC(op_6e00_6)(uae_u32 opcode)
{
{{	uae_s16 src = regs.irc;
	do_cycles (2 * 256);
	if (!cctrue(14)) goto didnt_jump;
	if (src & 1) {
		exception3 (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)src);
		goto endlabel4788;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	return 0;
didnt_jump:;
	m68k_incpc (4);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4788: ;
return 0;
}
/* Bcc.B #<data> */
unsigned long REGPARAM2 CPUFUNC(op_6e01_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
{{	uae_u32 src = srcreg;
	do_cycles (2 * 256);
	if (!cctrue(14)) goto didnt_jump;
	if (src & 1) {
		exception3 (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)src);
		goto endlabel4789;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	return 0;
didnt_jump:;
	m68k_incpc (2);
	do_cycles (2 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4789: ;
return 0;
}
/* Bcc.L #<data>.L */
unsigned long REGPARAM2 CPUFUNC(op_6eff_6)(uae_u32 opcode)
{
{	do_cycles (2 * 256);
	if (cctrue(14)) {
		exception3 (opcode, m68k_getpc() + 2, m68k_getpc() + 1);
		goto endlabel4790;
	}
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	goto endlabel4790;
{	uae_s32 src;
	src = get_word_ce_prefetch (2) << 16;
	src |= regs.irc;
	do_cycles (2 * 256);
	if (!cctrue(14)) goto didnt_jump;
	if (src & 1) {
		exception3 (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)src);
		goto endlabel4790;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	return 0;
didnt_jump:;
	m68k_incpc (4);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4790: ;
return 0;
}
/* Bcc.W #<data>.W */
unsigned long REGPARAM2 CPUFUNC(op_6f00_6)(uae_u32 opcode)
{
{{	uae_s16 src = regs.irc;
	do_cycles (2 * 256);
	if (!cctrue(15)) goto didnt_jump;
	if (src & 1) {
		exception3 (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)src);
		goto endlabel4791;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	return 0;
didnt_jump:;
	m68k_incpc (4);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4791: ;
return 0;
}
/* Bcc.B #<data> */
unsigned long REGPARAM2 CPUFUNC(op_6f01_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
{{	uae_u32 src = srcreg;
	do_cycles (2 * 256);
	if (!cctrue(15)) goto didnt_jump;
	if (src & 1) {
		exception3 (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)src);
		goto endlabel4792;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	return 0;
didnt_jump:;
	m68k_incpc (2);
	do_cycles (2 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4792: ;
return 0;
}
/* Bcc.L #<data>.L */
unsigned long REGPARAM2 CPUFUNC(op_6fff_6)(uae_u32 opcode)
{
{	do_cycles (2 * 256);
	if (cctrue(15)) {
		exception3 (opcode, m68k_getpc() + 2, m68k_getpc() + 1);
		goto endlabel4793;
	}
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	goto endlabel4793;
{	uae_s32 src;
	src = get_word_ce_prefetch (2) << 16;
	src |= regs.irc;
	do_cycles (2 * 256);
	if (!cctrue(15)) goto didnt_jump;
	if (src & 1) {
		exception3 (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)src);
		goto endlabel4793;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	return 0;
didnt_jump:;
	m68k_incpc (4);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4793: ;
return 0;
}
/* MOVE.L #<data>,Dn */
unsigned long REGPARAM2 CPUFUNC(op_7000_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_u32 src = srcreg;
{	m68k_dreg(regs, dstreg) = (src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}return 0;
}
/* OR.B Dn,Dn */
unsigned long REGPARAM2 CPUFUNC(op_8000_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	uae_s8 dst = m68k_dreg(regs, dstreg);
	src |= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xff) | ((src) & 0xff);
}}}	m68k_incpc (2);
return 0;
}
/* OR.B (An),Dn */
unsigned long REGPARAM2 CPUFUNC(op_8010_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
{	uae_s8 src = get_byte_ce (srca);
{	uae_s8 dst = m68k_dreg(regs, dstreg);
	src |= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xff) | ((src) & 0xff);
}}}}	m68k_incpc (2);
return 0;
}
/* OR.B (An)+,Dn */
unsigned long REGPARAM2 CPUFUNC(op_8018_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
{	uae_s8 src = get_byte_ce (srca);
	m68k_areg(regs, srcreg) += areg_byteinc[srcreg];
{	uae_s8 dst = m68k_dreg(regs, dstreg);
	src |= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xff) | ((src) & 0xff);
}}}}	m68k_incpc (2);
return 0;
}
/* OR.B -(An),Dn */
unsigned long REGPARAM2 CPUFUNC(op_8020_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = get_byte_ce (srca);
	m68k_areg (regs, srcreg) = srca;
{	uae_s8 dst = m68k_dreg(regs, dstreg);
	src |= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xff) | ((src) & 0xff);
}}}}	m68k_incpc (2);
return 0;
}
/* OR.B (d16,An),Dn */
unsigned long REGPARAM2 CPUFUNC(op_8028_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 src = get_byte_ce (srca);
{	uae_s8 dst = m68k_dreg(regs, dstreg);
	src |= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xff) | ((src) & 0xff);
}}}}	m68k_incpc (4);
return 0;
}
/* OR.B (d8,An,Xn),Dn */
unsigned long REGPARAM2 CPUFUNC(op_8030_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
{	uae_s8 src = get_byte_ce (srca);
{	uae_s8 dst = m68k_dreg(regs, dstreg);
	src |= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xff) | ((src) & 0xff);
}}}}	m68k_incpc (4);
return 0;
}
/* OR.B (xxx).W,Dn */
unsigned long REGPARAM2 CPUFUNC(op_8038_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 src = get_byte_ce (srca);
{	uae_s8 dst = m68k_dreg(regs, dstreg);
	src |= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xff) | ((src) & 0xff);
}}}}	m68k_incpc (4);
return 0;
}
/* OR.B (xxx).L,Dn */
unsigned long REGPARAM2 CPUFUNC(op_8039_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
{	uae_s8 src = get_byte_ce (srca);
{	uae_s8 dst = m68k_dreg(regs, dstreg);
	src |= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xff) | ((src) & 0xff);
}}}}	m68k_incpc (6);
return 0;
}
/* OR.B (d16,PC),Dn */
unsigned long REGPARAM2 CPUFUNC(op_803a_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 src = get_byte_ce (srca);
{	uae_s8 dst = m68k_dreg(regs, dstreg);
	src |= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xff) | ((src) & 0xff);
}}}}	m68k_incpc (4);
return 0;
}
/* OR.B (d8,PC,Xn),Dn */
unsigned long REGPARAM2 CPUFUNC(op_803b_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	do_cycles (2 * 256);
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
{	uae_s8 src = get_byte_ce (srca);
{	uae_s8 dst = m68k_dreg(regs, dstreg);
	src |= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xff) | ((src) & 0xff);
}}}}	m68k_incpc (4);
return 0;
}
/* OR.B #<data>.B,Dn */
unsigned long REGPARAM2 CPUFUNC(op_803c_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s8 src = (uae_u8)get_word_ce_prefetch (4);
{	uae_s8 dst = m68k_dreg(regs, dstreg);
	src |= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xff) | ((src) & 0xff);
}}}	m68k_incpc (4);
return 0;
}
/* OR.W Dn,Dn */
unsigned long REGPARAM2 CPUFUNC(op_8040_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	src |= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xffff) | ((src) & 0xffff);
}}}	m68k_incpc (2);
return 0;
}
/* OR.W (An),Dn */
unsigned long REGPARAM2 CPUFUNC(op_8050_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4807;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	src |= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xffff) | ((src) & 0xffff);
}}}}}	m68k_incpc (2);
endlabel4807: ;
return 0;
}
/* OR.W (An)+,Dn */
unsigned long REGPARAM2 CPUFUNC(op_8058_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4808;
	}
{{	uae_s16 src = get_word_ce (srca);
	m68k_areg(regs, srcreg) += 2;
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	src |= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xffff) | ((src) & 0xffff);
}}}}}	m68k_incpc (2);
endlabel4808: ;
return 0;
}
/* OR.W -(An),Dn */
unsigned long REGPARAM2 CPUFUNC(op_8060_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = m68k_areg(regs, srcreg) - 2;
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4809;
	}
{{	uae_s16 src = get_word_ce (srca);
	m68k_areg (regs, srcreg) = srca;
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	src |= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xffff) | ((src) & 0xffff);
}}}}}	m68k_incpc (2);
endlabel4809: ;
return 0;
}
/* OR.W (d16,An),Dn */
unsigned long REGPARAM2 CPUFUNC(op_8068_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4810;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	src |= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xffff) | ((src) & 0xffff);
}}}}}	m68k_incpc (4);
endlabel4810: ;
return 0;
}
/* OR.W (d8,An,Xn),Dn */
unsigned long REGPARAM2 CPUFUNC(op_8070_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4811;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	src |= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xffff) | ((src) & 0xffff);
}}}}}	m68k_incpc (4);
endlabel4811: ;
return 0;
}
/* OR.W (xxx).W,Dn */
unsigned long REGPARAM2 CPUFUNC(op_8078_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4812;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	src |= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xffff) | ((src) & 0xffff);
}}}}}	m68k_incpc (4);
endlabel4812: ;
return 0;
}
/* OR.W (xxx).L,Dn */
unsigned long REGPARAM2 CPUFUNC(op_8079_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4813;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	src |= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xffff) | ((src) & 0xffff);
}}}}}	m68k_incpc (6);
endlabel4813: ;
return 0;
}
/* OR.W (d16,PC),Dn */
unsigned long REGPARAM2 CPUFUNC(op_807a_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4814;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	src |= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xffff) | ((src) & 0xffff);
}}}}}	m68k_incpc (4);
endlabel4814: ;
return 0;
}
/* OR.W (d8,PC,Xn),Dn */
unsigned long REGPARAM2 CPUFUNC(op_807b_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	do_cycles (2 * 256);
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4815;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	src |= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xffff) | ((src) & 0xffff);
}}}}}	m68k_incpc (4);
endlabel4815: ;
return 0;
}
/* OR.W #<data>.W,Dn */
unsigned long REGPARAM2 CPUFUNC(op_807c_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	src |= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xffff) | ((src) & 0xffff);
}}}	m68k_incpc (4);
return 0;
}
/* OR.L Dn,Dn */
unsigned long REGPARAM2 CPUFUNC(op_8080_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	src |= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	do_cycles (4 * 256);
	m68k_dreg(regs, dstreg) = (src);
}}}	m68k_incpc (2);
return 0;
}
/* OR.L (An),Dn */
unsigned long REGPARAM2 CPUFUNC(op_8090_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4818;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	src |= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	do_cycles (4 * 256);
	m68k_dreg(regs, dstreg) = (src);
}}}}}	m68k_incpc (2);
endlabel4818: ;
return 0;
}
/* OR.L (An)+,Dn */
unsigned long REGPARAM2 CPUFUNC(op_8098_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4819;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	m68k_areg(regs, srcreg) += 4;
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	src |= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	do_cycles (4 * 256);
	m68k_dreg(regs, dstreg) = (src);
}}}}}	m68k_incpc (2);
endlabel4819: ;
return 0;
}
/* OR.L -(An),Dn */
unsigned long REGPARAM2 CPUFUNC(op_80a0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = m68k_areg(regs, srcreg) - 4;
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4820;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	m68k_areg (regs, srcreg) = srca;
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	src |= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	do_cycles (4 * 256);
	m68k_dreg(regs, dstreg) = (src);
}}}}}	m68k_incpc (2);
endlabel4820: ;
return 0;
}
/* OR.L (d16,An),Dn */
unsigned long REGPARAM2 CPUFUNC(op_80a8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4821;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	src |= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	do_cycles (4 * 256);
	m68k_dreg(regs, dstreg) = (src);
}}}}}	m68k_incpc (4);
endlabel4821: ;
return 0;
}
/* OR.L (d8,An,Xn),Dn */
unsigned long REGPARAM2 CPUFUNC(op_80b0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4822;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	src |= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	do_cycles (4 * 256);
	m68k_dreg(regs, dstreg) = (src);
}}}}}	m68k_incpc (4);
endlabel4822: ;
return 0;
}
/* OR.L (xxx).W,Dn */
unsigned long REGPARAM2 CPUFUNC(op_80b8_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4823;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	src |= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	do_cycles (4 * 256);
	m68k_dreg(regs, dstreg) = (src);
}}}}}	m68k_incpc (4);
endlabel4823: ;
return 0;
}
/* OR.L (xxx).L,Dn */
unsigned long REGPARAM2 CPUFUNC(op_80b9_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4824;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	src |= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	do_cycles (4 * 256);
	m68k_dreg(regs, dstreg) = (src);
}}}}}	m68k_incpc (6);
endlabel4824: ;
return 0;
}
/* OR.L (d16,PC),Dn */
unsigned long REGPARAM2 CPUFUNC(op_80ba_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4825;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	src |= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	do_cycles (4 * 256);
	m68k_dreg(regs, dstreg) = (src);
}}}}}	m68k_incpc (4);
endlabel4825: ;
return 0;
}
/* OR.L (d8,PC,Xn),Dn */
unsigned long REGPARAM2 CPUFUNC(op_80bb_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	do_cycles (2 * 256);
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4826;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	src |= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	do_cycles (4 * 256);
	m68k_dreg(regs, dstreg) = (src);
}}}}}	m68k_incpc (4);
endlabel4826: ;
return 0;
}
/* OR.L #<data>.L,Dn */
unsigned long REGPARAM2 CPUFUNC(op_80bc_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	src |= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	do_cycles (4 * 256);
	m68k_dreg(regs, dstreg) = (src);
}}}	m68k_incpc (6);
return 0;
}
/* DIVU.W Dn,Dn */
unsigned long REGPARAM2 CPUFUNC(op_80c0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{	uaecptr oldpc = m68k_getpc();
{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	if (src == 0) {
		SET_VFLG (0);
		Exception (5, oldpc);
		goto endlabel4828;
	} else {
		uae_u32 newv = (uae_u32)dst / (uae_u32)(uae_u16)src;
		uae_u32 rem = (uae_u32)dst % (uae_u32)(uae_u16)src;
		do_cycles (132 * 256);
		if (newv > 0xffff) {
			SET_VFLG (1); SET_NFLG (1); SET_CFLG (0);
		} else {
			CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_NFLG (((uae_s16)(newv)) < 0);
			newv = (newv & 0xffff) | ((uae_u32)rem << 16);
			m68k_dreg(regs, dstreg) = (newv);
		}
	}
}}}endlabel4828: ;
return 0;
}
/* DIVU.W (An),Dn */
unsigned long REGPARAM2 CPUFUNC(op_80d0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{	uaecptr oldpc = m68k_getpc();
{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4829;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	if (src == 0) {
		SET_VFLG (0);
		Exception (5, oldpc);
		goto endlabel4829;
	} else {
		uae_u32 newv = (uae_u32)dst / (uae_u32)(uae_u16)src;
		uae_u32 rem = (uae_u32)dst % (uae_u32)(uae_u16)src;
		do_cycles (132 * 256);
		if (newv > 0xffff) {
			SET_VFLG (1); SET_NFLG (1); SET_CFLG (0);
		} else {
			CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_NFLG (((uae_s16)(newv)) < 0);
			newv = (newv & 0xffff) | ((uae_u32)rem << 16);
			m68k_dreg(regs, dstreg) = (newv);
		}
	}
}}}}}endlabel4829: ;
return 0;
}
/* DIVU.W (An)+,Dn */
unsigned long REGPARAM2 CPUFUNC(op_80d8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{	uaecptr oldpc = m68k_getpc();
{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4830;
	}
{{	uae_s16 src = get_word_ce (srca);
	m68k_areg(regs, srcreg) += 2;
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	if (src == 0) {
		SET_VFLG (0);
		Exception (5, oldpc);
		goto endlabel4830;
	} else {
		uae_u32 newv = (uae_u32)dst / (uae_u32)(uae_u16)src;
		uae_u32 rem = (uae_u32)dst % (uae_u32)(uae_u16)src;
		do_cycles (132 * 256);
		if (newv > 0xffff) {
			SET_VFLG (1); SET_NFLG (1); SET_CFLG (0);
		} else {
			CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_NFLG (((uae_s16)(newv)) < 0);
			newv = (newv & 0xffff) | ((uae_u32)rem << 16);
			m68k_dreg(regs, dstreg) = (newv);
		}
	}
}}}}}endlabel4830: ;
return 0;
}
/* DIVU.W -(An),Dn */
unsigned long REGPARAM2 CPUFUNC(op_80e0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{	uaecptr oldpc = m68k_getpc();
{	uaecptr srca;
	do_cycles (2 * 256);
	srca = m68k_areg(regs, srcreg) - 2;
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4831;
	}
{{	uae_s16 src = get_word_ce (srca);
	m68k_areg (regs, srcreg) = srca;
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	if (src == 0) {
		SET_VFLG (0);
		Exception (5, oldpc);
		goto endlabel4831;
	} else {
		uae_u32 newv = (uae_u32)dst / (uae_u32)(uae_u16)src;
		uae_u32 rem = (uae_u32)dst % (uae_u32)(uae_u16)src;
		do_cycles (132 * 256);
		if (newv > 0xffff) {
			SET_VFLG (1); SET_NFLG (1); SET_CFLG (0);
		} else {
			CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_NFLG (((uae_s16)(newv)) < 0);
			newv = (newv & 0xffff) | ((uae_u32)rem << 16);
			m68k_dreg(regs, dstreg) = (newv);
		}
	}
}}}}}endlabel4831: ;
return 0;
}
/* DIVU.W (d16,An),Dn */
unsigned long REGPARAM2 CPUFUNC(op_80e8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{	uaecptr oldpc = m68k_getpc();
{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4832;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	if (src == 0) {
		SET_VFLG (0);
		Exception (5, oldpc);
		goto endlabel4832;
	} else {
		uae_u32 newv = (uae_u32)dst / (uae_u32)(uae_u16)src;
		uae_u32 rem = (uae_u32)dst % (uae_u32)(uae_u16)src;
		do_cycles (132 * 256);
		if (newv > 0xffff) {
			SET_VFLG (1); SET_NFLG (1); SET_CFLG (0);
		} else {
			CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_NFLG (((uae_s16)(newv)) < 0);
			newv = (newv & 0xffff) | ((uae_u32)rem << 16);
			m68k_dreg(regs, dstreg) = (newv);
		}
	}
}}}}}endlabel4832: ;
return 0;
}
/* DIVU.W (d8,An,Xn),Dn */
unsigned long REGPARAM2 CPUFUNC(op_80f0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{	uaecptr oldpc = m68k_getpc();
{	uaecptr srca;
	do_cycles (2 * 256);
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4833;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	if (src == 0) {
		SET_VFLG (0);
		Exception (5, oldpc);
		goto endlabel4833;
	} else {
		uae_u32 newv = (uae_u32)dst / (uae_u32)(uae_u16)src;
		uae_u32 rem = (uae_u32)dst % (uae_u32)(uae_u16)src;
		do_cycles (132 * 256);
		if (newv > 0xffff) {
			SET_VFLG (1); SET_NFLG (1); SET_CFLG (0);
		} else {
			CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_NFLG (((uae_s16)(newv)) < 0);
			newv = (newv & 0xffff) | ((uae_u32)rem << 16);
			m68k_dreg(regs, dstreg) = (newv);
		}
	}
}}}}}endlabel4833: ;
return 0;
}
/* DIVU.W (xxx).W,Dn */
unsigned long REGPARAM2 CPUFUNC(op_80f8_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{	uaecptr oldpc = m68k_getpc();
{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4834;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	if (src == 0) {
		SET_VFLG (0);
		Exception (5, oldpc);
		goto endlabel4834;
	} else {
		uae_u32 newv = (uae_u32)dst / (uae_u32)(uae_u16)src;
		uae_u32 rem = (uae_u32)dst % (uae_u32)(uae_u16)src;
		do_cycles (132 * 256);
		if (newv > 0xffff) {
			SET_VFLG (1); SET_NFLG (1); SET_CFLG (0);
		} else {
			CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_NFLG (((uae_s16)(newv)) < 0);
			newv = (newv & 0xffff) | ((uae_u32)rem << 16);
			m68k_dreg(regs, dstreg) = (newv);
		}
	}
}}}}}endlabel4834: ;
return 0;
}
/* DIVU.W (xxx).L,Dn */
unsigned long REGPARAM2 CPUFUNC(op_80f9_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{	uaecptr oldpc = m68k_getpc();
{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4835;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	if (src == 0) {
		SET_VFLG (0);
		Exception (5, oldpc);
		goto endlabel4835;
	} else {
		uae_u32 newv = (uae_u32)dst / (uae_u32)(uae_u16)src;
		uae_u32 rem = (uae_u32)dst % (uae_u32)(uae_u16)src;
		do_cycles (132 * 256);
		if (newv > 0xffff) {
			SET_VFLG (1); SET_NFLG (1); SET_CFLG (0);
		} else {
			CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_NFLG (((uae_s16)(newv)) < 0);
			newv = (newv & 0xffff) | ((uae_u32)rem << 16);
			m68k_dreg(regs, dstreg) = (newv);
		}
	}
}}}}}endlabel4835: ;
return 0;
}
/* DIVU.W (d16,PC),Dn */
unsigned long REGPARAM2 CPUFUNC(op_80fa_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{	uaecptr oldpc = m68k_getpc();
{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4836;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	if (src == 0) {
		SET_VFLG (0);
		Exception (5, oldpc);
		goto endlabel4836;
	} else {
		uae_u32 newv = (uae_u32)dst / (uae_u32)(uae_u16)src;
		uae_u32 rem = (uae_u32)dst % (uae_u32)(uae_u16)src;
		do_cycles (132 * 256);
		if (newv > 0xffff) {
			SET_VFLG (1); SET_NFLG (1); SET_CFLG (0);
		} else {
			CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_NFLG (((uae_s16)(newv)) < 0);
			newv = (newv & 0xffff) | ((uae_u32)rem << 16);
			m68k_dreg(regs, dstreg) = (newv);
		}
	}
}}}}}endlabel4836: ;
return 0;
}
/* DIVU.W (d8,PC,Xn),Dn */
unsigned long REGPARAM2 CPUFUNC(op_80fb_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{	uaecptr oldpc = m68k_getpc();
{	uaecptr tmppc;
	uaecptr srca;
	do_cycles (2 * 256);
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4837;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	if (src == 0) {
		SET_VFLG (0);
		Exception (5, oldpc);
		goto endlabel4837;
	} else {
		uae_u32 newv = (uae_u32)dst / (uae_u32)(uae_u16)src;
		uae_u32 rem = (uae_u32)dst % (uae_u32)(uae_u16)src;
		do_cycles (132 * 256);
		if (newv > 0xffff) {
			SET_VFLG (1); SET_NFLG (1); SET_CFLG (0);
		} else {
			CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_NFLG (((uae_s16)(newv)) < 0);
			newv = (newv & 0xffff) | ((uae_u32)rem << 16);
			m68k_dreg(regs, dstreg) = (newv);
		}
	}
}}}}}endlabel4837: ;
return 0;
}
/* DIVU.W #<data>.W,Dn */
unsigned long REGPARAM2 CPUFUNC(op_80fc_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{	uaecptr oldpc = m68k_getpc();
{	uae_s16 src = get_word_ce_prefetch (4);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	if (src == 0) {
		SET_VFLG (0);
		Exception (5, oldpc);
		goto endlabel4838;
	} else {
		uae_u32 newv = (uae_u32)dst / (uae_u32)(uae_u16)src;
		uae_u32 rem = (uae_u32)dst % (uae_u32)(uae_u16)src;
		do_cycles (132 * 256);
		if (newv > 0xffff) {
			SET_VFLG (1); SET_NFLG (1); SET_CFLG (0);
		} else {
			CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_NFLG (((uae_s16)(newv)) < 0);
			newv = (newv & 0xffff) | ((uae_u32)rem << 16);
			m68k_dreg(regs, dstreg) = (newv);
		}
	}
}}}endlabel4838: ;
return 0;
}
/* SBCD.B Dn,Dn */
unsigned long REGPARAM2 CPUFUNC(op_8100_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	uae_s8 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u16 newv_lo = (dst & 0xF) - (src & 0xF) - (GET_XFLG ? 1 : 0);
	uae_u16 newv_hi = (dst & 0xF0) - (src & 0xF0);
	uae_u16 newv, tmp_newv;
	int bcd = 0;
	newv = tmp_newv = newv_hi + newv_lo;
	if (newv_lo & 0xF0) { newv -= 6; bcd = 6; };
	if ((((dst & 0xFF) - (src & 0xFF) - (GET_XFLG ? 1 : 0)) & 0x100) > 0xFF) { newv -= 0x60; }
	SET_CFLG ((((dst & 0xFF) - (src & 0xFF) - bcd - (GET_XFLG ? 1 : 0)) & 0x300) > 0xFF);
	COPY_CARRY;
	SET_ZFLG (GET_ZFLG & (((uae_s8)(newv)) == 0));
	SET_NFLG (((uae_s8)(newv)) < 0);
	SET_VFLG ((tmp_newv & 0x80) != 0 && (newv & 0x80) == 0);
	do_cycles (2 * 256);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xff) | ((newv) & 0xff);
}}}}	m68k_incpc (2);
return 0;
}
/* SBCD.B -(An),-(An) */
unsigned long REGPARAM2 CPUFUNC(op_8108_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = get_byte_ce (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = get_byte_ce (dsta);
	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u16 newv_lo = (dst & 0xF) - (src & 0xF) - (GET_XFLG ? 1 : 0);
	uae_u16 newv_hi = (dst & 0xF0) - (src & 0xF0);
	uae_u16 newv, tmp_newv;
	int bcd = 0;
	newv = tmp_newv = newv_hi + newv_lo;
	if (newv_lo & 0xF0) { newv -= 6; bcd = 6; };
	if ((((dst & 0xFF) - (src & 0xFF) - (GET_XFLG ? 1 : 0)) & 0x100) > 0xFF) { newv -= 0x60; }
	SET_CFLG ((((dst & 0xFF) - (src & 0xFF) - bcd - (GET_XFLG ? 1 : 0)) & 0x300) > 0xFF);
	COPY_CARRY;
	SET_ZFLG (GET_ZFLG & (((uae_s8)(newv)) == 0));
	SET_NFLG (((uae_s8)(newv)) < 0);
	SET_VFLG ((tmp_newv & 0x80) != 0 && (newv & 0x80) == 0);
	do_cycles (2 * 256);
	put_byte_ce (dsta,newv);
}}}}}}	m68k_incpc (2);
return 0;
}
/* OR.B Dn,(An) */
unsigned long REGPARAM2 CPUFUNC(op_8110_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = m68k_areg(regs, dstreg);
{	uae_s8 dst = get_byte_ce (dsta);
	src |= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	put_byte_ce (dsta,src);
}}}}	m68k_incpc (2);
return 0;
}
/* OR.B Dn,(An)+ */
unsigned long REGPARAM2 CPUFUNC(op_8118_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = m68k_areg(regs, dstreg);
{	uae_s8 dst = get_byte_ce (dsta);
	m68k_areg(regs, dstreg) += areg_byteinc[dstreg];
	src |= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	put_byte_ce (dsta,src);
}}}}	m68k_incpc (2);
return 0;
}
/* OR.B Dn,-(An) */
unsigned long REGPARAM2 CPUFUNC(op_8120_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = m68k_areg(regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = get_byte_ce (dsta);
	m68k_areg (regs, dstreg) = dsta;
	src |= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	put_byte_ce (dsta,src);
}}}}	m68k_incpc (2);
return 0;
}
/* OR.B Dn,(d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_8128_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 dst = get_byte_ce (dsta);
	src |= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	put_byte_ce (dsta,src);
}}}}	m68k_incpc (4);
return 0;
}
/* OR.B Dn,(d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_8130_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (4));
{	uae_s8 dst = get_byte_ce (dsta);
	src |= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	put_byte_ce (dsta,src);
}}}}	m68k_incpc (4);
return 0;
}
/* OR.B Dn,(xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_8138_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 dst = get_byte_ce (dsta);
	src |= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	put_byte_ce (dsta,src);
}}}}	m68k_incpc (4);
return 0;
}
/* OR.B Dn,(xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_8139_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (4) << 16;
	dsta |= get_word_ce_prefetch (6);
{	uae_s8 dst = get_byte_ce (dsta);
	src |= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	put_byte_ce (dsta,src);
}}}}	m68k_incpc (6);
return 0;
}
/* OR.W Dn,(An) */
unsigned long REGPARAM2 CPUFUNC(op_8150_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4848;
	}
{{	uae_s16 dst = get_word_ce (dsta);
	src |= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	put_word_ce (dsta,src);
}}}}}	m68k_incpc (2);
endlabel4848: ;
return 0;
}
/* OR.W Dn,(An)+ */
unsigned long REGPARAM2 CPUFUNC(op_8158_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4849;
	}
{{	uae_s16 dst = get_word_ce (dsta);
	m68k_areg(regs, dstreg) += 2;
	src |= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	put_word_ce (dsta,src);
}}}}}	m68k_incpc (2);
endlabel4849: ;
return 0;
}
/* OR.W Dn,-(An) */
unsigned long REGPARAM2 CPUFUNC(op_8160_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = m68k_areg(regs, dstreg) - 2;
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4850;
	}
{{	uae_s16 dst = get_word_ce (dsta);
	m68k_areg (regs, dstreg) = dsta;
	src |= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	put_word_ce (dsta,src);
}}}}}	m68k_incpc (2);
endlabel4850: ;
return 0;
}
/* OR.W Dn,(d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_8168_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4851;
	}
{{	uae_s16 dst = get_word_ce (dsta);
	src |= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	put_word_ce (dsta,src);
}}}}}	m68k_incpc (4);
endlabel4851: ;
return 0;
}
/* OR.W Dn,(d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_8170_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (4));
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4852;
	}
{{	uae_s16 dst = get_word_ce (dsta);
	src |= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	put_word_ce (dsta,src);
}}}}}	m68k_incpc (4);
endlabel4852: ;
return 0;
}
/* OR.W Dn,(xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_8178_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4853;
	}
{{	uae_s16 dst = get_word_ce (dsta);
	src |= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	put_word_ce (dsta,src);
}}}}}	m68k_incpc (4);
endlabel4853: ;
return 0;
}
/* OR.W Dn,(xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_8179_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (4) << 16;
	dsta |= get_word_ce_prefetch (6);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4854;
	}
{{	uae_s16 dst = get_word_ce (dsta);
	src |= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	put_word_ce (dsta,src);
}}}}}	m68k_incpc (6);
endlabel4854: ;
return 0;
}
/* OR.L Dn,(An) */
unsigned long REGPARAM2 CPUFUNC(op_8190_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4855;
	}
{{	uae_s32 dst = get_word_ce (dsta) << 16; dst |= get_word_ce (dsta + 2);
	src |= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
}}}}}	m68k_incpc (2);
endlabel4855: ;
return 0;
}
/* OR.L Dn,(An)+ */
unsigned long REGPARAM2 CPUFUNC(op_8198_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4856;
	}
{{	uae_s32 dst = get_word_ce (dsta) << 16; dst |= get_word_ce (dsta + 2);
	m68k_areg(regs, dstreg) += 4;
	src |= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
}}}}}	m68k_incpc (2);
endlabel4856: ;
return 0;
}
/* OR.L Dn,-(An) */
unsigned long REGPARAM2 CPUFUNC(op_81a0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = m68k_areg(regs, dstreg) - 4;
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4857;
	}
{{	uae_s32 dst = get_word_ce (dsta) << 16; dst |= get_word_ce (dsta + 2);
	m68k_areg (regs, dstreg) = dsta;
	src |= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
}}}}}	m68k_incpc (2);
endlabel4857: ;
return 0;
}
/* OR.L Dn,(d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_81a8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4858;
	}
{{	uae_s32 dst = get_word_ce (dsta) << 16; dst |= get_word_ce (dsta + 2);
	src |= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
}}}}}	m68k_incpc (4);
endlabel4858: ;
return 0;
}
/* OR.L Dn,(d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_81b0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (4));
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4859;
	}
{{	uae_s32 dst = get_word_ce (dsta) << 16; dst |= get_word_ce (dsta + 2);
	src |= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
}}}}}	m68k_incpc (4);
endlabel4859: ;
return 0;
}
/* OR.L Dn,(xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_81b8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4860;
	}
{{	uae_s32 dst = get_word_ce (dsta) << 16; dst |= get_word_ce (dsta + 2);
	src |= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
}}}}}	m68k_incpc (4);
endlabel4860: ;
return 0;
}
/* OR.L Dn,(xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_81b9_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (4) << 16;
	dsta |= get_word_ce_prefetch (6);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4861;
	}
{{	uae_s32 dst = get_word_ce (dsta) << 16; dst |= get_word_ce (dsta + 2);
	src |= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
}}}}}	m68k_incpc (6);
endlabel4861: ;
return 0;
}
/* DIVS.W Dn,Dn */
unsigned long REGPARAM2 CPUFUNC(op_81c0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{	uaecptr oldpc = m68k_getpc();
{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	if (src == 0) {
		SET_VFLG (0);
		Exception (5, oldpc);
		goto endlabel4862;
	} else {
		uae_s32 newv = (uae_s32)dst / (uae_s32)(uae_s16)src;
		uae_u16 rem = (uae_s32)dst % (uae_s32)(uae_s16)src;
		do_cycles (150 * 256);
		if ((newv & 0xffff8000) != 0 && (newv & 0xffff8000) != 0xffff8000) {
			SET_VFLG (1); SET_NFLG (1); SET_CFLG (0);
		} else {
			if (((uae_s16)rem < 0) != ((uae_s32)dst < 0)) rem = -rem;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_NFLG (((uae_s16)(newv)) < 0);
			newv = (newv & 0xffff) | ((uae_u32)rem << 16);
			m68k_dreg(regs, dstreg) = (newv);
		}
	}
}}}endlabel4862: ;
return 0;
}
/* DIVS.W (An),Dn */
unsigned long REGPARAM2 CPUFUNC(op_81d0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{	uaecptr oldpc = m68k_getpc();
{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4863;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	if (src == 0) {
		SET_VFLG (0);
		Exception (5, oldpc);
		goto endlabel4863;
	} else {
		uae_s32 newv = (uae_s32)dst / (uae_s32)(uae_s16)src;
		uae_u16 rem = (uae_s32)dst % (uae_s32)(uae_s16)src;
		do_cycles (150 * 256);
		if ((newv & 0xffff8000) != 0 && (newv & 0xffff8000) != 0xffff8000) {
			SET_VFLG (1); SET_NFLG (1); SET_CFLG (0);
		} else {
			if (((uae_s16)rem < 0) != ((uae_s32)dst < 0)) rem = -rem;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_NFLG (((uae_s16)(newv)) < 0);
			newv = (newv & 0xffff) | ((uae_u32)rem << 16);
			m68k_dreg(regs, dstreg) = (newv);
		}
	}
}}}}}endlabel4863: ;
return 0;
}
/* DIVS.W (An)+,Dn */
unsigned long REGPARAM2 CPUFUNC(op_81d8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{	uaecptr oldpc = m68k_getpc();
{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4864;
	}
{{	uae_s16 src = get_word_ce (srca);
	m68k_areg(regs, srcreg) += 2;
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	if (src == 0) {
		SET_VFLG (0);
		Exception (5, oldpc);
		goto endlabel4864;
	} else {
		uae_s32 newv = (uae_s32)dst / (uae_s32)(uae_s16)src;
		uae_u16 rem = (uae_s32)dst % (uae_s32)(uae_s16)src;
		do_cycles (150 * 256);
		if ((newv & 0xffff8000) != 0 && (newv & 0xffff8000) != 0xffff8000) {
			SET_VFLG (1); SET_NFLG (1); SET_CFLG (0);
		} else {
			if (((uae_s16)rem < 0) != ((uae_s32)dst < 0)) rem = -rem;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_NFLG (((uae_s16)(newv)) < 0);
			newv = (newv & 0xffff) | ((uae_u32)rem << 16);
			m68k_dreg(regs, dstreg) = (newv);
		}
	}
}}}}}endlabel4864: ;
return 0;
}
/* DIVS.W -(An),Dn */
unsigned long REGPARAM2 CPUFUNC(op_81e0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{	uaecptr oldpc = m68k_getpc();
{	uaecptr srca;
	do_cycles (2 * 256);
	srca = m68k_areg(regs, srcreg) - 2;
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4865;
	}
{{	uae_s16 src = get_word_ce (srca);
	m68k_areg (regs, srcreg) = srca;
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	if (src == 0) {
		SET_VFLG (0);
		Exception (5, oldpc);
		goto endlabel4865;
	} else {
		uae_s32 newv = (uae_s32)dst / (uae_s32)(uae_s16)src;
		uae_u16 rem = (uae_s32)dst % (uae_s32)(uae_s16)src;
		do_cycles (150 * 256);
		if ((newv & 0xffff8000) != 0 && (newv & 0xffff8000) != 0xffff8000) {
			SET_VFLG (1); SET_NFLG (1); SET_CFLG (0);
		} else {
			if (((uae_s16)rem < 0) != ((uae_s32)dst < 0)) rem = -rem;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_NFLG (((uae_s16)(newv)) < 0);
			newv = (newv & 0xffff) | ((uae_u32)rem << 16);
			m68k_dreg(regs, dstreg) = (newv);
		}
	}
}}}}}endlabel4865: ;
return 0;
}
/* DIVS.W (d16,An),Dn */
unsigned long REGPARAM2 CPUFUNC(op_81e8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{	uaecptr oldpc = m68k_getpc();
{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4866;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	if (src == 0) {
		SET_VFLG (0);
		Exception (5, oldpc);
		goto endlabel4866;
	} else {
		uae_s32 newv = (uae_s32)dst / (uae_s32)(uae_s16)src;
		uae_u16 rem = (uae_s32)dst % (uae_s32)(uae_s16)src;
		do_cycles (150 * 256);
		if ((newv & 0xffff8000) != 0 && (newv & 0xffff8000) != 0xffff8000) {
			SET_VFLG (1); SET_NFLG (1); SET_CFLG (0);
		} else {
			if (((uae_s16)rem < 0) != ((uae_s32)dst < 0)) rem = -rem;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_NFLG (((uae_s16)(newv)) < 0);
			newv = (newv & 0xffff) | ((uae_u32)rem << 16);
			m68k_dreg(regs, dstreg) = (newv);
		}
	}
}}}}}endlabel4866: ;
return 0;
}
/* DIVS.W (d8,An,Xn),Dn */
unsigned long REGPARAM2 CPUFUNC(op_81f0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{	uaecptr oldpc = m68k_getpc();
{	uaecptr srca;
	do_cycles (2 * 256);
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4867;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	if (src == 0) {
		SET_VFLG (0);
		Exception (5, oldpc);
		goto endlabel4867;
	} else {
		uae_s32 newv = (uae_s32)dst / (uae_s32)(uae_s16)src;
		uae_u16 rem = (uae_s32)dst % (uae_s32)(uae_s16)src;
		do_cycles (150 * 256);
		if ((newv & 0xffff8000) != 0 && (newv & 0xffff8000) != 0xffff8000) {
			SET_VFLG (1); SET_NFLG (1); SET_CFLG (0);
		} else {
			if (((uae_s16)rem < 0) != ((uae_s32)dst < 0)) rem = -rem;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_NFLG (((uae_s16)(newv)) < 0);
			newv = (newv & 0xffff) | ((uae_u32)rem << 16);
			m68k_dreg(regs, dstreg) = (newv);
		}
	}
}}}}}endlabel4867: ;
return 0;
}
/* DIVS.W (xxx).W,Dn */
unsigned long REGPARAM2 CPUFUNC(op_81f8_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{	uaecptr oldpc = m68k_getpc();
{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4868;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	if (src == 0) {
		SET_VFLG (0);
		Exception (5, oldpc);
		goto endlabel4868;
	} else {
		uae_s32 newv = (uae_s32)dst / (uae_s32)(uae_s16)src;
		uae_u16 rem = (uae_s32)dst % (uae_s32)(uae_s16)src;
		do_cycles (150 * 256);
		if ((newv & 0xffff8000) != 0 && (newv & 0xffff8000) != 0xffff8000) {
			SET_VFLG (1); SET_NFLG (1); SET_CFLG (0);
		} else {
			if (((uae_s16)rem < 0) != ((uae_s32)dst < 0)) rem = -rem;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_NFLG (((uae_s16)(newv)) < 0);
			newv = (newv & 0xffff) | ((uae_u32)rem << 16);
			m68k_dreg(regs, dstreg) = (newv);
		}
	}
}}}}}endlabel4868: ;
return 0;
}
/* DIVS.W (xxx).L,Dn */
unsigned long REGPARAM2 CPUFUNC(op_81f9_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{	uaecptr oldpc = m68k_getpc();
{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4869;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	if (src == 0) {
		SET_VFLG (0);
		Exception (5, oldpc);
		goto endlabel4869;
	} else {
		uae_s32 newv = (uae_s32)dst / (uae_s32)(uae_s16)src;
		uae_u16 rem = (uae_s32)dst % (uae_s32)(uae_s16)src;
		do_cycles (150 * 256);
		if ((newv & 0xffff8000) != 0 && (newv & 0xffff8000) != 0xffff8000) {
			SET_VFLG (1); SET_NFLG (1); SET_CFLG (0);
		} else {
			if (((uae_s16)rem < 0) != ((uae_s32)dst < 0)) rem = -rem;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_NFLG (((uae_s16)(newv)) < 0);
			newv = (newv & 0xffff) | ((uae_u32)rem << 16);
			m68k_dreg(regs, dstreg) = (newv);
		}
	}
}}}}}endlabel4869: ;
return 0;
}
/* DIVS.W (d16,PC),Dn */
unsigned long REGPARAM2 CPUFUNC(op_81fa_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{	uaecptr oldpc = m68k_getpc();
{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4870;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	if (src == 0) {
		SET_VFLG (0);
		Exception (5, oldpc);
		goto endlabel4870;
	} else {
		uae_s32 newv = (uae_s32)dst / (uae_s32)(uae_s16)src;
		uae_u16 rem = (uae_s32)dst % (uae_s32)(uae_s16)src;
		do_cycles (150 * 256);
		if ((newv & 0xffff8000) != 0 && (newv & 0xffff8000) != 0xffff8000) {
			SET_VFLG (1); SET_NFLG (1); SET_CFLG (0);
		} else {
			if (((uae_s16)rem < 0) != ((uae_s32)dst < 0)) rem = -rem;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_NFLG (((uae_s16)(newv)) < 0);
			newv = (newv & 0xffff) | ((uae_u32)rem << 16);
			m68k_dreg(regs, dstreg) = (newv);
		}
	}
}}}}}endlabel4870: ;
return 0;
}
/* DIVS.W (d8,PC,Xn),Dn */
unsigned long REGPARAM2 CPUFUNC(op_81fb_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{	uaecptr oldpc = m68k_getpc();
{	uaecptr tmppc;
	uaecptr srca;
	do_cycles (2 * 256);
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4871;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	if (src == 0) {
		SET_VFLG (0);
		Exception (5, oldpc);
		goto endlabel4871;
	} else {
		uae_s32 newv = (uae_s32)dst / (uae_s32)(uae_s16)src;
		uae_u16 rem = (uae_s32)dst % (uae_s32)(uae_s16)src;
		do_cycles (150 * 256);
		if ((newv & 0xffff8000) != 0 && (newv & 0xffff8000) != 0xffff8000) {
			SET_VFLG (1); SET_NFLG (1); SET_CFLG (0);
		} else {
			if (((uae_s16)rem < 0) != ((uae_s32)dst < 0)) rem = -rem;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_NFLG (((uae_s16)(newv)) < 0);
			newv = (newv & 0xffff) | ((uae_u32)rem << 16);
			m68k_dreg(regs, dstreg) = (newv);
		}
	}
}}}}}endlabel4871: ;
return 0;
}
/* DIVS.W #<data>.W,Dn */
unsigned long REGPARAM2 CPUFUNC(op_81fc_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{	uaecptr oldpc = m68k_getpc();
{	uae_s16 src = get_word_ce_prefetch (4);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	if (src == 0) {
		SET_VFLG (0);
		Exception (5, oldpc);
		goto endlabel4872;
	} else {
		uae_s32 newv = (uae_s32)dst / (uae_s32)(uae_s16)src;
		uae_u16 rem = (uae_s32)dst % (uae_s32)(uae_s16)src;
		do_cycles (150 * 256);
		if ((newv & 0xffff8000) != 0 && (newv & 0xffff8000) != 0xffff8000) {
			SET_VFLG (1); SET_NFLG (1); SET_CFLG (0);
		} else {
			if (((uae_s16)rem < 0) != ((uae_s32)dst < 0)) rem = -rem;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_NFLG (((uae_s16)(newv)) < 0);
			newv = (newv & 0xffff) | ((uae_u32)rem << 16);
			m68k_dreg(regs, dstreg) = (newv);
		}
	}
}}}endlabel4872: ;
return 0;
}
/* SUB.B Dn,Dn */
unsigned long REGPARAM2 CPUFUNC(op_9000_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	uae_s8 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xff) | ((newv) & 0xff);
}}}}}}	m68k_incpc (2);
return 0;
}
/* SUB.B (An),Dn */
unsigned long REGPARAM2 CPUFUNC(op_9010_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
{	uae_s8 src = get_byte_ce (srca);
{	uae_s8 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xff) | ((newv) & 0xff);
}}}}}}}	m68k_incpc (2);
return 0;
}
/* SUB.B (An)+,Dn */
unsigned long REGPARAM2 CPUFUNC(op_9018_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
{	uae_s8 src = get_byte_ce (srca);
	m68k_areg(regs, srcreg) += areg_byteinc[srcreg];
{	uae_s8 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xff) | ((newv) & 0xff);
}}}}}}}	m68k_incpc (2);
return 0;
}
/* SUB.B -(An),Dn */
unsigned long REGPARAM2 CPUFUNC(op_9020_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = get_byte_ce (srca);
	m68k_areg (regs, srcreg) = srca;
{	uae_s8 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xff) | ((newv) & 0xff);
}}}}}}}	m68k_incpc (2);
return 0;
}
/* SUB.B (d16,An),Dn */
unsigned long REGPARAM2 CPUFUNC(op_9028_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 src = get_byte_ce (srca);
{	uae_s8 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xff) | ((newv) & 0xff);
}}}}}}}	m68k_incpc (4);
return 0;
}
/* SUB.B (d8,An,Xn),Dn */
unsigned long REGPARAM2 CPUFUNC(op_9030_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
{	uae_s8 src = get_byte_ce (srca);
{	uae_s8 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xff) | ((newv) & 0xff);
}}}}}}}	m68k_incpc (4);
return 0;
}
/* SUB.B (xxx).W,Dn */
unsigned long REGPARAM2 CPUFUNC(op_9038_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 src = get_byte_ce (srca);
{	uae_s8 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xff) | ((newv) & 0xff);
}}}}}}}	m68k_incpc (4);
return 0;
}
/* SUB.B (xxx).L,Dn */
unsigned long REGPARAM2 CPUFUNC(op_9039_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
{	uae_s8 src = get_byte_ce (srca);
{	uae_s8 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xff) | ((newv) & 0xff);
}}}}}}}	m68k_incpc (6);
return 0;
}
/* SUB.B (d16,PC),Dn */
unsigned long REGPARAM2 CPUFUNC(op_903a_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 src = get_byte_ce (srca);
{	uae_s8 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xff) | ((newv) & 0xff);
}}}}}}}	m68k_incpc (4);
return 0;
}
/* SUB.B (d8,PC,Xn),Dn */
unsigned long REGPARAM2 CPUFUNC(op_903b_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	do_cycles (2 * 256);
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
{	uae_s8 src = get_byte_ce (srca);
{	uae_s8 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xff) | ((newv) & 0xff);
}}}}}}}	m68k_incpc (4);
return 0;
}
/* SUB.B #<data>.B,Dn */
unsigned long REGPARAM2 CPUFUNC(op_903c_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s8 src = (uae_u8)get_word_ce_prefetch (4);
{	uae_s8 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xff) | ((newv) & 0xff);
}}}}}}	m68k_incpc (4);
return 0;
}
/* SUB.W Dn,Dn */
unsigned long REGPARAM2 CPUFUNC(op_9040_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xffff) | ((newv) & 0xffff);
}}}}}}	m68k_incpc (2);
return 0;
}
/* SUB.W An,Dn */
unsigned long REGPARAM2 CPUFUNC(op_9048_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_areg(regs, srcreg);
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xffff) | ((newv) & 0xffff);
}}}}}}	m68k_incpc (2);
return 0;
}
/* SUB.W (An),Dn */
unsigned long REGPARAM2 CPUFUNC(op_9050_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4886;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xffff) | ((newv) & 0xffff);
}}}}}}}}	m68k_incpc (2);
endlabel4886: ;
return 0;
}
/* SUB.W (An)+,Dn */
unsigned long REGPARAM2 CPUFUNC(op_9058_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4887;
	}
{{	uae_s16 src = get_word_ce (srca);
	m68k_areg(regs, srcreg) += 2;
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xffff) | ((newv) & 0xffff);
}}}}}}}}	m68k_incpc (2);
endlabel4887: ;
return 0;
}
/* SUB.W -(An),Dn */
unsigned long REGPARAM2 CPUFUNC(op_9060_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = m68k_areg(regs, srcreg) - 2;
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4888;
	}
{{	uae_s16 src = get_word_ce (srca);
	m68k_areg (regs, srcreg) = srca;
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xffff) | ((newv) & 0xffff);
}}}}}}}}	m68k_incpc (2);
endlabel4888: ;
return 0;
}
/* SUB.W (d16,An),Dn */
unsigned long REGPARAM2 CPUFUNC(op_9068_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4889;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xffff) | ((newv) & 0xffff);
}}}}}}}}	m68k_incpc (4);
endlabel4889: ;
return 0;
}
/* SUB.W (d8,An,Xn),Dn */
unsigned long REGPARAM2 CPUFUNC(op_9070_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4890;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xffff) | ((newv) & 0xffff);
}}}}}}}}	m68k_incpc (4);
endlabel4890: ;
return 0;
}
/* SUB.W (xxx).W,Dn */
unsigned long REGPARAM2 CPUFUNC(op_9078_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4891;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xffff) | ((newv) & 0xffff);
}}}}}}}}	m68k_incpc (4);
endlabel4891: ;
return 0;
}
/* SUB.W (xxx).L,Dn */
unsigned long REGPARAM2 CPUFUNC(op_9079_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4892;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xffff) | ((newv) & 0xffff);
}}}}}}}}	m68k_incpc (6);
endlabel4892: ;
return 0;
}
/* SUB.W (d16,PC),Dn */
unsigned long REGPARAM2 CPUFUNC(op_907a_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4893;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xffff) | ((newv) & 0xffff);
}}}}}}}}	m68k_incpc (4);
endlabel4893: ;
return 0;
}
/* SUB.W (d8,PC,Xn),Dn */
unsigned long REGPARAM2 CPUFUNC(op_907b_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	do_cycles (2 * 256);
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4894;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xffff) | ((newv) & 0xffff);
}}}}}}}}	m68k_incpc (4);
endlabel4894: ;
return 0;
}
/* SUB.W #<data>.W,Dn */
unsigned long REGPARAM2 CPUFUNC(op_907c_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xffff) | ((newv) & 0xffff);
}}}}}}	m68k_incpc (4);
return 0;
}
/* SUB.L Dn,Dn */
unsigned long REGPARAM2 CPUFUNC(op_9080_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	do_cycles (2 * 256);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	m68k_dreg(regs, dstreg) = (newv);
}}}}}}	m68k_incpc (2);
return 0;
}
/* SUB.L An,Dn */
unsigned long REGPARAM2 CPUFUNC(op_9088_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_areg(regs, srcreg);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	do_cycles (2 * 256);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	m68k_dreg(regs, dstreg) = (newv);
}}}}}}	m68k_incpc (2);
return 0;
}
/* SUB.L (An),Dn */
unsigned long REGPARAM2 CPUFUNC(op_9090_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4898;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	do_cycles (2 * 256);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	m68k_dreg(regs, dstreg) = (newv);
}}}}}}}}	m68k_incpc (2);
endlabel4898: ;
return 0;
}
/* SUB.L (An)+,Dn */
unsigned long REGPARAM2 CPUFUNC(op_9098_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4899;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	m68k_areg(regs, srcreg) += 4;
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	do_cycles (2 * 256);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	m68k_dreg(regs, dstreg) = (newv);
}}}}}}}}	m68k_incpc (2);
endlabel4899: ;
return 0;
}
/* SUB.L -(An),Dn */
unsigned long REGPARAM2 CPUFUNC(op_90a0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = m68k_areg(regs, srcreg) - 4;
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4900;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	m68k_areg (regs, srcreg) = srca;
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	do_cycles (2 * 256);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	m68k_dreg(regs, dstreg) = (newv);
}}}}}}}}	m68k_incpc (2);
endlabel4900: ;
return 0;
}
/* SUB.L (d16,An),Dn */
unsigned long REGPARAM2 CPUFUNC(op_90a8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4901;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	do_cycles (2 * 256);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	m68k_dreg(regs, dstreg) = (newv);
}}}}}}}}	m68k_incpc (4);
endlabel4901: ;
return 0;
}
/* SUB.L (d8,An,Xn),Dn */
unsigned long REGPARAM2 CPUFUNC(op_90b0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4902;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	do_cycles (2 * 256);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	m68k_dreg(regs, dstreg) = (newv);
}}}}}}}}	m68k_incpc (4);
endlabel4902: ;
return 0;
}
/* SUB.L (xxx).W,Dn */
unsigned long REGPARAM2 CPUFUNC(op_90b8_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4903;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	do_cycles (2 * 256);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	m68k_dreg(regs, dstreg) = (newv);
}}}}}}}}	m68k_incpc (4);
endlabel4903: ;
return 0;
}
/* SUB.L (xxx).L,Dn */
unsigned long REGPARAM2 CPUFUNC(op_90b9_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4904;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	do_cycles (2 * 256);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	m68k_dreg(regs, dstreg) = (newv);
}}}}}}}}	m68k_incpc (6);
endlabel4904: ;
return 0;
}
/* SUB.L (d16,PC),Dn */
unsigned long REGPARAM2 CPUFUNC(op_90ba_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4905;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	do_cycles (2 * 256);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	m68k_dreg(regs, dstreg) = (newv);
}}}}}}}}	m68k_incpc (4);
endlabel4905: ;
return 0;
}
/* SUB.L (d8,PC,Xn),Dn */
unsigned long REGPARAM2 CPUFUNC(op_90bb_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	do_cycles (2 * 256);
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4906;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	do_cycles (2 * 256);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	m68k_dreg(regs, dstreg) = (newv);
}}}}}}}}	m68k_incpc (4);
endlabel4906: ;
return 0;
}
/* SUB.L #<data>.L,Dn */
unsigned long REGPARAM2 CPUFUNC(op_90bc_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	do_cycles (4 * 256);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	m68k_dreg(regs, dstreg) = (newv);
}}}}}}	m68k_incpc (6);
return 0;
}
/* SUBA.W Dn,An */
unsigned long REGPARAM2 CPUFUNC(op_90c0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	do_cycles (2 * 256);
{	uae_u32 newv = dst - src;
	m68k_areg(regs, dstreg) = (newv);
}}}}	m68k_incpc (2);
return 0;
}
/* SUBA.W An,An */
unsigned long REGPARAM2 CPUFUNC(op_90c8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_areg(regs, srcreg);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	do_cycles (2 * 256);
{	uae_u32 newv = dst - src;
	m68k_areg(regs, dstreg) = (newv);
}}}}	m68k_incpc (2);
return 0;
}
/* SUBA.W (An),An */
unsigned long REGPARAM2 CPUFUNC(op_90d0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4910;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	do_cycles (2 * 256);
{	uae_u32 newv = dst - src;
	m68k_areg(regs, dstreg) = (newv);
}}}}}}	m68k_incpc (2);
endlabel4910: ;
return 0;
}
/* SUBA.W (An)+,An */
unsigned long REGPARAM2 CPUFUNC(op_90d8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4911;
	}
{{	uae_s16 src = get_word_ce (srca);
	m68k_areg(regs, srcreg) += 2;
{	uae_s32 dst = m68k_areg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	do_cycles (2 * 256);
{	uae_u32 newv = dst - src;
	m68k_areg(regs, dstreg) = (newv);
}}}}}}	m68k_incpc (2);
endlabel4911: ;
return 0;
}
/* SUBA.W -(An),An */
unsigned long REGPARAM2 CPUFUNC(op_90e0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = m68k_areg(regs, srcreg) - 2;
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4912;
	}
{{	uae_s16 src = get_word_ce (srca);
	m68k_areg (regs, srcreg) = srca;
{	uae_s32 dst = m68k_areg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	do_cycles (2 * 256);
{	uae_u32 newv = dst - src;
	m68k_areg(regs, dstreg) = (newv);
}}}}}}	m68k_incpc (2);
endlabel4912: ;
return 0;
}
/* SUBA.W (d16,An),An */
unsigned long REGPARAM2 CPUFUNC(op_90e8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4913;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	do_cycles (2 * 256);
{	uae_u32 newv = dst - src;
	m68k_areg(regs, dstreg) = (newv);
}}}}}}	m68k_incpc (4);
endlabel4913: ;
return 0;
}
/* SUBA.W (d8,An,Xn),An */
unsigned long REGPARAM2 CPUFUNC(op_90f0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4914;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	do_cycles (2 * 256);
{	uae_u32 newv = dst - src;
	m68k_areg(regs, dstreg) = (newv);
}}}}}}	m68k_incpc (4);
endlabel4914: ;
return 0;
}
/* SUBA.W (xxx).W,An */
unsigned long REGPARAM2 CPUFUNC(op_90f8_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4915;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	do_cycles (2 * 256);
{	uae_u32 newv = dst - src;
	m68k_areg(regs, dstreg) = (newv);
}}}}}}	m68k_incpc (4);
endlabel4915: ;
return 0;
}
/* SUBA.W (xxx).L,An */
unsigned long REGPARAM2 CPUFUNC(op_90f9_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4916;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	do_cycles (2 * 256);
{	uae_u32 newv = dst - src;
	m68k_areg(regs, dstreg) = (newv);
}}}}}}	m68k_incpc (6);
endlabel4916: ;
return 0;
}
/* SUBA.W (d16,PC),An */
unsigned long REGPARAM2 CPUFUNC(op_90fa_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4917;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	do_cycles (2 * 256);
{	uae_u32 newv = dst - src;
	m68k_areg(regs, dstreg) = (newv);
}}}}}}	m68k_incpc (4);
endlabel4917: ;
return 0;
}
/* SUBA.W (d8,PC,Xn),An */
unsigned long REGPARAM2 CPUFUNC(op_90fb_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	do_cycles (2 * 256);
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4918;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	do_cycles (2 * 256);
{	uae_u32 newv = dst - src;
	m68k_areg(regs, dstreg) = (newv);
}}}}}}	m68k_incpc (4);
endlabel4918: ;
return 0;
}
/* SUBA.W #<data>.W,An */
unsigned long REGPARAM2 CPUFUNC(op_90fc_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	do_cycles (2 * 256);
{	uae_u32 newv = dst - src;
	m68k_areg(regs, dstreg) = (newv);
}}}}	m68k_incpc (4);
return 0;
}
/* SUBX.B Dn,Dn */
unsigned long REGPARAM2 CPUFUNC(op_9100_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	uae_s8 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 newv = dst - src - (GET_XFLG ? 1 : 0);
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_VFLG ((flgs ^ flgo) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
	COPY_CARRY;
	SET_ZFLG (GET_ZFLG & (((uae_s8)(newv)) == 0));
	SET_NFLG (((uae_s8)(newv)) < 0);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xff) | ((newv) & 0xff);
}}}}}	m68k_incpc (2);
return 0;
}
/* SUBX.B -(An),-(An) */
unsigned long REGPARAM2 CPUFUNC(op_9108_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = get_byte_ce (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = get_byte_ce (dsta);
	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 newv = dst - src - (GET_XFLG ? 1 : 0);
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_VFLG ((flgs ^ flgo) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
	COPY_CARRY;
	SET_ZFLG (GET_ZFLG & (((uae_s8)(newv)) == 0));
	SET_NFLG (((uae_s8)(newv)) < 0);
	do_cycles (2 * 256);
	put_byte_ce (dsta,newv);
}}}}}}}	m68k_incpc (2);
return 0;
}
/* SUB.B Dn,(An) */
unsigned long REGPARAM2 CPUFUNC(op_9110_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = m68k_areg(regs, dstreg);
{	uae_s8 dst = get_byte_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_byte_ce (dsta,newv);
}}}}}}}	m68k_incpc (2);
return 0;
}
/* SUB.B Dn,(An)+ */
unsigned long REGPARAM2 CPUFUNC(op_9118_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = m68k_areg(regs, dstreg);
{	uae_s8 dst = get_byte_ce (dsta);
	m68k_areg(regs, dstreg) += areg_byteinc[dstreg];
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_byte_ce (dsta,newv);
}}}}}}}	m68k_incpc (2);
return 0;
}
/* SUB.B Dn,-(An) */
unsigned long REGPARAM2 CPUFUNC(op_9120_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = m68k_areg(regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = get_byte_ce (dsta);
	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_byte_ce (dsta,newv);
}}}}}}}	m68k_incpc (2);
return 0;
}
/* SUB.B Dn,(d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_9128_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 dst = get_byte_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_byte_ce (dsta,newv);
}}}}}}}	m68k_incpc (4);
return 0;
}
/* SUB.B Dn,(d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_9130_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (4));
{	uae_s8 dst = get_byte_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_byte_ce (dsta,newv);
}}}}}}}	m68k_incpc (4);
return 0;
}
/* SUB.B Dn,(xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_9138_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 dst = get_byte_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_byte_ce (dsta,newv);
}}}}}}}	m68k_incpc (4);
return 0;
}
/* SUB.B Dn,(xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_9139_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (4) << 16;
	dsta |= get_word_ce_prefetch (6);
{	uae_s8 dst = get_byte_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_byte_ce (dsta,newv);
}}}}}}}	m68k_incpc (6);
return 0;
}
/* SUBX.W Dn,Dn */
unsigned long REGPARAM2 CPUFUNC(op_9140_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 newv = dst - src - (GET_XFLG ? 1 : 0);
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_VFLG ((flgs ^ flgo) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
	COPY_CARRY;
	SET_ZFLG (GET_ZFLG & (((uae_s16)(newv)) == 0));
	SET_NFLG (((uae_s16)(newv)) < 0);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xffff) | ((newv) & 0xffff);
}}}}}	m68k_incpc (2);
return 0;
}
/* SUBX.W -(An),-(An) */
unsigned long REGPARAM2 CPUFUNC(op_9148_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg(regs, srcreg) - 2;
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4930;
	}
{{	uae_s16 src = get_word_ce (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - 2;
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4930;
	}
{{	uae_s16 dst = get_word_ce (dsta);
	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 newv = dst - src - (GET_XFLG ? 1 : 0);
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_VFLG ((flgs ^ flgo) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
	COPY_CARRY;
	SET_ZFLG (GET_ZFLG & (((uae_s16)(newv)) == 0));
	SET_NFLG (((uae_s16)(newv)) < 0);
	do_cycles (2 * 256);
	put_word_ce (dsta,newv);
}}}}}}}}}	m68k_incpc (2);
endlabel4930: ;
return 0;
}
/* SUB.W Dn,(An) */
unsigned long REGPARAM2 CPUFUNC(op_9150_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4931;
	}
{{	uae_s16 dst = get_word_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_word_ce (dsta,newv);
}}}}}}}}	m68k_incpc (2);
endlabel4931: ;
return 0;
}
/* SUB.W Dn,(An)+ */
unsigned long REGPARAM2 CPUFUNC(op_9158_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4932;
	}
{{	uae_s16 dst = get_word_ce (dsta);
	m68k_areg(regs, dstreg) += 2;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_word_ce (dsta,newv);
}}}}}}}}	m68k_incpc (2);
endlabel4932: ;
return 0;
}
/* SUB.W Dn,-(An) */
unsigned long REGPARAM2 CPUFUNC(op_9160_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = m68k_areg(regs, dstreg) - 2;
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4933;
	}
{{	uae_s16 dst = get_word_ce (dsta);
	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_word_ce (dsta,newv);
}}}}}}}}	m68k_incpc (2);
endlabel4933: ;
return 0;
}
/* SUB.W Dn,(d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_9168_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4934;
	}
{{	uae_s16 dst = get_word_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_word_ce (dsta,newv);
}}}}}}}}	m68k_incpc (4);
endlabel4934: ;
return 0;
}
/* SUB.W Dn,(d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_9170_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (4));
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4935;
	}
{{	uae_s16 dst = get_word_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_word_ce (dsta,newv);
}}}}}}}}	m68k_incpc (4);
endlabel4935: ;
return 0;
}
/* SUB.W Dn,(xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_9178_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4936;
	}
{{	uae_s16 dst = get_word_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_word_ce (dsta,newv);
}}}}}}}}	m68k_incpc (4);
endlabel4936: ;
return 0;
}
/* SUB.W Dn,(xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_9179_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (4) << 16;
	dsta |= get_word_ce_prefetch (6);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4937;
	}
{{	uae_s16 dst = get_word_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_word_ce (dsta,newv);
}}}}}}}}	m68k_incpc (6);
endlabel4937: ;
return 0;
}
/* SUBX.L Dn,Dn */
unsigned long REGPARAM2 CPUFUNC(op_9180_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 newv = dst - src - (GET_XFLG ? 1 : 0);
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_VFLG ((flgs ^ flgo) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
	COPY_CARRY;
	SET_ZFLG (GET_ZFLG & (((uae_s32)(newv)) == 0));
	SET_NFLG (((uae_s32)(newv)) < 0);
	do_cycles (2 * 256);
	m68k_dreg(regs, dstreg) = (newv);
}}}}}	m68k_incpc (2);
return 0;
}
/* SUBX.L -(An),-(An) */
unsigned long REGPARAM2 CPUFUNC(op_9188_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg(regs, srcreg) - 4;
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4939;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - 4;
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4939;
	}
{{	uae_s32 dst = get_word_ce (dsta) << 16; dst |= get_word_ce (dsta + 2);
	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 newv = dst - src - (GET_XFLG ? 1 : 0);
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_VFLG ((flgs ^ flgo) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
	COPY_CARRY;
	SET_ZFLG (GET_ZFLG & (((uae_s32)(newv)) == 0));
	SET_NFLG (((uae_s32)(newv)) < 0);
	do_cycles (2 * 256);
	put_word_ce (dsta, newv >> 16); put_word_ce (dsta + 2, newv);
}}}}}}}}}	m68k_incpc (2);
endlabel4939: ;
return 0;
}
/* SUB.L Dn,(An) */
unsigned long REGPARAM2 CPUFUNC(op_9190_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4940;
	}
{{	uae_s32 dst = get_word_ce (dsta) << 16; dst |= get_word_ce (dsta + 2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_word_ce (dsta, newv >> 16); put_word_ce (dsta + 2, newv);
}}}}}}}}	m68k_incpc (2);
endlabel4940: ;
return 0;
}
/* SUB.L Dn,(An)+ */
unsigned long REGPARAM2 CPUFUNC(op_9198_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4941;
	}
{{	uae_s32 dst = get_word_ce (dsta) << 16; dst |= get_word_ce (dsta + 2);
	m68k_areg(regs, dstreg) += 4;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_word_ce (dsta, newv >> 16); put_word_ce (dsta + 2, newv);
}}}}}}}}	m68k_incpc (2);
endlabel4941: ;
return 0;
}
/* SUB.L Dn,-(An) */
unsigned long REGPARAM2 CPUFUNC(op_91a0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = m68k_areg(regs, dstreg) - 4;
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4942;
	}
{{	uae_s32 dst = get_word_ce (dsta) << 16; dst |= get_word_ce (dsta + 2);
	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_word_ce (dsta, newv >> 16); put_word_ce (dsta + 2, newv);
}}}}}}}}	m68k_incpc (2);
endlabel4942: ;
return 0;
}
/* SUB.L Dn,(d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_91a8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4943;
	}
{{	uae_s32 dst = get_word_ce (dsta) << 16; dst |= get_word_ce (dsta + 2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_word_ce (dsta, newv >> 16); put_word_ce (dsta + 2, newv);
}}}}}}}}	m68k_incpc (4);
endlabel4943: ;
return 0;
}
/* SUB.L Dn,(d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_91b0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (4));
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4944;
	}
{{	uae_s32 dst = get_word_ce (dsta) << 16; dst |= get_word_ce (dsta + 2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_word_ce (dsta, newv >> 16); put_word_ce (dsta + 2, newv);
}}}}}}}}	m68k_incpc (4);
endlabel4944: ;
return 0;
}
/* SUB.L Dn,(xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_91b8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4945;
	}
{{	uae_s32 dst = get_word_ce (dsta) << 16; dst |= get_word_ce (dsta + 2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_word_ce (dsta, newv >> 16); put_word_ce (dsta + 2, newv);
}}}}}}}}	m68k_incpc (4);
endlabel4945: ;
return 0;
}
/* SUB.L Dn,(xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_91b9_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (4) << 16;
	dsta |= get_word_ce_prefetch (6);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4946;
	}
{{	uae_s32 dst = get_word_ce (dsta) << 16; dst |= get_word_ce (dsta + 2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_word_ce (dsta, newv >> 16); put_word_ce (dsta + 2, newv);
}}}}}}}}	m68k_incpc (6);
endlabel4946: ;
return 0;
}
/* SUBA.L Dn,An */
unsigned long REGPARAM2 CPUFUNC(op_91c0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	do_cycles (2 * 256);
{	uae_u32 newv = dst - src;
	m68k_areg(regs, dstreg) = (newv);
}}}}	m68k_incpc (2);
return 0;
}
/* SUBA.L An,An */
unsigned long REGPARAM2 CPUFUNC(op_91c8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_areg(regs, srcreg);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	do_cycles (2 * 256);
{	uae_u32 newv = dst - src;
	m68k_areg(regs, dstreg) = (newv);
}}}}	m68k_incpc (2);
return 0;
}
/* SUBA.L (An),An */
unsigned long REGPARAM2 CPUFUNC(op_91d0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4949;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	do_cycles (2 * 256);
{	uae_u32 newv = dst - src;
	m68k_areg(regs, dstreg) = (newv);
}}}}}}	m68k_incpc (2);
endlabel4949: ;
return 0;
}
/* SUBA.L (An)+,An */
unsigned long REGPARAM2 CPUFUNC(op_91d8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4950;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	m68k_areg(regs, srcreg) += 4;
{	uae_s32 dst = m68k_areg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	do_cycles (2 * 256);
{	uae_u32 newv = dst - src;
	m68k_areg(regs, dstreg) = (newv);
}}}}}}	m68k_incpc (2);
endlabel4950: ;
return 0;
}
/* SUBA.L -(An),An */
unsigned long REGPARAM2 CPUFUNC(op_91e0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = m68k_areg(regs, srcreg) - 4;
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4951;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	m68k_areg (regs, srcreg) = srca;
{	uae_s32 dst = m68k_areg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	do_cycles (2 * 256);
{	uae_u32 newv = dst - src;
	m68k_areg(regs, dstreg) = (newv);
}}}}}}	m68k_incpc (2);
endlabel4951: ;
return 0;
}
/* SUBA.L (d16,An),An */
unsigned long REGPARAM2 CPUFUNC(op_91e8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4952;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	do_cycles (2 * 256);
{	uae_u32 newv = dst - src;
	m68k_areg(regs, dstreg) = (newv);
}}}}}}	m68k_incpc (4);
endlabel4952: ;
return 0;
}
/* SUBA.L (d8,An,Xn),An */
unsigned long REGPARAM2 CPUFUNC(op_91f0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4953;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	do_cycles (2 * 256);
{	uae_u32 newv = dst - src;
	m68k_areg(regs, dstreg) = (newv);
}}}}}}	m68k_incpc (4);
endlabel4953: ;
return 0;
}
/* SUBA.L (xxx).W,An */
unsigned long REGPARAM2 CPUFUNC(op_91f8_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4954;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	do_cycles (2 * 256);
{	uae_u32 newv = dst - src;
	m68k_areg(regs, dstreg) = (newv);
}}}}}}	m68k_incpc (4);
endlabel4954: ;
return 0;
}
/* SUBA.L (xxx).L,An */
unsigned long REGPARAM2 CPUFUNC(op_91f9_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4955;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	do_cycles (2 * 256);
{	uae_u32 newv = dst - src;
	m68k_areg(regs, dstreg) = (newv);
}}}}}}	m68k_incpc (6);
endlabel4955: ;
return 0;
}
/* SUBA.L (d16,PC),An */
unsigned long REGPARAM2 CPUFUNC(op_91fa_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4956;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	do_cycles (2 * 256);
{	uae_u32 newv = dst - src;
	m68k_areg(regs, dstreg) = (newv);
}}}}}}	m68k_incpc (4);
endlabel4956: ;
return 0;
}
/* SUBA.L (d8,PC,Xn),An */
unsigned long REGPARAM2 CPUFUNC(op_91fb_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	do_cycles (2 * 256);
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4957;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	do_cycles (2 * 256);
{	uae_u32 newv = dst - src;
	m68k_areg(regs, dstreg) = (newv);
}}}}}}	m68k_incpc (4);
endlabel4957: ;
return 0;
}
/* SUBA.L #<data>.L,An */
unsigned long REGPARAM2 CPUFUNC(op_91fc_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	do_cycles (2 * 256);
{	uae_u32 newv = dst - src;
	m68k_areg(regs, dstreg) = (newv);
}}}}	m68k_incpc (6);
return 0;
}
/* CMP.B Dn,Dn */
unsigned long REGPARAM2 CPUFUNC(op_b000_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	uae_s8 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}	m68k_incpc (2);
return 0;
}
/* CMP.B (An),Dn */
unsigned long REGPARAM2 CPUFUNC(op_b010_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
{	uae_s8 src = get_byte_ce (srca);
{	uae_s8 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}	m68k_incpc (2);
return 0;
}
/* CMP.B (An)+,Dn */
unsigned long REGPARAM2 CPUFUNC(op_b018_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
{	uae_s8 src = get_byte_ce (srca);
	m68k_areg(regs, srcreg) += areg_byteinc[srcreg];
{	uae_s8 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}	m68k_incpc (2);
return 0;
}
/* CMP.B -(An),Dn */
unsigned long REGPARAM2 CPUFUNC(op_b020_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = get_byte_ce (srca);
	m68k_areg (regs, srcreg) = srca;
{	uae_s8 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}	m68k_incpc (2);
return 0;
}
/* CMP.B (d16,An),Dn */
unsigned long REGPARAM2 CPUFUNC(op_b028_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 src = get_byte_ce (srca);
{	uae_s8 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}	m68k_incpc (4);
return 0;
}
/* CMP.B (d8,An,Xn),Dn */
unsigned long REGPARAM2 CPUFUNC(op_b030_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
{	uae_s8 src = get_byte_ce (srca);
{	uae_s8 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}	m68k_incpc (4);
return 0;
}
/* CMP.B (xxx).W,Dn */
unsigned long REGPARAM2 CPUFUNC(op_b038_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 src = get_byte_ce (srca);
{	uae_s8 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}	m68k_incpc (4);
return 0;
}
/* CMP.B (xxx).L,Dn */
unsigned long REGPARAM2 CPUFUNC(op_b039_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
{	uae_s8 src = get_byte_ce (srca);
{	uae_s8 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}	m68k_incpc (6);
return 0;
}
/* CMP.B (d16,PC),Dn */
unsigned long REGPARAM2 CPUFUNC(op_b03a_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 src = get_byte_ce (srca);
{	uae_s8 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}	m68k_incpc (4);
return 0;
}
/* CMP.B (d8,PC,Xn),Dn */
unsigned long REGPARAM2 CPUFUNC(op_b03b_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	do_cycles (2 * 256);
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
{	uae_s8 src = get_byte_ce (srca);
{	uae_s8 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}	m68k_incpc (4);
return 0;
}
/* CMP.B #<data>.B,Dn */
unsigned long REGPARAM2 CPUFUNC(op_b03c_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s8 src = (uae_u8)get_word_ce_prefetch (4);
{	uae_s8 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}	m68k_incpc (4);
return 0;
}
/* CMP.W Dn,Dn */
unsigned long REGPARAM2 CPUFUNC(op_b040_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}	m68k_incpc (2);
return 0;
}
#endif

#ifdef PART_7
/* CMP.W An,Dn */
unsigned long REGPARAM2 CPUFUNC(op_b048_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_areg(regs, srcreg);
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}	m68k_incpc (2);
return 0;
}
/* CMP.W (An),Dn */
unsigned long REGPARAM2 CPUFUNC(op_b050_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4972;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (2);
endlabel4972: ;
return 0;
}
/* CMP.W (An)+,Dn */
unsigned long REGPARAM2 CPUFUNC(op_b058_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4973;
	}
{{	uae_s16 src = get_word_ce (srca);
	m68k_areg(regs, srcreg) += 2;
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (2);
endlabel4973: ;
return 0;
}
/* CMP.W -(An),Dn */
unsigned long REGPARAM2 CPUFUNC(op_b060_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = m68k_areg(regs, srcreg) - 2;
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4974;
	}
{{	uae_s16 src = get_word_ce (srca);
	m68k_areg (regs, srcreg) = srca;
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (2);
endlabel4974: ;
return 0;
}
/* CMP.W (d16,An),Dn */
unsigned long REGPARAM2 CPUFUNC(op_b068_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4975;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (4);
endlabel4975: ;
return 0;
}
/* CMP.W (d8,An,Xn),Dn */
unsigned long REGPARAM2 CPUFUNC(op_b070_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4976;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (4);
endlabel4976: ;
return 0;
}
/* CMP.W (xxx).W,Dn */
unsigned long REGPARAM2 CPUFUNC(op_b078_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4977;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (4);
endlabel4977: ;
return 0;
}
/* CMP.W (xxx).L,Dn */
unsigned long REGPARAM2 CPUFUNC(op_b079_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4978;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (6);
endlabel4978: ;
return 0;
}
/* CMP.W (d16,PC),Dn */
unsigned long REGPARAM2 CPUFUNC(op_b07a_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4979;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (4);
endlabel4979: ;
return 0;
}
/* CMP.W (d8,PC,Xn),Dn */
unsigned long REGPARAM2 CPUFUNC(op_b07b_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	do_cycles (2 * 256);
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4980;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (4);
endlabel4980: ;
return 0;
}
/* CMP.W #<data>.W,Dn */
unsigned long REGPARAM2 CPUFUNC(op_b07c_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}	m68k_incpc (4);
return 0;
}
/* CMP.L Dn,Dn */
unsigned long REGPARAM2 CPUFUNC(op_b080_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	do_cycles (2 * 256);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}	m68k_incpc (2);
return 0;
}
/* CMP.L An,Dn */
unsigned long REGPARAM2 CPUFUNC(op_b088_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_areg(regs, srcreg);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	do_cycles (2 * 256);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}	m68k_incpc (2);
return 0;
}
/* CMP.L (An),Dn */
unsigned long REGPARAM2 CPUFUNC(op_b090_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4984;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	do_cycles (2 * 256);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (2);
endlabel4984: ;
return 0;
}
/* CMP.L (An)+,Dn */
unsigned long REGPARAM2 CPUFUNC(op_b098_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4985;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	m68k_areg(regs, srcreg) += 4;
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	do_cycles (2 * 256);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (2);
endlabel4985: ;
return 0;
}
/* CMP.L -(An),Dn */
unsigned long REGPARAM2 CPUFUNC(op_b0a0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = m68k_areg(regs, srcreg) - 4;
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4986;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	m68k_areg (regs, srcreg) = srca;
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	do_cycles (2 * 256);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (2);
endlabel4986: ;
return 0;
}
/* CMP.L (d16,An),Dn */
unsigned long REGPARAM2 CPUFUNC(op_b0a8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4987;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	do_cycles (2 * 256);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (4);
endlabel4987: ;
return 0;
}
/* CMP.L (d8,An,Xn),Dn */
unsigned long REGPARAM2 CPUFUNC(op_b0b0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4988;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	do_cycles (2 * 256);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (4);
endlabel4988: ;
return 0;
}
/* CMP.L (xxx).W,Dn */
unsigned long REGPARAM2 CPUFUNC(op_b0b8_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4989;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	do_cycles (2 * 256);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (4);
endlabel4989: ;
return 0;
}
/* CMP.L (xxx).L,Dn */
unsigned long REGPARAM2 CPUFUNC(op_b0b9_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4990;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	do_cycles (2 * 256);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (6);
endlabel4990: ;
return 0;
}
/* CMP.L (d16,PC),Dn */
unsigned long REGPARAM2 CPUFUNC(op_b0ba_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4991;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	do_cycles (2 * 256);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (4);
endlabel4991: ;
return 0;
}
/* CMP.L (d8,PC,Xn),Dn */
unsigned long REGPARAM2 CPUFUNC(op_b0bb_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	do_cycles (2 * 256);
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4992;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	do_cycles (2 * 256);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (4);
endlabel4992: ;
return 0;
}
/* CMP.L #<data>.L,Dn */
unsigned long REGPARAM2 CPUFUNC(op_b0bc_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	do_cycles (2 * 256);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}	m68k_incpc (6);
return 0;
}
/* CMPA.W Dn,An */
unsigned long REGPARAM2 CPUFUNC(op_b0c0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	do_cycles (2 * 256);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}	m68k_incpc (2);
return 0;
}
/* CMPA.W An,An */
unsigned long REGPARAM2 CPUFUNC(op_b0c8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_areg(regs, srcreg);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	do_cycles (2 * 256);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}	m68k_incpc (2);
return 0;
}
/* CMPA.W (An),An */
unsigned long REGPARAM2 CPUFUNC(op_b0d0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4996;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	do_cycles (2 * 256);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (2);
endlabel4996: ;
return 0;
}
/* CMPA.W (An)+,An */
unsigned long REGPARAM2 CPUFUNC(op_b0d8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4997;
	}
{{	uae_s16 src = get_word_ce (srca);
	m68k_areg(regs, srcreg) += 2;
{	uae_s32 dst = m68k_areg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	do_cycles (2 * 256);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (2);
endlabel4997: ;
return 0;
}
/* CMPA.W -(An),An */
unsigned long REGPARAM2 CPUFUNC(op_b0e0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = m68k_areg(regs, srcreg) - 2;
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4998;
	}
{{	uae_s16 src = get_word_ce (srca);
	m68k_areg (regs, srcreg) = srca;
{	uae_s32 dst = m68k_areg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	do_cycles (2 * 256);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (2);
endlabel4998: ;
return 0;
}
/* CMPA.W (d16,An),An */
unsigned long REGPARAM2 CPUFUNC(op_b0e8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4999;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	do_cycles (2 * 256);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (4);
endlabel4999: ;
return 0;
}
/* CMPA.W (d8,An,Xn),An */
unsigned long REGPARAM2 CPUFUNC(op_b0f0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5000;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	do_cycles (2 * 256);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (4);
endlabel5000: ;
return 0;
}
/* CMPA.W (xxx).W,An */
unsigned long REGPARAM2 CPUFUNC(op_b0f8_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5001;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	do_cycles (2 * 256);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (4);
endlabel5001: ;
return 0;
}
/* CMPA.W (xxx).L,An */
unsigned long REGPARAM2 CPUFUNC(op_b0f9_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5002;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	do_cycles (2 * 256);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (6);
endlabel5002: ;
return 0;
}
/* CMPA.W (d16,PC),An */
unsigned long REGPARAM2 CPUFUNC(op_b0fa_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5003;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	do_cycles (2 * 256);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (4);
endlabel5003: ;
return 0;
}
/* CMPA.W (d8,PC,Xn),An */
unsigned long REGPARAM2 CPUFUNC(op_b0fb_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	do_cycles (2 * 256);
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5004;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	do_cycles (2 * 256);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (4);
endlabel5004: ;
return 0;
}
/* CMPA.W #<data>.W,An */
unsigned long REGPARAM2 CPUFUNC(op_b0fc_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	do_cycles (2 * 256);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}	m68k_incpc (4);
return 0;
}
/* EOR.B Dn,Dn */
unsigned long REGPARAM2 CPUFUNC(op_b100_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	uae_s8 dst = m68k_dreg(regs, dstreg);
	src ^= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xff) | ((src) & 0xff);
}}}	m68k_incpc (2);
return 0;
}
/* CMPM.B (An)+,(An)+ */
unsigned long REGPARAM2 CPUFUNC(op_b108_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
{	uae_s8 src = get_byte_ce (srca);
	m68k_areg(regs, srcreg) += areg_byteinc[srcreg];
{	uaecptr dsta = m68k_areg(regs, dstreg);
{	uae_s8 dst = get_byte_ce (dsta);
	m68k_areg(regs, dstreg) += areg_byteinc[dstreg];
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (2);
return 0;
}
/* EOR.B Dn,(An) */
unsigned long REGPARAM2 CPUFUNC(op_b110_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = m68k_areg(regs, dstreg);
{	uae_s8 dst = get_byte_ce (dsta);
	src ^= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	put_byte_ce (dsta,src);
}}}}	m68k_incpc (2);
return 0;
}
/* EOR.B Dn,(An)+ */
unsigned long REGPARAM2 CPUFUNC(op_b118_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = m68k_areg(regs, dstreg);
{	uae_s8 dst = get_byte_ce (dsta);
	m68k_areg(regs, dstreg) += areg_byteinc[dstreg];
	src ^= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	put_byte_ce (dsta,src);
}}}}	m68k_incpc (2);
return 0;
}
/* EOR.B Dn,-(An) */
unsigned long REGPARAM2 CPUFUNC(op_b120_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = m68k_areg(regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = get_byte_ce (dsta);
	m68k_areg (regs, dstreg) = dsta;
	src ^= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	put_byte_ce (dsta,src);
}}}}	m68k_incpc (2);
return 0;
}
/* EOR.B Dn,(d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_b128_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 dst = get_byte_ce (dsta);
	src ^= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	put_byte_ce (dsta,src);
}}}}	m68k_incpc (4);
return 0;
}
/* EOR.B Dn,(d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_b130_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (4));
{	uae_s8 dst = get_byte_ce (dsta);
	src ^= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	put_byte_ce (dsta,src);
}}}}	m68k_incpc (4);
return 0;
}
/* EOR.B Dn,(xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_b138_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 dst = get_byte_ce (dsta);
	src ^= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	put_byte_ce (dsta,src);
}}}}	m68k_incpc (4);
return 0;
}
/* EOR.B Dn,(xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_b139_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (4) << 16;
	dsta |= get_word_ce_prefetch (6);
{	uae_s8 dst = get_byte_ce (dsta);
	src ^= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	put_byte_ce (dsta,src);
}}}}	m68k_incpc (6);
return 0;
}
/* EOR.W Dn,Dn */
unsigned long REGPARAM2 CPUFUNC(op_b140_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	src ^= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xffff) | ((src) & 0xffff);
}}}	m68k_incpc (2);
return 0;
}
/* CMPM.W (An)+,(An)+ */
unsigned long REGPARAM2 CPUFUNC(op_b148_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5016;
	}
{{	uae_s16 src = get_word_ce (srca);
	m68k_areg(regs, srcreg) += 2;
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel5016;
	}
{{	uae_s16 dst = get_word_ce (dsta);
	m68k_areg(regs, dstreg) += 2;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}}}	m68k_incpc (2);
endlabel5016: ;
return 0;
}
/* EOR.W Dn,(An) */
unsigned long REGPARAM2 CPUFUNC(op_b150_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel5017;
	}
{{	uae_s16 dst = get_word_ce (dsta);
	src ^= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	put_word_ce (dsta,src);
}}}}}	m68k_incpc (2);
endlabel5017: ;
return 0;
}
/* EOR.W Dn,(An)+ */
unsigned long REGPARAM2 CPUFUNC(op_b158_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel5018;
	}
{{	uae_s16 dst = get_word_ce (dsta);
	m68k_areg(regs, dstreg) += 2;
	src ^= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	put_word_ce (dsta,src);
}}}}}	m68k_incpc (2);
endlabel5018: ;
return 0;
}
/* EOR.W Dn,-(An) */
unsigned long REGPARAM2 CPUFUNC(op_b160_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = m68k_areg(regs, dstreg) - 2;
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel5019;
	}
{{	uae_s16 dst = get_word_ce (dsta);
	m68k_areg (regs, dstreg) = dsta;
	src ^= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	put_word_ce (dsta,src);
}}}}}	m68k_incpc (2);
endlabel5019: ;
return 0;
}
/* EOR.W Dn,(d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_b168_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel5020;
	}
{{	uae_s16 dst = get_word_ce (dsta);
	src ^= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	put_word_ce (dsta,src);
}}}}}	m68k_incpc (4);
endlabel5020: ;
return 0;
}
/* EOR.W Dn,(d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_b170_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (4));
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel5021;
	}
{{	uae_s16 dst = get_word_ce (dsta);
	src ^= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	put_word_ce (dsta,src);
}}}}}	m68k_incpc (4);
endlabel5021: ;
return 0;
}
/* EOR.W Dn,(xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_b178_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel5022;
	}
{{	uae_s16 dst = get_word_ce (dsta);
	src ^= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	put_word_ce (dsta,src);
}}}}}	m68k_incpc (4);
endlabel5022: ;
return 0;
}
/* EOR.W Dn,(xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_b179_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (4) << 16;
	dsta |= get_word_ce_prefetch (6);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel5023;
	}
{{	uae_s16 dst = get_word_ce (dsta);
	src ^= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	put_word_ce (dsta,src);
}}}}}	m68k_incpc (6);
endlabel5023: ;
return 0;
}
/* EOR.L Dn,Dn */
unsigned long REGPARAM2 CPUFUNC(op_b180_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	src ^= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	do_cycles (4 * 256);
	m68k_dreg(regs, dstreg) = (src);
}}}	m68k_incpc (2);
return 0;
}
/* CMPM.L (An)+,(An)+ */
unsigned long REGPARAM2 CPUFUNC(op_b188_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5025;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	m68k_areg(regs, srcreg) += 4;
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel5025;
	}
{{	uae_s32 dst = get_word_ce (dsta) << 16; dst |= get_word_ce (dsta + 2);
	m68k_areg(regs, dstreg) += 4;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}}}	m68k_incpc (2);
endlabel5025: ;
return 0;
}
/* EOR.L Dn,(An) */
unsigned long REGPARAM2 CPUFUNC(op_b190_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel5026;
	}
{{	uae_s32 dst = get_word_ce (dsta) << 16; dst |= get_word_ce (dsta + 2);
	src ^= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
}}}}}	m68k_incpc (2);
endlabel5026: ;
return 0;
}
/* EOR.L Dn,(An)+ */
unsigned long REGPARAM2 CPUFUNC(op_b198_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel5027;
	}
{{	uae_s32 dst = get_word_ce (dsta) << 16; dst |= get_word_ce (dsta + 2);
	m68k_areg(regs, dstreg) += 4;
	src ^= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
}}}}}	m68k_incpc (2);
endlabel5027: ;
return 0;
}
/* EOR.L Dn,-(An) */
unsigned long REGPARAM2 CPUFUNC(op_b1a0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = m68k_areg(regs, dstreg) - 4;
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel5028;
	}
{{	uae_s32 dst = get_word_ce (dsta) << 16; dst |= get_word_ce (dsta + 2);
	m68k_areg (regs, dstreg) = dsta;
	src ^= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
}}}}}	m68k_incpc (2);
endlabel5028: ;
return 0;
}
/* EOR.L Dn,(d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_b1a8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel5029;
	}
{{	uae_s32 dst = get_word_ce (dsta) << 16; dst |= get_word_ce (dsta + 2);
	src ^= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
}}}}}	m68k_incpc (4);
endlabel5029: ;
return 0;
}
/* EOR.L Dn,(d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_b1b0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (4));
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel5030;
	}
{{	uae_s32 dst = get_word_ce (dsta) << 16; dst |= get_word_ce (dsta + 2);
	src ^= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
}}}}}	m68k_incpc (4);
endlabel5030: ;
return 0;
}
/* EOR.L Dn,(xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_b1b8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel5031;
	}
{{	uae_s32 dst = get_word_ce (dsta) << 16; dst |= get_word_ce (dsta + 2);
	src ^= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
}}}}}	m68k_incpc (4);
endlabel5031: ;
return 0;
}
/* EOR.L Dn,(xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_b1b9_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (4) << 16;
	dsta |= get_word_ce_prefetch (6);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel5032;
	}
{{	uae_s32 dst = get_word_ce (dsta) << 16; dst |= get_word_ce (dsta + 2);
	src ^= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
}}}}}	m68k_incpc (6);
endlabel5032: ;
return 0;
}
/* CMPA.L Dn,An */
unsigned long REGPARAM2 CPUFUNC(op_b1c0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	do_cycles (2 * 256);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}	m68k_incpc (2);
return 0;
}
/* CMPA.L An,An */
unsigned long REGPARAM2 CPUFUNC(op_b1c8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_areg(regs, srcreg);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	do_cycles (2 * 256);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}	m68k_incpc (2);
return 0;
}
/* CMPA.L (An),An */
unsigned long REGPARAM2 CPUFUNC(op_b1d0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5035;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	do_cycles (2 * 256);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (2);
endlabel5035: ;
return 0;
}
/* CMPA.L (An)+,An */
unsigned long REGPARAM2 CPUFUNC(op_b1d8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5036;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	m68k_areg(regs, srcreg) += 4;
{	uae_s32 dst = m68k_areg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	do_cycles (2 * 256);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (2);
endlabel5036: ;
return 0;
}
/* CMPA.L -(An),An */
unsigned long REGPARAM2 CPUFUNC(op_b1e0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = m68k_areg(regs, srcreg) - 4;
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5037;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	m68k_areg (regs, srcreg) = srca;
{	uae_s32 dst = m68k_areg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	do_cycles (2 * 256);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (2);
endlabel5037: ;
return 0;
}
/* CMPA.L (d16,An),An */
unsigned long REGPARAM2 CPUFUNC(op_b1e8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5038;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	do_cycles (2 * 256);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (4);
endlabel5038: ;
return 0;
}
/* CMPA.L (d8,An,Xn),An */
unsigned long REGPARAM2 CPUFUNC(op_b1f0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5039;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	do_cycles (2 * 256);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (4);
endlabel5039: ;
return 0;
}
/* CMPA.L (xxx).W,An */
unsigned long REGPARAM2 CPUFUNC(op_b1f8_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5040;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	do_cycles (2 * 256);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (4);
endlabel5040: ;
return 0;
}
/* CMPA.L (xxx).L,An */
unsigned long REGPARAM2 CPUFUNC(op_b1f9_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5041;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	do_cycles (2 * 256);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (6);
endlabel5041: ;
return 0;
}
/* CMPA.L (d16,PC),An */
unsigned long REGPARAM2 CPUFUNC(op_b1fa_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5042;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	do_cycles (2 * 256);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (4);
endlabel5042: ;
return 0;
}
/* CMPA.L (d8,PC,Xn),An */
unsigned long REGPARAM2 CPUFUNC(op_b1fb_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	do_cycles (2 * 256);
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5043;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	do_cycles (2 * 256);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (4);
endlabel5043: ;
return 0;
}
/* CMPA.L #<data>.L,An */
unsigned long REGPARAM2 CPUFUNC(op_b1fc_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	do_cycles (2 * 256);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}	m68k_incpc (6);
return 0;
}
/* AND.B Dn,Dn */
unsigned long REGPARAM2 CPUFUNC(op_c000_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	uae_s8 dst = m68k_dreg(regs, dstreg);
	src &= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xff) | ((src) & 0xff);
}}}	m68k_incpc (2);
return 0;
}
/* AND.B (An),Dn */
unsigned long REGPARAM2 CPUFUNC(op_c010_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
{	uae_s8 src = get_byte_ce (srca);
{	uae_s8 dst = m68k_dreg(regs, dstreg);
	src &= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xff) | ((src) & 0xff);
}}}}	m68k_incpc (2);
return 0;
}
/* AND.B (An)+,Dn */
unsigned long REGPARAM2 CPUFUNC(op_c018_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
{	uae_s8 src = get_byte_ce (srca);
	m68k_areg(regs, srcreg) += areg_byteinc[srcreg];
{	uae_s8 dst = m68k_dreg(regs, dstreg);
	src &= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xff) | ((src) & 0xff);
}}}}	m68k_incpc (2);
return 0;
}
/* AND.B -(An),Dn */
unsigned long REGPARAM2 CPUFUNC(op_c020_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = get_byte_ce (srca);
	m68k_areg (regs, srcreg) = srca;
{	uae_s8 dst = m68k_dreg(regs, dstreg);
	src &= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xff) | ((src) & 0xff);
}}}}	m68k_incpc (2);
return 0;
}
/* AND.B (d16,An),Dn */
unsigned long REGPARAM2 CPUFUNC(op_c028_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 src = get_byte_ce (srca);
{	uae_s8 dst = m68k_dreg(regs, dstreg);
	src &= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xff) | ((src) & 0xff);
}}}}	m68k_incpc (4);
return 0;
}
/* AND.B (d8,An,Xn),Dn */
unsigned long REGPARAM2 CPUFUNC(op_c030_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
{	uae_s8 src = get_byte_ce (srca);
{	uae_s8 dst = m68k_dreg(regs, dstreg);
	src &= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xff) | ((src) & 0xff);
}}}}	m68k_incpc (4);
return 0;
}
/* AND.B (xxx).W,Dn */
unsigned long REGPARAM2 CPUFUNC(op_c038_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 src = get_byte_ce (srca);
{	uae_s8 dst = m68k_dreg(regs, dstreg);
	src &= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xff) | ((src) & 0xff);
}}}}	m68k_incpc (4);
return 0;
}
/* AND.B (xxx).L,Dn */
unsigned long REGPARAM2 CPUFUNC(op_c039_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
{	uae_s8 src = get_byte_ce (srca);
{	uae_s8 dst = m68k_dreg(regs, dstreg);
	src &= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xff) | ((src) & 0xff);
}}}}	m68k_incpc (6);
return 0;
}
/* AND.B (d16,PC),Dn */
unsigned long REGPARAM2 CPUFUNC(op_c03a_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 src = get_byte_ce (srca);
{	uae_s8 dst = m68k_dreg(regs, dstreg);
	src &= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xff) | ((src) & 0xff);
}}}}	m68k_incpc (4);
return 0;
}
/* AND.B (d8,PC,Xn),Dn */
unsigned long REGPARAM2 CPUFUNC(op_c03b_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	do_cycles (2 * 256);
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
{	uae_s8 src = get_byte_ce (srca);
{	uae_s8 dst = m68k_dreg(regs, dstreg);
	src &= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xff) | ((src) & 0xff);
}}}}	m68k_incpc (4);
return 0;
}
/* AND.B #<data>.B,Dn */
unsigned long REGPARAM2 CPUFUNC(op_c03c_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s8 src = (uae_u8)get_word_ce_prefetch (4);
{	uae_s8 dst = m68k_dreg(regs, dstreg);
	src &= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xff) | ((src) & 0xff);
}}}	m68k_incpc (4);
return 0;
}
/* AND.W Dn,Dn */
unsigned long REGPARAM2 CPUFUNC(op_c040_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	src &= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xffff) | ((src) & 0xffff);
}}}	m68k_incpc (2);
return 0;
}
/* AND.W (An),Dn */
unsigned long REGPARAM2 CPUFUNC(op_c050_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5057;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	src &= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xffff) | ((src) & 0xffff);
}}}}}	m68k_incpc (2);
endlabel5057: ;
return 0;
}
/* AND.W (An)+,Dn */
unsigned long REGPARAM2 CPUFUNC(op_c058_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5058;
	}
{{	uae_s16 src = get_word_ce (srca);
	m68k_areg(regs, srcreg) += 2;
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	src &= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xffff) | ((src) & 0xffff);
}}}}}	m68k_incpc (2);
endlabel5058: ;
return 0;
}
/* AND.W -(An),Dn */
unsigned long REGPARAM2 CPUFUNC(op_c060_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = m68k_areg(regs, srcreg) - 2;
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5059;
	}
{{	uae_s16 src = get_word_ce (srca);
	m68k_areg (regs, srcreg) = srca;
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	src &= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xffff) | ((src) & 0xffff);
}}}}}	m68k_incpc (2);
endlabel5059: ;
return 0;
}
/* AND.W (d16,An),Dn */
unsigned long REGPARAM2 CPUFUNC(op_c068_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5060;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	src &= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xffff) | ((src) & 0xffff);
}}}}}	m68k_incpc (4);
endlabel5060: ;
return 0;
}
/* AND.W (d8,An,Xn),Dn */
unsigned long REGPARAM2 CPUFUNC(op_c070_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5061;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	src &= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xffff) | ((src) & 0xffff);
}}}}}	m68k_incpc (4);
endlabel5061: ;
return 0;
}
/* AND.W (xxx).W,Dn */
unsigned long REGPARAM2 CPUFUNC(op_c078_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5062;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	src &= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xffff) | ((src) & 0xffff);
}}}}}	m68k_incpc (4);
endlabel5062: ;
return 0;
}
/* AND.W (xxx).L,Dn */
unsigned long REGPARAM2 CPUFUNC(op_c079_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5063;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	src &= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xffff) | ((src) & 0xffff);
}}}}}	m68k_incpc (6);
endlabel5063: ;
return 0;
}
/* AND.W (d16,PC),Dn */
unsigned long REGPARAM2 CPUFUNC(op_c07a_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5064;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	src &= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xffff) | ((src) & 0xffff);
}}}}}	m68k_incpc (4);
endlabel5064: ;
return 0;
}
/* AND.W (d8,PC,Xn),Dn */
unsigned long REGPARAM2 CPUFUNC(op_c07b_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	do_cycles (2 * 256);
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5065;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	src &= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xffff) | ((src) & 0xffff);
}}}}}	m68k_incpc (4);
endlabel5065: ;
return 0;
}
/* AND.W #<data>.W,Dn */
unsigned long REGPARAM2 CPUFUNC(op_c07c_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	src &= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xffff) | ((src) & 0xffff);
}}}	m68k_incpc (4);
return 0;
}
/* AND.L Dn,Dn */
unsigned long REGPARAM2 CPUFUNC(op_c080_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	src &= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	do_cycles (2 * 256);
	m68k_dreg(regs, dstreg) = (src);
}}}	m68k_incpc (2);
return 0;
}
/* AND.L (An),Dn */
unsigned long REGPARAM2 CPUFUNC(op_c090_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5068;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	src &= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	do_cycles (2 * 256);
	m68k_dreg(regs, dstreg) = (src);
}}}}}	m68k_incpc (2);
endlabel5068: ;
return 0;
}
/* AND.L (An)+,Dn */
unsigned long REGPARAM2 CPUFUNC(op_c098_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5069;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	m68k_areg(regs, srcreg) += 4;
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	src &= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	do_cycles (2 * 256);
	m68k_dreg(regs, dstreg) = (src);
}}}}}	m68k_incpc (2);
endlabel5069: ;
return 0;
}
/* AND.L -(An),Dn */
unsigned long REGPARAM2 CPUFUNC(op_c0a0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = m68k_areg(regs, srcreg) - 4;
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5070;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	m68k_areg (regs, srcreg) = srca;
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	src &= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	do_cycles (2 * 256);
	m68k_dreg(regs, dstreg) = (src);
}}}}}	m68k_incpc (2);
endlabel5070: ;
return 0;
}
/* AND.L (d16,An),Dn */
unsigned long REGPARAM2 CPUFUNC(op_c0a8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5071;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	src &= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	do_cycles (2 * 256);
	m68k_dreg(regs, dstreg) = (src);
}}}}}	m68k_incpc (4);
endlabel5071: ;
return 0;
}
/* AND.L (d8,An,Xn),Dn */
unsigned long REGPARAM2 CPUFUNC(op_c0b0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5072;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	src &= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	do_cycles (2 * 256);
	m68k_dreg(regs, dstreg) = (src);
}}}}}	m68k_incpc (4);
endlabel5072: ;
return 0;
}
/* AND.L (xxx).W,Dn */
unsigned long REGPARAM2 CPUFUNC(op_c0b8_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5073;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	src &= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	do_cycles (2 * 256);
	m68k_dreg(regs, dstreg) = (src);
}}}}}	m68k_incpc (4);
endlabel5073: ;
return 0;
}
/* AND.L (xxx).L,Dn */
unsigned long REGPARAM2 CPUFUNC(op_c0b9_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5074;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	src &= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	do_cycles (2 * 256);
	m68k_dreg(regs, dstreg) = (src);
}}}}}	m68k_incpc (6);
endlabel5074: ;
return 0;
}
/* AND.L (d16,PC),Dn */
unsigned long REGPARAM2 CPUFUNC(op_c0ba_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5075;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	src &= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	do_cycles (2 * 256);
	m68k_dreg(regs, dstreg) = (src);
}}}}}	m68k_incpc (4);
endlabel5075: ;
return 0;
}
/* AND.L (d8,PC,Xn),Dn */
unsigned long REGPARAM2 CPUFUNC(op_c0bb_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	do_cycles (2 * 256);
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5076;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	src &= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	do_cycles (2 * 256);
	m68k_dreg(regs, dstreg) = (src);
}}}}}	m68k_incpc (4);
endlabel5076: ;
return 0;
}
/* AND.L #<data>.L,Dn */
unsigned long REGPARAM2 CPUFUNC(op_c0bc_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	src &= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	do_cycles (2 * 256);
	m68k_dreg(regs, dstreg) = (src);
}}}	m68k_incpc (6);
return 0;
}
/* MULU.W Dn,Dn */
unsigned long REGPARAM2 CPUFUNC(op_c0c0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 newv = (uae_u32)(uae_u16)dst * (uae_u32)(uae_u16)src;
	int cycles = 34, bits;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_NFLG (((uae_s32)(newv)) < 0);
	for(bits = 0; bits < 16 && src; bits++, src >>= 1)
		if (src & 1) cycles += 2;
	do_cycles (cycles * 256);
	m68k_dreg(regs, dstreg) = (newv);
	m68k_incpc (2);
}}}}return 0;
}
/* MULU.W (An),Dn */
unsigned long REGPARAM2 CPUFUNC(op_c0d0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5079;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 newv = (uae_u32)(uae_u16)dst * (uae_u32)(uae_u16)src;
	int cycles = 34, bits;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_NFLG (((uae_s32)(newv)) < 0);
	for(bits = 0; bits < 16 && src; bits++, src >>= 1)
		if (src & 1) cycles += 2;
	do_cycles (cycles * 256);
	m68k_dreg(regs, dstreg) = (newv);
	m68k_incpc (2);
}}}}}}endlabel5079: ;
return 0;
}
/* MULU.W (An)+,Dn */
unsigned long REGPARAM2 CPUFUNC(op_c0d8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5080;
	}
{{	uae_s16 src = get_word_ce (srca);
	m68k_areg(regs, srcreg) += 2;
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 newv = (uae_u32)(uae_u16)dst * (uae_u32)(uae_u16)src;
	int cycles = 34, bits;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_NFLG (((uae_s32)(newv)) < 0);
	for(bits = 0; bits < 16 && src; bits++, src >>= 1)
		if (src & 1) cycles += 2;
	do_cycles (cycles * 256);
	m68k_dreg(regs, dstreg) = (newv);
	m68k_incpc (2);
}}}}}}endlabel5080: ;
return 0;
}
/* MULU.W -(An),Dn */
unsigned long REGPARAM2 CPUFUNC(op_c0e0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = m68k_areg(regs, srcreg) - 2;
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5081;
	}
{{	uae_s16 src = get_word_ce (srca);
	m68k_areg (regs, srcreg) = srca;
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 newv = (uae_u32)(uae_u16)dst * (uae_u32)(uae_u16)src;
	int cycles = 34, bits;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_NFLG (((uae_s32)(newv)) < 0);
	for(bits = 0; bits < 16 && src; bits++, src >>= 1)
		if (src & 1) cycles += 2;
	do_cycles (cycles * 256);
	m68k_dreg(regs, dstreg) = (newv);
	m68k_incpc (2);
}}}}}}endlabel5081: ;
return 0;
}
/* MULU.W (d16,An),Dn */
unsigned long REGPARAM2 CPUFUNC(op_c0e8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5082;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	uae_u32 newv = (uae_u32)(uae_u16)dst * (uae_u32)(uae_u16)src;
	int cycles = 34, bits;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_NFLG (((uae_s32)(newv)) < 0);
	for(bits = 0; bits < 16 && src; bits++, src >>= 1)
		if (src & 1) cycles += 2;
	do_cycles (cycles * 256);
	m68k_dreg(regs, dstreg) = (newv);
	m68k_incpc (4);
}}}}}}endlabel5082: ;
return 0;
}
/* MULU.W (d8,An,Xn),Dn */
unsigned long REGPARAM2 CPUFUNC(op_c0f0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5083;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	uae_u32 newv = (uae_u32)(uae_u16)dst * (uae_u32)(uae_u16)src;
	int cycles = 34, bits;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_NFLG (((uae_s32)(newv)) < 0);
	for(bits = 0; bits < 16 && src; bits++, src >>= 1)
		if (src & 1) cycles += 2;
	do_cycles (cycles * 256);
	m68k_dreg(regs, dstreg) = (newv);
	m68k_incpc (4);
}}}}}}endlabel5083: ;
return 0;
}
/* MULU.W (xxx).W,Dn */
unsigned long REGPARAM2 CPUFUNC(op_c0f8_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5084;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	uae_u32 newv = (uae_u32)(uae_u16)dst * (uae_u32)(uae_u16)src;
	int cycles = 34, bits;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_NFLG (((uae_s32)(newv)) < 0);
	for(bits = 0; bits < 16 && src; bits++, src >>= 1)
		if (src & 1) cycles += 2;
	do_cycles (cycles * 256);
	m68k_dreg(regs, dstreg) = (newv);
	m68k_incpc (4);
}}}}}}endlabel5084: ;
return 0;
}
/* MULU.W (xxx).L,Dn */
unsigned long REGPARAM2 CPUFUNC(op_c0f9_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5085;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
{	uae_u32 newv = (uae_u32)(uae_u16)dst * (uae_u32)(uae_u16)src;
	int cycles = 34, bits;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_NFLG (((uae_s32)(newv)) < 0);
	for(bits = 0; bits < 16 && src; bits++, src >>= 1)
		if (src & 1) cycles += 2;
	do_cycles (cycles * 256);
	m68k_dreg(regs, dstreg) = (newv);
	m68k_incpc (6);
}}}}}}endlabel5085: ;
return 0;
}
/* MULU.W (d16,PC),Dn */
unsigned long REGPARAM2 CPUFUNC(op_c0fa_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5086;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	uae_u32 newv = (uae_u32)(uae_u16)dst * (uae_u32)(uae_u16)src;
	int cycles = 34, bits;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_NFLG (((uae_s32)(newv)) < 0);
	for(bits = 0; bits < 16 && src; bits++, src >>= 1)
		if (src & 1) cycles += 2;
	do_cycles (cycles * 256);
	m68k_dreg(regs, dstreg) = (newv);
	m68k_incpc (4);
}}}}}}endlabel5086: ;
return 0;
}
/* MULU.W (d8,PC,Xn),Dn */
unsigned long REGPARAM2 CPUFUNC(op_c0fb_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	do_cycles (2 * 256);
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5087;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	uae_u32 newv = (uae_u32)(uae_u16)dst * (uae_u32)(uae_u16)src;
	int cycles = 34, bits;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_NFLG (((uae_s32)(newv)) < 0);
	for(bits = 0; bits < 16 && src; bits++, src >>= 1)
		if (src & 1) cycles += 2;
	do_cycles (cycles * 256);
	m68k_dreg(regs, dstreg) = (newv);
	m68k_incpc (4);
}}}}}}endlabel5087: ;
return 0;
}
/* MULU.W #<data>.W,Dn */
unsigned long REGPARAM2 CPUFUNC(op_c0fc_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	uae_u32 newv = (uae_u32)(uae_u16)dst * (uae_u32)(uae_u16)src;
	int cycles = 34, bits;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_NFLG (((uae_s32)(newv)) < 0);
	for(bits = 0; bits < 16 && src; bits++, src >>= 1)
		if (src & 1) cycles += 2;
	do_cycles (cycles * 256);
	m68k_dreg(regs, dstreg) = (newv);
	m68k_incpc (4);
}}}}return 0;
}
/* ABCD.B Dn,Dn */
unsigned long REGPARAM2 CPUFUNC(op_c100_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	uae_s8 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u16 newv_lo = (src & 0xF) + (dst & 0xF) + (GET_XFLG ? 1 : 0);
	uae_u16 newv_hi = (src & 0xF0) + (dst & 0xF0);
	uae_u16 newv, tmp_newv;
	int cflg;
	newv = tmp_newv = newv_hi + newv_lo;	if (newv_lo > 9) { newv += 6; }
	cflg = (newv & 0x3F0) > 0x90;
	if (cflg) newv += 0x60;
	SET_CFLG (cflg);
	COPY_CARRY;
	SET_ZFLG (GET_ZFLG & (((uae_s8)(newv)) == 0));
	SET_NFLG (((uae_s8)(newv)) < 0);
	SET_VFLG ((tmp_newv & 0x80) == 0 && (newv & 0x80) != 0);
	do_cycles (2 * 256);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xff) | ((newv) & 0xff);
}}}}	m68k_incpc (2);
return 0;
}
/* ABCD.B -(An),-(An) */
unsigned long REGPARAM2 CPUFUNC(op_c108_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = get_byte_ce (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = get_byte_ce (dsta);
	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u16 newv_lo = (src & 0xF) + (dst & 0xF) + (GET_XFLG ? 1 : 0);
	uae_u16 newv_hi = (src & 0xF0) + (dst & 0xF0);
	uae_u16 newv, tmp_newv;
	int cflg;
	newv = tmp_newv = newv_hi + newv_lo;	if (newv_lo > 9) { newv += 6; }
	cflg = (newv & 0x3F0) > 0x90;
	if (cflg) newv += 0x60;
	SET_CFLG (cflg);
	COPY_CARRY;
	SET_ZFLG (GET_ZFLG & (((uae_s8)(newv)) == 0));
	SET_NFLG (((uae_s8)(newv)) < 0);
	SET_VFLG ((tmp_newv & 0x80) == 0 && (newv & 0x80) != 0);
	do_cycles (2 * 256);
	put_byte_ce (dsta,newv);
}}}}}}	m68k_incpc (2);
return 0;
}
/* AND.B Dn,(An) */
unsigned long REGPARAM2 CPUFUNC(op_c110_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = m68k_areg(regs, dstreg);
{	uae_s8 dst = get_byte_ce (dsta);
	src &= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	put_byte_ce (dsta,src);
}}}}	m68k_incpc (2);
return 0;
}
/* AND.B Dn,(An)+ */
unsigned long REGPARAM2 CPUFUNC(op_c118_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = m68k_areg(regs, dstreg);
{	uae_s8 dst = get_byte_ce (dsta);
	m68k_areg(regs, dstreg) += areg_byteinc[dstreg];
	src &= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	put_byte_ce (dsta,src);
}}}}	m68k_incpc (2);
return 0;
}
/* AND.B Dn,-(An) */
unsigned long REGPARAM2 CPUFUNC(op_c120_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = m68k_areg(regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = get_byte_ce (dsta);
	m68k_areg (regs, dstreg) = dsta;
	src &= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	put_byte_ce (dsta,src);
}}}}	m68k_incpc (2);
return 0;
}
/* AND.B Dn,(d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_c128_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 dst = get_byte_ce (dsta);
	src &= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	put_byte_ce (dsta,src);
}}}}	m68k_incpc (4);
return 0;
}
/* AND.B Dn,(d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_c130_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (4));
{	uae_s8 dst = get_byte_ce (dsta);
	src &= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	put_byte_ce (dsta,src);
}}}}	m68k_incpc (4);
return 0;
}
/* AND.B Dn,(xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_c138_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 dst = get_byte_ce (dsta);
	src &= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	put_byte_ce (dsta,src);
}}}}	m68k_incpc (4);
return 0;
}
/* AND.B Dn,(xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_c139_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (4) << 16;
	dsta |= get_word_ce_prefetch (6);
{	uae_s8 dst = get_byte_ce (dsta);
	src &= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	put_byte_ce (dsta,src);
}}}}	m68k_incpc (6);
return 0;
}
/* EXG.L Dn,Dn */
unsigned long REGPARAM2 CPUFUNC(op_c140_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	do_cycles (2 * 256);
	m68k_dreg(regs, srcreg) = (dst);
	m68k_dreg(regs, dstreg) = (src);
}}}	m68k_incpc (2);
return 0;
}
/* EXG.L An,An */
unsigned long REGPARAM2 CPUFUNC(op_c148_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_areg(regs, srcreg);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	do_cycles (2 * 256);
	m68k_areg(regs, srcreg) = (dst);
	m68k_areg(regs, dstreg) = (src);
}}}	m68k_incpc (2);
return 0;
}
/* AND.W Dn,(An) */
unsigned long REGPARAM2 CPUFUNC(op_c150_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel5100;
	}
{{	uae_s16 dst = get_word_ce (dsta);
	src &= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	put_word_ce (dsta,src);
}}}}}	m68k_incpc (2);
endlabel5100: ;
return 0;
}
/* AND.W Dn,(An)+ */
unsigned long REGPARAM2 CPUFUNC(op_c158_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel5101;
	}
{{	uae_s16 dst = get_word_ce (dsta);
	m68k_areg(regs, dstreg) += 2;
	src &= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	put_word_ce (dsta,src);
}}}}}	m68k_incpc (2);
endlabel5101: ;
return 0;
}
/* AND.W Dn,-(An) */
unsigned long REGPARAM2 CPUFUNC(op_c160_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = m68k_areg(regs, dstreg) - 2;
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel5102;
	}
{{	uae_s16 dst = get_word_ce (dsta);
	m68k_areg (regs, dstreg) = dsta;
	src &= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	put_word_ce (dsta,src);
}}}}}	m68k_incpc (2);
endlabel5102: ;
return 0;
}
/* AND.W Dn,(d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_c168_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel5103;
	}
{{	uae_s16 dst = get_word_ce (dsta);
	src &= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	put_word_ce (dsta,src);
}}}}}	m68k_incpc (4);
endlabel5103: ;
return 0;
}
/* AND.W Dn,(d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_c170_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (4));
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel5104;
	}
{{	uae_s16 dst = get_word_ce (dsta);
	src &= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	put_word_ce (dsta,src);
}}}}}	m68k_incpc (4);
endlabel5104: ;
return 0;
}
/* AND.W Dn,(xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_c178_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel5105;
	}
{{	uae_s16 dst = get_word_ce (dsta);
	src &= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	put_word_ce (dsta,src);
}}}}}	m68k_incpc (4);
endlabel5105: ;
return 0;
}
/* AND.W Dn,(xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_c179_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (4) << 16;
	dsta |= get_word_ce_prefetch (6);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel5106;
	}
{{	uae_s16 dst = get_word_ce (dsta);
	src &= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	put_word_ce (dsta,src);
}}}}}	m68k_incpc (6);
endlabel5106: ;
return 0;
}
/* EXG.L Dn,An */
unsigned long REGPARAM2 CPUFUNC(op_c188_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	do_cycles (2 * 256);
	m68k_dreg(regs, srcreg) = (dst);
	m68k_areg(regs, dstreg) = (src);
}}}	m68k_incpc (2);
return 0;
}
/* AND.L Dn,(An) */
unsigned long REGPARAM2 CPUFUNC(op_c190_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel5108;
	}
{{	uae_s32 dst = get_word_ce (dsta) << 16; dst |= get_word_ce (dsta + 2);
	src &= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
}}}}}	m68k_incpc (2);
endlabel5108: ;
return 0;
}
/* AND.L Dn,(An)+ */
unsigned long REGPARAM2 CPUFUNC(op_c198_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel5109;
	}
{{	uae_s32 dst = get_word_ce (dsta) << 16; dst |= get_word_ce (dsta + 2);
	m68k_areg(regs, dstreg) += 4;
	src &= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
}}}}}	m68k_incpc (2);
endlabel5109: ;
return 0;
}
/* AND.L Dn,-(An) */
unsigned long REGPARAM2 CPUFUNC(op_c1a0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = m68k_areg(regs, dstreg) - 4;
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel5110;
	}
{{	uae_s32 dst = get_word_ce (dsta) << 16; dst |= get_word_ce (dsta + 2);
	m68k_areg (regs, dstreg) = dsta;
	src &= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
}}}}}	m68k_incpc (2);
endlabel5110: ;
return 0;
}
/* AND.L Dn,(d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_c1a8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel5111;
	}
{{	uae_s32 dst = get_word_ce (dsta) << 16; dst |= get_word_ce (dsta + 2);
	src &= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
}}}}}	m68k_incpc (4);
endlabel5111: ;
return 0;
}
/* AND.L Dn,(d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_c1b0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (4));
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel5112;
	}
{{	uae_s32 dst = get_word_ce (dsta) << 16; dst |= get_word_ce (dsta + 2);
	src &= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
}}}}}	m68k_incpc (4);
endlabel5112: ;
return 0;
}
/* AND.L Dn,(xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_c1b8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel5113;
	}
{{	uae_s32 dst = get_word_ce (dsta) << 16; dst |= get_word_ce (dsta + 2);
	src &= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
}}}}}	m68k_incpc (4);
endlabel5113: ;
return 0;
}
/* AND.L Dn,(xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_c1b9_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (4) << 16;
	dsta |= get_word_ce_prefetch (6);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel5114;
	}
{{	uae_s32 dst = get_word_ce (dsta) << 16; dst |= get_word_ce (dsta + 2);
	src &= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
}}}}}	m68k_incpc (6);
endlabel5114: ;
return 0;
}
/* MULS.W Dn,Dn */
unsigned long REGPARAM2 CPUFUNC(op_c1c0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 newv = (uae_s32)(uae_s16)dst * (uae_s32)(uae_s16)src;
	int cycles = 34, bits;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_NFLG (((uae_s32)(newv)) < 0);
	for(bits = 0; bits < 16 && src; bits++, src >>= 1)
		if ((src & 3) == 1) cycles += 2;
	do_cycles (cycles * 256);
	m68k_dreg(regs, dstreg) = (newv);
}}}}	m68k_incpc (2);
return 0;
}
/* MULS.W (An),Dn */
unsigned long REGPARAM2 CPUFUNC(op_c1d0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5116;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 newv = (uae_s32)(uae_s16)dst * (uae_s32)(uae_s16)src;
	int cycles = 34, bits;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_NFLG (((uae_s32)(newv)) < 0);
	for(bits = 0; bits < 16 && src; bits++, src >>= 1)
		if ((src & 3) == 1) cycles += 2;
	do_cycles (cycles * 256);
	m68k_dreg(regs, dstreg) = (newv);
}}}}}}	m68k_incpc (2);
endlabel5116: ;
return 0;
}
/* MULS.W (An)+,Dn */
unsigned long REGPARAM2 CPUFUNC(op_c1d8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5117;
	}
{{	uae_s16 src = get_word_ce (srca);
	m68k_areg(regs, srcreg) += 2;
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 newv = (uae_s32)(uae_s16)dst * (uae_s32)(uae_s16)src;
	int cycles = 34, bits;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_NFLG (((uae_s32)(newv)) < 0);
	for(bits = 0; bits < 16 && src; bits++, src >>= 1)
		if ((src & 3) == 1) cycles += 2;
	do_cycles (cycles * 256);
	m68k_dreg(regs, dstreg) = (newv);
}}}}}}	m68k_incpc (2);
endlabel5117: ;
return 0;
}
/* MULS.W -(An),Dn */
unsigned long REGPARAM2 CPUFUNC(op_c1e0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = m68k_areg(regs, srcreg) - 2;
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5118;
	}
{{	uae_s16 src = get_word_ce (srca);
	m68k_areg (regs, srcreg) = srca;
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 newv = (uae_s32)(uae_s16)dst * (uae_s32)(uae_s16)src;
	int cycles = 34, bits;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_NFLG (((uae_s32)(newv)) < 0);
	for(bits = 0; bits < 16 && src; bits++, src >>= 1)
		if ((src & 3) == 1) cycles += 2;
	do_cycles (cycles * 256);
	m68k_dreg(regs, dstreg) = (newv);
}}}}}}	m68k_incpc (2);
endlabel5118: ;
return 0;
}
/* MULS.W (d16,An),Dn */
unsigned long REGPARAM2 CPUFUNC(op_c1e8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5119;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	uae_u32 newv = (uae_s32)(uae_s16)dst * (uae_s32)(uae_s16)src;
	int cycles = 34, bits;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_NFLG (((uae_s32)(newv)) < 0);
	for(bits = 0; bits < 16 && src; bits++, src >>= 1)
		if ((src & 3) == 1) cycles += 2;
	do_cycles (cycles * 256);
	m68k_dreg(regs, dstreg) = (newv);
}}}}}}	m68k_incpc (4);
endlabel5119: ;
return 0;
}
/* MULS.W (d8,An,Xn),Dn */
unsigned long REGPARAM2 CPUFUNC(op_c1f0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5120;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	uae_u32 newv = (uae_s32)(uae_s16)dst * (uae_s32)(uae_s16)src;
	int cycles = 34, bits;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_NFLG (((uae_s32)(newv)) < 0);
	for(bits = 0; bits < 16 && src; bits++, src >>= 1)
		if ((src & 3) == 1) cycles += 2;
	do_cycles (cycles * 256);
	m68k_dreg(regs, dstreg) = (newv);
}}}}}}	m68k_incpc (4);
endlabel5120: ;
return 0;
}
/* MULS.W (xxx).W,Dn */
unsigned long REGPARAM2 CPUFUNC(op_c1f8_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5121;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	uae_u32 newv = (uae_s32)(uae_s16)dst * (uae_s32)(uae_s16)src;
	int cycles = 34, bits;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_NFLG (((uae_s32)(newv)) < 0);
	for(bits = 0; bits < 16 && src; bits++, src >>= 1)
		if ((src & 3) == 1) cycles += 2;
	do_cycles (cycles * 256);
	m68k_dreg(regs, dstreg) = (newv);
}}}}}}	m68k_incpc (4);
endlabel5121: ;
return 0;
}
/* MULS.W (xxx).L,Dn */
unsigned long REGPARAM2 CPUFUNC(op_c1f9_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5122;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
{	uae_u32 newv = (uae_s32)(uae_s16)dst * (uae_s32)(uae_s16)src;
	int cycles = 34, bits;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_NFLG (((uae_s32)(newv)) < 0);
	for(bits = 0; bits < 16 && src; bits++, src >>= 1)
		if ((src & 3) == 1) cycles += 2;
	do_cycles (cycles * 256);
	m68k_dreg(regs, dstreg) = (newv);
}}}}}}	m68k_incpc (6);
endlabel5122: ;
return 0;
}
/* MULS.W (d16,PC),Dn */
unsigned long REGPARAM2 CPUFUNC(op_c1fa_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5123;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	uae_u32 newv = (uae_s32)(uae_s16)dst * (uae_s32)(uae_s16)src;
	int cycles = 34, bits;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_NFLG (((uae_s32)(newv)) < 0);
	for(bits = 0; bits < 16 && src; bits++, src >>= 1)
		if ((src & 3) == 1) cycles += 2;
	do_cycles (cycles * 256);
	m68k_dreg(regs, dstreg) = (newv);
}}}}}}	m68k_incpc (4);
endlabel5123: ;
return 0;
}
/* MULS.W (d8,PC,Xn),Dn */
unsigned long REGPARAM2 CPUFUNC(op_c1fb_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	do_cycles (2 * 256);
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5124;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	uae_u32 newv = (uae_s32)(uae_s16)dst * (uae_s32)(uae_s16)src;
	int cycles = 34, bits;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_NFLG (((uae_s32)(newv)) < 0);
	for(bits = 0; bits < 16 && src; bits++, src >>= 1)
		if ((src & 3) == 1) cycles += 2;
	do_cycles (cycles * 256);
	m68k_dreg(regs, dstreg) = (newv);
}}}}}}	m68k_incpc (4);
endlabel5124: ;
return 0;
}
/* MULS.W #<data>.W,Dn */
unsigned long REGPARAM2 CPUFUNC(op_c1fc_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	uae_u32 newv = (uae_s32)(uae_s16)dst * (uae_s32)(uae_s16)src;
	int cycles = 34, bits;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_NFLG (((uae_s32)(newv)) < 0);
	for(bits = 0; bits < 16 && src; bits++, src >>= 1)
		if ((src & 3) == 1) cycles += 2;
	do_cycles (cycles * 256);
	m68k_dreg(regs, dstreg) = (newv);
}}}}	m68k_incpc (4);
return 0;
}
/* ADD.B Dn,Dn */
unsigned long REGPARAM2 CPUFUNC(op_d000_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	uae_s8 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xff) | ((newv) & 0xff);
}}}}}}	m68k_incpc (2);
return 0;
}
/* ADD.B (An),Dn */
unsigned long REGPARAM2 CPUFUNC(op_d010_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
{	uae_s8 src = get_byte_ce (srca);
{	uae_s8 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xff) | ((newv) & 0xff);
}}}}}}}	m68k_incpc (2);
return 0;
}
/* ADD.B (An)+,Dn */
unsigned long REGPARAM2 CPUFUNC(op_d018_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
{	uae_s8 src = get_byte_ce (srca);
	m68k_areg(regs, srcreg) += areg_byteinc[srcreg];
{	uae_s8 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xff) | ((newv) & 0xff);
}}}}}}}	m68k_incpc (2);
return 0;
}
/* ADD.B -(An),Dn */
unsigned long REGPARAM2 CPUFUNC(op_d020_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = get_byte_ce (srca);
	m68k_areg (regs, srcreg) = srca;
{	uae_s8 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xff) | ((newv) & 0xff);
}}}}}}}	m68k_incpc (2);
return 0;
}
/* ADD.B (d16,An),Dn */
unsigned long REGPARAM2 CPUFUNC(op_d028_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 src = get_byte_ce (srca);
{	uae_s8 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xff) | ((newv) & 0xff);
}}}}}}}	m68k_incpc (4);
return 0;
}
/* ADD.B (d8,An,Xn),Dn */
unsigned long REGPARAM2 CPUFUNC(op_d030_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
{	uae_s8 src = get_byte_ce (srca);
{	uae_s8 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xff) | ((newv) & 0xff);
}}}}}}}	m68k_incpc (4);
return 0;
}
/* ADD.B (xxx).W,Dn */
unsigned long REGPARAM2 CPUFUNC(op_d038_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 src = get_byte_ce (srca);
{	uae_s8 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xff) | ((newv) & 0xff);
}}}}}}}	m68k_incpc (4);
return 0;
}
/* ADD.B (xxx).L,Dn */
unsigned long REGPARAM2 CPUFUNC(op_d039_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
{	uae_s8 src = get_byte_ce (srca);
{	uae_s8 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xff) | ((newv) & 0xff);
}}}}}}}	m68k_incpc (6);
return 0;
}
/* ADD.B (d16,PC),Dn */
unsigned long REGPARAM2 CPUFUNC(op_d03a_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 src = get_byte_ce (srca);
{	uae_s8 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xff) | ((newv) & 0xff);
}}}}}}}	m68k_incpc (4);
return 0;
}
/* ADD.B (d8,PC,Xn),Dn */
unsigned long REGPARAM2 CPUFUNC(op_d03b_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	do_cycles (2 * 256);
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
{	uae_s8 src = get_byte_ce (srca);
{	uae_s8 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xff) | ((newv) & 0xff);
}}}}}}}	m68k_incpc (4);
return 0;
}
/* ADD.B #<data>.B,Dn */
unsigned long REGPARAM2 CPUFUNC(op_d03c_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s8 src = (uae_u8)get_word_ce_prefetch (4);
{	uae_s8 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xff) | ((newv) & 0xff);
}}}}}}	m68k_incpc (4);
return 0;
}
/* ADD.W Dn,Dn */
unsigned long REGPARAM2 CPUFUNC(op_d040_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xffff) | ((newv) & 0xffff);
}}}}}}	m68k_incpc (2);
return 0;
}
/* ADD.W An,Dn */
unsigned long REGPARAM2 CPUFUNC(op_d048_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_areg(regs, srcreg);
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xffff) | ((newv) & 0xffff);
}}}}}}	m68k_incpc (2);
return 0;
}
/* ADD.W (An),Dn */
unsigned long REGPARAM2 CPUFUNC(op_d050_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5139;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xffff) | ((newv) & 0xffff);
}}}}}}}}	m68k_incpc (2);
endlabel5139: ;
return 0;
}
/* ADD.W (An)+,Dn */
unsigned long REGPARAM2 CPUFUNC(op_d058_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5140;
	}
{{	uae_s16 src = get_word_ce (srca);
	m68k_areg(regs, srcreg) += 2;
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xffff) | ((newv) & 0xffff);
}}}}}}}}	m68k_incpc (2);
endlabel5140: ;
return 0;
}
/* ADD.W -(An),Dn */
unsigned long REGPARAM2 CPUFUNC(op_d060_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = m68k_areg(regs, srcreg) - 2;
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5141;
	}
{{	uae_s16 src = get_word_ce (srca);
	m68k_areg (regs, srcreg) = srca;
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xffff) | ((newv) & 0xffff);
}}}}}}}}	m68k_incpc (2);
endlabel5141: ;
return 0;
}
/* ADD.W (d16,An),Dn */
unsigned long REGPARAM2 CPUFUNC(op_d068_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5142;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xffff) | ((newv) & 0xffff);
}}}}}}}}	m68k_incpc (4);
endlabel5142: ;
return 0;
}
/* ADD.W (d8,An,Xn),Dn */
unsigned long REGPARAM2 CPUFUNC(op_d070_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5143;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xffff) | ((newv) & 0xffff);
}}}}}}}}	m68k_incpc (4);
endlabel5143: ;
return 0;
}
/* ADD.W (xxx).W,Dn */
unsigned long REGPARAM2 CPUFUNC(op_d078_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5144;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xffff) | ((newv) & 0xffff);
}}}}}}}}	m68k_incpc (4);
endlabel5144: ;
return 0;
}
/* ADD.W (xxx).L,Dn */
unsigned long REGPARAM2 CPUFUNC(op_d079_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5145;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xffff) | ((newv) & 0xffff);
}}}}}}}}	m68k_incpc (6);
endlabel5145: ;
return 0;
}
/* ADD.W (d16,PC),Dn */
unsigned long REGPARAM2 CPUFUNC(op_d07a_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5146;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xffff) | ((newv) & 0xffff);
}}}}}}}}	m68k_incpc (4);
endlabel5146: ;
return 0;
}
/* ADD.W (d8,PC,Xn),Dn */
unsigned long REGPARAM2 CPUFUNC(op_d07b_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	do_cycles (2 * 256);
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5147;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xffff) | ((newv) & 0xffff);
}}}}}}}}	m68k_incpc (4);
endlabel5147: ;
return 0;
}
/* ADD.W #<data>.W,Dn */
unsigned long REGPARAM2 CPUFUNC(op_d07c_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xffff) | ((newv) & 0xffff);
}}}}}}	m68k_incpc (4);
return 0;
}
/* ADD.L Dn,Dn */
unsigned long REGPARAM2 CPUFUNC(op_d080_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	do_cycles (2 * 256);
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	m68k_dreg(regs, dstreg) = (newv);
}}}}}}	m68k_incpc (2);
return 0;
}
/* ADD.L An,Dn */
unsigned long REGPARAM2 CPUFUNC(op_d088_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_areg(regs, srcreg);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	do_cycles (2 * 256);
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	m68k_dreg(regs, dstreg) = (newv);
}}}}}}	m68k_incpc (2);
return 0;
}
/* ADD.L (An),Dn */
unsigned long REGPARAM2 CPUFUNC(op_d090_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5151;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	do_cycles (2 * 256);
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	m68k_dreg(regs, dstreg) = (newv);
}}}}}}}}	m68k_incpc (2);
endlabel5151: ;
return 0;
}
/* ADD.L (An)+,Dn */
unsigned long REGPARAM2 CPUFUNC(op_d098_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5152;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	m68k_areg(regs, srcreg) += 4;
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	do_cycles (2 * 256);
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	m68k_dreg(regs, dstreg) = (newv);
}}}}}}}}	m68k_incpc (2);
endlabel5152: ;
return 0;
}
/* ADD.L -(An),Dn */
unsigned long REGPARAM2 CPUFUNC(op_d0a0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = m68k_areg(regs, srcreg) - 4;
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5153;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	m68k_areg (regs, srcreg) = srca;
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	do_cycles (2 * 256);
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	m68k_dreg(regs, dstreg) = (newv);
}}}}}}}}	m68k_incpc (2);
endlabel5153: ;
return 0;
}
/* ADD.L (d16,An),Dn */
unsigned long REGPARAM2 CPUFUNC(op_d0a8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5154;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	do_cycles (2 * 256);
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	m68k_dreg(regs, dstreg) = (newv);
}}}}}}}}	m68k_incpc (4);
endlabel5154: ;
return 0;
}
/* ADD.L (d8,An,Xn),Dn */
unsigned long REGPARAM2 CPUFUNC(op_d0b0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5155;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	do_cycles (2 * 256);
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	m68k_dreg(regs, dstreg) = (newv);
}}}}}}}}	m68k_incpc (4);
endlabel5155: ;
return 0;
}
/* ADD.L (xxx).W,Dn */
unsigned long REGPARAM2 CPUFUNC(op_d0b8_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5156;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	do_cycles (2 * 256);
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	m68k_dreg(regs, dstreg) = (newv);
}}}}}}}}	m68k_incpc (4);
endlabel5156: ;
return 0;
}
/* ADD.L (xxx).L,Dn */
unsigned long REGPARAM2 CPUFUNC(op_d0b9_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5157;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	do_cycles (2 * 256);
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	m68k_dreg(regs, dstreg) = (newv);
}}}}}}}}	m68k_incpc (6);
endlabel5157: ;
return 0;
}
/* ADD.L (d16,PC),Dn */
unsigned long REGPARAM2 CPUFUNC(op_d0ba_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5158;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	do_cycles (2 * 256);
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	m68k_dreg(regs, dstreg) = (newv);
}}}}}}}}	m68k_incpc (4);
endlabel5158: ;
return 0;
}
/* ADD.L (d8,PC,Xn),Dn */
unsigned long REGPARAM2 CPUFUNC(op_d0bb_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	do_cycles (2 * 256);
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5159;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	do_cycles (2 * 256);
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	m68k_dreg(regs, dstreg) = (newv);
}}}}}}}}	m68k_incpc (4);
endlabel5159: ;
return 0;
}
/* ADD.L #<data>.L,Dn */
unsigned long REGPARAM2 CPUFUNC(op_d0bc_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	do_cycles (4 * 256);
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	m68k_dreg(regs, dstreg) = (newv);
}}}}}}	m68k_incpc (6);
return 0;
}
/* ADDA.W Dn,An */
unsigned long REGPARAM2 CPUFUNC(op_d0c0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	do_cycles (2 * 256);
{	uae_u32 newv = dst + src;
	m68k_areg(regs, dstreg) = (newv);
}}}}	m68k_incpc (2);
return 0;
}
/* ADDA.W An,An */
unsigned long REGPARAM2 CPUFUNC(op_d0c8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_areg(regs, srcreg);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	do_cycles (2 * 256);
{	uae_u32 newv = dst + src;
	m68k_areg(regs, dstreg) = (newv);
}}}}	m68k_incpc (2);
return 0;
}
/* ADDA.W (An),An */
unsigned long REGPARAM2 CPUFUNC(op_d0d0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5163;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	do_cycles (2 * 256);
{	uae_u32 newv = dst + src;
	m68k_areg(regs, dstreg) = (newv);
}}}}}}	m68k_incpc (2);
endlabel5163: ;
return 0;
}
/* ADDA.W (An)+,An */
unsigned long REGPARAM2 CPUFUNC(op_d0d8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5164;
	}
{{	uae_s16 src = get_word_ce (srca);
	m68k_areg(regs, srcreg) += 2;
{	uae_s32 dst = m68k_areg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	do_cycles (2 * 256);
{	uae_u32 newv = dst + src;
	m68k_areg(regs, dstreg) = (newv);
}}}}}}	m68k_incpc (2);
endlabel5164: ;
return 0;
}
/* ADDA.W -(An),An */
unsigned long REGPARAM2 CPUFUNC(op_d0e0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = m68k_areg(regs, srcreg) - 2;
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5165;
	}
{{	uae_s16 src = get_word_ce (srca);
	m68k_areg (regs, srcreg) = srca;
{	uae_s32 dst = m68k_areg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	do_cycles (2 * 256);
{	uae_u32 newv = dst + src;
	m68k_areg(regs, dstreg) = (newv);
}}}}}}	m68k_incpc (2);
endlabel5165: ;
return 0;
}
/* ADDA.W (d16,An),An */
unsigned long REGPARAM2 CPUFUNC(op_d0e8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5166;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	do_cycles (2 * 256);
{	uae_u32 newv = dst + src;
	m68k_areg(regs, dstreg) = (newv);
}}}}}}	m68k_incpc (4);
endlabel5166: ;
return 0;
}
/* ADDA.W (d8,An,Xn),An */
unsigned long REGPARAM2 CPUFUNC(op_d0f0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5167;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	do_cycles (2 * 256);
{	uae_u32 newv = dst + src;
	m68k_areg(regs, dstreg) = (newv);
}}}}}}	m68k_incpc (4);
endlabel5167: ;
return 0;
}
/* ADDA.W (xxx).W,An */
unsigned long REGPARAM2 CPUFUNC(op_d0f8_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5168;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	do_cycles (2 * 256);
{	uae_u32 newv = dst + src;
	m68k_areg(regs, dstreg) = (newv);
}}}}}}	m68k_incpc (4);
endlabel5168: ;
return 0;
}
/* ADDA.W (xxx).L,An */
unsigned long REGPARAM2 CPUFUNC(op_d0f9_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5169;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	do_cycles (2 * 256);
{	uae_u32 newv = dst + src;
	m68k_areg(regs, dstreg) = (newv);
}}}}}}	m68k_incpc (6);
endlabel5169: ;
return 0;
}
/* ADDA.W (d16,PC),An */
unsigned long REGPARAM2 CPUFUNC(op_d0fa_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5170;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	do_cycles (2 * 256);
{	uae_u32 newv = dst + src;
	m68k_areg(regs, dstreg) = (newv);
}}}}}}	m68k_incpc (4);
endlabel5170: ;
return 0;
}
/* ADDA.W (d8,PC,Xn),An */
unsigned long REGPARAM2 CPUFUNC(op_d0fb_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	do_cycles (2 * 256);
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5171;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	do_cycles (2 * 256);
{	uae_u32 newv = dst + src;
	m68k_areg(regs, dstreg) = (newv);
}}}}}}	m68k_incpc (4);
endlabel5171: ;
return 0;
}
/* ADDA.W #<data>.W,An */
unsigned long REGPARAM2 CPUFUNC(op_d0fc_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	do_cycles (2 * 256);
{	uae_u32 newv = dst + src;
	m68k_areg(regs, dstreg) = (newv);
}}}}	m68k_incpc (4);
return 0;
}
/* ADDX.B Dn,Dn */
unsigned long REGPARAM2 CPUFUNC(op_d100_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	uae_s8 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 newv = dst + src + (GET_XFLG ? 1 : 0);
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgo) & (flgo ^ flgn)));
	COPY_CARRY;
	SET_ZFLG (GET_ZFLG & (((uae_s8)(newv)) == 0));
	SET_NFLG (((uae_s8)(newv)) < 0);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xff) | ((newv) & 0xff);
}}}}}	m68k_incpc (2);
return 0;
}
/* ADDX.B -(An),-(An) */
unsigned long REGPARAM2 CPUFUNC(op_d108_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = get_byte_ce (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = get_byte_ce (dsta);
	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 newv = dst + src + (GET_XFLG ? 1 : 0);
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgo) & (flgo ^ flgn)));
	COPY_CARRY;
	SET_ZFLG (GET_ZFLG & (((uae_s8)(newv)) == 0));
	SET_NFLG (((uae_s8)(newv)) < 0);
	do_cycles (2 * 256);
	put_byte_ce (dsta,newv);
}}}}}}}	m68k_incpc (2);
return 0;
}
/* ADD.B Dn,(An) */
unsigned long REGPARAM2 CPUFUNC(op_d110_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = m68k_areg(regs, dstreg);
{	uae_s8 dst = get_byte_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_byte_ce (dsta,newv);
}}}}}}}	m68k_incpc (2);
return 0;
}
/* ADD.B Dn,(An)+ */
unsigned long REGPARAM2 CPUFUNC(op_d118_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = m68k_areg(regs, dstreg);
{	uae_s8 dst = get_byte_ce (dsta);
	m68k_areg(regs, dstreg) += areg_byteinc[dstreg];
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_byte_ce (dsta,newv);
}}}}}}}	m68k_incpc (2);
return 0;
}
/* ADD.B Dn,-(An) */
unsigned long REGPARAM2 CPUFUNC(op_d120_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = m68k_areg(regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = get_byte_ce (dsta);
	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_byte_ce (dsta,newv);
}}}}}}}	m68k_incpc (2);
return 0;
}
/* ADD.B Dn,(d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_d128_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 dst = get_byte_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_byte_ce (dsta,newv);
}}}}}}}	m68k_incpc (4);
return 0;
}
/* ADD.B Dn,(d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_d130_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (4));
{	uae_s8 dst = get_byte_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_byte_ce (dsta,newv);
}}}}}}}	m68k_incpc (4);
return 0;
}
/* ADD.B Dn,(xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_d138_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 dst = get_byte_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_byte_ce (dsta,newv);
}}}}}}}	m68k_incpc (4);
return 0;
}
/* ADD.B Dn,(xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_d139_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (4) << 16;
	dsta |= get_word_ce_prefetch (6);
{	uae_s8 dst = get_byte_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_byte_ce (dsta,newv);
}}}}}}}	m68k_incpc (6);
return 0;
}
/* ADDX.W Dn,Dn */
unsigned long REGPARAM2 CPUFUNC(op_d140_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 newv = dst + src + (GET_XFLG ? 1 : 0);
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgo) & (flgo ^ flgn)));
	COPY_CARRY;
	SET_ZFLG (GET_ZFLG & (((uae_s16)(newv)) == 0));
	SET_NFLG (((uae_s16)(newv)) < 0);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xffff) | ((newv) & 0xffff);
}}}}}	m68k_incpc (2);
return 0;
}
/* ADDX.W -(An),-(An) */
unsigned long REGPARAM2 CPUFUNC(op_d148_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg(regs, srcreg) - 2;
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5183;
	}
{{	uae_s16 src = get_word_ce (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - 2;
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel5183;
	}
{{	uae_s16 dst = get_word_ce (dsta);
	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 newv = dst + src + (GET_XFLG ? 1 : 0);
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgo) & (flgo ^ flgn)));
	COPY_CARRY;
	SET_ZFLG (GET_ZFLG & (((uae_s16)(newv)) == 0));
	SET_NFLG (((uae_s16)(newv)) < 0);
	do_cycles (2 * 256);
	put_word_ce (dsta,newv);
}}}}}}}}}	m68k_incpc (2);
endlabel5183: ;
return 0;
}
/* ADD.W Dn,(An) */
unsigned long REGPARAM2 CPUFUNC(op_d150_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel5184;
	}
{{	uae_s16 dst = get_word_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_word_ce (dsta,newv);
}}}}}}}}	m68k_incpc (2);
endlabel5184: ;
return 0;
}
/* ADD.W Dn,(An)+ */
unsigned long REGPARAM2 CPUFUNC(op_d158_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel5185;
	}
{{	uae_s16 dst = get_word_ce (dsta);
	m68k_areg(regs, dstreg) += 2;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_word_ce (dsta,newv);
}}}}}}}}	m68k_incpc (2);
endlabel5185: ;
return 0;
}
/* ADD.W Dn,-(An) */
unsigned long REGPARAM2 CPUFUNC(op_d160_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = m68k_areg(regs, dstreg) - 2;
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel5186;
	}
{{	uae_s16 dst = get_word_ce (dsta);
	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_word_ce (dsta,newv);
}}}}}}}}	m68k_incpc (2);
endlabel5186: ;
return 0;
}
/* ADD.W Dn,(d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_d168_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel5187;
	}
{{	uae_s16 dst = get_word_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_word_ce (dsta,newv);
}}}}}}}}	m68k_incpc (4);
endlabel5187: ;
return 0;
}
/* ADD.W Dn,(d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_d170_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (4));
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel5188;
	}
{{	uae_s16 dst = get_word_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_word_ce (dsta,newv);
}}}}}}}}	m68k_incpc (4);
endlabel5188: ;
return 0;
}
/* ADD.W Dn,(xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_d178_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel5189;
	}
{{	uae_s16 dst = get_word_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_word_ce (dsta,newv);
}}}}}}}}	m68k_incpc (4);
endlabel5189: ;
return 0;
}
/* ADD.W Dn,(xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_d179_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (4) << 16;
	dsta |= get_word_ce_prefetch (6);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel5190;
	}
{{	uae_s16 dst = get_word_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_word_ce (dsta,newv);
}}}}}}}}	m68k_incpc (6);
endlabel5190: ;
return 0;
}
/* ADDX.L Dn,Dn */
unsigned long REGPARAM2 CPUFUNC(op_d180_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 newv = dst + src + (GET_XFLG ? 1 : 0);
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgo) & (flgo ^ flgn)));
	COPY_CARRY;
	SET_ZFLG (GET_ZFLG & (((uae_s32)(newv)) == 0));
	SET_NFLG (((uae_s32)(newv)) < 0);
	do_cycles (2 * 256);
	m68k_dreg(regs, dstreg) = (newv);
}}}}}	m68k_incpc (2);
return 0;
}
/* ADDX.L -(An),-(An) */
unsigned long REGPARAM2 CPUFUNC(op_d188_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg(regs, srcreg) - 4;
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5192;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - 4;
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel5192;
	}
{{	uae_s32 dst = get_word_ce (dsta) << 16; dst |= get_word_ce (dsta + 2);
	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 newv = dst + src + (GET_XFLG ? 1 : 0);
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgo) & (flgo ^ flgn)));
	COPY_CARRY;
	SET_ZFLG (GET_ZFLG & (((uae_s32)(newv)) == 0));
	SET_NFLG (((uae_s32)(newv)) < 0);
	do_cycles (2 * 256);
	put_word_ce (dsta, newv >> 16); put_word_ce (dsta + 2, newv);
}}}}}}}}}	m68k_incpc (2);
endlabel5192: ;
return 0;
}
/* ADD.L Dn,(An) */
unsigned long REGPARAM2 CPUFUNC(op_d190_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel5193;
	}
{{	uae_s32 dst = get_word_ce (dsta) << 16; dst |= get_word_ce (dsta + 2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_word_ce (dsta, newv >> 16); put_word_ce (dsta + 2, newv);
}}}}}}}}	m68k_incpc (2);
endlabel5193: ;
return 0;
}
/* ADD.L Dn,(An)+ */
unsigned long REGPARAM2 CPUFUNC(op_d198_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel5194;
	}
{{	uae_s32 dst = get_word_ce (dsta) << 16; dst |= get_word_ce (dsta + 2);
	m68k_areg(regs, dstreg) += 4;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_word_ce (dsta, newv >> 16); put_word_ce (dsta + 2, newv);
}}}}}}}}	m68k_incpc (2);
endlabel5194: ;
return 0;
}
/* ADD.L Dn,-(An) */
unsigned long REGPARAM2 CPUFUNC(op_d1a0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = m68k_areg(regs, dstreg) - 4;
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel5195;
	}
{{	uae_s32 dst = get_word_ce (dsta) << 16; dst |= get_word_ce (dsta + 2);
	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_word_ce (dsta, newv >> 16); put_word_ce (dsta + 2, newv);
}}}}}}}}	m68k_incpc (2);
endlabel5195: ;
return 0;
}
/* ADD.L Dn,(d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_d1a8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel5196;
	}
{{	uae_s32 dst = get_word_ce (dsta) << 16; dst |= get_word_ce (dsta + 2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_word_ce (dsta, newv >> 16); put_word_ce (dsta + 2, newv);
}}}}}}}}	m68k_incpc (4);
endlabel5196: ;
return 0;
}
/* ADD.L Dn,(d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_d1b0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	do_cycles (2 * 256);
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (4));
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel5197;
	}
{{	uae_s32 dst = get_word_ce (dsta) << 16; dst |= get_word_ce (dsta + 2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_word_ce (dsta, newv >> 16); put_word_ce (dsta + 2, newv);
}}}}}}}}	m68k_incpc (4);
endlabel5197: ;
return 0;
}
/* ADD.L Dn,(xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_d1b8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel5198;
	}
{{	uae_s32 dst = get_word_ce (dsta) << 16; dst |= get_word_ce (dsta + 2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_word_ce (dsta, newv >> 16); put_word_ce (dsta + 2, newv);
}}}}}}}}	m68k_incpc (4);
endlabel5198: ;
return 0;
}
/* ADD.L Dn,(xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_d1b9_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (4) << 16;
	dsta |= get_word_ce_prefetch (6);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel5199;
	}
{{	uae_s32 dst = get_word_ce (dsta) << 16; dst |= get_word_ce (dsta + 2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY;
	SET_NFLG (flgn != 0);
	put_word_ce (dsta, newv >> 16); put_word_ce (dsta + 2, newv);
}}}}}}}}	m68k_incpc (6);
endlabel5199: ;
return 0;
}
/* ADDA.L Dn,An */
unsigned long REGPARAM2 CPUFUNC(op_d1c0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	do_cycles (2 * 256);
{	uae_u32 newv = dst + src;
	m68k_areg(regs, dstreg) = (newv);
}}}}	m68k_incpc (2);
return 0;
}
/* ADDA.L An,An */
unsigned long REGPARAM2 CPUFUNC(op_d1c8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_areg(regs, srcreg);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	do_cycles (2 * 256);
{	uae_u32 newv = dst + src;
	m68k_areg(regs, dstreg) = (newv);
}}}}	m68k_incpc (2);
return 0;
}
/* ADDA.L (An),An */
unsigned long REGPARAM2 CPUFUNC(op_d1d0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5202;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	do_cycles (2 * 256);
{	uae_u32 newv = dst + src;
	m68k_areg(regs, dstreg) = (newv);
}}}}}}	m68k_incpc (2);
endlabel5202: ;
return 0;
}
/* ADDA.L (An)+,An */
unsigned long REGPARAM2 CPUFUNC(op_d1d8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5203;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	m68k_areg(regs, srcreg) += 4;
{	uae_s32 dst = m68k_areg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	do_cycles (2 * 256);
{	uae_u32 newv = dst + src;
	m68k_areg(regs, dstreg) = (newv);
}}}}}}	m68k_incpc (2);
endlabel5203: ;
return 0;
}
#endif

#ifdef PART_8
/* ADDA.L -(An),An */
unsigned long REGPARAM2 CPUFUNC(op_d1e0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = m68k_areg(regs, srcreg) - 4;
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5204;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	m68k_areg (regs, srcreg) = srca;
{	uae_s32 dst = m68k_areg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	do_cycles (2 * 256);
{	uae_u32 newv = dst + src;
	m68k_areg(regs, dstreg) = (newv);
}}}}}}	m68k_incpc (2);
endlabel5204: ;
return 0;
}
/* ADDA.L (d16,An),An */
unsigned long REGPARAM2 CPUFUNC(op_d1e8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5205;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	do_cycles (2 * 256);
{	uae_u32 newv = dst + src;
	m68k_areg(regs, dstreg) = (newv);
}}}}}}	m68k_incpc (4);
endlabel5205: ;
return 0;
}
/* ADDA.L (d8,An,Xn),An */
unsigned long REGPARAM2 CPUFUNC(op_d1f0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	do_cycles (2 * 256);
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5206;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	do_cycles (2 * 256);
{	uae_u32 newv = dst + src;
	m68k_areg(regs, dstreg) = (newv);
}}}}}}	m68k_incpc (4);
endlabel5206: ;
return 0;
}
/* ADDA.L (xxx).W,An */
unsigned long REGPARAM2 CPUFUNC(op_d1f8_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5207;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	do_cycles (2 * 256);
{	uae_u32 newv = dst + src;
	m68k_areg(regs, dstreg) = (newv);
}}}}}}	m68k_incpc (4);
endlabel5207: ;
return 0;
}
/* ADDA.L (xxx).L,An */
unsigned long REGPARAM2 CPUFUNC(op_d1f9_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5208;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	do_cycles (2 * 256);
{	uae_u32 newv = dst + src;
	m68k_areg(regs, dstreg) = (newv);
}}}}}}	m68k_incpc (6);
endlabel5208: ;
return 0;
}
/* ADDA.L (d16,PC),An */
unsigned long REGPARAM2 CPUFUNC(op_d1fa_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5209;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	do_cycles (2 * 256);
{	uae_u32 newv = dst + src;
	m68k_areg(regs, dstreg) = (newv);
}}}}}}	m68k_incpc (4);
endlabel5209: ;
return 0;
}
/* ADDA.L (d8,PC,Xn),An */
unsigned long REGPARAM2 CPUFUNC(op_d1fb_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	do_cycles (2 * 256);
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5210;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	do_cycles (2 * 256);
{	uae_u32 newv = dst + src;
	m68k_areg(regs, dstreg) = (newv);
}}}}}}	m68k_incpc (4);
endlabel5210: ;
return 0;
}
/* ADDA.L #<data>.L,An */
unsigned long REGPARAM2 CPUFUNC(op_d1fc_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	do_cycles (2 * 256);
{	uae_u32 newv = dst + src;
	m68k_areg(regs, dstreg) = (newv);
}}}}	m68k_incpc (6);
return 0;
}
/* ASR.B #<data>,Dn */
unsigned long REGPARAM2 CPUFUNC(op_e000_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 cnt = srcreg;
{	uae_s8 data = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 val = (uae_u8)data;
	uae_u32 sign = (0x80 & val) >> 7;
	cnt &= 63;
	CLEAR_CZNV;
	if (cnt >= 8) {
		val = 0xff & (uae_u32)-sign;
		SET_CFLG (sign);
	COPY_CARRY;
	} else {
		val >>= cnt - 1;
		SET_CFLG (val & 1);
	COPY_CARRY;
		val >>= 1;
		val |= (0xff << (8 - cnt)) & (uae_u32)-sign;
		val &= 0xff;
	}
	SET_ZFLG (((uae_s8)(val)) == 0);
	SET_NFLG (((uae_s8)(val)) < 0);
	{
		int cycles = 2;
		cycles += 2 * cnt;
		do_cycles (cycles * 256);
	}
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xff) | ((val) & 0xff);
}}}}	m68k_incpc (2);
return 0;
}
/* LSR.B #<data>,Dn */
unsigned long REGPARAM2 CPUFUNC(op_e008_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 cnt = srcreg;
{	uae_s8 data = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 val = (uae_u8)data;
	cnt &= 63;
	CLEAR_CZNV;
	if (cnt >= 8) {
		SET_CFLG ((cnt == 8) & (val >> 7));
	COPY_CARRY;
		val = 0;
	} else {
		val >>= cnt - 1;
		SET_CFLG (val & 1);
	COPY_CARRY;
		val >>= 1;
	}
	SET_ZFLG (((uae_s8)(val)) == 0);
	SET_NFLG (((uae_s8)(val)) < 0);
	{
		int cycles = 2;
		cycles += 2 * cnt;
		do_cycles (cycles * 256);
	}
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xff) | ((val) & 0xff);
}}}}	m68k_incpc (2);
return 0;
}
/* ROXR.B #<data>,Dn */
unsigned long REGPARAM2 CPUFUNC(op_e010_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 cnt = srcreg;
{	uae_s8 data = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 val = (uae_u8)data;
	cnt &= 63;
	CLEAR_CZNV;
{	cnt--;
	{
	uae_u32 carry;
	uae_u32 hival = (val << 1) | GET_XFLG;
	hival <<= (7 - cnt);
	val >>= cnt;
	carry = val & 1;
	val >>= 1;
	val |= hival;
	SET_XFLG (carry);
	val &= 0xff;
	} }
	SET_CFLG (GET_XFLG);
	SET_ZFLG (((uae_s8)(val)) == 0);
	SET_NFLG (((uae_s8)(val)) < 0);
	{
		int cycles = 2;
		cycles += 2 * cnt;
		do_cycles (cycles * 256);
	}
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xff) | ((val) & 0xff);
}}}}	m68k_incpc (2);
return 0;
}
/* ROR.B #<data>,Dn */
unsigned long REGPARAM2 CPUFUNC(op_e018_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 cnt = srcreg;
{	uae_s8 data = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 val = (uae_u8)data;
	cnt &= 63;
	CLEAR_CZNV;
{	uae_u32 hival;
	cnt &= 7;
	hival = val << (8 - cnt);
	val >>= cnt;
	val |= hival;
	val &= 0xff;
	SET_CFLG ((val & 0x80) >> 7);
	}
	SET_ZFLG (((uae_s8)(val)) == 0);
	SET_NFLG (((uae_s8)(val)) < 0);
	{
		int cycles = 2;
		cycles += 2 * cnt;
		do_cycles (cycles * 256);
	}
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xff) | ((val) & 0xff);
}}}}	m68k_incpc (2);
return 0;
}
/* ASR.B Dn,Dn */
unsigned long REGPARAM2 CPUFUNC(op_e020_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 cnt = m68k_dreg(regs, srcreg);
{	uae_s8 data = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 val = (uae_u8)data;
	uae_u32 sign = (0x80 & val) >> 7;
	cnt &= 63;
	CLEAR_CZNV;
	if (cnt >= 8) {
		val = 0xff & (uae_u32)-sign;
		SET_CFLG (sign);
	COPY_CARRY;
	} else if (cnt > 0) {
		val >>= cnt - 1;
		SET_CFLG (val & 1);
	COPY_CARRY;
		val >>= 1;
		val |= (0xff << (8 - cnt)) & (uae_u32)-sign;
		val &= 0xff;
	}
	SET_ZFLG (((uae_s8)(val)) == 0);
	SET_NFLG (((uae_s8)(val)) < 0);
	{
		int cycles = 2;
		cycles += 2 * cnt;
		do_cycles (cycles * 256);
	}
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xff) | ((val) & 0xff);
}}}}	m68k_incpc (2);
return 0;
}
/* LSR.B Dn,Dn */
unsigned long REGPARAM2 CPUFUNC(op_e028_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 cnt = m68k_dreg(regs, srcreg);
{	uae_s8 data = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 val = (uae_u8)data;
	cnt &= 63;
	CLEAR_CZNV;
	if (cnt >= 8) {
		SET_CFLG ((cnt == 8) & (val >> 7));
	COPY_CARRY;
		val = 0;
	} else if (cnt > 0) {
		val >>= cnt - 1;
		SET_CFLG (val & 1);
	COPY_CARRY;
		val >>= 1;
	}
	SET_ZFLG (((uae_s8)(val)) == 0);
	SET_NFLG (((uae_s8)(val)) < 0);
	{
		int cycles = 2;
		cycles += 2 * cnt;
		do_cycles (cycles * 256);
	}
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xff) | ((val) & 0xff);
}}}}	m68k_incpc (2);
return 0;
}
/* ROXR.B Dn,Dn */
unsigned long REGPARAM2 CPUFUNC(op_e030_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 cnt = m68k_dreg(regs, srcreg);
{	uae_s8 data = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 val = (uae_u8)data;
	cnt &= 63;
	CLEAR_CZNV;
	if (cnt >= 36) cnt -= 36;
	if (cnt >= 18) cnt -= 18;
	if (cnt >= 9) cnt -= 9;
	if (cnt > 0) {
	cnt--;
	{
	uae_u32 carry;
	uae_u32 hival = (val << 1) | GET_XFLG;
	hival <<= (7 - cnt);
	val >>= cnt;
	carry = val & 1;
	val >>= 1;
	val |= hival;
	SET_XFLG (carry);
	val &= 0xff;
	} }
	SET_CFLG (GET_XFLG);
	SET_ZFLG (((uae_s8)(val)) == 0);
	SET_NFLG (((uae_s8)(val)) < 0);
	{
		int cycles = 2;
		cycles += 2 * cnt;
		do_cycles (cycles * 256);
	}
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xff) | ((val) & 0xff);
}}}}	m68k_incpc (2);
return 0;
}
/* ROR.B Dn,Dn */
unsigned long REGPARAM2 CPUFUNC(op_e038_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 cnt = m68k_dreg(regs, srcreg);
{	uae_s8 data = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 val = (uae_u8)data;
	cnt &= 63;
	CLEAR_CZNV;
	if (cnt > 0) {	uae_u32 hival;
	cnt &= 7;
	hival = val << (8 - cnt);
	val >>= cnt;
	val |= hival;
	val &= 0xff;
	SET_CFLG ((val & 0x80) >> 7);
	}
	SET_ZFLG (((uae_s8)(val)) == 0);
	SET_NFLG (((uae_s8)(val)) < 0);
	{
		int cycles = 2;
		cycles += 2 * cnt;
		do_cycles (cycles * 256);
	}
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xff) | ((val) & 0xff);
}}}}	m68k_incpc (2);
return 0;
}
/* ASR.W #<data>,Dn */
unsigned long REGPARAM2 CPUFUNC(op_e040_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 cnt = srcreg;
{	uae_s16 data = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 val = (uae_u16)data;
	uae_u32 sign = (0x8000 & val) >> 15;
	cnt &= 63;
	CLEAR_CZNV;
	if (cnt >= 16) {
		val = 0xffff & (uae_u32)-sign;
		SET_CFLG (sign);
	COPY_CARRY;
	} else {
		val >>= cnt - 1;
		SET_CFLG (val & 1);
	COPY_CARRY;
		val >>= 1;
		val |= (0xffff << (16 - cnt)) & (uae_u32)-sign;
		val &= 0xffff;
	}
	SET_ZFLG (((uae_s16)(val)) == 0);
	SET_NFLG (((uae_s16)(val)) < 0);
	{
		int cycles = 2;
		cycles += 2 * cnt;
		do_cycles (cycles * 256);
	}
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xffff) | ((val) & 0xffff);
}}}}	m68k_incpc (2);
return 0;
}
/* LSR.W #<data>,Dn */
unsigned long REGPARAM2 CPUFUNC(op_e048_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 cnt = srcreg;
{	uae_s16 data = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 val = (uae_u16)data;
	cnt &= 63;
	CLEAR_CZNV;
	if (cnt >= 16) {
		SET_CFLG ((cnt == 16) & (val >> 15));
	COPY_CARRY;
		val = 0;
	} else {
		val >>= cnt - 1;
		SET_CFLG (val & 1);
	COPY_CARRY;
		val >>= 1;
	}
	SET_ZFLG (((uae_s16)(val)) == 0);
	SET_NFLG (((uae_s16)(val)) < 0);
	{
		int cycles = 2;
		cycles += 2 * cnt;
		do_cycles (cycles * 256);
	}
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xffff) | ((val) & 0xffff);
}}}}	m68k_incpc (2);
return 0;
}
/* ROXR.W #<data>,Dn */
unsigned long REGPARAM2 CPUFUNC(op_e050_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 cnt = srcreg;
{	uae_s16 data = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 val = (uae_u16)data;
	cnt &= 63;
	CLEAR_CZNV;
{	cnt--;
	{
	uae_u32 carry;
	uae_u32 hival = (val << 1) | GET_XFLG;
	hival <<= (15 - cnt);
	val >>= cnt;
	carry = val & 1;
	val >>= 1;
	val |= hival;
	SET_XFLG (carry);
	val &= 0xffff;
	} }
	SET_CFLG (GET_XFLG);
	SET_ZFLG (((uae_s16)(val)) == 0);
	SET_NFLG (((uae_s16)(val)) < 0);
	{
		int cycles = 2;
		cycles += 2 * cnt;
		do_cycles (cycles * 256);
	}
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xffff) | ((val) & 0xffff);
}}}}	m68k_incpc (2);
return 0;
}
/* ROR.W #<data>,Dn */
unsigned long REGPARAM2 CPUFUNC(op_e058_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 cnt = srcreg;
{	uae_s16 data = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 val = (uae_u16)data;
	cnt &= 63;
	CLEAR_CZNV;
{	uae_u32 hival;
	cnt &= 15;
	hival = val << (16 - cnt);
	val >>= cnt;
	val |= hival;
	val &= 0xffff;
	SET_CFLG ((val & 0x8000) >> 15);
	}
	SET_ZFLG (((uae_s16)(val)) == 0);
	SET_NFLG (((uae_s16)(val)) < 0);
	{
		int cycles = 2;
		cycles += 2 * cnt;
		do_cycles (cycles * 256);
	}
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xffff) | ((val) & 0xffff);
}}}}	m68k_incpc (2);
return 0;
}
/* ASR.W Dn,Dn */
unsigned long REGPARAM2 CPUFUNC(op_e060_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 cnt = m68k_dreg(regs, srcreg);
{	uae_s16 data = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 val = (uae_u16)data;
	uae_u32 sign = (0x8000 & val) >> 15;
	cnt &= 63;
	CLEAR_CZNV;
	if (cnt >= 16) {
		val = 0xffff & (uae_u32)-sign;
		SET_CFLG (sign);
	COPY_CARRY;
	} else if (cnt > 0) {
		val >>= cnt - 1;
		SET_CFLG (val & 1);
	COPY_CARRY;
		val >>= 1;
		val |= (0xffff << (16 - cnt)) & (uae_u32)-sign;
		val &= 0xffff;
	}
	SET_ZFLG (((uae_s16)(val)) == 0);
	SET_NFLG (((uae_s16)(val)) < 0);
	{
		int cycles = 2;
		cycles += 2 * cnt;
		do_cycles (cycles * 256);
	}
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xffff) | ((val) & 0xffff);
}}}}	m68k_incpc (2);
return 0;
}
/* LSR.W Dn,Dn */
unsigned long REGPARAM2 CPUFUNC(op_e068_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 cnt = m68k_dreg(regs, srcreg);
{	uae_s16 data = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 val = (uae_u16)data;
	cnt &= 63;
	CLEAR_CZNV;
	if (cnt >= 16) {
		SET_CFLG ((cnt == 16) & (val >> 15));
	COPY_CARRY;
		val = 0;
	} else if (cnt > 0) {
		val >>= cnt - 1;
		SET_CFLG (val & 1);
	COPY_CARRY;
		val >>= 1;
	}
	SET_ZFLG (((uae_s16)(val)) == 0);
	SET_NFLG (((uae_s16)(val)) < 0);
	{
		int cycles = 2;
		cycles += 2 * cnt;
		do_cycles (cycles * 256);
	}
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xffff) | ((val) & 0xffff);
}}}}	m68k_incpc (2);
return 0;
}
/* ROXR.W Dn,Dn */
unsigned long REGPARAM2 CPUFUNC(op_e070_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 cnt = m68k_dreg(regs, srcreg);
{	uae_s16 data = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 val = (uae_u16)data;
	cnt &= 63;
	CLEAR_CZNV;
	if (cnt >= 34) cnt -= 34;
	if (cnt >= 17) cnt -= 17;
	if (cnt > 0) {
	cnt--;
	{
	uae_u32 carry;
	uae_u32 hival = (val << 1) | GET_XFLG;
	hival <<= (15 - cnt);
	val >>= cnt;
	carry = val & 1;
	val >>= 1;
	val |= hival;
	SET_XFLG (carry);
	val &= 0xffff;
	} }
	SET_CFLG (GET_XFLG);
	SET_ZFLG (((uae_s16)(val)) == 0);
	SET_NFLG (((uae_s16)(val)) < 0);
	{
		int cycles = 2;
		cycles += 2 * cnt;
		do_cycles (cycles * 256);
	}
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xffff) | ((val) & 0xffff);
}}}}	m68k_incpc (2);
return 0;
}
/* ROR.W Dn,Dn */
unsigned long REGPARAM2 CPUFUNC(op_e078_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 cnt = m68k_dreg(regs, srcreg);
{	uae_s16 data = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 val = (uae_u16)data;
	cnt &= 63;
	CLEAR_CZNV;
	if (cnt > 0) {	uae_u32 hival;
	cnt &= 15;
	hival = val << (16 - cnt);
	val >>= cnt;
	val |= hival;
	val &= 0xffff;
	SET_CFLG ((val & 0x8000) >> 15);
	}
	SET_ZFLG (((uae_s16)(val)) == 0);
	SET_NFLG (((uae_s16)(val)) < 0);
	{
		int cycles = 2;
		cycles += 2 * cnt;
		do_cycles (cycles * 256);
	}
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xffff) | ((val) & 0xffff);
}}}}	m68k_incpc (2);
return 0;
}
/* ASR.L #<data>,Dn */
unsigned long REGPARAM2 CPUFUNC(op_e080_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 cnt = srcreg;
{	uae_s32 data = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 val = data;
	uae_u32 sign = (0x80000000 & val) >> 31;
	cnt &= 63;
	CLEAR_CZNV;
	if (cnt >= 32) {
		val = 0xffffffff & (uae_u32)-sign;
		SET_CFLG (sign);
	COPY_CARRY;
	} else {
		val >>= cnt - 1;
		SET_CFLG (val & 1);
	COPY_CARRY;
		val >>= 1;
		val |= (0xffffffff << (32 - cnt)) & (uae_u32)-sign;
		val &= 0xffffffff;
	}
	SET_ZFLG (((uae_s32)(val)) == 0);
	SET_NFLG (((uae_s32)(val)) < 0);
	{
		int cycles = 4;
		cycles += 2 * cnt;
		do_cycles (cycles * 256);
	}
	m68k_dreg(regs, dstreg) = (val);
}}}}	m68k_incpc (2);
return 0;
}
/* LSR.L #<data>,Dn */
unsigned long REGPARAM2 CPUFUNC(op_e088_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 cnt = srcreg;
{	uae_s32 data = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 val = data;
	cnt &= 63;
	CLEAR_CZNV;
	if (cnt >= 32) {
		SET_CFLG ((cnt == 32) & (val >> 31));
	COPY_CARRY;
		val = 0;
	} else {
		val >>= cnt - 1;
		SET_CFLG (val & 1);
	COPY_CARRY;
		val >>= 1;
	}
	SET_ZFLG (((uae_s32)(val)) == 0);
	SET_NFLG (((uae_s32)(val)) < 0);
	{
		int cycles = 4;
		cycles += 2 * cnt;
		do_cycles (cycles * 256);
	}
	m68k_dreg(regs, dstreg) = (val);
}}}}	m68k_incpc (2);
return 0;
}
/* ROXR.L #<data>,Dn */
unsigned long REGPARAM2 CPUFUNC(op_e090_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 cnt = srcreg;
{	uae_s32 data = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 val = data;
	cnt &= 63;
	CLEAR_CZNV;
{	cnt--;
	{
	uae_u32 carry;
	uae_u32 hival = (val << 1) | GET_XFLG;
	hival <<= (31 - cnt);
	val >>= cnt;
	carry = val & 1;
	val >>= 1;
	val |= hival;
	SET_XFLG (carry);
	val &= 0xffffffff;
	} }
	SET_CFLG (GET_XFLG);
	SET_ZFLG (((uae_s32)(val)) == 0);
	SET_NFLG (((uae_s32)(val)) < 0);
	{
		int cycles = 4;
		cycles += 2 * cnt;
		do_cycles (cycles * 256);
	}
	m68k_dreg(regs, dstreg) = (val);
}}}}	m68k_incpc (2);
return 0;
}
/* ROR.L #<data>,Dn */
unsigned long REGPARAM2 CPUFUNC(op_e098_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 cnt = srcreg;
{	uae_s32 data = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 val = data;
	cnt &= 63;
	CLEAR_CZNV;
{	uae_u32 hival;
	cnt &= 31;
	hival = val << (32 - cnt);
	val >>= cnt;
	val |= hival;
	val &= 0xffffffff;
	SET_CFLG ((val & 0x80000000) >> 31);
	}
	SET_ZFLG (((uae_s32)(val)) == 0);
	SET_NFLG (((uae_s32)(val)) < 0);
	{
		int cycles = 4;
		cycles += 2 * cnt;
		do_cycles (cycles * 256);
	}
	m68k_dreg(regs, dstreg) = (val);
}}}}	m68k_incpc (2);
return 0;
}
/* ASR.L Dn,Dn */
unsigned long REGPARAM2 CPUFUNC(op_e0a0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 cnt = m68k_dreg(regs, srcreg);
{	uae_s32 data = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 val = data;
	uae_u32 sign = (0x80000000 & val) >> 31;
	cnt &= 63;
	CLEAR_CZNV;
	if (cnt >= 32) {
		val = 0xffffffff & (uae_u32)-sign;
		SET_CFLG (sign);
	COPY_CARRY;
	} else if (cnt > 0) {
		val >>= cnt - 1;
		SET_CFLG (val & 1);
	COPY_CARRY;
		val >>= 1;
		val |= (0xffffffff << (32 - cnt)) & (uae_u32)-sign;
		val &= 0xffffffff;
	}
	SET_ZFLG (((uae_s32)(val)) == 0);
	SET_NFLG (((uae_s32)(val)) < 0);
	{
		int cycles = 4;
		cycles += 2 * cnt;
		do_cycles (cycles * 256);
	}
	m68k_dreg(regs, dstreg) = (val);
}}}}	m68k_incpc (2);
return 0;
}
/* LSR.L Dn,Dn */
unsigned long REGPARAM2 CPUFUNC(op_e0a8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 cnt = m68k_dreg(regs, srcreg);
{	uae_s32 data = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 val = data;
	cnt &= 63;
	CLEAR_CZNV;
	if (cnt >= 32) {
		SET_CFLG ((cnt == 32) & (val >> 31));
	COPY_CARRY;
		val = 0;
	} else if (cnt > 0) {
		val >>= cnt - 1;
		SET_CFLG (val & 1);
	COPY_CARRY;
		val >>= 1;
	}
	SET_ZFLG (((uae_s32)(val)) == 0);
	SET_NFLG (((uae_s32)(val)) < 0);
	{
		int cycles = 4;
		cycles += 2 * cnt;
		do_cycles (cycles * 256);
	}
	m68k_dreg(regs, dstreg) = (val);
}}}}	m68k_incpc (2);
return 0;
}
/* ROXR.L Dn,Dn */
unsigned long REGPARAM2 CPUFUNC(op_e0b0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 cnt = m68k_dreg(regs, srcreg);
{	uae_s32 data = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 val = data;
	cnt &= 63;
	CLEAR_CZNV;
	if (cnt >= 33) cnt -= 33;
	if (cnt > 0) {
	cnt--;
	{
	uae_u32 carry;
	uae_u32 hival = (val << 1) | GET_XFLG;
	hival <<= (31 - cnt);
	val >>= cnt;
	carry = val & 1;
	val >>= 1;
	val |= hival;
	SET_XFLG (carry);
	val &= 0xffffffff;
	} }
	SET_CFLG (GET_XFLG);
	SET_ZFLG (((uae_s32)(val)) == 0);
	SET_NFLG (((uae_s32)(val)) < 0);
	{
		int cycles = 4;
		cycles += 2 * cnt;
		do_cycles (cycles * 256);
	}
	m68k_dreg(regs, dstreg) = (val);
}}}}	m68k_incpc (2);
return 0;
}
/* ROR.L Dn,Dn */
unsigned long REGPARAM2 CPUFUNC(op_e0b8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 cnt = m68k_dreg(regs, srcreg);
{	uae_s32 data = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 val = data;
	cnt &= 63;
	CLEAR_CZNV;
	if (cnt > 0) {	uae_u32 hival;
	cnt &= 31;
	hival = val << (32 - cnt);
	val >>= cnt;
	val |= hival;
	val &= 0xffffffff;
	SET_CFLG ((val & 0x80000000) >> 31);
	}
	SET_ZFLG (((uae_s32)(val)) == 0);
	SET_NFLG (((uae_s32)(val)) < 0);
	{
		int cycles = 4;
		cycles += 2 * cnt;
		do_cycles (cycles * 256);
	}
	m68k_dreg(regs, dstreg) = (val);
}}}}	m68k_incpc (2);
return 0;
}
/* ASRW.W (An) */
unsigned long REGPARAM2 CPUFUNC(op_e0d0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg(regs, srcreg);
	if (dataa & 1) {
		exception3 (opcode, m68k_getpc() + 2, dataa);
		goto endlabel5236;
	}
{{	uae_s16 data = get_word_ce (dataa);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 val = (uae_u16)data;
	uae_u32 sign = 0x8000 & val;
	uae_u32 cflg = val & 1;
	val = (val >> 1) | sign;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(val)) == 0);
	SET_NFLG (((uae_s16)(val)) < 0);
	SET_CFLG (cflg);
	COPY_CARRY;
	put_word_ce (dataa,val);
}}}}}	m68k_incpc (2);
endlabel5236: ;
return 0;
}
/* ASRW.W (An)+ */
unsigned long REGPARAM2 CPUFUNC(op_e0d8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg(regs, srcreg);
	if (dataa & 1) {
		exception3 (opcode, m68k_getpc() + 2, dataa);
		goto endlabel5237;
	}
{{	uae_s16 data = get_word_ce (dataa);
	m68k_areg(regs, srcreg) += 2;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 val = (uae_u16)data;
	uae_u32 sign = 0x8000 & val;
	uae_u32 cflg = val & 1;
	val = (val >> 1) | sign;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(val)) == 0);
	SET_NFLG (((uae_s16)(val)) < 0);
	SET_CFLG (cflg);
	COPY_CARRY;
	put_word_ce (dataa,val);
}}}}}	m68k_incpc (2);
endlabel5237: ;
return 0;
}
/* ASRW.W -(An) */
unsigned long REGPARAM2 CPUFUNC(op_e0e0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa;
	do_cycles (2 * 256);
	dataa = m68k_areg(regs, srcreg) - 2;
	if (dataa & 1) {
		exception3 (opcode, m68k_getpc() + 2, dataa);
		goto endlabel5238;
	}
{{	uae_s16 data = get_word_ce (dataa);
	m68k_areg (regs, srcreg) = dataa;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 val = (uae_u16)data;
	uae_u32 sign = 0x8000 & val;
	uae_u32 cflg = val & 1;
	val = (val >> 1) | sign;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(val)) == 0);
	SET_NFLG (((uae_s16)(val)) < 0);
	SET_CFLG (cflg);
	COPY_CARRY;
	put_word_ce (dataa,val);
}}}}}	m68k_incpc (2);
endlabel5238: ;
return 0;
}
/* ASRW.W (d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_e0e8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dataa & 1) {
		exception3 (opcode, m68k_getpc() + 2, dataa);
		goto endlabel5239;
	}
{{	uae_s16 data = get_word_ce (dataa);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	uae_u32 val = (uae_u16)data;
	uae_u32 sign = 0x8000 & val;
	uae_u32 cflg = val & 1;
	val = (val >> 1) | sign;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(val)) == 0);
	SET_NFLG (((uae_s16)(val)) < 0);
	SET_CFLG (cflg);
	COPY_CARRY;
	put_word_ce (dataa,val);
}}}}}	m68k_incpc (4);
endlabel5239: ;
return 0;
}
/* ASRW.W (d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_e0f0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa;
	do_cycles (2 * 256);
	dataa = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	if (dataa & 1) {
		exception3 (opcode, m68k_getpc() + 2, dataa);
		goto endlabel5240;
	}
{{	uae_s16 data = get_word_ce (dataa);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	uae_u32 val = (uae_u16)data;
	uae_u32 sign = 0x8000 & val;
	uae_u32 cflg = val & 1;
	val = (val >> 1) | sign;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(val)) == 0);
	SET_NFLG (((uae_s16)(val)) < 0);
	SET_CFLG (cflg);
	COPY_CARRY;
	put_word_ce (dataa,val);
}}}}}	m68k_incpc (4);
endlabel5240: ;
return 0;
}
/* ASRW.W (xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_e0f8_6)(uae_u32 opcode)
{
{{	uaecptr dataa = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dataa & 1) {
		exception3 (opcode, m68k_getpc() + 2, dataa);
		goto endlabel5241;
	}
{{	uae_s16 data = get_word_ce (dataa);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	uae_u32 val = (uae_u16)data;
	uae_u32 sign = 0x8000 & val;
	uae_u32 cflg = val & 1;
	val = (val >> 1) | sign;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(val)) == 0);
	SET_NFLG (((uae_s16)(val)) < 0);
	SET_CFLG (cflg);
	COPY_CARRY;
	put_word_ce (dataa,val);
}}}}}	m68k_incpc (4);
endlabel5241: ;
return 0;
}
/* ASRW.W (xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_e0f9_6)(uae_u32 opcode)
{
{{	uaecptr dataa;
	dataa = get_word_ce_prefetch (4) << 16;
	dataa |= get_word_ce_prefetch (6);
	if (dataa & 1) {
		exception3 (opcode, m68k_getpc() + 2, dataa);
		goto endlabel5242;
	}
{{	uae_s16 data = get_word_ce (dataa);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
{	uae_u32 val = (uae_u16)data;
	uae_u32 sign = 0x8000 & val;
	uae_u32 cflg = val & 1;
	val = (val >> 1) | sign;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(val)) == 0);
	SET_NFLG (((uae_s16)(val)) < 0);
	SET_CFLG (cflg);
	COPY_CARRY;
	put_word_ce (dataa,val);
}}}}}	m68k_incpc (6);
endlabel5242: ;
return 0;
}
/* ASL.B #<data>,Dn */
unsigned long REGPARAM2 CPUFUNC(op_e100_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 cnt = srcreg;
{	uae_s8 data = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 val = (uae_u8)data;
	cnt &= 63;
	CLEAR_CZNV;
	if (cnt >= 8) {
		SET_VFLG (val != 0);
		SET_CFLG (cnt == 8 ? val & 1 : 0);
	COPY_CARRY;
		val = 0;
	} else {
		uae_u32 mask = (0xff << (7 - cnt)) & 0xff;
		SET_VFLG ((val & mask) != mask && (val & mask) != 0);
		val <<= cnt - 1;
		SET_CFLG ((val & 0x80) >> 7);
	COPY_CARRY;
		val <<= 1;
		val &= 0xff;
	}
	SET_ZFLG (((uae_s8)(val)) == 0);
	SET_NFLG (((uae_s8)(val)) < 0);
	{
		int cycles = 2;
		cycles += 2 * cnt;
		do_cycles (cycles * 256);
	}
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xff) | ((val) & 0xff);
}}}}	m68k_incpc (2);
return 0;
}
/* LSL.B #<data>,Dn */
unsigned long REGPARAM2 CPUFUNC(op_e108_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 cnt = srcreg;
{	uae_s8 data = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 val = (uae_u8)data;
	cnt &= 63;
	CLEAR_CZNV;
	if (cnt >= 8) {
		SET_CFLG (cnt == 8 ? val & 1 : 0);
	COPY_CARRY;
		val = 0;
	} else {
		val <<= (cnt - 1);
		SET_CFLG ((val & 0x80) >> 7);
	COPY_CARRY;
		val <<= 1;
	val &= 0xff;
	}
	SET_ZFLG (((uae_s8)(val)) == 0);
	SET_NFLG (((uae_s8)(val)) < 0);
	{
		int cycles = 2;
		cycles += 2 * cnt;
		do_cycles (cycles * 256);
	}
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xff) | ((val) & 0xff);
}}}}	m68k_incpc (2);
return 0;
}
/* ROXL.B #<data>,Dn */
unsigned long REGPARAM2 CPUFUNC(op_e110_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 cnt = srcreg;
{	uae_s8 data = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 val = (uae_u8)data;
	cnt &= 63;
	CLEAR_CZNV;
{	cnt--;
	{
	uae_u32 carry;
	uae_u32 loval = val >> (7 - cnt);
	carry = loval & 1;
	val = (((val << 1) | GET_XFLG) << cnt) | (loval >> 1);
	SET_XFLG (carry);
	val &= 0xff;
	} }
	SET_CFLG (GET_XFLG);
	SET_ZFLG (((uae_s8)(val)) == 0);
	SET_NFLG (((uae_s8)(val)) < 0);
	{
		int cycles = 2;
		cycles += 2 * cnt;
		do_cycles (cycles * 256);
	}
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xff) | ((val) & 0xff);
}}}}	m68k_incpc (2);
return 0;
}
/* ROL.B #<data>,Dn */
unsigned long REGPARAM2 CPUFUNC(op_e118_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 cnt = srcreg;
{	uae_s8 data = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 val = (uae_u8)data;
	cnt &= 63;
	CLEAR_CZNV;
{	uae_u32 loval;
	cnt &= 7;
	loval = val >> (8 - cnt);
	val <<= cnt;
	val |= loval;
	val &= 0xff;
	SET_CFLG (val & 1);
}
	SET_ZFLG (((uae_s8)(val)) == 0);
	SET_NFLG (((uae_s8)(val)) < 0);
	{
		int cycles = 2;
		cycles += 2 * cnt;
		do_cycles (cycles * 256);
	}
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xff) | ((val) & 0xff);
}}}}	m68k_incpc (2);
return 0;
}
/* ASL.B Dn,Dn */
unsigned long REGPARAM2 CPUFUNC(op_e120_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 cnt = m68k_dreg(regs, srcreg);
{	uae_s8 data = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 val = (uae_u8)data;
	cnt &= 63;
	CLEAR_CZNV;
	if (cnt >= 8) {
		SET_VFLG (val != 0);
		SET_CFLG (cnt == 8 ? val & 1 : 0);
	COPY_CARRY;
		val = 0;
	} else if (cnt > 0) {
		uae_u32 mask = (0xff << (7 - cnt)) & 0xff;
		SET_VFLG ((val & mask) != mask && (val & mask) != 0);
		val <<= cnt - 1;
		SET_CFLG ((val & 0x80) >> 7);
	COPY_CARRY;
		val <<= 1;
		val &= 0xff;
	}
	SET_ZFLG (((uae_s8)(val)) == 0);
	SET_NFLG (((uae_s8)(val)) < 0);
	{
		int cycles = 2;
		cycles += 2 * cnt;
		do_cycles (cycles * 256);
	}
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xff) | ((val) & 0xff);
}}}}	m68k_incpc (2);
return 0;
}
/* LSL.B Dn,Dn */
unsigned long REGPARAM2 CPUFUNC(op_e128_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 cnt = m68k_dreg(regs, srcreg);
{	uae_s8 data = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 val = (uae_u8)data;
	cnt &= 63;
	CLEAR_CZNV;
	if (cnt >= 8) {
		SET_CFLG (cnt == 8 ? val & 1 : 0);
	COPY_CARRY;
		val = 0;
	} else if (cnt > 0) {
		val <<= (cnt - 1);
		SET_CFLG ((val & 0x80) >> 7);
	COPY_CARRY;
		val <<= 1;
	val &= 0xff;
	}
	SET_ZFLG (((uae_s8)(val)) == 0);
	SET_NFLG (((uae_s8)(val)) < 0);
	{
		int cycles = 2;
		cycles += 2 * cnt;
		do_cycles (cycles * 256);
	}
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xff) | ((val) & 0xff);
}}}}	m68k_incpc (2);
return 0;
}
/* ROXL.B Dn,Dn */
unsigned long REGPARAM2 CPUFUNC(op_e130_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 cnt = m68k_dreg(regs, srcreg);
{	uae_s8 data = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 val = (uae_u8)data;
	cnt &= 63;
	CLEAR_CZNV;
	if (cnt >= 36) cnt -= 36;
	if (cnt >= 18) cnt -= 18;
	if (cnt >= 9) cnt -= 9;
	if (cnt > 0) {
	cnt--;
	{
	uae_u32 carry;
	uae_u32 loval = val >> (7 - cnt);
	carry = loval & 1;
	val = (((val << 1) | GET_XFLG) << cnt) | (loval >> 1);
	SET_XFLG (carry);
	val &= 0xff;
	} }
	SET_CFLG (GET_XFLG);
	SET_ZFLG (((uae_s8)(val)) == 0);
	SET_NFLG (((uae_s8)(val)) < 0);
	{
		int cycles = 2;
		cycles += 2 * cnt;
		do_cycles (cycles * 256);
	}
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xff) | ((val) & 0xff);
}}}}	m68k_incpc (2);
return 0;
}
/* ROL.B Dn,Dn */
unsigned long REGPARAM2 CPUFUNC(op_e138_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 cnt = m68k_dreg(regs, srcreg);
{	uae_s8 data = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 val = (uae_u8)data;
	cnt &= 63;
	CLEAR_CZNV;
	if (cnt > 0) {
	uae_u32 loval;
	cnt &= 7;
	loval = val >> (8 - cnt);
	val <<= cnt;
	val |= loval;
	val &= 0xff;
	SET_CFLG (val & 1);
}
	SET_ZFLG (((uae_s8)(val)) == 0);
	SET_NFLG (((uae_s8)(val)) < 0);
	{
		int cycles = 2;
		cycles += 2 * cnt;
		do_cycles (cycles * 256);
	}
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xff) | ((val) & 0xff);
}}}}	m68k_incpc (2);
return 0;
}
/* ASL.W #<data>,Dn */
unsigned long REGPARAM2 CPUFUNC(op_e140_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 cnt = srcreg;
{	uae_s16 data = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 val = (uae_u16)data;
	cnt &= 63;
	CLEAR_CZNV;
	if (cnt >= 16) {
		SET_VFLG (val != 0);
		SET_CFLG (cnt == 16 ? val & 1 : 0);
	COPY_CARRY;
		val = 0;
	} else {
		uae_u32 mask = (0xffff << (15 - cnt)) & 0xffff;
		SET_VFLG ((val & mask) != mask && (val & mask) != 0);
		val <<= cnt - 1;
		SET_CFLG ((val & 0x8000) >> 15);
	COPY_CARRY;
		val <<= 1;
		val &= 0xffff;
	}
	SET_ZFLG (((uae_s16)(val)) == 0);
	SET_NFLG (((uae_s16)(val)) < 0);
	{
		int cycles = 2;
		cycles += 2 * cnt;
		do_cycles (cycles * 256);
	}
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xffff) | ((val) & 0xffff);
}}}}	m68k_incpc (2);
return 0;
}
/* LSL.W #<data>,Dn */
unsigned long REGPARAM2 CPUFUNC(op_e148_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 cnt = srcreg;
{	uae_s16 data = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 val = (uae_u16)data;
	cnt &= 63;
	CLEAR_CZNV;
	if (cnt >= 16) {
		SET_CFLG (cnt == 16 ? val & 1 : 0);
	COPY_CARRY;
		val = 0;
	} else {
		val <<= (cnt - 1);
		SET_CFLG ((val & 0x8000) >> 15);
	COPY_CARRY;
		val <<= 1;
	val &= 0xffff;
	}
	SET_ZFLG (((uae_s16)(val)) == 0);
	SET_NFLG (((uae_s16)(val)) < 0);
	{
		int cycles = 2;
		cycles += 2 * cnt;
		do_cycles (cycles * 256);
	}
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xffff) | ((val) & 0xffff);
}}}}	m68k_incpc (2);
return 0;
}
/* ROXL.W #<data>,Dn */
unsigned long REGPARAM2 CPUFUNC(op_e150_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 cnt = srcreg;
{	uae_s16 data = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 val = (uae_u16)data;
	cnt &= 63;
	CLEAR_CZNV;
{	cnt--;
	{
	uae_u32 carry;
	uae_u32 loval = val >> (15 - cnt);
	carry = loval & 1;
	val = (((val << 1) | GET_XFLG) << cnt) | (loval >> 1);
	SET_XFLG (carry);
	val &= 0xffff;
	} }
	SET_CFLG (GET_XFLG);
	SET_ZFLG (((uae_s16)(val)) == 0);
	SET_NFLG (((uae_s16)(val)) < 0);
	{
		int cycles = 2;
		cycles += 2 * cnt;
		do_cycles (cycles * 256);
	}
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xffff) | ((val) & 0xffff);
}}}}	m68k_incpc (2);
return 0;
}
/* ROL.W #<data>,Dn */
unsigned long REGPARAM2 CPUFUNC(op_e158_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 cnt = srcreg;
{	uae_s16 data = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 val = (uae_u16)data;
	cnt &= 63;
	CLEAR_CZNV;
{	uae_u32 loval;
	cnt &= 15;
	loval = val >> (16 - cnt);
	val <<= cnt;
	val |= loval;
	val &= 0xffff;
	SET_CFLG (val & 1);
}
	SET_ZFLG (((uae_s16)(val)) == 0);
	SET_NFLG (((uae_s16)(val)) < 0);
	{
		int cycles = 2;
		cycles += 2 * cnt;
		do_cycles (cycles * 256);
	}
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xffff) | ((val) & 0xffff);
}}}}	m68k_incpc (2);
return 0;
}
/* ASL.W Dn,Dn */
unsigned long REGPARAM2 CPUFUNC(op_e160_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 cnt = m68k_dreg(regs, srcreg);
{	uae_s16 data = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 val = (uae_u16)data;
	cnt &= 63;
	CLEAR_CZNV;
	if (cnt >= 16) {
		SET_VFLG (val != 0);
		SET_CFLG (cnt == 16 ? val & 1 : 0);
	COPY_CARRY;
		val = 0;
	} else if (cnt > 0) {
		uae_u32 mask = (0xffff << (15 - cnt)) & 0xffff;
		SET_VFLG ((val & mask) != mask && (val & mask) != 0);
		val <<= cnt - 1;
		SET_CFLG ((val & 0x8000) >> 15);
	COPY_CARRY;
		val <<= 1;
		val &= 0xffff;
	}
	SET_ZFLG (((uae_s16)(val)) == 0);
	SET_NFLG (((uae_s16)(val)) < 0);
	{
		int cycles = 2;
		cycles += 2 * cnt;
		do_cycles (cycles * 256);
	}
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xffff) | ((val) & 0xffff);
}}}}	m68k_incpc (2);
return 0;
}
/* LSL.W Dn,Dn */
unsigned long REGPARAM2 CPUFUNC(op_e168_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 cnt = m68k_dreg(regs, srcreg);
{	uae_s16 data = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 val = (uae_u16)data;
	cnt &= 63;
	CLEAR_CZNV;
	if (cnt >= 16) {
		SET_CFLG (cnt == 16 ? val & 1 : 0);
	COPY_CARRY;
		val = 0;
	} else if (cnt > 0) {
		val <<= (cnt - 1);
		SET_CFLG ((val & 0x8000) >> 15);
	COPY_CARRY;
		val <<= 1;
	val &= 0xffff;
	}
	SET_ZFLG (((uae_s16)(val)) == 0);
	SET_NFLG (((uae_s16)(val)) < 0);
	{
		int cycles = 2;
		cycles += 2 * cnt;
		do_cycles (cycles * 256);
	}
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xffff) | ((val) & 0xffff);
}}}}	m68k_incpc (2);
return 0;
}
/* ROXL.W Dn,Dn */
unsigned long REGPARAM2 CPUFUNC(op_e170_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 cnt = m68k_dreg(regs, srcreg);
{	uae_s16 data = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 val = (uae_u16)data;
	cnt &= 63;
	CLEAR_CZNV;
	if (cnt >= 34) cnt -= 34;
	if (cnt >= 17) cnt -= 17;
	if (cnt > 0) {
	cnt--;
	{
	uae_u32 carry;
	uae_u32 loval = val >> (15 - cnt);
	carry = loval & 1;
	val = (((val << 1) | GET_XFLG) << cnt) | (loval >> 1);
	SET_XFLG (carry);
	val &= 0xffff;
	} }
	SET_CFLG (GET_XFLG);
	SET_ZFLG (((uae_s16)(val)) == 0);
	SET_NFLG (((uae_s16)(val)) < 0);
	{
		int cycles = 2;
		cycles += 2 * cnt;
		do_cycles (cycles * 256);
	}
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xffff) | ((val) & 0xffff);
}}}}	m68k_incpc (2);
return 0;
}
/* ROL.W Dn,Dn */
unsigned long REGPARAM2 CPUFUNC(op_e178_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 cnt = m68k_dreg(regs, srcreg);
{	uae_s16 data = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 val = (uae_u16)data;
	cnt &= 63;
	CLEAR_CZNV;
	if (cnt > 0) {
	uae_u32 loval;
	cnt &= 15;
	loval = val >> (16 - cnt);
	val <<= cnt;
	val |= loval;
	val &= 0xffff;
	SET_CFLG (val & 1);
}
	SET_ZFLG (((uae_s16)(val)) == 0);
	SET_NFLG (((uae_s16)(val)) < 0);
	{
		int cycles = 2;
		cycles += 2 * cnt;
		do_cycles (cycles * 256);
	}
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xffff) | ((val) & 0xffff);
}}}}	m68k_incpc (2);
return 0;
}
/* ASL.L #<data>,Dn */
unsigned long REGPARAM2 CPUFUNC(op_e180_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 cnt = srcreg;
{	uae_s32 data = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 val = data;
	cnt &= 63;
	CLEAR_CZNV;
	if (cnt >= 32) {
		SET_VFLG (val != 0);
		SET_CFLG (cnt == 32 ? val & 1 : 0);
	COPY_CARRY;
		val = 0;
	} else {
		uae_u32 mask = (0xffffffff << (31 - cnt)) & 0xffffffff;
		SET_VFLG ((val & mask) != mask && (val & mask) != 0);
		val <<= cnt - 1;
		SET_CFLG ((val & 0x80000000) >> 31);
	COPY_CARRY;
		val <<= 1;
		val &= 0xffffffff;
	}
	SET_ZFLG (((uae_s32)(val)) == 0);
	SET_NFLG (((uae_s32)(val)) < 0);
	{
		int cycles = 4;
		cycles += 2 * cnt;
		do_cycles (cycles * 256);
	}
	m68k_dreg(regs, dstreg) = (val);
}}}}	m68k_incpc (2);
return 0;
}
/* LSL.L #<data>,Dn */
unsigned long REGPARAM2 CPUFUNC(op_e188_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 cnt = srcreg;
{	uae_s32 data = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 val = data;
	cnt &= 63;
	CLEAR_CZNV;
	if (cnt >= 32) {
		SET_CFLG (cnt == 32 ? val & 1 : 0);
	COPY_CARRY;
		val = 0;
	} else {
		val <<= (cnt - 1);
		SET_CFLG ((val & 0x80000000) >> 31);
	COPY_CARRY;
		val <<= 1;
	val &= 0xffffffff;
	}
	SET_ZFLG (((uae_s32)(val)) == 0);
	SET_NFLG (((uae_s32)(val)) < 0);
	{
		int cycles = 4;
		cycles += 2 * cnt;
		do_cycles (cycles * 256);
	}
	m68k_dreg(regs, dstreg) = (val);
}}}}	m68k_incpc (2);
return 0;
}
/* ROXL.L #<data>,Dn */
unsigned long REGPARAM2 CPUFUNC(op_e190_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 cnt = srcreg;
{	uae_s32 data = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 val = data;
	cnt &= 63;
	CLEAR_CZNV;
{	cnt--;
	{
	uae_u32 carry;
	uae_u32 loval = val >> (31 - cnt);
	carry = loval & 1;
	val = (((val << 1) | GET_XFLG) << cnt) | (loval >> 1);
	SET_XFLG (carry);
	val &= 0xffffffff;
	} }
	SET_CFLG (GET_XFLG);
	SET_ZFLG (((uae_s32)(val)) == 0);
	SET_NFLG (((uae_s32)(val)) < 0);
	{
		int cycles = 4;
		cycles += 2 * cnt;
		do_cycles (cycles * 256);
	}
	m68k_dreg(regs, dstreg) = (val);
}}}}	m68k_incpc (2);
return 0;
}
/* ROL.L #<data>,Dn */
unsigned long REGPARAM2 CPUFUNC(op_e198_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 cnt = srcreg;
{	uae_s32 data = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 val = data;
	cnt &= 63;
	CLEAR_CZNV;
{	uae_u32 loval;
	cnt &= 31;
	loval = val >> (32 - cnt);
	val <<= cnt;
	val |= loval;
	val &= 0xffffffff;
	SET_CFLG (val & 1);
}
	SET_ZFLG (((uae_s32)(val)) == 0);
	SET_NFLG (((uae_s32)(val)) < 0);
	{
		int cycles = 4;
		cycles += 2 * cnt;
		do_cycles (cycles * 256);
	}
	m68k_dreg(regs, dstreg) = (val);
}}}}	m68k_incpc (2);
return 0;
}
/* ASL.L Dn,Dn */
unsigned long REGPARAM2 CPUFUNC(op_e1a0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 cnt = m68k_dreg(regs, srcreg);
{	uae_s32 data = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 val = data;
	cnt &= 63;
	CLEAR_CZNV;
	if (cnt >= 32) {
		SET_VFLG (val != 0);
		SET_CFLG (cnt == 32 ? val & 1 : 0);
	COPY_CARRY;
		val = 0;
	} else if (cnt > 0) {
		uae_u32 mask = (0xffffffff << (31 - cnt)) & 0xffffffff;
		SET_VFLG ((val & mask) != mask && (val & mask) != 0);
		val <<= cnt - 1;
		SET_CFLG ((val & 0x80000000) >> 31);
	COPY_CARRY;
		val <<= 1;
		val &= 0xffffffff;
	}
	SET_ZFLG (((uae_s32)(val)) == 0);
	SET_NFLG (((uae_s32)(val)) < 0);
	{
		int cycles = 4;
		cycles += 2 * cnt;
		do_cycles (cycles * 256);
	}
	m68k_dreg(regs, dstreg) = (val);
}}}}	m68k_incpc (2);
return 0;
}
/* LSL.L Dn,Dn */
unsigned long REGPARAM2 CPUFUNC(op_e1a8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 cnt = m68k_dreg(regs, srcreg);
{	uae_s32 data = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 val = data;
	cnt &= 63;
	CLEAR_CZNV;
	if (cnt >= 32) {
		SET_CFLG (cnt == 32 ? val & 1 : 0);
	COPY_CARRY;
		val = 0;
	} else if (cnt > 0) {
		val <<= (cnt - 1);
		SET_CFLG ((val & 0x80000000) >> 31);
	COPY_CARRY;
		val <<= 1;
	val &= 0xffffffff;
	}
	SET_ZFLG (((uae_s32)(val)) == 0);
	SET_NFLG (((uae_s32)(val)) < 0);
	{
		int cycles = 4;
		cycles += 2 * cnt;
		do_cycles (cycles * 256);
	}
	m68k_dreg(regs, dstreg) = (val);
}}}}	m68k_incpc (2);
return 0;
}
/* ROXL.L Dn,Dn */
unsigned long REGPARAM2 CPUFUNC(op_e1b0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 cnt = m68k_dreg(regs, srcreg);
{	uae_s32 data = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 val = data;
	cnt &= 63;
	CLEAR_CZNV;
	if (cnt >= 33) cnt -= 33;
	if (cnt > 0) {
	cnt--;
	{
	uae_u32 carry;
	uae_u32 loval = val >> (31 - cnt);
	carry = loval & 1;
	val = (((val << 1) | GET_XFLG) << cnt) | (loval >> 1);
	SET_XFLG (carry);
	val &= 0xffffffff;
	} }
	SET_CFLG (GET_XFLG);
	SET_ZFLG (((uae_s32)(val)) == 0);
	SET_NFLG (((uae_s32)(val)) < 0);
	{
		int cycles = 4;
		cycles += 2 * cnt;
		do_cycles (cycles * 256);
	}
	m68k_dreg(regs, dstreg) = (val);
}}}}	m68k_incpc (2);
return 0;
}
/* ROL.L Dn,Dn */
unsigned long REGPARAM2 CPUFUNC(op_e1b8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 cnt = m68k_dreg(regs, srcreg);
{	uae_s32 data = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 val = data;
	cnt &= 63;
	CLEAR_CZNV;
	if (cnt > 0) {
	uae_u32 loval;
	cnt &= 31;
	loval = val >> (32 - cnt);
	val <<= cnt;
	val |= loval;
	val &= 0xffffffff;
	SET_CFLG (val & 1);
}
	SET_ZFLG (((uae_s32)(val)) == 0);
	SET_NFLG (((uae_s32)(val)) < 0);
	{
		int cycles = 4;
		cycles += 2 * cnt;
		do_cycles (cycles * 256);
	}
	m68k_dreg(regs, dstreg) = (val);
}}}}	m68k_incpc (2);
return 0;
}
/* ASLW.W (An) */
unsigned long REGPARAM2 CPUFUNC(op_e1d0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg(regs, srcreg);
	if (dataa & 1) {
		exception3 (opcode, m68k_getpc() + 2, dataa);
		goto endlabel5267;
	}
{{	uae_s16 data = get_word_ce (dataa);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 val = (uae_u16)data;
	uae_u32 sign = 0x8000 & val;
	uae_u32 sign2;
	val <<= 1;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(val)) == 0);
	SET_NFLG (((uae_s16)(val)) < 0);
	sign2 = 0x8000 & val;
	SET_CFLG (sign != 0);
	COPY_CARRY;
	SET_VFLG (GET_VFLG | (sign2 != sign));
	put_word_ce (dataa,val);
}}}}}	m68k_incpc (2);
endlabel5267: ;
return 0;
}
/* ASLW.W (An)+ */
unsigned long REGPARAM2 CPUFUNC(op_e1d8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg(regs, srcreg);
	if (dataa & 1) {
		exception3 (opcode, m68k_getpc() + 2, dataa);
		goto endlabel5268;
	}
{{	uae_s16 data = get_word_ce (dataa);
	m68k_areg(regs, srcreg) += 2;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 val = (uae_u16)data;
	uae_u32 sign = 0x8000 & val;
	uae_u32 sign2;
	val <<= 1;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(val)) == 0);
	SET_NFLG (((uae_s16)(val)) < 0);
	sign2 = 0x8000 & val;
	SET_CFLG (sign != 0);
	COPY_CARRY;
	SET_VFLG (GET_VFLG | (sign2 != sign));
	put_word_ce (dataa,val);
}}}}}	m68k_incpc (2);
endlabel5268: ;
return 0;
}
/* ASLW.W -(An) */
unsigned long REGPARAM2 CPUFUNC(op_e1e0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa;
	do_cycles (2 * 256);
	dataa = m68k_areg(regs, srcreg) - 2;
	if (dataa & 1) {
		exception3 (opcode, m68k_getpc() + 2, dataa);
		goto endlabel5269;
	}
{{	uae_s16 data = get_word_ce (dataa);
	m68k_areg (regs, srcreg) = dataa;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 val = (uae_u16)data;
	uae_u32 sign = 0x8000 & val;
	uae_u32 sign2;
	val <<= 1;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(val)) == 0);
	SET_NFLG (((uae_s16)(val)) < 0);
	sign2 = 0x8000 & val;
	SET_CFLG (sign != 0);
	COPY_CARRY;
	SET_VFLG (GET_VFLG | (sign2 != sign));
	put_word_ce (dataa,val);
}}}}}	m68k_incpc (2);
endlabel5269: ;
return 0;
}
/* ASLW.W (d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_e1e8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dataa & 1) {
		exception3 (opcode, m68k_getpc() + 2, dataa);
		goto endlabel5270;
	}
{{	uae_s16 data = get_word_ce (dataa);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	uae_u32 val = (uae_u16)data;
	uae_u32 sign = 0x8000 & val;
	uae_u32 sign2;
	val <<= 1;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(val)) == 0);
	SET_NFLG (((uae_s16)(val)) < 0);
	sign2 = 0x8000 & val;
	SET_CFLG (sign != 0);
	COPY_CARRY;
	SET_VFLG (GET_VFLG | (sign2 != sign));
	put_word_ce (dataa,val);
}}}}}	m68k_incpc (4);
endlabel5270: ;
return 0;
}
/* ASLW.W (d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_e1f0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa;
	do_cycles (2 * 256);
	dataa = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	if (dataa & 1) {
		exception3 (opcode, m68k_getpc() + 2, dataa);
		goto endlabel5271;
	}
{{	uae_s16 data = get_word_ce (dataa);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	uae_u32 val = (uae_u16)data;
	uae_u32 sign = 0x8000 & val;
	uae_u32 sign2;
	val <<= 1;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(val)) == 0);
	SET_NFLG (((uae_s16)(val)) < 0);
	sign2 = 0x8000 & val;
	SET_CFLG (sign != 0);
	COPY_CARRY;
	SET_VFLG (GET_VFLG | (sign2 != sign));
	put_word_ce (dataa,val);
}}}}}	m68k_incpc (4);
endlabel5271: ;
return 0;
}
/* ASLW.W (xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_e1f8_6)(uae_u32 opcode)
{
{{	uaecptr dataa = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dataa & 1) {
		exception3 (opcode, m68k_getpc() + 2, dataa);
		goto endlabel5272;
	}
{{	uae_s16 data = get_word_ce (dataa);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	uae_u32 val = (uae_u16)data;
	uae_u32 sign = 0x8000 & val;
	uae_u32 sign2;
	val <<= 1;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(val)) == 0);
	SET_NFLG (((uae_s16)(val)) < 0);
	sign2 = 0x8000 & val;
	SET_CFLG (sign != 0);
	COPY_CARRY;
	SET_VFLG (GET_VFLG | (sign2 != sign));
	put_word_ce (dataa,val);
}}}}}	m68k_incpc (4);
endlabel5272: ;
return 0;
}
/* ASLW.W (xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_e1f9_6)(uae_u32 opcode)
{
{{	uaecptr dataa;
	dataa = get_word_ce_prefetch (4) << 16;
	dataa |= get_word_ce_prefetch (6);
	if (dataa & 1) {
		exception3 (opcode, m68k_getpc() + 2, dataa);
		goto endlabel5273;
	}
{{	uae_s16 data = get_word_ce (dataa);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
{	uae_u32 val = (uae_u16)data;
	uae_u32 sign = 0x8000 & val;
	uae_u32 sign2;
	val <<= 1;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(val)) == 0);
	SET_NFLG (((uae_s16)(val)) < 0);
	sign2 = 0x8000 & val;
	SET_CFLG (sign != 0);
	COPY_CARRY;
	SET_VFLG (GET_VFLG | (sign2 != sign));
	put_word_ce (dataa,val);
}}}}}	m68k_incpc (6);
endlabel5273: ;
return 0;
}
/* LSRW.W (An) */
unsigned long REGPARAM2 CPUFUNC(op_e2d0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg(regs, srcreg);
	if (dataa & 1) {
		exception3 (opcode, m68k_getpc() + 2, dataa);
		goto endlabel5274;
	}
{{	uae_s16 data = get_word_ce (dataa);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 val = (uae_u16)data;
	uae_u32 carry = val & 1;
	val >>= 1;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(val)) == 0);
	SET_NFLG (((uae_s16)(val)) < 0);
SET_CFLG (carry);
	COPY_CARRY;
	put_word_ce (dataa,val);
}}}}}	m68k_incpc (2);
endlabel5274: ;
return 0;
}
/* LSRW.W (An)+ */
unsigned long REGPARAM2 CPUFUNC(op_e2d8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg(regs, srcreg);
	if (dataa & 1) {
		exception3 (opcode, m68k_getpc() + 2, dataa);
		goto endlabel5275;
	}
{{	uae_s16 data = get_word_ce (dataa);
	m68k_areg(regs, srcreg) += 2;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 val = (uae_u16)data;
	uae_u32 carry = val & 1;
	val >>= 1;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(val)) == 0);
	SET_NFLG (((uae_s16)(val)) < 0);
SET_CFLG (carry);
	COPY_CARRY;
	put_word_ce (dataa,val);
}}}}}	m68k_incpc (2);
endlabel5275: ;
return 0;
}
/* LSRW.W -(An) */
unsigned long REGPARAM2 CPUFUNC(op_e2e0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa;
	do_cycles (2 * 256);
	dataa = m68k_areg(regs, srcreg) - 2;
	if (dataa & 1) {
		exception3 (opcode, m68k_getpc() + 2, dataa);
		goto endlabel5276;
	}
{{	uae_s16 data = get_word_ce (dataa);
	m68k_areg (regs, srcreg) = dataa;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 val = (uae_u16)data;
	uae_u32 carry = val & 1;
	val >>= 1;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(val)) == 0);
	SET_NFLG (((uae_s16)(val)) < 0);
SET_CFLG (carry);
	COPY_CARRY;
	put_word_ce (dataa,val);
}}}}}	m68k_incpc (2);
endlabel5276: ;
return 0;
}
/* LSRW.W (d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_e2e8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dataa & 1) {
		exception3 (opcode, m68k_getpc() + 2, dataa);
		goto endlabel5277;
	}
{{	uae_s16 data = get_word_ce (dataa);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	uae_u32 val = (uae_u16)data;
	uae_u32 carry = val & 1;
	val >>= 1;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(val)) == 0);
	SET_NFLG (((uae_s16)(val)) < 0);
SET_CFLG (carry);
	COPY_CARRY;
	put_word_ce (dataa,val);
}}}}}	m68k_incpc (4);
endlabel5277: ;
return 0;
}
/* LSRW.W (d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_e2f0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa;
	do_cycles (2 * 256);
	dataa = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	if (dataa & 1) {
		exception3 (opcode, m68k_getpc() + 2, dataa);
		goto endlabel5278;
	}
{{	uae_s16 data = get_word_ce (dataa);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	uae_u32 val = (uae_u16)data;
	uae_u32 carry = val & 1;
	val >>= 1;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(val)) == 0);
	SET_NFLG (((uae_s16)(val)) < 0);
SET_CFLG (carry);
	COPY_CARRY;
	put_word_ce (dataa,val);
}}}}}	m68k_incpc (4);
endlabel5278: ;
return 0;
}
/* LSRW.W (xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_e2f8_6)(uae_u32 opcode)
{
{{	uaecptr dataa = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dataa & 1) {
		exception3 (opcode, m68k_getpc() + 2, dataa);
		goto endlabel5279;
	}
{{	uae_s16 data = get_word_ce (dataa);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	uae_u32 val = (uae_u16)data;
	uae_u32 carry = val & 1;
	val >>= 1;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(val)) == 0);
	SET_NFLG (((uae_s16)(val)) < 0);
SET_CFLG (carry);
	COPY_CARRY;
	put_word_ce (dataa,val);
}}}}}	m68k_incpc (4);
endlabel5279: ;
return 0;
}
/* LSRW.W (xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_e2f9_6)(uae_u32 opcode)
{
{{	uaecptr dataa;
	dataa = get_word_ce_prefetch (4) << 16;
	dataa |= get_word_ce_prefetch (6);
	if (dataa & 1) {
		exception3 (opcode, m68k_getpc() + 2, dataa);
		goto endlabel5280;
	}
{{	uae_s16 data = get_word_ce (dataa);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
{	uae_u32 val = (uae_u16)data;
	uae_u32 carry = val & 1;
	val >>= 1;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(val)) == 0);
	SET_NFLG (((uae_s16)(val)) < 0);
SET_CFLG (carry);
	COPY_CARRY;
	put_word_ce (dataa,val);
}}}}}	m68k_incpc (6);
endlabel5280: ;
return 0;
}
/* LSLW.W (An) */
unsigned long REGPARAM2 CPUFUNC(op_e3d0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg(regs, srcreg);
	if (dataa & 1) {
		exception3 (opcode, m68k_getpc() + 2, dataa);
		goto endlabel5281;
	}
{{	uae_s16 data = get_word_ce (dataa);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u16 val = data;
	uae_u32 carry = val & 0x8000;
	val <<= 1;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(val)) == 0);
	SET_NFLG (((uae_s16)(val)) < 0);
SET_CFLG (carry >> 15);
	COPY_CARRY;
	put_word_ce (dataa,val);
}}}}}	m68k_incpc (2);
endlabel5281: ;
return 0;
}
/* LSLW.W (An)+ */
unsigned long REGPARAM2 CPUFUNC(op_e3d8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg(regs, srcreg);
	if (dataa & 1) {
		exception3 (opcode, m68k_getpc() + 2, dataa);
		goto endlabel5282;
	}
{{	uae_s16 data = get_word_ce (dataa);
	m68k_areg(regs, srcreg) += 2;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u16 val = data;
	uae_u32 carry = val & 0x8000;
	val <<= 1;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(val)) == 0);
	SET_NFLG (((uae_s16)(val)) < 0);
SET_CFLG (carry >> 15);
	COPY_CARRY;
	put_word_ce (dataa,val);
}}}}}	m68k_incpc (2);
endlabel5282: ;
return 0;
}
/* LSLW.W -(An) */
unsigned long REGPARAM2 CPUFUNC(op_e3e0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa;
	do_cycles (2 * 256);
	dataa = m68k_areg(regs, srcreg) - 2;
	if (dataa & 1) {
		exception3 (opcode, m68k_getpc() + 2, dataa);
		goto endlabel5283;
	}
{{	uae_s16 data = get_word_ce (dataa);
	m68k_areg (regs, srcreg) = dataa;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u16 val = data;
	uae_u32 carry = val & 0x8000;
	val <<= 1;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(val)) == 0);
	SET_NFLG (((uae_s16)(val)) < 0);
SET_CFLG (carry >> 15);
	COPY_CARRY;
	put_word_ce (dataa,val);
}}}}}	m68k_incpc (2);
endlabel5283: ;
return 0;
}
/* LSLW.W (d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_e3e8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dataa & 1) {
		exception3 (opcode, m68k_getpc() + 2, dataa);
		goto endlabel5284;
	}
{{	uae_s16 data = get_word_ce (dataa);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	uae_u16 val = data;
	uae_u32 carry = val & 0x8000;
	val <<= 1;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(val)) == 0);
	SET_NFLG (((uae_s16)(val)) < 0);
SET_CFLG (carry >> 15);
	COPY_CARRY;
	put_word_ce (dataa,val);
}}}}}	m68k_incpc (4);
endlabel5284: ;
return 0;
}
/* LSLW.W (d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_e3f0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa;
	do_cycles (2 * 256);
	dataa = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	if (dataa & 1) {
		exception3 (opcode, m68k_getpc() + 2, dataa);
		goto endlabel5285;
	}
{{	uae_s16 data = get_word_ce (dataa);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	uae_u16 val = data;
	uae_u32 carry = val & 0x8000;
	val <<= 1;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(val)) == 0);
	SET_NFLG (((uae_s16)(val)) < 0);
SET_CFLG (carry >> 15);
	COPY_CARRY;
	put_word_ce (dataa,val);
}}}}}	m68k_incpc (4);
endlabel5285: ;
return 0;
}
/* LSLW.W (xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_e3f8_6)(uae_u32 opcode)
{
{{	uaecptr dataa = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dataa & 1) {
		exception3 (opcode, m68k_getpc() + 2, dataa);
		goto endlabel5286;
	}
{{	uae_s16 data = get_word_ce (dataa);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	uae_u16 val = data;
	uae_u32 carry = val & 0x8000;
	val <<= 1;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(val)) == 0);
	SET_NFLG (((uae_s16)(val)) < 0);
SET_CFLG (carry >> 15);
	COPY_CARRY;
	put_word_ce (dataa,val);
}}}}}	m68k_incpc (4);
endlabel5286: ;
return 0;
}
/* LSLW.W (xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_e3f9_6)(uae_u32 opcode)
{
{{	uaecptr dataa;
	dataa = get_word_ce_prefetch (4) << 16;
	dataa |= get_word_ce_prefetch (6);
	if (dataa & 1) {
		exception3 (opcode, m68k_getpc() + 2, dataa);
		goto endlabel5287;
	}
{{	uae_s16 data = get_word_ce (dataa);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
{	uae_u16 val = data;
	uae_u32 carry = val & 0x8000;
	val <<= 1;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(val)) == 0);
	SET_NFLG (((uae_s16)(val)) < 0);
SET_CFLG (carry >> 15);
	COPY_CARRY;
	put_word_ce (dataa,val);
}}}}}	m68k_incpc (6);
endlabel5287: ;
return 0;
}
/* ROXRW.W (An) */
unsigned long REGPARAM2 CPUFUNC(op_e4d0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg(regs, srcreg);
	if (dataa & 1) {
		exception3 (opcode, m68k_getpc() + 2, dataa);
		goto endlabel5288;
	}
{{	uae_s16 data = get_word_ce (dataa);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u16 val = data;
	uae_u32 carry = val & 1;
	val >>= 1;
	if (GET_XFLG) val |= 0x8000;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(val)) == 0);
	SET_NFLG (((uae_s16)(val)) < 0);
SET_CFLG (carry);
	COPY_CARRY;
	put_word_ce (dataa,val);
}}}}}	m68k_incpc (2);
endlabel5288: ;
return 0;
}
/* ROXRW.W (An)+ */
unsigned long REGPARAM2 CPUFUNC(op_e4d8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg(regs, srcreg);
	if (dataa & 1) {
		exception3 (opcode, m68k_getpc() + 2, dataa);
		goto endlabel5289;
	}
{{	uae_s16 data = get_word_ce (dataa);
	m68k_areg(regs, srcreg) += 2;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u16 val = data;
	uae_u32 carry = val & 1;
	val >>= 1;
	if (GET_XFLG) val |= 0x8000;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(val)) == 0);
	SET_NFLG (((uae_s16)(val)) < 0);
SET_CFLG (carry);
	COPY_CARRY;
	put_word_ce (dataa,val);
}}}}}	m68k_incpc (2);
endlabel5289: ;
return 0;
}
/* ROXRW.W -(An) */
unsigned long REGPARAM2 CPUFUNC(op_e4e0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa;
	do_cycles (2 * 256);
	dataa = m68k_areg(regs, srcreg) - 2;
	if (dataa & 1) {
		exception3 (opcode, m68k_getpc() + 2, dataa);
		goto endlabel5290;
	}
{{	uae_s16 data = get_word_ce (dataa);
	m68k_areg (regs, srcreg) = dataa;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u16 val = data;
	uae_u32 carry = val & 1;
	val >>= 1;
	if (GET_XFLG) val |= 0x8000;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(val)) == 0);
	SET_NFLG (((uae_s16)(val)) < 0);
SET_CFLG (carry);
	COPY_CARRY;
	put_word_ce (dataa,val);
}}}}}	m68k_incpc (2);
endlabel5290: ;
return 0;
}
/* ROXRW.W (d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_e4e8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dataa & 1) {
		exception3 (opcode, m68k_getpc() + 2, dataa);
		goto endlabel5291;
	}
{{	uae_s16 data = get_word_ce (dataa);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	uae_u16 val = data;
	uae_u32 carry = val & 1;
	val >>= 1;
	if (GET_XFLG) val |= 0x8000;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(val)) == 0);
	SET_NFLG (((uae_s16)(val)) < 0);
SET_CFLG (carry);
	COPY_CARRY;
	put_word_ce (dataa,val);
}}}}}	m68k_incpc (4);
endlabel5291: ;
return 0;
}
/* ROXRW.W (d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_e4f0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa;
	do_cycles (2 * 256);
	dataa = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	if (dataa & 1) {
		exception3 (opcode, m68k_getpc() + 2, dataa);
		goto endlabel5292;
	}
{{	uae_s16 data = get_word_ce (dataa);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	uae_u16 val = data;
	uae_u32 carry = val & 1;
	val >>= 1;
	if (GET_XFLG) val |= 0x8000;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(val)) == 0);
	SET_NFLG (((uae_s16)(val)) < 0);
SET_CFLG (carry);
	COPY_CARRY;
	put_word_ce (dataa,val);
}}}}}	m68k_incpc (4);
endlabel5292: ;
return 0;
}
/* ROXRW.W (xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_e4f8_6)(uae_u32 opcode)
{
{{	uaecptr dataa = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dataa & 1) {
		exception3 (opcode, m68k_getpc() + 2, dataa);
		goto endlabel5293;
	}
{{	uae_s16 data = get_word_ce (dataa);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	uae_u16 val = data;
	uae_u32 carry = val & 1;
	val >>= 1;
	if (GET_XFLG) val |= 0x8000;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(val)) == 0);
	SET_NFLG (((uae_s16)(val)) < 0);
SET_CFLG (carry);
	COPY_CARRY;
	put_word_ce (dataa,val);
}}}}}	m68k_incpc (4);
endlabel5293: ;
return 0;
}
/* ROXRW.W (xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_e4f9_6)(uae_u32 opcode)
{
{{	uaecptr dataa;
	dataa = get_word_ce_prefetch (4) << 16;
	dataa |= get_word_ce_prefetch (6);
	if (dataa & 1) {
		exception3 (opcode, m68k_getpc() + 2, dataa);
		goto endlabel5294;
	}
{{	uae_s16 data = get_word_ce (dataa);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
{	uae_u16 val = data;
	uae_u32 carry = val & 1;
	val >>= 1;
	if (GET_XFLG) val |= 0x8000;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(val)) == 0);
	SET_NFLG (((uae_s16)(val)) < 0);
SET_CFLG (carry);
	COPY_CARRY;
	put_word_ce (dataa,val);
}}}}}	m68k_incpc (6);
endlabel5294: ;
return 0;
}
/* ROXLW.W (An) */
unsigned long REGPARAM2 CPUFUNC(op_e5d0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg(regs, srcreg);
	if (dataa & 1) {
		exception3 (opcode, m68k_getpc() + 2, dataa);
		goto endlabel5295;
	}
{{	uae_s16 data = get_word_ce (dataa);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u16 val = data;
	uae_u32 carry = val & 0x8000;
	val <<= 1;
	if (GET_XFLG) val |= 1;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(val)) == 0);
	SET_NFLG (((uae_s16)(val)) < 0);
SET_CFLG (carry >> 15);
	COPY_CARRY;
	put_word_ce (dataa,val);
}}}}}	m68k_incpc (2);
endlabel5295: ;
return 0;
}
/* ROXLW.W (An)+ */
unsigned long REGPARAM2 CPUFUNC(op_e5d8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg(regs, srcreg);
	if (dataa & 1) {
		exception3 (opcode, m68k_getpc() + 2, dataa);
		goto endlabel5296;
	}
{{	uae_s16 data = get_word_ce (dataa);
	m68k_areg(regs, srcreg) += 2;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u16 val = data;
	uae_u32 carry = val & 0x8000;
	val <<= 1;
	if (GET_XFLG) val |= 1;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(val)) == 0);
	SET_NFLG (((uae_s16)(val)) < 0);
SET_CFLG (carry >> 15);
	COPY_CARRY;
	put_word_ce (dataa,val);
}}}}}	m68k_incpc (2);
endlabel5296: ;
return 0;
}
/* ROXLW.W -(An) */
unsigned long REGPARAM2 CPUFUNC(op_e5e0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa;
	do_cycles (2 * 256);
	dataa = m68k_areg(regs, srcreg) - 2;
	if (dataa & 1) {
		exception3 (opcode, m68k_getpc() + 2, dataa);
		goto endlabel5297;
	}
{{	uae_s16 data = get_word_ce (dataa);
	m68k_areg (regs, srcreg) = dataa;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u16 val = data;
	uae_u32 carry = val & 0x8000;
	val <<= 1;
	if (GET_XFLG) val |= 1;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(val)) == 0);
	SET_NFLG (((uae_s16)(val)) < 0);
SET_CFLG (carry >> 15);
	COPY_CARRY;
	put_word_ce (dataa,val);
}}}}}	m68k_incpc (2);
endlabel5297: ;
return 0;
}
/* ROXLW.W (d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_e5e8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dataa & 1) {
		exception3 (opcode, m68k_getpc() + 2, dataa);
		goto endlabel5298;
	}
{{	uae_s16 data = get_word_ce (dataa);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	uae_u16 val = data;
	uae_u32 carry = val & 0x8000;
	val <<= 1;
	if (GET_XFLG) val |= 1;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(val)) == 0);
	SET_NFLG (((uae_s16)(val)) < 0);
SET_CFLG (carry >> 15);
	COPY_CARRY;
	put_word_ce (dataa,val);
}}}}}	m68k_incpc (4);
endlabel5298: ;
return 0;
}
/* ROXLW.W (d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_e5f0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa;
	do_cycles (2 * 256);
	dataa = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	if (dataa & 1) {
		exception3 (opcode, m68k_getpc() + 2, dataa);
		goto endlabel5299;
	}
{{	uae_s16 data = get_word_ce (dataa);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	uae_u16 val = data;
	uae_u32 carry = val & 0x8000;
	val <<= 1;
	if (GET_XFLG) val |= 1;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(val)) == 0);
	SET_NFLG (((uae_s16)(val)) < 0);
SET_CFLG (carry >> 15);
	COPY_CARRY;
	put_word_ce (dataa,val);
}}}}}	m68k_incpc (4);
endlabel5299: ;
return 0;
}
/* ROXLW.W (xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_e5f8_6)(uae_u32 opcode)
{
{{	uaecptr dataa = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dataa & 1) {
		exception3 (opcode, m68k_getpc() + 2, dataa);
		goto endlabel5300;
	}
{{	uae_s16 data = get_word_ce (dataa);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	uae_u16 val = data;
	uae_u32 carry = val & 0x8000;
	val <<= 1;
	if (GET_XFLG) val |= 1;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(val)) == 0);
	SET_NFLG (((uae_s16)(val)) < 0);
SET_CFLG (carry >> 15);
	COPY_CARRY;
	put_word_ce (dataa,val);
}}}}}	m68k_incpc (4);
endlabel5300: ;
return 0;
}
/* ROXLW.W (xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_e5f9_6)(uae_u32 opcode)
{
{{	uaecptr dataa;
	dataa = get_word_ce_prefetch (4) << 16;
	dataa |= get_word_ce_prefetch (6);
	if (dataa & 1) {
		exception3 (opcode, m68k_getpc() + 2, dataa);
		goto endlabel5301;
	}
{{	uae_s16 data = get_word_ce (dataa);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
{	uae_u16 val = data;
	uae_u32 carry = val & 0x8000;
	val <<= 1;
	if (GET_XFLG) val |= 1;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(val)) == 0);
	SET_NFLG (((uae_s16)(val)) < 0);
SET_CFLG (carry >> 15);
	COPY_CARRY;
	put_word_ce (dataa,val);
}}}}}	m68k_incpc (6);
endlabel5301: ;
return 0;
}
/* RORW.W (An) */
unsigned long REGPARAM2 CPUFUNC(op_e6d0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg(regs, srcreg);
	if (dataa & 1) {
		exception3 (opcode, m68k_getpc() + 2, dataa);
		goto endlabel5302;
	}
{{	uae_s16 data = get_word_ce (dataa);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u16 val = data;
	uae_u32 carry = val & 1;
	val >>= 1;
	if (carry) val |= 0x8000;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(val)) == 0);
	SET_NFLG (((uae_s16)(val)) < 0);
SET_CFLG (carry);
	put_word_ce (dataa,val);
}}}}}	m68k_incpc (2);
endlabel5302: ;
return 0;
}
/* RORW.W (An)+ */
unsigned long REGPARAM2 CPUFUNC(op_e6d8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg(regs, srcreg);
	if (dataa & 1) {
		exception3 (opcode, m68k_getpc() + 2, dataa);
		goto endlabel5303;
	}
{{	uae_s16 data = get_word_ce (dataa);
	m68k_areg(regs, srcreg) += 2;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u16 val = data;
	uae_u32 carry = val & 1;
	val >>= 1;
	if (carry) val |= 0x8000;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(val)) == 0);
	SET_NFLG (((uae_s16)(val)) < 0);
SET_CFLG (carry);
	put_word_ce (dataa,val);
}}}}}	m68k_incpc (2);
endlabel5303: ;
return 0;
}
/* RORW.W -(An) */
unsigned long REGPARAM2 CPUFUNC(op_e6e0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa;
	do_cycles (2 * 256);
	dataa = m68k_areg(regs, srcreg) - 2;
	if (dataa & 1) {
		exception3 (opcode, m68k_getpc() + 2, dataa);
		goto endlabel5304;
	}
{{	uae_s16 data = get_word_ce (dataa);
	m68k_areg (regs, srcreg) = dataa;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u16 val = data;
	uae_u32 carry = val & 1;
	val >>= 1;
	if (carry) val |= 0x8000;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(val)) == 0);
	SET_NFLG (((uae_s16)(val)) < 0);
SET_CFLG (carry);
	put_word_ce (dataa,val);
}}}}}	m68k_incpc (2);
endlabel5304: ;
return 0;
}
/* RORW.W (d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_e6e8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dataa & 1) {
		exception3 (opcode, m68k_getpc() + 2, dataa);
		goto endlabel5305;
	}
{{	uae_s16 data = get_word_ce (dataa);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	uae_u16 val = data;
	uae_u32 carry = val & 1;
	val >>= 1;
	if (carry) val |= 0x8000;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(val)) == 0);
	SET_NFLG (((uae_s16)(val)) < 0);
SET_CFLG (carry);
	put_word_ce (dataa,val);
}}}}}	m68k_incpc (4);
endlabel5305: ;
return 0;
}
/* RORW.W (d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_e6f0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa;
	do_cycles (2 * 256);
	dataa = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	if (dataa & 1) {
		exception3 (opcode, m68k_getpc() + 2, dataa);
		goto endlabel5306;
	}
{{	uae_s16 data = get_word_ce (dataa);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	uae_u16 val = data;
	uae_u32 carry = val & 1;
	val >>= 1;
	if (carry) val |= 0x8000;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(val)) == 0);
	SET_NFLG (((uae_s16)(val)) < 0);
SET_CFLG (carry);
	put_word_ce (dataa,val);
}}}}}	m68k_incpc (4);
endlabel5306: ;
return 0;
}
/* RORW.W (xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_e6f8_6)(uae_u32 opcode)
{
{{	uaecptr dataa = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dataa & 1) {
		exception3 (opcode, m68k_getpc() + 2, dataa);
		goto endlabel5307;
	}
{{	uae_s16 data = get_word_ce (dataa);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	uae_u16 val = data;
	uae_u32 carry = val & 1;
	val >>= 1;
	if (carry) val |= 0x8000;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(val)) == 0);
	SET_NFLG (((uae_s16)(val)) < 0);
SET_CFLG (carry);
	put_word_ce (dataa,val);
}}}}}	m68k_incpc (4);
endlabel5307: ;
return 0;
}
/* RORW.W (xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_e6f9_6)(uae_u32 opcode)
{
{{	uaecptr dataa;
	dataa = get_word_ce_prefetch (4) << 16;
	dataa |= get_word_ce_prefetch (6);
	if (dataa & 1) {
		exception3 (opcode, m68k_getpc() + 2, dataa);
		goto endlabel5308;
	}
{{	uae_s16 data = get_word_ce (dataa);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
{	uae_u16 val = data;
	uae_u32 carry = val & 1;
	val >>= 1;
	if (carry) val |= 0x8000;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(val)) == 0);
	SET_NFLG (((uae_s16)(val)) < 0);
SET_CFLG (carry);
	put_word_ce (dataa,val);
}}}}}	m68k_incpc (6);
endlabel5308: ;
return 0;
}
/* ROLW.W (An) */
unsigned long REGPARAM2 CPUFUNC(op_e7d0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg(regs, srcreg);
	if (dataa & 1) {
		exception3 (opcode, m68k_getpc() + 2, dataa);
		goto endlabel5309;
	}
{{	uae_s16 data = get_word_ce (dataa);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u16 val = data;
	uae_u32 carry = val & 0x8000;
	val <<= 1;
	if (carry)  val |= 1;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(val)) == 0);
	SET_NFLG (((uae_s16)(val)) < 0);
SET_CFLG (carry >> 15);
	put_word_ce (dataa,val);
}}}}}	m68k_incpc (2);
endlabel5309: ;
return 0;
}
/* ROLW.W (An)+ */
unsigned long REGPARAM2 CPUFUNC(op_e7d8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg(regs, srcreg);
	if (dataa & 1) {
		exception3 (opcode, m68k_getpc() + 2, dataa);
		goto endlabel5310;
	}
{{	uae_s16 data = get_word_ce (dataa);
	m68k_areg(regs, srcreg) += 2;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u16 val = data;
	uae_u32 carry = val & 0x8000;
	val <<= 1;
	if (carry)  val |= 1;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(val)) == 0);
	SET_NFLG (((uae_s16)(val)) < 0);
SET_CFLG (carry >> 15);
	put_word_ce (dataa,val);
}}}}}	m68k_incpc (2);
endlabel5310: ;
return 0;
}
/* ROLW.W -(An) */
unsigned long REGPARAM2 CPUFUNC(op_e7e0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa;
	do_cycles (2 * 256);
	dataa = m68k_areg(regs, srcreg) - 2;
	if (dataa & 1) {
		exception3 (opcode, m68k_getpc() + 2, dataa);
		goto endlabel5311;
	}
{{	uae_s16 data = get_word_ce (dataa);
	m68k_areg (regs, srcreg) = dataa;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u16 val = data;
	uae_u32 carry = val & 0x8000;
	val <<= 1;
	if (carry)  val |= 1;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(val)) == 0);
	SET_NFLG (((uae_s16)(val)) < 0);
SET_CFLG (carry >> 15);
	put_word_ce (dataa,val);
}}}}}	m68k_incpc (2);
endlabel5311: ;
return 0;
}
/* ROLW.W (d16,An) */
unsigned long REGPARAM2 CPUFUNC(op_e7e8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dataa & 1) {
		exception3 (opcode, m68k_getpc() + 2, dataa);
		goto endlabel5312;
	}
{{	uae_s16 data = get_word_ce (dataa);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	uae_u16 val = data;
	uae_u32 carry = val & 0x8000;
	val <<= 1;
	if (carry)  val |= 1;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(val)) == 0);
	SET_NFLG (((uae_s16)(val)) < 0);
SET_CFLG (carry >> 15);
	put_word_ce (dataa,val);
}}}}}	m68k_incpc (4);
endlabel5312: ;
return 0;
}
/* ROLW.W (d8,An,Xn) */
unsigned long REGPARAM2 CPUFUNC(op_e7f0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa;
	do_cycles (2 * 256);
	dataa = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	if (dataa & 1) {
		exception3 (opcode, m68k_getpc() + 2, dataa);
		goto endlabel5313;
	}
{{	uae_s16 data = get_word_ce (dataa);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	uae_u16 val = data;
	uae_u32 carry = val & 0x8000;
	val <<= 1;
	if (carry)  val |= 1;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(val)) == 0);
	SET_NFLG (((uae_s16)(val)) < 0);
SET_CFLG (carry >> 15);
	put_word_ce (dataa,val);
}}}}}	m68k_incpc (4);
endlabel5313: ;
return 0;
}
/* ROLW.W (xxx).W */
unsigned long REGPARAM2 CPUFUNC(op_e7f8_6)(uae_u32 opcode)
{
{{	uaecptr dataa = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dataa & 1) {
		exception3 (opcode, m68k_getpc() + 2, dataa);
		goto endlabel5314;
	}
{{	uae_s16 data = get_word_ce (dataa);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	uae_u16 val = data;
	uae_u32 carry = val & 0x8000;
	val <<= 1;
	if (carry)  val |= 1;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(val)) == 0);
	SET_NFLG (((uae_s16)(val)) < 0);
SET_CFLG (carry >> 15);
	put_word_ce (dataa,val);
}}}}}	m68k_incpc (4);
endlabel5314: ;
return 0;
}
/* ROLW.W (xxx).L */
unsigned long REGPARAM2 CPUFUNC(op_e7f9_6)(uae_u32 opcode)
{
{{	uaecptr dataa;
	dataa = get_word_ce_prefetch (4) << 16;
	dataa |= get_word_ce_prefetch (6);
	if (dataa & 1) {
		exception3 (opcode, m68k_getpc() + 2, dataa);
		goto endlabel5315;
	}
{{	uae_s16 data = get_word_ce (dataa);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
{	uae_u16 val = data;
	uae_u32 carry = val & 0x8000;
	val <<= 1;
	if (carry)  val |= 1;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(val)) == 0);
	SET_NFLG (((uae_s16)(val)) < 0);
SET_CFLG (carry >> 15);
	put_word_ce (dataa,val);
}}}}}	m68k_incpc (6);
endlabel5315: ;
return 0;
}
#endif


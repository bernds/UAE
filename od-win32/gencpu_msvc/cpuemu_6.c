#include "sysconfig.h"
#include "sysdeps.h"
#include "config.h"
#include "options.h"
#include "memory.h"
#include "custom.h"
#include "events.h"
#include "newcpu.h"
#include "cpu_prefetch.h"
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
void REGPARAM2 CPUFUNC(op_0000_6)(uae_u32 opcode)
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
}
/* OR.B #<data>.B,(An) */
void REGPARAM2 CPUFUNC(op_0010_6)(uae_u32 opcode)
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
}
/* OR.B #<data>.B,(An)+ */
void REGPARAM2 CPUFUNC(op_0018_6)(uae_u32 opcode)
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
}
/* OR.B #<data>.B,-(An) */
void REGPARAM2 CPUFUNC(op_0020_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce_prefetch (4);
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - areg_byteinc[dstreg];
	do_cycles_ce (2 * 256);
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
}
/* OR.B #<data>.B,(d16,An) */
void REGPARAM2 CPUFUNC(op_0028_6)(uae_u32 opcode)
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
}
/* OR.B #<data>.B,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_0030_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce_prefetch (4);
{	uaecptr dsta;
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (6));
	do_cycles_ce (2 * 256);
{	uae_s8 dst = get_byte_ce (dsta);
	src |= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	put_byte_ce (dsta,src);
}}}}	m68k_incpc (6);
}
/* OR.B #<data>.B,(xxx).W */
void REGPARAM2 CPUFUNC(op_0038_6)(uae_u32 opcode)
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
}
/* OR.B #<data>.B,(xxx).L */
void REGPARAM2 CPUFUNC(op_0039_6)(uae_u32 opcode)
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
}
/* ORSR.B #<data>.W */
void REGPARAM2 CPUFUNC(op_003c_6)(uae_u32 opcode)
{
{	MakeSR();
{	uae_s16 src = get_word_ce_prefetch (4);
	src &= 0xFF;
	do_cycles_ce (4 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	regs.sr |= src;
	MakeFromSR();
}}	m68k_incpc (4);
}
/* OR.W #<data>.W,Dn */
void REGPARAM2 CPUFUNC(op_0040_6)(uae_u32 opcode)
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
}
/* OR.W #<data>.W,(An) */
void REGPARAM2 CPUFUNC(op_0050_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel3719;
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
endlabel3719: ;
}
/* OR.W #<data>.W,(An)+ */
void REGPARAM2 CPUFUNC(op_0058_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel3720;
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
endlabel3720: ;
}
/* OR.W #<data>.W,-(An) */
void REGPARAM2 CPUFUNC(op_0060_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - 2;
	do_cycles_ce (2 * 256);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel3721;
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
endlabel3721: ;
}
/* OR.W #<data>.W,(d16,An) */
void REGPARAM2 CPUFUNC(op_0068_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (6);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel3722;
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
endlabel3722: ;
}
/* OR.W #<data>.W,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_0070_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta;
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (6));
	do_cycles_ce (2 * 256);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel3723;
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
endlabel3723: ;
}
/* OR.W #<data>.W,(xxx).W */
void REGPARAM2 CPUFUNC(op_0078_6)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (6);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel3724;
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
endlabel3724: ;
}
/* OR.W #<data>.W,(xxx).L */
void REGPARAM2 CPUFUNC(op_0079_6)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (6) << 16;
	dsta |= get_word_ce_prefetch (8);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel3725;
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
endlabel3725: ;
}
/* ORSR.W #<data>.W */
void REGPARAM2 CPUFUNC(op_007c_6)(uae_u32 opcode)
{
{if (!regs.s) { Exception(8,0); goto endlabel3726; }
{	MakeSR();
{	uae_s16 src = get_word_ce_prefetch (4);
	do_cycles_ce (4 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	regs.sr |= src;
	MakeFromSR();
}}}	m68k_incpc (4);
endlabel3726: ;
}
/* OR.L #<data>.L,Dn */
void REGPARAM2 CPUFUNC(op_0080_6)(uae_u32 opcode)
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
	do_cycles_ce (4 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	m68k_dreg(regs, dstreg) = (src);
}}}	m68k_incpc (6);
}
/* OR.L #<data>.L,(An) */
void REGPARAM2 CPUFUNC(op_0090_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel3728;
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
endlabel3728: ;
}
/* OR.L #<data>.L,(An)+ */
void REGPARAM2 CPUFUNC(op_0098_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel3729;
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
endlabel3729: ;
}
/* OR.L #<data>.L,-(An) */
void REGPARAM2 CPUFUNC(op_00a0_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - 4;
	do_cycles_ce (2 * 256);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel3730;
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
endlabel3730: ;
}
/* OR.L #<data>.L,(d16,An) */
void REGPARAM2 CPUFUNC(op_00a8_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (8);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel3731;
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
endlabel3731: ;
}
/* OR.L #<data>.L,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_00b0_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uaecptr dsta;
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (8));
	do_cycles_ce (2 * 256);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel3732;
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
endlabel3732: ;
}
/* OR.L #<data>.L,(xxx).W */
void REGPARAM2 CPUFUNC(op_00b8_6)(uae_u32 opcode)
{
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (8);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel3733;
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
endlabel3733: ;
}
/* OR.L #<data>.L,(xxx).L */
void REGPARAM2 CPUFUNC(op_00b9_6)(uae_u32 opcode)
{
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (8) << 16;
	dsta |= get_word_ce_prefetch (10);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel3734;
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
endlabel3734: ;
}
/* BTST.L Dn,Dn */
void REGPARAM2 CPUFUNC(op_0100_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	do_cycles_ce (2 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	src &= 31;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
}}}	m68k_incpc (2);
}
/* MVPMR.W (d16,An),Dn */
void REGPARAM2 CPUFUNC(op_0108_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{	uaecptr memp = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_u16 val = (get_byte_ce (memp) << 8) + get_byte_ce (memp + 2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xffff) | ((val) & 0xffff);
}}	m68k_incpc (4);
}
/* BTST.B Dn,(An) */
void REGPARAM2 CPUFUNC(op_0110_6)(uae_u32 opcode)
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
}
/* BTST.B Dn,(An)+ */
void REGPARAM2 CPUFUNC(op_0118_6)(uae_u32 opcode)
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
}
/* BTST.B Dn,-(An) */
void REGPARAM2 CPUFUNC(op_0120_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - areg_byteinc[dstreg];
	do_cycles_ce (2 * 256);
{	uae_s8 dst = get_byte_ce (dsta);
	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
}}}}	m68k_incpc (2);
}
/* BTST.B Dn,(d16,An) */
void REGPARAM2 CPUFUNC(op_0128_6)(uae_u32 opcode)
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
}
/* BTST.B Dn,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_0130_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
{	uae_s8 dst = get_byte_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
}}}}	m68k_incpc (4);
}
/* BTST.B Dn,(xxx).W */
void REGPARAM2 CPUFUNC(op_0138_6)(uae_u32 opcode)
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
}
/* BTST.B Dn,(xxx).L */
void REGPARAM2 CPUFUNC(op_0139_6)(uae_u32 opcode)
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
}
/* BTST.B Dn,(d16,PC) */
void REGPARAM2 CPUFUNC(op_013a_6)(uae_u32 opcode)
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
}
/* BTST.B Dn,(d8,PC,Xn) */
void REGPARAM2 CPUFUNC(op_013b_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = 3;
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	uaecptr tmppc;
	uaecptr dsta;
	tmppc = m68k_getpc() + 2;
	dsta = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
{	uae_s8 dst = get_byte_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
}}}}	m68k_incpc (4);
}
/* BTST.B Dn,#<data>.B */
void REGPARAM2 CPUFUNC(op_013c_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	uae_s8 dst = (uae_u8)get_word_ce_prefetch (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
}}}	m68k_incpc (4);
}
/* BCHG.L Dn,Dn */
void REGPARAM2 CPUFUNC(op_0140_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	do_cycles_ce (4 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	src &= 31;
	dst ^= (1 << src);
	SET_ZFLG (((uae_u32)dst & (1 << src)) >> src);
	m68k_dreg(regs, dstreg) = (dst);
}}}	m68k_incpc (2);
}
/* MVPMR.L (d16,An),Dn */
void REGPARAM2 CPUFUNC(op_0148_6)(uae_u32 opcode)
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
}
/* BCHG.B Dn,(An) */
void REGPARAM2 CPUFUNC(op_0150_6)(uae_u32 opcode)
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
}
/* BCHG.B Dn,(An)+ */
void REGPARAM2 CPUFUNC(op_0158_6)(uae_u32 opcode)
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
}
/* BCHG.B Dn,-(An) */
void REGPARAM2 CPUFUNC(op_0160_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - areg_byteinc[dstreg];
	do_cycles_ce (2 * 256);
{	uae_s8 dst = get_byte_ce (dsta);
	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	src &= 7;
	dst ^= (1 << src);
	SET_ZFLG (((uae_u32)dst & (1 << src)) >> src);
	put_byte_ce (dsta,dst);
}}}}	m68k_incpc (2);
}
/* BCHG.B Dn,(d16,An) */
void REGPARAM2 CPUFUNC(op_0168_6)(uae_u32 opcode)
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
}
/* BCHG.B Dn,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_0170_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
{	uae_s8 dst = get_byte_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	src &= 7;
	dst ^= (1 << src);
	SET_ZFLG (((uae_u32)dst & (1 << src)) >> src);
	put_byte_ce (dsta,dst);
}}}}	m68k_incpc (4);
}
/* BCHG.B Dn,(xxx).W */
void REGPARAM2 CPUFUNC(op_0178_6)(uae_u32 opcode)
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
}
/* BCHG.B Dn,(xxx).L */
void REGPARAM2 CPUFUNC(op_0179_6)(uae_u32 opcode)
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
}
/* BCHG.B Dn,(d16,PC) */
void REGPARAM2 CPUFUNC(op_017a_6)(uae_u32 opcode)
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
}
/* BCHG.B Dn,(d8,PC,Xn) */
void REGPARAM2 CPUFUNC(op_017b_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = 3;
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	uaecptr tmppc;
	uaecptr dsta;
	tmppc = m68k_getpc() + 2;
	dsta = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
{	uae_s8 dst = get_byte_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	src &= 7;
	dst ^= (1 << src);
	SET_ZFLG (((uae_u32)dst & (1 << src)) >> src);
	put_byte_ce (dsta,dst);
}}}}	m68k_incpc (4);
}
/* BCLR.L Dn,Dn */
void REGPARAM2 CPUFUNC(op_0180_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	do_cycles_ce (4 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	src &= 31;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst &= ~(1 << src);
	m68k_dreg(regs, dstreg) = (dst);
}}}	m68k_incpc (2);
}
/* MVPRM.W Dn,(d16,An) */
void REGPARAM2 CPUFUNC(op_0188_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg(regs, srcreg);
	uaecptr memp = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	put_byte_ce (memp, src >> 8); put_byte_ce (memp + 2, src);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
}}	m68k_incpc (4);
}
/* BCLR.B Dn,(An) */
void REGPARAM2 CPUFUNC(op_0190_6)(uae_u32 opcode)
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
}
/* BCLR.B Dn,(An)+ */
void REGPARAM2 CPUFUNC(op_0198_6)(uae_u32 opcode)
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
}
/* BCLR.B Dn,-(An) */
void REGPARAM2 CPUFUNC(op_01a0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - areg_byteinc[dstreg];
	do_cycles_ce (2 * 256);
{	uae_s8 dst = get_byte_ce (dsta);
	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst &= ~(1 << src);
	put_byte_ce (dsta,dst);
}}}}	m68k_incpc (2);
}
/* BCLR.B Dn,(d16,An) */
void REGPARAM2 CPUFUNC(op_01a8_6)(uae_u32 opcode)
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
}
/* BCLR.B Dn,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_01b0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
{	uae_s8 dst = get_byte_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst &= ~(1 << src);
	put_byte_ce (dsta,dst);
}}}}	m68k_incpc (4);
}
/* BCLR.B Dn,(xxx).W */
void REGPARAM2 CPUFUNC(op_01b8_6)(uae_u32 opcode)
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
}
/* BCLR.B Dn,(xxx).L */
void REGPARAM2 CPUFUNC(op_01b9_6)(uae_u32 opcode)
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
}
/* BCLR.B Dn,(d16,PC) */
void REGPARAM2 CPUFUNC(op_01ba_6)(uae_u32 opcode)
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
}
/* BCLR.B Dn,(d8,PC,Xn) */
void REGPARAM2 CPUFUNC(op_01bb_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = 3;
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	uaecptr tmppc;
	uaecptr dsta;
	tmppc = m68k_getpc() + 2;
	dsta = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
{	uae_s8 dst = get_byte_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst &= ~(1 << src);
	put_byte_ce (dsta,dst);
}}}}	m68k_incpc (4);
}
/* BSET.L Dn,Dn */
void REGPARAM2 CPUFUNC(op_01c0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	do_cycles_ce (4 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	src &= 31;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst |= (1 << src);
	m68k_dreg(regs, dstreg) = (dst);
}}}	m68k_incpc (2);
}
/* MVPRM.L Dn,(d16,An) */
void REGPARAM2 CPUFUNC(op_01c8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg(regs, srcreg);
	uaecptr memp = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	put_byte_ce (memp, src >> 24); put_byte_ce (memp + 2, src >> 16);
	put_byte_ce (memp + 4, src >> 8); put_byte_ce (memp + 6, src);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
}}	m68k_incpc (4);
}
/* BSET.B Dn,(An) */
void REGPARAM2 CPUFUNC(op_01d0_6)(uae_u32 opcode)
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
}
/* BSET.B Dn,(An)+ */
void REGPARAM2 CPUFUNC(op_01d8_6)(uae_u32 opcode)
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
}
/* BSET.B Dn,-(An) */
void REGPARAM2 CPUFUNC(op_01e0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - areg_byteinc[dstreg];
	do_cycles_ce (2 * 256);
{	uae_s8 dst = get_byte_ce (dsta);
	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst |= (1 << src);
	put_byte_ce (dsta,dst);
}}}}	m68k_incpc (2);
}
/* BSET.B Dn,(d16,An) */
void REGPARAM2 CPUFUNC(op_01e8_6)(uae_u32 opcode)
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
}
/* BSET.B Dn,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_01f0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
{	uae_s8 dst = get_byte_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst |= (1 << src);
	put_byte_ce (dsta,dst);
}}}}	m68k_incpc (4);
}
/* BSET.B Dn,(xxx).W */
void REGPARAM2 CPUFUNC(op_01f8_6)(uae_u32 opcode)
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
}
/* BSET.B Dn,(xxx).L */
void REGPARAM2 CPUFUNC(op_01f9_6)(uae_u32 opcode)
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
}
/* BSET.B Dn,(d16,PC) */
void REGPARAM2 CPUFUNC(op_01fa_6)(uae_u32 opcode)
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
}
/* BSET.B Dn,(d8,PC,Xn) */
void REGPARAM2 CPUFUNC(op_01fb_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = 3;
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	uaecptr tmppc;
	uaecptr dsta;
	tmppc = m68k_getpc() + 2;
	dsta = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
{	uae_s8 dst = get_byte_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst |= (1 << src);
	put_byte_ce (dsta,dst);
}}}}	m68k_incpc (4);
}
/* AND.B #<data>.B,Dn */
void REGPARAM2 CPUFUNC(op_0200_6)(uae_u32 opcode)
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
}
/* AND.B #<data>.B,(An) */
void REGPARAM2 CPUFUNC(op_0210_6)(uae_u32 opcode)
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
}
/* AND.B #<data>.B,(An)+ */
void REGPARAM2 CPUFUNC(op_0218_6)(uae_u32 opcode)
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
}
/* AND.B #<data>.B,-(An) */
void REGPARAM2 CPUFUNC(op_0220_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce_prefetch (4);
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - areg_byteinc[dstreg];
	do_cycles_ce (2 * 256);
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
}
/* AND.B #<data>.B,(d16,An) */
void REGPARAM2 CPUFUNC(op_0228_6)(uae_u32 opcode)
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
}
/* AND.B #<data>.B,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_0230_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce_prefetch (4);
{	uaecptr dsta;
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (6));
	do_cycles_ce (2 * 256);
{	uae_s8 dst = get_byte_ce (dsta);
	src &= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	put_byte_ce (dsta,src);
}}}}	m68k_incpc (6);
}
/* AND.B #<data>.B,(xxx).W */
void REGPARAM2 CPUFUNC(op_0238_6)(uae_u32 opcode)
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
}
/* AND.B #<data>.B,(xxx).L */
void REGPARAM2 CPUFUNC(op_0239_6)(uae_u32 opcode)
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
}
/* ANDSR.B #<data>.W */
void REGPARAM2 CPUFUNC(op_023c_6)(uae_u32 opcode)
{
{	MakeSR();
{	uae_s16 src = get_word_ce_prefetch (4);
	src |= 0xFF00;
	do_cycles_ce (4 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	regs.sr &= src;
	MakeFromSR();
}}	m68k_incpc (4);
}
/* AND.W #<data>.W,Dn */
void REGPARAM2 CPUFUNC(op_0240_6)(uae_u32 opcode)
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
}
/* AND.W #<data>.W,(An) */
void REGPARAM2 CPUFUNC(op_0250_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel3790;
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
endlabel3790: ;
}
/* AND.W #<data>.W,(An)+ */
void REGPARAM2 CPUFUNC(op_0258_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel3791;
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
endlabel3791: ;
}
/* AND.W #<data>.W,-(An) */
void REGPARAM2 CPUFUNC(op_0260_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - 2;
	do_cycles_ce (2 * 256);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel3792;
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
endlabel3792: ;
}
/* AND.W #<data>.W,(d16,An) */
void REGPARAM2 CPUFUNC(op_0268_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (6);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel3793;
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
endlabel3793: ;
}
/* AND.W #<data>.W,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_0270_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta;
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (6));
	do_cycles_ce (2 * 256);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel3794;
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
endlabel3794: ;
}
/* AND.W #<data>.W,(xxx).W */
void REGPARAM2 CPUFUNC(op_0278_6)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (6);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel3795;
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
endlabel3795: ;
}
/* AND.W #<data>.W,(xxx).L */
void REGPARAM2 CPUFUNC(op_0279_6)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (6) << 16;
	dsta |= get_word_ce_prefetch (8);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel3796;
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
endlabel3796: ;
}
/* ANDSR.W #<data>.W */
void REGPARAM2 CPUFUNC(op_027c_6)(uae_u32 opcode)
{
{if (!regs.s) { Exception(8,0); goto endlabel3797; }
{	MakeSR();
{	uae_s16 src = get_word_ce_prefetch (4);
	do_cycles_ce (4 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	regs.sr &= src;
	MakeFromSR();
}}}	m68k_incpc (4);
endlabel3797: ;
}
/* AND.L #<data>.L,Dn */
void REGPARAM2 CPUFUNC(op_0280_6)(uae_u32 opcode)
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
	do_cycles_ce (2 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	m68k_dreg(regs, dstreg) = (src);
}}}	m68k_incpc (6);
}
/* AND.L #<data>.L,(An) */
void REGPARAM2 CPUFUNC(op_0290_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel3799;
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
endlabel3799: ;
}
/* AND.L #<data>.L,(An)+ */
void REGPARAM2 CPUFUNC(op_0298_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel3800;
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
endlabel3800: ;
}
/* AND.L #<data>.L,-(An) */
void REGPARAM2 CPUFUNC(op_02a0_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - 4;
	do_cycles_ce (2 * 256);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel3801;
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
endlabel3801: ;
}
/* AND.L #<data>.L,(d16,An) */
void REGPARAM2 CPUFUNC(op_02a8_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (8);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel3802;
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
endlabel3802: ;
}
/* AND.L #<data>.L,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_02b0_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uaecptr dsta;
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (8));
	do_cycles_ce (2 * 256);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel3803;
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
endlabel3803: ;
}
/* AND.L #<data>.L,(xxx).W */
void REGPARAM2 CPUFUNC(op_02b8_6)(uae_u32 opcode)
{
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (8);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel3804;
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
endlabel3804: ;
}
/* AND.L #<data>.L,(xxx).L */
void REGPARAM2 CPUFUNC(op_02b9_6)(uae_u32 opcode)
{
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (8) << 16;
	dsta |= get_word_ce_prefetch (10);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel3805;
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
endlabel3805: ;
}
/* SUB.B #<data>.B,Dn */
void REGPARAM2 CPUFUNC(op_0400_6)(uae_u32 opcode)
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
}
/* SUB.B #<data>.B,(An) */
void REGPARAM2 CPUFUNC(op_0410_6)(uae_u32 opcode)
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
}
/* SUB.B #<data>.B,(An)+ */
void REGPARAM2 CPUFUNC(op_0418_6)(uae_u32 opcode)
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
}
/* SUB.B #<data>.B,-(An) */
void REGPARAM2 CPUFUNC(op_0420_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce_prefetch (4);
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - areg_byteinc[dstreg];
	do_cycles_ce (2 * 256);
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
}
/* SUB.B #<data>.B,(d16,An) */
void REGPARAM2 CPUFUNC(op_0428_6)(uae_u32 opcode)
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
}
/* SUB.B #<data>.B,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_0430_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce_prefetch (4);
{	uaecptr dsta;
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (6));
	do_cycles_ce (2 * 256);
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
}
/* SUB.B #<data>.B,(xxx).W */
void REGPARAM2 CPUFUNC(op_0438_6)(uae_u32 opcode)
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
}
/* SUB.B #<data>.B,(xxx).L */
void REGPARAM2 CPUFUNC(op_0439_6)(uae_u32 opcode)
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
}
/* SUB.W #<data>.W,Dn */
void REGPARAM2 CPUFUNC(op_0440_6)(uae_u32 opcode)
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
}
/* SUB.W #<data>.W,(An) */
void REGPARAM2 CPUFUNC(op_0450_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel3815;
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
endlabel3815: ;
}
/* SUB.W #<data>.W,(An)+ */
void REGPARAM2 CPUFUNC(op_0458_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel3816;
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
endlabel3816: ;
}
/* SUB.W #<data>.W,-(An) */
void REGPARAM2 CPUFUNC(op_0460_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - 2;
	do_cycles_ce (2 * 256);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel3817;
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
endlabel3817: ;
}
/* SUB.W #<data>.W,(d16,An) */
void REGPARAM2 CPUFUNC(op_0468_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (6);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel3818;
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
endlabel3818: ;
}
/* SUB.W #<data>.W,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_0470_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta;
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (6));
	do_cycles_ce (2 * 256);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel3819;
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
endlabel3819: ;
}
/* SUB.W #<data>.W,(xxx).W */
void REGPARAM2 CPUFUNC(op_0478_6)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (6);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel3820;
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
endlabel3820: ;
}
/* SUB.W #<data>.W,(xxx).L */
void REGPARAM2 CPUFUNC(op_0479_6)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (6) << 16;
	dsta |= get_word_ce_prefetch (8);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel3821;
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
endlabel3821: ;
}
/* SUB.L #<data>.L,Dn */
void REGPARAM2 CPUFUNC(op_0480_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	do_cycles_ce (4 * 256);
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
	m68k_dreg(regs, dstreg) = (newv);
}}}}}}	m68k_incpc (6);
}
/* SUB.L #<data>.L,(An) */
void REGPARAM2 CPUFUNC(op_0490_6)(uae_u32 opcode)
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
endlabel3823: ;
}
/* SUB.L #<data>.L,(An)+ */
void REGPARAM2 CPUFUNC(op_0498_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel3824;
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
endlabel3824: ;
}
/* SUB.L #<data>.L,-(An) */
void REGPARAM2 CPUFUNC(op_04a0_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - 4;
	do_cycles_ce (2 * 256);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel3825;
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
endlabel3825: ;
}
/* SUB.L #<data>.L,(d16,An) */
void REGPARAM2 CPUFUNC(op_04a8_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (8);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel3826;
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
endlabel3826: ;
}
/* SUB.L #<data>.L,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_04b0_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uaecptr dsta;
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (8));
	do_cycles_ce (2 * 256);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel3827;
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
endlabel3827: ;
}
/* SUB.L #<data>.L,(xxx).W */
void REGPARAM2 CPUFUNC(op_04b8_6)(uae_u32 opcode)
{
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (8);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel3828;
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
endlabel3828: ;
}
/* SUB.L #<data>.L,(xxx).L */
void REGPARAM2 CPUFUNC(op_04b9_6)(uae_u32 opcode)
{
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (8) << 16;
	dsta |= get_word_ce_prefetch (10);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel3829;
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
endlabel3829: ;
}
/* ADD.B #<data>.B,Dn */
void REGPARAM2 CPUFUNC(op_0600_6)(uae_u32 opcode)
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
}
/* ADD.B #<data>.B,(An) */
void REGPARAM2 CPUFUNC(op_0610_6)(uae_u32 opcode)
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
}
/* ADD.B #<data>.B,(An)+ */
void REGPARAM2 CPUFUNC(op_0618_6)(uae_u32 opcode)
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
}
/* ADD.B #<data>.B,-(An) */
void REGPARAM2 CPUFUNC(op_0620_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce_prefetch (4);
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - areg_byteinc[dstreg];
	do_cycles_ce (2 * 256);
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
}
/* ADD.B #<data>.B,(d16,An) */
void REGPARAM2 CPUFUNC(op_0628_6)(uae_u32 opcode)
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
}
/* ADD.B #<data>.B,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_0630_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce_prefetch (4);
{	uaecptr dsta;
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (6));
	do_cycles_ce (2 * 256);
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
}
/* ADD.B #<data>.B,(xxx).W */
void REGPARAM2 CPUFUNC(op_0638_6)(uae_u32 opcode)
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
}
/* ADD.B #<data>.B,(xxx).L */
void REGPARAM2 CPUFUNC(op_0639_6)(uae_u32 opcode)
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
}
/* ADD.W #<data>.W,Dn */
void REGPARAM2 CPUFUNC(op_0640_6)(uae_u32 opcode)
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
}
/* ADD.W #<data>.W,(An) */
void REGPARAM2 CPUFUNC(op_0650_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel3839;
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
endlabel3839: ;
}
/* ADD.W #<data>.W,(An)+ */
void REGPARAM2 CPUFUNC(op_0658_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel3840;
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
endlabel3840: ;
}
/* ADD.W #<data>.W,-(An) */
void REGPARAM2 CPUFUNC(op_0660_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - 2;
	do_cycles_ce (2 * 256);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel3841;
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
endlabel3841: ;
}
/* ADD.W #<data>.W,(d16,An) */
void REGPARAM2 CPUFUNC(op_0668_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (6);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel3842;
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
endlabel3842: ;
}
/* ADD.W #<data>.W,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_0670_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta;
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (6));
	do_cycles_ce (2 * 256);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel3843;
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
endlabel3843: ;
}
/* ADD.W #<data>.W,(xxx).W */
void REGPARAM2 CPUFUNC(op_0678_6)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (6);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel3844;
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
endlabel3844: ;
}
/* ADD.W #<data>.W,(xxx).L */
void REGPARAM2 CPUFUNC(op_0679_6)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (6) << 16;
	dsta |= get_word_ce_prefetch (8);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel3845;
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
endlabel3845: ;
}
/* ADD.L #<data>.L,Dn */
void REGPARAM2 CPUFUNC(op_0680_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	do_cycles_ce (4 * 256);
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
	m68k_dreg(regs, dstreg) = (newv);
}}}}}}	m68k_incpc (6);
}
/* ADD.L #<data>.L,(An) */
void REGPARAM2 CPUFUNC(op_0690_6)(uae_u32 opcode)
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
endlabel3847: ;
}
/* ADD.L #<data>.L,(An)+ */
void REGPARAM2 CPUFUNC(op_0698_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel3848;
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
endlabel3848: ;
}
/* ADD.L #<data>.L,-(An) */
void REGPARAM2 CPUFUNC(op_06a0_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - 4;
	do_cycles_ce (2 * 256);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel3849;
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
endlabel3849: ;
}
/* ADD.L #<data>.L,(d16,An) */
void REGPARAM2 CPUFUNC(op_06a8_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (8);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel3850;
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
endlabel3850: ;
}
/* ADD.L #<data>.L,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_06b0_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uaecptr dsta;
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (8));
	do_cycles_ce (2 * 256);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel3851;
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
endlabel3851: ;
}
/* ADD.L #<data>.L,(xxx).W */
void REGPARAM2 CPUFUNC(op_06b8_6)(uae_u32 opcode)
{
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (8);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel3852;
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
endlabel3852: ;
}
/* ADD.L #<data>.L,(xxx).L */
void REGPARAM2 CPUFUNC(op_06b9_6)(uae_u32 opcode)
{
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (8) << 16;
	dsta |= get_word_ce_prefetch (10);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel3853;
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
endlabel3853: ;
}
/* BTST.L #<data>.W,Dn */
void REGPARAM2 CPUFUNC(op_0800_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	do_cycles_ce (2 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	src &= 31;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
}}}	m68k_incpc (4);
}
/* BTST.B #<data>.W,(An) */
void REGPARAM2 CPUFUNC(op_0810_6)(uae_u32 opcode)
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
}
/* BTST.B #<data>.W,(An)+ */
void REGPARAM2 CPUFUNC(op_0818_6)(uae_u32 opcode)
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
}
/* BTST.B #<data>.W,-(An) */
void REGPARAM2 CPUFUNC(op_0820_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - areg_byteinc[dstreg];
	do_cycles_ce (2 * 256);
{	uae_s8 dst = get_byte_ce (dsta);
	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
}}}}	m68k_incpc (4);
}
/* BTST.B #<data>.W,(d16,An) */
void REGPARAM2 CPUFUNC(op_0828_6)(uae_u32 opcode)
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
}
/* BTST.B #<data>.W,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_0830_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta;
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (6));
	do_cycles_ce (2 * 256);
{	uae_s8 dst = get_byte_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
}}}}	m68k_incpc (6);
}
/* BTST.B #<data>.W,(xxx).W */
void REGPARAM2 CPUFUNC(op_0838_6)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (6);
{	uae_s8 dst = get_byte_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
}}}}	m68k_incpc (6);
}
/* BTST.B #<data>.W,(xxx).L */
void REGPARAM2 CPUFUNC(op_0839_6)(uae_u32 opcode)
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
}
/* BTST.B #<data>.W,(d16,PC) */
void REGPARAM2 CPUFUNC(op_083a_6)(uae_u32 opcode)
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
}
/* BTST.B #<data>.W,(d8,PC,Xn) */
void REGPARAM2 CPUFUNC(op_083b_6)(uae_u32 opcode)
{
	uae_u32 dstreg = 3;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr tmppc;
	uaecptr dsta;
	tmppc = m68k_getpc() + 4;
	dsta = get_disp_ea_000(tmppc, get_word_ce_prefetch (6));
	do_cycles_ce (2 * 256);
{	uae_s8 dst = get_byte_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
}}}}	m68k_incpc (6);
}
/* BTST.B #<data>.W,#<data>.B */
void REGPARAM2 CPUFUNC(op_083c_6)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uae_s8 dst = (uae_u8)get_word_ce_prefetch (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
}}}	m68k_incpc (6);
}
/* BCHG.L #<data>.W,Dn */
void REGPARAM2 CPUFUNC(op_0840_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	do_cycles_ce (4 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	src &= 31;
	dst ^= (1 << src);
	SET_ZFLG (((uae_u32)dst & (1 << src)) >> src);
	m68k_dreg(regs, dstreg) = (dst);
}}}	m68k_incpc (4);
}
/* BCHG.B #<data>.W,(An) */
void REGPARAM2 CPUFUNC(op_0850_6)(uae_u32 opcode)
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
}
/* BCHG.B #<data>.W,(An)+ */
void REGPARAM2 CPUFUNC(op_0858_6)(uae_u32 opcode)
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
}
/* BCHG.B #<data>.W,-(An) */
void REGPARAM2 CPUFUNC(op_0860_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - areg_byteinc[dstreg];
	do_cycles_ce (2 * 256);
{	uae_s8 dst = get_byte_ce (dsta);
	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	src &= 7;
	dst ^= (1 << src);
	SET_ZFLG (((uae_u32)dst & (1 << src)) >> src);
	put_byte_ce (dsta,dst);
}}}}	m68k_incpc (4);
}
/* BCHG.B #<data>.W,(d16,An) */
void REGPARAM2 CPUFUNC(op_0868_6)(uae_u32 opcode)
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
}
/* BCHG.B #<data>.W,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_0870_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta;
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (6));
	do_cycles_ce (2 * 256);
{	uae_s8 dst = get_byte_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	src &= 7;
	dst ^= (1 << src);
	SET_ZFLG (((uae_u32)dst & (1 << src)) >> src);
	put_byte_ce (dsta,dst);
}}}}	m68k_incpc (6);
}
/* BCHG.B #<data>.W,(xxx).W */
void REGPARAM2 CPUFUNC(op_0878_6)(uae_u32 opcode)
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
}
/* BCHG.B #<data>.W,(xxx).L */
void REGPARAM2 CPUFUNC(op_0879_6)(uae_u32 opcode)
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
}
/* BCHG.B #<data>.W,(d16,PC) */
void REGPARAM2 CPUFUNC(op_087a_6)(uae_u32 opcode)
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
}
/* BCHG.B #<data>.W,(d8,PC,Xn) */
void REGPARAM2 CPUFUNC(op_087b_6)(uae_u32 opcode)
{
	uae_u32 dstreg = 3;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr tmppc;
	uaecptr dsta;
	tmppc = m68k_getpc() + 4;
	dsta = get_disp_ea_000(tmppc, get_word_ce_prefetch (6));
	do_cycles_ce (2 * 256);
{	uae_s8 dst = get_byte_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	src &= 7;
	dst ^= (1 << src);
	SET_ZFLG (((uae_u32)dst & (1 << src)) >> src);
	put_byte_ce (dsta,dst);
}}}}	m68k_incpc (6);
}
/* BCLR.L #<data>.W,Dn */
void REGPARAM2 CPUFUNC(op_0880_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	do_cycles_ce (4 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	src &= 31;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst &= ~(1 << src);
	m68k_dreg(regs, dstreg) = (dst);
}}}	m68k_incpc (4);
}
/* BCLR.B #<data>.W,(An) */
void REGPARAM2 CPUFUNC(op_0890_6)(uae_u32 opcode)
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
}
/* BCLR.B #<data>.W,(An)+ */
void REGPARAM2 CPUFUNC(op_0898_6)(uae_u32 opcode)
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
}
/* BCLR.B #<data>.W,-(An) */
void REGPARAM2 CPUFUNC(op_08a0_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - areg_byteinc[dstreg];
	do_cycles_ce (2 * 256);
{	uae_s8 dst = get_byte_ce (dsta);
	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst &= ~(1 << src);
	put_byte_ce (dsta,dst);
}}}}	m68k_incpc (4);
}
/* BCLR.B #<data>.W,(d16,An) */
void REGPARAM2 CPUFUNC(op_08a8_6)(uae_u32 opcode)
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
}
/* BCLR.B #<data>.W,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_08b0_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta;
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (6));
	do_cycles_ce (2 * 256);
{	uae_s8 dst = get_byte_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst &= ~(1 << src);
	put_byte_ce (dsta,dst);
}}}}	m68k_incpc (6);
}
/* BCLR.B #<data>.W,(xxx).W */
void REGPARAM2 CPUFUNC(op_08b8_6)(uae_u32 opcode)
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
}
/* BCLR.B #<data>.W,(xxx).L */
void REGPARAM2 CPUFUNC(op_08b9_6)(uae_u32 opcode)
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
}
/* BCLR.B #<data>.W,(d16,PC) */
void REGPARAM2 CPUFUNC(op_08ba_6)(uae_u32 opcode)
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
}
/* BCLR.B #<data>.W,(d8,PC,Xn) */
void REGPARAM2 CPUFUNC(op_08bb_6)(uae_u32 opcode)
{
	uae_u32 dstreg = 3;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr tmppc;
	uaecptr dsta;
	tmppc = m68k_getpc() + 4;
	dsta = get_disp_ea_000(tmppc, get_word_ce_prefetch (6));
	do_cycles_ce (2 * 256);
{	uae_s8 dst = get_byte_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst &= ~(1 << src);
	put_byte_ce (dsta,dst);
}}}}	m68k_incpc (6);
}
/* BSET.L #<data>.W,Dn */
void REGPARAM2 CPUFUNC(op_08c0_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	do_cycles_ce (4 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	src &= 31;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst |= (1 << src);
	m68k_dreg(regs, dstreg) = (dst);
}}}	m68k_incpc (4);
}
/* BSET.B #<data>.W,(An) */
void REGPARAM2 CPUFUNC(op_08d0_6)(uae_u32 opcode)
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
}
/* BSET.B #<data>.W,(An)+ */
void REGPARAM2 CPUFUNC(op_08d8_6)(uae_u32 opcode)
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
}
/* BSET.B #<data>.W,-(An) */
void REGPARAM2 CPUFUNC(op_08e0_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - areg_byteinc[dstreg];
	do_cycles_ce (2 * 256);
{	uae_s8 dst = get_byte_ce (dsta);
	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst |= (1 << src);
	put_byte_ce (dsta,dst);
}}}}	m68k_incpc (4);
}
/* BSET.B #<data>.W,(d16,An) */
void REGPARAM2 CPUFUNC(op_08e8_6)(uae_u32 opcode)
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
}
/* BSET.B #<data>.W,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_08f0_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta;
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (6));
	do_cycles_ce (2 * 256);
{	uae_s8 dst = get_byte_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst |= (1 << src);
	put_byte_ce (dsta,dst);
}}}}	m68k_incpc (6);
}
/* BSET.B #<data>.W,(xxx).W */
void REGPARAM2 CPUFUNC(op_08f8_6)(uae_u32 opcode)
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
}
/* BSET.B #<data>.W,(xxx).L */
void REGPARAM2 CPUFUNC(op_08f9_6)(uae_u32 opcode)
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
}
/* BSET.B #<data>.W,(d16,PC) */
void REGPARAM2 CPUFUNC(op_08fa_6)(uae_u32 opcode)
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
}
/* BSET.B #<data>.W,(d8,PC,Xn) */
void REGPARAM2 CPUFUNC(op_08fb_6)(uae_u32 opcode)
{
	uae_u32 dstreg = 3;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr tmppc;
	uaecptr dsta;
	tmppc = m68k_getpc() + 4;
	dsta = get_disp_ea_000(tmppc, get_word_ce_prefetch (6));
	do_cycles_ce (2 * 256);
{	uae_s8 dst = get_byte_ce (dsta);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst |= (1 << src);
	put_byte_ce (dsta,dst);
}}}}	m68k_incpc (6);
}
/* EOR.B #<data>.B,Dn */
void REGPARAM2 CPUFUNC(op_0a00_6)(uae_u32 opcode)
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
}
/* EOR.B #<data>.B,(An) */
void REGPARAM2 CPUFUNC(op_0a10_6)(uae_u32 opcode)
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
}
/* EOR.B #<data>.B,(An)+ */
void REGPARAM2 CPUFUNC(op_0a18_6)(uae_u32 opcode)
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
}
/* EOR.B #<data>.B,-(An) */
void REGPARAM2 CPUFUNC(op_0a20_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce_prefetch (4);
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - areg_byteinc[dstreg];
	do_cycles_ce (2 * 256);
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
}
/* EOR.B #<data>.B,(d16,An) */
void REGPARAM2 CPUFUNC(op_0a28_6)(uae_u32 opcode)
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
}
/* EOR.B #<data>.B,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_0a30_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce_prefetch (4);
{	uaecptr dsta;
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (6));
	do_cycles_ce (2 * 256);
{	uae_s8 dst = get_byte_ce (dsta);
	src ^= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	put_byte_ce (dsta,src);
}}}}	m68k_incpc (6);
}
/* EOR.B #<data>.B,(xxx).W */
void REGPARAM2 CPUFUNC(op_0a38_6)(uae_u32 opcode)
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
}
/* EOR.B #<data>.B,(xxx).L */
void REGPARAM2 CPUFUNC(op_0a39_6)(uae_u32 opcode)
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
}
/* EORSR.B #<data>.W */
void REGPARAM2 CPUFUNC(op_0a3c_6)(uae_u32 opcode)
{
{	MakeSR();
{	uae_s16 src = get_word_ce_prefetch (4);
	src &= 0xFF;
	do_cycles_ce (4 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	regs.sr ^= src;
	MakeFromSR();
}}	m68k_incpc (4);
}
/* EOR.W #<data>.W,Dn */
void REGPARAM2 CPUFUNC(op_0a40_6)(uae_u32 opcode)
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
}
/* EOR.W #<data>.W,(An) */
void REGPARAM2 CPUFUNC(op_0a50_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel3905;
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
endlabel3905: ;
}
/* EOR.W #<data>.W,(An)+ */
void REGPARAM2 CPUFUNC(op_0a58_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel3906;
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
endlabel3906: ;
}
/* EOR.W #<data>.W,-(An) */
void REGPARAM2 CPUFUNC(op_0a60_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - 2;
	do_cycles_ce (2 * 256);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel3907;
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
endlabel3907: ;
}
/* EOR.W #<data>.W,(d16,An) */
void REGPARAM2 CPUFUNC(op_0a68_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (6);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel3908;
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
endlabel3908: ;
}
/* EOR.W #<data>.W,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_0a70_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta;
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (6));
	do_cycles_ce (2 * 256);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel3909;
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
endlabel3909: ;
}
/* EOR.W #<data>.W,(xxx).W */
void REGPARAM2 CPUFUNC(op_0a78_6)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (6);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel3910;
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
endlabel3910: ;
}
/* EOR.W #<data>.W,(xxx).L */
void REGPARAM2 CPUFUNC(op_0a79_6)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (6) << 16;
	dsta |= get_word_ce_prefetch (8);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel3911;
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
endlabel3911: ;
}
#endif

#ifdef PART_2
/* EORSR.W #<data>.W */
void REGPARAM2 CPUFUNC(op_0a7c_6)(uae_u32 opcode)
{
{if (!regs.s) { Exception(8,0); goto endlabel3912; }
{	MakeSR();
{	uae_s16 src = get_word_ce_prefetch (4);
	do_cycles_ce (4 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	regs.sr ^= src;
	MakeFromSR();
}}}	m68k_incpc (4);
endlabel3912: ;
}
/* EOR.L #<data>.L,Dn */
void REGPARAM2 CPUFUNC(op_0a80_6)(uae_u32 opcode)
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
	do_cycles_ce (4 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	m68k_dreg(regs, dstreg) = (src);
}}}	m68k_incpc (6);
}
/* EOR.L #<data>.L,(An) */
void REGPARAM2 CPUFUNC(op_0a90_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel3914;
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
endlabel3914: ;
}
/* EOR.L #<data>.L,(An)+ */
void REGPARAM2 CPUFUNC(op_0a98_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel3915;
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
endlabel3915: ;
}
/* EOR.L #<data>.L,-(An) */
void REGPARAM2 CPUFUNC(op_0aa0_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - 4;
	do_cycles_ce (2 * 256);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel3916;
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
endlabel3916: ;
}
/* EOR.L #<data>.L,(d16,An) */
void REGPARAM2 CPUFUNC(op_0aa8_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (8);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel3917;
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
endlabel3917: ;
}
/* EOR.L #<data>.L,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_0ab0_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uaecptr dsta;
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (8));
	do_cycles_ce (2 * 256);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel3918;
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
endlabel3918: ;
}
/* EOR.L #<data>.L,(xxx).W */
void REGPARAM2 CPUFUNC(op_0ab8_6)(uae_u32 opcode)
{
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (8);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel3919;
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
endlabel3919: ;
}
/* EOR.L #<data>.L,(xxx).L */
void REGPARAM2 CPUFUNC(op_0ab9_6)(uae_u32 opcode)
{
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (8) << 16;
	dsta |= get_word_ce_prefetch (10);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel3920;
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
endlabel3920: ;
}
/* CMP.B #<data>.B,Dn */
void REGPARAM2 CPUFUNC(op_0c00_6)(uae_u32 opcode)
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
}
/* CMP.B #<data>.B,(An) */
void REGPARAM2 CPUFUNC(op_0c10_6)(uae_u32 opcode)
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
}
/* CMP.B #<data>.B,(An)+ */
void REGPARAM2 CPUFUNC(op_0c18_6)(uae_u32 opcode)
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
}
/* CMP.B #<data>.B,-(An) */
void REGPARAM2 CPUFUNC(op_0c20_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce_prefetch (4);
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - areg_byteinc[dstreg];
	do_cycles_ce (2 * 256);
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
}
/* CMP.B #<data>.B,(d16,An) */
void REGPARAM2 CPUFUNC(op_0c28_6)(uae_u32 opcode)
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
}
/* CMP.B #<data>.B,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_0c30_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce_prefetch (4);
{	uaecptr dsta;
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (6));
	do_cycles_ce (2 * 256);
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
}
/* CMP.B #<data>.B,(xxx).W */
void REGPARAM2 CPUFUNC(op_0c38_6)(uae_u32 opcode)
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
}
/* CMP.B #<data>.B,(xxx).L */
void REGPARAM2 CPUFUNC(op_0c39_6)(uae_u32 opcode)
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
}
/* CMP.W #<data>.W,Dn */
void REGPARAM2 CPUFUNC(op_0c40_6)(uae_u32 opcode)
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
}
/* CMP.W #<data>.W,(An) */
void REGPARAM2 CPUFUNC(op_0c50_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel3930;
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
endlabel3930: ;
}
/* CMP.W #<data>.W,(An)+ */
void REGPARAM2 CPUFUNC(op_0c58_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel3931;
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
endlabel3931: ;
}
/* CMP.W #<data>.W,-(An) */
void REGPARAM2 CPUFUNC(op_0c60_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - 2;
	do_cycles_ce (2 * 256);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel3932;
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
endlabel3932: ;
}
/* CMP.W #<data>.W,(d16,An) */
void REGPARAM2 CPUFUNC(op_0c68_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (6);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel3933;
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
endlabel3933: ;
}
/* CMP.W #<data>.W,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_0c70_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta;
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (6));
	do_cycles_ce (2 * 256);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel3934;
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
endlabel3934: ;
}
/* CMP.W #<data>.W,(xxx).W */
void REGPARAM2 CPUFUNC(op_0c78_6)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (6);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel3935;
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
endlabel3935: ;
}
/* CMP.W #<data>.W,(xxx).L */
void REGPARAM2 CPUFUNC(op_0c79_6)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (6) << 16;
	dsta |= get_word_ce_prefetch (8);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel3936;
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
endlabel3936: ;
}
/* CMP.L #<data>.L,Dn */
void REGPARAM2 CPUFUNC(op_0c80_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	do_cycles_ce (2 * 256);
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
}}}}}}	m68k_incpc (6);
}
/* CMP.L #<data>.L,(An) */
void REGPARAM2 CPUFUNC(op_0c90_6)(uae_u32 opcode)
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
endlabel3938: ;
}
/* CMP.L #<data>.L,(An)+ */
void REGPARAM2 CPUFUNC(op_0c98_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel3939;
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
endlabel3939: ;
}
/* CMP.L #<data>.L,-(An) */
void REGPARAM2 CPUFUNC(op_0ca0_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - 4;
	do_cycles_ce (2 * 256);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel3940;
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
endlabel3940: ;
}
/* CMP.L #<data>.L,(d16,An) */
void REGPARAM2 CPUFUNC(op_0ca8_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (8);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel3941;
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
endlabel3941: ;
}
/* CMP.L #<data>.L,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_0cb0_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uaecptr dsta;
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (8));
	do_cycles_ce (2 * 256);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel3942;
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
endlabel3942: ;
}
/* CMP.L #<data>.L,(xxx).W */
void REGPARAM2 CPUFUNC(op_0cb8_6)(uae_u32 opcode)
{
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (8);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel3943;
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
endlabel3943: ;
}
/* CMP.L #<data>.L,(xxx).L */
void REGPARAM2 CPUFUNC(op_0cb9_6)(uae_u32 opcode)
{
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (8) << 16;
	dsta |= get_word_ce_prefetch (10);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel3944;
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
endlabel3944: ;
}
/* MOVE.B Dn,Dn */
void REGPARAM2 CPUFUNC(op_1000_6)(uae_u32 opcode)
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
}}}}
/* MOVE.B (An),Dn */
void REGPARAM2 CPUFUNC(op_1010_6)(uae_u32 opcode)
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
}}}}}
/* MOVE.B (An)+,Dn */
void REGPARAM2 CPUFUNC(op_1018_6)(uae_u32 opcode)
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
}}}}}
/* MOVE.B -(An),Dn */
void REGPARAM2 CPUFUNC(op_1020_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
	do_cycles_ce (2 * 256);
{	uae_s8 src = get_byte_ce (srca);
	m68k_areg (regs, srcreg) = srca;
{	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xff) | ((src) & 0xff);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}
/* MOVE.B (d16,An),Dn */
void REGPARAM2 CPUFUNC(op_1028_6)(uae_u32 opcode)
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
}}}}}
/* MOVE.B (d8,An,Xn),Dn */
void REGPARAM2 CPUFUNC(op_1030_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
{	uae_s8 src = get_byte_ce (srca);
{	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xff) | ((src) & 0xff);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}
/* MOVE.B (xxx).W,Dn */
void REGPARAM2 CPUFUNC(op_1038_6)(uae_u32 opcode)
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
}}}}}
/* MOVE.B (xxx).L,Dn */
void REGPARAM2 CPUFUNC(op_1039_6)(uae_u32 opcode)
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
}}}}}
/* MOVE.B (d16,PC),Dn */
void REGPARAM2 CPUFUNC(op_103a_6)(uae_u32 opcode)
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
}}}}}
/* MOVE.B (d8,PC,Xn),Dn */
void REGPARAM2 CPUFUNC(op_103b_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
{	uae_s8 src = get_byte_ce (srca);
{	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xff) | ((src) & 0xff);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}
/* MOVE.B #<data>.B,Dn */
void REGPARAM2 CPUFUNC(op_103c_6)(uae_u32 opcode)
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
}}}}
/* MOVE.B Dn,(An) */
void REGPARAM2 CPUFUNC(op_1080_6)(uae_u32 opcode)
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
}}}}
/* MOVE.B (An),(An) */
void REGPARAM2 CPUFUNC(op_1090_6)(uae_u32 opcode)
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
}}}}}
/* MOVE.B (An)+,(An) */
void REGPARAM2 CPUFUNC(op_1098_6)(uae_u32 opcode)
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
}}}}}
/* MOVE.B -(An),(An) */
void REGPARAM2 CPUFUNC(op_10a0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
	do_cycles_ce (2 * 256);
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
}}}}}
/* MOVE.B (d16,An),(An) */
void REGPARAM2 CPUFUNC(op_10a8_6)(uae_u32 opcode)
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
}}}}}
/* MOVE.B (d8,An,Xn),(An) */
void REGPARAM2 CPUFUNC(op_10b0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
{	uae_s8 src = get_byte_ce (srca);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	put_byte_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}
/* MOVE.B (xxx).W,(An) */
void REGPARAM2 CPUFUNC(op_10b8_6)(uae_u32 opcode)
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
}}}}}
/* MOVE.B (xxx).L,(An) */
void REGPARAM2 CPUFUNC(op_10b9_6)(uae_u32 opcode)
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
}}}}}
/* MOVE.B (d16,PC),(An) */
void REGPARAM2 CPUFUNC(op_10ba_6)(uae_u32 opcode)
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
}}}}}
/* MOVE.B (d8,PC,Xn),(An) */
void REGPARAM2 CPUFUNC(op_10bb_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
{	uae_s8 src = get_byte_ce (srca);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	put_byte_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}
/* MOVE.B #<data>.B,(An) */
void REGPARAM2 CPUFUNC(op_10bc_6)(uae_u32 opcode)
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
}}}}
/* MOVE.B Dn,(An)+ */
void REGPARAM2 CPUFUNC(op_10c0_6)(uae_u32 opcode)
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
}}}}
/* MOVE.B (An),(An)+ */
void REGPARAM2 CPUFUNC(op_10d0_6)(uae_u32 opcode)
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
}}}}}
/* MOVE.B (An)+,(An)+ */
void REGPARAM2 CPUFUNC(op_10d8_6)(uae_u32 opcode)
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
}}}}}
/* MOVE.B -(An),(An)+ */
void REGPARAM2 CPUFUNC(op_10e0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
	do_cycles_ce (2 * 256);
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
}}}}}
/* MOVE.B (d16,An),(An)+ */
void REGPARAM2 CPUFUNC(op_10e8_6)(uae_u32 opcode)
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
}}}}}
/* MOVE.B (d8,An,Xn),(An)+ */
void REGPARAM2 CPUFUNC(op_10f0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
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
}}}}}
/* MOVE.B (xxx).W,(An)+ */
void REGPARAM2 CPUFUNC(op_10f8_6)(uae_u32 opcode)
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
}}}}}
/* MOVE.B (xxx).L,(An)+ */
void REGPARAM2 CPUFUNC(op_10f9_6)(uae_u32 opcode)
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
}}}}}
/* MOVE.B (d16,PC),(An)+ */
void REGPARAM2 CPUFUNC(op_10fa_6)(uae_u32 opcode)
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
}}}}}
/* MOVE.B (d8,PC,Xn),(An)+ */
void REGPARAM2 CPUFUNC(op_10fb_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
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
}}}}}
/* MOVE.B #<data>.B,(An)+ */
void REGPARAM2 CPUFUNC(op_10fc_6)(uae_u32 opcode)
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
}}}}
/* MOVE.B Dn,-(An) */
void REGPARAM2 CPUFUNC(op_1100_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - areg_byteinc[dstreg];
	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	put_byte_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (2);
}}}}
/* MOVE.B (An),-(An) */
void REGPARAM2 CPUFUNC(op_1110_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
{	uae_s8 src = get_byte_ce (srca);
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - areg_byteinc[dstreg];
	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	put_byte_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (2);
}}}}}
/* MOVE.B (An)+,-(An) */
void REGPARAM2 CPUFUNC(op_1118_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
{	uae_s8 src = get_byte_ce (srca);
	m68k_areg(regs, srcreg) += areg_byteinc[srcreg];
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - areg_byteinc[dstreg];
	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	put_byte_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (2);
}}}}}
/* MOVE.B -(An),-(An) */
void REGPARAM2 CPUFUNC(op_1120_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
	do_cycles_ce (2 * 256);
{	uae_s8 src = get_byte_ce (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - areg_byteinc[dstreg];
	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	put_byte_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (2);
}}}}}
/* MOVE.B (d16,An),-(An) */
void REGPARAM2 CPUFUNC(op_1128_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 src = get_byte_ce (srca);
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - areg_byteinc[dstreg];
	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	put_byte_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (4);
}}}}}
/* MOVE.B (d8,An,Xn),-(An) */
void REGPARAM2 CPUFUNC(op_1130_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
{	uae_s8 src = get_byte_ce (srca);
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - areg_byteinc[dstreg];
	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	put_byte_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (4);
}}}}}
/* MOVE.B (xxx).W,-(An) */
void REGPARAM2 CPUFUNC(op_1138_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 src = get_byte_ce (srca);
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - areg_byteinc[dstreg];
	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	put_byte_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (4);
}}}}}
/* MOVE.B (xxx).L,-(An) */
void REGPARAM2 CPUFUNC(op_1139_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
{	uae_s8 src = get_byte_ce (srca);
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - areg_byteinc[dstreg];
	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	put_byte_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (6);
}}}}}
/* MOVE.B (d16,PC),-(An) */
void REGPARAM2 CPUFUNC(op_113a_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 src = get_byte_ce (srca);
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - areg_byteinc[dstreg];
	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	put_byte_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (4);
}}}}}
/* MOVE.B (d8,PC,Xn),-(An) */
void REGPARAM2 CPUFUNC(op_113b_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
{	uae_s8 src = get_byte_ce (srca);
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - areg_byteinc[dstreg];
	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	put_byte_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (4);
}}}}}
/* MOVE.B #<data>.B,-(An) */
void REGPARAM2 CPUFUNC(op_113c_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s8 src = (uae_u8)get_word_ce_prefetch (4);
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - areg_byteinc[dstreg];
	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	put_byte_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (4);
}}}}
/* MOVE.B Dn,(d16,An) */
void REGPARAM2 CPUFUNC(op_1140_6)(uae_u32 opcode)
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
}}}}
/* MOVE.B (An),(d16,An) */
void REGPARAM2 CPUFUNC(op_1150_6)(uae_u32 opcode)
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
}}}}}
/* MOVE.B (An)+,(d16,An) */
void REGPARAM2 CPUFUNC(op_1158_6)(uae_u32 opcode)
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
}}}}}
/* MOVE.B -(An),(d16,An) */
void REGPARAM2 CPUFUNC(op_1160_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
	do_cycles_ce (2 * 256);
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
}}}}}
/* MOVE.B (d16,An),(d16,An) */
void REGPARAM2 CPUFUNC(op_1168_6)(uae_u32 opcode)
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
}}}}}
/* MOVE.B (d8,An,Xn),(d16,An) */
void REGPARAM2 CPUFUNC(op_1170_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
{	uae_s8 src = get_byte_ce (srca);
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (6);
	put_byte_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}
/* MOVE.B (xxx).W,(d16,An) */
void REGPARAM2 CPUFUNC(op_1178_6)(uae_u32 opcode)
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
}}}}}
/* MOVE.B (xxx).L,(d16,An) */
void REGPARAM2 CPUFUNC(op_1179_6)(uae_u32 opcode)
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
}}}}}
/* MOVE.B (d16,PC),(d16,An) */
void REGPARAM2 CPUFUNC(op_117a_6)(uae_u32 opcode)
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
}}}}}
/* MOVE.B (d8,PC,Xn),(d16,An) */
void REGPARAM2 CPUFUNC(op_117b_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
{	uae_s8 src = get_byte_ce (srca);
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (6);
	put_byte_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}
/* MOVE.B #<data>.B,(d16,An) */
void REGPARAM2 CPUFUNC(op_117c_6)(uae_u32 opcode)
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
}}}}
/* MOVE.B Dn,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_1180_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	put_byte_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}
/* MOVE.B (An),(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_1190_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
{	uae_s8 src = get_byte_ce (srca);
{	uaecptr dsta;
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	put_byte_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}
/* MOVE.B (An)+,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_1198_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
{	uae_s8 src = get_byte_ce (srca);
	m68k_areg(regs, srcreg) += areg_byteinc[srcreg];
{	uaecptr dsta;
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	put_byte_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}
/* MOVE.B -(An),(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_11a0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
	do_cycles_ce (2 * 256);
{	uae_s8 src = get_byte_ce (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta;
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	put_byte_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}
/* MOVE.B (d16,An),(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_11a8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 src = get_byte_ce (srca);
{	uaecptr dsta;
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (6));
	do_cycles_ce (2 * 256);
	put_byte_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}
/* MOVE.B (d8,An,Xn),(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_11b0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
{	uae_s8 src = get_byte_ce (srca);
{	uaecptr dsta;
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (6));
	do_cycles_ce (2 * 256);
	put_byte_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}
/* MOVE.B (xxx).W,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_11b8_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 src = get_byte_ce (srca);
{	uaecptr dsta;
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (6));
	do_cycles_ce (2 * 256);
	put_byte_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}
/* MOVE.B (xxx).L,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_11b9_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
{	uae_s8 src = get_byte_ce (srca);
{	uaecptr dsta;
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (8));
	do_cycles_ce (2 * 256);
	put_byte_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (8);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}
/* MOVE.B (d16,PC),(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_11ba_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 src = get_byte_ce (srca);
{	uaecptr dsta;
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (6));
	do_cycles_ce (2 * 256);
	put_byte_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}
/* MOVE.B (d8,PC,Xn),(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_11bb_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
{	uae_s8 src = get_byte_ce (srca);
{	uaecptr dsta;
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (6));
	do_cycles_ce (2 * 256);
	put_byte_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}
/* MOVE.B #<data>.B,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_11bc_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s8 src = (uae_u8)get_word_ce_prefetch (4);
{	uaecptr dsta;
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (6));
	do_cycles_ce (2 * 256);
	put_byte_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}
/* MOVE.B Dn,(xxx).W */
void REGPARAM2 CPUFUNC(op_11c0_6)(uae_u32 opcode)
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
}}}}
/* MOVE.B (An),(xxx).W */
void REGPARAM2 CPUFUNC(op_11d0_6)(uae_u32 opcode)
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
}}}}}
/* MOVE.B (An)+,(xxx).W */
void REGPARAM2 CPUFUNC(op_11d8_6)(uae_u32 opcode)
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
}}}}}
/* MOVE.B -(An),(xxx).W */
void REGPARAM2 CPUFUNC(op_11e0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
	do_cycles_ce (2 * 256);
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
}}}}}
/* MOVE.B (d16,An),(xxx).W */
void REGPARAM2 CPUFUNC(op_11e8_6)(uae_u32 opcode)
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
}}}}}
/* MOVE.B (d8,An,Xn),(xxx).W */
void REGPARAM2 CPUFUNC(op_11f0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
{	uae_s8 src = get_byte_ce (srca);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (6);
	put_byte_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}
/* MOVE.B (xxx).W,(xxx).W */
void REGPARAM2 CPUFUNC(op_11f8_6)(uae_u32 opcode)
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
}}}}}
/* MOVE.B (xxx).L,(xxx).W */
void REGPARAM2 CPUFUNC(op_11f9_6)(uae_u32 opcode)
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
}}}}}
/* MOVE.B (d16,PC),(xxx).W */
void REGPARAM2 CPUFUNC(op_11fa_6)(uae_u32 opcode)
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
}}}}}
/* MOVE.B (d8,PC,Xn),(xxx).W */
void REGPARAM2 CPUFUNC(op_11fb_6)(uae_u32 opcode)
{
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
{	uae_s8 src = get_byte_ce (srca);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (6);
	put_byte_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}
/* MOVE.B #<data>.B,(xxx).W */
void REGPARAM2 CPUFUNC(op_11fc_6)(uae_u32 opcode)
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
}}}}
/* MOVE.B Dn,(xxx).L */
void REGPARAM2 CPUFUNC(op_13c0_6)(uae_u32 opcode)
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
}}}}
/* MOVE.B (An),(xxx).L */
void REGPARAM2 CPUFUNC(op_13d0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
{	uae_s8 src = get_byte_ce (srca);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (4) << 16;
	dsta |= regs.irc;
	put_byte_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (6);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}
/* MOVE.B (An)+,(xxx).L */
void REGPARAM2 CPUFUNC(op_13d8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
{	uae_s8 src = get_byte_ce (srca);
	m68k_areg(regs, srcreg) += areg_byteinc[srcreg];
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (4) << 16;
	dsta |= regs.irc;
	put_byte_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (6);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}
/* MOVE.B -(An),(xxx).L */
void REGPARAM2 CPUFUNC(op_13e0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
	do_cycles_ce (2 * 256);
{	uae_s8 src = get_byte_ce (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (4) << 16;
	dsta |= regs.irc;
	put_byte_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (6);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}
/* MOVE.B (d16,An),(xxx).L */
void REGPARAM2 CPUFUNC(op_13e8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 src = get_byte_ce (srca);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (6) << 16;
	dsta |= regs.irc;
	put_byte_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (8);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}
/* MOVE.B (d8,An,Xn),(xxx).L */
void REGPARAM2 CPUFUNC(op_13f0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
{	uae_s8 src = get_byte_ce (srca);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (6) << 16;
	dsta |= regs.irc;
	put_byte_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (8);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}
/* MOVE.B (xxx).W,(xxx).L */
void REGPARAM2 CPUFUNC(op_13f8_6)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 src = get_byte_ce (srca);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (6) << 16;
	dsta |= regs.irc;
	put_byte_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (8);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}
/* MOVE.B (xxx).L,(xxx).L */
void REGPARAM2 CPUFUNC(op_13f9_6)(uae_u32 opcode)
{
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
{	uae_s8 src = get_byte_ce (srca);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (8) << 16;
	dsta |= regs.irc;
	put_byte_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (10);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}
/* MOVE.B (d16,PC),(xxx).L */
void REGPARAM2 CPUFUNC(op_13fa_6)(uae_u32 opcode)
{
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 src = get_byte_ce (srca);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (6) << 16;
	dsta |= regs.irc;
	put_byte_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (8);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}
/* MOVE.B (d8,PC,Xn),(xxx).L */
void REGPARAM2 CPUFUNC(op_13fb_6)(uae_u32 opcode)
{
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
{	uae_s8 src = get_byte_ce (srca);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (6) << 16;
	dsta |= regs.irc;
	put_byte_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	m68k_incpc (8);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}
/* MOVE.B #<data>.B,(xxx).L */
void REGPARAM2 CPUFUNC(op_13fc_6)(uae_u32 opcode)
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
}}}}
/* MOVE.L Dn,Dn */
void REGPARAM2 CPUFUNC(op_2000_6)(uae_u32 opcode)
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
}}}}
/* MOVE.L An,Dn */
void REGPARAM2 CPUFUNC(op_2008_6)(uae_u32 opcode)
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
}}}}
/* MOVE.L (An),Dn */
void REGPARAM2 CPUFUNC(op_2010_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4035;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	m68k_dreg(regs, dstreg) = (src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}endlabel4035: ;
}
/* MOVE.L (An)+,Dn */
void REGPARAM2 CPUFUNC(op_2018_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4036;
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
}}}}}endlabel4036: ;
}
/* MOVE.L -(An),Dn */
void REGPARAM2 CPUFUNC(op_2020_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg(regs, srcreg) - 4;
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4037;
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
}}}}}endlabel4037: ;
}
/* MOVE.L (d16,An),Dn */
void REGPARAM2 CPUFUNC(op_2028_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4038;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	m68k_dreg(regs, dstreg) = (src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}endlabel4038: ;
}
/* MOVE.L (d8,An,Xn),Dn */
void REGPARAM2 CPUFUNC(op_2030_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4039;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	m68k_dreg(regs, dstreg) = (src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}endlabel4039: ;
}
/* MOVE.L (xxx).W,Dn */
void REGPARAM2 CPUFUNC(op_2038_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4040;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	m68k_dreg(regs, dstreg) = (src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}endlabel4040: ;
}
/* MOVE.L (xxx).L,Dn */
void REGPARAM2 CPUFUNC(op_2039_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4041;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	m68k_dreg(regs, dstreg) = (src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}endlabel4041: ;
}
/* MOVE.L (d16,PC),Dn */
void REGPARAM2 CPUFUNC(op_203a_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4042;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	m68k_dreg(regs, dstreg) = (src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}endlabel4042: ;
}
/* MOVE.L (d8,PC,Xn),Dn */
void REGPARAM2 CPUFUNC(op_203b_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4043;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	m68k_dreg(regs, dstreg) = (src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}endlabel4043: ;
}
/* MOVE.L #<data>.L,Dn */
void REGPARAM2 CPUFUNC(op_203c_6)(uae_u32 opcode)
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
}}}}
/* MOVEA.L Dn,An */
void REGPARAM2 CPUFUNC(op_2040_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	m68k_areg(regs, dstreg) = (src);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}
/* MOVEA.L An,An */
void REGPARAM2 CPUFUNC(op_2048_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_areg(regs, srcreg);
{	m68k_areg(regs, dstreg) = (src);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}
/* MOVEA.L (An),An */
void REGPARAM2 CPUFUNC(op_2050_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4047;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	m68k_areg(regs, dstreg) = (src);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}endlabel4047: ;
}
/* MOVEA.L (An)+,An */
void REGPARAM2 CPUFUNC(op_2058_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4048;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	m68k_areg(regs, srcreg) += 4;
{	m68k_areg(regs, dstreg) = (src);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}endlabel4048: ;
}
/* MOVEA.L -(An),An */
void REGPARAM2 CPUFUNC(op_2060_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg(regs, srcreg) - 4;
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4049;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	m68k_areg (regs, srcreg) = srca;
{	m68k_areg(regs, dstreg) = (src);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}endlabel4049: ;
}
/* MOVEA.L (d16,An),An */
void REGPARAM2 CPUFUNC(op_2068_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4050;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	m68k_areg(regs, dstreg) = (src);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}endlabel4050: ;
}
/* MOVEA.L (d8,An,Xn),An */
void REGPARAM2 CPUFUNC(op_2070_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4051;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	m68k_areg(regs, dstreg) = (src);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}endlabel4051: ;
}
/* MOVEA.L (xxx).W,An */
void REGPARAM2 CPUFUNC(op_2078_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4052;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	m68k_areg(regs, dstreg) = (src);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}endlabel4052: ;
}
/* MOVEA.L (xxx).L,An */
void REGPARAM2 CPUFUNC(op_2079_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4053;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	m68k_areg(regs, dstreg) = (src);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}endlabel4053: ;
}
/* MOVEA.L (d16,PC),An */
void REGPARAM2 CPUFUNC(op_207a_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4054;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	m68k_areg(regs, dstreg) = (src);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}endlabel4054: ;
}
/* MOVEA.L (d8,PC,Xn),An */
void REGPARAM2 CPUFUNC(op_207b_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4055;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	m68k_areg(regs, dstreg) = (src);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}endlabel4055: ;
}
/* MOVEA.L #<data>.L,An */
void REGPARAM2 CPUFUNC(op_207c_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	m68k_areg(regs, dstreg) = (src);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}
/* MOVE.L Dn,(An) */
void REGPARAM2 CPUFUNC(op_2080_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4057;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}endlabel4057: ;
}
/* MOVE.L An,(An) */
void REGPARAM2 CPUFUNC(op_2088_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_areg(regs, srcreg);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4058;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}endlabel4058: ;
}
/* MOVE.L (An),(An) */
void REGPARAM2 CPUFUNC(op_2090_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4059;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4059;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4059: ;
}
/* MOVE.L (An)+,(An) */
void REGPARAM2 CPUFUNC(op_2098_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4060;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	m68k_areg(regs, srcreg) += 4;
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4060;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4060: ;
}
/* MOVE.L -(An),(An) */
void REGPARAM2 CPUFUNC(op_20a0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg(regs, srcreg) - 4;
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4061;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4061;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4061: ;
}
/* MOVE.L (d16,An),(An) */
void REGPARAM2 CPUFUNC(op_20a8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4062;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4062;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4062: ;
}
/* MOVE.L (d8,An,Xn),(An) */
void REGPARAM2 CPUFUNC(op_20b0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4063;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4063;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4063: ;
}
/* MOVE.L (xxx).W,(An) */
void REGPARAM2 CPUFUNC(op_20b8_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4064;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4064;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4064: ;
}
/* MOVE.L (xxx).L,(An) */
void REGPARAM2 CPUFUNC(op_20b9_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4065;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel4065;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4065: ;
}
/* MOVE.L (d16,PC),(An) */
void REGPARAM2 CPUFUNC(op_20ba_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4066;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4066;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4066: ;
}
/* MOVE.L (d8,PC,Xn),(An) */
void REGPARAM2 CPUFUNC(op_20bb_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4067;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4067;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4067: ;
}
/* MOVE.L #<data>.L,(An) */
void REGPARAM2 CPUFUNC(op_20bc_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel4068;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}endlabel4068: ;
}
/* MOVE.L Dn,(An)+ */
void REGPARAM2 CPUFUNC(op_20c0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4069;
	}
{	m68k_areg(regs, dstreg) += 4;
	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}endlabel4069: ;
}
/* MOVE.L An,(An)+ */
void REGPARAM2 CPUFUNC(op_20c8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_areg(regs, srcreg);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4070;
	}
{	m68k_areg(regs, dstreg) += 4;
	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}endlabel4070: ;
}
/* MOVE.L (An),(An)+ */
void REGPARAM2 CPUFUNC(op_20d0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4071;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4071;
	}
{	m68k_areg(regs, dstreg) += 4;
	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4071: ;
}
/* MOVE.L (An)+,(An)+ */
void REGPARAM2 CPUFUNC(op_20d8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4072;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	m68k_areg(regs, srcreg) += 4;
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4072;
	}
{	m68k_areg(regs, dstreg) += 4;
	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4072: ;
}
/* MOVE.L -(An),(An)+ */
void REGPARAM2 CPUFUNC(op_20e0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg(regs, srcreg) - 4;
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4073;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4073;
	}
{	m68k_areg(regs, dstreg) += 4;
	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4073: ;
}
/* MOVE.L (d16,An),(An)+ */
void REGPARAM2 CPUFUNC(op_20e8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4074;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4074;
	}
{	m68k_areg(regs, dstreg) += 4;
	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4074: ;
}
/* MOVE.L (d8,An,Xn),(An)+ */
void REGPARAM2 CPUFUNC(op_20f0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4075;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4075;
	}
{	m68k_areg(regs, dstreg) += 4;
	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4075: ;
}
/* MOVE.L (xxx).W,(An)+ */
void REGPARAM2 CPUFUNC(op_20f8_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4076;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4076;
	}
{	m68k_areg(regs, dstreg) += 4;
	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4076: ;
}
/* MOVE.L (xxx).L,(An)+ */
void REGPARAM2 CPUFUNC(op_20f9_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4077;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel4077;
	}
{	m68k_areg(regs, dstreg) += 4;
	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4077: ;
}
/* MOVE.L (d16,PC),(An)+ */
void REGPARAM2 CPUFUNC(op_20fa_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4078;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4078;
	}
{	m68k_areg(regs, dstreg) += 4;
	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4078: ;
}
/* MOVE.L (d8,PC,Xn),(An)+ */
void REGPARAM2 CPUFUNC(op_20fb_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4079;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4079;
	}
{	m68k_areg(regs, dstreg) += 4;
	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4079: ;
}
/* MOVE.L #<data>.L,(An)+ */
void REGPARAM2 CPUFUNC(op_20fc_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel4080;
	}
{	m68k_areg(regs, dstreg) += 4;
	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}endlabel4080: ;
}
/* MOVE.L Dn,-(An) */
void REGPARAM2 CPUFUNC(op_2100_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - 4;
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4081;
	}
{	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (2);
}}}}endlabel4081: ;
}
/* MOVE.L An,-(An) */
void REGPARAM2 CPUFUNC(op_2108_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_areg(regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - 4;
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4082;
	}
{	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (2);
}}}}endlabel4082: ;
}
/* MOVE.L (An),-(An) */
void REGPARAM2 CPUFUNC(op_2110_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4083;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - 4;
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4083;
	}
{	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (2);
}}}}}}endlabel4083: ;
}
/* MOVE.L (An)+,-(An) */
void REGPARAM2 CPUFUNC(op_2118_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4084;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	m68k_areg(regs, srcreg) += 4;
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - 4;
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4084;
	}
{	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (2);
}}}}}}endlabel4084: ;
}
/* MOVE.L -(An),-(An) */
void REGPARAM2 CPUFUNC(op_2120_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg(regs, srcreg) - 4;
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4085;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - 4;
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4085;
	}
{	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (2);
}}}}}}endlabel4085: ;
}
/* MOVE.L (d16,An),-(An) */
void REGPARAM2 CPUFUNC(op_2128_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4086;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - 4;
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4086;
	}
{	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (4);
}}}}}}endlabel4086: ;
}
/* MOVE.L (d8,An,Xn),-(An) */
void REGPARAM2 CPUFUNC(op_2130_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4087;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - 4;
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4087;
	}
{	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (4);
}}}}}}endlabel4087: ;
}
/* MOVE.L (xxx).W,-(An) */
void REGPARAM2 CPUFUNC(op_2138_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4088;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - 4;
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4088;
	}
{	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (4);
}}}}}}endlabel4088: ;
}
/* MOVE.L (xxx).L,-(An) */
void REGPARAM2 CPUFUNC(op_2139_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4089;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - 4;
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel4089;
	}
{	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (6);
}}}}}}endlabel4089: ;
}
/* MOVE.L (d16,PC),-(An) */
void REGPARAM2 CPUFUNC(op_213a_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4090;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - 4;
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4090;
	}
{	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (4);
}}}}}}endlabel4090: ;
}
/* MOVE.L (d8,PC,Xn),-(An) */
void REGPARAM2 CPUFUNC(op_213b_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4091;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - 4;
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4091;
	}
{	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (4);
}}}}}}endlabel4091: ;
}
/* MOVE.L #<data>.L,-(An) */
void REGPARAM2 CPUFUNC(op_213c_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - 4;
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel4092;
	}
{	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (6);
}}}}endlabel4092: ;
}
/* MOVE.L Dn,(d16,An) */
void REGPARAM2 CPUFUNC(op_2140_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4093;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}endlabel4093: ;
}
/* MOVE.L An,(d16,An) */
void REGPARAM2 CPUFUNC(op_2148_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_areg(regs, srcreg);
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4094;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}endlabel4094: ;
}
#endif

#ifdef PART_3
/* MOVE.L (An),(d16,An) */
void REGPARAM2 CPUFUNC(op_2150_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4095;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
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
}
/* MOVE.L (An)+,(d16,An) */
void REGPARAM2 CPUFUNC(op_2158_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4096;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	m68k_areg(regs, srcreg) += 4;
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
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
}
/* MOVE.L -(An),(d16,An) */
void REGPARAM2 CPUFUNC(op_2160_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg(regs, srcreg) - 4;
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4097;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4097;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4097: ;
}
/* MOVE.L (d16,An),(d16,An) */
void REGPARAM2 CPUFUNC(op_2168_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4098;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (6);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4098;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4098: ;
}
/* MOVE.L (d8,An,Xn),(d16,An) */
void REGPARAM2 CPUFUNC(op_2170_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4099;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (6);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4099;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4099: ;
}
/* MOVE.L (xxx).W,(d16,An) */
void REGPARAM2 CPUFUNC(op_2178_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4100;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (6);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4100;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4100: ;
}
/* MOVE.L (xxx).L,(d16,An) */
void REGPARAM2 CPUFUNC(op_2179_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4101;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (8);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel4101;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (8);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4101: ;
}
/* MOVE.L (d16,PC),(d16,An) */
void REGPARAM2 CPUFUNC(op_217a_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4102;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (6);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4102;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4102: ;
}
/* MOVE.L (d8,PC,Xn),(d16,An) */
void REGPARAM2 CPUFUNC(op_217b_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4103;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (6);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4103;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4103: ;
}
/* MOVE.L #<data>.L,(d16,An) */
void REGPARAM2 CPUFUNC(op_217c_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (8);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel4104;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (8);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}endlabel4104: ;
}
/* MOVE.L Dn,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_2180_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4105;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}endlabel4105: ;
}
/* MOVE.L An,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_2188_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_areg(regs, srcreg);
{	uaecptr dsta;
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4106;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}endlabel4106: ;
}
/* MOVE.L (An),(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_2190_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4107;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uaecptr dsta;
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4107;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4107: ;
}
/* MOVE.L (An)+,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_2198_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4108;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	m68k_areg(regs, srcreg) += 4;
{	uaecptr dsta;
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4108;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4108: ;
}
/* MOVE.L -(An),(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_21a0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg(regs, srcreg) - 4;
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4109;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta;
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4109;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4109: ;
}
/* MOVE.L (d16,An),(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_21a8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4110;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uaecptr dsta;
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (6));
	do_cycles_ce (2 * 256);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4110;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4110: ;
}
/* MOVE.L (d8,An,Xn),(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_21b0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4111;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uaecptr dsta;
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (6));
	do_cycles_ce (2 * 256);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4111;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4111: ;
}
/* MOVE.L (xxx).W,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_21b8_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4112;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uaecptr dsta;
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (6));
	do_cycles_ce (2 * 256);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4112;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4112: ;
}
/* MOVE.L (xxx).L,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_21b9_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4113;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uaecptr dsta;
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (8));
	do_cycles_ce (2 * 256);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel4113;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (8);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4113: ;
}
/* MOVE.L (d16,PC),(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_21ba_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4114;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uaecptr dsta;
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (6));
	do_cycles_ce (2 * 256);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4114;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4114: ;
}
/* MOVE.L (d8,PC,Xn),(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_21bb_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4115;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uaecptr dsta;
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (6));
	do_cycles_ce (2 * 256);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4115;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4115: ;
}
/* MOVE.L #<data>.L,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_21bc_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uaecptr dsta;
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (8));
	do_cycles_ce (2 * 256);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel4116;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (8);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}endlabel4116: ;
}
/* MOVE.L Dn,(xxx).W */
void REGPARAM2 CPUFUNC(op_21c0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4117;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}endlabel4117: ;
}
/* MOVE.L An,(xxx).W */
void REGPARAM2 CPUFUNC(op_21c8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s32 src = m68k_areg(regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4118;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}endlabel4118: ;
}
/* MOVE.L (An),(xxx).W */
void REGPARAM2 CPUFUNC(op_21d0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4119;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4119;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4119: ;
}
/* MOVE.L (An)+,(xxx).W */
void REGPARAM2 CPUFUNC(op_21d8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4120;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	m68k_areg(regs, srcreg) += 4;
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4120;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4120: ;
}
/* MOVE.L -(An),(xxx).W */
void REGPARAM2 CPUFUNC(op_21e0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg(regs, srcreg) - 4;
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4121;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4121;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4121: ;
}
/* MOVE.L (d16,An),(xxx).W */
void REGPARAM2 CPUFUNC(op_21e8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4122;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (6);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4122;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4122: ;
}
/* MOVE.L (d8,An,Xn),(xxx).W */
void REGPARAM2 CPUFUNC(op_21f0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4123;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (6);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4123;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4123: ;
}
/* MOVE.L (xxx).W,(xxx).W */
void REGPARAM2 CPUFUNC(op_21f8_6)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4124;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (6);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4124;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4124: ;
}
/* MOVE.L (xxx).L,(xxx).W */
void REGPARAM2 CPUFUNC(op_21f9_6)(uae_u32 opcode)
{
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4125;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (8);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel4125;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (8);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4125: ;
}
/* MOVE.L (d16,PC),(xxx).W */
void REGPARAM2 CPUFUNC(op_21fa_6)(uae_u32 opcode)
{
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4126;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (6);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4126;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4126: ;
}
/* MOVE.L (d8,PC,Xn),(xxx).W */
void REGPARAM2 CPUFUNC(op_21fb_6)(uae_u32 opcode)
{
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4127;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (6);
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
}
/* MOVE.L #<data>.L,(xxx).W */
void REGPARAM2 CPUFUNC(op_21fc_6)(uae_u32 opcode)
{
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (8);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel4128;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (8);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}endlabel4128: ;
}
/* MOVE.L Dn,(xxx).L */
void REGPARAM2 CPUFUNC(op_23c0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (4) << 16;
	dsta |= get_word_ce_prefetch (6);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4129;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}endlabel4129: ;
}
/* MOVE.L An,(xxx).L */
void REGPARAM2 CPUFUNC(op_23c8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s32 src = m68k_areg(regs, srcreg);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (4) << 16;
	dsta |= get_word_ce_prefetch (6);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4130;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}endlabel4130: ;
}
/* MOVE.L (An),(xxx).L */
void REGPARAM2 CPUFUNC(op_23d0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4131;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (4) << 16;
	dsta |= regs.irc;
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4131;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (6);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4131: ;
}
/* MOVE.L (An)+,(xxx).L */
void REGPARAM2 CPUFUNC(op_23d8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4132;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	m68k_areg(regs, srcreg) += 4;
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (4) << 16;
	dsta |= regs.irc;
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4132;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (6);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4132: ;
}
/* MOVE.L -(An),(xxx).L */
void REGPARAM2 CPUFUNC(op_23e0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg(regs, srcreg) - 4;
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4133;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (4) << 16;
	dsta |= regs.irc;
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4133;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (6);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4133: ;
}
/* MOVE.L (d16,An),(xxx).L */
void REGPARAM2 CPUFUNC(op_23e8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4134;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (6) << 16;
	dsta |= regs.irc;
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4134;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (8);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4134: ;
}
/* MOVE.L (d8,An,Xn),(xxx).L */
void REGPARAM2 CPUFUNC(op_23f0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4135;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (6) << 16;
	dsta |= regs.irc;
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4135;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (8);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4135: ;
}
/* MOVE.L (xxx).W,(xxx).L */
void REGPARAM2 CPUFUNC(op_23f8_6)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4136;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (6) << 16;
	dsta |= regs.irc;
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4136;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (8);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4136: ;
}
/* MOVE.L (xxx).L,(xxx).L */
void REGPARAM2 CPUFUNC(op_23f9_6)(uae_u32 opcode)
{
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4137;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (8) << 16;
	dsta |= regs.irc;
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel4137;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (10);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4137: ;
}
/* MOVE.L (d16,PC),(xxx).L */
void REGPARAM2 CPUFUNC(op_23fa_6)(uae_u32 opcode)
{
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4138;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (6) << 16;
	dsta |= regs.irc;
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4138;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (8);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4138: ;
}
/* MOVE.L (d8,PC,Xn),(xxx).L */
void REGPARAM2 CPUFUNC(op_23fb_6)(uae_u32 opcode)
{
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4139;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (6) << 16;
	dsta |= regs.irc;
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4139;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (8);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4139: ;
}
/* MOVE.L #<data>.L,(xxx).L */
void REGPARAM2 CPUFUNC(op_23fc_6)(uae_u32 opcode)
{
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (8) << 16;
	dsta |= get_word_ce_prefetch (10);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel4140;
	}
{	put_word_ce (dsta, src >> 16); put_word_ce (dsta + 2, src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	m68k_incpc (10);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}endlabel4140: ;
}
/* MOVE.W Dn,Dn */
void REGPARAM2 CPUFUNC(op_3000_6)(uae_u32 opcode)
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
}}}}
/* MOVE.W An,Dn */
void REGPARAM2 CPUFUNC(op_3008_6)(uae_u32 opcode)
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
}}}}
/* MOVE.W (An),Dn */
void REGPARAM2 CPUFUNC(op_3010_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4143;
	}
{{	uae_s16 src = get_word_ce (srca);
{	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xffff) | ((src) & 0xffff);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}endlabel4143: ;
}
/* MOVE.W (An)+,Dn */
void REGPARAM2 CPUFUNC(op_3018_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4144;
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
}}}}}endlabel4144: ;
}
/* MOVE.W -(An),Dn */
void REGPARAM2 CPUFUNC(op_3020_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg(regs, srcreg) - 2;
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4145;
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
}}}}}endlabel4145: ;
}
/* MOVE.W (d16,An),Dn */
void REGPARAM2 CPUFUNC(op_3028_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4146;
	}
{{	uae_s16 src = get_word_ce (srca);
{	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xffff) | ((src) & 0xffff);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}endlabel4146: ;
}
/* MOVE.W (d8,An,Xn),Dn */
void REGPARAM2 CPUFUNC(op_3030_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4147;
	}
{{	uae_s16 src = get_word_ce (srca);
{	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xffff) | ((src) & 0xffff);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}endlabel4147: ;
}
/* MOVE.W (xxx).W,Dn */
void REGPARAM2 CPUFUNC(op_3038_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4148;
	}
{{	uae_s16 src = get_word_ce (srca);
{	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xffff) | ((src) & 0xffff);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}endlabel4148: ;
}
/* MOVE.W (xxx).L,Dn */
void REGPARAM2 CPUFUNC(op_3039_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4149;
	}
{{	uae_s16 src = get_word_ce (srca);
{	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xffff) | ((src) & 0xffff);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}endlabel4149: ;
}
/* MOVE.W (d16,PC),Dn */
void REGPARAM2 CPUFUNC(op_303a_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4150;
	}
{{	uae_s16 src = get_word_ce (srca);
{	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xffff) | ((src) & 0xffff);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}endlabel4150: ;
}
/* MOVE.W (d8,PC,Xn),Dn */
void REGPARAM2 CPUFUNC(op_303b_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4151;
	}
{{	uae_s16 src = get_word_ce (srca);
{	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xffff) | ((src) & 0xffff);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}endlabel4151: ;
}
/* MOVE.W #<data>.W,Dn */
void REGPARAM2 CPUFUNC(op_303c_6)(uae_u32 opcode)
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
}}}}
/* MOVEA.W Dn,An */
void REGPARAM2 CPUFUNC(op_3040_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	src = (uae_s32)(uae_s16)src;
	m68k_areg(regs, dstreg) = (uae_s32)(uae_s16)(src);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}
/* MOVEA.W An,An */
void REGPARAM2 CPUFUNC(op_3048_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_areg(regs, srcreg);
{	src = (uae_s32)(uae_s16)src;
	m68k_areg(regs, dstreg) = (uae_s32)(uae_s16)(src);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}
/* MOVEA.W (An),An */
void REGPARAM2 CPUFUNC(op_3050_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4155;
	}
{{	uae_s16 src = get_word_ce (srca);
{	src = (uae_s32)(uae_s16)src;
	m68k_areg(regs, dstreg) = (uae_s32)(uae_s16)(src);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}endlabel4155: ;
}
/* MOVEA.W (An)+,An */
void REGPARAM2 CPUFUNC(op_3058_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4156;
	}
{{	uae_s16 src = get_word_ce (srca);
	m68k_areg(regs, srcreg) += 2;
{	src = (uae_s32)(uae_s16)src;
	m68k_areg(regs, dstreg) = (uae_s32)(uae_s16)(src);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}endlabel4156: ;
}
/* MOVEA.W -(An),An */
void REGPARAM2 CPUFUNC(op_3060_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg(regs, srcreg) - 2;
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4157;
	}
{{	uae_s16 src = get_word_ce (srca);
	m68k_areg (regs, srcreg) = srca;
{	src = (uae_s32)(uae_s16)src;
	m68k_areg(regs, dstreg) = (uae_s32)(uae_s16)(src);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}endlabel4157: ;
}
/* MOVEA.W (d16,An),An */
void REGPARAM2 CPUFUNC(op_3068_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4158;
	}
{{	uae_s16 src = get_word_ce (srca);
{	src = (uae_s32)(uae_s16)src;
	m68k_areg(regs, dstreg) = (uae_s32)(uae_s16)(src);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}endlabel4158: ;
}
/* MOVEA.W (d8,An,Xn),An */
void REGPARAM2 CPUFUNC(op_3070_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4159;
	}
{{	uae_s16 src = get_word_ce (srca);
{	src = (uae_s32)(uae_s16)src;
	m68k_areg(regs, dstreg) = (uae_s32)(uae_s16)(src);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}endlabel4159: ;
}
/* MOVEA.W (xxx).W,An */
void REGPARAM2 CPUFUNC(op_3078_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4160;
	}
{{	uae_s16 src = get_word_ce (srca);
{	src = (uae_s32)(uae_s16)src;
	m68k_areg(regs, dstreg) = (uae_s32)(uae_s16)(src);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}endlabel4160: ;
}
/* MOVEA.W (xxx).L,An */
void REGPARAM2 CPUFUNC(op_3079_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4161;
	}
{{	uae_s16 src = get_word_ce (srca);
{	src = (uae_s32)(uae_s16)src;
	m68k_areg(regs, dstreg) = (uae_s32)(uae_s16)(src);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}endlabel4161: ;
}
/* MOVEA.W (d16,PC),An */
void REGPARAM2 CPUFUNC(op_307a_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4162;
	}
{{	uae_s16 src = get_word_ce (srca);
{	src = (uae_s32)(uae_s16)src;
	m68k_areg(regs, dstreg) = (uae_s32)(uae_s16)(src);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}endlabel4162: ;
}
/* MOVEA.W (d8,PC,Xn),An */
void REGPARAM2 CPUFUNC(op_307b_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4163;
	}
{{	uae_s16 src = get_word_ce (srca);
{	src = (uae_s32)(uae_s16)src;
	m68k_areg(regs, dstreg) = (uae_s32)(uae_s16)(src);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}endlabel4163: ;
}
/* MOVEA.W #<data>.W,An */
void REGPARAM2 CPUFUNC(op_307c_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	src = (uae_s32)(uae_s16)src;
	m68k_areg(regs, dstreg) = (uae_s32)(uae_s16)(src);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}
/* MOVE.W Dn,(An) */
void REGPARAM2 CPUFUNC(op_3080_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4165;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}endlabel4165: ;
}
/* MOVE.W An,(An) */
void REGPARAM2 CPUFUNC(op_3088_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_areg(regs, srcreg);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4166;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}endlabel4166: ;
}
/* MOVE.W (An),(An) */
void REGPARAM2 CPUFUNC(op_3090_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4167;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4167;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4167: ;
}
/* MOVE.W (An)+,(An) */
void REGPARAM2 CPUFUNC(op_3098_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4168;
	}
{{	uae_s16 src = get_word_ce (srca);
	m68k_areg(regs, srcreg) += 2;
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4168;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4168: ;
}
/* MOVE.W -(An),(An) */
void REGPARAM2 CPUFUNC(op_30a0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg(regs, srcreg) - 2;
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4169;
	}
{{	uae_s16 src = get_word_ce (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4169;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4169: ;
}
/* MOVE.W (d16,An),(An) */
void REGPARAM2 CPUFUNC(op_30a8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4170;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4170;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4170: ;
}
/* MOVE.W (d8,An,Xn),(An) */
void REGPARAM2 CPUFUNC(op_30b0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4171;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4171;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4171: ;
}
/* MOVE.W (xxx).W,(An) */
void REGPARAM2 CPUFUNC(op_30b8_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4172;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4172;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4172: ;
}
/* MOVE.W (xxx).L,(An) */
void REGPARAM2 CPUFUNC(op_30b9_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4173;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel4173;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4173: ;
}
/* MOVE.W (d16,PC),(An) */
void REGPARAM2 CPUFUNC(op_30ba_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4174;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4174;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4174: ;
}
/* MOVE.W (d8,PC,Xn),(An) */
void REGPARAM2 CPUFUNC(op_30bb_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4175;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4175;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4175: ;
}
/* MOVE.W #<data>.W,(An) */
void REGPARAM2 CPUFUNC(op_30bc_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4176;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}endlabel4176: ;
}
/* MOVE.W Dn,(An)+ */
void REGPARAM2 CPUFUNC(op_30c0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4177;
	}
{	m68k_areg(regs, dstreg) += 2;
	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}endlabel4177: ;
}
/* MOVE.W An,(An)+ */
void REGPARAM2 CPUFUNC(op_30c8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_areg(regs, srcreg);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4178;
	}
{	m68k_areg(regs, dstreg) += 2;
	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}endlabel4178: ;
}
/* MOVE.W (An),(An)+ */
void REGPARAM2 CPUFUNC(op_30d0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4179;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4179;
	}
{	m68k_areg(regs, dstreg) += 2;
	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4179: ;
}
/* MOVE.W (An)+,(An)+ */
void REGPARAM2 CPUFUNC(op_30d8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4180;
	}
{{	uae_s16 src = get_word_ce (srca);
	m68k_areg(regs, srcreg) += 2;
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4180;
	}
{	m68k_areg(regs, dstreg) += 2;
	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4180: ;
}
/* MOVE.W -(An),(An)+ */
void REGPARAM2 CPUFUNC(op_30e0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg(regs, srcreg) - 2;
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4181;
	}
{{	uae_s16 src = get_word_ce (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4181;
	}
{	m68k_areg(regs, dstreg) += 2;
	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4181: ;
}
/* MOVE.W (d16,An),(An)+ */
void REGPARAM2 CPUFUNC(op_30e8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4182;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4182;
	}
{	m68k_areg(regs, dstreg) += 2;
	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4182: ;
}
/* MOVE.W (d8,An,Xn),(An)+ */
void REGPARAM2 CPUFUNC(op_30f0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4183;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4183;
	}
{	m68k_areg(regs, dstreg) += 2;
	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4183: ;
}
/* MOVE.W (xxx).W,(An)+ */
void REGPARAM2 CPUFUNC(op_30f8_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4184;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4184;
	}
{	m68k_areg(regs, dstreg) += 2;
	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4184: ;
}
/* MOVE.W (xxx).L,(An)+ */
void REGPARAM2 CPUFUNC(op_30f9_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4185;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel4185;
	}
{	m68k_areg(regs, dstreg) += 2;
	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4185: ;
}
/* MOVE.W (d16,PC),(An)+ */
void REGPARAM2 CPUFUNC(op_30fa_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4186;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4186;
	}
{	m68k_areg(regs, dstreg) += 2;
	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4186: ;
}
/* MOVE.W (d8,PC,Xn),(An)+ */
void REGPARAM2 CPUFUNC(op_30fb_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4187;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4187;
	}
{	m68k_areg(regs, dstreg) += 2;
	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4187: ;
}
/* MOVE.W #<data>.W,(An)+ */
void REGPARAM2 CPUFUNC(op_30fc_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4188;
	}
{	m68k_areg(regs, dstreg) += 2;
	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}endlabel4188: ;
}
/* MOVE.W Dn,-(An) */
void REGPARAM2 CPUFUNC(op_3100_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - 2;
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4189;
	}
{	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (2);
}}}}endlabel4189: ;
}
/* MOVE.W An,-(An) */
void REGPARAM2 CPUFUNC(op_3108_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_areg(regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - 2;
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4190;
	}
{	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (2);
}}}}endlabel4190: ;
}
/* MOVE.W (An),-(An) */
void REGPARAM2 CPUFUNC(op_3110_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4191;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - 2;
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4191;
	}
{	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (2);
}}}}}}endlabel4191: ;
}
/* MOVE.W (An)+,-(An) */
void REGPARAM2 CPUFUNC(op_3118_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4192;
	}
{{	uae_s16 src = get_word_ce (srca);
	m68k_areg(regs, srcreg) += 2;
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - 2;
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4192;
	}
{	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (2);
}}}}}}endlabel4192: ;
}
/* MOVE.W -(An),-(An) */
void REGPARAM2 CPUFUNC(op_3120_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg(regs, srcreg) - 2;
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4193;
	}
{{	uae_s16 src = get_word_ce (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - 2;
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4193;
	}
{	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (2);
}}}}}}endlabel4193: ;
}
/* MOVE.W (d16,An),-(An) */
void REGPARAM2 CPUFUNC(op_3128_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4194;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - 2;
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4194;
	}
{	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (4);
}}}}}}endlabel4194: ;
}
/* MOVE.W (d8,An,Xn),-(An) */
void REGPARAM2 CPUFUNC(op_3130_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4195;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - 2;
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4195;
	}
{	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (4);
}}}}}}endlabel4195: ;
}
/* MOVE.W (xxx).W,-(An) */
void REGPARAM2 CPUFUNC(op_3138_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4196;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - 2;
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4196;
	}
{	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (4);
}}}}}}endlabel4196: ;
}
/* MOVE.W (xxx).L,-(An) */
void REGPARAM2 CPUFUNC(op_3139_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4197;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - 2;
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel4197;
	}
{	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (6);
}}}}}}endlabel4197: ;
}
/* MOVE.W (d16,PC),-(An) */
void REGPARAM2 CPUFUNC(op_313a_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4198;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - 2;
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4198;
	}
{	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (4);
}}}}}}endlabel4198: ;
}
/* MOVE.W (d8,PC,Xn),-(An) */
void REGPARAM2 CPUFUNC(op_313b_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4199;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - 2;
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4199;
	}
{	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (4);
}}}}}}endlabel4199: ;
}
/* MOVE.W #<data>.W,-(An) */
void REGPARAM2 CPUFUNC(op_313c_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - 2;
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4200;
	}
{	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (4);
}}}}endlabel4200: ;
}
/* MOVE.W Dn,(d16,An) */
void REGPARAM2 CPUFUNC(op_3140_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4201;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}endlabel4201: ;
}
/* MOVE.W An,(d16,An) */
void REGPARAM2 CPUFUNC(op_3148_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_areg(regs, srcreg);
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4202;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}endlabel4202: ;
}
/* MOVE.W (An),(d16,An) */
void REGPARAM2 CPUFUNC(op_3150_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4203;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
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
}
/* MOVE.W (An)+,(d16,An) */
void REGPARAM2 CPUFUNC(op_3158_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4204;
	}
{{	uae_s16 src = get_word_ce (srca);
	m68k_areg(regs, srcreg) += 2;
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
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
}
/* MOVE.W -(An),(d16,An) */
void REGPARAM2 CPUFUNC(op_3160_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg(regs, srcreg) - 2;
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4205;
	}
{{	uae_s16 src = get_word_ce (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4205;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4205: ;
}
/* MOVE.W (d16,An),(d16,An) */
void REGPARAM2 CPUFUNC(op_3168_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4206;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (6);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4206;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4206: ;
}
/* MOVE.W (d8,An,Xn),(d16,An) */
void REGPARAM2 CPUFUNC(op_3170_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4207;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (6);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4207;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4207: ;
}
/* MOVE.W (xxx).W,(d16,An) */
void REGPARAM2 CPUFUNC(op_3178_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4208;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (6);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4208;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4208: ;
}
/* MOVE.W (xxx).L,(d16,An) */
void REGPARAM2 CPUFUNC(op_3179_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4209;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (8);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel4209;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (8);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4209: ;
}
/* MOVE.W (d16,PC),(d16,An) */
void REGPARAM2 CPUFUNC(op_317a_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4210;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (6);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4210;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4210: ;
}
/* MOVE.W (d8,PC,Xn),(d16,An) */
void REGPARAM2 CPUFUNC(op_317b_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4211;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (6);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4211;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4211: ;
}
/* MOVE.W #<data>.W,(d16,An) */
void REGPARAM2 CPUFUNC(op_317c_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (6);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4212;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}endlabel4212: ;
}
/* MOVE.W Dn,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_3180_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4213;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}endlabel4213: ;
}
/* MOVE.W An,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_3188_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_areg(regs, srcreg);
{	uaecptr dsta;
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4214;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}endlabel4214: ;
}
/* MOVE.W (An),(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_3190_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4215;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uaecptr dsta;
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4215;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4215: ;
}
/* MOVE.W (An)+,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_3198_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4216;
	}
{{	uae_s16 src = get_word_ce (srca);
	m68k_areg(regs, srcreg) += 2;
{	uaecptr dsta;
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4216;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4216: ;
}
/* MOVE.W -(An),(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_31a0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg(regs, srcreg) - 2;
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4217;
	}
{{	uae_s16 src = get_word_ce (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta;
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4217;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4217: ;
}
/* MOVE.W (d16,An),(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_31a8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4218;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uaecptr dsta;
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (6));
	do_cycles_ce (2 * 256);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4218;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4218: ;
}
/* MOVE.W (d8,An,Xn),(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_31b0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4219;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uaecptr dsta;
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (6));
	do_cycles_ce (2 * 256);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4219;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4219: ;
}
/* MOVE.W (xxx).W,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_31b8_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4220;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uaecptr dsta;
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (6));
	do_cycles_ce (2 * 256);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4220;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4220: ;
}
/* MOVE.W (xxx).L,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_31b9_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4221;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uaecptr dsta;
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (8));
	do_cycles_ce (2 * 256);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel4221;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (8);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4221: ;
}
/* MOVE.W (d16,PC),(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_31ba_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4222;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uaecptr dsta;
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (6));
	do_cycles_ce (2 * 256);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4222;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4222: ;
}
/* MOVE.W (d8,PC,Xn),(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_31bb_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4223;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uaecptr dsta;
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (6));
	do_cycles_ce (2 * 256);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4223;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4223: ;
}
/* MOVE.W #<data>.W,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_31bc_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta;
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (6));
	do_cycles_ce (2 * 256);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4224;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}endlabel4224: ;
}
/* MOVE.W Dn,(xxx).W */
void REGPARAM2 CPUFUNC(op_31c0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4225;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}endlabel4225: ;
}
/* MOVE.W An,(xxx).W */
void REGPARAM2 CPUFUNC(op_31c8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_areg(regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4226;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}endlabel4226: ;
}
/* MOVE.W (An),(xxx).W */
void REGPARAM2 CPUFUNC(op_31d0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4227;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4227;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4227: ;
}
/* MOVE.W (An)+,(xxx).W */
void REGPARAM2 CPUFUNC(op_31d8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4228;
	}
{{	uae_s16 src = get_word_ce (srca);
	m68k_areg(regs, srcreg) += 2;
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4228;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4228: ;
}
/* MOVE.W -(An),(xxx).W */
void REGPARAM2 CPUFUNC(op_31e0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg(regs, srcreg) - 2;
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4229;
	}
{{	uae_s16 src = get_word_ce (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4229;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4229: ;
}
/* MOVE.W (d16,An),(xxx).W */
void REGPARAM2 CPUFUNC(op_31e8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4230;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (6);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4230;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4230: ;
}
/* MOVE.W (d8,An,Xn),(xxx).W */
void REGPARAM2 CPUFUNC(op_31f0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4231;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (6);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4231;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4231: ;
}
/* MOVE.W (xxx).W,(xxx).W */
void REGPARAM2 CPUFUNC(op_31f8_6)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4232;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (6);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4232;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4232: ;
}
/* MOVE.W (xxx).L,(xxx).W */
void REGPARAM2 CPUFUNC(op_31f9_6)(uae_u32 opcode)
{
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4233;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (8);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel4233;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (8);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4233: ;
}
/* MOVE.W (d16,PC),(xxx).W */
void REGPARAM2 CPUFUNC(op_31fa_6)(uae_u32 opcode)
{
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4234;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (6);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4234;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4234: ;
}
/* MOVE.W (d8,PC,Xn),(xxx).W */
void REGPARAM2 CPUFUNC(op_31fb_6)(uae_u32 opcode)
{
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4235;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (6);
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
}
/* MOVE.W #<data>.W,(xxx).W */
void REGPARAM2 CPUFUNC(op_31fc_6)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (6);
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
}}}}endlabel4236: ;
}
/* MOVE.W Dn,(xxx).L */
void REGPARAM2 CPUFUNC(op_33c0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (4) << 16;
	dsta |= get_word_ce_prefetch (6);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4237;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}endlabel4237: ;
}
/* MOVE.W An,(xxx).L */
void REGPARAM2 CPUFUNC(op_33c8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_areg(regs, srcreg);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (4) << 16;
	dsta |= get_word_ce_prefetch (6);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4238;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}endlabel4238: ;
}
/* MOVE.W (An),(xxx).L */
void REGPARAM2 CPUFUNC(op_33d0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4239;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (4) << 16;
	dsta |= regs.irc;
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4239;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (6);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4239: ;
}
/* MOVE.W (An)+,(xxx).L */
void REGPARAM2 CPUFUNC(op_33d8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4240;
	}
{{	uae_s16 src = get_word_ce (srca);
	m68k_areg(regs, srcreg) += 2;
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (4) << 16;
	dsta |= regs.irc;
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4240;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (6);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4240: ;
}
/* MOVE.W -(An),(xxx).L */
void REGPARAM2 CPUFUNC(op_33e0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg(regs, srcreg) - 2;
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4241;
	}
{{	uae_s16 src = get_word_ce (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (4) << 16;
	dsta |= regs.irc;
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4241;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (6);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4241: ;
}
/* MOVE.W (d16,An),(xxx).L */
void REGPARAM2 CPUFUNC(op_33e8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4242;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (6) << 16;
	dsta |= regs.irc;
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4242;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (8);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4242: ;
}
/* MOVE.W (d8,An,Xn),(xxx).L */
void REGPARAM2 CPUFUNC(op_33f0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4243;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (6) << 16;
	dsta |= regs.irc;
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4243;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (8);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4243: ;
}
/* MOVE.W (xxx).W,(xxx).L */
void REGPARAM2 CPUFUNC(op_33f8_6)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4244;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (6) << 16;
	dsta |= regs.irc;
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4244;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (8);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4244: ;
}
/* MOVE.W (xxx).L,(xxx).L */
void REGPARAM2 CPUFUNC(op_33f9_6)(uae_u32 opcode)
{
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4245;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (8) << 16;
	dsta |= regs.irc;
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel4245;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (10);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4245: ;
}
/* MOVE.W (d16,PC),(xxx).L */
void REGPARAM2 CPUFUNC(op_33fa_6)(uae_u32 opcode)
{
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4246;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (6) << 16;
	dsta |= regs.irc;
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4246;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (8);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4246: ;
}
/* MOVE.W (d8,PC,Xn),(xxx).L */
void REGPARAM2 CPUFUNC(op_33fb_6)(uae_u32 opcode)
{
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4247;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (6) << 16;
	dsta |= regs.irc;
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4247;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (8);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}endlabel4247: ;
}
/* MOVE.W #<data>.W,(xxx).L */
void REGPARAM2 CPUFUNC(op_33fc_6)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (6) << 16;
	dsta |= get_word_ce_prefetch (8);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4248;
	}
{	put_word_ce (dsta,src);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
	m68k_incpc (8);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}endlabel4248: ;
}
/* NEGX.B Dn */
void REGPARAM2 CPUFUNC(op_4000_6)(uae_u32 opcode)
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
}
/* NEGX.B (An) */
void REGPARAM2 CPUFUNC(op_4010_6)(uae_u32 opcode)
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
}
/* NEGX.B (An)+ */
void REGPARAM2 CPUFUNC(op_4018_6)(uae_u32 opcode)
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
}
/* NEGX.B -(An) */
void REGPARAM2 CPUFUNC(op_4020_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
	do_cycles_ce (2 * 256);
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
}
/* NEGX.B (d16,An) */
void REGPARAM2 CPUFUNC(op_4028_6)(uae_u32 opcode)
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
}
/* NEGX.B (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_4030_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
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
}
/* NEGX.B (xxx).W */
void REGPARAM2 CPUFUNC(op_4038_6)(uae_u32 opcode)
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
}
/* NEGX.B (xxx).L */
void REGPARAM2 CPUFUNC(op_4039_6)(uae_u32 opcode)
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
}
/* NEGX.W Dn */
void REGPARAM2 CPUFUNC(op_4040_6)(uae_u32 opcode)
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
}
/* NEGX.W (An) */
void REGPARAM2 CPUFUNC(op_4050_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4258;
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
endlabel4258: ;
}
/* NEGX.W (An)+ */
void REGPARAM2 CPUFUNC(op_4058_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4259;
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
endlabel4259: ;
}
/* NEGX.W -(An) */
void REGPARAM2 CPUFUNC(op_4060_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg(regs, srcreg) - 2;
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4260;
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
endlabel4260: ;
}
/* NEGX.W (d16,An) */
void REGPARAM2 CPUFUNC(op_4068_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4261;
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
endlabel4261: ;
}
/* NEGX.W (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_4070_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4262;
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
endlabel4262: ;
}
/* NEGX.W (xxx).W */
void REGPARAM2 CPUFUNC(op_4078_6)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4263;
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
endlabel4263: ;
}
/* NEGX.W (xxx).L */
void REGPARAM2 CPUFUNC(op_4079_6)(uae_u32 opcode)
{
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4264;
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
endlabel4264: ;
}
/* NEGX.L Dn */
void REGPARAM2 CPUFUNC(op_4080_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s32 src = m68k_dreg(regs, srcreg);
	do_cycles_ce (2 * 256);
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
	m68k_dreg(regs, srcreg) = (newv);
}}}}	m68k_incpc (2);
}
/* NEGX.L (An) */
void REGPARAM2 CPUFUNC(op_4090_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4266;
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
	put_word_ce (srca + 2, newv); put_word_ce (srca, newv >> 16);
}}}}}}	m68k_incpc (2);
endlabel4266: ;
}
/* NEGX.L (An)+ */
void REGPARAM2 CPUFUNC(op_4098_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4267;
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
	put_word_ce (srca + 2, newv); put_word_ce (srca, newv >> 16);
}}}}}}	m68k_incpc (2);
endlabel4267: ;
}
/* NEGX.L -(An) */
void REGPARAM2 CPUFUNC(op_40a0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg(regs, srcreg) - 4;
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4268;
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
	put_word_ce (srca + 2, newv); put_word_ce (srca, newv >> 16);
}}}}}}	m68k_incpc (2);
endlabel4268: ;
}
/* NEGX.L (d16,An) */
void REGPARAM2 CPUFUNC(op_40a8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4269;
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
	put_word_ce (srca + 2, newv); put_word_ce (srca, newv >> 16);
}}}}}}	m68k_incpc (4);
endlabel4269: ;
}
/* NEGX.L (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_40b0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4270;
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
	put_word_ce (srca + 2, newv); put_word_ce (srca, newv >> 16);
}}}}}}	m68k_incpc (4);
endlabel4270: ;
}
/* NEGX.L (xxx).W */
void REGPARAM2 CPUFUNC(op_40b8_6)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4271;
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
	put_word_ce (srca + 2, newv); put_word_ce (srca, newv >> 16);
}}}}}}	m68k_incpc (4);
endlabel4271: ;
}
/* NEGX.L (xxx).L */
void REGPARAM2 CPUFUNC(op_40b9_6)(uae_u32 opcode)
{
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4272;
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
	put_word_ce (srca + 2, newv); put_word_ce (srca, newv >> 16);
}}}}}}	m68k_incpc (6);
endlabel4272: ;
}
/* MVSR2.W Dn */
void REGPARAM2 CPUFUNC(op_40c0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	do_cycles_ce (2 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	MakeSR();
	m68k_dreg(regs, srcreg) = (m68k_dreg(regs, srcreg) & ~0xffff) | ((regs.sr) & 0xffff);
}}	m68k_incpc (2);
}
/* MVSR2.W (An) */
void REGPARAM2 CPUFUNC(op_40d0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4274;
	}
{	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	MakeSR();
	put_word_ce (srca,regs.sr);
}}}	m68k_incpc (2);
endlabel4274: ;
}
/* MVSR2.W (An)+ */
void REGPARAM2 CPUFUNC(op_40d8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4275;
	}
{	m68k_areg(regs, srcreg) += 2;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	MakeSR();
	put_word_ce (srca,regs.sr);
}}}	m68k_incpc (2);
endlabel4275: ;
}
/* MVSR2.W -(An) */
void REGPARAM2 CPUFUNC(op_40e0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg(regs, srcreg) - 2;
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4276;
	}
{	m68k_areg (regs, srcreg) = srca;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	MakeSR();
	put_word_ce (srca,regs.sr);
}}}	m68k_incpc (2);
endlabel4276: ;
}
/* MVSR2.W (d16,An) */
void REGPARAM2 CPUFUNC(op_40e8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4277;
	}
{	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	MakeSR();
	put_word_ce (srca,regs.sr);
}}}	m68k_incpc (4);
endlabel4277: ;
}
/* MVSR2.W (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_40f0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4278;
	}
{	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	MakeSR();
	put_word_ce (srca,regs.sr);
}}}	m68k_incpc (4);
endlabel4278: ;
}
/* MVSR2.W (xxx).W */
void REGPARAM2 CPUFUNC(op_40f8_6)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4279;
	}
{	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	MakeSR();
	put_word_ce (srca,regs.sr);
}}}	m68k_incpc (4);
endlabel4279: ;
}
/* MVSR2.W (xxx).L */
void REGPARAM2 CPUFUNC(op_40f9_6)(uae_u32 opcode)
{
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4280;
	}
{	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	MakeSR();
	put_word_ce (srca,regs.sr);
}}}	m68k_incpc (6);
endlabel4280: ;
}
/* CHK.L Dn,Dn */
void REGPARAM2 CPUFUNC(op_4100_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{	uaecptr oldpc = m68k_getpc();
{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	m68k_incpc (2);
	do_cycles_ce (6 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6, oldpc);
		goto endlabel4281;
	} else if (dst > src) {
		SET_NFLG (0);
		Exception (6, oldpc);
		goto endlabel4281;
	}
}}}endlabel4281: ;
}
/* CHK.L (An),Dn */
void REGPARAM2 CPUFUNC(op_4110_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{	uaecptr oldpc = m68k_getpc();
{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4282;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	m68k_incpc (2);
	do_cycles_ce (6 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6, oldpc);
		goto endlabel4282;
	} else if (dst > src) {
		SET_NFLG (0);
		Exception (6, oldpc);
		goto endlabel4282;
	}
}}}}}endlabel4282: ;
}
/* CHK.L (An)+,Dn */
void REGPARAM2 CPUFUNC(op_4118_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{	uaecptr oldpc = m68k_getpc();
{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4283;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	m68k_areg(regs, srcreg) += 4;
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	m68k_incpc (2);
	do_cycles_ce (6 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6, oldpc);
		goto endlabel4283;
	} else if (dst > src) {
		SET_NFLG (0);
		Exception (6, oldpc);
		goto endlabel4283;
	}
}}}}}endlabel4283: ;
}
/* CHK.L -(An),Dn */
void REGPARAM2 CPUFUNC(op_4120_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{	uaecptr oldpc = m68k_getpc();
{	uaecptr srca;
	srca = m68k_areg(regs, srcreg) - 4;
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4284;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	m68k_areg (regs, srcreg) = srca;
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	m68k_incpc (2);
	do_cycles_ce (6 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6, oldpc);
		goto endlabel4284;
	} else if (dst > src) {
		SET_NFLG (0);
		Exception (6, oldpc);
		goto endlabel4284;
	}
}}}}}endlabel4284: ;
}
/* CHK.L (d16,An),Dn */
void REGPARAM2 CPUFUNC(op_4128_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{	uaecptr oldpc = m68k_getpc();
{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4285;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	m68k_incpc (4);
	do_cycles_ce (6 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6, oldpc);
		goto endlabel4285;
	} else if (dst > src) {
		SET_NFLG (0);
		Exception (6, oldpc);
		goto endlabel4285;
	}
}}}}}endlabel4285: ;
}
/* CHK.L (d8,An,Xn),Dn */
void REGPARAM2 CPUFUNC(op_4130_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{	uaecptr oldpc = m68k_getpc();
{	uaecptr srca;
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4286;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	m68k_incpc (4);
	do_cycles_ce (6 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6, oldpc);
		goto endlabel4286;
	} else if (dst > src) {
		SET_NFLG (0);
		Exception (6, oldpc);
		goto endlabel4286;
	}
}}}}}endlabel4286: ;
}
/* CHK.L (xxx).W,Dn */
void REGPARAM2 CPUFUNC(op_4138_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{	uaecptr oldpc = m68k_getpc();
{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4287;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	m68k_incpc (4);
	do_cycles_ce (6 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6, oldpc);
		goto endlabel4287;
	} else if (dst > src) {
		SET_NFLG (0);
		Exception (6, oldpc);
		goto endlabel4287;
	}
}}}}}endlabel4287: ;
}
/* CHK.L (xxx).L,Dn */
void REGPARAM2 CPUFUNC(op_4139_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{	uaecptr oldpc = m68k_getpc();
{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4288;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	m68k_incpc (6);
	do_cycles_ce (6 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6, oldpc);
		goto endlabel4288;
	} else if (dst > src) {
		SET_NFLG (0);
		Exception (6, oldpc);
		goto endlabel4288;
	}
}}}}}endlabel4288: ;
}
/* CHK.L (d16,PC),Dn */
void REGPARAM2 CPUFUNC(op_413a_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{	uaecptr oldpc = m68k_getpc();
{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4289;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	m68k_incpc (4);
	do_cycles_ce (6 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6, oldpc);
		goto endlabel4289;
	} else if (dst > src) {
		SET_NFLG (0);
		Exception (6, oldpc);
		goto endlabel4289;
	}
}}}}}endlabel4289: ;
}
/* CHK.L (d8,PC,Xn),Dn */
void REGPARAM2 CPUFUNC(op_413b_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{	uaecptr oldpc = m68k_getpc();
{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4290;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	m68k_incpc (4);
	do_cycles_ce (6 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6, oldpc);
		goto endlabel4290;
	} else if (dst > src) {
		SET_NFLG (0);
		Exception (6, oldpc);
		goto endlabel4290;
	}
}}}}}endlabel4290: ;
}
/* CHK.L #<data>.L,Dn */
void REGPARAM2 CPUFUNC(op_413c_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{	uaecptr oldpc = m68k_getpc();
{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	m68k_incpc (6);
	do_cycles_ce (6 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6, oldpc);
		goto endlabel4291;
	} else if (dst > src) {
		SET_NFLG (0);
		Exception (6, oldpc);
		goto endlabel4291;
	}
}}}endlabel4291: ;
}
/* CHK.W Dn,Dn */
void REGPARAM2 CPUFUNC(op_4180_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{	uaecptr oldpc = m68k_getpc();
{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	m68k_incpc (2);
	do_cycles_ce (6 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6, oldpc);
		goto endlabel4292;
	} else if (dst > src) {
		SET_NFLG (0);
		Exception (6, oldpc);
		goto endlabel4292;
	}
}}}endlabel4292: ;
}
/* CHK.W (An),Dn */
void REGPARAM2 CPUFUNC(op_4190_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{	uaecptr oldpc = m68k_getpc();
{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4293;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	m68k_incpc (2);
	do_cycles_ce (6 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6, oldpc);
		goto endlabel4293;
	} else if (dst > src) {
		SET_NFLG (0);
		Exception (6, oldpc);
		goto endlabel4293;
	}
}}}}}endlabel4293: ;
}
/* CHK.W (An)+,Dn */
void REGPARAM2 CPUFUNC(op_4198_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{	uaecptr oldpc = m68k_getpc();
{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4294;
	}
{{	uae_s16 src = get_word_ce (srca);
	m68k_areg(regs, srcreg) += 2;
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	m68k_incpc (2);
	do_cycles_ce (6 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6, oldpc);
		goto endlabel4294;
	} else if (dst > src) {
		SET_NFLG (0);
		Exception (6, oldpc);
		goto endlabel4294;
	}
}}}}}endlabel4294: ;
}
/* CHK.W -(An),Dn */
void REGPARAM2 CPUFUNC(op_41a0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{	uaecptr oldpc = m68k_getpc();
{	uaecptr srca;
	srca = m68k_areg(regs, srcreg) - 2;
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4295;
	}
{{	uae_s16 src = get_word_ce (srca);
	m68k_areg (regs, srcreg) = srca;
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	m68k_incpc (2);
	do_cycles_ce (6 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6, oldpc);
		goto endlabel4295;
	} else if (dst > src) {
		SET_NFLG (0);
		Exception (6, oldpc);
		goto endlabel4295;
	}
}}}}}endlabel4295: ;
}
/* CHK.W (d16,An),Dn */
void REGPARAM2 CPUFUNC(op_41a8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{	uaecptr oldpc = m68k_getpc();
{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4296;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	m68k_incpc (4);
	do_cycles_ce (6 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6, oldpc);
		goto endlabel4296;
	} else if (dst > src) {
		SET_NFLG (0);
		Exception (6, oldpc);
		goto endlabel4296;
	}
}}}}}endlabel4296: ;
}
/* CHK.W (d8,An,Xn),Dn */
void REGPARAM2 CPUFUNC(op_41b0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{	uaecptr oldpc = m68k_getpc();
{	uaecptr srca;
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4297;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	m68k_incpc (4);
	do_cycles_ce (6 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6, oldpc);
		goto endlabel4297;
	} else if (dst > src) {
		SET_NFLG (0);
		Exception (6, oldpc);
		goto endlabel4297;
	}
}}}}}endlabel4297: ;
}
/* CHK.W (xxx).W,Dn */
void REGPARAM2 CPUFUNC(op_41b8_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{	uaecptr oldpc = m68k_getpc();
{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4298;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	m68k_incpc (4);
	do_cycles_ce (6 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6, oldpc);
		goto endlabel4298;
	} else if (dst > src) {
		SET_NFLG (0);
		Exception (6, oldpc);
		goto endlabel4298;
	}
}}}}}endlabel4298: ;
}
/* CHK.W (xxx).L,Dn */
void REGPARAM2 CPUFUNC(op_41b9_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{	uaecptr oldpc = m68k_getpc();
{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4299;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	m68k_incpc (6);
	do_cycles_ce (6 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6, oldpc);
		goto endlabel4299;
	} else if (dst > src) {
		SET_NFLG (0);
		Exception (6, oldpc);
		goto endlabel4299;
	}
}}}}}endlabel4299: ;
}
/* CHK.W (d16,PC),Dn */
void REGPARAM2 CPUFUNC(op_41ba_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{	uaecptr oldpc = m68k_getpc();
{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4300;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	m68k_incpc (4);
	do_cycles_ce (6 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6, oldpc);
		goto endlabel4300;
	} else if (dst > src) {
		SET_NFLG (0);
		Exception (6, oldpc);
		goto endlabel4300;
	}
}}}}}endlabel4300: ;
}
/* CHK.W (d8,PC,Xn),Dn */
void REGPARAM2 CPUFUNC(op_41bb_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{	uaecptr oldpc = m68k_getpc();
{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4301;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	m68k_incpc (4);
	do_cycles_ce (6 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6, oldpc);
		goto endlabel4301;
	} else if (dst > src) {
		SET_NFLG (0);
		Exception (6, oldpc);
		goto endlabel4301;
	}
}}}}}endlabel4301: ;
}
/* CHK.W #<data>.W,Dn */
void REGPARAM2 CPUFUNC(op_41bc_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{	uaecptr oldpc = m68k_getpc();
{	uae_s16 src = get_word_ce_prefetch (4);
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	m68k_incpc (4);
	do_cycles_ce (6 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6, oldpc);
		goto endlabel4302;
	} else if (dst > src) {
		SET_NFLG (0);
		Exception (6, oldpc);
		goto endlabel4302;
	}
}}}endlabel4302: ;
}
/* LEA.L (An),An */
void REGPARAM2 CPUFUNC(op_41d0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
{	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	m68k_areg(regs, dstreg) = (srca);
}}}	m68k_incpc (2);
}
/* LEA.L (d16,An),An */
void REGPARAM2 CPUFUNC(op_41e8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	m68k_areg(regs, dstreg) = (srca);
}}}	m68k_incpc (4);
}
/* LEA.L (d8,An,Xn),An */
void REGPARAM2 CPUFUNC(op_41f0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
{	do_cycles_ce (4 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	m68k_areg(regs, dstreg) = (srca);
}}}	m68k_incpc (4);
}
/* LEA.L (xxx).W,An */
void REGPARAM2 CPUFUNC(op_41f8_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	m68k_areg(regs, dstreg) = (srca);
}}}	m68k_incpc (4);
}
/* LEA.L (xxx).L,An */
void REGPARAM2 CPUFUNC(op_41f9_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
{	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	m68k_areg(regs, dstreg) = (srca);
}}}	m68k_incpc (6);
}
/* LEA.L (d16,PC),An */
void REGPARAM2 CPUFUNC(op_41fa_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	m68k_areg(regs, dstreg) = (srca);
}}}	m68k_incpc (4);
}
/* LEA.L (d8,PC,Xn),An */
void REGPARAM2 CPUFUNC(op_41fb_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
{	do_cycles_ce (4 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	m68k_areg(regs, dstreg) = (srca);
}}}	m68k_incpc (4);
}
/* CLR.B Dn */
void REGPARAM2 CPUFUNC(op_4200_6)(uae_u32 opcode)
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
}
/* CLR.B (An) */
void REGPARAM2 CPUFUNC(op_4210_6)(uae_u32 opcode)
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
}
/* CLR.B (An)+ */
void REGPARAM2 CPUFUNC(op_4218_6)(uae_u32 opcode)
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
}
/* CLR.B -(An) */
void REGPARAM2 CPUFUNC(op_4220_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
	do_cycles_ce (2 * 256);
{	uae_s8 src = get_byte_ce (srca);
	m68k_areg (regs, srcreg) = srca;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(0)) == 0);
	SET_NFLG (((uae_s8)(0)) < 0);
	put_byte_ce (srca,0);
}}}	m68k_incpc (2);
}
/* CLR.B (d16,An) */
void REGPARAM2 CPUFUNC(op_4228_6)(uae_u32 opcode)
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
}
/* CLR.B (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_4230_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
{	uae_s8 src = get_byte_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(0)) == 0);
	SET_NFLG (((uae_s8)(0)) < 0);
	put_byte_ce (srca,0);
}}}	m68k_incpc (4);
}
/* CLR.B (xxx).W */
void REGPARAM2 CPUFUNC(op_4238_6)(uae_u32 opcode)
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
}
/* CLR.B (xxx).L */
void REGPARAM2 CPUFUNC(op_4239_6)(uae_u32 opcode)
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
}
/* CLR.W Dn */
void REGPARAM2 CPUFUNC(op_4240_6)(uae_u32 opcode)
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
}
/* CLR.W (An) */
void REGPARAM2 CPUFUNC(op_4250_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4319;
	}
{{	uae_s16 src = get_word_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(0)) == 0);
	SET_NFLG (((uae_s16)(0)) < 0);
	put_word_ce (srca,0);
}}}}	m68k_incpc (2);
endlabel4319: ;
}
/* CLR.W (An)+ */
void REGPARAM2 CPUFUNC(op_4258_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4320;
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
endlabel4320: ;
}
/* CLR.W -(An) */
void REGPARAM2 CPUFUNC(op_4260_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg(regs, srcreg) - 2;
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4321;
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
endlabel4321: ;
}
/* CLR.W (d16,An) */
void REGPARAM2 CPUFUNC(op_4268_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4322;
	}
{{	uae_s16 src = get_word_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(0)) == 0);
	SET_NFLG (((uae_s16)(0)) < 0);
	put_word_ce (srca,0);
}}}}	m68k_incpc (4);
endlabel4322: ;
}
/* CLR.W (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_4270_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4323;
	}
{{	uae_s16 src = get_word_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(0)) == 0);
	SET_NFLG (((uae_s16)(0)) < 0);
	put_word_ce (srca,0);
}}}}	m68k_incpc (4);
endlabel4323: ;
}
/* CLR.W (xxx).W */
void REGPARAM2 CPUFUNC(op_4278_6)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4324;
	}
{{	uae_s16 src = get_word_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(0)) == 0);
	SET_NFLG (((uae_s16)(0)) < 0);
	put_word_ce (srca,0);
}}}}	m68k_incpc (4);
endlabel4324: ;
}
/* CLR.W (xxx).L */
void REGPARAM2 CPUFUNC(op_4279_6)(uae_u32 opcode)
{
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4325;
	}
{{	uae_s16 src = get_word_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(0)) == 0);
	SET_NFLG (((uae_s16)(0)) < 0);
	put_word_ce (srca,0);
}}}}	m68k_incpc (6);
endlabel4325: ;
}
/* CLR.L Dn */
void REGPARAM2 CPUFUNC(op_4280_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s32 src = m68k_dreg(regs, srcreg);
	do_cycles_ce (2 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(0)) == 0);
	SET_NFLG (((uae_s32)(0)) < 0);
	m68k_dreg(regs, srcreg) = (0);
}}	m68k_incpc (2);
}
/* CLR.L (An) */
void REGPARAM2 CPUFUNC(op_4290_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4327;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(0)) == 0);
	SET_NFLG (((uae_s32)(0)) < 0);
	put_word_ce (srca + 2, 0); put_word_ce (srca, 0 >> 16);
}}}}	m68k_incpc (2);
endlabel4327: ;
}
#endif

#ifdef PART_4
/* CLR.L (An)+ */
void REGPARAM2 CPUFUNC(op_4298_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4328;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	m68k_areg(regs, srcreg) += 4;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(0)) == 0);
	SET_NFLG (((uae_s32)(0)) < 0);
	put_word_ce (srca + 2, 0); put_word_ce (srca, 0 >> 16);
}}}}	m68k_incpc (2);
endlabel4328: ;
}
/* CLR.L -(An) */
void REGPARAM2 CPUFUNC(op_42a0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg(regs, srcreg) - 4;
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4329;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	m68k_areg (regs, srcreg) = srca;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(0)) == 0);
	SET_NFLG (((uae_s32)(0)) < 0);
	put_word_ce (srca + 2, 0); put_word_ce (srca, 0 >> 16);
}}}}	m68k_incpc (2);
endlabel4329: ;
}
/* CLR.L (d16,An) */
void REGPARAM2 CPUFUNC(op_42a8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4330;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(0)) == 0);
	SET_NFLG (((uae_s32)(0)) < 0);
	put_word_ce (srca + 2, 0); put_word_ce (srca, 0 >> 16);
}}}}	m68k_incpc (4);
endlabel4330: ;
}
/* CLR.L (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_42b0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4331;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(0)) == 0);
	SET_NFLG (((uae_s32)(0)) < 0);
	put_word_ce (srca + 2, 0); put_word_ce (srca, 0 >> 16);
}}}}	m68k_incpc (4);
endlabel4331: ;
}
/* CLR.L (xxx).W */
void REGPARAM2 CPUFUNC(op_42b8_6)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4332;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(0)) == 0);
	SET_NFLG (((uae_s32)(0)) < 0);
	put_word_ce (srca + 2, 0); put_word_ce (srca, 0 >> 16);
}}}}	m68k_incpc (4);
endlabel4332: ;
}
/* CLR.L (xxx).L */
void REGPARAM2 CPUFUNC(op_42b9_6)(uae_u32 opcode)
{
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4333;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(0)) == 0);
	SET_NFLG (((uae_s32)(0)) < 0);
	put_word_ce (srca + 2, 0); put_word_ce (srca, 0 >> 16);
}}}}	m68k_incpc (6);
endlabel4333: ;
}
/* NEG.B Dn */
void REGPARAM2 CPUFUNC(op_4400_6)(uae_u32 opcode)
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
}
/* NEG.B (An) */
void REGPARAM2 CPUFUNC(op_4410_6)(uae_u32 opcode)
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
}
/* NEG.B (An)+ */
void REGPARAM2 CPUFUNC(op_4418_6)(uae_u32 opcode)
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
}
/* NEG.B -(An) */
void REGPARAM2 CPUFUNC(op_4420_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
	do_cycles_ce (2 * 256);
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
}
/* NEG.B (d16,An) */
void REGPARAM2 CPUFUNC(op_4428_6)(uae_u32 opcode)
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
}
/* NEG.B (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_4430_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
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
}
/* NEG.B (xxx).W */
void REGPARAM2 CPUFUNC(op_4438_6)(uae_u32 opcode)
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
}
/* NEG.B (xxx).L */
void REGPARAM2 CPUFUNC(op_4439_6)(uae_u32 opcode)
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
}
/* NEG.W Dn */
void REGPARAM2 CPUFUNC(op_4440_6)(uae_u32 opcode)
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
}
/* NEG.W (An) */
void REGPARAM2 CPUFUNC(op_4450_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4343;
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
endlabel4343: ;
}
/* NEG.W (An)+ */
void REGPARAM2 CPUFUNC(op_4458_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4344;
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
endlabel4344: ;
}
/* NEG.W -(An) */
void REGPARAM2 CPUFUNC(op_4460_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg(regs, srcreg) - 2;
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4345;
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
endlabel4345: ;
}
/* NEG.W (d16,An) */
void REGPARAM2 CPUFUNC(op_4468_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4346;
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
endlabel4346: ;
}
/* NEG.W (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_4470_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4347;
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
endlabel4347: ;
}
/* NEG.W (xxx).W */
void REGPARAM2 CPUFUNC(op_4478_6)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4348;
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
endlabel4348: ;
}
/* NEG.W (xxx).L */
void REGPARAM2 CPUFUNC(op_4479_6)(uae_u32 opcode)
{
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4349;
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
endlabel4349: ;
}
/* NEG.L Dn */
void REGPARAM2 CPUFUNC(op_4480_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s32 src = m68k_dreg(regs, srcreg);
	do_cycles_ce (2 * 256);
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
	m68k_dreg(regs, srcreg) = (dst);
}}}}}	m68k_incpc (2);
}
/* NEG.L (An) */
void REGPARAM2 CPUFUNC(op_4490_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4351;
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
	put_word_ce (srca + 2, dst); put_word_ce (srca, dst >> 16);
}}}}}}}	m68k_incpc (2);
endlabel4351: ;
}
/* NEG.L (An)+ */
void REGPARAM2 CPUFUNC(op_4498_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4352;
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
	put_word_ce (srca + 2, dst); put_word_ce (srca, dst >> 16);
}}}}}}}	m68k_incpc (2);
endlabel4352: ;
}
/* NEG.L -(An) */
void REGPARAM2 CPUFUNC(op_44a0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg(regs, srcreg) - 4;
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4353;
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
	put_word_ce (srca + 2, dst); put_word_ce (srca, dst >> 16);
}}}}}}}	m68k_incpc (2);
endlabel4353: ;
}
/* NEG.L (d16,An) */
void REGPARAM2 CPUFUNC(op_44a8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4354;
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
	put_word_ce (srca + 2, dst); put_word_ce (srca, dst >> 16);
}}}}}}}	m68k_incpc (4);
endlabel4354: ;
}
/* NEG.L (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_44b0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4355;
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
	put_word_ce (srca + 2, dst); put_word_ce (srca, dst >> 16);
}}}}}}}	m68k_incpc (4);
endlabel4355: ;
}
/* NEG.L (xxx).W */
void REGPARAM2 CPUFUNC(op_44b8_6)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4356;
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
	put_word_ce (srca + 2, dst); put_word_ce (srca, dst >> 16);
}}}}}}}	m68k_incpc (4);
endlabel4356: ;
}
/* NEG.L (xxx).L */
void REGPARAM2 CPUFUNC(op_44b9_6)(uae_u32 opcode)
{
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4357;
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
	put_word_ce (srca + 2, dst); put_word_ce (srca, dst >> 16);
}}}}}}}	m68k_incpc (6);
endlabel4357: ;
}
/* MV2SR.B Dn */
void REGPARAM2 CPUFUNC(op_44c0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_dreg(regs, srcreg);
	do_cycles_ce (8 * 256);
	MakeSR();
	regs.sr &= 0xFF00;
	regs.sr |= src & 0xFF;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	MakeFromSR();
}}	m68k_incpc (2);
}
/* MV2SR.B (An) */
void REGPARAM2 CPUFUNC(op_44d0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4359;
	}
{{	uae_s16 src = get_word_ce (srca);
	do_cycles_ce (8 * 256);
	MakeSR();
	regs.sr &= 0xFF00;
	regs.sr |= src & 0xFF;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	MakeFromSR();
}}}}	m68k_incpc (2);
endlabel4359: ;
}
/* MV2SR.B (An)+ */
void REGPARAM2 CPUFUNC(op_44d8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4360;
	}
{{	uae_s16 src = get_word_ce (srca);
	m68k_areg(regs, srcreg) += 2;
	do_cycles_ce (8 * 256);
	MakeSR();
	regs.sr &= 0xFF00;
	regs.sr |= src & 0xFF;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	MakeFromSR();
}}}}	m68k_incpc (2);
endlabel4360: ;
}
/* MV2SR.B -(An) */
void REGPARAM2 CPUFUNC(op_44e0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg(regs, srcreg) - 2;
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4361;
	}
{{	uae_s16 src = get_word_ce (srca);
	m68k_areg (regs, srcreg) = srca;
	do_cycles_ce (8 * 256);
	MakeSR();
	regs.sr &= 0xFF00;
	regs.sr |= src & 0xFF;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	MakeFromSR();
}}}}	m68k_incpc (2);
endlabel4361: ;
}
/* MV2SR.B (d16,An) */
void REGPARAM2 CPUFUNC(op_44e8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4362;
	}
{{	uae_s16 src = get_word_ce (srca);
	do_cycles_ce (8 * 256);
	MakeSR();
	regs.sr &= 0xFF00;
	regs.sr |= src & 0xFF;
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	MakeFromSR();
}}}}	m68k_incpc (4);
endlabel4362: ;
}
/* MV2SR.B (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_44f0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4363;
	}
{{	uae_s16 src = get_word_ce (srca);
	do_cycles_ce (8 * 256);
	MakeSR();
	regs.sr &= 0xFF00;
	regs.sr |= src & 0xFF;
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	MakeFromSR();
}}}}	m68k_incpc (4);
endlabel4363: ;
}
/* MV2SR.B (xxx).W */
void REGPARAM2 CPUFUNC(op_44f8_6)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4364;
	}
{{	uae_s16 src = get_word_ce (srca);
	do_cycles_ce (8 * 256);
	MakeSR();
	regs.sr &= 0xFF00;
	regs.sr |= src & 0xFF;
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	MakeFromSR();
}}}}	m68k_incpc (4);
endlabel4364: ;
}
/* MV2SR.B (xxx).L */
void REGPARAM2 CPUFUNC(op_44f9_6)(uae_u32 opcode)
{
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4365;
	}
{{	uae_s16 src = get_word_ce (srca);
	do_cycles_ce (8 * 256);
	MakeSR();
	regs.sr &= 0xFF00;
	regs.sr |= src & 0xFF;
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	MakeFromSR();
}}}}	m68k_incpc (6);
endlabel4365: ;
}
/* MV2SR.B (d16,PC) */
void REGPARAM2 CPUFUNC(op_44fa_6)(uae_u32 opcode)
{
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4366;
	}
{{	uae_s16 src = get_word_ce (srca);
	do_cycles_ce (8 * 256);
	MakeSR();
	regs.sr &= 0xFF00;
	regs.sr |= src & 0xFF;
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	MakeFromSR();
}}}}	m68k_incpc (4);
endlabel4366: ;
}
/* MV2SR.B (d8,PC,Xn) */
void REGPARAM2 CPUFUNC(op_44fb_6)(uae_u32 opcode)
{
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4367;
	}
{{	uae_s16 src = get_word_ce (srca);
	do_cycles_ce (8 * 256);
	MakeSR();
	regs.sr &= 0xFF00;
	regs.sr |= src & 0xFF;
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	MakeFromSR();
}}}}	m68k_incpc (4);
endlabel4367: ;
}
/* MV2SR.B #<data>.B */
void REGPARAM2 CPUFUNC(op_44fc_6)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce_prefetch (4);
	do_cycles_ce (8 * 256);
	MakeSR();
	regs.sr &= 0xFF00;
	regs.sr |= src & 0xFF;
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	MakeFromSR();
}}	m68k_incpc (4);
}
/* NOT.B Dn */
void REGPARAM2 CPUFUNC(op_4600_6)(uae_u32 opcode)
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
}
/* NOT.B (An) */
void REGPARAM2 CPUFUNC(op_4610_6)(uae_u32 opcode)
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
}
/* NOT.B (An)+ */
void REGPARAM2 CPUFUNC(op_4618_6)(uae_u32 opcode)
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
}
/* NOT.B -(An) */
void REGPARAM2 CPUFUNC(op_4620_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
	do_cycles_ce (2 * 256);
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
}
/* NOT.B (d16,An) */
void REGPARAM2 CPUFUNC(op_4628_6)(uae_u32 opcode)
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
}
/* NOT.B (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_4630_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
{	uae_s8 src = get_byte_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	uae_u32 dst = ~src;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(dst)) == 0);
	SET_NFLG (((uae_s8)(dst)) < 0);
	put_byte_ce (srca,dst);
}}}}	m68k_incpc (4);
}
/* NOT.B (xxx).W */
void REGPARAM2 CPUFUNC(op_4638_6)(uae_u32 opcode)
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
}
/* NOT.B (xxx).L */
void REGPARAM2 CPUFUNC(op_4639_6)(uae_u32 opcode)
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
}
/* NOT.W Dn */
void REGPARAM2 CPUFUNC(op_4640_6)(uae_u32 opcode)
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
}
/* NOT.W (An) */
void REGPARAM2 CPUFUNC(op_4650_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4378;
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
endlabel4378: ;
}
/* NOT.W (An)+ */
void REGPARAM2 CPUFUNC(op_4658_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4379;
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
endlabel4379: ;
}
/* NOT.W -(An) */
void REGPARAM2 CPUFUNC(op_4660_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg(regs, srcreg) - 2;
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4380;
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
endlabel4380: ;
}
/* NOT.W (d16,An) */
void REGPARAM2 CPUFUNC(op_4668_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4381;
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
endlabel4381: ;
}
/* NOT.W (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_4670_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4382;
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
endlabel4382: ;
}
/* NOT.W (xxx).W */
void REGPARAM2 CPUFUNC(op_4678_6)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4383;
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
endlabel4383: ;
}
/* NOT.W (xxx).L */
void REGPARAM2 CPUFUNC(op_4679_6)(uae_u32 opcode)
{
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4384;
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
endlabel4384: ;
}
/* NOT.L Dn */
void REGPARAM2 CPUFUNC(op_4680_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s32 src = m68k_dreg(regs, srcreg);
	do_cycles_ce (2 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 dst = ~src;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(dst)) == 0);
	SET_NFLG (((uae_s32)(dst)) < 0);
	m68k_dreg(regs, srcreg) = (dst);
}}}	m68k_incpc (2);
}
/* NOT.L (An) */
void REGPARAM2 CPUFUNC(op_4690_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4386;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 dst = ~src;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(dst)) == 0);
	SET_NFLG (((uae_s32)(dst)) < 0);
	put_word_ce (srca + 2, dst); put_word_ce (srca, dst >> 16);
}}}}}	m68k_incpc (2);
endlabel4386: ;
}
/* NOT.L (An)+ */
void REGPARAM2 CPUFUNC(op_4698_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4387;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	m68k_areg(regs, srcreg) += 4;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 dst = ~src;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(dst)) == 0);
	SET_NFLG (((uae_s32)(dst)) < 0);
	put_word_ce (srca + 2, dst); put_word_ce (srca, dst >> 16);
}}}}}	m68k_incpc (2);
endlabel4387: ;
}
/* NOT.L -(An) */
void REGPARAM2 CPUFUNC(op_46a0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg(regs, srcreg) - 4;
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4388;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	m68k_areg (regs, srcreg) = srca;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 dst = ~src;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(dst)) == 0);
	SET_NFLG (((uae_s32)(dst)) < 0);
	put_word_ce (srca + 2, dst); put_word_ce (srca, dst >> 16);
}}}}}	m68k_incpc (2);
endlabel4388: ;
}
/* NOT.L (d16,An) */
void REGPARAM2 CPUFUNC(op_46a8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4389;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	uae_u32 dst = ~src;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(dst)) == 0);
	SET_NFLG (((uae_s32)(dst)) < 0);
	put_word_ce (srca + 2, dst); put_word_ce (srca, dst >> 16);
}}}}}	m68k_incpc (4);
endlabel4389: ;
}
/* NOT.L (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_46b0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4390;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	uae_u32 dst = ~src;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(dst)) == 0);
	SET_NFLG (((uae_s32)(dst)) < 0);
	put_word_ce (srca + 2, dst); put_word_ce (srca, dst >> 16);
}}}}}	m68k_incpc (4);
endlabel4390: ;
}
/* NOT.L (xxx).W */
void REGPARAM2 CPUFUNC(op_46b8_6)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4391;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	uae_u32 dst = ~src;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(dst)) == 0);
	SET_NFLG (((uae_s32)(dst)) < 0);
	put_word_ce (srca + 2, dst); put_word_ce (srca, dst >> 16);
}}}}}	m68k_incpc (4);
endlabel4391: ;
}
/* NOT.L (xxx).L */
void REGPARAM2 CPUFUNC(op_46b9_6)(uae_u32 opcode)
{
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4392;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
{	uae_u32 dst = ~src;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(dst)) == 0);
	SET_NFLG (((uae_s32)(dst)) < 0);
	put_word_ce (srca + 2, dst); put_word_ce (srca, dst >> 16);
}}}}}	m68k_incpc (6);
endlabel4392: ;
}
/* MV2SR.W Dn */
void REGPARAM2 CPUFUNC(op_46c0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception(8,0); goto endlabel4393; }
{{	uae_s16 src = m68k_dreg(regs, srcreg);
	do_cycles_ce (4 * 256);
	regs.sr = src;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	MakeFromSR();
}}}	m68k_incpc (2);
endlabel4393: ;
}
/* MV2SR.W (An) */
void REGPARAM2 CPUFUNC(op_46d0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception(8,0); goto endlabel4394; }
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4394;
	}
{{	uae_s16 src = get_word_ce (srca);
	do_cycles_ce (4 * 256);
	regs.sr = src;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	MakeFromSR();
}}}}}	m68k_incpc (2);
endlabel4394: ;
}
/* MV2SR.W (An)+ */
void REGPARAM2 CPUFUNC(op_46d8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception(8,0); goto endlabel4395; }
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4395;
	}
{{	uae_s16 src = get_word_ce (srca);
	m68k_areg(regs, srcreg) += 2;
	do_cycles_ce (4 * 256);
	regs.sr = src;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	MakeFromSR();
}}}}}	m68k_incpc (2);
endlabel4395: ;
}
/* MV2SR.W -(An) */
void REGPARAM2 CPUFUNC(op_46e0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception(8,0); goto endlabel4396; }
{{	uaecptr srca;
	srca = m68k_areg(regs, srcreg) - 2;
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4396;
	}
{{	uae_s16 src = get_word_ce (srca);
	m68k_areg (regs, srcreg) = srca;
	do_cycles_ce (4 * 256);
	regs.sr = src;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	MakeFromSR();
}}}}}	m68k_incpc (2);
endlabel4396: ;
}
/* MV2SR.W (d16,An) */
void REGPARAM2 CPUFUNC(op_46e8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception(8,0); goto endlabel4397; }
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4397;
	}
{{	uae_s16 src = get_word_ce (srca);
	do_cycles_ce (4 * 256);
	regs.sr = src;
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	MakeFromSR();
}}}}}	m68k_incpc (4);
endlabel4397: ;
}
/* MV2SR.W (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_46f0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception(8,0); goto endlabel4398; }
{{	uaecptr srca;
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4398;
	}
{{	uae_s16 src = get_word_ce (srca);
	do_cycles_ce (4 * 256);
	regs.sr = src;
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	MakeFromSR();
}}}}}	m68k_incpc (4);
endlabel4398: ;
}
/* MV2SR.W (xxx).W */
void REGPARAM2 CPUFUNC(op_46f8_6)(uae_u32 opcode)
{
{if (!regs.s) { Exception(8,0); goto endlabel4399; }
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4399;
	}
{{	uae_s16 src = get_word_ce (srca);
	do_cycles_ce (4 * 256);
	regs.sr = src;
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	MakeFromSR();
}}}}}	m68k_incpc (4);
endlabel4399: ;
}
/* MV2SR.W (xxx).L */
void REGPARAM2 CPUFUNC(op_46f9_6)(uae_u32 opcode)
{
{if (!regs.s) { Exception(8,0); goto endlabel4400; }
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4400;
	}
{{	uae_s16 src = get_word_ce (srca);
	do_cycles_ce (4 * 256);
	regs.sr = src;
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	MakeFromSR();
}}}}}	m68k_incpc (6);
endlabel4400: ;
}
/* MV2SR.W (d16,PC) */
void REGPARAM2 CPUFUNC(op_46fa_6)(uae_u32 opcode)
{
{if (!regs.s) { Exception(8,0); goto endlabel4401; }
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4401;
	}
{{	uae_s16 src = get_word_ce (srca);
	do_cycles_ce (4 * 256);
	regs.sr = src;
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	MakeFromSR();
}}}}}	m68k_incpc (4);
endlabel4401: ;
}
/* MV2SR.W (d8,PC,Xn) */
void REGPARAM2 CPUFUNC(op_46fb_6)(uae_u32 opcode)
{
{if (!regs.s) { Exception(8,0); goto endlabel4402; }
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4402;
	}
{{	uae_s16 src = get_word_ce (srca);
	do_cycles_ce (4 * 256);
	regs.sr = src;
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	MakeFromSR();
}}}}}	m68k_incpc (4);
endlabel4402: ;
}
/* MV2SR.W #<data>.W */
void REGPARAM2 CPUFUNC(op_46fc_6)(uae_u32 opcode)
{
{if (!regs.s) { Exception(8,0); goto endlabel4403; }
{{	uae_s16 src = get_word_ce_prefetch (4);
	do_cycles_ce (4 * 256);
	regs.sr = src;
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	MakeFromSR();
}}}	m68k_incpc (4);
endlabel4403: ;
}
/* NBCD.B Dn */
void REGPARAM2 CPUFUNC(op_4800_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s8 src = m68k_dreg(regs, srcreg);
	do_cycles_ce (2 * 256);
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
	m68k_dreg(regs, srcreg) = (m68k_dreg(regs, srcreg) & ~0xff) | ((newv) & 0xff);
}}}	m68k_incpc (2);
}
/* NBCD.B (An) */
void REGPARAM2 CPUFUNC(op_4810_6)(uae_u32 opcode)
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
}
/* NBCD.B (An)+ */
void REGPARAM2 CPUFUNC(op_4818_6)(uae_u32 opcode)
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
}
/* NBCD.B -(An) */
void REGPARAM2 CPUFUNC(op_4820_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
	do_cycles_ce (2 * 256);
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
}
/* NBCD.B (d16,An) */
void REGPARAM2 CPUFUNC(op_4828_6)(uae_u32 opcode)
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
}
/* NBCD.B (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_4830_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
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
}
/* NBCD.B (xxx).W */
void REGPARAM2 CPUFUNC(op_4838_6)(uae_u32 opcode)
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
}
/* NBCD.B (xxx).L */
void REGPARAM2 CPUFUNC(op_4839_6)(uae_u32 opcode)
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
}
/* SWAP.W Dn */
void REGPARAM2 CPUFUNC(op_4840_6)(uae_u32 opcode)
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
}
/* PEA.L (An) */
void REGPARAM2 CPUFUNC(op_4850_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg(regs, 7) - 4;
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4413;
	}
{	m68k_areg (regs, 7) = dsta;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	put_word_ce (dsta, srca >> 16); put_word_ce (dsta + 2, srca);
}}}}	m68k_incpc (2);
endlabel4413: ;
}
/* PEA.L (d16,An) */
void REGPARAM2 CPUFUNC(op_4868_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uaecptr dsta;
	dsta = m68k_areg(regs, 7) - 4;
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4414;
	}
{	m68k_areg (regs, 7) = dsta;
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	put_word_ce (dsta, srca >> 16); put_word_ce (dsta + 2, srca);
}}}}	m68k_incpc (4);
endlabel4414: ;
}
/* PEA.L (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_4870_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
{	uaecptr dsta;
	dsta = m68k_areg(regs, 7) - 4;
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4415;
	}
{	m68k_areg (regs, 7) = dsta;
	do_cycles_ce (4 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	put_word_ce (dsta, srca >> 16); put_word_ce (dsta + 2, srca);
}}}}	m68k_incpc (4);
endlabel4415: ;
}
/* PEA.L (xxx).W */
void REGPARAM2 CPUFUNC(op_4878_6)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uaecptr dsta;
	dsta = m68k_areg(regs, 7) - 4;
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4416;
	}
{	m68k_areg (regs, 7) = dsta;
	put_word_ce (dsta, srca >> 16); put_word_ce (dsta + 2, srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
}}}}	m68k_incpc (4);
endlabel4416: ;
}
/* PEA.L (xxx).L */
void REGPARAM2 CPUFUNC(op_4879_6)(uae_u32 opcode)
{
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
{	uaecptr dsta;
	dsta = m68k_areg(regs, 7) - 4;
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 6, dsta);
		goto endlabel4417;
	}
{	m68k_areg (regs, 7) = dsta;
	put_word_ce (dsta, srca >> 16); put_word_ce (dsta + 2, srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
}}}}	m68k_incpc (6);
endlabel4417: ;
}
/* PEA.L (d16,PC) */
void REGPARAM2 CPUFUNC(op_487a_6)(uae_u32 opcode)
{
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uaecptr dsta;
	dsta = m68k_areg(regs, 7) - 4;
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4418;
	}
{	m68k_areg (regs, 7) = dsta;
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	put_word_ce (dsta, srca >> 16); put_word_ce (dsta + 2, srca);
}}}}	m68k_incpc (4);
endlabel4418: ;
}
/* PEA.L (d8,PC,Xn) */
void REGPARAM2 CPUFUNC(op_487b_6)(uae_u32 opcode)
{
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
{	uaecptr dsta;
	dsta = m68k_areg(regs, 7) - 4;
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 4, dsta);
		goto endlabel4419;
	}
{	m68k_areg (regs, 7) = dsta;
	do_cycles_ce (4 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	put_word_ce (dsta, srca >> 16); put_word_ce (dsta + 2, srca);
}}}}	m68k_incpc (4);
endlabel4419: ;
}
/* EXT.W Dn */
void REGPARAM2 CPUFUNC(op_4880_6)(uae_u32 opcode)
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
}
/* MVMLE.W #<data>.W,(An) */
void REGPARAM2 CPUFUNC(op_4890_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{	uae_u16 mask = get_word_ce_prefetch (4);
{	uaecptr srca = m68k_areg(regs, dstreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 4, srca);
		goto endlabel4421;
	}
{{	uae_u16 dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
	while (dmask) { put_word_ce (srca, m68k_dreg(regs, movem_index1[dmask])); srca += 2; dmask = movem_next[dmask]; }
	while (amask) { put_word_ce (srca, m68k_areg(regs, movem_index1[amask])); srca += 2; amask = movem_next[amask]; }
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
}}}}	m68k_incpc (4);
endlabel4421: ;
}
/* MVMLE.W #<data>.W,-(An) */
void REGPARAM2 CPUFUNC(op_48a0_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{	uae_u16 mask = get_word_ce_prefetch (4);
{	uaecptr srca;
	srca = m68k_areg(regs, dstreg) - 0;
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 4, srca);
		goto endlabel4422;
	}
{{	uae_u16 amask = mask & 0xff, dmask = (mask >> 8) & 0xff;
	while (amask) { srca -= 2; put_word_ce (srca, m68k_areg(regs, movem_index2[amask])); amask = movem_next[amask]; }
	while (dmask) { srca -= 2; put_word_ce (srca, m68k_dreg(regs, movem_index2[dmask])); dmask = movem_next[dmask]; }
	m68k_areg(regs, dstreg) = srca;
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
}}}}	m68k_incpc (4);
endlabel4422: ;
}
/* MVMLE.W #<data>.W,(d16,An) */
void REGPARAM2 CPUFUNC(op_48a8_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{	uae_u16 mask = get_word_ce_prefetch (4);
{	uaecptr srca = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 4, srca);
		goto endlabel4423;
	}
{{	uae_u16 dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
	while (dmask) { put_word_ce (srca, m68k_dreg(regs, movem_index1[dmask])); srca += 2; dmask = movem_next[dmask]; }
	while (amask) { put_word_ce (srca, m68k_areg(regs, movem_index1[amask])); srca += 2; amask = movem_next[amask]; }
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
}}}}	m68k_incpc (6);
endlabel4423: ;
}
/* MVMLE.W #<data>.W,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_48b0_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{	uae_u16 mask = get_word_ce_prefetch (4);
{	uaecptr srca;
	srca = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (6));
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 4, srca);
		goto endlabel4424;
	}
{	do_cycles_ce (2 * 256);
{	uae_u16 dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
	while (dmask) { put_word_ce (srca, m68k_dreg(regs, movem_index1[dmask])); srca += 2; dmask = movem_next[dmask]; }
	while (amask) { put_word_ce (srca, m68k_areg(regs, movem_index1[amask])); srca += 2; amask = movem_next[amask]; }
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
}}}}	m68k_incpc (6);
endlabel4424: ;
}
/* MVMLE.W #<data>.W,(xxx).W */
void REGPARAM2 CPUFUNC(op_48b8_6)(uae_u32 opcode)
{
{	uae_u16 mask = get_word_ce_prefetch (4);
{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 4, srca);
		goto endlabel4425;
	}
{{	uae_u16 dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
	while (dmask) { put_word_ce (srca, m68k_dreg(regs, movem_index1[dmask])); srca += 2; dmask = movem_next[dmask]; }
	while (amask) { put_word_ce (srca, m68k_areg(regs, movem_index1[amask])); srca += 2; amask = movem_next[amask]; }
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
}}}}	m68k_incpc (6);
endlabel4425: ;
}
/* MVMLE.W #<data>.W,(xxx).L */
void REGPARAM2 CPUFUNC(op_48b9_6)(uae_u32 opcode)
{
{	uae_u16 mask = get_word_ce_prefetch (4);
{	uaecptr srca;
	srca = get_word_ce_prefetch (6) << 16;
	srca |= get_word_ce_prefetch (8);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 4, srca);
		goto endlabel4426;
	}
{{	uae_u16 dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
	while (dmask) { put_word_ce (srca, m68k_dreg(regs, movem_index1[dmask])); srca += 2; dmask = movem_next[dmask]; }
	while (amask) { put_word_ce (srca, m68k_areg(regs, movem_index1[amask])); srca += 2; amask = movem_next[amask]; }
	regs.ir = regs.irc;
	get_word_ce_prefetch (10);
}}}}	m68k_incpc (8);
endlabel4426: ;
}
/* EXT.L Dn */
void REGPARAM2 CPUFUNC(op_48c0_6)(uae_u32 opcode)
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
}
/* MVMLE.L #<data>.W,(An) */
void REGPARAM2 CPUFUNC(op_48d0_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{	uae_u16 mask = get_word_ce_prefetch (4);
{	uaecptr srca = m68k_areg(regs, dstreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 4, srca);
		goto endlabel4428;
	}
{{	uae_u16 dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
	while (dmask) { put_word_ce (srca, m68k_dreg(regs, movem_index1[dmask]) >> 16); put_word_ce (srca + 2, m68k_dreg(regs, movem_index1[dmask])); srca += 4; dmask = movem_next[dmask]; }
	while (amask) { put_word_ce (srca, m68k_areg(regs, movem_index1[amask]) >> 16); put_word_ce (srca + 2, m68k_areg(regs, movem_index1[amask])); srca += 4; amask = movem_next[amask]; }
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
}}}}	m68k_incpc (4);
endlabel4428: ;
}
/* MVMLE.L #<data>.W,-(An) */
void REGPARAM2 CPUFUNC(op_48e0_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{	uae_u16 mask = get_word_ce_prefetch (4);
{	uaecptr srca;
	srca = m68k_areg(regs, dstreg) - 0;
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 4, srca);
		goto endlabel4429;
	}
{{	uae_u16 amask = mask & 0xff, dmask = (mask >> 8) & 0xff;
	while (amask) { srca -= 4; put_word_ce (srca, m68k_areg(regs, movem_index2[amask]) >> 16); put_word_ce (srca + 2, m68k_areg(regs, movem_index2[amask])); amask = movem_next[amask]; }
	while (dmask) { srca -= 4; put_word_ce (srca, m68k_dreg(regs, movem_index2[dmask]) >> 16); put_word_ce (srca + 2, m68k_dreg(regs, movem_index2[dmask])); dmask = movem_next[dmask]; }
	m68k_areg(regs, dstreg) = srca;
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
}}}}	m68k_incpc (4);
endlabel4429: ;
}
/* MVMLE.L #<data>.W,(d16,An) */
void REGPARAM2 CPUFUNC(op_48e8_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{	uae_u16 mask = get_word_ce_prefetch (4);
{	uaecptr srca = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 4, srca);
		goto endlabel4430;
	}
{{	uae_u16 dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
	while (dmask) { put_word_ce (srca, m68k_dreg(regs, movem_index1[dmask]) >> 16); put_word_ce (srca + 2, m68k_dreg(regs, movem_index1[dmask])); srca += 4; dmask = movem_next[dmask]; }
	while (amask) { put_word_ce (srca, m68k_areg(regs, movem_index1[amask]) >> 16); put_word_ce (srca + 2, m68k_areg(regs, movem_index1[amask])); srca += 4; amask = movem_next[amask]; }
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
}}}}	m68k_incpc (6);
endlabel4430: ;
}
/* MVMLE.L #<data>.W,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_48f0_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{	uae_u16 mask = get_word_ce_prefetch (4);
{	uaecptr srca;
	srca = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (6));
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 4, srca);
		goto endlabel4431;
	}
{	do_cycles_ce (2 * 256);
{	uae_u16 dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
	while (dmask) { put_word_ce (srca, m68k_dreg(regs, movem_index1[dmask]) >> 16); put_word_ce (srca + 2, m68k_dreg(regs, movem_index1[dmask])); srca += 4; dmask = movem_next[dmask]; }
	while (amask) { put_word_ce (srca, m68k_areg(regs, movem_index1[amask]) >> 16); put_word_ce (srca + 2, m68k_areg(regs, movem_index1[amask])); srca += 4; amask = movem_next[amask]; }
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
}}}}	m68k_incpc (6);
endlabel4431: ;
}
/* MVMLE.L #<data>.W,(xxx).W */
void REGPARAM2 CPUFUNC(op_48f8_6)(uae_u32 opcode)
{
{	uae_u16 mask = get_word_ce_prefetch (4);
{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 4, srca);
		goto endlabel4432;
	}
{{	uae_u16 dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
	while (dmask) { put_word_ce (srca, m68k_dreg(regs, movem_index1[dmask]) >> 16); put_word_ce (srca + 2, m68k_dreg(regs, movem_index1[dmask])); srca += 4; dmask = movem_next[dmask]; }
	while (amask) { put_word_ce (srca, m68k_areg(regs, movem_index1[amask]) >> 16); put_word_ce (srca + 2, m68k_areg(regs, movem_index1[amask])); srca += 4; amask = movem_next[amask]; }
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
}}}}	m68k_incpc (6);
endlabel4432: ;
}
/* MVMLE.L #<data>.W,(xxx).L */
void REGPARAM2 CPUFUNC(op_48f9_6)(uae_u32 opcode)
{
{	uae_u16 mask = get_word_ce_prefetch (4);
{	uaecptr srca;
	srca = get_word_ce_prefetch (6) << 16;
	srca |= get_word_ce_prefetch (8);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 4, srca);
		goto endlabel4433;
	}
{{	uae_u16 dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
	while (dmask) { put_word_ce (srca, m68k_dreg(regs, movem_index1[dmask]) >> 16); put_word_ce (srca + 2, m68k_dreg(regs, movem_index1[dmask])); srca += 4; dmask = movem_next[dmask]; }
	while (amask) { put_word_ce (srca, m68k_areg(regs, movem_index1[amask]) >> 16); put_word_ce (srca + 2, m68k_areg(regs, movem_index1[amask])); srca += 4; amask = movem_next[amask]; }
	regs.ir = regs.irc;
	get_word_ce_prefetch (10);
}}}}	m68k_incpc (8);
endlabel4433: ;
}
/* EXT.B Dn */
void REGPARAM2 CPUFUNC(op_49c0_6)(uae_u32 opcode)
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
}
/* TST.B Dn */
void REGPARAM2 CPUFUNC(op_4a00_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s8 src = m68k_dreg(regs, srcreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
}}	m68k_incpc (2);
}
/* TST.B (An) */
void REGPARAM2 CPUFUNC(op_4a10_6)(uae_u32 opcode)
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
}
/* TST.B (An)+ */
void REGPARAM2 CPUFUNC(op_4a18_6)(uae_u32 opcode)
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
}
/* TST.B -(An) */
void REGPARAM2 CPUFUNC(op_4a20_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
	do_cycles_ce (2 * 256);
{	uae_s8 src = get_byte_ce (srca);
	m68k_areg (regs, srcreg) = srca;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
}}}	m68k_incpc (2);
}
/* TST.B (d16,An) */
void REGPARAM2 CPUFUNC(op_4a28_6)(uae_u32 opcode)
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
}
/* TST.B (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_4a30_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
{	uae_s8 src = get_byte_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
}}}	m68k_incpc (4);
}
/* TST.B (xxx).W */
void REGPARAM2 CPUFUNC(op_4a38_6)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 src = get_byte_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
}}}	m68k_incpc (4);
}
/* TST.B (xxx).L */
void REGPARAM2 CPUFUNC(op_4a39_6)(uae_u32 opcode)
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
}
/* TST.W Dn */
void REGPARAM2 CPUFUNC(op_4a40_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_dreg(regs, srcreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
}}	m68k_incpc (2);
}
/* TST.W (An) */
void REGPARAM2 CPUFUNC(op_4a50_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4444;
	}
{{	uae_s16 src = get_word_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
}}}}	m68k_incpc (2);
endlabel4444: ;
}
/* TST.W (An)+ */
void REGPARAM2 CPUFUNC(op_4a58_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4445;
	}
{{	uae_s16 src = get_word_ce (srca);
	m68k_areg(regs, srcreg) += 2;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
}}}}	m68k_incpc (2);
endlabel4445: ;
}
/* TST.W -(An) */
void REGPARAM2 CPUFUNC(op_4a60_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg(regs, srcreg) - 2;
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4446;
	}
{{	uae_s16 src = get_word_ce (srca);
	m68k_areg (regs, srcreg) = srca;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
}}}}	m68k_incpc (2);
endlabel4446: ;
}
/* TST.W (d16,An) */
void REGPARAM2 CPUFUNC(op_4a68_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4447;
	}
{{	uae_s16 src = get_word_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
}}}}	m68k_incpc (4);
endlabel4447: ;
}
/* TST.W (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_4a70_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4448;
	}
{{	uae_s16 src = get_word_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
}}}}	m68k_incpc (4);
endlabel4448: ;
}
/* TST.W (xxx).W */
void REGPARAM2 CPUFUNC(op_4a78_6)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4449;
	}
{{	uae_s16 src = get_word_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
}}}}	m68k_incpc (4);
endlabel4449: ;
}
/* TST.W (xxx).L */
void REGPARAM2 CPUFUNC(op_4a79_6)(uae_u32 opcode)
{
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4450;
	}
{{	uae_s16 src = get_word_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(src)) == 0);
	SET_NFLG (((uae_s16)(src)) < 0);
}}}}	m68k_incpc (6);
endlabel4450: ;
}
/* TST.L Dn */
void REGPARAM2 CPUFUNC(op_4a80_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s32 src = m68k_dreg(regs, srcreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
}}	m68k_incpc (2);
}
/* TST.L (An) */
void REGPARAM2 CPUFUNC(op_4a90_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4452;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
}}}}	m68k_incpc (2);
endlabel4452: ;
}
/* TST.L (An)+ */
void REGPARAM2 CPUFUNC(op_4a98_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4453;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	m68k_areg(regs, srcreg) += 4;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
}}}}	m68k_incpc (2);
endlabel4453: ;
}
/* TST.L -(An) */
void REGPARAM2 CPUFUNC(op_4aa0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg(regs, srcreg) - 4;
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4454;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	m68k_areg (regs, srcreg) = srca;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
}}}}	m68k_incpc (2);
endlabel4454: ;
}
/* TST.L (d16,An) */
void REGPARAM2 CPUFUNC(op_4aa8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4455;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
}}}}	m68k_incpc (4);
endlabel4455: ;
}
/* TST.L (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_4ab0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4456;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
}}}}	m68k_incpc (4);
endlabel4456: ;
}
/* TST.L (xxx).W */
void REGPARAM2 CPUFUNC(op_4ab8_6)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4457;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
}}}}	m68k_incpc (4);
endlabel4457: ;
}
/* TST.L (xxx).L */
void REGPARAM2 CPUFUNC(op_4ab9_6)(uae_u32 opcode)
{
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4458;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
}}}}	m68k_incpc (6);
endlabel4458: ;
}
/* TAS.B Dn */
void REGPARAM2 CPUFUNC(op_4ac0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s8 src = m68k_dreg(regs, srcreg);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	src |= 0x80;
	m68k_dreg(regs, srcreg) = (m68k_dreg(regs, srcreg) & ~0xff) | ((src) & 0xff);
}}	m68k_incpc (2);
}
/* TAS.B (An) */
void REGPARAM2 CPUFUNC(op_4ad0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
{	uae_s8 src = get_byte_ce (srca);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	do_cycles_ce (2 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	src |= 0x80;
	put_byte_ce (srca,src);
}}}	m68k_incpc (2);
}
/* TAS.B (An)+ */
void REGPARAM2 CPUFUNC(op_4ad8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
{	uae_s8 src = get_byte_ce (srca);
	m68k_areg(regs, srcreg) += areg_byteinc[srcreg];
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	do_cycles_ce (2 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	src |= 0x80;
	put_byte_ce (srca,src);
}}}	m68k_incpc (2);
}
/* TAS.B -(An) */
void REGPARAM2 CPUFUNC(op_4ae0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
	do_cycles_ce (2 * 256);
{	uae_s8 src = get_byte_ce (srca);
	m68k_areg (regs, srcreg) = srca;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	do_cycles_ce (2 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	src |= 0x80;
	put_byte_ce (srca,src);
}}}	m68k_incpc (2);
}
/* TAS.B (d16,An) */
void REGPARAM2 CPUFUNC(op_4ae8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 src = get_byte_ce (srca);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	do_cycles_ce (2 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	src |= 0x80;
	put_byte_ce (srca,src);
}}}	m68k_incpc (4);
}
/* TAS.B (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_4af0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
{	uae_s8 src = get_byte_ce (srca);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	do_cycles_ce (2 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	src |= 0x80;
	put_byte_ce (srca,src);
}}}	m68k_incpc (4);
}
/* TAS.B (xxx).W */
void REGPARAM2 CPUFUNC(op_4af8_6)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 src = get_byte_ce (srca);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	do_cycles_ce (2 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	src |= 0x80;
	put_byte_ce (srca,src);
}}}	m68k_incpc (4);
}
/* TAS.B (xxx).L */
void REGPARAM2 CPUFUNC(op_4af9_6)(uae_u32 opcode)
{
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
{	uae_s8 src = get_byte_ce (srca);
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	do_cycles_ce (2 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	src |= 0x80;
	put_byte_ce (srca,src);
}}}	m68k_incpc (6);
}
/* MVMEL.W #<data>.W,(An) */
void REGPARAM2 CPUFUNC(op_4c90_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{	uae_u16 mask = get_word_ce_prefetch (4);
	unsigned int dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
	uae_u32 v;
{	uaecptr srca = m68k_areg(regs, dstreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 4, srca);
		goto endlabel4467;
	}
{{	while (dmask) { m68k_dreg(regs, movem_index1[dmask]) = (uae_s32)(uae_s16)get_word_ce(srca); srca += 2; dmask = movem_next[dmask]; }
	while (amask) { m68k_areg(regs, movem_index1[amask]) = (uae_s32)(uae_s16)get_word_ce(srca); srca += 2; amask = movem_next[amask]; }
	get_word_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
}}}}	m68k_incpc (4);
endlabel4467: ;
}
/* MVMEL.W #<data>.W,(An)+ */
void REGPARAM2 CPUFUNC(op_4c98_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{	uae_u16 mask = get_word_ce_prefetch (4);
	unsigned int dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
	uae_u32 v;
{	uaecptr srca = m68k_areg(regs, dstreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 4, srca);
		goto endlabel4468;
	}
{{	while (dmask) { m68k_dreg(regs, movem_index1[dmask]) = (uae_s32)(uae_s16)get_word_ce(srca); srca += 2; dmask = movem_next[dmask]; }
	while (amask) { m68k_areg(regs, movem_index1[amask]) = (uae_s32)(uae_s16)get_word_ce(srca); srca += 2; amask = movem_next[amask]; }
	get_word_ce (srca);
	m68k_areg(regs, dstreg) = srca;
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
}}}}	m68k_incpc (4);
endlabel4468: ;
}
/* MVMEL.W #<data>.W,(d16,An) */
void REGPARAM2 CPUFUNC(op_4ca8_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{	uae_u16 mask = get_word_ce_prefetch (4);
	unsigned int dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
	uae_u32 v;
{	uaecptr srca = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 4, srca);
		goto endlabel4469;
	}
{{	while (dmask) { m68k_dreg(regs, movem_index1[dmask]) = (uae_s32)(uae_s16)get_word_ce(srca); srca += 2; dmask = movem_next[dmask]; }
	while (amask) { m68k_areg(regs, movem_index1[amask]) = (uae_s32)(uae_s16)get_word_ce(srca); srca += 2; amask = movem_next[amask]; }
	get_word_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
}}}}	m68k_incpc (6);
endlabel4469: ;
}
/* MVMEL.W #<data>.W,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_4cb0_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{	uae_u16 mask = get_word_ce_prefetch (4);
	unsigned int dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
	uae_u32 v;
{	uaecptr srca;
	srca = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (6));
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 4, srca);
		goto endlabel4470;
	}
{	do_cycles_ce (2 * 256);
{	while (dmask) { m68k_dreg(regs, movem_index1[dmask]) = (uae_s32)(uae_s16)get_word_ce(srca); srca += 2; dmask = movem_next[dmask]; }
	while (amask) { m68k_areg(regs, movem_index1[amask]) = (uae_s32)(uae_s16)get_word_ce(srca); srca += 2; amask = movem_next[amask]; }
	get_word_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
}}}}	m68k_incpc (6);
endlabel4470: ;
}
/* MVMEL.W #<data>.W,(xxx).W */
void REGPARAM2 CPUFUNC(op_4cb8_6)(uae_u32 opcode)
{
{	uae_u16 mask = get_word_ce_prefetch (4);
	unsigned int dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
	uae_u32 v;
{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 4, srca);
		goto endlabel4471;
	}
{{	while (dmask) { m68k_dreg(regs, movem_index1[dmask]) = (uae_s32)(uae_s16)get_word_ce(srca); srca += 2; dmask = movem_next[dmask]; }
	while (amask) { m68k_areg(regs, movem_index1[amask]) = (uae_s32)(uae_s16)get_word_ce(srca); srca += 2; amask = movem_next[amask]; }
	get_word_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
}}}}	m68k_incpc (6);
endlabel4471: ;
}
/* MVMEL.W #<data>.W,(xxx).L */
void REGPARAM2 CPUFUNC(op_4cb9_6)(uae_u32 opcode)
{
{	uae_u16 mask = get_word_ce_prefetch (4);
	unsigned int dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
	uae_u32 v;
{	uaecptr srca;
	srca = get_word_ce_prefetch (6) << 16;
	srca |= get_word_ce_prefetch (8);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 4, srca);
		goto endlabel4472;
	}
{{	while (dmask) { m68k_dreg(regs, movem_index1[dmask]) = (uae_s32)(uae_s16)get_word_ce(srca); srca += 2; dmask = movem_next[dmask]; }
	while (amask) { m68k_areg(regs, movem_index1[amask]) = (uae_s32)(uae_s16)get_word_ce(srca); srca += 2; amask = movem_next[amask]; }
	get_word_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (10);
}}}}	m68k_incpc (8);
endlabel4472: ;
}
/* MVMEL.W #<data>.W,(d16,PC) */
void REGPARAM2 CPUFUNC(op_4cba_6)(uae_u32 opcode)
{
	uae_u32 dstreg = 2;
{	uae_u16 mask = get_word_ce_prefetch (4);
	unsigned int dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
	uae_u32 v;
{	uaecptr srca = m68k_getpc () + 4;
	srca += (uae_s32)(uae_s16)get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 4, srca);
		goto endlabel4473;
	}
{{	while (dmask) { m68k_dreg(regs, movem_index1[dmask]) = (uae_s32)(uae_s16)get_word_ce(srca); srca += 2; dmask = movem_next[dmask]; }
	while (amask) { m68k_areg(regs, movem_index1[amask]) = (uae_s32)(uae_s16)get_word_ce(srca); srca += 2; amask = movem_next[amask]; }
	get_word_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
}}}}	m68k_incpc (6);
endlabel4473: ;
}
/* MVMEL.W #<data>.W,(d8,PC,Xn) */
void REGPARAM2 CPUFUNC(op_4cbb_6)(uae_u32 opcode)
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
		goto endlabel4474;
	}
{	do_cycles_ce (2 * 256);
{	while (dmask) { m68k_dreg(regs, movem_index1[dmask]) = (uae_s32)(uae_s16)get_word_ce(srca); srca += 2; dmask = movem_next[dmask]; }
	while (amask) { m68k_areg(regs, movem_index1[amask]) = (uae_s32)(uae_s16)get_word_ce(srca); srca += 2; amask = movem_next[amask]; }
	get_word_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
}}}}	m68k_incpc (6);
endlabel4474: ;
}
/* MVMEL.L #<data>.W,(An) */
void REGPARAM2 CPUFUNC(op_4cd0_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{	uae_u16 mask = get_word_ce_prefetch (4);
	unsigned int dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
	uae_u32 v;
{	uaecptr srca = m68k_areg(regs, dstreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 4, srca);
		goto endlabel4475;
	}
{{	while (dmask) { v = get_word_ce(srca) << 16; v |= get_word_ce(srca + 2); m68k_dreg(regs, movem_index1[dmask]) = v; srca += 4; dmask = movem_next[dmask]; }
	while (amask) { v = get_word_ce(srca) << 16; v |= get_word_ce(srca + 2); m68k_areg(regs, movem_index1[amask]) = v; srca += 4; amask = movem_next[amask]; }
	get_word_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
}}}}	m68k_incpc (4);
endlabel4475: ;
}
/* MVMEL.L #<data>.W,(An)+ */
void REGPARAM2 CPUFUNC(op_4cd8_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{	uae_u16 mask = get_word_ce_prefetch (4);
	unsigned int dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
	uae_u32 v;
{	uaecptr srca = m68k_areg(regs, dstreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 4, srca);
		goto endlabel4476;
	}
{{	while (dmask) { v = get_word_ce(srca) << 16; v |= get_word_ce(srca + 2); m68k_dreg(regs, movem_index1[dmask]) = v; srca += 4; dmask = movem_next[dmask]; }
	while (amask) { v = get_word_ce(srca) << 16; v |= get_word_ce(srca + 2); m68k_areg(regs, movem_index1[amask]) = v; srca += 4; amask = movem_next[amask]; }
	get_word_ce (srca);
	m68k_areg(regs, dstreg) = srca;
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
}}}}	m68k_incpc (4);
endlabel4476: ;
}
/* MVMEL.L #<data>.W,(d16,An) */
void REGPARAM2 CPUFUNC(op_4ce8_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{	uae_u16 mask = get_word_ce_prefetch (4);
	unsigned int dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
	uae_u32 v;
{	uaecptr srca = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 4, srca);
		goto endlabel4477;
	}
{{	while (dmask) { v = get_word_ce(srca) << 16; v |= get_word_ce(srca + 2); m68k_dreg(regs, movem_index1[dmask]) = v; srca += 4; dmask = movem_next[dmask]; }
	while (amask) { v = get_word_ce(srca) << 16; v |= get_word_ce(srca + 2); m68k_areg(regs, movem_index1[amask]) = v; srca += 4; amask = movem_next[amask]; }
	get_word_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
}}}}	m68k_incpc (6);
endlabel4477: ;
}
/* MVMEL.L #<data>.W,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_4cf0_6)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{	uae_u16 mask = get_word_ce_prefetch (4);
	unsigned int dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
	uae_u32 v;
{	uaecptr srca;
	srca = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (6));
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 4, srca);
		goto endlabel4478;
	}
{	do_cycles_ce (2 * 256);
{	while (dmask) { v = get_word_ce(srca) << 16; v |= get_word_ce(srca + 2); m68k_dreg(regs, movem_index1[dmask]) = v; srca += 4; dmask = movem_next[dmask]; }
	while (amask) { v = get_word_ce(srca) << 16; v |= get_word_ce(srca + 2); m68k_areg(regs, movem_index1[amask]) = v; srca += 4; amask = movem_next[amask]; }
	get_word_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
}}}}	m68k_incpc (6);
endlabel4478: ;
}
/* MVMEL.L #<data>.W,(xxx).W */
void REGPARAM2 CPUFUNC(op_4cf8_6)(uae_u32 opcode)
{
{	uae_u16 mask = get_word_ce_prefetch (4);
	unsigned int dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
	uae_u32 v;
{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 4, srca);
		goto endlabel4479;
	}
{{	while (dmask) { v = get_word_ce(srca) << 16; v |= get_word_ce(srca + 2); m68k_dreg(regs, movem_index1[dmask]) = v; srca += 4; dmask = movem_next[dmask]; }
	while (amask) { v = get_word_ce(srca) << 16; v |= get_word_ce(srca + 2); m68k_areg(regs, movem_index1[amask]) = v; srca += 4; amask = movem_next[amask]; }
	get_word_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
}}}}	m68k_incpc (6);
endlabel4479: ;
}
/* MVMEL.L #<data>.W,(xxx).L */
void REGPARAM2 CPUFUNC(op_4cf9_6)(uae_u32 opcode)
{
{	uae_u16 mask = get_word_ce_prefetch (4);
	unsigned int dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
	uae_u32 v;
{	uaecptr srca;
	srca = get_word_ce_prefetch (6) << 16;
	srca |= get_word_ce_prefetch (8);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 4, srca);
		goto endlabel4480;
	}
{{	while (dmask) { v = get_word_ce(srca) << 16; v |= get_word_ce(srca + 2); m68k_dreg(regs, movem_index1[dmask]) = v; srca += 4; dmask = movem_next[dmask]; }
	while (amask) { v = get_word_ce(srca) << 16; v |= get_word_ce(srca + 2); m68k_areg(regs, movem_index1[amask]) = v; srca += 4; amask = movem_next[amask]; }
	get_word_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (10);
}}}}	m68k_incpc (8);
endlabel4480: ;
}
/* MVMEL.L #<data>.W,(d16,PC) */
void REGPARAM2 CPUFUNC(op_4cfa_6)(uae_u32 opcode)
{
	uae_u32 dstreg = 2;
{	uae_u16 mask = get_word_ce_prefetch (4);
	unsigned int dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
	uae_u32 v;
{	uaecptr srca = m68k_getpc () + 4;
	srca += (uae_s32)(uae_s16)get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 4, srca);
		goto endlabel4481;
	}
{{	while (dmask) { v = get_word_ce(srca) << 16; v |= get_word_ce(srca + 2); m68k_dreg(regs, movem_index1[dmask]) = v; srca += 4; dmask = movem_next[dmask]; }
	while (amask) { v = get_word_ce(srca) << 16; v |= get_word_ce(srca + 2); m68k_areg(regs, movem_index1[amask]) = v; srca += 4; amask = movem_next[amask]; }
	get_word_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
}}}}	m68k_incpc (6);
endlabel4481: ;
}
/* MVMEL.L #<data>.W,(d8,PC,Xn) */
void REGPARAM2 CPUFUNC(op_4cfb_6)(uae_u32 opcode)
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
		goto endlabel4482;
	}
{	do_cycles_ce (2 * 256);
{	while (dmask) { v = get_word_ce(srca) << 16; v |= get_word_ce(srca + 2); m68k_dreg(regs, movem_index1[dmask]) = v; srca += 4; dmask = movem_next[dmask]; }
	while (amask) { v = get_word_ce(srca) << 16; v |= get_word_ce(srca + 2); m68k_areg(regs, movem_index1[amask]) = v; srca += 4; amask = movem_next[amask]; }
	get_word_ce (srca);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
}}}}	m68k_incpc (6);
endlabel4482: ;
}
/* TRAP.L #<data> */
void REGPARAM2 CPUFUNC(op_4e40_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 15);
{{	uae_u32 src = srcreg;
	m68k_incpc (2);
	Exception (src + 32,0);
}}}
/* LINK.W An,#<data>.W */
void REGPARAM2 CPUFUNC(op_4e50_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr olda;
	olda = m68k_areg(regs, 7) - 4;
	if (olda & 1) {
		exception3 (opcode, m68k_getpc() + 2, olda);
		goto endlabel4484;
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
endlabel4484: ;
}
/* UNLK.L An */
void REGPARAM2 CPUFUNC(op_4e58_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s32 src = m68k_areg(regs, srcreg);
	m68k_areg(regs, 7) = src;
{	uaecptr olda = m68k_areg(regs, 7);
	if (olda & 1) {
		exception3 (opcode, m68k_getpc() + 2, olda);
		goto endlabel4485;
	}
{{	uae_s32 old = get_word_ce (olda) << 16; old |= get_word_ce (olda + 2);
	m68k_areg(regs, 7) += 4;
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	m68k_areg(regs, srcreg) = (old);
}}}}}	m68k_incpc (2);
endlabel4485: ;
}
/* MVR2USP.L An */
void REGPARAM2 CPUFUNC(op_4e60_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception(8,0); goto endlabel4486; }
{{	uae_s32 src = m68k_areg(regs, srcreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	regs.usp = src;
}}}	m68k_incpc (2);
endlabel4486: ;
}
/* MVUSP2R.L An */
void REGPARAM2 CPUFUNC(op_4e68_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception(8,0); goto endlabel4487; }
{{	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	m68k_areg(regs, srcreg) = (regs.usp);
}}}	m68k_incpc (2);
endlabel4487: ;
}
/* RESET.L  */
void REGPARAM2 CPUFUNC(op_4e70_6)(uae_u32 opcode)
{
{if (!regs.s) { Exception(8,0); goto endlabel4488; }
{	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	cpureset();
	regs.irc = get_iword(4);
}}	m68k_incpc (2);
endlabel4488: ;
}
/* NOP.L  */
void REGPARAM2 CPUFUNC(op_4e71_6)(uae_u32 opcode)
{
{	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
}	m68k_incpc (2);
}
/* STOP.L #<data>.W */
void REGPARAM2 CPUFUNC(op_4e72_6)(uae_u32 opcode)
{
{if (!regs.s) { Exception(8,0); goto endlabel4490; }
{{	uae_s16 src = get_word_ce_prefetch (4);
	regs.sr = src;
	MakeFromSR();
	m68k_setstopped(1);
	m68k_incpc (4);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}endlabel4490: ;
}
/* RTE.L  */
void REGPARAM2 CPUFUNC(op_4e73_6)(uae_u32 opcode)
{
{if (!regs.s) { Exception(8,0); goto endlabel4491; }
{{	uaecptr sra = m68k_areg(regs, 7);
	if (sra & 1) {
		exception3 (opcode, m68k_getpc() + 2, sra);
		goto endlabel4491;
	}
{{	uae_s16 sr = get_word_ce (sra);
	m68k_areg(regs, 7) += 2;
{	uaecptr pca = m68k_areg(regs, 7);
	if (pca & 1) {
		exception3 (opcode, m68k_getpc() + 2, pca);
		goto endlabel4491;
	}
{{	uae_s32 pc = get_word_ce (pca) << 16; pc |= get_word_ce (pca + 2);
	m68k_areg(regs, 7) += 4;
	regs.sr = sr; m68k_setpc_rte(pc);
	MakeFromSR();
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}}}endlabel4491: ;
}
/* RTD.L #<data>.W */
void REGPARAM2 CPUFUNC(op_4e74_6)(uae_u32 opcode)
{
{{	uaecptr pca = m68k_areg(regs, 7);
	if (pca & 1) {
		exception3 (opcode, m68k_getpc() + 2, pca);
		goto endlabel4492;
	}
{{	uae_s32 pc = get_word_ce (pca) << 16; pc |= get_word_ce (pca + 2);
	m68k_areg(regs, 7) += 4;
{	uae_s16 offs = get_word_ce_prefetch (4);
	m68k_areg(regs, 7) += offs;
	if (pc & 1)
		exception3 (0x4E74, m68k_getpc(), pc);
	else
		m68k_setpc_rte(pc);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}endlabel4492: ;
}
/* RTS.L  */
void REGPARAM2 CPUFUNC(op_4e75_6)(uae_u32 opcode)
{
{	m68k_do_rts_ce();
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}
/* TRAPV.L  */
void REGPARAM2 CPUFUNC(op_4e76_6)(uae_u32 opcode)
{
{	m68k_incpc (2);
	if (GET_VFLG) {
		Exception (7, m68k_getpc ());
		goto endlabel4494;
	}
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}endlabel4494: ;
}
/* RTR.L  */
void REGPARAM2 CPUFUNC(op_4e77_6)(uae_u32 opcode)
{
{	MakeSR();
{	uaecptr sra = m68k_areg(regs, 7);
	if (sra & 1) {
		exception3 (opcode, m68k_getpc() + 2, sra);
		goto endlabel4495;
	}
{{	uae_s16 sr = get_word_ce (sra);
	m68k_areg(regs, 7) += 2;
{	uaecptr pca = m68k_areg(regs, 7);
	if (pca & 1) {
		exception3 (opcode, m68k_getpc() + 2, pca);
		goto endlabel4495;
	}
{{	uae_s32 pc = get_word_ce (pca) << 16; pc |= get_word_ce (pca + 2);
	m68k_areg(regs, 7) += 4;
	regs.sr &= 0xFF00; sr &= 0xFF;
	regs.sr |= sr; m68k_setpc(pc);
	MakeFromSR();
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}}}}}endlabel4495: ;
}
/* JSR.L (An) */
void REGPARAM2 CPUFUNC(op_4e90_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
{	uaecptr oldpc = m68k_getpc() + 2;
	if (srca & 1) {
		exception3i (opcode, oldpc, srca);
		goto endlabel4496;
	}
	m68k_setpc (srca);
	get_word_ce_prefetch (0);
	m68k_areg (regs, 7) -= 4;
	put_word_ce (m68k_areg (regs, 7), oldpc >> 16);
	put_word_ce (m68k_areg (regs, 7) + 2, oldpc);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}endlabel4496: ;
}
/* JSR.L (d16,An) */
void REGPARAM2 CPUFUNC(op_4ea8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)regs.irc;
{	uaecptr oldpc = m68k_getpc() + 4;
	if (srca & 1) {
		exception3i (opcode, oldpc, srca);
		goto endlabel4497;
	}
	m68k_setpc (srca);
	get_word_ce_prefetch (0);
	m68k_areg (regs, 7) -= 4;
	put_word_ce (m68k_areg (regs, 7), oldpc >> 16);
	put_word_ce (m68k_areg (regs, 7) + 2, oldpc);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}endlabel4497: ;
}
/* JSR.L (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_4eb0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), regs.irc);
{	uaecptr oldpc = m68k_getpc() + 4;
	if (srca & 1) {
		exception3i (opcode, oldpc, srca);
		goto endlabel4498;
	}
	m68k_setpc (srca);
	get_word_ce_prefetch (0);
	m68k_areg (regs, 7) -= 4;
	put_word_ce (m68k_areg (regs, 7), oldpc >> 16);
	put_word_ce (m68k_areg (regs, 7) + 2, oldpc);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}endlabel4498: ;
}
/* JSR.L (xxx).W */
void REGPARAM2 CPUFUNC(op_4eb8_6)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)regs.irc;
{	uaecptr oldpc = m68k_getpc() + 4;
	if (srca & 1) {
		exception3i (opcode, oldpc, srca);
		goto endlabel4499;
	}
	m68k_setpc (srca);
	get_word_ce_prefetch (0);
	m68k_areg (regs, 7) -= 4;
	put_word_ce (m68k_areg (regs, 7), oldpc >> 16);
	put_word_ce (m68k_areg (regs, 7) + 2, oldpc);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}endlabel4499: ;
}
/* JSR.L (xxx).L */
void REGPARAM2 CPUFUNC(op_4eb9_6)(uae_u32 opcode)
{
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= regs.irc;
{	uaecptr oldpc = m68k_getpc() + 6;
	if (srca & 1) {
		exception3i (opcode, oldpc, srca);
		goto endlabel4500;
	}
	m68k_setpc (srca);
	get_word_ce_prefetch (0);
	m68k_areg (regs, 7) -= 4;
	put_word_ce (m68k_areg (regs, 7), oldpc >> 16);
	put_word_ce (m68k_areg (regs, 7) + 2, oldpc);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}endlabel4500: ;
}
/* JSR.L (d16,PC) */
void REGPARAM2 CPUFUNC(op_4eba_6)(uae_u32 opcode)
{
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)regs.irc;
{	uaecptr oldpc = m68k_getpc() + 4;
	if (srca & 1) {
		exception3i (opcode, oldpc, srca);
		goto endlabel4501;
	}
	m68k_setpc (srca);
	get_word_ce_prefetch (0);
	m68k_areg (regs, 7) -= 4;
	put_word_ce (m68k_areg (regs, 7), oldpc >> 16);
	put_word_ce (m68k_areg (regs, 7) + 2, oldpc);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}endlabel4501: ;
}
/* JSR.L (d8,PC,Xn) */
void REGPARAM2 CPUFUNC(op_4ebb_6)(uae_u32 opcode)
{
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, regs.irc);
{	uaecptr oldpc = m68k_getpc() + 4;
	if (srca & 1) {
		exception3i (opcode, oldpc, srca);
		goto endlabel4502;
	}
	m68k_setpc (srca);
	get_word_ce_prefetch (0);
	m68k_areg (regs, 7) -= 4;
	put_word_ce (m68k_areg (regs, 7), oldpc >> 16);
	put_word_ce (m68k_areg (regs, 7) + 2, oldpc);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}endlabel4502: ;
}
/* JMP.L (An) */
void REGPARAM2 CPUFUNC(op_4ed0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3i (opcode, m68k_getpc() + 6, srca);
		goto endlabel4503;
	}
	m68k_setpc(srca);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4503: ;
}
/* JMP.L (d16,An) */
void REGPARAM2 CPUFUNC(op_4ee8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)regs.irc;
	if (srca & 1) {
		exception3i (opcode, m68k_getpc() + 6, srca);
		goto endlabel4504;
	}
	do_cycles_ce (2 * 256);
	m68k_setpc(srca);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4504: ;
}
/* JMP.L (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_4ef0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), regs.irc);
	if (srca & 1) {
		exception3i (opcode, m68k_getpc() + 6, srca);
		goto endlabel4505;
	}
	do_cycles_ce (2 * 256);
	m68k_setpc(srca);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4505: ;
}
/* JMP.L (xxx).W */
void REGPARAM2 CPUFUNC(op_4ef8_6)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)regs.irc;
	if (srca & 1) {
		exception3i (opcode, m68k_getpc() + 6, srca);
		goto endlabel4506;
	}
	do_cycles_ce (2 * 256);
	m68k_setpc(srca);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4506: ;
}
/* JMP.L (xxx).L */
void REGPARAM2 CPUFUNC(op_4ef9_6)(uae_u32 opcode)
{
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= regs.irc;
	if (srca & 1) {
		exception3i (opcode, m68k_getpc() + 6, srca);
		goto endlabel4507;
	}
	m68k_setpc(srca);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4507: ;
}
/* JMP.L (d16,PC) */
void REGPARAM2 CPUFUNC(op_4efa_6)(uae_u32 opcode)
{
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)regs.irc;
	if (srca & 1) {
		exception3i (opcode, m68k_getpc() + 6, srca);
		goto endlabel4508;
	}
	do_cycles_ce (2 * 256);
	m68k_setpc(srca);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4508: ;
}
/* JMP.L (d8,PC,Xn) */
void REGPARAM2 CPUFUNC(op_4efb_6)(uae_u32 opcode)
{
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, regs.irc);
	if (srca & 1) {
		exception3i (opcode, m68k_getpc() + 6, srca);
		goto endlabel4509;
	}
	do_cycles_ce (2 * 256);
	m68k_setpc(srca);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4509: ;
}
/* ADD.B #<data>,Dn */
void REGPARAM2 CPUFUNC(op_5000_6)(uae_u32 opcode)
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
}
/* ADD.B #<data>,(An) */
void REGPARAM2 CPUFUNC(op_5010_6)(uae_u32 opcode)
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
}
/* ADD.B #<data>,(An)+ */
void REGPARAM2 CPUFUNC(op_5018_6)(uae_u32 opcode)
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
}
/* ADD.B #<data>,-(An) */
void REGPARAM2 CPUFUNC(op_5020_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - areg_byteinc[dstreg];
	do_cycles_ce (2 * 256);
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
}
/* ADD.B #<data>,(d16,An) */
void REGPARAM2 CPUFUNC(op_5028_6)(uae_u32 opcode)
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
}
/* ADD.B #<data>,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_5030_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta;
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
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
}
/* ADD.B #<data>,(xxx).W */
void REGPARAM2 CPUFUNC(op_5038_6)(uae_u32 opcode)
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
}
#endif

#ifdef PART_5
/* ADD.B #<data>,(xxx).L */
void REGPARAM2 CPUFUNC(op_5039_6)(uae_u32 opcode)
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
}
/* ADD.W #<data>,Dn */
void REGPARAM2 CPUFUNC(op_5040_6)(uae_u32 opcode)
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
}
/* ADDA.W #<data>,An */
void REGPARAM2 CPUFUNC(op_5048_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uae_s32 dst = m68k_areg(regs, dstreg);
	do_cycles_ce (4 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 newv = dst + src;
	m68k_areg(regs, dstreg) = (newv);
}}}}	m68k_incpc (2);
}
/* ADD.W #<data>,(An) */
void REGPARAM2 CPUFUNC(op_5050_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4520;
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
endlabel4520: ;
}
/* ADD.W #<data>,(An)+ */
void REGPARAM2 CPUFUNC(op_5058_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4521;
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
endlabel4521: ;
}
/* ADD.W #<data>,-(An) */
void REGPARAM2 CPUFUNC(op_5060_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - 2;
	do_cycles_ce (2 * 256);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4522;
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
endlabel4522: ;
}
/* ADD.W #<data>,(d16,An) */
void REGPARAM2 CPUFUNC(op_5068_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4523;
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
endlabel4523: ;
}
/* ADD.W #<data>,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_5070_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta;
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4524;
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
endlabel4524: ;
}
/* ADD.W #<data>,(xxx).W */
void REGPARAM2 CPUFUNC(op_5078_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
{{	uae_u32 src = srcreg;
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4525;
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
endlabel4525: ;
}
/* ADD.W #<data>,(xxx).L */
void REGPARAM2 CPUFUNC(op_5079_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
{{	uae_u32 src = srcreg;
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (4) << 16;
	dsta |= get_word_ce_prefetch (6);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4526;
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
endlabel4526: ;
}
/* ADD.L #<data>,Dn */
void REGPARAM2 CPUFUNC(op_5080_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	do_cycles_ce (4 * 256);
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
	m68k_dreg(regs, dstreg) = (newv);
}}}}}}	m68k_incpc (2);
}
/* ADDA.L #<data>,An */
void REGPARAM2 CPUFUNC(op_5088_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uae_s32 dst = m68k_areg(regs, dstreg);
	do_cycles_ce (2 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 newv = dst + src;
	m68k_areg(regs, dstreg) = (newv);
}}}}	m68k_incpc (2);
}
/* ADD.L #<data>,(An) */
void REGPARAM2 CPUFUNC(op_5090_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4529;
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
endlabel4529: ;
}
/* ADD.L #<data>,(An)+ */
void REGPARAM2 CPUFUNC(op_5098_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4530;
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
endlabel4530: ;
}
/* ADD.L #<data>,-(An) */
void REGPARAM2 CPUFUNC(op_50a0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - 4;
	do_cycles_ce (2 * 256);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4531;
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
endlabel4531: ;
}
/* ADD.L #<data>,(d16,An) */
void REGPARAM2 CPUFUNC(op_50a8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4532;
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
endlabel4532: ;
}
/* ADD.L #<data>,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_50b0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta;
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4533;
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
endlabel4533: ;
}
/* ADD.L #<data>,(xxx).W */
void REGPARAM2 CPUFUNC(op_50b8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
{{	uae_u32 src = srcreg;
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4534;
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
endlabel4534: ;
}
/* ADD.L #<data>,(xxx).L */
void REGPARAM2 CPUFUNC(op_50b9_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
{{	uae_u32 src = srcreg;
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (4) << 16;
	dsta |= get_word_ce_prefetch (6);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4535;
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
endlabel4535: ;
}
/* Scc.B Dn */
void REGPARAM2 CPUFUNC(op_50c0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(0) ? 0xff : 0;
	int cycles = 0;
	if (val) cycles += 2 * 256;
	if (cycles > 0) do_cycles_ce (cycles);
	m68k_dreg(regs, srcreg) = (m68k_dreg(regs, srcreg) & ~0xff) | ((val) & 0xff);
}}}}	m68k_incpc (2);
}
/* DBcc.W Dn,#<data>.W */
void REGPARAM2 CPUFUNC(op_50c8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uae_s16 offs = regs.irc;
	uaecptr oldpc = m68k_getpc();
	do_cycles_ce (2 * 256);
	if (!cctrue(0)) {
		m68k_incpc((uae_s32)offs + 2);
		get_word_ce_prefetch (0);
		m68k_dreg(regs, srcreg) = (m68k_dreg(regs, srcreg) & ~0xffff) | (((src-1)) & 0xffff);
		if (src) {
			if (offs & 1) {
				exception3i (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)offs + 2);
				goto endlabel4537;
			}
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
			return;
		}
	} else {
		do_cycles_ce (2 * 256);
	}
	m68k_setpc (oldpc + 4);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}endlabel4537: ;
}
/* Scc.B (An) */
void REGPARAM2 CPUFUNC(op_50d0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
{	uae_s8 src = get_byte_ce (srca);
{	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(0) ? 0xff : 0;
	int cycles = 0;
	if (cycles > 0) do_cycles_ce (cycles);
	put_byte_ce (srca,val);
}}}}}	m68k_incpc (2);
}
/* Scc.B (An)+ */
void REGPARAM2 CPUFUNC(op_50d8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
{	uae_s8 src = get_byte_ce (srca);
	m68k_areg(regs, srcreg) += areg_byteinc[srcreg];
{	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(0) ? 0xff : 0;
	int cycles = 0;
	if (cycles > 0) do_cycles_ce (cycles);
	put_byte_ce (srca,val);
}}}}}	m68k_incpc (2);
}
/* Scc.B -(An) */
void REGPARAM2 CPUFUNC(op_50e0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
	do_cycles_ce (2 * 256);
{	uae_s8 src = get_byte_ce (srca);
	m68k_areg (regs, srcreg) = srca;
{	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(0) ? 0xff : 0;
	int cycles = 0;
	if (cycles > 0) do_cycles_ce (cycles);
	put_byte_ce (srca,val);
}}}}}	m68k_incpc (2);
}
/* Scc.B (d16,An) */
void REGPARAM2 CPUFUNC(op_50e8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 src = get_byte_ce (srca);
{	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	int val = cctrue(0) ? 0xff : 0;
	int cycles = 0;
	if (cycles > 0) do_cycles_ce (cycles);
	put_byte_ce (srca,val);
}}}}}	m68k_incpc (4);
}
/* Scc.B (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_50f0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
{	uae_s8 src = get_byte_ce (srca);
{	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	int val = cctrue(0) ? 0xff : 0;
	int cycles = 0;
	if (cycles > 0) do_cycles_ce (cycles);
	put_byte_ce (srca,val);
}}}}}	m68k_incpc (4);
}
/* Scc.B (xxx).W */
void REGPARAM2 CPUFUNC(op_50f8_6)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 src = get_byte_ce (srca);
{	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	int val = cctrue(0) ? 0xff : 0;
	int cycles = 0;
	if (cycles > 0) do_cycles_ce (cycles);
	put_byte_ce (srca,val);
}}}}}	m68k_incpc (4);
}
/* Scc.B (xxx).L */
void REGPARAM2 CPUFUNC(op_50f9_6)(uae_u32 opcode)
{
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
{	uae_s8 src = get_byte_ce (srca);
{	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
{	int val = cctrue(0) ? 0xff : 0;
	int cycles = 0;
	if (cycles > 0) do_cycles_ce (cycles);
	put_byte_ce (srca,val);
}}}}}	m68k_incpc (6);
}
/* SUB.B #<data>,Dn */
void REGPARAM2 CPUFUNC(op_5100_6)(uae_u32 opcode)
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
}
/* SUB.B #<data>,(An) */
void REGPARAM2 CPUFUNC(op_5110_6)(uae_u32 opcode)
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
}
/* SUB.B #<data>,(An)+ */
void REGPARAM2 CPUFUNC(op_5118_6)(uae_u32 opcode)
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
}
/* SUB.B #<data>,-(An) */
void REGPARAM2 CPUFUNC(op_5120_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - areg_byteinc[dstreg];
	do_cycles_ce (2 * 256);
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
}
/* SUB.B #<data>,(d16,An) */
void REGPARAM2 CPUFUNC(op_5128_6)(uae_u32 opcode)
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
}
/* SUB.B #<data>,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_5130_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta;
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
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
}
/* SUB.B #<data>,(xxx).W */
void REGPARAM2 CPUFUNC(op_5138_6)(uae_u32 opcode)
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
}
/* SUB.B #<data>,(xxx).L */
void REGPARAM2 CPUFUNC(op_5139_6)(uae_u32 opcode)
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
}
/* SUB.W #<data>,Dn */
void REGPARAM2 CPUFUNC(op_5140_6)(uae_u32 opcode)
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
}
/* SUBA.W #<data>,An */
void REGPARAM2 CPUFUNC(op_5148_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uae_s32 dst = m68k_areg(regs, dstreg);
	do_cycles_ce (4 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 newv = dst - src;
	m68k_areg(regs, dstreg) = (newv);
}}}}	m68k_incpc (2);
}
/* SUB.W #<data>,(An) */
void REGPARAM2 CPUFUNC(op_5150_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4555;
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
endlabel4555: ;
}
/* SUB.W #<data>,(An)+ */
void REGPARAM2 CPUFUNC(op_5158_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4556;
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
endlabel4556: ;
}
/* SUB.W #<data>,-(An) */
void REGPARAM2 CPUFUNC(op_5160_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - 2;
	do_cycles_ce (2 * 256);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4557;
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
endlabel4557: ;
}
/* SUB.W #<data>,(d16,An) */
void REGPARAM2 CPUFUNC(op_5168_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4558;
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
endlabel4558: ;
}
/* SUB.W #<data>,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_5170_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta;
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4559;
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
endlabel4559: ;
}
/* SUB.W #<data>,(xxx).W */
void REGPARAM2 CPUFUNC(op_5178_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
{{	uae_u32 src = srcreg;
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4560;
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
endlabel4560: ;
}
/* SUB.W #<data>,(xxx).L */
void REGPARAM2 CPUFUNC(op_5179_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
{{	uae_u32 src = srcreg;
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (4) << 16;
	dsta |= get_word_ce_prefetch (6);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4561;
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
endlabel4561: ;
}
/* SUB.L #<data>,Dn */
void REGPARAM2 CPUFUNC(op_5180_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	do_cycles_ce (4 * 256);
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
	m68k_dreg(regs, dstreg) = (newv);
}}}}}}	m68k_incpc (2);
}
/* SUBA.L #<data>,An */
void REGPARAM2 CPUFUNC(op_5188_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uae_s32 dst = m68k_areg(regs, dstreg);
	do_cycles_ce (4 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 newv = dst - src;
	m68k_areg(regs, dstreg) = (newv);
}}}}	m68k_incpc (2);
}
/* SUB.L #<data>,(An) */
void REGPARAM2 CPUFUNC(op_5190_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4564;
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
endlabel4564: ;
}
/* SUB.L #<data>,(An)+ */
void REGPARAM2 CPUFUNC(op_5198_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4565;
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
endlabel4565: ;
}
/* SUB.L #<data>,-(An) */
void REGPARAM2 CPUFUNC(op_51a0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - 4;
	do_cycles_ce (2 * 256);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4566;
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
endlabel4566: ;
}
/* SUB.L #<data>,(d16,An) */
void REGPARAM2 CPUFUNC(op_51a8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4567;
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
endlabel4567: ;
}
/* SUB.L #<data>,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_51b0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta;
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4568;
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
endlabel4568: ;
}
/* SUB.L #<data>,(xxx).W */
void REGPARAM2 CPUFUNC(op_51b8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
{{	uae_u32 src = srcreg;
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4569;
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
endlabel4569: ;
}
/* SUB.L #<data>,(xxx).L */
void REGPARAM2 CPUFUNC(op_51b9_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
{{	uae_u32 src = srcreg;
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (4) << 16;
	dsta |= get_word_ce_prefetch (6);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4570;
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
endlabel4570: ;
}
/* Scc.B Dn */
void REGPARAM2 CPUFUNC(op_51c0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(1) ? 0xff : 0;
	int cycles = 0;
	if (val) cycles += 2 * 256;
	if (cycles > 0) do_cycles_ce (cycles);
	m68k_dreg(regs, srcreg) = (m68k_dreg(regs, srcreg) & ~0xff) | ((val) & 0xff);
}}}}	m68k_incpc (2);
}
/* DBcc.W Dn,#<data>.W */
void REGPARAM2 CPUFUNC(op_51c8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uae_s16 offs = regs.irc;
	uaecptr oldpc = m68k_getpc();
	do_cycles_ce (2 * 256);
	if (!cctrue(1)) {
		m68k_incpc((uae_s32)offs + 2);
		get_word_ce_prefetch (0);
		m68k_dreg(regs, srcreg) = (m68k_dreg(regs, srcreg) & ~0xffff) | (((src-1)) & 0xffff);
		if (src) {
			if (offs & 1) {
				exception3i (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)offs + 2);
				goto endlabel4572;
			}
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
			return;
		}
	} else {
		do_cycles_ce (2 * 256);
	}
	m68k_setpc (oldpc + 4);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}endlabel4572: ;
}
/* Scc.B (An) */
void REGPARAM2 CPUFUNC(op_51d0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
{	uae_s8 src = get_byte_ce (srca);
{	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(1) ? 0xff : 0;
	int cycles = 0;
	if (cycles > 0) do_cycles_ce (cycles);
	put_byte_ce (srca,val);
}}}}}	m68k_incpc (2);
}
/* Scc.B (An)+ */
void REGPARAM2 CPUFUNC(op_51d8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
{	uae_s8 src = get_byte_ce (srca);
	m68k_areg(regs, srcreg) += areg_byteinc[srcreg];
{	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(1) ? 0xff : 0;
	int cycles = 0;
	if (cycles > 0) do_cycles_ce (cycles);
	put_byte_ce (srca,val);
}}}}}	m68k_incpc (2);
}
/* Scc.B -(An) */
void REGPARAM2 CPUFUNC(op_51e0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
	do_cycles_ce (2 * 256);
{	uae_s8 src = get_byte_ce (srca);
	m68k_areg (regs, srcreg) = srca;
{	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(1) ? 0xff : 0;
	int cycles = 0;
	if (cycles > 0) do_cycles_ce (cycles);
	put_byte_ce (srca,val);
}}}}}	m68k_incpc (2);
}
/* Scc.B (d16,An) */
void REGPARAM2 CPUFUNC(op_51e8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 src = get_byte_ce (srca);
{	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	int val = cctrue(1) ? 0xff : 0;
	int cycles = 0;
	if (cycles > 0) do_cycles_ce (cycles);
	put_byte_ce (srca,val);
}}}}}	m68k_incpc (4);
}
/* Scc.B (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_51f0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
{	uae_s8 src = get_byte_ce (srca);
{	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	int val = cctrue(1) ? 0xff : 0;
	int cycles = 0;
	if (cycles > 0) do_cycles_ce (cycles);
	put_byte_ce (srca,val);
}}}}}	m68k_incpc (4);
}
/* Scc.B (xxx).W */
void REGPARAM2 CPUFUNC(op_51f8_6)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 src = get_byte_ce (srca);
{	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	int val = cctrue(1) ? 0xff : 0;
	int cycles = 0;
	if (cycles > 0) do_cycles_ce (cycles);
	put_byte_ce (srca,val);
}}}}}	m68k_incpc (4);
}
/* Scc.B (xxx).L */
void REGPARAM2 CPUFUNC(op_51f9_6)(uae_u32 opcode)
{
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
{	uae_s8 src = get_byte_ce (srca);
{	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
{	int val = cctrue(1) ? 0xff : 0;
	int cycles = 0;
	if (cycles > 0) do_cycles_ce (cycles);
	put_byte_ce (srca,val);
}}}}}	m68k_incpc (6);
}
/* Scc.B Dn */
void REGPARAM2 CPUFUNC(op_52c0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(2) ? 0xff : 0;
	int cycles = 0;
	if (val) cycles += 2 * 256;
	if (cycles > 0) do_cycles_ce (cycles);
	m68k_dreg(regs, srcreg) = (m68k_dreg(regs, srcreg) & ~0xff) | ((val) & 0xff);
}}}}	m68k_incpc (2);
}
/* DBcc.W Dn,#<data>.W */
void REGPARAM2 CPUFUNC(op_52c8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uae_s16 offs = regs.irc;
	uaecptr oldpc = m68k_getpc();
	do_cycles_ce (2 * 256);
	if (!cctrue(2)) {
		m68k_incpc((uae_s32)offs + 2);
		get_word_ce_prefetch (0);
		m68k_dreg(regs, srcreg) = (m68k_dreg(regs, srcreg) & ~0xffff) | (((src-1)) & 0xffff);
		if (src) {
			if (offs & 1) {
				exception3i (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)offs + 2);
				goto endlabel4581;
			}
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
			return;
		}
	} else {
		do_cycles_ce (2 * 256);
	}
	m68k_setpc (oldpc + 4);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}endlabel4581: ;
}
/* Scc.B (An) */
void REGPARAM2 CPUFUNC(op_52d0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
{	uae_s8 src = get_byte_ce (srca);
{	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(2) ? 0xff : 0;
	int cycles = 0;
	if (cycles > 0) do_cycles_ce (cycles);
	put_byte_ce (srca,val);
}}}}}	m68k_incpc (2);
}
/* Scc.B (An)+ */
void REGPARAM2 CPUFUNC(op_52d8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
{	uae_s8 src = get_byte_ce (srca);
	m68k_areg(regs, srcreg) += areg_byteinc[srcreg];
{	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(2) ? 0xff : 0;
	int cycles = 0;
	if (cycles > 0) do_cycles_ce (cycles);
	put_byte_ce (srca,val);
}}}}}	m68k_incpc (2);
}
/* Scc.B -(An) */
void REGPARAM2 CPUFUNC(op_52e0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
	do_cycles_ce (2 * 256);
{	uae_s8 src = get_byte_ce (srca);
	m68k_areg (regs, srcreg) = srca;
{	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(2) ? 0xff : 0;
	int cycles = 0;
	if (cycles > 0) do_cycles_ce (cycles);
	put_byte_ce (srca,val);
}}}}}	m68k_incpc (2);
}
/* Scc.B (d16,An) */
void REGPARAM2 CPUFUNC(op_52e8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 src = get_byte_ce (srca);
{	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	int val = cctrue(2) ? 0xff : 0;
	int cycles = 0;
	if (cycles > 0) do_cycles_ce (cycles);
	put_byte_ce (srca,val);
}}}}}	m68k_incpc (4);
}
/* Scc.B (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_52f0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
{	uae_s8 src = get_byte_ce (srca);
{	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	int val = cctrue(2) ? 0xff : 0;
	int cycles = 0;
	if (cycles > 0) do_cycles_ce (cycles);
	put_byte_ce (srca,val);
}}}}}	m68k_incpc (4);
}
/* Scc.B (xxx).W */
void REGPARAM2 CPUFUNC(op_52f8_6)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 src = get_byte_ce (srca);
{	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	int val = cctrue(2) ? 0xff : 0;
	int cycles = 0;
	if (cycles > 0) do_cycles_ce (cycles);
	put_byte_ce (srca,val);
}}}}}	m68k_incpc (4);
}
/* Scc.B (xxx).L */
void REGPARAM2 CPUFUNC(op_52f9_6)(uae_u32 opcode)
{
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
{	uae_s8 src = get_byte_ce (srca);
{	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
{	int val = cctrue(2) ? 0xff : 0;
	int cycles = 0;
	if (cycles > 0) do_cycles_ce (cycles);
	put_byte_ce (srca,val);
}}}}}	m68k_incpc (6);
}
/* Scc.B Dn */
void REGPARAM2 CPUFUNC(op_53c0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(3) ? 0xff : 0;
	int cycles = 0;
	if (val) cycles += 2 * 256;
	if (cycles > 0) do_cycles_ce (cycles);
	m68k_dreg(regs, srcreg) = (m68k_dreg(regs, srcreg) & ~0xff) | ((val) & 0xff);
}}}}	m68k_incpc (2);
}
/* DBcc.W Dn,#<data>.W */
void REGPARAM2 CPUFUNC(op_53c8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uae_s16 offs = regs.irc;
	uaecptr oldpc = m68k_getpc();
	do_cycles_ce (2 * 256);
	if (!cctrue(3)) {
		m68k_incpc((uae_s32)offs + 2);
		get_word_ce_prefetch (0);
		m68k_dreg(regs, srcreg) = (m68k_dreg(regs, srcreg) & ~0xffff) | (((src-1)) & 0xffff);
		if (src) {
			if (offs & 1) {
				exception3i (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)offs + 2);
				goto endlabel4590;
			}
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
			return;
		}
	} else {
		do_cycles_ce (2 * 256);
	}
	m68k_setpc (oldpc + 4);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}endlabel4590: ;
}
/* Scc.B (An) */
void REGPARAM2 CPUFUNC(op_53d0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
{	uae_s8 src = get_byte_ce (srca);
{	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(3) ? 0xff : 0;
	int cycles = 0;
	if (cycles > 0) do_cycles_ce (cycles);
	put_byte_ce (srca,val);
}}}}}	m68k_incpc (2);
}
/* Scc.B (An)+ */
void REGPARAM2 CPUFUNC(op_53d8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
{	uae_s8 src = get_byte_ce (srca);
	m68k_areg(regs, srcreg) += areg_byteinc[srcreg];
{	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(3) ? 0xff : 0;
	int cycles = 0;
	if (cycles > 0) do_cycles_ce (cycles);
	put_byte_ce (srca,val);
}}}}}	m68k_incpc (2);
}
/* Scc.B -(An) */
void REGPARAM2 CPUFUNC(op_53e0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
	do_cycles_ce (2 * 256);
{	uae_s8 src = get_byte_ce (srca);
	m68k_areg (regs, srcreg) = srca;
{	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(3) ? 0xff : 0;
	int cycles = 0;
	if (cycles > 0) do_cycles_ce (cycles);
	put_byte_ce (srca,val);
}}}}}	m68k_incpc (2);
}
/* Scc.B (d16,An) */
void REGPARAM2 CPUFUNC(op_53e8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 src = get_byte_ce (srca);
{	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	int val = cctrue(3) ? 0xff : 0;
	int cycles = 0;
	if (cycles > 0) do_cycles_ce (cycles);
	put_byte_ce (srca,val);
}}}}}	m68k_incpc (4);
}
/* Scc.B (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_53f0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
{	uae_s8 src = get_byte_ce (srca);
{	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	int val = cctrue(3) ? 0xff : 0;
	int cycles = 0;
	if (cycles > 0) do_cycles_ce (cycles);
	put_byte_ce (srca,val);
}}}}}	m68k_incpc (4);
}
/* Scc.B (xxx).W */
void REGPARAM2 CPUFUNC(op_53f8_6)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 src = get_byte_ce (srca);
{	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	int val = cctrue(3) ? 0xff : 0;
	int cycles = 0;
	if (cycles > 0) do_cycles_ce (cycles);
	put_byte_ce (srca,val);
}}}}}	m68k_incpc (4);
}
/* Scc.B (xxx).L */
void REGPARAM2 CPUFUNC(op_53f9_6)(uae_u32 opcode)
{
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
{	uae_s8 src = get_byte_ce (srca);
{	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
{	int val = cctrue(3) ? 0xff : 0;
	int cycles = 0;
	if (cycles > 0) do_cycles_ce (cycles);
	put_byte_ce (srca,val);
}}}}}	m68k_incpc (6);
}
/* Scc.B Dn */
void REGPARAM2 CPUFUNC(op_54c0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(4) ? 0xff : 0;
	int cycles = 0;
	if (val) cycles += 2 * 256;
	if (cycles > 0) do_cycles_ce (cycles);
	m68k_dreg(regs, srcreg) = (m68k_dreg(regs, srcreg) & ~0xff) | ((val) & 0xff);
}}}}	m68k_incpc (2);
}
/* DBcc.W Dn,#<data>.W */
void REGPARAM2 CPUFUNC(op_54c8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uae_s16 offs = regs.irc;
	uaecptr oldpc = m68k_getpc();
	do_cycles_ce (2 * 256);
	if (!cctrue(4)) {
		m68k_incpc((uae_s32)offs + 2);
		get_word_ce_prefetch (0);
		m68k_dreg(regs, srcreg) = (m68k_dreg(regs, srcreg) & ~0xffff) | (((src-1)) & 0xffff);
		if (src) {
			if (offs & 1) {
				exception3i (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)offs + 2);
				goto endlabel4599;
			}
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
			return;
		}
	} else {
		do_cycles_ce (2 * 256);
	}
	m68k_setpc (oldpc + 4);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}endlabel4599: ;
}
/* Scc.B (An) */
void REGPARAM2 CPUFUNC(op_54d0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
{	uae_s8 src = get_byte_ce (srca);
{	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(4) ? 0xff : 0;
	int cycles = 0;
	if (cycles > 0) do_cycles_ce (cycles);
	put_byte_ce (srca,val);
}}}}}	m68k_incpc (2);
}
/* Scc.B (An)+ */
void REGPARAM2 CPUFUNC(op_54d8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
{	uae_s8 src = get_byte_ce (srca);
	m68k_areg(regs, srcreg) += areg_byteinc[srcreg];
{	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(4) ? 0xff : 0;
	int cycles = 0;
	if (cycles > 0) do_cycles_ce (cycles);
	put_byte_ce (srca,val);
}}}}}	m68k_incpc (2);
}
/* Scc.B -(An) */
void REGPARAM2 CPUFUNC(op_54e0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
	do_cycles_ce (2 * 256);
{	uae_s8 src = get_byte_ce (srca);
	m68k_areg (regs, srcreg) = srca;
{	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(4) ? 0xff : 0;
	int cycles = 0;
	if (cycles > 0) do_cycles_ce (cycles);
	put_byte_ce (srca,val);
}}}}}	m68k_incpc (2);
}
/* Scc.B (d16,An) */
void REGPARAM2 CPUFUNC(op_54e8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 src = get_byte_ce (srca);
{	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	int val = cctrue(4) ? 0xff : 0;
	int cycles = 0;
	if (cycles > 0) do_cycles_ce (cycles);
	put_byte_ce (srca,val);
}}}}}	m68k_incpc (4);
}
/* Scc.B (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_54f0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
{	uae_s8 src = get_byte_ce (srca);
{	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	int val = cctrue(4) ? 0xff : 0;
	int cycles = 0;
	if (cycles > 0) do_cycles_ce (cycles);
	put_byte_ce (srca,val);
}}}}}	m68k_incpc (4);
}
/* Scc.B (xxx).W */
void REGPARAM2 CPUFUNC(op_54f8_6)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 src = get_byte_ce (srca);
{	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	int val = cctrue(4) ? 0xff : 0;
	int cycles = 0;
	if (cycles > 0) do_cycles_ce (cycles);
	put_byte_ce (srca,val);
}}}}}	m68k_incpc (4);
}
/* Scc.B (xxx).L */
void REGPARAM2 CPUFUNC(op_54f9_6)(uae_u32 opcode)
{
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
{	uae_s8 src = get_byte_ce (srca);
{	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
{	int val = cctrue(4) ? 0xff : 0;
	int cycles = 0;
	if (cycles > 0) do_cycles_ce (cycles);
	put_byte_ce (srca,val);
}}}}}	m68k_incpc (6);
}
/* Scc.B Dn */
void REGPARAM2 CPUFUNC(op_55c0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(5) ? 0xff : 0;
	int cycles = 0;
	if (val) cycles += 2 * 256;
	if (cycles > 0) do_cycles_ce (cycles);
	m68k_dreg(regs, srcreg) = (m68k_dreg(regs, srcreg) & ~0xff) | ((val) & 0xff);
}}}}	m68k_incpc (2);
}
/* DBcc.W Dn,#<data>.W */
void REGPARAM2 CPUFUNC(op_55c8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uae_s16 offs = regs.irc;
	uaecptr oldpc = m68k_getpc();
	do_cycles_ce (2 * 256);
	if (!cctrue(5)) {
		m68k_incpc((uae_s32)offs + 2);
		get_word_ce_prefetch (0);
		m68k_dreg(regs, srcreg) = (m68k_dreg(regs, srcreg) & ~0xffff) | (((src-1)) & 0xffff);
		if (src) {
			if (offs & 1) {
				exception3i (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)offs + 2);
				goto endlabel4608;
			}
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
			return;
		}
	} else {
		do_cycles_ce (2 * 256);
	}
	m68k_setpc (oldpc + 4);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}endlabel4608: ;
}
/* Scc.B (An) */
void REGPARAM2 CPUFUNC(op_55d0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
{	uae_s8 src = get_byte_ce (srca);
{	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(5) ? 0xff : 0;
	int cycles = 0;
	if (cycles > 0) do_cycles_ce (cycles);
	put_byte_ce (srca,val);
}}}}}	m68k_incpc (2);
}
/* Scc.B (An)+ */
void REGPARAM2 CPUFUNC(op_55d8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
{	uae_s8 src = get_byte_ce (srca);
	m68k_areg(regs, srcreg) += areg_byteinc[srcreg];
{	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(5) ? 0xff : 0;
	int cycles = 0;
	if (cycles > 0) do_cycles_ce (cycles);
	put_byte_ce (srca,val);
}}}}}	m68k_incpc (2);
}
/* Scc.B -(An) */
void REGPARAM2 CPUFUNC(op_55e0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
	do_cycles_ce (2 * 256);
{	uae_s8 src = get_byte_ce (srca);
	m68k_areg (regs, srcreg) = srca;
{	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(5) ? 0xff : 0;
	int cycles = 0;
	if (cycles > 0) do_cycles_ce (cycles);
	put_byte_ce (srca,val);
}}}}}	m68k_incpc (2);
}
/* Scc.B (d16,An) */
void REGPARAM2 CPUFUNC(op_55e8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 src = get_byte_ce (srca);
{	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	int val = cctrue(5) ? 0xff : 0;
	int cycles = 0;
	if (cycles > 0) do_cycles_ce (cycles);
	put_byte_ce (srca,val);
}}}}}	m68k_incpc (4);
}
/* Scc.B (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_55f0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
{	uae_s8 src = get_byte_ce (srca);
{	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	int val = cctrue(5) ? 0xff : 0;
	int cycles = 0;
	if (cycles > 0) do_cycles_ce (cycles);
	put_byte_ce (srca,val);
}}}}}	m68k_incpc (4);
}
/* Scc.B (xxx).W */
void REGPARAM2 CPUFUNC(op_55f8_6)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 src = get_byte_ce (srca);
{	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	int val = cctrue(5) ? 0xff : 0;
	int cycles = 0;
	if (cycles > 0) do_cycles_ce (cycles);
	put_byte_ce (srca,val);
}}}}}	m68k_incpc (4);
}
/* Scc.B (xxx).L */
void REGPARAM2 CPUFUNC(op_55f9_6)(uae_u32 opcode)
{
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
{	uae_s8 src = get_byte_ce (srca);
{	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
{	int val = cctrue(5) ? 0xff : 0;
	int cycles = 0;
	if (cycles > 0) do_cycles_ce (cycles);
	put_byte_ce (srca,val);
}}}}}	m68k_incpc (6);
}
/* Scc.B Dn */
void REGPARAM2 CPUFUNC(op_56c0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(6) ? 0xff : 0;
	int cycles = 0;
	if (val) cycles += 2 * 256;
	if (cycles > 0) do_cycles_ce (cycles);
	m68k_dreg(regs, srcreg) = (m68k_dreg(regs, srcreg) & ~0xff) | ((val) & 0xff);
}}}}	m68k_incpc (2);
}
/* DBcc.W Dn,#<data>.W */
void REGPARAM2 CPUFUNC(op_56c8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uae_s16 offs = regs.irc;
	uaecptr oldpc = m68k_getpc();
	do_cycles_ce (2 * 256);
	if (!cctrue(6)) {
		m68k_incpc((uae_s32)offs + 2);
		get_word_ce_prefetch (0);
		m68k_dreg(regs, srcreg) = (m68k_dreg(regs, srcreg) & ~0xffff) | (((src-1)) & 0xffff);
		if (src) {
			if (offs & 1) {
				exception3i (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)offs + 2);
				goto endlabel4617;
			}
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
			return;
		}
	} else {
		do_cycles_ce (2 * 256);
	}
	m68k_setpc (oldpc + 4);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}endlabel4617: ;
}
/* Scc.B (An) */
void REGPARAM2 CPUFUNC(op_56d0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
{	uae_s8 src = get_byte_ce (srca);
{	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(6) ? 0xff : 0;
	int cycles = 0;
	if (cycles > 0) do_cycles_ce (cycles);
	put_byte_ce (srca,val);
}}}}}	m68k_incpc (2);
}
/* Scc.B (An)+ */
void REGPARAM2 CPUFUNC(op_56d8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
{	uae_s8 src = get_byte_ce (srca);
	m68k_areg(regs, srcreg) += areg_byteinc[srcreg];
{	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(6) ? 0xff : 0;
	int cycles = 0;
	if (cycles > 0) do_cycles_ce (cycles);
	put_byte_ce (srca,val);
}}}}}	m68k_incpc (2);
}
/* Scc.B -(An) */
void REGPARAM2 CPUFUNC(op_56e0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
	do_cycles_ce (2 * 256);
{	uae_s8 src = get_byte_ce (srca);
	m68k_areg (regs, srcreg) = srca;
{	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(6) ? 0xff : 0;
	int cycles = 0;
	if (cycles > 0) do_cycles_ce (cycles);
	put_byte_ce (srca,val);
}}}}}	m68k_incpc (2);
}
/* Scc.B (d16,An) */
void REGPARAM2 CPUFUNC(op_56e8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 src = get_byte_ce (srca);
{	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	int val = cctrue(6) ? 0xff : 0;
	int cycles = 0;
	if (cycles > 0) do_cycles_ce (cycles);
	put_byte_ce (srca,val);
}}}}}	m68k_incpc (4);
}
/* Scc.B (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_56f0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
{	uae_s8 src = get_byte_ce (srca);
{	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	int val = cctrue(6) ? 0xff : 0;
	int cycles = 0;
	if (cycles > 0) do_cycles_ce (cycles);
	put_byte_ce (srca,val);
}}}}}	m68k_incpc (4);
}
/* Scc.B (xxx).W */
void REGPARAM2 CPUFUNC(op_56f8_6)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 src = get_byte_ce (srca);
{	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	int val = cctrue(6) ? 0xff : 0;
	int cycles = 0;
	if (cycles > 0) do_cycles_ce (cycles);
	put_byte_ce (srca,val);
}}}}}	m68k_incpc (4);
}
/* Scc.B (xxx).L */
void REGPARAM2 CPUFUNC(op_56f9_6)(uae_u32 opcode)
{
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
{	uae_s8 src = get_byte_ce (srca);
{	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
{	int val = cctrue(6) ? 0xff : 0;
	int cycles = 0;
	if (cycles > 0) do_cycles_ce (cycles);
	put_byte_ce (srca,val);
}}}}}	m68k_incpc (6);
}
/* Scc.B Dn */
void REGPARAM2 CPUFUNC(op_57c0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(7) ? 0xff : 0;
	int cycles = 0;
	if (val) cycles += 2 * 256;
	if (cycles > 0) do_cycles_ce (cycles);
	m68k_dreg(regs, srcreg) = (m68k_dreg(regs, srcreg) & ~0xff) | ((val) & 0xff);
}}}}	m68k_incpc (2);
}
/* DBcc.W Dn,#<data>.W */
void REGPARAM2 CPUFUNC(op_57c8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uae_s16 offs = regs.irc;
	uaecptr oldpc = m68k_getpc();
	do_cycles_ce (2 * 256);
	if (!cctrue(7)) {
		m68k_incpc((uae_s32)offs + 2);
		get_word_ce_prefetch (0);
		m68k_dreg(regs, srcreg) = (m68k_dreg(regs, srcreg) & ~0xffff) | (((src-1)) & 0xffff);
		if (src) {
			if (offs & 1) {
				exception3i (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)offs + 2);
				goto endlabel4626;
			}
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
			return;
		}
	} else {
		do_cycles_ce (2 * 256);
	}
	m68k_setpc (oldpc + 4);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}endlabel4626: ;
}
/* Scc.B (An) */
void REGPARAM2 CPUFUNC(op_57d0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
{	uae_s8 src = get_byte_ce (srca);
{	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(7) ? 0xff : 0;
	int cycles = 0;
	if (cycles > 0) do_cycles_ce (cycles);
	put_byte_ce (srca,val);
}}}}}	m68k_incpc (2);
}
/* Scc.B (An)+ */
void REGPARAM2 CPUFUNC(op_57d8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
{	uae_s8 src = get_byte_ce (srca);
	m68k_areg(regs, srcreg) += areg_byteinc[srcreg];
{	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(7) ? 0xff : 0;
	int cycles = 0;
	if (cycles > 0) do_cycles_ce (cycles);
	put_byte_ce (srca,val);
}}}}}	m68k_incpc (2);
}
/* Scc.B -(An) */
void REGPARAM2 CPUFUNC(op_57e0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
	do_cycles_ce (2 * 256);
{	uae_s8 src = get_byte_ce (srca);
	m68k_areg (regs, srcreg) = srca;
{	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(7) ? 0xff : 0;
	int cycles = 0;
	if (cycles > 0) do_cycles_ce (cycles);
	put_byte_ce (srca,val);
}}}}}	m68k_incpc (2);
}
/* Scc.B (d16,An) */
void REGPARAM2 CPUFUNC(op_57e8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 src = get_byte_ce (srca);
{	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	int val = cctrue(7) ? 0xff : 0;
	int cycles = 0;
	if (cycles > 0) do_cycles_ce (cycles);
	put_byte_ce (srca,val);
}}}}}	m68k_incpc (4);
}
/* Scc.B (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_57f0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
{	uae_s8 src = get_byte_ce (srca);
{	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	int val = cctrue(7) ? 0xff : 0;
	int cycles = 0;
	if (cycles > 0) do_cycles_ce (cycles);
	put_byte_ce (srca,val);
}}}}}	m68k_incpc (4);
}
/* Scc.B (xxx).W */
void REGPARAM2 CPUFUNC(op_57f8_6)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 src = get_byte_ce (srca);
{	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	int val = cctrue(7) ? 0xff : 0;
	int cycles = 0;
	if (cycles > 0) do_cycles_ce (cycles);
	put_byte_ce (srca,val);
}}}}}	m68k_incpc (4);
}
/* Scc.B (xxx).L */
void REGPARAM2 CPUFUNC(op_57f9_6)(uae_u32 opcode)
{
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
{	uae_s8 src = get_byte_ce (srca);
{	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
{	int val = cctrue(7) ? 0xff : 0;
	int cycles = 0;
	if (cycles > 0) do_cycles_ce (cycles);
	put_byte_ce (srca,val);
}}}}}	m68k_incpc (6);
}
/* Scc.B Dn */
void REGPARAM2 CPUFUNC(op_58c0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(8) ? 0xff : 0;
	int cycles = 0;
	if (val) cycles += 2 * 256;
	if (cycles > 0) do_cycles_ce (cycles);
	m68k_dreg(regs, srcreg) = (m68k_dreg(regs, srcreg) & ~0xff) | ((val) & 0xff);
}}}}	m68k_incpc (2);
}
/* DBcc.W Dn,#<data>.W */
void REGPARAM2 CPUFUNC(op_58c8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uae_s16 offs = regs.irc;
	uaecptr oldpc = m68k_getpc();
	do_cycles_ce (2 * 256);
	if (!cctrue(8)) {
		m68k_incpc((uae_s32)offs + 2);
		get_word_ce_prefetch (0);
		m68k_dreg(regs, srcreg) = (m68k_dreg(regs, srcreg) & ~0xffff) | (((src-1)) & 0xffff);
		if (src) {
			if (offs & 1) {
				exception3i (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)offs + 2);
				goto endlabel4635;
			}
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
			return;
		}
	} else {
		do_cycles_ce (2 * 256);
	}
	m68k_setpc (oldpc + 4);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}endlabel4635: ;
}
/* Scc.B (An) */
void REGPARAM2 CPUFUNC(op_58d0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
{	uae_s8 src = get_byte_ce (srca);
{	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(8) ? 0xff : 0;
	int cycles = 0;
	if (cycles > 0) do_cycles_ce (cycles);
	put_byte_ce (srca,val);
}}}}}	m68k_incpc (2);
}
/* Scc.B (An)+ */
void REGPARAM2 CPUFUNC(op_58d8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
{	uae_s8 src = get_byte_ce (srca);
	m68k_areg(regs, srcreg) += areg_byteinc[srcreg];
{	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(8) ? 0xff : 0;
	int cycles = 0;
	if (cycles > 0) do_cycles_ce (cycles);
	put_byte_ce (srca,val);
}}}}}	m68k_incpc (2);
}
/* Scc.B -(An) */
void REGPARAM2 CPUFUNC(op_58e0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
	do_cycles_ce (2 * 256);
{	uae_s8 src = get_byte_ce (srca);
	m68k_areg (regs, srcreg) = srca;
{	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(8) ? 0xff : 0;
	int cycles = 0;
	if (cycles > 0) do_cycles_ce (cycles);
	put_byte_ce (srca,val);
}}}}}	m68k_incpc (2);
}
/* Scc.B (d16,An) */
void REGPARAM2 CPUFUNC(op_58e8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 src = get_byte_ce (srca);
{	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	int val = cctrue(8) ? 0xff : 0;
	int cycles = 0;
	if (cycles > 0) do_cycles_ce (cycles);
	put_byte_ce (srca,val);
}}}}}	m68k_incpc (4);
}
/* Scc.B (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_58f0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
{	uae_s8 src = get_byte_ce (srca);
{	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	int val = cctrue(8) ? 0xff : 0;
	int cycles = 0;
	if (cycles > 0) do_cycles_ce (cycles);
	put_byte_ce (srca,val);
}}}}}	m68k_incpc (4);
}
/* Scc.B (xxx).W */
void REGPARAM2 CPUFUNC(op_58f8_6)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 src = get_byte_ce (srca);
{	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	int val = cctrue(8) ? 0xff : 0;
	int cycles = 0;
	if (cycles > 0) do_cycles_ce (cycles);
	put_byte_ce (srca,val);
}}}}}	m68k_incpc (4);
}
/* Scc.B (xxx).L */
void REGPARAM2 CPUFUNC(op_58f9_6)(uae_u32 opcode)
{
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
{	uae_s8 src = get_byte_ce (srca);
{	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
{	int val = cctrue(8) ? 0xff : 0;
	int cycles = 0;
	if (cycles > 0) do_cycles_ce (cycles);
	put_byte_ce (srca,val);
}}}}}	m68k_incpc (6);
}
/* Scc.B Dn */
void REGPARAM2 CPUFUNC(op_59c0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(9) ? 0xff : 0;
	int cycles = 0;
	if (val) cycles += 2 * 256;
	if (cycles > 0) do_cycles_ce (cycles);
	m68k_dreg(regs, srcreg) = (m68k_dreg(regs, srcreg) & ~0xff) | ((val) & 0xff);
}}}}	m68k_incpc (2);
}
/* DBcc.W Dn,#<data>.W */
void REGPARAM2 CPUFUNC(op_59c8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uae_s16 offs = regs.irc;
	uaecptr oldpc = m68k_getpc();
	do_cycles_ce (2 * 256);
	if (!cctrue(9)) {
		m68k_incpc((uae_s32)offs + 2);
		get_word_ce_prefetch (0);
		m68k_dreg(regs, srcreg) = (m68k_dreg(regs, srcreg) & ~0xffff) | (((src-1)) & 0xffff);
		if (src) {
			if (offs & 1) {
				exception3i (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)offs + 2);
				goto endlabel4644;
			}
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
			return;
		}
	} else {
		do_cycles_ce (2 * 256);
	}
	m68k_setpc (oldpc + 4);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}endlabel4644: ;
}
/* Scc.B (An) */
void REGPARAM2 CPUFUNC(op_59d0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
{	uae_s8 src = get_byte_ce (srca);
{	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(9) ? 0xff : 0;
	int cycles = 0;
	if (cycles > 0) do_cycles_ce (cycles);
	put_byte_ce (srca,val);
}}}}}	m68k_incpc (2);
}
/* Scc.B (An)+ */
void REGPARAM2 CPUFUNC(op_59d8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
{	uae_s8 src = get_byte_ce (srca);
	m68k_areg(regs, srcreg) += areg_byteinc[srcreg];
{	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(9) ? 0xff : 0;
	int cycles = 0;
	if (cycles > 0) do_cycles_ce (cycles);
	put_byte_ce (srca,val);
}}}}}	m68k_incpc (2);
}
/* Scc.B -(An) */
void REGPARAM2 CPUFUNC(op_59e0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
	do_cycles_ce (2 * 256);
{	uae_s8 src = get_byte_ce (srca);
	m68k_areg (regs, srcreg) = srca;
{	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(9) ? 0xff : 0;
	int cycles = 0;
	if (cycles > 0) do_cycles_ce (cycles);
	put_byte_ce (srca,val);
}}}}}	m68k_incpc (2);
}
/* Scc.B (d16,An) */
void REGPARAM2 CPUFUNC(op_59e8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 src = get_byte_ce (srca);
{	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	int val = cctrue(9) ? 0xff : 0;
	int cycles = 0;
	if (cycles > 0) do_cycles_ce (cycles);
	put_byte_ce (srca,val);
}}}}}	m68k_incpc (4);
}
/* Scc.B (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_59f0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
{	uae_s8 src = get_byte_ce (srca);
{	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	int val = cctrue(9) ? 0xff : 0;
	int cycles = 0;
	if (cycles > 0) do_cycles_ce (cycles);
	put_byte_ce (srca,val);
}}}}}	m68k_incpc (4);
}
/* Scc.B (xxx).W */
void REGPARAM2 CPUFUNC(op_59f8_6)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 src = get_byte_ce (srca);
{	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	int val = cctrue(9) ? 0xff : 0;
	int cycles = 0;
	if (cycles > 0) do_cycles_ce (cycles);
	put_byte_ce (srca,val);
}}}}}	m68k_incpc (4);
}
/* Scc.B (xxx).L */
void REGPARAM2 CPUFUNC(op_59f9_6)(uae_u32 opcode)
{
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
{	uae_s8 src = get_byte_ce (srca);
{	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
{	int val = cctrue(9) ? 0xff : 0;
	int cycles = 0;
	if (cycles > 0) do_cycles_ce (cycles);
	put_byte_ce (srca,val);
}}}}}	m68k_incpc (6);
}
/* Scc.B Dn */
void REGPARAM2 CPUFUNC(op_5ac0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(10) ? 0xff : 0;
	int cycles = 0;
	if (val) cycles += 2 * 256;
	if (cycles > 0) do_cycles_ce (cycles);
	m68k_dreg(regs, srcreg) = (m68k_dreg(regs, srcreg) & ~0xff) | ((val) & 0xff);
}}}}	m68k_incpc (2);
}
/* DBcc.W Dn,#<data>.W */
void REGPARAM2 CPUFUNC(op_5ac8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uae_s16 offs = regs.irc;
	uaecptr oldpc = m68k_getpc();
	do_cycles_ce (2 * 256);
	if (!cctrue(10)) {
		m68k_incpc((uae_s32)offs + 2);
		get_word_ce_prefetch (0);
		m68k_dreg(regs, srcreg) = (m68k_dreg(regs, srcreg) & ~0xffff) | (((src-1)) & 0xffff);
		if (src) {
			if (offs & 1) {
				exception3i (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)offs + 2);
				goto endlabel4653;
			}
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
			return;
		}
	} else {
		do_cycles_ce (2 * 256);
	}
	m68k_setpc (oldpc + 4);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}endlabel4653: ;
}
/* Scc.B (An) */
void REGPARAM2 CPUFUNC(op_5ad0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
{	uae_s8 src = get_byte_ce (srca);
{	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(10) ? 0xff : 0;
	int cycles = 0;
	if (cycles > 0) do_cycles_ce (cycles);
	put_byte_ce (srca,val);
}}}}}	m68k_incpc (2);
}
/* Scc.B (An)+ */
void REGPARAM2 CPUFUNC(op_5ad8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
{	uae_s8 src = get_byte_ce (srca);
	m68k_areg(regs, srcreg) += areg_byteinc[srcreg];
{	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(10) ? 0xff : 0;
	int cycles = 0;
	if (cycles > 0) do_cycles_ce (cycles);
	put_byte_ce (srca,val);
}}}}}	m68k_incpc (2);
}
/* Scc.B -(An) */
void REGPARAM2 CPUFUNC(op_5ae0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
	do_cycles_ce (2 * 256);
{	uae_s8 src = get_byte_ce (srca);
	m68k_areg (regs, srcreg) = srca;
{	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(10) ? 0xff : 0;
	int cycles = 0;
	if (cycles > 0) do_cycles_ce (cycles);
	put_byte_ce (srca,val);
}}}}}	m68k_incpc (2);
}
/* Scc.B (d16,An) */
void REGPARAM2 CPUFUNC(op_5ae8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 src = get_byte_ce (srca);
{	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	int val = cctrue(10) ? 0xff : 0;
	int cycles = 0;
	if (cycles > 0) do_cycles_ce (cycles);
	put_byte_ce (srca,val);
}}}}}	m68k_incpc (4);
}
/* Scc.B (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_5af0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
{	uae_s8 src = get_byte_ce (srca);
{	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	int val = cctrue(10) ? 0xff : 0;
	int cycles = 0;
	if (cycles > 0) do_cycles_ce (cycles);
	put_byte_ce (srca,val);
}}}}}	m68k_incpc (4);
}
/* Scc.B (xxx).W */
void REGPARAM2 CPUFUNC(op_5af8_6)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 src = get_byte_ce (srca);
{	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	int val = cctrue(10) ? 0xff : 0;
	int cycles = 0;
	if (cycles > 0) do_cycles_ce (cycles);
	put_byte_ce (srca,val);
}}}}}	m68k_incpc (4);
}
/* Scc.B (xxx).L */
void REGPARAM2 CPUFUNC(op_5af9_6)(uae_u32 opcode)
{
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
{	uae_s8 src = get_byte_ce (srca);
{	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
{	int val = cctrue(10) ? 0xff : 0;
	int cycles = 0;
	if (cycles > 0) do_cycles_ce (cycles);
	put_byte_ce (srca,val);
}}}}}	m68k_incpc (6);
}
/* Scc.B Dn */
void REGPARAM2 CPUFUNC(op_5bc0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(11) ? 0xff : 0;
	int cycles = 0;
	if (val) cycles += 2 * 256;
	if (cycles > 0) do_cycles_ce (cycles);
	m68k_dreg(regs, srcreg) = (m68k_dreg(regs, srcreg) & ~0xff) | ((val) & 0xff);
}}}}	m68k_incpc (2);
}
/* DBcc.W Dn,#<data>.W */
void REGPARAM2 CPUFUNC(op_5bc8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uae_s16 offs = regs.irc;
	uaecptr oldpc = m68k_getpc();
	do_cycles_ce (2 * 256);
	if (!cctrue(11)) {
		m68k_incpc((uae_s32)offs + 2);
		get_word_ce_prefetch (0);
		m68k_dreg(regs, srcreg) = (m68k_dreg(regs, srcreg) & ~0xffff) | (((src-1)) & 0xffff);
		if (src) {
			if (offs & 1) {
				exception3i (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)offs + 2);
				goto endlabel4662;
			}
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
			return;
		}
	} else {
		do_cycles_ce (2 * 256);
	}
	m68k_setpc (oldpc + 4);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}endlabel4662: ;
}
/* Scc.B (An) */
void REGPARAM2 CPUFUNC(op_5bd0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
{	uae_s8 src = get_byte_ce (srca);
{	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(11) ? 0xff : 0;
	int cycles = 0;
	if (cycles > 0) do_cycles_ce (cycles);
	put_byte_ce (srca,val);
}}}}}	m68k_incpc (2);
}
/* Scc.B (An)+ */
void REGPARAM2 CPUFUNC(op_5bd8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
{	uae_s8 src = get_byte_ce (srca);
	m68k_areg(regs, srcreg) += areg_byteinc[srcreg];
{	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(11) ? 0xff : 0;
	int cycles = 0;
	if (cycles > 0) do_cycles_ce (cycles);
	put_byte_ce (srca,val);
}}}}}	m68k_incpc (2);
}
/* Scc.B -(An) */
void REGPARAM2 CPUFUNC(op_5be0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
	do_cycles_ce (2 * 256);
{	uae_s8 src = get_byte_ce (srca);
	m68k_areg (regs, srcreg) = srca;
{	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(11) ? 0xff : 0;
	int cycles = 0;
	if (cycles > 0) do_cycles_ce (cycles);
	put_byte_ce (srca,val);
}}}}}	m68k_incpc (2);
}
/* Scc.B (d16,An) */
void REGPARAM2 CPUFUNC(op_5be8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 src = get_byte_ce (srca);
{	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	int val = cctrue(11) ? 0xff : 0;
	int cycles = 0;
	if (cycles > 0) do_cycles_ce (cycles);
	put_byte_ce (srca,val);
}}}}}	m68k_incpc (4);
}
/* Scc.B (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_5bf0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
{	uae_s8 src = get_byte_ce (srca);
{	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	int val = cctrue(11) ? 0xff : 0;
	int cycles = 0;
	if (cycles > 0) do_cycles_ce (cycles);
	put_byte_ce (srca,val);
}}}}}	m68k_incpc (4);
}
/* Scc.B (xxx).W */
void REGPARAM2 CPUFUNC(op_5bf8_6)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 src = get_byte_ce (srca);
{	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	int val = cctrue(11) ? 0xff : 0;
	int cycles = 0;
	if (cycles > 0) do_cycles_ce (cycles);
	put_byte_ce (srca,val);
}}}}}	m68k_incpc (4);
}
/* Scc.B (xxx).L */
void REGPARAM2 CPUFUNC(op_5bf9_6)(uae_u32 opcode)
{
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
{	uae_s8 src = get_byte_ce (srca);
{	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
{	int val = cctrue(11) ? 0xff : 0;
	int cycles = 0;
	if (cycles > 0) do_cycles_ce (cycles);
	put_byte_ce (srca,val);
}}}}}	m68k_incpc (6);
}
/* Scc.B Dn */
void REGPARAM2 CPUFUNC(op_5cc0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(12) ? 0xff : 0;
	int cycles = 0;
	if (val) cycles += 2 * 256;
	if (cycles > 0) do_cycles_ce (cycles);
	m68k_dreg(regs, srcreg) = (m68k_dreg(regs, srcreg) & ~0xff) | ((val) & 0xff);
}}}}	m68k_incpc (2);
}
/* DBcc.W Dn,#<data>.W */
void REGPARAM2 CPUFUNC(op_5cc8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uae_s16 offs = regs.irc;
	uaecptr oldpc = m68k_getpc();
	do_cycles_ce (2 * 256);
	if (!cctrue(12)) {
		m68k_incpc((uae_s32)offs + 2);
		get_word_ce_prefetch (0);
		m68k_dreg(regs, srcreg) = (m68k_dreg(regs, srcreg) & ~0xffff) | (((src-1)) & 0xffff);
		if (src) {
			if (offs & 1) {
				exception3i (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)offs + 2);
				goto endlabel4671;
			}
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
			return;
		}
	} else {
		do_cycles_ce (2 * 256);
	}
	m68k_setpc (oldpc + 4);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}endlabel4671: ;
}
/* Scc.B (An) */
void REGPARAM2 CPUFUNC(op_5cd0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
{	uae_s8 src = get_byte_ce (srca);
{	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(12) ? 0xff : 0;
	int cycles = 0;
	if (cycles > 0) do_cycles_ce (cycles);
	put_byte_ce (srca,val);
}}}}}	m68k_incpc (2);
}
/* Scc.B (An)+ */
void REGPARAM2 CPUFUNC(op_5cd8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
{	uae_s8 src = get_byte_ce (srca);
	m68k_areg(regs, srcreg) += areg_byteinc[srcreg];
{	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(12) ? 0xff : 0;
	int cycles = 0;
	if (cycles > 0) do_cycles_ce (cycles);
	put_byte_ce (srca,val);
}}}}}	m68k_incpc (2);
}
/* Scc.B -(An) */
void REGPARAM2 CPUFUNC(op_5ce0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
	do_cycles_ce (2 * 256);
{	uae_s8 src = get_byte_ce (srca);
	m68k_areg (regs, srcreg) = srca;
{	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(12) ? 0xff : 0;
	int cycles = 0;
	if (cycles > 0) do_cycles_ce (cycles);
	put_byte_ce (srca,val);
}}}}}	m68k_incpc (2);
}
/* Scc.B (d16,An) */
void REGPARAM2 CPUFUNC(op_5ce8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 src = get_byte_ce (srca);
{	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	int val = cctrue(12) ? 0xff : 0;
	int cycles = 0;
	if (cycles > 0) do_cycles_ce (cycles);
	put_byte_ce (srca,val);
}}}}}	m68k_incpc (4);
}
/* Scc.B (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_5cf0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
{	uae_s8 src = get_byte_ce (srca);
{	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	int val = cctrue(12) ? 0xff : 0;
	int cycles = 0;
	if (cycles > 0) do_cycles_ce (cycles);
	put_byte_ce (srca,val);
}}}}}	m68k_incpc (4);
}
/* Scc.B (xxx).W */
void REGPARAM2 CPUFUNC(op_5cf8_6)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 src = get_byte_ce (srca);
{	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	int val = cctrue(12) ? 0xff : 0;
	int cycles = 0;
	if (cycles > 0) do_cycles_ce (cycles);
	put_byte_ce (srca,val);
}}}}}	m68k_incpc (4);
}
/* Scc.B (xxx).L */
void REGPARAM2 CPUFUNC(op_5cf9_6)(uae_u32 opcode)
{
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
{	uae_s8 src = get_byte_ce (srca);
{	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
{	int val = cctrue(12) ? 0xff : 0;
	int cycles = 0;
	if (cycles > 0) do_cycles_ce (cycles);
	put_byte_ce (srca,val);
}}}}}	m68k_incpc (6);
}
/* Scc.B Dn */
void REGPARAM2 CPUFUNC(op_5dc0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(13) ? 0xff : 0;
	int cycles = 0;
	if (val) cycles += 2 * 256;
	if (cycles > 0) do_cycles_ce (cycles);
	m68k_dreg(regs, srcreg) = (m68k_dreg(regs, srcreg) & ~0xff) | ((val) & 0xff);
}}}}	m68k_incpc (2);
}
/* DBcc.W Dn,#<data>.W */
void REGPARAM2 CPUFUNC(op_5dc8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uae_s16 offs = regs.irc;
	uaecptr oldpc = m68k_getpc();
	do_cycles_ce (2 * 256);
	if (!cctrue(13)) {
		m68k_incpc((uae_s32)offs + 2);
		get_word_ce_prefetch (0);
		m68k_dreg(regs, srcreg) = (m68k_dreg(regs, srcreg) & ~0xffff) | (((src-1)) & 0xffff);
		if (src) {
			if (offs & 1) {
				exception3i (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)offs + 2);
				goto endlabel4680;
			}
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
			return;
		}
	} else {
		do_cycles_ce (2 * 256);
	}
	m68k_setpc (oldpc + 4);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}endlabel4680: ;
}
/* Scc.B (An) */
void REGPARAM2 CPUFUNC(op_5dd0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
{	uae_s8 src = get_byte_ce (srca);
{	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(13) ? 0xff : 0;
	int cycles = 0;
	if (cycles > 0) do_cycles_ce (cycles);
	put_byte_ce (srca,val);
}}}}}	m68k_incpc (2);
}
/* Scc.B (An)+ */
void REGPARAM2 CPUFUNC(op_5dd8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
{	uae_s8 src = get_byte_ce (srca);
	m68k_areg(regs, srcreg) += areg_byteinc[srcreg];
{	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(13) ? 0xff : 0;
	int cycles = 0;
	if (cycles > 0) do_cycles_ce (cycles);
	put_byte_ce (srca,val);
}}}}}	m68k_incpc (2);
}
/* Scc.B -(An) */
void REGPARAM2 CPUFUNC(op_5de0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
	do_cycles_ce (2 * 256);
{	uae_s8 src = get_byte_ce (srca);
	m68k_areg (regs, srcreg) = srca;
{	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(13) ? 0xff : 0;
	int cycles = 0;
	if (cycles > 0) do_cycles_ce (cycles);
	put_byte_ce (srca,val);
}}}}}	m68k_incpc (2);
}
/* Scc.B (d16,An) */
void REGPARAM2 CPUFUNC(op_5de8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 src = get_byte_ce (srca);
{	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	int val = cctrue(13) ? 0xff : 0;
	int cycles = 0;
	if (cycles > 0) do_cycles_ce (cycles);
	put_byte_ce (srca,val);
}}}}}	m68k_incpc (4);
}
/* Scc.B (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_5df0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
{	uae_s8 src = get_byte_ce (srca);
{	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	int val = cctrue(13) ? 0xff : 0;
	int cycles = 0;
	if (cycles > 0) do_cycles_ce (cycles);
	put_byte_ce (srca,val);
}}}}}	m68k_incpc (4);
}
/* Scc.B (xxx).W */
void REGPARAM2 CPUFUNC(op_5df8_6)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 src = get_byte_ce (srca);
{	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	int val = cctrue(13) ? 0xff : 0;
	int cycles = 0;
	if (cycles > 0) do_cycles_ce (cycles);
	put_byte_ce (srca,val);
}}}}}	m68k_incpc (4);
}
/* Scc.B (xxx).L */
void REGPARAM2 CPUFUNC(op_5df9_6)(uae_u32 opcode)
{
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
{	uae_s8 src = get_byte_ce (srca);
{	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
{	int val = cctrue(13) ? 0xff : 0;
	int cycles = 0;
	if (cycles > 0) do_cycles_ce (cycles);
	put_byte_ce (srca,val);
}}}}}	m68k_incpc (6);
}
/* Scc.B Dn */
void REGPARAM2 CPUFUNC(op_5ec0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(14) ? 0xff : 0;
	int cycles = 0;
	if (val) cycles += 2 * 256;
	if (cycles > 0) do_cycles_ce (cycles);
	m68k_dreg(regs, srcreg) = (m68k_dreg(regs, srcreg) & ~0xff) | ((val) & 0xff);
}}}}	m68k_incpc (2);
}
/* DBcc.W Dn,#<data>.W */
void REGPARAM2 CPUFUNC(op_5ec8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uae_s16 offs = regs.irc;
	uaecptr oldpc = m68k_getpc();
	do_cycles_ce (2 * 256);
	if (!cctrue(14)) {
		m68k_incpc((uae_s32)offs + 2);
		get_word_ce_prefetch (0);
		m68k_dreg(regs, srcreg) = (m68k_dreg(regs, srcreg) & ~0xffff) | (((src-1)) & 0xffff);
		if (src) {
			if (offs & 1) {
				exception3i (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)offs + 2);
				goto endlabel4689;
			}
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
			return;
		}
	} else {
		do_cycles_ce (2 * 256);
	}
	m68k_setpc (oldpc + 4);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}endlabel4689: ;
}
/* Scc.B (An) */
void REGPARAM2 CPUFUNC(op_5ed0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
{	uae_s8 src = get_byte_ce (srca);
{	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(14) ? 0xff : 0;
	int cycles = 0;
	if (cycles > 0) do_cycles_ce (cycles);
	put_byte_ce (srca,val);
}}}}}	m68k_incpc (2);
}
/* Scc.B (An)+ */
void REGPARAM2 CPUFUNC(op_5ed8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
{	uae_s8 src = get_byte_ce (srca);
	m68k_areg(regs, srcreg) += areg_byteinc[srcreg];
{	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(14) ? 0xff : 0;
	int cycles = 0;
	if (cycles > 0) do_cycles_ce (cycles);
	put_byte_ce (srca,val);
}}}}}	m68k_incpc (2);
}
/* Scc.B -(An) */
void REGPARAM2 CPUFUNC(op_5ee0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
	do_cycles_ce (2 * 256);
{	uae_s8 src = get_byte_ce (srca);
	m68k_areg (regs, srcreg) = srca;
{	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(14) ? 0xff : 0;
	int cycles = 0;
	if (cycles > 0) do_cycles_ce (cycles);
	put_byte_ce (srca,val);
}}}}}	m68k_incpc (2);
}
/* Scc.B (d16,An) */
void REGPARAM2 CPUFUNC(op_5ee8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 src = get_byte_ce (srca);
{	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	int val = cctrue(14) ? 0xff : 0;
	int cycles = 0;
	if (cycles > 0) do_cycles_ce (cycles);
	put_byte_ce (srca,val);
}}}}}	m68k_incpc (4);
}
/* Scc.B (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_5ef0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
{	uae_s8 src = get_byte_ce (srca);
{	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	int val = cctrue(14) ? 0xff : 0;
	int cycles = 0;
	if (cycles > 0) do_cycles_ce (cycles);
	put_byte_ce (srca,val);
}}}}}	m68k_incpc (4);
}
/* Scc.B (xxx).W */
void REGPARAM2 CPUFUNC(op_5ef8_6)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 src = get_byte_ce (srca);
{	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	int val = cctrue(14) ? 0xff : 0;
	int cycles = 0;
	if (cycles > 0) do_cycles_ce (cycles);
	put_byte_ce (srca,val);
}}}}}	m68k_incpc (4);
}
/* Scc.B (xxx).L */
void REGPARAM2 CPUFUNC(op_5ef9_6)(uae_u32 opcode)
{
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
{	uae_s8 src = get_byte_ce (srca);
{	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
{	int val = cctrue(14) ? 0xff : 0;
	int cycles = 0;
	if (cycles > 0) do_cycles_ce (cycles);
	put_byte_ce (srca,val);
}}}}}	m68k_incpc (6);
}
/* Scc.B Dn */
void REGPARAM2 CPUFUNC(op_5fc0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(15) ? 0xff : 0;
	int cycles = 0;
	if (val) cycles += 2 * 256;
	if (cycles > 0) do_cycles_ce (cycles);
	m68k_dreg(regs, srcreg) = (m68k_dreg(regs, srcreg) & ~0xff) | ((val) & 0xff);
}}}}	m68k_incpc (2);
}
/* DBcc.W Dn,#<data>.W */
void REGPARAM2 CPUFUNC(op_5fc8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uae_s16 offs = regs.irc;
	uaecptr oldpc = m68k_getpc();
	do_cycles_ce (2 * 256);
	if (!cctrue(15)) {
		m68k_incpc((uae_s32)offs + 2);
		get_word_ce_prefetch (0);
		m68k_dreg(regs, srcreg) = (m68k_dreg(regs, srcreg) & ~0xffff) | (((src-1)) & 0xffff);
		if (src) {
			if (offs & 1) {
				exception3i (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)offs + 2);
				goto endlabel4698;
			}
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
			return;
		}
	} else {
		do_cycles_ce (2 * 256);
	}
	m68k_setpc (oldpc + 4);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}}endlabel4698: ;
}
/* Scc.B (An) */
void REGPARAM2 CPUFUNC(op_5fd0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
{	uae_s8 src = get_byte_ce (srca);
{	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(15) ? 0xff : 0;
	int cycles = 0;
	if (cycles > 0) do_cycles_ce (cycles);
	put_byte_ce (srca,val);
}}}}}	m68k_incpc (2);
}
/* Scc.B (An)+ */
void REGPARAM2 CPUFUNC(op_5fd8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg);
{	uae_s8 src = get_byte_ce (srca);
	m68k_areg(regs, srcreg) += areg_byteinc[srcreg];
{	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(15) ? 0xff : 0;
	int cycles = 0;
	if (cycles > 0) do_cycles_ce (cycles);
	put_byte_ce (srca,val);
}}}}}	m68k_incpc (2);
}
/* Scc.B -(An) */
void REGPARAM2 CPUFUNC(op_5fe0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
	do_cycles_ce (2 * 256);
{	uae_s8 src = get_byte_ce (srca);
	m68k_areg (regs, srcreg) = srca;
{	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	int val = cctrue(15) ? 0xff : 0;
	int cycles = 0;
	if (cycles > 0) do_cycles_ce (cycles);
	put_byte_ce (srca,val);
}}}}}	m68k_incpc (2);
}
/* Scc.B (d16,An) */
void REGPARAM2 CPUFUNC(op_5fe8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 src = get_byte_ce (srca);
{	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	int val = cctrue(15) ? 0xff : 0;
	int cycles = 0;
	if (cycles > 0) do_cycles_ce (cycles);
	put_byte_ce (srca,val);
}}}}}	m68k_incpc (4);
}
/* Scc.B (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_5ff0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
{	uae_s8 src = get_byte_ce (srca);
{	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	int val = cctrue(15) ? 0xff : 0;
	int cycles = 0;
	if (cycles > 0) do_cycles_ce (cycles);
	put_byte_ce (srca,val);
}}}}}	m68k_incpc (4);
}
/* Scc.B (xxx).W */
void REGPARAM2 CPUFUNC(op_5ff8_6)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
{	uae_s8 src = get_byte_ce (srca);
{	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	int val = cctrue(15) ? 0xff : 0;
	int cycles = 0;
	if (cycles > 0) do_cycles_ce (cycles);
	put_byte_ce (srca,val);
}}}}}	m68k_incpc (4);
}
#endif

#ifdef PART_6
/* Scc.B (xxx).L */
void REGPARAM2 CPUFUNC(op_5ff9_6)(uae_u32 opcode)
{
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
{	uae_s8 src = get_byte_ce (srca);
{	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
{	int val = cctrue(15) ? 0xff : 0;
	int cycles = 0;
	if (cycles > 0) do_cycles_ce (cycles);
	put_byte_ce (srca,val);
}}}}}	m68k_incpc (6);
}
/* Bcc.W #<data>.W */
void REGPARAM2 CPUFUNC(op_6000_6)(uae_u32 opcode)
{
{{	uae_s16 src = regs.irc;
	do_cycles_ce (2 * 256);
	if (!cctrue(0)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)src);
		goto endlabel4706;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	return;
didnt_jump:;
	m68k_incpc (4);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4706: ;
}
/* Bcc.B #<data> */
void REGPARAM2 CPUFUNC(op_6001_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
{{	uae_u32 src = srcreg;
	do_cycles_ce (2 * 256);
	if (!cctrue(0)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)src);
		goto endlabel4707;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	return;
didnt_jump:;
	m68k_incpc (2);
	do_cycles_ce (2 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4707: ;
}
/* Bcc.L #<data>.L */
void REGPARAM2 CPUFUNC(op_60ff_6)(uae_u32 opcode)
{
{	do_cycles_ce (2 * 256);
	if (cctrue(0)) {
		exception3i (opcode, m68k_getpc() + 2, m68k_getpc() + 1);
		goto endlabel4708;
	}
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	goto endlabel4708;
{	uae_s32 src;
	src = get_word_ce_prefetch (2) << 16;
	src |= regs.irc;
	do_cycles_ce (2 * 256);
	if (!cctrue(0)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)src);
		goto endlabel4708;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	return;
didnt_jump:;
	m68k_incpc (4);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4708: ;
}
/* BSR.W #<data>.W */
void REGPARAM2 CPUFUNC(op_6100_6)(uae_u32 opcode)
{
{	uae_s32 s;
{	uae_s16 src = regs.irc;
	s = (uae_s32)src + 2;
	if (src & 1) {
		exception3i (opcode, m68k_getpc() + 2, m68k_getpc() + s);
		goto endlabel4709;
	}
	do_cycles_ce (2 * 256);
	m68k_do_bsr_ce (m68k_getpc() + 4, s);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4709: ;
}
/* BSR.B #<data> */
void REGPARAM2 CPUFUNC(op_6101_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
{	uae_s32 s;
{	uae_u32 src = srcreg;
	s = (uae_s32)src + 2;
	if (src & 1) {
		exception3i (opcode, m68k_getpc() + 2, m68k_getpc() + s);
		goto endlabel4710;
	}
	do_cycles_ce (2 * 256);
	m68k_do_bsr_ce (m68k_getpc() + 2, s);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4710: ;
}
/* BSR.L #<data>.L */
void REGPARAM2 CPUFUNC(op_61ff_6)(uae_u32 opcode)
{
{	uae_s32 s;
{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= regs.irc;
	s = (uae_s32)src + 2;
	if (src & 1) {
		exception3i (opcode, m68k_getpc() + 2, m68k_getpc() + s);
		goto endlabel4711;
	}
	do_cycles_ce (2 * 256);
	m68k_do_bsr_ce (m68k_getpc() + 6, s);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4711: ;
}
/* Bcc.W #<data>.W */
void REGPARAM2 CPUFUNC(op_6200_6)(uae_u32 opcode)
{
{{	uae_s16 src = regs.irc;
	do_cycles_ce (2 * 256);
	if (!cctrue(2)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)src);
		goto endlabel4712;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	return;
didnt_jump:;
	m68k_incpc (4);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4712: ;
}
/* Bcc.B #<data> */
void REGPARAM2 CPUFUNC(op_6201_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
{{	uae_u32 src = srcreg;
	do_cycles_ce (2 * 256);
	if (!cctrue(2)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)src);
		goto endlabel4713;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	return;
didnt_jump:;
	m68k_incpc (2);
	do_cycles_ce (2 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4713: ;
}
/* Bcc.L #<data>.L */
void REGPARAM2 CPUFUNC(op_62ff_6)(uae_u32 opcode)
{
{	do_cycles_ce (2 * 256);
	if (cctrue(2)) {
		exception3i (opcode, m68k_getpc() + 2, m68k_getpc() + 1);
		goto endlabel4714;
	}
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	goto endlabel4714;
{	uae_s32 src;
	src = get_word_ce_prefetch (2) << 16;
	src |= regs.irc;
	do_cycles_ce (2 * 256);
	if (!cctrue(2)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)src);
		goto endlabel4714;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	return;
didnt_jump:;
	m68k_incpc (4);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4714: ;
}
/* Bcc.W #<data>.W */
void REGPARAM2 CPUFUNC(op_6300_6)(uae_u32 opcode)
{
{{	uae_s16 src = regs.irc;
	do_cycles_ce (2 * 256);
	if (!cctrue(3)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)src);
		goto endlabel4715;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	return;
didnt_jump:;
	m68k_incpc (4);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4715: ;
}
/* Bcc.B #<data> */
void REGPARAM2 CPUFUNC(op_6301_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
{{	uae_u32 src = srcreg;
	do_cycles_ce (2 * 256);
	if (!cctrue(3)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)src);
		goto endlabel4716;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	return;
didnt_jump:;
	m68k_incpc (2);
	do_cycles_ce (2 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4716: ;
}
/* Bcc.L #<data>.L */
void REGPARAM2 CPUFUNC(op_63ff_6)(uae_u32 opcode)
{
{	do_cycles_ce (2 * 256);
	if (cctrue(3)) {
		exception3i (opcode, m68k_getpc() + 2, m68k_getpc() + 1);
		goto endlabel4717;
	}
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	goto endlabel4717;
{	uae_s32 src;
	src = get_word_ce_prefetch (2) << 16;
	src |= regs.irc;
	do_cycles_ce (2 * 256);
	if (!cctrue(3)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)src);
		goto endlabel4717;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	return;
didnt_jump:;
	m68k_incpc (4);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4717: ;
}
/* Bcc.W #<data>.W */
void REGPARAM2 CPUFUNC(op_6400_6)(uae_u32 opcode)
{
{{	uae_s16 src = regs.irc;
	do_cycles_ce (2 * 256);
	if (!cctrue(4)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)src);
		goto endlabel4718;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	return;
didnt_jump:;
	m68k_incpc (4);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4718: ;
}
/* Bcc.B #<data> */
void REGPARAM2 CPUFUNC(op_6401_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
{{	uae_u32 src = srcreg;
	do_cycles_ce (2 * 256);
	if (!cctrue(4)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)src);
		goto endlabel4719;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	return;
didnt_jump:;
	m68k_incpc (2);
	do_cycles_ce (2 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4719: ;
}
/* Bcc.L #<data>.L */
void REGPARAM2 CPUFUNC(op_64ff_6)(uae_u32 opcode)
{
{	do_cycles_ce (2 * 256);
	if (cctrue(4)) {
		exception3i (opcode, m68k_getpc() + 2, m68k_getpc() + 1);
		goto endlabel4720;
	}
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	goto endlabel4720;
{	uae_s32 src;
	src = get_word_ce_prefetch (2) << 16;
	src |= regs.irc;
	do_cycles_ce (2 * 256);
	if (!cctrue(4)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)src);
		goto endlabel4720;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	return;
didnt_jump:;
	m68k_incpc (4);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4720: ;
}
/* Bcc.W #<data>.W */
void REGPARAM2 CPUFUNC(op_6500_6)(uae_u32 opcode)
{
{{	uae_s16 src = regs.irc;
	do_cycles_ce (2 * 256);
	if (!cctrue(5)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)src);
		goto endlabel4721;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	return;
didnt_jump:;
	m68k_incpc (4);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4721: ;
}
/* Bcc.B #<data> */
void REGPARAM2 CPUFUNC(op_6501_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
{{	uae_u32 src = srcreg;
	do_cycles_ce (2 * 256);
	if (!cctrue(5)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)src);
		goto endlabel4722;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	return;
didnt_jump:;
	m68k_incpc (2);
	do_cycles_ce (2 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4722: ;
}
/* Bcc.L #<data>.L */
void REGPARAM2 CPUFUNC(op_65ff_6)(uae_u32 opcode)
{
{	do_cycles_ce (2 * 256);
	if (cctrue(5)) {
		exception3i (opcode, m68k_getpc() + 2, m68k_getpc() + 1);
		goto endlabel4723;
	}
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	goto endlabel4723;
{	uae_s32 src;
	src = get_word_ce_prefetch (2) << 16;
	src |= regs.irc;
	do_cycles_ce (2 * 256);
	if (!cctrue(5)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)src);
		goto endlabel4723;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	return;
didnt_jump:;
	m68k_incpc (4);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4723: ;
}
/* Bcc.W #<data>.W */
void REGPARAM2 CPUFUNC(op_6600_6)(uae_u32 opcode)
{
{{	uae_s16 src = regs.irc;
	do_cycles_ce (2 * 256);
	if (!cctrue(6)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)src);
		goto endlabel4724;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	return;
didnt_jump:;
	m68k_incpc (4);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4724: ;
}
/* Bcc.B #<data> */
void REGPARAM2 CPUFUNC(op_6601_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
{{	uae_u32 src = srcreg;
	do_cycles_ce (2 * 256);
	if (!cctrue(6)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)src);
		goto endlabel4725;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	return;
didnt_jump:;
	m68k_incpc (2);
	do_cycles_ce (2 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4725: ;
}
/* Bcc.L #<data>.L */
void REGPARAM2 CPUFUNC(op_66ff_6)(uae_u32 opcode)
{
{	do_cycles_ce (2 * 256);
	if (cctrue(6)) {
		exception3i (opcode, m68k_getpc() + 2, m68k_getpc() + 1);
		goto endlabel4726;
	}
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	goto endlabel4726;
{	uae_s32 src;
	src = get_word_ce_prefetch (2) << 16;
	src |= regs.irc;
	do_cycles_ce (2 * 256);
	if (!cctrue(6)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)src);
		goto endlabel4726;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	return;
didnt_jump:;
	m68k_incpc (4);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4726: ;
}
/* Bcc.W #<data>.W */
void REGPARAM2 CPUFUNC(op_6700_6)(uae_u32 opcode)
{
{{	uae_s16 src = regs.irc;
	do_cycles_ce (2 * 256);
	if (!cctrue(7)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)src);
		goto endlabel4727;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	return;
didnt_jump:;
	m68k_incpc (4);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4727: ;
}
/* Bcc.B #<data> */
void REGPARAM2 CPUFUNC(op_6701_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
{{	uae_u32 src = srcreg;
	do_cycles_ce (2 * 256);
	if (!cctrue(7)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)src);
		goto endlabel4728;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	return;
didnt_jump:;
	m68k_incpc (2);
	do_cycles_ce (2 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4728: ;
}
/* Bcc.L #<data>.L */
void REGPARAM2 CPUFUNC(op_67ff_6)(uae_u32 opcode)
{
{	do_cycles_ce (2 * 256);
	if (cctrue(7)) {
		exception3i (opcode, m68k_getpc() + 2, m68k_getpc() + 1);
		goto endlabel4729;
	}
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	goto endlabel4729;
{	uae_s32 src;
	src = get_word_ce_prefetch (2) << 16;
	src |= regs.irc;
	do_cycles_ce (2 * 256);
	if (!cctrue(7)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)src);
		goto endlabel4729;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	return;
didnt_jump:;
	m68k_incpc (4);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4729: ;
}
/* Bcc.W #<data>.W */
void REGPARAM2 CPUFUNC(op_6800_6)(uae_u32 opcode)
{
{{	uae_s16 src = regs.irc;
	do_cycles_ce (2 * 256);
	if (!cctrue(8)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)src);
		goto endlabel4730;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	return;
didnt_jump:;
	m68k_incpc (4);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4730: ;
}
/* Bcc.B #<data> */
void REGPARAM2 CPUFUNC(op_6801_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
{{	uae_u32 src = srcreg;
	do_cycles_ce (2 * 256);
	if (!cctrue(8)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)src);
		goto endlabel4731;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	return;
didnt_jump:;
	m68k_incpc (2);
	do_cycles_ce (2 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4731: ;
}
/* Bcc.L #<data>.L */
void REGPARAM2 CPUFUNC(op_68ff_6)(uae_u32 opcode)
{
{	do_cycles_ce (2 * 256);
	if (cctrue(8)) {
		exception3i (opcode, m68k_getpc() + 2, m68k_getpc() + 1);
		goto endlabel4732;
	}
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	goto endlabel4732;
{	uae_s32 src;
	src = get_word_ce_prefetch (2) << 16;
	src |= regs.irc;
	do_cycles_ce (2 * 256);
	if (!cctrue(8)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)src);
		goto endlabel4732;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	return;
didnt_jump:;
	m68k_incpc (4);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4732: ;
}
/* Bcc.W #<data>.W */
void REGPARAM2 CPUFUNC(op_6900_6)(uae_u32 opcode)
{
{{	uae_s16 src = regs.irc;
	do_cycles_ce (2 * 256);
	if (!cctrue(9)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)src);
		goto endlabel4733;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	return;
didnt_jump:;
	m68k_incpc (4);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4733: ;
}
/* Bcc.B #<data> */
void REGPARAM2 CPUFUNC(op_6901_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
{{	uae_u32 src = srcreg;
	do_cycles_ce (2 * 256);
	if (!cctrue(9)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)src);
		goto endlabel4734;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	return;
didnt_jump:;
	m68k_incpc (2);
	do_cycles_ce (2 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4734: ;
}
/* Bcc.L #<data>.L */
void REGPARAM2 CPUFUNC(op_69ff_6)(uae_u32 opcode)
{
{	do_cycles_ce (2 * 256);
	if (cctrue(9)) {
		exception3i (opcode, m68k_getpc() + 2, m68k_getpc() + 1);
		goto endlabel4735;
	}
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	goto endlabel4735;
{	uae_s32 src;
	src = get_word_ce_prefetch (2) << 16;
	src |= regs.irc;
	do_cycles_ce (2 * 256);
	if (!cctrue(9)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)src);
		goto endlabel4735;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	return;
didnt_jump:;
	m68k_incpc (4);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4735: ;
}
/* Bcc.W #<data>.W */
void REGPARAM2 CPUFUNC(op_6a00_6)(uae_u32 opcode)
{
{{	uae_s16 src = regs.irc;
	do_cycles_ce (2 * 256);
	if (!cctrue(10)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)src);
		goto endlabel4736;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	return;
didnt_jump:;
	m68k_incpc (4);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4736: ;
}
/* Bcc.B #<data> */
void REGPARAM2 CPUFUNC(op_6a01_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
{{	uae_u32 src = srcreg;
	do_cycles_ce (2 * 256);
	if (!cctrue(10)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)src);
		goto endlabel4737;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	return;
didnt_jump:;
	m68k_incpc (2);
	do_cycles_ce (2 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4737: ;
}
/* Bcc.L #<data>.L */
void REGPARAM2 CPUFUNC(op_6aff_6)(uae_u32 opcode)
{
{	do_cycles_ce (2 * 256);
	if (cctrue(10)) {
		exception3i (opcode, m68k_getpc() + 2, m68k_getpc() + 1);
		goto endlabel4738;
	}
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	goto endlabel4738;
{	uae_s32 src;
	src = get_word_ce_prefetch (2) << 16;
	src |= regs.irc;
	do_cycles_ce (2 * 256);
	if (!cctrue(10)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)src);
		goto endlabel4738;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	return;
didnt_jump:;
	m68k_incpc (4);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4738: ;
}
/* Bcc.W #<data>.W */
void REGPARAM2 CPUFUNC(op_6b00_6)(uae_u32 opcode)
{
{{	uae_s16 src = regs.irc;
	do_cycles_ce (2 * 256);
	if (!cctrue(11)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)src);
		goto endlabel4739;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	return;
didnt_jump:;
	m68k_incpc (4);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4739: ;
}
/* Bcc.B #<data> */
void REGPARAM2 CPUFUNC(op_6b01_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
{{	uae_u32 src = srcreg;
	do_cycles_ce (2 * 256);
	if (!cctrue(11)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)src);
		goto endlabel4740;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	return;
didnt_jump:;
	m68k_incpc (2);
	do_cycles_ce (2 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4740: ;
}
/* Bcc.L #<data>.L */
void REGPARAM2 CPUFUNC(op_6bff_6)(uae_u32 opcode)
{
{	do_cycles_ce (2 * 256);
	if (cctrue(11)) {
		exception3i (opcode, m68k_getpc() + 2, m68k_getpc() + 1);
		goto endlabel4741;
	}
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	goto endlabel4741;
{	uae_s32 src;
	src = get_word_ce_prefetch (2) << 16;
	src |= regs.irc;
	do_cycles_ce (2 * 256);
	if (!cctrue(11)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)src);
		goto endlabel4741;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	return;
didnt_jump:;
	m68k_incpc (4);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4741: ;
}
/* Bcc.W #<data>.W */
void REGPARAM2 CPUFUNC(op_6c00_6)(uae_u32 opcode)
{
{{	uae_s16 src = regs.irc;
	do_cycles_ce (2 * 256);
	if (!cctrue(12)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)src);
		goto endlabel4742;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	return;
didnt_jump:;
	m68k_incpc (4);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4742: ;
}
/* Bcc.B #<data> */
void REGPARAM2 CPUFUNC(op_6c01_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
{{	uae_u32 src = srcreg;
	do_cycles_ce (2 * 256);
	if (!cctrue(12)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)src);
		goto endlabel4743;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	return;
didnt_jump:;
	m68k_incpc (2);
	do_cycles_ce (2 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4743: ;
}
/* Bcc.L #<data>.L */
void REGPARAM2 CPUFUNC(op_6cff_6)(uae_u32 opcode)
{
{	do_cycles_ce (2 * 256);
	if (cctrue(12)) {
		exception3i (opcode, m68k_getpc() + 2, m68k_getpc() + 1);
		goto endlabel4744;
	}
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	goto endlabel4744;
{	uae_s32 src;
	src = get_word_ce_prefetch (2) << 16;
	src |= regs.irc;
	do_cycles_ce (2 * 256);
	if (!cctrue(12)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)src);
		goto endlabel4744;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	return;
didnt_jump:;
	m68k_incpc (4);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4744: ;
}
/* Bcc.W #<data>.W */
void REGPARAM2 CPUFUNC(op_6d00_6)(uae_u32 opcode)
{
{{	uae_s16 src = regs.irc;
	do_cycles_ce (2 * 256);
	if (!cctrue(13)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)src);
		goto endlabel4745;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	return;
didnt_jump:;
	m68k_incpc (4);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4745: ;
}
/* Bcc.B #<data> */
void REGPARAM2 CPUFUNC(op_6d01_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
{{	uae_u32 src = srcreg;
	do_cycles_ce (2 * 256);
	if (!cctrue(13)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)src);
		goto endlabel4746;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	return;
didnt_jump:;
	m68k_incpc (2);
	do_cycles_ce (2 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4746: ;
}
/* Bcc.L #<data>.L */
void REGPARAM2 CPUFUNC(op_6dff_6)(uae_u32 opcode)
{
{	do_cycles_ce (2 * 256);
	if (cctrue(13)) {
		exception3i (opcode, m68k_getpc() + 2, m68k_getpc() + 1);
		goto endlabel4747;
	}
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	goto endlabel4747;
{	uae_s32 src;
	src = get_word_ce_prefetch (2) << 16;
	src |= regs.irc;
	do_cycles_ce (2 * 256);
	if (!cctrue(13)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)src);
		goto endlabel4747;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	return;
didnt_jump:;
	m68k_incpc (4);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4747: ;
}
/* Bcc.W #<data>.W */
void REGPARAM2 CPUFUNC(op_6e00_6)(uae_u32 opcode)
{
{{	uae_s16 src = regs.irc;
	do_cycles_ce (2 * 256);
	if (!cctrue(14)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)src);
		goto endlabel4748;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	return;
didnt_jump:;
	m68k_incpc (4);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4748: ;
}
/* Bcc.B #<data> */
void REGPARAM2 CPUFUNC(op_6e01_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
{{	uae_u32 src = srcreg;
	do_cycles_ce (2 * 256);
	if (!cctrue(14)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)src);
		goto endlabel4749;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	return;
didnt_jump:;
	m68k_incpc (2);
	do_cycles_ce (2 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4749: ;
}
/* Bcc.L #<data>.L */
void REGPARAM2 CPUFUNC(op_6eff_6)(uae_u32 opcode)
{
{	do_cycles_ce (2 * 256);
	if (cctrue(14)) {
		exception3i (opcode, m68k_getpc() + 2, m68k_getpc() + 1);
		goto endlabel4750;
	}
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	goto endlabel4750;
{	uae_s32 src;
	src = get_word_ce_prefetch (2) << 16;
	src |= regs.irc;
	do_cycles_ce (2 * 256);
	if (!cctrue(14)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)src);
		goto endlabel4750;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	return;
didnt_jump:;
	m68k_incpc (4);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4750: ;
}
/* Bcc.W #<data>.W */
void REGPARAM2 CPUFUNC(op_6f00_6)(uae_u32 opcode)
{
{{	uae_s16 src = regs.irc;
	do_cycles_ce (2 * 256);
	if (!cctrue(15)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)src);
		goto endlabel4751;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	return;
didnt_jump:;
	m68k_incpc (4);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4751: ;
}
/* Bcc.B #<data> */
void REGPARAM2 CPUFUNC(op_6f01_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
{{	uae_u32 src = srcreg;
	do_cycles_ce (2 * 256);
	if (!cctrue(15)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)src);
		goto endlabel4752;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	return;
didnt_jump:;
	m68k_incpc (2);
	do_cycles_ce (2 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4752: ;
}
/* Bcc.L #<data>.L */
void REGPARAM2 CPUFUNC(op_6fff_6)(uae_u32 opcode)
{
{	do_cycles_ce (2 * 256);
	if (cctrue(15)) {
		exception3i (opcode, m68k_getpc() + 2, m68k_getpc() + 1);
		goto endlabel4753;
	}
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	goto endlabel4753;
{	uae_s32 src;
	src = get_word_ce_prefetch (2) << 16;
	src |= regs.irc;
	do_cycles_ce (2 * 256);
	if (!cctrue(15)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc() + 2, m68k_getpc() + 2 + (uae_s32)src);
		goto endlabel4753;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
	return;
didnt_jump:;
	m68k_incpc (4);
	get_word_ce_prefetch (0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (2);
}}endlabel4753: ;
}
/* MOVE.L #<data>,Dn */
void REGPARAM2 CPUFUNC(op_7000_6)(uae_u32 opcode)
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
}}}}
/* OR.B Dn,Dn */
void REGPARAM2 CPUFUNC(op_8000_6)(uae_u32 opcode)
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
}
/* OR.B (An),Dn */
void REGPARAM2 CPUFUNC(op_8010_6)(uae_u32 opcode)
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
}
/* OR.B (An)+,Dn */
void REGPARAM2 CPUFUNC(op_8018_6)(uae_u32 opcode)
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
}
/* OR.B -(An),Dn */
void REGPARAM2 CPUFUNC(op_8020_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
	do_cycles_ce (2 * 256);
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
}
/* OR.B (d16,An),Dn */
void REGPARAM2 CPUFUNC(op_8028_6)(uae_u32 opcode)
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
}
/* OR.B (d8,An,Xn),Dn */
void REGPARAM2 CPUFUNC(op_8030_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
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
}
/* OR.B (xxx).W,Dn */
void REGPARAM2 CPUFUNC(op_8038_6)(uae_u32 opcode)
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
}
/* OR.B (xxx).L,Dn */
void REGPARAM2 CPUFUNC(op_8039_6)(uae_u32 opcode)
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
}
/* OR.B (d16,PC),Dn */
void REGPARAM2 CPUFUNC(op_803a_6)(uae_u32 opcode)
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
}
/* OR.B (d8,PC,Xn),Dn */
void REGPARAM2 CPUFUNC(op_803b_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
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
}
/* OR.B #<data>.B,Dn */
void REGPARAM2 CPUFUNC(op_803c_6)(uae_u32 opcode)
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
}
/* OR.W Dn,Dn */
void REGPARAM2 CPUFUNC(op_8040_6)(uae_u32 opcode)
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
}
/* OR.W (An),Dn */
void REGPARAM2 CPUFUNC(op_8050_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4767;
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
endlabel4767: ;
}
/* OR.W (An)+,Dn */
void REGPARAM2 CPUFUNC(op_8058_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4768;
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
endlabel4768: ;
}
/* OR.W -(An),Dn */
void REGPARAM2 CPUFUNC(op_8060_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg(regs, srcreg) - 2;
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4769;
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
endlabel4769: ;
}
/* OR.W (d16,An),Dn */
void REGPARAM2 CPUFUNC(op_8068_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4770;
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
endlabel4770: ;
}
/* OR.W (d8,An,Xn),Dn */
void REGPARAM2 CPUFUNC(op_8070_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4771;
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
endlabel4771: ;
}
/* OR.W (xxx).W,Dn */
void REGPARAM2 CPUFUNC(op_8078_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4772;
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
endlabel4772: ;
}
/* OR.W (xxx).L,Dn */
void REGPARAM2 CPUFUNC(op_8079_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4773;
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
endlabel4773: ;
}
/* OR.W (d16,PC),Dn */
void REGPARAM2 CPUFUNC(op_807a_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4774;
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
endlabel4774: ;
}
/* OR.W (d8,PC,Xn),Dn */
void REGPARAM2 CPUFUNC(op_807b_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4775;
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
endlabel4775: ;
}
/* OR.W #<data>.W,Dn */
void REGPARAM2 CPUFUNC(op_807c_6)(uae_u32 opcode)
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
}
/* OR.L Dn,Dn */
void REGPARAM2 CPUFUNC(op_8080_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	src |= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	do_cycles_ce (4 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	m68k_dreg(regs, dstreg) = (src);
}}}	m68k_incpc (2);
}
/* OR.L (An),Dn */
void REGPARAM2 CPUFUNC(op_8090_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4778;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	src |= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	do_cycles_ce (4 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	m68k_dreg(regs, dstreg) = (src);
}}}}}	m68k_incpc (2);
endlabel4778: ;
}
/* OR.L (An)+,Dn */
void REGPARAM2 CPUFUNC(op_8098_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4779;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	m68k_areg(regs, srcreg) += 4;
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	src |= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	do_cycles_ce (4 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	m68k_dreg(regs, dstreg) = (src);
}}}}}	m68k_incpc (2);
endlabel4779: ;
}
/* OR.L -(An),Dn */
void REGPARAM2 CPUFUNC(op_80a0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg(regs, srcreg) - 4;
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4780;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	m68k_areg (regs, srcreg) = srca;
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	src |= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	do_cycles_ce (4 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	m68k_dreg(regs, dstreg) = (src);
}}}}}	m68k_incpc (2);
endlabel4780: ;
}
/* OR.L (d16,An),Dn */
void REGPARAM2 CPUFUNC(op_80a8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4781;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	src |= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	do_cycles_ce (4 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	m68k_dreg(regs, dstreg) = (src);
}}}}}	m68k_incpc (4);
endlabel4781: ;
}
/* OR.L (d8,An,Xn),Dn */
void REGPARAM2 CPUFUNC(op_80b0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4782;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	src |= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	do_cycles_ce (4 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	m68k_dreg(regs, dstreg) = (src);
}}}}}	m68k_incpc (4);
endlabel4782: ;
}
/* OR.L (xxx).W,Dn */
void REGPARAM2 CPUFUNC(op_80b8_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4783;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	src |= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	do_cycles_ce (4 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	m68k_dreg(regs, dstreg) = (src);
}}}}}	m68k_incpc (4);
endlabel4783: ;
}
/* OR.L (xxx).L,Dn */
void REGPARAM2 CPUFUNC(op_80b9_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4784;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	src |= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	do_cycles_ce (4 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	m68k_dreg(regs, dstreg) = (src);
}}}}}	m68k_incpc (6);
endlabel4784: ;
}
/* OR.L (d16,PC),Dn */
void REGPARAM2 CPUFUNC(op_80ba_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4785;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	src |= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	do_cycles_ce (4 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	m68k_dreg(regs, dstreg) = (src);
}}}}}	m68k_incpc (4);
endlabel4785: ;
}
/* OR.L (d8,PC,Xn),Dn */
void REGPARAM2 CPUFUNC(op_80bb_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4786;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	src |= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	do_cycles_ce (4 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	m68k_dreg(regs, dstreg) = (src);
}}}}}	m68k_incpc (4);
endlabel4786: ;
}
/* OR.L #<data>.L,Dn */
void REGPARAM2 CPUFUNC(op_80bc_6)(uae_u32 opcode)
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
	do_cycles_ce (4 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	m68k_dreg(regs, dstreg) = (src);
}}}	m68k_incpc (6);
}
/* DIVU.W Dn,Dn */
void REGPARAM2 CPUFUNC(op_80c0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{	uaecptr oldpc = m68k_getpc();
{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	CLEAR_CZNV;
	if (src == 0) {
		m68k_incpc (2);
		Exception (5, oldpc);
		goto endlabel4788;
	} else {
		uae_u32 newv = (uae_u32)dst / (uae_u32)(uae_u16)src;
		uae_u32 rem = (uae_u32)dst % (uae_u32)(uae_u16)src;
		int cycles = getDivu68kCycles((uae_u32)dst, (uae_u16)src) * 256;
		if (cycles > 0) do_cycles_ce (cycles);
		if (newv > 0xffff) {
			SET_VFLG (1);
			SET_NFLG (1);
		} else {
			CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_NFLG (((uae_s16)(newv)) < 0);
			newv = (newv & 0xffff) | ((uae_u32)rem << 16);
			m68k_dreg(regs, dstreg) = (newv);
		}
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	m68k_incpc (2);
	}
}}}endlabel4788: ;
}
/* DIVU.W (An),Dn */
void REGPARAM2 CPUFUNC(op_80d0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{	uaecptr oldpc = m68k_getpc();
{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4789;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	CLEAR_CZNV;
	if (src == 0) {
		m68k_incpc (2);
		Exception (5, oldpc);
		goto endlabel4789;
	} else {
		uae_u32 newv = (uae_u32)dst / (uae_u32)(uae_u16)src;
		uae_u32 rem = (uae_u32)dst % (uae_u32)(uae_u16)src;
		int cycles = getDivu68kCycles((uae_u32)dst, (uae_u16)src) * 256;
		if (cycles > 0) do_cycles_ce (cycles);
		if (newv > 0xffff) {
			SET_VFLG (1);
			SET_NFLG (1);
		} else {
			CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_NFLG (((uae_s16)(newv)) < 0);
			newv = (newv & 0xffff) | ((uae_u32)rem << 16);
			m68k_dreg(regs, dstreg) = (newv);
		}
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	m68k_incpc (2);
	}
}}}}}endlabel4789: ;
}
/* DIVU.W (An)+,Dn */
void REGPARAM2 CPUFUNC(op_80d8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{	uaecptr oldpc = m68k_getpc();
{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4790;
	}
{{	uae_s16 src = get_word_ce (srca);
	m68k_areg(regs, srcreg) += 2;
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	CLEAR_CZNV;
	if (src == 0) {
		m68k_incpc (2);
		Exception (5, oldpc);
		goto endlabel4790;
	} else {
		uae_u32 newv = (uae_u32)dst / (uae_u32)(uae_u16)src;
		uae_u32 rem = (uae_u32)dst % (uae_u32)(uae_u16)src;
		int cycles = getDivu68kCycles((uae_u32)dst, (uae_u16)src) * 256;
		if (cycles > 0) do_cycles_ce (cycles);
		if (newv > 0xffff) {
			SET_VFLG (1);
			SET_NFLG (1);
		} else {
			CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_NFLG (((uae_s16)(newv)) < 0);
			newv = (newv & 0xffff) | ((uae_u32)rem << 16);
			m68k_dreg(regs, dstreg) = (newv);
		}
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	m68k_incpc (2);
	}
}}}}}endlabel4790: ;
}
/* DIVU.W -(An),Dn */
void REGPARAM2 CPUFUNC(op_80e0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{	uaecptr oldpc = m68k_getpc();
{	uaecptr srca;
	srca = m68k_areg(regs, srcreg) - 2;
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4791;
	}
{{	uae_s16 src = get_word_ce (srca);
	m68k_areg (regs, srcreg) = srca;
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	CLEAR_CZNV;
	if (src == 0) {
		m68k_incpc (2);
		Exception (5, oldpc);
		goto endlabel4791;
	} else {
		uae_u32 newv = (uae_u32)dst / (uae_u32)(uae_u16)src;
		uae_u32 rem = (uae_u32)dst % (uae_u32)(uae_u16)src;
		int cycles = getDivu68kCycles((uae_u32)dst, (uae_u16)src) * 256;
		if (cycles > 0) do_cycles_ce (cycles);
		if (newv > 0xffff) {
			SET_VFLG (1);
			SET_NFLG (1);
		} else {
			CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_NFLG (((uae_s16)(newv)) < 0);
			newv = (newv & 0xffff) | ((uae_u32)rem << 16);
			m68k_dreg(regs, dstreg) = (newv);
		}
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	m68k_incpc (2);
	}
}}}}}endlabel4791: ;
}
/* DIVU.W (d16,An),Dn */
void REGPARAM2 CPUFUNC(op_80e8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{	uaecptr oldpc = m68k_getpc();
{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4792;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	CLEAR_CZNV;
	if (src == 0) {
		m68k_incpc (4);
		Exception (5, oldpc);
		goto endlabel4792;
	} else {
		uae_u32 newv = (uae_u32)dst / (uae_u32)(uae_u16)src;
		uae_u32 rem = (uae_u32)dst % (uae_u32)(uae_u16)src;
		int cycles = getDivu68kCycles((uae_u32)dst, (uae_u16)src) * 256;
		if (cycles > 0) do_cycles_ce (cycles);
		if (newv > 0xffff) {
			SET_VFLG (1);
			SET_NFLG (1);
		} else {
			CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_NFLG (((uae_s16)(newv)) < 0);
			newv = (newv & 0xffff) | ((uae_u32)rem << 16);
			m68k_dreg(regs, dstreg) = (newv);
		}
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	m68k_incpc (4);
	}
}}}}}endlabel4792: ;
}
/* DIVU.W (d8,An,Xn),Dn */
void REGPARAM2 CPUFUNC(op_80f0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{	uaecptr oldpc = m68k_getpc();
{	uaecptr srca;
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4793;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	CLEAR_CZNV;
	if (src == 0) {
		m68k_incpc (4);
		Exception (5, oldpc);
		goto endlabel4793;
	} else {
		uae_u32 newv = (uae_u32)dst / (uae_u32)(uae_u16)src;
		uae_u32 rem = (uae_u32)dst % (uae_u32)(uae_u16)src;
		int cycles = getDivu68kCycles((uae_u32)dst, (uae_u16)src) * 256;
		if (cycles > 0) do_cycles_ce (cycles);
		if (newv > 0xffff) {
			SET_VFLG (1);
			SET_NFLG (1);
		} else {
			CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_NFLG (((uae_s16)(newv)) < 0);
			newv = (newv & 0xffff) | ((uae_u32)rem << 16);
			m68k_dreg(regs, dstreg) = (newv);
		}
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	m68k_incpc (4);
	}
}}}}}endlabel4793: ;
}
/* DIVU.W (xxx).W,Dn */
void REGPARAM2 CPUFUNC(op_80f8_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{	uaecptr oldpc = m68k_getpc();
{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4794;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	CLEAR_CZNV;
	if (src == 0) {
		m68k_incpc (4);
		Exception (5, oldpc);
		goto endlabel4794;
	} else {
		uae_u32 newv = (uae_u32)dst / (uae_u32)(uae_u16)src;
		uae_u32 rem = (uae_u32)dst % (uae_u32)(uae_u16)src;
		int cycles = getDivu68kCycles((uae_u32)dst, (uae_u16)src) * 256;
		if (cycles > 0) do_cycles_ce (cycles);
		if (newv > 0xffff) {
			SET_VFLG (1);
			SET_NFLG (1);
		} else {
			CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_NFLG (((uae_s16)(newv)) < 0);
			newv = (newv & 0xffff) | ((uae_u32)rem << 16);
			m68k_dreg(regs, dstreg) = (newv);
		}
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	m68k_incpc (4);
	}
}}}}}endlabel4794: ;
}
/* DIVU.W (xxx).L,Dn */
void REGPARAM2 CPUFUNC(op_80f9_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{	uaecptr oldpc = m68k_getpc();
{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4795;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	CLEAR_CZNV;
	if (src == 0) {
		m68k_incpc (6);
		Exception (5, oldpc);
		goto endlabel4795;
	} else {
		uae_u32 newv = (uae_u32)dst / (uae_u32)(uae_u16)src;
		uae_u32 rem = (uae_u32)dst % (uae_u32)(uae_u16)src;
		int cycles = getDivu68kCycles((uae_u32)dst, (uae_u16)src) * 256;
		if (cycles > 0) do_cycles_ce (cycles);
		if (newv > 0xffff) {
			SET_VFLG (1);
			SET_NFLG (1);
		} else {
			CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_NFLG (((uae_s16)(newv)) < 0);
			newv = (newv & 0xffff) | ((uae_u32)rem << 16);
			m68k_dreg(regs, dstreg) = (newv);
		}
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	m68k_incpc (6);
	}
}}}}}endlabel4795: ;
}
/* DIVU.W (d16,PC),Dn */
void REGPARAM2 CPUFUNC(op_80fa_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{	uaecptr oldpc = m68k_getpc();
{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4796;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	CLEAR_CZNV;
	if (src == 0) {
		m68k_incpc (4);
		Exception (5, oldpc);
		goto endlabel4796;
	} else {
		uae_u32 newv = (uae_u32)dst / (uae_u32)(uae_u16)src;
		uae_u32 rem = (uae_u32)dst % (uae_u32)(uae_u16)src;
		int cycles = getDivu68kCycles((uae_u32)dst, (uae_u16)src) * 256;
		if (cycles > 0) do_cycles_ce (cycles);
		if (newv > 0xffff) {
			SET_VFLG (1);
			SET_NFLG (1);
		} else {
			CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_NFLG (((uae_s16)(newv)) < 0);
			newv = (newv & 0xffff) | ((uae_u32)rem << 16);
			m68k_dreg(regs, dstreg) = (newv);
		}
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	m68k_incpc (4);
	}
}}}}}endlabel4796: ;
}
/* DIVU.W (d8,PC,Xn),Dn */
void REGPARAM2 CPUFUNC(op_80fb_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{	uaecptr oldpc = m68k_getpc();
{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4797;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	CLEAR_CZNV;
	if (src == 0) {
		m68k_incpc (4);
		Exception (5, oldpc);
		goto endlabel4797;
	} else {
		uae_u32 newv = (uae_u32)dst / (uae_u32)(uae_u16)src;
		uae_u32 rem = (uae_u32)dst % (uae_u32)(uae_u16)src;
		int cycles = getDivu68kCycles((uae_u32)dst, (uae_u16)src) * 256;
		if (cycles > 0) do_cycles_ce (cycles);
		if (newv > 0xffff) {
			SET_VFLG (1);
			SET_NFLG (1);
		} else {
			CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_NFLG (((uae_s16)(newv)) < 0);
			newv = (newv & 0xffff) | ((uae_u32)rem << 16);
			m68k_dreg(regs, dstreg) = (newv);
		}
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	m68k_incpc (4);
	}
}}}}}endlabel4797: ;
}
/* DIVU.W #<data>.W,Dn */
void REGPARAM2 CPUFUNC(op_80fc_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{	uaecptr oldpc = m68k_getpc();
{	uae_s16 src = get_word_ce_prefetch (4);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	CLEAR_CZNV;
	if (src == 0) {
		m68k_incpc (4);
		Exception (5, oldpc);
		goto endlabel4798;
	} else {
		uae_u32 newv = (uae_u32)dst / (uae_u32)(uae_u16)src;
		uae_u32 rem = (uae_u32)dst % (uae_u32)(uae_u16)src;
		int cycles = getDivu68kCycles((uae_u32)dst, (uae_u16)src) * 256;
		if (cycles > 0) do_cycles_ce (cycles);
		if (newv > 0xffff) {
			SET_VFLG (1);
			SET_NFLG (1);
		} else {
			CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_NFLG (((uae_s16)(newv)) < 0);
			newv = (newv & 0xffff) | ((uae_u32)rem << 16);
			m68k_dreg(regs, dstreg) = (newv);
		}
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	m68k_incpc (4);
	}
}}}endlabel4798: ;
}
/* SBCD.B Dn,Dn */
void REGPARAM2 CPUFUNC(op_8100_6)(uae_u32 opcode)
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
	do_cycles_ce (2 * 256);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xff) | ((newv) & 0xff);
}}}}	m68k_incpc (2);
}
/* SBCD.B -(An),-(An) */
void REGPARAM2 CPUFUNC(op_8108_6)(uae_u32 opcode)
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
	do_cycles_ce (2 * 256);
	put_byte_ce (dsta,newv);
}}}}}}	m68k_incpc (2);
}
/* OR.B Dn,(An) */
void REGPARAM2 CPUFUNC(op_8110_6)(uae_u32 opcode)
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
}
/* OR.B Dn,(An)+ */
void REGPARAM2 CPUFUNC(op_8118_6)(uae_u32 opcode)
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
}
/* OR.B Dn,-(An) */
void REGPARAM2 CPUFUNC(op_8120_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - areg_byteinc[dstreg];
	do_cycles_ce (2 * 256);
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
}
/* OR.B Dn,(d16,An) */
void REGPARAM2 CPUFUNC(op_8128_6)(uae_u32 opcode)
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
}
/* OR.B Dn,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_8130_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
{	uae_s8 dst = get_byte_ce (dsta);
	src |= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	put_byte_ce (dsta,src);
}}}}	m68k_incpc (4);
}
/* OR.B Dn,(xxx).W */
void REGPARAM2 CPUFUNC(op_8138_6)(uae_u32 opcode)
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
}
/* OR.B Dn,(xxx).L */
void REGPARAM2 CPUFUNC(op_8139_6)(uae_u32 opcode)
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
}
/* OR.W Dn,(An) */
void REGPARAM2 CPUFUNC(op_8150_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4808;
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
endlabel4808: ;
}
/* OR.W Dn,(An)+ */
void REGPARAM2 CPUFUNC(op_8158_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4809;
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
endlabel4809: ;
}
/* OR.W Dn,-(An) */
void REGPARAM2 CPUFUNC(op_8160_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - 2;
	do_cycles_ce (2 * 256);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4810;
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
endlabel4810: ;
}
/* OR.W Dn,(d16,An) */
void REGPARAM2 CPUFUNC(op_8168_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4811;
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
endlabel4811: ;
}
/* OR.W Dn,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_8170_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4812;
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
endlabel4812: ;
}
/* OR.W Dn,(xxx).W */
void REGPARAM2 CPUFUNC(op_8178_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4813;
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
endlabel4813: ;
}
/* OR.W Dn,(xxx).L */
void REGPARAM2 CPUFUNC(op_8179_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (4) << 16;
	dsta |= get_word_ce_prefetch (6);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4814;
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
endlabel4814: ;
}
/* OR.L Dn,(An) */
void REGPARAM2 CPUFUNC(op_8190_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4815;
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
endlabel4815: ;
}
/* OR.L Dn,(An)+ */
void REGPARAM2 CPUFUNC(op_8198_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4816;
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
endlabel4816: ;
}
/* OR.L Dn,-(An) */
void REGPARAM2 CPUFUNC(op_81a0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - 4;
	do_cycles_ce (2 * 256);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4817;
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
endlabel4817: ;
}
/* OR.L Dn,(d16,An) */
void REGPARAM2 CPUFUNC(op_81a8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4818;
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
endlabel4818: ;
}
/* OR.L Dn,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_81b0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4819;
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
endlabel4819: ;
}
/* OR.L Dn,(xxx).W */
void REGPARAM2 CPUFUNC(op_81b8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4820;
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
endlabel4820: ;
}
/* OR.L Dn,(xxx).L */
void REGPARAM2 CPUFUNC(op_81b9_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (4) << 16;
	dsta |= get_word_ce_prefetch (6);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4821;
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
endlabel4821: ;
}
/* DIVS.W Dn,Dn */
void REGPARAM2 CPUFUNC(op_81c0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{	uaecptr oldpc = m68k_getpc();
{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	CLEAR_CZNV;
	if (src == 0) {
		m68k_incpc (2);
		Exception (5, oldpc);
		goto endlabel4822;
	} else {
		uae_s32 newv = (uae_s32)dst / (uae_s32)(uae_s16)src;
		uae_u16 rem = (uae_s32)dst % (uae_s32)(uae_s16)src;
		int cycles = getDivs68kCycles((uae_s32)dst, (uae_s16)src) * 256;
		if (cycles > 0) do_cycles_ce (cycles);
		if ((newv & 0xffff8000) != 0 && (newv & 0xffff8000) != 0xffff8000) {
			SET_VFLG (1);
			SET_NFLG (1);
		} else {
			if (((uae_s16)rem < 0) != ((uae_s32)dst < 0)) rem = -rem;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_NFLG (((uae_s16)(newv)) < 0);
			newv = (newv & 0xffff) | ((uae_u32)rem << 16);
			m68k_dreg(regs, dstreg) = (newv);
		}
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	m68k_incpc (2);
	}
}}}endlabel4822: ;
}
/* DIVS.W (An),Dn */
void REGPARAM2 CPUFUNC(op_81d0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{	uaecptr oldpc = m68k_getpc();
{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4823;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	CLEAR_CZNV;
	if (src == 0) {
		m68k_incpc (2);
		Exception (5, oldpc);
		goto endlabel4823;
	} else {
		uae_s32 newv = (uae_s32)dst / (uae_s32)(uae_s16)src;
		uae_u16 rem = (uae_s32)dst % (uae_s32)(uae_s16)src;
		int cycles = getDivs68kCycles((uae_s32)dst, (uae_s16)src) * 256;
		if (cycles > 0) do_cycles_ce (cycles);
		if ((newv & 0xffff8000) != 0 && (newv & 0xffff8000) != 0xffff8000) {
			SET_VFLG (1);
			SET_NFLG (1);
		} else {
			if (((uae_s16)rem < 0) != ((uae_s32)dst < 0)) rem = -rem;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_NFLG (((uae_s16)(newv)) < 0);
			newv = (newv & 0xffff) | ((uae_u32)rem << 16);
			m68k_dreg(regs, dstreg) = (newv);
		}
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	m68k_incpc (2);
	}
}}}}}endlabel4823: ;
}
/* DIVS.W (An)+,Dn */
void REGPARAM2 CPUFUNC(op_81d8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{	uaecptr oldpc = m68k_getpc();
{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4824;
	}
{{	uae_s16 src = get_word_ce (srca);
	m68k_areg(regs, srcreg) += 2;
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	CLEAR_CZNV;
	if (src == 0) {
		m68k_incpc (2);
		Exception (5, oldpc);
		goto endlabel4824;
	} else {
		uae_s32 newv = (uae_s32)dst / (uae_s32)(uae_s16)src;
		uae_u16 rem = (uae_s32)dst % (uae_s32)(uae_s16)src;
		int cycles = getDivs68kCycles((uae_s32)dst, (uae_s16)src) * 256;
		if (cycles > 0) do_cycles_ce (cycles);
		if ((newv & 0xffff8000) != 0 && (newv & 0xffff8000) != 0xffff8000) {
			SET_VFLG (1);
			SET_NFLG (1);
		} else {
			if (((uae_s16)rem < 0) != ((uae_s32)dst < 0)) rem = -rem;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_NFLG (((uae_s16)(newv)) < 0);
			newv = (newv & 0xffff) | ((uae_u32)rem << 16);
			m68k_dreg(regs, dstreg) = (newv);
		}
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	m68k_incpc (2);
	}
}}}}}endlabel4824: ;
}
/* DIVS.W -(An),Dn */
void REGPARAM2 CPUFUNC(op_81e0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{	uaecptr oldpc = m68k_getpc();
{	uaecptr srca;
	srca = m68k_areg(regs, srcreg) - 2;
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4825;
	}
{{	uae_s16 src = get_word_ce (srca);
	m68k_areg (regs, srcreg) = srca;
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	CLEAR_CZNV;
	if (src == 0) {
		m68k_incpc (2);
		Exception (5, oldpc);
		goto endlabel4825;
	} else {
		uae_s32 newv = (uae_s32)dst / (uae_s32)(uae_s16)src;
		uae_u16 rem = (uae_s32)dst % (uae_s32)(uae_s16)src;
		int cycles = getDivs68kCycles((uae_s32)dst, (uae_s16)src) * 256;
		if (cycles > 0) do_cycles_ce (cycles);
		if ((newv & 0xffff8000) != 0 && (newv & 0xffff8000) != 0xffff8000) {
			SET_VFLG (1);
			SET_NFLG (1);
		} else {
			if (((uae_s16)rem < 0) != ((uae_s32)dst < 0)) rem = -rem;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_NFLG (((uae_s16)(newv)) < 0);
			newv = (newv & 0xffff) | ((uae_u32)rem << 16);
			m68k_dreg(regs, dstreg) = (newv);
		}
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	m68k_incpc (2);
	}
}}}}}endlabel4825: ;
}
/* DIVS.W (d16,An),Dn */
void REGPARAM2 CPUFUNC(op_81e8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{	uaecptr oldpc = m68k_getpc();
{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4826;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	CLEAR_CZNV;
	if (src == 0) {
		m68k_incpc (4);
		Exception (5, oldpc);
		goto endlabel4826;
	} else {
		uae_s32 newv = (uae_s32)dst / (uae_s32)(uae_s16)src;
		uae_u16 rem = (uae_s32)dst % (uae_s32)(uae_s16)src;
		int cycles = getDivs68kCycles((uae_s32)dst, (uae_s16)src) * 256;
		if (cycles > 0) do_cycles_ce (cycles);
		if ((newv & 0xffff8000) != 0 && (newv & 0xffff8000) != 0xffff8000) {
			SET_VFLG (1);
			SET_NFLG (1);
		} else {
			if (((uae_s16)rem < 0) != ((uae_s32)dst < 0)) rem = -rem;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_NFLG (((uae_s16)(newv)) < 0);
			newv = (newv & 0xffff) | ((uae_u32)rem << 16);
			m68k_dreg(regs, dstreg) = (newv);
		}
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	m68k_incpc (4);
	}
}}}}}endlabel4826: ;
}
/* DIVS.W (d8,An,Xn),Dn */
void REGPARAM2 CPUFUNC(op_81f0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{	uaecptr oldpc = m68k_getpc();
{	uaecptr srca;
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4827;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	CLEAR_CZNV;
	if (src == 0) {
		m68k_incpc (4);
		Exception (5, oldpc);
		goto endlabel4827;
	} else {
		uae_s32 newv = (uae_s32)dst / (uae_s32)(uae_s16)src;
		uae_u16 rem = (uae_s32)dst % (uae_s32)(uae_s16)src;
		int cycles = getDivs68kCycles((uae_s32)dst, (uae_s16)src) * 256;
		if (cycles > 0) do_cycles_ce (cycles);
		if ((newv & 0xffff8000) != 0 && (newv & 0xffff8000) != 0xffff8000) {
			SET_VFLG (1);
			SET_NFLG (1);
		} else {
			if (((uae_s16)rem < 0) != ((uae_s32)dst < 0)) rem = -rem;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_NFLG (((uae_s16)(newv)) < 0);
			newv = (newv & 0xffff) | ((uae_u32)rem << 16);
			m68k_dreg(regs, dstreg) = (newv);
		}
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	m68k_incpc (4);
	}
}}}}}endlabel4827: ;
}
/* DIVS.W (xxx).W,Dn */
void REGPARAM2 CPUFUNC(op_81f8_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{	uaecptr oldpc = m68k_getpc();
{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4828;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	CLEAR_CZNV;
	if (src == 0) {
		m68k_incpc (4);
		Exception (5, oldpc);
		goto endlabel4828;
	} else {
		uae_s32 newv = (uae_s32)dst / (uae_s32)(uae_s16)src;
		uae_u16 rem = (uae_s32)dst % (uae_s32)(uae_s16)src;
		int cycles = getDivs68kCycles((uae_s32)dst, (uae_s16)src) * 256;
		if (cycles > 0) do_cycles_ce (cycles);
		if ((newv & 0xffff8000) != 0 && (newv & 0xffff8000) != 0xffff8000) {
			SET_VFLG (1);
			SET_NFLG (1);
		} else {
			if (((uae_s16)rem < 0) != ((uae_s32)dst < 0)) rem = -rem;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_NFLG (((uae_s16)(newv)) < 0);
			newv = (newv & 0xffff) | ((uae_u32)rem << 16);
			m68k_dreg(regs, dstreg) = (newv);
		}
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	m68k_incpc (4);
	}
}}}}}endlabel4828: ;
}
/* DIVS.W (xxx).L,Dn */
void REGPARAM2 CPUFUNC(op_81f9_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{	uaecptr oldpc = m68k_getpc();
{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4829;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	CLEAR_CZNV;
	if (src == 0) {
		m68k_incpc (6);
		Exception (5, oldpc);
		goto endlabel4829;
	} else {
		uae_s32 newv = (uae_s32)dst / (uae_s32)(uae_s16)src;
		uae_u16 rem = (uae_s32)dst % (uae_s32)(uae_s16)src;
		int cycles = getDivs68kCycles((uae_s32)dst, (uae_s16)src) * 256;
		if (cycles > 0) do_cycles_ce (cycles);
		if ((newv & 0xffff8000) != 0 && (newv & 0xffff8000) != 0xffff8000) {
			SET_VFLG (1);
			SET_NFLG (1);
		} else {
			if (((uae_s16)rem < 0) != ((uae_s32)dst < 0)) rem = -rem;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_NFLG (((uae_s16)(newv)) < 0);
			newv = (newv & 0xffff) | ((uae_u32)rem << 16);
			m68k_dreg(regs, dstreg) = (newv);
		}
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	m68k_incpc (6);
	}
}}}}}endlabel4829: ;
}
/* DIVS.W (d16,PC),Dn */
void REGPARAM2 CPUFUNC(op_81fa_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{	uaecptr oldpc = m68k_getpc();
{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4830;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	CLEAR_CZNV;
	if (src == 0) {
		m68k_incpc (4);
		Exception (5, oldpc);
		goto endlabel4830;
	} else {
		uae_s32 newv = (uae_s32)dst / (uae_s32)(uae_s16)src;
		uae_u16 rem = (uae_s32)dst % (uae_s32)(uae_s16)src;
		int cycles = getDivs68kCycles((uae_s32)dst, (uae_s16)src) * 256;
		if (cycles > 0) do_cycles_ce (cycles);
		if ((newv & 0xffff8000) != 0 && (newv & 0xffff8000) != 0xffff8000) {
			SET_VFLG (1);
			SET_NFLG (1);
		} else {
			if (((uae_s16)rem < 0) != ((uae_s32)dst < 0)) rem = -rem;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_NFLG (((uae_s16)(newv)) < 0);
			newv = (newv & 0xffff) | ((uae_u32)rem << 16);
			m68k_dreg(regs, dstreg) = (newv);
		}
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	m68k_incpc (4);
	}
}}}}}endlabel4830: ;
}
/* DIVS.W (d8,PC,Xn),Dn */
void REGPARAM2 CPUFUNC(op_81fb_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{	uaecptr oldpc = m68k_getpc();
{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4831;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	CLEAR_CZNV;
	if (src == 0) {
		m68k_incpc (4);
		Exception (5, oldpc);
		goto endlabel4831;
	} else {
		uae_s32 newv = (uae_s32)dst / (uae_s32)(uae_s16)src;
		uae_u16 rem = (uae_s32)dst % (uae_s32)(uae_s16)src;
		int cycles = getDivs68kCycles((uae_s32)dst, (uae_s16)src) * 256;
		if (cycles > 0) do_cycles_ce (cycles);
		if ((newv & 0xffff8000) != 0 && (newv & 0xffff8000) != 0xffff8000) {
			SET_VFLG (1);
			SET_NFLG (1);
		} else {
			if (((uae_s16)rem < 0) != ((uae_s32)dst < 0)) rem = -rem;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_NFLG (((uae_s16)(newv)) < 0);
			newv = (newv & 0xffff) | ((uae_u32)rem << 16);
			m68k_dreg(regs, dstreg) = (newv);
		}
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	m68k_incpc (4);
	}
}}}}}endlabel4831: ;
}
/* DIVS.W #<data>.W,Dn */
void REGPARAM2 CPUFUNC(op_81fc_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{	uaecptr oldpc = m68k_getpc();
{	uae_s16 src = get_word_ce_prefetch (4);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	CLEAR_CZNV;
	if (src == 0) {
		m68k_incpc (4);
		Exception (5, oldpc);
		goto endlabel4832;
	} else {
		uae_s32 newv = (uae_s32)dst / (uae_s32)(uae_s16)src;
		uae_u16 rem = (uae_s32)dst % (uae_s32)(uae_s16)src;
		int cycles = getDivs68kCycles((uae_s32)dst, (uae_s16)src) * 256;
		if (cycles > 0) do_cycles_ce (cycles);
		if ((newv & 0xffff8000) != 0 && (newv & 0xffff8000) != 0xffff8000) {
			SET_VFLG (1);
			SET_NFLG (1);
		} else {
			if (((uae_s16)rem < 0) != ((uae_s32)dst < 0)) rem = -rem;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_NFLG (((uae_s16)(newv)) < 0);
			newv = (newv & 0xffff) | ((uae_u32)rem << 16);
			m68k_dreg(regs, dstreg) = (newv);
		}
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	m68k_incpc (4);
	}
}}}endlabel4832: ;
}
/* SUB.B Dn,Dn */
void REGPARAM2 CPUFUNC(op_9000_6)(uae_u32 opcode)
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
}
/* SUB.B (An),Dn */
void REGPARAM2 CPUFUNC(op_9010_6)(uae_u32 opcode)
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
}
/* SUB.B (An)+,Dn */
void REGPARAM2 CPUFUNC(op_9018_6)(uae_u32 opcode)
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
}
/* SUB.B -(An),Dn */
void REGPARAM2 CPUFUNC(op_9020_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
	do_cycles_ce (2 * 256);
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
}
/* SUB.B (d16,An),Dn */
void REGPARAM2 CPUFUNC(op_9028_6)(uae_u32 opcode)
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
}
/* SUB.B (d8,An,Xn),Dn */
void REGPARAM2 CPUFUNC(op_9030_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
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
}
/* SUB.B (xxx).W,Dn */
void REGPARAM2 CPUFUNC(op_9038_6)(uae_u32 opcode)
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
}
/* SUB.B (xxx).L,Dn */
void REGPARAM2 CPUFUNC(op_9039_6)(uae_u32 opcode)
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
}
/* SUB.B (d16,PC),Dn */
void REGPARAM2 CPUFUNC(op_903a_6)(uae_u32 opcode)
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
}
/* SUB.B (d8,PC,Xn),Dn */
void REGPARAM2 CPUFUNC(op_903b_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
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
}
/* SUB.B #<data>.B,Dn */
void REGPARAM2 CPUFUNC(op_903c_6)(uae_u32 opcode)
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
}
/* SUB.W Dn,Dn */
void REGPARAM2 CPUFUNC(op_9040_6)(uae_u32 opcode)
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
}
/* SUB.W An,Dn */
void REGPARAM2 CPUFUNC(op_9048_6)(uae_u32 opcode)
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
}
/* SUB.W (An),Dn */
void REGPARAM2 CPUFUNC(op_9050_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4846;
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
endlabel4846: ;
}
/* SUB.W (An)+,Dn */
void REGPARAM2 CPUFUNC(op_9058_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4847;
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
endlabel4847: ;
}
/* SUB.W -(An),Dn */
void REGPARAM2 CPUFUNC(op_9060_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg(regs, srcreg) - 2;
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4848;
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
endlabel4848: ;
}
/* SUB.W (d16,An),Dn */
void REGPARAM2 CPUFUNC(op_9068_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4849;
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
endlabel4849: ;
}
/* SUB.W (d8,An,Xn),Dn */
void REGPARAM2 CPUFUNC(op_9070_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4850;
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
endlabel4850: ;
}
/* SUB.W (xxx).W,Dn */
void REGPARAM2 CPUFUNC(op_9078_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4851;
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
endlabel4851: ;
}
/* SUB.W (xxx).L,Dn */
void REGPARAM2 CPUFUNC(op_9079_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4852;
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
endlabel4852: ;
}
/* SUB.W (d16,PC),Dn */
void REGPARAM2 CPUFUNC(op_907a_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4853;
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
endlabel4853: ;
}
/* SUB.W (d8,PC,Xn),Dn */
void REGPARAM2 CPUFUNC(op_907b_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4854;
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
endlabel4854: ;
}
/* SUB.W #<data>.W,Dn */
void REGPARAM2 CPUFUNC(op_907c_6)(uae_u32 opcode)
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
}
/* SUB.L Dn,Dn */
void REGPARAM2 CPUFUNC(op_9080_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	do_cycles_ce (2 * 256);
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
	m68k_dreg(regs, dstreg) = (newv);
}}}}}}	m68k_incpc (2);
}
/* SUB.L An,Dn */
void REGPARAM2 CPUFUNC(op_9088_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_areg(regs, srcreg);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	do_cycles_ce (2 * 256);
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
	m68k_dreg(regs, dstreg) = (newv);
}}}}}}	m68k_incpc (2);
}
/* SUB.L (An),Dn */
void REGPARAM2 CPUFUNC(op_9090_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4858;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	do_cycles_ce (2 * 256);
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
	m68k_dreg(regs, dstreg) = (newv);
}}}}}}}}	m68k_incpc (2);
endlabel4858: ;
}
/* SUB.L (An)+,Dn */
void REGPARAM2 CPUFUNC(op_9098_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4859;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	m68k_areg(regs, srcreg) += 4;
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	do_cycles_ce (2 * 256);
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
	m68k_dreg(regs, dstreg) = (newv);
}}}}}}}}	m68k_incpc (2);
endlabel4859: ;
}
/* SUB.L -(An),Dn */
void REGPARAM2 CPUFUNC(op_90a0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg(regs, srcreg) - 4;
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4860;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	m68k_areg (regs, srcreg) = srca;
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	do_cycles_ce (2 * 256);
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
	m68k_dreg(regs, dstreg) = (newv);
}}}}}}}}	m68k_incpc (2);
endlabel4860: ;
}
/* SUB.L (d16,An),Dn */
void REGPARAM2 CPUFUNC(op_90a8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4861;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	do_cycles_ce (2 * 256);
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
	m68k_dreg(regs, dstreg) = (newv);
}}}}}}}}	m68k_incpc (4);
endlabel4861: ;
}
/* SUB.L (d8,An,Xn),Dn */
void REGPARAM2 CPUFUNC(op_90b0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4862;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	do_cycles_ce (2 * 256);
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
	m68k_dreg(regs, dstreg) = (newv);
}}}}}}}}	m68k_incpc (4);
endlabel4862: ;
}
/* SUB.L (xxx).W,Dn */
void REGPARAM2 CPUFUNC(op_90b8_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4863;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	do_cycles_ce (2 * 256);
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
	m68k_dreg(regs, dstreg) = (newv);
}}}}}}}}	m68k_incpc (4);
endlabel4863: ;
}
/* SUB.L (xxx).L,Dn */
void REGPARAM2 CPUFUNC(op_90b9_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4864;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	do_cycles_ce (2 * 256);
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
	m68k_dreg(regs, dstreg) = (newv);
}}}}}}}}	m68k_incpc (6);
endlabel4864: ;
}
/* SUB.L (d16,PC),Dn */
void REGPARAM2 CPUFUNC(op_90ba_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4865;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	do_cycles_ce (2 * 256);
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
	m68k_dreg(regs, dstreg) = (newv);
}}}}}}}}	m68k_incpc (4);
endlabel4865: ;
}
/* SUB.L (d8,PC,Xn),Dn */
void REGPARAM2 CPUFUNC(op_90bb_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4866;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	do_cycles_ce (2 * 256);
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
	m68k_dreg(regs, dstreg) = (newv);
}}}}}}}}	m68k_incpc (4);
endlabel4866: ;
}
/* SUB.L #<data>.L,Dn */
void REGPARAM2 CPUFUNC(op_90bc_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	do_cycles_ce (4 * 256);
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
	m68k_dreg(regs, dstreg) = (newv);
}}}}}}	m68k_incpc (6);
}
/* SUBA.W Dn,An */
void REGPARAM2 CPUFUNC(op_90c0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	do_cycles_ce (4 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 newv = dst - src;
	m68k_areg(regs, dstreg) = (newv);
}}}}	m68k_incpc (2);
}
/* SUBA.W An,An */
void REGPARAM2 CPUFUNC(op_90c8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_areg(regs, srcreg);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	do_cycles_ce (4 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 newv = dst - src;
	m68k_areg(regs, dstreg) = (newv);
}}}}	m68k_incpc (2);
}
/* SUBA.W (An),An */
void REGPARAM2 CPUFUNC(op_90d0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4870;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	do_cycles_ce (4 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 newv = dst - src;
	m68k_areg(regs, dstreg) = (newv);
}}}}}}	m68k_incpc (2);
endlabel4870: ;
}
/* SUBA.W (An)+,An */
void REGPARAM2 CPUFUNC(op_90d8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4871;
	}
{{	uae_s16 src = get_word_ce (srca);
	m68k_areg(regs, srcreg) += 2;
{	uae_s32 dst = m68k_areg(regs, dstreg);
	do_cycles_ce (4 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 newv = dst - src;
	m68k_areg(regs, dstreg) = (newv);
}}}}}}	m68k_incpc (2);
endlabel4871: ;
}
/* SUBA.W -(An),An */
void REGPARAM2 CPUFUNC(op_90e0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg(regs, srcreg) - 2;
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4872;
	}
{{	uae_s16 src = get_word_ce (srca);
	m68k_areg (regs, srcreg) = srca;
{	uae_s32 dst = m68k_areg(regs, dstreg);
	do_cycles_ce (4 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 newv = dst - src;
	m68k_areg(regs, dstreg) = (newv);
}}}}}}	m68k_incpc (2);
endlabel4872: ;
}
/* SUBA.W (d16,An),An */
void REGPARAM2 CPUFUNC(op_90e8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4873;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	do_cycles_ce (4 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	uae_u32 newv = dst - src;
	m68k_areg(regs, dstreg) = (newv);
}}}}}}	m68k_incpc (4);
endlabel4873: ;
}
/* SUBA.W (d8,An,Xn),An */
void REGPARAM2 CPUFUNC(op_90f0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4874;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	do_cycles_ce (4 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	uae_u32 newv = dst - src;
	m68k_areg(regs, dstreg) = (newv);
}}}}}}	m68k_incpc (4);
endlabel4874: ;
}
/* SUBA.W (xxx).W,An */
void REGPARAM2 CPUFUNC(op_90f8_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4875;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	do_cycles_ce (4 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	uae_u32 newv = dst - src;
	m68k_areg(regs, dstreg) = (newv);
}}}}}}	m68k_incpc (4);
endlabel4875: ;
}
/* SUBA.W (xxx).L,An */
void REGPARAM2 CPUFUNC(op_90f9_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4876;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	do_cycles_ce (4 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
{	uae_u32 newv = dst - src;
	m68k_areg(regs, dstreg) = (newv);
}}}}}}	m68k_incpc (6);
endlabel4876: ;
}
/* SUBA.W (d16,PC),An */
void REGPARAM2 CPUFUNC(op_90fa_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4877;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	do_cycles_ce (4 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	uae_u32 newv = dst - src;
	m68k_areg(regs, dstreg) = (newv);
}}}}}}	m68k_incpc (4);
endlabel4877: ;
}
/* SUBA.W (d8,PC,Xn),An */
void REGPARAM2 CPUFUNC(op_90fb_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4878;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	do_cycles_ce (4 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	uae_u32 newv = dst - src;
	m68k_areg(regs, dstreg) = (newv);
}}}}}}	m68k_incpc (4);
endlabel4878: ;
}
/* SUBA.W #<data>.W,An */
void REGPARAM2 CPUFUNC(op_90fc_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	do_cycles_ce (4 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	uae_u32 newv = dst - src;
	m68k_areg(regs, dstreg) = (newv);
}}}}	m68k_incpc (4);
}
/* SUBX.B Dn,Dn */
void REGPARAM2 CPUFUNC(op_9100_6)(uae_u32 opcode)
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
}
/* SUBX.B -(An),-(An) */
void REGPARAM2 CPUFUNC(op_9108_6)(uae_u32 opcode)
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
	do_cycles_ce (2 * 256);
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
	put_byte_ce (dsta,newv);
}}}}}}}	m68k_incpc (2);
}
/* SUB.B Dn,(An) */
void REGPARAM2 CPUFUNC(op_9110_6)(uae_u32 opcode)
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
}
/* SUB.B Dn,(An)+ */
void REGPARAM2 CPUFUNC(op_9118_6)(uae_u32 opcode)
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
}
/* SUB.B Dn,-(An) */
void REGPARAM2 CPUFUNC(op_9120_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - areg_byteinc[dstreg];
	do_cycles_ce (2 * 256);
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
}
/* SUB.B Dn,(d16,An) */
void REGPARAM2 CPUFUNC(op_9128_6)(uae_u32 opcode)
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
}
/* SUB.B Dn,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_9130_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
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
}
/* SUB.B Dn,(xxx).W */
void REGPARAM2 CPUFUNC(op_9138_6)(uae_u32 opcode)
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
}
/* SUB.B Dn,(xxx).L */
void REGPARAM2 CPUFUNC(op_9139_6)(uae_u32 opcode)
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
}
/* SUBX.W Dn,Dn */
void REGPARAM2 CPUFUNC(op_9140_6)(uae_u32 opcode)
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
}
/* SUBX.W -(An),-(An) */
void REGPARAM2 CPUFUNC(op_9148_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg(regs, srcreg) - 2;
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4890;
	}
{{	uae_s16 src = get_word_ce (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - 2;
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4890;
	}
{{	uae_s16 dst = get_word_ce (dsta);
	m68k_areg (regs, dstreg) = dsta;
	do_cycles_ce (2 * 256);
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
	put_word_ce (dsta,newv);
}}}}}}}}}	m68k_incpc (2);
endlabel4890: ;
}
/* SUB.W Dn,(An) */
void REGPARAM2 CPUFUNC(op_9150_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4891;
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
endlabel4891: ;
}
/* SUB.W Dn,(An)+ */
void REGPARAM2 CPUFUNC(op_9158_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4892;
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
endlabel4892: ;
}
/* SUB.W Dn,-(An) */
void REGPARAM2 CPUFUNC(op_9160_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - 2;
	do_cycles_ce (2 * 256);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4893;
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
endlabel4893: ;
}
/* SUB.W Dn,(d16,An) */
void REGPARAM2 CPUFUNC(op_9168_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4894;
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
endlabel4894: ;
}
/* SUB.W Dn,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_9170_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4895;
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
endlabel4895: ;
}
/* SUB.W Dn,(xxx).W */
void REGPARAM2 CPUFUNC(op_9178_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4896;
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
endlabel4896: ;
}
/* SUB.W Dn,(xxx).L */
void REGPARAM2 CPUFUNC(op_9179_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (4) << 16;
	dsta |= get_word_ce_prefetch (6);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4897;
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
endlabel4897: ;
}
/* SUBX.L Dn,Dn */
void REGPARAM2 CPUFUNC(op_9180_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	do_cycles_ce (2 * 256);
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
	m68k_dreg(regs, dstreg) = (newv);
}}}}}	m68k_incpc (2);
}
/* SUBX.L -(An),-(An) */
void REGPARAM2 CPUFUNC(op_9188_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg(regs, srcreg) - 4;
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4899;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - 4;
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4899;
	}
{{	uae_s32 dst = get_word_ce (dsta) << 16; dst |= get_word_ce (dsta + 2);
	m68k_areg (regs, dstreg) = dsta;
	do_cycles_ce (2 * 256);
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
	put_word_ce (dsta, newv >> 16); put_word_ce (dsta + 2, newv);
}}}}}}}}}	m68k_incpc (2);
endlabel4899: ;
}
/* SUB.L Dn,(An) */
void REGPARAM2 CPUFUNC(op_9190_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4900;
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
endlabel4900: ;
}
/* SUB.L Dn,(An)+ */
void REGPARAM2 CPUFUNC(op_9198_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4901;
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
endlabel4901: ;
}
/* SUB.L Dn,-(An) */
void REGPARAM2 CPUFUNC(op_91a0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - 4;
	do_cycles_ce (2 * 256);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4902;
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
endlabel4902: ;
}
/* SUB.L Dn,(d16,An) */
void REGPARAM2 CPUFUNC(op_91a8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4903;
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
endlabel4903: ;
}
/* SUB.L Dn,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_91b0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4904;
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
endlabel4904: ;
}
/* SUB.L Dn,(xxx).W */
void REGPARAM2 CPUFUNC(op_91b8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4905;
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
endlabel4905: ;
}
/* SUB.L Dn,(xxx).L */
void REGPARAM2 CPUFUNC(op_91b9_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (4) << 16;
	dsta |= get_word_ce_prefetch (6);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4906;
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
endlabel4906: ;
}
/* SUBA.L Dn,An */
void REGPARAM2 CPUFUNC(op_91c0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	do_cycles_ce (4 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 newv = dst - src;
	m68k_areg(regs, dstreg) = (newv);
}}}}	m68k_incpc (2);
}
/* SUBA.L An,An */
void REGPARAM2 CPUFUNC(op_91c8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_areg(regs, srcreg);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	do_cycles_ce (4 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 newv = dst - src;
	m68k_areg(regs, dstreg) = (newv);
}}}}	m68k_incpc (2);
}
/* SUBA.L (An),An */
void REGPARAM2 CPUFUNC(op_91d0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4909;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	do_cycles_ce (4 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 newv = dst - src;
	m68k_areg(regs, dstreg) = (newv);
}}}}}}	m68k_incpc (2);
endlabel4909: ;
}
/* SUBA.L (An)+,An */
void REGPARAM2 CPUFUNC(op_91d8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4910;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	m68k_areg(regs, srcreg) += 4;
{	uae_s32 dst = m68k_areg(regs, dstreg);
	do_cycles_ce (4 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 newv = dst - src;
	m68k_areg(regs, dstreg) = (newv);
}}}}}}	m68k_incpc (2);
endlabel4910: ;
}
/* SUBA.L -(An),An */
void REGPARAM2 CPUFUNC(op_91e0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg(regs, srcreg) - 4;
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4911;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	m68k_areg (regs, srcreg) = srca;
{	uae_s32 dst = m68k_areg(regs, dstreg);
	do_cycles_ce (4 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 newv = dst - src;
	m68k_areg(regs, dstreg) = (newv);
}}}}}}	m68k_incpc (2);
endlabel4911: ;
}
/* SUBA.L (d16,An),An */
void REGPARAM2 CPUFUNC(op_91e8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4912;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	do_cycles_ce (4 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	uae_u32 newv = dst - src;
	m68k_areg(regs, dstreg) = (newv);
}}}}}}	m68k_incpc (4);
endlabel4912: ;
}
/* SUBA.L (d8,An,Xn),An */
void REGPARAM2 CPUFUNC(op_91f0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4913;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	do_cycles_ce (4 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	uae_u32 newv = dst - src;
	m68k_areg(regs, dstreg) = (newv);
}}}}}}	m68k_incpc (4);
endlabel4913: ;
}
/* SUBA.L (xxx).W,An */
void REGPARAM2 CPUFUNC(op_91f8_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4914;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	do_cycles_ce (4 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	uae_u32 newv = dst - src;
	m68k_areg(regs, dstreg) = (newv);
}}}}}}	m68k_incpc (4);
endlabel4914: ;
}
/* SUBA.L (xxx).L,An */
void REGPARAM2 CPUFUNC(op_91f9_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4915;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	do_cycles_ce (4 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
{	uae_u32 newv = dst - src;
	m68k_areg(regs, dstreg) = (newv);
}}}}}}	m68k_incpc (6);
endlabel4915: ;
}
/* SUBA.L (d16,PC),An */
void REGPARAM2 CPUFUNC(op_91fa_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4916;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	do_cycles_ce (4 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	uae_u32 newv = dst - src;
	m68k_areg(regs, dstreg) = (newv);
}}}}}}	m68k_incpc (4);
endlabel4916: ;
}
/* SUBA.L (d8,PC,Xn),An */
void REGPARAM2 CPUFUNC(op_91fb_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4917;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	do_cycles_ce (4 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	uae_u32 newv = dst - src;
	m68k_areg(regs, dstreg) = (newv);
}}}}}}	m68k_incpc (4);
endlabel4917: ;
}
/* SUBA.L #<data>.L,An */
void REGPARAM2 CPUFUNC(op_91fc_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	do_cycles_ce (4 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
{	uae_u32 newv = dst - src;
	m68k_areg(regs, dstreg) = (newv);
}}}}	m68k_incpc (6);
}
/* CMP.B Dn,Dn */
void REGPARAM2 CPUFUNC(op_b000_6)(uae_u32 opcode)
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
}
/* CMP.B (An),Dn */
void REGPARAM2 CPUFUNC(op_b010_6)(uae_u32 opcode)
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
}
/* CMP.B (An)+,Dn */
void REGPARAM2 CPUFUNC(op_b018_6)(uae_u32 opcode)
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
}
/* CMP.B -(An),Dn */
void REGPARAM2 CPUFUNC(op_b020_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
	do_cycles_ce (2 * 256);
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
}
/* CMP.B (d16,An),Dn */
void REGPARAM2 CPUFUNC(op_b028_6)(uae_u32 opcode)
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
}
/* CMP.B (d8,An,Xn),Dn */
void REGPARAM2 CPUFUNC(op_b030_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
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
}
/* CMP.B (xxx).W,Dn */
void REGPARAM2 CPUFUNC(op_b038_6)(uae_u32 opcode)
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
}
/* CMP.B (xxx).L,Dn */
void REGPARAM2 CPUFUNC(op_b039_6)(uae_u32 opcode)
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
}
/* CMP.B (d16,PC),Dn */
void REGPARAM2 CPUFUNC(op_b03a_6)(uae_u32 opcode)
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
}
/* CMP.B (d8,PC,Xn),Dn */
void REGPARAM2 CPUFUNC(op_b03b_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
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
}
/* CMP.B #<data>.B,Dn */
void REGPARAM2 CPUFUNC(op_b03c_6)(uae_u32 opcode)
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
}
/* CMP.W Dn,Dn */
void REGPARAM2 CPUFUNC(op_b040_6)(uae_u32 opcode)
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
}
#endif

#ifdef PART_7
/* CMP.W An,Dn */
void REGPARAM2 CPUFUNC(op_b048_6)(uae_u32 opcode)
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
}
/* CMP.W (An),Dn */
void REGPARAM2 CPUFUNC(op_b050_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4932;
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
endlabel4932: ;
}
/* CMP.W (An)+,Dn */
void REGPARAM2 CPUFUNC(op_b058_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4933;
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
endlabel4933: ;
}
/* CMP.W -(An),Dn */
void REGPARAM2 CPUFUNC(op_b060_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg(regs, srcreg) - 2;
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4934;
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
endlabel4934: ;
}
/* CMP.W (d16,An),Dn */
void REGPARAM2 CPUFUNC(op_b068_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4935;
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
endlabel4935: ;
}
/* CMP.W (d8,An,Xn),Dn */
void REGPARAM2 CPUFUNC(op_b070_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4936;
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
endlabel4936: ;
}
/* CMP.W (xxx).W,Dn */
void REGPARAM2 CPUFUNC(op_b078_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4937;
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
endlabel4937: ;
}
/* CMP.W (xxx).L,Dn */
void REGPARAM2 CPUFUNC(op_b079_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4938;
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
endlabel4938: ;
}
/* CMP.W (d16,PC),Dn */
void REGPARAM2 CPUFUNC(op_b07a_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4939;
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
endlabel4939: ;
}
/* CMP.W (d8,PC,Xn),Dn */
void REGPARAM2 CPUFUNC(op_b07b_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4940;
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
endlabel4940: ;
}
/* CMP.W #<data>.W,Dn */
void REGPARAM2 CPUFUNC(op_b07c_6)(uae_u32 opcode)
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
}
/* CMP.L Dn,Dn */
void REGPARAM2 CPUFUNC(op_b080_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	do_cycles_ce (2 * 256);
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
}}}}}}	m68k_incpc (2);
}
/* CMP.L An,Dn */
void REGPARAM2 CPUFUNC(op_b088_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_areg(regs, srcreg);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	do_cycles_ce (2 * 256);
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
}}}}}}	m68k_incpc (2);
}
/* CMP.L (An),Dn */
void REGPARAM2 CPUFUNC(op_b090_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4944;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	do_cycles_ce (2 * 256);
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
}}}}}}}}	m68k_incpc (2);
endlabel4944: ;
}
/* CMP.L (An)+,Dn */
void REGPARAM2 CPUFUNC(op_b098_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4945;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	m68k_areg(regs, srcreg) += 4;
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	do_cycles_ce (2 * 256);
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
}}}}}}}}	m68k_incpc (2);
endlabel4945: ;
}
/* CMP.L -(An),Dn */
void REGPARAM2 CPUFUNC(op_b0a0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg(regs, srcreg) - 4;
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4946;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	m68k_areg (regs, srcreg) = srca;
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	do_cycles_ce (2 * 256);
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
}}}}}}}}	m68k_incpc (2);
endlabel4946: ;
}
/* CMP.L (d16,An),Dn */
void REGPARAM2 CPUFUNC(op_b0a8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4947;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	do_cycles_ce (2 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (4);
endlabel4947: ;
}
/* CMP.L (d8,An,Xn),Dn */
void REGPARAM2 CPUFUNC(op_b0b0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4948;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	do_cycles_ce (2 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (4);
endlabel4948: ;
}
/* CMP.L (xxx).W,Dn */
void REGPARAM2 CPUFUNC(op_b0b8_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4949;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	do_cycles_ce (2 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (4);
endlabel4949: ;
}
/* CMP.L (xxx).L,Dn */
void REGPARAM2 CPUFUNC(op_b0b9_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4950;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	do_cycles_ce (2 * 256);
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
endlabel4950: ;
}
/* CMP.L (d16,PC),Dn */
void REGPARAM2 CPUFUNC(op_b0ba_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4951;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	do_cycles_ce (2 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (4);
endlabel4951: ;
}
/* CMP.L (d8,PC,Xn),Dn */
void REGPARAM2 CPUFUNC(op_b0bb_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4952;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	do_cycles_ce (2 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (4);
endlabel4952: ;
}
/* CMP.L #<data>.L,Dn */
void REGPARAM2 CPUFUNC(op_b0bc_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	do_cycles_ce (2 * 256);
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
}}}}}}	m68k_incpc (6);
}
/* CMPA.W Dn,An */
void REGPARAM2 CPUFUNC(op_b0c0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	do_cycles_ce (2 * 256);
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
}}}}}}	m68k_incpc (2);
}
/* CMPA.W An,An */
void REGPARAM2 CPUFUNC(op_b0c8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_areg(regs, srcreg);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	do_cycles_ce (2 * 256);
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
}}}}}}	m68k_incpc (2);
}
/* CMPA.W (An),An */
void REGPARAM2 CPUFUNC(op_b0d0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4956;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	do_cycles_ce (2 * 256);
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
}}}}}}}}	m68k_incpc (2);
endlabel4956: ;
}
/* CMPA.W (An)+,An */
void REGPARAM2 CPUFUNC(op_b0d8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4957;
	}
{{	uae_s16 src = get_word_ce (srca);
	m68k_areg(regs, srcreg) += 2;
{	uae_s32 dst = m68k_areg(regs, dstreg);
	do_cycles_ce (2 * 256);
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
}}}}}}}}	m68k_incpc (2);
endlabel4957: ;
}
/* CMPA.W -(An),An */
void REGPARAM2 CPUFUNC(op_b0e0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg(regs, srcreg) - 2;
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4958;
	}
{{	uae_s16 src = get_word_ce (srca);
	m68k_areg (regs, srcreg) = srca;
{	uae_s32 dst = m68k_areg(regs, dstreg);
	do_cycles_ce (2 * 256);
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
}}}}}}}}	m68k_incpc (2);
endlabel4958: ;
}
/* CMPA.W (d16,An),An */
void REGPARAM2 CPUFUNC(op_b0e8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4959;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	do_cycles_ce (2 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (4);
endlabel4959: ;
}
/* CMPA.W (d8,An,Xn),An */
void REGPARAM2 CPUFUNC(op_b0f0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4960;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	do_cycles_ce (2 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (4);
endlabel4960: ;
}
/* CMPA.W (xxx).W,An */
void REGPARAM2 CPUFUNC(op_b0f8_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4961;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	do_cycles_ce (2 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (4);
endlabel4961: ;
}
/* CMPA.W (xxx).L,An */
void REGPARAM2 CPUFUNC(op_b0f9_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4962;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	do_cycles_ce (2 * 256);
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
endlabel4962: ;
}
/* CMPA.W (d16,PC),An */
void REGPARAM2 CPUFUNC(op_b0fa_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4963;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	do_cycles_ce (2 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (4);
endlabel4963: ;
}
/* CMPA.W (d8,PC,Xn),An */
void REGPARAM2 CPUFUNC(op_b0fb_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4964;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	do_cycles_ce (2 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (4);
endlabel4964: ;
}
/* CMPA.W #<data>.W,An */
void REGPARAM2 CPUFUNC(op_b0fc_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	do_cycles_ce (2 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}	m68k_incpc (4);
}
/* EOR.B Dn,Dn */
void REGPARAM2 CPUFUNC(op_b100_6)(uae_u32 opcode)
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
}
/* CMPM.B (An)+,(An)+ */
void REGPARAM2 CPUFUNC(op_b108_6)(uae_u32 opcode)
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
}
/* EOR.B Dn,(An) */
void REGPARAM2 CPUFUNC(op_b110_6)(uae_u32 opcode)
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
}
/* EOR.B Dn,(An)+ */
void REGPARAM2 CPUFUNC(op_b118_6)(uae_u32 opcode)
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
}
/* EOR.B Dn,-(An) */
void REGPARAM2 CPUFUNC(op_b120_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - areg_byteinc[dstreg];
	do_cycles_ce (2 * 256);
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
}
/* EOR.B Dn,(d16,An) */
void REGPARAM2 CPUFUNC(op_b128_6)(uae_u32 opcode)
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
}
/* EOR.B Dn,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_b130_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
{	uae_s8 dst = get_byte_ce (dsta);
	src ^= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	put_byte_ce (dsta,src);
}}}}	m68k_incpc (4);
}
/* EOR.B Dn,(xxx).W */
void REGPARAM2 CPUFUNC(op_b138_6)(uae_u32 opcode)
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
}
/* EOR.B Dn,(xxx).L */
void REGPARAM2 CPUFUNC(op_b139_6)(uae_u32 opcode)
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
}
/* EOR.W Dn,Dn */
void REGPARAM2 CPUFUNC(op_b140_6)(uae_u32 opcode)
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
}
/* CMPM.W (An)+,(An)+ */
void REGPARAM2 CPUFUNC(op_b148_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4976;
	}
{{	uae_s16 src = get_word_ce (srca);
	m68k_areg(regs, srcreg) += 2;
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4976;
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
endlabel4976: ;
}
/* EOR.W Dn,(An) */
void REGPARAM2 CPUFUNC(op_b150_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4977;
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
endlabel4977: ;
}
/* EOR.W Dn,(An)+ */
void REGPARAM2 CPUFUNC(op_b158_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4978;
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
endlabel4978: ;
}
/* EOR.W Dn,-(An) */
void REGPARAM2 CPUFUNC(op_b160_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - 2;
	do_cycles_ce (2 * 256);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4979;
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
endlabel4979: ;
}
/* EOR.W Dn,(d16,An) */
void REGPARAM2 CPUFUNC(op_b168_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4980;
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
endlabel4980: ;
}
/* EOR.W Dn,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_b170_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4981;
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
endlabel4981: ;
}
/* EOR.W Dn,(xxx).W */
void REGPARAM2 CPUFUNC(op_b178_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4982;
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
endlabel4982: ;
}
/* EOR.W Dn,(xxx).L */
void REGPARAM2 CPUFUNC(op_b179_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (4) << 16;
	dsta |= get_word_ce_prefetch (6);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4983;
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
endlabel4983: ;
}
/* EOR.L Dn,Dn */
void REGPARAM2 CPUFUNC(op_b180_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	src ^= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	do_cycles_ce (4 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	m68k_dreg(regs, dstreg) = (src);
}}}	m68k_incpc (2);
}
/* CMPM.L (An)+,(An)+ */
void REGPARAM2 CPUFUNC(op_b188_6)(uae_u32 opcode)
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
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4985;
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
endlabel4985: ;
}
/* EOR.L Dn,(An) */
void REGPARAM2 CPUFUNC(op_b190_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4986;
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
endlabel4986: ;
}
/* EOR.L Dn,(An)+ */
void REGPARAM2 CPUFUNC(op_b198_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4987;
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
endlabel4987: ;
}
/* EOR.L Dn,-(An) */
void REGPARAM2 CPUFUNC(op_b1a0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - 4;
	do_cycles_ce (2 * 256);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4988;
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
endlabel4988: ;
}
/* EOR.L Dn,(d16,An) */
void REGPARAM2 CPUFUNC(op_b1a8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4989;
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
endlabel4989: ;
}
/* EOR.L Dn,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_b1b0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4990;
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
endlabel4990: ;
}
/* EOR.L Dn,(xxx).W */
void REGPARAM2 CPUFUNC(op_b1b8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4991;
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
endlabel4991: ;
}
/* EOR.L Dn,(xxx).L */
void REGPARAM2 CPUFUNC(op_b1b9_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (4) << 16;
	dsta |= get_word_ce_prefetch (6);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel4992;
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
endlabel4992: ;
}
/* CMPA.L Dn,An */
void REGPARAM2 CPUFUNC(op_b1c0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	do_cycles_ce (2 * 256);
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
}}}}}}	m68k_incpc (2);
}
/* CMPA.L An,An */
void REGPARAM2 CPUFUNC(op_b1c8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_areg(regs, srcreg);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	do_cycles_ce (2 * 256);
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
}}}}}}	m68k_incpc (2);
}
/* CMPA.L (An),An */
void REGPARAM2 CPUFUNC(op_b1d0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4995;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	do_cycles_ce (2 * 256);
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
}}}}}}}}	m68k_incpc (2);
endlabel4995: ;
}
/* CMPA.L (An)+,An */
void REGPARAM2 CPUFUNC(op_b1d8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4996;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	m68k_areg(regs, srcreg) += 4;
{	uae_s32 dst = m68k_areg(regs, dstreg);
	do_cycles_ce (2 * 256);
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
}}}}}}}}	m68k_incpc (2);
endlabel4996: ;
}
/* CMPA.L -(An),An */
void REGPARAM2 CPUFUNC(op_b1e0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg(regs, srcreg) - 4;
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4997;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	m68k_areg (regs, srcreg) = srca;
{	uae_s32 dst = m68k_areg(regs, dstreg);
	do_cycles_ce (2 * 256);
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
}}}}}}}}	m68k_incpc (2);
endlabel4997: ;
}
/* CMPA.L (d16,An),An */
void REGPARAM2 CPUFUNC(op_b1e8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4998;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	do_cycles_ce (2 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (4);
endlabel4998: ;
}
/* CMPA.L (d8,An,Xn),An */
void REGPARAM2 CPUFUNC(op_b1f0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel4999;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	do_cycles_ce (2 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
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
}
/* CMPA.L (xxx).W,An */
void REGPARAM2 CPUFUNC(op_b1f8_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5000;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	do_cycles_ce (2 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
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
}
/* CMPA.L (xxx).L,An */
void REGPARAM2 CPUFUNC(op_b1f9_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5001;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	do_cycles_ce (2 * 256);
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
endlabel5001: ;
}
/* CMPA.L (d16,PC),An */
void REGPARAM2 CPUFUNC(op_b1fa_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5002;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	do_cycles_ce (2 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (4);
endlabel5002: ;
}
/* CMPA.L (d8,PC,Xn),An */
void REGPARAM2 CPUFUNC(op_b1fb_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5003;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	do_cycles_ce (2 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
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
}
/* CMPA.L #<data>.L,An */
void REGPARAM2 CPUFUNC(op_b1fc_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	do_cycles_ce (2 * 256);
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
}}}}}}	m68k_incpc (6);
}
/* AND.B Dn,Dn */
void REGPARAM2 CPUFUNC(op_c000_6)(uae_u32 opcode)
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
}
/* AND.B (An),Dn */
void REGPARAM2 CPUFUNC(op_c010_6)(uae_u32 opcode)
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
}
/* AND.B (An)+,Dn */
void REGPARAM2 CPUFUNC(op_c018_6)(uae_u32 opcode)
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
}
/* AND.B -(An),Dn */
void REGPARAM2 CPUFUNC(op_c020_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
	do_cycles_ce (2 * 256);
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
}
/* AND.B (d16,An),Dn */
void REGPARAM2 CPUFUNC(op_c028_6)(uae_u32 opcode)
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
}
/* AND.B (d8,An,Xn),Dn */
void REGPARAM2 CPUFUNC(op_c030_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
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
}
/* AND.B (xxx).W,Dn */
void REGPARAM2 CPUFUNC(op_c038_6)(uae_u32 opcode)
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
}
/* AND.B (xxx).L,Dn */
void REGPARAM2 CPUFUNC(op_c039_6)(uae_u32 opcode)
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
}
/* AND.B (d16,PC),Dn */
void REGPARAM2 CPUFUNC(op_c03a_6)(uae_u32 opcode)
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
}
/* AND.B (d8,PC,Xn),Dn */
void REGPARAM2 CPUFUNC(op_c03b_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
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
}
/* AND.B #<data>.B,Dn */
void REGPARAM2 CPUFUNC(op_c03c_6)(uae_u32 opcode)
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
}
/* AND.W Dn,Dn */
void REGPARAM2 CPUFUNC(op_c040_6)(uae_u32 opcode)
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
}
/* AND.W (An),Dn */
void REGPARAM2 CPUFUNC(op_c050_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5017;
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
endlabel5017: ;
}
/* AND.W (An)+,Dn */
void REGPARAM2 CPUFUNC(op_c058_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5018;
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
endlabel5018: ;
}
/* AND.W -(An),Dn */
void REGPARAM2 CPUFUNC(op_c060_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg(regs, srcreg) - 2;
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5019;
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
endlabel5019: ;
}
/* AND.W (d16,An),Dn */
void REGPARAM2 CPUFUNC(op_c068_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5020;
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
endlabel5020: ;
}
/* AND.W (d8,An,Xn),Dn */
void REGPARAM2 CPUFUNC(op_c070_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5021;
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
endlabel5021: ;
}
/* AND.W (xxx).W,Dn */
void REGPARAM2 CPUFUNC(op_c078_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5022;
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
endlabel5022: ;
}
/* AND.W (xxx).L,Dn */
void REGPARAM2 CPUFUNC(op_c079_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5023;
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
endlabel5023: ;
}
/* AND.W (d16,PC),Dn */
void REGPARAM2 CPUFUNC(op_c07a_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5024;
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
endlabel5024: ;
}
/* AND.W (d8,PC,Xn),Dn */
void REGPARAM2 CPUFUNC(op_c07b_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5025;
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
endlabel5025: ;
}
/* AND.W #<data>.W,Dn */
void REGPARAM2 CPUFUNC(op_c07c_6)(uae_u32 opcode)
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
}
/* AND.L Dn,Dn */
void REGPARAM2 CPUFUNC(op_c080_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	src &= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	do_cycles_ce (2 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	m68k_dreg(regs, dstreg) = (src);
}}}	m68k_incpc (2);
}
/* AND.L (An),Dn */
void REGPARAM2 CPUFUNC(op_c090_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5028;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	src &= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	do_cycles_ce (2 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	m68k_dreg(regs, dstreg) = (src);
}}}}}	m68k_incpc (2);
endlabel5028: ;
}
/* AND.L (An)+,Dn */
void REGPARAM2 CPUFUNC(op_c098_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5029;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	m68k_areg(regs, srcreg) += 4;
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	src &= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	do_cycles_ce (2 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	m68k_dreg(regs, dstreg) = (src);
}}}}}	m68k_incpc (2);
endlabel5029: ;
}
/* AND.L -(An),Dn */
void REGPARAM2 CPUFUNC(op_c0a0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg(regs, srcreg) - 4;
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5030;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	m68k_areg (regs, srcreg) = srca;
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	src &= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	do_cycles_ce (2 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	m68k_dreg(regs, dstreg) = (src);
}}}}}	m68k_incpc (2);
endlabel5030: ;
}
/* AND.L (d16,An),Dn */
void REGPARAM2 CPUFUNC(op_c0a8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5031;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	src &= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	do_cycles_ce (2 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	m68k_dreg(regs, dstreg) = (src);
}}}}}	m68k_incpc (4);
endlabel5031: ;
}
/* AND.L (d8,An,Xn),Dn */
void REGPARAM2 CPUFUNC(op_c0b0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5032;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	src &= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	do_cycles_ce (2 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	m68k_dreg(regs, dstreg) = (src);
}}}}}	m68k_incpc (4);
endlabel5032: ;
}
/* AND.L (xxx).W,Dn */
void REGPARAM2 CPUFUNC(op_c0b8_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5033;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	src &= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	do_cycles_ce (2 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	m68k_dreg(regs, dstreg) = (src);
}}}}}	m68k_incpc (4);
endlabel5033: ;
}
/* AND.L (xxx).L,Dn */
void REGPARAM2 CPUFUNC(op_c0b9_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5034;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	src &= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	do_cycles_ce (2 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	m68k_dreg(regs, dstreg) = (src);
}}}}}	m68k_incpc (6);
endlabel5034: ;
}
/* AND.L (d16,PC),Dn */
void REGPARAM2 CPUFUNC(op_c0ba_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5035;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	src &= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	do_cycles_ce (2 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	m68k_dreg(regs, dstreg) = (src);
}}}}}	m68k_incpc (4);
endlabel5035: ;
}
/* AND.L (d8,PC,Xn),Dn */
void REGPARAM2 CPUFUNC(op_c0bb_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5036;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	src &= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(src)) == 0);
	SET_NFLG (((uae_s32)(src)) < 0);
	do_cycles_ce (2 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	m68k_dreg(regs, dstreg) = (src);
}}}}}	m68k_incpc (4);
endlabel5036: ;
}
/* AND.L #<data>.L,Dn */
void REGPARAM2 CPUFUNC(op_c0bc_6)(uae_u32 opcode)
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
	do_cycles_ce (2 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
	m68k_dreg(regs, dstreg) = (src);
}}}	m68k_incpc (6);
}
/* MULU.W Dn,Dn */
void REGPARAM2 CPUFUNC(op_c0c0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 newv = (uae_u32)(uae_u16)dst * (uae_u32)(uae_u16)src;
	int cycles = 36 * 256, bits;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_NFLG (((uae_s32)(newv)) < 0);
	for(bits = 0; bits < 16 && src; bits++, src >>= 1)
		if (src & 1) cycles += 2 * 256;
	if (cycles > 0) do_cycles_ce (cycles);
	m68k_dreg(regs, dstreg) = (newv);
	m68k_incpc (2);
}}}}}
/* MULU.W (An),Dn */
void REGPARAM2 CPUFUNC(op_c0d0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5039;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 newv = (uae_u32)(uae_u16)dst * (uae_u32)(uae_u16)src;
	int cycles = 36 * 256, bits;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_NFLG (((uae_s32)(newv)) < 0);
	for(bits = 0; bits < 16 && src; bits++, src >>= 1)
		if (src & 1) cycles += 2 * 256;
	if (cycles > 0) do_cycles_ce (cycles);
	m68k_dreg(regs, dstreg) = (newv);
	m68k_incpc (2);
}}}}}}endlabel5039: ;
}
/* MULU.W (An)+,Dn */
void REGPARAM2 CPUFUNC(op_c0d8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5040;
	}
{{	uae_s16 src = get_word_ce (srca);
	m68k_areg(regs, srcreg) += 2;
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 newv = (uae_u32)(uae_u16)dst * (uae_u32)(uae_u16)src;
	int cycles = 36 * 256, bits;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_NFLG (((uae_s32)(newv)) < 0);
	for(bits = 0; bits < 16 && src; bits++, src >>= 1)
		if (src & 1) cycles += 2 * 256;
	if (cycles > 0) do_cycles_ce (cycles);
	m68k_dreg(regs, dstreg) = (newv);
	m68k_incpc (2);
}}}}}}endlabel5040: ;
}
/* MULU.W -(An),Dn */
void REGPARAM2 CPUFUNC(op_c0e0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg(regs, srcreg) - 2;
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5041;
	}
{{	uae_s16 src = get_word_ce (srca);
	m68k_areg (regs, srcreg) = srca;
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 newv = (uae_u32)(uae_u16)dst * (uae_u32)(uae_u16)src;
	int cycles = 36 * 256, bits;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_NFLG (((uae_s32)(newv)) < 0);
	for(bits = 0; bits < 16 && src; bits++, src >>= 1)
		if (src & 1) cycles += 2 * 256;
	if (cycles > 0) do_cycles_ce (cycles);
	m68k_dreg(regs, dstreg) = (newv);
	m68k_incpc (2);
}}}}}}endlabel5041: ;
}
/* MULU.W (d16,An),Dn */
void REGPARAM2 CPUFUNC(op_c0e8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5042;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	uae_u32 newv = (uae_u32)(uae_u16)dst * (uae_u32)(uae_u16)src;
	int cycles = 36 * 256, bits;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_NFLG (((uae_s32)(newv)) < 0);
	for(bits = 0; bits < 16 && src; bits++, src >>= 1)
		if (src & 1) cycles += 2 * 256;
	if (cycles > 0) do_cycles_ce (cycles);
	m68k_dreg(regs, dstreg) = (newv);
	m68k_incpc (4);
}}}}}}endlabel5042: ;
}
/* MULU.W (d8,An,Xn),Dn */
void REGPARAM2 CPUFUNC(op_c0f0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5043;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	uae_u32 newv = (uae_u32)(uae_u16)dst * (uae_u32)(uae_u16)src;
	int cycles = 36 * 256, bits;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_NFLG (((uae_s32)(newv)) < 0);
	for(bits = 0; bits < 16 && src; bits++, src >>= 1)
		if (src & 1) cycles += 2 * 256;
	if (cycles > 0) do_cycles_ce (cycles);
	m68k_dreg(regs, dstreg) = (newv);
	m68k_incpc (4);
}}}}}}endlabel5043: ;
}
/* MULU.W (xxx).W,Dn */
void REGPARAM2 CPUFUNC(op_c0f8_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5044;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	uae_u32 newv = (uae_u32)(uae_u16)dst * (uae_u32)(uae_u16)src;
	int cycles = 36 * 256, bits;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_NFLG (((uae_s32)(newv)) < 0);
	for(bits = 0; bits < 16 && src; bits++, src >>= 1)
		if (src & 1) cycles += 2 * 256;
	if (cycles > 0) do_cycles_ce (cycles);
	m68k_dreg(regs, dstreg) = (newv);
	m68k_incpc (4);
}}}}}}endlabel5044: ;
}
/* MULU.W (xxx).L,Dn */
void REGPARAM2 CPUFUNC(op_c0f9_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5045;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
{	uae_u32 newv = (uae_u32)(uae_u16)dst * (uae_u32)(uae_u16)src;
	int cycles = 36 * 256, bits;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_NFLG (((uae_s32)(newv)) < 0);
	for(bits = 0; bits < 16 && src; bits++, src >>= 1)
		if (src & 1) cycles += 2 * 256;
	if (cycles > 0) do_cycles_ce (cycles);
	m68k_dreg(regs, dstreg) = (newv);
	m68k_incpc (6);
}}}}}}endlabel5045: ;
}
/* MULU.W (d16,PC),Dn */
void REGPARAM2 CPUFUNC(op_c0fa_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5046;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	uae_u32 newv = (uae_u32)(uae_u16)dst * (uae_u32)(uae_u16)src;
	int cycles = 36 * 256, bits;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_NFLG (((uae_s32)(newv)) < 0);
	for(bits = 0; bits < 16 && src; bits++, src >>= 1)
		if (src & 1) cycles += 2 * 256;
	if (cycles > 0) do_cycles_ce (cycles);
	m68k_dreg(regs, dstreg) = (newv);
	m68k_incpc (4);
}}}}}}endlabel5046: ;
}
/* MULU.W (d8,PC,Xn),Dn */
void REGPARAM2 CPUFUNC(op_c0fb_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5047;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	uae_u32 newv = (uae_u32)(uae_u16)dst * (uae_u32)(uae_u16)src;
	int cycles = 36 * 256, bits;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_NFLG (((uae_s32)(newv)) < 0);
	for(bits = 0; bits < 16 && src; bits++, src >>= 1)
		if (src & 1) cycles += 2 * 256;
	if (cycles > 0) do_cycles_ce (cycles);
	m68k_dreg(regs, dstreg) = (newv);
	m68k_incpc (4);
}}}}}}endlabel5047: ;
}
/* MULU.W #<data>.W,Dn */
void REGPARAM2 CPUFUNC(op_c0fc_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	uae_u32 newv = (uae_u32)(uae_u16)dst * (uae_u32)(uae_u16)src;
	int cycles = 36 * 256, bits;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_NFLG (((uae_s32)(newv)) < 0);
	for(bits = 0; bits < 16 && src; bits++, src >>= 1)
		if (src & 1) cycles += 2 * 256;
	if (cycles > 0) do_cycles_ce (cycles);
	m68k_dreg(regs, dstreg) = (newv);
	m68k_incpc (4);
}}}}}
/* ABCD.B Dn,Dn */
void REGPARAM2 CPUFUNC(op_c100_6)(uae_u32 opcode)
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
	do_cycles_ce (2 * 256);
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xff) | ((newv) & 0xff);
}}}}	m68k_incpc (2);
}
/* ABCD.B -(An),-(An) */
void REGPARAM2 CPUFUNC(op_c108_6)(uae_u32 opcode)
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
	do_cycles_ce (2 * 256);
	put_byte_ce (dsta,newv);
}}}}}}	m68k_incpc (2);
}
/* AND.B Dn,(An) */
void REGPARAM2 CPUFUNC(op_c110_6)(uae_u32 opcode)
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
}
/* AND.B Dn,(An)+ */
void REGPARAM2 CPUFUNC(op_c118_6)(uae_u32 opcode)
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
}
/* AND.B Dn,-(An) */
void REGPARAM2 CPUFUNC(op_c120_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - areg_byteinc[dstreg];
	do_cycles_ce (2 * 256);
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
}
/* AND.B Dn,(d16,An) */
void REGPARAM2 CPUFUNC(op_c128_6)(uae_u32 opcode)
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
}
/* AND.B Dn,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_c130_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
{	uae_s8 dst = get_byte_ce (dsta);
	src &= dst;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s8)(src)) == 0);
	SET_NFLG (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
	put_byte_ce (dsta,src);
}}}}	m68k_incpc (4);
}
/* AND.B Dn,(xxx).W */
void REGPARAM2 CPUFUNC(op_c138_6)(uae_u32 opcode)
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
}
/* AND.B Dn,(xxx).L */
void REGPARAM2 CPUFUNC(op_c139_6)(uae_u32 opcode)
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
}
/* EXG.L Dn,Dn */
void REGPARAM2 CPUFUNC(op_c140_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	do_cycles_ce (2 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	m68k_dreg(regs, srcreg) = (dst);
	m68k_dreg(regs, dstreg) = (src);
}}}	m68k_incpc (2);
}
/* EXG.L An,An */
void REGPARAM2 CPUFUNC(op_c148_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_areg(regs, srcreg);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	do_cycles_ce (2 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	m68k_areg(regs, srcreg) = (dst);
	m68k_areg(regs, dstreg) = (src);
}}}	m68k_incpc (2);
}
/* AND.W Dn,(An) */
void REGPARAM2 CPUFUNC(op_c150_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel5060;
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
endlabel5060: ;
}
/* AND.W Dn,(An)+ */
void REGPARAM2 CPUFUNC(op_c158_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel5061;
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
endlabel5061: ;
}
/* AND.W Dn,-(An) */
void REGPARAM2 CPUFUNC(op_c160_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - 2;
	do_cycles_ce (2 * 256);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel5062;
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
endlabel5062: ;
}
/* AND.W Dn,(d16,An) */
void REGPARAM2 CPUFUNC(op_c168_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel5063;
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
endlabel5063: ;
}
/* AND.W Dn,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_c170_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel5064;
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
endlabel5064: ;
}
/* AND.W Dn,(xxx).W */
void REGPARAM2 CPUFUNC(op_c178_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel5065;
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
endlabel5065: ;
}
/* AND.W Dn,(xxx).L */
void REGPARAM2 CPUFUNC(op_c179_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (4) << 16;
	dsta |= get_word_ce_prefetch (6);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel5066;
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
endlabel5066: ;
}
/* EXG.L Dn,An */
void REGPARAM2 CPUFUNC(op_c188_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	do_cycles_ce (2 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
	m68k_dreg(regs, srcreg) = (dst);
	m68k_areg(regs, dstreg) = (src);
}}}	m68k_incpc (2);
}
/* AND.L Dn,(An) */
void REGPARAM2 CPUFUNC(op_c190_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel5068;
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
endlabel5068: ;
}
/* AND.L Dn,(An)+ */
void REGPARAM2 CPUFUNC(op_c198_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel5069;
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
endlabel5069: ;
}
/* AND.L Dn,-(An) */
void REGPARAM2 CPUFUNC(op_c1a0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - 4;
	do_cycles_ce (2 * 256);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel5070;
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
endlabel5070: ;
}
/* AND.L Dn,(d16,An) */
void REGPARAM2 CPUFUNC(op_c1a8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel5071;
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
endlabel5071: ;
}
/* AND.L Dn,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_c1b0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel5072;
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
endlabel5072: ;
}
/* AND.L Dn,(xxx).W */
void REGPARAM2 CPUFUNC(op_c1b8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel5073;
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
endlabel5073: ;
}
/* AND.L Dn,(xxx).L */
void REGPARAM2 CPUFUNC(op_c1b9_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (4) << 16;
	dsta |= get_word_ce_prefetch (6);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel5074;
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
endlabel5074: ;
}
/* MULS.W Dn,Dn */
void REGPARAM2 CPUFUNC(op_c1c0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 newv = (uae_s32)(uae_s16)dst * (uae_s32)(uae_s16)src;
	int cycles = 36 * 256, bits;
	uae_u32 usrc;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_NFLG (((uae_s32)(newv)) < 0);
	usrc = ((uae_u32)src) << 1;
	for(bits = 0; bits < 16 && usrc; bits++, usrc >>= 1)
		if ((usrc & 3) == 1 || (usrc & 3) == 2) cycles += 2 * 256;
	if (cycles > 0) do_cycles_ce (cycles);
	m68k_dreg(regs, dstreg) = (newv);
}}}}	m68k_incpc (2);
}
/* MULS.W (An),Dn */
void REGPARAM2 CPUFUNC(op_c1d0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5076;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 newv = (uae_s32)(uae_s16)dst * (uae_s32)(uae_s16)src;
	int cycles = 36 * 256, bits;
	uae_u32 usrc;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_NFLG (((uae_s32)(newv)) < 0);
	usrc = ((uae_u32)src) << 1;
	for(bits = 0; bits < 16 && usrc; bits++, usrc >>= 1)
		if ((usrc & 3) == 1 || (usrc & 3) == 2) cycles += 2 * 256;
	if (cycles > 0) do_cycles_ce (cycles);
	m68k_dreg(regs, dstreg) = (newv);
}}}}}}	m68k_incpc (2);
endlabel5076: ;
}
/* MULS.W (An)+,Dn */
void REGPARAM2 CPUFUNC(op_c1d8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5077;
	}
{{	uae_s16 src = get_word_ce (srca);
	m68k_areg(regs, srcreg) += 2;
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 newv = (uae_s32)(uae_s16)dst * (uae_s32)(uae_s16)src;
	int cycles = 36 * 256, bits;
	uae_u32 usrc;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_NFLG (((uae_s32)(newv)) < 0);
	usrc = ((uae_u32)src) << 1;
	for(bits = 0; bits < 16 && usrc; bits++, usrc >>= 1)
		if ((usrc & 3) == 1 || (usrc & 3) == 2) cycles += 2 * 256;
	if (cycles > 0) do_cycles_ce (cycles);
	m68k_dreg(regs, dstreg) = (newv);
}}}}}}	m68k_incpc (2);
endlabel5077: ;
}
/* MULS.W -(An),Dn */
void REGPARAM2 CPUFUNC(op_c1e0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg(regs, srcreg) - 2;
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5078;
	}
{{	uae_s16 src = get_word_ce (srca);
	m68k_areg (regs, srcreg) = srca;
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 newv = (uae_s32)(uae_s16)dst * (uae_s32)(uae_s16)src;
	int cycles = 36 * 256, bits;
	uae_u32 usrc;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_NFLG (((uae_s32)(newv)) < 0);
	usrc = ((uae_u32)src) << 1;
	for(bits = 0; bits < 16 && usrc; bits++, usrc >>= 1)
		if ((usrc & 3) == 1 || (usrc & 3) == 2) cycles += 2 * 256;
	if (cycles > 0) do_cycles_ce (cycles);
	m68k_dreg(regs, dstreg) = (newv);
}}}}}}	m68k_incpc (2);
endlabel5078: ;
}
/* MULS.W (d16,An),Dn */
void REGPARAM2 CPUFUNC(op_c1e8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5079;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	uae_u32 newv = (uae_s32)(uae_s16)dst * (uae_s32)(uae_s16)src;
	int cycles = 36 * 256, bits;
	uae_u32 usrc;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_NFLG (((uae_s32)(newv)) < 0);
	usrc = ((uae_u32)src) << 1;
	for(bits = 0; bits < 16 && usrc; bits++, usrc >>= 1)
		if ((usrc & 3) == 1 || (usrc & 3) == 2) cycles += 2 * 256;
	if (cycles > 0) do_cycles_ce (cycles);
	m68k_dreg(regs, dstreg) = (newv);
}}}}}}	m68k_incpc (4);
endlabel5079: ;
}
/* MULS.W (d8,An,Xn),Dn */
void REGPARAM2 CPUFUNC(op_c1f0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5080;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	uae_u32 newv = (uae_s32)(uae_s16)dst * (uae_s32)(uae_s16)src;
	int cycles = 36 * 256, bits;
	uae_u32 usrc;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_NFLG (((uae_s32)(newv)) < 0);
	usrc = ((uae_u32)src) << 1;
	for(bits = 0; bits < 16 && usrc; bits++, usrc >>= 1)
		if ((usrc & 3) == 1 || (usrc & 3) == 2) cycles += 2 * 256;
	if (cycles > 0) do_cycles_ce (cycles);
	m68k_dreg(regs, dstreg) = (newv);
}}}}}}	m68k_incpc (4);
endlabel5080: ;
}
/* MULS.W (xxx).W,Dn */
void REGPARAM2 CPUFUNC(op_c1f8_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5081;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	uae_u32 newv = (uae_s32)(uae_s16)dst * (uae_s32)(uae_s16)src;
	int cycles = 36 * 256, bits;
	uae_u32 usrc;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_NFLG (((uae_s32)(newv)) < 0);
	usrc = ((uae_u32)src) << 1;
	for(bits = 0; bits < 16 && usrc; bits++, usrc >>= 1)
		if ((usrc & 3) == 1 || (usrc & 3) == 2) cycles += 2 * 256;
	if (cycles > 0) do_cycles_ce (cycles);
	m68k_dreg(regs, dstreg) = (newv);
}}}}}}	m68k_incpc (4);
endlabel5081: ;
}
/* MULS.W (xxx).L,Dn */
void REGPARAM2 CPUFUNC(op_c1f9_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5082;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
{	uae_u32 newv = (uae_s32)(uae_s16)dst * (uae_s32)(uae_s16)src;
	int cycles = 36 * 256, bits;
	uae_u32 usrc;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_NFLG (((uae_s32)(newv)) < 0);
	usrc = ((uae_u32)src) << 1;
	for(bits = 0; bits < 16 && usrc; bits++, usrc >>= 1)
		if ((usrc & 3) == 1 || (usrc & 3) == 2) cycles += 2 * 256;
	if (cycles > 0) do_cycles_ce (cycles);
	m68k_dreg(regs, dstreg) = (newv);
}}}}}}	m68k_incpc (6);
endlabel5082: ;
}
/* MULS.W (d16,PC),Dn */
void REGPARAM2 CPUFUNC(op_c1fa_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5083;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	uae_u32 newv = (uae_s32)(uae_s16)dst * (uae_s32)(uae_s16)src;
	int cycles = 36 * 256, bits;
	uae_u32 usrc;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_NFLG (((uae_s32)(newv)) < 0);
	usrc = ((uae_u32)src) << 1;
	for(bits = 0; bits < 16 && usrc; bits++, usrc >>= 1)
		if ((usrc & 3) == 1 || (usrc & 3) == 2) cycles += 2 * 256;
	if (cycles > 0) do_cycles_ce (cycles);
	m68k_dreg(regs, dstreg) = (newv);
}}}}}}	m68k_incpc (4);
endlabel5083: ;
}
/* MULS.W (d8,PC,Xn),Dn */
void REGPARAM2 CPUFUNC(op_c1fb_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5084;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	uae_u32 newv = (uae_s32)(uae_s16)dst * (uae_s32)(uae_s16)src;
	int cycles = 36 * 256, bits;
	uae_u32 usrc;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_NFLG (((uae_s32)(newv)) < 0);
	usrc = ((uae_u32)src) << 1;
	for(bits = 0; bits < 16 && usrc; bits++, usrc >>= 1)
		if ((usrc & 3) == 1 || (usrc & 3) == 2) cycles += 2 * 256;
	if (cycles > 0) do_cycles_ce (cycles);
	m68k_dreg(regs, dstreg) = (newv);
}}}}}}	m68k_incpc (4);
endlabel5084: ;
}
/* MULS.W #<data>.W,Dn */
void REGPARAM2 CPUFUNC(op_c1fc_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uae_s16 dst = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	uae_u32 newv = (uae_s32)(uae_s16)dst * (uae_s32)(uae_s16)src;
	int cycles = 36 * 256, bits;
	uae_u32 usrc;
	CLEAR_CZNV;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_NFLG (((uae_s32)(newv)) < 0);
	usrc = ((uae_u32)src) << 1;
	for(bits = 0; bits < 16 && usrc; bits++, usrc >>= 1)
		if ((usrc & 3) == 1 || (usrc & 3) == 2) cycles += 2 * 256;
	if (cycles > 0) do_cycles_ce (cycles);
	m68k_dreg(regs, dstreg) = (newv);
}}}}	m68k_incpc (4);
}
/* ADD.B Dn,Dn */
void REGPARAM2 CPUFUNC(op_d000_6)(uae_u32 opcode)
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
}
/* ADD.B (An),Dn */
void REGPARAM2 CPUFUNC(op_d010_6)(uae_u32 opcode)
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
}
/* ADD.B (An)+,Dn */
void REGPARAM2 CPUFUNC(op_d018_6)(uae_u32 opcode)
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
}
/* ADD.B -(An),Dn */
void REGPARAM2 CPUFUNC(op_d020_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
	do_cycles_ce (2 * 256);
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
}
/* ADD.B (d16,An),Dn */
void REGPARAM2 CPUFUNC(op_d028_6)(uae_u32 opcode)
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
}
/* ADD.B (d8,An,Xn),Dn */
void REGPARAM2 CPUFUNC(op_d030_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
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
}
/* ADD.B (xxx).W,Dn */
void REGPARAM2 CPUFUNC(op_d038_6)(uae_u32 opcode)
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
}
/* ADD.B (xxx).L,Dn */
void REGPARAM2 CPUFUNC(op_d039_6)(uae_u32 opcode)
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
}
/* ADD.B (d16,PC),Dn */
void REGPARAM2 CPUFUNC(op_d03a_6)(uae_u32 opcode)
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
}
/* ADD.B (d8,PC,Xn),Dn */
void REGPARAM2 CPUFUNC(op_d03b_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
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
}
/* ADD.B #<data>.B,Dn */
void REGPARAM2 CPUFUNC(op_d03c_6)(uae_u32 opcode)
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
}
/* ADD.W Dn,Dn */
void REGPARAM2 CPUFUNC(op_d040_6)(uae_u32 opcode)
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
}
/* ADD.W An,Dn */
void REGPARAM2 CPUFUNC(op_d048_6)(uae_u32 opcode)
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
}
/* ADD.W (An),Dn */
void REGPARAM2 CPUFUNC(op_d050_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5099;
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
endlabel5099: ;
}
/* ADD.W (An)+,Dn */
void REGPARAM2 CPUFUNC(op_d058_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5100;
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
endlabel5100: ;
}
/* ADD.W -(An),Dn */
void REGPARAM2 CPUFUNC(op_d060_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg(regs, srcreg) - 2;
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5101;
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
endlabel5101: ;
}
/* ADD.W (d16,An),Dn */
void REGPARAM2 CPUFUNC(op_d068_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5102;
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
endlabel5102: ;
}
/* ADD.W (d8,An,Xn),Dn */
void REGPARAM2 CPUFUNC(op_d070_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5103;
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
endlabel5103: ;
}
/* ADD.W (xxx).W,Dn */
void REGPARAM2 CPUFUNC(op_d078_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5104;
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
endlabel5104: ;
}
/* ADD.W (xxx).L,Dn */
void REGPARAM2 CPUFUNC(op_d079_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5105;
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
endlabel5105: ;
}
/* ADD.W (d16,PC),Dn */
void REGPARAM2 CPUFUNC(op_d07a_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5106;
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
endlabel5106: ;
}
/* ADD.W (d8,PC,Xn),Dn */
void REGPARAM2 CPUFUNC(op_d07b_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5107;
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
endlabel5107: ;
}
/* ADD.W #<data>.W,Dn */
void REGPARAM2 CPUFUNC(op_d07c_6)(uae_u32 opcode)
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
}
/* ADD.L Dn,Dn */
void REGPARAM2 CPUFUNC(op_d080_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	do_cycles_ce (2 * 256);
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
	m68k_dreg(regs, dstreg) = (newv);
}}}}}}	m68k_incpc (2);
}
/* ADD.L An,Dn */
void REGPARAM2 CPUFUNC(op_d088_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_areg(regs, srcreg);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	do_cycles_ce (2 * 256);
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
	m68k_dreg(regs, dstreg) = (newv);
}}}}}}	m68k_incpc (2);
}
/* ADD.L (An),Dn */
void REGPARAM2 CPUFUNC(op_d090_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5111;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	do_cycles_ce (2 * 256);
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
	m68k_dreg(regs, dstreg) = (newv);
}}}}}}}}	m68k_incpc (2);
endlabel5111: ;
}
/* ADD.L (An)+,Dn */
void REGPARAM2 CPUFUNC(op_d098_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5112;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	m68k_areg(regs, srcreg) += 4;
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	do_cycles_ce (2 * 256);
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
	m68k_dreg(regs, dstreg) = (newv);
}}}}}}}}	m68k_incpc (2);
endlabel5112: ;
}
/* ADD.L -(An),Dn */
void REGPARAM2 CPUFUNC(op_d0a0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg(regs, srcreg) - 4;
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5113;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	m68k_areg (regs, srcreg) = srca;
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	do_cycles_ce (2 * 256);
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
	m68k_dreg(regs, dstreg) = (newv);
}}}}}}}}	m68k_incpc (2);
endlabel5113: ;
}
/* ADD.L (d16,An),Dn */
void REGPARAM2 CPUFUNC(op_d0a8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5114;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	do_cycles_ce (2 * 256);
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
	m68k_dreg(regs, dstreg) = (newv);
}}}}}}}}	m68k_incpc (4);
endlabel5114: ;
}
/* ADD.L (d8,An,Xn),Dn */
void REGPARAM2 CPUFUNC(op_d0b0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5115;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	do_cycles_ce (2 * 256);
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
	m68k_dreg(regs, dstreg) = (newv);
}}}}}}}}	m68k_incpc (4);
endlabel5115: ;
}
/* ADD.L (xxx).W,Dn */
void REGPARAM2 CPUFUNC(op_d0b8_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5116;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	do_cycles_ce (2 * 256);
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
	m68k_dreg(regs, dstreg) = (newv);
}}}}}}}}	m68k_incpc (4);
endlabel5116: ;
}
/* ADD.L (xxx).L,Dn */
void REGPARAM2 CPUFUNC(op_d0b9_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5117;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	do_cycles_ce (2 * 256);
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
	m68k_dreg(regs, dstreg) = (newv);
}}}}}}}}	m68k_incpc (6);
endlabel5117: ;
}
/* ADD.L (d16,PC),Dn */
void REGPARAM2 CPUFUNC(op_d0ba_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5118;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	do_cycles_ce (2 * 256);
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
	m68k_dreg(regs, dstreg) = (newv);
}}}}}}}}	m68k_incpc (4);
endlabel5118: ;
}
/* ADD.L (d8,PC,Xn),Dn */
void REGPARAM2 CPUFUNC(op_d0bb_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5119;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	do_cycles_ce (2 * 256);
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
	m68k_dreg(regs, dstreg) = (newv);
}}}}}}}}	m68k_incpc (4);
endlabel5119: ;
}
/* ADD.L #<data>.L,Dn */
void REGPARAM2 CPUFUNC(op_d0bc_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	do_cycles_ce (4 * 256);
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
	m68k_dreg(regs, dstreg) = (newv);
}}}}}}	m68k_incpc (6);
}
/* ADDA.W Dn,An */
void REGPARAM2 CPUFUNC(op_d0c0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	do_cycles_ce (4 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 newv = dst + src;
	m68k_areg(regs, dstreg) = (newv);
}}}}	m68k_incpc (2);
}
/* ADDA.W An,An */
void REGPARAM2 CPUFUNC(op_d0c8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_areg(regs, srcreg);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	do_cycles_ce (4 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 newv = dst + src;
	m68k_areg(regs, dstreg) = (newv);
}}}}	m68k_incpc (2);
}
/* ADDA.W (An),An */
void REGPARAM2 CPUFUNC(op_d0d0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5123;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	do_cycles_ce (4 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 newv = dst + src;
	m68k_areg(regs, dstreg) = (newv);
}}}}}}	m68k_incpc (2);
endlabel5123: ;
}
/* ADDA.W (An)+,An */
void REGPARAM2 CPUFUNC(op_d0d8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5124;
	}
{{	uae_s16 src = get_word_ce (srca);
	m68k_areg(regs, srcreg) += 2;
{	uae_s32 dst = m68k_areg(regs, dstreg);
	do_cycles_ce (4 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 newv = dst + src;
	m68k_areg(regs, dstreg) = (newv);
}}}}}}	m68k_incpc (2);
endlabel5124: ;
}
/* ADDA.W -(An),An */
void REGPARAM2 CPUFUNC(op_d0e0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg(regs, srcreg) - 2;
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5125;
	}
{{	uae_s16 src = get_word_ce (srca);
	m68k_areg (regs, srcreg) = srca;
{	uae_s32 dst = m68k_areg(regs, dstreg);
	do_cycles_ce (4 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 newv = dst + src;
	m68k_areg(regs, dstreg) = (newv);
}}}}}}	m68k_incpc (2);
endlabel5125: ;
}
/* ADDA.W (d16,An),An */
void REGPARAM2 CPUFUNC(op_d0e8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5126;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	do_cycles_ce (4 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	uae_u32 newv = dst + src;
	m68k_areg(regs, dstreg) = (newv);
}}}}}}	m68k_incpc (4);
endlabel5126: ;
}
/* ADDA.W (d8,An,Xn),An */
void REGPARAM2 CPUFUNC(op_d0f0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5127;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	do_cycles_ce (4 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	uae_u32 newv = dst + src;
	m68k_areg(regs, dstreg) = (newv);
}}}}}}	m68k_incpc (4);
endlabel5127: ;
}
/* ADDA.W (xxx).W,An */
void REGPARAM2 CPUFUNC(op_d0f8_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5128;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	do_cycles_ce (4 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	uae_u32 newv = dst + src;
	m68k_areg(regs, dstreg) = (newv);
}}}}}}	m68k_incpc (4);
endlabel5128: ;
}
/* ADDA.W (xxx).L,An */
void REGPARAM2 CPUFUNC(op_d0f9_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5129;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	do_cycles_ce (4 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
{	uae_u32 newv = dst + src;
	m68k_areg(regs, dstreg) = (newv);
}}}}}}	m68k_incpc (6);
endlabel5129: ;
}
/* ADDA.W (d16,PC),An */
void REGPARAM2 CPUFUNC(op_d0fa_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5130;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	do_cycles_ce (4 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	uae_u32 newv = dst + src;
	m68k_areg(regs, dstreg) = (newv);
}}}}}}	m68k_incpc (4);
endlabel5130: ;
}
/* ADDA.W (d8,PC,Xn),An */
void REGPARAM2 CPUFUNC(op_d0fb_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5131;
	}
{{	uae_s16 src = get_word_ce (srca);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	do_cycles_ce (4 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	uae_u32 newv = dst + src;
	m68k_areg(regs, dstreg) = (newv);
}}}}}}	m68k_incpc (4);
endlabel5131: ;
}
/* ADDA.W #<data>.W,An */
void REGPARAM2 CPUFUNC(op_d0fc_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = get_word_ce_prefetch (4);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	do_cycles_ce (4 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	uae_u32 newv = dst + src;
	m68k_areg(regs, dstreg) = (newv);
}}}}	m68k_incpc (4);
}
/* ADDX.B Dn,Dn */
void REGPARAM2 CPUFUNC(op_d100_6)(uae_u32 opcode)
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
}
/* ADDX.B -(An),-(An) */
void REGPARAM2 CPUFUNC(op_d108_6)(uae_u32 opcode)
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
	do_cycles_ce (2 * 256);
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
	put_byte_ce (dsta,newv);
}}}}}}}	m68k_incpc (2);
}
/* ADD.B Dn,(An) */
void REGPARAM2 CPUFUNC(op_d110_6)(uae_u32 opcode)
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
}
/* ADD.B Dn,(An)+ */
void REGPARAM2 CPUFUNC(op_d118_6)(uae_u32 opcode)
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
}
/* ADD.B Dn,-(An) */
void REGPARAM2 CPUFUNC(op_d120_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - areg_byteinc[dstreg];
	do_cycles_ce (2 * 256);
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
}
/* ADD.B Dn,(d16,An) */
void REGPARAM2 CPUFUNC(op_d128_6)(uae_u32 opcode)
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
}
/* ADD.B Dn,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_d130_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
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
}
/* ADD.B Dn,(xxx).W */
void REGPARAM2 CPUFUNC(op_d138_6)(uae_u32 opcode)
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
}
/* ADD.B Dn,(xxx).L */
void REGPARAM2 CPUFUNC(op_d139_6)(uae_u32 opcode)
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
}
/* ADDX.W Dn,Dn */
void REGPARAM2 CPUFUNC(op_d140_6)(uae_u32 opcode)
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
}
/* ADDX.W -(An),-(An) */
void REGPARAM2 CPUFUNC(op_d148_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg(regs, srcreg) - 2;
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5143;
	}
{{	uae_s16 src = get_word_ce (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - 2;
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel5143;
	}
{{	uae_s16 dst = get_word_ce (dsta);
	m68k_areg (regs, dstreg) = dsta;
	do_cycles_ce (2 * 256);
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
	put_word_ce (dsta,newv);
}}}}}}}}}	m68k_incpc (2);
endlabel5143: ;
}
/* ADD.W Dn,(An) */
void REGPARAM2 CPUFUNC(op_d150_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel5144;
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
endlabel5144: ;
}
/* ADD.W Dn,(An)+ */
void REGPARAM2 CPUFUNC(op_d158_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel5145;
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
endlabel5145: ;
}
/* ADD.W Dn,-(An) */
void REGPARAM2 CPUFUNC(op_d160_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - 2;
	do_cycles_ce (2 * 256);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel5146;
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
endlabel5146: ;
}
/* ADD.W Dn,(d16,An) */
void REGPARAM2 CPUFUNC(op_d168_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel5147;
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
endlabel5147: ;
}
/* ADD.W Dn,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_d170_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel5148;
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
endlabel5148: ;
}
/* ADD.W Dn,(xxx).W */
void REGPARAM2 CPUFUNC(op_d178_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel5149;
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
endlabel5149: ;
}
/* ADD.W Dn,(xxx).L */
void REGPARAM2 CPUFUNC(op_d179_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s16 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (4) << 16;
	dsta |= get_word_ce_prefetch (6);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel5150;
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
endlabel5150: ;
}
/* ADDX.L Dn,Dn */
void REGPARAM2 CPUFUNC(op_d180_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uae_s32 dst = m68k_dreg(regs, dstreg);
	do_cycles_ce (2 * 256);
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
	m68k_dreg(regs, dstreg) = (newv);
}}}}}	m68k_incpc (2);
}
/* ADDX.L -(An),-(An) */
void REGPARAM2 CPUFUNC(op_d188_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg(regs, srcreg) - 4;
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5152;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - 4;
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel5152;
	}
{{	uae_s32 dst = get_word_ce (dsta) << 16; dst |= get_word_ce (dsta + 2);
	m68k_areg (regs, dstreg) = dsta;
	do_cycles_ce (2 * 256);
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
	put_word_ce (dsta, newv >> 16); put_word_ce (dsta + 2, newv);
}}}}}}}}}	m68k_incpc (2);
endlabel5152: ;
}
/* ADD.L Dn,(An) */
void REGPARAM2 CPUFUNC(op_d190_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel5153;
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
endlabel5153: ;
}
/* ADD.L Dn,(An)+ */
void REGPARAM2 CPUFUNC(op_d198_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = m68k_areg(regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel5154;
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
endlabel5154: ;
}
/* ADD.L Dn,-(An) */
void REGPARAM2 CPUFUNC(op_d1a0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg(regs, dstreg) - 4;
	do_cycles_ce (2 * 256);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel5155;
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
endlabel5155: ;
}
/* ADD.L Dn,(d16,An) */
void REGPARAM2 CPUFUNC(op_d1a8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel5156;
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
endlabel5156: ;
}
/* ADD.L Dn,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_d1b0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel5157;
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
endlabel5157: ;
}
/* ADD.L Dn,(xxx).W */
void REGPARAM2 CPUFUNC(op_d1b8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel5158;
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
endlabel5158: ;
}
/* ADD.L Dn,(xxx).L */
void REGPARAM2 CPUFUNC(op_d1b9_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uaecptr dsta;
	dsta = get_word_ce_prefetch (4) << 16;
	dsta |= get_word_ce_prefetch (6);
	if (dsta & 1) {
		exception3 (opcode, m68k_getpc() + 2, dsta);
		goto endlabel5159;
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
endlabel5159: ;
}
/* ADDA.L Dn,An */
void REGPARAM2 CPUFUNC(op_d1c0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_dreg(regs, srcreg);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	do_cycles_ce (4 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 newv = dst + src;
	m68k_areg(regs, dstreg) = (newv);
}}}}	m68k_incpc (2);
}
/* ADDA.L An,An */
void REGPARAM2 CPUFUNC(op_d1c8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_areg(regs, srcreg);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	do_cycles_ce (4 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 newv = dst + src;
	m68k_areg(regs, dstreg) = (newv);
}}}}	m68k_incpc (2);
}
/* ADDA.L (An),An */
void REGPARAM2 CPUFUNC(op_d1d0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5162;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	do_cycles_ce (2 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 newv = dst + src;
	m68k_areg(regs, dstreg) = (newv);
}}}}}}	m68k_incpc (2);
endlabel5162: ;
}
/* ADDA.L (An)+,An */
void REGPARAM2 CPUFUNC(op_d1d8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5163;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	m68k_areg(regs, srcreg) += 4;
{	uae_s32 dst = m68k_areg(regs, dstreg);
	do_cycles_ce (2 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 newv = dst + src;
	m68k_areg(regs, dstreg) = (newv);
}}}}}}	m68k_incpc (2);
endlabel5163: ;
}
#endif

#ifdef PART_8
/* ADDA.L -(An),An */
void REGPARAM2 CPUFUNC(op_d1e0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg(regs, srcreg) - 4;
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5164;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
	m68k_areg (regs, srcreg) = srca;
{	uae_s32 dst = m68k_areg(regs, dstreg);
	do_cycles_ce (2 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 newv = dst + src;
	m68k_areg(regs, dstreg) = (newv);
}}}}}}	m68k_incpc (2);
endlabel5164: ;
}
/* ADDA.L (d16,An),An */
void REGPARAM2 CPUFUNC(op_d1e8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5165;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	do_cycles_ce (2 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	uae_u32 newv = dst + src;
	m68k_areg(regs, dstreg) = (newv);
}}}}}}	m68k_incpc (4);
endlabel5165: ;
}
/* ADDA.L (d8,An,Xn),An */
void REGPARAM2 CPUFUNC(op_d1f0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5166;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	do_cycles_ce (2 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	uae_u32 newv = dst + src;
	m68k_areg(regs, dstreg) = (newv);
}}}}}}	m68k_incpc (4);
endlabel5166: ;
}
/* ADDA.L (xxx).W,An */
void REGPARAM2 CPUFUNC(op_d1f8_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5167;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	do_cycles_ce (2 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	uae_u32 newv = dst + src;
	m68k_areg(regs, dstreg) = (newv);
}}}}}}	m68k_incpc (4);
endlabel5167: ;
}
/* ADDA.L (xxx).L,An */
void REGPARAM2 CPUFUNC(op_d1f9_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_word_ce_prefetch (4) << 16;
	srca |= get_word_ce_prefetch (6);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5168;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	do_cycles_ce (2 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
{	uae_u32 newv = dst + src;
	m68k_areg(regs, dstreg) = (newv);
}}}}}}	m68k_incpc (6);
endlabel5168: ;
}
/* ADDA.L (d16,PC),An */
void REGPARAM2 CPUFUNC(op_d1fa_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5169;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	do_cycles_ce (2 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	uae_u32 newv = dst + src;
	m68k_areg(regs, dstreg) = (newv);
}}}}}}	m68k_incpc (4);
endlabel5169: ;
}
/* ADDA.L (d8,PC,Xn),An */
void REGPARAM2 CPUFUNC(op_d1fb_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc() + 2;
	srca = get_disp_ea_000(tmppc, get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (srca & 1) {
		exception3 (opcode, m68k_getpc() + 2, srca);
		goto endlabel5170;
	}
{{	uae_s32 src = get_word_ce (srca) << 16; src |= get_word_ce (srca + 2);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	do_cycles_ce (2 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (6);
{	uae_u32 newv = dst + src;
	m68k_areg(regs, dstreg) = (newv);
}}}}}}	m68k_incpc (4);
endlabel5170: ;
}
/* ADDA.L #<data>.L,An */
void REGPARAM2 CPUFUNC(op_d1fc_6)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src;
	src = get_word_ce_prefetch (4) << 16;
	src |= get_word_ce_prefetch (6);
{	uae_s32 dst = m68k_areg(regs, dstreg);
	do_cycles_ce (4 * 256);
	regs.ir = regs.irc;
	get_word_ce_prefetch (8);
{	uae_u32 newv = dst + src;
	m68k_areg(regs, dstreg) = (newv);
}}}}	m68k_incpc (6);
}
/* ASR.B #<data>,Dn */
void REGPARAM2 CPUFUNC(op_e000_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 cnt = srcreg;
{	uae_s8 data = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 val = (uae_u8)data;
	uae_u32 sign = (0x80 & val) >> 7;
	int ccnt = cnt & 63;
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
		int cycles = 6 * 256;
		cycles += 2 * 256 * ccnt;
		if (cycles > 0) do_cycles_ce (cycles);
	}
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xff) | ((val) & 0xff);
}}}}	m68k_incpc (2);
}
/* LSR.B #<data>,Dn */
void REGPARAM2 CPUFUNC(op_e008_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 cnt = srcreg;
{	uae_s8 data = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 val = (uae_u8)data;
	int ccnt = cnt & 63;
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
		int cycles = 6 * 256;
		cycles += 2 * 256 * ccnt;
		if (cycles > 0) do_cycles_ce (cycles);
	}
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xff) | ((val) & 0xff);
}}}}	m68k_incpc (2);
}
/* ROXR.B #<data>,Dn */
void REGPARAM2 CPUFUNC(op_e010_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 cnt = srcreg;
{	uae_s8 data = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 val = (uae_u8)data;
	int ccnt = cnt & 63;
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
		int cycles = 6 * 256;
		cycles += 2 * 256 * ccnt;
		if (cycles > 0) do_cycles_ce (cycles);
	}
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xff) | ((val) & 0xff);
}}}}	m68k_incpc (2);
}
/* ROR.B #<data>,Dn */
void REGPARAM2 CPUFUNC(op_e018_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 cnt = srcreg;
{	uae_s8 data = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 val = (uae_u8)data;
	int ccnt = cnt & 63;
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
		int cycles = 6 * 256;
		cycles += 2 * 256 * ccnt;
		if (cycles > 0) do_cycles_ce (cycles);
	}
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xff) | ((val) & 0xff);
}}}}	m68k_incpc (2);
}
/* ASR.B Dn,Dn */
void REGPARAM2 CPUFUNC(op_e020_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 cnt = m68k_dreg(regs, srcreg);
{	uae_s8 data = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 val = (uae_u8)data;
	uae_u32 sign = (0x80 & val) >> 7;
	int ccnt = cnt & 63;
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
		int cycles = 6 * 256;
		cycles += 2 * 256 * ccnt;
		if (cycles > 0) do_cycles_ce (cycles);
	}
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xff) | ((val) & 0xff);
}}}}	m68k_incpc (2);
}
/* LSR.B Dn,Dn */
void REGPARAM2 CPUFUNC(op_e028_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 cnt = m68k_dreg(regs, srcreg);
{	uae_s8 data = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 val = (uae_u8)data;
	int ccnt = cnt & 63;
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
		int cycles = 6 * 256;
		cycles += 2 * 256 * ccnt;
		if (cycles > 0) do_cycles_ce (cycles);
	}
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xff) | ((val) & 0xff);
}}}}	m68k_incpc (2);
}
/* ROXR.B Dn,Dn */
void REGPARAM2 CPUFUNC(op_e030_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 cnt = m68k_dreg(regs, srcreg);
{	uae_s8 data = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 val = (uae_u8)data;
	int ccnt = cnt & 63;
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
		int cycles = 6 * 256;
		cycles += 2 * 256 * ccnt;
		if (cycles > 0) do_cycles_ce (cycles);
	}
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xff) | ((val) & 0xff);
}}}}	m68k_incpc (2);
}
/* ROR.B Dn,Dn */
void REGPARAM2 CPUFUNC(op_e038_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 cnt = m68k_dreg(regs, srcreg);
{	uae_s8 data = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 val = (uae_u8)data;
	int ccnt = cnt & 63;
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
		int cycles = 6 * 256;
		cycles += 2 * 256 * ccnt;
		if (cycles > 0) do_cycles_ce (cycles);
	}
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xff) | ((val) & 0xff);
}}}}	m68k_incpc (2);
}
/* ASR.W #<data>,Dn */
void REGPARAM2 CPUFUNC(op_e040_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 cnt = srcreg;
{	uae_s16 data = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 val = (uae_u16)data;
	uae_u32 sign = (0x8000 & val) >> 15;
	int ccnt = cnt & 63;
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
		int cycles = 6 * 256;
		cycles += 2 * 256 * ccnt;
		if (cycles > 0) do_cycles_ce (cycles);
	}
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xffff) | ((val) & 0xffff);
}}}}	m68k_incpc (2);
}
/* LSR.W #<data>,Dn */
void REGPARAM2 CPUFUNC(op_e048_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 cnt = srcreg;
{	uae_s16 data = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 val = (uae_u16)data;
	int ccnt = cnt & 63;
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
		int cycles = 6 * 256;
		cycles += 2 * 256 * ccnt;
		if (cycles > 0) do_cycles_ce (cycles);
	}
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xffff) | ((val) & 0xffff);
}}}}	m68k_incpc (2);
}
/* ROXR.W #<data>,Dn */
void REGPARAM2 CPUFUNC(op_e050_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 cnt = srcreg;
{	uae_s16 data = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 val = (uae_u16)data;
	int ccnt = cnt & 63;
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
		int cycles = 6 * 256;
		cycles += 2 * 256 * ccnt;
		if (cycles > 0) do_cycles_ce (cycles);
	}
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xffff) | ((val) & 0xffff);
}}}}	m68k_incpc (2);
}
/* ROR.W #<data>,Dn */
void REGPARAM2 CPUFUNC(op_e058_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 cnt = srcreg;
{	uae_s16 data = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 val = (uae_u16)data;
	int ccnt = cnt & 63;
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
		int cycles = 6 * 256;
		cycles += 2 * 256 * ccnt;
		if (cycles > 0) do_cycles_ce (cycles);
	}
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xffff) | ((val) & 0xffff);
}}}}	m68k_incpc (2);
}
/* ASR.W Dn,Dn */
void REGPARAM2 CPUFUNC(op_e060_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 cnt = m68k_dreg(regs, srcreg);
{	uae_s16 data = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 val = (uae_u16)data;
	uae_u32 sign = (0x8000 & val) >> 15;
	int ccnt = cnt & 63;
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
		int cycles = 6 * 256;
		cycles += 2 * 256 * ccnt;
		if (cycles > 0) do_cycles_ce (cycles);
	}
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xffff) | ((val) & 0xffff);
}}}}	m68k_incpc (2);
}
/* LSR.W Dn,Dn */
void REGPARAM2 CPUFUNC(op_e068_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 cnt = m68k_dreg(regs, srcreg);
{	uae_s16 data = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 val = (uae_u16)data;
	int ccnt = cnt & 63;
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
		int cycles = 6 * 256;
		cycles += 2 * 256 * ccnt;
		if (cycles > 0) do_cycles_ce (cycles);
	}
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xffff) | ((val) & 0xffff);
}}}}	m68k_incpc (2);
}
/* ROXR.W Dn,Dn */
void REGPARAM2 CPUFUNC(op_e070_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 cnt = m68k_dreg(regs, srcreg);
{	uae_s16 data = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 val = (uae_u16)data;
	int ccnt = cnt & 63;
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
		int cycles = 6 * 256;
		cycles += 2 * 256 * ccnt;
		if (cycles > 0) do_cycles_ce (cycles);
	}
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xffff) | ((val) & 0xffff);
}}}}	m68k_incpc (2);
}
/* ROR.W Dn,Dn */
void REGPARAM2 CPUFUNC(op_e078_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 cnt = m68k_dreg(regs, srcreg);
{	uae_s16 data = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 val = (uae_u16)data;
	int ccnt = cnt & 63;
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
		int cycles = 6 * 256;
		cycles += 2 * 256 * ccnt;
		if (cycles > 0) do_cycles_ce (cycles);
	}
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xffff) | ((val) & 0xffff);
}}}}	m68k_incpc (2);
}
/* ASR.L #<data>,Dn */
void REGPARAM2 CPUFUNC(op_e080_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 cnt = srcreg;
{	uae_s32 data = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 val = data;
	uae_u32 sign = (0x80000000 & val) >> 31;
	int ccnt = cnt & 63;
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
		int cycles = 8 * 256;
		cycles += 2 * 256 * ccnt;
		if (cycles > 0) do_cycles_ce (cycles);
	}
	m68k_dreg(regs, dstreg) = (val);
}}}}	m68k_incpc (2);
}
/* LSR.L #<data>,Dn */
void REGPARAM2 CPUFUNC(op_e088_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 cnt = srcreg;
{	uae_s32 data = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 val = data;
	int ccnt = cnt & 63;
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
		int cycles = 8 * 256;
		cycles += 2 * 256 * ccnt;
		if (cycles > 0) do_cycles_ce (cycles);
	}
	m68k_dreg(regs, dstreg) = (val);
}}}}	m68k_incpc (2);
}
/* ROXR.L #<data>,Dn */
void REGPARAM2 CPUFUNC(op_e090_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 cnt = srcreg;
{	uae_s32 data = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 val = data;
	int ccnt = cnt & 63;
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
		int cycles = 8 * 256;
		cycles += 2 * 256 * ccnt;
		if (cycles > 0) do_cycles_ce (cycles);
	}
	m68k_dreg(regs, dstreg) = (val);
}}}}	m68k_incpc (2);
}
/* ROR.L #<data>,Dn */
void REGPARAM2 CPUFUNC(op_e098_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 cnt = srcreg;
{	uae_s32 data = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 val = data;
	int ccnt = cnt & 63;
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
		int cycles = 8 * 256;
		cycles += 2 * 256 * ccnt;
		if (cycles > 0) do_cycles_ce (cycles);
	}
	m68k_dreg(regs, dstreg) = (val);
}}}}	m68k_incpc (2);
}
/* ASR.L Dn,Dn */
void REGPARAM2 CPUFUNC(op_e0a0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 cnt = m68k_dreg(regs, srcreg);
{	uae_s32 data = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 val = data;
	uae_u32 sign = (0x80000000 & val) >> 31;
	int ccnt = cnt & 63;
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
		int cycles = 8 * 256;
		cycles += 2 * 256 * ccnt;
		if (cycles > 0) do_cycles_ce (cycles);
	}
	m68k_dreg(regs, dstreg) = (val);
}}}}	m68k_incpc (2);
}
/* LSR.L Dn,Dn */
void REGPARAM2 CPUFUNC(op_e0a8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 cnt = m68k_dreg(regs, srcreg);
{	uae_s32 data = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 val = data;
	int ccnt = cnt & 63;
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
		int cycles = 8 * 256;
		cycles += 2 * 256 * ccnt;
		if (cycles > 0) do_cycles_ce (cycles);
	}
	m68k_dreg(regs, dstreg) = (val);
}}}}	m68k_incpc (2);
}
/* ROXR.L Dn,Dn */
void REGPARAM2 CPUFUNC(op_e0b0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 cnt = m68k_dreg(regs, srcreg);
{	uae_s32 data = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 val = data;
	int ccnt = cnt & 63;
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
		int cycles = 8 * 256;
		cycles += 2 * 256 * ccnt;
		if (cycles > 0) do_cycles_ce (cycles);
	}
	m68k_dreg(regs, dstreg) = (val);
}}}}	m68k_incpc (2);
}
/* ROR.L Dn,Dn */
void REGPARAM2 CPUFUNC(op_e0b8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 cnt = m68k_dreg(regs, srcreg);
{	uae_s32 data = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 val = data;
	int ccnt = cnt & 63;
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
		int cycles = 8 * 256;
		cycles += 2 * 256 * ccnt;
		if (cycles > 0) do_cycles_ce (cycles);
	}
	m68k_dreg(regs, dstreg) = (val);
}}}}	m68k_incpc (2);
}
/* ASRW.W (An) */
void REGPARAM2 CPUFUNC(op_e0d0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg(regs, srcreg);
	if (dataa & 1) {
		exception3 (opcode, m68k_getpc() + 2, dataa);
		goto endlabel5196;
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
endlabel5196: ;
}
/* ASRW.W (An)+ */
void REGPARAM2 CPUFUNC(op_e0d8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg(regs, srcreg);
	if (dataa & 1) {
		exception3 (opcode, m68k_getpc() + 2, dataa);
		goto endlabel5197;
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
endlabel5197: ;
}
/* ASRW.W -(An) */
void REGPARAM2 CPUFUNC(op_e0e0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa;
	dataa = m68k_areg(regs, srcreg) - 2;
	do_cycles_ce (2 * 256);
	if (dataa & 1) {
		exception3 (opcode, m68k_getpc() + 2, dataa);
		goto endlabel5198;
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
endlabel5198: ;
}
/* ASRW.W (d16,An) */
void REGPARAM2 CPUFUNC(op_e0e8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dataa & 1) {
		exception3 (opcode, m68k_getpc() + 2, dataa);
		goto endlabel5199;
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
endlabel5199: ;
}
/* ASRW.W (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_e0f0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa;
	dataa = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (dataa & 1) {
		exception3 (opcode, m68k_getpc() + 2, dataa);
		goto endlabel5200;
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
endlabel5200: ;
}
/* ASRW.W (xxx).W */
void REGPARAM2 CPUFUNC(op_e0f8_6)(uae_u32 opcode)
{
{{	uaecptr dataa = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dataa & 1) {
		exception3 (opcode, m68k_getpc() + 2, dataa);
		goto endlabel5201;
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
endlabel5201: ;
}
/* ASRW.W (xxx).L */
void REGPARAM2 CPUFUNC(op_e0f9_6)(uae_u32 opcode)
{
{{	uaecptr dataa;
	dataa = get_word_ce_prefetch (4) << 16;
	dataa |= get_word_ce_prefetch (6);
	if (dataa & 1) {
		exception3 (opcode, m68k_getpc() + 2, dataa);
		goto endlabel5202;
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
endlabel5202: ;
}
/* ASL.B #<data>,Dn */
void REGPARAM2 CPUFUNC(op_e100_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 cnt = srcreg;
{	uae_s8 data = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 val = (uae_u8)data;
	int ccnt = cnt & 63;
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
		int cycles = 6 * 256;
		cycles += 2 * 256 * ccnt;
		if (cycles > 0) do_cycles_ce (cycles);
	}
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xff) | ((val) & 0xff);
}}}}	m68k_incpc (2);
}
/* LSL.B #<data>,Dn */
void REGPARAM2 CPUFUNC(op_e108_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 cnt = srcreg;
{	uae_s8 data = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 val = (uae_u8)data;
	int ccnt = cnt & 63;
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
		int cycles = 6 * 256;
		cycles += 2 * 256 * ccnt;
		if (cycles > 0) do_cycles_ce (cycles);
	}
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xff) | ((val) & 0xff);
}}}}	m68k_incpc (2);
}
/* ROXL.B #<data>,Dn */
void REGPARAM2 CPUFUNC(op_e110_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 cnt = srcreg;
{	uae_s8 data = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 val = (uae_u8)data;
	int ccnt = cnt & 63;
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
		int cycles = 6 * 256;
		cycles += 2 * 256 * ccnt;
		if (cycles > 0) do_cycles_ce (cycles);
	}
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xff) | ((val) & 0xff);
}}}}	m68k_incpc (2);
}
/* ROL.B #<data>,Dn */
void REGPARAM2 CPUFUNC(op_e118_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 cnt = srcreg;
{	uae_s8 data = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 val = (uae_u8)data;
	int ccnt = cnt & 63;
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
		int cycles = 6 * 256;
		cycles += 2 * 256 * ccnt;
		if (cycles > 0) do_cycles_ce (cycles);
	}
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xff) | ((val) & 0xff);
}}}}	m68k_incpc (2);
}
/* ASL.B Dn,Dn */
void REGPARAM2 CPUFUNC(op_e120_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 cnt = m68k_dreg(regs, srcreg);
{	uae_s8 data = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 val = (uae_u8)data;
	int ccnt = cnt & 63;
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
		int cycles = 6 * 256;
		cycles += 2 * 256 * ccnt;
		if (cycles > 0) do_cycles_ce (cycles);
	}
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xff) | ((val) & 0xff);
}}}}	m68k_incpc (2);
}
/* LSL.B Dn,Dn */
void REGPARAM2 CPUFUNC(op_e128_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 cnt = m68k_dreg(regs, srcreg);
{	uae_s8 data = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 val = (uae_u8)data;
	int ccnt = cnt & 63;
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
		int cycles = 6 * 256;
		cycles += 2 * 256 * ccnt;
		if (cycles > 0) do_cycles_ce (cycles);
	}
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xff) | ((val) & 0xff);
}}}}	m68k_incpc (2);
}
/* ROXL.B Dn,Dn */
void REGPARAM2 CPUFUNC(op_e130_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 cnt = m68k_dreg(regs, srcreg);
{	uae_s8 data = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 val = (uae_u8)data;
	int ccnt = cnt & 63;
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
		int cycles = 6 * 256;
		cycles += 2 * 256 * ccnt;
		if (cycles > 0) do_cycles_ce (cycles);
	}
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xff) | ((val) & 0xff);
}}}}	m68k_incpc (2);
}
/* ROL.B Dn,Dn */
void REGPARAM2 CPUFUNC(op_e138_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 cnt = m68k_dreg(regs, srcreg);
{	uae_s8 data = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 val = (uae_u8)data;
	int ccnt = cnt & 63;
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
		int cycles = 6 * 256;
		cycles += 2 * 256 * ccnt;
		if (cycles > 0) do_cycles_ce (cycles);
	}
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xff) | ((val) & 0xff);
}}}}	m68k_incpc (2);
}
/* ASL.W #<data>,Dn */
void REGPARAM2 CPUFUNC(op_e140_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 cnt = srcreg;
{	uae_s16 data = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 val = (uae_u16)data;
	int ccnt = cnt & 63;
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
		int cycles = 6 * 256;
		cycles += 2 * 256 * ccnt;
		if (cycles > 0) do_cycles_ce (cycles);
	}
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xffff) | ((val) & 0xffff);
}}}}	m68k_incpc (2);
}
/* LSL.W #<data>,Dn */
void REGPARAM2 CPUFUNC(op_e148_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 cnt = srcreg;
{	uae_s16 data = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 val = (uae_u16)data;
	int ccnt = cnt & 63;
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
		int cycles = 6 * 256;
		cycles += 2 * 256 * ccnt;
		if (cycles > 0) do_cycles_ce (cycles);
	}
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xffff) | ((val) & 0xffff);
}}}}	m68k_incpc (2);
}
/* ROXL.W #<data>,Dn */
void REGPARAM2 CPUFUNC(op_e150_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 cnt = srcreg;
{	uae_s16 data = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 val = (uae_u16)data;
	int ccnt = cnt & 63;
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
		int cycles = 6 * 256;
		cycles += 2 * 256 * ccnt;
		if (cycles > 0) do_cycles_ce (cycles);
	}
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xffff) | ((val) & 0xffff);
}}}}	m68k_incpc (2);
}
/* ROL.W #<data>,Dn */
void REGPARAM2 CPUFUNC(op_e158_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 cnt = srcreg;
{	uae_s16 data = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 val = (uae_u16)data;
	int ccnt = cnt & 63;
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
		int cycles = 6 * 256;
		cycles += 2 * 256 * ccnt;
		if (cycles > 0) do_cycles_ce (cycles);
	}
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xffff) | ((val) & 0xffff);
}}}}	m68k_incpc (2);
}
/* ASL.W Dn,Dn */
void REGPARAM2 CPUFUNC(op_e160_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 cnt = m68k_dreg(regs, srcreg);
{	uae_s16 data = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 val = (uae_u16)data;
	int ccnt = cnt & 63;
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
		int cycles = 6 * 256;
		cycles += 2 * 256 * ccnt;
		if (cycles > 0) do_cycles_ce (cycles);
	}
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xffff) | ((val) & 0xffff);
}}}}	m68k_incpc (2);
}
/* LSL.W Dn,Dn */
void REGPARAM2 CPUFUNC(op_e168_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 cnt = m68k_dreg(regs, srcreg);
{	uae_s16 data = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 val = (uae_u16)data;
	int ccnt = cnt & 63;
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
		int cycles = 6 * 256;
		cycles += 2 * 256 * ccnt;
		if (cycles > 0) do_cycles_ce (cycles);
	}
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xffff) | ((val) & 0xffff);
}}}}	m68k_incpc (2);
}
/* ROXL.W Dn,Dn */
void REGPARAM2 CPUFUNC(op_e170_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 cnt = m68k_dreg(regs, srcreg);
{	uae_s16 data = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 val = (uae_u16)data;
	int ccnt = cnt & 63;
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
		int cycles = 6 * 256;
		cycles += 2 * 256 * ccnt;
		if (cycles > 0) do_cycles_ce (cycles);
	}
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xffff) | ((val) & 0xffff);
}}}}	m68k_incpc (2);
}
/* ROL.W Dn,Dn */
void REGPARAM2 CPUFUNC(op_e178_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 cnt = m68k_dreg(regs, srcreg);
{	uae_s16 data = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 val = (uae_u16)data;
	int ccnt = cnt & 63;
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
		int cycles = 6 * 256;
		cycles += 2 * 256 * ccnt;
		if (cycles > 0) do_cycles_ce (cycles);
	}
	m68k_dreg(regs, dstreg) = (m68k_dreg(regs, dstreg) & ~0xffff) | ((val) & 0xffff);
}}}}	m68k_incpc (2);
}
/* ASL.L #<data>,Dn */
void REGPARAM2 CPUFUNC(op_e180_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 cnt = srcreg;
{	uae_s32 data = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 val = data;
	int ccnt = cnt & 63;
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
		int cycles = 8 * 256;
		cycles += 2 * 256 * ccnt;
		if (cycles > 0) do_cycles_ce (cycles);
	}
	m68k_dreg(regs, dstreg) = (val);
}}}}	m68k_incpc (2);
}
/* LSL.L #<data>,Dn */
void REGPARAM2 CPUFUNC(op_e188_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 cnt = srcreg;
{	uae_s32 data = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 val = data;
	int ccnt = cnt & 63;
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
		int cycles = 8 * 256;
		cycles += 2 * 256 * ccnt;
		if (cycles > 0) do_cycles_ce (cycles);
	}
	m68k_dreg(regs, dstreg) = (val);
}}}}	m68k_incpc (2);
}
/* ROXL.L #<data>,Dn */
void REGPARAM2 CPUFUNC(op_e190_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 cnt = srcreg;
{	uae_s32 data = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 val = data;
	int ccnt = cnt & 63;
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
		int cycles = 8 * 256;
		cycles += 2 * 256 * ccnt;
		if (cycles > 0) do_cycles_ce (cycles);
	}
	m68k_dreg(regs, dstreg) = (val);
}}}}	m68k_incpc (2);
}
/* ROL.L #<data>,Dn */
void REGPARAM2 CPUFUNC(op_e198_6)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 cnt = srcreg;
{	uae_s32 data = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 val = data;
	int ccnt = cnt & 63;
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
		int cycles = 8 * 256;
		cycles += 2 * 256 * ccnt;
		if (cycles > 0) do_cycles_ce (cycles);
	}
	m68k_dreg(regs, dstreg) = (val);
}}}}	m68k_incpc (2);
}
/* ASL.L Dn,Dn */
void REGPARAM2 CPUFUNC(op_e1a0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 cnt = m68k_dreg(regs, srcreg);
{	uae_s32 data = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 val = data;
	int ccnt = cnt & 63;
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
		int cycles = 8 * 256;
		cycles += 2 * 256 * ccnt;
		if (cycles > 0) do_cycles_ce (cycles);
	}
	m68k_dreg(regs, dstreg) = (val);
}}}}	m68k_incpc (2);
}
/* LSL.L Dn,Dn */
void REGPARAM2 CPUFUNC(op_e1a8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 cnt = m68k_dreg(regs, srcreg);
{	uae_s32 data = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 val = data;
	int ccnt = cnt & 63;
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
		int cycles = 8 * 256;
		cycles += 2 * 256 * ccnt;
		if (cycles > 0) do_cycles_ce (cycles);
	}
	m68k_dreg(regs, dstreg) = (val);
}}}}	m68k_incpc (2);
}
/* ROXL.L Dn,Dn */
void REGPARAM2 CPUFUNC(op_e1b0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 cnt = m68k_dreg(regs, srcreg);
{	uae_s32 data = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 val = data;
	int ccnt = cnt & 63;
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
		int cycles = 8 * 256;
		cycles += 2 * 256 * ccnt;
		if (cycles > 0) do_cycles_ce (cycles);
	}
	m68k_dreg(regs, dstreg) = (val);
}}}}	m68k_incpc (2);
}
/* ROL.L Dn,Dn */
void REGPARAM2 CPUFUNC(op_e1b8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 cnt = m68k_dreg(regs, srcreg);
{	uae_s32 data = m68k_dreg(regs, dstreg);
	regs.ir = regs.irc;
	get_word_ce_prefetch (4);
{	uae_u32 val = data;
	int ccnt = cnt & 63;
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
		int cycles = 8 * 256;
		cycles += 2 * 256 * ccnt;
		if (cycles > 0) do_cycles_ce (cycles);
	}
	m68k_dreg(regs, dstreg) = (val);
}}}}	m68k_incpc (2);
}
/* ASLW.W (An) */
void REGPARAM2 CPUFUNC(op_e1d0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg(regs, srcreg);
	if (dataa & 1) {
		exception3 (opcode, m68k_getpc() + 2, dataa);
		goto endlabel5227;
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
endlabel5227: ;
}
/* ASLW.W (An)+ */
void REGPARAM2 CPUFUNC(op_e1d8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg(regs, srcreg);
	if (dataa & 1) {
		exception3 (opcode, m68k_getpc() + 2, dataa);
		goto endlabel5228;
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
endlabel5228: ;
}
/* ASLW.W -(An) */
void REGPARAM2 CPUFUNC(op_e1e0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa;
	dataa = m68k_areg(regs, srcreg) - 2;
	do_cycles_ce (2 * 256);
	if (dataa & 1) {
		exception3 (opcode, m68k_getpc() + 2, dataa);
		goto endlabel5229;
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
endlabel5229: ;
}
/* ASLW.W (d16,An) */
void REGPARAM2 CPUFUNC(op_e1e8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dataa & 1) {
		exception3 (opcode, m68k_getpc() + 2, dataa);
		goto endlabel5230;
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
endlabel5230: ;
}
/* ASLW.W (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_e1f0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa;
	dataa = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (dataa & 1) {
		exception3 (opcode, m68k_getpc() + 2, dataa);
		goto endlabel5231;
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
endlabel5231: ;
}
/* ASLW.W (xxx).W */
void REGPARAM2 CPUFUNC(op_e1f8_6)(uae_u32 opcode)
{
{{	uaecptr dataa = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dataa & 1) {
		exception3 (opcode, m68k_getpc() + 2, dataa);
		goto endlabel5232;
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
endlabel5232: ;
}
/* ASLW.W (xxx).L */
void REGPARAM2 CPUFUNC(op_e1f9_6)(uae_u32 opcode)
{
{{	uaecptr dataa;
	dataa = get_word_ce_prefetch (4) << 16;
	dataa |= get_word_ce_prefetch (6);
	if (dataa & 1) {
		exception3 (opcode, m68k_getpc() + 2, dataa);
		goto endlabel5233;
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
endlabel5233: ;
}
/* LSRW.W (An) */
void REGPARAM2 CPUFUNC(op_e2d0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg(regs, srcreg);
	if (dataa & 1) {
		exception3 (opcode, m68k_getpc() + 2, dataa);
		goto endlabel5234;
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
endlabel5234: ;
}
/* LSRW.W (An)+ */
void REGPARAM2 CPUFUNC(op_e2d8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg(regs, srcreg);
	if (dataa & 1) {
		exception3 (opcode, m68k_getpc() + 2, dataa);
		goto endlabel5235;
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
endlabel5235: ;
}
/* LSRW.W -(An) */
void REGPARAM2 CPUFUNC(op_e2e0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa;
	dataa = m68k_areg(regs, srcreg) - 2;
	do_cycles_ce (2 * 256);
	if (dataa & 1) {
		exception3 (opcode, m68k_getpc() + 2, dataa);
		goto endlabel5236;
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
endlabel5236: ;
}
/* LSRW.W (d16,An) */
void REGPARAM2 CPUFUNC(op_e2e8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dataa & 1) {
		exception3 (opcode, m68k_getpc() + 2, dataa);
		goto endlabel5237;
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
endlabel5237: ;
}
/* LSRW.W (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_e2f0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa;
	dataa = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (dataa & 1) {
		exception3 (opcode, m68k_getpc() + 2, dataa);
		goto endlabel5238;
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
endlabel5238: ;
}
/* LSRW.W (xxx).W */
void REGPARAM2 CPUFUNC(op_e2f8_6)(uae_u32 opcode)
{
{{	uaecptr dataa = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dataa & 1) {
		exception3 (opcode, m68k_getpc() + 2, dataa);
		goto endlabel5239;
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
endlabel5239: ;
}
/* LSRW.W (xxx).L */
void REGPARAM2 CPUFUNC(op_e2f9_6)(uae_u32 opcode)
{
{{	uaecptr dataa;
	dataa = get_word_ce_prefetch (4) << 16;
	dataa |= get_word_ce_prefetch (6);
	if (dataa & 1) {
		exception3 (opcode, m68k_getpc() + 2, dataa);
		goto endlabel5240;
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
endlabel5240: ;
}
/* LSLW.W (An) */
void REGPARAM2 CPUFUNC(op_e3d0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg(regs, srcreg);
	if (dataa & 1) {
		exception3 (opcode, m68k_getpc() + 2, dataa);
		goto endlabel5241;
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
endlabel5241: ;
}
/* LSLW.W (An)+ */
void REGPARAM2 CPUFUNC(op_e3d8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg(regs, srcreg);
	if (dataa & 1) {
		exception3 (opcode, m68k_getpc() + 2, dataa);
		goto endlabel5242;
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
endlabel5242: ;
}
/* LSLW.W -(An) */
void REGPARAM2 CPUFUNC(op_e3e0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa;
	dataa = m68k_areg(regs, srcreg) - 2;
	do_cycles_ce (2 * 256);
	if (dataa & 1) {
		exception3 (opcode, m68k_getpc() + 2, dataa);
		goto endlabel5243;
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
endlabel5243: ;
}
/* LSLW.W (d16,An) */
void REGPARAM2 CPUFUNC(op_e3e8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dataa & 1) {
		exception3 (opcode, m68k_getpc() + 2, dataa);
		goto endlabel5244;
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
endlabel5244: ;
}
/* LSLW.W (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_e3f0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa;
	dataa = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (dataa & 1) {
		exception3 (opcode, m68k_getpc() + 2, dataa);
		goto endlabel5245;
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
endlabel5245: ;
}
/* LSLW.W (xxx).W */
void REGPARAM2 CPUFUNC(op_e3f8_6)(uae_u32 opcode)
{
{{	uaecptr dataa = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dataa & 1) {
		exception3 (opcode, m68k_getpc() + 2, dataa);
		goto endlabel5246;
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
endlabel5246: ;
}
/* LSLW.W (xxx).L */
void REGPARAM2 CPUFUNC(op_e3f9_6)(uae_u32 opcode)
{
{{	uaecptr dataa;
	dataa = get_word_ce_prefetch (4) << 16;
	dataa |= get_word_ce_prefetch (6);
	if (dataa & 1) {
		exception3 (opcode, m68k_getpc() + 2, dataa);
		goto endlabel5247;
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
endlabel5247: ;
}
/* ROXRW.W (An) */
void REGPARAM2 CPUFUNC(op_e4d0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg(regs, srcreg);
	if (dataa & 1) {
		exception3 (opcode, m68k_getpc() + 2, dataa);
		goto endlabel5248;
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
endlabel5248: ;
}
/* ROXRW.W (An)+ */
void REGPARAM2 CPUFUNC(op_e4d8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg(regs, srcreg);
	if (dataa & 1) {
		exception3 (opcode, m68k_getpc() + 2, dataa);
		goto endlabel5249;
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
endlabel5249: ;
}
/* ROXRW.W -(An) */
void REGPARAM2 CPUFUNC(op_e4e0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa;
	dataa = m68k_areg(regs, srcreg) - 2;
	do_cycles_ce (2 * 256);
	if (dataa & 1) {
		exception3 (opcode, m68k_getpc() + 2, dataa);
		goto endlabel5250;
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
endlabel5250: ;
}
/* ROXRW.W (d16,An) */
void REGPARAM2 CPUFUNC(op_e4e8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dataa & 1) {
		exception3 (opcode, m68k_getpc() + 2, dataa);
		goto endlabel5251;
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
endlabel5251: ;
}
/* ROXRW.W (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_e4f0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa;
	dataa = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (dataa & 1) {
		exception3 (opcode, m68k_getpc() + 2, dataa);
		goto endlabel5252;
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
endlabel5252: ;
}
/* ROXRW.W (xxx).W */
void REGPARAM2 CPUFUNC(op_e4f8_6)(uae_u32 opcode)
{
{{	uaecptr dataa = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dataa & 1) {
		exception3 (opcode, m68k_getpc() + 2, dataa);
		goto endlabel5253;
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
endlabel5253: ;
}
/* ROXRW.W (xxx).L */
void REGPARAM2 CPUFUNC(op_e4f9_6)(uae_u32 opcode)
{
{{	uaecptr dataa;
	dataa = get_word_ce_prefetch (4) << 16;
	dataa |= get_word_ce_prefetch (6);
	if (dataa & 1) {
		exception3 (opcode, m68k_getpc() + 2, dataa);
		goto endlabel5254;
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
endlabel5254: ;
}
/* ROXLW.W (An) */
void REGPARAM2 CPUFUNC(op_e5d0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg(regs, srcreg);
	if (dataa & 1) {
		exception3 (opcode, m68k_getpc() + 2, dataa);
		goto endlabel5255;
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
endlabel5255: ;
}
/* ROXLW.W (An)+ */
void REGPARAM2 CPUFUNC(op_e5d8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg(regs, srcreg);
	if (dataa & 1) {
		exception3 (opcode, m68k_getpc() + 2, dataa);
		goto endlabel5256;
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
endlabel5256: ;
}
/* ROXLW.W -(An) */
void REGPARAM2 CPUFUNC(op_e5e0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa;
	dataa = m68k_areg(regs, srcreg) - 2;
	do_cycles_ce (2 * 256);
	if (dataa & 1) {
		exception3 (opcode, m68k_getpc() + 2, dataa);
		goto endlabel5257;
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
endlabel5257: ;
}
/* ROXLW.W (d16,An) */
void REGPARAM2 CPUFUNC(op_e5e8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dataa & 1) {
		exception3 (opcode, m68k_getpc() + 2, dataa);
		goto endlabel5258;
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
endlabel5258: ;
}
/* ROXLW.W (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_e5f0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa;
	dataa = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (dataa & 1) {
		exception3 (opcode, m68k_getpc() + 2, dataa);
		goto endlabel5259;
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
endlabel5259: ;
}
/* ROXLW.W (xxx).W */
void REGPARAM2 CPUFUNC(op_e5f8_6)(uae_u32 opcode)
{
{{	uaecptr dataa = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dataa & 1) {
		exception3 (opcode, m68k_getpc() + 2, dataa);
		goto endlabel5260;
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
endlabel5260: ;
}
/* ROXLW.W (xxx).L */
void REGPARAM2 CPUFUNC(op_e5f9_6)(uae_u32 opcode)
{
{{	uaecptr dataa;
	dataa = get_word_ce_prefetch (4) << 16;
	dataa |= get_word_ce_prefetch (6);
	if (dataa & 1) {
		exception3 (opcode, m68k_getpc() + 2, dataa);
		goto endlabel5261;
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
endlabel5261: ;
}
/* RORW.W (An) */
void REGPARAM2 CPUFUNC(op_e6d0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg(regs, srcreg);
	if (dataa & 1) {
		exception3 (opcode, m68k_getpc() + 2, dataa);
		goto endlabel5262;
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
endlabel5262: ;
}
/* RORW.W (An)+ */
void REGPARAM2 CPUFUNC(op_e6d8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg(regs, srcreg);
	if (dataa & 1) {
		exception3 (opcode, m68k_getpc() + 2, dataa);
		goto endlabel5263;
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
endlabel5263: ;
}
/* RORW.W -(An) */
void REGPARAM2 CPUFUNC(op_e6e0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa;
	dataa = m68k_areg(regs, srcreg) - 2;
	do_cycles_ce (2 * 256);
	if (dataa & 1) {
		exception3 (opcode, m68k_getpc() + 2, dataa);
		goto endlabel5264;
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
endlabel5264: ;
}
/* RORW.W (d16,An) */
void REGPARAM2 CPUFUNC(op_e6e8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dataa & 1) {
		exception3 (opcode, m68k_getpc() + 2, dataa);
		goto endlabel5265;
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
endlabel5265: ;
}
/* RORW.W (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_e6f0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa;
	dataa = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (dataa & 1) {
		exception3 (opcode, m68k_getpc() + 2, dataa);
		goto endlabel5266;
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
endlabel5266: ;
}
/* RORW.W (xxx).W */
void REGPARAM2 CPUFUNC(op_e6f8_6)(uae_u32 opcode)
{
{{	uaecptr dataa = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dataa & 1) {
		exception3 (opcode, m68k_getpc() + 2, dataa);
		goto endlabel5267;
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
endlabel5267: ;
}
/* RORW.W (xxx).L */
void REGPARAM2 CPUFUNC(op_e6f9_6)(uae_u32 opcode)
{
{{	uaecptr dataa;
	dataa = get_word_ce_prefetch (4) << 16;
	dataa |= get_word_ce_prefetch (6);
	if (dataa & 1) {
		exception3 (opcode, m68k_getpc() + 2, dataa);
		goto endlabel5268;
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
endlabel5268: ;
}
/* ROLW.W (An) */
void REGPARAM2 CPUFUNC(op_e7d0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg(regs, srcreg);
	if (dataa & 1) {
		exception3 (opcode, m68k_getpc() + 2, dataa);
		goto endlabel5269;
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
endlabel5269: ;
}
/* ROLW.W (An)+ */
void REGPARAM2 CPUFUNC(op_e7d8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg(regs, srcreg);
	if (dataa & 1) {
		exception3 (opcode, m68k_getpc() + 2, dataa);
		goto endlabel5270;
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
endlabel5270: ;
}
/* ROLW.W -(An) */
void REGPARAM2 CPUFUNC(op_e7e0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa;
	dataa = m68k_areg(regs, srcreg) - 2;
	do_cycles_ce (2 * 256);
	if (dataa & 1) {
		exception3 (opcode, m68k_getpc() + 2, dataa);
		goto endlabel5271;
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
endlabel5271: ;
}
/* ROLW.W (d16,An) */
void REGPARAM2 CPUFUNC(op_e7e8_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dataa & 1) {
		exception3 (opcode, m68k_getpc() + 2, dataa);
		goto endlabel5272;
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
endlabel5272: ;
}
/* ROLW.W (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_e7f0_6)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa;
	dataa = get_disp_ea_000(m68k_areg(regs, srcreg), get_word_ce_prefetch (4));
	do_cycles_ce (2 * 256);
	if (dataa & 1) {
		exception3 (opcode, m68k_getpc() + 2, dataa);
		goto endlabel5273;
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
endlabel5273: ;
}
/* ROLW.W (xxx).W */
void REGPARAM2 CPUFUNC(op_e7f8_6)(uae_u32 opcode)
{
{{	uaecptr dataa = (uae_s32)(uae_s16)get_word_ce_prefetch (4);
	if (dataa & 1) {
		exception3 (opcode, m68k_getpc() + 2, dataa);
		goto endlabel5274;
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
endlabel5274: ;
}
/* ROLW.W (xxx).L */
void REGPARAM2 CPUFUNC(op_e7f9_6)(uae_u32 opcode)
{
{{	uaecptr dataa;
	dataa = get_word_ce_prefetch (4) << 16;
	dataa |= get_word_ce_prefetch (6);
	if (dataa & 1) {
		exception3 (opcode, m68k_getpc() + 2, dataa);
		goto endlabel5275;
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
endlabel5275: ;
}
#endif


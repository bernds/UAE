#include "sysconfig.h"
#include "sysdeps.h"
#include "options.h"
#include "memory.h"
#include "custom.h"
#include "events.h"
#include "newcpu.h"
#include "cpu_prefetch.h"
#include "cputbl.h"
#define CPUFUNC(x) x##_ff
#define SET_CFLG_ALWAYS(flags, x) SET_CFLG(flags, x)
#define SET_NFLG_ALWAYS(flags, x) SET_NFLG(flags, x)
#ifdef NOFLAGS
#include "noflags.h"
#endif
#ifdef CPUEMU_0
const struct cputbl CPUFUNC(op_smalltbl_0)[] = {
{ CPUFUNC(op_0000_0), 0 }, /* OR */
{ CPUFUNC(op_0010_0), 16 }, /* OR */
{ CPUFUNC(op_0018_0), 24 }, /* OR */
{ CPUFUNC(op_0020_0), 32 }, /* OR */
{ CPUFUNC(op_0028_0), 40 }, /* OR */
{ CPUFUNC(op_0030_0), 48 }, /* OR */
{ CPUFUNC(op_0038_0), 56 }, /* OR */
{ CPUFUNC(op_0039_0), 57 }, /* OR */
{ CPUFUNC(op_003c_0), 60 }, /* ORSR */
{ CPUFUNC(op_0040_0), 64 }, /* OR */
{ CPUFUNC(op_0050_0), 80 }, /* OR */
{ CPUFUNC(op_0058_0), 88 }, /* OR */
{ CPUFUNC(op_0060_0), 96 }, /* OR */
{ CPUFUNC(op_0068_0), 104 }, /* OR */
{ CPUFUNC(op_0070_0), 112 }, /* OR */
{ CPUFUNC(op_0078_0), 120 }, /* OR */
{ CPUFUNC(op_0079_0), 121 }, /* OR */
{ CPUFUNC(op_007c_0), 124 }, /* ORSR */
{ CPUFUNC(op_0080_0), 128 }, /* OR */
{ CPUFUNC(op_0090_0), 144 }, /* OR */
{ CPUFUNC(op_0098_0), 152 }, /* OR */
{ CPUFUNC(op_00a0_0), 160 }, /* OR */
{ CPUFUNC(op_00a8_0), 168 }, /* OR */
{ CPUFUNC(op_00b0_0), 176 }, /* OR */
{ CPUFUNC(op_00b8_0), 184 }, /* OR */
{ CPUFUNC(op_00b9_0), 185 }, /* OR */
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_00d0_0), 208 }, /* CHK2 */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_00e8_0), 232 }, /* CHK2 */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_00f0_0), 240 }, /* CHK2 */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_00f8_0), 248 }, /* CHK2 */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_00f9_0), 249 }, /* CHK2 */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_00fa_0), 250 }, /* CHK2 */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_00fb_0), 251 }, /* CHK2 */
#endif
{ CPUFUNC(op_0100_0), 256 }, /* BTST */
{ CPUFUNC(op_0108_0), 264 }, /* MVPMR */
{ CPUFUNC(op_0110_0), 272 }, /* BTST */
{ CPUFUNC(op_0118_0), 280 }, /* BTST */
{ CPUFUNC(op_0120_0), 288 }, /* BTST */
{ CPUFUNC(op_0128_0), 296 }, /* BTST */
{ CPUFUNC(op_0130_0), 304 }, /* BTST */
{ CPUFUNC(op_0138_0), 312 }, /* BTST */
{ CPUFUNC(op_0139_0), 313 }, /* BTST */
{ CPUFUNC(op_013a_0), 314 }, /* BTST */
{ CPUFUNC(op_013b_0), 315 }, /* BTST */
{ CPUFUNC(op_013c_0), 316 }, /* BTST */
{ CPUFUNC(op_0140_0), 320 }, /* BCHG */
{ CPUFUNC(op_0148_0), 328 }, /* MVPMR */
{ CPUFUNC(op_0150_0), 336 }, /* BCHG */
{ CPUFUNC(op_0158_0), 344 }, /* BCHG */
{ CPUFUNC(op_0160_0), 352 }, /* BCHG */
{ CPUFUNC(op_0168_0), 360 }, /* BCHG */
{ CPUFUNC(op_0170_0), 368 }, /* BCHG */
{ CPUFUNC(op_0178_0), 376 }, /* BCHG */
{ CPUFUNC(op_0179_0), 377 }, /* BCHG */
{ CPUFUNC(op_017a_0), 378 }, /* BCHG */
{ CPUFUNC(op_017b_0), 379 }, /* BCHG */
{ CPUFUNC(op_0180_0), 384 }, /* BCLR */
{ CPUFUNC(op_0188_0), 392 }, /* MVPRM */
{ CPUFUNC(op_0190_0), 400 }, /* BCLR */
{ CPUFUNC(op_0198_0), 408 }, /* BCLR */
{ CPUFUNC(op_01a0_0), 416 }, /* BCLR */
{ CPUFUNC(op_01a8_0), 424 }, /* BCLR */
{ CPUFUNC(op_01b0_0), 432 }, /* BCLR */
{ CPUFUNC(op_01b8_0), 440 }, /* BCLR */
{ CPUFUNC(op_01b9_0), 441 }, /* BCLR */
{ CPUFUNC(op_01ba_0), 442 }, /* BCLR */
{ CPUFUNC(op_01bb_0), 443 }, /* BCLR */
{ CPUFUNC(op_01c0_0), 448 }, /* BSET */
{ CPUFUNC(op_01c8_0), 456 }, /* MVPRM */
{ CPUFUNC(op_01d0_0), 464 }, /* BSET */
{ CPUFUNC(op_01d8_0), 472 }, /* BSET */
{ CPUFUNC(op_01e0_0), 480 }, /* BSET */
{ CPUFUNC(op_01e8_0), 488 }, /* BSET */
{ CPUFUNC(op_01f0_0), 496 }, /* BSET */
{ CPUFUNC(op_01f8_0), 504 }, /* BSET */
{ CPUFUNC(op_01f9_0), 505 }, /* BSET */
{ CPUFUNC(op_01fa_0), 506 }, /* BSET */
{ CPUFUNC(op_01fb_0), 507 }, /* BSET */
{ CPUFUNC(op_0200_0), 512 }, /* AND */
{ CPUFUNC(op_0210_0), 528 }, /* AND */
{ CPUFUNC(op_0218_0), 536 }, /* AND */
{ CPUFUNC(op_0220_0), 544 }, /* AND */
{ CPUFUNC(op_0228_0), 552 }, /* AND */
{ CPUFUNC(op_0230_0), 560 }, /* AND */
{ CPUFUNC(op_0238_0), 568 }, /* AND */
{ CPUFUNC(op_0239_0), 569 }, /* AND */
{ CPUFUNC(op_023c_0), 572 }, /* ANDSR */
{ CPUFUNC(op_0240_0), 576 }, /* AND */
{ CPUFUNC(op_0250_0), 592 }, /* AND */
{ CPUFUNC(op_0258_0), 600 }, /* AND */
{ CPUFUNC(op_0260_0), 608 }, /* AND */
{ CPUFUNC(op_0268_0), 616 }, /* AND */
{ CPUFUNC(op_0270_0), 624 }, /* AND */
{ CPUFUNC(op_0278_0), 632 }, /* AND */
{ CPUFUNC(op_0279_0), 633 }, /* AND */
{ CPUFUNC(op_027c_0), 636 }, /* ANDSR */
{ CPUFUNC(op_0280_0), 640 }, /* AND */
{ CPUFUNC(op_0290_0), 656 }, /* AND */
{ CPUFUNC(op_0298_0), 664 }, /* AND */
{ CPUFUNC(op_02a0_0), 672 }, /* AND */
{ CPUFUNC(op_02a8_0), 680 }, /* AND */
{ CPUFUNC(op_02b0_0), 688 }, /* AND */
{ CPUFUNC(op_02b8_0), 696 }, /* AND */
{ CPUFUNC(op_02b9_0), 697 }, /* AND */
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_02d0_0), 720 }, /* CHK2 */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_02e8_0), 744 }, /* CHK2 */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_02f0_0), 752 }, /* CHK2 */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_02f8_0), 760 }, /* CHK2 */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_02f9_0), 761 }, /* CHK2 */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_02fa_0), 762 }, /* CHK2 */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_02fb_0), 763 }, /* CHK2 */
#endif
{ CPUFUNC(op_0400_0), 1024 }, /* SUB */
{ CPUFUNC(op_0410_0), 1040 }, /* SUB */
{ CPUFUNC(op_0418_0), 1048 }, /* SUB */
{ CPUFUNC(op_0420_0), 1056 }, /* SUB */
{ CPUFUNC(op_0428_0), 1064 }, /* SUB */
{ CPUFUNC(op_0430_0), 1072 }, /* SUB */
{ CPUFUNC(op_0438_0), 1080 }, /* SUB */
{ CPUFUNC(op_0439_0), 1081 }, /* SUB */
{ CPUFUNC(op_0440_0), 1088 }, /* SUB */
{ CPUFUNC(op_0450_0), 1104 }, /* SUB */
{ CPUFUNC(op_0458_0), 1112 }, /* SUB */
{ CPUFUNC(op_0460_0), 1120 }, /* SUB */
{ CPUFUNC(op_0468_0), 1128 }, /* SUB */
{ CPUFUNC(op_0470_0), 1136 }, /* SUB */
{ CPUFUNC(op_0478_0), 1144 }, /* SUB */
{ CPUFUNC(op_0479_0), 1145 }, /* SUB */
{ CPUFUNC(op_0480_0), 1152 }, /* SUB */
{ CPUFUNC(op_0490_0), 1168 }, /* SUB */
{ CPUFUNC(op_0498_0), 1176 }, /* SUB */
{ CPUFUNC(op_04a0_0), 1184 }, /* SUB */
{ CPUFUNC(op_04a8_0), 1192 }, /* SUB */
{ CPUFUNC(op_04b0_0), 1200 }, /* SUB */
{ CPUFUNC(op_04b8_0), 1208 }, /* SUB */
{ CPUFUNC(op_04b9_0), 1209 }, /* SUB */
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_04d0_0), 1232 }, /* CHK2 */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_04e8_0), 1256 }, /* CHK2 */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_04f0_0), 1264 }, /* CHK2 */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_04f8_0), 1272 }, /* CHK2 */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_04f9_0), 1273 }, /* CHK2 */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_04fa_0), 1274 }, /* CHK2 */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_04fb_0), 1275 }, /* CHK2 */
#endif
{ CPUFUNC(op_0600_0), 1536 }, /* ADD */
{ CPUFUNC(op_0610_0), 1552 }, /* ADD */
{ CPUFUNC(op_0618_0), 1560 }, /* ADD */
{ CPUFUNC(op_0620_0), 1568 }, /* ADD */
{ CPUFUNC(op_0628_0), 1576 }, /* ADD */
{ CPUFUNC(op_0630_0), 1584 }, /* ADD */
{ CPUFUNC(op_0638_0), 1592 }, /* ADD */
{ CPUFUNC(op_0639_0), 1593 }, /* ADD */
{ CPUFUNC(op_0640_0), 1600 }, /* ADD */
{ CPUFUNC(op_0650_0), 1616 }, /* ADD */
{ CPUFUNC(op_0658_0), 1624 }, /* ADD */
{ CPUFUNC(op_0660_0), 1632 }, /* ADD */
{ CPUFUNC(op_0668_0), 1640 }, /* ADD */
{ CPUFUNC(op_0670_0), 1648 }, /* ADD */
{ CPUFUNC(op_0678_0), 1656 }, /* ADD */
{ CPUFUNC(op_0679_0), 1657 }, /* ADD */
{ CPUFUNC(op_0680_0), 1664 }, /* ADD */
{ CPUFUNC(op_0690_0), 1680 }, /* ADD */
{ CPUFUNC(op_0698_0), 1688 }, /* ADD */
{ CPUFUNC(op_06a0_0), 1696 }, /* ADD */
{ CPUFUNC(op_06a8_0), 1704 }, /* ADD */
{ CPUFUNC(op_06b0_0), 1712 }, /* ADD */
{ CPUFUNC(op_06b8_0), 1720 }, /* ADD */
{ CPUFUNC(op_06b9_0), 1721 }, /* ADD */
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_06c0_0), 1728 }, /* RTM */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_06c8_0), 1736 }, /* RTM */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_06d0_0), 1744 }, /* CALLM */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_06e8_0), 1768 }, /* CALLM */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_06f0_0), 1776 }, /* CALLM */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_06f8_0), 1784 }, /* CALLM */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_06f9_0), 1785 }, /* CALLM */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_06fa_0), 1786 }, /* CALLM */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_06fb_0), 1787 }, /* CALLM */
#endif
{ CPUFUNC(op_0800_0), 2048 }, /* BTST */
{ CPUFUNC(op_0810_0), 2064 }, /* BTST */
{ CPUFUNC(op_0818_0), 2072 }, /* BTST */
{ CPUFUNC(op_0820_0), 2080 }, /* BTST */
{ CPUFUNC(op_0828_0), 2088 }, /* BTST */
{ CPUFUNC(op_0830_0), 2096 }, /* BTST */
{ CPUFUNC(op_0838_0), 2104 }, /* BTST */
{ CPUFUNC(op_0839_0), 2105 }, /* BTST */
{ CPUFUNC(op_083a_0), 2106 }, /* BTST */
{ CPUFUNC(op_083b_0), 2107 }, /* BTST */
{ CPUFUNC(op_083c_0), 2108 }, /* BTST */
{ CPUFUNC(op_0840_0), 2112 }, /* BCHG */
{ CPUFUNC(op_0850_0), 2128 }, /* BCHG */
{ CPUFUNC(op_0858_0), 2136 }, /* BCHG */
{ CPUFUNC(op_0860_0), 2144 }, /* BCHG */
{ CPUFUNC(op_0868_0), 2152 }, /* BCHG */
{ CPUFUNC(op_0870_0), 2160 }, /* BCHG */
{ CPUFUNC(op_0878_0), 2168 }, /* BCHG */
{ CPUFUNC(op_0879_0), 2169 }, /* BCHG */
{ CPUFUNC(op_087a_0), 2170 }, /* BCHG */
{ CPUFUNC(op_087b_0), 2171 }, /* BCHG */
{ CPUFUNC(op_0880_0), 2176 }, /* BCLR */
{ CPUFUNC(op_0890_0), 2192 }, /* BCLR */
{ CPUFUNC(op_0898_0), 2200 }, /* BCLR */
{ CPUFUNC(op_08a0_0), 2208 }, /* BCLR */
{ CPUFUNC(op_08a8_0), 2216 }, /* BCLR */
{ CPUFUNC(op_08b0_0), 2224 }, /* BCLR */
{ CPUFUNC(op_08b8_0), 2232 }, /* BCLR */
{ CPUFUNC(op_08b9_0), 2233 }, /* BCLR */
{ CPUFUNC(op_08ba_0), 2234 }, /* BCLR */
{ CPUFUNC(op_08bb_0), 2235 }, /* BCLR */
{ CPUFUNC(op_08c0_0), 2240 }, /* BSET */
{ CPUFUNC(op_08d0_0), 2256 }, /* BSET */
{ CPUFUNC(op_08d8_0), 2264 }, /* BSET */
{ CPUFUNC(op_08e0_0), 2272 }, /* BSET */
{ CPUFUNC(op_08e8_0), 2280 }, /* BSET */
{ CPUFUNC(op_08f0_0), 2288 }, /* BSET */
{ CPUFUNC(op_08f8_0), 2296 }, /* BSET */
{ CPUFUNC(op_08f9_0), 2297 }, /* BSET */
{ CPUFUNC(op_08fa_0), 2298 }, /* BSET */
{ CPUFUNC(op_08fb_0), 2299 }, /* BSET */
{ CPUFUNC(op_0a00_0), 2560 }, /* EOR */
{ CPUFUNC(op_0a10_0), 2576 }, /* EOR */
{ CPUFUNC(op_0a18_0), 2584 }, /* EOR */
{ CPUFUNC(op_0a20_0), 2592 }, /* EOR */
{ CPUFUNC(op_0a28_0), 2600 }, /* EOR */
{ CPUFUNC(op_0a30_0), 2608 }, /* EOR */
{ CPUFUNC(op_0a38_0), 2616 }, /* EOR */
{ CPUFUNC(op_0a39_0), 2617 }, /* EOR */
{ CPUFUNC(op_0a3c_0), 2620 }, /* EORSR */
{ CPUFUNC(op_0a40_0), 2624 }, /* EOR */
{ CPUFUNC(op_0a50_0), 2640 }, /* EOR */
{ CPUFUNC(op_0a58_0), 2648 }, /* EOR */
{ CPUFUNC(op_0a60_0), 2656 }, /* EOR */
{ CPUFUNC(op_0a68_0), 2664 }, /* EOR */
{ CPUFUNC(op_0a70_0), 2672 }, /* EOR */
{ CPUFUNC(op_0a78_0), 2680 }, /* EOR */
{ CPUFUNC(op_0a79_0), 2681 }, /* EOR */
{ CPUFUNC(op_0a7c_0), 2684 }, /* EORSR */
{ CPUFUNC(op_0a80_0), 2688 }, /* EOR */
{ CPUFUNC(op_0a90_0), 2704 }, /* EOR */
{ CPUFUNC(op_0a98_0), 2712 }, /* EOR */
{ CPUFUNC(op_0aa0_0), 2720 }, /* EOR */
{ CPUFUNC(op_0aa8_0), 2728 }, /* EOR */
{ CPUFUNC(op_0ab0_0), 2736 }, /* EOR */
{ CPUFUNC(op_0ab8_0), 2744 }, /* EOR */
{ CPUFUNC(op_0ab9_0), 2745 }, /* EOR */
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_0ad0_0), 2768 }, /* CAS */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_0ad8_0), 2776 }, /* CAS */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_0ae0_0), 2784 }, /* CAS */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_0ae8_0), 2792 }, /* CAS */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_0af0_0), 2800 }, /* CAS */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_0af8_0), 2808 }, /* CAS */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_0af9_0), 2809 }, /* CAS */
#endif
{ CPUFUNC(op_0c00_0), 3072 }, /* CMP */
{ CPUFUNC(op_0c10_0), 3088 }, /* CMP */
{ CPUFUNC(op_0c18_0), 3096 }, /* CMP */
{ CPUFUNC(op_0c20_0), 3104 }, /* CMP */
{ CPUFUNC(op_0c28_0), 3112 }, /* CMP */
{ CPUFUNC(op_0c30_0), 3120 }, /* CMP */
{ CPUFUNC(op_0c38_0), 3128 }, /* CMP */
{ CPUFUNC(op_0c39_0), 3129 }, /* CMP */
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_0c3a_0), 3130 }, /* CMP */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_0c3b_0), 3131 }, /* CMP */
#endif
{ CPUFUNC(op_0c40_0), 3136 }, /* CMP */
{ CPUFUNC(op_0c50_0), 3152 }, /* CMP */
{ CPUFUNC(op_0c58_0), 3160 }, /* CMP */
{ CPUFUNC(op_0c60_0), 3168 }, /* CMP */
{ CPUFUNC(op_0c68_0), 3176 }, /* CMP */
{ CPUFUNC(op_0c70_0), 3184 }, /* CMP */
{ CPUFUNC(op_0c78_0), 3192 }, /* CMP */
{ CPUFUNC(op_0c79_0), 3193 }, /* CMP */
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_0c7a_0), 3194 }, /* CMP */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_0c7b_0), 3195 }, /* CMP */
#endif
{ CPUFUNC(op_0c80_0), 3200 }, /* CMP */
{ CPUFUNC(op_0c90_0), 3216 }, /* CMP */
{ CPUFUNC(op_0c98_0), 3224 }, /* CMP */
{ CPUFUNC(op_0ca0_0), 3232 }, /* CMP */
{ CPUFUNC(op_0ca8_0), 3240 }, /* CMP */
{ CPUFUNC(op_0cb0_0), 3248 }, /* CMP */
{ CPUFUNC(op_0cb8_0), 3256 }, /* CMP */
{ CPUFUNC(op_0cb9_0), 3257 }, /* CMP */
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_0cba_0), 3258 }, /* CMP */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_0cbb_0), 3259 }, /* CMP */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_0cd0_0), 3280 }, /* CAS */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_0cd8_0), 3288 }, /* CAS */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_0ce0_0), 3296 }, /* CAS */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_0ce8_0), 3304 }, /* CAS */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_0cf0_0), 3312 }, /* CAS */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_0cf8_0), 3320 }, /* CAS */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_0cf9_0), 3321 }, /* CAS */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_0cfc_0), 3324 }, /* CAS2 */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_0e10_0), 3600 }, /* MOVES */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_0e18_0), 3608 }, /* MOVES */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_0e20_0), 3616 }, /* MOVES */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_0e28_0), 3624 }, /* MOVES */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_0e30_0), 3632 }, /* MOVES */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_0e38_0), 3640 }, /* MOVES */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_0e39_0), 3641 }, /* MOVES */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_0e50_0), 3664 }, /* MOVES */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_0e58_0), 3672 }, /* MOVES */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_0e60_0), 3680 }, /* MOVES */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_0e68_0), 3688 }, /* MOVES */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_0e70_0), 3696 }, /* MOVES */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_0e78_0), 3704 }, /* MOVES */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_0e79_0), 3705 }, /* MOVES */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_0e90_0), 3728 }, /* MOVES */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_0e98_0), 3736 }, /* MOVES */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_0ea0_0), 3744 }, /* MOVES */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_0ea8_0), 3752 }, /* MOVES */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_0eb0_0), 3760 }, /* MOVES */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_0eb8_0), 3768 }, /* MOVES */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_0eb9_0), 3769 }, /* MOVES */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_0ed0_0), 3792 }, /* CAS */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_0ed8_0), 3800 }, /* CAS */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_0ee0_0), 3808 }, /* CAS */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_0ee8_0), 3816 }, /* CAS */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_0ef0_0), 3824 }, /* CAS */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_0ef8_0), 3832 }, /* CAS */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_0ef9_0), 3833 }, /* CAS */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_0efc_0), 3836 }, /* CAS2 */
#endif
{ CPUFUNC(op_1000_0), 4096 }, /* MOVE */
{ CPUFUNC(op_1010_0), 4112 }, /* MOVE */
{ CPUFUNC(op_1018_0), 4120 }, /* MOVE */
{ CPUFUNC(op_1020_0), 4128 }, /* MOVE */
{ CPUFUNC(op_1028_0), 4136 }, /* MOVE */
{ CPUFUNC(op_1030_0), 4144 }, /* MOVE */
{ CPUFUNC(op_1038_0), 4152 }, /* MOVE */
{ CPUFUNC(op_1039_0), 4153 }, /* MOVE */
{ CPUFUNC(op_103a_0), 4154 }, /* MOVE */
{ CPUFUNC(op_103b_0), 4155 }, /* MOVE */
{ CPUFUNC(op_103c_0), 4156 }, /* MOVE */
{ CPUFUNC(op_1080_0), 4224 }, /* MOVE */
{ CPUFUNC(op_1090_0), 4240 }, /* MOVE */
{ CPUFUNC(op_1098_0), 4248 }, /* MOVE */
{ CPUFUNC(op_10a0_0), 4256 }, /* MOVE */
{ CPUFUNC(op_10a8_0), 4264 }, /* MOVE */
{ CPUFUNC(op_10b0_0), 4272 }, /* MOVE */
{ CPUFUNC(op_10b8_0), 4280 }, /* MOVE */
{ CPUFUNC(op_10b9_0), 4281 }, /* MOVE */
{ CPUFUNC(op_10ba_0), 4282 }, /* MOVE */
{ CPUFUNC(op_10bb_0), 4283 }, /* MOVE */
{ CPUFUNC(op_10bc_0), 4284 }, /* MOVE */
{ CPUFUNC(op_10c0_0), 4288 }, /* MOVE */
{ CPUFUNC(op_10d0_0), 4304 }, /* MOVE */
{ CPUFUNC(op_10d8_0), 4312 }, /* MOVE */
{ CPUFUNC(op_10e0_0), 4320 }, /* MOVE */
{ CPUFUNC(op_10e8_0), 4328 }, /* MOVE */
{ CPUFUNC(op_10f0_0), 4336 }, /* MOVE */
{ CPUFUNC(op_10f8_0), 4344 }, /* MOVE */
{ CPUFUNC(op_10f9_0), 4345 }, /* MOVE */
{ CPUFUNC(op_10fa_0), 4346 }, /* MOVE */
{ CPUFUNC(op_10fb_0), 4347 }, /* MOVE */
{ CPUFUNC(op_10fc_0), 4348 }, /* MOVE */
{ CPUFUNC(op_1100_0), 4352 }, /* MOVE */
{ CPUFUNC(op_1110_0), 4368 }, /* MOVE */
{ CPUFUNC(op_1118_0), 4376 }, /* MOVE */
{ CPUFUNC(op_1120_0), 4384 }, /* MOVE */
{ CPUFUNC(op_1128_0), 4392 }, /* MOVE */
{ CPUFUNC(op_1130_0), 4400 }, /* MOVE */
{ CPUFUNC(op_1138_0), 4408 }, /* MOVE */
{ CPUFUNC(op_1139_0), 4409 }, /* MOVE */
{ CPUFUNC(op_113a_0), 4410 }, /* MOVE */
{ CPUFUNC(op_113b_0), 4411 }, /* MOVE */
{ CPUFUNC(op_113c_0), 4412 }, /* MOVE */
{ CPUFUNC(op_1140_0), 4416 }, /* MOVE */
{ CPUFUNC(op_1150_0), 4432 }, /* MOVE */
{ CPUFUNC(op_1158_0), 4440 }, /* MOVE */
{ CPUFUNC(op_1160_0), 4448 }, /* MOVE */
{ CPUFUNC(op_1168_0), 4456 }, /* MOVE */
{ CPUFUNC(op_1170_0), 4464 }, /* MOVE */
{ CPUFUNC(op_1178_0), 4472 }, /* MOVE */
{ CPUFUNC(op_1179_0), 4473 }, /* MOVE */
{ CPUFUNC(op_117a_0), 4474 }, /* MOVE */
{ CPUFUNC(op_117b_0), 4475 }, /* MOVE */
{ CPUFUNC(op_117c_0), 4476 }, /* MOVE */
{ CPUFUNC(op_1180_0), 4480 }, /* MOVE */
{ CPUFUNC(op_1190_0), 4496 }, /* MOVE */
{ CPUFUNC(op_1198_0), 4504 }, /* MOVE */
{ CPUFUNC(op_11a0_0), 4512 }, /* MOVE */
{ CPUFUNC(op_11a8_0), 4520 }, /* MOVE */
{ CPUFUNC(op_11b0_0), 4528 }, /* MOVE */
{ CPUFUNC(op_11b8_0), 4536 }, /* MOVE */
{ CPUFUNC(op_11b9_0), 4537 }, /* MOVE */
{ CPUFUNC(op_11ba_0), 4538 }, /* MOVE */
{ CPUFUNC(op_11bb_0), 4539 }, /* MOVE */
{ CPUFUNC(op_11bc_0), 4540 }, /* MOVE */
{ CPUFUNC(op_11c0_0), 4544 }, /* MOVE */
{ CPUFUNC(op_11d0_0), 4560 }, /* MOVE */
{ CPUFUNC(op_11d8_0), 4568 }, /* MOVE */
{ CPUFUNC(op_11e0_0), 4576 }, /* MOVE */
{ CPUFUNC(op_11e8_0), 4584 }, /* MOVE */
{ CPUFUNC(op_11f0_0), 4592 }, /* MOVE */
{ CPUFUNC(op_11f8_0), 4600 }, /* MOVE */
{ CPUFUNC(op_11f9_0), 4601 }, /* MOVE */
{ CPUFUNC(op_11fa_0), 4602 }, /* MOVE */
{ CPUFUNC(op_11fb_0), 4603 }, /* MOVE */
{ CPUFUNC(op_11fc_0), 4604 }, /* MOVE */
{ CPUFUNC(op_13c0_0), 5056 }, /* MOVE */
{ CPUFUNC(op_13d0_0), 5072 }, /* MOVE */
{ CPUFUNC(op_13d8_0), 5080 }, /* MOVE */
{ CPUFUNC(op_13e0_0), 5088 }, /* MOVE */
{ CPUFUNC(op_13e8_0), 5096 }, /* MOVE */
{ CPUFUNC(op_13f0_0), 5104 }, /* MOVE */
{ CPUFUNC(op_13f8_0), 5112 }, /* MOVE */
{ CPUFUNC(op_13f9_0), 5113 }, /* MOVE */
{ CPUFUNC(op_13fa_0), 5114 }, /* MOVE */
{ CPUFUNC(op_13fb_0), 5115 }, /* MOVE */
{ CPUFUNC(op_13fc_0), 5116 }, /* MOVE */
{ CPUFUNC(op_2000_0), 8192 }, /* MOVE */
{ CPUFUNC(op_2008_0), 8200 }, /* MOVE */
{ CPUFUNC(op_2010_0), 8208 }, /* MOVE */
{ CPUFUNC(op_2018_0), 8216 }, /* MOVE */
{ CPUFUNC(op_2020_0), 8224 }, /* MOVE */
{ CPUFUNC(op_2028_0), 8232 }, /* MOVE */
{ CPUFUNC(op_2030_0), 8240 }, /* MOVE */
{ CPUFUNC(op_2038_0), 8248 }, /* MOVE */
{ CPUFUNC(op_2039_0), 8249 }, /* MOVE */
{ CPUFUNC(op_203a_0), 8250 }, /* MOVE */
{ CPUFUNC(op_203b_0), 8251 }, /* MOVE */
{ CPUFUNC(op_203c_0), 8252 }, /* MOVE */
{ CPUFUNC(op_2040_0), 8256 }, /* MOVEA */
{ CPUFUNC(op_2048_0), 8264 }, /* MOVEA */
{ CPUFUNC(op_2050_0), 8272 }, /* MOVEA */
{ CPUFUNC(op_2058_0), 8280 }, /* MOVEA */
{ CPUFUNC(op_2060_0), 8288 }, /* MOVEA */
{ CPUFUNC(op_2068_0), 8296 }, /* MOVEA */
{ CPUFUNC(op_2070_0), 8304 }, /* MOVEA */
{ CPUFUNC(op_2078_0), 8312 }, /* MOVEA */
{ CPUFUNC(op_2079_0), 8313 }, /* MOVEA */
{ CPUFUNC(op_207a_0), 8314 }, /* MOVEA */
{ CPUFUNC(op_207b_0), 8315 }, /* MOVEA */
{ CPUFUNC(op_207c_0), 8316 }, /* MOVEA */
{ CPUFUNC(op_2080_0), 8320 }, /* MOVE */
{ CPUFUNC(op_2088_0), 8328 }, /* MOVE */
{ CPUFUNC(op_2090_0), 8336 }, /* MOVE */
{ CPUFUNC(op_2098_0), 8344 }, /* MOVE */
{ CPUFUNC(op_20a0_0), 8352 }, /* MOVE */
{ CPUFUNC(op_20a8_0), 8360 }, /* MOVE */
{ CPUFUNC(op_20b0_0), 8368 }, /* MOVE */
{ CPUFUNC(op_20b8_0), 8376 }, /* MOVE */
{ CPUFUNC(op_20b9_0), 8377 }, /* MOVE */
{ CPUFUNC(op_20ba_0), 8378 }, /* MOVE */
{ CPUFUNC(op_20bb_0), 8379 }, /* MOVE */
{ CPUFUNC(op_20bc_0), 8380 }, /* MOVE */
{ CPUFUNC(op_20c0_0), 8384 }, /* MOVE */
{ CPUFUNC(op_20c8_0), 8392 }, /* MOVE */
{ CPUFUNC(op_20d0_0), 8400 }, /* MOVE */
{ CPUFUNC(op_20d8_0), 8408 }, /* MOVE */
{ CPUFUNC(op_20e0_0), 8416 }, /* MOVE */
{ CPUFUNC(op_20e8_0), 8424 }, /* MOVE */
{ CPUFUNC(op_20f0_0), 8432 }, /* MOVE */
{ CPUFUNC(op_20f8_0), 8440 }, /* MOVE */
{ CPUFUNC(op_20f9_0), 8441 }, /* MOVE */
{ CPUFUNC(op_20fa_0), 8442 }, /* MOVE */
{ CPUFUNC(op_20fb_0), 8443 }, /* MOVE */
{ CPUFUNC(op_20fc_0), 8444 }, /* MOVE */
{ CPUFUNC(op_2100_0), 8448 }, /* MOVE */
{ CPUFUNC(op_2108_0), 8456 }, /* MOVE */
{ CPUFUNC(op_2110_0), 8464 }, /* MOVE */
{ CPUFUNC(op_2118_0), 8472 }, /* MOVE */
{ CPUFUNC(op_2120_0), 8480 }, /* MOVE */
{ CPUFUNC(op_2128_0), 8488 }, /* MOVE */
{ CPUFUNC(op_2130_0), 8496 }, /* MOVE */
{ CPUFUNC(op_2138_0), 8504 }, /* MOVE */
{ CPUFUNC(op_2139_0), 8505 }, /* MOVE */
{ CPUFUNC(op_213a_0), 8506 }, /* MOVE */
{ CPUFUNC(op_213b_0), 8507 }, /* MOVE */
{ CPUFUNC(op_213c_0), 8508 }, /* MOVE */
{ CPUFUNC(op_2140_0), 8512 }, /* MOVE */
{ CPUFUNC(op_2148_0), 8520 }, /* MOVE */
{ CPUFUNC(op_2150_0), 8528 }, /* MOVE */
{ CPUFUNC(op_2158_0), 8536 }, /* MOVE */
{ CPUFUNC(op_2160_0), 8544 }, /* MOVE */
{ CPUFUNC(op_2168_0), 8552 }, /* MOVE */
{ CPUFUNC(op_2170_0), 8560 }, /* MOVE */
{ CPUFUNC(op_2178_0), 8568 }, /* MOVE */
{ CPUFUNC(op_2179_0), 8569 }, /* MOVE */
{ CPUFUNC(op_217a_0), 8570 }, /* MOVE */
{ CPUFUNC(op_217b_0), 8571 }, /* MOVE */
{ CPUFUNC(op_217c_0), 8572 }, /* MOVE */
{ CPUFUNC(op_2180_0), 8576 }, /* MOVE */
{ CPUFUNC(op_2188_0), 8584 }, /* MOVE */
{ CPUFUNC(op_2190_0), 8592 }, /* MOVE */
{ CPUFUNC(op_2198_0), 8600 }, /* MOVE */
{ CPUFUNC(op_21a0_0), 8608 }, /* MOVE */
{ CPUFUNC(op_21a8_0), 8616 }, /* MOVE */
{ CPUFUNC(op_21b0_0), 8624 }, /* MOVE */
{ CPUFUNC(op_21b8_0), 8632 }, /* MOVE */
{ CPUFUNC(op_21b9_0), 8633 }, /* MOVE */
{ CPUFUNC(op_21ba_0), 8634 }, /* MOVE */
{ CPUFUNC(op_21bb_0), 8635 }, /* MOVE */
{ CPUFUNC(op_21bc_0), 8636 }, /* MOVE */
{ CPUFUNC(op_21c0_0), 8640 }, /* MOVE */
{ CPUFUNC(op_21c8_0), 8648 }, /* MOVE */
{ CPUFUNC(op_21d0_0), 8656 }, /* MOVE */
{ CPUFUNC(op_21d8_0), 8664 }, /* MOVE */
{ CPUFUNC(op_21e0_0), 8672 }, /* MOVE */
{ CPUFUNC(op_21e8_0), 8680 }, /* MOVE */
{ CPUFUNC(op_21f0_0), 8688 }, /* MOVE */
{ CPUFUNC(op_21f8_0), 8696 }, /* MOVE */
{ CPUFUNC(op_21f9_0), 8697 }, /* MOVE */
{ CPUFUNC(op_21fa_0), 8698 }, /* MOVE */
{ CPUFUNC(op_21fb_0), 8699 }, /* MOVE */
{ CPUFUNC(op_21fc_0), 8700 }, /* MOVE */
{ CPUFUNC(op_23c0_0), 9152 }, /* MOVE */
{ CPUFUNC(op_23c8_0), 9160 }, /* MOVE */
{ CPUFUNC(op_23d0_0), 9168 }, /* MOVE */
{ CPUFUNC(op_23d8_0), 9176 }, /* MOVE */
{ CPUFUNC(op_23e0_0), 9184 }, /* MOVE */
{ CPUFUNC(op_23e8_0), 9192 }, /* MOVE */
{ CPUFUNC(op_23f0_0), 9200 }, /* MOVE */
{ CPUFUNC(op_23f8_0), 9208 }, /* MOVE */
{ CPUFUNC(op_23f9_0), 9209 }, /* MOVE */
{ CPUFUNC(op_23fa_0), 9210 }, /* MOVE */
{ CPUFUNC(op_23fb_0), 9211 }, /* MOVE */
{ CPUFUNC(op_23fc_0), 9212 }, /* MOVE */
{ CPUFUNC(op_3000_0), 12288 }, /* MOVE */
{ CPUFUNC(op_3008_0), 12296 }, /* MOVE */
{ CPUFUNC(op_3010_0), 12304 }, /* MOVE */
{ CPUFUNC(op_3018_0), 12312 }, /* MOVE */
{ CPUFUNC(op_3020_0), 12320 }, /* MOVE */
{ CPUFUNC(op_3028_0), 12328 }, /* MOVE */
{ CPUFUNC(op_3030_0), 12336 }, /* MOVE */
{ CPUFUNC(op_3038_0), 12344 }, /* MOVE */
{ CPUFUNC(op_3039_0), 12345 }, /* MOVE */
{ CPUFUNC(op_303a_0), 12346 }, /* MOVE */
{ CPUFUNC(op_303b_0), 12347 }, /* MOVE */
{ CPUFUNC(op_303c_0), 12348 }, /* MOVE */
{ CPUFUNC(op_3040_0), 12352 }, /* MOVEA */
{ CPUFUNC(op_3048_0), 12360 }, /* MOVEA */
{ CPUFUNC(op_3050_0), 12368 }, /* MOVEA */
{ CPUFUNC(op_3058_0), 12376 }, /* MOVEA */
{ CPUFUNC(op_3060_0), 12384 }, /* MOVEA */
{ CPUFUNC(op_3068_0), 12392 }, /* MOVEA */
{ CPUFUNC(op_3070_0), 12400 }, /* MOVEA */
{ CPUFUNC(op_3078_0), 12408 }, /* MOVEA */
{ CPUFUNC(op_3079_0), 12409 }, /* MOVEA */
{ CPUFUNC(op_307a_0), 12410 }, /* MOVEA */
{ CPUFUNC(op_307b_0), 12411 }, /* MOVEA */
{ CPUFUNC(op_307c_0), 12412 }, /* MOVEA */
{ CPUFUNC(op_3080_0), 12416 }, /* MOVE */
{ CPUFUNC(op_3088_0), 12424 }, /* MOVE */
{ CPUFUNC(op_3090_0), 12432 }, /* MOVE */
{ CPUFUNC(op_3098_0), 12440 }, /* MOVE */
{ CPUFUNC(op_30a0_0), 12448 }, /* MOVE */
{ CPUFUNC(op_30a8_0), 12456 }, /* MOVE */
{ CPUFUNC(op_30b0_0), 12464 }, /* MOVE */
{ CPUFUNC(op_30b8_0), 12472 }, /* MOVE */
{ CPUFUNC(op_30b9_0), 12473 }, /* MOVE */
{ CPUFUNC(op_30ba_0), 12474 }, /* MOVE */
{ CPUFUNC(op_30bb_0), 12475 }, /* MOVE */
{ CPUFUNC(op_30bc_0), 12476 }, /* MOVE */
{ CPUFUNC(op_30c0_0), 12480 }, /* MOVE */
{ CPUFUNC(op_30c8_0), 12488 }, /* MOVE */
{ CPUFUNC(op_30d0_0), 12496 }, /* MOVE */
{ CPUFUNC(op_30d8_0), 12504 }, /* MOVE */
{ CPUFUNC(op_30e0_0), 12512 }, /* MOVE */
{ CPUFUNC(op_30e8_0), 12520 }, /* MOVE */
{ CPUFUNC(op_30f0_0), 12528 }, /* MOVE */
{ CPUFUNC(op_30f8_0), 12536 }, /* MOVE */
{ CPUFUNC(op_30f9_0), 12537 }, /* MOVE */
{ CPUFUNC(op_30fa_0), 12538 }, /* MOVE */
{ CPUFUNC(op_30fb_0), 12539 }, /* MOVE */
{ CPUFUNC(op_30fc_0), 12540 }, /* MOVE */
{ CPUFUNC(op_3100_0), 12544 }, /* MOVE */
{ CPUFUNC(op_3108_0), 12552 }, /* MOVE */
{ CPUFUNC(op_3110_0), 12560 }, /* MOVE */
{ CPUFUNC(op_3118_0), 12568 }, /* MOVE */
{ CPUFUNC(op_3120_0), 12576 }, /* MOVE */
{ CPUFUNC(op_3128_0), 12584 }, /* MOVE */
{ CPUFUNC(op_3130_0), 12592 }, /* MOVE */
{ CPUFUNC(op_3138_0), 12600 }, /* MOVE */
{ CPUFUNC(op_3139_0), 12601 }, /* MOVE */
{ CPUFUNC(op_313a_0), 12602 }, /* MOVE */
{ CPUFUNC(op_313b_0), 12603 }, /* MOVE */
{ CPUFUNC(op_313c_0), 12604 }, /* MOVE */
{ CPUFUNC(op_3140_0), 12608 }, /* MOVE */
{ CPUFUNC(op_3148_0), 12616 }, /* MOVE */
{ CPUFUNC(op_3150_0), 12624 }, /* MOVE */
{ CPUFUNC(op_3158_0), 12632 }, /* MOVE */
{ CPUFUNC(op_3160_0), 12640 }, /* MOVE */
{ CPUFUNC(op_3168_0), 12648 }, /* MOVE */
{ CPUFUNC(op_3170_0), 12656 }, /* MOVE */
{ CPUFUNC(op_3178_0), 12664 }, /* MOVE */
{ CPUFUNC(op_3179_0), 12665 }, /* MOVE */
{ CPUFUNC(op_317a_0), 12666 }, /* MOVE */
{ CPUFUNC(op_317b_0), 12667 }, /* MOVE */
{ CPUFUNC(op_317c_0), 12668 }, /* MOVE */
{ CPUFUNC(op_3180_0), 12672 }, /* MOVE */
{ CPUFUNC(op_3188_0), 12680 }, /* MOVE */
{ CPUFUNC(op_3190_0), 12688 }, /* MOVE */
{ CPUFUNC(op_3198_0), 12696 }, /* MOVE */
{ CPUFUNC(op_31a0_0), 12704 }, /* MOVE */
{ CPUFUNC(op_31a8_0), 12712 }, /* MOVE */
{ CPUFUNC(op_31b0_0), 12720 }, /* MOVE */
{ CPUFUNC(op_31b8_0), 12728 }, /* MOVE */
{ CPUFUNC(op_31b9_0), 12729 }, /* MOVE */
{ CPUFUNC(op_31ba_0), 12730 }, /* MOVE */
{ CPUFUNC(op_31bb_0), 12731 }, /* MOVE */
{ CPUFUNC(op_31bc_0), 12732 }, /* MOVE */
{ CPUFUNC(op_31c0_0), 12736 }, /* MOVE */
{ CPUFUNC(op_31c8_0), 12744 }, /* MOVE */
{ CPUFUNC(op_31d0_0), 12752 }, /* MOVE */
{ CPUFUNC(op_31d8_0), 12760 }, /* MOVE */
{ CPUFUNC(op_31e0_0), 12768 }, /* MOVE */
{ CPUFUNC(op_31e8_0), 12776 }, /* MOVE */
{ CPUFUNC(op_31f0_0), 12784 }, /* MOVE */
{ CPUFUNC(op_31f8_0), 12792 }, /* MOVE */
{ CPUFUNC(op_31f9_0), 12793 }, /* MOVE */
{ CPUFUNC(op_31fa_0), 12794 }, /* MOVE */
{ CPUFUNC(op_31fb_0), 12795 }, /* MOVE */
{ CPUFUNC(op_31fc_0), 12796 }, /* MOVE */
{ CPUFUNC(op_33c0_0), 13248 }, /* MOVE */
{ CPUFUNC(op_33c8_0), 13256 }, /* MOVE */
{ CPUFUNC(op_33d0_0), 13264 }, /* MOVE */
{ CPUFUNC(op_33d8_0), 13272 }, /* MOVE */
{ CPUFUNC(op_33e0_0), 13280 }, /* MOVE */
{ CPUFUNC(op_33e8_0), 13288 }, /* MOVE */
{ CPUFUNC(op_33f0_0), 13296 }, /* MOVE */
{ CPUFUNC(op_33f8_0), 13304 }, /* MOVE */
{ CPUFUNC(op_33f9_0), 13305 }, /* MOVE */
{ CPUFUNC(op_33fa_0), 13306 }, /* MOVE */
{ CPUFUNC(op_33fb_0), 13307 }, /* MOVE */
{ CPUFUNC(op_33fc_0), 13308 }, /* MOVE */
{ CPUFUNC(op_4000_0), 16384 }, /* NEGX */
{ CPUFUNC(op_4010_0), 16400 }, /* NEGX */
{ CPUFUNC(op_4018_0), 16408 }, /* NEGX */
{ CPUFUNC(op_4020_0), 16416 }, /* NEGX */
{ CPUFUNC(op_4028_0), 16424 }, /* NEGX */
{ CPUFUNC(op_4030_0), 16432 }, /* NEGX */
{ CPUFUNC(op_4038_0), 16440 }, /* NEGX */
{ CPUFUNC(op_4039_0), 16441 }, /* NEGX */
{ CPUFUNC(op_4040_0), 16448 }, /* NEGX */
{ CPUFUNC(op_4050_0), 16464 }, /* NEGX */
{ CPUFUNC(op_4058_0), 16472 }, /* NEGX */
{ CPUFUNC(op_4060_0), 16480 }, /* NEGX */
{ CPUFUNC(op_4068_0), 16488 }, /* NEGX */
{ CPUFUNC(op_4070_0), 16496 }, /* NEGX */
{ CPUFUNC(op_4078_0), 16504 }, /* NEGX */
{ CPUFUNC(op_4079_0), 16505 }, /* NEGX */
{ CPUFUNC(op_4080_0), 16512 }, /* NEGX */
{ CPUFUNC(op_4090_0), 16528 }, /* NEGX */
{ CPUFUNC(op_4098_0), 16536 }, /* NEGX */
{ CPUFUNC(op_40a0_0), 16544 }, /* NEGX */
{ CPUFUNC(op_40a8_0), 16552 }, /* NEGX */
{ CPUFUNC(op_40b0_0), 16560 }, /* NEGX */
{ CPUFUNC(op_40b8_0), 16568 }, /* NEGX */
{ CPUFUNC(op_40b9_0), 16569 }, /* NEGX */
{ CPUFUNC(op_40c0_0), 16576 }, /* MVSR2 */
{ CPUFUNC(op_40d0_0), 16592 }, /* MVSR2 */
{ CPUFUNC(op_40d8_0), 16600 }, /* MVSR2 */
{ CPUFUNC(op_40e0_0), 16608 }, /* MVSR2 */
{ CPUFUNC(op_40e8_0), 16616 }, /* MVSR2 */
{ CPUFUNC(op_40f0_0), 16624 }, /* MVSR2 */
{ CPUFUNC(op_40f8_0), 16632 }, /* MVSR2 */
{ CPUFUNC(op_40f9_0), 16633 }, /* MVSR2 */
{ CPUFUNC(op_4100_0), 16640 }, /* CHK */
{ CPUFUNC(op_4110_0), 16656 }, /* CHK */
{ CPUFUNC(op_4118_0), 16664 }, /* CHK */
{ CPUFUNC(op_4120_0), 16672 }, /* CHK */
{ CPUFUNC(op_4128_0), 16680 }, /* CHK */
{ CPUFUNC(op_4130_0), 16688 }, /* CHK */
{ CPUFUNC(op_4138_0), 16696 }, /* CHK */
{ CPUFUNC(op_4139_0), 16697 }, /* CHK */
{ CPUFUNC(op_413a_0), 16698 }, /* CHK */
{ CPUFUNC(op_413b_0), 16699 }, /* CHK */
{ CPUFUNC(op_413c_0), 16700 }, /* CHK */
{ CPUFUNC(op_4180_0), 16768 }, /* CHK */
{ CPUFUNC(op_4190_0), 16784 }, /* CHK */
{ CPUFUNC(op_4198_0), 16792 }, /* CHK */
{ CPUFUNC(op_41a0_0), 16800 }, /* CHK */
{ CPUFUNC(op_41a8_0), 16808 }, /* CHK */
{ CPUFUNC(op_41b0_0), 16816 }, /* CHK */
{ CPUFUNC(op_41b8_0), 16824 }, /* CHK */
{ CPUFUNC(op_41b9_0), 16825 }, /* CHK */
{ CPUFUNC(op_41ba_0), 16826 }, /* CHK */
{ CPUFUNC(op_41bb_0), 16827 }, /* CHK */
{ CPUFUNC(op_41bc_0), 16828 }, /* CHK */
{ CPUFUNC(op_41d0_0), 16848 }, /* LEA */
{ CPUFUNC(op_41e8_0), 16872 }, /* LEA */
{ CPUFUNC(op_41f0_0), 16880 }, /* LEA */
{ CPUFUNC(op_41f8_0), 16888 }, /* LEA */
{ CPUFUNC(op_41f9_0), 16889 }, /* LEA */
{ CPUFUNC(op_41fa_0), 16890 }, /* LEA */
{ CPUFUNC(op_41fb_0), 16891 }, /* LEA */
{ CPUFUNC(op_4200_0), 16896 }, /* CLR */
{ CPUFUNC(op_4210_0), 16912 }, /* CLR */
{ CPUFUNC(op_4218_0), 16920 }, /* CLR */
{ CPUFUNC(op_4220_0), 16928 }, /* CLR */
{ CPUFUNC(op_4228_0), 16936 }, /* CLR */
{ CPUFUNC(op_4230_0), 16944 }, /* CLR */
{ CPUFUNC(op_4238_0), 16952 }, /* CLR */
{ CPUFUNC(op_4239_0), 16953 }, /* CLR */
{ CPUFUNC(op_4240_0), 16960 }, /* CLR */
{ CPUFUNC(op_4250_0), 16976 }, /* CLR */
{ CPUFUNC(op_4258_0), 16984 }, /* CLR */
{ CPUFUNC(op_4260_0), 16992 }, /* CLR */
{ CPUFUNC(op_4268_0), 17000 }, /* CLR */
{ CPUFUNC(op_4270_0), 17008 }, /* CLR */
{ CPUFUNC(op_4278_0), 17016 }, /* CLR */
{ CPUFUNC(op_4279_0), 17017 }, /* CLR */
{ CPUFUNC(op_4280_0), 17024 }, /* CLR */
{ CPUFUNC(op_4290_0), 17040 }, /* CLR */
{ CPUFUNC(op_4298_0), 17048 }, /* CLR */
{ CPUFUNC(op_42a0_0), 17056 }, /* CLR */
{ CPUFUNC(op_42a8_0), 17064 }, /* CLR */
{ CPUFUNC(op_42b0_0), 17072 }, /* CLR */
{ CPUFUNC(op_42b8_0), 17080 }, /* CLR */
{ CPUFUNC(op_42b9_0), 17081 }, /* CLR */
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_42c0_0), 17088 }, /* MVSR2 */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_42d0_0), 17104 }, /* MVSR2 */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_42d8_0), 17112 }, /* MVSR2 */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_42e0_0), 17120 }, /* MVSR2 */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_42e8_0), 17128 }, /* MVSR2 */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_42f0_0), 17136 }, /* MVSR2 */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_42f8_0), 17144 }, /* MVSR2 */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_42f9_0), 17145 }, /* MVSR2 */
#endif
{ CPUFUNC(op_4400_0), 17408 }, /* NEG */
{ CPUFUNC(op_4410_0), 17424 }, /* NEG */
{ CPUFUNC(op_4418_0), 17432 }, /* NEG */
{ CPUFUNC(op_4420_0), 17440 }, /* NEG */
{ CPUFUNC(op_4428_0), 17448 }, /* NEG */
{ CPUFUNC(op_4430_0), 17456 }, /* NEG */
{ CPUFUNC(op_4438_0), 17464 }, /* NEG */
{ CPUFUNC(op_4439_0), 17465 }, /* NEG */
{ CPUFUNC(op_4440_0), 17472 }, /* NEG */
{ CPUFUNC(op_4450_0), 17488 }, /* NEG */
{ CPUFUNC(op_4458_0), 17496 }, /* NEG */
{ CPUFUNC(op_4460_0), 17504 }, /* NEG */
{ CPUFUNC(op_4468_0), 17512 }, /* NEG */
{ CPUFUNC(op_4470_0), 17520 }, /* NEG */
{ CPUFUNC(op_4478_0), 17528 }, /* NEG */
{ CPUFUNC(op_4479_0), 17529 }, /* NEG */
{ CPUFUNC(op_4480_0), 17536 }, /* NEG */
{ CPUFUNC(op_4490_0), 17552 }, /* NEG */
{ CPUFUNC(op_4498_0), 17560 }, /* NEG */
{ CPUFUNC(op_44a0_0), 17568 }, /* NEG */
{ CPUFUNC(op_44a8_0), 17576 }, /* NEG */
{ CPUFUNC(op_44b0_0), 17584 }, /* NEG */
{ CPUFUNC(op_44b8_0), 17592 }, /* NEG */
{ CPUFUNC(op_44b9_0), 17593 }, /* NEG */
{ CPUFUNC(op_44c0_0), 17600 }, /* MV2SR */
{ CPUFUNC(op_44d0_0), 17616 }, /* MV2SR */
{ CPUFUNC(op_44d8_0), 17624 }, /* MV2SR */
{ CPUFUNC(op_44e0_0), 17632 }, /* MV2SR */
{ CPUFUNC(op_44e8_0), 17640 }, /* MV2SR */
{ CPUFUNC(op_44f0_0), 17648 }, /* MV2SR */
{ CPUFUNC(op_44f8_0), 17656 }, /* MV2SR */
{ CPUFUNC(op_44f9_0), 17657 }, /* MV2SR */
{ CPUFUNC(op_44fa_0), 17658 }, /* MV2SR */
{ CPUFUNC(op_44fb_0), 17659 }, /* MV2SR */
{ CPUFUNC(op_44fc_0), 17660 }, /* MV2SR */
{ CPUFUNC(op_4600_0), 17920 }, /* NOT */
{ CPUFUNC(op_4610_0), 17936 }, /* NOT */
{ CPUFUNC(op_4618_0), 17944 }, /* NOT */
{ CPUFUNC(op_4620_0), 17952 }, /* NOT */
{ CPUFUNC(op_4628_0), 17960 }, /* NOT */
{ CPUFUNC(op_4630_0), 17968 }, /* NOT */
{ CPUFUNC(op_4638_0), 17976 }, /* NOT */
{ CPUFUNC(op_4639_0), 17977 }, /* NOT */
{ CPUFUNC(op_4640_0), 17984 }, /* NOT */
{ CPUFUNC(op_4650_0), 18000 }, /* NOT */
{ CPUFUNC(op_4658_0), 18008 }, /* NOT */
{ CPUFUNC(op_4660_0), 18016 }, /* NOT */
{ CPUFUNC(op_4668_0), 18024 }, /* NOT */
{ CPUFUNC(op_4670_0), 18032 }, /* NOT */
{ CPUFUNC(op_4678_0), 18040 }, /* NOT */
{ CPUFUNC(op_4679_0), 18041 }, /* NOT */
{ CPUFUNC(op_4680_0), 18048 }, /* NOT */
{ CPUFUNC(op_4690_0), 18064 }, /* NOT */
{ CPUFUNC(op_4698_0), 18072 }, /* NOT */
{ CPUFUNC(op_46a0_0), 18080 }, /* NOT */
{ CPUFUNC(op_46a8_0), 18088 }, /* NOT */
{ CPUFUNC(op_46b0_0), 18096 }, /* NOT */
{ CPUFUNC(op_46b8_0), 18104 }, /* NOT */
{ CPUFUNC(op_46b9_0), 18105 }, /* NOT */
{ CPUFUNC(op_46c0_0), 18112 }, /* MV2SR */
{ CPUFUNC(op_46d0_0), 18128 }, /* MV2SR */
{ CPUFUNC(op_46d8_0), 18136 }, /* MV2SR */
{ CPUFUNC(op_46e0_0), 18144 }, /* MV2SR */
{ CPUFUNC(op_46e8_0), 18152 }, /* MV2SR */
{ CPUFUNC(op_46f0_0), 18160 }, /* MV2SR */
{ CPUFUNC(op_46f8_0), 18168 }, /* MV2SR */
{ CPUFUNC(op_46f9_0), 18169 }, /* MV2SR */
{ CPUFUNC(op_46fa_0), 18170 }, /* MV2SR */
{ CPUFUNC(op_46fb_0), 18171 }, /* MV2SR */
{ CPUFUNC(op_46fc_0), 18172 }, /* MV2SR */
{ CPUFUNC(op_4800_0), 18432 }, /* NBCD */
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_4808_0), 18440 }, /* LINK */
#endif
{ CPUFUNC(op_4810_0), 18448 }, /* NBCD */
{ CPUFUNC(op_4818_0), 18456 }, /* NBCD */
{ CPUFUNC(op_4820_0), 18464 }, /* NBCD */
{ CPUFUNC(op_4828_0), 18472 }, /* NBCD */
{ CPUFUNC(op_4830_0), 18480 }, /* NBCD */
{ CPUFUNC(op_4838_0), 18488 }, /* NBCD */
{ CPUFUNC(op_4839_0), 18489 }, /* NBCD */
{ CPUFUNC(op_4840_0), 18496 }, /* SWAP */
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_4848_0), 18504 }, /* BKPT */
#endif
{ CPUFUNC(op_4850_0), 18512 }, /* PEA */
{ CPUFUNC(op_4868_0), 18536 }, /* PEA */
{ CPUFUNC(op_4870_0), 18544 }, /* PEA */
{ CPUFUNC(op_4878_0), 18552 }, /* PEA */
{ CPUFUNC(op_4879_0), 18553 }, /* PEA */
{ CPUFUNC(op_487a_0), 18554 }, /* PEA */
{ CPUFUNC(op_487b_0), 18555 }, /* PEA */
{ CPUFUNC(op_4880_0), 18560 }, /* EXT */
{ CPUFUNC(op_4890_0), 18576 }, /* MVMLE */
{ CPUFUNC(op_48a0_0), 18592 }, /* MVMLE */
{ CPUFUNC(op_48a8_0), 18600 }, /* MVMLE */
{ CPUFUNC(op_48b0_0), 18608 }, /* MVMLE */
{ CPUFUNC(op_48b8_0), 18616 }, /* MVMLE */
{ CPUFUNC(op_48b9_0), 18617 }, /* MVMLE */
{ CPUFUNC(op_48c0_0), 18624 }, /* EXT */
{ CPUFUNC(op_48d0_0), 18640 }, /* MVMLE */
{ CPUFUNC(op_48e0_0), 18656 }, /* MVMLE */
{ CPUFUNC(op_48e8_0), 18664 }, /* MVMLE */
{ CPUFUNC(op_48f0_0), 18672 }, /* MVMLE */
{ CPUFUNC(op_48f8_0), 18680 }, /* MVMLE */
{ CPUFUNC(op_48f9_0), 18681 }, /* MVMLE */
{ CPUFUNC(op_49c0_0), 18880 }, /* EXT */
{ CPUFUNC(op_4a00_0), 18944 }, /* TST */
{ CPUFUNC(op_4a10_0), 18960 }, /* TST */
{ CPUFUNC(op_4a18_0), 18968 }, /* TST */
{ CPUFUNC(op_4a20_0), 18976 }, /* TST */
{ CPUFUNC(op_4a28_0), 18984 }, /* TST */
{ CPUFUNC(op_4a30_0), 18992 }, /* TST */
{ CPUFUNC(op_4a38_0), 19000 }, /* TST */
{ CPUFUNC(op_4a39_0), 19001 }, /* TST */
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_4a3a_0), 19002 }, /* TST */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_4a3b_0), 19003 }, /* TST */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_4a3c_0), 19004 }, /* TST */
#endif
{ CPUFUNC(op_4a40_0), 19008 }, /* TST */
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_4a48_0), 19016 }, /* TST */
#endif
{ CPUFUNC(op_4a50_0), 19024 }, /* TST */
{ CPUFUNC(op_4a58_0), 19032 }, /* TST */
{ CPUFUNC(op_4a60_0), 19040 }, /* TST */
{ CPUFUNC(op_4a68_0), 19048 }, /* TST */
{ CPUFUNC(op_4a70_0), 19056 }, /* TST */
{ CPUFUNC(op_4a78_0), 19064 }, /* TST */
{ CPUFUNC(op_4a79_0), 19065 }, /* TST */
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_4a7a_0), 19066 }, /* TST */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_4a7b_0), 19067 }, /* TST */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_4a7c_0), 19068 }, /* TST */
#endif
{ CPUFUNC(op_4a80_0), 19072 }, /* TST */
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_4a88_0), 19080 }, /* TST */
#endif
{ CPUFUNC(op_4a90_0), 19088 }, /* TST */
{ CPUFUNC(op_4a98_0), 19096 }, /* TST */
{ CPUFUNC(op_4aa0_0), 19104 }, /* TST */
{ CPUFUNC(op_4aa8_0), 19112 }, /* TST */
{ CPUFUNC(op_4ab0_0), 19120 }, /* TST */
{ CPUFUNC(op_4ab8_0), 19128 }, /* TST */
{ CPUFUNC(op_4ab9_0), 19129 }, /* TST */
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_4aba_0), 19130 }, /* TST */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_4abb_0), 19131 }, /* TST */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_4abc_0), 19132 }, /* TST */
#endif
{ CPUFUNC(op_4ac0_0), 19136 }, /* TAS */
{ CPUFUNC(op_4ad0_0), 19152 }, /* TAS */
{ CPUFUNC(op_4ad8_0), 19160 }, /* TAS */
{ CPUFUNC(op_4ae0_0), 19168 }, /* TAS */
{ CPUFUNC(op_4ae8_0), 19176 }, /* TAS */
{ CPUFUNC(op_4af0_0), 19184 }, /* TAS */
{ CPUFUNC(op_4af8_0), 19192 }, /* TAS */
{ CPUFUNC(op_4af9_0), 19193 }, /* TAS */
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_4c00_0), 19456 }, /* MULL */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_4c10_0), 19472 }, /* MULL */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_4c18_0), 19480 }, /* MULL */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_4c20_0), 19488 }, /* MULL */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_4c28_0), 19496 }, /* MULL */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_4c30_0), 19504 }, /* MULL */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_4c38_0), 19512 }, /* MULL */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_4c39_0), 19513 }, /* MULL */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_4c3a_0), 19514 }, /* MULL */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_4c3b_0), 19515 }, /* MULL */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_4c3c_0), 19516 }, /* MULL */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_4c40_0), 19520 }, /* DIVL */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_4c50_0), 19536 }, /* DIVL */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_4c58_0), 19544 }, /* DIVL */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_4c60_0), 19552 }, /* DIVL */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_4c68_0), 19560 }, /* DIVL */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_4c70_0), 19568 }, /* DIVL */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_4c78_0), 19576 }, /* DIVL */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_4c79_0), 19577 }, /* DIVL */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_4c7a_0), 19578 }, /* DIVL */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_4c7b_0), 19579 }, /* DIVL */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_4c7c_0), 19580 }, /* DIVL */
#endif
{ CPUFUNC(op_4c90_0), 19600 }, /* MVMEL */
{ CPUFUNC(op_4c98_0), 19608 }, /* MVMEL */
{ CPUFUNC(op_4ca8_0), 19624 }, /* MVMEL */
{ CPUFUNC(op_4cb0_0), 19632 }, /* MVMEL */
{ CPUFUNC(op_4cb8_0), 19640 }, /* MVMEL */
{ CPUFUNC(op_4cb9_0), 19641 }, /* MVMEL */
{ CPUFUNC(op_4cba_0), 19642 }, /* MVMEL */
{ CPUFUNC(op_4cbb_0), 19643 }, /* MVMEL */
{ CPUFUNC(op_4cd0_0), 19664 }, /* MVMEL */
{ CPUFUNC(op_4cd8_0), 19672 }, /* MVMEL */
{ CPUFUNC(op_4ce8_0), 19688 }, /* MVMEL */
{ CPUFUNC(op_4cf0_0), 19696 }, /* MVMEL */
{ CPUFUNC(op_4cf8_0), 19704 }, /* MVMEL */
{ CPUFUNC(op_4cf9_0), 19705 }, /* MVMEL */
{ CPUFUNC(op_4cfa_0), 19706 }, /* MVMEL */
{ CPUFUNC(op_4cfb_0), 19707 }, /* MVMEL */
{ CPUFUNC(op_4e40_0), 20032 }, /* TRAP */
{ CPUFUNC(op_4e50_0), 20048 }, /* LINK */
{ CPUFUNC(op_4e58_0), 20056 }, /* UNLK */
{ CPUFUNC(op_4e60_0), 20064 }, /* MVR2USP */
{ CPUFUNC(op_4e68_0), 20072 }, /* MVUSP2R */
{ CPUFUNC(op_4e70_0), 20080 }, /* RESET */
{ CPUFUNC(op_4e71_0), 20081 }, /* NOP */
{ CPUFUNC(op_4e72_0), 20082 }, /* STOP */
{ CPUFUNC(op_4e73_0), 20083 }, /* RTE */
{ CPUFUNC(op_4e74_0), 20084 }, /* RTD */
{ CPUFUNC(op_4e75_0), 20085 }, /* RTS */
{ CPUFUNC(op_4e76_0), 20086 }, /* TRAPV */
{ CPUFUNC(op_4e77_0), 20087 }, /* RTR */
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_4e7a_0), 20090 }, /* MOVEC2 */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_4e7b_0), 20091 }, /* MOVE2C */
#endif
{ CPUFUNC(op_4e90_0), 20112 }, /* JSR */
{ CPUFUNC(op_4ea8_0), 20136 }, /* JSR */
{ CPUFUNC(op_4eb0_0), 20144 }, /* JSR */
{ CPUFUNC(op_4eb8_0), 20152 }, /* JSR */
{ CPUFUNC(op_4eb9_0), 20153 }, /* JSR */
{ CPUFUNC(op_4eba_0), 20154 }, /* JSR */
{ CPUFUNC(op_4ebb_0), 20155 }, /* JSR */
{ CPUFUNC(op_4ed0_0), 20176 }, /* JMP */
{ CPUFUNC(op_4ee8_0), 20200 }, /* JMP */
{ CPUFUNC(op_4ef0_0), 20208 }, /* JMP */
{ CPUFUNC(op_4ef8_0), 20216 }, /* JMP */
{ CPUFUNC(op_4ef9_0), 20217 }, /* JMP */
{ CPUFUNC(op_4efa_0), 20218 }, /* JMP */
{ CPUFUNC(op_4efb_0), 20219 }, /* JMP */
{ CPUFUNC(op_5000_0), 20480 }, /* ADD */
{ CPUFUNC(op_5010_0), 20496 }, /* ADD */
{ CPUFUNC(op_5018_0), 20504 }, /* ADD */
{ CPUFUNC(op_5020_0), 20512 }, /* ADD */
{ CPUFUNC(op_5028_0), 20520 }, /* ADD */
{ CPUFUNC(op_5030_0), 20528 }, /* ADD */
{ CPUFUNC(op_5038_0), 20536 }, /* ADD */
{ CPUFUNC(op_5039_0), 20537 }, /* ADD */
{ CPUFUNC(op_5040_0), 20544 }, /* ADD */
{ CPUFUNC(op_5048_0), 20552 }, /* ADDA */
{ CPUFUNC(op_5050_0), 20560 }, /* ADD */
{ CPUFUNC(op_5058_0), 20568 }, /* ADD */
{ CPUFUNC(op_5060_0), 20576 }, /* ADD */
{ CPUFUNC(op_5068_0), 20584 }, /* ADD */
{ CPUFUNC(op_5070_0), 20592 }, /* ADD */
{ CPUFUNC(op_5078_0), 20600 }, /* ADD */
{ CPUFUNC(op_5079_0), 20601 }, /* ADD */
{ CPUFUNC(op_5080_0), 20608 }, /* ADD */
{ CPUFUNC(op_5088_0), 20616 }, /* ADDA */
{ CPUFUNC(op_5090_0), 20624 }, /* ADD */
{ CPUFUNC(op_5098_0), 20632 }, /* ADD */
{ CPUFUNC(op_50a0_0), 20640 }, /* ADD */
{ CPUFUNC(op_50a8_0), 20648 }, /* ADD */
{ CPUFUNC(op_50b0_0), 20656 }, /* ADD */
{ CPUFUNC(op_50b8_0), 20664 }, /* ADD */
{ CPUFUNC(op_50b9_0), 20665 }, /* ADD */
{ CPUFUNC(op_50c0_0), 20672 }, /* Scc */
{ CPUFUNC(op_50c8_0), 20680 }, /* DBcc */
{ CPUFUNC(op_50d0_0), 20688 }, /* Scc */
{ CPUFUNC(op_50d8_0), 20696 }, /* Scc */
{ CPUFUNC(op_50e0_0), 20704 }, /* Scc */
{ CPUFUNC(op_50e8_0), 20712 }, /* Scc */
{ CPUFUNC(op_50f0_0), 20720 }, /* Scc */
{ CPUFUNC(op_50f8_0), 20728 }, /* Scc */
{ CPUFUNC(op_50f9_0), 20729 }, /* Scc */
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_50fa_0), 20730 }, /* TRAPcc */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_50fb_0), 20731 }, /* TRAPcc */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_50fc_0), 20732 }, /* TRAPcc */
#endif
{ CPUFUNC(op_5100_0), 20736 }, /* SUB */
{ CPUFUNC(op_5110_0), 20752 }, /* SUB */
{ CPUFUNC(op_5118_0), 20760 }, /* SUB */
{ CPUFUNC(op_5120_0), 20768 }, /* SUB */
{ CPUFUNC(op_5128_0), 20776 }, /* SUB */
{ CPUFUNC(op_5130_0), 20784 }, /* SUB */
{ CPUFUNC(op_5138_0), 20792 }, /* SUB */
{ CPUFUNC(op_5139_0), 20793 }, /* SUB */
{ CPUFUNC(op_5140_0), 20800 }, /* SUB */
{ CPUFUNC(op_5148_0), 20808 }, /* SUBA */
{ CPUFUNC(op_5150_0), 20816 }, /* SUB */
{ CPUFUNC(op_5158_0), 20824 }, /* SUB */
{ CPUFUNC(op_5160_0), 20832 }, /* SUB */
{ CPUFUNC(op_5168_0), 20840 }, /* SUB */
{ CPUFUNC(op_5170_0), 20848 }, /* SUB */
{ CPUFUNC(op_5178_0), 20856 }, /* SUB */
{ CPUFUNC(op_5179_0), 20857 }, /* SUB */
{ CPUFUNC(op_5180_0), 20864 }, /* SUB */
{ CPUFUNC(op_5188_0), 20872 }, /* SUBA */
{ CPUFUNC(op_5190_0), 20880 }, /* SUB */
{ CPUFUNC(op_5198_0), 20888 }, /* SUB */
{ CPUFUNC(op_51a0_0), 20896 }, /* SUB */
{ CPUFUNC(op_51a8_0), 20904 }, /* SUB */
{ CPUFUNC(op_51b0_0), 20912 }, /* SUB */
{ CPUFUNC(op_51b8_0), 20920 }, /* SUB */
{ CPUFUNC(op_51b9_0), 20921 }, /* SUB */
{ CPUFUNC(op_51c0_0), 20928 }, /* Scc */
{ CPUFUNC(op_51c8_0), 20936 }, /* DBcc */
{ CPUFUNC(op_51d0_0), 20944 }, /* Scc */
{ CPUFUNC(op_51d8_0), 20952 }, /* Scc */
{ CPUFUNC(op_51e0_0), 20960 }, /* Scc */
{ CPUFUNC(op_51e8_0), 20968 }, /* Scc */
{ CPUFUNC(op_51f0_0), 20976 }, /* Scc */
{ CPUFUNC(op_51f8_0), 20984 }, /* Scc */
{ CPUFUNC(op_51f9_0), 20985 }, /* Scc */
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_51fa_0), 20986 }, /* TRAPcc */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_51fb_0), 20987 }, /* TRAPcc */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_51fc_0), 20988 }, /* TRAPcc */
#endif
{ CPUFUNC(op_52c0_0), 21184 }, /* Scc */
{ CPUFUNC(op_52c8_0), 21192 }, /* DBcc */
{ CPUFUNC(op_52d0_0), 21200 }, /* Scc */
{ CPUFUNC(op_52d8_0), 21208 }, /* Scc */
{ CPUFUNC(op_52e0_0), 21216 }, /* Scc */
{ CPUFUNC(op_52e8_0), 21224 }, /* Scc */
{ CPUFUNC(op_52f0_0), 21232 }, /* Scc */
{ CPUFUNC(op_52f8_0), 21240 }, /* Scc */
{ CPUFUNC(op_52f9_0), 21241 }, /* Scc */
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_52fa_0), 21242 }, /* TRAPcc */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_52fb_0), 21243 }, /* TRAPcc */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_52fc_0), 21244 }, /* TRAPcc */
#endif
{ CPUFUNC(op_53c0_0), 21440 }, /* Scc */
{ CPUFUNC(op_53c8_0), 21448 }, /* DBcc */
{ CPUFUNC(op_53d0_0), 21456 }, /* Scc */
{ CPUFUNC(op_53d8_0), 21464 }, /* Scc */
{ CPUFUNC(op_53e0_0), 21472 }, /* Scc */
{ CPUFUNC(op_53e8_0), 21480 }, /* Scc */
{ CPUFUNC(op_53f0_0), 21488 }, /* Scc */
{ CPUFUNC(op_53f8_0), 21496 }, /* Scc */
{ CPUFUNC(op_53f9_0), 21497 }, /* Scc */
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_53fa_0), 21498 }, /* TRAPcc */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_53fb_0), 21499 }, /* TRAPcc */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_53fc_0), 21500 }, /* TRAPcc */
#endif
{ CPUFUNC(op_54c0_0), 21696 }, /* Scc */
{ CPUFUNC(op_54c8_0), 21704 }, /* DBcc */
{ CPUFUNC(op_54d0_0), 21712 }, /* Scc */
{ CPUFUNC(op_54d8_0), 21720 }, /* Scc */
{ CPUFUNC(op_54e0_0), 21728 }, /* Scc */
{ CPUFUNC(op_54e8_0), 21736 }, /* Scc */
{ CPUFUNC(op_54f0_0), 21744 }, /* Scc */
{ CPUFUNC(op_54f8_0), 21752 }, /* Scc */
{ CPUFUNC(op_54f9_0), 21753 }, /* Scc */
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_54fa_0), 21754 }, /* TRAPcc */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_54fb_0), 21755 }, /* TRAPcc */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_54fc_0), 21756 }, /* TRAPcc */
#endif
{ CPUFUNC(op_55c0_0), 21952 }, /* Scc */
{ CPUFUNC(op_55c8_0), 21960 }, /* DBcc */
{ CPUFUNC(op_55d0_0), 21968 }, /* Scc */
{ CPUFUNC(op_55d8_0), 21976 }, /* Scc */
{ CPUFUNC(op_55e0_0), 21984 }, /* Scc */
{ CPUFUNC(op_55e8_0), 21992 }, /* Scc */
{ CPUFUNC(op_55f0_0), 22000 }, /* Scc */
{ CPUFUNC(op_55f8_0), 22008 }, /* Scc */
{ CPUFUNC(op_55f9_0), 22009 }, /* Scc */
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_55fa_0), 22010 }, /* TRAPcc */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_55fb_0), 22011 }, /* TRAPcc */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_55fc_0), 22012 }, /* TRAPcc */
#endif
{ CPUFUNC(op_56c0_0), 22208 }, /* Scc */
{ CPUFUNC(op_56c8_0), 22216 }, /* DBcc */
{ CPUFUNC(op_56d0_0), 22224 }, /* Scc */
{ CPUFUNC(op_56d8_0), 22232 }, /* Scc */
{ CPUFUNC(op_56e0_0), 22240 }, /* Scc */
{ CPUFUNC(op_56e8_0), 22248 }, /* Scc */
{ CPUFUNC(op_56f0_0), 22256 }, /* Scc */
{ CPUFUNC(op_56f8_0), 22264 }, /* Scc */
{ CPUFUNC(op_56f9_0), 22265 }, /* Scc */
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_56fa_0), 22266 }, /* TRAPcc */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_56fb_0), 22267 }, /* TRAPcc */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_56fc_0), 22268 }, /* TRAPcc */
#endif
{ CPUFUNC(op_57c0_0), 22464 }, /* Scc */
{ CPUFUNC(op_57c8_0), 22472 }, /* DBcc */
{ CPUFUNC(op_57d0_0), 22480 }, /* Scc */
{ CPUFUNC(op_57d8_0), 22488 }, /* Scc */
{ CPUFUNC(op_57e0_0), 22496 }, /* Scc */
{ CPUFUNC(op_57e8_0), 22504 }, /* Scc */
{ CPUFUNC(op_57f0_0), 22512 }, /* Scc */
{ CPUFUNC(op_57f8_0), 22520 }, /* Scc */
{ CPUFUNC(op_57f9_0), 22521 }, /* Scc */
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_57fa_0), 22522 }, /* TRAPcc */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_57fb_0), 22523 }, /* TRAPcc */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_57fc_0), 22524 }, /* TRAPcc */
#endif
{ CPUFUNC(op_58c0_0), 22720 }, /* Scc */
{ CPUFUNC(op_58c8_0), 22728 }, /* DBcc */
{ CPUFUNC(op_58d0_0), 22736 }, /* Scc */
{ CPUFUNC(op_58d8_0), 22744 }, /* Scc */
{ CPUFUNC(op_58e0_0), 22752 }, /* Scc */
{ CPUFUNC(op_58e8_0), 22760 }, /* Scc */
{ CPUFUNC(op_58f0_0), 22768 }, /* Scc */
{ CPUFUNC(op_58f8_0), 22776 }, /* Scc */
{ CPUFUNC(op_58f9_0), 22777 }, /* Scc */
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_58fa_0), 22778 }, /* TRAPcc */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_58fb_0), 22779 }, /* TRAPcc */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_58fc_0), 22780 }, /* TRAPcc */
#endif
{ CPUFUNC(op_59c0_0), 22976 }, /* Scc */
{ CPUFUNC(op_59c8_0), 22984 }, /* DBcc */
{ CPUFUNC(op_59d0_0), 22992 }, /* Scc */
{ CPUFUNC(op_59d8_0), 23000 }, /* Scc */
{ CPUFUNC(op_59e0_0), 23008 }, /* Scc */
{ CPUFUNC(op_59e8_0), 23016 }, /* Scc */
{ CPUFUNC(op_59f0_0), 23024 }, /* Scc */
{ CPUFUNC(op_59f8_0), 23032 }, /* Scc */
{ CPUFUNC(op_59f9_0), 23033 }, /* Scc */
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_59fa_0), 23034 }, /* TRAPcc */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_59fb_0), 23035 }, /* TRAPcc */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_59fc_0), 23036 }, /* TRAPcc */
#endif
{ CPUFUNC(op_5ac0_0), 23232 }, /* Scc */
{ CPUFUNC(op_5ac8_0), 23240 }, /* DBcc */
{ CPUFUNC(op_5ad0_0), 23248 }, /* Scc */
{ CPUFUNC(op_5ad8_0), 23256 }, /* Scc */
{ CPUFUNC(op_5ae0_0), 23264 }, /* Scc */
{ CPUFUNC(op_5ae8_0), 23272 }, /* Scc */
{ CPUFUNC(op_5af0_0), 23280 }, /* Scc */
{ CPUFUNC(op_5af8_0), 23288 }, /* Scc */
{ CPUFUNC(op_5af9_0), 23289 }, /* Scc */
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_5afa_0), 23290 }, /* TRAPcc */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_5afb_0), 23291 }, /* TRAPcc */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_5afc_0), 23292 }, /* TRAPcc */
#endif
{ CPUFUNC(op_5bc0_0), 23488 }, /* Scc */
{ CPUFUNC(op_5bc8_0), 23496 }, /* DBcc */
{ CPUFUNC(op_5bd0_0), 23504 }, /* Scc */
{ CPUFUNC(op_5bd8_0), 23512 }, /* Scc */
{ CPUFUNC(op_5be0_0), 23520 }, /* Scc */
{ CPUFUNC(op_5be8_0), 23528 }, /* Scc */
{ CPUFUNC(op_5bf0_0), 23536 }, /* Scc */
{ CPUFUNC(op_5bf8_0), 23544 }, /* Scc */
{ CPUFUNC(op_5bf9_0), 23545 }, /* Scc */
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_5bfa_0), 23546 }, /* TRAPcc */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_5bfb_0), 23547 }, /* TRAPcc */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_5bfc_0), 23548 }, /* TRAPcc */
#endif
{ CPUFUNC(op_5cc0_0), 23744 }, /* Scc */
{ CPUFUNC(op_5cc8_0), 23752 }, /* DBcc */
{ CPUFUNC(op_5cd0_0), 23760 }, /* Scc */
{ CPUFUNC(op_5cd8_0), 23768 }, /* Scc */
{ CPUFUNC(op_5ce0_0), 23776 }, /* Scc */
{ CPUFUNC(op_5ce8_0), 23784 }, /* Scc */
{ CPUFUNC(op_5cf0_0), 23792 }, /* Scc */
{ CPUFUNC(op_5cf8_0), 23800 }, /* Scc */
{ CPUFUNC(op_5cf9_0), 23801 }, /* Scc */
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_5cfa_0), 23802 }, /* TRAPcc */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_5cfb_0), 23803 }, /* TRAPcc */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_5cfc_0), 23804 }, /* TRAPcc */
#endif
{ CPUFUNC(op_5dc0_0), 24000 }, /* Scc */
{ CPUFUNC(op_5dc8_0), 24008 }, /* DBcc */
{ CPUFUNC(op_5dd0_0), 24016 }, /* Scc */
{ CPUFUNC(op_5dd8_0), 24024 }, /* Scc */
{ CPUFUNC(op_5de0_0), 24032 }, /* Scc */
{ CPUFUNC(op_5de8_0), 24040 }, /* Scc */
{ CPUFUNC(op_5df0_0), 24048 }, /* Scc */
{ CPUFUNC(op_5df8_0), 24056 }, /* Scc */
{ CPUFUNC(op_5df9_0), 24057 }, /* Scc */
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_5dfa_0), 24058 }, /* TRAPcc */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_5dfb_0), 24059 }, /* TRAPcc */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_5dfc_0), 24060 }, /* TRAPcc */
#endif
{ CPUFUNC(op_5ec0_0), 24256 }, /* Scc */
{ CPUFUNC(op_5ec8_0), 24264 }, /* DBcc */
{ CPUFUNC(op_5ed0_0), 24272 }, /* Scc */
{ CPUFUNC(op_5ed8_0), 24280 }, /* Scc */
{ CPUFUNC(op_5ee0_0), 24288 }, /* Scc */
{ CPUFUNC(op_5ee8_0), 24296 }, /* Scc */
{ CPUFUNC(op_5ef0_0), 24304 }, /* Scc */
{ CPUFUNC(op_5ef8_0), 24312 }, /* Scc */
{ CPUFUNC(op_5ef9_0), 24313 }, /* Scc */
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_5efa_0), 24314 }, /* TRAPcc */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_5efb_0), 24315 }, /* TRAPcc */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_5efc_0), 24316 }, /* TRAPcc */
#endif
{ CPUFUNC(op_5fc0_0), 24512 }, /* Scc */
{ CPUFUNC(op_5fc8_0), 24520 }, /* DBcc */
{ CPUFUNC(op_5fd0_0), 24528 }, /* Scc */
{ CPUFUNC(op_5fd8_0), 24536 }, /* Scc */
{ CPUFUNC(op_5fe0_0), 24544 }, /* Scc */
{ CPUFUNC(op_5fe8_0), 24552 }, /* Scc */
{ CPUFUNC(op_5ff0_0), 24560 }, /* Scc */
{ CPUFUNC(op_5ff8_0), 24568 }, /* Scc */
{ CPUFUNC(op_5ff9_0), 24569 }, /* Scc */
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_5ffa_0), 24570 }, /* TRAPcc */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_5ffb_0), 24571 }, /* TRAPcc */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_5ffc_0), 24572 }, /* TRAPcc */
#endif
{ CPUFUNC(op_6000_0), 24576 }, /* Bcc */
{ CPUFUNC(op_6001_0), 24577 }, /* Bcc */
{ CPUFUNC(op_60ff_0), 24831 }, /* Bcc */
{ CPUFUNC(op_6100_0), 24832 }, /* BSR */
{ CPUFUNC(op_6101_0), 24833 }, /* BSR */
{ CPUFUNC(op_61ff_0), 25087 }, /* BSR */
{ CPUFUNC(op_6200_0), 25088 }, /* Bcc */
{ CPUFUNC(op_6201_0), 25089 }, /* Bcc */
{ CPUFUNC(op_62ff_0), 25343 }, /* Bcc */
{ CPUFUNC(op_6300_0), 25344 }, /* Bcc */
{ CPUFUNC(op_6301_0), 25345 }, /* Bcc */
{ CPUFUNC(op_63ff_0), 25599 }, /* Bcc */
{ CPUFUNC(op_6400_0), 25600 }, /* Bcc */
{ CPUFUNC(op_6401_0), 25601 }, /* Bcc */
{ CPUFUNC(op_64ff_0), 25855 }, /* Bcc */
{ CPUFUNC(op_6500_0), 25856 }, /* Bcc */
{ CPUFUNC(op_6501_0), 25857 }, /* Bcc */
{ CPUFUNC(op_65ff_0), 26111 }, /* Bcc */
{ CPUFUNC(op_6600_0), 26112 }, /* Bcc */
{ CPUFUNC(op_6601_0), 26113 }, /* Bcc */
{ CPUFUNC(op_66ff_0), 26367 }, /* Bcc */
{ CPUFUNC(op_6700_0), 26368 }, /* Bcc */
{ CPUFUNC(op_6701_0), 26369 }, /* Bcc */
{ CPUFUNC(op_67ff_0), 26623 }, /* Bcc */
{ CPUFUNC(op_6800_0), 26624 }, /* Bcc */
{ CPUFUNC(op_6801_0), 26625 }, /* Bcc */
{ CPUFUNC(op_68ff_0), 26879 }, /* Bcc */
{ CPUFUNC(op_6900_0), 26880 }, /* Bcc */
{ CPUFUNC(op_6901_0), 26881 }, /* Bcc */
{ CPUFUNC(op_69ff_0), 27135 }, /* Bcc */
{ CPUFUNC(op_6a00_0), 27136 }, /* Bcc */
{ CPUFUNC(op_6a01_0), 27137 }, /* Bcc */
{ CPUFUNC(op_6aff_0), 27391 }, /* Bcc */
{ CPUFUNC(op_6b00_0), 27392 }, /* Bcc */
{ CPUFUNC(op_6b01_0), 27393 }, /* Bcc */
{ CPUFUNC(op_6bff_0), 27647 }, /* Bcc */
{ CPUFUNC(op_6c00_0), 27648 }, /* Bcc */
{ CPUFUNC(op_6c01_0), 27649 }, /* Bcc */
{ CPUFUNC(op_6cff_0), 27903 }, /* Bcc */
{ CPUFUNC(op_6d00_0), 27904 }, /* Bcc */
{ CPUFUNC(op_6d01_0), 27905 }, /* Bcc */
{ CPUFUNC(op_6dff_0), 28159 }, /* Bcc */
{ CPUFUNC(op_6e00_0), 28160 }, /* Bcc */
{ CPUFUNC(op_6e01_0), 28161 }, /* Bcc */
{ CPUFUNC(op_6eff_0), 28415 }, /* Bcc */
{ CPUFUNC(op_6f00_0), 28416 }, /* Bcc */
{ CPUFUNC(op_6f01_0), 28417 }, /* Bcc */
{ CPUFUNC(op_6fff_0), 28671 }, /* Bcc */
{ CPUFUNC(op_7000_0), 28672 }, /* MOVE */
{ CPUFUNC(op_8000_0), 32768 }, /* OR */
{ CPUFUNC(op_8010_0), 32784 }, /* OR */
{ CPUFUNC(op_8018_0), 32792 }, /* OR */
{ CPUFUNC(op_8020_0), 32800 }, /* OR */
{ CPUFUNC(op_8028_0), 32808 }, /* OR */
{ CPUFUNC(op_8030_0), 32816 }, /* OR */
{ CPUFUNC(op_8038_0), 32824 }, /* OR */
{ CPUFUNC(op_8039_0), 32825 }, /* OR */
{ CPUFUNC(op_803a_0), 32826 }, /* OR */
{ CPUFUNC(op_803b_0), 32827 }, /* OR */
{ CPUFUNC(op_803c_0), 32828 }, /* OR */
{ CPUFUNC(op_8040_0), 32832 }, /* OR */
{ CPUFUNC(op_8050_0), 32848 }, /* OR */
{ CPUFUNC(op_8058_0), 32856 }, /* OR */
{ CPUFUNC(op_8060_0), 32864 }, /* OR */
{ CPUFUNC(op_8068_0), 32872 }, /* OR */
{ CPUFUNC(op_8070_0), 32880 }, /* OR */
{ CPUFUNC(op_8078_0), 32888 }, /* OR */
{ CPUFUNC(op_8079_0), 32889 }, /* OR */
{ CPUFUNC(op_807a_0), 32890 }, /* OR */
{ CPUFUNC(op_807b_0), 32891 }, /* OR */
{ CPUFUNC(op_807c_0), 32892 }, /* OR */
{ CPUFUNC(op_8080_0), 32896 }, /* OR */
{ CPUFUNC(op_8090_0), 32912 }, /* OR */
{ CPUFUNC(op_8098_0), 32920 }, /* OR */
{ CPUFUNC(op_80a0_0), 32928 }, /* OR */
{ CPUFUNC(op_80a8_0), 32936 }, /* OR */
{ CPUFUNC(op_80b0_0), 32944 }, /* OR */
{ CPUFUNC(op_80b8_0), 32952 }, /* OR */
{ CPUFUNC(op_80b9_0), 32953 }, /* OR */
{ CPUFUNC(op_80ba_0), 32954 }, /* OR */
{ CPUFUNC(op_80bb_0), 32955 }, /* OR */
{ CPUFUNC(op_80bc_0), 32956 }, /* OR */
{ CPUFUNC(op_80c0_0), 32960 }, /* DIVU */
{ CPUFUNC(op_80d0_0), 32976 }, /* DIVU */
{ CPUFUNC(op_80d8_0), 32984 }, /* DIVU */
{ CPUFUNC(op_80e0_0), 32992 }, /* DIVU */
{ CPUFUNC(op_80e8_0), 33000 }, /* DIVU */
{ CPUFUNC(op_80f0_0), 33008 }, /* DIVU */
{ CPUFUNC(op_80f8_0), 33016 }, /* DIVU */
{ CPUFUNC(op_80f9_0), 33017 }, /* DIVU */
{ CPUFUNC(op_80fa_0), 33018 }, /* DIVU */
{ CPUFUNC(op_80fb_0), 33019 }, /* DIVU */
{ CPUFUNC(op_80fc_0), 33020 }, /* DIVU */
{ CPUFUNC(op_8100_0), 33024 }, /* SBCD */
{ CPUFUNC(op_8108_0), 33032 }, /* SBCD */
{ CPUFUNC(op_8110_0), 33040 }, /* OR */
{ CPUFUNC(op_8118_0), 33048 }, /* OR */
{ CPUFUNC(op_8120_0), 33056 }, /* OR */
{ CPUFUNC(op_8128_0), 33064 }, /* OR */
{ CPUFUNC(op_8130_0), 33072 }, /* OR */
{ CPUFUNC(op_8138_0), 33080 }, /* OR */
{ CPUFUNC(op_8139_0), 33081 }, /* OR */
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_8140_0), 33088 }, /* PACK */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_8148_0), 33096 }, /* PACK */
#endif
{ CPUFUNC(op_8150_0), 33104 }, /* OR */
{ CPUFUNC(op_8158_0), 33112 }, /* OR */
{ CPUFUNC(op_8160_0), 33120 }, /* OR */
{ CPUFUNC(op_8168_0), 33128 }, /* OR */
{ CPUFUNC(op_8170_0), 33136 }, /* OR */
{ CPUFUNC(op_8178_0), 33144 }, /* OR */
{ CPUFUNC(op_8179_0), 33145 }, /* OR */
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_8180_0), 33152 }, /* UNPK */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_8188_0), 33160 }, /* UNPK */
#endif
{ CPUFUNC(op_8190_0), 33168 }, /* OR */
{ CPUFUNC(op_8198_0), 33176 }, /* OR */
{ CPUFUNC(op_81a0_0), 33184 }, /* OR */
{ CPUFUNC(op_81a8_0), 33192 }, /* OR */
{ CPUFUNC(op_81b0_0), 33200 }, /* OR */
{ CPUFUNC(op_81b8_0), 33208 }, /* OR */
{ CPUFUNC(op_81b9_0), 33209 }, /* OR */
{ CPUFUNC(op_81c0_0), 33216 }, /* DIVS */
{ CPUFUNC(op_81d0_0), 33232 }, /* DIVS */
{ CPUFUNC(op_81d8_0), 33240 }, /* DIVS */
{ CPUFUNC(op_81e0_0), 33248 }, /* DIVS */
{ CPUFUNC(op_81e8_0), 33256 }, /* DIVS */
{ CPUFUNC(op_81f0_0), 33264 }, /* DIVS */
{ CPUFUNC(op_81f8_0), 33272 }, /* DIVS */
{ CPUFUNC(op_81f9_0), 33273 }, /* DIVS */
{ CPUFUNC(op_81fa_0), 33274 }, /* DIVS */
{ CPUFUNC(op_81fb_0), 33275 }, /* DIVS */
{ CPUFUNC(op_81fc_0), 33276 }, /* DIVS */
{ CPUFUNC(op_9000_0), 36864 }, /* SUB */
{ CPUFUNC(op_9010_0), 36880 }, /* SUB */
{ CPUFUNC(op_9018_0), 36888 }, /* SUB */
{ CPUFUNC(op_9020_0), 36896 }, /* SUB */
{ CPUFUNC(op_9028_0), 36904 }, /* SUB */
{ CPUFUNC(op_9030_0), 36912 }, /* SUB */
{ CPUFUNC(op_9038_0), 36920 }, /* SUB */
{ CPUFUNC(op_9039_0), 36921 }, /* SUB */
{ CPUFUNC(op_903a_0), 36922 }, /* SUB */
{ CPUFUNC(op_903b_0), 36923 }, /* SUB */
{ CPUFUNC(op_903c_0), 36924 }, /* SUB */
{ CPUFUNC(op_9040_0), 36928 }, /* SUB */
{ CPUFUNC(op_9048_0), 36936 }, /* SUB */
{ CPUFUNC(op_9050_0), 36944 }, /* SUB */
{ CPUFUNC(op_9058_0), 36952 }, /* SUB */
{ CPUFUNC(op_9060_0), 36960 }, /* SUB */
{ CPUFUNC(op_9068_0), 36968 }, /* SUB */
{ CPUFUNC(op_9070_0), 36976 }, /* SUB */
{ CPUFUNC(op_9078_0), 36984 }, /* SUB */
{ CPUFUNC(op_9079_0), 36985 }, /* SUB */
{ CPUFUNC(op_907a_0), 36986 }, /* SUB */
{ CPUFUNC(op_907b_0), 36987 }, /* SUB */
{ CPUFUNC(op_907c_0), 36988 }, /* SUB */
{ CPUFUNC(op_9080_0), 36992 }, /* SUB */
{ CPUFUNC(op_9088_0), 37000 }, /* SUB */
{ CPUFUNC(op_9090_0), 37008 }, /* SUB */
{ CPUFUNC(op_9098_0), 37016 }, /* SUB */
{ CPUFUNC(op_90a0_0), 37024 }, /* SUB */
{ CPUFUNC(op_90a8_0), 37032 }, /* SUB */
{ CPUFUNC(op_90b0_0), 37040 }, /* SUB */
{ CPUFUNC(op_90b8_0), 37048 }, /* SUB */
{ CPUFUNC(op_90b9_0), 37049 }, /* SUB */
{ CPUFUNC(op_90ba_0), 37050 }, /* SUB */
{ CPUFUNC(op_90bb_0), 37051 }, /* SUB */
{ CPUFUNC(op_90bc_0), 37052 }, /* SUB */
{ CPUFUNC(op_90c0_0), 37056 }, /* SUBA */
{ CPUFUNC(op_90c8_0), 37064 }, /* SUBA */
{ CPUFUNC(op_90d0_0), 37072 }, /* SUBA */
{ CPUFUNC(op_90d8_0), 37080 }, /* SUBA */
{ CPUFUNC(op_90e0_0), 37088 }, /* SUBA */
{ CPUFUNC(op_90e8_0), 37096 }, /* SUBA */
{ CPUFUNC(op_90f0_0), 37104 }, /* SUBA */
{ CPUFUNC(op_90f8_0), 37112 }, /* SUBA */
{ CPUFUNC(op_90f9_0), 37113 }, /* SUBA */
{ CPUFUNC(op_90fa_0), 37114 }, /* SUBA */
{ CPUFUNC(op_90fb_0), 37115 }, /* SUBA */
{ CPUFUNC(op_90fc_0), 37116 }, /* SUBA */
{ CPUFUNC(op_9100_0), 37120 }, /* SUBX */
{ CPUFUNC(op_9108_0), 37128 }, /* SUBX */
{ CPUFUNC(op_9110_0), 37136 }, /* SUB */
{ CPUFUNC(op_9118_0), 37144 }, /* SUB */
{ CPUFUNC(op_9120_0), 37152 }, /* SUB */
{ CPUFUNC(op_9128_0), 37160 }, /* SUB */
{ CPUFUNC(op_9130_0), 37168 }, /* SUB */
{ CPUFUNC(op_9138_0), 37176 }, /* SUB */
{ CPUFUNC(op_9139_0), 37177 }, /* SUB */
{ CPUFUNC(op_9140_0), 37184 }, /* SUBX */
{ CPUFUNC(op_9148_0), 37192 }, /* SUBX */
{ CPUFUNC(op_9150_0), 37200 }, /* SUB */
{ CPUFUNC(op_9158_0), 37208 }, /* SUB */
{ CPUFUNC(op_9160_0), 37216 }, /* SUB */
{ CPUFUNC(op_9168_0), 37224 }, /* SUB */
{ CPUFUNC(op_9170_0), 37232 }, /* SUB */
{ CPUFUNC(op_9178_0), 37240 }, /* SUB */
{ CPUFUNC(op_9179_0), 37241 }, /* SUB */
{ CPUFUNC(op_9180_0), 37248 }, /* SUBX */
{ CPUFUNC(op_9188_0), 37256 }, /* SUBX */
{ CPUFUNC(op_9190_0), 37264 }, /* SUB */
{ CPUFUNC(op_9198_0), 37272 }, /* SUB */
{ CPUFUNC(op_91a0_0), 37280 }, /* SUB */
{ CPUFUNC(op_91a8_0), 37288 }, /* SUB */
{ CPUFUNC(op_91b0_0), 37296 }, /* SUB */
{ CPUFUNC(op_91b8_0), 37304 }, /* SUB */
{ CPUFUNC(op_91b9_0), 37305 }, /* SUB */
{ CPUFUNC(op_91c0_0), 37312 }, /* SUBA */
{ CPUFUNC(op_91c8_0), 37320 }, /* SUBA */
{ CPUFUNC(op_91d0_0), 37328 }, /* SUBA */
{ CPUFUNC(op_91d8_0), 37336 }, /* SUBA */
{ CPUFUNC(op_91e0_0), 37344 }, /* SUBA */
{ CPUFUNC(op_91e8_0), 37352 }, /* SUBA */
{ CPUFUNC(op_91f0_0), 37360 }, /* SUBA */
{ CPUFUNC(op_91f8_0), 37368 }, /* SUBA */
{ CPUFUNC(op_91f9_0), 37369 }, /* SUBA */
{ CPUFUNC(op_91fa_0), 37370 }, /* SUBA */
{ CPUFUNC(op_91fb_0), 37371 }, /* SUBA */
{ CPUFUNC(op_91fc_0), 37372 }, /* SUBA */
{ CPUFUNC(op_b000_0), 45056 }, /* CMP */
{ CPUFUNC(op_b010_0), 45072 }, /* CMP */
{ CPUFUNC(op_b018_0), 45080 }, /* CMP */
{ CPUFUNC(op_b020_0), 45088 }, /* CMP */
{ CPUFUNC(op_b028_0), 45096 }, /* CMP */
{ CPUFUNC(op_b030_0), 45104 }, /* CMP */
{ CPUFUNC(op_b038_0), 45112 }, /* CMP */
{ CPUFUNC(op_b039_0), 45113 }, /* CMP */
{ CPUFUNC(op_b03a_0), 45114 }, /* CMP */
{ CPUFUNC(op_b03b_0), 45115 }, /* CMP */
{ CPUFUNC(op_b03c_0), 45116 }, /* CMP */
{ CPUFUNC(op_b040_0), 45120 }, /* CMP */
{ CPUFUNC(op_b048_0), 45128 }, /* CMP */
{ CPUFUNC(op_b050_0), 45136 }, /* CMP */
{ CPUFUNC(op_b058_0), 45144 }, /* CMP */
{ CPUFUNC(op_b060_0), 45152 }, /* CMP */
{ CPUFUNC(op_b068_0), 45160 }, /* CMP */
{ CPUFUNC(op_b070_0), 45168 }, /* CMP */
{ CPUFUNC(op_b078_0), 45176 }, /* CMP */
{ CPUFUNC(op_b079_0), 45177 }, /* CMP */
{ CPUFUNC(op_b07a_0), 45178 }, /* CMP */
{ CPUFUNC(op_b07b_0), 45179 }, /* CMP */
{ CPUFUNC(op_b07c_0), 45180 }, /* CMP */
{ CPUFUNC(op_b080_0), 45184 }, /* CMP */
{ CPUFUNC(op_b088_0), 45192 }, /* CMP */
{ CPUFUNC(op_b090_0), 45200 }, /* CMP */
{ CPUFUNC(op_b098_0), 45208 }, /* CMP */
{ CPUFUNC(op_b0a0_0), 45216 }, /* CMP */
{ CPUFUNC(op_b0a8_0), 45224 }, /* CMP */
{ CPUFUNC(op_b0b0_0), 45232 }, /* CMP */
{ CPUFUNC(op_b0b8_0), 45240 }, /* CMP */
{ CPUFUNC(op_b0b9_0), 45241 }, /* CMP */
{ CPUFUNC(op_b0ba_0), 45242 }, /* CMP */
{ CPUFUNC(op_b0bb_0), 45243 }, /* CMP */
{ CPUFUNC(op_b0bc_0), 45244 }, /* CMP */
{ CPUFUNC(op_b0c0_0), 45248 }, /* CMPA */
{ CPUFUNC(op_b0c8_0), 45256 }, /* CMPA */
{ CPUFUNC(op_b0d0_0), 45264 }, /* CMPA */
{ CPUFUNC(op_b0d8_0), 45272 }, /* CMPA */
{ CPUFUNC(op_b0e0_0), 45280 }, /* CMPA */
{ CPUFUNC(op_b0e8_0), 45288 }, /* CMPA */
{ CPUFUNC(op_b0f0_0), 45296 }, /* CMPA */
{ CPUFUNC(op_b0f8_0), 45304 }, /* CMPA */
{ CPUFUNC(op_b0f9_0), 45305 }, /* CMPA */
{ CPUFUNC(op_b0fa_0), 45306 }, /* CMPA */
{ CPUFUNC(op_b0fb_0), 45307 }, /* CMPA */
{ CPUFUNC(op_b0fc_0), 45308 }, /* CMPA */
{ CPUFUNC(op_b100_0), 45312 }, /* EOR */
{ CPUFUNC(op_b108_0), 45320 }, /* CMPM */
{ CPUFUNC(op_b110_0), 45328 }, /* EOR */
{ CPUFUNC(op_b118_0), 45336 }, /* EOR */
{ CPUFUNC(op_b120_0), 45344 }, /* EOR */
{ CPUFUNC(op_b128_0), 45352 }, /* EOR */
{ CPUFUNC(op_b130_0), 45360 }, /* EOR */
{ CPUFUNC(op_b138_0), 45368 }, /* EOR */
{ CPUFUNC(op_b139_0), 45369 }, /* EOR */
{ CPUFUNC(op_b140_0), 45376 }, /* EOR */
{ CPUFUNC(op_b148_0), 45384 }, /* CMPM */
{ CPUFUNC(op_b150_0), 45392 }, /* EOR */
{ CPUFUNC(op_b158_0), 45400 }, /* EOR */
{ CPUFUNC(op_b160_0), 45408 }, /* EOR */
{ CPUFUNC(op_b168_0), 45416 }, /* EOR */
{ CPUFUNC(op_b170_0), 45424 }, /* EOR */
{ CPUFUNC(op_b178_0), 45432 }, /* EOR */
{ CPUFUNC(op_b179_0), 45433 }, /* EOR */
{ CPUFUNC(op_b180_0), 45440 }, /* EOR */
{ CPUFUNC(op_b188_0), 45448 }, /* CMPM */
{ CPUFUNC(op_b190_0), 45456 }, /* EOR */
{ CPUFUNC(op_b198_0), 45464 }, /* EOR */
{ CPUFUNC(op_b1a0_0), 45472 }, /* EOR */
{ CPUFUNC(op_b1a8_0), 45480 }, /* EOR */
{ CPUFUNC(op_b1b0_0), 45488 }, /* EOR */
{ CPUFUNC(op_b1b8_0), 45496 }, /* EOR */
{ CPUFUNC(op_b1b9_0), 45497 }, /* EOR */
{ CPUFUNC(op_b1c0_0), 45504 }, /* CMPA */
{ CPUFUNC(op_b1c8_0), 45512 }, /* CMPA */
{ CPUFUNC(op_b1d0_0), 45520 }, /* CMPA */
{ CPUFUNC(op_b1d8_0), 45528 }, /* CMPA */
{ CPUFUNC(op_b1e0_0), 45536 }, /* CMPA */
{ CPUFUNC(op_b1e8_0), 45544 }, /* CMPA */
{ CPUFUNC(op_b1f0_0), 45552 }, /* CMPA */
{ CPUFUNC(op_b1f8_0), 45560 }, /* CMPA */
{ CPUFUNC(op_b1f9_0), 45561 }, /* CMPA */
{ CPUFUNC(op_b1fa_0), 45562 }, /* CMPA */
{ CPUFUNC(op_b1fb_0), 45563 }, /* CMPA */
{ CPUFUNC(op_b1fc_0), 45564 }, /* CMPA */
{ CPUFUNC(op_c000_0), 49152 }, /* AND */
{ CPUFUNC(op_c010_0), 49168 }, /* AND */
{ CPUFUNC(op_c018_0), 49176 }, /* AND */
{ CPUFUNC(op_c020_0), 49184 }, /* AND */
{ CPUFUNC(op_c028_0), 49192 }, /* AND */
{ CPUFUNC(op_c030_0), 49200 }, /* AND */
{ CPUFUNC(op_c038_0), 49208 }, /* AND */
{ CPUFUNC(op_c039_0), 49209 }, /* AND */
{ CPUFUNC(op_c03a_0), 49210 }, /* AND */
{ CPUFUNC(op_c03b_0), 49211 }, /* AND */
{ CPUFUNC(op_c03c_0), 49212 }, /* AND */
{ CPUFUNC(op_c040_0), 49216 }, /* AND */
{ CPUFUNC(op_c050_0), 49232 }, /* AND */
{ CPUFUNC(op_c058_0), 49240 }, /* AND */
{ CPUFUNC(op_c060_0), 49248 }, /* AND */
{ CPUFUNC(op_c068_0), 49256 }, /* AND */
{ CPUFUNC(op_c070_0), 49264 }, /* AND */
{ CPUFUNC(op_c078_0), 49272 }, /* AND */
{ CPUFUNC(op_c079_0), 49273 }, /* AND */
{ CPUFUNC(op_c07a_0), 49274 }, /* AND */
{ CPUFUNC(op_c07b_0), 49275 }, /* AND */
{ CPUFUNC(op_c07c_0), 49276 }, /* AND */
{ CPUFUNC(op_c080_0), 49280 }, /* AND */
{ CPUFUNC(op_c090_0), 49296 }, /* AND */
{ CPUFUNC(op_c098_0), 49304 }, /* AND */
{ CPUFUNC(op_c0a0_0), 49312 }, /* AND */
{ CPUFUNC(op_c0a8_0), 49320 }, /* AND */
{ CPUFUNC(op_c0b0_0), 49328 }, /* AND */
{ CPUFUNC(op_c0b8_0), 49336 }, /* AND */
{ CPUFUNC(op_c0b9_0), 49337 }, /* AND */
{ CPUFUNC(op_c0ba_0), 49338 }, /* AND */
{ CPUFUNC(op_c0bb_0), 49339 }, /* AND */
{ CPUFUNC(op_c0bc_0), 49340 }, /* AND */
{ CPUFUNC(op_c0c0_0), 49344 }, /* MULU */
{ CPUFUNC(op_c0d0_0), 49360 }, /* MULU */
{ CPUFUNC(op_c0d8_0), 49368 }, /* MULU */
{ CPUFUNC(op_c0e0_0), 49376 }, /* MULU */
{ CPUFUNC(op_c0e8_0), 49384 }, /* MULU */
{ CPUFUNC(op_c0f0_0), 49392 }, /* MULU */
{ CPUFUNC(op_c0f8_0), 49400 }, /* MULU */
{ CPUFUNC(op_c0f9_0), 49401 }, /* MULU */
{ CPUFUNC(op_c0fa_0), 49402 }, /* MULU */
{ CPUFUNC(op_c0fb_0), 49403 }, /* MULU */
{ CPUFUNC(op_c0fc_0), 49404 }, /* MULU */
{ CPUFUNC(op_c100_0), 49408 }, /* ABCD */
{ CPUFUNC(op_c108_0), 49416 }, /* ABCD */
{ CPUFUNC(op_c110_0), 49424 }, /* AND */
{ CPUFUNC(op_c118_0), 49432 }, /* AND */
{ CPUFUNC(op_c120_0), 49440 }, /* AND */
{ CPUFUNC(op_c128_0), 49448 }, /* AND */
{ CPUFUNC(op_c130_0), 49456 }, /* AND */
{ CPUFUNC(op_c138_0), 49464 }, /* AND */
{ CPUFUNC(op_c139_0), 49465 }, /* AND */
{ CPUFUNC(op_c140_0), 49472 }, /* EXG */
{ CPUFUNC(op_c148_0), 49480 }, /* EXG */
{ CPUFUNC(op_c150_0), 49488 }, /* AND */
{ CPUFUNC(op_c158_0), 49496 }, /* AND */
{ CPUFUNC(op_c160_0), 49504 }, /* AND */
{ CPUFUNC(op_c168_0), 49512 }, /* AND */
{ CPUFUNC(op_c170_0), 49520 }, /* AND */
{ CPUFUNC(op_c178_0), 49528 }, /* AND */
{ CPUFUNC(op_c179_0), 49529 }, /* AND */
{ CPUFUNC(op_c188_0), 49544 }, /* EXG */
{ CPUFUNC(op_c190_0), 49552 }, /* AND */
{ CPUFUNC(op_c198_0), 49560 }, /* AND */
{ CPUFUNC(op_c1a0_0), 49568 }, /* AND */
{ CPUFUNC(op_c1a8_0), 49576 }, /* AND */
{ CPUFUNC(op_c1b0_0), 49584 }, /* AND */
{ CPUFUNC(op_c1b8_0), 49592 }, /* AND */
{ CPUFUNC(op_c1b9_0), 49593 }, /* AND */
{ CPUFUNC(op_c1c0_0), 49600 }, /* MULS */
{ CPUFUNC(op_c1d0_0), 49616 }, /* MULS */
{ CPUFUNC(op_c1d8_0), 49624 }, /* MULS */
{ CPUFUNC(op_c1e0_0), 49632 }, /* MULS */
{ CPUFUNC(op_c1e8_0), 49640 }, /* MULS */
{ CPUFUNC(op_c1f0_0), 49648 }, /* MULS */
{ CPUFUNC(op_c1f8_0), 49656 }, /* MULS */
{ CPUFUNC(op_c1f9_0), 49657 }, /* MULS */
{ CPUFUNC(op_c1fa_0), 49658 }, /* MULS */
{ CPUFUNC(op_c1fb_0), 49659 }, /* MULS */
{ CPUFUNC(op_c1fc_0), 49660 }, /* MULS */
{ CPUFUNC(op_d000_0), 53248 }, /* ADD */
{ CPUFUNC(op_d010_0), 53264 }, /* ADD */
{ CPUFUNC(op_d018_0), 53272 }, /* ADD */
{ CPUFUNC(op_d020_0), 53280 }, /* ADD */
{ CPUFUNC(op_d028_0), 53288 }, /* ADD */
{ CPUFUNC(op_d030_0), 53296 }, /* ADD */
{ CPUFUNC(op_d038_0), 53304 }, /* ADD */
{ CPUFUNC(op_d039_0), 53305 }, /* ADD */
{ CPUFUNC(op_d03a_0), 53306 }, /* ADD */
{ CPUFUNC(op_d03b_0), 53307 }, /* ADD */
{ CPUFUNC(op_d03c_0), 53308 }, /* ADD */
{ CPUFUNC(op_d040_0), 53312 }, /* ADD */
{ CPUFUNC(op_d048_0), 53320 }, /* ADD */
{ CPUFUNC(op_d050_0), 53328 }, /* ADD */
{ CPUFUNC(op_d058_0), 53336 }, /* ADD */
{ CPUFUNC(op_d060_0), 53344 }, /* ADD */
{ CPUFUNC(op_d068_0), 53352 }, /* ADD */
{ CPUFUNC(op_d070_0), 53360 }, /* ADD */
{ CPUFUNC(op_d078_0), 53368 }, /* ADD */
{ CPUFUNC(op_d079_0), 53369 }, /* ADD */
{ CPUFUNC(op_d07a_0), 53370 }, /* ADD */
{ CPUFUNC(op_d07b_0), 53371 }, /* ADD */
{ CPUFUNC(op_d07c_0), 53372 }, /* ADD */
{ CPUFUNC(op_d080_0), 53376 }, /* ADD */
{ CPUFUNC(op_d088_0), 53384 }, /* ADD */
{ CPUFUNC(op_d090_0), 53392 }, /* ADD */
{ CPUFUNC(op_d098_0), 53400 }, /* ADD */
{ CPUFUNC(op_d0a0_0), 53408 }, /* ADD */
{ CPUFUNC(op_d0a8_0), 53416 }, /* ADD */
{ CPUFUNC(op_d0b0_0), 53424 }, /* ADD */
{ CPUFUNC(op_d0b8_0), 53432 }, /* ADD */
{ CPUFUNC(op_d0b9_0), 53433 }, /* ADD */
{ CPUFUNC(op_d0ba_0), 53434 }, /* ADD */
{ CPUFUNC(op_d0bb_0), 53435 }, /* ADD */
{ CPUFUNC(op_d0bc_0), 53436 }, /* ADD */
{ CPUFUNC(op_d0c0_0), 53440 }, /* ADDA */
{ CPUFUNC(op_d0c8_0), 53448 }, /* ADDA */
{ CPUFUNC(op_d0d0_0), 53456 }, /* ADDA */
{ CPUFUNC(op_d0d8_0), 53464 }, /* ADDA */
{ CPUFUNC(op_d0e0_0), 53472 }, /* ADDA */
{ CPUFUNC(op_d0e8_0), 53480 }, /* ADDA */
{ CPUFUNC(op_d0f0_0), 53488 }, /* ADDA */
{ CPUFUNC(op_d0f8_0), 53496 }, /* ADDA */
{ CPUFUNC(op_d0f9_0), 53497 }, /* ADDA */
{ CPUFUNC(op_d0fa_0), 53498 }, /* ADDA */
{ CPUFUNC(op_d0fb_0), 53499 }, /* ADDA */
{ CPUFUNC(op_d0fc_0), 53500 }, /* ADDA */
{ CPUFUNC(op_d100_0), 53504 }, /* ADDX */
{ CPUFUNC(op_d108_0), 53512 }, /* ADDX */
{ CPUFUNC(op_d110_0), 53520 }, /* ADD */
{ CPUFUNC(op_d118_0), 53528 }, /* ADD */
{ CPUFUNC(op_d120_0), 53536 }, /* ADD */
{ CPUFUNC(op_d128_0), 53544 }, /* ADD */
{ CPUFUNC(op_d130_0), 53552 }, /* ADD */
{ CPUFUNC(op_d138_0), 53560 }, /* ADD */
{ CPUFUNC(op_d139_0), 53561 }, /* ADD */
{ CPUFUNC(op_d140_0), 53568 }, /* ADDX */
{ CPUFUNC(op_d148_0), 53576 }, /* ADDX */
{ CPUFUNC(op_d150_0), 53584 }, /* ADD */
{ CPUFUNC(op_d158_0), 53592 }, /* ADD */
{ CPUFUNC(op_d160_0), 53600 }, /* ADD */
{ CPUFUNC(op_d168_0), 53608 }, /* ADD */
{ CPUFUNC(op_d170_0), 53616 }, /* ADD */
{ CPUFUNC(op_d178_0), 53624 }, /* ADD */
{ CPUFUNC(op_d179_0), 53625 }, /* ADD */
{ CPUFUNC(op_d180_0), 53632 }, /* ADDX */
{ CPUFUNC(op_d188_0), 53640 }, /* ADDX */
{ CPUFUNC(op_d190_0), 53648 }, /* ADD */
{ CPUFUNC(op_d198_0), 53656 }, /* ADD */
{ CPUFUNC(op_d1a0_0), 53664 }, /* ADD */
{ CPUFUNC(op_d1a8_0), 53672 }, /* ADD */
{ CPUFUNC(op_d1b0_0), 53680 }, /* ADD */
{ CPUFUNC(op_d1b8_0), 53688 }, /* ADD */
{ CPUFUNC(op_d1b9_0), 53689 }, /* ADD */
{ CPUFUNC(op_d1c0_0), 53696 }, /* ADDA */
{ CPUFUNC(op_d1c8_0), 53704 }, /* ADDA */
{ CPUFUNC(op_d1d0_0), 53712 }, /* ADDA */
{ CPUFUNC(op_d1d8_0), 53720 }, /* ADDA */
{ CPUFUNC(op_d1e0_0), 53728 }, /* ADDA */
{ CPUFUNC(op_d1e8_0), 53736 }, /* ADDA */
{ CPUFUNC(op_d1f0_0), 53744 }, /* ADDA */
{ CPUFUNC(op_d1f8_0), 53752 }, /* ADDA */
{ CPUFUNC(op_d1f9_0), 53753 }, /* ADDA */
{ CPUFUNC(op_d1fa_0), 53754 }, /* ADDA */
{ CPUFUNC(op_d1fb_0), 53755 }, /* ADDA */
{ CPUFUNC(op_d1fc_0), 53756 }, /* ADDA */
{ CPUFUNC(op_e000_0), 57344 }, /* ASR */
{ CPUFUNC(op_e008_0), 57352 }, /* LSR */
{ CPUFUNC(op_e010_0), 57360 }, /* ROXR */
{ CPUFUNC(op_e018_0), 57368 }, /* ROR */
{ CPUFUNC(op_e020_0), 57376 }, /* ASR */
{ CPUFUNC(op_e028_0), 57384 }, /* LSR */
{ CPUFUNC(op_e030_0), 57392 }, /* ROXR */
{ CPUFUNC(op_e038_0), 57400 }, /* ROR */
{ CPUFUNC(op_e040_0), 57408 }, /* ASR */
{ CPUFUNC(op_e048_0), 57416 }, /* LSR */
{ CPUFUNC(op_e050_0), 57424 }, /* ROXR */
{ CPUFUNC(op_e058_0), 57432 }, /* ROR */
{ CPUFUNC(op_e060_0), 57440 }, /* ASR */
{ CPUFUNC(op_e068_0), 57448 }, /* LSR */
{ CPUFUNC(op_e070_0), 57456 }, /* ROXR */
{ CPUFUNC(op_e078_0), 57464 }, /* ROR */
{ CPUFUNC(op_e080_0), 57472 }, /* ASR */
{ CPUFUNC(op_e088_0), 57480 }, /* LSR */
{ CPUFUNC(op_e090_0), 57488 }, /* ROXR */
{ CPUFUNC(op_e098_0), 57496 }, /* ROR */
{ CPUFUNC(op_e0a0_0), 57504 }, /* ASR */
{ CPUFUNC(op_e0a8_0), 57512 }, /* LSR */
{ CPUFUNC(op_e0b0_0), 57520 }, /* ROXR */
{ CPUFUNC(op_e0b8_0), 57528 }, /* ROR */
{ CPUFUNC(op_e0d0_0), 57552 }, /* ASRW */
{ CPUFUNC(op_e0d8_0), 57560 }, /* ASRW */
{ CPUFUNC(op_e0e0_0), 57568 }, /* ASRW */
{ CPUFUNC(op_e0e8_0), 57576 }, /* ASRW */
{ CPUFUNC(op_e0f0_0), 57584 }, /* ASRW */
{ CPUFUNC(op_e0f8_0), 57592 }, /* ASRW */
{ CPUFUNC(op_e0f9_0), 57593 }, /* ASRW */
{ CPUFUNC(op_e100_0), 57600 }, /* ASL */
{ CPUFUNC(op_e108_0), 57608 }, /* LSL */
{ CPUFUNC(op_e110_0), 57616 }, /* ROXL */
{ CPUFUNC(op_e118_0), 57624 }, /* ROL */
{ CPUFUNC(op_e120_0), 57632 }, /* ASL */
{ CPUFUNC(op_e128_0), 57640 }, /* LSL */
{ CPUFUNC(op_e130_0), 57648 }, /* ROXL */
{ CPUFUNC(op_e138_0), 57656 }, /* ROL */
{ CPUFUNC(op_e140_0), 57664 }, /* ASL */
{ CPUFUNC(op_e148_0), 57672 }, /* LSL */
{ CPUFUNC(op_e150_0), 57680 }, /* ROXL */
{ CPUFUNC(op_e158_0), 57688 }, /* ROL */
{ CPUFUNC(op_e160_0), 57696 }, /* ASL */
{ CPUFUNC(op_e168_0), 57704 }, /* LSL */
{ CPUFUNC(op_e170_0), 57712 }, /* ROXL */
{ CPUFUNC(op_e178_0), 57720 }, /* ROL */
{ CPUFUNC(op_e180_0), 57728 }, /* ASL */
{ CPUFUNC(op_e188_0), 57736 }, /* LSL */
{ CPUFUNC(op_e190_0), 57744 }, /* ROXL */
{ CPUFUNC(op_e198_0), 57752 }, /* ROL */
{ CPUFUNC(op_e1a0_0), 57760 }, /* ASL */
{ CPUFUNC(op_e1a8_0), 57768 }, /* LSL */
{ CPUFUNC(op_e1b0_0), 57776 }, /* ROXL */
{ CPUFUNC(op_e1b8_0), 57784 }, /* ROL */
{ CPUFUNC(op_e1d0_0), 57808 }, /* ASLW */
{ CPUFUNC(op_e1d8_0), 57816 }, /* ASLW */
{ CPUFUNC(op_e1e0_0), 57824 }, /* ASLW */
{ CPUFUNC(op_e1e8_0), 57832 }, /* ASLW */
{ CPUFUNC(op_e1f0_0), 57840 }, /* ASLW */
{ CPUFUNC(op_e1f8_0), 57848 }, /* ASLW */
{ CPUFUNC(op_e1f9_0), 57849 }, /* ASLW */
{ CPUFUNC(op_e2d0_0), 58064 }, /* LSRW */
{ CPUFUNC(op_e2d8_0), 58072 }, /* LSRW */
{ CPUFUNC(op_e2e0_0), 58080 }, /* LSRW */
{ CPUFUNC(op_e2e8_0), 58088 }, /* LSRW */
{ CPUFUNC(op_e2f0_0), 58096 }, /* LSRW */
{ CPUFUNC(op_e2f8_0), 58104 }, /* LSRW */
{ CPUFUNC(op_e2f9_0), 58105 }, /* LSRW */
{ CPUFUNC(op_e3d0_0), 58320 }, /* LSLW */
{ CPUFUNC(op_e3d8_0), 58328 }, /* LSLW */
{ CPUFUNC(op_e3e0_0), 58336 }, /* LSLW */
{ CPUFUNC(op_e3e8_0), 58344 }, /* LSLW */
{ CPUFUNC(op_e3f0_0), 58352 }, /* LSLW */
{ CPUFUNC(op_e3f8_0), 58360 }, /* LSLW */
{ CPUFUNC(op_e3f9_0), 58361 }, /* LSLW */
{ CPUFUNC(op_e4d0_0), 58576 }, /* ROXRW */
{ CPUFUNC(op_e4d8_0), 58584 }, /* ROXRW */
{ CPUFUNC(op_e4e0_0), 58592 }, /* ROXRW */
{ CPUFUNC(op_e4e8_0), 58600 }, /* ROXRW */
{ CPUFUNC(op_e4f0_0), 58608 }, /* ROXRW */
{ CPUFUNC(op_e4f8_0), 58616 }, /* ROXRW */
{ CPUFUNC(op_e4f9_0), 58617 }, /* ROXRW */
{ CPUFUNC(op_e5d0_0), 58832 }, /* ROXLW */
{ CPUFUNC(op_e5d8_0), 58840 }, /* ROXLW */
{ CPUFUNC(op_e5e0_0), 58848 }, /* ROXLW */
{ CPUFUNC(op_e5e8_0), 58856 }, /* ROXLW */
{ CPUFUNC(op_e5f0_0), 58864 }, /* ROXLW */
{ CPUFUNC(op_e5f8_0), 58872 }, /* ROXLW */
{ CPUFUNC(op_e5f9_0), 58873 }, /* ROXLW */
{ CPUFUNC(op_e6d0_0), 59088 }, /* RORW */
{ CPUFUNC(op_e6d8_0), 59096 }, /* RORW */
{ CPUFUNC(op_e6e0_0), 59104 }, /* RORW */
{ CPUFUNC(op_e6e8_0), 59112 }, /* RORW */
{ CPUFUNC(op_e6f0_0), 59120 }, /* RORW */
{ CPUFUNC(op_e6f8_0), 59128 }, /* RORW */
{ CPUFUNC(op_e6f9_0), 59129 }, /* RORW */
{ CPUFUNC(op_e7d0_0), 59344 }, /* ROLW */
{ CPUFUNC(op_e7d8_0), 59352 }, /* ROLW */
{ CPUFUNC(op_e7e0_0), 59360 }, /* ROLW */
{ CPUFUNC(op_e7e8_0), 59368 }, /* ROLW */
{ CPUFUNC(op_e7f0_0), 59376 }, /* ROLW */
{ CPUFUNC(op_e7f8_0), 59384 }, /* ROLW */
{ CPUFUNC(op_e7f9_0), 59385 }, /* ROLW */
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_e8c0_0), 59584 }, /* BFTST */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_e8d0_0), 59600 }, /* BFTST */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_e8e8_0), 59624 }, /* BFTST */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_e8f0_0), 59632 }, /* BFTST */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_e8f8_0), 59640 }, /* BFTST */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_e8f9_0), 59641 }, /* BFTST */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_e8fa_0), 59642 }, /* BFTST */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_e8fb_0), 59643 }, /* BFTST */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_e9c0_0), 59840 }, /* BFEXTU */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_e9d0_0), 59856 }, /* BFEXTU */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_e9e8_0), 59880 }, /* BFEXTU */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_e9f0_0), 59888 }, /* BFEXTU */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_e9f8_0), 59896 }, /* BFEXTU */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_e9f9_0), 59897 }, /* BFEXTU */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_e9fa_0), 59898 }, /* BFEXTU */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_e9fb_0), 59899 }, /* BFEXTU */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_eac0_0), 60096 }, /* BFCHG */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_ead0_0), 60112 }, /* BFCHG */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_eae8_0), 60136 }, /* BFCHG */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_eaf0_0), 60144 }, /* BFCHG */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_eaf8_0), 60152 }, /* BFCHG */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_eaf9_0), 60153 }, /* BFCHG */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_ebc0_0), 60352 }, /* BFEXTS */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_ebd0_0), 60368 }, /* BFEXTS */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_ebe8_0), 60392 }, /* BFEXTS */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_ebf0_0), 60400 }, /* BFEXTS */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_ebf8_0), 60408 }, /* BFEXTS */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_ebf9_0), 60409 }, /* BFEXTS */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_ebfa_0), 60410 }, /* BFEXTS */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_ebfb_0), 60411 }, /* BFEXTS */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_ecc0_0), 60608 }, /* BFCLR */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_ecd0_0), 60624 }, /* BFCLR */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_ece8_0), 60648 }, /* BFCLR */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_ecf0_0), 60656 }, /* BFCLR */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_ecf8_0), 60664 }, /* BFCLR */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_ecf9_0), 60665 }, /* BFCLR */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_edc0_0), 60864 }, /* BFFFO */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_edd0_0), 60880 }, /* BFFFO */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_ede8_0), 60904 }, /* BFFFO */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_edf0_0), 60912 }, /* BFFFO */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_edf8_0), 60920 }, /* BFFFO */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_edf9_0), 60921 }, /* BFFFO */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_edfa_0), 60922 }, /* BFFFO */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_edfb_0), 60923 }, /* BFFFO */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_eec0_0), 61120 }, /* BFSET */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_eed0_0), 61136 }, /* BFSET */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_eee8_0), 61160 }, /* BFSET */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_eef0_0), 61168 }, /* BFSET */
#endif
#ifndef CPUEMU_68000_ONLY
{ CPUFUNC(op_eef8_0), 61176 }, /* BFSET */
#endif
#ifndef CPUEMU_
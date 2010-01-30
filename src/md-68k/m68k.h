 /* 
  * UAE - The Un*x Amiga Emulator
  * 
  * MC68000 emulation - machine dependent bits
  *
  * Copyright 1996 Bernd Schmidt, Samuel Devulder
  */

 
struct flag_struct {
    /* tricky: be sure that bit-offset(x)=bit-offset(c) in word representation. */
    /* This depends heavily on the bit-order and the compiler used. I am not    */
    /* sure the generated code is better than uae-0.6.1 */
    int :12;           /* First word */
    unsigned int n:1;
    unsigned int z:1;
    unsigned int v:1;
    unsigned int c:1;
    
    int :15;           /* Second word */
    unsigned int x:1;
};

extern struct flag_struct regflags;

#define ZFLG (regflags.z)
#define NFLG (regflags.n)
#define CFLG (regflags.c)
#define VFLG (regflags.v)
#define XFLG (regflags.x)

STATIC_INLINE int cctrue(const int cc)
{
    switch(cc){
     case 0: return 1;                       /* T */
     case 1: return 0;                       /* F */
     case 2: return !CFLG && !ZFLG;          /* HI */
     case 3: return CFLG || ZFLG;            /* LS */
     case 4: return !CFLG;                   /* CC */
     case 5: return CFLG;                    /* CS */
     case 6: return !ZFLG;                   /* NE */
     case 7: return ZFLG;                    /* EQ */
     case 8: return !VFLG;                   /* VC */
     case 9: return VFLG;                    /* VS */
     case 10:return !NFLG;                   /* PL */
     case 11:return NFLG;                    /* MI */
     case 12:return NFLG == VFLG;            /* GE */
     case 13:return NFLG != VFLG;            /* LT */
     case 14:return !ZFLG && (NFLG == VFLG); /* GT */
     case 15:return ZFLG || (NFLG != VFLG);  /* LE */
    }
    abort();
    return 0;
}

/* sam: MIT or MOTOROLA syntax ? */
#ifdef __linux__
#define MITMOT(mit,mot) mot
#else /* Amiga os */
#define MITMOT(mit,mot) mit
#endif

#define m68k_flag_tst(l, v)                                           \
  __asm__ __volatile__(MITMOT("tst"#l" %0\n\tmovew ccr,_regflags",    \
                              "tst."#l" %0\n\tmove.w %%ccr,regflags") \
                              : : "dmi" (v) : "cc")

#define m68k_flag_add(l, v, s, d)                                  \
  __asm__ __volatile__(MITMOT("add"#l" %3,%1\n\tmovew ccr,%0",     \
                              "add."#l" %3,%1\n\tmove.w %%ccr,%0") \
                       : "=d" (ccr), "=&d" (v)                     \
                       : "1" (s), "dmi" (d)                        \
                       : "cc")

#define m68k_flag_sub(l, v, s, d)                                  \
  __asm__ __volatile__(MITMOT("sub"#l" %2,%1\n\tmovew ccr,%0",     \
                              "sub."#l" %2,%1\n\tmove.w %%ccr,%0") \
                       : "=d" (ccr), "=&d" (v)                     \
                       : "dmi" (s), "1" (d)                        \
                       : "cc")

#define m68k_flag_cmp(l, s, d)                                           \
  __asm__ __volatile__(MITMOT("cmp"#l" %0,%1\n\tmovew ccr,_regflags",    \
                              "cmp."#l" %0,%1\n\tmove.w %%ccr,regflags") \
                       : : "dmi" (s), "d" (d) : "cc")


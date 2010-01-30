 /* 
  * UAE - The Un*x Amiga Emulator
  * 
  * MC68000 emulation - machine dependent bits
  *
  * Copyright 1997 Bernd Schmidt
  */
 
struct flag_struct {
    unsigned int c:1; /* first byte */
    int :5;
    unsigned int z:1;
    unsigned int n:1;
    int :3;           /* second, third & fourth byte */
    unsigned int v:1; 
    int :20;
    unsigned int x:1; /* fifth */
    int :31;
};

extern struct flag_struct regflags;

#define ZFLG (regflags.z)
#define NFLG (regflags.n)
#define CFLG (regflags.c)
#define VFLG (regflags.v)
#define XFLG (regflags.x)

static __inline__ int cctrue(const int cc)
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

void x86_flag_testl (uae_u32 v);
void x86_flag_testw (uae_s16 v);
void x86_flag_testb (uae_s8 v);
void x86_flag_cmpl (uae_u32 s, uae_s32 d);
void x86_flag_cmpw (uae_s16 s, uae_s16 d);
void x86_flag_cmpb (uae_s8 s, uae_s8 d);
uae_u32 x86_flag_doaddl (uae_u32 s, uae_s32 d);
uae_u32 x86_flag_doaddw (uae_s16 s, uae_s16 d);
uae_u32 x86_flag_doaddb (uae_s8 s, uae_s8 d);
uae_u32 x86_flag_dosubl (uae_u32 s, uae_s32 d);
uae_u32 x86_flag_dosubw (uae_s16 s, uae_s16 d);
uae_u32 x86_flag_dosubb (uae_s8 s, uae_s8 d);

#define x86_flag_addl(v,s,d) do { v = x86_flag_doaddl(s,d); } while (0)
#define x86_flag_addw(v,s,d) do { v = x86_flag_doaddw(s,d); } while (0)
#define x86_flag_addb(v,s,d) do { v = x86_flag_doaddb(s,d); } while (0)
#define x86_flag_subl(v,s,d) do { v = x86_flag_dosubl(s,d); } while (0)
#define x86_flag_subw(v,s,d) do { v = x86_flag_dosubw(s,d); } while (0)
#define x86_flag_subb(v,s,d) do { v = x86_flag_dosubb(s,d); } while (0)

#pragma aux x86_flag_testl = \
 "test edx,edx" \
 "pushfd" \
 "pop ecx" \
 "mov DWORD [regflags],ecx" \
 parm [edx] \
 modify [ecx]

#pragma aux x86_flag_testw = \
 "test dx,dx" \
 "pushfd" \
 "pop ecx" \
 "mov DWORD [ regflags],ecx" \
 parm [dx] \
 modify [ecx]

#pragma aux x86_flag_testb = \
 "test dl,dl" \
 "pushfd" \
 "pop ecx" \
 "mov DWORD [ regflags],ecx" \
 parm [dl] \
 modify [ecx]

#pragma aux x86_flag_doaddl = \
 "add edx,ecx" \
 "pushfd" \
 "pop ecx" \
 "mov DWORD [ regflags],ecx" \
 "mov DWORD [ regflags + 4],ecx" \
 parm [ecx] [edx] \
 value [edx] \
 modify [ecx]

#pragma aux x86_flag_doaddw = \
 "add dx,cx" \
 "pushfd" \
 "pop ecx" \
 "mov DWORD [ regflags],ecx" \
 "mov DWORD [ regflags + 4],ecx" \
 parm [cx] [dx] \
 value [edx] \
 modify [ecx]

#pragma aux x86_flag_doaddb = \
 "add dl,cl" \
 "pushfd" \
 "pop ecx" \
 "mov DWORD [ regflags],ecx" \
 "mov DWORD [ regflags + 4],ecx" \
 parm [cl] [dl] \
 value [edx] \
 modify [ecx]

#pragma aux x86_flag_dosubl = \
 "sub edx,ecx" \
 "pushfd" \
 "pop ecx" \
 "mov DWORD [ regflags],ecx" \
 "mov DWORD [ regflags + 4],ecx" \
 parm [ecx] [edx] \
 value [edx] \
 modify [ecx]

#pragma aux x86_flag_dosubw = \
 "sub dx,cx" \
 "pushfd" \
 "pop ecx" \
 "mov DWORD [ regflags],ecx" \
 "mov DWORD [ regflags + 4],ecx" \
 parm [cx] [dx] \
 value [edx] \
 modify [ecx]

#pragma aux x86_flag_dosubb = \
 "sub dl,cl" \
 "pushfd" \
 "pop ecx" \
 "mov DWORD [ regflags],ecx" \
 "mov DWORD [ regflags + 4],ecx" \
 parm [cl] [dl] \
 value [edx] \
 modify [ecx]

#pragma aux x86_flag_cmpl = \
 "cmp edx,ecx" \
 "pushfd" \
 "pop ecx" \
 "mov DWORD [ regflags],ecx" \
 parm [ecx] [edx] \
 modify [ecx]

#pragma aux x86_flag_cmpw = \
 "cmp dx,cx" \
 "pushfd" \
 "pop ecx" \
 "mov DWORD [ regflags],ecx" \
 parm [cx] [dx] \
 modify [ecx]

#pragma aux x86_flag_cmpb = \
 "cmp dl,cl" \
 "pushfd" \
 "pop ecx" \
 "mov DWORD [ regflags],ecx" \
 parm [cl] [dl] \
 modify [ecx]


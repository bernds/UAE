 /* 
  * UAE - The Un*x Amiga Emulator
  * 
  * Memory access functions
  *
  * Copyright 1997 Bernd Schmidt
  */

extern uae_u32 do_get_mem_long(uae_u32 *a);
#pragma aux do_get_mem_long = \
 "mov edx,[eax]" \
 "bswap edx" \
 parm [eax] \
 value [edx];

extern uae_u32 do_get_mem_word(uae_u16 *a);
#pragma aux do_get_mem_word = \
 "xor edx,edx" \
 "mov dx,[eax]" \
 "rol dx,8" \
 parm [eax] \
 value [edx];

static __inline__ uae_u32 do_get_mem_byte(uae_u8 *a)
{
    return *a;
}

extern void do_put_mem_long(uae_u32 *a, uae_u32 v);
#pragma aux do_put_mem_long = \
 "bswap edx" \
 "mov [eax],edx" \
 parm [eax] [edx] \
 modify [edx];

extern void do_put_mem_word(uae_u16 *a, uae_u16 v);
#pragma aux do_put_mem_word = \
 "rol dx,8" \
 "mov [eax],dx" \
 parm [eax] [dx] \
 modify [edx];

static __inline__ void do_put_mem_byte(uae_u8 *a, uae_u8 v)
{
    *a = v;
}

#define call_mem_get_func(func,addr) ((*func)(addr))
#define call_mem_put_func(func,addr,v) ((*func)(addr,v))

#undef NO_INLINE_MEMORY_ACCESS
#undef MD_HAVE_MEM_1_FUNCS

 /* 
  * UAE - The Un*x Amiga Emulator
  * 
  * Miscellaneous machine dependent support functions and definitions
  *
  * Copyright 1997 Bernd Schmidt
  */

#include "sysconfig.h"
#include "sysdeps.h"

#include "config.h"
#include "options.h"
#include "machdep/m68k.h"

struct flag_struct regflags;

int fast_memcmp (const void *a, const void *b, int len)
{
    return memcmp (a, b, len);
}

int do_memcmpy(char *foo, const char *bar, int len);
#pragma aux do_memcmpy = \
"xor eax,eax" \
"sub esi,edi" \
"sub edx,16" \
"jc LLB7" \
"LLB8:" \
"mov ebx,[esi + edi]" \
"mov ecx,[edi]" \
"cmp ebx,ecx" \
"jne LLC1" \
"mov ebx,[esi + edi + 4]" \
"mov ecx,[edi + 4]" \
"cmp ebx,ecx" \
"jne LLC2" \
"mov ebx,[esi + edi + 8]" \
"mov ecx,[edi + 8]" \
"cmp ebx,ecx" \
"jne LLC3" \
"mov ebx,[esi + edi + 12]" \
"mov ecx,[edi + 12]" \
"cmp ebx,ecx" \
"jne LLC4" \
"add edi,16" \
"sub edx,16" \
"jnc LLB8" \
"LLB7:" \
"add edx,16" \
"sub edx,4" \
"jc LLB2" \
"LLB1:" \
"mov ebx,[esi + edi]" \
"mov ecx,[edi]" \
"cmp ebx,ecx" \
"jne LLC5" \
"add edi,4" \
"sub edx,4" \
"jnc  LLB1" \
"LLB2:" \
"add edx, 4" \
"jz LLB9" \
"LLB3:" \
"mov bl,[esi + edi]" \
"mov cl,[edi]" \
"cmp bl,cl" \
"jne LLC6" \
"inc edi" \
"dec edx" \
"jnz LLB3" \
"jmp LLB9" \
"LLC01:" \
"mov ebx,[esi + edi]" \
"LLC1:" \
"mov [edi],ebx" \
"mov ebx,[esi + edi + 4]" \
"LLC2:" \
"mov [edi + 4],ebx" \
"mov ebx,[esi + edi + 8]" \
"LLC3:" \
"mov [edi + 8],ebx" \
"mov ebx,[esi + edi + 12]" \
"LLC4:" \
"mov [edi + 12],ebx" \
"add edi,16" \
"sub edx,16" \
"jnc LLC01" \
"add edx,16" \
"sub edx,4" \
"jc  LLC03" \
"LLC02:" \
"mov ebx,[esi + edi]" \
"LLC5:" \
"mov [edi],ebx" \
"add edi,4" \
"sub edx,4" \
"jnc  LLC02" \
"LLC03:" \
"add edx,4" \
"jz LLC05" \
"LLC04:" \
"mov bl,[esi + edi]" \
"LLC6:" \
"mov [edi],bl" \
"inc edi" \
"dec edx" \
"jnz LLC04" \
"LLC05:" \
"mov eax,1" \
"LLB9:" \
 parm [edi] [esi] [edx] \
 value [eax] \
 modify [esi edi edx ecx ebx]


int memcmpy (void *foo, const void *bar, int len)
{
    return do_memcmpy (foo, bar, len);
}

#if 0 /* This is handled in od-win32 by all Win32 targets.  */
void machdep_init (void)
{
}
#endif

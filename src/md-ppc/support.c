 /* 
  * UAE - The Un*x Amiga Emulator
  * 
  * Miscellaneous machine dependent support functions and definitions
  *
  * Copyright 1996 Bernd Schmidt
  */

#include "sysconfig.h"
#include "sysdeps.h"

#include "config.h"
#include "options.h"
#include "machdep/m68k.h"

struct flag_struct regflags;

int fast_memcmp(const void *foo, const void *bar, int len)
{
	return memcmp(foo, bar, len);
}

int memcmpy(void *foo, const void *bar, int len)
{
	while ((len -= 4) >= 0)
		if(*((long*)foo)++ != *((long*)bar)++)
			goto diff4;
	len += 4;
    
	while (len--)
		if (*((char*)foo)++ != *((char*)bar)++) {
			((char*)foo)[-1] = ((char*)bar)[-1];
			goto diff1;
		}
	return 0;

diff4:
	((long*)foo)[-1] = ((long*)bar)[-1];
	while ((len -= 4) >= 0)
		*((long*)foo)++ = *((long*)bar)++;
	len += 4;

diff1:
	while (len--)
		*((char*)foo)++ = *((char*)bar)++;
	return 1;
}

void machdep_init (void)
{
}

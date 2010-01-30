/*
 * cdrecord/libscg uses its own type system.
 * Map it to the UAE types.
 */
 
#ifndef SCG_DEFS_H
#define SCG_DEFS_H

#include <string.h>

/* cdrecord lib uses its own code to achive maximum performance -
 * use that code if the clib functions are really that bad. */
#define fillbytes(addr,size,b) memset(addr,b,size)
#define movebytes(f,t,s) memmove(t,f,s)

#endif

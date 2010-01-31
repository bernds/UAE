#ifndef __FDI2RAW_H
#define __FDI2RAW_H

//#include "types.h"
#include <stdio.h>
typedef struct fdi FDI;

#ifdef __cplusplus
extern "C" {
#endif

extern uae_u8 *fdi2raw_read_track(FDI *, int track, int *len);
extern FDI *fdi2raw_header(struct zfile *f);
extern void fdi2raw_header_free (FDI *);
extern int fdi2raw_get_last_track(FDI *);
extern int fdi2raw_get_last_head(FDI *);
extern int fdi2raw_get_type (FDI *);
extern int fdi2raw_get_bit_rate (FDI *);
extern int fdi2raw_get_rotation (FDI *);
extern int fdi2raw_get_write_protect (FDI *);

#ifdef __cplusplus
}
#endif

#endif

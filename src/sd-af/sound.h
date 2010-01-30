 /* 
  * UAE - The Un*x Amiga Emulator
  * 
  * Support for the AF sound system
  * 
  * Copyright 1996 Marcus Sundberg
  */

#include <AF/AFlib.h>

extern AFAudioConn  *aud;
extern AC            ac;
extern long          aftime;
extern int           rate;
extern int sndbufsize;
extern uae_u16 sndbuffer[];
extern uae_u16 *sndbufpt;

static __inline__ void flush_sound_buffer(void)
{
    long size = (char *)sndbufpt - (char *)sndbuffer;
    if (AFGetTime(ac) > aftime)
	aftime = AFGetTime(ac);
    AFPlaySamples(ac, aftime, size, (unsigned char*) sndbuffer);
    aftime += size / 2;
    sndbufpt = sndbuffer;
}

static __inline__ void check_sound_buffers (void)
{
    if ((char *)sndbufpt - (char *)sndbuffer >= sndbufsize) {
	flush_sound_buffer();
    }
}

#define PUT_SOUND_BYTE(b) do { *(uae_u8 *)sndbufpt = b; sndbufpt = (uae_u16 *)(((uae_u8 *)sndbufpt) + 1); } while (0)
#define PUT_SOUND_WORD(b) do { *(uae_u16 *)sndbufpt = b; sndbufpt = (uae_u16 *)(((uae_u8 *)sndbufpt) + 2); } while (0)
#define SOUND16_BASE_VAL 0
#define SOUND8_BASE_VAL 128

#define DEFAULT_SOUND_BSIZ 8192
#define DEFAULT_SOUND_BITS 16
#define DEFAULT_SOUND_FREQ 44100


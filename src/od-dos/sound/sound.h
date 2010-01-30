 /*
  * UAE - The Un*x Amiga Emulator
  *
  * Support for DOS
  *
  * Copyright 1997, 1998 Gustavo Goedert
  */

extern int sound_fd;
extern uae_u16 sndbuffer[];
extern uae_u16 *sndbufpt;
extern int sndbufsize;

static __inline__ void check_sound_buffers (void)
{
}

#define PUT_SOUND_BYTE(b) do { ; } while (0)
#define PUT_SOUND_WORD(b) do { ; } while (0)
#define PUT_SOUND_BYTE_RIGHT(b) do { ; } while (0)
#define PUT_SOUND_BYTE_LEFT(b) do { ; } while (0)
#define PUT_SOUND_WORD_RIGHT(b) do { ; } while (0)
#define PUT_SOUND_WORD_LEFT(b) do { ; } while (0)
#define SOUND16_BASE_VAL 0
#define SOUND8_BASE_VAL 128

#define DEFAULT_SOUND_MAXB 8192
#define DEFAULT_SOUND_MINB 2048
#define DEFAULT_SOUND_BITS 16
#define DEFAULT_SOUND_FREQ 44100
#define HAVE_STEREO_SUPPORT

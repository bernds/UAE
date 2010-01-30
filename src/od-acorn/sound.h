 /* 
  * UAE - The Un*x Amiga Emulator
  * 
  * Support for Acorn RISC-OS sound
  * 
  * Copyright 1996, 1997 Peter Teichmann
  */

extern uae_u16 sndbuffer[];
extern uae_u16 *sndbufpt;
extern void sound_output(char *b, int l);
extern int sndbufsize;

static __inline__ void flush_sound_buffer(void)
{
  sound_output(sndbuffer, sndbufsize);
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
#define SOUND8_BASE_VAL 0

#define DEFAULT_SOUND_MINB 8192
#define DEFAULT_SOUND_MAXB 8192
#define DEFAULT_SOUND_BITS 8
#define DEFAULT_SOUND_FREQ 44100

#undef DONT_WANT_SOUND

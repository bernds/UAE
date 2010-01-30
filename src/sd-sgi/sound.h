 /* 
  * UAE - The Un*x Amiga Emulator
  * 
  * Support for the Silicon Graphics Audio Library (AL)
  * 
  * Copyright 1998 Ari Heikkinen
  */

#include <dmedia/audio.h>

extern uae_u16 sndbuffer[];
extern uae_u16 *sndbufpt;
extern int sndblocksize;
extern ALport al_port;
extern int to_frames_divisor;

static __inline__ void check_sound_buffers(void)
{
	int bytes = (char *)sndbufpt - (char *)sndbuffer;
	if (bytes >= sndblocksize) {
		alWriteFrames(al_port, sndbuffer, bytes / to_frames_divisor);
		sndbufpt = sndbuffer;
	}
}

#define PUT_SOUND_BYTE(b) ( *(uae_u8 *)sndbufpt++ = (b) )
#define PUT_SOUND_WORD(b) ( *(uae_u16 *)sndbufpt++ = (b) )
#define PUT_SOUND_BYTE_LEFT(b) PUT_SOUND_BYTE(b)
#define PUT_SOUND_WORD_LEFT(b) PUT_SOUND_WORD(b)
#define PUT_SOUND_BYTE_RIGHT(b) PUT_SOUND_BYTE(b)
#define PUT_SOUND_WORD_RIGHT(b) PUT_SOUND_WORD(b)
#define SOUND16_BASE_VAL 0
#define SOUND8_BASE_VAL 0

#define DEFAULT_SOUND_MINB 8192
#define DEFAULT_SOUND_MAXB 8192
#define DEFAULT_SOUND_BITS 16
#define DEFAULT_SOUND_FREQ 44100
#define HAVE_STEREO_SUPPORT

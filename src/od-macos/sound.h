 /* 
  * UAE - The Un*x Amiga Emulator
  * 
  * Support for Mac OS sound
  * 
  * Copyright 1996 Ernesto Corvi
  */

#include <Sound.h>

extern SndChannelPtr newChannel;
extern ExtSoundHeader theSndBuffer;
extern SndCommand theCmd;

/* The buffer is too large... */
extern uae_u16 buffer0[44100], buffer1[44100], *sndbufpt;

extern int have_sound;
extern int nextbuf;
extern Boolean sFlag;
extern int sndbufsize;

static __inline__ void flush_sound_buffer (void)
{
    sndbufpt = buffer0;
    
    theSndBuffer.samplePtr = (Ptr)buffer0;
    theSndBuffer.numChannels = 1;
    theSndBuffer.sampleRate = 0xac440000;
    theSndBuffer.encode = extSH;
    theSndBuffer.numFrames = sndbufsize;
    theSndBuffer.sampleSize = 8;
    theCmd.param1 = 0;
    theCmd.param2 = (long)&theSndBuffer;
    theCmd.cmd = bufferCmd;
    SndDoCommand(newChannel, &theCmd, false);    
}

static __inline__ void check_sound_buffers (void)
{
    if ((char *)sndbufpt - (char *)sndbuffer >= sndbufsize) {
	flush_sound_buffer ();
    }
}

#define PUT_SOUND_BYTE(b) do { *(uae_u8 *)sndbufpt = b; sndbufpt = (uae_u16 *)(((uae_u8 *)sndbufpt) + 1); } while (0)
#define PUT_SOUND_WORD(b) do { *(uae_u16 *)sndbufpt = b; sndbufpt = (uae_u16 *)(((uae_u8 *)sndbufpt) + 2); } while (0)
#define SOUND16_BASE_VAL 0
#define SOUND8_BASE_VAL 0

#define DEFAULT_SOUND_BSIZ 8192
#define DEFAULT_SOUND_BITS 16
#define DEFAULT_SOUND_FREQ 44100

#undef DONT_WANT_SOUND

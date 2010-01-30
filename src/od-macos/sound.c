 /* 
  * UAE - The Un*x Amiga Emulator
  * 
  * Support for Mac OS sound
  * 
  * Copyright 1996 Ernesto Corvi
  */

#include "sysconfig.h"
#include "sysdeps.h"

#include "config.h"
#include "options.h"
#include "memory.h"
#include "custom.h"
#include "audio.h"
#include "gensound.h"
#include "sounddep/sound.h"
#include "events.h"

SndChannelPtr newChannel;
ExtSoundHeader theSndBuffer;
SndCommand theCmd;
int sndbufsize;

/* The buffer is too large... */
uae_u16 buffer0[44100], buffer1[44100], *sndbufpt;

int have_sound;
int nextbuf=0;
Boolean sFlag=true;

void close_sound(void)
{
}

int init_sound (void)
{	
    if (SndNewChannel(&newChannel, sampledSynth, initMono, NULL)) 
	return 0;
    sndbufsize = 44100;
    init_sound_table8 ();

    sndbufpt = buffer0;
    sound_available = 1;
    return 1;
}


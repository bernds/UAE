 /*
  * UAE - The Un*x Amiga Emulator
  * 
  * Support for Acorn RISC-OS sound
  * 
  * Copyright 1996, 1997 Peter Teichmann
  */

#include "sysconfig.h"
#include "sysdeps.h"

#include "config.h"
#include "options.h"
#include "memory.h"
#include "events.h"
#include "custom.h"
#include "audio.h"
#include "gensound.h"
#include "sounddep/sound.h"

#include <math.h>

extern int sratecode;
uae_u16 sndbuffer[44100];
uae_u16 *sndbufpt;
int sndbufsize;

int init_sound (void)
{
    int rate, dspbits;
  
    dspbits = sound_desired_bits;
    rate = sound_desired_freq;
    sratecode = 4096.0*log(rate/4096.0)/log(2.0);
    sndbufsize = sound_desired_maxbsiz;

    scaled_sample_evtime = (unsigned long)maxhpos * maxvpos * vblank_hz * CYCLE_UNIT / rate;
    scaled_sample_evtime_ok = 1;

    if (dspbits == 16)
    {
	init_sound_table16 ();
	sample_handler = sample16_handler;
    } else {
	init_sound_table8 ();
	sample_handler = sample8_handler;
    }

    sound_available=1;
    sndbufpt=sndbuffer;
  
    return 1;
}

void close_sound(void)
{
}


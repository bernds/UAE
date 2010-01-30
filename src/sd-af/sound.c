 /* 
  * UAE - The Un*x Amiga Emulator
  * 
  * Support for the AF sound system
  * 
  * Copyright 1996 Marcus Sundberg
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

AFAudioConn  *aud;
AC            ac;
long          aftime;
int           rate;
uae_u16 sndbuffer[44100];
uae_u16 *sndbufpt;
int sndbufsize;

static int have_sound;

void close_sound(void)
{
}

int init_sound (void)
{
    AFSetACAttributes   attributes;
    AFDeviceDescriptor *aDev;
    int                 device;
    
    aud = AFOpenAudioConn(NULL);
    have_sound = !(aud == NULL);
    if (!have_sound) {
	return 0;
    }
    
    for(device = 0; device < ANumberOfAudioDevices(aud); device++) {
	aDev = AAudioDeviceDescriptor(aud, device);
	rate = aDev->playSampleFreq;
	sndbufsize = (rate / 8) * 4;
	if(aDev->inputsFromPhone == 0
	   && aDev->outputsToPhone == 0
	   && aDev->playNchannels == 1)
	    break;
    }
    if (device == ANumberOfAudioDevices(aud)) {
	return 0;
    }
    
    attributes.type = LIN16;
    ac = AFCreateAC(aud, device, ACEncodingType, &attributes);
    aftime = AFGetTime(ac);

    init_sound_table16 ();
    sample_handler = sample16_handler;
    sample_evtime = (long)MAXHPOS_PAL * MAXVPOS_PAL * VBLANK_HZ_PAL / rate;

    sndbufpt = sndbuffer;
    sound_available = 1;
    printf ("Sound driver found and configured for %d bits at %d Hz, buffer is %d bytes\n", 16, rate, sndbufsize);
    return 1;
}

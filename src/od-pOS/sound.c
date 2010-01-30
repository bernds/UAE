 /* 
  * UAE - The Un*x Amiga Emulator
  * 
  * Support for Amiga audio.device sound
  * 
  * Copyright 1996, 1997 Samuel Devulder
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

#include <pDOS/Lock.h>
#include <pDOS/Files.h>
#include <pDOS/DosSig.h>
#include <pDOS/DosErrors.h>
#include <pInline/pDos2.h>

extern struct pOS_DosBase         *gb_DosBase;

static struct pOS_MsgPort      AudioMP;
struct pOS_AudioIO     *AudioIO;
struct pOS_Std8AudioMap AudioMap;

unsigned char *buffers[2];
uae_u16 *sndbuffer;
uae_u16 *sndbufpt;
int sndbufsize;
int bufidx;
static int port_created, devopen, channel_allocated;

int have_sound;

FILE *AUDIO_FILE;

static FILE *TST_AUDIO_FILE(char *buff, char *name, int rate, int bsize)
{
    struct pOS_FileLock *lock;

    if(!name) return 0;
    sprintf(buff,name,rate,bsize);
    lock = pOS_LockObject(NULL, buff, FILELKACC_Exclusive|FILELKACC_NoReq);
    if(lock) {
        pOS_UnlockObject(lock);
        return fopen(buff,"wb");
    }
    return NULL;
}

int setup_sound(void)
{
    sound_available = 1;
    return 1;
}

int init_sound (void) 
{
    int rate;
    char buff[256],*devname = NULL;

    atexit(close_sound); /* if only amiga os had resource tracking */

    sndbufsize = (currprefs.sound_maxbsiz + 1)&~1;

    /* check freq */
    if (!currprefs.sound_freq) currprefs.sound_freq = 11025;
    rate = currprefs.sound_freq;

    /* check for $AUDIONAME or AUD: or AUDIO: device */
    devname = buff;
    AUDIO_FILE = TST_AUDIO_FILE(buff, getenv("AUDIONAME"),
                                rate, sndbufsize);
    if(!AUDIO_FILE) /* AHI */
    AUDIO_FILE = TST_AUDIO_FILE(buff, "AUDIO:FREQUENCY=%d/BUFFER=%d",
                                rate, sndbufsize);
    if(!AUDIO_FILE) /* AUD: */
    AUDIO_FILE = TST_AUDIO_FILE(buff, "AUDIO:FREQUENCY%d/BUFFER%d",
                                rate, sndbufsize);
    if(!AUDIO_FILE)
    AUDIO_FILE = TST_AUDIO_FILE(buff, "AUD:FREQUENCY%d/BUFFER%d",
                                rate, sndbufsize);

    /* else use audio.device */
    if(!AUDIO_FILE) {
        if(!pOS_ConstructMsgPort(&AudioMP)) goto fail;
        port_created = 1;
        AudioIO      = (void*)pOS_CreateIORequest(&AudioMP,
                                                  sizeof(struct pOS_AudioIO));
        if(!AudioIO) goto fail;

        if(pOS_OpenDevice(devname = "pAudio.device", 0, (void*)AudioIO, 0,0)) 
            goto fail;
        devopen = 1;

        AudioIO->aio_Command                 = AUDIOCMD_AllocChannel;
        AudioIO->aio_U.aio_AllocCh.alio_Tags = NULL;
        if(pOS_DoIO((struct pOS_IORequest*)AudioIO)) goto fail;
        channel_allocated = 1;
    }

    /* get the buffers */
    if(AUDIO_FILE) {
        buffers[0] = malloc(sndbufsize);
        buffers[1] = NULL;
        if(!buffers[0]) goto fail;
    } else {
        buffers[0] = malloc(sndbufsize);
        buffers[1] = malloc(sndbufsize);
        if(!buffers[0] || !buffers[1]) goto fail;
    }
    bufidx = 0;
    sndbuffer = sndbufpt = (uae_u16*)buffers[bufidx];

    AudioIO->aio_U.aio_Map.amio_AudioMap = (struct pOS_AudioMap*)&AudioMap;
    AudioMap.bam_Audio.am_Type           = AUDIOMAPTYP_Std8Bit;
    AudioMap.bam_Audio.am_Frequency      = rate;
    AudioMap.bam_Length                  = sndbufsize;
    AudioMap.bam_Sample                  = (void*)sndbuffer;

    scaled_sample_evtime = (unsigned long)maxhpos * maxvpos * vblank_hz * CYCLE_UNIT / rate;
    scaled_sample_evtime_ok = 1;

    init_sound_table8 ();
    sample_handler = sample8_handler;

    fprintf(stderr, "Sound driver found and configured for %d bits "
                    "at %d Hz, buffer is %d bytes (%s)\n",
                    8, rate, sndbufsize,devname);

    sound_available = 1;
    return 1;
fail:
    sound_available = 0;
    return 0;
}

void close_sound(void)
{
    if(AUDIO_FILE) {
        fclose(AUDIO_FILE);
        AUDIO_FILE = NULL;
    }
    if(channel_allocated) {
        AudioIO->aio_Command = AUDIOCMD_FreeChannel;
        pOS_DoIO((struct pOS_IORequest*)AudioIO);
        channel_allocated = 0;
    }
    if(devopen) {
        pOS_CloseDevice((void*)AudioIO);
        devopen = 0;
    }
    if(AudioIO) {
        pOS_DeleteIORequest((void*)AudioIO);
        AudioIO = NULL;
    }
    if(port_created) {
        pOS_DestructMsgPort(&AudioMP);
        port_created = 0;
    }
    if(buffers[0]) {free(buffers[0]);buffers[0] = 0;}
    if(buffers[1]) {free(buffers[1]);buffers[1] = 0;}
}

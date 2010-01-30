 /* 
  * UAE - The Un*x Amiga Emulator
  * 
  * Support for Amiga audio.device sound
  * 
  * Copyright 1996, 1997, 1998 Samuel Devulder, Holger Jakob (AHI).
  *
  * History:
  *	22/02/98: Added AHI support from Holger Jakob.
  *     05/04/98: Added AHI_DMA_MODE from Holger Jakob.
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

#include <hardware/custom.h>
#include <hardware/cia.h>

#if defined(POWERUP)
#include <clib/alib_protos.h>
#include <dos/dosextens.h>
#undef  AllocMem
#undef  FreeMem
#define AllocMem PPCAllocMem
#define FreeMem  PPCFreeMem
#endif

#ifdef USE_AHIDEVICE
struct MsgPort    *AHImp=NULL; 
struct AHIRequest *AHIio[2]={NULL,NULL};
struct AHIRequest *linkio=NULL;

/* sam: AHI_DMA_MODE come from Holger Jakob. In this mode, uae 
   will output the sound in real time. It is to be used in conjunction 
   with FRAME_RATE_HACK. Use this on fast hardwares only! */
#ifdef AHI_DMA_MODE
#ifndef USE_CLIB
#include <powerup/ppcproto/ahi.h>
#else /* USE_CLIB */
#define AHI_AudioRequest(a0, tags...) \
	({ULONG _tags[] = { tags }; AHI_AudioRequestA((a0), (struct TagItem *)_tags);})

#define AHI_ControlAudio(a0, tags...) \
	({ULONG _tags[] = { tags }; AHI_ControlAudioA((a0), (struct TagItem *)_tags);})

#define AHI_AllocAudio(tags...) \
	({ULONG _tags[] = { tags }; AHI_AllocAudioA((struct TagItem *)_tags);})

#define AHI_AllocAudioRequest(tags...) \
	({ULONG _tags[] = { tags }; AHI_AllocAudioRequestA((struct TagItem *)_tags);})
#endif /* USE_CLIB */

#include <utility/hooks.h>
struct Library    *AHIBase=NULL;
struct AHIAudioCtrl *actrl=NULL;
struct AHIEffect {
  struct	AHIEffChannelInfo eff;
  ULONG		Offset[2];
} effect = {0};
struct AHISampleInfo Sample0 =
{
  AHIST_M8S,
  NULL,
  NULL,
};
uae_u16 *sndbufptrmax=NULL;

#if defined(POWERUP)
unsigned short dummyfunc[]={0x4E75,0x4E75}; /* rts */
#else /* not POWERUP => plain old mc68k */
long __saveds dummyfunc(void);
__asm("
	.text
	.globl  _dummyfunc
_dummyfunc:
	rts
");
#endif /* POWERUP */
struct Hook hook = { 0, 0, (void *)dummyfunc, NULL, NULL };

static unsigned long basevsynctime;
signed long bufsamples;
#endif /* AHI_DMA_MODE */
#endif /* AHI_DEVICE */

#define CIAAPRA 0xBFE001 
#define CUSTOM  0xDFF000

static struct Custom *custom= (struct Custom*) CUSTOM;
static struct CIA *cia = (struct CIA *) CIAAPRA;

/*
 * Compared to Linux, AF_SOUND, and mac above, the AMIGA sound processing
 * with OS routines is awfull. (sam). But with AHI DOSDriver it is far more
 * easier (but it is still a mess here !).
 */

char whichchannel[]={1,2,4,8};
struct IOAudio *AudioIO=NULL;
struct MsgPort *AudioMP=NULL;
struct Message *AudioMSG=NULL;

unsigned char *buffers[2];
uae_u16 *sndbuffer;
uae_u16 *sndbufpt;
int sndbufsize;
int bufidx, devopen,ahiopen;

int have_sound, clockval, oldledstate, period;

ULONG AUDIO_FILE;

static ULONG TST_AUDIO_FILE(char *buff, char *name, int rate, int bsize)
{
    struct Process *pr = (void*)FindTask(NULL);
    ULONG wd, fd;

    if(!name) return 0;
    wd = (ULONG)pr->pr_WindowPtr;
    pr->pr_WindowPtr = (APTR)-1;
    sprintf(buff,name,rate,bsize);
    fd = Open(buff, MODE_NEWFILE);
    pr->pr_WindowPtr = (APTR)wd;
    return fd;
}

int setup_sound(void)
{
    sound_available = 1;
    return 1;
}

static char* open_AHI(void)
{
#ifdef USE_AHIDEVICE
    if( (AHImp=CreateMsgPort()) ) {
    if( (AHIio[0]=(struct AHIRequest *)CreateIORequest(AHImp,sizeof(struct AHIRequest))) ) { 
    AHIio[0]->ahir_Version=4;
#ifdef AHI_DMA_MODE
    if( !OpenDevice(AHINAME,AHI_NO_UNIT,(struct IORequest *)AHIio[0],NULL) ) {
#else
     if( !OpenDevice(AHINAME,0,(struct IORequest *)AHIio[0],NULL) ) {
#endif
    if( (AHIio[1] = malloc(sizeof(struct AHIRequest))) ) {
#ifdef AHI_DMA_MODE
	AHIBase=(struct Library *)AHIio[0]->ahir_Std.io_Device;
	actrl=(struct AHIAudioCtrl *)AHI_AllocAudio(
		AHIA_AudioID,0x00020008,
		AHIA_MixFreq,11025,
		AHIA_Channels,1,
		AHIA_Sounds,1,
		TAG_DONE);
#endif
    memcpy(AHIio[1], AHIio[0], sizeof(struct AHIRequest));
    ahiopen = 1;
    return AHINAME;
    }}}}
#endif
    ahiopen = 0;
    return NULL;
}

static void close_AHI(void)
{
#ifdef USE_AHIDEVICE
    if( ahiopen ) {
#ifdef AHI_DMA_MODE
      if(actrl) {
	effect.eff.ahie_Effect=AHIET_CHANNELINFO | AHIET_CANCEL;
	AHI_SetEffect(&effect,actrl);
	AHI_FreeAudio(actrl); actrl==NULL;
      }
#endif
      if(AHIio[0]->ahir_Std.io_Length) {
        AbortIO((struct IORequest *) AHIio[0]);
        WaitIO((struct IORequest *) AHIio[0]);
      }
      if(linkio) { /* Only if the second request was started */
        AbortIO((struct IORequest *) AHIio[1]);
        WaitIO((struct IORequest *) AHIio[1]);
      }
      CloseDevice((struct IORequest *)AHIio[0]);
      DeleteIORequest((void*)AHIio[0]);
      if(AHImp) DeleteMsgPort((void*)AHImp);
      AHIio[0]=NULL;
      AHIio[1]=NULL;
      ahiopen = 0;
    }
#endif
}

int init_sound (void) 
{ /* too complex ? No it is only the allocation of a single channel ! */
  /* it would have been far less painfull if AmigaOS provided a */
  /* SOUND: device handler */
    int rate;
    char buff[256],*devname = NULL;

    atexit(close_sound); /* if only amiga os had resource tracking */
    
    /* determine the clock */
    { 
	struct GfxBase *GB;
	GB = (void*)OpenLibrary("graphics.library",0L);
	if(!GB) goto fail;
	if (GB->DisplayFlags & PAL)
	    clockval = 3546895;        /* PAL clock */
	else
	    clockval = 3579545;        /* NTSC clock */
	CloseLibrary((void*)GB);
    }

    /* check buffsize */
    if (currprefs.sound_maxbsiz < 2 || currprefs.sound_maxbsiz > (256*1024)) {
        fprintf(stderr, "Sound buffer size %d out of range.\n", currprefs.sound_maxbsiz);
        currprefs.sound_maxbsiz = 8192;
    } 
    sndbufsize = (currprefs.sound_maxbsiz + 1)&~1;

    /* check freq */
    if (!currprefs.sound_freq) currprefs.sound_freq = 1;
    if (clockval/currprefs.sound_freq < 80/*124*/ || clockval/currprefs.sound_freq > 65535) {
	fprintf(stderr, "Can't use sound with desired frequency %d Hz\n", currprefs.sound_freq);
        currprefs.sound_freq = 22000;
    }
    rate   = currprefs.sound_freq;
    period = (uae_u16)(clockval/rate);

    /* check for $AUDIONAME */
    devname = buff;
    AUDIO_FILE = TST_AUDIO_FILE(buff, getenv("AUDIONAME"),
                                rate, sndbufsize);

    if(!AUDIO_FILE) {char *s=open_AHI();if(s) devname=s;}

    if(!AUDIO_FILE && !ahiopen) /* AHI dos-handler */
    AUDIO_FILE = TST_AUDIO_FILE(buff, "AUDIO:FREQUENCY=%d/BUFFER=%d",
                                rate, sndbufsize);
    /* check for AUD: or AUDIO: device */
    if(!AUDIO_FILE && !ahiopen) /* AUDIO: */
    AUDIO_FILE = TST_AUDIO_FILE(buff, "AUDIO:FREQUENCY%d/BUFFER%d",
                                rate, sndbufsize);
    if(!AUDIO_FILE && !ahiopen) /* AUD: */
    AUDIO_FILE = TST_AUDIO_FILE(buff, "AUD:FREQUENCY%d/BUFFER%d",
                                rate, sndbufsize);
                                
    if(!AUDIO_FILE && !ahiopen) {
        /* else use the plain old audio.device */
        /* setup the stuff */
        AudioMP = CreateMsgPort();
        if(!AudioMP) goto fail;
            AudioIO = (struct IOAudio *)CreateIORequest(AudioMP, 
                                                    sizeof(struct IOAudio));
        if(!AudioIO) goto fail;

        AudioIO->ioa_Request.io_Message.mn_Node.ln_Pri /*pfew!!*/ = 85;
        AudioIO->ioa_Data = whichchannel;
        AudioIO->ioa_Length = sizeof(whichchannel);
        AudioIO->ioa_AllocKey = 0;
        if(OpenDevice(devname = AUDIONAME, 0, (void*)AudioIO, 0)) goto fail;
        devopen = 1;
    }

    /* get the buffers */
    if(AUDIO_FILE) {
        buffers[0] = (void*)AllocMem(sndbufsize,MEMF_ANY|MEMF_CLEAR);
        buffers[1] = NULL;
        if(!buffers[0]) goto fail;
    } else if( ahiopen ) {
        buffers[0] = (void*)AllocMem(sndbufsize,MEMF_PUBLIC|MEMF_CLEAR);
        buffers[1] = (void*)AllocMem(sndbufsize,MEMF_PUBLIC|MEMF_CLEAR);
        if(!buffers[0] || !buffers[1]) goto fail;

#ifdef AHI_DMA_MODE
	sndbufptrmax = (uae_u16 *)(((uae_u8 *)buffers[0]) + sndbufsize);
	basevsynctime = vsynctime;

	if(currprefs.sound_bits == 16)
		Sample0.ahisi_Type=(currprefs.stereo==1)?AHIST_S16S:AHIST_M16S;
	else
		Sample0.ahisi_Type=(currprefs.stereo==1)?AHIST_S8S:AHIST_M8S;
	Sample0.ahisi_Length=sndbufsize/((currprefs.stereo?2:1)*(currprefs.sound_bits==16?2:1));
	Sample0.ahisi_Address=buffers[0];
	AHI_LoadSound(0,AHIST_SAMPLE,&Sample0,actrl);

	AHI_SetFreq(0,rate,actrl,AHISF_IMM);
	AHI_SetVol(0,0x10000L,0x08000L,actrl,AHISF_IMM);
	AHI_SetSound(0,0,0,0,actrl,AHISF_IMM);
	AHI_ControlAudio(actrl, AHIC_Play,TRUE, TAG_DONE);

	effect.eff.ahie_Effect=AHIET_CHANNELINFO;
	effect.eff.ahieci_Func=&hook;
	effect.eff.ahieci_Channels=1;
	effect.eff.ahieci_Pad=0;

	AHI_SetEffect(&effect,actrl);
#elif defined(FRAME_RATE_HACK)
/* reduce vsynctime a bit to give the sound emulation more room to breathe.*/
    vsynctime = vsynctime * 9 / 10;
#endif /* !AHI_DMA_MODE && ! FRAME_RATE_HACK */
    } else {
        buffers[0] = (void*)AllocMem(sndbufsize,MEMF_CHIP|MEMF_CLEAR);
        buffers[1] = (void*)AllocMem(sndbufsize,MEMF_CHIP|MEMF_CLEAR);
        if(!buffers[0] || !buffers[1]) goto fail;
    }
    bufidx = 0;
    sndbuffer = sndbufpt = (uae_u16*)buffers[bufidx];

    oldledstate = cia->ciapra & (1<<CIAB_LED);
    cia->ciapra |= (1<<CIAB_LED);

    scaled_sample_evtime = (unsigned long)maxhpos * maxvpos * vblank_hz * CYCLE_UNIT / rate;
    scaled_sample_evtime_ok = 1;

    if (ahiopen) {
    	if(currprefs.sound_bits == 16) {
	   init_sound_table16 ();
	   sample_handler = currprefs.stereo ? sample16s_handler
					     : sample16_handler;
	} else {
	   init_sound_table8 ();
	   sample_handler = currprefs.stereo ? sample8s_handler
					     : sample8_handler;
	}
    } else {
    	currprefs.stereo = 0;
	init_sound_table8 ();
        sample_handler = sample8_handler;
    }

    fprintf(stderr, "Sound driver found and configured for %d bits %s "
                    "at %d Hz, buffer is %d bytes (%s)\n",
                    currprefs.sound_bits,(currprefs.stereo==1 ? "stereo" : "mono"), 
		    rate, sndbufsize,devname);

    sound_available = 1;
    return 1;
fail:
    sound_available = 0;
    return 0;
}

void adjust_sound_timing (void)
{
#ifdef AHI_DMA_MODE
    static unsigned long last;
    signed long diff;
    unsigned long samplepos;

    samplepos = (unsigned long)(effect.Offset[0]*(1));
    bufsamples=sndbufsize/2;

    if (!samplepos)
	samplepos++;

    if (last == samplepos)
	return;
    last = samplepos;

/*    diff = samplecount - samplepos;*/
    diff = (int)((uae_u8 *)sndbufpt-buffers[0]) - samplepos;
    diff-=1024;


/*    if (diff < -bufsamples || diff > bufsamples) {
	sndbufpt = (uae_u16 *)buffers[0];
    }*/
    if (diff < -bufsamples) diff+=sndbufsize; else if (diff > bufsamples) diff-=sndbufsize;
    if (diff < 0)
	vsynctime = basevsynctime * 5 / 6;
    else if (diff > 0)
	vsynctime = basevsynctime * 7 / 6;
#endif
}

void close_sound(void)
{
    if(ahiopen) {close_AHI();ahiopen = 0;}
    if(AUDIO_FILE) {Close(AUDIO_FILE);AUDIO_FILE=NULL;}
    if(devopen) {CloseDevice((void*)AudioIO);devopen = 0;}
    if(AudioIO) {DeleteIORequest((void*)AudioIO);AudioIO = NULL;}
    if(AudioMP) {DeleteMsgPort((void*)AudioMP);AudioMP = NULL;}
    if(buffers[0]) {FreeMem((APTR)buffers[0],sndbufsize);buffers[0] = 0;}
    if(buffers[1]) {FreeMem((APTR)buffers[1],sndbufsize);buffers[1] = 0;}
    if(sound_available) {
    	cia->ciapra = (cia->ciapra & ~(1<<CIAB_LED)) | oldledstate;
	sound_available = 0;
    }
}

 /* 
  * UAE - The Un*x Amiga Emulator
  * 
  * Support for Amiga audio.device and AHI.device sound.
  * 
  * Copyright 1996, 1997, 1998 Samuel Devulder, Holger Jakob (AHI)
  */

#ifdef HAVE_DEVICES_AHI_H
#define USE_AHIDEVICE 
#ifdef FRAME_RATE_HACK
/* holger:In this mode emulation speed is adjusted with sound dma
          and sound output delay does not depend on sndbufsize
#define AHI_DMA_MODE
 */
#endif
#endif 

#include <exec/memory.h>
#include <exec/devices.h>
#include <exec/io.h>

#include <graphics/gfxbase.h>
#include <devices/timer.h>
#include <devices/audio.h>

#if defined(POWERUP)
#ifndef USE_CLIB
#include <powerup/ppcproto/exec.h>
#include <powerup/ppcproto/dos.h>
/* sam: no inline/alib.h on powerup, so use this instead :( */
static void BeginIO(struct IORequest *iorequest)
{
	struct Caos	MyCaos;
	MyCaos.a1		= (ULONG) iorequest;
	MyCaos.M68kCacheMode	= IF_CACHEFLUSHALL;
	MyCaos.PPCCacheMode	= IF_CACHEFLUSHALL;
	MyCaos.caos_Un.Offset	= (-0x1e);
	MyCaos.a6		= (ULONG) iorequest->io_Device;
	PPCCallOS(&MyCaos);
}
#else
#include <clib/exec_protos.h>
#include <clib/alib_protos.h>
#endif
#else
#include <proto/exec.h>
#include <proto/dos.h>
#ifdef __GNUC__
#include <proto/alib.h>
#elif defined(__SASC)
#include <clib/alib_protos.h>
#endif
#endif

#ifdef USE_AHIDEVICE
#include <devices/ahi.h>
extern struct AHIRequest *AHIio[]; 
extern struct AHIRequest *linkio;
#ifdef AHI_DMA_MODE
extern struct Library    *AHIBase;
extern struct AHIAudioCtrl *actrl;
extern uae_u16 *sndbufptrmax;
#endif /* AHI_DMA_MODE */
#endif /* USE_AHI_DEVICE */

extern char whichchannel[];
extern struct IOAudio *AudioIO;
extern struct MsgPort *AudioMP;
extern struct Message *AudioMSG;

extern unsigned char *buffers[2];
extern uae_u16 *sndbuffer, *sndbufpt;
extern int bufidx, devopen,ahiopen;
extern int sndbufsize;

extern int have_sound, clockval, oldledstate, period;

extern ULONG AUDIO_FILE;

static __inline__ void flush_sound_buffer(void)
{
    if(AUDIO_FILE) {
        Write(AUDIO_FILE, 
              sndbuffer = sndbufpt = (uae_u16*)buffers[bufidx], 
              sndbufsize);
    } else if(ahiopen) {
#ifdef USE_AHIDEVICE
	void *tmp;

	AHIio[bufidx]->ahir_Std.io_Message.mn_Node.ln_Pri = 0;
	AHIio[bufidx]->ahir_Std.io_Command  = CMD_WRITE;
	AHIio[bufidx]->ahir_Std.io_Data     = buffers[bufidx];
	AHIio[bufidx]->ahir_Std.io_Length   = sndbufsize;
	AHIio[bufidx]->ahir_Std.io_Offset   = 0;
	AHIio[bufidx]->ahir_Frequency       = currprefs.sound_freq;
	if( currprefs.stereo )
		AHIio[bufidx]->ahir_Type    = (currprefs.sound_bits==16) ? 
						AHIST_S16S : AHIST_S8S;
	else
		AHIio[bufidx]->ahir_Type    = (currprefs.sound_bits==16) ? 
						AHIST_M16S : AHIST_M8S;
	AHIio[bufidx]->ahir_Volume          = 0x10000;          /* Full volume */
	AHIio[bufidx]->ahir_Position        = 0x8000;           /* Centered */
	AHIio[bufidx]->ahir_Link            = linkio;
	SendIO((struct IORequest *) AHIio[bufidx]);

	if( linkio ) WaitIO((struct IORequest *) linkio);
	linkio = AHIio[bufidx];
	/* double buffering */
	bufidx = 1 - bufidx;
	sndbuffer = sndbufpt = (uae_u16*)buffers[bufidx];
#endif
   } else {
	static char IOSent = 0;

	AudioIO->ioa_Request.io_Command = CMD_WRITE;
	AudioIO->ioa_Request.io_Flags   = ADIOF_PERVOL|IOF_QUICK;
	AudioIO->ioa_Data               = (uae_s8 *)buffers[bufidx];
	AudioIO->ioa_Length             = sndbufsize;
	AudioIO->ioa_Period             = period;
	AudioIO->ioa_Volume             = 64;
	AudioIO->ioa_Cycles             = 1;

	if(IOSent) WaitIO((void*)AudioIO); else IOSent=1;
	BeginIO((void*)AudioIO);

	/* double buffering */
	bufidx = 1 - bufidx;
	sndbuffer = sndbufpt = (uae_u16*)buffers[bufidx];
    }
}

static __inline__ void check_sound_buffers (void)
{
#ifndef AHI_DMA_MODE
    if ((char *)sndbufpt - (char *)sndbuffer >= sndbufsize) {
	flush_sound_buffer();
    }
#endif
}

#ifdef AHI_DMA_MODE
#define PUT_SOUND_BYTE(b) \
	if (sndbufpt == sndbufptrmax) { \
		sndbufpt = (uae_u16 *)buffers[0]; \
	} \
	*(uae_u8 *)sndbufpt = b; \
	sndbufpt = (uae_u16 *)(((uae_u8 *)sndbufpt) + 1);

#define PUT_SOUND_WORD(b) \
	if (sndbufpt == sndbufptrmax) { \
		sndbufpt = (uae_u16 *)buffers[0]; \
	} \
	*(uae_u16 *)sndbufpt = b; \
	sndbufpt = (uae_u16 *)(((uae_u8 *)sndbufpt) + 2);
#else
#define PUT_SOUND_BYTE(b) do { *(uae_u8 *)sndbufpt = b; sndbufpt = (uae_u16 *)(((uae_u8 *)sndbufpt) + 1); } while (0)
#define PUT_SOUND_WORD(b) do { *(uae_u16 *)sndbufpt = b; sndbufpt = (uae_u16 *)(((uae_u8 *)sndbufpt) + 2); } while (0)
#endif

#ifdef USE_AHIDEVICE
#define HAVE_STEREO_SUPPORT
/* left and right channel on wrong side */
#define PUT_SOUND_BYTE_LEFT(b) PUT_SOUND_BYTE(b)
#define PUT_SOUND_WORD_LEFT(b) PUT_SOUND_WORD(b)
#define PUT_SOUND_BYTE_RIGHT(b) PUT_SOUND_BYTE(b)
#define PUT_SOUND_WORD_RIGHT(b) PUT_SOUND_WORD(b)
#endif

#define SOUND16_BASE_VAL 0
#define SOUND8_BASE_VAL 0

#define DEFAULT_SOUND_MAXB 8192
#define DEFAULT_SOUND_MINB 8192
#define DEFAULT_SOUND_BITS 16
#define DEFAULT_SOUND_FREQ 11025

#undef DONT_WANT_SOUND

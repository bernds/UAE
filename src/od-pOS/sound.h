 /* 
  * UAE - The Un*x Amiga Emulator
  * 
  * Support for Amiga audio.device sound
  * 
  * Copyright 1996, 1997 Samuel Devulder
  */

struct pOS_Locale;
struct pOS_Library;
struct pOS_ExecBase;
struct pOS_DosBase;
struct pOS_Method;
struct pOS_Resource;
struct pOS_RawDoFmtData;
struct pOS_AsciiDoFmtData;
struct pOS_AsciiFmtData;
struct pOS_ClassGrp;
struct pOS_DosDevPathInfo;
struct pOS_ShellScript;
struct pOS_SegmentInfo;
struct IClass;
struct Isrvstr;
struct TagItem;
struct MemHeader;
struct SemaphoreMessage;
struct StackSwapStruct;
struct Interrupt;
struct StandardPacket;
struct InfoData;
struct WBStartup;

#define _SIZE_T /* to avoid interference with <sys/types.h> */
#include <pExec/Types.h>
#include <pDOS/ArgTags.h>
#include <pDOS/DosSig.h>
#include <pDOS/DosErrors.h>
#include <pAudio/Audio.h>
#include <pAudio/AudioTags.h>
#include <pAudio/InfoData.h>
#include <pAudio/StdAMap.h>
#include <pAudio/AudioLib.h>

#include <pInline/pExec2.h>

extern struct pOS_ExecBase        *gb_ExecBase;
extern struct pOS_ExecLibFunction *gb_ExecLib;
extern struct pOS_DosBase         *gb_DosBase;

extern struct pOS_AudioIO     *AudioIO;
extern struct pOS_Std8AudioMap AudioMap;

extern unsigned char *buffers[2];
extern uae_u16 *sndbuffer, *sndbufpt;
extern int bufidx, devopen;
extern int sndbufsize;

extern int have_sound;

extern FILE *AUDIO_FILE;

static __inline__ void flush_sound_buffer(void)
{
    if(AUDIO_FILE) {
	fwrite(buffers[bufidx], sndbufsize, 1, AUDIO_FILE);
    } else {
        static char IOSent = 0;

	AudioMap.bam_Sample                   = (SBYTE *)buffers[bufidx];

	AudioIO->aio_Command                  = CMD_WRITE;
	AudioIO->aio_Flags                    = IOREQF_Quick;
	AudioIO->aio_U.aio_Map.amio_Volume    = 0xFFFF; 
	AudioIO->aio_U.aio_Map.amio_Cycles    = 1;
	AudioIO->aio_U.aio_Map.amio_Frequency = currprefs.sound_freq;  
	AudioIO->aio_U.aio_Map.amio_Length    = sndbufsize;

        if(IOSent) pOS_WaitIO((void*)AudioIO); else IOSent=1;
	pOS_BeginIO((void*)AudioIO);

        /* double buffering */
        bufidx = 1 - bufidx;
        sndbuffer = sndbufpt = (uae_u16*)buffers[bufidx];
    }
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

#define DEFAULT_SOUND_MAXB 8192
#define DEFAULT_SOUND_MINB 8192
#define DEFAULT_SOUND_BITS 8
#define DEFAULT_SOUND_FREQ 11025

#define UNSUPPORTED_OPTION_b

#undef DONT_WANT_SOUND

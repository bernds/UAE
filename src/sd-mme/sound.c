 /* 
  * UAE - The Un*x Amiga Emulator
  * 
  * Support for Digital Unix/MME
  * 
  * Copyright 1997 Marcus Sundberg
  */

#define MME_SOUND_C
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

/*#include <sys/ioctl.h>*/
#include <mme/mme_api.h>

HWAVEOUT       mme_handle = 0;
LPSTR          mme_audiobuf = NULL;
LPWAVEHDR      WaveHeader = NULL;
static char   *mme_sndbufpt;

static int have_sound;
static int bytes_per_sample;

uae_u16 *sndbuffer;
uae_u16 *sndbufpt;
int sndbufsize;
int mme_free_bufs = 0, mme_nextbuf = 0;


/* callbackfunction used by mme */
static void mme_callback(HANDLE hWaveOut,
			UINT wMsg,
			DWORD dwInstance,
			LPARAM lParam1,
			LPARAM lParam2)
{
  switch(wMsg) {
  case WOM_OPEN:
  case WOM_CLOSE:
    /* Ignore these */
    break;
  case WOM_DONE:
    mme_free_bufs--;
    break;
  default:
    break;
  }
}

void close_sound(void)
{
    if (have_sound) {
	mmeFreeMem(WaveHeader);
	mmeFreeBuffer(mme_audiobuf);
	waveOutClose(mme_handle);
    }
    mme_handle=0;
}

int setup_sound(void)
{
    if (waveOutGetNumDevs() < 1) {
	have_sound = 0;
	return 0;
    }
    have_sound = 1;

    sound_available = 1;
    return 1;
}

int init_sound (void)
{
    int rate;
    int dspbits;
    int channels;
    MMRESULT	  status;
    LPPCMWAVEFORMAT waveformat;

    if (currprefs.sound_maxbsiz < 128 || currprefs.sound_maxbsiz > 16384) {
	fprintf(stderr, "Sound buffer size %d out of range.\n", currprefs.sound_maxbsiz);
	currprefs.sound_maxbsiz = 8192;
    }
    
    sndbufsize = currprefs.sound_maxbsiz;

    dspbits = currprefs.sound_bits;

    rate = currprefs.sound_freq;
    
    channels = 1;
    
    if((waveformat = (LPPCMWAVEFORMAT)
	mmeAllocMem(sizeof(PCMWAVEFORMAT))) == NULL ) {
	fprintf(stderr, "Failed to allocate PCMWAVEFORMAT struct\n");
	return 0;
    }
    waveformat->wf.nSamplesPerSec = rate;
    waveformat->wf.nChannels = channels;
    waveformat->wBitsPerSample = dspbits;
    waveformat->wf.wFormatTag = WAVE_FORMAT_PCM;
   
    bytes_per_sample = waveformat->wf.nChannels *
	(waveformat->wBitsPerSample/8);
    waveformat->wf.nBlockAlign = bytes_per_sample;
    waveformat->wf.nAvgBytesPerSec = bytes_per_sample *
	waveformat->wf.nSamplesPerSec;
   
    /* Open the audio device with desired rate/format */
    status = waveOutOpen( &mme_handle,
			  WAVE_MAPPER,
			  (LPWAVEFORMAT)waveformat,
			  (void (*)())mme_callback,
			  (unsigned int)NULL,
			  CALLBACK_FUNCTION | WAVE_OPEN_SHAREABLE );
    mmeFreeMem(waveformat);
   
    if(status != MMSYSERR_NOERROR) {
	fprintf(stderr, "waveOutOpen failed - status = %d\n", status);
	return 0;
    }
   
    /* Allocate wave header for use in write */
    if((WaveHeader = (LPWAVEHDR)
	mmeAllocMem(sizeof(WAVEHDR))) == NULL ) {
	fprintf(stderr, "Failed to allocate WAVEHDR struct\n");
	return 0;
    }
    /* Allocate shared audio buffer for communicating with audio device */
    if ((mme_audiobuf = (LPSTR)
	 mmeAllocBuffer(sndbufsize*SOUND_NUMBUF*bytes_per_sample*2)) == NULL) {
	fprintf(stderr, "Failed to allocate shared audio buffer\n");
	mmeFreeMem(WaveHeader);
	return 0;
    }
    sndbuffer = mme_audiobuf;
    scaled_sample_evtime = (unsigned long)MAXHPOS_PAL * MAXVPOS_PAL * VBLANK_HZ_PAL * CYCLE_UNIT / rate;
    scaled_sample_evtime_ok = 1;

    if (dspbits == 16) {
	init_sound_table16 ();
	sample_handler = sample16_handler;
    } else {
	init_sound_table8 ();
	sample_handler = sample8_handler;
    }
    sound_available = 1;
    printf ("Sound driver found and configured for %d bits at %d Hz, buffer is %d bytes\n", dspbits, rate, sndbufsize);
    mme_sndbufpt = sndbufpt = sndbuffer;
    return 1;
}


__inline__ void check_sound_buffers (void)
{
    if (((char *)sndbufpt - (char *)mme_sndbufpt) < sndbufsize)
	return;
    if (mmeCheckForCallbacks())
	mmeProcessCallbacks();
    if (mme_free_bufs >= SOUND_NUMBUF)
	return;
    WaveHeader->lpData = (LPSTR)mme_sndbufpt;
    WaveHeader->dwBufferLength = ((char *)sndbufpt - (char *)mme_sndbufpt);
    mme_nextbuf++;
    if (mme_nextbuf == SOUND_NUMBUF) {
	mme_sndbufpt = sndbuffer;
	mme_nextbuf = 0;
    }
    else mme_sndbufpt += bytes_per_sample*sndbufsize*2;
    sndbufpt = mme_sndbufpt;
    if (waveOutWrite(mme_handle, WaveHeader,
		     sizeof(WAVEHDR)) == MMSYSERR_NOERROR)
	mme_free_bufs++;
}

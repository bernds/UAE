/*
 * UAE - The Un*x Amiga Emulator
 *
 * Win32 interface
 *
 * Copyright 1997 Mathias Ortmann
 */

#include <windows.h>
#include <ddraw.h>
#include <dsound.h>
#include <stdlib.h>
#include <stdarg.h>
#include <commctrl.h>
#include <commdlg.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <io.h>

#include "config.h"
#include "sysconfig.h"
#include "sysdeps.h"
#include "options.h"
#include "gensound.h"
#include "sounddep/sound.h"
#include "events.h"
#include "uae.h"
#include "include/memory.h"
#include "events.h"
#include "custom.h"
#include "osdep/win32gui.h"
#include "osdep/resource.h"
#include "osdep/win32.h"

/* Sound emulation, Win32 interface */
char *sndptr, *sndptrmax, soundneutral;

static WAVEFORMATEX wavfmt;

LPSTR lpData;
static frame_time_t basevsynctime;
signed long bufsamples;
unsigned int samplecount;
DWORD soundbufsize;

int setup_sound (void)
{
    sound_available = 1;
    return 1;
}

int use_direct_sound = 1;

static HINSTANCE hDSound = NULL;

static HRESULT (WINAPI *pDirectSoundCreate) (GUID FAR *, LPDIRECTSOUND FAR *, IUnknown FAR *);

extern HWND hAmigaWnd;

LPDIRECTSOUND lpDS = NULL;
LPDIRECTSOUNDBUFFER lpDSBprimary = NULL;
LPDIRECTSOUNDBUFFER lpDSB = NULL;

char *DSError( HRESULT error )
{
    switch( error )
    {
    case DSERR_ALLOCATED:
        return "Allocated";

    case DSERR_UNINITIALIZED:
        return "Uninitialized";

    case DSERR_BADFORMAT:
        return "Bad Format";

    case DSERR_INVALIDCALL:
        return "Invalid Call";

    case DSERR_INVALIDPARAM:
        return "Invalid Parameter";

    case DSERR_OUTOFMEMORY:
        return "Out of Memory";

    case DSERR_PRIOLEVELNEEDED:
        return "Priority Level Needed";

    case DSERR_UNSUPPORTED:
        return "Unsupported";

    default:
        return "Unknown";
    }
}

LPWAVEHDR lpWaveHdr;
HANDLE hWaveOut;

MMTIME mmtime;

void close_sound (void)
{
    if( use_direct_sound )
    {
        if( lpDSB )
        {
            IDirectSoundBuffer_Release( lpDSB );
            lpDSB = NULL;
        }
        if( lpDSBprimary )
        {
            IDirectSoundBuffer_Release( lpDSBprimary );
            lpDSBprimary = NULL;
        }
        if( lpDS )
        {
            IDirectSound_Release( lpDS );
            lpDS = NULL;
        }
        if( hDSound )
            FreeLibrary( hDSound );
    }
    else
    {
        if( hWaveOut )
        {
	        waveOutReset( hWaveOut );
	        waveOutUnprepareHeader( hWaveOut, lpWaveHdr, sizeof( WAVEHDR ) );
	        GlobalFree( lpWaveHdr );
	        waveOutClose( hWaveOut );

	        hWaveOut = NULL;
        }
    }
    GlobalFree( lpData );
}

static __inline__ void direct_sound_play (LPSTR buffer)
{
    HRESULT hr;
    LPVOID lpvPtr1, lpvPtr2;
    DWORD dwBytes1, dwBytes2, status = 0;

    hr = IDirectSoundBuffer_Lock( lpDSB, 0, soundbufsize, &lpvPtr1, &dwBytes1, &lpvPtr2, &dwBytes2, 0 );
    if( hr == DSERR_BUFFERLOST )
    {
        IDirectSoundBuffer_Restore( lpDSB );
        hr = IDirectSoundBuffer_Lock( lpDSB, 0, soundbufsize, &lpvPtr1, &dwBytes1, &lpvPtr2, &dwBytes2, 0 );
    }
    if (hr == DS_OK) {
        memcpy (lpvPtr1, buffer, dwBytes1);
        if (lpvPtr2 != NULL) {
            memcpy (lpvPtr2, buffer+dwBytes1, dwBytes2);
        }
        hr = IDirectSoundBuffer_Unlock( lpDSB, lpvPtr1, dwBytes1, lpvPtr2, dwBytes2 );
        hr = IDirectSoundBuffer_SetCurrentPosition( lpDSB, 0 );
        hr = IDirectSoundBuffer_GetStatus (lpDSB, &status);
        if (status & DSBSTATUS_PLAYING)
        {
            /* Already playing */
        }
        else
        {
            /* We took too long to fill in the buffer - get it playing again */
            hr = IDirectSoundBuffer_Play( lpDSB, 0, 0, 0 );
	    if (hr != DS_OK) {
		write_log( "SoundBuffer_Play() failure: %s", DSError( hr ) );
	    }
        }
    }
    else
    {
        write_log( "SoundLock() failure: %s", DSError( hr ) );
    }
}
    
void flush_sound_buffer( void )
{
    if (use_direct_sound)
        direct_sound_play (lpData);
}

void startsound( void )
{
    HRESULT hr;

    if (use_direct_sound)
    {
#if 0
        hr = IDirectSoundBuffer_Play (lpDSBprimary, 0, 0, DSBPLAY_LOOPING);
	if (hr != DS_OK) {
            write_log( "SoundPlay() failure: %s", DSError( hr ) );
        }
        hr = IDirectSoundBuffer_Play( lpDSB, 0, 0, 0 );

        if( hr == DS_OK )
        {
            write_log( "Sound Started...\n" );
            hr = IDirectSoundBuffer_SetVolume( lpDSB, 0 );
        } else {
            write_log( "SoundPlay() failure: %s", DSError( hr ) );
        }
#endif
    }
    else
    {
        lpWaveHdr->lpData = lpData;
        lpWaveHdr->dwBufferLength = soundbufsize;
        lpWaveHdr->dwFlags = WHDR_BEGINLOOP | WHDR_ENDLOOP;
        lpWaveHdr->dwLoops = 0x7fffffffL;
        waveOutPrepareHeader( hWaveOut, lpWaveHdr, sizeof(WAVEHDR) ); 

        if( waveOutWrite( hWaveOut, lpWaveHdr, sizeof(WAVEHDR) ) )
        { 
            write_log( "Failed to write to sound card\n" );
        } 
    }
}

void stopsound (void)
{
    HRESULT hr;

    sndptr = lpData;
    samplecount = 0;

    if( use_direct_sound )
    {
#if 0
        hr = IDirectSoundBuffer_Stop( lpDSB );
        if( hr != DS_OK )
        {
            write_log( "SoundStop() failure: %s", DSError( hr ) );
        }
        else
            write_log( "Sound Stopped...\n" );
#endif
    }
    else
    {
        memset( lpData, soundneutral, soundbufsize );
        waveOutReset( hWaveOut );
        waveOutUnprepareHeader( hWaveOut, lpWaveHdr, sizeof( WAVEHDR ) );
    }
}

static HWND dsound_tmpw;

int dsound_newwindow (HWND w)
{
    HRESULT hr;

    if (lpDS == 0)
	return 1;

    if (w == 0)
	w = dsound_tmpw;

    hr = IDirectSound_SetCooperativeLevel (lpDS, w, DSSCL_PRIORITY);
    if (hr != DS_OK) {
	write_log( "SetCooperativeLevel() failure: %s", DSError (hr));
	return 0;
    }
    return 1;
}

static int init_sound_win32 (void)
{
    HRESULT hr;
    DSBUFFERDESC sound_buffer;
    MMRESULT mmres;
    static int first_time = 1;

    if( currprefs.produce_sound < 0 ) 
    {
	    currprefs.produce_sound = 0;
	    write_log( "FrameSync disabled.\n" );
    }
    else
    {
#ifdef FRAME_RATE_HACK
        if (first_time)
        {
	        figure_processor_speed ();
            first_time = 0;
        }
#endif
    }

    if (currprefs.produce_sound < 2) {
	write_log ("Sound output disabled.\n");
	return 1;
    }

    wavfmt.wFormatTag = WAVE_FORMAT_PCM;
    wavfmt.nChannels = 1 + currprefs.stereo;
    wavfmt.nSamplesPerSec = currprefs.sound_freq;
    wavfmt.nBlockAlign = currprefs.sound_bits / 8 * wavfmt.nChannels;
    wavfmt.nAvgBytesPerSec = wavfmt.nBlockAlign * currprefs.sound_freq;
    wavfmt.wBitsPerSample = currprefs.sound_bits;

    soundneutral = currprefs.sound_bits == 8 ? 128 : 0;
    bufsamples = use_direct_sound ? 4096:2048 * currprefs.sound_freq / 11025;
    soundbufsize = bufsamples * wavfmt.nBlockAlign;

    if( !( lpData = (LPSTR)GlobalAlloc( GMEM_FIXED | GMEM_ZEROINIT, soundbufsize ) ) )
    {
	    write_log( "Failed to allocate sound buffer!\n" );
	    return 0;
    }

    if( use_direct_sound )
    {
	hDSound = LoadLibrary( "DSOUND.DLL" );
        if (hDSound == 0) {
    	    write_log ("You have to install DirectX on your system before you can use UAE.\n"
		       "Refer to the documentation for further details.\n");
	    return 0;
        }
	pDirectSoundCreate = GetProcAddress (hDSound, "DirectSoundCreate");

	basevsynctime = vsynctime;
	sndptrmax = lpData+soundbufsize;
	hr = pDirectSoundCreate( NULL, &lpDS, NULL );
	if (hr != DS_OK) {
	    write_log( "DirectSoundCreate() failure: %s", DSError (hr));
	    return 0;
	}

	dsound_tmpw = CreateWindowEx (WS_EX_ACCEPTFILES,
				      "PCsuxRox",
				      "Argh",
				      WS_CAPTION,
				      CW_USEDEFAULT, CW_USEDEFAULT,
				      10, 10,
				      NULL,
				      NULL,
				      0,
				      NULL);

	if (! dsound_newwindow (0))
	    return 0;

	memset (&sound_buffer, 0, sizeof( DSBUFFERDESC ));
	sound_buffer.dwSize = sizeof( DSBUFFERDESC );
	sound_buffer.dwFlags = DSBCAPS_PRIMARYBUFFER;
	sound_buffer.dwBufferBytes = 0;
	sound_buffer.lpwfxFormat = NULL;

	hr = IDirectSound_CreateSoundBuffer (lpDS, &sound_buffer, &lpDSBprimary, NULL);
	if (hr != DS_OK) {
	    write_log( "CreateSoundBuffer() failure: %s", DSError( hr ) );
	    return 0;
	}

	hr = IDirectSoundBuffer_SetFormat (lpDSBprimary, &wavfmt);
	if (hr != DS_OK) {
	    write_log( "SetFormat() failure: %s", DSError (hr));
	    return 0;
	}
#if 0
	hr = IDirectSoundBuffer_SetVolume (lpDSBprimary, 0);
	if (hr != DS_OK) {
	    write_log( "SetVolume() 1 failure: %s", DSError (hr));
	    return 0;
	}
#endif
	sound_buffer.dwBufferBytes = soundbufsize;
	sound_buffer.lpwfxFormat = &wavfmt;
	sound_buffer.dwFlags = DSBCAPS_CTRLDEFAULT | DSBCAPS_STATIC;
	hr = IDirectSound_CreateSoundBuffer( lpDS, &sound_buffer, &lpDSB, NULL );
	if (hr != DS_OK) {
	    write_log ("CreateSoundBuffer() failure: %s", DSError (hr));
	    return 0;
	}
	hr = IDirectSoundBuffer_SetVolume (lpDSB, 0);
	if (hr != DS_OK) {
	    write_log( "SetVolume() 2 failure: %s", DSError (hr));
	    return 0;
	}
    } else {
        mmtime.wType = TIME_SAMPLES;

        if( !( lpWaveHdr = GlobalAlloc( GMEM_FIXED | GMEM_ZEROINIT, (DWORD)sizeof( WAVEHDR ) ) ) )
        {
	        write_log( "Failed to allocate wave header!\n" );
	        return 0;
        }

        if( mmres = waveOutOpen( &hWaveOut, WAVE_MAPPER, &wavfmt, 0, 0, CALLBACK_NULL ) )
        {
	        write_log( "Sound device failed to open with error code %d.\n", mmres );
	        return 0;
        }
    }
    basevsynctime = vsynctime;
    sndptrmax = lpData + soundbufsize;

    stopsound();
    startsound();

    return currprefs.sound_freq;
}

void adjust_sound_timing (void)
{
    static DWORD last;
    signed long diff;

    if( use_direct_sound )
        return;

    waveOutGetPosition (hWaveOut, &mmtime, sizeof (mmtime));
    if (!mmtime.u.sample)
	mmtime.u.sample++;

    if (last == mmtime.u.sample)
	return;
    last = mmtime.u.sample;

    diff = samplecount - mmtime.u.sample;

    if (diff < -bufsamples || diff > bufsamples) {
	samplecount = mmtime.u.sample;
	sndptr = lpData + (samplecount % bufsamples) * wavfmt.nBlockAlign;
    }

/*
    if (diff < 0)
	vsynctime = basevsynctime * 5 / 6;
    else if (diff > 0)
	vsynctime = basevsynctime * 7 / 6;
*/
    return;
}

int init_sound (void)
{
    int rate;

    if ((rate = init_sound_win32 ()) < 2)
	return rate;

    scaled_sample_evtime = (unsigned long) maxhpos * maxvpos * vblank_hz * CYCLE_UNIT / rate;
    scaled_sample_evtime_ok = 1;

    if (currprefs.sound_bits == 16) {
	init_sound_table16 ();
	sample_handler = currprefs.stereo ? sample16s_handler : sample16_handler;
    } else {
	init_sound_table8 ();
	sample_handler = currprefs.stereo ? sample8s_handler : sample8_handler;
    }

    write_log ("Sound driver found and configured for %d bits at %d Hz %s\n",
			 currprefs.sound_bits, rate, currprefs.stereo ? "stereo" : "");

    return 1;
}

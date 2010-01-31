#include "sysconfig.h"
#include "sysdeps.h"

#include "config.h"
#include "options.h"

#include <windows.h>
#include <dsound.h>

static LPDIRECTSOUND lpDS;
static LPDIRECTSOUNDBUFFER lpDSBprimary;
static LPDIRECTSOUNDBUFFER lpDSBsecondary;

static char *DirectSound_ErrorText (HRESULT error)
{
    switch( error )
    {
    case DSERR_ALLOCATED:
        return "Allocated";
    case DSERR_CONTROLUNAVAIL:
        return "Control Unavailable";
    case DSERR_INVALIDPARAM:
        return "Invalid Parameter";
    case DSERR_INVALIDCALL:
        return "Invalid Call";
    case DSERR_GENERIC:
        return "Generic";
    case DSERR_PRIOLEVELNEEDED:
        return "Priority Level Needed";
    case DSERR_OUTOFMEMORY:
        return "Out of Memory";
    case DSERR_BADFORMAT:
        return "Bad Format";
    case DSERR_UNSUPPORTED:
        return "Unsupported";
    case DSERR_NODRIVER:
        return "No Driver";
    case DSERR_ALREADYINITIALIZED:
        return "Already Initialized";
    case DSERR_NOAGGREGATION:
        return "No Aggregation";
    case DSERR_BUFFERLOST:
        return "Buffer Lost";
    case DSERR_OTHERAPPHASPRIO:
        return "Other Application Has Priority";
    case DSERR_UNINITIALIZED:
        return "Uninitialized";
    case DSERR_NOINTERFACE:
        return "No Interface";
    default:
        return "Unknown";
    }
}
static char *DirectSound_ErrorString (HRESULT dsval)
{
    static char dserr[200];
    sprintf(dserr, "%08.8X S=%d F=%04.4X C=%04.4X (%d) (%s)",
	dsval, (dsval & 0x80000000) ? 1 : 0,
	HRESULT_FACILITY(dsval),
	HRESULT_CODE(dsval),
	HRESULT_CODE(dsval),
	DirectSound_ErrorText (dsval));
    return dserr;
}

static void clearbuffer (void)
{
    void *buffer;
    DWORD size;

    HRESULT hr = IDirectSoundBuffer_Lock (lpDSBsecondary, 0, dsoundbuf, &buffer, &size, NULL, NULL, 0);
    if (hr == DSERR_BUFFERLOST) {
	IDirectSoundBuffer_Restore (lpDSBsecondary);
	hr = IDirectSoundBuffer_Lock (lpDSBsecondary, 0, dsoundbuf, &buffer, &size, NULL, NULL, 0);
    }
    if (hr != DS_OK) {
	write_log ("failed to Lock sound buffer (clear): %s\n", DirectSound_ErrorString (hr));
	return;
    }
    memset (buffer, 0, size);
    IDirectSoundBuffer_Unlock (lpDSBsecondary, buffer, size, NULL, 0);
    memset (sndbuffer, 0, sizeof (sndbuffer));
}

static void pause_audio_ds (void)
{
    IDirectSoundBuffer_Stop (lpDSBsecondary);
    clearbuffer ();
}

static void resume_audio_ds (void)
{
    HRESULT hr;
    
    paused = 0;
    clearbuffer ();
    hr = IDirectSoundBuffer_Play (lpDSBsecondary, 0, 0, DSBPLAY_LOOPING);
    if (hr != DS_OK)
	write_log ("Play failed: %s\n", DirectSound_ErrorString (hr));
    writepos = snd_configsize;
}

static int restore (DWORD hr)
{
    if (hr != DSERR_BUFFERLOST)
	return 0;
    hr = IDirectSoundBuffer_Restore (lpDSBsecondary);
    if (hr != DS_OK) {
	return 1;
    }
    resume_audio_ds ();
    return 1;
}

static void close_audio_ds (void)
{
    if (lpDSBsecondary)
	IDirectSound_Release (lpDSBsecondary);
    if (lpDSBprimary)
	IDirectSound_Release (lpDSBprimary);
    lpDSBsecondary = lpDSBprimary = 0;
    if (lpDS) {
	IDirectSound_Release (lpDS);
	write_log ("DirectSound driver freed\n");
    }
    lpDS = 0;
}

extern HWND hMainWnd;

static int open_audio_ds (int size)
{
    HRESULT hr;
    DSBUFFERDESC sound_buffer;
    DSCAPS DSCaps;
    DSBCAPS DSBCaps;
    WAVEFORMATEX wavfmt;
    int minfreq, maxfreq;
    int freq = currprefs.sound_freq;
    
    enumerate_sound_devices (0);
    size <<= 1;
    if (currprefs.stereo)
        size <<= 1;
    snd_configsize = size;
    sndbufsize = size / 32;
    if (sndbufsize > SND_MAX_BUFFER)
        sndbufsize = SND_MAX_BUFFER;

    hr = DirectSoundCreate (&sound_device_guid[currprefs.win32_soundcard], &lpDS, NULL);
    if (hr != DS_OK)  {
        write_log ("SOUND: DirectSoundCreate() failure: %s\n", DirectSound_ErrorString (hr));
        return 0;
    }
    memset (&DSCaps, 0, sizeof (DSCaps));
    DSCaps.dwSize = sizeof(DSCaps);
    hr = IDirectSound_GetCaps (lpDS, &DSCaps);
    if (hr!= DS_OK) {
	write_log ("SOUND: Error getting DirectSound capabilities: %s\n", DirectSound_ErrorString (hr));
	goto error;
    }
    if (DSCaps.dwFlags & DSCAPS_EMULDRIVER) {
	write_log ("SOUND: Emulated DirectSound driver detected, don't complain if sound quality is crap :)\n");
    }
    minfreq = DSCaps.dwMinSecondarySampleRate;
    maxfreq = DSCaps.dwMaxSecondarySampleRate;
    if (maxfreq > 11000) {
	if (minfreq > freq) {
	    freq = minfreq;
	    write_log("SOUND: minimum supported frequency: %d\n", minfreq);
	}
	if (maxfreq < freq) {
	    freq = maxfreq;
	    write_log("SOUND: maximum supported frequency: %d\n", maxfreq);
	}
    } else {
	write_log("SOUND: ignored weird min (%d) or max (%d) sample rate\n", minfreq, maxfreq);
    }
    memset (&sound_buffer, 0, sizeof (sound_buffer));
    sound_buffer.dwSize = sizeof (sound_buffer);
    sound_buffer.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_GETCURRENTPOSITION2;
    hr = IDirectSound_CreateSoundBuffer (lpDS, &sound_buffer, &lpDSBprimary, NULL);
    if( hr != DS_OK )  {
        write_log ("SOUND: Primary CreateSoundBuffer() failure: %s\n", DirectSound_ErrorString (hr));
	goto error;
    }

    memset(&DSBCaps, 0, sizeof(DSBCaps));
    DSBCaps.dwSize = sizeof(DSBCaps);
    hr = IDirectSoundBuffer_GetCaps(lpDSBprimary, &DSBCaps);
    if (hr != DS_OK) {
	write_log ("SOUND: Primary GetCaps() failure: %s\n",  DirectSound_ErrorString (hr));
	goto error;
    }

    wavfmt.wFormatTag = WAVE_FORMAT_PCM;
    wavfmt.nChannels = currprefs.stereo ? 2 : 1;
    wavfmt.nSamplesPerSec = freq;
    wavfmt.wBitsPerSample = 16;
    wavfmt.nBlockAlign = 16 / 8 * wavfmt.nChannels;
    wavfmt.nAvgBytesPerSec = wavfmt.nBlockAlign * freq;

    hr = IDirectSound_SetCooperativeLevel (lpDS, hMainWnd, DSSCL_PRIORITY);
    if (hr != DS_OK) {
        write_log ("SOUND: Can't set cooperativelevel: %s\n", DirectSound_ErrorString (hr));
        goto error;
    }
    if (dsoundbuf < DSBSIZE_MIN)
        dsoundbuf = DSBSIZE_MIN;
    if (dsoundbuf > DSBSIZE_MAX)
        dsoundbuf = DSBSIZE_MAX;
    if (max_sndbufsize > dsoundbuf)
        max_sndbufsize = dsoundbuf;

    memset (&sound_buffer, 0, sizeof (sound_buffer));
    sound_buffer.dwSize = sizeof (sound_buffer);
    sound_buffer.dwBufferBytes = dsoundbuf;
    sound_buffer.lpwfxFormat = &wavfmt;
    sound_buffer.dwFlags = DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_GLOBALFOCUS | DSBCAPS_STATIC;

    hr = IDirectSound_CreateSoundBuffer( lpDS, &sound_buffer, &lpDSBsecondary, NULL );
    if (hr != DS_OK) {
        write_log ("SOUND: Secondary CreateSoundBuffer() failure: %s\n", DirectSound_ErrorString (hr));
        goto error;
    }

    hr = IDirectSoundBuffer_SetFormat (lpDSBprimary, &wavfmt);
    if( hr != DS_OK )  {
        write_log ("SOUND: Primary SetFormat() failure: %s\n", DirectSound_ErrorString (hr));
        goto error;
    }
    
    clearbuffer ();

    init_sound_table16 ();
    if (dsound_hardware_mixing)
	sample_handler = sample16ss_handler;
    else
	sample_handler = currprefs.stereo ? sample16s_handler : sample16_handler;

    write_log ("DS driver '%s'/%d/%d bits/%d Hz/buffer %d/dist %d\n",
	sound_devices[currprefs.win32_soundcard], dsound_hardware_mixing ? 4 : (currprefs.stereo ? 2 : 1), 16, freq, max_sndbufsize, snd_configsize);
    obtainedfreq = currprefs.sound_freq;

    return 1;

error:
    close_audio_ds ();
    return 0;
}

static int open_sound (void)
{
    int ret;
    int size = currprefs.sound_maxbsiz;

    if (!currprefs.produce_sound)
	return 0;
    /* Always interpret buffer size as number of samples, not as actual
       buffer size.  Of course, since 8192 is the default, we'll have to
       scale that to a sane value (assuming that otherwise 16 bits and
       stereo would have been enabled and we'd have done the shift by
       two anyway).  */
    size >>= 2;
    if (size & (size - 1))
	size = DEFAULT_SOUND_MAXB;
    if (size < 512)
	size = 512;

    ret = open_audio_ds (size);
    if (!ret)
	return 0;

    have_sound = 1;
    sound_available = 1;
    update_sound (vblank_hz);
    scaled_sample_evtime_ok = 1;
    sndbufpt = sndbuffer;

    return 1;
}

void close_sound (void)
{
    if (! have_sound)
	return;
    pause_sound ();
    close_audio_ds ();
    have_sound = 0;
}

int init_sound (void)
{
    if (have_sound)
	return 1;
    if (!open_sound ())
	return 0;
    paused = 1;
    resume_sound ();
    return 1;
}

void pause_sound (void)
{
    if (paused)
	return;
    paused = 1;
    if (!have_sound)
	return;
    pause_audio_ds ();
}

void resume_sound (void)
{
    if (!paused)
	return;
    if (!have_sound)
	return;
    resume_audio_ds ();
}

#ifdef JIT
extern uae_u8* compiled_code;
#endif
extern int vsynctime_orig;

static void setadjust (double v)
{
    double mult;

    mult = (1000.0 + currprefs.sound_adjust + v);
    if (currprefs.gfx_vsync && currprefs.gfx_afullscreen) {
	vsynctime = vsynctime_orig;
	scaled_sample_evtime = (long)(((double)scaled_sample_evtime_orig) * mult / 1000.0);
#ifdef JIT
    } else if (compiled_code) {
	vsynctime = (long)(((double)vsynctime_orig) * mult / 1000.0);
	scaled_sample_evtime = scaled_sample_evtime_orig;
#endif
    } else
	vsynctime = vsynctime_orig * 9 / 10;
}

static void finish_sound_buffer_ds (void)
{
    DWORD playpos, safepos;
    HRESULT hr;
    void *b1, *b2;
    DWORD s1, s2;
    int diff;
    double vdiff, m, skipmode;
   
    for (;;) {
	hr = IDirectSoundBuffer_GetCurrentPosition (lpDSBsecondary, &playpos, &safepos);
	if (hr != DS_OK) {
	    restore (hr);
	    //write_log ("GetCurrentPosition failed: %s\n", DirectSound_ErrorText (hr));
	    return;
	}

	if (writepos >= playpos)
	    diff = writepos - playpos;
	else
	    diff = dsoundbuf - playpos + writepos;

	if (diff >= max_sndbufsize) {
	    writepos = safepos + snd_configsize;
	    if (writepos >= dsoundbuf)
		writepos -= dsoundbuf;
	    diff = snd_configsize;
	    break;
	}

	if (diff > max_sndbufsize * 6 / 8) {
	    sleep_millis_busy (1);
	    continue;
	}
	break;
    }

    hr = IDirectSoundBuffer_Lock (lpDSBsecondary, writepos, sndbufsize, &b1, &s1, &b2, &s2, 0);
    if (restore (hr))
	return;
    if (hr != DS_OK) {
        write_log ("lock failed: %s (%d %d)\n", DirectSound_ErrorText (hr), writepos, sndbufsize);
        return;
    }
    memcpy (b1, sndbuffer, sndbufsize >= s1 ? s1 : sndbufsize);
    if (b2)
        memcpy (b2, (uae_u8*)sndbuffer + s1, sndbufsize - s1);
    IDirectSoundBuffer_Unlock (lpDSBsecondary, b1, s1, b2, s2);

    vdiff = diff - snd_configsize;
    m = 100.0 * vdiff / max_sndbufsize;
    skipmode = pow (m < 0 ? -m : m, EXP)/ 10.0;

    if (m < 0) skipmode = -skipmode;
    if (skipmode < -ADJUST_SIZE) skipmode = -ADJUST_SIZE;
    if (skipmode > ADJUST_SIZE) skipmode = ADJUST_SIZE;

#ifdef SOUND_DEBUG
    if (!(timeframes % 10)) {
	write_log ("b=%5d,%5d,%5d,%5d diff=%5d vdiff=%5.0f vdiff2=%5d skip=%+02.1f\n",
	    sndbufsize, snd_configsize, max_sndbufsize, dsoundbuf, diff, vdiff, diff - snd_configsize, skipmode);
    }
#endif

    writepos += sndbufsize;
    if (writepos >= dsoundbuf)
	writepos -= dsoundbuf;

    setadjust (skipmode);
}

static void filtercheck (uae_s16 *sndbuffer, int len)
{
    int ch = dsound_hardware_mixing ? 4 : (currprefs.stereo ? 2 : 1);
    int i;
    static double cold[4];
    double old0, old1, v;
    
    if (!currprefs.sound_filter)
	return;
    if (gui_data.powerled || currprefs.sound_filter == 2) {
	if (ch == 1) {
	    old0 = cold[0];
	    for (i = 0; i < len; i++) {
		v = old0 = old0 * filter_mul1 + filter_mul2 * sndbuffer[i];
		if (v < -32768) v = -32768;
		if (v > 32767) v = 32767;
		sndbuffer[i] = v;
	    }
	    cold[0] = old0;
	} else {
	    old0 = cold[0];
	    old1 = cold[1];
	    for (i = 0; i < len; i += 2) {
		v = old0 = old0 * filter_mul1 + filter_mul2 * sndbuffer[i];
		if (v < -32768) v = -32768;
		if (v > 32767) v = 32767;
		sndbuffer[i] = v;
		v = old1 = old1 * filter_mul1 + filter_mul2 * sndbuffer[i + 1];
		if (v < -32768) v = -32768;
		if (v > 32767) v = 32767;
		sndbuffer[i + 1] = v;
	    }
	    cold[0] = old0;
	    cold[1] = old1;
	}
    }
}

void finish_sound_buffer (void)
{
    if (!have_sound || turbo_emulation)
	return;
    filtercheck ((uae_s16*)sndbuffer, sndbufsize / 2);
#ifdef AVIOUTPUT
    if (avioutput_audio)
        AVIOutput_WriteAudio ((uae_u8*)sndbuffer, sndbufsize);
    if (avioutput_video)
	memset (sndbuffer, 0, sndbufsize);
#endif
    finish_sound_buffer_ds ();
}

static BOOL CALLBACK DSEnumProc(LPGUID lpGUID, LPCTSTR lpszDesc, LPCTSTR lpszDrvName,  LPVOID lpContext)
{
    int i = num_sound_devices;
    if (i == MAX_SOUND_DEVICES)
	return TRUE;
    if (lpGUID != NULL)
	memcpy (&sound_device_guid[i], lpGUID, sizeof (GUID));
    sound_devices[i] = my_strdup (lpszDesc);
    num_sound_devices++;
    return TRUE;
}

char **enumerate_sound_devices (int *total)
{
    if (!num_sound_devices)
        DirectSoundEnumerate ((LPDSENUMCALLBACK)DSEnumProc, 0);
    if (total)
	*total = num_sound_devices;
    if (currprefs.win32_soundcard >= num_sound_devices)
	currprefs.win32_soundcard = 0;
    if (num_sound_devices)
	return sound_devices;
    return 0;
}

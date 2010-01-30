/* 
 * UAE - The Un*x Amiga Emulator
 *
 * Not a parser, but parallel and serial emulation for Win32
 *
 * Copyright 1997 Mathias Ortmann
 */

#include "config.h"
#include "sysconfig.h"

#include <windows.h>
#include <ddraw.h>
#include <stdlib.h>
#include <stdarg.h>
#include <commctrl.h>
#include <commdlg.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <io.h>

#include "sysdeps.h"
#include "options.h"
#include "gensound.h"
#include "sounddep/sound.h"
#include "events.h"
#include "uae.h"
#include "include/memory.h"
#include "custom.h"
#include "osdep/win32gui.h"

#define PRTBUFSIZE 1024
UINT prttimer;
char prtbuf[PRTBUFSIZE];
int prtbufbytes;
HANDLE hPrt = INVALID_HANDLE_VALUE;
extern HWND hAmigaWnd;

void flushprtbuf (void)
{
    DWORD written;

    if (hPrt == INVALID_HANDLE_VALUE)
	hPrt = CreateFile (prtname, GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, NULL);
    if (hPrt == INVALID_HANDLE_VALUE)
	hPrt = CreateFile (prtname, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);

    if (hPrt != INVALID_HANDLE_VALUE)
	WriteFile (hPrt, prtbuf, prtbufbytes, &written, 0);

    prtbufbytes = 0;
}

void finishjob (void)
{
    flushprtbuf ();
    if (hPrt != INVALID_HANDLE_VALUE) {
	CloseHandle (hPrt);
	hPrt = INVALID_HANDLE_VALUE;
    }
    KillTimer (hAmigaWnd, prttimer);
    prttimer = 0;
}

void putprinter (char val)
{
    PostMessage (hAmigaWnd, WM_USER + 0x200, val, 0);
}

void DoSomeWeirdPrintingStuff (WPARAM wParam)
{
    if (prttimer)
	KillTimer (hAmigaWnd, prttimer);
    if (prtbufbytes < PRTBUFSIZE) {
	prtbuf[prtbufbytes++] = wParam;
	prttimer = SetTimer (hAmigaWnd, 1, 2000, NULL);
    } else {
	flushprtbuf ();
	*prtbuf = wParam;
	prtbufbytes = 1;
	prttimer = 0;
    }
}

static HANDLE hCom = INVALID_HANDLE_VALUE;
static HANDLE writeevent;
static DCB dcb;

char inbuf[1024], outbuf[1024];
int inptr, inlast, outlast;

int openser (char *sername)
{
    char buf[32];
    COMMTIMEOUTS CommTimeOuts;

    sprintf (buf, "\\.\\\\%s", sername);

    if (!(writeevent = CreateEvent (NULL, TRUE, FALSE, NULL))) {
	write_log ("Failed to create serial event!\n");
	return 0;
    }
    if ((hCom = CreateFile (buf, GENERIC_READ | GENERIC_WRITE,
			    0,
			    NULL,
			    OPEN_EXISTING,
			    FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
			    NULL)) != INVALID_HANDLE_VALUE) {
	SetCommMask (hCom, EV_RXFLAG);
	SetupComm (hCom, 4096, 4096);
	PurgeComm (hCom, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
	CommTimeOuts.ReadIntervalTimeout = 0xFFFFFFFF;
	CommTimeOuts.ReadTotalTimeoutMultiplier = 0;
	CommTimeOuts.ReadTotalTimeoutConstant = 0;
	CommTimeOuts.WriteTotalTimeoutMultiplier = 0;
	CommTimeOuts.WriteTotalTimeoutConstant = 0;
	SetCommTimeouts (hCom, &CommTimeOuts);

	GetCommState (hCom, &dcb);

	dcb.BaudRate = 9600;
	dcb.ByteSize = 8;
	dcb.Parity = 0;
	dcb.StopBits = NOPARITY;

	dcb.fOutxCtsFlow = TRUE;
	dcb.fOutxDsrFlow = FALSE;
	dcb.fDtrControl = DTR_CONTROL_ENABLE;
	dcb.fTXContinueOnXoff = FALSE;
	dcb.fOutX = FALSE;
	dcb.fInX = FALSE;
	dcb.fNull = FALSE;
	dcb.fRtsControl = RTS_CONTROL_HANDSHAKE;
	dcb.fAbortOnError = FALSE;

	if (SetCommState (hCom, &dcb)) {
	    write_log ("Using %s\n", sername);
	    return 1;
	}
	CloseHandle (hCom);
	hCom = INVALID_HANDLE_VALUE;
    }
    return 0;
}

void closeser (void)
{
    if (hCom != INVALID_HANDLE_VALUE) {
	CloseHandle (hCom);
	hCom = INVALID_HANDLE_VALUE;
    }
}

void doserout (void)
{
    DWORD dwErrorFlags;
    unsigned long actual;
    OVERLAPPED ol = { 0 };

    if (hCom != INVALID_HANDLE_VALUE) {
	if (outlast) {
	    ResetEvent (ol.hEvent = writeevent);

	    actual = 0;
	    if (!WriteFile (hCom, outbuf, outlast, &actual, &ol)) {
		while (outlast -= actual) {
		    if ((dwErrorFlags = GetLastError ()) == ERROR_IO_INCOMPLETE || dwErrorFlags == ERROR_IO_PENDING) {
			actual = 0;
			GetOverlappedResult (hCom, &ol, &actual, FALSE);

			if ((dwErrorFlags = GetLastError ()) != ERROR_IO_INCOMPLETE && dwErrorFlags != ERROR_IO_PENDING) {
			    write_log ("writeser: error %d, lost %d chars!\n", GetLastError (), outlast - actual);
			    outlast = 0;
			    break;
			}
			if (WaitForSingleObject (writeevent, 1000) == WAIT_TIMEOUT) {
			    write_log ("writeser: timeout, lost %d chars!\n", outlast - actual);
			    outlast = 0;
			    break;
			}
		    } else {
			if (dwErrorFlags) {
			    write_log ("writeser: error %d while writing, lost %d chars!\n", dwErrorFlags, outlast - actual);
			    ClearCommError (hCom, &dwErrorFlags, NULL);
			}
			outlast = 0;
			break;
		    }
		}
	    }
	}
    } else {
	outlast = 0;
	inptr = inlast = 0;
    }
}

void writeser (char c)
{
    outbuf[outlast++] = c;
    if (outlast == sizeof outbuf)
	doserout ();
}

int readser (char *buffer)
{
    COMSTAT ComStat;
    DWORD dwErrorFlags;
    DWORD result;
    unsigned long actual;
    OVERLAPPED ol =
    {0};

    if (inptr < inlast) {
	*buffer = inbuf[inptr++];
	return 1;
    }
    if (hCom != INVALID_HANDLE_VALUE) {
	inptr = inlast = 0;

	/* only try to read number of bytes in queue */
	ClearCommError (hCom, &dwErrorFlags, &ComStat);
	if (ComStat.cbInQue) {
	    if (!ReadFile (hCom, inbuf, min (ComStat.cbInQue, sizeof inbuf), &inlast, &ol)) {
		if (GetLastError () == ERROR_IO_PENDING) {
		    write_log ("readser: INTERNAL ERROR - intermittent loss of serial data!\n");
		    for (;;) {
			actual = 0;
			result = GetOverlappedResult (hCom, &ol, &actual, TRUE);
			inlast += actual;

			if (result)
			    break;

			if (GetLastError () != ERROR_IO_INCOMPLETE) {
			    ClearCommError (hCom, &dwErrorFlags, &ComStat);
			    break;
			}
		    }
		} else
		    ClearCommError (hCom, &dwErrorFlags, &ComStat);
	    }
	}
	if (inptr < inlast) {
	    *buffer = inbuf[inptr++];
	    return 1;
	}
    }
    return 0;
}

void getserstat (int *status)
{
    DWORD stat;

    *status = 0;

    if (hCom != INVALID_HANDLE_VALUE) {
	GetCommModemStatus (hCom, &stat);
/* @@@ This "ouch" section was #ifdef 0 before - BDK */
#if 0
#define TIOCM_CAR 1
#define TIOCM_DSR 2
    /* ouch */
	if (stat & MS_RLSD_ON)
	    *status |= TIOCM_CAR;
	if (stat & MS_DSR_ON)
	    *status |= TIOCM_DSR;
#endif
    }
}

int setbaud (long baud)
{
    if (hCom != INVALID_HANDLE_VALUE) {
	if (GetCommState (hCom, &dcb)) {
	    dcb.BaudRate = baud;
	    if (!SetCommState (hCom, &dcb))
		write_log ("Error setting baud rate %d!\n", baud);
	} else
	    write_log ("setbaud: internal error!\n");
    }
    return 0;
}


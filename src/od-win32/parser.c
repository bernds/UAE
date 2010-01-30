/* 
 * UAE - The Un*x Amiga Emulator
 *
 * Not a parser, but parallel and serial emulation for Win32
 *
 * Copyright 1997 Mathias Ortmann
 * Copyright 1998-1999 Brian King - added MIDI output support
 */

#include "config.h"
#include "sysconfig.h"
#include <windows.h>
#include <winspool.h>
#include <stdlib.h>
#include <stdarg.h>
#include <mmsystem.h>
#include <ddraw.h>
#include <commctrl.h>
#include <commdlg.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <io.h>

#include "sysdeps.h"
#include "options.h"
#include "gensound.h"
#include "events.h"
#include "uae.h"
#include "memory.h"
#include "custom.h"
#include "osdep/win32gui.h"
#include "osdep/parser.h"
#include "osdep/midi.h"

UINT prttimer;
char prtbuf[PRTBUFSIZE];
int prtbufbytes,wantwrite;
HANDLE hPrt = INVALID_HANDLE_VALUE;
DWORD  dwJob;
extern HWND hAmigaWnd;
 OVERLAPPED ol = { 0 };

void flushprtbuf (void)
{
    DWORD written = 0;

    if (hPrt != INVALID_HANDLE_VALUE)
    {
	if( WritePrinter( hPrt, prtbuf, prtbufbytes, &written ) )
	{
	    if( written != prtbufbytes )
		write_log( "PRINTER: Only wrote %d of %d bytes!\n", written, prtbufbytes );
	}
	else
	{
	    write_log( "PRINTER: Couldn't write data!\n" );
	}
    }
    else
    {
	write_log( "PRINTER: Not open!\n" );
    }
    prtbufbytes = 0;
}

void finishjob (void)
{
    flushprtbuf ();
}

void DoSomeWeirdPrintingStuff( char val )
{
    if (prttimer)
	KillTimer (hAmigaWnd, prttimer);
    if (prtbufbytes < PRTBUFSIZE) {
	prtbuf[prtbufbytes++] = val;
	prttimer = SetTimer (hAmigaWnd, 1, 2000, NULL);
    } else {
	flushprtbuf ();
	*prtbuf = val;
	prtbufbytes = 1;
	prttimer = 0;
    }
}

FILE *openprinter( void )
{
    FILE *result = NULL;
    DOC_INFO_1 DocInfo;

    if( ( hPrt == INVALID_HANDLE_VALUE ) && *currprefs.prtname )
    {
	if( OpenPrinter(currprefs.prtname, &hPrt, NULL ) )
	{
	    // Fill in the structure with info about this "document."
	    DocInfo.pDocName = "My Document";
	    DocInfo.pOutputFile = NULL;
	    DocInfo.pDatatype = "RAW";
	    // Inform the spooler the document is beginning.
	    if( (dwJob = StartDocPrinter( hPrt, 1, (LPSTR)&DocInfo )) == 0 )
	    {
		ClosePrinter( hPrt );
		hPrt = INVALID_HANDLE_VALUE;
	    }
	    else if( StartPagePrinter( hPrt ) )
	    {
		result = 1;
	    }
	}
	else
	{
	    hPrt = INVALID_HANDLE_VALUE; // Stupid bug in Win32, where OpenPrinter fails, but hPrt ends up being zero
	}
    }
    if( hPrt != INVALID_HANDLE_VALUE )
    {
	write_log( "PRINTER: Opening printer \"%s\" with handle 0x%x.\n", currprefs.prtname, hPrt );
    }
    else if( *currprefs.prtname )
    {
	write_log( "PRINTER: ERROR - Couldn't open printer \"%s\" for output.\n", currprefs.prtname );
    }

    return result;
}

void closeprinter( void )
{
    if( hPrt != INVALID_HANDLE_VALUE )
    {
	EndPagePrinter( hPrt );
	EndDocPrinter( hPrt );
	ClosePrinter( hPrt );
	hPrt = INVALID_HANDLE_VALUE;
	write_log( "PRINTER: Closing printer.\n" );
    }
    KillTimer( hAmigaWnd, prttimer );
    prttimer = 0;
}

void putprinter (char val)
{
    DoSomeWeirdPrintingStuff( val );
}

static HANDLE hCom = INVALID_HANDLE_VALUE;
static HANDLE writeevent = NULL;
static DCB dcb;

char inbuf[1024], outbuf[1024];
int inptr, inlast, outlast;

int openser (char *sername)
{
    char buf[32];
    COMMTIMEOUTS CommTimeOuts;

    sprintf (buf, "\\.\\\\%s", sername);

    if (!(writeevent = CreateEvent (NULL, TRUE, FALSE, NULL))) {
	write_log ("SERIAL: Failed to create event!\n");
	return 0;
    }
    if ((hCom = CreateFile (buf, GENERIC_READ | GENERIC_WRITE,
			    0,
			    NULL,
			    OPEN_EXISTING,
			    FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
			    NULL)) != INVALID_HANDLE_VALUE) {
	SetCommMask (hCom, EV_RXFLAG);
	SetupComm (hCom, 65536,128);
	PurgeComm (hCom, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
	CommTimeOuts.ReadIntervalTimeout = 0xFFFFFFFF;
	CommTimeOuts.ReadTotalTimeoutMultiplier = 0;
	CommTimeOuts.ReadTotalTimeoutConstant = 0;
	CommTimeOuts.WriteTotalTimeoutMultiplier = 0;
	CommTimeOuts.WriteTotalTimeoutConstant = 0;
	SetCommTimeouts (hCom, &CommTimeOuts);

	GetCommState (hCom, &dcb);

	dcb.BaudRate = 9600;
	//dcb.ByteSize = 8;
	//dcb.Parity = 0;
	//dcb.StopBits = NOPARITY;

	//dcb.fOutxCtsFlow = TRUE;
	//dcb.fOutxDsrFlow = FALSE;
	//dcb.fDtrControl = DTR_CONTROL_ENABLE;
	//dcb.fTXContinueOnXoff = FALSE;
	//dcb.fOutX = FALSE;
	//dcb.fInX = FALSE;
	//dcb.fNull = FALSE;
	//dcb.fRtsControl = RTS_CONTROL_HANDSHAKE;
	//dcb.fAbortOnError = FALSE;

	if (SetCommState (hCom, &dcb)) {
	    write_log ("SERIAL: Using %s\n", sername);
	    return 1;
	}
	CloseHandle (hCom);
	hCom = INVALID_HANDLE_VALUE;
    }
    return 0;
}

void closeser (void)
{
    if (hCom != INVALID_HANDLE_VALUE) 
    {
	    CloseHandle (hCom);
	    hCom = INVALID_HANDLE_VALUE;
    }
    if( writeevent )
	CloseHandle( writeevent );
}

void doserout( void )
{
    DWORD dwErrorFlags;
    unsigned long actual;

    if (hCom != INVALID_HANDLE_VALUE) 
    {
        if (outlast) 
        {
            ResetEvent (ol.hEvent = writeevent);
            actual = 0;
			            
            if (!WriteFile (hCom, outbuf, outlast, &actual, &ol)) 
            {
			 //GetOverlappedResult (hCom, &ol, &actual, FALSE);

             /*   while (outlast -= actual) 
                {
                    if ((dwErrorFlags = GetLastError ()) == ERROR_IO_INCOMPLETE || dwErrorFlags == ERROR_IO_PENDING) 
                    {
			actual = 0;
			GetOverlappedResult (hCom, &ol, &actual, FALSE);

			if ((dwErrorFlags = GetLastError ()) != ERROR_IO_INCOMPLETE && dwErrorFlags != ERROR_IO_PENDING) 
                        {
			    write_log ("writeser: error %d, lost %d chars!\n", GetLastError (), outlast - actual);
			    outlast = 0;
			    break;
			}
			if (WaitForSingleObject (writeevent, 100) == WAIT_TIMEOUT) 
                        {
			    write_log ("writeser: timeout, lost %d chars!\n", outlast - actual);
			    outlast = 0;
			    break;
			}
    		    }
                    else
                    {
			if (dwErrorFlags) 
                        {
			    write_log ("writeser: error %d while writing, lost %d chars!\n", dwErrorFlags, outlast - actual);
			    ClearCommError (hCom, &dwErrorFlags, NULL);
			}
			
			outlast = 0;
			break;
		    }
			
		}
		*/
        outlast=0;
	    }
	}
    }
    else 
    {
	outlast = 0;
	inptr = inlast = 0;
    }
}

void writeser (char c)
{
	COMSTAT ComStat;
    DWORD dwErrorFlags;
	extern 	uae_u16 serdat;
    {
	outbuf[ outlast++ ] = c;
//serial
	if (outlast == 100/*sizeof outbuf*/)
	{
	    doserout();
        wantwrite=2000;
		return;
	}
    }
    serdat|=0x2000; /* Set TBE in the SERDATR ... */
    intreq|=1;      /* ... and in INTREQ register */
    INTREQ( 0x8000 | 0x01 );
}
int state,oldstatelw,oldstaterw;

void hsyncstuff( void )   //only generate Interrupts when 
//writebuffer is complete flushed
//check state of lwin rwin

{
    static int keycheck=0;
    extern 	uae_u16 serdat;
    extern int serdev;
    
    keycheck++;
    if(keycheck==1000)
    {
	state=GetAsyncKeyState(VK_LWIN);
	
	if (state!=oldstatelw){my_kbd_handler(VK_LWIN,0,state);
	oldstatelw=state;
	}
	state=GetAsyncKeyState(VK_RWIN);
	
	if (state!=oldstaterw){my_kbd_handler(VK_RWIN,0,state);
	oldstaterw=state;
	}
	keycheck=0;
    }
    
    if (!serdev)           /* || (serdat&0x4000)) */
	return;

    if(wantwrite)
    {
	int actual=0;
	if(wantwrite==1)
	{
	    serdat|=0x2000;
	    intreq|=0x1;
	    INTREQ(0x8000 | (0x01));
	    wantwrite=0;
	}
	wantwrite--;
	GetOverlappedResult (hCom, &ol, &actual, FALSE);           	
	if (actual==100)
	{		 serdat|=0x2000;
	intreq|=0x1;
	INTREQ(0x8000 | (0x01));
	wantwrite=0;
	}
    }
}

int readser (char *buffer)
{
    COMSTAT ComStat;
    DWORD dwErrorFlags;
    DWORD result;
    unsigned long actual;
    
    
    {
	
	
	//	
		//if(intreq&&1);
		//else
	/*if (WaitForSingleObject (writeevent,0) != WAIT_TIMEOUT) 
                        {
			 serdat|=0x2000;
		intreq|=0x1;
		INTREQ(0x8000 | (0x01));
		 ResetEvent (ol.hEvent = writeevent);

			}
	else {INTREQ(0x1);
	serdat &=0xdfff;
	}*/
	if (inptr < inlast) 
	{
	    *buffer = inbuf[inptr++];
	    return 1;
	}
	if (hCom != INVALID_HANDLE_VALUE) 
	{
	    inptr = inlast = 0;
	    
	    /* only try to read number of bytes in queue */
	    ClearCommError (hCom, &dwErrorFlags, &ComStat);
	    if (ComStat.cbInQue) 
	    {
		
		if (!ReadFile (hCom, inbuf, min (ComStat.cbInQue, sizeof inbuf), &inlast, &ol)) 
		{
		    if (GetLastError () == ERROR_IO_PENDING) 
		    {
			write_log ("readser: INTERNAL ERROR - intermittent loss of serial data!\n");
			for (;;) 
			{
			    actual = 0;
			    result = GetOverlappedResult (hCom, &ol, &actual, TRUE);
			    inlast += actual;
			    
			    if (result)
				break;
			    
			    if (GetLastError () != ERROR_IO_INCOMPLETE) 
			    {
				ClearCommError (hCom, &dwErrorFlags, &ComStat);
				break;
			    }
			}
		    }
		    else
		    {
			ClearCommError (hCom, &dwErrorFlags, &ComStat);
		    }
		}
	    }
	    if (inptr < inlast) 
	    {
		*buffer = inbuf[inptr++];
		return 1;
	    }
	}
    }
    return 0;
}

void getserstat (int *status)
{
    DWORD stat;

    *status = 0;
    if (hCom != INVALID_HANDLE_VALUE) {
	//GetCommModemStatus (hCom, &stat);
/* @@@ This "ouch" section was #ifdef 0 before - BDK */
#if 0
#define TIOCM_CAR 1
#define TIOCM_DSR 2
    /* ouch */
    if (stat & MS_CTS_ON)
	    *status |= TIOCM_CAR;
	if (stat & MS_RLSD_ON)
	    *status |= TIOCM_CAR;
	if (stat & MS_DSR_ON)
	    *status |= TIOCM_DSR;
#endif
    }
}

int setbaud (long baud)
{
    write_log( "Baud-rate is %d\n", baud );
    {
        if (hCom != INVALID_HANDLE_VALUE) 
        {
	        if (GetCommState (hCom, &dcb)) 
            {
	            dcb.BaudRate = baud;
	            if (!SetCommState (hCom, &dcb))
		        write_log ("SERIAL: Error setting baud rate %d!\n", baud);
	        } 
            else
            {
	            write_log ("SERIAL: setbaud internal error!\n");
            }
        }
    }
    return 0;
}


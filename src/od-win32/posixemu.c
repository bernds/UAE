/* 
 * UAE - The Un*x Amiga Emulator
 *
 * Win32 interface
 *
 * Copyright 1997 Mathias Ortmann
 */

#include "config.h"
#include "sysconfig.h"
#include "sysdeps.h"

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

#include "options.h"

/* stdioemu, posixemu, mallocemu, and various file system helper routines */
static DWORD lasterror;

static int isillegal (unsigned char *str)
{
    unsigned char a = *str, b = str[1], c = str[2];

    if (a >= 'a' && a <= 'z')
	a &= ~' ';
    if (b >= 'a' && b <= 'z')
	b &= ~' ';
    if (c >= 'a' && c <= 'z')
	c &= ~' ';

    return (a == 'A' && b == 'U' && c == 'X' ||
	    a == 'C' && b == 'O' && c == 'N' ||
	    a == 'P' && b == 'R' && c == 'N' ||
	    a == 'N' && b == 'U' && c == 'L');
}

static int checkspace (char *str, char s, char d)
{
    char *ptr = str;

    while (*ptr && *ptr == s)
	ptr++;

    if (!*ptr || *ptr == '/' || *ptr == '\\') {
	while (str < ptr)
	    *(str++) = d;
	return 0;
    }
    return 1;
}

/* This is sick and incomplete... in the meantime, I have discovered six new illegal file name formats
 * M$ sucks! */
void fname_atow (const char *src, char *dst, int size)
{
    char *lastslash = dst, *strt = dst, *posn = NULL, *temp = NULL;
    int i, j;

    temp = xmalloc( size );

    while (size-- > 0) {
	if (!(*dst = *src++))
	    break;

	if (*dst == '~' || *dst == '|' || *dst == '*' || *dst == '?') {
	    if (size > 2) {
		sprintf (dst, "~%02x", *dst);
		size -= 2;
		dst += 2;
	    }
	} else if (*dst == '/') {
	    if (checkspace (lastslash, ' ', 0xa0) && (dst - lastslash == 3 || (dst - lastslash > 3 && lastslash[3] == '.')) && isillegal (lastslash)) {
		i = dst - lastslash - 3;
		dst++;
		for (j = i + 1; j--; dst--)
		    *dst = dst[-1];
		*(dst++) = 0xa0;
		dst += i;
		size--;
	    } else if (*lastslash == '.' && (dst - lastslash == 1 || lastslash[1] == '.' && dst - lastslash == 2) && size) {
		*(dst++) = 0xa0;
		size--;
	    }
	    *dst = '\\';
	    lastslash = dst + 1;
	}
	dst++;
    }

    if (checkspace (lastslash, ' ', 0xa0) && (dst - lastslash == 3 || (dst - lastslash > 3 && lastslash[3] == '.')) && isillegal (lastslash) && size > 1) {
	i = dst - lastslash - 3;
	dst++;
	for (j = i + 1; j--; dst--)
	    *dst = dst[-1];
	*(dst++) = 0xa0;
    } else if (!strcmp (lastslash, ".") || !strcmp (lastslash, ".."))
	strcat (lastslash, "\xa0");

    /* Major kludge, because I can't find the problem... */
    if( ( posn = strstr( strt, "..\xA0\\" ) ) == strt && temp)
    {
        strcpy( temp, "..\\" );
        strcat( temp, strt + 4 );
        strcpy( strt, temp );
    }
}

static int hextol (char a)
{
    if (a >= '0' && a <= '9')
	return a - '0';
    if (a >= 'a' && a <= 'f')
	return a - 'a' + 10;
    if (a >= 'A' && a <= 'F')
	return a - 'A' + 10;
    return 2;
}

/* Win32 file name restrictions suck... */
void fname_wtoa (unsigned char *ptr)
{
    unsigned char *lastslash = ptr;

    while (*ptr) {
	if (*ptr == '~') {
	    *ptr = hextol (ptr[1]) * 16 + hextol (ptr[2]);
	    strcpy (ptr + 1, ptr + 3);
	} else if (*ptr == '\\') {
	    if (checkspace (lastslash, ' ', 0xa0) && ptr - lastslash > 3 && lastslash[3] == 0xa0 && isillegal (lastslash)) {
		ptr--;
		strcpy (lastslash + 3, lastslash + 4);
	    }
	    *ptr = '/';
	    lastslash = ptr + 1;
	}
	ptr++;
    }

    if (checkspace (lastslash, ' ', 0xa0) && ptr - lastslash > 3 && lastslash[3] == 0xa0 && isillegal (lastslash))
	strcpy (lastslash + 3, lastslash + 4);
}

/* pthread Win32 emulation */
void sem_init (HANDLE * event, int manual_reset, int initial_state)
{
    *event = CreateEvent (NULL, manual_reset, initial_state, NULL);
}

void sem_wait (HANDLE * event)
{
    WaitForSingleObject (*event, INFINITE);
}

void sem_post (HANDLE * event)
{
    SetEvent (*event);
}

int sem_trywait (HANDLE * event)
{
    return WaitForSingleObject (*event, 0) == WAIT_OBJECT_0;
}

/* Mega-klduge to prevent problems with Watcom's habit of passing
 * arguments in registers... */
static HANDLE thread_sem;
static void *(*thread_startfunc) (void *);
#ifndef __GNUC__
static void * __stdcall thread_starter (void *arg)
#else
static void * thread_starter( void *arg )
#endif
{
    void *(*func) (void *) = thread_startfunc;
    SetEvent (thread_sem);
    return (*func) (arg);
}

/* this creates high priority threads by default to speed up the file system (kludge, will be
 * replaced by a set_penguin_priority() routine soon) */
int start_penguin (void *(*f) (void *), void *arg, DWORD * foo)
{
    static int have_event = 0;
    HANDLE hThread;

    if (! have_event) {
	thread_sem = CreateEvent (NULL, 0, 0, NULL);
	have_event = 1;
    }
    thread_startfunc = f;
    hThread = CreateThread (NULL, 0, (LPTHREAD_START_ROUTINE) thread_starter, arg, 0, foo);
    SetThreadPriority (hThread, THREAD_PRIORITY_HIGHEST);
    WaitForSingleObject (thread_sem, INFINITE);
}

#ifndef HAVE_TRUNCATE
int truncate (const char *name, long int len)
{
    /* @@@ - there doesn't seem to be a way to truncate a file under Windows??? */
    return 0;
}
#endif

/*==========================================================================
 *
 *  Copyright (C) 1996 Brian King
 *
 *  File:       win32gui.c
 *  Content:    Win32-specific gui features for UAE port.
 *
 ***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <windows.h>
#include <winspool.h>
#include <winuser.h>
#ifdef _MSC_VER
#include <mmsystem.h>
#include <shlobj.h>
#include <commdlg.h>
#include <dlgs.h>
#include <process.h>
#include <prsht.h>
#include <richedit.h>
#include <shellapi.h>
#else
#include <shlobj.h>
#include <commdlg.h>
#include <richedit.h>
#endif

#include "resource.h"
#include "sysconfig.h"
#include "sysdeps.h"
#include "gui.h"
#include "options.h"
#include "memory.h"
#include "custom.h"
#include "newcpu.h"
#include "disk.h"
#include "uae.h"
#include "threaddep/thread.h"
#include "filesys.h"
#include "autoconf.h"

#include "picasso96.h"
#include "osdep/win32gui.h"
#include "osdep/win32.h"
#include "osdep/win32gfx.h"
#include "osdep/dxwrap.h"
#include "sounddep/sound.h"
#include "target.h"
#include "savestate.h"

#define DISK_FORMAT_STRING "(*.ADF;*.ADZ;*.GZ;*.DMS;*.FDI)\0*.ADF;*.ADZ;*.GZ;*.DMS;*.FDI\0"

static int allow_quit;
static int full_property_sheet;
static struct uae_prefs *pguiprefs;
static struct uae_prefs workprefs;

extern HWND (WINAPI *pHtmlHelp)(HWND, LPCSTR, UINT, LPDWORD );
#undef HtmlHelp
#ifndef HH_DISPLAY_TOPIC
#define HH_DISPLAY_TOPIC 0
#endif
#define HtmlHelp(a,b,c,d) if( pHtmlHelp ) (*pHtmlHelp)(a,b,c,(LPDWORD)d); else \
{ char szMessage[ MAX_PATH ]; WIN32GUI_LoadUIString( IDS_NOHELP, szMessage, MAX_PATH ); gui_message( szMessage ); }

extern HWND hAmigaWnd;
extern char help_file[ MAX_PATH ];

extern int mouseactive;
extern char *start_path;

extern char configname[256];
char config_filename[ MAX_PATH ] = "";

HANDLE win32uae_key = NULL;

drive_specs blankdrive =
{"", "", 1, 32, 1, 2, 0, 0};

#define Error(x) MessageBox( NULL, (x), "WinUAE Error", MB_OK )

void WIN32GUI_LoadUIString( DWORD id, char *string, DWORD dwStringLen )
{
    if( LoadString( hUIDLL ? hUIDLL : hInst, id, string, dwStringLen ) == 0 )
	LoadString( hInst, id, string, dwStringLen );
}

HWND hPropertySheet = NULL;

/* C_PAGES must be last */
enum {
    LOADSAVE_ID, MEMORY_ID, KICKSTART_ID, CPU_ID,
    DISPLAY_ID, CHIPSET_ID, SOUND_ID, FLOPPY_ID, HARDDISK_ID,
    PORTS_ID, MISC_ID, ABOUT_ID, C_PAGES
};

static HWND pages[C_PAGES];

#define MIN_CHIP_MEM 0
#define MAX_CHIP_MEM 4
#define MIN_FAST_MEM 0
#define MAX_FAST_MEM 4
#define MIN_SLOW_MEM 0
#define MAX_SLOW_MEM 2
#define MIN_Z3_MEM 0
#define MAX_Z3_MEM 10
#define MIN_P96_MEM 0
#define MAX_P96_MEM 6
#define MIN_M68K_PRIORITY 1
#define MAX_M68K_PRIORITY 16
#define MIN_CACHE_SIZE 0
#define MAX_CACHE_SIZE 8
#define MIN_REFRESH_RATE 1
#define MAX_REFRESH_RATE 10
#define MIN_SOUND_MEM 0
#define MAX_SOUND_MEM 6

static char szNone[ MAX_PATH ] = "None";

/* if drive is -1, show the full GUI, otherwise file-requester for DF[drive] */
void WIN32GUI_DisplayGUI( int drive )
{
    HRESULT ddrval;
    int flipflop = 0;

    if( ( !WIN32GFX_IsPicassoScreen() && currprefs.gfx_afullscreen && ( currprefs.gfx_width < 640 ) ) || 
	( WIN32GFX_IsPicassoScreen() && currprefs.gfx_pfullscreen && ( picasso96_state.Width < 640 ) ) )
	flipflop = 1;

    WIN32GFX_ClearPalette();

    if( !WIN32GFX_IsFullScreen() )
	setmouseactive (FALSE);
    else
    {
	if( DirectDraw_GetLockableType() == primary_surface )
	{
	    /* Blit our primary to our secondary, so we can then use the secondary to refresh from */
	    ddrval = DirectDraw_Blt( secondary_surface, NULL, primary_surface, NULL, DDBLT_WAIT, NULL );
	}
    }
    manual_painting_needed = 1; /* So that WM_PAINT will refresh the display */
    pause_sound(0);

    DirectDraw_FlipToGDISurface();

    if( drive == -1 )
    {
	if( flipflop )
	    ShowWindow( hAmigaWnd, SW_MINIMIZE );
	GetSettings(0);
	if( flipflop )
	    ShowWindow( hAmigaWnd, SW_RESTORE );
    }
    else
    {
	DiskSelection( hAmigaWnd, IDC_DF0+drive, 0, &changed_prefs );
    }
    resume_sound();
    WIN32GFX_SetPalette();
    manual_painting_needed = 0; /* So that WM_PAINT doesn't need to use custom refreshing */
    if( !WIN32GFX_IsFullScreen() || !mouseactive)
	SetCursor (NULL);

    // This is a hack to fix the fact that time is passing while the GUI was present,
    // and we don't want our frames-per-second calculation in drawing.c to get skewed.
#ifdef HAVE_GETTIMEOFDAY
    {
	struct timeval tv;
	gettimeofday(&tv,NULL);
	msecs = (tv.tv_sec-seconds_base) * 1000 + tv.tv_usec / 1000;
    }
#endif
}

static void prefs_to_gui (struct uae_prefs *p)
{
    workprefs = *p;
    workprefs.mountinfo = dup_mountinfo (p->mountinfo);
    /* Could also duplicate unknown lines, but no need - we never
       modify those.  */
}

static void gui_to_prefs (void)
{
    /* Always copy our prefs to changed_prefs, ... */
    changed_prefs = workprefs;

    /* ... but use currprefs for the mountinfo.  The filesystem code
       keeps a private copy, and only looks at currprefs once per reset.
       It never looks at changed_prefs.  */
    free_mountinfo (currprefs.mountinfo);
    currprefs.mountinfo = dup_mountinfo (workprefs.mountinfo);
}

#define TRACK_WRITE_LEN (6250*2)

static void disk_checksum(uae_u8 *p, uae_u8 *c)
{
    uae_u32 cs = 0;
    int i;

    for (i = 0; i < 512; i+= 4) cs += (p[i] << 24) | (p[i+1] << 16) | (p[i+2] << 8) | (p[i+3] << 0);
    cs = -cs;
    c[0] = cs >> 24; c[1] = cs >> 16; c[2] = cs >> 8; c[3] = cs >> 0;
}

static void disk_date (uae_u8 *p)
{
    time_t t;
    struct tm *today;
    int year, days, minutes, ticks;
    char tmp[10];
    
    time (&t);
    today = localtime( &t );
    strftime (tmp, sizeof(tmp), "%Y", today);
    year = atol (tmp);
    strftime (tmp, sizeof(tmp), "%j", today);
    days = atol (tmp) - 1;
    strftime (tmp, sizeof(tmp), "%H", today);
    minutes = atol (tmp) * 60;
    strftime (tmp, sizeof(tmp), "%M", today);
    minutes += atol (tmp);
    strftime (tmp, sizeof(tmp), "%S", today);
    ticks = atol (tmp) * 50;

    while (year > 1978) {
	if ( !(year % 100) ? !(year % 400) : !(year % 4) ) days++;
	days += 365;
	year--;
    }

    p[0] = days >> 24; p[1] = days >> 16; p[2] = days >> 8; p[3] = days >> 0;
    p[4] = minutes >> 24; p[5] = minutes >> 16; p[6] = minutes >> 8; p[7] = minutes >> 0;
    p[8] = ticks >> 24; p[9] = ticks >> 16; p[10] = ticks >> 8; p[11] = ticks >> 0; 
}

static void CreateDiskFile (char *name, int type)
{
    HANDLE adf;
    int i, j, file_size = 880 * 1024, tracks;
    char *chunk = NULL;
    DWORD count;
    uae_u8 tmp[3*4];
    char *disk_name = "empty";

    SetCursor (LoadCursor (NULL, IDC_WAIT));
    adf = CreateFile (name, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
    if (adf != INVALID_HANDLE_VALUE) {
	if ((chunk = xmalloc (16384)) != NULL) {
	memset(chunk,0,16384);
	switch(type)
	    {
	    case 0:
	    for (i = 0; i < file_size; i += 11264) {
		memset(chunk, 0, 11264);
		if (i == 0) {
		    /* boot block */
		    strcpy (chunk, "DOS");
		} else if (i == 880 * 512) {
		    /* root block */
		    chunk[0+3] = 2;
		    chunk[12+3] = 0x48;
		    chunk[312] = chunk[313] = chunk[314] = chunk[315] = 0xff;
		    chunk[316+2] = 881 >> 8; chunk[316+3] = 881 & 255;
		    chunk[432] = strlen (disk_name);
		    strcpy (chunk + 433, disk_name);
		    chunk[508 + 3] = 1;
		    disk_date (chunk + 420);
		    memcpy (chunk + 472, chunk + 420, 3 * 4);
		    memcpy (chunk + 484, chunk + 420, 3 * 4);
		    disk_checksum(chunk, chunk + 20);
		    /* bitmap block */
		    memset (chunk + 512 + 4, 0xff, 220);
		    chunk[512 + 112 + 2] = 0x3f;
		    disk_checksum(chunk + 512, chunk + 512);

		}
		WriteFile (adf, chunk, 11264, &count, NULL);
	    }
	    break;
	    case 1:
	    tracks=2*80;
	    WriteFile(adf,"UAE-1ADF",8,&count,0);
	    tmp[0]=0;tmp[1]=0; /* flags (reserved) */
	    tmp[2]=0;tmp[3]=tracks; /* number of tracks */
	    WriteFile(adf,tmp,4,&count,0);
	    tmp[0]=0;tmp[1]=0; /* flags (reserved) */
	    tmp[2]=0;tmp[3]=1; /* track type */
	    tmp[4]=0;tmp[5]=0;tmp[6]=TRACK_WRITE_LEN>>8;tmp[7]=TRACK_WRITE_LEN;
	    tmp[8]=0;tmp[9]=(TRACK_WRITE_LEN*8)>>16;tmp[10]=(TRACK_WRITE_LEN*8)>>8;tmp[11]=(TRACK_WRITE_LEN*8);
	    for (i=0;i<tracks;i++) WriteFile(adf,tmp,sizeof(tmp),&count,0);
	    for (i=0;i<tracks;i++) WriteFile(adf,chunk,TRACK_WRITE_LEN,&count,NULL);
	    break;
	    }
	}
	CloseHandle (adf);
    }
    SetCursor (LoadCursor (NULL, IDC_ARROW));
}

// Common routine for popping up a file-requester
// flag - 0 for floppy loading, 1 for floppy creation, 2 for loading hdf, 3 for saving hdf
// flag - 4 for loading .uae config-files, 5 for saving .uae config-files
// flag = 6 for loading .rom files, 7 for loading .key files
// flag = 8 for loading anything you want
// flag = 9 for saving snapshots
// flag = 10 for loading snapshots
void DiskSelection( HWND hDlg, WPARAM wParam, int flag, struct uae_prefs *prefs )
{
    OPENFILENAME openFileName;
    char regfloppypath[MAX_PATH] = "";
    char regrompath[MAX_PATH] = "";
    char reghdfpath[MAX_PATH] = "";
    DWORD dwType = REG_SZ;
    DWORD dwRFPsize = MAX_PATH;
    DWORD dwRRPsize = MAX_PATH;
    DWORD dwRHPsize = MAX_PATH;
    
    char full_path[MAX_PATH] = "";
    char file_name[MAX_PATH] = "";
    char init_path[MAX_PATH] = "";
    BOOL result = FALSE;
    char *amiga_path = NULL;
    char description[ CFG_DESCRIPTION_LENGTH ] = "";
    int i;

    char szTitle[ MAX_PATH ];
    char szFormat[ MAX_PATH ];
    char szFilter[ MAX_PATH ];
    
    if( hWinUAEKey )
    {
	RegQueryValueEx( hWinUAEKey, "FloppyPath", 0, &dwType, (LPBYTE)regfloppypath, &dwRFPsize );
	RegQueryValueEx( hWinUAEKey, "KickstartPath", 0, &dwType, (LPBYTE)regrompath, &dwRRPsize );
	RegQueryValueEx( hWinUAEKey, "hdfPath", 0, &dwType, (LPBYTE)reghdfpath, &dwRHPsize );
    }
    
    strncpy( init_path, start_path, MAX_PATH );
    switch( flag )
    {
	case 0:
	case 1:
	    if( regfloppypath[0] )
		strncpy( init_path, regfloppypath, MAX_PATH );
	    else
		strncat( init_path, "\\..\\shared\\adf\\", MAX_PATH );
	break;
	case 2:
	case 3:
	    if( reghdfpath[0] )
		strncpy( init_path, reghdfpath, MAX_PATH );
	    else
		strncat( init_path, "\\..\\shared\\hdf\\", MAX_PATH );
	break;
	case 6:
	case 7:
	    if( regrompath[0] )
		strncpy( init_path, regrompath, MAX_PATH );
	    else
		strncat( init_path, "\\..\\shared\\rom\\", MAX_PATH );
	break;
	case 4:
	case 5:
	default:
	    strncat( init_path, "\\Configurations\\", MAX_PATH );
	break;
    }

    openFileName.lStructSize = sizeof (OPENFILENAME);
    openFileName.hwndOwner = hDlg;
    openFileName.hInstance = hInst;
    
    switch (flag) {
    case 0:
	WIN32GUI_LoadUIString( IDS_SELECTADF, szTitle, MAX_PATH );
	WIN32GUI_LoadUIString( IDS_ADF, szFormat, MAX_PATH );
	sprintf( szFilter, "%s ", szFormat );
	memcpy( szFilter + strlen( szFilter ), DISK_FORMAT_STRING, sizeof( DISK_FORMAT_STRING ) + 1 );

	openFileName.lpstrTitle = szTitle;
	openFileName.lpstrDefExt = "ADF";
	openFileName.lpstrFilter = szFilter;
	break;
    case 1:
	WIN32GUI_LoadUIString( IDS_CHOOSEBLANK, szTitle, MAX_PATH );
	WIN32GUI_LoadUIString( IDS_ADF, szFormat, MAX_PATH );
	sprintf( szFilter, "%s ", szFormat );
	memcpy( szFilter + strlen( szFilter ), "(*.ADF)\0*.ADF\0", 15 );

	openFileName.lpstrTitle = szTitle;
	openFileName.lpstrDefExt = "ADF";
	openFileName.lpstrFilter = szFilter;
	break;
    case 2:
    case 3:
	WIN32GUI_LoadUIString( IDS_SELECTHDF, szTitle, MAX_PATH );
	WIN32GUI_LoadUIString( IDS_HDF, szFormat, MAX_PATH );
	sprintf( szFilter, "%s ", szFormat );
	memcpy( szFilter + strlen( szFilter ), "(*.HDF)\0*.HDF\0", 15 );

	openFileName.lpstrTitle = szTitle;
	openFileName.lpstrDefExt = "HDF";
	openFileName.lpstrFilter = szFilter;
	break;
    case 4:
    case 5:
	WIN32GUI_LoadUIString( IDS_SELECTUAE, szTitle, MAX_PATH );
	WIN32GUI_LoadUIString( IDS_UAE, szFormat, MAX_PATH );
	sprintf( szFilter, "%s ", szFormat );
	memcpy( szFilter + strlen( szFilter ), "(*.UAE)\0*.UAE\0", 15 );

	openFileName.lpstrTitle  = szTitle;
	openFileName.lpstrDefExt = "UAE";
	openFileName.lpstrFilter = szFilter;
	break;
    case 6:
	WIN32GUI_LoadUIString( IDS_SELECTROM, szTitle, MAX_PATH );
	WIN32GUI_LoadUIString( IDS_ROM, szFormat, MAX_PATH );
	sprintf( szFilter, "%s ", szFormat );
	memcpy( szFilter + strlen( szFilter ), "(*.ROM)\0*.ROM\0", 15 );

	openFileName.lpstrTitle = szTitle;
	openFileName.lpstrDefExt = "ROM";
	openFileName.lpstrFilter = szFilter;
	break;
    case 7:
	WIN32GUI_LoadUIString( IDS_SELECTKEY, szTitle, MAX_PATH );
	WIN32GUI_LoadUIString( IDS_KEY, szFormat, MAX_PATH );
	sprintf( szFilter, "%s ", szFormat );
	memcpy( szFilter + strlen( szFilter ), "(*.KEY)\0*.KEY\0", 15 );

	openFileName.lpstrTitle = szTitle;
	openFileName.lpstrDefExt = "KEY";
	openFileName.lpstrFilter = szFilter;
	break;
    case 9:
    case 10:
	WIN32GUI_LoadUIString( IDS_SELECTUSS, szTitle, MAX_PATH );
	WIN32GUI_LoadUIString( IDS_USS, szFormat, MAX_PATH );
	sprintf( szFilter, "%s ", szFormat );
	memcpy( szFilter + strlen( szFilter ), "(*.USS)\0*.USS\0", 15 );

	openFileName.lpstrTitle  = szTitle;
	openFileName.lpstrDefExt = "USS";
	openFileName.lpstrFilter = szFilter;
	break;
    case 8:
    default:
	WIN32GUI_LoadUIString( IDS_SELECTINFO, szTitle, MAX_PATH );

	openFileName.lpstrTitle = szTitle;
	openFileName.lpstrFilter = NULL;
	openFileName.lpstrDefExt = NULL;
	break;
    }
    openFileName.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_LONGNAMES | OFN_HIDEREADONLY | OFN_NOCHANGEDIR;
    openFileName.lpstrCustomFilter = NULL;
    openFileName.nMaxCustFilter = 0;
    openFileName.nFilterIndex = 0;
    openFileName.lpstrFile = full_path;
    openFileName.nMaxFile = MAX_PATH;
    openFileName.lpstrFileTitle = file_name;
    openFileName.nMaxFileTitle = MAX_PATH;
    openFileName.lpstrInitialDir = init_path;
    openFileName.lpfnHook = NULL;
    openFileName.lpTemplateName = NULL;
    openFileName.lCustData = 0;
    if (flag == 1 || flag == 3 || flag == 5 || flag == 9)
    {
	if( !(result = GetSaveFileName (&openFileName)) )
	    write_log ("GetSaveFileName() failed.\n");
    }
    else
    {
	if( !(result = GetOpenFileName (&openFileName)) )
	    write_log ("GetOpenFileName() failed.\n");
    }

    if (result)
    {
	switch (wParam) 
	{
	case IDC_PATH_NAME:
	    if( flag == 8 )
	    {
		if( strstr( full_path, "Configurations\\" ) )
		{
		    strcpy( full_path, init_path );
		    strcat( full_path, file_name );
		}
	    }
	    SetDlgItemText (hDlg, wParam, full_path);
	    break;
	case IDC_DF0:
	    SetDlgItemText (hDlg, IDC_DF0TEXT, full_path);
	    strcpy( prefs->df[0], full_path );
	    disk_insert( 0, full_path );
	    break;
	case IDC_DF1:
	    SetDlgItemText (hDlg, IDC_DF1TEXT, full_path);
	    strcpy( prefs->df[1], full_path );
	    disk_insert( 1, full_path );
	    break;
	case IDC_DF2:
	    SetDlgItemText (hDlg, IDC_DF2TEXT, full_path);
	    strcpy( prefs->df[2], full_path );
	    disk_insert( 2, full_path );
	    break;
	case IDC_DF3:
	    SetDlgItemText (hDlg, IDC_DF3TEXT, full_path);
	    strcpy( prefs->df[3], full_path );
	    disk_insert( 3, full_path );
	    break;
	case IDC_DOSAVESTATE:
	case IDC_DOLOADSTATE:
	    strcpy( savestate_filename, full_path );
	    break;
	case IDC_CREATE:
	    CreateDiskFile( full_path, 0 );
	    break;
	case IDC_CREATE_RAW:
	    CreateDiskFile( full_path, 1 );
	    break;
	case IDC_LOAD:
	    if( cfgfile_load( &workprefs, full_path ) == 0 )
	    {
		char szMessage[ MAX_PATH ];
		WIN32GUI_LoadUIString( IDS_COULDNOTLOADCONFIG, szMessage, MAX_PATH );
		gui_message( szMessage );
	    }
	    else
	    {
		cfgfile_get_description( full_path, description );
		for( i = 0; i < C_PAGES; i++ )
		{
		    if( pages[i] )
			SendMessage( pages[i], WM_USER, 0, 0 );
		}
		
		SetDlgItemText( hDlg, IDC_EDITDESCRIPTION, description );
		SetDlgItemText( hDlg, IDC_EDITNAME, full_path );
	    }
	    break;
	case IDC_SAVE:
	    SetDlgItemText( hDlg, IDC_EDITNAME, full_path );
	    break;
	case IDC_ROMFILE:
	    SetDlgItemText( hDlg, IDC_ROMFILE, full_path );
	    strcpy( workprefs.romfile, full_path );
	    break;
	case IDC_KEYFILE:
	    SetDlgItemText( hDlg, IDC_KEYFILE, full_path );
	    strcpy( workprefs.keyfile, full_path );
	    break;
	}
	if( flag == 0 || flag == 1 )
	{
	    amiga_path = strstr( openFileName.lpstrFile, openFileName.lpstrFileTitle );
	    if( amiga_path && amiga_path != openFileName.lpstrFile )
	    {
		*amiga_path = 0;
		if( hWinUAEKey )
		    RegSetValueEx( hWinUAEKey, "FloppyPath", 0, REG_SZ, (CONST BYTE *)openFileName.lpstrFile, strlen( openFileName.lpstrFile ) );
	    }
	}
	else if( flag == 2 || flag == 3 )
	{
	    amiga_path = strstr( openFileName.lpstrFile, openFileName.lpstrFileTitle );
	    if( amiga_path && amiga_path != openFileName.lpstrFile )
	    {
		*amiga_path = 0;
		if( hWinUAEKey )
		    RegSetValueEx( hWinUAEKey, "hdfPath", 0, REG_SZ, (CONST BYTE *)openFileName.lpstrFile, strlen( openFileName.lpstrFile ) );
	    }
	}
	else if( flag == 6 || flag == 7 )
	{
	    amiga_path = strstr( openFileName.lpstrFile, openFileName.lpstrFileTitle );
	    if( amiga_path && amiga_path != openFileName.lpstrFile )
	    {
		*amiga_path = 0;
		if( hWinUAEKey )
		    RegSetValueEx( hWinUAEKey, "KickstartPath", 0, REG_SZ, (CONST BYTE *)openFileName.lpstrFile, strlen( openFileName.lpstrFile ) );
	    }
	}
    }
}

static BOOL CreateHardFile (HWND hDlg, UINT hfsize)
{
    HANDLE hf;
    int i = 0;
    BOOL result = FALSE;

    char init_path[MAX_PATH] = "";

    DiskSelection (hDlg, IDC_PATH_NAME, 3, &workprefs);
    GetDlgItemText (hDlg, IDC_PATH_NAME, init_path, MAX_PATH);
    if( *init_path && hfsize ) 
    {
	    SetCursor( LoadCursor( NULL, IDC_WAIT ) );
	    if( ( hf = CreateFile (init_path, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL) ) != INVALID_HANDLE_VALUE )
	{
	    if( SetFilePointer( hf, hfsize, NULL, FILE_BEGIN ) == hfsize )
	    {
		result = SetEndOfFile( hf );
	    }
	    else
	    {
		write_log ( "SetFilePointer() failure for %s to posn %d\n", init_path, hfsize );
	    }
	    CloseHandle( hf );
	}
	else
	{
	    write_log ( "CreateFile() failed to create %s\n", init_path );
	}
	SetCursor( LoadCursor( NULL, IDC_ARROW ) );
    }
    return result;
}

static const char *memsize_names[] = {
/* 0 */ szNone,
/* 1 */ "256 K",
/* 2 */ "512 K",
/* 3 */ "1 MB",
/* 4 */ "2 MB",
/* 5 */ "4 MB",
/* 6 */ "8 MB",
/* 7 */ "16 MB",
/* 8 */ "32 MB",
/* 9 */ "64 MB",
/* 10*/ "128 MB",
/* 11*/ "256 MB",
/* 12*/ "512 MB",
/* 13*/ "1 GB",

};

static unsigned long memsizes[] = {
/* 0 */ 0,
/* 1 */ 0x00040000, /*  256-K */
/* 2 */ 0x00080000, /*  512-K */
/* 3 */ 0x00100000, /*  1-meg */
/* 4 */ 0x00200000, /*  2-meg */
/* 5 */ 0x00400000, /*  4-meg */
/* 6 */ 0x00800000, /*  8-meg */
/* 7 */ 0x01000000, /* 16-meg */
/* 8 */ 0x02000000, /* 32-meg */
/* 9 */ 0x04000000, /* 64-meg */
/* 10*/ 0x08000000, //128 Meg
/* 11*/ 0x10000000, //256 Meg 
/* 12*/ 0x20000000, //512 Meg The correct size is set in mman.c
/* 13*/ 0x40000000, //1GB Meg
};

static int msi_chip[] = { 2, 3, 4, 5, 6 };
static int msi_bogo[] = { 0, 2, 3 };
static int msi_fast[] = { 0, 3, 4, 5, 6 };
static int msi_z3fast[] = { 0, 3, 4, 5, 6, 7, 8, 9, 10,11,12,13 };
static int msi_gfx[] = { 0, 3, 4, 5, 6,7,8};

static UINT CalculateHardfileSize (HWND hDlg)
{
    BOOL Translated = FALSE;
    UINT bytes = 0;
    bytes = GetDlgItemInt( hDlg, IDC_HFSIZE, &Translated, FALSE );
    if( !Translated )
	bytes = 0;
    return bytes;
}


static const char *nth[] = {
    "", "second ", "third ", "fourth ", "fifth ", "sixth ", "seventh ", "eighth ", "ninth ", "tenth "
};

ConfigStructPtr AllocateConfigStruct( void )
{
    return xmalloc( sizeof( ConfigStruct ) );
}

void FreeConfigStruct( ConfigStructPtr cfgptr )
{
    free( cfgptr );
}

ConfigStructPtr GetFirstConfigEntry( HANDLE *file_handle, LPWIN32_FIND_DATA find_data )
{
    ConfigStructPtr config = NULL;
    DWORD num_bytes = 0;
    char init_path[ MAX_PATH ] = "", *posn;
    char description[ CFG_DESCRIPTION_LENGTH ] = "";

    if( start_path )
    {
	strncpy( init_path, start_path, MAX_PATH );
	strncat( init_path, "\\Configurations\\*.UAE", MAX_PATH );
    }

    if( ( *file_handle = FindFirstFile( init_path, find_data ) ) != INVALID_HANDLE_VALUE )
    {
	config = AllocateConfigStruct();
	sprintf( init_path, "%s\\Configurations\\%s", start_path, find_data->cFileName );
	if( cfgfile_get_description( init_path, description ) )
	{
	    if( ( posn = strrchr( find_data->cFileName, '.' ) ) != NULL )
		*posn = '\0';
	    strcpy( config->Name, find_data->cFileName );

	    strcpy( config->Description, description );
	}
	else
	{
	    FreeConfigStruct( config );
	    config = NULL;
	}
    }
    else
    {
	// Either the directory has no .CFG files, or doesn't exist.

	// Create the directory, even if it already exists.  No harm, and don't check return codes, because
	// we may be doing this on a read-only media like CD-ROM.
	sprintf( init_path, "%s\\Configurations", start_path );
	CreateDirectory( init_path, NULL );
    }
    return config;
}

ConfigStructPtr GetNextConfigEntry( HANDLE *file_handle, LPWIN32_FIND_DATA find_data )
{
    ConfigStructPtr config = NULL;
    char init_path[ MAX_PATH ] = "";
    char desc[ CFG_DESCRIPTION_LENGTH ] = "";
    char *posn;

    if( FindNextFile( *file_handle, find_data ) == 0 )
    {
	FindClose( *file_handle );
    }
    else
    {
	config = AllocateConfigStruct();
	sprintf( init_path, "%s\\Configurations\\%s", start_path, find_data->cFileName );
	if( cfgfile_get_description( init_path, desc ) )
	{
	    if( ( posn = strrchr( find_data->cFileName, '.' ) ) != NULL )
		*posn = '\0';
	    strcpy( config->Name, find_data->cFileName );

	    strcpy( config->Description, desc );
	}
	else
	{
	    FreeConfigStruct( config );
	    config = NULL;
	}
    }
    return config;
}

void HandleConfiguration( HWND hDlg, int flag )
{
    char full_path[MAX_PATH] = "";
    char file_name[MAX_PATH] = "";
    char init_path[MAX_PATH] = "";
    ConfigStructPtr cfgptr = NULL;
    int i;

    if( ( cfgptr = AllocateConfigStruct() ) != NULL )
    {
	switch( flag )
	{
	    case CONFIG_SAVE_FULL:
		DiskSelection( hDlg, IDC_SAVE, 5, &workprefs );
		GetDlgItemText( hDlg, IDC_EDITNAME, full_path, MAX_PATH );
		GetDlgItemText( hDlg, IDC_EDITDESCRIPTION, workprefs.description, 256 );
		cfgfile_save( &workprefs, full_path );
	    break;

	    case CONFIG_LOAD_FULL:
		DiskSelection( hDlg, IDC_LOAD, 4, &workprefs );
		for( i = 0; i < C_PAGES; i++ )
		{
		    if( pages[i] )
			SendMessage( pages[i], WM_USER, 0, 0 );
		}
		EnableWindow( GetDlgItem( hDlg, IDC_VIEWINFO ), workprefs.info[0] );
	    break;
	    
	    case CONFIG_SAVE:
		GetDlgItemText( hDlg, IDC_EDITNAME, cfgptr->Name, MAX_PATH );
		if( cfgptr->Name[0] == '\0' )
		{
		    char szMessage[ MAX_PATH ];
		    WIN32GUI_LoadUIString( IDS_MUSTENTERNAME, szMessage, MAX_PATH );
		    gui_message( szMessage );
		}
		else
		{
		    if( !strchr( cfgptr->Name, '\\' ) && !strchr( cfgptr->Name, '/' ) )
		    {
			if( start_path )
			{
			    strncpy( init_path, start_path, MAX_PATH );
			    strncat( init_path, "\\Configurations\\", MAX_PATH );
			}

			sprintf( full_path, "%s%s.UAE", init_path, cfgptr->Name );
		    }
		    else
		    {
			strcpy( full_path, cfgptr->Name );
		    }
		    GetDlgItemText( hDlg, IDC_EDITDESCRIPTION, workprefs.description, 256 );
		    cfgfile_save( &workprefs, full_path );
		}
	    break;
	
	    case CONFIG_LOAD:
		GetDlgItemText( hDlg, IDC_EDITNAME, cfgptr->Name, MAX_PATH );
		if( cfgptr->Name[0] == '\0' )
		{
		    char szMessage[ MAX_PATH ];
		    WIN32GUI_LoadUIString( IDS_MUSTSELECTCONFIG, szMessage, MAX_PATH );
		    gui_message( szMessage );
		}
		else
		{
		    if( start_path )
		    {
			strncpy( init_path, start_path, MAX_PATH );
			strncat( init_path, "\\Configurations\\", MAX_PATH );
		    }

		    sprintf( full_path, "%s%s.UAE", init_path, cfgptr->Name );
		    strcpy( config_filename, cfgptr->Name );

		    if( cfgfile_load( &workprefs, full_path ) == 0 )
		    {
			char szMessage[ MAX_PATH ];
			WIN32GUI_LoadUIString( IDS_COULDNOTLOADCONFIG, szMessage, MAX_PATH );
			gui_message( szMessage );
		    }

		    for( i = 0; i < C_PAGES; i++ )
		    {
			if( pages[i] )
			    SendMessage( pages[i], WM_USER, 0, 0 );
		    }
		    EnableWindow( GetDlgItem( hDlg, IDC_VIEWINFO ), workprefs.info[0] );
		}
	    break;

	    case CONFIG_DELETE:
		GetDlgItemText( hDlg, IDC_EDITNAME, cfgptr->Name, MAX_PATH );
		if( cfgptr->Name[0] == '\0' )
		{
		    char szMessage[ MAX_PATH ];
		    WIN32GUI_LoadUIString( IDS_MUSTSELECTCONFIGFORDELETE, szMessage, MAX_PATH );
		    gui_message( szMessage );
		}
		else
		{
		    char szMessage[ MAX_PATH ];
		    char szTitle[ MAX_PATH ];
		    WIN32GUI_LoadUIString( IDS_DELETECONFIGCONFIRMATION, szMessage, MAX_PATH );
		    WIN32GUI_LoadUIString( IDS_DELETECONFIGTITLE, szTitle, MAX_PATH );
		    if( MessageBox( hDlg, szMessage, szTitle,
				    MB_YESNO | MB_ICONWARNING | MB_APPLMODAL | MB_SETFOREGROUND ) == IDYES )
		    {
			if( start_path )
			{
			    strncpy( init_path, start_path, MAX_PATH );
			    strncat( init_path, "\\Configurations\\", MAX_PATH );
			}

			sprintf( full_path, "%s%s.UAE", init_path, cfgptr->Name );
			DeleteFile( full_path );
		    }
		}
	    break;
	}
	FreeConfigStruct( cfgptr );
    }
}

static int clicked_entry = -1;

#define LOADSAVE_COLUMNS 2
#define HARDDISK_COLUMNS 7
#define MAX_COLUMN_HEADING_WIDTH 20

void InitializeListView( HWND hDlg )
{
    HANDLE file_handle = NULL;
    WIN32_FIND_DATA find_data;
    BOOL rc = TRUE;
    HWND list;
    LV_ITEM lvstruct;
    LV_COLUMN lvcolumn;
    RECT rect;
    int num_columns = ( hDlg == pages[ LOADSAVE_ID ] ) ? LOADSAVE_COLUMNS : HARDDISK_COLUMNS;
    char column_heading[ HARDDISK_COLUMNS ][ MAX_COLUMN_HEADING_WIDTH ];
    char sectors_str[6]   = "";
    char surfaces_str[6]  = "";
    char reserved_str[6]  = "";
    char blocksize_str[6] = "";
    char readwrite_str[4] = "";
    char volname_str[ MAX_PATH ] = "";
    int width = 0, column_width[ HARDDISK_COLUMNS ];
    int items = 0, result = 0, i, entry = 0, temp = 0;
    ConfigStructPtr config = NULL;

    if( num_columns == LOADSAVE_COLUMNS )
    {
	WIN32GUI_LoadUIString( IDS_NAME, column_heading[0], MAX_COLUMN_HEADING_WIDTH );
	WIN32GUI_LoadUIString( IDS_DESCRIPTION, column_heading[1], MAX_COLUMN_HEADING_WIDTH );

	list = GetDlgItem( hDlg, IDC_CONFIGLIST );
    }
    else
    {
	WIN32GUI_LoadUIString( IDS_VOLUME, column_heading[0], MAX_COLUMN_HEADING_WIDTH );
	WIN32GUI_LoadUIString( IDS_PATH, column_heading[1], MAX_COLUMN_HEADING_WIDTH );
	WIN32GUI_LoadUIString( IDS_RW, column_heading[2], MAX_COLUMN_HEADING_WIDTH );
	WIN32GUI_LoadUIString( IDS_SECTORS, column_heading[3], MAX_COLUMN_HEADING_WIDTH );
	WIN32GUI_LoadUIString( IDS_SURFACES, column_heading[4], MAX_COLUMN_HEADING_WIDTH );
	WIN32GUI_LoadUIString( IDS_RESERVED, column_heading[5], MAX_COLUMN_HEADING_WIDTH );
	WIN32GUI_LoadUIString( IDS_BLOCKSIZE, column_heading[6], MAX_COLUMN_HEADING_WIDTH );

	list = GetDlgItem( hDlg, IDC_VOLUMELIST );
    }

    ListView_DeleteAllItems( list );

    for( i = 0; i < num_columns; i++ )
	column_width[i] = ListView_GetStringWidth( list, column_heading[i] ) + 15;

    // If there are no columns, then insert some
    lvcolumn.mask = LVCF_WIDTH;
    if( ListView_GetColumn( list, 1, &lvcolumn ) == FALSE )
    {
	for( i = 0; i < num_columns; i++ )
	{
	    lvcolumn.mask     = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
	    lvcolumn.iSubItem = i;
	    lvcolumn.fmt      = LVCFMT_LEFT;
	    lvcolumn.pszText  = column_heading[i];
	    lvcolumn.cx       = column_width[i];
	    ListView_InsertColumn( list, i, &lvcolumn );
	}
    }
    if( num_columns == LOADSAVE_COLUMNS )
    {
	if( ( config = GetFirstConfigEntry( &file_handle, &find_data ) ) != NULL )
	{
	    while( config )
	    {
		lvstruct.mask     = LVIF_TEXT | LVIF_PARAM;
		lvstruct.pszText  = config->Name;
		lvstruct.lParam   = 0;
		lvstruct.iItem    = entry;
		lvstruct.iSubItem = 0;
		result = ListView_InsertItem( list, &lvstruct );
		if( result != -1 )
		{
		    width = ListView_GetStringWidth( list, lvstruct.pszText ) + 15;
		    if( width > column_width[ lvstruct.iSubItem ] )
			column_width[ lvstruct.iSubItem ] = width;

		    ListView_SetItemText( list, result, 1, config->Description );
		    width = ListView_GetStringWidth( list, config->Description ) + 15;
		    if( width > column_width[ 1 ] )
			column_width[ 1 ] = width;

		    entry++;
		}
		FreeConfigStruct( config );
		config = GetNextConfigEntry( &file_handle, &find_data );
	    }
	}
    }
    else
    {
	for( i = 0; i < nr_units( workprefs.mountinfo ); i++ )
	{
	    int secspertrack, surfaces, reserved, blocksize, size;
	    int cylinders, readonly;
	    char *volname, *rootdir;
	    char *failure;

	    failure = get_filesys_unit (workprefs.mountinfo, i,
					&volname, &rootdir, &readonly,
					&secspertrack, &surfaces, &reserved,
					&cylinders, &size, &blocksize);
	    
	    if( is_hardfile( workprefs.mountinfo, i ) )
	    {
		sprintf( sectors_str, "%d", secspertrack );
		sprintf( surfaces_str, "%d", surfaces );
		sprintf( reserved_str, "%d", reserved );
		sprintf( blocksize_str, "%d", blocksize );
		sprintf( volname_str, "DH%d", i );
	    }
	    else
	    {
		strcpy( sectors_str, "n/a" );
		strcpy( surfaces_str, "n/a" );
		strcpy( reserved_str, "n/a" );
		strcpy( blocksize_str, "n/a" );
		strcpy( volname_str, volname );
	    }

	    sprintf( readwrite_str, "%s", readonly ? "no" : "yes" );
	    lvstruct.mask     = LVIF_TEXT | LVIF_PARAM;
	    lvstruct.pszText  = volname_str;
	    lvstruct.lParam   = 0;
	    lvstruct.iItem    = i;
	    lvstruct.iSubItem = 0;
	    result = ListView_InsertItem (list, &lvstruct);
	    if (result != -1) {
		width = ListView_GetStringWidth( list, lvstruct.pszText ) + 15;
		if( width > column_width[ lvstruct.iSubItem ] )
		    column_width[ lvstruct.iSubItem ] = width;

		ListView_SetItemText( list, result, 1, rootdir );
		width = ListView_GetStringWidth( list, rootdir ) + 15;
		if( width > column_width[ 1 ] )
		    column_width[ 1 ] = width;

		ListView_SetItemText( list, result, 2, readwrite_str );
		width = ListView_GetStringWidth( list, readwrite_str ) + 15;
		if( width > column_width[ 2 ] )
		    column_width[ 2 ] = width;

		ListView_SetItemText( list, result, 3, sectors_str );
		width = ListView_GetStringWidth( list, sectors_str ) + 15;
		if( width > column_width[ 3 ] )
		    column_width[ 3 ] = width;

		ListView_SetItemText( list, result, 4, surfaces_str );
		width = ListView_GetStringWidth( list, surfaces_str ) + 15;
		if( width > column_width[ 4 ] )
		    column_width[ 4 ] = width;

		ListView_SetItemText( list, result, 5, reserved_str );
		width = ListView_GetStringWidth( list, reserved_str ) + 15;
		if( width > column_width[ 5 ] )
		    column_width[ 5 ] = width;

		ListView_SetItemText( list, result, 6, blocksize_str );
		width = ListView_GetStringWidth( list, blocksize_str ) + 15;
		if( width > column_width[ 6 ] )
		    column_width[ 6 ] = width;
	    }
	}
    }
    if( rc == FALSE )
    {
	FreeConfigStruct( config );
    }

    if( result != -1 )
    {
	if( GetWindowRect( list, &rect ) )
	{
	    ScreenToClient( hDlg, (LPPOINT)&rect );
	    ScreenToClient( hDlg, (LPPOINT)&rect.right );
	    if( num_columns == 2 )
	    {
		if( ( temp = rect.right - rect.left - column_width[ 0 ] - 4 ) > column_width[1] )
		    column_width[1] = temp;
	    }
	}

	// Adjust our column widths so that we can see the contents...
	for( i = 0; i < num_columns; i++ )
	{
	    ListView_SetColumnWidth( list, i, column_width[i] );
	}

#if 0
	// Turn on full-row-select option
	ListView_SetExtendedListViewStyle( list, LVS_EX_FULLROWSELECT );
#endif

	// Redraw the items in the list...
	items = ListView_GetItemCount( list );
	ListView_RedrawItems( list, 0, items );
    }
}

static int listview_find_selected (HWND list)
{
    int i, items;
    items = ListView_GetItemCount (list);
    for (i = 0; i < items; i++) {
	if (ListView_GetItemState (list, i, LVIS_SELECTED) == LVIS_SELECTED)
	    return i;
    }
    return -1;
}

static int listview_entry_from_click (HWND list)
{
    POINT point;
    DWORD pos = GetMessagePos ();
    int items, entry;

    point.x = LOWORD (pos);
    point.y = HIWORD (pos);
    ScreenToClient (list, &point);
    entry = ListView_GetTopIndex (list);
    items = entry + ListView_GetCountPerPage (list);
    if (items > ListView_GetItemCount (list))
	items = ListView_GetItemCount (list);

    while (entry <= items) {
	RECT rect;
	/* Get the bounding rectangle of an item. If the mouse
	 * location is within the bounding rectangle of the item,
	 * you know you have found the item that was being clicked.  */
	ListView_GetItemRect (list, entry, &rect, LVIR_BOUNDS);
	if (PtInRect (&rect, point)) {
	    UINT flag = LVIS_SELECTED | LVIS_FOCUSED;
	    ListView_SetItemState (list, entry, flag, flag);
	    return entry;
	}
	entry++;
    }
    return -1;
}

static int CALLBACK InfoSettingsProc (HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
    static int recursive = 0;
    
    switch (msg) 
    {
	case WM_INITDIALOG:
	    recursive++;
	    SetDlgItemText (hDlg, IDC_PATH_NAME, workprefs.info);
	    recursive--;
	return TRUE;
	
	case WM_COMMAND:
	    if (recursive)
		break;
	    recursive++;
	
	    switch( wParam ) 
	    {
		case IDC_SELECTOR:
		    DiskSelection (hDlg, IDC_PATH_NAME, 8, &workprefs );
		break;
		case IDOK:
		    EndDialog (hDlg, 1);
		break;
		case IDCANCEL:
		    EndDialog (hDlg, 0);
		break;
	    }
	
	    GetDlgItemText( hDlg, IDC_PATH_NAME, workprefs.info, sizeof workprefs.info );
	    recursive--;
	break;
    }
    return FALSE;
}

static BOOL CALLBACK LoadSaveDlgProc (HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
    char name_buf[MAX_PATH] = "", desc_buf[128] = "";
    char *posn = NULL;
    HWND list;
    int dblclick = 0;
    NM_LISTVIEW *nmlistview;
    int items = 0, entry = 0;
    LPHELPINFO lpHelpInfo;
    
    switch (msg) {
    case WM_INITDIALOG:
	pages[LOADSAVE_ID] = hDlg;
	InitializeListView(hDlg);
	if( !full_property_sheet ) 
	{
	    EnableWindow( GetDlgItem( hDlg, IDC_SAVE ), FALSE );
	}
	EnableWindow( GetDlgItem( hDlg, IDC_VIEWINFO ), workprefs.info[0] );
#if 0
	if( !config_filename[0] && configname[0] )
	{
	    if( posn = strrchr( configname, '\\' ) )
		strcpy( configname, ++posn );
	    if( posn = strrchr( configname, '.' ) )
		*posn = '\0';
	    strcpy( config_filename, configname );
	}
#endif
	SetDlgItemText( hDlg, IDC_EDITNAME, config_filename );
	SetDlgItemText( hDlg, IDC_EDITDESCRIPTION, workprefs.description );
	return TRUE;
	
    case WM_USER:
	break;
	
    case WM_HELP:
	lpHelpInfo = (LPHELPINFO)lParam;
	break;
	
    case WM_COMMAND:
	switch (wParam) 
	{
	    case IDC_SAVE:
		HandleConfiguration( hDlg, CONFIG_SAVE_FULL );
		InitializeListView( hDlg );
	    break;
	    case IDC_QUICKSAVE:
		HandleConfiguration( hDlg, CONFIG_SAVE );
		InitializeListView( hDlg );
	    break;
	    case IDC_QUICKLOAD:
		HandleConfiguration( hDlg, CONFIG_LOAD );
	    break;
	    case IDC_LOAD:
		HandleConfiguration( hDlg, CONFIG_LOAD_FULL );
	    break;
	    case IDC_DELETE:
		HandleConfiguration( hDlg, CONFIG_DELETE );
		InitializeListView( hDlg );
	    break;
	    case IDC_VIEWINFO:
		if( workprefs.info[0] )
		{
		    if( strstr( workprefs.info, "Configurations\\" ) )
			sprintf( name_buf, "%s\\%s", start_path, workprefs.info );
		    else
			strcpy( name_buf, workprefs.info );
		    ShellExecute( NULL, NULL, name_buf, NULL, NULL, SW_SHOWNORMAL );
		}
	    break;
	    case IDC_SETINFO:
		if( DialogBox( hUIDLL ? hUIDLL : hInst, MAKEINTRESOURCE (IDD_SETINFO), hDlg, InfoSettingsProc ) )
		{
		    EnableWindow( GetDlgItem( hDlg, IDC_VIEWINFO ), workprefs.info[0] );
		}
	    break;
	}
	break;
	
	case WM_NOTIFY:
	    if (((LPNMHDR) lParam)->idFrom == IDC_CONFIGLIST) 
	    {
		nmlistview = (NM_LISTVIEW *) lParam;
		list = nmlistview->hdr.hwndFrom;
		
		switch (nmlistview->hdr.code) 
		{
		case NM_DBLCLK:
		    dblclick = 1;
		    /* fall-through */
		case NM_CLICK:
		    entry = listview_entry_from_click (list);
		    /* Copy the item's name and description to the gadgets at the bottom... */
		    if (entry >= 0) 
		    {
			ListView_GetItemText (list, entry, 0, name_buf, MAX_PATH);
			ListView_GetItemText (list, entry, 1, desc_buf, 128);
			SetDlgItemText (hDlg, IDC_EDITNAME, name_buf);
			SetDlgItemText (hDlg, IDC_EDITDESCRIPTION, desc_buf);
			ListView_RedrawItems (list, 0, items);
			
			if( dblclick ) 
			{
			    HandleConfiguration( hDlg, CONFIG_LOAD );
			    /* Copy stuff from workprefs and config_xxx settings */
			    gui_to_prefs ();
			    PropSheet_PressButton( hPropertySheet, PSBTN_OK );
			}
		    }
		    break;
		}
	    }
	    else
	    {
		switch (((NMHDR *) lParam)->code) 
		{
		case PSN_HELP:
		    HtmlHelp( NULL, help_file, HH_DISPLAY_TOPIC, "configurations.htm" );
		    break;
		case PSN_APPLY:
		    /* Copy stuff from workprefs and config_xxx settings */
		    gui_to_prefs ();
		    break;
		    
		case PSN_RESET:
		    if (allow_quit) 
		    {
			quit_program = 1;
			regs.spcflags |= SPCFLAG_BRK;
		    }
		    break;
		}
	    }
	    break;
    }
    
    return FALSE;
}

#define MAX_CONTRIBUTORS_LENGTH 2048

static int CALLBACK ContributorsProc (HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
    CHARFORMAT CharFormat;
    char szContributors1[ MAX_CONTRIBUTORS_LENGTH ];
    char szContributors2[ MAX_CONTRIBUTORS_LENGTH ];
    char szContributors[ MAX_CONTRIBUTORS_LENGTH*2 ];

    switch (msg) {
     case WM_COMMAND:
	if (wParam == ID_OK) {
	    EndDialog (hDlg, 1);
	    return TRUE;
	}
	break;
     case WM_INITDIALOG:
	CharFormat.cbSize = sizeof (CharFormat);

	WIN32GUI_LoadUIString( IDS_CONTRIBUTORS1, szContributors1, MAX_CONTRIBUTORS_LENGTH );
	WIN32GUI_LoadUIString( IDS_CONTRIBUTORS2, szContributors2, MAX_CONTRIBUTORS_LENGTH );
	sprintf( szContributors, "%s%s", szContributors1, szContributors2 );

	SetDlgItemText (hDlg, IDC_CONTRIBUTORS, szContributors );
	SendDlgItemMessage (hDlg, IDC_CONTRIBUTORS, EM_GETCHARFORMAT, 0, (LPARAM) & CharFormat);
	CharFormat.dwMask |= CFM_SIZE | CFM_FACE;
	CharFormat.yHeight = 10 * 20;	/* height in twips, where a twip is 1/20th of a point - for a pt.size of 18 */

	strcpy (CharFormat.szFaceName, "Times New Roman");
	SendDlgItemMessage (hDlg, IDC_CONTRIBUTORS, EM_SETCHARFORMAT, SCF_ALL, (LPARAM) & CharFormat);
	/* SendDlgItemMessage(hDlg, IDC_CONTRIBUTORS, EM_SETBKGNDCOLOR,0,GetSysColor( COLOR_3DFACE ) ); */

	return TRUE;
    }
    return FALSE;
}

static void DisplayContributors (HWND hDlg)
{
    DialogBox( hUIDLL ? hUIDLL : hInst, MAKEINTRESOURCE (IDD_CONTRIBUTORS), hDlg, ContributorsProc);
}

#define NUM_URLS 7
typedef struct url_info
{
    int   id;
    BOOL  state;
    char *display;
    char *url;
} urlinfo;

static urlinfo urls[NUM_URLS] = 
{
    {IDC_CLOANTOHOME, FALSE, "Cloanto's Amiga Forever", "http://www.cloanto.com/amiga/forever/"},
    {IDC_AMIGAHOME, FALSE, "Amiga Inc.", "http://www.amiga.com"},
    {IDC_PICASSOHOME, FALSE, "Picasso96 Home Page", "http://www.picasso96.cogito.de/"}, 
    {IDC_UAEHOME, FALSE, "UAE Home Page", "http://www.freiburg.linux.de/~uae/"},
    {IDC_WINUAEHOME, FALSE, "WinUAE Home Page", "http://www.CodePoet.com/UAE/"},
    {IDC_AIABHOME, FALSE, "AIAB", "http://aiab.emuunlim.com/"},
    {IDC_THEROOTS, FALSE, "Back To The Roots", "http://back2roots.emuunlim.com/"}
};

static void SetupRichText( HWND hDlg, urlinfo *url )
{
    CHARFORMAT CharFormat;
    CharFormat.cbSize = sizeof (CharFormat);

    SetDlgItemText( hDlg, url->id, url->display );
    SendDlgItemMessage( hDlg, url->id, EM_GETCHARFORMAT, 0, (LPARAM)&CharFormat );
    CharFormat.dwMask   |= CFM_UNDERLINE | CFM_SIZE | CFM_FACE | CFM_COLOR;
    CharFormat.dwEffects = url->state ? CFE_UNDERLINE : 0;
    CharFormat.yHeight = 10 * 20;	/* height in twips, where a twip is 1/20th of a point - for a pt.size of 18 */

    CharFormat.crTextColor = GetSysColor( COLOR_ACTIVECAPTION );
    strcpy( CharFormat.szFaceName, "Tahoma" );
    SendDlgItemMessage( hDlg, url->id, EM_SETCHARFORMAT, SCF_ALL, (LPARAM)&CharFormat );
    SendDlgItemMessage( hDlg, url->id, EM_SETBKGNDCOLOR, 0, GetSysColor( COLOR_3DFACE ) );
}

static void url_handler(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam )
{
    static int last_rectangle = -1;
    int i;
    BOOL found = FALSE;
    HCURSOR m_hCursor = NULL;
    POINT point;
    point.x = LOWORD (lParam);
    point.y = HIWORD (lParam);
    
    for (i = 0; i < NUM_URLS; i++) 
    {
	RECT rect;
	GetWindowRect( GetDlgItem( hDlg, urls[i].id), &rect );
	ScreenToClient( hDlg, (POINT *) &rect );
	ScreenToClient( hDlg, (POINT *) &(rect.right) );
	if( PtInRect( &rect, point ) ) 
	{
	    if( msg == WM_LBUTTONDOWN )
	    {
		ShellExecute (NULL, NULL, urls[i].url , NULL, NULL, SW_SHOWNORMAL);
		SetCursor( LoadCursor( NULL, MAKEINTRESOURCE(IDC_ARROW) ) );
	    }
	    else
	    {
		if( ( i != last_rectangle ) )
		{
		    // try and load the system hand (Win2000+)
		    m_hCursor = LoadCursor(NULL, MAKEINTRESOURCE(IDC_HAND) );
		    if (!m_hCursor)
		    {
			// retry with our fallback hand
			m_hCursor = LoadCursor(hInst, MAKEINTRESOURCE(IDC_MYHAND) );
		    }
		    SetCursor( m_hCursor );
		    urls[i].state = TRUE;
		    SetupRichText( hDlg, &urls[i] );

		    if( last_rectangle != -1 )
		    {
			urls[last_rectangle].state = FALSE;
			SetupRichText( hDlg, &urls[last_rectangle] );
		    }
		}
	    }
	    last_rectangle = i;
	    found = TRUE;
	    break;
	}
    }

    if( !found && last_rectangle >= 0 )
    {
	SetCursor( LoadCursor( NULL, MAKEINTRESOURCE(IDC_ARROW) ) );
	urls[last_rectangle].state = FALSE;
	SetupRichText( hDlg, &urls[last_rectangle] );
	last_rectangle = -1;
    }
}

static void init_aboutdlg (HWND hDlg)
{
    CHARFORMAT CharFormat;
    int i;

    CharFormat.cbSize = sizeof (CharFormat);

    SetDlgItemText (hDlg, IDC_RICHEDIT1, "UAE/Win32");
    SendDlgItemMessage (hDlg, IDC_RICHEDIT1, EM_GETCHARFORMAT, 0, (LPARAM) & CharFormat);
    CharFormat.dwMask |= CFM_BOLD | CFM_SIZE | CFM_FACE;
    CharFormat.dwEffects = CFE_BOLD;
    CharFormat.yHeight = 18 * 20;	/* height in twips, where a twip is 1/20th of a point - for a pt.size of 18 */

    strcpy (CharFormat.szFaceName, "Times New Roman");
    SendDlgItemMessage (hDlg, IDC_RICHEDIT1, EM_SETCHARFORMAT, SCF_ALL, (LPARAM) & CharFormat);
    SendDlgItemMessage (hDlg, IDC_RICHEDIT1, EM_SETBKGNDCOLOR, 0, GetSysColor (COLOR_3DFACE));

    SetDlgItemText (hDlg, IDC_RICHEDIT2, VersionStr );
    SendDlgItemMessage (hDlg, IDC_RICHEDIT2, EM_GETCHARFORMAT, 0, (LPARAM) & CharFormat);
    CharFormat.dwMask |= CFM_SIZE | CFM_FACE;
    CharFormat.yHeight = 10 * 20;
    strcpy (CharFormat.szFaceName, "Times New Roman");
    SendDlgItemMessage (hDlg, IDC_RICHEDIT2, EM_SETCHARFORMAT, SCF_ALL, (LPARAM) & CharFormat);
    SendDlgItemMessage (hDlg, IDC_RICHEDIT2, EM_SETBKGNDCOLOR, 0, GetSysColor (COLOR_3DFACE));

    for( i = 0; i < NUM_URLS; i++ )
    {
	SetupRichText( hDlg, &urls[i] );
    }
}

static BOOL CALLBACK AboutDlgProc (HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch( msg )
    {
	case WM_INITDIALOG:
	    pages[ABOUT_ID] = hDlg;
	    init_aboutdlg (hDlg);
	    break;
	    
	case WM_COMMAND:
	    if (wParam == IDC_CONTRIBUTORS) 
	    {
		DisplayContributors (hDlg);
	    }
	    break;
	case WM_SETCURSOR:
	    return TRUE;
	    break;
	case WM_LBUTTONDOWN:
	case WM_MOUSEMOVE:
	    url_handler( hDlg, msg, wParam, lParam );
	    break;
	case WM_NOTIFY:
	    switch (((NMHDR *) lParam)->code) 
	    {
		case PSN_APPLY:
		    /* Copy stuff from workprefs and config_xxx settings */
		    gui_to_prefs ();
		    break;
		    
		case PSN_RESET:
		    if (allow_quit) 
		    {
			quit_program = 1;
			regs.spcflags |= SPCFLAG_BRK;
		    }
		    break;
	    }
	    break;
    }
    
    return FALSE;
}

static struct win32_displaymode *GetDisplayMode (int logical_number)
{
    struct win32_displaymode *dm = win32_displaymode_list;
    while (logical_number > 0 && dm != 0)
	dm = dm->next, logical_number--;
    return dm;
}

static void enable_for_displaydlg (HWND hDlg)
{
    if (! full_property_sheet) 
    {
	/* Disable certain controls which are only to be set once at start-up... */
	EnableWindow (GetDlgItem (hDlg, IDC_TEST16BIT), FALSE);
	EnableWindow (GetDlgItem (hDlg, IDC_LORES), FALSE);
	EnableWindow (GetDlgItem (hDlg, IDC_XCENTER), FALSE);
	EnableWindow (GetDlgItem (hDlg, IDC_YCENTER), FALSE);
	EnableWindow (GetDlgItem (hDlg, IDC_ASPECT), FALSE);
	EnableWindow (GetDlgItem (hDlg, IDC_LM_NORMAL), FALSE);
	EnableWindow (GetDlgItem (hDlg, IDC_LM_DOUBLED), FALSE);
	EnableWindow (GetDlgItem (hDlg, IDC_LM_SCANLINES), FALSE);
	EnableWindow (GetDlgItem (hDlg, IDC_RESOLUTION), FALSE);
	EnableWindow (GetDlgItem (hDlg, IDC_XSIZE), FALSE);
	EnableWindow (GetDlgItem (hDlg, IDC_YSIZE), FALSE);
    }
    else
    {
	EnableWindow( GetDlgItem( hDlg, IDC_PFULLSCREEN ), ! workprefs.address_space_24);
	EnableWindow( GetDlgItem( hDlg, IDC_XSIZE ), workprefs.gfx_afullscreen ? FALSE:TRUE );
	EnableWindow( GetDlgItem( hDlg, IDC_YSIZE ), workprefs.gfx_afullscreen ? FALSE:TRUE );
	EnableWindow( GetDlgItem( hDlg, IDC_RESOLUTION ), workprefs.gfx_afullscreen ? TRUE:FALSE );
    }
}

static void enable_for_chipsetdlg (HWND hDlg)
{
}

static void LoadNthString( DWORD value, char *nth, DWORD dwNthMax )
{
    switch( value )
    {
	case 1:
	    WIN32GUI_LoadUIString( IDS_SECOND, nth, dwNthMax );
	break;

	case 2:
	    WIN32GUI_LoadUIString( IDS_THIRD, nth, dwNthMax );	
	break;
	
	case 3:
	    WIN32GUI_LoadUIString( IDS_FOURTH, nth, dwNthMax );	
	break;
	
	case 4:
	    WIN32GUI_LoadUIString( IDS_FIFTH, nth, dwNthMax );	
	break;
	
	case 5:
	    WIN32GUI_LoadUIString( IDS_SIXTH, nth, dwNthMax );	
	break;
	
	case 6:
	    WIN32GUI_LoadUIString( IDS_SEVENTH, nth, dwNthMax );	
	break;
	
	case 7:
	    WIN32GUI_LoadUIString( IDS_EIGHTH, nth, dwNthMax );	
	break;
	
	case 8:
	    WIN32GUI_LoadUIString( IDS_NINTH, nth, dwNthMax );	
	break;
	
	case 9:
	    WIN32GUI_LoadUIString( IDS_TENTH, nth, dwNthMax );	
	break;
	
	default:
	    strcpy( nth, "" );
    }
}

#define MAX_FRAMERATE_LENGTH 40
#define MAX_NTH_LENGTH 20

static void values_to_displaydlg (HWND hDlg)
{
    char buffer[ MAX_FRAMERATE_LENGTH + MAX_NTH_LENGTH ];
    char Nth[ MAX_NTH_LENGTH ];
    LPSTR blah[1] = { Nth };
    LPTSTR string = NULL;
    int d,d2,index;

    switch( workprefs.color_mode )
    {
    case 2:
	d = 16;
	break;
    case 5:
	d = 32;
	break;
    default:
	d = 8;
	break;
    }

    if( workprefs.gfx_afullscreen )
    {
	d2 = d;
	if( ( index = WIN32GFX_AdjustScreenmode( &workprefs.gfx_width, &workprefs.gfx_height, &d2 ) ) >= 0 )
	{
	    switch( d2 )
	    {
	    case 15:
		workprefs.color_mode = 1;
		d = 2;
		break;
	    case 16:
		workprefs.color_mode = 2;
		d = 2;
		break;
	    case 32:
		workprefs.color_mode = 5;
		d = 4;
		break;
	    default:
		workprefs.color_mode = 0;
		d = 1;
		break;
	    }
	}
    }
    else
    {
	d = d/8;
    }

    if ((index = picasso_display_mode_index (workprefs.gfx_width, workprefs.gfx_height, d)) >= 0)
	SendDlgItemMessage( hDlg, IDC_RESOLUTION, CB_SETCURSEL, index, 0 );

    SetDlgItemInt( hDlg, IDC_XSIZE, workprefs.gfx_width, FALSE );
    SetDlgItemInt( hDlg, IDC_YSIZE, workprefs.gfx_height, FALSE );

    SendDlgItemMessage (hDlg, IDC_FRAMERATE, TBM_SETPOS, TRUE, workprefs.gfx_framerate);

    WIN32GUI_LoadUIString( IDS_FRAMERATE, buffer, MAX_FRAMERATE_LENGTH );
    LoadNthString( workprefs.gfx_framerate - 1, Nth, MAX_NTH_LENGTH );
    if( FormatMessage( FORMAT_MESSAGE_FROM_STRING | FORMAT_MESSAGE_ARGUMENT_ARRAY | FORMAT_MESSAGE_ALLOCATE_BUFFER,
		       buffer, 0, 0, (LPTSTR)&string, MAX_FRAMERATE_LENGTH + MAX_NTH_LENGTH, (va_list *)blah ) == 0 )
    {
	DWORD dwLastError = GetLastError();
	sprintf (buffer, "Every %s Frame", nth[workprefs.gfx_framerate - 1]);
	SetDlgItemText( hDlg, IDC_RATETEXT, buffer );
    }
    else
    {
	SetDlgItemText( hDlg, IDC_RATETEXT, string );
	LocalFree( string );
    }

    CheckRadioButton( hDlg, IDC_LM_NORMAL, IDC_LM_SCANLINES, IDC_LM_NORMAL+workprefs.gfx_linedbl );
    CheckDlgButton (hDlg, IDC_AFULLSCREEN, workprefs.gfx_afullscreen);
    CheckDlgButton (hDlg, IDC_PFULLSCREEN, workprefs.gfx_pfullscreen);
    CheckDlgButton (hDlg, IDC_ASPECT, workprefs.gfx_correct_aspect);
    CheckDlgButton (hDlg, IDC_LORES, workprefs.gfx_lores);
    
    CheckDlgButton (hDlg, IDC_XCENTER, workprefs.gfx_xcenter);
    CheckDlgButton (hDlg, IDC_YCENTER, workprefs.gfx_ycenter);
}

static void init_resolution_combo (HWND hDlg)
{
    int i;

    for( i = 0; i < picasso_nr_resolutions (); i++ )
    {
	SendDlgItemMessage( hDlg, IDC_RESOLUTION, CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)DisplayModes[i].name );
    }
}

static void values_from_displaydlg (HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
    BOOL success = FALSE;

    workprefs.gfx_pfullscreen    = IsDlgButtonChecked (hDlg, IDC_PFULLSCREEN);
    workprefs.gfx_afullscreen    = IsDlgButtonChecked (hDlg, IDC_AFULLSCREEN);
    EnableWindow( GetDlgItem( hDlg, IDC_RESOLUTION ), workprefs.gfx_afullscreen ? TRUE:FALSE );

    workprefs.gfx_lores          = IsDlgButtonChecked (hDlg, IDC_LORES);
    workprefs.gfx_correct_aspect = IsDlgButtonChecked (hDlg, IDC_ASPECT);
    workprefs.gfx_linedbl = ( IsDlgButtonChecked( hDlg, IDC_LM_SCANLINES ) ? 2 :
			      IsDlgButtonChecked( hDlg, IDC_LM_DOUBLED ) ? 1 : 0 );

    workprefs.gfx_framerate = SendDlgItemMessage (hDlg, IDC_FRAMERATE, TBM_GETPOS, 0, 0);

    {
	char buffer[ MAX_FRAMERATE_LENGTH ];
	char Nth[ MAX_NTH_LENGTH ];
	LPSTR blah[1] = { Nth };
	LPTSTR string = NULL;

	WIN32GUI_LoadUIString( IDS_FRAMERATE, buffer, MAX_FRAMERATE_LENGTH );
	LoadNthString( workprefs.gfx_framerate - 1, Nth, MAX_NTH_LENGTH );
	if( FormatMessage( FORMAT_MESSAGE_FROM_STRING | FORMAT_MESSAGE_ARGUMENT_ARRAY | FORMAT_MESSAGE_ALLOCATE_BUFFER,
			   buffer, 0, 0, (LPTSTR)&string, MAX_FRAMERATE_LENGTH + MAX_NTH_LENGTH, (va_list *)blah ) == 0 )
	{
	    DWORD dwLastError = GetLastError();
	    sprintf (buffer, "Every %s Frame", nth[workprefs.gfx_framerate - 1]);
	    SetDlgItemText( hDlg, IDC_RATETEXT, buffer );
	}
	else
	{
	    SetDlgItemText( hDlg, IDC_RATETEXT, string );
	    LocalFree( string );
	}
	workprefs.gfx_width  = GetDlgItemInt( hDlg, IDC_XSIZE, &success, FALSE );
	if( !success )
	    workprefs.gfx_width = 800;
	workprefs.gfx_height = GetDlgItemInt( hDlg, IDC_YSIZE, &success, FALSE );
	if( !success )
	    workprefs.gfx_height = 600;
    }
    workprefs.gfx_xcenter = (IsDlgButtonChecked (hDlg, IDC_XCENTER) ? 2 : 0 ); /* Smart centering */
    workprefs.gfx_ycenter = (IsDlgButtonChecked (hDlg, IDC_YCENTER) ? 2 : 0 ); /* Smart centering */

    if (msg == WM_COMMAND && LOWORD (wParam) == IDC_RESOLUTION && HIWORD (wParam) == CBN_SELCHANGE) 
    {
	    LONG posn;
	    posn = SendDlgItemMessage (hDlg, IDC_RESOLUTION, CB_GETCURSEL, 0, 0);
	    if (posn == CB_ERR)
		return;
	workprefs.gfx_width  = DisplayModes[posn].res.width;
	workprefs.gfx_height = DisplayModes[posn].res.height;
	switch( DisplayModes[posn].depth )
	{
	case 2:
	    workprefs.color_mode = 2;
	    break;
	case 3:
	case 4:
	    workprefs.color_mode = 5;
	    break;
	default:
	    workprefs.color_mode = 0;
	    break;
	}
	/* Set the Int boxes */
	SetDlgItemInt( hDlg, IDC_XSIZE, workprefs.gfx_width, FALSE );
	SetDlgItemInt( hDlg, IDC_YSIZE, workprefs.gfx_height, FALSE );
    }
}

static BOOL CALLBACK DisplayDlgProc (HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
    static int recursive = 0;
    HKEY hPixelFormatKey;
    RGBFTYPE colortype      = RGBFB_NONE;
    DWORD dwType            = REG_DWORD;
    DWORD dwDisplayInfoSize = sizeof( colortype );

    switch (msg) 
    {
    case WM_INITDIALOG:
	pages[DISPLAY_ID] = hDlg;
	SendDlgItemMessage (hDlg, IDC_FRAMERATE, TBM_SETPAGESIZE, 0, 1);
	SendDlgItemMessage (hDlg, IDC_FRAMERATE, TBM_SETRANGE, TRUE, MAKELONG (MIN_REFRESH_RATE, MAX_REFRESH_RATE));
	init_resolution_combo( hDlg );

    case WM_USER:
	recursive++;
	values_to_displaydlg (hDlg);
	enable_for_displaydlg (hDlg);
	recursive--;
	break;

    case WM_HSCROLL:
    case WM_COMMAND:
	if (recursive > 0)
	    break;
	recursive++;
	if( ( wParam == IDC_TEST16BIT ) && DirectDraw_Start() )
	{
	    if( RegOpenKeyEx( HKEY_CURRENT_USER, "Software\\CodePoet Computing\\WinUAE", 0, KEY_ALL_ACCESS, &hPixelFormatKey ) == ERROR_SUCCESS )
	    {
		char szMessage[ 4096 ];
		char szTitle[ MAX_PATH ];
		WIN32GUI_LoadUIString( IDS_GFXCARDCHECK, szMessage, 4096 );
		WIN32GUI_LoadUIString( IDS_GFXCARDTITLE, szTitle, MAX_PATH );
		    
		if( MessageBox( NULL, szMessage, szTitle, 
				MB_YESNO | MB_ICONWARNING | MB_TASKMODAL | MB_SETFOREGROUND ) == IDYES )
		{
		    colortype = WIN32GFX_FigurePixelFormats(0);
		    RegSetValueEx( hPixelFormatKey, "DisplayInfo", 0, REG_DWORD, (CONST BYTE *)&colortype, sizeof( colortype ) );
		}
		RegCloseKey( hPixelFormatKey );
	    }
	    else
	    {
		write_log ( "WinUAE is missing HKEY_LOCAL_MACHINE Registry keys!  These can only be created by a valid Setup program.\n" );
	    }
	    DirectDraw_Release();
	}
	else
	{
	    values_from_displaydlg (hDlg, msg, wParam, lParam);
	    enable_for_displaydlg( hDlg );
	}
	recursive--;
	break;

    case WM_NOTIFY:
	switch (((NMHDR *) lParam)->code) 
	{
	case PSN_HELP:
	    HtmlHelp( NULL, help_file, HH_DISPLAY_TOPIC, "display.htm" );
	    break;

	case PSN_APPLY:
	    /* Copy stuff from workprefs and config_xxx settings */
	    gui_to_prefs ();
	    break;

	case PSN_RESET:
	    if (allow_quit) 
	    {
		quit_program = 1;
		regs.spcflags |= SPCFLAG_BRK;
	    }
	    break;
	}
	break;
    }
    return FALSE;
}

static void values_to_chipsetdlg (HWND hDlg)
{
    char buffer[ MAX_FRAMERATE_LENGTH + MAX_NTH_LENGTH ];
    char Nth[ MAX_NTH_LENGTH ];
    LPSTR blah[1] = { Nth };
    LPTSTR string = NULL;
    int d,d2,index;

    switch( workprefs.chipset_mask )
    {
    case 0:
	CheckRadioButton( hDlg, IDC_OCS, IDC_AGA, IDC_OCS+0 );
	break;
    case CSMASK_ECS_AGNUS:
	CheckRadioButton( hDlg, IDC_OCS, IDC_AGA, IDC_OCS+1 );
	break;
    case CSMASK_ECS_DENISE:
	CheckRadioButton( hDlg, IDC_OCS, IDC_AGA, IDC_OCS+2 );
	break;
    case CSMASK_ECS_AGNUS | CSMASK_ECS_DENISE:
	CheckRadioButton( hDlg, IDC_OCS, IDC_AGA, IDC_OCS+3 );
	break;
    case CSMASK_AGA:
    case CSMASK_ECS_AGNUS | CSMASK_ECS_DENISE | CSMASK_AGA:
	CheckRadioButton( hDlg, IDC_OCS, IDC_AGA, IDC_OCS+4 );
	break;
    }

    CheckDlgButton (hDlg, IDC_BLITIMM, workprefs.immediate_blits);    
    CheckDlgButton (hDlg, IDC_NTSC, workprefs.ntscmode);

    CheckRadioButton (hDlg, IDC_COLLISION0, IDC_COLLISION3, IDC_COLLISION0 + workprefs.collision_level);
    CheckDlgButton (hDlg, IDC_FASTCOPPER, workprefs.fast_copper );
}

static void values_from_chipsetdlg (HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
    BOOL success = FALSE;

    workprefs.fast_copper = IsDlgButtonChecked (hDlg, IDC_FASTCOPPER);
    workprefs.immediate_blits = IsDlgButtonChecked (hDlg, IDC_BLITIMM);
    workprefs.collision_level = (IsDlgButtonChecked (hDlg, IDC_COLLISION0) ? 0
				 : IsDlgButtonChecked (hDlg, IDC_COLLISION1) ? 1
				 : IsDlgButtonChecked (hDlg, IDC_COLLISION2) ? 2 : 3);
    workprefs.chipset_mask = (IsDlgButtonChecked( hDlg, IDC_OCS ) ? 0
			      : IsDlgButtonChecked( hDlg, IDC_ECS_AGNUS ) ? CSMASK_ECS_AGNUS
			      : IsDlgButtonChecked( hDlg, IDC_ECS_DENISE ) ? CSMASK_ECS_DENISE
			      : IsDlgButtonChecked( hDlg, IDC_ECS ) ? CSMASK_ECS_AGNUS | CSMASK_ECS_DENISE
			      : CSMASK_AGA | CSMASK_ECS_AGNUS | CSMASK_ECS_DENISE);
    workprefs.ntscmode = (IsDlgButtonChecked (hDlg, IDC_NTSC) ? 1 : 0);
}

static BOOL CALLBACK ChipsetDlgProc (HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
    static int recursive = 0;
    HKEY hPixelFormatKey;
    RGBFTYPE colortype      = RGBFB_NONE;
    DWORD dwType            = REG_DWORD;
    DWORD dwDisplayInfoSize = sizeof( colortype );

    switch (msg) {
    case WM_INITDIALOG:
	pages[CHIPSET_ID] = hDlg;

    case WM_USER:
	recursive++;
	values_to_chipsetdlg (hDlg);
	enable_for_chipsetdlg (hDlg);
	recursive--;
	break;

    case WM_HSCROLL:
    case WM_COMMAND:
	if (recursive > 0)
	    break;
	recursive++;
	values_from_chipsetdlg (hDlg, msg, wParam, lParam);
	enable_for_chipsetdlg( hDlg );
	recursive--;
	break;

    case WM_NOTIFY:
	switch (((NMHDR *) lParam)->code) {
	case PSN_HELP:
	    HtmlHelp( NULL, help_file, HH_DISPLAY_TOPIC, "chipset.htm" );
	    break;

	case PSN_APPLY:
	    /* Copy stuff from workprefs and config_xxx settings */
	    gui_to_prefs ();
	    break;

	case PSN_RESET:
	    if (allow_quit) {
		quit_program = 1;
		regs.spcflags |= SPCFLAG_BRK;
	    }
	    break;
	}
	break;
    }
    return FALSE;
}

static void enable_for_memorydlg (HWND hDlg)
{
    EnableWindow (GetDlgItem (hDlg, IDC_Z3TEXT), ! workprefs.address_space_24);
    EnableWindow (GetDlgItem (hDlg, IDC_Z3FASTRAM), ! workprefs.address_space_24);
    EnableWindow (GetDlgItem (hDlg, IDC_Z3FASTMEM), ! workprefs.address_space_24);
    EnableWindow (GetDlgItem (hDlg, IDC_FASTMEM), workprefs.chipmem_size <= 0x200000);
    EnableWindow (GetDlgItem (hDlg, IDC_FASTRAM), workprefs.chipmem_size <= 0x200000);
    EnableWindow (GetDlgItem (hDlg, IDC_FASTTEXT), workprefs.chipmem_size <= 0x200000);
    EnableWindow (GetDlgItem (hDlg, IDC_GFXCARDTEXT), ! workprefs.address_space_24);
    EnableWindow (GetDlgItem (hDlg, IDC_P96RAM), ! workprefs.address_space_24);
    EnableWindow (GetDlgItem (hDlg, IDC_P96MEM), ! workprefs.address_space_24);
}

static void values_to_memorydlg (HWND hDlg)
{
    uae_u32 mem_size = 0;

    switch (workprefs.chipmem_size) {
     case 0x00080000: mem_size = 0; break;
     case 0x00100000: mem_size = 1; break;
     case 0x00200000: mem_size = 2; break;
     case 0x00400000: mem_size = 3; break;
     case 0x00800000: mem_size = 4; break;
    }
    SendDlgItemMessage (hDlg, IDC_CHIPMEM, TBM_SETPOS, TRUE, mem_size);
    SetDlgItemText (hDlg, IDC_CHIPRAM, memsize_names[msi_chip[mem_size]]);

    mem_size = 0;
    switch (workprefs.fastmem_size) {
     case 0x00000000: mem_size = 0; break;
     case 0x00100000: mem_size = 1; break;
     case 0x00200000: mem_size = 2; break;
     case 0x00400000: mem_size = 3; break;
     case 0x00800000: mem_size = 4; break;
     case 0x01000000: mem_size = 5; break;
    }
    SendDlgItemMessage (hDlg, IDC_FASTMEM, TBM_SETPOS, TRUE, mem_size);
    SetDlgItemText (hDlg, IDC_FASTRAM, memsize_names[msi_fast[mem_size]]);

    mem_size = 0;
    switch (workprefs.bogomem_size) {
     case 0x00000000: mem_size = 0; break;
     case 0x00080000: mem_size = 1; break;
     case 0x00100000: mem_size = 2; break;
    }
    SendDlgItemMessage (hDlg, IDC_SLOWMEM, TBM_SETPOS, TRUE, mem_size);
    SetDlgItemText (hDlg, IDC_SLOWRAM, memsize_names[msi_bogo[mem_size]]);

    mem_size = 0;
    switch (workprefs.z3fastmem_size) {
     case 0x00000000: mem_size = 0; break; /*   0-megs */
     case 0x00100000: mem_size = 1; break; /*   1-megs */
     case 0x00200000: mem_size = 2; break; /*   2-megs */
     case 0x00400000: mem_size = 3; break; /*   4-megs */
     case 0x00800000: mem_size = 4; break; /*   8-megs */
     case 0x01000000: mem_size = 5; break; /*  16-megs */
     case 0x02000000: mem_size = 6; break; /*  32-megs */
     case 0x04000000: mem_size = 7; break; /*  64-megs */
     case 0x08000000: mem_size = 8; break; /* 128-megs */
	 case 0x10000000: mem_size = 9; break; /* 256-megs */
	 case 0x20000000: mem_size = 10; break; /* 512-megs */
     case 0x40000000: mem_size = 11; break; /* 1 GB-megs */

    }
    SendDlgItemMessage (hDlg, IDC_Z3FASTMEM, TBM_SETPOS, TRUE, mem_size);
    SetDlgItemText (hDlg, IDC_Z3FASTRAM, memsize_names[msi_z3fast[mem_size]]);

    mem_size = 0;
    switch (workprefs.gfxmem_size) {
     case 0x00000000: mem_size = 0; break;
     case 0x00100000: mem_size = 1; break;
     case 0x00200000: mem_size = 2; break;
     case 0x00400000: mem_size = 3; break;
     case 0x00800000: mem_size = 4; break;
	 case 0x01000000: mem_size = 5; break;
	 case 0x02000000: mem_size = 6; break;
    }
    SendDlgItemMessage (hDlg, IDC_P96MEM, TBM_SETPOS, TRUE, mem_size);
    SetDlgItemText (hDlg, IDC_P96RAM, memsize_names[msi_gfx[mem_size]]);
}

static void fix_values_memorydlg (void)
{
    if (workprefs.chipmem_size > 0x200000)
	workprefs.fastmem_size = 0;
}

static BOOL CALLBACK MemoryDlgProc (HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
    static int recursive = 0;
    unsigned int max_z3_mem = MAX_Z3_MEM;
    MEMORYSTATUS memstats;

    switch (msg)
    {
    case WM_INITDIALOG:
	pages[MEMORY_ID] = hDlg;

	memstats.dwLength = sizeof( memstats );
	GlobalMemoryStatus( &memstats );
	while( ( memstats.dwAvailPageFile + memstats.dwAvailPhys - 32000000) < (DWORD)( 1 << (max_z3_mem + 19) ) )
	    max_z3_mem--;

	SendDlgItemMessage (hDlg, IDC_CHIPMEM, TBM_SETRANGE, TRUE, MAKELONG (MIN_CHIP_MEM, MAX_CHIP_MEM));
	SendDlgItemMessage (hDlg, IDC_FASTMEM, TBM_SETRANGE, TRUE, MAKELONG (MIN_FAST_MEM, MAX_FAST_MEM));
	SendDlgItemMessage (hDlg, IDC_SLOWMEM, TBM_SETRANGE, TRUE, MAKELONG (MIN_SLOW_MEM, MAX_SLOW_MEM));
	SendDlgItemMessage (hDlg, IDC_Z3FASTMEM, TBM_SETRANGE, TRUE, MAKELONG (MIN_Z3_MEM, max_z3_mem));
	SendDlgItemMessage (hDlg, IDC_P96MEM, TBM_SETRANGE, TRUE, MAKELONG (MIN_P96_MEM, MAX_P96_MEM));

    case WM_USER:
	recursive++;
	fix_values_memorydlg ();
	values_to_memorydlg (hDlg);
	enable_for_memorydlg (hDlg);
	recursive--;
	break;

    case WM_HSCROLL:
	workprefs.chipmem_size = memsizes[msi_chip[SendMessage (GetDlgItem (hDlg, IDC_CHIPMEM), TBM_GETPOS, 0, 0)]];
	workprefs.bogomem_size = memsizes[msi_bogo[SendMessage (GetDlgItem (hDlg, IDC_SLOWMEM), TBM_GETPOS, 0, 0)]];
	workprefs.fastmem_size = memsizes[msi_fast[SendMessage (GetDlgItem (hDlg, IDC_FASTMEM), TBM_GETPOS, 0, 0)]];
	workprefs.z3fastmem_size = memsizes[msi_z3fast[SendMessage (GetDlgItem (hDlg, IDC_Z3FASTMEM), TBM_GETPOS, 0, 0)]];
	workprefs.gfxmem_size = memsizes[msi_gfx[SendMessage (GetDlgItem (hDlg, IDC_P96MEM), TBM_GETPOS, 0, 0)]];
	fix_values_memorydlg();
	values_to_memorydlg(hDlg);
	enable_for_memorydlg(hDlg);
	break;

    case WM_COMMAND:
	if (recursive > 0)
	    break;
	recursive++;
	values_to_memorydlg (hDlg);
	recursive--;
	break;

    case WM_NOTIFY:
	switch (((NMHDR *) lParam)->code) 
	{
	case PSN_HELP:
	    HtmlHelp( NULL, help_file, HH_DISPLAY_TOPIC, "ram.htm" );
	    break;

	case PSN_APPLY:
	    /* Copy stuff from workprefs and config_xxx settings */
	    gui_to_prefs ();
	    break;

	case PSN_RESET:
	    if (allow_quit) 
	    {
		quit_program = 1;
		regs.spcflags |= SPCFLAG_BRK;
	    }
	    break;
	}
    }
    return FALSE;
}

static void values_to_kickstartdlg (HWND hDlg)
{
    SetDlgItemText( hDlg, IDC_ROMFILE, workprefs.romfile );
    SetDlgItemText( hDlg, IDC_KEYFILE, workprefs.keyfile );
    // Disable Expansion ROM settings for now (CD32 related)
    ShowWindow( GetDlgItem( hDlg, IDC_ROMFILE2TEXT ), SW_HIDE );
    ShowWindow( GetDlgItem( hDlg, IDC_ROMFILE2 ), SW_HIDE );
    ShowWindow( GetDlgItem( hDlg, IDC_ROMCHOOSER2 ), SW_HIDE );
}

static BOOL CALLBACK KickstartDlgProc (HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch( msg ) 
    {
    case WM_INITDIALOG:
	pages[KICKSTART_ID] = hDlg;

    case WM_USER:
	values_to_kickstartdlg (hDlg);
	return TRUE;

    case WM_COMMAND:
	switch( wParam ) 
	{
	case IDC_KICKCHOOSER:
	    DiskSelection( hDlg, IDC_ROMFILE, 6, &workprefs );
	    break;
		    
	case IDC_KEYCHOOSER:
	    DiskSelection( hDlg, IDC_KEYFILE, 7, &workprefs );
	    break;
		
	default:
	    if( SendMessage( GetDlgItem( hDlg, IDC_ROMFILE ), EM_GETMODIFY, 0, 0 ) )
	    {
		GetDlgItemText( hDlg, IDC_ROMFILE, workprefs.romfile, CFG_ROM_LENGTH);
		SendMessage( GetDlgItem( hDlg, IDC_ROMFILE ), EM_SETMODIFY, 0, 0 );
	    }
	    if( SendMessage( GetDlgItem( hDlg, IDC_KEYFILE ), EM_GETMODIFY, 0, 0 ) )
	    {
		GetDlgItemText( hDlg, IDC_KEYFILE, workprefs.keyfile, CFG_KEY_LENGTH);
		SendMessage( GetDlgItem( hDlg, IDC_ROMFILE ), EM_SETMODIFY, 0, 0 );
	    }
	    break;
	}
	break;

    case WM_NOTIFY:
	switch( ((NMHDR *) lParam)->code ) 
	{
	case PSN_HELP:
	    HtmlHelp( NULL, help_file, HH_DISPLAY_TOPIC, "rom.htm" );
	    break;

	case PSN_APPLY:
	    /* Copy stuff from workprefs and config_xxx settings */
	    gui_to_prefs ();
	    break;
		
	case PSN_RESET:
	    if (allow_quit) 
	    {
		quit_program = 1;
		regs.spcflags |= SPCFLAG_BRK;
	    }
	    break;
	}
	break;
    }
    return FALSE;
}

static void enable_for_miscdlg (HWND hDlg)
{
    if( !full_property_sheet )
    {
	EnableWindow( GetDlgItem( hDlg, IDC_JULIAN), FALSE);
	EnableWindow( GetDlgItem( hDlg, IDC_SOCKETS), FALSE);
	EnableWindow( GetDlgItem( hDlg, IDC_SHOWGUI ), FALSE );
	EnableWindow( GetDlgItem( hDlg, IDC_AUTOCONFIG ), FALSE );
	EnableWindow( GetDlgItem( hDlg, IDC_CREATELOGFILE ), FALSE );
	EnableWindow( GetDlgItem( hDlg, IDC_ILLEGAL ), FALSE );
	EnableWindow( GetDlgItem( hDlg, IDC_NOSPEED ), FALSE );
	EnableWindow( GetDlgItem( hDlg, IDC_NOSOUND ), FALSE );
	EnableWindow( GetDlgItem( hDlg, IDC_NOOVERLAY ), FALSE );
	ShowWindow( GetDlgItem( hDlg, IDC_RESETAMIGA ), SW_SHOW );
	ShowWindow( GetDlgItem( hDlg, IDC_QUITEMU ), SW_SHOW );
    }
    // Turn off cd.device and c2p h/w (Akiko) check-boxes for now
    ShowWindow( GetDlgItem( hDlg, IDC_AKIKOC2P ), SW_HIDE );
    ShowWindow( GetDlgItem( hDlg, IDC_CDDEVICE ), SW_HIDE );
}

static void values_to_miscdlg (HWND hDlg)
{
    CheckDlgButton( hDlg, IDC_SOCKETS, workprefs.socket_emu );
    CheckDlgButton( hDlg, IDC_ILLEGAL, workprefs.illegal_mem);
}

static BOOL CALLBACK MiscDlgProc (HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg) 
    {
    case WM_INITDIALOG:
	pages[MISC_ID] = hDlg;

    case WM_USER:
	values_to_miscdlg (hDlg);
	enable_for_miscdlg (hDlg);
	return TRUE;

    case WM_COMMAND:
	switch( wParam )
	{
	case IDC_DOSAVESTATE:
	    DiskSelection( hDlg, wParam, 9, &workprefs );
	    if( strlen( savestate_filename ) )
	    {
		savestate_state = STATE_DOSAVE;
	    }
	    break;
	case IDC_DOLOADSTATE:
	    DiskSelection( hDlg, wParam, 10, &workprefs );
	    if( strlen( savestate_filename ) )
	    {
		savestate_state = STATE_DORESTORE;
	    }
	    break;
	case IDC_RESETAMIGA:
	    uae_reset();
	    break;
	case IDC_QUITEMU:
	    uae_quit();
	    PropSheet_PressButton( hPropertySheet, PSBTN_CANCEL );
	    break;
	case IDC_SOCKETS:
	    workprefs.socket_emu   = IsDlgButtonChecked( hDlg, IDC_SOCKETS );
	    break;
	case IDC_ILLEGAL:
	    workprefs.illegal_mem = IsDlgButtonChecked (hDlg, IDC_ILLEGAL);
	    break;
	}
	break;

    case WM_NOTIFY:
	switch (((NMHDR *) lParam)->code) 
	{
	case PSN_HELP:
	    HtmlHelp( NULL, help_file, HH_DISPLAY_TOPIC, "misc.htm" );
	    break;

	case PSN_APPLY:
	    /* Copy stuff from workprefs and config_xxx settings */
	    gui_to_prefs ();
	    break;

	case PSN_RESET:
	    if (allow_quit) 
	    {
		quit_program = 1;
		regs.spcflags |= SPCFLAG_BRK;
	    }
	    break;
	}
	break;
    }
    return FALSE;
}

static int cpu_ids[]   = { IDC_CPU0, IDC_CPU0, IDC_CPU1, IDC_CPU1, IDC_CPU2, IDC_CPU4, IDC_CPU3, IDC_CPU5, IDC_CPU6, IDC_CPU6 };
static int trust_ids[] = { IDC_TRUST0, IDC_TRUST1, IDC_TRUST1, IDC_TRUST2 };

static void enable_for_cpudlg (HWND hDlg)
{
    /* The "compatible" checkbox is only available when CPU type is 68000 */
    EnableWindow (GetDlgItem (hDlg, IDC_COMPATIBLE), workprefs.cpu_level == 0);

    /* These four items only get enabled when adjustable CPU style is enabled */
    EnableWindow (GetDlgItem (hDlg, IDC_SPEED), workprefs.m68k_speed > 0);
    EnableWindow (GetDlgItem (hDlg, IDC_CS_CPU_TEXT), workprefs.m68k_speed > 0);
    EnableWindow (GetDlgItem (hDlg, IDC_CS_CHIPSET_TEXT), workprefs.m68k_speed > 0);
    EnableWindow (GetDlgItem (hDlg, IDC_CPUTEXT), workprefs.m68k_speed > 0 );

}

static void values_to_cpudlg (HWND hDlg)
{
    SendDlgItemMessage (hDlg, IDC_SPEED, TBM_SETPOS, TRUE, workprefs.m68k_speed <= 0 ? 1 : workprefs.m68k_speed / CYCLE_UNIT );
    SetDlgItemInt( hDlg, IDC_CPUTEXT, workprefs.m68k_speed <= 0 ? 1 : workprefs.m68k_speed / CYCLE_UNIT, FALSE );
    CheckDlgButton (hDlg, IDC_COMPATIBLE, workprefs.cpu_compatible);
    CheckRadioButton (hDlg, IDC_CPU0, IDC_CPU6, cpu_ids[workprefs.cpu_level * 2 + !workprefs.address_space_24]);

    if (workprefs.m68k_speed == -1)
	CheckRadioButton( hDlg, IDC_CS_HOST, IDC_CS_ADJUSTABLE, IDC_CS_HOST );
    else if (workprefs.m68k_speed == 0)
	CheckRadioButton( hDlg, IDC_CS_HOST, IDC_CS_ADJUSTABLE, IDC_CS_68000 );
    else
	CheckRadioButton( hDlg, IDC_CS_HOST, IDC_CS_ADJUSTABLE, IDC_CS_ADJUSTABLE );
}

static void values_from_cpudlg (HWND hDlg)
{
    int newcpu;
    int newtrust;
    
    workprefs.cpu_compatible = IsDlgButtonChecked (hDlg, IDC_COMPATIBLE);
    workprefs.m68k_speed = IsDlgButtonChecked (hDlg, IDC_CS_HOST) ? -1
	: IsDlgButtonChecked (hDlg, IDC_CS_68000) ? 0
	: SendMessage (GetDlgItem (hDlg, IDC_SPEED), TBM_GETPOS, 0, 0) * CYCLE_UNIT;
    
    newcpu = (IsDlgButtonChecked (hDlg, IDC_CPU0) ? 0
	: IsDlgButtonChecked (hDlg, IDC_CPU1) ? 1
	: IsDlgButtonChecked (hDlg, IDC_CPU2) ? 2
	: IsDlgButtonChecked (hDlg, IDC_CPU3) ? 3
	: IsDlgButtonChecked (hDlg, IDC_CPU4) ? 4
	: IsDlgButtonChecked (hDlg, IDC_CPU5) ? 5 : 6);
    /* When switching away from 68000, disable 24 bit addressing.  */
    switch( newcpu )
    {
	case 0: // 68000
	case 1: // 68010
	case 2: // 68EC020
	case 3: // 68EC020+FPU
	    workprefs.address_space_24 = 1;
	    workprefs.cpu_level = newcpu;
	break;

	case 4: // 68020
	case 5: // 68020+FPU
	case 6: // 68040
	    workprefs.address_space_24 = 0;
	    workprefs.cpu_level = newcpu - 2;
	break;
    }
    if (newcpu > 0)
	workprefs.cpu_compatible = 0;

    if( pages[ KICKSTART_ID ] )
	SendMessage( pages[ KICKSTART_ID ], WM_USER, 0, 0 );
    if( pages[ DISPLAY_ID ] )
	SendMessage( pages[ DISPLAY_ID ], WM_USER, 0, 0 );
    if( pages[ MEMORY_ID ] )
	SendMessage( pages[ MEMORY_ID ], WM_USER, 0, 0 );
}

static BOOL CALLBACK CPUDlgProc (HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
    static int recursive = 0;

    switch (msg) {
    case WM_INITDIALOG:
	pages[CPU_ID] = hDlg;
	SendDlgItemMessage (hDlg, IDC_SPEED, TBM_SETRANGE, TRUE, MAKELONG (MIN_M68K_PRIORITY, MAX_M68K_PRIORITY));
	SendDlgItemMessage (hDlg, IDC_SPEED, TBM_SETPAGESIZE, 0, 1);

    case WM_USER:
	recursive++;
	values_to_cpudlg (hDlg);
	enable_for_cpudlg (hDlg);
	recursive--;
	return TRUE;

    case WM_COMMAND:
	if (recursive > 0)
	    break;
	recursive++;
	values_from_cpudlg (hDlg);
	values_to_cpudlg (hDlg);
	enable_for_cpudlg (hDlg);
	recursive--;
	break;

    case WM_HSCROLL:
	recursive++;
	values_from_cpudlg( hDlg );
	values_to_cpudlg( hDlg );
	enable_for_cpudlg( hDlg );
	recursive--;
	break;

    case WM_NOTIFY:
	switch (((NMHDR *) lParam)->code) {
	case PSN_HELP:
	    HtmlHelp( NULL, help_file, HH_DISPLAY_TOPIC, "cpu.htm" );
	    break;
	case PSN_APPLY:
	    /* Copy stuff from workprefs and config_xxx settings */
	    gui_to_prefs ();
	    break;

	case PSN_RESET:
	    if (allow_quit) {
		quit_program = 1;
		regs.spcflags |= SPCFLAG_BRK;
	    }
	    break;
	}
    }
    return FALSE;
}

static BOOL sound_frequency_supported[ 4 ] = { 0, 0, 0, 0 };

static void fill_sound_frequency_supported( void )
{
    WAVEFORMATEX wavfmt;
    HWAVEOUT hWaveOut = NULL;
    int i = 0;
    MMRESULT result;

    wavfmt.cbSize = 0;
    wavfmt.wFormatTag = WAVE_FORMAT_PCM;
    wavfmt.nChannels = 2;
    wavfmt.nSamplesPerSec = 11025;
    wavfmt.wBitsPerSample = 16;
    wavfmt.nBlockAlign = 4;
    wavfmt.nAvgBytesPerSec = wavfmt.nBlockAlign * wavfmt.nSamplesPerSec;

    for( i = 0; i < 4; i++ )
    {
	if( i == 3 )
	    wavfmt.nSamplesPerSec = 48000;
	else
	    wavfmt.nSamplesPerSec = 11025 * (i+1);
	wavfmt.nAvgBytesPerSec = wavfmt.nBlockAlign * wavfmt.nSamplesPerSec;
	result = waveOutOpen( &hWaveOut, WAVE_MAPPER, &wavfmt, 0, 0, WAVE_FORMAT_QUERY );
	if( result == MMSYSERR_NOERROR )
	{
	    sound_frequency_supported[i] = 1;
	}
	else
	{
	    write_log ( "WAVEOUT: Unsupported format of %d Hz\n", wavfmt.nSamplesPerSec );
	}
    }
}

static void enable_for_sounddlg (HWND hDlg)
{
    EnableWindow( GetDlgItem( hDlg, IDC_FREQUENCY ), workprefs.produce_sound );
    EnableWindow( GetDlgItem( hDlg, IDC_11KHZ ), workprefs.produce_sound && sound_frequency_supported[0] );
    EnableWindow( GetDlgItem( hDlg, IDC_22KHZ ), workprefs.produce_sound && sound_frequency_supported[1] );
    EnableWindow( GetDlgItem( hDlg, IDC_44KHZ ), workprefs.produce_sound && sound_frequency_supported[2] );
    EnableWindow( GetDlgItem( hDlg, IDC_48KHZ ), workprefs.produce_sound && sound_frequency_supported[3] );

    EnableWindow( GetDlgItem( hDlg, IDC_SOUNDSIZE ), workprefs.produce_sound );
    EnableWindow( GetDlgItem( hDlg, IDC_8BIT ), workprefs.produce_sound );
    EnableWindow( GetDlgItem( hDlg, IDC_16BIT ), workprefs.produce_sound );

    EnableWindow( GetDlgItem( hDlg, IDC_STEREOMODE ), workprefs.produce_sound );
    EnableWindow( GetDlgItem( hDlg, IDC_STEREOMODE0 ), workprefs.produce_sound );
    EnableWindow( GetDlgItem( hDlg, IDC_STEREOMODE1 ), workprefs.produce_sound );
    EnableWindow( GetDlgItem( hDlg, IDC_STEREOMODE2 ), workprefs.produce_sound );

    EnableWindow( GetDlgItem( hDlg, IDC_SOUNDINTERPOLATION ), workprefs.produce_sound );
    EnableWindow( GetDlgItem( hDlg, IDC_INTERPOLATION0 ), workprefs.produce_sound );
    EnableWindow( GetDlgItem( hDlg, IDC_INTERPOLATION1 ), workprefs.produce_sound );

    EnableWindow( GetDlgItem( hDlg, IDC_SOUNDBUFFERMEM ), workprefs.produce_sound );
    EnableWindow( GetDlgItem( hDlg, IDC_SOUNDBUFFERRAM ), workprefs.produce_sound );
    EnableWindow( GetDlgItem( hDlg, IDC_SOUNDBUFFERTEXT ), workprefs.produce_sound );

    EnableWindow( GetDlgItem( hDlg, IDC_AUDIOSYNC ), workprefs.produce_sound );
}

static int exact_log2 (int v)
{
    int l = 0;
    while ((v >>= 1) != 0)
	l++;
    return l;
}

static void values_to_sounddlg (HWND hDlg)
{
    int which_button;

    if( (workprefs.sound_freq <= 11025) && sound_frequency_supported[0] )
	which_button = IDC_11KHZ;
    else if( (workprefs.sound_freq <= 22050) && sound_frequency_supported[1] )
	which_button = IDC_22KHZ;
    else if( (workprefs.sound_freq <= 44100) && sound_frequency_supported[2] )
	which_button = IDC_44KHZ;
    else if( sound_frequency_supported[3] )
	which_button = IDC_48KHZ;
    else
	which_button = IDC_22KHZ; /* default */
    CheckRadioButton (hDlg, IDC_11KHZ, IDC_48KHZ, which_button);

    CheckRadioButton (hDlg, IDC_8BIT, IDC_16BIT, workprefs.sound_bits == 16 ? IDC_16BIT : IDC_8BIT);

    switch (workprefs.produce_sound) {
     case 0: which_button = IDC_SOUND0; break;
     case 1: which_button = IDC_SOUND1; break;
     case 2: which_button = IDC_SOUND2; break;
     case 3: which_button = IDC_SOUND3; break;
    }
    
    CheckRadioButton( hDlg, IDC_SOUND0, IDC_SOUND3, which_button );

    switch (workprefs.stereo) 
    {
    case 0:
	// mono
	which_button = IDC_STEREOMODE0;
	break;
    case 1:
    default:
	// stereo, but which type?
	if( workprefs.mixed_stereo )
	    which_button = IDC_STEREOMODE2;
	else
	    which_button = IDC_STEREOMODE1;
	break;
    }
    CheckRadioButton( hDlg, IDC_STEREOMODE0, IDC_STEREOMODE2, which_button );

    CheckRadioButton( hDlg, IDC_INTERPOLATION0, IDC_INTERPOLATION1, IDC_INTERPOLATION0 + workprefs.sound_interpol );
    SendDlgItemMessage( hDlg, IDC_SOUNDBUFFERRAM, TBM_SETPOS, TRUE, exact_log2 (currprefs.sound_maxbsiz / 256));
    SetDlgItemInt( hDlg, IDC_SOUNDBUFFERMEM, workprefs.sound_maxbsiz, FALSE );
}

static void values_from_sounddlg (HWND hDlg)
{
    workprefs.sound_bits = IsDlgButtonChecked (hDlg, IDC_8BIT) ? 8 : 16;
    workprefs.sound_freq = (IsDlgButtonChecked (hDlg, IDC_11KHZ) ? 11025
			    : IsDlgButtonChecked (hDlg, IDC_22KHZ) ? 22050
			    : IsDlgButtonChecked (hDlg, IDC_44KHZ) ? 44100 : 48000);
    workprefs.produce_sound = (IsDlgButtonChecked (hDlg, IDC_SOUND0) ? 0
			       : IsDlgButtonChecked (hDlg, IDC_SOUND1) ? 1
			       : IsDlgButtonChecked (hDlg, IDC_SOUND2) ? 2 : 3);
    workprefs.mixed_stereo = 0;
    workprefs.stereo = IsDlgButtonChecked (hDlg, IDC_STEREOMODE0) ? 0 :
		       IsDlgButtonChecked( hDlg, IDC_STEREOMODE1 ) ? 1 : (workprefs.mixed_stereo = 1);

    workprefs.sound_interpol = IsDlgButtonChecked( hDlg, IDC_INTERPOLATION0 ) ? 0 : 1;
    workprefs.sound_maxbsiz = GetDlgItemInt( hDlg, IDC_SOUNDBUFFERMEM, NULL, FALSE );
}

static BOOL CALLBACK SoundDlgProc (HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
    static int recursive = 0;

    switch (msg) {
    case WM_INITDIALOG:
	fill_sound_frequency_supported();
	SendDlgItemMessage (hDlg, IDC_SOUNDBUFFERRAM, TBM_SETRANGE, TRUE, MAKELONG (MIN_SOUND_MEM, MAX_SOUND_MEM));
	SendDlgItemMessage (hDlg, IDC_SOUNDBUFFERRAM, TBM_SETPAGESIZE, 0, 1);

	pages[SOUND_ID] = hDlg;

    case WM_USER:
	recursive++;
	values_to_sounddlg (hDlg);
	enable_for_sounddlg (hDlg);
	recursive--;
	return TRUE;

    case WM_COMMAND:
	if (recursive > 0)
	    break;
	recursive++;
	values_from_sounddlg (hDlg);
	enable_for_sounddlg (hDlg);
	recursive--;
	break;

     case WM_HSCROLL:
	workprefs.sound_maxbsiz = 512 << SendMessage( GetDlgItem( hDlg, IDC_SOUNDBUFFERRAM ), TBM_GETPOS, 0, 0 );
	SetDlgItemInt( hDlg, IDC_SOUNDBUFFERMEM, workprefs.sound_maxbsiz, FALSE );
	break;

    case WM_NOTIFY:
	switch (((NMHDR *) lParam)->code) {
	case PSN_HELP:
	    HtmlHelp( NULL, help_file, HH_DISPLAY_TOPIC, "sound.htm" );
	    break;
	case PSN_APPLY:
	    /* Copy stuff from workprefs and config_xxx settings */
	    gui_to_prefs ();
	    break;
	case PSN_RESET:
	    if (allow_quit) {
		quit_program = 1;
		regs.spcflags |= SPCFLAG_BRK;
	    }
	    break;
	}
	break;
    }
    return FALSE;
}

struct fsvdlg_vals
{
    char volume[4096];
    char rootdir[4096];
    int rw;
};

static struct fsvdlg_vals empty_fsvdlg = { "", "", 1 };
static struct fsvdlg_vals current_fsvdlg;

struct hfdlg_vals
{
    char volumename[4096];
    char filename[4096];
    int sectors;
    int reserved;
    int surfaces;
    int cylinders;
    int blocksize;
    int rw;
};

static struct hfdlg_vals empty_hfdlg = { "", "", 32, 2, 1, 0, 512, 1 };
static struct hfdlg_vals current_hfdlg;

static int CALLBACK VolumeSettingsProc (HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
    static int recursive = 0;
    BROWSEINFO browse_info;
    char directory_path[MAX_PATH] = "";
    LPITEMIDLIST browse;
    char szTitle[ MAX_PATH ];

    WIN32GUI_LoadUIString( IDS_SELECTFILESYSROOT, szTitle, MAX_PATH );

    browse_info.hwndOwner = hDlg;
    browse_info.pidlRoot = NULL;
    browse_info.pszDisplayName = directory_path;
    browse_info.lpszTitle = "";
    browse_info.ulFlags = BIF_DONTGOBELOWDOMAIN | BIF_RETURNONLYFSDIRS;
    browse_info.lpfn = NULL;
    browse_info.iImage = 0;

    switch (msg) {
     case WM_INITDIALOG:
	recursive++;
	SetDlgItemText (hDlg, IDC_VOLUME_NAME, current_fsvdlg.volume);
	SetDlgItemText (hDlg, IDC_PATH_NAME, current_fsvdlg.rootdir);
	CheckDlgButton (hDlg, IDC_RW, current_fsvdlg.rw);
	recursive--;
	return TRUE;

     case WM_COMMAND:
	if (recursive)
	    break;
	recursive++;
	if (HIWORD (wParam) == BN_CLICKED) {
	    switch (LOWORD (wParam)) {
	     case IDC_SELECTOR:
		if ((browse = SHBrowseForFolder (&browse_info)) != NULL) {
		    SHGetPathFromIDList (browse, directory_path);
		    SetDlgItemText (hDlg, IDC_PATH_NAME, directory_path);
		}
		break;
	     case IDOK:
		if( strlen( current_fsvdlg.rootdir ) == 0 ) 
		{
		    char szMessage[ MAX_PATH ];
		    char szTitle[ MAX_PATH ];
		    WIN32GUI_LoadUIString( IDS_MUSTSELECTPATH, szMessage, MAX_PATH );
		    WIN32GUI_LoadUIString( IDS_SETTINGSERROR, szTitle, MAX_PATH );

		    MessageBox( hDlg, szMessage, szTitle,
				MB_OK | MB_ICONERROR | MB_APPLMODAL | MB_SETFOREGROUND);
		    break;
		}
		if( strlen( current_fsvdlg.volume ) == 0 )
		{
		    char szMessage[ MAX_PATH ];
		    char szTitle[ MAX_PATH ];
		    WIN32GUI_LoadUIString( IDS_MUSTSELECTNAME, szMessage, MAX_PATH );
		    WIN32GUI_LoadUIString( IDS_SETTINGSERROR, szTitle, MAX_PATH );

		    MessageBox( hDlg, szMessage, szTitle,
				MB_OK | MB_ICONERROR | MB_APPLMODAL | MB_SETFOREGROUND);
		    break;
		}
		EndDialog (hDlg, 1);

		break;
	     case IDCANCEL:
		EndDialog (hDlg, 0);
		break;
	    }
	}
	GetDlgItemText (hDlg, IDC_PATH_NAME, current_fsvdlg.rootdir, sizeof current_fsvdlg.rootdir);
	GetDlgItemText (hDlg, IDC_VOLUME_NAME, current_fsvdlg.volume, sizeof current_fsvdlg.volume);
	current_fsvdlg.rw = IsDlgButtonChecked (hDlg, IDC_RW);
	recursive--;
	break;
    }
    return FALSE;
}

static int CALLBACK HardfileSettingsProc (HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
    static int recursive = 0;
    UINT setting;

    switch (msg) {
    case WM_INITDIALOG:
	recursive++;
	SetDlgItemText (hDlg, IDC_PATH_NAME, current_hfdlg.filename);
	SetDlgItemInt( hDlg, IDC_SECTORS, current_hfdlg.sectors, FALSE);
	SetDlgItemInt( hDlg, IDC_HEADS, current_hfdlg.surfaces, FALSE);
	SetDlgItemInt( hDlg, IDC_RESERVED, current_hfdlg.reserved, FALSE);
	SetDlgItemInt( hDlg, IDC_BLOCKSIZE, current_hfdlg.blocksize, FALSE);
	CheckDlgButton (hDlg, IDC_RW, current_hfdlg.rw);
	recursive--;
	return TRUE;

    case WM_COMMAND:
	if (recursive)
	    break;
	recursive++;

	if (HIWORD (wParam) == BN_CLICKED) {
	    switch (LOWORD (wParam)) {
	    case IDC_CREATEHF:
		setting = CalculateHardfileSize (hDlg);
		if( !CreateHardFile(hDlg, setting) )
		{
		    char szMessage[ MAX_PATH ];
		    char szTitle[ MAX_PATH ];
		    WIN32GUI_LoadUIString( IDS_FAILEDHARDFILECREATION, szMessage, MAX_PATH );
		    WIN32GUI_LoadUIString( IDS_CREATIONERROR, szTitle, MAX_PATH );

		    MessageBox( hDlg, szMessage, szTitle,
				MB_OK | MB_ICONERROR | MB_APPLMODAL | MB_SETFOREGROUND);
		}
		break;
	    case IDC_SELECTOR:
		DiskSelection (hDlg, IDC_PATH_NAME, 2, &workprefs );
		break;
	    case IDOK:
		if( strlen( current_hfdlg.filename ) == 0 ) 
		{
		    char szMessage[ MAX_PATH ];
		    char szTitle[ MAX_PATH ];
		    WIN32GUI_LoadUIString( IDS_MUSTSELECTFILE, szMessage, MAX_PATH );
		    WIN32GUI_LoadUIString( IDS_SETTINGSERROR, szTitle, MAX_PATH );

		    MessageBox( hDlg, szMessage, szTitle,
				MB_OK | MB_ICONERROR | MB_APPLMODAL | MB_SETFOREGROUND);
		    break;
		}
		EndDialog (hDlg, 1);
		break;
	    case IDCANCEL:
		EndDialog (hDlg, 0);
		break;
	    }
	}

	GetDlgItemText (hDlg, IDC_PATH_NAME, current_hfdlg.filename, sizeof current_hfdlg.filename);
	current_hfdlg.sectors   = GetDlgItemInt( hDlg, IDC_SECTORS, FALSE, FALSE );
	current_hfdlg.reserved  = GetDlgItemInt( hDlg, IDC_RESERVED, FALSE, FALSE );
	current_hfdlg.surfaces  = GetDlgItemInt( hDlg, IDC_HEADS, FALSE, FALSE );
	current_hfdlg.blocksize = GetDlgItemInt( hDlg, IDC_BLOCKSIZE, FALSE, FALSE );
	current_hfdlg.rw = IsDlgButtonChecked (hDlg, IDC_RW);
	recursive--;

	break;
    }
    return FALSE;
}

static void new_filesys (HWND hDlg)
{
    const char *result;

    result = add_filesys_unit (workprefs.mountinfo, current_fsvdlg.volume,
			       current_fsvdlg.rootdir, ! current_fsvdlg.rw, 0, 0, 0, 0);
    if (result)
	MessageBox (hDlg, result, "Bad directory",
		    MB_OK | MB_ICONERROR | MB_APPLMODAL | MB_SETFOREGROUND);
}

static void new_hardfile (HWND hDlg)
{
    const char *result;

    result = add_filesys_unit (workprefs.mountinfo, 0, current_hfdlg.filename,
			       ! current_hfdlg.rw, current_hfdlg.sectors, current_hfdlg.surfaces,
			       current_hfdlg.reserved, current_hfdlg.blocksize);
    if (result)
	MessageBox (hDlg, result, "Bad hardfile",
		    MB_OK | MB_ICONERROR | MB_APPLMODAL | MB_SETFOREGROUND);
}

static void harddisk_remove (HWND hDlg)
{
    int entry = listview_find_selected (GetDlgItem (hDlg, IDC_VOLUMELIST));
    if (entry < 0)
	return;
    kill_filesys_unit (workprefs.mountinfo, entry);
}

static void harddisk_move (HWND hDlg, int up)
{
    int entry = listview_find_selected (GetDlgItem (hDlg, IDC_VOLUMELIST));
    if (entry < 0)
	return;
    move_filesys_unit (workprefs.mountinfo, entry, up ? entry - 1 : entry + 1);
}

static void harddisk_edit (HWND hDlg)
{
    int entry = listview_find_selected (GetDlgItem (hDlg, IDC_VOLUMELIST));
    char *volname, *rootdir;
    int secspertrack, surfaces, cylinders, reserved, size, blocksize, readonly;
    const char *failure;

    if (entry < 0)
	return;
    
    failure = get_filesys_unit (workprefs.mountinfo, entry, &volname, &rootdir, &readonly,
			    &secspertrack, &surfaces, &reserved, &cylinders, &size,
			    &blocksize);
    
    if( is_hardfile( workprefs.mountinfo, entry ) )
    {
	current_hfdlg.sectors = secspertrack;
	current_hfdlg.surfaces = surfaces;
	current_hfdlg.reserved = reserved;
	current_hfdlg.cylinders = cylinders;
	current_hfdlg.blocksize = blocksize;

	strncpy (current_hfdlg.filename, rootdir, (sizeof current_hfdlg.filename) - 1);
	current_hfdlg.filename[(sizeof current_hfdlg.filename) - 1] = '\0';
	current_hfdlg.rw = !readonly;
	if (DialogBox( hUIDLL ? hUIDLL : hInst, MAKEINTRESOURCE (IDD_HARDFILE), hDlg, HardfileSettingsProc)) 
	{
	    const char *result;
	    result = set_filesys_unit (workprefs.mountinfo, entry, 0, current_hfdlg.filename,
				       ! current_hfdlg.rw, current_hfdlg.sectors, current_hfdlg.surfaces,
				       current_hfdlg.reserved, current_hfdlg.blocksize);
	    if (result)
		MessageBox (hDlg, result, "Bad hardfile",
		MB_OK | MB_ICONERROR | MB_APPLMODAL | MB_SETFOREGROUND);
	}
    }
    else /* Filesystem */
    {
	strncpy (current_fsvdlg.rootdir, rootdir, (sizeof current_fsvdlg.rootdir) - 1);
	current_fsvdlg.rootdir[(sizeof current_fsvdlg.rootdir) - 1] = '\0';
	strncpy (current_fsvdlg.volume, volname, (sizeof current_fsvdlg.volume) - 1);
	current_fsvdlg.volume[(sizeof current_fsvdlg.volume) - 1] = '\0';
	current_fsvdlg.rw = !readonly;
	if (DialogBox( hUIDLL ? hUIDLL : hInst, MAKEINTRESOURCE (IDD_FILESYS), hDlg, VolumeSettingsProc)) {
	    const char *result;
	    result = set_filesys_unit (workprefs.mountinfo, entry, current_fsvdlg.volume,
				       current_fsvdlg.rootdir, ! current_fsvdlg.rw, 0, 0, 0, 0);
	    if (result)
		MessageBox (hDlg, result, "Bad hardfile",
		MB_OK | MB_ICONERROR | MB_APPLMODAL | MB_SETFOREGROUND);
	}
    }
}

static HWND cachedlist = NULL;

static void harddiskdlg_button (HWND hDlg, int button)
{
    switch (button) {
     case IDC_NEW_FS:
	current_fsvdlg = empty_fsvdlg;
	if (DialogBox( hUIDLL ? hUIDLL : hInst, MAKEINTRESOURCE (IDD_FILESYS), hDlg, VolumeSettingsProc))
	    new_filesys (hDlg);
	break;

     case IDC_NEW_HF:
	current_hfdlg = empty_hfdlg;
	if (DialogBox( hUIDLL ? hUIDLL : hInst, MAKEINTRESOURCE (IDD_HARDFILE), hDlg, HardfileSettingsProc))
	    new_hardfile (hDlg);
	break;

     case IDC_EDIT:
	harddisk_edit (hDlg);
	break;

     case IDC_REMOVE:
	harddisk_remove (hDlg);
	break;

     case IDC_UP:
	harddisk_move (hDlg, 1);
	clicked_entry--;
	break;

     case IDC_DOWN:
	harddisk_move (hDlg, 0);
	clicked_entry++;
	break;
     
    case IDC_MAPDRIVES:
	break;
    }
}

static void harddiskdlg_volume_notify (HWND hDlg, NM_LISTVIEW *nmlistview)
{
    HWND list = nmlistview->hdr.hwndFrom;
    int dblclick = 0;
    int entry = 0;

    switch (nmlistview->hdr.code) {
     case NM_DBLCLK:
	dblclick = 1;
	/* fall through */
     case NM_CLICK:
	entry = listview_entry_from_click (list);
	if (entry >= 0)
	{
	    if(dblclick)
		harddisk_edit (hDlg);
	    InitializeListView( hDlg );
	    clicked_entry = entry;
	    cachedlist = list;
	    // Hilite the current selected item
	    ListView_SetItemState( cachedlist, clicked_entry, LVIS_SELECTED, LVIS_SELECTED );
	}
	break;
    }
}

static BOOL CALLBACK HarddiskDlgProc (HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
    static HICON hMoveUp = NULL, hMoveDown = NULL;

    switch (msg) {
    case WM_INITDIALOG:
	clicked_entry = 0;
	pages[HARDDISK_ID] = hDlg;
	if( !hMoveUp )
	    hMoveUp = (HICON)LoadImage( hInst, MAKEINTRESOURCE( IDI_MOVE_UP ), IMAGE_ICON, 16, 16, LR_LOADMAP3DCOLORS );
	if( !hMoveDown )
	    hMoveDown = (HICON)LoadImage( hInst, MAKEINTRESOURCE( IDI_MOVE_DOWN ), IMAGE_ICON, 16, 16, LR_LOADMAP3DCOLORS );
	SendMessage( GetDlgItem( hDlg, IDC_UP ), BM_SETIMAGE, (WPARAM)IMAGE_ICON, (LPARAM)hMoveUp );
	SendMessage( GetDlgItem( hDlg, IDC_DOWN ), BM_SETIMAGE, (WPARAM)IMAGE_ICON, (LPARAM)hMoveDown );
	
    case WM_USER:
	InitializeListView( hDlg );
	break;
	
    case WM_COMMAND:
	if (HIWORD (wParam) == BN_CLICKED)
	{
	    harddiskdlg_button (hDlg, LOWORD (wParam));
	    InitializeListView( hDlg );

	    if( clicked_entry < 0 )
		clicked_entry = 0;
	    if( clicked_entry >= ListView_GetItemCount( cachedlist ) )
		clicked_entry = ListView_GetItemCount( cachedlist ) - 1;

	    if( cachedlist && clicked_entry >= 0 )
	    {
		// Hilite the current selected item
		ListView_SetItemState( cachedlist, clicked_entry, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED );
	    }
	}
	break;
	
    case WM_NOTIFY:
	if (((LPNMHDR) lParam)->idFrom == IDC_VOLUMELIST)
	    harddiskdlg_volume_notify (hDlg, (NM_LISTVIEW *) lParam);
	else {
	    switch (((NMHDR *) lParam)->code) {
	    case PSN_HELP:
		HtmlHelp( NULL, help_file, HH_DISPLAY_TOPIC, "hard-drives.htm" );
		break;
	    case PSN_APPLY:
		/* Copy stuff from workprefs and config_xxx settings */
		gui_to_prefs ();
		break;
	    case PSN_RESET:
		if (allow_quit) {
		    quit_program = 1;
		    regs.spcflags |= SPCFLAG_BRK;
		}
		break;
	    }
	}
	return TRUE;
    default:
	return FALSE;
    }
    
    return FALSE;
}

static void EnableFloppyButtons( HWND hDlg, int floppies )
{
    switch( floppies )
    {
	case 1:
	    EnableWindow( GetDlgItem( hDlg, IDC_DISABLE1 ), TRUE );
	    EnableWindow( GetDlgItem( hDlg, IDC_DF1TEXT ), FALSE );
	    EnableWindow( GetDlgItem( hDlg, IDC_DF1 ), FALSE );
	    EnableWindow( GetDlgItem( hDlg, IDC_EJECT1 ), FALSE );

	    EnableWindow( GetDlgItem( hDlg, IDC_DISABLE2 ), FALSE );
	    EnableWindow( GetDlgItem( hDlg, IDC_DF2TEXT ), FALSE );
	    EnableWindow( GetDlgItem( hDlg, IDC_DF2 ), FALSE );
	    EnableWindow( GetDlgItem( hDlg, IDC_EJECT2 ), FALSE );

	    EnableWindow( GetDlgItem( hDlg, IDC_DISABLE3 ), FALSE );
	    EnableWindow( GetDlgItem( hDlg, IDC_DF3TEXT ), FALSE );
	    EnableWindow( GetDlgItem( hDlg, IDC_DF3 ), FALSE );
	    EnableWindow( GetDlgItem( hDlg, IDC_EJECT3 ), FALSE );

	    CheckDlgButton( hDlg, IDC_DISABLE1, BST_CHECKED );
	    CheckDlgButton( hDlg, IDC_DISABLE2, BST_CHECKED );
	    CheckDlgButton( hDlg, IDC_DISABLE3, BST_CHECKED );
	break;
	case 2:
	    EnableWindow( GetDlgItem( hDlg, IDC_DISABLE1 ), TRUE );
	    EnableWindow( GetDlgItem( hDlg, IDC_DF1TEXT ), TRUE );
	    EnableWindow( GetDlgItem( hDlg, IDC_DF1 ), TRUE );
	    EnableWindow( GetDlgItem( hDlg, IDC_EJECT1 ), TRUE );

	    EnableWindow( GetDlgItem( hDlg, IDC_DISABLE2 ), TRUE );
	    EnableWindow( GetDlgItem( hDlg, IDC_DF2TEXT ), FALSE );
	    EnableWindow( GetDlgItem( hDlg, IDC_DF2 ), FALSE );
	    EnableWindow( GetDlgItem( hDlg, IDC_EJECT2 ), FALSE );

	    EnableWindow( GetDlgItem( hDlg, IDC_DISABLE3 ), FALSE );
	    EnableWindow( GetDlgItem( hDlg, IDC_DF3TEXT ), FALSE );
	    EnableWindow( GetDlgItem( hDlg, IDC_DF3 ), FALSE );
	    EnableWindow( GetDlgItem( hDlg, IDC_EJECT3 ), FALSE );

	    CheckDlgButton( hDlg, IDC_DISABLE2, BST_CHECKED );
	    CheckDlgButton( hDlg, IDC_DISABLE3, BST_CHECKED );
	break;
	case 3:
	    EnableWindow( GetDlgItem( hDlg, IDC_DISABLE2 ), TRUE );
	    EnableWindow( GetDlgItem( hDlg, IDC_DF2TEXT ), TRUE );
	    EnableWindow( GetDlgItem( hDlg, IDC_DF2 ), TRUE );
	    EnableWindow( GetDlgItem( hDlg, IDC_EJECT2 ), TRUE );

	    EnableWindow( GetDlgItem( hDlg, IDC_DISABLE3 ), TRUE );
	    EnableWindow( GetDlgItem( hDlg, IDC_DF3TEXT ), FALSE );
	    EnableWindow( GetDlgItem( hDlg, IDC_DF3 ), FALSE );
	    EnableWindow( GetDlgItem( hDlg, IDC_EJECT3 ), FALSE );

	    CheckDlgButton( hDlg, IDC_DISABLE3, BST_CHECKED );
	break;
	case 4:
	    EnableWindow( GetDlgItem( hDlg, IDC_DISABLE3 ), TRUE );
	    EnableWindow( GetDlgItem( hDlg, IDC_DF3TEXT ), TRUE );
	    EnableWindow( GetDlgItem( hDlg, IDC_DF3 ), TRUE );
	    EnableWindow( GetDlgItem( hDlg, IDC_EJECT3 ), TRUE );
	break;
    }
}

static BOOL CALLBACK FloppyDlgProc (HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
    static int recursive = 0;

    switch (msg) 
    {
    case WM_INITDIALOG:
	pages[FLOPPY_ID] = hDlg;
	EnableFloppyButtons( hDlg, workprefs.nr_floppies );
	    
    case WM_USER:
	recursive++;
	SetDlgItemText (hDlg, IDC_DF0TEXT, workprefs.df[0]);
	SetDlgItemText (hDlg, IDC_DF1TEXT, workprefs.df[1]);
	SetDlgItemText (hDlg, IDC_DF2TEXT, workprefs.df[2]);
	SetDlgItemText (hDlg, IDC_DF3TEXT, workprefs.df[3]);
	recursive--;
	break;
	    
    case WM_COMMAND:
	if (recursive > 0)
	    break;
	recursive++;
	switch (wParam) 
	{
	case IDC_DISABLE1:
	    if( IsDlgButtonChecked( hDlg, IDC_DISABLE1 ) == BST_CHECKED )
	    {
		EnableFloppyButtons( hDlg, 1 );
		workprefs.nr_floppies = 1;
	    }
	    else
	    {
		EnableFloppyButtons( hDlg, 2 );
		workprefs.nr_floppies = 2;
	    }
	    break;
	case IDC_DISABLE2:
	    if( IsDlgButtonChecked( hDlg, IDC_DISABLE2 ) == BST_CHECKED )
	    {
		EnableWindow( GetDlgItem( hDlg, IDC_DISABLE3 ), FALSE );
		CheckDlgButton( hDlg, IDC_DISABLE3, BST_CHECKED );
		EnableFloppyButtons( hDlg, 2 );
		workprefs.nr_floppies = 2;
	    }
	    else
	    {
		EnableFloppyButtons( hDlg, 3 );
		workprefs.nr_floppies = 3;
	    }
	    break;
	case IDC_DISABLE3:
	    if( IsDlgButtonChecked( hDlg, IDC_DISABLE3 ) == BST_CHECKED )
	    {
		EnableFloppyButtons( hDlg, 3 );
		workprefs.nr_floppies = 3;
	    }
	    else
	    {
		EnableFloppyButtons( hDlg, 4 );
		workprefs.nr_floppies = 4;
	    }
	    break;
	case IDC_DF0:
	    DiskSelection (hDlg, wParam, 0, &workprefs );
	    break;
	case IDC_DF1:
	    DiskSelection (hDlg, wParam, 0, &workprefs );
	    break;
	case IDC_DF2:
	    DiskSelection (hDlg, wParam, 0, &workprefs );
	    break;
	case IDC_DF3:
	    DiskSelection (hDlg, wParam, 0, &workprefs );
	    break;
	case IDC_EJECT0:
	    disk_eject(0);
	    SetDlgItemText (hDlg, IDC_DF0TEXT, "");
	    workprefs.df[0][0] = 0;
	    break;
	case IDC_EJECT1:
	    disk_eject(1);
	    SetDlgItemText (hDlg, IDC_DF1TEXT, "");
	    workprefs.df[1][0] = 0;
	    break;
	case IDC_EJECT2:
	    disk_eject(2);
	    SetDlgItemText (hDlg, IDC_DF2TEXT, "");
	    workprefs.df[2][0] = 0;
	    break;
	case IDC_EJECT3:
	    disk_eject(3);
	    SetDlgItemText (hDlg, IDC_DF3TEXT, "");
	    workprefs.df[3][0] = 0;
	    break;
	case IDC_CREATE:
	    DiskSelection (hDlg, wParam, 1, &workprefs );
	    break;
	case IDC_CREATE_RAW:
	    DiskSelection( hDlg, wParam, 1, &workprefs );
	    break;
	}
	if( SendMessage( GetDlgItem( hDlg, IDC_DF0TEXT ), EM_GETMODIFY, 0, 0 ) )
	{
	    GetDlgItemText (hDlg, IDC_DF0TEXT, workprefs.df[0], 255);
	    SendMessage( GetDlgItem( hDlg, IDC_DF0TEXT ), EM_SETMODIFY, 0, 0 );
	}

	if( SendMessage( GetDlgItem( hDlg, IDC_DF1TEXT ), EM_GETMODIFY, 0, 0 ) )
	{
	    GetDlgItemText (hDlg, IDC_DF1TEXT, workprefs.df[1], 255);
	    SendMessage( GetDlgItem( hDlg, IDC_DF1TEXT ), EM_SETMODIFY, 0, 0 );
	}

	if( SendMessage( GetDlgItem( hDlg, IDC_DF2TEXT ), EM_GETMODIFY, 0, 0 ) )
	{
	    GetDlgItemText (hDlg, IDC_DF2TEXT, workprefs.df[2], 255);
	    SendMessage( GetDlgItem( hDlg, IDC_DF2TEXT ), EM_SETMODIFY, 0, 0 );
	}

	if( SendMessage( GetDlgItem( hDlg, IDC_DF3TEXT ), EM_GETMODIFY, 0, 0 ) )
	{
	    GetDlgItemText (hDlg, IDC_DF3TEXT, workprefs.df[3], 255);
	    SendMessage( GetDlgItem( hDlg, IDC_DF3TEXT ), EM_SETMODIFY, 0, 0 );
	}
	recursive--;
	break;

    case WM_NOTIFY:
	switch (((NMHDR *) lParam)->code) 
	{
	case PSN_HELP:
	    HtmlHelp( NULL, help_file, HH_DISPLAY_TOPIC, "floppies.htm" );
	    break;
	case PSN_APPLY:
	    /* Copy stuff from workprefs and config_xxx settings */
	    gui_to_prefs ();
	    break;
	case PSN_RESET:
	    if (allow_quit) {
		quit_program = 1;
		regs.spcflags |= SPCFLAG_BRK;
	    }
	    break;
	}
	return TRUE;
    default:
	return FALSE;
    }

    return FALSE;
}

PRINTER_INFO_1 *pInfo = NULL;
DWORD dwEnumeratedPrinters = 0;

static int joy0idc[] = {
    IDC_PORT0_JOY0, IDC_PORT0_JOY1, IDC_PORT0_MOUSE, IDC_PORT0_KBDA, IDC_PORT0_KBDB, IDC_PORT0_KBDC
};

static int joy1idc[] = {
    IDC_PORT1_JOY0, IDC_PORT1_JOY1, IDC_PORT1_MOUSE, IDC_PORT1_KBDA, IDC_PORT1_KBDB, IDC_PORT1_KBDC
};

static BOOL bNoMidiIn = FALSE;

static void enable_for_portsdlg( HWND hDlg )
{
    int i;
    if (! full_property_sheet) 
    {
	/* Disable certain controls which are only to be set once at start-up... */
	EnableWindow( GetDlgItem( hDlg, IDC_SERIAL ), FALSE );
	EnableWindow( GetDlgItem( hDlg, IDC_PARALLEL ), FALSE );
	EnableWindow( GetDlgItem( hDlg, IDC_MIDIOUTLIST ), FALSE );
	EnableWindow( GetDlgItem( hDlg, IDC_MIDIINLIST ), FALSE );
	EnableWindow( GetDlgItem( hDlg, IDC_SHARED ), FALSE );
    }
    for( i = 0; i < 6; i++ )
    {
	EnableWindow( GetDlgItem( hDlg, joy0idc[i] ), TRUE );
	EnableWindow( GetDlgItem( hDlg, joy1idc[i] ), TRUE );
    }
}

static void UpdatePortRadioButtons( HWND hDlg )
{
    int which_button1, which_button2;

    enable_for_portsdlg( hDlg );
	which_button1 = joy0idc[workprefs.jport0];
	if (CheckRadioButton (hDlg, IDC_PORT0_JOY0, IDC_PORT0_KBDC, which_button1) == 0)
	    which_button1 = 0;
    else
    {
	EnableWindow( GetDlgItem( hDlg, joy1idc[workprefs.jport0] ), FALSE );
    }
	which_button2 = joy1idc[workprefs.jport1];
    if( workprefs.jport1 == workprefs.jport0 )
    {
	if( which_button2 == IDC_PORT1_KBDC )
	    which_button2 = IDC_PORT1_KBDB;
	else
	    which_button2++;
    }
	if (CheckRadioButton (hDlg, IDC_PORT1_JOY0, IDC_PORT1_KBDC, which_button2) == 0)
	    which_button2 = 0;
    else
    {
	EnableWindow( GetDlgItem( hDlg, joy0idc[ workprefs.jport1 ] ), FALSE );
    }
}

static void values_from_portsdlg (HWND hDlg)
{
    int item;
    /* 0 - joystick 0
     * 1 - joystick 1
     * 2 - mouse
     * 3 - numpad
     * 4 - cursor keys
     * 5 - elsewhere
     */
    if (IsDlgButtonChecked (hDlg, IDC_PORT0_JOY0)) {
	    workprefs.jport0 = 0;
    }
    if (IsDlgButtonChecked (hDlg, IDC_PORT0_JOY1)) {
	    workprefs.jport0 = 1;
    }
    if (IsDlgButtonChecked (hDlg, IDC_PORT0_MOUSE))
	    workprefs.jport0 = 2;
    if (IsDlgButtonChecked (hDlg, IDC_PORT0_KBDA))
	    workprefs.jport0 = 3;
    if (IsDlgButtonChecked (hDlg, IDC_PORT0_KBDB))
	    workprefs.jport0 = 4;
    if (IsDlgButtonChecked (hDlg, IDC_PORT0_KBDC))
	    workprefs.jport0 = 5;

    if (IsDlgButtonChecked (hDlg, IDC_PORT1_JOY0)) {
	    workprefs.jport1 = 0;
    }
    if (IsDlgButtonChecked (hDlg, IDC_PORT1_JOY1)) {
	    workprefs.jport1 = 1;
    }
    if (IsDlgButtonChecked (hDlg, IDC_PORT1_MOUSE))
	    workprefs.jport1 = 2;
    if (IsDlgButtonChecked (hDlg, IDC_PORT1_KBDA))
	    workprefs.jport1 = 3;
    if (IsDlgButtonChecked (hDlg, IDC_PORT1_KBDB))
	    workprefs.jport1 = 4;
    if (IsDlgButtonChecked (hDlg, IDC_PORT1_KBDC))
	    workprefs.jport1 = 5;

    item = SendDlgItemMessage( hDlg, IDC_PRINTERLIST, CB_GETCURSEL, 0, 0L );
    if( item != CB_ERR )
    {
	if( item )
	    strcpy( workprefs.prtname, pInfo[item-1].pName );
	else
	    strcpy( workprefs.prtname, "" );
    }

    item = SendDlgItemMessage (hDlg, IDC_SERIAL, CB_GETCURSEL, 0, 0L);
    switch( item ) 
    {
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
	    workprefs.use_serial = 1;
	    sprintf( workprefs.sername, "COM%d", item );
	    EnableWindow( GetDlgItem( hDlg, IDC_MIDIOUTLIST ), TRUE );
	    EnableWindow( GetDlgItem( hDlg, IDC_MIDIINLIST ), TRUE );
	break;

	default:
	    workprefs.use_serial = 0;
	    strcpy( workprefs.sername, "none" );
	    EnableWindow( GetDlgItem( hDlg, IDC_MIDIOUTLIST ), FALSE );
	    EnableWindow( GetDlgItem( hDlg, IDC_MIDIINLIST ), FALSE );
	break;
    }
    if( IsDlgButtonChecked( hDlg, IDC_SHARED ) )
	workprefs.serial_demand = 1;
    else
	workprefs.serial_demand = 0;
}

static void values_to_portsdlg (HWND hDlg)
{
    LONG item_height, result = 0;
    RECT rect;

    if( *workprefs.prtname )
    {
	result = SendDlgItemMessage( hDlg, IDC_PRINTERLIST, CB_FINDSTRINGEXACT, -1, (LPARAM)workprefs.prtname );
	if( result < 0 )
	{
	    // Warn the user that their printer-port selection is not valid on this machine
	    char szMessage[ MAX_PATH ];
	    WIN32GUI_LoadUIString( IDS_INVALIDPRTPORT, szMessage, MAX_PATH );
	    gui_message( szMessage );
	    
	    // Disable the invalid parallel-port selection
	    strcpy( workprefs.prtname, "none" );

	    result = 0;
	}
    }
    SendDlgItemMessage( hDlg, IDC_PRINTERLIST, CB_SETCURSEL, result, 0 );
    
    CheckDlgButton( hDlg, IDC_SHARED, workprefs.serial_demand );
    
    if( strcasecmp( workprefs.sername, szNone ) == 0 ) 
    {
	SendDlgItemMessage (hDlg, IDC_SERIAL, CB_SETCURSEL, 0, 0L);
	workprefs.use_serial = 0;
    }
    else
    {
	int t = (workprefs.sername[0] == '\0' ? 0 : workprefs.sername[3] - '0');
	result = SendDlgItemMessage( hDlg, IDC_SERIAL, CB_SETCURSEL, t, 0L );
	if( result < 0 )
	{
	    if (t > 0) {
		// Warn the user that their COM-port selection is not valid on this machine
		char szMessage[ MAX_PATH ];
		WIN32GUI_LoadUIString( IDS_INVALIDCOMPORT, szMessage, MAX_PATH );
		gui_message( szMessage );

		// Select "none" as the COM-port
		SendDlgItemMessage( hDlg, IDC_SERIAL, CB_SETCURSEL, 0L, 0L );		
	    }
	    // Disable the chosen serial-port selection
	    strcpy( workprefs.sername, "none" );
	    workprefs.use_serial = 0;
	}
	else
	{
	    workprefs.use_serial = 1;
	}
    }

    if( workprefs.use_serial )
    {
	EnableWindow( GetDlgItem( hDlg, IDC_MIDIOUTLIST ), TRUE );
	if( !bNoMidiIn )
	    EnableWindow( GetDlgItem( hDlg, IDC_MIDIINLIST ), TRUE );
    }
    else
    {
	EnableWindow( GetDlgItem( hDlg, IDC_MIDIOUTLIST ), FALSE );
	EnableWindow( GetDlgItem( hDlg, IDC_MIDIINLIST ), FALSE );
    }
    /* Retrieve the height, in pixels, of a list item. */
    item_height = SendDlgItemMessage (hDlg, IDC_SERIAL, CB_GETITEMHEIGHT, 0, 0L);
    if (item_height != CB_ERR) {
	/* Get actual box position and size. */
	GetWindowRect (GetDlgItem (hDlg, IDC_SERIAL), &rect);
	rect.bottom = (rect.top + item_height * 5
	    + SendDlgItemMessage (hDlg, IDC_SERIAL, CB_GETITEMHEIGHT, (WPARAM) - 1, 0L)
	    + item_height);
	SetWindowPos (GetDlgItem (hDlg, IDC_SERIAL), 0, 0, 0, rect.right - rect.left,
	    rect.bottom - rect.top, SWP_NOMOVE | SWP_NOZORDER);
    }
}

#define MAX_PRINTERS 10

static void init_portsdlg( HWND hDlg )
{
    int port, numdevs;
    char comport[5];
    COMMCONFIG cc;
    DWORD size = sizeof(COMMCONFIG);

    MIDIOUTCAPS midiOutCaps;
    MIDIINCAPS midiInCaps;

    DWORD cbNeeded = 0;

    SendDlgItemMessage (hDlg, IDC_SERIAL, CB_RESETCONTENT, 0, 0L);
    SendDlgItemMessage (hDlg, IDC_SERIAL, CB_ADDSTRING, 0, (LPARAM)szNone );
    for( port = 0; port < 8; port++ )
    {
	sprintf( comport, "COM%d", port );
	if( GetDefaultCommConfig( comport, &cc, &size ) )
	{
	    SendDlgItemMessage( hDlg, IDC_SERIAL, CB_ADDSTRING, 0, (LPARAM)comport );
	}
    }

    SendDlgItemMessage (hDlg, IDC_PRINTERLIST, CB_RESETCONTENT, 0, 0L);
    SendDlgItemMessage (hDlg, IDC_PRINTERLIST, CB_ADDSTRING, 0, (LPARAM)szNone );
    if( !pInfo )
    {
	if( !EnumPrinters( PRINTER_ENUM_LOCAL, NULL, 1, (LPBYTE)pInfo, 0, &cbNeeded, &dwEnumeratedPrinters ) )
	{
	    if( cbNeeded )
	    {
		pInfo = calloc( 1, cbNeeded );
		if( EnumPrinters( PRINTER_ENUM_LOCAL, NULL, 1, (LPBYTE)pInfo, cbNeeded, &cbNeeded, &dwEnumeratedPrinters ) )
		{
		    for( port = 0; port < dwEnumeratedPrinters; port++ )
		    {
			SendDlgItemMessage( hDlg, IDC_PRINTERLIST, CB_ADDSTRING, 0, (LPARAM)pInfo[port].pName );
		    }
		}
	    }
	    else
	    {
		EnableWindow( GetDlgItem( hDlg, IDC_PRINTERLIST ), FALSE );
	    }
	}
    }

    if( ( numdevs = midiOutGetNumDevs() ) == 0 )
    {
	EnableWindow( GetDlgItem( hDlg, IDC_MIDIOUTLIST ), FALSE );
    }
    else
    {
	char szMidiOut[ MAX_PATH ];
	WIN32GUI_LoadUIString( IDS_DEFAULTMIDIOUT, szMidiOut, MAX_PATH );
	SendDlgItemMessage( hDlg, IDC_MIDIOUTLIST, CB_RESETCONTENT, 0, 0L );
	SendDlgItemMessage( hDlg, IDC_MIDIOUTLIST, CB_ADDSTRING, 0, (LPARAM)szMidiOut );

	for( port = 0; port < numdevs; port++ )
	{
	    if( midiOutGetDevCaps( port, &midiOutCaps, sizeof( midiOutCaps ) ) == MMSYSERR_NOERROR )
	    {
		SendDlgItemMessage( hDlg, IDC_MIDIOUTLIST, CB_ADDSTRING, 0, (LPARAM)midiOutCaps.szPname );
	    }
	}
    }

    if( ( numdevs = midiInGetNumDevs() ) == 0 )
    {
	EnableWindow( GetDlgItem( hDlg, IDC_MIDIINLIST ), FALSE );
	bNoMidiIn = TRUE;
    }
    else
    {
	SendDlgItemMessage( hDlg, IDC_MIDIINLIST, CB_RESETCONTENT, 0, 0L );

	for( port = 0; port < numdevs; port++ )
	{
	    if( midiInGetDevCaps( port, &midiInCaps, sizeof( midiInCaps ) ) == MMSYSERR_NOERROR )
	    {
		SendDlgItemMessage( hDlg, IDC_MIDIINLIST, CB_ADDSTRING, 0, (LPARAM)midiInCaps.szPname );
	    }
	}
    }
}

/* Handle messages for the Joystick Settings page of our property-sheet */
static BOOL CALLBACK PortsDlgProc (HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
    static int recursive = 0;
    int temp;

    switch (msg) 
    {
    case WM_INITDIALOG:
	pages[PORTS_ID] = hDlg;
	init_portsdlg( hDlg );
	    
    case WM_USER:
	recursive++;
	enable_for_portsdlg( hDlg );
	values_to_portsdlg (hDlg);
	UpdatePortRadioButtons( hDlg );
	recursive--;
	return TRUE;

    case WM_COMMAND:
	if( wParam == IDC_SWAP )
	{
	    temp = workprefs.jport0;
	    workprefs.jport0 = workprefs.jport1;
	    workprefs.jport1 = temp;
	    UpdatePortRadioButtons( hDlg );
	}
	else
	{
	    if (recursive > 0)
		break;
	    recursive++;
	    values_from_portsdlg (hDlg);
	    UpdatePortRadioButtons( hDlg );
	    recursive--;
	}
	break;

    case WM_NOTIFY:
	switch (((NMHDR *) lParam)->code) 
	{
	case PSN_HELP:
	    HtmlHelp( NULL, help_file, HH_DISPLAY_TOPIC, "ports.htm" );
	    break;
	case PSN_APPLY:
	    /* Copy stuff from workprefs and config_xxx settings */
	    gui_to_prefs ();
	    break;
	case PSN_RESET:
	    if (allow_quit) {
		quit_program = 1;
		regs.spcflags |= SPCFLAG_BRK;
	    }
	    break;
	}
	return FALSE;
    }
    return FALSE;
}

static void CALLBACK InitPropertySheet (HWND hDlg, UINT msg, LPARAM lParam)
{
    int i;
    hPropertySheet = hDlg;

    switch (msg) 
    {
    case PSCB_INITIALIZED:
	if (! full_property_sheet) {
	    /* This shouldn't be necessary.  */
	    PropSheet_RemovePage( hDlg, 0, NULL ); /* Remove LoadSave */
	} else {
	    for (i = 0; i < C_PAGES; i++)
		pages[i] = NULL;
	}
	break;
    }
}

static void init_page (PROPSHEETPAGE *ppage, int id, int tmpl, int icon, int title,
	       BOOL (CALLBACK FAR *func) (HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam))
{
    ppage[id].dwSize = sizeof (PROPSHEETPAGE);
    ppage[id].dwFlags = PSP_USETITLE | PSP_USEICONID | ((id != ABOUT_ID ) ? PSP_HASHELP : 0);
    ppage[id].pszTemplate = MAKEINTRESOURCE (tmpl);
    ppage[id].hInstance = hUIDLL ? hUIDLL : hInst;
    ppage[id].pszIcon = MAKEINTRESOURCE (icon);

    if( hUIDLL )
    {
	LPTSTR lpstrTitle = calloc( 1, MAX_PATH );
	LoadString( hUIDLL, title, lpstrTitle, MAX_PATH );
	ppage[id].pszTitle = lpstrTitle;
    }
    else
    {
	ppage[id].pszTitle = MAKEINTRESOURCE (title);
    }
    ppage[id].pfnDlgProc = func;
    ppage[id].lParam = 0;
    ppage[id].pfnCallback = NULL;
    ppage[id].pcRefParent = NULL;
}

int GetSettings (int all_options)
{
    static int init_called = 0;
    int psresult;
    static PROPSHEETPAGE ppage[C_PAGES];
    PROPSHEETHEADER pHeader;

    full_property_sheet = all_options;
    allow_quit = all_options;
    pguiprefs = all_options ? &currprefs : &changed_prefs;
    default_prefs( &workprefs );

    WIN32GUI_LoadUIString( IDS_NONE, szNone, MAX_PATH );

    prefs_to_gui (&changed_prefs);

    if( !init_called )
    {
	init_page (ppage, LOADSAVE_ID, IDD_LOADSAVE, IDI_LOADSAVE, IDS_LOADSAVE, LoadSaveDlgProc);
	init_page (ppage, MEMORY_ID, IDD_MEMORY, IDI_MEMORY, IDS_MEMORY, MemoryDlgProc);
	init_page (ppage, KICKSTART_ID, IDD_KICKSTART, IDI_MEMORY, IDS_KICKSTART, KickstartDlgProc);
	init_page (ppage, CPU_ID, IDD_CPU, IDI_CPU, IDS_CPU, CPUDlgProc);
	init_page (ppage, DISPLAY_ID, IDD_DISPLAY, IDI_DISPLAY, IDS_DISPLAY, DisplayDlgProc);
	init_page (ppage, CHIPSET_ID, IDD_CHIPSET, IDI_DISPLAY, IDS_CHIPSET, ChipsetDlgProc);
	init_page (ppage, SOUND_ID, IDD_SOUND, IDI_SOUND, IDS_SOUND, SoundDlgProc);
	init_page (ppage, FLOPPY_ID, IDD_FLOPPY, IDI_FLOPPY, IDS_FLOPPY, FloppyDlgProc);
	init_page (ppage, HARDDISK_ID, IDD_HARDDISK, IDI_HARDDISK, IDS_HARDDISK, HarddiskDlgProc);
	init_page (ppage, PORTS_ID, IDD_PORTS, IDI_PORTS, IDS_PORTS, PortsDlgProc);
	init_page (ppage, MISC_ID, IDD_MISC, IDI_MISC, IDS_MISC, MiscDlgProc);
	init_page (ppage, ABOUT_ID, IDD_ABOUT, IDI_ABOUT, IDS_ABOUT, AboutDlgProc);

	init_called = 1;
    }

    pHeader.dwSize = sizeof (PROPSHEETHEADER);
    pHeader.dwFlags = PSH_PROPSHEETPAGE | PSH_PROPTITLE | PSH_USEICONID | PSH_USECALLBACK | PSH_NOAPPLYNOW | PSH_HASHELP;
    if (! all_options && workprefs.gfx_afullscreen && workprefs.gfx_width < 640)
	pHeader.hwndParent = NULL;
    else
	pHeader.hwndParent = hAmigaWnd;
    pHeader.hInstance = hInst;
    pHeader.pszIcon = MAKEINTRESOURCE (IDI_APPICON);
    pHeader.pszCaption = "UAE/Win32";
    pHeader.nPages = C_PAGES;
    pHeader.nStartPage = 0;
    pHeader.ppsp = ppage;
    pHeader.pfnCallback = (PFNPROPSHEETCALLBACK) InitPropertySheet;


    psresult = PropertySheet (&pHeader);

    if (quit_program)
	psresult = -2;

    return psresult;
}

int gui_init (void)
{
    return( GetSettings(1) );
}

int gui_update (void)
{
    return 1;
}

void gui_exit (void)
{
#if 0
    free_prefs( &workprefs, 1 );
#endif
    closeprinter(); // Bernd Roesch
}

extern HWND hStatusWnd;
struct gui_info gui_data;

void check_prefs_changed_gui( void )
{
}

void gui_led (int led, int on)
{
    WORD type;
    static char drive_text[5*6];
    char *ptr;
    extern int screen_is_picasso;
    
    if( hStatusWnd )
    {
	if( on )
	    type = SBT_POPOUT;
	else
	    type = 0;
	
	ptr = drive_text + led*6;
	if( led > 0 )
	{
	    sprintf( ptr , "%02d", gui_data.drive_track[led-1]);
	}
	else
	{
	    strcpy( ptr, "Power");
	}
	PostMessage (hStatusWnd, SB_SETTEXT, (WPARAM) ((led+2) | type), (LPARAM) ptr);
    }
}

void gui_fps( int fps )
{
    static char fps_text[10];

    if( hStatusWnd )
    {
	sprintf( fps_text, "FPS: %2u", fps+1 );
	PostMessage( hStatusWnd, SB_SETTEXT, (WPARAM)( 1 ), (LPARAM)fps_text );
    }
}

void gui_filename (int num, const char *name)
{
}

void gui_message(const char *format,...)
{
    char msg[2048];
    char szTitle[ MAX_PATH ];
    va_list parms;
    int flipflop = 0;
    int fullscreen = 0;
    int reactivate_mouse = 0;
    HWND window = NULL;

    if( DirectDraw_GetCooperativeLevel( &window, &fullscreen ) && fullscreen )
	flipflop = 1;

    pause_sound(0);
    if( flipflop )
	ShowWindow( window, SW_MINIMIZE );

    va_start (parms, format);
    vsprintf( msg, format, parms );
    va_end (parms);
    write_log ( msg );

    WIN32GUI_LoadUIString( IDS_ERRORTITLE, szTitle, MAX_PATH );
    if( mouseactive )
    {
	setmouseactive( 0 );
	reactivate_mouse = 1;
    }

    MessageBox( NULL, msg, szTitle, MB_OK | MB_ICONWARNING | MB_TASKMODAL | MB_SETFOREGROUND );

    if( reactivate_mouse )
	setmouseactive( 1 );

    if( flipflop )
	ShowWindow( window, SW_RESTORE );

    resume_sound();
}

void gui_lock (void)
{
}

void gui_unlock (void)
{
}

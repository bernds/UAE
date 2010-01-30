/*==========================================================================
 *
 *  Copyright (C) 1996 Brian King
 *
 *  File:       wingui.c
 *  Content:    Win32-specific gui features for UAE port.
 *
-U 4 -c 4 -F 8 -n i3 -w 2 -m system3.1:d:\amiga\hd0 -m work:d:\amiga\hd1
 ***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <windows.h>
#include <windowsx.h>
#include <mmsystem.h>
#include <commctrl.h>
#include <commdlg.h>
#include <dlgs.h>
#include <winuser.h>
#include <ddraw.h>
#include <process.h>
#include <shlobj.h>
#include <prsht.h>
#include <richedit.h>

#include "config.h"
#include "resource.h"
#include "sysconfig.h"
#include "sysdeps.h"
#include "gui.h"
#include "options.h"
#include "include/memory.h"
#include "custom.h"
#include "readcpu.h"
#include "newcpu.h"
#include "disk.h"
#include "uae.h"
#include "autoconf.h"

#include "picasso96.h"
#include "osdep/win32gui.h"
#include "osdep/win32.h"
#include "sounddep/sound.h"

/* HACK HACK HACK */
extern int debug_logging;
/* ! HACK HACK HACK */

static int allow_quit;
static int full_property_sheet;
static struct uae_prefs *pguiprefs;
static struct uae_prefs workprefs;
static struct uaedev_mount_info *work_mountinfo;

extern int use_direct_sound;
extern BOOL running_winnt;
extern HINSTANCE hInst;
extern HWND hAmigaWnd;
extern uae_u8 julian_mode;

extern int mouseactive;
extern char *start_path;
extern int joystickspresent[MAXJOYSTICKS];
extern int parse_joy_spec (char *spec);

HANDLE win32uae_key = NULL;
drive_specs blankdrive =
{"", "", 1, 32, 1, 2, 0, 0};

#define Error(x) MessageBox( NULL, (x), "WinUAE Error", MB_OK )

/* Number of pages in the settings dialog */
#define C_PAGES 11
enum {
    LOADSAVE_ID, STARTUP_ID, SOUND_ID, FLOPPY_ID,
    ADISPLAY_ID, PDISPLAY_ID, CPU_ID,
    HARDDISK_ID, PORTS_ID, ADVANCED_ID, ABOUT_ID
};

#define MIN_CHIP_MEM 0
#define MAX_CHIP_MEM 4
#define MIN_FAST_MEM 0
#define MAX_FAST_MEM 4
#define MIN_SLOW_MEM 0
#define MAX_SLOW_MEM 2
#define MIN_Z3_MEM 0
#define MAX_Z3_MEM 7
#define MIN_P96_MEM 0
#define MAX_P96_MEM 4
#define MIN_M68K_PRIORITY 1
#define MAX_M68K_PRIORITY 16
#define MIN_REFRESH_RATE 1
#define MAX_REFRESH_RATE 10

#define MAX_LINES 580

static void accept_workprefs (void)
{
    struct uaedev_mount_info *oldmi = pguiprefs->mountinfo;
    *pguiprefs = workprefs;
    if (oldmi != workprefs.mountinfo) {
	free_mountinfo (oldmi);
	/* Ugh.  This isn't very nice.  */
	currprefs.mountinfo = workprefs.mountinfo;
    } else
	free_mountinfo (work_mountinfo);
    work_mountinfo = dup_mountinfo (workprefs.mountinfo);
}

static void CreateDiskFile (char *name)
{
    HANDLE adf;
    int i, file_size = 880 * 1024;
    char *chunk = NULL;
    DWORD count;

    SetCursor (LoadCursor (NULL, IDC_WAIT));
    adf = CreateFile (name, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
    if (adf != INVALID_HANDLE_VALUE) {
	if ((chunk = malloc (10240)) != NULL) {
	    for (i = 0; i < file_size; i += 10240) {
		WriteFile (adf, chunk, 10240, &count, NULL);
	    }
	}
	CloseHandle (adf);
    }
    SetCursor (LoadCursor (NULL, IDC_ARROW));
}

static void DiskSelection (HWND hDlg, WPARAM wParam, int flag)
{
    OPENFILENAME openFileName;
    char full_path[MAX_PATH] = "";
    char file_name[MAX_PATH] = "";
    char init_path[MAX_PATH] = "";
    BOOL result = FALSE;
    char *amiga_path = NULL;

    if ((amiga_path = getenv ("AmigaPath")) != NULL) {
	strncpy (init_path, amiga_path, MAX_PATH);
	if (flag == 2)
	    strncat (init_path, "\\hdf\\", MAX_PATH);
	else
	    strncat (init_path, "\\adf\\", MAX_PATH);
    } else if (start_path) {
	strncpy (init_path, start_path, MAX_PATH);
	if (flag == 2 || flag == 3)
	    strncat (init_path, "\\..\\shared\\hdf\\", MAX_PATH);
	else
	    strncat (init_path, "\\..\\shared\\adf\\", MAX_PATH);
    }
    openFileName.lStructSize = sizeof (OPENFILENAME);
    openFileName.hwndOwner = hDlg;
    openFileName.hInstance = hInst;
    switch (flag) {
     case 0:
	openFileName.lpstrTitle = "Select an Amiga Disk File image...";
	openFileName.lpstrDefExt = "ADF";
	openFileName.lpstrFilter = "Amiga Disk Files (*.ADF;*.ADZ;*.DMS)\0*.ADF;*.ADZ;*.DMS\0";
	break;
     case 1:
	openFileName.lpstrTitle = "Choose your blank Amiga Disk File...";
	openFileName.lpstrDefExt = "ADF";
	openFileName.lpstrFilter = "Amiga Disk Files (*.ADF)\0*.ADF\0";
	break;
     case 2:
     case 3:
	openFileName.lpstrTitle = "Select a Hard Disk File...";
	openFileName.lpstrDefExt = "HDF";
	openFileName.lpstrFilter = "Hard Disk Files (*.HDF)\0*.HDF\0";
	break;
    }
    openFileName.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_LONGNAMES | OFN_HIDEREADONLY | OFN_NOCHANGEDIR;
    openFileName.lpstrCustomFilter = NULL;
    openFileName.nMaxCustFilter = 0;
    openFileName.nFilterIndex = 0;
    openFileName.lpstrFile = full_path;
    openFileName.nMaxFile = MAX_PATH;
    openFileName.lpstrFileTitle = file_name;
    openFileName.nMaxFileTitle = MAX_PATH;
    if (start_path)
	openFileName.lpstrInitialDir = init_path;
    else
	openFileName.lpstrInitialDir = NULL;
    openFileName.lpfnHook = NULL;
    openFileName.lpTemplateName = NULL;
    openFileName.lCustData = 0;
    if (flag == 1 || flag == 3)
	result = GetSaveFileName (&openFileName);
    else
	result = GetOpenFileName (&openFileName);

    if (result == FALSE)
	write_log ("GetOpenFileName() failed.\n");
    else {
	switch (wParam) {
	 case IDC_PATH_NAME:
	    SetDlgItemText (hDlg, wParam, full_path);
	    break;
	 case IDC_DF0:
	    SetDlgItemText (hDlg, IDC_DF0TEXT, full_path);
	    break;
	 case IDC_DF1:
	    SetDlgItemText (hDlg, IDC_DF1TEXT, full_path);
	    break;
	 case IDC_DF2:
	    SetDlgItemText (hDlg, IDC_DF2TEXT, full_path);
	    break;
	 case IDC_DF3:
	    SetDlgItemText (hDlg, IDC_DF3TEXT, full_path);
	    break;
	 case IDC_CREATE:
	    CreateDiskFile (full_path);
	    break;
	}
    }
}

static long CreateHardFile (HWND hDlg, LONG hfsize)
{
    HANDLE hf;
    long result = 0;
    int i = 0, file_size = hfsize * 1024 * 1024;

    char init_path[MAX_PATH] = "";
    char *chunk = NULL;
    DWORD count;

    DiskSelection (hDlg, IDC_PATH_NAME, 3);
    GetDlgItemText (hDlg, IDC_PATH_NAME, init_path, MAX_PATH);
    if (*init_path) {
	SetCursor (LoadCursor (NULL, IDC_WAIT));
	hf = CreateFile (init_path, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
	if (hf != INVALID_HANDLE_VALUE) {
	    chunk = malloc (1024);
	    if (chunk != 0) {
		memset (chunk, 0, 1024);
		for (i; i < file_size; i += 1024) {
		    WriteFile (hf, chunk, 1024, &count, NULL);
		}
	    }
	    CloseHandle (hf);
	    result = i;
	}
	SetCursor (LoadCursor (NULL, IDC_ARROW));
    }
    return result;
}

static const char *memsize_names[] = {
    "None", "256 K", "512 K", "1 Meg", "2 Megs", "4 Megs", "8 Megs", "16 Megs", "32 Megs", "64 Megs"
};

static unsigned long memsizes[] = {
    0, 0x40000, 0x80000, 0x100000, 0x200000, 0x400000, 0x800000, 0x1000000, 0x2000000, 0x4000000
};

static int msi_chip[] = { 2, 3, 4, 5, 6, 7 };
static int msi_bogo[] = { 0, 2, 3 };
static int msi_fast[] = { 0, 3, 4, 5, 6 };
static int msi_z3fast[] = { 0, 3, 4, 5, 6, 7, 8, 9 };
static int msi_gfx[] = { 0, 3, 4, 5, 6 };

static HWND pages[C_PAGES];

static LONG CalculateHardfileSize (HWND hDlg)
{
    return 1 << SendDlgItemMessage (hDlg, IDC_HARDFILE_SLIDER, TBM_GETPOS, 0, 0);
}

static int joy0idc[] = {
    IDC_PORT0_JOY0, IDC_PORT0_JOY1, IDC_PORT0_MOUSE, IDC_PORT0_KBDA, IDC_PORT0_KBDB, IDC_PORT0_KBDC
};

static int joy1idc[] = {
    IDC_PORT1_JOY0, IDC_PORT1_JOY1, IDC_PORT1_MOUSE, IDC_PORT1_KBDA, IDC_PORT1_KBDB, IDC_PORT1_KBDC
};

static void UpdateRadioButtons (HWND hDlg, HWND lParam)
{
    int which_button;

    if (GetDlgItem (hDlg, IDC_PORT0) == lParam) {
	which_button = joy0idc[workprefs.fake_joystick & 0x00FF];
	if (CheckRadioButton (hDlg, IDC_PORT0_JOY0, IDC_PORT0_KBDC, which_button) == 0)
	    which_button = 0;
    } else if (GetDlgItem (hDlg, IDC_PORT1) == lParam) {
	which_button = joy1idc[(workprefs.fake_joystick & 0xFF00) >> 8];
	if (CheckRadioButton (hDlg, IDC_PORT1_JOY0, IDC_PORT1_KBDC, which_button) == 0)
	    which_button = 0;
    }
}

static const char *nth[] = {
    "", "second ", "third ", "fourth ", "fifth ", "sixth ", "seventh ", "eighth ", "ninth ", "tenth "
};

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

static BOOL CALLBACK LoadSaveDlgProc (HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
    char name_buf[MAX_PATH] = "", desc_buf[128] = "";
    HWND list;
    int dblclick = 0;
    NM_LISTVIEW *nmlistview;
    int items = 0, entry = 0;

    switch (msg) {
     case WM_INITDIALOG:
	pages[LOADSAVE_ID] = hDlg;
	if (! full_property_sheet) {
	    EnableWindow (GetDlgItem (hDlg, IDC_SAVE), FALSE);
	    EnableWindow (GetDlgItem (hDlg, IDC_IMPORT), FALSE);
	}
	return TRUE;

     case WM_USER:
	break;

     case WM_COMMAND:
	switch (wParam) {
	 case IDC_SAVE:
	    break;
	 case IDC_LOAD:
	    break;
	 case IDC_DELETE:
	    break;
	}
	break;

     case WM_NOTIFY:
	if (((LPNMHDR) lParam)->idFrom == IDC_CONFIGLIST) {
	    nmlistview = (NM_LISTVIEW *) lParam;
	    list = nmlistview->hdr.hwndFrom;

	    switch (nmlistview->hdr.code) {
	    case NM_DBLCLK:
		dblclick = 1;
		/* fall-through */
	    case NM_CLICK:
		entry = listview_entry_from_click (list);
		/* Copy the item's name and description to the gadgets at the bottom... */
		if (entry >= 0) {
		    ListView_GetItemText (list, entry, 0, name_buf, MAX_PATH);
		    ListView_GetItemText (list, entry, 1, desc_buf, 128);
		    SetDlgItemText (hDlg, IDC_EDITNAME, name_buf);
		    SetDlgItemText (hDlg, IDC_EDITDESCRIPTION, desc_buf);
		    ListView_RedrawItems (list, 0, items);

		    if (dblclick) {
			/* do the config-loading */

			/* start the emulation... */
		    }
		}
		break;
	    }
	} else {
	    switch (((NMHDR *) lParam)->code) {
	    case PSN_RESET:
		if (allow_quit) {
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

static int CALLBACK ContributorsProc (HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
    CHARFORMAT CharFormat;

    switch (msg) {
     case WM_COMMAND:
	if (wParam == ID_OK) {
	    EndDialog (hDlg, 1);
	    return TRUE;
	}
	break;
     case WM_INITDIALOG:
	CharFormat.cbSize = sizeof (CharFormat);

	SetDlgItemText (hDlg, IDC_CONTRIBUTORS,
			"Bernd Schmidt - The Grand-Master\n"
			"Mathias Ortmann - WinUAE Main Guy\n"
			"Brian King - Picasso96 Support, Integrated GUI for WinUAE, AHI (Working on it)\n"
			"Gustavo Goedert/Peter Remmers/Michael Sontheimer/Tomi Hakala/Tim Gunn/Nemo Pohle - DOS Port Stuff\n"
			"Samuel Devulder/Olaf Barthel - Amiga Port\n"
			"Krister Bergman - XFree86 and OS/2 Port\n"
			"A. Blanchard/Ernesto Corvi - MacOS Port\n"
			"Christian Bauer - BeOS Port\n"
			"Ian Stephenson - NextStep Port\n"
			"Peter Teichmann - Acorn/RiscOS Port\n"
			"Stefan Reinauer - ZorroII/III AutoConfig, Serial Support\n"
			"Christian Schmitt/Chris Hames - Serial Support\n"
			"Herman ten Brugge - 68020/68881 Emulation Code\n"
			"Tauno Taipaleenmaki - Various UAE-Control/UAE-Library Support\n"
			"Brett Eden/Tim Gunn/Paolo Besser/Nemo Pohle - Various Docs and Web-Sites\n"
			"Special thanks to Alexander Kneer and Tobias Abt (The Picasso96 Team)");

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
    DialogBox (hInst, MAKEINTRESOURCE (IDD_CONTRIBUTORS), hDlg, ContributorsProc);
}

static void show_url_from_click (HWND hDlg, LPARAM pos)
{
    static int ids[5] = { IDC_CLOANTOHOME, IDC_AMIGAHOME, IDC_PICASSOHOME, IDC_UAEHOME, IDC_WINUAEHOME };
    static const char *urls[5] = {
	"http://www.cloanto.com/amiga/forever",
	"http://www.amiga.de",
	"http://www.villagetronic.com/amiga/support/ftp96.html", 
	"http://www.freiburg.linux.de/~uae/",
	"http://www.CodePoet.com/UAE"
    };
    int i;
    POINT point;

    point.x = LOWORD (pos);
    point.y = HIWORD (pos);

    for (i = 0; i < 5; i++) {
	RECT rect;
    
	GetWindowRect (GetDlgItem (hDlg, ids[i]), &rect);
	ScreenToClient (hDlg, (POINT *) & rect);
	ScreenToClient (hDlg, (POINT *) & (rect.right));
	if (PtInRect (&rect, point)) {
	    ShellExecute (NULL, NULL, urls[i] , NULL, NULL, SW_SHOWNORMAL);
	    return;
	}
    }
}

static void SetupRichText (HWND hDlg, UINT id, char *text)
{
    CHARFORMAT CharFormat;
    CharFormat.cbSize = sizeof (CharFormat);

    SetDlgItemText (hDlg, id, text);
    SendDlgItemMessage (hDlg, id, EM_GETCHARFORMAT, 0, (LPARAM)&CharFormat);
    CharFormat.dwMask |= CFM_UNDERLINE | CFM_SIZE | CFM_FACE | CFM_COLOR;
    CharFormat.dwEffects = CFE_UNDERLINE;
    CharFormat.yHeight = 12 * 20;	/* height in twips, where a twip is 1/20th of a point - for a pt.size of 18 */

    CharFormat.crTextColor = GetSysColor (COLOR_ACTIVECAPTION);
    strcpy (CharFormat.szFaceName, "Times New Roman");
    SendDlgItemMessage (hDlg, id, EM_SETCHARFORMAT, SCF_ALL, (LPARAM)&CharFormat);
    SendDlgItemMessage (hDlg, id, EM_SETBKGNDCOLOR, 0, GetSysColor (COLOR_3DFACE));
}

static void init_aboutdlg (HWND hDlg)
{
    CHARFORMAT CharFormat;
    CharFormat.cbSize = sizeof (CharFormat);

    SetDlgItemText (hDlg, IDC_RICHEDIT1, "UAE for Win32/DirectX");
    SendDlgItemMessage (hDlg, IDC_RICHEDIT1, EM_GETCHARFORMAT, 0, (LPARAM) & CharFormat);
    CharFormat.dwMask |= CFM_BOLD | CFM_SIZE | CFM_FACE;
    CharFormat.dwEffects = CFE_BOLD;
    CharFormat.yHeight = 18 * 20;	/* height in twips, where a twip is 1/20th of a point - for a pt.size of 18 */

    strcpy (CharFormat.szFaceName, "Times New Roman");
    SendDlgItemMessage (hDlg, IDC_RICHEDIT1, EM_SETCHARFORMAT, SCF_ALL, (LPARAM) & CharFormat);
    SendDlgItemMessage (hDlg, IDC_RICHEDIT1, EM_SETBKGNDCOLOR, 0, GetSysColor (COLOR_3DFACE));

    SetDlgItemText (hDlg, IDC_RICHEDIT2, PROGNAME);
    SendDlgItemMessage (hDlg, IDC_RICHEDIT2, EM_GETCHARFORMAT, 0, (LPARAM) & CharFormat);
    CharFormat.dwMask |= CFM_SIZE | CFM_FACE;
    CharFormat.yHeight = 10 * 20;
    strcpy (CharFormat.szFaceName, "Times New Roman");
    SendDlgItemMessage (hDlg, IDC_RICHEDIT2, EM_SETCHARFORMAT, SCF_ALL, (LPARAM) & CharFormat);
    SendDlgItemMessage (hDlg, IDC_RICHEDIT2, EM_SETBKGNDCOLOR, 0, GetSysColor (COLOR_3DFACE));

    SetupRichText( hDlg, IDC_AMIGAHOME, "Amiga Inc." );
    SetupRichText( hDlg, IDC_CLOANTOHOME, "Cloanto's Amiga Forever" );
    SetupRichText( hDlg, IDC_WINUAEHOME, "WinUAE Home Page" );
    SetupRichText( hDlg, IDC_AIABHOME, "" ); // "Amiga In A Box"
    SetupRichText( hDlg, IDC_UAEHOME, "UAE Home Page" );
    SetupRichText( hDlg, IDC_PICASSOHOME, "Picasso96 Home Page" );
}

static BOOL CALLBACK AboutDlgProc (HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg) {
     case WM_INITDIALOG:
	pages[ABOUT_ID] = hDlg;
	init_aboutdlg (hDlg);
	break;

     case WM_COMMAND:
	if (wParam == IDC_CONTRIBUTORS) {
	    DisplayContributors (hDlg);
	}
	break;
     case WM_LBUTTONDOWN:
	show_url_from_click (hDlg, lParam);
	break;
     case WM_NOTIFY:
	switch (((NMHDR *) lParam)->code) {
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

static struct win32_displaymode *GetDisplayMode (int logical_number)
{
    struct win32_displaymode *dm = win32_displaymode_list;
    while (logical_number > 0 && dm != 0)
	dm = dm->next, logical_number--;
    return dm;
}

static void enable_for_adisplaydlg (HWND hDlg)
{
    if (! full_property_sheet) {
	/* Disable certain controls which are only to be set once at start-up... */
	EnableWindow (GetDlgItem (hDlg, IDC_LORES), FALSE);
	EnableWindow (GetDlgItem (hDlg, IDC_XCENTER), FALSE);
	EnableWindow (GetDlgItem (hDlg, IDC_YCENTER), FALSE);
	EnableWindow (GetDlgItem (hDlg, IDC_ASPECT), FALSE);
	EnableWindow (GetDlgItem (hDlg, IDC_LINEDBL), FALSE);
	EnableWindow (GetDlgItem (hDlg, IDC_RESOLUTION), FALSE);
	EnableWindow (GetDlgItem (hDlg, IDC_XSIZE), FALSE);
	EnableWindow (GetDlgItem (hDlg, IDC_YSIZE), FALSE);
	EnableWindow (GetDlgItem (hDlg, IDC_XCENTER_NONE), FALSE);
	EnableWindow (GetDlgItem (hDlg, IDC_XCENTER_SIMPLE), FALSE);
	EnableWindow (GetDlgItem (hDlg, IDC_XCENTER_SMART), FALSE);
	EnableWindow (GetDlgItem (hDlg, IDC_YCENTER_NONE), FALSE);
	EnableWindow (GetDlgItem (hDlg, IDC_YCENTER_SIMPLE), FALSE);
	EnableWindow (GetDlgItem (hDlg, IDC_YCENTER_SMART), FALSE);
    }
}

static void values_to_adisplaydlg (HWND hDlg)
{
    char buffer[40];

    sprintf (buffer, "%d", workprefs.gfx_width);
    SetDlgItemText (hDlg, IDC_XSIZE, buffer);
    sprintf (buffer, "%d", workprefs.gfx_height);
    SetDlgItemText (hDlg, IDC_YSIZE, buffer);

    SendDlgItemMessage (hDlg, IDC_FRAMERATE, TBM_SETPOS, TRUE, workprefs.framerate);
    sprintf (buffer, "Every %sFrame", nth[workprefs.framerate - 1]);
    SetDlgItemText (hDlg, IDC_RATETEXT, buffer);

    CheckDlgButton (hDlg, IDC_LINEDBL, workprefs.gfx_linedbl);
    CheckDlgButton (hDlg, IDC_AFULLSCREEN, workprefs.gfx_afullscreen);
    CheckDlgButton (hDlg, IDC_ASPECT, workprefs.gfx_correct_aspect);
    CheckDlgButton (hDlg, IDC_LORES, workprefs.gfx_lores);
    
    if (workprefs.gfx_ycenter == 0)
	CheckRadioButton (hDlg, IDC_YCENTER_NONE, IDC_YCENTER_SMART, IDC_YCENTER_NONE);
    else if (workprefs.gfx_ycenter == 1)
	CheckRadioButton (hDlg, IDC_YCENTER_NONE, IDC_YCENTER_SMART, IDC_YCENTER_SIMPLE);
    else
	CheckRadioButton (hDlg, IDC_YCENTER_NONE, IDC_YCENTER_SMART, IDC_YCENTER_SMART);

    if (workprefs.gfx_xcenter == 0)
	CheckRadioButton (hDlg, IDC_XCENTER_NONE, IDC_XCENTER_SMART, IDC_XCENTER_NONE);
    else if (workprefs.gfx_xcenter == 1)
	CheckRadioButton (hDlg, IDC_XCENTER_NONE, IDC_XCENTER_SMART, IDC_XCENTER_SIMPLE);
    else
	CheckRadioButton (hDlg, IDC_XCENTER_NONE, IDC_XCENTER_SMART, IDC_XCENTER_SMART);
}

static void init_resolution_combo (HWND hDlg)
{
    struct win32_displaymode *dm;

    SendDlgItemMessage (hDlg, IDC_RESOLUTION, CB_RESETCONTENT, 0, 0);
    for (dm = win32_displaymode_list; dm != 0; dm = dm->next) {
	char tmpstr[50];
	sprintf (tmpstr, "%dx%d", dm->width, dm->height);
	SendDlgItemMessage (hDlg, IDC_RESOLUTION, CB_ADDSTRING, 0, (LPARAM) (LPCTSTR) tmpstr);
	memset (tmpstr, 0, sizeof tmpstr);
    }
}

static void values_from_adisplaydlg (HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
    workprefs.gfx_afullscreen = IsDlgButtonChecked (hDlg, IDC_AFULLSCREEN);
    workprefs.gfx_lores = IsDlgButtonChecked (hDlg, IDC_LORES);
    workprefs.gfx_correct_aspect = IsDlgButtonChecked (hDlg, IDC_ASPECT);
    workprefs.gfx_linedbl = IsDlgButtonChecked (hDlg, IDC_LINEDBL);

    workprefs.framerate = SendDlgItemMessage (hDlg, IDC_FRAMERATE, TBM_GETPOS, 0, 0);

    {
	char buffer[40];
	sprintf (buffer, "Every %sFrame", nth[workprefs.framerate - 1]);
	SetDlgItemText (hDlg, IDC_RATETEXT, buffer);

 	GetDlgItemText (hDlg, IDC_XSIZE, buffer, sizeof buffer);
	workprefs.gfx_width = atoi (buffer);
	GetDlgItemText (hDlg, IDC_YSIZE, buffer, sizeof buffer);
	workprefs.gfx_height = atoi (buffer);
    }
    workprefs.gfx_xcenter = (IsDlgButtonChecked (hDlg, IDC_XCENTER_SMART) ? 2
			     : IsDlgButtonChecked (hDlg, IDC_XCENTER_SIMPLE) ? 1 : 0);
    workprefs.gfx_ycenter = (IsDlgButtonChecked (hDlg, IDC_YCENTER_SMART) ? 2
			     : IsDlgButtonChecked (hDlg, IDC_YCENTER_SIMPLE) ? 1 : 0);

    if (msg == WM_COMMAND && LOWORD (wParam) == IDC_RESOLUTION && HIWORD (wParam) == CBN_SELCHANGE) {
	LONG posn;
	struct win32_displaymode *dm;
	posn = SendDlgItemMessage (hDlg, IDC_RESOLUTION, CB_GETCURSEL, 0, 0);
	if (posn == CB_ERR)
	    return;
	dm = GetDisplayMode (posn);
	if (dm == 0) {
	    printf ("Bug...\n");
	    return;
	}
	workprefs.gfx_width = dm->width;
	workprefs.gfx_height = dm->height;
    }
}

static BOOL CALLBACK ADisplayDlgProc (HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
    static int recursive = 0;
    switch (msg) {
     case WM_INITDIALOG:
	pages[ADISPLAY_ID] = hDlg;
	SendDlgItemMessage (hDlg, IDC_FRAMERATE, TBM_SETPAGESIZE, 0, 1);
	SendDlgItemMessage (hDlg, IDC_FRAMERATE, TBM_SETRANGE, TRUE, MAKELONG (MIN_REFRESH_RATE, MAX_REFRESH_RATE));
	init_resolution_combo (hDlg);

     case WM_USER:
	recursive++;
	values_to_adisplaydlg (hDlg);
	recursive--;
	enable_for_adisplaydlg (hDlg);
	break;

     case WM_HSCROLL:
     case WM_COMMAND:
	if (recursive > 0)
	    break;
	recursive++;
	values_from_adisplaydlg (hDlg, msg, wParam, lParam);
	values_to_adisplaydlg (hDlg);
	recursive--;
	break;

     case WM_NOTIFY:
	switch (((NMHDR *) lParam)->code) {
	 case PSN_APPLY:
	    accept_workprefs ();
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

static void enable_for_pdisplaydlg (HWND hDlg)
{
    EnableWindow (GetDlgItem (hDlg, IDC_GFXCARDTEXT), ! workprefs.address_space_24);
    EnableWindow (GetDlgItem (hDlg, IDC_P96RAM), ! workprefs.address_space_24);
    EnableWindow (GetDlgItem (hDlg, IDC_P96MEM), ! workprefs.address_space_24);
    EnableWindow (GetDlgItem (hDlg, IDC_PFULLSCREEN), ! workprefs.address_space_24);
}

static void values_to_pdisplaydlg (HWND hDlg)
{
    int mem_size;

    CheckDlgButton (hDlg, IDC_PFULLSCREEN, workprefs.gfx_pfullscreen);

    switch (workprefs.gfxmem_size) {
     case 0x00000000: mem_size = 0; break;
     case 0x00100000: mem_size = 1; break;
     case 0x00200000: mem_size = 2; break;
     case 0x00400000: mem_size = 3; break;
     case 0x00800000: mem_size = 4; break;
    }
    SendDlgItemMessage (hDlg, IDC_P96MEM, TBM_SETPOS, TRUE, mem_size);
    SetDlgItemText (hDlg, IDC_P96RAM, memsize_names[msi_gfx[mem_size]]);
}

static BOOL CALLBACK PDisplayDlgProc (HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
    static int recursive = 0;
    switch (msg) {
     case WM_INITDIALOG:
	pages[PDISPLAY_ID] = hDlg;
	SendDlgItemMessage (hDlg, IDC_P96MEM, TBM_SETRANGE, TRUE, MAKELONG (MIN_P96_MEM, MAX_P96_MEM));

     case WM_USER:
	recursive++;
	values_to_pdisplaydlg (hDlg);
	enable_for_pdisplaydlg (hDlg);
	recursive--;
	break;

    case WM_HSCROLL:
    case WM_COMMAND:
	if (recursive > 0)
	    break;
	recursive++;
	workprefs.gfx_pfullscreen = IsDlgButtonChecked (hDlg, IDC_PFULLSCREEN);
	workprefs.gfxmem_size = memsizes[msi_gfx[SendMessage (GetDlgItem (hDlg, IDC_P96MEM), TBM_GETPOS, 0, 0)]];
	values_to_pdisplaydlg (hDlg);
	recursive--;
	break;

    case WM_NOTIFY:
	switch (((NMHDR *) lParam)->code) {
	 case PSN_APPLY:
	    accept_workprefs ();
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

static void KickStuffSelection (HWND hDlg, int which)
{
    OPENFILENAME openFileName;
    char full_path[MAX_PATH] = "";
    char file_name[MAX_PATH] = "";
    char init_path[MAX_PATH] = "";
    char *amiga_path = NULL;

    if ((amiga_path = getenv ("AmigaPath")) != NULL) {
	strncpy (init_path, amiga_path, MAX_PATH);
	strncat (init_path, "\\ROM\\", MAX_PATH);
    } else if (start_path) {
	strncpy (init_path, start_path, MAX_PATH);
	strncat (init_path, "\\..\\shared\\ROM\\", MAX_PATH);
    }
    openFileName.lStructSize = sizeof (OPENFILENAME);
    openFileName.hwndOwner = hDlg;
    openFileName.hInstance = hInst;
    if (which) {
	openFileName.lpstrFilter = "Amiga Kickstart Files (*.ROM)\0*.ROM\0\0";
	openFileName.lpstrTitle = "Select an Amiga ROM file...";
	openFileName.lpstrDefExt = "ROM";
    } else {
	openFileName.lpstrFilter = "Amiga Kickstart Key-Files (*.KEY)\0*.KEY\0\0";
	openFileName.lpstrTitle = "Select an Amiga Key-File...";
	openFileName.lpstrDefExt = "KEY";
    }
    openFileName.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_LONGNAMES | OFN_HIDEREADONLY | OFN_NOCHANGEDIR;
    openFileName.lpstrCustomFilter = NULL;
    openFileName.nMaxCustFilter = 0;
    openFileName.nFilterIndex = 0;
    openFileName.lpstrFile = full_path;
    openFileName.nMaxFile = MAX_PATH;
    openFileName.lpstrFileTitle = file_name;
    openFileName.nMaxFileTitle = MAX_PATH;
    if (start_path)
	openFileName.lpstrInitialDir = init_path;
    else
	openFileName.lpstrInitialDir = NULL;
    openFileName.lpfnHook = NULL;
    openFileName.lpTemplateName = NULL;
    openFileName.lCustData = 0;
    if (GetOpenFileName (&openFileName) == FALSE)
	write_log ("GetOpenFileName() failed.\n");
    else {
	if (which)
	    SetDlgItemText (hDlg, IDC_ROMFILE, full_path);
	else
	    SetDlgItemText (hDlg, IDC_KEYFILE, full_path);
    }
}

static void enable_for_startupdlg (HWND hDlg)
{
    EnableWindow (GetDlgItem (hDlg, IDC_Z3TEXT), ! workprefs.address_space_24);
    EnableWindow (GetDlgItem (hDlg, IDC_Z3FASTRAM), ! workprefs.address_space_24);
    EnableWindow (GetDlgItem (hDlg, IDC_Z3FASTMEM), ! workprefs.address_space_24);
    EnableWindow (GetDlgItem (hDlg, IDC_FASTMEM), workprefs.chipmem_size <= 0x200000);
    EnableWindow (GetDlgItem (hDlg, IDC_FASTRAM), workprefs.chipmem_size <= 0x200000);
    EnableWindow (GetDlgItem (hDlg, IDC_FASTTEXT), workprefs.chipmem_size <= 0x200000);
}

static void values_to_startupdlg (HWND hDlg)
{
    uae_u32 mem_size = 0;

    SetDlgItemText (hDlg, IDC_ROMFILE, romfile);
    SetDlgItemText (hDlg, IDC_KEYFILE, keyfile);

    CheckDlgButton (hDlg, IDC_ILLEGAL, workprefs.illegal_mem);

    switch (workprefs.chipmem_size) {
     case 0x00080000: mem_size = 0; break;
     case 0x00100000: mem_size = 1; break;
     case 0x00200000: mem_size = 2; break;
     case 0x00400000: mem_size = 3; break;
     case 0x00800000: mem_size = 4; break;
    }
    SendDlgItemMessage (hDlg, IDC_CHIPMEM, TBM_SETPOS, TRUE, mem_size);
    SetDlgItemText (hDlg, IDC_CHIPRAM, memsize_names[msi_chip[mem_size]]);

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

    switch (workprefs.bogomem_size) {
     case 0x00000000: mem_size = 0; break;
     case 0x00080000: mem_size = 1; break;
     case 0x00100000: mem_size = 2; break;
    }
    SendDlgItemMessage (hDlg, IDC_SLOWMEM, TBM_SETPOS, TRUE, mem_size);
    SetDlgItemText (hDlg, IDC_SLOWRAM, memsize_names[msi_bogo[mem_size]]);

    switch (workprefs.z3fastmem_size) {
     case 0x00000000: mem_size = 0; break;
     case 0x00100000: mem_size = 1; break;
     case 0x00200000: mem_size = 2; break;
     case 0x00400000: mem_size = 3; break;
     case 0x00800000: mem_size = 4; break;
     case 0x01000000: mem_size = 5; break;
     case 0x02000000: mem_size = 6; break;
     case 0x04000000: mem_size = 7; break;
    }
    SendDlgItemMessage (hDlg, IDC_Z3FASTMEM, TBM_SETPOS, TRUE, mem_size);
    SetDlgItemText (hDlg, IDC_Z3FASTRAM, memsize_names[msi_z3fast[mem_size]]);
}

static void fix_values_startupdlg (void)
{
    if (workprefs.chipmem_size > 0x200000)
	workprefs.fastmem_size = 0;
}

static BOOL CALLBACK StartupDlgProc (HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg) {
     case WM_INITDIALOG:
	pages[STARTUP_ID] = hDlg;
	SendDlgItemMessage (hDlg, IDC_CHIPMEM, TBM_SETRANGE, TRUE, MAKELONG (MIN_CHIP_MEM, MAX_CHIP_MEM));
	SendDlgItemMessage (hDlg, IDC_FASTMEM, TBM_SETRANGE, TRUE, MAKELONG (MIN_FAST_MEM, MAX_FAST_MEM));
	SendDlgItemMessage (hDlg, IDC_SLOWMEM, TBM_SETRANGE, TRUE, MAKELONG (MIN_SLOW_MEM, MAX_SLOW_MEM));
	SendDlgItemMessage (hDlg, IDC_Z3FASTMEM, TBM_SETRANGE, TRUE, MAKELONG (MIN_Z3_MEM, MAX_Z3_MEM));

     case WM_USER:
	fix_values_startupdlg ();
	values_to_startupdlg (hDlg);
	enable_for_startupdlg (hDlg);
	return TRUE;

    case WM_COMMAND:
	switch (wParam) {
	 case IDC_KICKCHOOSER:
	    KickStuffSelection (hDlg, 1);
	    break;
	 case IDC_KEYCHOOSER:
	    KickStuffSelection (hDlg, 0);
	    break;
	 default:
	    GetDlgItemText (hDlg, IDC_ROMFILE, romfile, CFG_ROM_LENGTH);
	    GetDlgItemText (hDlg, IDC_KEYFILE, keyfile, CFG_KEY_LENGTH);
	    workprefs.illegal_mem = IsDlgButtonChecked (hDlg, IDC_ILLEGAL);
	}
	break;

    case WM_HSCROLL:
	workprefs.chipmem_size = memsizes[msi_chip[SendMessage (GetDlgItem (hDlg, IDC_CHIPMEM), TBM_GETPOS, 0, 0)]];
	workprefs.bogomem_size = memsizes[msi_bogo[SendMessage (GetDlgItem (hDlg, IDC_SLOWMEM), TBM_GETPOS, 0, 0)]];
	workprefs.fastmem_size = memsizes[msi_fast[SendMessage (GetDlgItem (hDlg, IDC_FASTMEM), TBM_GETPOS, 0, 0)]];
	workprefs.z3fastmem_size = memsizes[msi_z3fast[SendMessage (GetDlgItem (hDlg, IDC_Z3FASTMEM), TBM_GETPOS, 0, 0)]];
	fix_values_startupdlg ();
	values_to_startupdlg (hDlg);
	enable_for_startupdlg (hDlg);	
	break;

    case WM_NOTIFY:
	switch (((NMHDR *) lParam)->code) {
	 case PSN_APPLY:
	    accept_workprefs ();
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

static int logging_ids[] = { IDC_CPU0, IDC_CPU1, IDC_CPU2, IDC_CPU3 };

static void enable_for_advanceddlg (HWND hDlg)
{
    if (! running_winnt) {
	EnableWindow (GetDlgItem (hDlg, IDC_LOGGING0), FALSE);
	EnableWindow (GetDlgItem (hDlg, IDC_LOGGING2), FALSE);
    }
}

static void values_to_advanceddlg (HWND hDlg)
{
    CheckDlgButton (hDlg, IDC_BLITIMM, workprefs.immediate_blits);
    CheckDlgButton (hDlg, IDC_BLIT32, workprefs.blits_32bit_enabled);
    CheckDlgButton (hDlg, IDC_JULIAN, julian_mode);
    CheckRadioButton (hDlg, IDC_LOGGING0, IDC_LOGGING3, logging_ids[debug_logging]);
}

static BOOL CALLBACK AdvancedDlgProc (HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg) {
     case WM_INITDIALOG:
	pages[ADVANCED_ID] = hDlg;

     case WM_USER:
	values_to_advanceddlg (hDlg);
	enable_for_advanceddlg (hDlg);
	return TRUE;

     case WM_COMMAND:
	debug_logging = (IsDlgButtonChecked (hDlg, IDC_LOGGING0) ? 0
			 : IsDlgButtonChecked (hDlg, IDC_LOGGING1) ? 1
			 : IsDlgButtonChecked (hDlg, IDC_LOGGING2) ? 2 : 3);
	workprefs.immediate_blits = IsDlgButtonChecked (hDlg, IDC_BLITIMM);
	workprefs.blits_32bit_enabled = IsDlgButtonChecked (hDlg, IDC_BLIT32);
	julian_mode = IsDlgButtonChecked (hDlg, IDC_JULIAN);
	break;

     case WM_NOTIFY:
	switch (((NMHDR *) lParam)->code) {
	 case PSN_APPLY:
	    accept_workprefs ();
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

static int cpu_ids[] = { IDC_CPU0, IDC_CPU1, IDC_CPU2, IDC_CPU3 };

static void enable_for_cpudlg (HWND hDlg)
{
    EnableWindow (GetDlgItem (hDlg, IDC_SPEED), workprefs.m68k_speed > 0);
    EnableWindow (GetDlgItem (hDlg, IDC_CS_CPU_TEXT), workprefs.m68k_speed > 0);
    EnableWindow (GetDlgItem (hDlg, IDC_CS_CHIPSET_TEXT), workprefs.m68k_speed > 0);
    EnableWindow (GetDlgItem (hDlg, IDC_COMPATIBLE), workprefs.cpu_level == 0);
    EnableWindow (GetDlgItem (hDlg, IDC_24BIT), workprefs.cpu_level > 0);
}

static void values_to_cpudlg (HWND hDlg)
{
    SendDlgItemMessage (hDlg, IDC_SPEED, TBM_SETPOS, TRUE, workprefs.m68k_speed <= 0 ? 1 : workprefs.m68k_speed);

    CheckDlgButton (hDlg, IDC_24BIT, workprefs.address_space_24);
    CheckDlgButton (hDlg, IDC_COMPATIBLE, workprefs.cpu_compatible);
    CheckRadioButton (hDlg, IDC_CPU0, IDC_CPU3, cpu_ids[workprefs.cpu_level]);

    if (workprefs.m68k_speed == -1)
	CheckRadioButton (hDlg, IDC_CS_HOST, IDC_CS_ADJUSTABLE, IDC_CS_HOST);
    else if (workprefs.m68k_speed == 0)
	CheckRadioButton (hDlg, IDC_CS_HOST, IDC_CS_ADJUSTABLE, IDC_CS_68000);
    else
	CheckRadioButton (hDlg, IDC_CS_HOST, IDC_CS_ADJUSTABLE, IDC_CS_ADJUSTABLE);
}

static void values_from_cpudlg (HWND hDlg)
{
    int newcpu;

    workprefs.cpu_compatible = IsDlgButtonChecked (hDlg, IDC_COMPATIBLE);
    workprefs.address_space_24 = IsDlgButtonChecked (hDlg, IDC_24BIT);
    workprefs.m68k_speed = (IsDlgButtonChecked (hDlg, IDC_CS_HOST) ? -1
			    : IsDlgButtonChecked (hDlg, IDC_CS_68000) ? 0
			    : SendMessage (GetDlgItem (hDlg, IDC_SPEED), TBM_GETPOS, 0, 0));

    newcpu = (IsDlgButtonChecked (hDlg, IDC_CPU0) ? 0
	      : IsDlgButtonChecked (hDlg, IDC_CPU1) ? 1
	      : IsDlgButtonChecked (hDlg, IDC_CPU2) ? 2 : 3);
    if (newcpu != workprefs.cpu_level) {
	/* When switching away from 68000, disable 24 bit addressing.  */
	if (workprefs.cpu_level == 0)
	    workprefs.address_space_24 = 0;
	if (newcpu > 0)
	    workprefs.cpu_compatible = 0;
	else
	    workprefs.address_space_24 = 1;
	workprefs.cpu_level = newcpu;
    }
    if (pages[STARTUP_ID])
	SendMessage (pages[STARTUP_ID], WM_USER, 0, 0);
    if (pages[PDISPLAY_ID])
	SendMessage (pages[PDISPLAY_ID], WM_USER, 0, 0);
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

     case WM_NOTIFY:
	switch (((NMHDR *) lParam)->code) {
	 case PSN_APPLY:
	    accept_workprefs ();
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

static void enable_for_sounddlg (HWND hDlg)
{
    EnableWindow (GetDlgItem (hDlg, IDC_FREQUENCY), workprefs.produce_sound > 0);
    EnableWindow (GetDlgItem (hDlg, IDC_11KHZ), workprefs.produce_sound > 0);
    EnableWindow (GetDlgItem (hDlg, IDC_22KHZ), workprefs.produce_sound > 0);
    EnableWindow (GetDlgItem (hDlg, IDC_44KHZ), workprefs.produce_sound > 0);
    EnableWindow (GetDlgItem (hDlg, IDC_48KHZ), workprefs.produce_sound > 0);

    EnableWindow (GetDlgItem (hDlg, IDC_SOUNDSIZE), workprefs.produce_sound > 0);
    EnableWindow (GetDlgItem (hDlg, IDC_8BIT), workprefs.produce_sound > 0);
    EnableWindow (GetDlgItem (hDlg, IDC_16BIT), workprefs.produce_sound > 0);

    EnableWindow (GetDlgItem (hDlg, IDC_STEREO), workprefs.produce_sound > 0);
    EnableWindow (GetDlgItem (hDlg, IDC_DIRECTSOUND), full_property_sheet);
}

static void values_to_sounddlg (HWND hDlg)
{
    int which_button;

    if (workprefs.sound_freq <= 11025)
	which_button = IDC_11KHZ;
    else if (workprefs.sound_freq <= 22050)
	which_button = IDC_22KHZ;
    else if (workprefs.sound_freq <= 44100)
	which_button = IDC_44KHZ;
    else
	which_button = IDC_48KHZ;
    CheckRadioButton (hDlg, IDC_11KHZ, IDC_48KHZ, which_button);

    CheckRadioButton (hDlg, IDC_8BIT, IDC_16BIT, workprefs.sound_bits == 16 ? IDC_16BIT : IDC_8BIT);

    switch (workprefs.produce_sound) {
     case 0: which_button = IDC_SOUND0; break;
     case 1: which_button = IDC_SOUND1; break;
     case 2: which_button = IDC_SOUND2; break;
     case 3: which_button = IDC_SOUND3; break;
    }
    
    CheckRadioButton (hDlg, IDC_SOUND0, IDC_SOUND3, which_button);

    CheckDlgButton (hDlg, IDC_STEREO, workprefs.stereo);
    CheckDlgButton (hDlg, IDC_DIRECTSOUND, use_direct_sound);
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
    workprefs.stereo = IsDlgButtonChecked (hDlg, IDC_STEREO) ? 1 : 0;
    use_direct_sound = IsDlgButtonChecked (hDlg, IDC_DIRECTSOUND) ? 1 : 0;
}

static BOOL CALLBACK SoundDlgProc (HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
    static int recursive = 0;
    switch (msg) {
     case WM_INITDIALOG:
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

     case WM_NOTIFY:
	switch (((NMHDR *) lParam)->code) {
	 case PSN_APPLY:
	    accept_workprefs ();
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
    char sectors[16];
    char reserved[16];
    char surfaces[16];
    char cylinders[16];
    int rw;
};

static struct hfdlg_vals empty_hfdlg = { "", "", "", "", "", "", 1 };
static struct hfdlg_vals current_hfdlg;

static int CALLBACK VolumeSettingsProc (HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
    static int recursive = 0;
    BROWSEINFO browse_info;
    char directory_path[MAX_PATH] = "";
    LPITEMIDLIST browse;
    LONG setting;

    browse_info.hwndOwner = hDlg;
    browse_info.pidlRoot = NULL;
    browse_info.pszDisplayName = directory_path;
    browse_info.lpszTitle = "Please select your file-system root directory...";
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
		if (strlen (current_fsvdlg.rootdir) == 0) {
		    MessageBox (hDlg, "You must select a path!", "Settings Error",
				MB_OK | MB_ICONERROR | MB_APPLMODAL | MB_SETFOREGROUND);
		    break;
		}
		if (strlen (current_fsvdlg.volume) == 0) {
		    MessageBox (hDlg, "You must select a name for the volume!", "Settings Error",
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
    LONG setting;

    switch (msg) {
     case WM_INITDIALOG:
	recursive++;
	SetDlgItemText (hDlg, IDC_PATH_NAME, current_hfdlg.filename);
	SetDlgItemText (hDlg, IDC_SECTORS, current_hfdlg.sectors);
	SetDlgItemText (hDlg, IDC_HEADS, current_hfdlg.surfaces);
	SetDlgItemText (hDlg, IDC_RESERVED, current_hfdlg.reserved);
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
		CreateHardFile (hDlg, setting);
		break;
	     case IDC_SELECTOR:
		DiskSelection (hDlg, IDC_PATH_NAME, 2);
		break;
	     case IDOK:
		if (strlen (current_hfdlg.filename) == 0) {
		    MessageBox (hDlg, "You must select a file...", "Settings Error",
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
	GetDlgItemText (hDlg, IDC_SECTORS, current_hfdlg.sectors, sizeof current_hfdlg.sectors);
	GetDlgItemText (hDlg, IDC_RESERVED, current_hfdlg.reserved, sizeof current_hfdlg.reserved);
	GetDlgItemText (hDlg, IDC_HEADS, current_hfdlg.surfaces, sizeof current_hfdlg.surfaces);
	current_hfdlg.rw = IsDlgButtonChecked (hDlg, IDC_RW);
	recursive--;

	break;
    }
    return FALSE;
}

static char *hd_columns[] = { "Volume", "Path", "R/W", "Sectors", "Surfaces", "Reserved" };

static void init_harddisk_list (HWND hDlg, HWND list)
{
    int i;
    
    for (i = 0; i < 6; i++) {
	LV_COLUMN lvcolumn;
	int width;
	lvcolumn.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
	lvcolumn.iSubItem = i;
	lvcolumn.fmt = LVCFMT_LEFT;
	lvcolumn.pszText = hd_columns[i];
	width = ListView_GetStringWidth (list, hd_columns[i]) + 15;
	lvcolumn.cx = width;
	ListView_InsertColumn (list, i, &lvcolumn);
	ListView_SetColumnWidth (list, i, width);
    }

    /* Turn on full-row-select option */
    ListView_SetExtendedListViewStyle (list, LVS_EX_FULLROWSELECT);
}

static void fill_harddisk_entry (HWND hDlg, HWND list, int i)
{
    char *volume, *rootdir;
    int size, cylinders, sectors, reserved, surfaces, readonly;
    int count;

    get_filesys_unit (work_mountinfo, i, &volume, &rootdir, &readonly,
		      &sectors, &surfaces, &reserved, &cylinders, &size);

    count = ListView_GetItemCount (list);
    while (i >= count) {
	LV_ITEM lvstruct;
	lvstruct.mask = LVIF_TEXT | LVIF_PARAM;
	lvstruct.pszText = "";
	lvstruct.lParam = 0;
	lvstruct.iItem = count;
	lvstruct.iSubItem = 0;
	ListView_InsertItem (list, &lvstruct);
	count++;
    }

    if (is_hardfile (work_mountinfo, i)) {
	char buffer[20];
	ListView_SetItemText (list, i, 0, volume);
	ListView_SetItemText (list, i, 1, rootdir);
	ListView_SetItemText (list, i, 2, readonly ? "yes" : "no");
	sprintf (buffer, "%d", sectors);
	ListView_SetItemText (list, i, 3, buffer);
	sprintf (buffer, "%d", surfaces);
	ListView_SetItemText (list, i, 4, buffer);
	sprintf (buffer, "%d", reserved);
	ListView_SetItemText (list, i, 5, buffer);
    } else {
	ListView_SetItemText (list, i, 0, volume);
	ListView_SetItemText (list, i, 1, rootdir);
	ListView_SetItemText (list, i, 2, "n/a");
	ListView_SetItemText (list, i, 3, "n/a");
	ListView_SetItemText (list, i, 4, "n/a");
	ListView_SetItemText (list, i, 5, "n/a");
    }

    if (volume)
	free (volume);
    if (rootdir)
	free (rootdir);
}

static void values_to_harddiskdlg (HWND hDlg)
{
    HWND list = GetDlgItem (hDlg, IDC_VOLUMELIST);
    int nr = nr_units (work_mountinfo);
    int i;
    
    ListView_DeleteAllItems (list);
    for (i = 0; i < nr; i++) {
	fill_harddisk_entry (hDlg, list, i);
    }
}

static void new_filesys (HWND hDlg)
{
    const char *result;

    result = add_filesys_unit (work_mountinfo, current_fsvdlg.volume,
			       current_fsvdlg.rootdir, ! current_fsvdlg.rw, 0, 0, 0);
    if (result)
	MessageBox (hDlg, result, "Bad directory",
		    MB_OK | MB_ICONERROR | MB_APPLMODAL | MB_SETFOREGROUND);
    else
	workprefs.mountinfo = work_mountinfo;
}

static void new_hardfile (HWND hDlg)
{
    int sectors, reserved, surfaces;
    const char *result;
    sectors = atoi (current_hfdlg.sectors);
    surfaces = atoi (current_hfdlg.surfaces);
    reserved = atoi (current_hfdlg.reserved);

    result = add_filesys_unit (work_mountinfo, 0, current_hfdlg.filename,
			       ! current_hfdlg.rw, sectors, surfaces, reserved);
    if (result)
	MessageBox (hDlg, result, "Bad hardfile",
		    MB_OK | MB_ICONERROR | MB_APPLMODAL | MB_SETFOREGROUND);
    else
	workprefs.mountinfo = work_mountinfo;
}

static void harddisk_remove (HWND hDlg)
{
    int entry = listview_find_selected (GetDlgItem (hDlg, IDC_VOLUMELIST));
    if (entry < 0)
	return;
    kill_filesys_unit (work_mountinfo, entry);
    workprefs.mountinfo = work_mountinfo;
}

static void harddisk_move (HWND hDlg, int up)
{
    int entry = listview_find_selected (GetDlgItem (hDlg, IDC_VOLUMELIST));
    if (entry < 0)
	return;
    if (move_filesys_unit (work_mountinfo, entry, up ? entry - 1 : entry + 1) >= 0)
	workprefs.mountinfo = work_mountinfo;
}

static void harddisk_edit (HWND hDlg)
{
    char *volume, *rootdir;
    int size, cylinders, sectors, reserved, surfaces, readonly;

    int entry = listview_find_selected (GetDlgItem (hDlg, IDC_VOLUMELIST));
    if (entry < 0)
	return;

    get_filesys_unit (work_mountinfo, entry, &volume, &rootdir, &readonly,
		      &sectors, &surfaces, &reserved, &cylinders, &size);

    if (volume == 0) {
	/* Hardfile */
	strncpy (current_hfdlg.filename, rootdir, (sizeof current_hfdlg.filename) - 1);
	current_hfdlg.filename[(sizeof current_hfdlg.filename) - 1] = '\0';
	sprintf (current_hfdlg.sectors, "%d", sectors);
	sprintf (current_hfdlg.reserved, "%d", reserved);
	sprintf (current_hfdlg.surfaces, "%d", surfaces);
	sprintf (current_hfdlg.cylinders, "%d", cylinders);
	current_hfdlg.rw = readonly;
	if (DialogBox (hInst, MAKEINTRESOURCE (IDD_HARDFILE), hDlg, HardfileSettingsProc)) {
	    const char *result;
	    sectors = atoi (current_hfdlg.sectors);
	    surfaces = atoi (current_hfdlg.surfaces);
	    reserved = atoi (current_hfdlg.reserved);
	    cylinders = atoi (current_hfdlg.cylinders);
	    result = set_filesys_unit (work_mountinfo, entry, 0, current_hfdlg.filename,
				       ! current_hfdlg.rw, sectors, surfaces, reserved);
	    if (result)
		MessageBox (hDlg, result, "Bad hardfile",
			    MB_OK | MB_ICONERROR | MB_APPLMODAL | MB_SETFOREGROUND);
	    else
		workprefs.mountinfo = work_mountinfo;
	}
    } else {
	/* Filesystem */
	strncpy (current_fsvdlg.rootdir, rootdir, (sizeof current_fsvdlg.rootdir) - 1);
	current_fsvdlg.rootdir[(sizeof current_fsvdlg.rootdir) - 1] = '\0';
	strncpy (current_fsvdlg.volume, volume, (sizeof current_fsvdlg.volume) - 1);
	current_fsvdlg.volume[(sizeof current_fsvdlg.volume) - 1] = '\0';
	current_fsvdlg.rw = readonly;
	if (DialogBox (hInst, MAKEINTRESOURCE (IDD_FILESYS), hDlg, VolumeSettingsProc)) {
	    const char *result;
	    result = set_filesys_unit (work_mountinfo, entry, current_fsvdlg.volume,
				       current_fsvdlg.rootdir, ! current_fsvdlg.rw, 0, 0, 0);
	    if (result)
		MessageBox (hDlg, result, "Bad hardfile",
			    MB_OK | MB_ICONERROR | MB_APPLMODAL | MB_SETFOREGROUND);
	    else
		workprefs.mountinfo = work_mountinfo;
	}
    }
    free (volume);
    free (rootdir);

}

static void harddiskdlg_button (HWND hDlg, int button)
{
    int i;
    int clicked_entry;
    switch (button) {
     case IDC_NEW_FS:
	current_fsvdlg = empty_fsvdlg;
	if (DialogBox (hInst, MAKEINTRESOURCE (IDD_FILESYS), hDlg, VolumeSettingsProc))
	    new_filesys (hDlg);
	break;

     case IDC_NEW_HF:
	current_hfdlg = empty_hfdlg;
	if (DialogBox (hInst, MAKEINTRESOURCE (IDD_HARDFILE), hDlg, HardfileSettingsProc))
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
	break;

     case IDC_DOWN:
	harddisk_move (hDlg, 0);
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
	if (entry >= 0 && dblclick) {
	    harddisk_edit (hDlg);
	    values_to_harddiskdlg (hDlg);
	}
	break;
    }
}

static BOOL CALLBACK HarddiskDlgProc (HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
    int i;
    drive_specs tempdrive;

    switch (msg) {
     case WM_INITDIALOG:
	pages[HARDDISK_ID] = hDlg;

	init_harddisk_list (hDlg, GetDlgItem (hDlg, IDC_VOLUMELIST));

     case WM_USER:
	values_to_harddiskdlg (hDlg);
	break;

     case WM_COMMAND:
	if (HIWORD (wParam) == BN_CLICKED)
	    harddiskdlg_button (hDlg, LOWORD (wParam));
	values_to_harddiskdlg (hDlg);
	break;

     case WM_NOTIFY:
	if (((LPNMHDR) lParam)->idFrom == IDC_VOLUMELIST)
	    harddiskdlg_volume_notify (hDlg, (NM_LISTVIEW *) lParam);
	else {
	    switch (((NMHDR *) lParam)->code) {
	     case PSN_APPLY:
		accept_workprefs ();
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

static BOOL CALLBACK FloppyDlgProc (HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
    static int recursive = 0;

    switch (msg) {
     case WM_INITDIALOG:
	pages[FLOPPY_ID] = hDlg;

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
	switch (wParam) {
	 case IDC_DF0:
	    DiskSelection (hDlg, wParam, 0);
	    break;
	 case IDC_DF1:
	    DiskSelection (hDlg, wParam, 0);
	    break;
	 case IDC_DF2:
	    DiskSelection (hDlg, wParam, 0);
	    break;
	 case IDC_DF3:
	    DiskSelection (hDlg, wParam, 0);
	    break;
	 case IDC_EJECT0:
	    SetDlgItemText (hDlg, IDC_DF0TEXT, "");
	    break;
	 case IDC_EJECT1:
	    SetDlgItemText (hDlg, IDC_DF1TEXT, "");
	    break;
	 case IDC_EJECT2:
	    SetDlgItemText (hDlg, IDC_DF2TEXT, "");
	    break;
	 case IDC_EJECT3:
	    SetDlgItemText (hDlg, IDC_DF3TEXT, "");
	    break;
	 case IDC_CREATE:
	    DiskSelection (hDlg, wParam, 1);
	    break;
	}
	GetDlgItemText (hDlg, IDC_DF0TEXT, workprefs.df[0], 255);
	GetDlgItemText (hDlg, IDC_DF1TEXT, workprefs.df[1], 255);
	GetDlgItemText (hDlg, IDC_DF2TEXT, workprefs.df[2], 255);
	GetDlgItemText (hDlg, IDC_DF3TEXT, workprefs.df[3], 255);
	recursive--;
	break;

     case WM_NOTIFY:
	switch (((NMHDR *) lParam)->code) {
	 case PSN_APPLY:
	    accept_workprefs ();
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

static void values_from_portsdlg (HWND hDlg)
{
    int item;
    char joyspec[3] = "MA";
    if (IsDlgButtonChecked (hDlg, IDC_PORT0_JOY0)) {
	if (joystickspresent[0])
	    joyspec[1] = '0';
	else
	    joyspec[1] = 'A';
    }
    if (IsDlgButtonChecked (hDlg, IDC_PORT0_JOY1)) {
	if (joystickspresent[1])
	    joyspec[1] = '1';
	else
	    joyspec[1] = 'B';
    }
    if (IsDlgButtonChecked (hDlg, IDC_PORT0_MOUSE))
	joyspec[0] = 'M';
    if (IsDlgButtonChecked (hDlg, IDC_PORT0_KBDA))
	joyspec[0] = 'A';
    if (IsDlgButtonChecked (hDlg, IDC_PORT0_KBDB))
	joyspec[0] = 'B';
    if (IsDlgButtonChecked (hDlg, IDC_PORT0_KBDC))
	joyspec[0] = 'C';

    if (IsDlgButtonChecked (hDlg, IDC_PORT1_JOY0)) {
	if (joystickspresent[0])
	    joyspec[1] = '0';
	else
	    joyspec[1] = 'A';
    }
    if (IsDlgButtonChecked (hDlg, IDC_PORT1_JOY1)) {
	if (joystickspresent[1])
	    joyspec[1] = '1';
	else
	    joyspec[1] = 'B';
    }
    if (IsDlgButtonChecked (hDlg, IDC_PORT1_MOUSE))
	joyspec[1] = 'M';
    if (IsDlgButtonChecked (hDlg, IDC_PORT1_KBDA))
	joyspec[1] = 'A';
    if (IsDlgButtonChecked (hDlg, IDC_PORT1_KBDB))
	joyspec[1] = 'B';
    if (IsDlgButtonChecked (hDlg, IDC_PORT1_KBDC))
	joyspec[1] = 'C';

    workprefs.fake_joystick = parse_joy_spec (joyspec);

    GetDlgItemText (hDlg, IDC_PARALLEL, prtname, CFG_PAR_LENGTH);

    item = SendDlgItemMessage (hDlg, IDC_SERIAL, CB_GETCURSEL, 0, 0L);
    switch (item) {
     case 1:
     case 2:
     case 3:
     case 4:
	workprefs.use_serial = 1;
#ifdef __GNUC__
	sprintf (sername, "COM%d", item);
#else
	_snprintf (sername, CFG_SER_LENGTH, "COM%d", item);
#endif
	break;
     default:
	workprefs.use_serial = 0;
	break;
    }
}

static void values_to_portsdlg (HWND hDlg)
{
    LONG item_height;
    RECT rect;

    UpdateRadioButtons (hDlg, GetDlgItem (hDlg, IDC_PORT0));
    UpdateRadioButtons (hDlg, GetDlgItem (hDlg, IDC_PORT1));
    SetDlgItemText (hDlg, IDC_PARALLEL, prtname);
    SendDlgItemMessage (hDlg, IDC_SERIAL, CB_RESETCONTENT, 0, 0L);
    SendDlgItemMessage (hDlg, IDC_SERIAL, CB_ADDSTRING, 0, (LPARAM) "None");
    SendDlgItemMessage (hDlg, IDC_SERIAL, CB_ADDSTRING, 0, (LPARAM) "COM1");
    SendDlgItemMessage (hDlg, IDC_SERIAL, CB_ADDSTRING, 0, (LPARAM) "COM2");
    SendDlgItemMessage (hDlg, IDC_SERIAL, CB_ADDSTRING, 0, (LPARAM) "COM3");
    SendDlgItemMessage (hDlg, IDC_SERIAL, CB_ADDSTRING, 0, (LPARAM) "COM4");
    if (workprefs.use_serial == 0)
	SendDlgItemMessage (hDlg, IDC_SERIAL, CB_SETCURSEL, 0, 0L);
    else {
	switch (sername[strlen (sername) - 1]) {
	 case '2':
	    SendDlgItemMessage (hDlg, IDC_SERIAL, CB_SETCURSEL, 2, 0L);
	    break;
	 case '3':
	    SendDlgItemMessage (hDlg, IDC_SERIAL, CB_SETCURSEL, 3, 0L);
	    break;
	 case '4':
	    SendDlgItemMessage (hDlg, IDC_SERIAL, CB_SETCURSEL, 4, 0L);
	    break;
	 default:
	    SendDlgItemMessage (hDlg, IDC_SERIAL, CB_SETCURSEL, 1, 0L);
	    break;
	}
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

/* Handle messages for the Joystick Settings page of our property-sheet */
static BOOL CALLBACK PortsDlgProc (HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
    static int recursive = 0;

    switch (msg) {
     case WM_INITDIALOG:
	pages[PORTS_ID] = hDlg;
	if (! full_property_sheet) {
	    EnableWindow (GetDlgItem (hDlg, IDC_SERIAL), FALSE);
	    EnableWindow (GetDlgItem (hDlg, IDC_PARALLEL), FALSE);
	}

     case WM_USER:
	recursive++;
	values_to_portsdlg (hDlg);
	recursive--;
	return TRUE;

     case WM_COMMAND:
	if (recursive > 0)
	    break;
	recursive++;
	values_from_portsdlg (hDlg);
	recursive--;
	break;

     case WM_NOTIFY:
	switch (((NMHDR *) lParam)->code) {
	 case PSN_APPLY:
	    accept_workprefs ();
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

    switch (msg) {
     case PSCB_INITIALIZED:
#if 0
	if (! full_property_sheet) {
	    PropSheet_RemovePage (hDlg, 0, NULL);
	    PropSheet_RemovePage (hDlg, 0, NULL);
	    PropSheet_RemovePage (hDlg, 0, NULL);
	    PropSheet_RemovePage (hDlg, 2, NULL);
	}
#endif

	for (i = 0; i < C_PAGES; i++)
	    pages[i] = NULL;

	break;
    }
}


#ifdef __GNUC__
#define DUN1 u1 ## .
#define DUN2 u2 ## .
#define DUN3 u3 ## .
#else
#define DUN1
#define DUN2
#define DUN3
#endif

static void init_page (PROPSHEETPAGE *ppage, int id, int tmpl, int icon, int title,
		       BOOL (* CALLBACK func) (HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam))
{
    ppage[id].dwSize = sizeof (PROPSHEETPAGE);
    ppage[id].dwFlags = PSP_USETITLE | PSP_USEICONID;
    ppage[id].hInstance = hInst;
    ppage[id].DUN1 pszTemplate = MAKEINTRESOURCE (tmpl);
    ppage[id].DUN2 pszIcon = MAKEINTRESOURCE (icon);
    ppage[id].pszTitle = MAKEINTRESOURCE (title);
    ppage[id].pfnDlgProc = (LPFNPSPCALLBACK) func;
    ppage[id].lParam = 0;
    ppage[id].pfnCallback = NULL;
    ppage[id].pcRefParent = NULL;
}

int GetSettings (int all_options)
{
    int psresult;
    PROPSHEETPAGE ppage[C_PAGES];
    PROPSHEETHEADER pHeader;

    full_property_sheet = all_options;
    allow_quit = ! all_options;
    pguiprefs = all_options ? &currprefs : &changed_prefs;
    workprefs = *pguiprefs;
    printf ("A\n");
    work_mountinfo = dup_mountinfo (pguiprefs->mountinfo);
    printf ("B\n");

    init_page (ppage, LOADSAVE_ID, IDD_LOADSAVE, IDI_LOADSAVE, IDS_LOADSAVE, LoadSaveDlgProc);
    init_page (ppage, STARTUP_ID, IDD_STARTUP, IDI_STARTUP, IDS_STARTUP, StartupDlgProc);
    init_page (ppage, SOUND_ID, IDD_SOUND, IDI_SOUND, IDS_SOUND, SoundDlgProc);
    init_page (ppage, ADISPLAY_ID, IDD_ADISPLAY, IDI_ADISPLAY, IDS_ADISPLAY, ADisplayDlgProc);
    init_page (ppage, PDISPLAY_ID, IDD_PDISPLAY, IDI_PDISPLAY, IDS_PDISPLAY, PDisplayDlgProc);
    init_page (ppage, CPU_ID, IDD_CPU, IDI_CPU, IDS_CPU, CPUDlgProc);
    init_page (ppage, HARDDISK_ID, IDD_HARDDISK, IDI_HARDDISK, IDS_HARDDISK, HarddiskDlgProc);
    init_page (ppage, FLOPPY_ID, IDD_FLOPPY, IDI_FLOPPY, IDS_FLOPPY, FloppyDlgProc);
    init_page (ppage, PORTS_ID, IDD_PORTS, IDI_PORTS, IDS_PORTS, PortsDlgProc);
    init_page (ppage, ADVANCED_ID, IDD_ADVANCED, IDI_ADVANCED, IDS_ADVANCED, AdvancedDlgProc);
    init_page (ppage, ABOUT_ID, IDD_ABOUT, IDI_ABOUT, IDS_ABOUT, AboutDlgProc);

    pHeader.dwSize = sizeof (PROPSHEETHEADER);
    pHeader.dwFlags = PSH_PROPSHEETPAGE | PSH_PROPTITLE | PSH_USEICONID | PSH_USECALLBACK | PSH_NOAPPLYNOW;
    if (! all_options && workprefs.gfx_afullscreen && workprefs.gfx_width < 640)
	pHeader.hwndParent = NULL;
    else
	pHeader.hwndParent = hAmigaWnd;
    pHeader.hInstance = hInst;
    pHeader.DUN1 pszIcon = MAKEINTRESOURCE (IDI_UAEICON);
    pHeader.pszCaption = "WinUAE";
    pHeader.nPages = C_PAGES;
    pHeader.DUN2 nStartPage = 0;
    pHeader.DUN3 ppsp = ppage;
    pHeader.pfnCallback = (PFNPROPSHEETCALLBACK) InitPropertySheet;

    psresult = PropertySheet (&pHeader);

    free_mountinfo (work_mountinfo);

    if (psresult == -1 || quit_program)
	return 0;

    return 1;
}

int gui_init (void)
{
    GetSettings (1);
    return 1;
}

int gui_update (void)
{
    return 1;
}

void gui_exit (void)
{
}

char *labels[5] =
{"Power", "0", "1", "2", "3"};
extern HWND hStatusWnd;
int led_states[5];

void gui_led (int led, int on)
{
    WORD type;

    return;

    if (hStatusWnd == 0)
	return;

    if (led_states[led] == on)
	return;

    led_states[led] = on;

    if (on)
	type = SBT_POPOUT;
    else
	type = 0;

    PostMessage (hStatusWnd, SB_SETTEXT, (WPARAM) ((led + 1) | type), (LPARAM) labels[led]);
}

void gui_filename (int num, const char *name)
{
}

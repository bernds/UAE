 /* 
  * UAE - The Un*x Amiga Emulator
  * 
  * Interface to the Tcl/Tk GUI
  * 
  * Copyright 1996 Bernd Schmidt
  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef __unix
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#endif
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <SIOUX.h>

#include "config.h"
#include "amiga.h"
#include "options.h"
#include "memory.h"
#include "custom.h"
#include "newcpu.h"
#include "disk.h"
#include "gui.h"

#define kAppleMenuID	128
#define kFileMenuID		129
#define	kEditMenuID		130
#define	kDrivesMenuID	131
#define	kMemoryMenuID	132
#define	kResMenuID		135
#define	kRateMenuID		134
#define	kVideoMenuID	133

MenuHandle	resMenu,rateMenu;
static int macLedState[5];
extern int use_quickdraw;
static Rect	powerRect, driveDf0Rect, driveDf1Rect, driveDf2Rect, driveDf3Rect;

static void sigchldhandler(int foo)
{
}

static void InitToolbox(void)
{
	InitGraf (&qd.thePort);
	InitFonts ();
	FlushEvents (everyEvent,0);
	InitWindows ();
	InitMenus ();
	TEInit ();
	InitDialogs (nil);
	InitCursor ();
}

#define kPrefStrRsrc 130

int gui_init(void)
{	char pathString[1024];
	Handle menuBar;
	
	InitToolbox();

	SIOUXSettings.initializeTB=false;
	SIOUXSettings.standalone=false;
	SIOUXSettings.setupmenus=false;
	SIOUXSettings.autocloseonquit=true;
	SIOUXSettings.asktosaveonclose=false;
	SIOUXSettings.showstatusline=false;
	SIOUXSettings.rows=11;
	SIOUXSettings.toppixel=350;
	SIOUXSettings.leftpixel=0;
	fprintf(stderr,"Starting up the GUIÉ\n");
	SIOUXSetTitle("\pInfo Window");
	
	if (ReadPrefs(pathString))
	{	if (*pathString != 0) add_filesys_unit("HD0", (char *)pathString, false);
	}
	else
	{	if (*pathString == 0)
		{	CreatePrefs();
			WritePrefs(0);
		}
	}
	
	menuBar=GetNewMBar(128);
	SetMenuBar(menuBar);
	DisposeHandle(menuBar);
	
	AddResMenu(GetMenuHandle(kAppleMenuID), 'DRVR');

	if (screen_res != 4) DisableItem (GetMenuHandle(kFileMenuID), 3);
	if (use_quickdraw) CheckItem(GetMenuHandle(kVideoMenuID),5,true);
    if (use_gfxlib) CheckItem(GetMenuHandle(kVideoMenuID),7,true);
    if (use_slow_mem) SetMenuItemText(GetMenuHandle(kMemoryMenuID), 1, "\pDisable 1 Mb (SlowMem)" );
    if (!automount_uaedev) CheckItem(GetMenuHandle(kDrivesMenuID),7,true);
    if (produce_sound) SetMenuItemText(GetMenuHandle(kFileMenuID), 9, "\pTurn Sound Off");
	else SetMenuItemText(GetMenuHandle(kFileMenuID), 9, "\pTurn Sound On");
    if (fake_joystick) SetMenuItemText(GetMenuHandle(kFileMenuID), 11, "\pTurn Joystick Off");
	else SetMenuItemText(GetMenuHandle(kFileMenuID), 11, "\pTurn Joystick On");
    
    rateMenu=GetMenu(kRateMenuID);
	InsertMenu(rateMenu, -1);
	if (framerate == 1) CheckItem(rateMenu,1,true);
	if (framerate == 3) CheckItem(rateMenu,2,true);
	if (framerate == 5) CheckItem(rateMenu,3,true);
	if (framerate == 7) CheckItem(rateMenu,4,true);
    
    resMenu=GetMenu(kResMenuID);
	InsertMenu(resMenu, -1);
	CheckItem(resMenu,(unsigned char)screen_res+1,true);
    
    DrawMenuBar();
    
    macLedState[0]=false;
    macLedState[1]=false;
    macLedState[2]=false;
    macLedState[3]=false;
    macLedState[4]=false;
    
    quit_program = 0;
}

int gui_update(void)
{
  return 0;
}

void gui_exit(void)
{
	DeleteMenu(kAppleMenuID);
	DeleteMenu(kFileMenuID);
	DeleteMenu(kEditMenuID);
	DeleteMenu(kDrivesMenuID);
	DeleteMenu(kMemoryMenuID);
	DeleteMenu(kResMenuID);
	DeleteMenu(kRateMenuID);
	DeleteMenu(kVideoMenuID);
}

void gui_prepare_leds(long screenV)
{	
	if (screen_res >= 3)
	{	SetRect(&powerRect,70,(short)screenV+1,110,(short)screenV+11);
		SetRect(&driveDf0Rect,220,(short)screenV+1,260,(short)screenV+11);
		SetRect(&driveDf1Rect,292,(short)screenV+1,332,(short)screenV+11);
		SetRect(&driveDf2Rect,365,(short)screenV+1,405,(short)screenV+11);
		SetRect(&driveDf3Rect,436,(short)screenV+1,476,(short)screenV+11);
	}
	else
	{	SetRect(&powerRect,46,(short)screenV+1,76,(short)screenV+11);
		SetRect(&driveDf0Rect,106,(short)screenV+1,136,(short)screenV+11);
		SetRect(&driveDf1Rect,166,(short)screenV+1,196,(short)screenV+11);
		SetRect(&driveDf2Rect,226,(short)screenV+1,256,(short)screenV+11);
		SetRect(&driveDf3Rect,286,(short)screenV+1,316,(short)screenV+11);
	}
}

void gui_update_leds(void)
{	int i;

	for(i=0;i<=4;i++) gui_led(i,macLedState[i]);
}

void gui_led(int led, int on)
{	
	macLedState[led]=on;
	if (dont_want_aspect || screen_res == 2)
	{	if (led == 0)
		{	if (on)
			{	ForeColor(redColor);
				PaintRect(&powerRect);
			}
			else
			{	ForeColor(whiteColor);
				PaintRect(&powerRect);
			}
		}
		else
		{	switch (led)
			{	case 1:
					if (on)
					{	ForeColor(greenColor);
						PaintRect(&driveDf0Rect);
					}
					else
					{	ForeColor(whiteColor);
						PaintRect(&driveDf0Rect);
					}
				break;
				
				case 2:
					if (on)
					{	ForeColor(greenColor);
						PaintRect(&driveDf1Rect);
					}
					else
					{	ForeColor(whiteColor);
						PaintRect(&driveDf1Rect);
					}
				break;
				
				case 3:
					if (on)
					{	ForeColor(greenColor);
						PaintRect(&driveDf2Rect);
					}
					else
					{	ForeColor(whiteColor);
						PaintRect(&driveDf2Rect);
					}
				break;
				
				case 4:
					if (on)
					{	ForeColor(greenColor);
						PaintRect(&driveDf3Rect);
					}
					else
					{	ForeColor(whiteColor);
						PaintRect(&driveDf3Rect);
					}
				break;
			}
		}
	ForeColor(blackColor);
	}
}

void gui_filename(int num, char *name)
{
}

static void getline(char *p)
{
}

void gui_handle_events(void)
{	
}

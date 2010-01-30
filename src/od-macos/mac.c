 /* 
  * UAE - The Un*x Amiga Emulator
  * 
  * Mac port specific stuff
  * 
  * (c) 1996 Ernesto Corvi
  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <QDOffscreen.h>
#include <Palettes.h>
#include <Profiler.h>
#include <Sound.h>
#include <errno.h>

#include "mackbd.h"
#include "config.h"
#include "threaddep/penguin.h"
#include "uae.h"
#include "amiga.h"
#include "options.h"
#include "memory.h"
#include "custom.h"
#include "newcpu.h"
#include "xwin.h"
#include "keyboard.h"
#include "keybuf.h"

#define kAppleMenuID	128
#define kFileMenuID		129
#define	kEditMenuID		130
#define	kDrivesMenuID	131
#define	kMemoryMenuID	132
#define	kResMenuID		135
#define	kRateMenuID		134
#define	kVideoMenuID	133

#define kAboutDialogID	128
#define kHardfileDialogID	129
#define kQuitError 130

#define mTopLeft(r)		(((Point *)&(r))[0])
#define mBotRight(r)	(((Point *)&(r))[1])

#define qd_CenterScreen 144

static GDHandle curDevice;
static int oldDepth=0;
static Boolean macCursorState=true;
int	use_quickdraw=true;
short	filesys_vRefNum=0;
static KeyMap keys;
static Boolean redraw=true,diskInit=true;
static int mBarState=1;
static int RM=0;
static int my_use_gfxlib=0;
static int my_automount_uaedev=1;
static int my_screen_res=3;
static int my_use_quickdraw=1;
static WindowPtr mywin;
static RgnHandle gOldVisRgn;
static short gOldMBarHeight;
static Boolean	macDiskStatus[4];
static GWorldPtr gOffscreenBuffer;
static PixMapHandle gOffscreenPixels;
static Rect gOffscreenRect,gDrawRect;	
static unsigned char *gOffscreenBaseAddr;
static unsigned long gOffscreenRowBytes;
static PaletteHandle mypal;
static int CenterScreen=0;
static CTabHandle	myCTAB;
static long screenH=0,screenV=0;
static GrafPtr	dummyPort;
extern SndChannelPtr newChannel;
static unsigned long refresh;
extern char *xlinebuffer;

 /* Keyboard and mouse */

static int keystate[256];

static int colors_allocated;

// Prototypes
static void HandleMenu (long mSelect);
static void macHandleCursors(void);
static Boolean CheckForSetup (void);
static void ParamAString( ConstStr255Param theStr);
static void printStatusLine (void);
static void Share_main(void);
static void Share_ResolvePath (FSSpec *fSpec, char *path);
// end Prototypes

static void my_setpalette(int count, int r, int g, int b)
{	RGBColor mycolor;
	
	mycolor.red=r*1024;
	mycolor.green=g*1024;
	mycolor.blue=b*1024;
	
	if (count == 0 || count == 255) SetEntryColor(mypal, count, &mycolor);
	else SetEntryColor(mypal, (~count & 0x000000ff), &mycolor);
}

static int get_color(int r, int g, int b, xcolnr *cnp)
{
    if (colors_allocated == 256) return -1;
    *cnp = colors_allocated;
	my_setpalette(colors_allocated, doMask(r, 6, 0), doMask(g, 6, 0), doMask(b, 6, 0));
    colors_allocated++;
    return 1;
}

static void init_colors(void)
{
    int rw = 5, gw = 5, bw = 5;
    colors_allocated = 0;
    
    if (gfxvidinfo.pixbytes == 1)
	alloc_colors256(get_color);
	else
	{	if (gfxvidinfo.pixbytes == 4) rw=gw=bw=8;
		alloc_colors64k(rw, gw, bw, gw+bw, bw, 0);
	}
}

static void HideMenuBar(GrafPtr grafPort)
{	Rect	menuRect={0,0,20,640};
	RgnHandle newVisRgn;
	GrafPtr savePort;
	
	GetPort(&savePort);
	SetPort(grafPort);

		// save off vis region
	gOldVisRgn = NewRgn();
	CopyRgn(grafPort->visRgn, gOldVisRgn);

		// expand the vis region to the port rect
	newVisRgn = NewRgn();
	RectRgn(newVisRgn, &grafPort->portRect);
	CopyRgn(newVisRgn, grafPort->visRgn);
	DisposeRgn(newVisRgn);

	PaintRect(&menuRect);

	gOldMBarHeight = GetMBarHeight();
	LMSetMBarHeight(0);
	
	SetPort(savePort);
}

static void ShowMenuBar(GrafPtr grafPort)
{
	GrafPtr savePort;
	RgnHandle junkRgn;

	GetPort(&savePort);
	SetPort(grafPort);

		// fill the rounded corners of the screen with black again
	junkRgn = NewRgn();
	CopyRgn(gOldVisRgn, junkRgn);
	DiffRgn(grafPort->visRgn, junkRgn, junkRgn);

	#ifdef dangerousPattern
	FillRgn(junkRgn, qd.black);
	#else
	FillRgn(junkRgn, &qd.black);
	#endif

	DisposeRgn(junkRgn);

		// restore the old vis region
	CopyRgn(gOldVisRgn, grafPort->visRgn);
	DisposeRgn(gOldVisRgn);
	gOldVisRgn = NULL;
	
	LMSetMBarHeight(gOldMBarHeight);
	
	DrawMenuBar();
}

int graphics_init()
{
    int i;
    char p1;
    long tmp;
    long memtop=0;
	Rect	windowRectangle,osRect={0,0,600,800};
	char *dst;
	CGrafPtr oldPort;
    GDHandle oldDevice;
	
	my_use_gfxlib=use_gfxlib;
	my_automount_uaedev=automount_uaedev;
	my_screen_res=screen_res;
	my_use_quickdraw=use_quickdraw;
	
	if (CheckForSetup()) ExitToShell();
	
	gfxvidinfo.maxblocklines=600; // Whatever...
	
	if (gfxvidinfo.pixbytes == 1)
	{	myCTAB=GetCTable(128);
		mypal=NewPalette(256, myCTAB, pmTolerant + pmExplicit, 0x0000);
	}
	
	windowRectangle.left=0;
	if (screen_res > 4 || screen_res < 0) screen_res=3;
	if (screen_res == 4 || screen_res == 2) dont_want_aspect = 0;
	else dont_want_aspect = 1;
	switch (screen_res)
	{	case 0:
		case 1:
		case 2:
			CenterScreen=(36*gfxvidinfo.pixbytes)/2;
			windowRectangle.right=320;
			use_lores = 1;
			windowRectangle.top=40;
			if (screen_res == 0) windowRectangle.bottom=200+windowRectangle.top+38;
			else if (screen_res == 1) windowRectangle.bottom=256+windowRectangle.top+38;
				 else windowRectangle.bottom=400+windowRectangle.top+38;
		break;
	
		case 3:
		case 4:
			CenterScreen=36*gfxvidinfo.pixbytes;
			use_lores = 0;
			windowRectangle.right=640;
			if (dont_want_aspect == 0)
			{	windowRectangle.top=0;
				windowRectangle.bottom=480;
			}
			else
			{	windowRectangle.top=40;
				windowRectangle.bottom=256+windowRectangle.top+38;
			}
		break;
	}
	
	screenV=windowRectangle.bottom-windowRectangle.top;
	if (dont_want_aspect == 1 || screen_res == 2) screenV -= 12;
	screenH=windowRectangle.right-windowRectangle.left;
	
	mywin = NewCWindow(nil, &windowRectangle, "\pThe Un*x Amiga Emulator", 1, 4, (WindowPtr)-1L, 0, 0);
    SetPort(mywin);
    
    gui_prepare_leds(screenV);
    
    init_colors();
    
    if (use_quickdraw)
    {	GetGWorld(&oldPort, &oldDevice);
    	NewGWorld( &gOffscreenBuffer, (gfxvidinfo.pixbytes*8), &osRect, 0L, oldDevice, 0);
		SetGWorld(oldPort, oldDevice);
		gOffscreenPixels = GetGWorldPixMap( gOffscreenBuffer );
		LockPixels(gOffscreenPixels);
		gDrawRect.top=(mywin->portRect).top;
		gDrawRect.left=(mywin->portRect).left;
		gDrawRect.right=(mywin->portRect).right;
		gDrawRect.bottom=(mywin->portRect).bottom;
		gDrawRect.bottom -= 12;
		gOffscreenRect.top=(mywin->portRect).top;
		gOffscreenRect.left=(mywin->portRect).left;
		gOffscreenRect.right=(mywin->portRect).right;
		gOffscreenRect.bottom=(mywin->portRect).bottom;
		gOffscreenRect.bottom -= 12;
		OffsetRect(&gOffscreenRect,(use_lores == 1) ? qd_CenterScreen/2 : qd_CenterScreen,0);
		gOffscreenBaseAddr = (unsigned char *)GetPixBaseAddr(gOffscreenPixels);
		gOffscreenRowBytes = (*gOffscreenPixels)->rowBytes & 0x3fff;
		gfxvidinfo.bufmem=(char *)gOffscreenBaseAddr;
		gfxvidinfo.rowbytes=gOffscreenRowBytes;
		SetGWorld(gOffscreenBuffer, NULL);
		PaintRect(&gOffscreenRect);
		SetGWorld(oldPort, oldDevice);
	}
	else
	{	gfxvidinfo.bufmem=NewPtrClear(600*(800*gfxvidinfo.pixbytes));
		if (gfxvidinfo.bufmem == 0) SysBeep(0);
	    gfxvidinfo.rowbytes=800*gfxvidinfo.pixbytes;
	}
    
    if (dont_want_aspect || screen_res == 2) printStatusLine();
    
    if (gfxvidinfo.pixbytes == 1)
    {	SetPalette((WindowPtr) -1L, mypal, false);
    }
    
    buttonstate[0] = buttonstate[1] = buttonstate[2] = 0;
    for(i=0; i<256; i++) keystate[i] = 0;
    
    lastmx = lastmy = 0; newmousecounters=0;
	
    refresh=TickCount();
    
//    tmp=ProfilerInit(collectDetailed,bestTimeBase,200,10);
    return 1;
}

void graphics_leave()
{
//	ProfilerDump((unsigned char *)"\pmyProf");
//	ProfilerTerm();
 
    ShowCursor();
	if (dont_want_aspect == 0) if (!mBarState) ShowMenuBar(mywin);
    if (oldDepth != 0) SetDepth(curDevice,oldDepth,0,0);
    FlushEvents (everyEvent,0);
    SndDisposeChannel(newChannel,true);
    if (gfxvidinfo.pixbytes == 1)
    {	DisposePalette(mypal);
    	DisposeCTable(myCTAB);
    }
    DisposeWindow(mywin);
    if (use_quickdraw)
    {	DisposeGWorld(gOffscreenBuffer);
    }
    else DisposePtr((Ptr)(gfxvidinfo.bufmem));
}

static int next_line_double;
static int next_line_pos = 0;

void flush_screen ()
{	short	y,x,y1=0;
    unsigned char *winbaseaddr;
    long *src,*dest;
    unsigned long winrowbytes;
    PixMapHandle	ph;
    CGrafPtr	oldPort;
	GDHandle oldDevice;

	if (TickCount() < refresh + 4) return;
	if (!redraw) return;
	if (FrontWindow() != mywin) return;
	if (inhibit_frame) return;
   	
   	GetGWorld(&oldPort,&oldDevice);
	SetGWorld((CWindowPtr) mywin, oldDevice);
    if (use_quickdraw)
    {	ForeColor(blackColor);	/* Prevents colorizing mode */
	    BackColor(whiteColor);	/* Prevents colorizing mode */
    	(*((*gOffscreenPixels)->pmTable))->ctSeed = (*((*((*(GetGDevice()))->gdPMap))->pmTable))->ctSeed;
    	CopyBits((BitMap *)(*gOffscreenPixels), &(mywin->portBits), &gOffscreenRect, &gDrawRect, srcCopy, (RgnHandle) 0L);
    }
    else
    {	ph=GetGWorldPixMap((CGrafPort *) mywin);
	    winbaseaddr=( unsigned char *) GetPixBaseAddr(ph);
	    winrowbytes=(*ph)->rowBytes & 0x3FFF;
	    winbaseaddr-=((**ph).bounds.left);
		winbaseaddr-=(((**ph).bounds.top)*winrowbytes);
	    dest=(long *)winbaseaddr;
	    src=(long *)(gfxvidinfo.bufmem)+CenterScreen;
	    
		y=0;
	    if ((dont_want_aspect == 0 && mBarState == true && y < 20)) y=20;
	    dest=(long *)(winbaseaddr+(y*winrowbytes));
		src=(long *)(gfxvidinfo.bufmem+(y*gfxvidinfo.rowbytes))+CenterScreen;
		for (; y< screenV; y++)
		{	for (x=0;x < ((screenH*gfxvidinfo.pixbytes)/sizeof(long));x++) *dest++ = *src++;
		dest=(long *)(winbaseaddr+(y*winrowbytes));
		src=(long *)(gfxvidinfo.bufmem+(y*gfxvidinfo.rowbytes))+CenterScreen;
	    }
	}
	SetGWorld(oldPort, oldDevice);
}

void flush_block (int start_y, int end_y)
{	short	y,x,y1=0;
    unsigned char *winbaseaddr;
    long *src,*dest;
    unsigned long winrowbytes;
    PixMapHandle	ph;
    Rect			srcRect,dstRect;
    CGrafPtr	oldPort;
	GDHandle oldDevice;

	if (TickCount() < refresh + 4) return;
	if (!redraw) return;
	if (FrontWindow() != mywin) return;
	if (inhibit_frame) return;
   	
   	if (end_y > screenV) end_y=screenV;
   	GetGWorld(&oldPort,&oldDevice);
	SetGWorld((CWindowPtr) mywin, oldDevice);
    if (use_quickdraw)
    {	
    	srcRect.top=start_y;
    	srcRect.bottom=end_y;
    	srcRect.left=gOffscreenRect.left;
    	srcRect.right=gOffscreenRect.right;
    	dstRect.top=start_y;
    	dstRect.bottom=end_y;
    	dstRect.left=gDrawRect.left;
    	dstRect.right=gDrawRect.right;
    	
    	/* Taken from my favorite book:
    	   "It is significant, if not surprising, that the Mac and PC cannot come
    	   to an agreement over the most fundamental issue in the universe: the
    	   distinction between black and white. On the PC, the pixel value 0
    	   indicates a pixel of zero intensity, or black. On the Mac, 0 indicates
    	   a page wich has not been written on, wich leaves it white."
    	   
    	   										Eric Johnston.
    	   							Tricks of the Mac Game Programming Gurus.
    	
    	Check set_palette and line_to_scr8() on how i fixed that.
    	If you know a better way let me know.
    	*/
    	
    	ForeColor(blackColor);	/* Prevents colorizing mode */
	    BackColor(whiteColor);	/* Prevents colorizing mode */
    	(*((*gOffscreenPixels)->pmTable))->ctSeed = (*((*((*(GetGDevice()))->gdPMap))->pmTable))->ctSeed;
    	CopyBits((BitMap *)(*gOffscreenPixels), &(mywin->portBits), &srcRect, &dstRect, srcCopy, (RgnHandle) 0L);
    }
    else
    {	ph=GetGWorldPixMap((CGrafPort *) mywin);
	    winbaseaddr=( unsigned char *) GetPixBaseAddr(ph);
	    winrowbytes=(*ph)->rowBytes & 0x3FFF;
	    winbaseaddr-=((**ph).bounds.left);
		winbaseaddr-=(((**ph).bounds.top)*winrowbytes);
	    dest=(long *)winbaseaddr;
	    src=(long *)(gfxvidinfo.bufmem)+CenterScreen;
	    
		y=start_y;
	    if ((dont_want_aspect == 0 && mBarState == true && y < 20)) y=20;
	    dest=(long *)(winbaseaddr+(y*winrowbytes));
		src=(long *)(gfxvidinfo.bufmem+(y*gfxvidinfo.rowbytes))+CenterScreen;
		for (; y <= end_y; y++)
		{	for (x=0;x < ((screenH*gfxvidinfo.pixbytes)/sizeof(long));x++) *dest++ = *src++;
		dest=(long *)(winbaseaddr+(y*winrowbytes));
		src=(long *)(gfxvidinfo.bufmem+(y*gfxvidinfo.rowbytes))+CenterScreen;
	    }
	}
	SetGWorld(oldPort, oldDevice);
}

void flush_line(int line_num)
{
    long i;
    char *dst,*src;
    CGrafPtr oldPort;
    GDHandle oldDevice;
	 
	if (use_quickdraw)
	{	GetGWorld(&oldPort, &oldDevice);
	    SetGWorld(gOffscreenBuffer, NULL);
    	dst=(char *)gOffscreenBaseAddr;
		dst+=(char *)((line_num)*gOffscreenRowBytes);
		src=(char *)xlinebuffer;
		BlockMove(src,dst,gOffscreenRowBytes);
		if (next_line_double)
		{	dst=(char *)gOffscreenBaseAddr;
			dst+=(char *)((line_num+1)*gOffscreenRowBytes);
			src=(char *)xlinebuffer;
			BlockMove(src,dst,gOffscreenRowBytes);
		}
		SetGWorld(oldPort, oldDevice);
	}
	else
	{	if (line_num < 481)
		{	dst=(char *)gfxvidinfo.bufmem+((line_num)*gfxvidinfo.rowbytes);
			src=(char *)xlinebuffer;
			BlockMove(src,dst,gfxvidinfo.rowbytes);
		}
    	if (next_line_double)
    	{	
    		if (line_num < 481)
			{   dst=(char *)gfxvidinfo.bufmem+((line_num+1)*gfxvidinfo.rowbytes);
				src=(char *)xlinebuffer;
			BlockMove(src,dst,gfxvidinfo.rowbytes);
			}
    	}
    }
}

/* Decode KeySyms. This function knows about all keys that are common 
 * between different keyboard languages.
 */
static int kc_decode (long ks)
{	
    switch (ks)
    {
     case kAKeyMap: return AK_A;
     case kBKeyMap: return AK_B;
     case kCKeyMap: return AK_C;
     case kDKeyMap: return AK_D;
     case kEKeyMap: return AK_E;
     case kFKeyMap: return AK_F;
     case kGKeyMap: return AK_G;
     case kHKeyMap: return AK_H;
     case kIKeyMap: return AK_I;
     case kJKeyMap: return AK_J;
     case kKKeyMap: return AK_K;
     case kLKeyMap: return AK_L;
     case kMKeyMap: return AK_M;
     case kNKeyMap: return AK_N;
     case kOKeyMap: return AK_O;
     case kPKeyMap: return AK_P;
     case kQKeyMap: return AK_Q;
     case kRKeyMap: return AK_R;
     case kSKeyMap: return AK_S;
     case kTKeyMap: return AK_T;
     case kUKeyMap: return AK_U;
     case kVKeyMap: return AK_V;
     case kWKeyMap: return AK_W;
     case kXKeyMap: return AK_X;
     
     case k0KeyMap: return AK_0;
     case k1KeyMap: return AK_1;
     case k2KeyMap: return AK_2;
     case k3KeyMap: return AK_3;
     case k4KeyMap: return AK_4;
     case k5KeyMap: return AK_5;
     case k6KeyMap: return AK_6;
     case k7KeyMap: return AK_7;
     case k8KeyMap: return AK_8;
     case k9KeyMap: return AK_9;
     
     case kKP0KeyMap: return AK_NP0;
     case kKP1KeyMap: return AK_NP1;
     case kKP2KeyMap: return AK_NP2;
     case kKP3KeyMap: return AK_NP3;
     case kKP4KeyMap: return AK_NP4;
     case kKP5KeyMap: return AK_NP5;
     case kKP6KeyMap: return AK_NP6;
     case kKP7KeyMap: return AK_NP7;
     case kKP8KeyMap: return AK_NP8;
     case kKP9KeyMap: return AK_NP9;
	
     case kF1KeyMap: return AK_F1;
     case kF2KeyMap: return AK_F2;
     case kF3KeyMap: return AK_F3;
     case kF4KeyMap: return AK_F4;
     case kF5KeyMap: return AK_F5;
     case kF6KeyMap: return AK_F6;
     case kF7KeyMap: return AK_F7;
     case kF8KeyMap: return AK_F8;
     case kF9KeyMap: return AK_F9;
     case kF10KeyMap: return AK_F10;
	
     case kBackSpaceKeyMap: return AK_BS;
     case kTabKeyMap: return AK_TAB;
     case kReturnKeyMap: return AK_RET;
     case kEscapeKeyMap: return AK_ESC;
     
     case kSpaceBarMap:  return AK_SPC;
     
     case kUpArrowKeyMap: return AK_UP;
     case kDownArrowKeyMap: return AK_DN;
     case kLeftArrowKeyMap: return AK_LF;
     case kRightArrowKeyMap: return AK_RT;
	
     case kF11KeyMap: return AK_inhibit;

     case kF12KeyMap: return AK_mousestuff;

     case kPgUpKeyMap: return AK_RAMI;
     case kPgDnKeyMap: return AK_LAMI;
     case kBackSlash: return AK_BACKSLASH;
    }
    return -1;
}

static int decode_us(long ks)
{
    switch(ks) {
	/* US specific */

     case kYKeyMap: return AK_Y;
     case kZKeyMap: return AK_Z;
     case kLBracketKeyMap: return AK_LBRACKET;
     case kRBracketKeyMap: return AK_RBRACKET;
     case kCommaKeyMap: return AK_COMMA;
     case kPeriodKeyMap: return AK_PERIOD;
     case kSlashKeyMap: return AK_SLASH;
     case kSemiColonKeyMap: return AK_SEMICOLON;
     case kMinusKeyMap: return AK_MINUS;
     case kEqualKeyMap: return AK_EQUAL;
     case kQuoteKeyMap: return AK_QUOTE;
    }

    return -1;
}

static int decode_de(long ks)
{
    switch(ks) {
/* DE specific
     case XK_Y: case XK_y: return AK_Z;
     case XK_Z: case XK_z: return AK_Y;
     case XK_Odiaeresis: case XK_odiaeresis: return AK_SEMICOLON;
     case XK_Adiaeresis: case XK_adiaeresis: return AK_QUOTE;
     case XK_Udiaeresis: case XK_udiaeresis: return AK_LBRACKET;
     case XK_plus: case XK_asterisk: return AK_RBRACKET;
     case XK_comma: return AK_COMMA;
     case XK_period: return AK_PERIOD;
     case XK_less: case XK_greater: return AK_LTGT;
     case XK_numbersign: return AK_NUMBERSIGN;
     case XK_ssharp: return AK_MINUS;
     case XK_apostrophe: return AK_EQUAL;
     case XK_asciicircum: return AK_00;
     case XK_minus: return AK_SLASH;	    
*/
    }

    return -1;
}

static int keycode2amiga(long code)
{
    long ks;
    int as;
    
    ks = (code & keyCodeMask) >> 8;
    as = kc_decode (ks);
	
    if (as == -1) {	    
	switch(keyboard_lang) {
	 case KBD_LANG_US:
	    as = decode_us(ks);
	    break;
		
	 case KBD_LANG_DE:
	    as = decode_de(ks);
	    break;
		
	 default:
	    as = -1;
	    break;
	}
    }
    if(-1 != as)
	return as;
    return -1;
}

void handle_events()
{	WindowPeek	wp;
	short		windowPart;
	Boolean repeat;
    Boolean itHappened;
    Point   mpos;
    EventRecord event;
    int kc,i,count;
    char	osKind;
    GrafPtr	oldSave;
	
	GetPort(&oldSave);
	if ((redraw) && mywin != FrontWindow()) SelectWindow(mywin);
    
    SetEventMask(-1);
    
    if (diskInit)
    {	macDiskStatus[0]=!disk_empty(0);
		SetMenuItemText(GetMenuHandle(kDrivesMenuID), 1, macDiskStatus[0] ? "\pEject Disk in DF0:" : "\pInsert Disk in DF0:");
    	macDiskStatus[1]=!disk_empty(1);
		SetMenuItemText(GetMenuHandle(kDrivesMenuID), 2, macDiskStatus[1] ? "\pEject Disk in DF1:" : "\pInsert Disk in DF1:");
    	macDiskStatus[2]=!disk_empty(2);
		SetMenuItemText(GetMenuHandle(kDrivesMenuID), 3, macDiskStatus[2] ? "\pEject Disk in DF2:" : "\pInsert Disk in DF2:");
    	macDiskStatus[3]=!disk_empty(3);
		SetMenuItemText(GetMenuHandle(kDrivesMenuID), 4, macDiskStatus[3] ? "\pEject Disk in DF3:" : "\pInsert Disk in DF3:");
    	diskInit=false;
    }
    
	if (redraw)
	{	gui_update_leds();
		macHandleCursors();
	}
    
    GetKeys(keys);
    if (BitTst(&keys, kCommandRawKey))
	buttonstate[2] = 1;
    else
	buttonstate[2] = 0;
    
    if (BitTst(&keys, kShiftRawKey))
    {	
	if (!keystate[AK_LSH]) {
	    keystate[AK_LSH] = 1;
	    record_key (AK_LSH << 1);
	    goto label1;
	}
    } else {
	if (keystate[AK_LSH]) {
	    keystate[AK_LSH] = 0;
	    record_key ((AK_LSH << 1) | 1);
	    goto label1;
	}
    }
    if (BitTst(&keys, kControlRawKey))
    {	
	if (!keystate[AK_CTRL]) {
	    keystate[AK_CTRL] = 1;
	    record_key (AK_CTRL << 1);
	    goto label1;
	}
    } else {	
	if (keystate[AK_CTRL]) {
	    keystate[AK_CTRL] = 0;
	    record_key ((AK_CTRL << 1) | 1);
	    goto label1;
	}
    }
    if (BitTst(&keys, kCapsRawKey))
    {	
	if (!keystate[AK_CAPSLOCK]) {
	    keystate[AK_CAPSLOCK] = 1;
	    record_key (AK_CAPSLOCK << 1);
	    goto label1;
	}
    } else {
	if (keystate[AK_CAPSLOCK]) {
	    keystate[AK_CAPSLOCK] = 0;
	    record_key ((AK_CAPSLOCK << 1) | 1);
	    goto label1;
	}
    }
    if (BitTst(&keys, kOptionRawKey))
    {	
	if (!keystate[AK_LALT]) {
	    keystate[AK_LALT] = 1;
	    record_key (AK_LALT << 1);
	    goto label1;
	}
    } else {
	if (keystate[AK_LALT]) {
	    keystate[AK_LALT] = 0;
	    record_key ((AK_LALT << 1) | 1);
	    goto label1;
	}
    }
    do {
	repeat = 0;
	newmousecounters=0;
	itHappened=WaitNextEvent(-1,&event,0L,(*mywin).visRgn);

	switch(event.what) {
	 case keyDown:
	 case autoKey: {
	 	 if ((event.modifiers & cmdKey) != 0)
	 	 {	HandleMenu(MenuKey((char) (event.message & charCodeMask)));
	 	 }
	 	 else
	 	 {	int kc = keycode2amiga(event.message);
	 	    if (kc == -1) break;
		     switch (kc)
		     {	case AK_mousestuff:
			  	   togglemouse();
			 		 break;
		
			    case AK_inhibit:
				 	 inhibit_frame ^= 1;
			 		 break;
		
			    default:
			     	 if (!keystate[kc])
			     	 { 	keystate[kc] = 1;
			    	 	record_key (kc << 1);
			 		 }
		 			 break;
			 }
		}
	 	break;
	 }
	 case keyUp: {
	     kc = keycode2amiga(event.message);
	     if (kc == -1) break;
	     keystate[kc] = 0;
	     record_key ((kc << 1) | 1);
	     break;
	 }
	 case mouseDown:
	 	windowPart = FindWindow (event.where, (WindowPtr*) &wp);
	 	if (windowPart == inMenuBar) HandleMenu(MenuSelect(event.where));
	 	else
	 	if (windowPart == inSysWindow) SystemClick (&event, (WindowPtr) wp);
	    else buttonstate[0] = 1;
	    break;
	 	
	 case osEvt:
	 	osKind=(event.message) >> 24;
	 	if (osKind == suspendResumeMessage)
	 	{	osKind=(event.message)&1;
	 		if (osKind)
	 		{	redraw=true; // Resume
	 			flush_screen();
	 			if (dont_want_aspect || screen_res == 2)
			    {	SelectWindow(mywin);
			    	printStatusLine();
			    }
			}
	 		else redraw=false; // Suspend
	 	}
	 	break;
	 case mouseUp:
	    buttonstate[0] = 0;
	    buttonstate[2] = 0;
	    break;
	}
	if (redraw)
	{	GetMouse(&mpos);
		if (mpos.h != lastmx) { lastmx=mpos.h; /* repeat = 1;*/ }
		if (mpos.v != lastmy) { lastmy=mpos.v; /*repeat = 1;*/ }
	}
    } while (repeat);
    
label1:
    /* "Affengriff" */
    if(keystate[AK_CTRL] && keystate[AK_LAMI] && keystate[AK_RAMI])
    	m68k_reset();
    SetPort(oldSave);
}

int debuggable()
{
    return 1;
}

int needmousehack()
{
    return 1;
}

void LED(int on)
{
}

void parse_cmdline ()
{
    /* No commandline on the Mac. */
}

static void PStrCat ( StringPtr p1, StringPtr p2)
{
	register int len;
	register int total;
	StringPtr	p3;
	
	len = *p1++;
	total = len +*p2;
	
	p3=p2;
	p2=p2+ (*p2 + 1);
	
	while (--len>=0) *p2++=*p1++;
	*p3=total;
}

static void PStrCopy ( StringPtr p1, StringPtr p2)
{
	register int len;
	
	len = *p2++ = *p1++;
	while (--len>=0) *p2++=*p1++;
}

short vRefNum=0;
long dirID=0;

static void HandleMenu (long mSelect)
{	short		menuID;
	short		menuItem;
	Str32		name;
	GrafPtr		savePort;
	WindowPtr	batchWin;
	Str255		batchStr,batchStr2;
	StandardFileReply		inputReply;
	char		strSize,i;
	long		count;
	FILE		*outFile;
	char		fileBuf[512];
	int			backup=0;

	menuID = HiWord(mSelect);
	menuItem = LoWord(mSelect);

	if (menuID == 0) return;
	switch (menuID)
	{	case kAppleMenuID:
	  		if (menuItem == 1)
	  		{	if (!macCursorState) ShowCursor();
				macCursorState=true;
	  			Alert(kAboutDialogID,0); // About Box
	  			if (dont_want_aspect || screen_res == 2) printStatusLine();
	  		}
	 		else
	 		{	GetPort(&savePort);
	 			GetItem(GetMenuHandle(kAppleMenuID), menuItem, name);
				OpenDeskAcc(name);
				SystemTask();
				SetPort(savePort);
			}
			break;
	 	
	 	case kFileMenuID:
			switch (menuItem)
			{	case 1:
					m68k_reset();
				break;
				
				case 3:
					if (dont_want_aspect == 0)
					{	if (mBarState) HideMenuBar(mywin);
						else ShowMenuBar(mywin);
						mBarState=!mBarState;
					}
				break;
				
				case 5:
			                activate_debugger();
				break;
				
				case 7:
					inhibit_frame ^= 1;
					if (inhibit_frame) SetMenuItemText(GetMenuHandle(kFileMenuID), 7, "\pTurn Screen Update On");
					else SetMenuItemText(GetMenuHandle(kFileMenuID), 7, "\pTurn Screen Update Off");
				break;
				
				case 9:
					produce_sound= !produce_sound;
					if (produce_sound) SetMenuItemText(GetMenuHandle(kFileMenuID), 9, "\pTurn Sound Off");
					else SetMenuItemText(GetMenuHandle(kFileMenuID), 9, "\pTurn Sound On");
					WritePrefs(0);
				break;
				
				case 11:
					fake_joystick= !fake_joystick;
					if (fake_joystick) SetMenuItemText(GetMenuHandle(kFileMenuID), 11, "\pTurn Joystick Off");
					else SetMenuItemText(GetMenuHandle(kFileMenuID), 11, "\pTurn Joystick On");
					WritePrefs(0);
				break;
				
				case 13:
					broken_in = 1;
					regs.spcflags |= SPCFLAG_BRK;
					quit_program=1;
				break;
			}
			break;
		
		case kDrivesMenuID:
			if (menuItem >= 1 && menuItem <= 4)
			{	if (macDiskStatus[menuItem-1]) disk_eject(menuItem-1);
					else
					{	if (!macCursorState) ShowCursor();
						if (!mBarState) ShowMenuBar(mywin);
						macCursorState=true;
						StandardGetFile(nil,-1L,nil,&inputReply);
						if (inputReply.sfGood)
						{	vRefNum=inputReply.sfFile.vRefNum;
							dirID=inputReply.sfFile.parID;
							disk_insert (menuItem-1, p2cstr(inputReply.sfFile.name));
						}
						else macDiskStatus[menuItem-1] = !macDiskStatus[menuItem-1];
						if (!mBarState) HideMenuBar(mywin);
					}
					macDiskStatus[menuItem-1] = !macDiskStatus[menuItem-1];
					if (macDiskStatus[menuItem-1]) PStrCopy("\pEject Disk in DF",batchStr);
					else PStrCopy("\pInsert Disk in DF",batchStr);
					NumToString((long)(menuItem-1),batchStr2);
					PStrCat(batchStr2,batchStr);
					PStrCat("\p:",batchStr);
					SetMenuItemText(GetMenuHandle(kDrivesMenuID), menuItem, batchStr);
		
			}
			if (menuItem == 6)
			{	if (!macCursorState) ShowCursor();
				macCursorState=true;
				for (count=0;count<512;count++) fileBuf[count]=0;
				errno=0;
				outFile=fopen("hardfile","wb");
				if (outFile != 0)
				{	for (count=0;count<16384;count++)
					{	fwrite(fileBuf, sizeof(char), 512, outFile);
						if (errno != 0)
						{	ParamAString("\pError generating the hardfile!.\n Make sure you have at least 8Mb of free storage on your Harddrive.");
							DisplayError(kQuitError);
						}
					}
					fclose(outFile);
					Alert(kHardfileDialogID,0);
				}
				else
				{	ParamAString("\pCan't create the hardfile!.\n Make sure your Harddrive isnt either software or hardware locked.");
					DisplayError(kQuitError);
				}
			}
			if (menuItem == 7)
			{	my_automount_uaedev=!my_automount_uaedev;
				WritePrefs(0);
				ParamAString("\pChanges will take effect the next time you run UAE.");
				DisplayError(kQuitError);
				CheckItem(GetMenuHandle(kDrivesMenuID),7,my_automount_uaedev ? false : true);
			}
			if (menuItem == 9)
			{	if (!macCursorState) ShowCursor();
				macCursorState=true;
				Share_main();
			}
		break;
		
		case kMemoryMenuID:
			if (menuItem == 1) use_slow_mem = !use_slow_mem;
			WritePrefs(0);
			ParamAString("\pChanges will take effect the next time you run UAE.");
			DisplayError(kQuitError);
			if (use_slow_mem) SetMenuItemText(GetMenuHandle(kMemoryMenuID), 1, "\pDisable 1 Mb (SlowMem)");
			else SetMenuItemText(GetMenuHandle(kMemoryMenuID), 1, "\pEnable 1 Mb (SlowMem)");
		break;
			
		case kResMenuID:
			my_screen_res=(int)menuItem-1;
			WritePrefs(0);
			CheckItem(GetMenuHandle(kResMenuID),screen_res+1,false);
			CheckItem(GetMenuHandle(kResMenuID),my_screen_res+1,true);
			ParamAString("\pChanges will take effect the next time you run UAE.");
			DisplayError(kQuitError);
		break;
		
		case kRateMenuID:
			for (count=1;count<5;count++) CheckItem(GetMenuHandle(kRateMenuID),count,false);
			if (menuItem == 1) framerate=1;
			if (menuItem == 2) framerate=3;
			if (menuItem == 3) framerate=5;
			if (menuItem == 4) framerate=7;
			CheckItem(GetMenuHandle(kRateMenuID),menuItem,true);
			WritePrefs(0);
		break;
		
		case kVideoMenuID:
		if (menuItem == 5)
		{	my_use_quickdraw = !my_use_quickdraw;
			WritePrefs(0);
			CheckItem(GetMenuHandle(kVideoMenuID),5,my_use_quickdraw ? true : false);
			ParamAString("\pChanges will take effect the next time you run UAE.");
			DisplayError(kQuitError);
		}
		if (menuItem == 7)
		{	my_use_gfxlib = !my_use_gfxlib;
			WritePrefs(0);
			CheckItem(GetMenuHandle(kVideoMenuID),7,my_use_gfxlib ? true : false);
			ParamAString("\pChanges will take effect the next time you run UAE.");
			DisplayError(kQuitError);
		}
		break;
		
		default:
			break;
	}
	HiliteMenu(0);
	flush_screen();
	if (dont_want_aspect  || screen_res == 2) printStatusLine();
}

static void macHandleCursors(void)
{	Point	mpos;

	if (redraw)
	{	GetMouse(&mpos);
		if (!dont_want_aspect && mBarState && screen_res == 4)
		{	if (mpos.v < 20)
			{	ShowCursor();
				macCursorState=true;
			}
			else
			{	if (macCursorState)
				{	HideCursor();
					macCursorState=false;
				}
			}
		}
		else
		if (PtInRect(mpos,&(mywin->portRect)))
		{	if (macCursorState)
			{	HideCursor();
				macCursorState=false;
			}
		}
		else
		{	if (!macCursorState)
			{	ShowCursor();
				macCursorState=true;
			}
		}
	}
}

// Check Minimal System Configuration and Setup
static Boolean CheckForSetup (void)
{	Boolean			retvalue=false;
	SysEnvRec		env;
	
	oldDepth=0;
	SysEnvirons( 2, &env );
	
	if ( env.systemVersion < 0x0700 )
	{	ParamAString("\pUAE requires System 7 or later!\n Press Ok to Quit...");
		DisplayError(kQuitError);
		retvalue=true;
	}
	if ( !env.hasColorQD)
	{	ParamAString("\pUAE requires Color QuickDraw!\n Press Ok to Quit...");
		DisplayError(kQuitError);
		retvalue=true;
	}
	curDevice = GetMainDevice();
	gfxvidinfo.pixbytes=((*(*curDevice)->gdPMap)->pixelSize)/8;
	return(retvalue);
}

// Shows up the standard error alert;
static int DisplayError(int ID)
{	int	ret=0;

	InitCursor();
	ret=Alert(ID,0);

	return (ret);
}

// Parses a Pascal string for error display
static void ParamAString( ConstStr255Param theStr )
{
	ParamText(theStr, "\p", "\p", "\p");
}

static void printStatusLine (void)
{	GrafPtr savePort;
	Rect	updateRect;

	updateRect.top=screenV;
	updateRect.bottom=screenV+38;
	updateRect.left=0;
	updateRect.right=screenH;
	GetPort(&savePort);
	SetPort(mywin);
	EraseRect(&updateRect);
	MoveTo(10,screenV+10);
	TextFont(monaco);
	TextSize(9);
	if (screen_res >= 3)
	DrawString("\pPower LED:        Drive LEDs:  DF0:        DF1:        DF2:        DF3:");
	else DrawString("\pPower:      DF0:      DF1:      DF2:      DF3:");
	TextFont(0);
 	TextSize(0);
 	SetPort(savePort);
}

typedef struct {
	StandardFileReply *replyPtr;
	FSSpec oldSelection;
} SFData, *SFDataPtr;

/* constants */

#define	kSelectItem			10
#define	kSFDlg				128
#define	kCanSelectDesktop	true
#define	kSelectStrRsrc		128
#define kDefaultSelectString "\pSelect"
#define	kDeskStrRsrc		129
#define	kDefaultDeskString	"\pDesktop"
#define	kSelectKey			's'

/* globals */

Boolean gHasFindFolder;
FSSpec gDeskFolderSpec;
Str255 gSelectString;
Str255 gDesktopFName;

FileFilterYDUPP Share_FilterAllFiles_UPP;
DlgHookYDUPP Share_MyDlgHook_UPP;
ModalFilterYDUPP Share_MyModalFilter_UPP;

/* prototypes */

static void Share_Init(void);
static Boolean Share_CustomGet(FSSpec *fSpec);
static pascal short Share_MyDlgHook(short item,DialogPtr theDlg,Ptr userData);
static pascal Boolean Share_MyModalFilter(DialogPtr theDlg,EventRecord *ev,short *itemHit,Ptr myData);
static void Share_HitButton(DialogPtr theDlg,short item);
static pascal Boolean Share_FilterAllFiles(CInfoPBPtr pb, Ptr myDataPtr);
static void Share_SetSelectButtonName(StringPtr selName,Boolean hilited,DialogPtr theDlg);
static Boolean Share_SameFile(FSSpec *file1,FSSpec *file2);
static Boolean Share_GetFSSpecPartialName(FSSpec *file,StringPtr fName);
static OSErr Share_GetDeskFolderSpec(FSSpec *fSpec,short vRefNum);
static OSErr Share_MakeCanonFSSpec(FSSpec *fSpec);
static Boolean Share_ShouldHiliteSelect(FSSpec *fSpec);


static void Share_main(void)
{	FSSpec fSpec;
	Boolean good;
	GrafPtr oldPort;
	char share_path[1024];
	Handle	share_handle;
	short backup;
	
	GetPort(&oldPort);
	Share_Init();
	
	good = Share_CustomGet(&fSpec);
	
	if (good)
	{	Share_ResolvePath(&fSpec,share_path);
		backup=filesys_vRefNum;
		filesys_vRefNum=fSpec.vRefNum;
		WritePrefs(share_path);
		filesys_vRefNum=backup;
	}
	SetPort(oldPort);
}

static void Share_Init(void)
{
	Handle strHndl;
	
	gHasFindFolder = true;
	
	Share_FilterAllFiles_UPP=NewFileFilterYDProc(Share_FilterAllFiles);
	Share_MyDlgHook_UPP=NewDlgHookYDProc(Share_MyDlgHook);
	Share_MyModalFilter_UPP=NewModalFilterYDProc(Share_MyModalFilter);
	
	strHndl = Get1Resource('STR ',kSelectStrRsrc);
	if (ResError()!=noErr || !strHndl || !*strHndl)
		BlockMove(kDefaultSelectString,gSelectString,kDefaultSelectString[0]+1);
	else {
		BlockMove(*strHndl,&gSelectString,(long)((unsigned char *)(*strHndl)[0]+1));
		ReleaseResource(strHndl);
	}

	strHndl = Get1Resource('STR ',kDeskStrRsrc);
	if (ResError()!=noErr || !strHndl || !*strHndl)
		BlockMove(kDefaultDeskString,gDesktopFName,kDefaultSelectString[0]+1);
	else {
		BlockMove(*strHndl,&gDesktopFName,(long)((unsigned char *)(*strHndl)[0]+1));
		ReleaseResource(strHndl);
	}
}


/* do getfile */

static Boolean Share_CustomGet(FSSpec *fSpec)
{
	Point where = {-1,-1};
	SFReply reply;
	DialogPtr theDialog;
	short item;
	StandardFileReply sfReply;
	SFData sfUserData;
	OSErr err;
	Boolean targetIsFolder,wasAliased;
	
	/* initialize user data area */
	
	sfUserData.replyPtr = &sfReply;
	sfUserData.oldSelection.vRefNum = -9999;	/* init to ridiculous value */
	
	CustomGetFile(Share_FilterAllFiles_UPP,-1,nil,&sfReply,kSFDlg,where,Share_MyDlgHook_UPP,
					Share_MyModalFilter_UPP,nil,nil,&sfUserData);
	
	if (sfReply.sfGood) {
		err = ResolveAliasFile(&sfReply.sfFile,true,&targetIsFolder,&wasAliased);
		if (err!=noErr)
			return false;
	}
	
	err = FSMakeFSSpec(sfReply.sfFile.vRefNum,sfReply.sfFile.parID,sfReply.sfFile.name,fSpec);
	if (err!=noErr)
		return false;
	
	return sfReply.sfGood;
}

static pascal short Share_MyDlgHook(short item,DialogPtr theDlg,Ptr userData)
{
	SFDataPtr sfUserData;
	Boolean hiliteButton;
	FSSpec curSpec;
	OSType refCon;
	
	refCon = GetWRefCon(theDlg);
	if (refCon!=sfMainDialogRefCon)
		return item;
		
	sfUserData = (SFDataPtr) userData;
	
	if (item==sfHookFirstCall || item==sfHookLastCall)
		return item;
	
	if (item==sfItemVolumeUser) {
		sfUserData->replyPtr->sfFile.name[0] = '\0';
		sfUserData->replyPtr->sfFile.parID = 2;
		sfUserData->replyPtr->sfIsFolder = false;
		sfUserData->replyPtr->sfIsVolume = false;
		sfUserData->replyPtr->sfFlags = 0;
		item = sfHookChangeSelection;
	}
		
	if (!Share_SameFile(&sfUserData->replyPtr->sfFile,&sfUserData->oldSelection)) {
		BlockMove(&sfUserData->replyPtr->sfFile,&curSpec,sizeof(FSSpec));
		Share_MakeCanonFSSpec(&curSpec);
		
		if (curSpec.vRefNum!=sfUserData->oldSelection.vRefNum)
			Share_GetDeskFolderSpec(&gDeskFolderSpec,curSpec.vRefNum);	
		Share_SetSelectButtonName(curSpec.name,Share_ShouldHiliteSelect(&curSpec),theDlg);
		
		BlockMove(&sfUserData->replyPtr->sfFile,&sfUserData->oldSelection,sizeof(FSSpec));
	}
	
	if (item==kSelectItem)
		item = sfItemOpenButton;
		
	return item;
}


static pascal Boolean Share_MyModalFilter(DialogPtr theDlg,EventRecord *ev,short *itemHit,Ptr myData)
{
	Boolean evHandled;
	char keyPressed;
	OSType refCon;
	
	refCon = GetWRefCon(theDlg);
	if (refCon!=sfMainDialogRefCon)
		return false;
		
	evHandled = false;
	
	switch (ev->what) {
		case keyDown:
		case autoKey:
			keyPressed = ev->message & charCodeMask;
			if ((ev->modifiers & cmdKey) != 0) {
				switch (keyPressed) {
					case kSelectKey:
						Share_HitButton(theDlg,kSelectItem);
						*itemHit = kSelectItem;
						evHandled = true;
						break;
				}
			}
			break;
	}
	
	return evHandled;
}


static void Share_HitButton(DialogPtr theDlg,short item)
{
	short iType;
	Handle iHndl;
	Rect iRect;
	long fTicks;
	
	GetDItem(theDlg,item,&iType,&iHndl,&iRect);
	HiliteControl((ControlHandle)iHndl,inButton);
	Delay(5,&fTicks);
	HiliteControl((ControlHandle)iHndl,0);
}


static pascal Boolean Share_FilterAllFiles(CInfoPBPtr pb, Ptr myDataPtr)
{
	if (pb->hFileInfo.ioFlAttrib & (1<<4))	/* file is a directory */
		return false;

	return true;
}


static void Share_SetSelectButtonName(StringPtr selName,Boolean hilited,DialogPtr theDlg)
{
	ControlHandle selectButton;
	short iType;
	Handle iHndl;
	Rect iRect;
	Str255 storeName,tempLenStr,tempSelName;
	short btnWidth;
	
	BlockMove(selName,tempSelName,selName[0]+1);
	GetDItem(theDlg,kSelectItem,&iType,&iHndl,&iRect);
	
	/* truncate select name to fit in button */
	
	btnWidth = iRect.right - iRect.left;
	BlockMove(gSelectString,tempLenStr,gSelectString[0]+1);
	p2cstr(tempLenStr);
	strcat((char *)tempLenStr," рс  ");
	c2pstr((char *)tempLenStr);
	btnWidth -= StringWidth(tempLenStr);
	TruncString(btnWidth,tempSelName,smTruncMiddle);
	
	BlockMove(gSelectString,storeName,gSelectString[0]+1);
	p2cstr(storeName);
	p2cstr(tempSelName);
	strcat((char *)storeName," р");
	strcat((char *)storeName,(char *)tempSelName);
	strcat((char *)storeName,"с");
	
	c2pstr((char *)storeName);
	c2pstr((char *)tempSelName);
	SetCTitle((ControlHandle)iHndl,storeName);
	
	SetDItem(theDlg,kSelectItem,iType,iHndl,&iRect);

	if (hilited)
		HiliteControl((ControlHandle)iHndl,0);
	else
		HiliteControl((ControlHandle)iHndl,255);		
}


static Boolean Share_SameFile(FSSpec *file1,FSSpec *file2)
{
	if (file1->vRefNum != file2->vRefNum)
		return false;
	if (file1->parID != file2->parID)
		return false;
	if (!EqualString(file1->name,file2->name,false,true))
		return false;
	
	return true;
}


static OSErr Share_GetDeskFolderSpec(FSSpec *fSpec,short vRefNum)
{
	DirInfo infoPB;
	OSErr err;
	
	if (!gHasFindFolder) {
		fSpec->vRefNum = -9999;
		return -1;
	}
	
	fSpec->name[0] = '\0';
	err = FindFolder(vRefNum,kDesktopFolderType,kDontCreateFolder,
						&fSpec->vRefNum,&fSpec->parID);
	if (err!=noErr)
		return err;
	
	return Share_MakeCanonFSSpec(fSpec);
}


static Boolean Share_ShouldHiliteSelect(FSSpec *fSpec)
{
	if (Share_SameFile(fSpec,&gDeskFolderSpec)) {
		BlockMove(gDesktopFName,fSpec->name,gDesktopFName[0]+1);
		return kCanSelectDesktop;
	}
	else
		return true;
}

static OSErr Share_MakeCanonFSSpec(FSSpec *fSpec)
{
	CInfoPBRec infoPB;
	OSErr err;

	if (fSpec->name[0] != '\0')
		return;
		
	infoPB.dirInfo.ioNamePtr = fSpec->name;
	infoPB.dirInfo.ioVRefNum = fSpec->vRefNum;
	infoPB.dirInfo.ioDrDirID = fSpec->parID;
	infoPB.dirInfo.ioFDirIndex = -1;
	err = PBGetCatInfo(&infoPB,false);
	fSpec->parID = infoPB.dirInfo.ioDrParID;
	
	return err;
}

static void Share_ResolvePath (FSSpec *fSpec, char *path)
{	char volname[128];
	char dirname[128];
	char path_copy[1024];
	short the_vRefNum;
	long  the_parID;
	CInfoPBRec infoPB;

	volname[0]=0;
	strcpy(dirname,(char *)fSpec->name);
	p2cstr((unsigned char *)dirname);
	
	strcpy(path,dirname);
	
	the_vRefNum=(fSpec->vRefNum);
	the_parID=(fSpec->parID);

	infoPB.dirInfo.ioNamePtr = (unsigned char *)volname;
	infoPB.dirInfo.ioVRefNum = the_vRefNum;
	infoPB.dirInfo.ioDrDirID = fsRtDirID;
	infoPB.dirInfo.ioFDirIndex = -1;
	PBGetCatInfo(&infoPB,false);

	p2cstr((unsigned char *)volname);

	while (strcmp(volname,dirname) != 0)
	{		
		infoPB.dirInfo.ioNamePtr = (unsigned char *)dirname;
		infoPB.dirInfo.ioVRefNum = the_vRefNum;
		infoPB.dirInfo.ioDrDirID = the_parID;
		infoPB.dirInfo.ioFDirIndex = -1;
		PBGetCatInfo(&infoPB,false);
		the_parID = infoPB.dirInfo.ioDrParID;
	
		p2cstr((unsigned char *)dirname);
		strcpy(path_copy,dirname);
		strcat(path_copy,":");
		strcat(path_copy,path);
		strcpy(path,path_copy);
	}
}

Boolean ReadPrefs (char *share_path)
{	OSErr err=noErr;
	short prefsFolder;
	Str255 prefsName="\pUAE Preferences";
	long prefDirID;
	short prefvRefNum;
	FSSpec spec;
	short home,preffile;
	Handle share_handle;
	Handle config_handle;
	int config_sets[128],count;

	home = CurResFile();
	preffile=0;
	*share_path=0;
	
	err=FindFolder(kOnSystemDisk, kPreferencesFolderType, true, &prefvRefNum, &prefDirID);
	if (err != noErr) return false;
	FSMakeFSSpec(prefvRefNum, prefDirID, prefsName, &spec);

	preffile=FSpOpenResFile(&spec, fsRdWrPerm);
	if (ResError() != noErr) return false;

	UseResFile(preffile);

	if (share_path != 0)
	{	share_handle = Get1Resource('STR ',128);
		if (ResError() == noErr && share_handle !=0 && *share_handle !=0)
		{	strcpy(share_path,*share_handle);
			ReleaseResource(share_handle);
		}
	}

	config_handle = Get1Resource('PREF',128);
	if (ResError() == noErr && config_handle !=0)
	{	BlockMove((Ptr) *config_handle,(Ptr) config_sets,128);
		ReleaseResource(config_handle);
		
		framerate=config_sets[0];
		use_slow_mem=config_sets[1];
		use_gfxlib=config_sets[2];
		automount_uaedev=config_sets[3];
		produce_sound=config_sets[4];
		fake_joystick=config_sets[5];
		screen_res=config_sets[6];
		use_quickdraw=config_sets[7];
		filesys_vRefNum=config_sets[8];
	}

	UseResFile(home);
	CloseResFile(preffile);
	return true;
}

void CreatePrefs (void)
{	OSErr err=noErr;
	short prefsFolder;
	Str255 prefsName="\pUAE Preferences";
	long prefDirID;
	short prefvRefNum;
	FSSpec spec;
	short home,preffile;
	
	home = CurResFile();
	preffile=0;
	
	err=FindFolder(kOnSystemDisk, kPreferencesFolderType, true, &prefvRefNum, &prefDirID);
	if (err != noErr) return;
	FSMakeFSSpec(prefvRefNum, prefDirID, prefsName, &spec);
	
	err=FSpCreate(&spec, 'mUAE', 'pref', 0);
	if (err != noErr) return;
	
	FSpCreateResFile(&spec, 'mUAE', 'pref', 0);
}

void WritePrefs (char *share_path)
{	OSErr err=noErr;
	short prefsFolder;
	Str255 prefsName="\pUAE Preferences";
	long prefDirID;
	short prefvRefNum;
	FSSpec spec;
	short home,preffile;
	Handle share_handle;
	Handle config_handle;
	int config_sets[128],count;
	
	home = CurResFile();
	preffile=0;
	
	err=FindFolder(kOnSystemDisk, kPreferencesFolderType, true, &prefvRefNum, &prefDirID);
	if (err != noErr) return;
	FSMakeFSSpec(prefvRefNum, prefDirID, prefsName, &spec);
	
	preffile=FSpOpenResFile(&spec, fsRdWrPerm);
	if (ResError() != noErr) return;
	
	for (count=0;count < 64;count++) config_sets[count]=0;
	
	config_sets[0]=framerate;
	config_sets[1]=use_slow_mem;
	config_sets[2]=my_use_gfxlib;
	config_sets[3]=my_automount_uaedev;
	config_sets[4]=produce_sound;
	config_sets[5]=fake_joystick;
	config_sets[6]=my_screen_res;
	config_sets[7]=my_use_quickdraw;
	config_sets[8]=filesys_vRefNum;
	
	UseResFile(preffile);
	
	
	if (share_path != 0)
	{	share_handle=Get1Resource('STR ',128);
		if (ResError() != noErr || share_handle == 0)
		{	share_handle=NewHandleClear(1024);
			strcpy(*share_handle,share_path);
			AddResource(share_handle,'STR ',128, "\pMac/Amiga sharing path");
			WriteResource(share_handle);
			DetachResource(share_handle);
		}
		else
		{	strcpy(*share_handle,share_path);
			ChangedResource(share_handle);
			WriteResource(share_handle);
			ReleaseResource(share_handle);
		}
	}
	
	config_handle=Get1Resource('PREF',128);
	if (ResError() != noErr || config_handle == 0)
	{	config_handle=NewHandleClear(1024);
		BlockMove((Ptr) config_sets,(Ptr) *config_handle,128);
		AddResource(config_handle,'PREF',128, "\pUAE Preferences");
		WriteResource(config_handle);
		DetachResource(config_handle);
	}
	else
	{	BlockMove((Ptr) config_sets,(Ptr) *config_handle,128);
		ChangedResource(config_handle);
		WriteResource(config_handle);
		ReleaseResource(config_handle);
	}
	
	UseResFile(home);
	CloseResFile(preffile);
}

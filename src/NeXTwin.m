 /*
  * UAE - The Un*x Amiga Emulator
  *
  * NeXT interface
  * NeXTwin.m
  *
  * Copyright 1995, 1996 Bernd Schmidt
  * Copyright 1996 Ed Hanway, Andre Beck
  * Copyright 1996-97 Ian Stephenson
  */

#include "sysconfig.h"
#include "sysdeps.h"

#import <appkit/appkit.h>

#include "config.h"
#include "options.h"
#include "threaddep/penguin.h"
#include "uae.h"
#include "memory.h"
#include "custom.h"
#include "readcpu.h"
#include "newcpu.h"
#include "xwin.h"
#include "keyboard.h"
#include "keybuf.h"
#include "gui.h"
#include "debug.h"
// If you are compiling on NeXTStep 3.2, uncomment the following line:
//#define NX_EightBitRGBDepth 514

int sigbrkhandler()
{
    activate_debugger();
    signal(SIGINT, sigbrkhandler);
}

void setup_brkhandler(void)
{
    signal(SIGINT, sigbrkhandler);
}


/* Keyboard and mouse */

static BOOL keystate[256];
int commandKey = -1;

int quit_program;

static NXCursor *cursor;

static View *screen;
static NXBitmapImageRep *bitmap;
static char * xlinebuffer;
static int bitOffset;
static int keycode2amiga(NXEvent * theEvent);

@interface AmigaView:View
{
}
//From Menu...
- reset:sender;
- quit:sender;
- joystick:sender;

//Floppy Stuff...
- eject:sender;
- insert:sender;

//Misc...
- (BOOL)acceptsFirstResponder;
- resetCursorRects;

//The ones which do the work...
- keyDown:(NXEvent *)theEvent;
- keyUp:(NXEvent *)theEvent;
- flagsChanged:(NXEvent *)theEvent;
- mouseDown:(NXEvent *)theEvent;
- mouseUp:(NXEvent *)theEvent;
- rightMouseDown:(NXEvent *)theEvent;
- rightMouseUp:(NXEvent *)theEvent;
@end

@implementation AmigaView
-reset:sender
	{
	uae_reset();
	//m68k_reset();
	return self;
	}
-quit:sender
	{
	uae_quit();
	return self;
	}
-joystick:sender
	{
	currprefs.fake_joystick=[sender state];
	return self;
	}
-eject:sender
	{
	disk_eject([sender tag]);
	return self;
	}
-insert:sender
	{
	disk_eject([sender tag]);
	disk_insert([sender tag],[sender stringValue]);
	return self;
	}
	
- (BOOL)acceptsFirstResponder
	{
	return YES;
	}
- keyDown:(NXEvent *)theEvent
	{
	if(theEvent->data.key.repeat == 0)
		{
		int kc = keycode2amiga((NXEvent *)theEvent);
		if (!keystate[kc])
			{
		     keystate[kc] = TRUE;
		     record_key (kc << 1);
			 }
		}
	return self;
	}
- keyUp:(NXEvent *)theEvent
	{
	int kc = keycode2amiga((NXEvent *)theEvent);
	if (kc == -1) return;
	keystate[kc] = FALSE;
	record_key ((kc << 1) | 1);
	
	return self;
	}
-flagsChanged:(NXEvent *)theEvent
	{
	if(theEvent->flags & NX_SHIFTMASK)
		{//Shift is Down
		if(!keystate[AK_LSH])
			{
			keystate[AK_LSH] = TRUE;
			record_key ((AK_LSH << 1));
			}
		if(!keystate[AK_RSH])
			{
			keystate[AK_RSH] = TRUE;
			record_key ((AK_RSH << 1));
			}
		}
	else
		{//Shift is Up
		if(keystate[AK_LSH])
			{
			keystate[AK_LSH] = FALSE;
			record_key ((AK_LSH << 1) | 1);
			}
		if(keystate[AK_RSH])
			{
			keystate[AK_RSH] = FALSE;
			record_key ((AK_RSH << 1) | 1);
			}
		}

	if(theEvent->flags & NX_CONTROLMASK)
		{
		if(!keystate[AK_CTRL])
			{
			keystate[AK_CTRL] = TRUE;
			record_key ((AK_CTRL << 1));
			}
		}
	else
		if(keystate[AK_CTRL])
			{
			keystate[AK_CTRL] = FALSE;
			record_key ((AK_CTRL << 1) | 1);
			}
		
	if(theEvent->flags & NX_ALTERNATEMASK)
		{//Alt is Down
		if(!keystate[AK_LALT])
			{
			keystate[AK_LALT] = TRUE;
			record_key ((AK_LALT << 1));
			}
		if(!keystate[AK_RALT])
			{
			keystate[AK_RALT] = TRUE;
			record_key ((AK_RALT << 1));
			}
		}
	else
		{//Alt is Up
		if(keystate[AK_LALT])
			{
			keystate[AK_LALT] = FALSE;
			record_key ((AK_LALT << 1) | 1);
			}
		if(keystate[AK_RALT])
			{
			keystate[AK_RALT] = FALSE;
			record_key ((AK_RALT << 1) | 1);
			}

		}
	return self;	
	}
- mouseDown:(NXEvent *)theEvent
	{
	buttonstate[0] = 1;
	return self;
	}
- mouseUp:(NXEvent *)theEvent
	{
	buttonstate[0] = 0;
	return self;
	}
- rightMouseDown:(NXEvent *)theEvent
	{
	buttonstate[2] = 1;
	return self;
	}
- rightMouseUp:(NXEvent *)theEvent
	{
	buttonstate[2] = 0;
	return self;
	}

- resetCursorRects
{
   NXRect visible;



   if ([self getVisibleRect:&visible])
	      [self addCursorRect:&visible cursor:cursor];
   return self;
}
@end
// End of AmigaView Object - common functions now!!!


void flush_block (int ystart, int ystop)
{
	id tmpBitmap;
	NXPoint where;
	
	//printf("Flush Block:%d->%d\n",ystart,ystop);
	if(ystart >= ystop)
		return;
	
	ystop++;	//Make sure we get the last line!
	
	tmpBitmap=[[NXBitmapImageRep alloc]
		initData:[bitmap data]+ystart*[bitmap bytesPerRow]
		pixelsWide:(int)800
		pixelsHigh:(int)(ystop-ystart)
		bitsPerSample:[bitmap bitsPerSample]
		samplesPerPixel:[bitmap samplesPerPixel]
		hasAlpha:(BOOL)[bitmap hasAlpha]
		isPlanar:(BOOL)NO
		colorSpace:[bitmap colorSpace]
		bytesPerRow:[bitmap bytesPerRow]
		bitsPerPixel:[bitmap bitsPerPixel]
		];
	
	where.x=0;
	where.y=283-ystop;
	[screen lockFocus];
		[tmpBitmap drawAt:&where];
	[screen unlockFocus];
	[screen display];
	[tmpBitmap free];
}

void flush_screen (int from, int to)
{
	//printf("Flush Screen:%d->%d\n", from, to);
	return;
	//This simple version is no longer required...
	[screen lockFocus];
		[bitmap draw];
	[screen unlockFocus];
	[screen display];
}

void flush_line(int y)
{	
	//printf("Flush Line:%d\n", y);
	return;
}

int graphics_setup(void)
{
    return 1;
}

int graphics_init(void)
{
    int i;
    NXRect rect;
		
    quit_program = NO;
    currprefs.fake_joystick = NO;

    gfxvidinfo.can_double = 0;

    [Application new];
    if (![NXApp loadNibSection:"Uae.nib" owner:NXApp withNames:NO])
		{
		puts("Can't find NIB file");
		exit(-1);
		}
	[NXApp perform:@selector(stop:) with:nil afterDelay:0.0 cancelPrevious:NO];
	[NXApp run];
	
	screen=[NXApp delegate];
	[NXApp setDelegate:nil];

	[[screen window] addToEventMask:NX_RMOUSEDOWNMASK|NX_RMOUSEUPMASK];
	
	cursor=[[NXCursor alloc] initFromImage:[NXImage findImageNamed:"dummy"]];
	
	switch([Window defaultDepthLimit])
		{
		case NX_TwentyFourBitRGBDepth:
			{
			for(i = 0; i < 4096; i++)
				{
				xcolors[i]=   NXSwapHostLongToBig(((i & 0x0f00) << (20))|
						((i & 0x00f0) << (16))|
						((i & 0x000f) << (12))|
						0xff);
				}
				
				bitmap=[[NXBitmapImageRep alloc]
					initData:(unsigned char *)NULL
					pixelsWide:(int)800
					pixelsHigh:(int)(313-29)
					bitsPerSample:(int)8
					samplesPerPixel:(int)4
					hasAlpha:(BOOL)YES
					isPlanar:(BOOL)NO
					colorSpace:(NXColorSpace)NX_RGBColorSpace
					bytesPerRow:(int)800*4
					bitsPerPixel:(int)32
					];
				gfxvidinfo.pixbytes=4;
			break;
			}
		case NX_TwelveBitRGBDepth:
		case NX_EightBitRGBDepth:
			{
			for(i = 0; i < 4096; i++)
				{
				xcolors[i] = NXSwapHostShortToBig((short)((i << 4) | 0xf));
				}
				
				bitmap=[[NXBitmapImageRep alloc]
					initData:(unsigned char *)NULL
					pixelsWide:(int)800
					pixelsHigh:(int)(313-29)
					bitsPerSample:(int)4
					samplesPerPixel:(int)4
					hasAlpha:(BOOL)YES
					isPlanar:(BOOL)NO					 
					colorSpace:(NXColorSpace)NX_RGBColorSpace
					bytesPerRow:(int)800*2
					bitsPerPixel:(int)16
					];
				gfxvidinfo.pixbytes=2;
			break;
			}
		case NX_EightBitGrayDepth:
		case NX_TwoBitGrayDepth:
		default:
		{
			for(i = 0; i < 4096; i++)
				{
				xcolors[i]=  ( ((i & 0x0f00) >> 5)+
						((i & 0x00f0) >>1 )+
						((i & 0x000f) <<2)) ;
						
				if(xcolors[i]>255)
					xcolors[i]=255;
				}
				
				bitmap=[[NXBitmapImageRep alloc]
					initData:(unsigned char *)NULL
					pixelsWide:(int)800
					pixelsHigh:(int)(313-29)
					bitsPerSample:(int)8
					samplesPerPixel:(int)1
					hasAlpha:(BOOL)NO
					isPlanar:(BOOL)NO
					colorSpace:(NXColorSpace)NX_OneIsWhiteColorSpace
					bytesPerRow:(int)800
					bitsPerPixel:(int)8
					];
				gfxvidinfo.pixbytes=1;
				break;
			}
#if 0
	//Sorry 2bit screeners (self included!)
	//Bernd won't support special code just for us NeXTStation owners
	//and I don't have time any more,
	//So we have to use 8bit, and dither :-(
	//On the bright side, it looks great (eventually!)
		case NX_TwoBitGrayDepth:
		{
			for(i = 0; i < 4096; i++)
				{
				xcolors[i]=   (((i & 0x0f00) >> (1+8))+
						((i & 0x00f0) >> (1+4))+
						((i & 0x000f) >> (2+0))) >> 2;
						
				if(xcolors[i]>3)
					xcolors[i]=3;
					
				}
				
				bitmap=[[NXBitmapImageRep alloc]
					initData:(unsigned char *)NULL
					pixelsWide:(int)800
					pixelsHigh:(int)(313-29)
					bitsPerSample:(int)2
					samplesPerPixel:(int)1
					hasAlpha:(BOOL)NO
					isPlanar:(BOOL)NO
					colorSpace:(NXColorSpace)NX_OneIsWhiteColorSpace
					bytesPerRow:(int)800/4
					bitsPerPixel:(int)2
					];
				gfxvidinfo.pixbytes=0;	//bit of a hack!...
			}
#endif
		}
		gfxvidinfo.bufmem=[bitmap data];
		gfxvidinfo.linemem = NULL;
		gfxvidinfo.rowbytes=[bitmap bytesPerRow];
		gfxvidinfo.width = 800;
		gfxvidinfo.height = (313-29); /* ??? */
		gfxvidinfo.maxblocklines = (313-29); /* ??? */

		gfxvidinfo.can_double = FALSE;

		newmousecounters = 0;
		return 1;
}



void graphics_leave(void)
{
    //[bitmap free];
    //[NXApp free];
}


static int keycode2amiga(NXEvent * theEvent)
{

		if((theEvent->flags & NX_COMMANDMASK))
		{
			switch ((char)(theEvent->data.key.charCode))  
			{
				case '1': commandKey = AK_F1; return AK_F1;
				case '2': commandKey = AK_F2; return AK_F2;
				case '3': commandKey = AK_F3; return AK_F3;
				case '4': commandKey = AK_F4; return AK_F4;
				case '5': commandKey = AK_F5; return AK_F5;
				case '6': commandKey = AK_F6; return AK_F6;
				case '7': commandKey = AK_F7; return AK_F7;
				case '8': commandKey = AK_F8; return AK_F8;
				case '9': commandKey = AK_F9; return AK_F9;
				case '0': commandKey = AK_F10; return AK_F10;
				default : return -1; //So not to generate stuck key.
			}
		}

	if ( theEvent->flags & NX_NUMERICPADMASK )
		{

		switch ((char)(theEvent->data.key.charCode)) {
			case '0': return AK_NP0;
			case '1': return currprefs.fake_joystick?AK_LAMI:AK_NP1;
			case '2': return AK_NP2;
			case '3': return currprefs.fake_joystick?AK_RAMI:AK_NP3;
			case '4': return AK_NP4;
			case '5': return AK_NP5;
			case '6': return AK_NP6;
			case '7': return AK_NP7;
			case '8': return AK_NP8;
			case '9': return AK_NP9;
			}
		}

    switch ((char)(theEvent->data.key.charCode)) {	
     case 'a': case 'A': return AK_A;
     case 'B': case 'b': return AK_B;
     case 'C': case 'c': return AK_C;
     case 'D': case 'd': return AK_D;
     case 'E': case 'e': return AK_E;
     case 'F': case 'f': return AK_F;
     case 'G': case 'g': return AK_G;
     case 'H': case 'h': return AK_H;
     case 'I': case 'i': return AK_I;
     case 'J': case 'j': return AK_J;
     case 'K': case 'k': return AK_K;
     case 'L': case 'l': return AK_L;
     case 'M': case 'm': return AK_M;
     case 'N': case 'n': return AK_N;
     case 'O': case 'o': return AK_O;
     case 'P': case 'p': return AK_P;
     case 'Q': case 'q': return AK_Q;
     case 'R': case 'r': return AK_R;
     case 'S': case 's': return AK_S;
     case 'T': case 't': return AK_T;
     case 'U': case 'u': return AK_U;
     case 'V': case 'v': return AK_V;
     case 'W': case 'w': return AK_W;
     case 'X': case 'x': return AK_X;
     case 'Y': case 'y': return AK_Y;
     case 'Z': case 'z': return AK_Z;
	
     case '0':case ')': return AK_0;
     case '1':case '!': return AK_1;
     case '2':case '@': return AK_2;
     case '3':case '#': return AK_3;
     case '4':case '$': return AK_4;
     case '5':case '%': return AK_5;
     case '6':case '^': return AK_6;
     case '7':case '&': return AK_7;
     case '8':case '*': return AK_8;
     case '9':case '(': return AK_9;
	


	 case ';': case ':': return AK_SEMICOLON;
	 case '-': case '_': return AK_MINUS;
	 case '/': case '?': return AK_SLASH;
	 case '.': case '>': return AK_PERIOD;
	 case ',': case '<': return AK_COMMA;
	 case '=': case '+': return AK_EQUAL;
	 case '[': case '{': return AK_LBRACKET;
	 case ']': case '}': return AK_RBRACKET;
	


     case 127: return AK_BS;
     case 9: return AK_TAB;
     case 13: return AK_RET;
     case 32: return AK_SPC;
     case 27: return AK_ESC;

     case -83: return AK_UP;
     case -81: return AK_DN;
     case -84: return AK_LF;
     case -82: return AK_RT;
	
     case '\\': return AK_BACKSLASH;
    }
    return -1;
}

void handle_events(void)
{
	NXEvent	dummy;				// used for throwaway checks
	NXPoint mouseLoc;
	
	//Update Mouse Position
	[[screen window] getMouseLocation:&mouseLoc];
	[screen convertPoint:&mouseLoc fromView:nil];

	lastmx=mouseLoc.x;
	lastmy=(313-29)-mouseLoc.y;

	//COMMAND'd keypresses do not generate key ups!
	//We therefore have to fake the key up...
	if(commandKey != -1)
		{
		keystate[commandKey]=FALSE;
		record_key ((commandKey << 1) | 1);
		commandKey = -1;
		}

	//Check for NeXT events, and run NXApp...
	if([NXApp peekNextEvent: NX_ALLEVENTS into: &dummy])
		{
		[NXApp perform:@selector(stop:) with:nil afterDelay:0.0 cancelPrevious:NO];
		[NXApp run];
		}
}

int debuggable(void)
{
    return TRUE;
}

int needmousehack(void)
{
    return TRUE;
}

void LED(int on)
{
}


//Keep X gui happy!

void gui_changesettings(void)
{

}
int gui_update(void)
{
    return 0;
}

int gui_init(void)
{
}

void gui_exit(void)
{
}

void gui_led(int led, int on)
{
}

void gui_filename(int num, const char *name)
{
}

void write_log (const char *buf)
{
    fprintf (stderr, buf);
}

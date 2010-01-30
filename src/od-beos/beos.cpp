/* 
 *  UAE - The Un*x Amiga Emulator
 *
 *  BeBox port specific stuff
 *
 *  (c) 1996-1998 Christian Bauer
 *  (c) 1996 Patrick Hanevold
 */

#include <AppKit.h>
#include <InterfaceKit.h>
#include <KernelKit.h>
#include <MediaKit.h>
#include <StorageKit.h>
#include <device/Joystick.h>

extern "C" {
#include "sysconfig.h"
#include "sysdeps.h"
#include "config.h"
#include "options.h"
#include "threaddep/penguin.h"
#include "uae.h"
#include "gensound.h"
#include "sounddep/sound.h"
#include "events.h"
#include "memory.h"
#include "custom.h"
#include "readcpu.h"
#include "newcpu.h"
#include "disk.h"
#include "gui.h"
#include "debug.h"
#include "xwin.h"
#include "joystick.h"
#include "keyboard.h"
#include "keybuf.h"
#include "gui.h"
#include "zfile.h"
#include "picasso96.h"
}


// Constants
const char APP_SIGNATURE[] = "application/x-vnd.cebix-UAE";

const uint32 MSG_INSERT_DF0 = 'idf0';	// Messages
const uint32 MSG_EJECT_DF0 = 'edf0';
const uint32 MSG_INSERT_DF1 = 'idf1';
const uint32 MSG_EJECT_DF1 = 'edf1';
const uint32 MSG_INSERT_DF2 = 'idf2';
const uint32 MSG_EJECT_DF2 = 'edf2';
const uint32 MSG_INSERT_DF3 = 'idf3';
const uint32 MSG_EJECT_DF3 = 'edf3';
const uint32 MSG_FLOPPY_PANEL_RETURNED = 'flpn';
const uint32 MSG_FR1 = 'mfr1';
const uint32 MSG_FR2 = 'mfr2';
const uint32 MSG_FR3 = 'mfr3';
const uint32 MSG_FR4 = 'mfr4';
const uint32 MSG_FR5 = 'mfr5';
const uint32 MSG_FR6 = 'mfr6';
const uint32 MSG_RESET = 'rset';
const uint32 MSG_DEBUG = 'dbug';
const uint32 MSG_REDRAW = 'draw';
const uint32 MSG_QUIT = 'quit';
const uint32 MSG_ABOUT = 'abut';

const rgb_color PowerDark = {0x40, 0x00, 0x00, 0};	// LED colors
const rgb_color PowerLight = {0xff, 0x00, 0x00, 0};
const rgb_color DriveDark = {0x00, 0x40, 0x00, 0};
const rgb_color DriveLight = {0x00, 0xff, 0x00, 0};


// Array for converting Be keycodes to Amiga keycodes
static const int keycode2amiga[128] = {
	-1, AK_ESC, AK_F1, AK_F2, AK_F3, AK_F4, AK_F5, AK_F6,
	AK_F7, AK_F8, AK_F9, AK_F10, -1, AK_mousestuff, -1, -1,

	-1, AK_BACKQUOTE, AK_1, AK_2, AK_3, AK_4, AK_5, AK_6,
	AK_7, AK_8, AK_9, AK_0, AK_MINUS, AK_EQUAL, AK_BS, AK_HELP,

	AK_NPLPAREN, AK_NPRPAREN, -1, AK_NPDIV, AK_NPMUL, AK_NPSUB, AK_TAB, AK_Q,
	AK_W, AK_E, AK_R, AK_T, AK_Y, AK_U, AK_I, AK_O,

	AK_P, AK_LBRACKET, AK_RBRACKET, AK_BACKSLASH, AK_DEL, AK_LALT, AK_RALT, AK_NP7,
	AK_NP8, AK_NP9, AK_NPADD, AK_CAPSLOCK, AK_A, AK_S, AK_D, AK_F,

	AK_G, AK_H, AK_J, AK_K, AK_L, AK_SEMICOLON, AK_QUOTE, AK_RET,
	AK_NP4, AK_NP5, AK_NP6, AK_LSH, AK_Z, AK_X, AK_C, AK_V,

	AK_B, AK_N, AK_M, AK_COMMA, AK_PERIOD, AK_SLASH, AK_RSH, AK_UP,
	AK_NP1, AK_NP2, AK_NP3, AK_ENT, AK_CTRL, AK_LAMI, AK_SPC, AK_RAMI,

	AK_RALT, AK_LF, AK_DN, AK_RT, AK_NP0, AK_NPDEL, AK_LALT, AK_RALT,
	AK_LTGT, -1, -1, -1, -1, -1, -1, -1,

	-1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1
};


// Global variables
static bool reset_thyself = false;

static key_info old_key_info;
static bool window_open;
static bool LEDs[4];

static int the_argc;
static char **the_argv;

static BJoystick *joy;


/*
 *  A simple view class for blitting a bitmap on the screen
 */

class BitmapView : public BView {
public:
	BitmapView(BRect frame, BBitmap *bitmap);
	virtual void Draw(BRect update);
	virtual void MouseMoved(BPoint point, uint32 transit, const BMessage *message);
	void Draw(BRect from, BRect to);

private:
	BBitmap *the_bitmap;
};

static BitmapView *bitmap_view;


/*
 *  The window in which the Amiga graphics are displayed, handles I/O
 */

class UAEWindow : public BWindow {
public:
	UAEWindow(BRect frame, BBitmap *bitmap);
	virtual bool QuitRequested(void);
	virtual void MessageReceived(BMessage *msg);

private:
	BMessenger this_messenger;
	BitmapView *main_view;
	BFilePanel *floppy_panel;
	int panel_drive_num;
};

static UAEWindow *bitmap_window;


/*
 *  The BeOS application object
 */

class UAE : public BApplication {
public:
	UAE();
	virtual void ReadyToRun(void);
	virtual bool QuitRequested(void);

	int GraphicsInit(void);
	void GraphicsLeave(void);

private:
	static long thread_func(void *obj);

	BBitmap *the_bitmap;
	UAEWindow *main_window;
	thread_id the_thread;
};

static UAE *the_app;


/*
 *	LED
 */

class LEDView : public BView {
public:
	LEDView(BRect frame, rgb_color active, rgb_color idle);
	virtual void Draw(BRect update);
	void SetState(bool new_state);

private:
	BRect bounds;
	rgb_color active_color;
	rgb_color idle_color;
	bool state;
};

static LEDView *PowerLED, *DriveLED[4];


/*
 *  Create application object and start it
 */

int main(int argc, char **argv)
{
	// Copy command line arguments
	the_argc = argc;
	the_argv = new char *[argc];
	for (int i=0; i<argc; i++)
		the_argv[i] = strdup(argv[i]);

	the_app = new UAE();
	the_app->Run();
	delete the_app;
	return 0;
}


/*
 *  UAE Constructor: Initialize member variables
 */

UAE::UAE() : BApplication('UAEm')
{
	// Find application directory and cwd to it
	app_info the_info;
	GetAppInfo(&the_info);
	BEntry the_file(&the_info.ref);
	BEntry the_dir;
	the_file.GetParent(&the_dir);
	BPath the_path;
	the_dir.GetPath(&the_path);
	chdir(the_path.Path());

	// Initialize other variables
	the_bitmap = NULL;
	main_window = NULL;
	window_open = false;
}


/*
 *  Arguments processed, create and start emulation
 */

void UAE::ReadyToRun(void)
{
	// Start the emulation thread
	the_thread = spawn_thread(thread_func, "UAE 68000", B_NORMAL_PRIORITY, this);
	resume_thread(the_thread);
}


/*
 *  Quit requested (either by menu or by closing the window)
 */

bool UAE::QuitRequested(void)
{
	// Quit the thread
	uae_quit();
	status_t l;
	wait_for_thread(the_thread, &l);
	return true;
}


/*
 *  The thread's main function
 */

long UAE::thread_func(void *obj)
{
	real_main(the_argc, the_argv);
	return 0;
}


/*
 *  UAE Window constructor
 */

UAEWindow::UAEWindow(BRect frame, BBitmap *bitmap) : BWindow(frame,"UAE", B_TITLED_WINDOW, B_NOT_RESIZABLE|B_NOT_ZOOMABLE), this_messenger(this)
{
	int i;

	// Initialize xcolors
	{
		BScreen scr(this);
		i = 0;
		for (int r=0; r<16; r++) {
			for (int g=0; g<16; g++) {
				for (int b=0; b<16; b++)
					xcolors[i++] = scr.IndexForColor(r<<4 | r, g<<4 | g, b<<4 | b);
			}
		}
	}

	// Move window to right position
	Lock();
	MoveTo(80, 80);

	// Set up menus
	BRect bounds = Bounds();
	bounds.OffsetBy(0, bounds.IntegerHeight() + 1);
	BMenuBar *bar = new BMenuBar(bounds, "menu");
	BMenu *menu = new BMenu("UAE");
	menu->AddItem(new BMenuItem("About UAE" B_UTF8_ELLIPSIS, new BMessage(MSG_ABOUT)));
	menu->AddItem(new BSeparatorItem);
	BMenu *submenu = new BMenu("Framerate");
	submenu->AddItem(new BMenuItem("1/1", new BMessage(MSG_FR1)));
	submenu->AddItem(new BMenuItem("1/2", new BMessage(MSG_FR2)));
	submenu->AddItem(new BMenuItem("1/3", new BMessage(MSG_FR3)));
	submenu->AddItem(new BMenuItem("1/4", new BMessage(MSG_FR4)));
	submenu->AddItem(new BMenuItem("1/5", new BMessage(MSG_FR5)));
	submenu->AddItem(new BMenuItem("1/6", new BMessage(MSG_FR6)));
	submenu->SetRadioMode(true);
	menu->AddItem(submenu);
	menu->AddItem(new BMenuItem("Reset", new BMessage(MSG_RESET)));
	menu->AddItem(new BMenuItem("Debugger", new BMessage(MSG_DEBUG)));
	menu->AddItem(new BSeparatorItem);
	menu->AddItem(new BMenuItem("Quit", new BMessage(MSG_QUIT)));
	bar->AddItem(menu);
	menu = new BMenu("Floppy");
	submenu = new BMenu("DF0");
	submenu->AddItem(new BMenuItem("Insert" B_UTF8_ELLIPSIS, new BMessage(MSG_INSERT_DF0)));
	submenu->AddItem(new BMenuItem("Eject", new BMessage(MSG_EJECT_DF0)));
	menu->AddItem(submenu);
	submenu = new BMenu("DF1");
	submenu->AddItem(new BMenuItem("Insert" B_UTF8_ELLIPSIS, new BMessage(MSG_INSERT_DF1)));
	submenu->AddItem(new BMenuItem("Eject", new BMessage(MSG_EJECT_DF1)));
	menu->AddItem(submenu);
	submenu = new BMenu("DF2");
	submenu->AddItem(new BMenuItem("Insert" B_UTF8_ELLIPSIS, new BMessage(MSG_INSERT_DF2)));
	submenu->AddItem(new BMenuItem("Eject", new BMessage(MSG_EJECT_DF2)));
	menu->AddItem(submenu);
	submenu = new BMenu("DF3");
	submenu->AddItem(new BMenuItem("Insert" B_UTF8_ELLIPSIS, new BMessage(MSG_INSERT_DF3)));
	submenu->AddItem(new BMenuItem("Eject", new BMessage(MSG_EJECT_DF3)));
	menu->AddItem(submenu);
	bar->AddItem(menu);
	AddChild(bar);
	bar->ResizeBy(-5*4, 0);	// Make room for LEDs
	int mbar_height = bar->Frame().IntegerHeight() + 1;

	// Resize window to fit menu bar
	ResizeBy(0, mbar_height);

	// Create LEDs
	BRect PowerRect = bar->Frame();
	PowerRect.left = PowerRect.right+1;
	PowerRect.right = PowerRect.left+3;
	PowerLED = new LEDView(PowerRect, PowerLight, PowerDark);
	AddChild(PowerLED);

	BRect DriveRect = PowerRect;
	for (i=0; i<4; i++) {
		DriveRect.left += 4;
		DriveRect.right += 4;
		DriveLED[i] = new LEDView(DriveRect, DriveLight, DriveDark);
		AddChild(DriveLED[i]);
	}

	// Create bitmap view
	BRect BitmapRect = frame;
	main_view = new BitmapView(BitmapRect, bitmap);
	AddChild(main_view);
	main_view->MakeFocus();
	get_key_info(&old_key_info);

	bitmap_view = main_view;
	bitmap_window = this;

	// Create file panel
	floppy_panel = new BFilePanel(B_OPEN_PANEL, &this_messenger, NULL, 0, false, new BMessage(MSG_FLOPPY_PANEL_RETURNED));
	panel_drive_num = 0;

	// Show window
	Unlock();
	Show();
	window_open = true;
}


/*
 *  Message received
 */

void UAEWindow::MessageReceived(BMessage *msg)
{
	BMessage *msg2;

	switch(msg->what) {
		case MSG_INSERT_DF0:
			panel_drive_num = 0;
			floppy_panel->Hide();
			floppy_panel->Window()->SetTitle("UAE: Insert floppy in DF0:");
			floppy_panel->Show();
			break;

		case MSG_EJECT_DF0:
			disk_eject(0);
			break;

		case MSG_INSERT_DF1:
			panel_drive_num = 1;
			floppy_panel->Hide();
			floppy_panel->Window()->SetTitle("UAE: Insert floppy in DF1:");
			floppy_panel->Show();
			break;

		case MSG_EJECT_DF1:
			disk_eject(1);
			break;

		case MSG_INSERT_DF2:
			panel_drive_num = 2;
			floppy_panel->Hide();
			floppy_panel->Window()->SetTitle("UAE: Insert floppy in DF2:");
			floppy_panel->Show();
			break;

		case MSG_EJECT_DF2:
			disk_eject(2);
			break;

		case MSG_INSERT_DF3:
			panel_drive_num = 3;
			floppy_panel->Hide();
			floppy_panel->Window()->SetTitle("UAE: Insert floppy in DF3:");
			floppy_panel->Show();
			break;

		case MSG_EJECT_DF3:
			disk_eject(3);
			break;

		case MSG_FLOPPY_PANEL_RETURNED: {
			entry_ref the_ref;
			BEntry the_entry;
			if (msg->FindRef("refs", &the_ref) == B_NO_ERROR)
				if (the_entry.SetTo(&the_ref) == B_NO_ERROR) {
					BPath the_path;
					the_entry.GetPath(&the_path);
					disk_eject(panel_drive_num);
					disk_insert(panel_drive_num, the_path.Path());
				}
			break;
		}

		case MSG_FR1:
			currprefs.framerate = 1;
			break;
		case MSG_FR2:
			currprefs.framerate = 2;
			break;
		case MSG_FR3:
			currprefs.framerate = 3;
			break;
		case MSG_FR4:
			currprefs.framerate = 4;
			break;
		case MSG_FR5:
			currprefs.framerate = 5;
			break;
		case MSG_FR6:
			currprefs.framerate = 6;
			break;

		case MSG_RESET:
			reset_thyself = true;
			break;

		case MSG_DEBUG:
			activate_debugger();
			break;

		case MSG_QUIT:
			be_app->PostMessage(B_QUIT_REQUESTED);
			break;

		case MSG_ABOUT: {
			char str[256];
			sprintf(str, "     Un*x Amiga Emulator V%d.%d.%d\n"
				     	"          by Bernd Schmidt\n"
				     	"    BeBox port by Christian Bauer\n"
				     	"Additional porting by Patrick Hanevold", UAEMAJOR, UAEMINOR, UAEURSAMINOR);
			BAlert *the_alert = new BAlert("", str, "OK");
			the_alert->Go();
			break;
		}

		case MSG_REDRAW:
			MessageQueue()->Lock();
			while ((msg2 = MessageQueue()->FindMessage(MSG_REDRAW, 0)) != NULL) {
				MessageQueue()->RemoveMessage(msg2);
				delete msg2;
			}
			MessageQueue()->Unlock();
			Lock();
			main_view->Draw(BRect(0, 0, currprefs.gfx_width-1, currprefs.gfx_height-1));
			Unlock();
			break;

		default:
			BWindow::MessageReceived(msg);
			break;
	}
}


/*
 *  Closing the window quits UAE
 */

bool UAEWindow::QuitRequested(void)
{
	window_open = false;
	be_app->PostMessage(B_QUIT_REQUESTED);
	return false;
}


/*
 *  Bitmap view constructor
 */

BitmapView::BitmapView(BRect frame, BBitmap *bitmap) : BView(frame, "", B_FOLLOW_NONE, B_WILL_DRAW)
{
	the_bitmap = bitmap;
}


/*
 *  Blit the bitmap
 */

void BitmapView::Draw(BRect update)
{
	DrawBitmap(the_bitmap, update, update);
}

void BitmapView::Draw(BRect from, BRect to)
{
	DrawBitmap(the_bitmap, from, to);
}


/*
 *  Mouse moved
 */

void BitmapView::MouseMoved(BPoint point, uint32 transit, const BMessage *message)
{
	newmousecounters = 0;
	switch (transit) {
		case B_ENTERED_VIEW:
			newmousecounters = 1;
			lastmx = point.x;
			lastmy = point.y;
			break;
		case B_EXITED_VIEW:
			break;
		case B_INSIDE_VIEW:
			lastmx = point.x;
			lastmy = point.y;
			break;
	}
}


/*
 *  LED
 */

LEDView::LEDView(BRect frame, rgb_color active, rgb_color idle) : BView(frame, "", B_FOLLOW_NONE, B_WILL_DRAW)
{
	active_color = active;
	idle_color = idle;
	state = false;
	SetViewColor(idle_color);
	SetHighColor(idle_color);
	bounds = Bounds();
}

void LEDView::Draw(BRect update)
{
	FillRect(bounds);
}

void LEDView::SetState(bool new_state)
{
	if (new_state != state) {
		state = new_state;
		Window()->Lock();
		if (state) {
			SetViewColor(active_color);
			SetHighColor(active_color);
		} else {
			SetViewColor(idle_color);
			SetHighColor(idle_color);
		}
		Draw(bounds);
		Window()->Unlock();
	}
}


/*
 *  Redraw a line
 */

void flush_line(int y)
{
	if (window_open) {
		bitmap_window->Lock();
		bitmap_view->Draw(BRect(0, y, currprefs.gfx_width-1, y));
		bitmap_window->Unlock();
	}
}


/*
 *  Redraw a block
 */

void flush_block(int ystart, int ystop)
{
}


/*
 *  Redraw the screen
 */

void flush_screen(int ystart, int ystop)
{
	if (window_open)
		bitmap_window->PostMessage(new BMessage(MSG_REDRAW));
}


/*
 *  Init graphics
 */

int graphics_setup(void)
{
	return 1;
}

int graphics_init(void)
{
	return the_app->GraphicsInit();
}

int UAE::GraphicsInit(void)
{
	if (currprefs.color_mode > 5)
		fprintf(stderr, "Bad color mode selected. Using default.\n"), currprefs.color_mode = 0;

	if (currprefs.gfx_width < 320)
		currprefs.gfx_width = 320;
	if (currprefs.gfx_height < 200)
		currprefs.gfx_height = 200;
	if (currprefs.gfx_height > 300 && ! currprefs.gfx_linedbl)
		currprefs.gfx_height = 300;
	if (currprefs.gfx_height > 600)
		currprefs.gfx_height = 600;

	gfxvidinfo.width = currprefs.gfx_width;
	gfxvidinfo.height = currprefs.gfx_height;
	gfxvidinfo.can_double = 1;

	// Allocate bitmap
	the_bitmap = new BBitmap(BRect(0, 0, currprefs.gfx_width-1, currprefs.gfx_height-1), B_COLOR_8_BIT);

	// Set up vidinfo
	gfxvidinfo.pixbytes = 1;
	gfxvidinfo.rowbytes = the_bitmap->BytesPerRow();
	gfxvidinfo.bufmem = (char *)the_bitmap->Bits();
	gfxvidinfo.linemem = NULL;
	gfxvidinfo.maxblocklines = 100; /* whatever... */

	// Open window
	main_window = new UAEWindow(BRect(0, 0, currprefs.gfx_width-1, currprefs.gfx_height-1), the_bitmap);

	// Initialize mouse and keyboard variables
	buttonstate[0] = buttonstate[1] = buttonstate[2] = 0;
	lastmx = lastmy = 0;
	newmousecounters = 0;

	return true;
}


/*
 * Exit graphics
 */

void graphics_leave(void)
{
	the_app->GraphicsLeave();
}

void UAE::GraphicsLeave(void)
{
	// Deallocate bitmap
	delete the_bitmap;
}


/*
 *  Poll mouse and keyboard
 */

void handle_events(void)
{
	key_info the_key_info;
	int be_code, be_byte, be_bit, amiga_code;
	BPoint mouse_point;
	uint32 mouse_buttons;

	if (reset_thyself) {
		uae_reset();
		reset_thyself = false;
	}

	// Redraw drive LEDs
	for (int i=0; i<4; i++)
		DriveLED[i]->SetState(LEDs[i]);

	if (window_open && bitmap_window->IsActive()) {

		get_key_info(&the_key_info);
		bitmap_window->Lock();
		bitmap_view->GetMouse(&mouse_point, &mouse_buttons, false);
		bitmap_window->Unlock();

		// Keyboard
		if (memcmp(the_key_info.key_states, old_key_info.key_states, sizeof(the_key_info.key_states))) {
			for (be_code=0; be_code<0x80; be_code++) {
				be_byte = be_code >> 3;
				be_bit = 1 << (~be_code & 7);

				// Key state changed?
				if ((the_key_info.key_states[be_byte] & be_bit)
				     != (old_key_info.key_states[be_byte] & be_bit)) {

					amiga_code = keycode2amiga[be_code];
					if (the_key_info.key_states[be_byte] & be_bit) {

						// Key pressed
						if (amiga_code == AK_mousestuff)
							togglemouse();
						else
							record_key(amiga_code << 1);
					} else {

						// Key released
						record_key((amiga_code << 1) | 1);
					}
				}
			}		
			old_key_info = the_key_info;
		}

		// "Affengriff"
		if ((the_key_info.key_states[0x5c >> 3] & (1 << (~0x5c & 7)))
		 && (the_key_info.key_states[0x5d >> 3] & (1 << (~0x5d & 7)))
		 && (the_key_info.key_states[0x5f >> 3] & (1 << (~0x5f & 7))))
			m68k_reset();

		// Scroll lock toggles inhibit_frame
		if (inhibit_frame != 2)
			inhibit_frame = the_key_info.key_states[0x0f >> 3] & (1 << (~0x0f & 7));

		// Mouse buttons
		if (mouse_point.x >= 0 && mouse_point.y >= 0 && mouse_point.x < currprefs.gfx_width && mouse_point.y < currprefs.gfx_height) {
			buttonstate[0] = mouse_buttons & B_PRIMARY_MOUSE_BUTTON;
			buttonstate[1] = mouse_buttons & B_TERTIARY_MOUSE_BUTTON;
			buttonstate[2] = mouse_buttons & B_SECONDARY_MOUSE_BUTTON;
		}
	}
}


/*
 *  Joystick routines
 */

extern "C" {
void init_joystick(void);
void close_joystick(void);
void read_joystick(int nr, unsigned int *dir, int *button);
};

int nr_joysticks;

void read_joystick(int nr, unsigned int *dir, int *button)
{
	static int joy_minx = 32767, joy_maxx = 0,
			   joy_miny = 32767, joy_maxy = 0;
	int left = 0, right = 0, top = 0, bot = 0;

	*dir = 0;
	*button = 0;

	if (nr >= nr_joysticks)
		return;

	if (joy->Update() != B_ERROR) {
		if (joy->horizontal > joy_maxx)
			joy_maxx = joy->horizontal;
		if (joy->horizontal < joy_minx)
			joy_minx = joy->horizontal;
		if (joy->vertical > joy_maxy)
			joy_maxy = joy->vertical;
		if (joy->vertical < joy_miny)
			joy_miny = joy->vertical;

		if (joy_maxx-joy_minx < 100 || joy_maxy-joy_miny < 100)
			return;

		if (joy->horizontal < (joy_minx + (joy_maxx-joy_minx)/3))
			right = 1;
		else if (joy->horizontal > (joy_minx + 2*(joy_maxx-joy_minx)/3))
			left = 1;

		if (joy->vertical < (joy_miny + (joy_maxy-joy_miny)/3))
			bot = 1;
		else if (joy->vertical > (joy_miny + 2*(joy_maxy-joy_miny)/3))
			top = 1;

		if (left) top = !top;
		if (right) bot = !bot;
	    *dir = bot | (right << 1) | (top << 8) | (left << 9);
	    *button = !joy->button1;
	}
}

void init_joystick(void)
{
	joy = new BJoystick();
	joy->Open("joystick1");
	nr_joysticks = 1;
}

void close_joystick(void)
{
	joy->Close();
	delete joy;
}


/*
 *  Sound routines
 */

extern "C" {
extern int sound_available;
extern int smplcnt;
extern int init_sound(void);
extern int setup_sound(void);
extern void flush_sound_buffer(void);
extern void init_sound_table16(void);
extern void sample16_handler(void);
};

uae_u16 *sndbuffer;
uae_u16 *sndbufpt;
int sndbufsize;

static uint8 *buffers[2] = {NULL, NULL};
static int buf_num;
static BDACStream *the_stream;
static BSubscriber *the_sub;
static bool sound_ready = false;
static sem_id sound_sync_sem;

bool stream_func(void *arg, char *buf, size_t count, void *header);

int init_sound(void)
{
	sound_sync_sem = create_sem(0, "UAE Sound Sync Semaphore");
	the_stream = new BDACStream();
	the_sub = new BSubscriber("UAE DAC subscriber");

	if (!currprefs.produce_sound)
		return 0;
	sound_ready = the_sub->Subscribe(the_stream) == B_NO_ERROR;
	if (!sound_ready)
		return 0;

	sndbufsize = 44100 / 8 * 4;
	buffers[0] = new uint8[sndbufsize];
	buffers[1] = new uint8[sndbufsize];
	memset(buffers[0], 0, sndbufsize);
	memset(buffers[1], 0, sndbufsize);
	buf_num = 0;
	sndbufpt = sndbuffer = (uae_u16 *)buffers[buf_num];

	init_sound_table16();
	eventtab[ev_sample].handler = sample16_handler;
	scaled_sample_evtime = (unsigned long)maxhpos * maxvpos * vblank_hz * CYCLE_UNIT / 44100;
	scaled_sample_evtime_ok = 1;

	sound_available = 1;
	the_stream->SetSamplingRate(44100);
	the_stream->SetStreamBuffers(sndbufsize, 4);
	the_sub->EnterStream(NULL, true, NULL, stream_func, NULL, true);
	return 1;
}

int setup_sound(void)
{
	sound_available = 1;
	return 1;
}

void close_sound(void)
{
	if (sound_ready) {
		the_sub->ExitStream(true);
		the_stream->SetStreamBuffers(4096, 8);
		the_sub->Unsubscribe();
		sound_ready = false;
	}
	delete the_sub;
	delete the_stream;

	delete_sem(sound_sync_sem);

	delete buffers[0];
	delete buffers[1];
}

void flush_sound_buffer(void)
{
	if (sound_ready) {
		long l;
		get_sem_count(sound_sync_sem, &l);
		if (l > 0)
			acquire_sem_etc(sound_sync_sem, l+1, 0, 0);
		else
			acquire_sem(sound_sync_sem);
	}

	sndbufpt = sndbuffer = (uae_u16 *)buffers[buf_num];
	buf_num ^= 1;
}

bool stream_func(void *arg, char *buf, size_t count, void *header)
{
	memcpy(buf, buffers[buf_num], count);
	release_sem(sound_sync_sem);
	return true;
}


/*
 *  Misc routines
 */

int debuggable(void)
{
    return true;
}

int needmousehack(void)
{
    return true;
}

void LED(int on)
{
	PowerLED->SetState(!on);
}

void setup_brkhandler(void)
{
}

void gui_changesettings(void)
{
}

void gui_led(int led, int on)
{
	if (led > 0 && led < 5)
		LEDs[led-1] = on;
}

int gui_init(void)
{
	LEDs[0] = LEDs[1] = LEDs[2] = LEDs[3] = false;
	quit_program = 0;
	return 0;
}

void gui_exit(void)
{
}

int gui_update(void)
{
	return 0;
}

void gui_filename(int num, const char *name)
{
}

static void getline(char *p)
{
}

void gui_handle_events(void)
{
}

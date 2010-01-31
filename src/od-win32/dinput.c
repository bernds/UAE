/*
 * UAE - The Un*x Amiga Emulator
 *
 * Win32 DirectInput interface
 *
 * Copyright 2002 - 2003 Toni Wilen
 */

//#define DI_DEBUG
//#define DI_DEBUG2

#include "config.h"
#include "sysconfig.h"

#include <stdlib.h>
#include <stdarg.h>
#include <signal.h>

#include <windows.h>
#include <dinput.h>

#include "sysdeps.h"
#include "options.h"
#include "inputdevice.h"
#include "keybuf.h"
#include "xwin.h"
#include "uae.h"
#include "catweasel.h"
#include "keyboard.h"
#include "custom.h"
#include "dxwrap.h"

#ifdef WINDDK
#include <winioctl.h>
#include <ntddkbd.h>
#endif

#include "win32.h"

#define MAX_MAPPINGS 256

#define DID_MOUSE 1
#define DID_JOYSTICK 2
#define DID_KEYBOARD 3

struct didata {
    int type;
    int disabled;
    int acquired;
    GUID guid;
    char *name;
    LPDIRECTINPUTDEVICE8 lpdi;
    int axles;
    int buttons;
    int axismappings[MAX_MAPPINGS];
    char *axisname[MAX_MAPPINGS];
    int axissort[MAX_MAPPINGS];
    int axistype[MAX_MAPPINGS];
    int buttonmappings[MAX_MAPPINGS];
    char *buttonname[MAX_MAPPINGS];
    int buttonsort[MAX_MAPPINGS];
};

#define DI_BUFFER 30

static struct didata di_mouse[MAX_INPUT_DEVICES];
static struct didata di_keyboard[MAX_INPUT_DEVICES];
static struct didata di_joystick[MAX_INPUT_DEVICES];
static int num_mouse, num_keyboard, num_joystick;
static int dd_inited, mouse_inited, keyboard_inited, joystick_inited;
static int stopoutput;
static HANDLE kbhandle = INVALID_HANDLE_VALUE;
static int oldleds, oldusedleds, newleds;

#if 0
void register_rawinput (void)
{
    static int registered;
    RAWINPUTDEVICE rid[1];

    if (registered)
	return;
    registered = 1;
    memset (rid, 0, sizeof (rid));
    rid[0].usUsagePage = 1;
    rid[0].usUsage = 2;

    if (RegisterRawInputDevices(rid, 1, sizeof (RAWINPUTDEVICE)) == FALSE) {
	write_log ("rawinput registration failed %d\n", GetLastError ());
	return;
    }
}

void handle_rawinput (DWORD lParam)
{
    UINT dwSize;
    BYTE lpb[1000];
    RAWINPUT* raw;
    static int reg;

    GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &dwSize, sizeof(RAWINPUTHEADER));
    if (dwSize >= sizeof (lpb))
	return;
    if (GetRawInputData((HRAWINPUT)lParam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER)) != dwSize )
	return;
    raw = (RAWINPUT*)lpb;
    if (raw->header.dwType != RIM_TYPEMOUSE)
	return;

    write_log ("%x %d %x %x %x %d %d %x\n",
          raw->data.mouse.usFlags, 
          raw->data.mouse.ulButtons, 
          raw->data.mouse.usButtonFlags, 
          raw->data.mouse.usButtonData, 
          raw->data.mouse.ulRawButtons, 
          raw->data.mouse.lLastX, 
          raw->data.mouse.lLastY, 
          raw->data.mouse.ulExtraInformation);
}
#endif

static void unacquire (LPDIRECTINPUTDEVICE8 lpdi, char *txt)
{
    if (lpdi) {
	HRESULT hr = IDirectInputDevice8_Unacquire (lpdi);
	if (hr != DI_OK && hr != DI_NOEFFECT)
	    write_log ("unacquire %s failed, %s\n", txt, DXError (hr));
    }
}
static int acquire (LPDIRECTINPUTDEVICE8 lpdi, char *txt)
{
    HRESULT hr = IDirectInputDevice8_Acquire (lpdi);
    if (hr != DI_OK && hr != 0x80070005) {
	write_log ("acquire %s failed, %s\n", txt, DXError (hr));
    }
    return hr == DI_OK ? 1 : 0;
}

static int setcoop (LPDIRECTINPUTDEVICE8 lpdi, DWORD mode, char *txt)
{
    HRESULT hr;
    hr = IDirectInputDevice8_SetCooperativeLevel (lpdi, hMainWnd, mode);
    if (hr != DI_OK && hr != E_NOTIMPL)
	write_log ("setcooperativelevel %s failed, %s\n", txt, DXError (hr));
    return hr == DI_OK ? 1 : 0;
}

static void sortdd (struct didata *dd, int num, int type)
{
    int i, j;
    struct didata ddtmp;
    for (i = 0; i < num; i++) {
	dd[i].type = type;
	for (j = i + 1; j < num; j++) {
	    dd[j].type = type;
	    if (strcmp (dd[i].name, dd[j].name) > 0) {
		memcpy (&ddtmp, &dd[i], sizeof(ddtmp));
		memcpy (&dd[i], &dd[j], sizeof(ddtmp));
		memcpy (&dd[j], &ddtmp, sizeof(ddtmp));
	    }
	}
    }
}

static void sortobjects (char *name, int *mappings, int *sort, char **names, int *types, int num)
{
    int i, j, tmpi;
    char *tmpc;

    for (i = 0; i < num; i++) {
        for (j = i + 1; j < num; j++) {
	    if (sort[i] > sort[j]) {
		tmpi = mappings[i]; mappings[i] = mappings[j]; mappings[j] = tmpi;
		tmpi = sort[i]; sort[i] = sort[j]; sort[j] = tmpi;
		if (types) {
		    tmpi = types[i]; types[i] = types[j]; types[j] = tmpi;
		}
		tmpc = names[i]; names[i] = names[j]; names[j] = tmpc;
	    }
	}
    }
#ifdef DI_DEBUG
    if (num > 0) {
	write_log ("%s:\n", name);
	for (i = 0; i < num; i++)
	    write_log ("%d '%s' (%d,%d)\n", mappings[i], names[i], sort[i], types ? types[i] : -1);
    }
#endif
}

static int isg (const GUID *g, const GUID *g2, int *dwofs, int v)
{
    if (!memcmp (g, g2, sizeof (GUID))) {
	*dwofs = v;
	return 1;
    }
    return 0;
}

static int makesort_joy (const GUID *g, int *dwofs)
{
    if (isg (g, &GUID_XAxis, dwofs, DIJOFS_X)) return -99;
    if (isg (g, &GUID_YAxis, dwofs, DIJOFS_Y)) return -98;
    if (isg (g, &GUID_ZAxis, dwofs, DIJOFS_Z)) return -97;
    if (isg (g, &GUID_RxAxis, dwofs, DIJOFS_RX)) return -89;
    if (isg (g, &GUID_RyAxis, dwofs, DIJOFS_RY)) return -88;
    if (isg (g, &GUID_RzAxis, dwofs, DIJOFS_RZ)) return -87;
    if (isg (g, &GUID_Slider, dwofs, DIJOFS_SLIDER(0))) return -79;
    if (isg (g, &GUID_Slider, dwofs, DIJOFS_SLIDER(1))) return -78;
    if (isg (g, &GUID_POV, dwofs, DIJOFS_POV(0))) return -69;
    if (isg (g, &GUID_POV, dwofs, DIJOFS_POV(1))) return -68;
    if (isg (g, &GUID_POV, dwofs, DIJOFS_POV(2))) return -67;
    if (isg (g, &GUID_POV, dwofs, DIJOFS_POV(3))) return -66;
    return *dwofs;
}

static int makesort_mouse (const GUID *g, int *dwofs)
{
    if (isg (g, &GUID_XAxis, dwofs, DIMOFS_X)) return -99;
    if (isg (g, &GUID_YAxis, dwofs, DIMOFS_Y)) return -98;
    if (isg (g, &GUID_ZAxis, dwofs, DIMOFS_Z)) return -97;
    return *dwofs;
}

static BOOL CALLBACK EnumObjectsCallback (const DIDEVICEOBJECTINSTANCE* pdidoi, VOID *pContext)
{
    struct didata *did = pContext;
    int i = 0;
    char tmp[100];

    if (pdidoi->dwType & DIDFT_AXIS) {
	if (did->axles >= MAX_MAPPINGS)
	    return DIENUM_CONTINUE;
	did->axismappings[did->axles] = pdidoi->dwOfs;
	did->axisname[did->axles] = my_strdup (pdidoi->tszName);
	if (did->type == DID_JOYSTICK)
	    did->axissort[did->axles] = makesort_joy (&pdidoi->guidType, &did->axismappings[did->axles]);
	else if (did->type == DID_MOUSE)
	    did->axissort[did->axles] = makesort_mouse (&pdidoi->guidType, &did->axismappings[did->axles]);
	did->axles++;
    }
    if (pdidoi->dwType & DIDFT_POV) {
	int numpov = 0;
	if (did->axles + 1 >= MAX_MAPPINGS)
	    return DIENUM_CONTINUE;
	for (i = 0; i < did->axles; i++) {
	    if (did->axistype[i]) {
		numpov++;
		i++;
	    }
	}
	if (did->type == DID_JOYSTICK)
	    did->axissort[did->axles] = makesort_joy (&pdidoi->guidType, &did->axismappings[did->axles]);
	else if (did->type == DID_MOUSE)
	    did->axissort[did->axles] = makesort_mouse (&pdidoi->guidType, &did->axismappings[did->axles]);
	for (i = 0; i < 2; i++) {
	    did->axismappings[did->axles + i] = DIJOFS_POV(numpov);
	    sprintf (tmp, "%s (%d)", pdidoi->tszName, i + 1);
	    did->axisname[did->axles + i] = my_strdup (tmp);
	    did->axissort[did->axles + i] = did->axissort[did->axles];
	    did->axistype[did->axles + i] = 1;
	}
	did->axles += 2;
    }
    if (pdidoi->dwType & DIDFT_BUTTON) {
	if (did->buttons >= MAX_MAPPINGS)
	    return DIENUM_CONTINUE;
	did->buttonname[did->buttons] = my_strdup (pdidoi->tszName);
	if (did->type == DID_JOYSTICK) {
	    did->buttonmappings[did->buttons] = DIJOFS_BUTTON(did->buttons); // pdidoi->dwOfs returns garbage!!
	    did->buttonsort[did->buttons] = makesort_joy (&pdidoi->guidType, &did->buttonmappings[did->buttons]);
	} else if (did->type == DID_MOUSE) {
	    did->buttonmappings[did->buttons] = FIELD_OFFSET(DIMOUSESTATE, rgbButtons) + did->buttons;
	    did->buttonsort[did->buttons] = makesort_mouse (&pdidoi->guidType, &did->buttonmappings[did->buttons]);
	} else {
	    did->buttonmappings[did->buttons] = pdidoi->dwOfs;
	}
	did->buttons++;
    }
    return DIENUM_CONTINUE;
}

static BOOL CALLBACK di_enumcallback (LPCDIDEVICEINSTANCE lpddi, LPVOID *dd)
{
    struct didata *did = (struct didata*)dd;
    int i, len;

    if (did == di_mouse) {
	did += num_mouse;
	num_mouse++;
    } else if (did == di_joystick) {
	did += num_joystick;
	num_joystick++;
    } else if (did == di_keyboard) {
	did += num_keyboard;
	num_keyboard++;
    }
    memset (did, 0, sizeof (*did));
    for (i = 0; i < MAX_MAPPINGS; i++) {
        did->axismappings[i] = -1;
        did->buttonmappings[i] = -1;
    }
    len = strlen (lpddi->tszInstanceName) + 1;
    did->name = malloc (len);
    strcpy (did->name, lpddi->tszInstanceName);
    did->guid = lpddi->guidInstance;
    return DIENUM_CONTINUE;
}

extern HINSTANCE hInst;
static LPDIRECTINPUT8 g_lpdi;

static int di_do_init (void)
{
    HRESULT hr; 

    num_mouse = num_joystick = num_keyboard = 0;
    hr = DirectInput8Create (hInst, DIRECTINPUT_VERSION, &IID_IDirectInput8A, (LPVOID *)&g_lpdi, NULL); 
    if (FAILED(hr)) {
	write_log ("DirectInput8Create failed, %s\n", DXError (hr));
	return 0;
    }
    IDirectInput8_EnumDevices (g_lpdi, DI8DEVCLASS_POINTER, di_enumcallback, di_mouse, DIEDFL_ATTACHEDONLY);
    sortdd (di_mouse, num_mouse, DID_MOUSE);
    IDirectInput8_EnumDevices (g_lpdi, DI8DEVCLASS_GAMECTRL, di_enumcallback, di_joystick, DIEDFL_ATTACHEDONLY);
    sortdd (di_joystick, num_joystick, DID_JOYSTICK);
    IDirectInput8_EnumDevices (g_lpdi, DI8DEVCLASS_KEYBOARD, di_enumcallback, di_keyboard, DIEDFL_ATTACHEDONLY);
    sortdd (di_keyboard, num_keyboard, DID_KEYBOARD);
    return 1;
}

static void di_dev_free (struct didata *did)
{
    free (did->name);
    memset (did, 0, sizeof (*did));
}

static int di_init (void)
{
    if (dd_inited++ > 0)
	return 1;
    if (!di_do_init ())
	return 0;
    return 1;
}

static void di_free (void)
{
    if (dd_inited == 0)
	return;
    dd_inited--;
    if (dd_inited > 0)
	return;
    IDirectInput8_Release (g_lpdi);
    g_lpdi = 0;
    di_dev_free (di_mouse);
    di_dev_free (di_joystick);
    di_dev_free (di_keyboard);
}

static int get_mouse_num (void)
{
    return num_mouse;
}

static char *get_mouse_name (int mouse)
{
    return di_mouse[mouse].name;
}

static int get_mouse_widget_num (int mouse)
{
    return di_mouse[mouse].axles + di_mouse[mouse].buttons;
}

static int get_mouse_widget_first (int mouse, int type)
{
    switch (type)
    {
	case IDEV_WIDGET_BUTTON:
	return di_mouse[mouse].axles;
	case IDEV_WIDGET_AXIS:
	return 0;
    }
    return -1;
}

static int get_mouse_widget_type (int mouse, int num, char *name, uae_u32 *code)
{
    struct didata *did = &di_mouse[mouse];

    int axles = did->axles;
    int buttons = did->buttons;
    if (num >= axles && num < axles + buttons) {
	if (name)
	    strcpy (name, did->buttonname[num - did->axles]);
	return IDEV_WIDGET_BUTTON;
    } else if (num < axles) {
	if (name)
	    strcpy (name, did->axisname[num]);
	return IDEV_WIDGET_AXIS;
    }
    return IDEV_WIDGET_NONE;
}

static int init_mouse (void)
{
    int i;
    LPDIRECTINPUTDEVICE8 lpdi;
    HRESULT hr; 
    struct didata *did;

    if (mouse_inited)
	return 1;
    di_init ();
    mouse_inited = 1;
    for (i = 0; i < num_mouse; i++) {
	did = &di_mouse[i];
	if (!did->disabled) {
	    hr = IDirectInput8_CreateDevice (g_lpdi, &did->guid, &lpdi, NULL);
	    if (hr == DI_OK) {
		hr = IDirectInputDevice8_SetDataFormat(lpdi, &c_dfDIMouse); 
		IDirectInputDevice8_EnumObjects (lpdi, EnumObjectsCallback, (void*)did, DIDFT_ALL);
		sortobjects (did->name, did->axismappings, did->axissort, did->axisname, did->axistype, did->axles);
		sortobjects (did->name, did->buttonmappings, did->buttonsort, did->buttonname, 0, did->buttons);
		did->lpdi = lpdi;
	    } else
		write_log ("mouse CreateDevice failed, %s\n", DXError (hr));
	}
    }
    return 1;
}

static void close_mouse (void)
{
    int i;

    if (!mouse_inited)
	return;
    mouse_inited = 0;
    for (i = 0; i < num_mouse; i++) {
	if (di_mouse[i].lpdi)
	    IDirectInputDevice8_Release (di_mouse[i].lpdi);
	di_mouse[i].lpdi = 0;
    }
    di_free ();
}

static int acquire_mouse (int num, int flags)
{
    LPDIRECTINPUTDEVICE8 lpdi = di_mouse[num].lpdi;
    DIPROPDWORD dipdw;
    HRESULT hr;

    //register_rawinput ();
    unacquire (lpdi, "mouse");
    setcoop (lpdi, flags ? (DISCL_FOREGROUND | DISCL_EXCLUSIVE) : (DISCL_BACKGROUND | DISCL_NONEXCLUSIVE), "mouse");
    dipdw.diph.dwSize = sizeof(DIPROPDWORD);
    dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
    dipdw.diph.dwObj = 0;
    dipdw.diph.dwHow = DIPH_DEVICE;
    dipdw.dwData = DI_BUFFER;
    hr = IDirectInputDevice8_SetProperty (lpdi, DIPROP_BUFFERSIZE, &dipdw.diph);
    if (hr != DI_OK)
        write_log ("mouse setpropertry failed, %s\n", DXError (hr));
    di_mouse[num].acquired = 1;
    return acquire (lpdi, "mouse");
}

static void unacquire_mouse (int num)
{
    unacquire (di_mouse[num].lpdi, "mouse");
    di_mouse[num].acquired = 0;
}

static void read_mouse (void)
{
    DIDEVICEOBJECTDATA didod[DI_BUFFER];
    DWORD elements;
    HRESULT hr;
    int i, j, k;
    int fs = isfullscreen();

    for (i = 0; i < MAX_INPUT_DEVICES; i++) {
	struct didata *did = &di_mouse[i];
	LPDIRECTINPUTDEVICE8 lpdi = did->lpdi;
	if (!lpdi)
	    continue;
	elements = DI_BUFFER;
	hr = IDirectInputDevice8_GetDeviceData (lpdi, sizeof (DIDEVICEOBJECTDATA), didod, &elements, 0);
	if (hr == DI_OK) {
	    for (j = 0; j < elements; j++) {
		int dimofs = didod[j].dwOfs;
		int data = didod[j].dwData;
		int state = (data & 0x80) ? 1 : 0;
		if (focus) {
		    if (mouseactive || fs) {
			for (k = 0; k < did->axles; k++) {
			    if (did->axismappings[k] == dimofs) {
				setmousestate (i, k, data, 0);
				break;
			    }
			}
			for (k = 0; k < did->buttons; k++) {
			    if (did->buttonmappings[k] == dimofs) {
#ifdef SINGLEFILE
				if (k == 0)
				    uae_quit ();
#endif
				if ((currprefs.win32_middle_mouse && k != 2) || !(currprefs.win32_middle_mouse)) {
				    setmousebuttonstate (i, k, state);
				    break;
				}
			    }
			}
		    }
		    if (i == 0 && dimofs == DIMOFS_BUTTON2 && state) {
			if (currprefs.win32_middle_mouse) {
			    if (isfullscreen ())
				minimizewindow ();
			    if (mouseactive)
				setmouseactive(0);
			}
		    }
		}
	    }
	} else if (hr == DIERR_INPUTLOST) {
	    acquire (lpdi, "mouse");
	} else if (did->acquired &&  hr == DIERR_NOTACQUIRED) {
	    acquire (lpdi, "mouse");
	}
        IDirectInputDevice8_Poll (lpdi);
    }
}

struct inputdevice_functions inputdevicefunc_mouse = {
    init_mouse, close_mouse, acquire_mouse, unacquire_mouse, read_mouse,
    get_mouse_num, get_mouse_name,
    get_mouse_widget_num, get_mouse_widget_type,
    get_mouse_widget_first
};


static int get_kb_num (void)
{
    return num_keyboard;
}

static char *get_kb_name (int kb)
{
    return di_keyboard[kb].name;
}

static int get_kb_widget_num (int kb)
{
    return di_keyboard[kb].buttons;
}

static int get_kb_widget_first (int kb, int type)
{
    return 0;
}

static int get_kb_widget_type (int kb, int num, char *name, uae_u32 *code)
{
    if (name)
	strcpy (name, di_keyboard[kb].buttonname[num]);
    if (code)
	*code = di_keyboard[kb].buttonmappings[num];
    return IDEV_WIDGET_KEY;
}

static int keyboard_german;

static BYTE ledkeystate[256];

static uae_u32 get_leds (void)
{
    uae_u32 led = 0;
    if (os_winnt && kbhandle != INVALID_HANDLE_VALUE) {
#ifdef WINDDK
	KEYBOARD_INDICATOR_PARAMETERS InputBuffer;
	KEYBOARD_INDICATOR_PARAMETERS OutputBuffer;
	ULONG DataLength = sizeof(KEYBOARD_INDICATOR_PARAMETERS);
	ULONG ReturnedLength;

	memset (&InputBuffer, 0, sizeof (InputBuffer));
	memset (&OutputBuffer, 0, sizeof (OutputBuffer));
	if (!DeviceIoControl(kbhandle, IOCTL_KEYBOARD_QUERY_INDICATORS,
	    &InputBuffer, DataLength, &OutputBuffer, DataLength, &ReturnedLength, NULL))
	    return 0;
	if (OutputBuffer.LedFlags & KEYBOARD_NUM_LOCK_ON) led |= KBLED_NUMLOCK;
	if (OutputBuffer.LedFlags & KEYBOARD_CAPS_LOCK_ON) led |= KBLED_CAPSLOCK;
	if (OutputBuffer.LedFlags & KEYBOARD_SCROLL_LOCK_ON) led |= KBLED_SCROLLLOCK;
#endif
    } else if (!os_winnt) {
	GetKeyboardState (ledkeystate);
	if (ledkeystate[VK_NUMLOCK] & 1) led |= KBLED_NUMLOCK;
	if (ledkeystate[VK_CAPITAL] & 1) led |= KBLED_CAPSLOCK;
	if (ledkeystate[VK_SCROLL] & 1) led |= KBLED_SCROLLLOCK;
    }
    return led;
}

static void set_leds (uae_u32 led)
{
    if (os_winnt && kbhandle != INVALID_HANDLE_VALUE) {
#ifdef WINDDK
	KEYBOARD_INDICATOR_PARAMETERS InputBuffer;
	ULONG DataLength = sizeof(KEYBOARD_INDICATOR_PARAMETERS);
	ULONG ReturnedLength;

	memset (&InputBuffer, 0, sizeof (InputBuffer));
   	if (led & KBLED_NUMLOCK) InputBuffer.LedFlags |= KEYBOARD_NUM_LOCK_ON;
	if (led & KBLED_CAPSLOCK) InputBuffer.LedFlags |= KEYBOARD_CAPS_LOCK_ON;
	if (led & KBLED_SCROLLLOCK) InputBuffer.LedFlags |= KEYBOARD_SCROLL_LOCK_ON;
	if (!DeviceIoControl(kbhandle, IOCTL_KEYBOARD_SET_INDICATORS,
	    &InputBuffer, DataLength, NULL, 0, &ReturnedLength, NULL))
		write_log ("kbleds: DeviceIoControl() failed %d\n", GetLastError());
#endif
    } else if (!os_winnt) {
	ledkeystate[VK_NUMLOCK] &= ~1;
	if (led & KBLED_NUMLOCK)
	    ledkeystate[VK_NUMLOCK] = 1;
	ledkeystate[VK_CAPITAL] &= ~1;
	if (led & KBLED_CAPSLOCK)
	    ledkeystate[VK_CAPITAL] = 1;
	ledkeystate[VK_SCROLL] &= ~1;
	if (led & KBLED_SCROLLLOCK)
	    ledkeystate[VK_SCROLL] = 1;
	SetKeyboardState (ledkeystate);
     }
}

static void update_leds (void)
{
    if (!currprefs.keyboard_leds_in_use)
	return;
    if (newleds != oldusedleds) {
	oldusedleds = newleds;
	set_leds (newleds);
    }
}

void indicator_leds (int num, int state)
{
    int i;

    if (!currprefs.keyboard_leds_in_use)
	return;
    for (i = 0; i < 3; i++) {
	if (currprefs.keyboard_leds[i] == num + 1) {
	    newleds &= ~(1 << i);
	    if (state) newleds |= (1 << i);
	}
    }
}

static int init_kb (void)
{
    int i;
    LPDIRECTINPUTDEVICE8 lpdi;
    DIPROPDWORD dipdw;
    HRESULT hr;
    HKL keyboardlayoutid;
    WORD keyboardlangid;

    if (keyboard_inited)
	return 1;
    di_init ();
    oldusedleds = -1;
    keyboard_inited = 1;
    for (i = 0; i < num_keyboard; i++) {
        struct didata *did = &di_keyboard[i];
	if (did->lpdi == 0 && !did->disabled) {
	    hr = IDirectInput8_CreateDevice (g_lpdi, &did->guid, &lpdi, NULL);
	    if (hr == DI_OK) {
		hr = IDirectInputDevice8_SetDataFormat(lpdi, &c_dfDIKeyboard); 
		if (hr != DI_OK)
		    write_log ("keyboard setdataformat failed, %s\n", DXError (hr));
		dipdw.diph.dwSize = sizeof(DIPROPDWORD);
		dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
		dipdw.diph.dwObj = 0;
		dipdw.diph.dwHow = DIPH_DEVICE;
		dipdw.dwData = DI_BUFFER;
		hr = IDirectInputDevice8_SetProperty (lpdi, DIPROP_BUFFERSIZE, &dipdw.diph);
		if (hr != DI_OK)
		    write_log ("keyboard setpropertry failed, %s\n", DXError (hr));
	        IDirectInputDevice8_EnumObjects (lpdi, EnumObjectsCallback, (void*)did, DIDFT_ALL);
		sortobjects (did->name, did->axismappings, did->axissort, did->axisname, did->axistype, did->axles);
		sortobjects (did->name, did->buttonmappings, did->buttonsort, did->buttonname, 0, did->buttons);
		did->lpdi = lpdi;
	    } else
		write_log ("keyboard CreateDevice failed, %s\n", DXError (hr));
	}
    }
    keyboardlayoutid = GetKeyboardLayout(0);      
    keyboardlangid = LOWORD(keyboardlayoutid);
    keyboard_german = 0;
    if (keyboardlangid == MAKELANGID (LANG_GERMAN, SUBLANG_GERMAN))
	keyboard_german = 1;

    return 1;
}

static void close_kb (void)
{
    int i;

    if (keyboard_inited == 0)
	return;
    keyboard_inited = 0;

    for (i = 0; i < num_keyboard; i++) {
	if (di_keyboard[i].lpdi)
	    IDirectInputDevice8_Release (di_keyboard[i].lpdi);
	di_keyboard[i].lpdi = 0;
    }
    di_free ();
}

#define MAX_KEYCODES 256
static uae_u8 di_keycodes[MAX_INPUT_DEVICES][MAX_KEYCODES];
static uae_u32 kb_do_refresh;

int ispressed (int key)
{
    int i;
    for (i = 0; i < MAX_INPUT_DEVICES; i++) {
	if (di_keycodes[i][key])
	    return 1;
    }
    return 0;
}

void release_keys (void)
{
    int i, j;

    for (j = 0; j < MAX_INPUT_DEVICES; j++) {
	for (i = 0; i < MAX_KEYCODES; i++) {
	    if (di_keycodes[j][i]) {
		my_kbd_handler (j, i, 0);
		di_keycodes[j][i] = 0;
	    }
	}
    }
}

static int refresh_kb (LPDIRECTINPUTDEVICE8 lpdi, int num)
{
    HRESULT hr;
    int i;
    uae_u8 kc[256];

    hr = IDirectInputDevice8_GetDeviceState (lpdi, sizeof (kc), kc);
    if (hr == DI_OK) {
	for (i = 0; i < sizeof (kc); i++) {
	    if (kc[i] & 0x80) kc[i] = 1; else kc[i] = 0;
	    if (kc[i] != di_keycodes[num][i]) {
		write_log ("%02.2X -> %d\n", i, kc[i]);
		di_keycodes[num][i] = kc[i];
		my_kbd_handler (num, i, kc[i]);
	    }
	}
    } else if (hr == DIERR_INPUTLOST) {
	acquire (lpdi, "keyboard");
	IDirectInputDevice8_Poll (lpdi);
	return 0;
    }
    IDirectInputDevice8_Poll (lpdi);
    return 1;
}


static int keyhack (int scancode,int pressed, int num)
{
    static byte backslashstate,apostrophstate;
    
     //check ALT-F4 
    if (pressed && !di_keycodes[num][DIK_F4] && scancode == DIK_F4 && di_keycodes[num][DIK_LALT] && !currprefs.win32_ctrl_F11_is_quit) {
	uae_quit ();
	return -1;
    }
#ifdef SINGLEFILE
    if (pressed && scancode == DIK_ESCAPE) {
	uae_quit ();
	return -1;
    }
#endif

    // release mouse if TAB and ALT is pressed 
    if (pressed && di_keycodes[num][DIK_LALT] && scancode == DIK_TAB) {
	disablecapture ();
	return -1;
    }

    if (!keyboard_german)
	return scancode;
 
 //This code look so ugly because there is no Directinput
 //key for # (called numbersign on win standard message)
 //available
 //so here need to change qulifier state and restore it when key
 //is release
    if (!keyboard_german)
	return scancode;
    if (scancode == DIK_BACKSLASH) // The # key
    {
	if (di_keycodes[num][DIK_LSHIFT] || di_keycodes[num][DIK_RSHIFT] || apostrophstate)
        {   
            if (pressed)
            {   apostrophstate=1;
                inputdevice_translatekeycode (num, DIK_RSHIFT, 0);
                inputdevice_translatekeycode (num, DIK_LSHIFT, 0);
                return 13;           // the german ' key
            }
            else
            {
                //best is add a real keystatecheck here but it still work so
                apostrophstate=0;
                return 13;
            }

        }
        if (pressed)
        {
            inputdevice_translatekeycode (num, DIK_LALT, 1);
            inputdevice_translatekeycode (num, DIK_LSHIFT,1);
            return 4;           // the german # key        
        }
            else
        {
            inputdevice_translatekeycode (num, DIK_LALT,0);
            inputdevice_translatekeycode (num, DIK_LSHIFT,0);
            // Here is the same not nice but do the job
            return 4;           // the german # key        
            
        }    
    }
    if ((di_keycodes[num][DIK_RALT]) || (backslashstate)) {
	switch (scancode)
	{
	    case 12:
	    if (pressed)
	    {   
		backslashstate=1;
		inputdevice_translatekeycode (num, DIK_RALT, 0);
		return DIK_BACKSLASH;      
	    }
	    else
	    {
	    backslashstate=0;
	    return DIK_BACKSLASH;
	    }
	}
    }
    return scancode;
}

static void read_kb (void)
{
    DIDEVICEOBJECTDATA didod[DI_BUFFER];
    DWORD elements;
    HRESULT hr; //, hr2;
    LPDIRECTINPUTDEVICE8 lpdi;
    int i, j;
//    uae_u8 kc[256];

    update_leds ();
    for (i = 0; i < MAX_INPUT_DEVICES; i++) {
	struct didata *did = &di_keyboard[i];
	lpdi = did->lpdi;
	if (!lpdi)
	    continue;
	if (kb_do_refresh & (1 << i)) {
	    if (!refresh_kb (lpdi, i))
		continue;
	    kb_do_refresh &= ~(1 << i);
	}
//	hr2 = IDirectInputDevice8_GetDeviceState (lpdi, sizeof (kc), kc);
	elements = DI_BUFFER;
	hr = IDirectInputDevice8_GetDeviceData(lpdi, sizeof(DIDEVICEOBJECTDATA), didod, &elements, 0);
	if (hr == DI_OK) {
	    for (j = 0; j < elements; j++) {
		int scancode = didod[j].dwOfs;
		int pressed = (didod[j].dwData & 0x80) ? 1 : 0;
/*
		if (hr2 == DI_OK && (kc[scancode] & 0x80) && pressed == 0) {
		    write_log ("confused key %02.2X\n", scancode);
		    continue;
		}
*/
	        scancode = keyhack (scancode, pressed, i);
	        if (scancode < 0)
		    continue;
		di_keycodes[i][scancode] = pressed;
		if (stopoutput == 0)
		    my_kbd_handler (i, scancode, pressed);
	    }
        } else if (hr == DIERR_INPUTLOST) {
	    acquire (lpdi, "keyboard");
	    kb_do_refresh |= 1 << i;
	} else if (did->acquired &&  hr == DIERR_NOTACQUIRED) {
	    acquire (lpdi, "keyboard");
	}
	IDirectInputDevice8_Poll (lpdi);
    }
#ifdef CATWEASEL
    {
	char kc;
	if (stopoutput == 0 && catweasel_read_keyboard (&kc))
	    inputdevice_do_keyboard (kc & 0x7f, kc & 0x80);
    }
#endif
}

void wait_keyrelease (void)
{
    int i, j, maxcount = 200, found;
    stopoutput++;

    while (maxcount-- > 0) {
	sleep_millis (10);
	read_kb ();
	found = 0;
	for (j = 0; j < MAX_INPUT_DEVICES; j++) {
	    for (i = 0; i < MAX_KEYCODES; i++) {
		if (di_keycodes[j][i]) found = 1;
	    }
	}
	if (!found)
	    break;
    }
    release_keys ();

    for (;;) {
	int ok = 0, nok = 0;
	for (i = 0; i < MAX_INPUT_DEVICES; i++) {
	    struct didata *did = &di_mouse[i];
	    DIMOUSESTATE dis;
	    LPDIRECTINPUTDEVICE8 lpdi = did->lpdi;
	    HRESULT hr;
	    int j;

	    if (!lpdi)
		continue;
	    nok++;
	    hr = IDirectInputDevice8_GetDeviceState (lpdi, sizeof (dis), &dis);
	    if (hr == DI_OK) {
		for (j = 0; j < 4; j++) {
		    if (dis.rgbButtons[j] & 0x80) break;
		}
		if (j == 4) ok++;
	    } else {
		ok++;
	    }
	}
	if (ok == nok) break;
	sleep_millis (10);
    }
    stopoutput--;
}

static int acquire_kb (int num, int flags)
{
    DWORD mode = DISCL_NOWINKEY | DISCL_FOREGROUND | DISCL_EXCLUSIVE;
    LPDIRECTINPUTDEVICE8 lpdi = di_keyboard[num].lpdi;

    unacquire (lpdi, "keyboard");
    setcoop (lpdi, mode, "keyboard");
    kb_do_refresh = ~0;
    di_keyboard[num].acquired = 1;

#ifdef WINDDK
    if (currprefs.keyboard_leds_in_use) {
	if (os_winnt) {
	    if (DefineDosDevice (DDD_RAW_TARGET_PATH, "Kbd","\\Device\\KeyboardClass0")) {
		kbhandle = CreateFile("\\\\.\\Kbd", GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
		if (kbhandle == INVALID_HANDLE_VALUE)
		    write_log ("kbled: CreateFile failed, error %d\n", GetLastError());
	    } else {
		write_log ("kbled: DefineDosDevice failed, error %d\n", GetLastError());
	    }
	}
	oldleds = get_leds ();
	if (oldusedleds < 0) 
	    oldusedleds = newleds = oldleds;
	set_leds (oldusedleds);
    }
#endif
    return acquire (lpdi, "keyboard");
}

static void unacquire_kb (int num)
{
    LPDIRECTINPUTDEVICE8 lpdi = di_keyboard[num].lpdi;
    unacquire (lpdi, "keyboard");
    di_keyboard[num].acquired = 0;
    release_keys ();

#ifdef WINDDK
    if (currprefs.keyboard_leds_in_use && oldusedleds >= 0) {
	set_leds (oldleds);
	oldusedleds = oldleds;
	if (kbhandle != INVALID_HANDLE_VALUE) {
	    CloseHandle(kbhandle);
	    DefineDosDevice (DDD_REMOVE_DEFINITION, "Kbd", NULL);
	    kbhandle = INVALID_HANDLE_VALUE;
	}
    }
#endif
}

struct inputdevice_functions inputdevicefunc_keyboard = {
    init_kb, close_kb, acquire_kb, unacquire_kb, read_kb,
    get_kb_num, get_kb_name,
    get_kb_widget_num, get_kb_widget_type,
    get_kb_widget_first
};


static int get_joystick_num (void)
{
    return num_joystick;
}

static int get_joystick_widget_num (int joy)
{
    return di_joystick[joy].axles + di_joystick[joy].buttons;
}

static int get_joystick_widget_type (int joy, int num, char *name, uae_u32 *code)
{
    struct didata *did = &di_joystick[joy];
    if (num >= did->axles && num < did->axles + did->buttons) {
	if (name)
	    strcpy (name, did->buttonname[num - did->axles]);
	return IDEV_WIDGET_BUTTON;
    } else if (num < di_joystick[joy].axles) {
	if (name)
	    strcpy (name, did->axisname[num]);
	return IDEV_WIDGET_AXIS;
    }
    return IDEV_WIDGET_NONE;
}

static int get_joystick_widget_first (int joy, int type)
{
    switch (type)
    {
	case IDEV_WIDGET_BUTTON:
	return di_joystick[joy].axles;
	case IDEV_WIDGET_AXIS:
	return 0;
    }
    return -1;
}

static char *get_joystick_name (int joy)
{
    return di_joystick[joy].name;
}

static int isjoy (int pcport, int amigaport)
{
    if (pcport == 0)
	return JSEM_ISJOY0 (amigaport, &currprefs);
    else
	return JSEM_ISJOY1 (amigaport, &currprefs);
}

static void read_joystick (void)
{
    DIDEVICEOBJECTDATA didod[DI_BUFFER];
    LPDIRECTINPUTDEVICE8 lpdi;
    DWORD elements;
    HRESULT hr;
    int i, j, k;

    for (i = 0; i < MAX_INPUT_DEVICES; i++) {
	struct didata *did = &di_joystick[i];
	lpdi = did->lpdi;
	if (!lpdi)
	    continue;
	if (currprefs.input_selected_setting == 0) {
	    if (i >= 2)
		break;
	    if (isjoy (i, 0)) {
		if (JSEM_ISNUMPAD (0, &currprefs) || JSEM_ISCURSOR (0, &currprefs) || JSEM_ISSOMEWHEREELSE (0, &currprefs))
		    continue;
	    } else if (isjoy (i, 1)) {
		if (JSEM_ISNUMPAD (1, &currprefs) || JSEM_ISCURSOR (1, &currprefs) || JSEM_ISSOMEWHEREELSE (1, &currprefs))
		    continue;
	    } else
		continue;
	}
	elements = DI_BUFFER;
	hr = IDirectInputDevice8_GetDeviceData (lpdi, sizeof (DIDEVICEOBJECTDATA), didod, &elements, 0);
	if (hr == DI_OK) {
	    for (j = 0; j < elements; j++) {
		int dimofs = didod[j].dwOfs;
		int data = didod[j].dwData;
		int data2 = data;
		int state = (data & 0x80) ? 1 : 0;
		data -= 32768;
		
		for (k = 0; k < did->buttons; k++) {
		    if (did->buttonmappings[k] == dimofs) {
#ifdef DI_DEBUG2
			write_log ("B:NUM=%d OFF=%d NAME=%s VAL=%d STATE=%d\n",
			    k, dimofs, did->buttonname[k], data, state);
#endif
		        setjoybuttonstate (i, k, state);
		        break;
		    }
		}
		for (k = 0; k < did->axles; k++) {
		    if (did->axismappings[k] == dimofs) {
			if (did->axistype[k]) {
			    setjoystickstate (i, k, (data2 >= 20250 && data2 <= 33750) ? -1 : (data2 >= 2250 && data2 <= 15750) ? 1 : 0, 1);
			    setjoystickstate (i, k + 1, ((data2 >= 29250 && data2 <= 33750) || (data2 >= 0 && data2 <= 6750)) ? -1 : (data2 >= 11250 && data2 <= 24750) ? 1 : 0, 1);
#ifdef DI_DEBUG2
			    write_log ("P:NUM=%d OFF=%d NAME=%s VAL=%d\n", k, dimofs, did->axisname[k], data2);
#endif
			} else {
#ifdef DI_DEBUG2
			    if (data < -20000 || data > 20000)
				write_log ("A:NUM=%d OFF=%d NAME=%s VAL=%d\n", k, dimofs, did->axisname[k], data);
#endif
			    setjoystickstate (i, k, data, 32768);
			}
		        break;
		    }
		}
	    }
	} else if (hr == DIERR_INPUTLOST) {
	    acquire (lpdi, "joystick");
	} else if (did->acquired &&  hr == DIERR_NOTACQUIRED) {
	    acquire (lpdi, "joystick");
	}
        IDirectInputDevice8_Poll (lpdi);
    }
#ifdef CATWEASEL
    {
	static uae_u8 odir, obut;
	uae_u8 dir, dir2, but;
	if (catweasel_read_joystick (&dir, &but)) {
	    if ((but & 0x80) != (obut & 0x80))
		handle_input_event (INPUTEVENT_JOY1_FIRE_BUTTON, !(but & 0x80), 1, 0);
	    if ((but & 0x40) != (obut & 0x40))
		handle_input_event (INPUTEVENT_JOY2_FIRE_BUTTON, !(but & 0x40), 1, 0);
	    dir2 = odir;
	    odir = dir;
	    obut = but;
	    if ((dir & 15) != (dir2 & 15)) {
	        handle_input_event (INPUTEVENT_JOY2_HORIZ, !(dir & 1) ? 1 : !(dir & 2) ? -1 : 0, 1, 0);
	        handle_input_event (INPUTEVENT_JOY2_VERT, !(dir & 4) ? 1 : !(dir & 8) ? -1 : 0, 1, 0);
	    }
	    dir >>= 4;
	    dir2 >>= 4;
	    if ((dir & 15) != (dir2 & 15)) {
	        handle_input_event (INPUTEVENT_JOY1_HORIZ, !(dir & 1) ? 1 : !(dir & 2) ? -1 : 0, 1, 0);
	        handle_input_event (INPUTEVENT_JOY1_VERT, !(dir & 4) ? 1 : !(dir & 8) ? -1 : 0, 1, 0);
	    }
	}
    }
#endif
}

static int init_joystick (void)
{
    int i;
    LPDIRECTINPUTDEVICE8 lpdi;
    HRESULT hr;
    struct didata *did;

    if (joystick_inited)
	return 1;
    di_init ();
    joystick_inited = 1;
    for (i = 0; i < num_joystick; i++) {
	did = &di_joystick[i];
	if (!did->disabled) {
	    hr = IDirectInput8_CreateDevice (g_lpdi, &did->guid, &lpdi, NULL);
	    if (hr == DI_OK) {
		hr = IDirectInputDevice8_SetDataFormat(lpdi, &c_dfDIJoystick);
		if (hr == DI_OK) {
		    did->lpdi = lpdi;
		    IDirectInputDevice8_EnumObjects (lpdi, EnumObjectsCallback, (void*)did, DIDFT_ALL);
		    sortobjects (did->name,did->axismappings, did->axissort, did->axisname, did->axistype, did->axles);
		    sortobjects (did->name,did->buttonmappings, did->buttonsort, did->buttonname, 0, did->buttons);
		}
	    } else
		write_log ("joystick createdevice failed, %s\n", DXError (hr));
	}
    }
    return 1;
}

static void close_joystick (void)
{
    int i;

    if (!joystick_inited)
	return;
    joystick_inited = 0;
    for (i = 0; i < num_joystick; i++) {
	if (di_joystick[i].lpdi)
	    IDirectInputDevice8_Release (di_joystick[i].lpdi);
	di_joystick[i].lpdi = 0;
    }
    di_free ();
}

static int acquire_joystick (int num, int flags)
{
    LPDIRECTINPUTDEVICE8 lpdi = di_joystick[num].lpdi;
    DIPROPDWORD dipdw;
    HRESULT hr;

    unacquire (lpdi, "joystick");
    setcoop (lpdi, flags ? (DISCL_FOREGROUND | DISCL_EXCLUSIVE) : (DISCL_BACKGROUND | DISCL_NONEXCLUSIVE), "joystick");
    memset (&dipdw, 0, sizeof (dipdw));
    dipdw.diph.dwSize = sizeof(DIPROPDWORD);
    dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
    dipdw.diph.dwObj = 0;
    dipdw.diph.dwHow = DIPH_DEVICE;
    dipdw.dwData = DI_BUFFER;
    hr = IDirectInputDevice8_SetProperty (lpdi, DIPROP_BUFFERSIZE, &dipdw.diph);
    if (hr != DI_OK)
	write_log ("joystick setpropertry failed, %s\n", DXError (hr));
    di_joystick[num].acquired = 1;
    return acquire (lpdi, "joystick");
}

static void unacquire_joystick (int num)
{
    unacquire (di_joystick[num].lpdi, "joystick");
    di_joystick[num].acquired = 0;
}

struct inputdevice_functions inputdevicefunc_joystick = {
    init_joystick, close_joystick, acquire_joystick, unacquire_joystick,
    read_joystick, get_joystick_num, get_joystick_name,
    get_joystick_widget_num, get_joystick_widget_type,
    get_joystick_widget_first
};


/* 
  * UAE - The Un*x Amiga Emulator
  * 
  * Win32-specific header file
  * 
  * (c) 1997 Mathias Ortmann
  */

extern int atime, acount;

#define UAEWINVERSION_BASE "0.8.5"
#define UAEWINRELEASE "1"
#define UAEWINVERSION UAEWINVERSION_BASE
#define PROGNAME "WinUAE " UAEWINVERSION ", Release " UAEWINRELEASE

// win32glue.c functions
extern int my_kbd_handler (int, int, int);

// win32.c functions
extern void toggle_fullscreen (void);
extern int currtime (void);
extern void setup_brkhandler (void);
extern void remove_brkhandler (void);

extern int checkkey (int vkey, long lParam);
extern void setmouseactive (int active);

extern int gunzip_hack (const char *src, const char *dst);

extern int requestfname (char *title, char *name);

extern void startsound (void);
extern void stopsound (void);

// globals
extern int bActive;
//extern int draw_all;

//extern int vpos;
//extern unsigned short lof;
extern int capslock;

extern int process_desired_pri;

extern int toggle_sound, bytesinbuf;

#define USE_ZFILE

void fname_wtoa(unsigned char *);
void fname_atow(const char *, char *, int);

extern int sound_available;
extern int framecnt;
extern char prtname[];
extern char VersionStr[256];

struct win32_displaymode {
    struct win32_displaymode *next;
    int width;
    int height;
    int refreshrate;
    /* Bit mask of RGBFF_xxx values.  */
    uae_u32 colormodes;
};

extern struct win32_displaymode *win32_displaymode_list;

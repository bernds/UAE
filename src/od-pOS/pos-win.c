 /*
  * UAE - The Un*x Amiga Emulator
  * 
  * p.OS interface
  * 
  * Copyright 1997 Samuel Devulder.
  */

/****************************************************************************/

#include "sysconfig.h"
#include "sysdeps.h"

#include <ctype.h>
#include <signal.h>
#include <ix.h>

/****************************************************************************/

#include "threaddep/thread.h"
#include "uae.h"
#include "config.h"
#include "options.h"
#include "../include/memory.h"
#include "custom.h" 
#include "readcpu.h" 
#include "newcpu.h" 
#include "xwin.h"
#include "keyboard.h" 
#include "keybuf.h" 
#include "disk.h" 
#include "debug.h"
#include "gui.h"
#include "uaeexe.h"
#include "picasso96.h"

/****************************************************************************/

struct pOS_Locale;
struct pOS_Library;
struct pOS_ExecBase;
struct pOS_DosBase;
struct pOS_Method;
struct pOS_Resource;
struct pOS_RawDoFmtData;
struct pOS_AsciiDoFmtData;
struct pOS_AsciiFmtData;
struct pOS_ClassGrp;
struct pOS_DosDevPathInfo;
struct pOS_ShellScript;
struct pOS_SegmentInfo;
struct IClass;
struct Isrvstr;
struct TagItem;
struct MemHeader;
struct SemaphoreMessage;
struct StackSwapStruct;
struct Interrupt;
struct StandardPacket;
struct InfoData;
struct WBStartup;

#define _SIZE_T /* to avoid interference with <syst/types.h> */
#include <pExec/Types.h>
#include <pDOS/Lock.h>
#include <pDOS/ArgTags.h>
#include <pDOS/DosSig.h>
#include <pDOS/DosErrors.h>
#include <psl/FileReq.h>
#include <psl/pSLTags.h>
#include <pGadget/Gadget.h>
#include <pGfx/StdGMap.h>
#include <pGfx/StdCMap.h>
#include <pGfx/GfxTags.h>
#include <pGfx/RastPort.h>
#include <pScreen/ScrTags.h>
#include <pScreen/Window.h>
#include <pScreen/Screen.h>
#include <pScreen/DrawInfo.h>
#include <pIntui/IntuMsg.h>
#include <pIntui/ICI.h>
#include <pIntui/Tags.h>
#include <pIntui/GClass.h>
#include <pIntui/DragTags.h>
#include <pDtType/DClass.h>
#include <pDtType/DtBase.h>
#include <pDtType/DtTags.h>
#include <pDtType/DosObj.h>

/*#include <pInline/pLibExt.h> */
#include <pInline/pExec2.h>
#include <pInline/pDos2.h>
#include <pInline/pIntui2.h>
#include <pInline/pGfx2.h>
#include <pInline/pPSL2.h>
#include <pInline/pDtType2.h>

/****************************************************************************/

xcolnr xcolors[4096];

 /* Keyboard and mouse */

static int keystate[256];

int buttonstate[3];
int lastmx, lastmy;
int newmousecounters;

static int inwindow;

struct vidbuf_description gfxvidinfo;
static char *oldpixbuf;

/****************************************************************************/ 
/*
 * prototypes & global vars
 */
extern int  rexx_init(void);
extern void rexx_exit(void);
extern void initpseudodevices(void);
extern void closepseudodevices(void);
extern char *to_unix_path(char *s);
extern char *from_unix_path(char *s);
extern void split_dir_file(char *src, char **dir, char **file);

void main_window_led(int led, int on);
ULONG pOS_GetObjectData(APTR obj,ULONG tag);

extern struct pOS_ExecBase        *gb_ExecBase;
extern struct pOS_ExecLibFunction *gb_ExecLib;
extern struct pOS_DosBase         *gb_DosBase;
extern struct pOS_LocaleBase      *gb_LocaleBase;

struct pOS_IntuiDevice            *gb_IntuiBase;
struct pOS_GfxBase                *gb_GfxBase;
struct pOS_pSLBase                *gb_PSLBase;
struct pOS_DataTypeBase           *gb_DtTypeBase;

/****************************************************************************/

static void disk_hotkeys(void);
static void set_title(void);
static void HandleDrop(struct pOS_IEDragDrop *DD);

static struct pOS_Window           *W;
static struct pOS_RastPort         *RP;
static struct pOS_StdRGB8GfxMap    *GM24;
static struct pOS_StdChunky8GfxMap *GM8;
static struct pOS_StdRLColorMap    *CM;
static struct pOS_Gadget           *DDGad; /* Drag&Drop gadget */

static int                          XOffset,YOffset;
static int                          use_gray;

/****************************************************************************/

static RETSIGTYPE sigbrkhandler(int foo)
{
    activate_debugger();
}

void setup_brkhandler(void)
{
    signal(SIGINT,sigbrkhandler);
}

/****************************************************************************/

__inline__ void flush_line(int y)
{
    int len, l;
    unsigned char *buf, *dst, *src;
    int xs;

    if(y<0 || y>=gfxvidinfo.height) return;
    xs = 0;
    len = gfxvidinfo.width;
    buf = gfxvidinfo.linemem; if(!buf) 
    buf = gfxvidinfo.bufmem + y*gfxvidinfo.rowbytes;

    if(currprefs.use_low_bandwidth) {
        switch(gfxvidinfo.pixbytes) {
        case 2: {
            short *newp = (short *)buf;
            short *oldp = (short *)(oldpixbuf + y*gfxvidinfo.rowbytes);
            while (*newp++ == *oldp++) if(!--len) return;
            src   = (char *)--newp;
            dst   = (char *)--oldp;
            newp += len;
            oldp += len;
            while (*--newp == *--oldp);
            len   = 1 + (oldp - (short *)dst);
            xs    = (src - buf)/2;
            pOS_CopyMem (src, dst, len * 2);
        } break;
        case 1: {
            char *newp = (char *)buf;
            char *oldp = (char *)(oldpixbuf + y*gfxvidinfo.rowbytes);
            while (*newp++ == *oldp++) if(!--len) return;
            src   = (char *)--newp;
            dst   = (char *)--oldp;
            newp += len;
            oldp += len;
            while (*--newp == *--oldp);
            len   = 1 + (oldp - (char *)dst);
            xs    = (src - buf);
            pOS_CopyMem (src, dst, len);
        } break;
        default: abort();
        }
    }
    switch(gfxvidinfo.pixbytes) {
      case 1:
        buf += xs;
        l = len; src = buf; dst = GM8->ckgm_Chunky+xs;
        while(l--) {*dst++ = *src++;}
        pOS_WriteStdGfxMapRastPort(&GM8->ckgm_Gfx,xs,0,
                                   RP,XOffset+xs,YOffset+y,len,1,0);
        break;
      case 2:
        /* if only pOS could provide a ARGB8 struct that fits on a ULONG */
        buf += 2*xs;
        l = len; src = buf;   dst = GM24->rgbgm_R+xs;
        while(l--) {*dst++ = (ULONG)*src * 0x11 ; src+=2;}
        l = len; src = buf+1; dst = GM24->rgbgm_G+xs;
        while(l--) {*dst++ = (ULONG)(*src>>4) * 0x11; src+=2;}
        l = len; src = buf+1; dst = GM24->rgbgm_B+xs;
        while(l--) {*dst++ = (ULONG)(*src&15) * 0x11; src+=2;}
        pOS_WriteStdGfxMapRastPort(&GM24->rgbgm_Gfx,xs,0,
                                   RP,XOffset+xs,YOffset+y,len,1,0);
        break;
    }
}

/****************************************************************************/

void flush_block (int ystart, int ystop)
{
    int y;
    for(y=ystart; y<=ystop; ++y) flush_line(y);
}

/****************************************************************************/

void flush_screen (int ystart, int ystop)
{
}

/****************************************************************************/

static int get_color(int r, int g, int b, xcolnr *cnp)
{
   static int col;

   r *= 0x11; g *= 0x11; b *= 0x11;
   if(use_gray) r = g = b = (77*r + 151*g + 29*b) / 256;

   CM->rlcm_Colors[col].rc_U.rc_E.rcue_R = r;
   CM->rlcm_Colors[col].rc_U.rc_E.rcue_G = g;
   CM->rlcm_Colors[col].rc_U.rc_E.rcue_B = b;

   *cnp = col++;

   return 1;
}

/****************************************************************************/

static int init_colors(void)
{
    int i;

    switch(currprefs.color_mode) {
      case 0: case 2:
        write_log("Using 256 gray scales.\n");   
        use_gray = 1;
        /* fall through */
      case 1:
        if(!use_gray) write_log("Using 256 colors.\n");
        alloc_colors256(get_color);
        break;
      case 3:
        printf("Using truecolor.\n");
        for(i=0;i<4096;++i) xcolors[i] = i;
        break;
    }
    switch (gfxvidinfo.pixbytes) {
      case 2:
        for (i = 0; i < 4096; i++)
            xcolors[i] = xcolors[i] * 0x00010001;
        gfxvidinfo.can_double = 1;
        break;
      case 1:
        for (i = 0; i < 4096; i++)
            xcolors[i] = xcolors[i] * 0x01010101;
        gfxvidinfo.can_double = 1;
        break;
      default:
        gfxvidinfo.can_double = 0;
        break;
    }
    return 1;
}

/****************************************************************************/

#define IND "==="

static int setup_publicscreen(void)
{
    static char ScreenTitle[100];
    static struct pOS_IBox ZoomArray = {0,0,0,0};
    struct pOS_Screen *S;

    sprintf(ScreenTitle,
            "UAE-%d.%d.%d © by Bernd Schmidt & contributors, "
            "p.OS Port by Samuel Devulder.",
            (version / 100) % 10, (version / 10) % 10, version % 10);

    S = pOS_LockPubScreen(NULL);
    if(!S) {
        write_log("No public screen !\n");
        return 0;
    }

    if(currprefs.gfx_height + S->scr_BarHeight + 1 >= (int)S->scr_Height) {
        currprefs.gfx_height >>= 1;
        currprefs.gfx_correct_aspect = 1;
    }
    
    DDGad = (void *)pOS_NewIObject( 
              NULL, S->scr_DrawInfo->dri_Names[SCRNAM_GClass], 0,
              ICLTAG_GadBorderType,         GADBORTYP_BlkBox,
              ICLTAG_Left,                  1,
              ICLTAG_Top,                   1,
              ICLTAG_RelWidth,             -1,
              ICLTAG_RelHeight,            -1,
              ICLTAG_StandardDrop,          TRUE,
              ICLTAG_AddDropAttName,        (ULONG)"#?dosobj/#?",
//            ICLTAG_StandardDrag,          TRUE,
              ICLTAG_HighlightDropBox,      TRUE,
              ICLTAG_NoSelect,              TRUE,
            TAG_DONE);

    ZoomArray.Top    = 0;
    ZoomArray.Left   = S->scr_Width - 128;
    ZoomArray.Width  = 128;
    ZoomArray.Height = S->scr_BarHeight+1;

    W=pOS_OpenWindow(
        SCRTAG_Title,          (ULONG)"UAE - Power: "IND" Disks: "
                                      IND" "IND" "IND" "IND,
        SCRTAG_ScreenTitle,    (ULONG)ScreenTitle,
        SCRTAG_Screen,         (ULONG)S,
        SCRTAG_InnerWidth,     currprefs.gfx_width,
        SCRTAG_InnerHeight,    currprefs.gfx_height,
        SCRTAG_AutoAdjust,     TRUE,
        SCRTAG_UnderMouse,     TRUE,
        SCRTAG_ZipGadget,      TRUE,
        SCRTAG_ZipIBox,        (ULONG)&ZoomArray,
        SCRTAG_Flags,          WINFLGF_DepthGadget|
                               WINFLGF_SimpleRefresh|
                               WINFLGF_CloseGadget|
                               WINFLGF_Dragbar|
                               WINFLGF_ReportMouse|
                               WINFLGF_RMBTrap|
//                             WINFLGF_Activate|
                               0,
        SCRTAG_IDCMP,          IDCMP_MouseButtons|
                               IDCMP_MouseMove|
                               IDCMP_RawKey|
                               IDCMP_ActiveWindow|
                               IDCMP_InactiveWindow|
                               IDCMP_CloseWindow|
                               IDCMP_RefreshWindow|
                               IDCMP_NewSize|
                               IDCMP_DragDrop|
                               0,
        SCRTAG_Gadget,         (ULONG)DDGad,
        TAG_DONE);
    pOS_UnlockPubScreen(S);
    if(!W) {
        fprintf(stderr,"Can't open window !\n");
        return 0;
    }

    /* now we know the window box and can setup the drag&drop gad. */
    {
        struct pOS_IBox Box;

        pOS_GetWindowIBox(W, &Box, NULL);
        --Box.Left;
        --Box.Top;
        
        DDGad->gad_LeftEdge = Box.Left;
        DDGad->gad_TopEdge  = Box.Top;
        DDGad->gad_Width    = -(W->win_BorRight + W->win_BorLeft) + 2;
        DDGad->gad_Height   = -(W->win_BorBottom + W->win_BorTop) + 2;
        /*
        pOS_DrawBorderBox(W->win_RastPort, S->scr_DrawInfo, 
                          (W->win_Flags&WINFLGF_Activate)?
                          GADBORTYP_BevelDownBox:GADBORTYP_HfBevelDownBox,
                          &Box);
        */
    }

    /* Give priority 0 when active, -1 when inactive */
    pOS_SetWindowTask(W, pOS_FindTask(NULL), 0, -1);

    gfxvidinfo.width  = (W->win_Width  - W->win_BorRight - W->win_BorLeft);
    gfxvidinfo.height = (W->win_Height - W->win_BorTop - W->win_BorBottom);
    XOffset = W->win_BorLeft;
    YOffset = W->win_BorTop;

    RP = W->win_RastPort;

    return 1;
}

/****************************************************************************/

#ifndef HAVE_SIGACTION /* test for -nixpos */
double frexp(double value, int *exp)
{
    *exp = 0;
    if(value>1)       while(value>1)  {value /= 2;++*exp;}
    else if(value>0)  while(value<1)  {value *= 2;--*exp;}
    else if(value<-1) while(value<-1) {value /= 2;++*exp;}
    else if(value<0)  while(value>-1) {value *= 2;--*exp;}
    return value;
}

double ldexp(double value, int exp)
{
    if(exp<0) while(exp++) value /= 2;
    else      while(exp--) value *= 2;
    return value;
}
#endif

/****************************************************************************/

int graphics_setup(void)
{
#ifdef HAVE_SIGACTION /* test for -nixpos */
    if(ix_os != OS_IS_POS) {
        ix_req(NULL, "Abort", NULL, "That version of %s is only for p.OS!", __progname);
        exit(20);
    }    
#endif
    gb_IntuiBase = (void*)pOS_OpenLibrary("pintui.library",0);
    if(!gb_IntuiBase) {
        fprintf(stderr,"No pintui.library ?\n");
        return 0;
    }
    gb_GfxBase   = (void*)pOS_OpenLibrary("pgraphics.library",0);
    if(!gb_GfxBase) {
        fprintf(stderr,"No pgraphics.library ?\n");
        return 0;
    }
    gb_DtTypeBase =(void*)pOS_OpenLibrary("pDtType.library",0);
    if(!gb_DtTypeBase) {
        fprintf(stderr,"No pDtType.library ?\n");
        return 0;
    }
    initpseudodevices();    
    atexit(graphics_leave);
    return 1;
}

/****************************************************************************/

int graphics_init(void)
{
    int i;

    if (currprefs.color_mode > 3) {
        fprintf(stderr, "Bad color mode selected. Using default.\n"); 
        currprefs.color_mode = 3;
    }
    if (currprefs.gfx_width < 320)
        currprefs.gfx_width = 320;
    if (currprefs.gfx_height < 100)
        currprefs.gfx_height = 100;
    currprefs.gfx_width += 7;
    currprefs.gfx_width &= ~7;
    
    gfxvidinfo.width  = currprefs.gfx_width;
    gfxvidinfo.height = currprefs.gfx_height;

    if(!setup_publicscreen()) return 0;
    
    switch(currprefs.color_mode) {
    case 0: case 1: case 2:
        gfxvidinfo.pixbytes = 1;        
        CM=(void*)pOS_AllocColorMap(NULL,
                                    GFXTAG_ColorType,   COLMAPTYP_StdRLut,
                                    GFXTAG_Colors,      256,
                                    currprefs.color_mode==2?TAG_IGNORE:
                                    GFXTAG_ColorFlags,  COLMAPF_Q1,
                                    TAG_END);
        if(CM)
        GM8=(void*)pOS_AllocGfxMap(NULL,
                                   GFXTAG_Width,       currprefs.gfx_width,
                                   GFXTAG_Height,      1,
                                   GFXTAG_GfxType,     GFXMAPTYP_StdChunky8,
                                   GFXTAG_CrtGfxMapEx, TRUE,
                                   GFXTAG_ColorMap,    (ULONG)CM,
                                   TAG_END);
        if(GM8) break;
        write_log("trying 24bit GfxMap...\n");
        /* fall through */
      case 3:  /* 24 bits */
        gfxvidinfo.pixbytes = 2;
        GM24=(void*)pOS_AllocGfxMap(NULL,
                                    GFXTAG_Width,    currprefs.gfx_width,
                                    GFXTAG_Height,   1,
                                    GFXTAG_GfxType,  GFXMAPTYP_StdRGB8,
                                    TAG_END);       
        break;
    }
    
    if(!GM8 && !GM24) {
        fprintf(stderr,"Unable to get GfxMap.\n");
        return 0;
    }

    gfxvidinfo.emergmem = 0;
    gfxvidinfo.linemem = 0;
    gfxvidinfo.rowbytes = gfxvidinfo.pixbytes * currprefs.gfx_width;
    gfxvidinfo.bufmem = (char *)calloc(gfxvidinfo.rowbytes, 
                                       currprefs.gfx_height);
    if(!gfxvidinfo.bufmem) {
        fprintf(stderr,"Not enough memory for video bufmem.\n");
        return 0;
    } 
    gfxvidinfo.maxblocklines = 0;

    if (currprefs.use_low_bandwidth) {
        oldpixbuf = (char *)calloc(gfxvidinfo.rowbytes,
                                   currprefs.gfx_height);
        if(!gfxvidinfo.bufmem) {
            write_log("Not enough mem for low-bandwidth!\n");
            currprefs.use_low_bandwidth = 0;
        } else gfxvidinfo.maxblocklines = currprefs.gfx_height-1;
    }

    set_title();

    if (!init_colors()) {
        write_log("Failed to init colors.\n");
        return 0;
    }

    buttonstate[0] = buttonstate[1] = buttonstate[2] = 0;
    for(i=0; i<256; i++)
        keystate[i] = 0;
    
    lastmx = lastmy = 0; 
    newmousecounters = 0;
    inwindow = 0;

    rexx_init();

    pOS_ActivateWindow(W);

    return 1;
}

/****************************************************************************/

void graphics_leave(void)
{
    rexx_exit();
    closepseudodevices();
    if(GM8) {
        pOS_FreeGfxMap(&GM8->ckgm_Gfx);
        GM8 = NULL;
    } 
    if(GM24) {
        pOS_FreeGfxMap(&GM24->rgbgm_Gfx);
        GM24 = NULL;
    } 
    if(CM) {
        pOS_FreeColorMap(&CM->rlcm_CM);
        CM = NULL;
    }
    if(W) {
        pOS_CloseWindow(W);
        W = NULL;
    }
    if(DDGad) {
        pOS_DisposeIObject(DDGad);
        DDGad = NULL;
    }
    if(gb_DtTypeBase) {
        pOS_CloseLibrary((void*)gb_DtTypeBase);
        gb_DtTypeBase = NULL;
    }
    if(gb_PSLBase) {
        pOS_CloseLibrary((void*)gb_PSLBase);
        gb_PSLBase = NULL;
    }
    if(gb_IntuiBase) {
        pOS_CloseLibrary((void*)gb_IntuiBase);
        gb_IntuiBase = NULL;
    }
    if(gb_GfxBase) {
        pOS_CloseLibrary((void*)gb_GfxBase);
        gb_GfxBase = NULL;
    }
}


/***************************************************************************/

void handle_events(void)
{
    struct pOS_IntuiMessage *msg;
    int mx,my,class,code;
  
    newmousecounters = 0;

    if(pOS_SetSignal(0L, DOSSIGF_CTRL_C|DOSSIGF_CTRL_D) & 
                        (DOSSIGF_CTRL_C|DOSSIGF_CTRL_D)) {
        activate_debugger();
    }

    while((msg=(void*)pOS_GetMsg(W->win_UserPort))) {
        class = msg->im_Class;
        code  = msg->im_Code;
        mx    = msg->im_MouseX;
        my    = msg->im_MouseY;

        mx -= XOffset;
        my -= YOffset;

        switch(class) {
          case IDCMP_DragDrop:
            if(code == IECODE_DROP_WORK) { /* object is dropped */
                struct pOS_IEDragDrop *DD;
                
                DD    = &((struct pOS_ISMDrop*)msg->im_Data)->ismd_IEDD;
                HandleDrop(DD);
                pOS_ActivateWindow(W);
            }
            break;

          case IDCMP_NewSize:
            /* where is WFLG_ZOOMED for pOS ? */
            inhibit_frame = W->win_Height<50?1:0;
            set_title();
            break;

          case IDCMP_RefreshWindow: 
            if(currprefs.use_low_bandwidth) {
                /* hack: this forces refresh */
                char *ptr = oldpixbuf;
                int i, len = gfxvidinfo.rowbytes;
                for(i=0;i<gfxvidinfo.height;++i) {
                    ptr[00000] ^= 255;
                    ptr[len-1] ^= 255;
                    ptr += len;
                }
            }
            pOS_BeginRefresh(W, NULL);
            flush_block(0,gfxvidinfo.height-1);
            pOS_EndRefresh(W, TRUE);
            set_title();
            break;

          case IDCMP_CloseWindow:
            activate_debugger();
            break;

          case IDCMP_RawKey: {
              int kc       = code&127;
              int released = code&128?1:0;
            
              if(released) {
                  keystate[kc] = 0;
                  record_key ((kc << 1) | 1);
              } else if (!keystate[kc])  {
                  keystate[kc] = 1;
                  record_key (kc << 1);
              }
          } break;
          
          case IDCMP_MouseMove:
            if(inwindow) {
                lastmx = mx;
                lastmy = my;
            } break;

          case IDCMP_MouseButtons:
            if(code==IECODE_LBUTTON)                    buttonstate[0]=1;
            if(code==IECODE_LBUTTON+IECODE_UP_PREFIX)   buttonstate[0]=0;
            if(code==IECODE_MBUTTON)                    buttonstate[1]=1;
            if(code==IECODE_MBUTTON+IECODE_UP_PREFIX)   buttonstate[1]=0;
            if(code==IECODE_RBUTTON)                    buttonstate[2]=1;
            if(code==IECODE_RBUTTON+IECODE_UP_PREFIX)   buttonstate[2]=0;
            break;
        
          case IDCMP_ActiveWindow:
            inwindow = 1;
            newmousecounters = 1;
            set_title();
            break;
                        
          case IDCMP_InactiveWindow:
            inwindow = 0;
            set_title();
            break;
          
          case IDCMP_ChangeWindow:
            set_title();
            break;

          default:
            fprintf(stderr, "Unknown class: 0x%x\n",class);
            break;
        }
        if(msg && !pOS_SysIMessage(msg)) pOS_ReplyMsg(&msg->im_Message);
    }
    /* "Affengriff" */
    if(keystate[AK_CTRL] && keystate[AK_LAMI] && keystate[AK_RAMI]) 
        m68k_reset();

    /* PC-like :-) CTRL-ALT-DEL => reboot */
    if(keystate[AK_CTRL] && (keystate[AK_LALT] || keystate[AK_RALT]) && 
       keystate[AK_DEL]) 
        m68k_reset();

    /* CTRL+LSHIFT+LALT+F10 on amiga => F12 on X11 */
    /*                  F9           => ScrollLock on X11 (inhibit_frame) */
    if(keystate[AK_CTRL] && keystate[AK_LSH] && keystate[AK_LALT]) {
        if(keystate[AK_F10]) togglemouse();
        if(keystate[AK_F9]) {
            inhibit_frame ^= 1;
            set_title();
            if(inhibit_frame) printf("display disabled\n");
            else printf("display enabled\n");
        }
    }
    disk_hotkeys();
    gui_handle_events();
}

/***************************************************************************/

int debuggable(void)
{
    return 1;
}

/***************************************************************************/

int needmousehack(void)
{
    return 1;
}

/***************************************************************************/

void LED(int on)
{
}

/***************************************************************************/

static int led_state[5];

/* prints some text */
static void t_print(char *t, const struct pOS_DrawInfo *di)
{
    pOS_SetABPenDrMd(RP, di->dri_Pens[(W->win_Flags&WINFLGF_Activate)?
                                      SCRPEN_ActiveWinTitle:
                                      SCRPEN_InactiveWinTitle],
                         di->dri_Pens[(W->win_Flags&WINFLGF_Activate)?
                                      SCRPEN_ActiveWindow:
                                      SCRPEN_InactiveWindow],
                         DRMD_Jam2);
    pOS_DrawText(RP, t, strlen(t));
/*
    RP->rp_CP.X += pOS_TextLength(RP, t, strlen(t));
*/
}

/* display one led */
static void t_led(ULONG BGR, const struct pOS_DrawInfo *di)
{
    struct pOS_Rectangle rect;
    int x,y,oy;

    oy = RP->rp_CP.Y;
    x  = RP->rp_CP.X;
    y  = (W->win_BorTop - 6)/2;

    rect.MinX = x;
    rect.MaxX = x+pOS_TextLength(RP,IND,strlen(IND))-1;
    rect.MinY = y;
    rect.MaxY = y+5;

    if(BGR<=0x100000)
    pOS_SetAPenR(RP, (struct pOS_RColor*)&BGR);
    else
    pOS_SetAPen(RP, di->dri_Pens[(W->win_Flags&WINFLGF_Activate)?
                                 SCRPEN_ActiveWindow:SCRPEN_InactiveWindow]);

    pOS_DrawRectFill(RP, rect.MinX+1,rect.MinY+1, rect.MaxX-1,rect.MaxY-1);
    pOS_DrawBorderBox(RP, di, (W->win_Flags&WINFLGF_Activate)?
                              GADBORTYP_BevelDownBox:GADBORTYP_HfBevelDownBox,
                      &rect);
    RP->rp_CP.X = rect.MaxX+1;
    RP->rp_CP.Y = oy;
}

static void set_title(void)
{
    int drk, red, grn;
    const struct pOS_DrawInfo *di = W->win_ICI->ici_DrawInfo;

    if(W->win_Width<=128) return; /* iconified: no txt */

    pOS_SetPosition(RP, 25, (W->win_BorTop - RP->rp_TxHeight)/2
                            + RP->rp_TxBaseline);


    red = (W->win_Flags&WINFLGF_Activate)?255:190;
    grn = ((W->win_Flags&WINFLGF_Activate)?255:190)<<8;
    drk = 0x1000000/*0xAAAAAA*/;

    t_print("UAE - Power: ", di);
    t_led(led_state[0]?red:drk, di);
    t_print(" Disks: ", di);
    t_led(led_state[1]?grn:drk, di);
    t_print(" ", di);
    t_led(led_state[2]?grn:drk, di);
    t_print(" ", di);
    t_led(led_state[3]?grn:drk, di);
    t_print(" ", di);
    t_led(led_state[4]?grn:drk, di);
}

/****************************************************************************/

void main_window_led(int led, int on)                /* is used in amigui.c */
{
  if(led>=0 && led<=4) led_state[led] = on;
  set_title();
}

/****************************************************************************/

static void unrecord(int kc)
{
    keystate[kc] = 0;
    record_key ((kc << 1) | 1);
}

/****************************************************************************/

static void disk_hotkeys(void)
{
    struct pOS_PslFileRequester *FileRequest;
    char buff[80];
    int drive;
    char *last_file,*last_dir,*s;

    if(!(keystate[AK_CTRL] && keystate[AK_LALT])) return;

    /* CTRL-LSHIFT-LALT F1-F4 => eject_disk */
    if(keystate[AK_LSH]) {
        int ok = 0;

        if(keystate[AK_F1]) {ok=1;disk_eject(0);
                             printf("drive DF0: ejected\n");}
        if(keystate[AK_F2]) {ok=1;disk_eject(1);
                             printf("drive DF1: ejected\n");}
        if(keystate[AK_F3]) {ok=1;disk_eject(2);
                             printf("drive DF2: ejected\n");}
        if(keystate[AK_F4]) {ok=1;disk_eject(3);
                             printf("drive DF3: ejected\n");}

        if(ok) {
            unrecord(AK_CTRL);unrecord(AK_LALT);unrecord(AK_LSH); 
            unrecord(AK_F1);unrecord(AK_F2);  
            unrecord(AK_F3);unrecord(AK_F4);
        }
        return;
    }

    /* CTRL-LALT F1-F4 => insert_disk */
    if(keystate[AK_F1])      {drive = 0;unrecord(AK_F1);}
    else if(keystate[AK_F2]) {drive = 1;unrecord(AK_F2);}
    else if(keystate[AK_F3]) {drive = 2;unrecord(AK_F3);}
    else if(keystate[AK_F4]) {drive = 3;unrecord(AK_F4);}
    else return;
    unrecord(AK_CTRL);unrecord(AK_LALT); 

    switch(drive) {
      case 0: case 1: case 2: case 3: last_file = currprefs.df[drive]; break;
      default: return;
    }

    split_dir_file(from_unix_path(last_file), &last_dir, &last_file);
    if(!last_file) return;
    if(!last_dir)  return;

    if(!gb_PSLBase) gb_PSLBase=(void*)pOS_OpenLibrary("psl.library",0);
    if(!gb_PSLBase) {
        write_log("Can't open psl.library!\n");
        return;
    }

    FileRequest = pOS_AllocFileRequest(TAG_DONE);
    if(!FileRequest) {
        write_log("Unable to allocate file requester.\n");
        return;
    }

    sprintf(buff,"Select file to use for drive DF%d:",drive);
    if(pOS_PslFileRequest(FileRequest,
                      PSLFRTAG_Title,          (ULONG)buff,
                      PSLFRTAG_PathPart,       (ULONG)last_dir,
                      PSLFRTAG_FilePart,       (ULONG)last_file,
                      PSLFRTAG_Pattern,        (ULONG)"(#?.ad(f|z)#?|df?|?)",
                      PSLFRTAG_PatternGad,     TRUE,
                      TAG_DONE)) {
        free(last_file);
        last_file = malloc(3 + strlen(FileRequest->pfr_Path) +
                           strlen(FileRequest->pfr_File));
        if(last_file) {
            s = last_file;
            strcpy(s,FileRequest->pfr_Path);
            if(*s && !(s[strlen(s)-1]==':' || s[strlen(s)-1]=='/')) 
                strcat(s,"/");
            strcat(s,FileRequest->pfr_File);
            last_file = to_unix_path(s);free(s);
        }
    } else {
        free(last_file);
        last_file = NULL;
    }
    pOS_FreePslRequest(FileRequest);
    free(last_dir);

    if(last_file) {
        disk_insert(drive,last_file);
        free(last_file);
    }
}

/****************************************************************************/

ULONG pOS_GetObjectData(APTR obj,ULONG tag)
{
  struct pOS_IntuiMethod Mth;
  ULONG Dt;

  Mth.imth_Method=ICLMTH_Get;
  Mth.imth_U.imth_Get.imgt_ID=tag;
  Mth.imth_U.imth_Get.imgt_Variable=&Dt;
  pOS_DoIMethodA(obj,(struct pOS_Method*)&Mth);
  return(Dt);
}

/****************************************************************************/

#define SIZE 1024

static void HandleDrop(struct pOS_IEDragDrop *dd)
{
    char *Type = (char*)pOS_GetObjectData(dd->iedd_Object, DCLTAG_Type);
    if(!stricmp(Type, pOS_DTFORM_DOSOBJ)) {
        char *Buffer = (CHAR*)pOS_AllocMem(2*SIZE, MEMF_ANY);
        if( Buffer ) {
            struct pOS_DtTypeDosObj DosObj = {NULL, 0, NULL};
            struct pOS_DClassInfo  DCI     = {
                pOS_DTFORM_DOSOBJ, NULL, DCLINFOF_None, NULL
            };
            
            pOS_DCLASSINFO(&DCI);
            DCI.dci_Data = &DosObj;
            
            DosObj.dtdos_Buffer   = Buffer+SIZE;
            DosObj.dtdos_BufSize  = SIZE;
            strcpy(DosObj.dtdos_Buffer, "run ");
            DosObj.dtdos_Buffer  += strlen(DosObj.dtdos_Buffer);
            DosObj.dtdos_BufSize -= strlen(DosObj.dtdos_Buffer);

            if(pOS_LoadDTObjectA(dd->iedd_Object, &DCI, NULL)) {
                if(DCI.dci_Result > 0) {
                    if(DosObj.dtdos_Lock) {
                        strcpy(Buffer, "cd ");
                        
                        pOS_NameFromObjectLock(DosObj.dtdos_Lock,
                                               Buffer+3, SIZE-3-1);
                        uaeexe(Buffer);
                        uaeexe(DosObj.dtdos_Buffer);
                    } else {
                        uaeexe(DosObj.dtdos_Buffer);
                    }
                }
            }
        }
        pOS_FreeMem(Buffer, 2*SIZE);
    }
}

/***************************************************************************/

/* sam: need to put all this in a separate module */

#ifdef PICASSO96

void DX_Invalidate (int first, int last)
{
}

int DX_BitsPerCannon (void)
{
    return 8;
}

void DX_SetPalette(int start, int count)
{
}

int DX_FillResolutions (uae_u16 *ppixel_format)
{
    return 0;
}

void gfx_set_picasso_modeinfo (int w, int h, int depth)
{
}

void gfx_set_picasso_baseaddr (uaecptr a)
{
}

void gfx_set_picasso_state (int on)
{
}

void begindrawing (void)
{
}

void enddrawing (void)
{
}

uae_u8 *lockscr (void)
{
return NULL;
}

void unlockscr (void)
{
}
#endif


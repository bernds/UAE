 /*
  * UAE - The Un*x Amiga Emulator
  * 
  * pos-rexx.c
  * 
  * Copyright 1997 Samuel Devulder.
  */

int  rexx_init(void);
void rexx_exit(void);
void rexx_led(int led, int on);
void rexx_filename(int num, char *name);
void rexx_handle_events(void);

int  rexx_init(void) {return 0;}
void rexx_exit(void) {}
void rexx_led(int led, int on) {}
void rexx_filename(int num, char *name) {}
void rexx_handle_events(void) {}

#if 0 /* nothing yet on pOS for rexx */

#include <proto/exec.h>
#include <proto/alib.h>
#include <proto/rexxsyslib.h>
#include <proto/dos.h>

#include <rexx/storage.h>
#include <rexx/errors.h>

/* this prevent a conflict between <rexx/rexxio.h> and <sys/dirent.h> */
#undef DT_DIR 

/****************************************************************************/

#include "sysconfig.h"
#include "sysdeps.h"

#include "uae.h"
#include "config.h"
#include "options.h"
#include "../include/memory.h" /* or else gcc includes machdep/memory.h */
#include "custom.h"
#include "readcpu.h"
#include "newcpu.h"
#include "disk.h"
#include "gui.h"
#include "os.h"

#include <ctype.h>

/****************************************************************************/

#define UAE_PORTNAME    "UAE"
#define RESULT_LEN      1024
#define PORT_LEN        80
#define CMD_LEN         1024

typedef struct
{
    char port[PORT_LEN];
    char cmd_on[CMD_LEN];
    char cmd_off[CMD_LEN];
} gui_rexx_s;

enum {LED_POW, LED_DF0, LED_DF1, LED_DF2, LED_DF3,
      NAME_DF0, NAME_DF1, NAME_DF2, NAME_DF3,
      ON_EXIT,
      GUI_REXX_MAX};

/****************************************************************************/

int  rexx_init(void);
void rexx_exit(void);
void rexx_led(int led, int on);
void rexx_filename(int num, char *name);
void rexx_handle_events(void);

/****************************************************************************/

struct /*RxsLib*/ Library
                        *RexxSysBase;
static struct MsgPort   *ARexxPort;
static gui_rexx_s        gui_rexx[GUI_REXX_MAX];
static char              RESULT[RESULT_LEN];
static int               led_state[5];

static int   ADDRESS(char *hostname, char *cmd);
static int   matchstr(char **line,char *pat);
static void  extractstr(char **line, char *result, int len);
static int   matchnum(char **line);

/****************************************************************************/

extern int quit_program;                                 /* ami-gui.c */
extern void activate_debugger(void);                     /* debug.c */
extern char *to_unix_path(char *s);                      /* ami-disk.c */
extern char *from_unix_path(char *s);                    /* ami-disk.c */

/****************************************************************************/

int rexx_init(void)
{
    quit_program = 0;
    RexxSysBase = (void*)OpenLibrary("rexxsyslib.library",0L);
    if(!RexxSysBase) {
        fprintf(stderr, "Can't find rexxsyslib.library!\n");
        return 1;
    }
    if(FindPort(UAE_PORTNAME)) {
        fprintf(stderr, "Port \"%s\" already exists!\n",UAE_PORTNAME);
        return 1;    
    }
    ARexxPort = CreatePort(UAE_PORTNAME,0);
    if(!ARexxPort) {
        fprintf(stderr, "Failed to open AREXX port \"%s\"!\n",UAE_PORTNAME);
        return 1;
    }
    fprintf(stderr,"Rexx port \"%s\" installed.\n", UAE_PORTNAME);
    rexx_handle_events();
    return 0;
}

/****************************************************************************/

void rexx_exit(void)
{
    if(ARexxPort) {
        struct RexxMsg *msg;
        gui_rexx_s *gui = &gui_rexx[ON_EXIT];

        if(gui->port[0] && gui->cmd_on[0]) {
            if(ADDRESS(gui->port, gui->cmd_on) != RC_OK) {
                fprintf(stderr,"%s:%s:%s\n", gui->port,
                                             gui->cmd_on,
                                             RESULT);
            }
            gui->port[0] = '\0';
         }
        Forbid();
        while((msg = (struct RexxMsg*)GetMsg(ARexxPort))) {
           msg->rm_Result1 = RC_ERROR;
           msg->rm_Result2 = NULL;
           ReplyMsg((void*)msg);
        }
        DeletePort(ARexxPort);
        Permit();
        ARexxPort = NULL;
    }
    if(RexxSysBase) {
        CloseLibrary((void*)RexxSysBase);
        RexxSysBase = NULL;
    }
}

/****************************************************************************/

static int EJECT(char *line)
{
    int drive = matchnum(&line);
    if(drive<0 || drive>3) return RC_WARN;
    disk_eject(drive);
    sprintf(RESULT,"Drive %d ejected",drive);
    return RC_OK;
}

/****************************************************************************/

static int INSERT(char *line)
{
    int drive = matchnum(&line);
    if(drive<0 || drive>3) return RC_WARN;
    if(disk_empty(drive)) {
        char buff[256];
        char *name;
        extractstr(&line, buff, 256);
        name = to_unix_path(buff);
        disk_insert(drive, name);
        free(name);
    } else {
        sprintf(RESULT,"Drive %d not empty!",drive);
        return RC_WARN;
    }
    return RC_OK;
}

/****************************************************************************/

static void QUIT(void)
{
    broken_in = 1;
    regs.spcflags |= SPCFLAG_BRK;
    quit_program = 1;
}

/****************************************************************************/

static int QUERY(char *line)
{
    char *res = NULL;
    int   alc = 0;

    if     (matchstr(&line, "LED_POW"))  res = led_state[0]?"1":"0";
    else if(matchstr(&line, "LED_DF0"))  res = led_state[1]?"1":"0";
    else if(matchstr(&line, "LED_DF1"))  res = led_state[2]?"1":"0";
    else if(matchstr(&line, "LED_DF2"))  res = led_state[3]?"1":"0";
    else if(matchstr(&line, "LED_DF3"))  res = led_state[4]?"1":"0";
    else if(matchstr(&line, "NAME_DF0")) res = from_unix_path(df0), alc = 1;
    else if(matchstr(&line, "NAME_DF1")) res = from_unix_path(df1), alc = 1;
    else if(matchstr(&line, "NAME_DF2")) res = from_unix_path(df2), alc = 1;
    else if(matchstr(&line, "NAME_DF3")) res = from_unix_path(df3), alc = 1;
    else if(matchstr(&line, "FAKEJOYSTICK")) res = fake_joystick?"1":"0";
    else if(matchstr(&line, "DISPLAY"))  res = inhibit_frame?"0":"1";
    else if(matchstr(&line, "FRAMERATE")) {
        sprintf(RESULT,"%d",framerate);
        return RC_OK;
    } else if(matchstr(&line, "SOUND")) {
        sprintf(RESULT,"%d",sound_available?produce_sound:-1);
        return RC_OK;
    } 
    else return RC_ERROR;

    if(res) {
        strncpy(RESULT, res, RESULT_LEN);
        if(alc) free(res);
    }
    return RC_OK;
}

/****************************************************************************/

static int FEEDBACK(char *line)
{
    gui_rexx_s *gui = NULL;

    if     (matchstr(&line,"LED_POW"))  gui = &gui_rexx[LED_POW];
    else if(matchstr(&line,"LED_DF0"))  gui = &gui_rexx[LED_DF0];
    else if(matchstr(&line,"LED_DF1"))  gui = &gui_rexx[LED_DF1];
    else if(matchstr(&line,"LED_DF2"))  gui = &gui_rexx[LED_DF2];
    else if(matchstr(&line,"LED_DF3"))  gui = &gui_rexx[LED_DF3];
    else if(matchstr(&line,"NAME_DF0")) gui = &gui_rexx[NAME_DF0];
    else if(matchstr(&line,"NAME_DF1")) gui = &gui_rexx[NAME_DF1];
    else if(matchstr(&line,"NAME_DF2")) gui = &gui_rexx[NAME_DF2];
    else if(matchstr(&line,"NAME_DF3")) gui = &gui_rexx[NAME_DF3];
    else if(matchstr(&line,"ON_EXIT"))  gui = &gui_rexx[ON_EXIT];
    else return RC_ERROR;

    while(1) {
        if(matchstr(&line, "ADDRESS") ||
           matchstr(&line, "PORT")) {
            extractstr(&line, gui->port, PORT_LEN);
        } else if(matchstr(&line,"COMMAND") ||
                  matchstr(&line,"CMD") ||
                  matchstr(&line,"CMD_ON")) {
            extractstr(&line, gui->cmd_on,  CMD_LEN);
        } else if(matchstr(&line,"CMD_OFF")) {
            extractstr(&line, gui->cmd_off, CMD_LEN);
        } else break;
    }
    return RC_OK;
}

/****************************************************************************/

static int VERSION(char *line)
{
    if(matchstr(&line,"STRING")) {
        sprintf(RESULT,
                "UAE-%d.%d.%d © by Bernd Schmidt & contributors, "
                "Amiga Port by Samuel Devulder.",
                (version / 100) % 10, (version / 10) % 10, version % 10);
    } else if(matchstr(&line,"NUM")) {
        sprintf(RESULT,"%d",version);
    } else if(matchstr(&line,"AUTHOR")) {
        sprintf(RESULT,"© by Bernd Schmidt & contributors");
    } else if(matchstr(&line,"PORT")) {
        sprintf(RESULT,"Amiga Port by Samuel Devulder");
    } else return RC_ERROR;
    return RC_OK;
}

/****************************************************************************/

static int FRAMERATE(char *line)
{
    int num;
    num = matchnum(&line);
    if(num>=1 && num<=20) {
        framerate = num;
    } else {
        sprintf(RESULT,"Invalid frame rate: %d\n", num);
        return RC_WARN;
    }
    return RC_OK;
}

/****************************************************************************/

static int FAKEJOYSTICK(char *line)
{
    if     (matchstr(&line,"ON"))     fake_joystick = 1;
    else if(matchstr(&line,"OFF"))    fake_joystick = 0;
    else if(matchstr(&line,"TOGGLE")) fake_joystick = fake_joystick?0:1;
    else return RC_ERROR;
    return RC_OK;
}

/****************************************************************************/

static int DISPLAY(char *line)
{
    if     (matchstr(&line,"ON"))     inhibit_frame = 0;
    else if(matchstr(&line,"OFF"))    inhibit_frame = 1;
    else if(matchstr(&line,"TOGGLE")) inhibit_frame = inhibit_frame?0:1;
    else return RC_ERROR;
    return RC_OK;
}

/****************************************************************************/

static int SOUND(char *line)
{
    if(!sound_available) {
        sprintf(RESULT,"Sound not available!");
        return RC_WARN;
    }

    if     (matchstr(&line,"ON"))     produce_sound = 2;
    else if(matchstr(&line,"OFF"))    produce_sound = 1;
    else if(matchstr(&line,"BEST"))   produce_sound = 3;
    else if(matchstr(&line,"TOGGLE")) produce_sound = produce_sound<=1?2:1;
    else return RC_ERROR;
    return RC_OK;
}

/****************************************************************************/

static int process_cmd(char *line)
{
    RESULT[0] = '\0';
    if     (matchstr(&line, "EJECT"))        return EJECT(line);
    else if(matchstr(&line, "INSERT"))       return INSERT(line);
    else if(matchstr(&line, "QUERY"))        return QUERY(line);
    else if(matchstr(&line, "FEEDBACK"))     return FEEDBACK(line);
    else if(matchstr(&line, "VERSION"))      return VERSION(line);
    else if(matchstr(&line, "BYE"))          QUIT();
    else if(matchstr(&line, "QUIT"))         QUIT();
    else if(matchstr(&line, "DEBUG"))        activate_debugger();
    else if(matchstr(&line, "RESET"))        m68k_reset();
    else if(matchstr(&line, "DISPLAY"))      return DISPLAY(line);
    else if(matchstr(&line, "FRAMERATE"))    return FRAMERATE(line);
    else if(matchstr(&line, "FAKEJOYSTICK")) return FAKEJOYSTICK(line);
    else if(matchstr(&line, "SOUND"))        return SOUND(line);
    else return RC_ERROR;
    return RC_OK;
}

/****************************************************************************/

void rexx_handle_events(void)
{
    struct RexxMsg *msg;
    while((msg = (struct RexxMsg*)GetMsg(ARexxPort))) {
        if(!(msg->rm_Action & RXCOMM)) {
            fprintf(stderr,"Unknown action '%08X' recieved!\n",
                    msg->rm_Action);
            continue;
        }
        msg->rm_Result1 = process_cmd(msg->rm_Args[0]);
        msg->rm_Result2 = NULL;
        if(msg->rm_Action & RXFF_RESULT) {
            msg->rm_Result2 = (LONG)CreateArgstring(RESULT,strlen(RESULT));
        }
        ReplyMsg((void*)msg);
    }
}

/****************************************************************************/

void rexx_led(int led, int on)
{
    gui_rexx_s *gui = NULL;

    if(led < 0 || led > 4) return;
    led_state[led] = on;

    if(led == 0) gui = &gui_rexx[LED_POW];
    if(led == 1) gui = &gui_rexx[LED_DF0];
    if(led == 2) gui = &gui_rexx[LED_DF1];
    if(led == 3) gui = &gui_rexx[LED_DF2];
    if(led == 4) gui = &gui_rexx[LED_DF3];

    if(gui->port[0] && gui->cmd_on[0] && gui->cmd_off[0]) {
        if(ADDRESS(gui->port, on ? gui->cmd_on : gui->cmd_off) != RC_OK) {
            fprintf(stderr,"%s:%s:%s\n", gui->port,
                                         on ? gui->cmd_on : gui->cmd_off,
                                         RESULT);
        }
    }
}

/****************************************************************************/

void rexx_filename(int num, char *filename)
{
    gui_rexx_s *gui = NULL;
    if(num < 0 || num > 3) return;
    gui = &gui_rexx[NAME_DF0 + num];
    if(gui->port[0] && gui->cmd_on[0]) {
        char buf[CMD_LEN];
        if(!(filename = from_unix_path(filename))) return;
        sprintf(buf, gui->cmd_on, filename);
        free(filename);
        if(ADDRESS(gui->port, buf) != RC_OK) {
            fprintf(stderr,"%s:%s:%s\n", gui->port, buf, RESULT);
        }
    }
}

/****************************************************************************/
/* send a message to an AREXX port.
 */
static int ADDRESS(char *hostname, char *cmd)
{
    struct MsgPort *RexxPort,
                   *ReplyPort;
    struct RexxMsg *HostMsg,
                   *answer;
    int result = RC_WARN;

    if(!stricmp(hostname, "COMMAND")) {
        return SystemTagList(cmd,NULL);
    }

    if((RexxPort = (void *)FindPort(hostname))) {
        if((ReplyPort = (void *)CreateMsgPort())) {
            if((HostMsg = CreateRexxMsg(ReplyPort, NULL, hostname))) {
                if((HostMsg->rm_Args[0] = CreateArgstring(cmd,strlen(cmd)))) {
                    HostMsg->rm_Action  = RXCOMM | RXFF_RESULT;
                    PutMsg(RexxPort, (void*)HostMsg);
                    WaitPort(ReplyPort);
                    while(!(answer = (void *)GetMsg(ReplyPort)));
                    result = answer->rm_Result1;
                    if(result == RC_OK) {
                        if(answer->rm_Result2) {
                        strncpy(RESULT,(char *)answer->rm_Result2,RESULT_LEN);
                        DeleteArgstring((char *)answer->rm_Result2);
                        } else RESULT[0] = '\0';
                    }
                    DeleteArgstring(HostMsg->rm_Args[0]);
                } else strcpy(RESULT, "Can't create argstring!");
                DeleteRexxMsg(HostMsg);
            } else strcpy(RESULT, "Can't create rexx message!");
            DeleteMsgPort(ReplyPort);
        } else strcpy(RESULT, "Can't alloc reply port!");
    } else sprintf(RESULT, "Port \"%s\" not found!",hostname);
    return result;
}

/****************************************************************************/
/* argument parsing routines
 */
static int matchstr(char **line,char *pat)
{
    char *s = *line;
    char match = 0;
    while(isspace(*s)) ++s;
    if(*s=='\"' || *s== '\'') match = *s++;
    while(*s && (tolower(*s)==tolower(*pat)) && (!match || *s!=match)) {++s;++pat;}
    if(match && *s==match && s[1]) ++s;
    if(!*pat && (!*s || isspace(*s))) {
        while(isspace(*s)) ++s;
        *line = s;
        return 1;
    }
    return 0;
}

/****************************************************************************/

static void extractstr(char **line, char *result, int len)
{
    char *s    = *line;
    char match = 0;

    while(isspace(*s)) ++s;

    if(*s=='\"' || *s=='\'') match = *s++;
    while(*s && *s != match) {
        if(*s == '\\' && (s[1] == '\'' || s[1] == '\"')) ++s;
        if(len > 1) {*result++ = *s;--len;}
        ++s;
        if(!match && isspace(*s)) break;
    }
    if(match && *s == match) ++s;
    while(isspace(*s)) ++s;

    *result  = '\0';
    *line    = s;
}

/****************************************************************************/

static int matchnum(char **line)
{
    char *s = *line, match = 0;
    int sign = 1, num = 0;

    while(isspace(*s)) ++s;
    if(*s=='\"' || *s=='\'') match = *s++;
    if(*s=='-') {sign = -1;++s;}
    if(*s=='+') ++s;
    while(isspace(*s)) ++s;
    while(*s>='0' && *s<='9') num = num*10 + (*s++ - '0');
    if(match && *s==match && s[1]) ++s;
    while(isspace(*s)) ++s;
    *line = s;
    return sign>0?num:-num;
}

#endif

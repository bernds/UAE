 /* 
  * UAE - The Un*x Amiga Emulator
  * 
  * pos-disk.c: Creates pseudo dev: handler. Copy tracks to rawfile
  * (used in zfile.c).
  * 
  * Copyright 1997, Samuel Devulder.
  */

/****************************************************************************/

#include "sysconfig.h"
#include "sysdeps.h"

#include <ctype.h>
#include <signal.h>

#include "config.h"
#include "options.h"
#include "memory.h"
#include "custom.h"
#include "keyboard.h"
#include "keybuf.h"
#include "disk.h"
#include "debug.h"
#include "gui.h"

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

#define _SIZE_T /* to avoid interference with <sys/types.h> */
#include <pExec/Types.h>
#include <pExec/MsgPort.h>
#include <pExec/Device.h>
#include <pDOS/Lock.h>
#include <pDOS/Files.h>
#include <pDOS/DosSig.h>
#include <pDOS/DosErrors.h>
#include <pDevice/Trackdisk.h>

#include <pInline/pExec2.h>
#include <pInline/pDos2.h>

extern struct pOS_ExecBase        *gb_ExecBase;
extern struct pOS_ExecLibFunction *gb_ExecLib;
extern struct pOS_DosBase         *gb_DosBase;

/****************************************************************************/

char *amiga_dev_path  = "DEV:";
char *ixemul_dev_path = "/dev/";

int readdevice(char *name, char *dst);
void initpseudodevices(void);
void closepseudodevices(void);
char *to_unix_path(char *s);
char *from_unix_path(char *s);
void split_dir_file(char *src, char **dir, char **file);

/****************************************************************************/

static char *pseudo_dev_path = "T:DEV";

static char pseudo_dev_created  = 0;
static char pseudo_dev_assigned = 0;
static char dfx_done[4];

static int device_exists(char *device_name, int device_unit);

/****************************************************************************/
/* support routines to handle unix filename convention 
 */
char *to_unix_path(char *s)
{
    char *t,*r,*u;
    int l;

    for(u=s,l=0;*u;++u,++l) if(*u=='/' || *u==':') l+=2;

    r = t = malloc(1+l);
    if(!r) return NULL;

    for(u=s;*u && *u!=':';++u);
    if(*u) {
        *t++='/';
        while(*s!=':') *t++=*s++;
        *t++='/';++s;
    }
    while(*s=='/') {*t++='.';*t++='.';*t++=*s++;}
    while(*s) {
        if(s[0]=='/' && s[1]=='/') {*t++=*s++;*t++='.';*t++='.';*t++=*s++;}
        else *t++=*s++;
    }
    *t='\0';
    return r;
}

/****************************************************************************/

char *from_unix_path(char *s)
{
    char *t,*r;

    r = t = malloc(strlen(s)+1);
    if(!r) return NULL;

    if(*s=='/') {
        ++s;
        while(*s && *s!='/') *t++=*s++;
        if(*s=='/') {*t++=':';++s;}
    }

    while(*s) {
        if(s[0]=='.' && s[1]=='.') s+=2;
        else *t++=*s++;
    }

    *t='\0';

    return r;
}

/****************************************************************************/

void split_dir_file(char *src, char **dir, char **file)
{   /* note: src is freed() */
    char *s=src;

    while(*s) ++s;
    while(s>src && (*s!=':' && *s!='/')) --s;
    if(*s==':' || *s=='/') {
        *file = my_strdup(s+1);
        s[1]  = '\0';
        *dir  = my_strdup(src);
        free(src);
    } else {
        *file = src;
        *dir  = my_strdup("");
    }
}

/****************************************************************************/
/*
 * Creates peudo DEV:DFx files.
 */
void initpseudodevices(void)
{
    struct pOS_FileLock *lock;
    int i;

    /* check for T: and TMP: */
    lock = pOS_LockObject(NULL, "T:", FILELKACC_Shared|FILELKACC_NoReq);
    if(!lock) {
	pOS_CreateDosAssign("T",NULL,"RAM:",DDTYP_Assign);
    } else pOS_UnlockObject(lock);

    lock = pOS_LockObject(NULL, "TMP:", FILELKACC_Shared|FILELKACC_NoReq);
    if(!lock) {
	pOS_CreateDosAssign("TMP",NULL,"RAM:",DDTYP_Assign);
    } else pOS_UnlockObject(lock);

    pseudo_dev_created  = 0;
    pseudo_dev_assigned = 0;
    for(i=0;i<4;++i) dfx_done[i]=0;

    /* check if dev: already exists */
    lock = pOS_LockObject(NULL, amiga_dev_path, 
			  FILELKACC_Shared|FILELKACC_NoReq);
    if(!lock) {
        char name[80];
	lock = pOS_LockObject(NULL, pseudo_dev_path, 
			      FILELKACC_Shared|FILELKACC_NoReq);
        if(!lock) {
            /* create it */
            lock = pOS_CreateDirectory(NULL, pseudo_dev_path);
            if(!lock) goto fail;
            pOS_UnlockObject(lock);
	    lock = pOS_LockObject(NULL, pseudo_dev_path, 
				  FILELKACC_Shared|FILELKACC_NoReq);
            pseudo_dev_created = 1;
        }
        strcpy(name,amiga_dev_path);
        if(*name && name[strlen(name)-1]==':') name[strlen(name)-1]='\0';
        if(!pOS_CreateDosAssign(name,lock,NULL, DDTYP_Assign)) {
	    pOS_UnlockObject(lock);
	    goto fail;
	}
        /* the lock is the assign now */
        pseudo_dev_assigned = 1;
    } else pOS_UnlockObject(lock);

    /* Create the dev:DFi entry */
    for(i=0;i<4;++i) if(device_exists("pTrackdisk.device",i)) {
        struct pOS_FileHandle *fd;
        char name[80];

        sprintf(name,"%sDF%d",amiga_dev_path,i);
        fd = pOS_OpenFile(NULL,name,FILEHDMOD_Write);
        if(fd) {pOS_CloseFile(fd);dfx_done[i]=1;}
    }

    return;
 fail:
    fprintf(stderr,"Failed to create pseudo DEV: entry!\n");
}

/****************************************************************************/
/*
 * Cleanup pseudo DEV:DFx
 */
void closepseudodevices(void)
{
    int i;
    for(i=0;i<4;++i) if(dfx_done[i]) {
        char name[80];
        sprintf(name,"%sDF%d",amiga_dev_path,i);
        pOS_DeleteObjectName(NULL, name);
        dfx_done[i] = 0;
    }

    if(pseudo_dev_assigned) {
        char name[80];
        strcpy(name,amiga_dev_path);
        if(*name && name[strlen(name)-1]==':') name[strlen(name)-1]='\0';
        pOS_DeleteDosAssign(name,NULL,0);
        pseudo_dev_assigned = 0;
    }

    if(pseudo_dev_created) {
        pOS_DeleteObjectName(NULL, pseudo_dev_path);
        pseudo_dev_created = 0;
    }
}

/****************************************************************************/
/*
 * checks if a device exists
 */
static int device_exists(char *device_name, int device_unit)
{
    struct pOS_MsgPort port;
    struct pOS_TrackdiskIO *IO;
    int ret = 0;

    if(pOS_ConstructMsgPort(&port)) {
	IO=(void *)pOS_CreateIORequest(&port, sizeof(struct pOS_TrackdiskIO));
	if(IO) {
	    if(!pOS_OpenDevice(device_name, device_unit, 
			       (struct pOS_IORequest*)IO, 0, 0)) {
		pOS_CloseDevice((struct pOS_IORequest*)IO);
		ret = 1;
	    }
	    pOS_DeleteIORequest((struct pOS_IORequest*)IO);
	}
	pOS_DestructMsgPort(&port);
    }
    return ret;
}

/****************************************************************************/
/*
 * extract the device and unit form a filename.
 */
static void extract_dev_unit(char *name, char **dev_name, int *dev_unit)
{
    struct pOS_DosMountDevice *dev = NULL;
    char *s;

    if((s = malloc(strlen(name)+2))) {
	sprintf(s,"%s:",name);
	dev = pOS_GetDosMountName(s);
	free(s);
    } 
    if(dev && dev->dmd_Type == DMDTYP_BOD) {
	*dev_name = strdup(dev->dmd_U.dmd_BOD.dmbod_DevName);
	*dev_unit = dev->dmd_U.dmd_BOD.dmbod_UnitNum;
    } else if((s = strrchr(name,'/'))) { 
        /* pTrackdisk[.device]/0 */
        *dev_unit = atoi(s+1);
        *dev_name = malloc(1 + s-name);
        if(*dev_name) {
            strncpy(*dev_name, name, 1 + s-name);
            (*dev_name)[s-name]='\0';
        }
    } else { 
        /* ?? STRANGEDISK0: ?? */
        *dev_unit = 0;
        *dev_name = strdup(name);
    }
    if(*dev_name) {
        char *s;
        if(!(s = strrchr(*dev_name,'.'))) { 
            /* .device is missing: add it */
            s = malloc(8+strlen(*dev_name));
            if(s) {
                sprintf(s,"%s.device",*dev_name);
                free(*dev_name);
                *dev_name = s;
            }
        }
    }
}

/****************************************************************************/

static int dev_inhibit(char *name, int on)
{
    struct pOS_DosDevice *dev;
    if((dev = pOS_GetDosDevice(NULL, name))) {
	pOS_InhibitDosDevice(dev,on?TRUE:FALSE);
	return 1;
    }
    return 0;
}

/****************************************************************************/
/*
 * copy a device to a FILE
 */
static int raw_copy(char *dev_name, int dev_unit, FILE *dst)
{
    struct pOS_MsgPort Port;
    struct pOS_TrackdiskIO *IO;
    int ret = 0;

    if(pOS_ConstructMsgPort(&Port)) {
    if((IO=(void*)pOS_CreateIORequest(&Port,sizeof(struct pOS_TrackdiskIO)))) {
    if(!pOS_OpenDevice(dev_name, dev_unit,(struct pOS_IORequest*)IO,0,0)) {
	struct pOS_DriveGeometry Geom;
	char *buf;

	memset(&Geom,0,sizeof(struct pOS_DriveGeometry));
	IO->tdio_Command = TDCMD_GetGeometry;
	IO->tdio_Data    = &Geom;
	IO->tdio_Length  = sizeof(struct pOS_DriveGeometry);
	pOS_DoIO((struct pOS_IORequest*)IO);
	if(IO->tdio_Error!=0) {
	    Geom.dg_SectorSize   = 512;	    
	    Geom.dg_TotalSectors = 880*2;
	}

	if((buf=pOS_AllocMem(Geom.dg_SectorSize,Geom.dg_BufMemType))) {
	    ULONG sec;
	    ret = 1;

	    for(sec = 0; sec<Geom.dg_TotalSectors; ++sec) {
		if((sec % Geom.dg_CylSectors) == 0) {
		    printf("Reading sector %d/%d (%02d%%) of %s unit %d    \r",
			   sec, Geom.dg_TotalSectors, 
			   (100*sec)/Geom.dg_TotalSectors, dev_name, dev_unit);
		    fflush(stdout);
		}
		
		IO->tdio_Command = CMD_READ;
		IO->tdio_Data    = buf;
		IO->tdio_Length  = Geom.dg_SectorSize;
		IO->tdio_LOffset = Geom.dg_SectorSize*sec;
		pOS_DoIO((struct pOS_IORequest*)IO);
		if(IO->tdio_Error) printf("Err. on\n");
		if(fwrite(buf,1,Geom.dg_SectorSize,dst)!=Geom.dg_SectorSize) {
		    ret = 0; 
		    break;
		}
	    }
	    IO->tdio_Command = TDCMD_Motor;
	    IO->tdio_Length  = 0;
	    pOS_DoIO((struct pOS_IORequest*)IO);
	    printf("                                                                        \r");
	    fflush(stdout);
	    
	    pOS_FreeMem(buf,Geom.dg_SectorSize);
	}
	pOS_CloseDevice((struct pOS_IORequest*)IO);
    }   pOS_DeleteIORequest((struct pOS_IORequest*)IO);
    }   pOS_DestructMsgPort(&Port);
    }
    return ret;
}

/****************************************************************************/
/*
 * Copy one raw disk to a file.
 */
int readdevice(char *name, char *dst) 
{
    FILE *f = NULL;
    char *device_name;
    int   device_unit;
    int   retstatus = 0;
#ifdef HAVE_SIGACTION
    struct sigaction oldsa;
    int oldsa_valid;
#endif
    int inhibited;

#ifdef HAVE_SIGACTION
    /* disable break */
    oldsa_valid = (0==sigaction(SIGINT, NULL, &oldsa));
    signal(SIGINT, SIG_IGN); /* <--- gcc complains about something */
                             /* in there but I don't know why. */
#endif
    /* get device name & unit */
    extract_dev_unit(name, &device_name, &device_unit);

    if(device_name) {
        /* if no destination then just check if the device exists */
        if(dst == NULL) 
           retstatus = device_exists(device_name, device_unit);
        else {
	    inhibited = dev_inhibit(name,1);
            /* open dest file */
            if((f = fopen(dst,"wb"))) {
                retstatus = raw_copy(device_name, device_unit, f);
                fclose(f);
            }
	    if(inhibited) dev_inhibit(name,0);
        }
        free(device_name);           
    }

#ifdef HAVE_SIGACTION
    /* enable break */
    if(oldsa_valid) sigaction(SIGINT, &oldsa, NULL);
#endif
    return retstatus;
}

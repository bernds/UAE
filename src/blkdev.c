 /*
  * UAE - The Un*x Amiga Emulator
  *
  * lowlevel device glue
  *
  */

#include "sysconfig.h"
#include "sysdeps.h"
#include "options.h"

#include "config.h"

#include "blkdev.h"

static struct device_functions *device_func[2];
static int ioctl;

#ifdef WIN32

#include "od-win32/win32.h"

extern struct device_functions devicefunc_win32_aspi;
extern struct device_functions devicefunc_win32_spti;
extern struct device_functions devicefunc_win32_ioctl;

static void install_driver (int flags)
{
    device_func[DF_SCSI] = &devicefunc_win32_aspi;
#ifdef WINDDK
    if (os_winnt && os_winnt_admin) {
	device_func[DF_IOCTL] = &devicefunc_win32_ioctl;
        device_func[DF_SCSI] = &devicefunc_win32_spti;
    }
    if (currprefs.win32_aspi) {
        device_func[DF_SCSI] = &devicefunc_win32_aspi;
	device_func[DF_IOCTL] = 0;
    }
#endif
}
#endif

int sys_command_open (int mode, int unitnum)
{
    if (mode == DF_SCSI || !ioctl)
	return device_func[DF_SCSI]->opendev (unitnum);
    else
	return device_func[DF_IOCTL]->opendev (unitnum);
}

void sys_command_close (int mode, int unitnum)
{
    if (mode == DF_SCSI || !ioctl)
	device_func[DF_SCSI]->closedev (unitnum);
    else
	device_func[DF_IOCTL]->closedev (unitnum);
}

int device_func_init (int flags)
{
    static int initialized;
    int support_scsi = 0, support_ioctl = 0;
    int oflags = (flags & DEVICE_TYPE_SCSI) ? 0 : (1 << INQ_ROMD);

    if (initialized)
	return initialized;
    install_driver (flags);
    if (device_func[DF_IOCTL])
	ioctl = 1;
    else
	ioctl = 0;
    support_scsi = device_func[DF_SCSI]->openbus (oflags) ? 1 : 0;
    if (ioctl)
	support_ioctl = device_func[DF_IOCTL]->openbus (1 << INQ_ROMD) ? 1 : 0;
    initialized = 1;
    write_log ("support_scsi = %d support_ioctl = %d\n", support_scsi, support_ioctl);
    return (support_scsi ? (1 << DF_SCSI) : 0) | (support_ioctl ? (1 << DF_IOCTL) : 0);
}

static int audiostatus (int unitnum)
{
    uae_u8 cmd[10] = {0x42,2,0x40,1,0,0,0,DEVICE_SCSI_BUFSIZE>>8,DEVICE_SCSI_BUFSIZE&0xff,0};
    uae_u8 *p = device_func[DF_SCSI]->exec_in (unitnum, cmd, sizeof (cmd), 0);
    if (!p)
	return 0;
    return p[1];
}

/* pause/unpause CD audio */
void sys_command_pause (int mode, int unitnum, int paused)
{
    if (mode == DF_SCSI || !ioctl) {
	int as = audiostatus (unitnum);
	if ((paused && as == 0x11) && (!paused && as == 0x12)) {
	    uae_u8 cmd[10] = {0x4b,0,0,0,0,0,0,0,paused?0:1,0};
	    device_func[DF_SCSI]->exec_out (unitnum, cmd, sizeof (cmd));
	}
	return;
    }
    device_func[DF_IOCTL]->pause (unitnum, paused);
}

/* stop CD audio */
void sys_command_stop (int mode, int unitnum)
{
    if (mode == DF_SCSI || !ioctl) {
	int as = audiostatus (unitnum);
	if (as == 0x11) {
	    uae_u8 cmd[6] = {0x4e,0,0,0,0,0};
	    device_func[DF_SCSI]->exec_out (unitnum, cmd, sizeof (cmd));
	}
	return;
    }
    device_func[DF_IOCTL]->stop (unitnum);
}

/* play CD audio */
int sys_command_play (int mode, int unitnum,uae_u32 startmsf, uae_u32 endmsf, int scan)
{
    if (mode == DF_SCSI || !ioctl) {
        uae_u8 cmd[12] = {0,0,0,0,0,0,0,0,0,0,0,0};
#if 0
	if (scan) {
	    cmd[0] = 0xba;
	    cmd[1] = scan < 0 ? 0x10 : 0x0;
	    cmd[3] = (uae_u8)(startmsf >> 16);
	    cmd[4] = (uae_u8)(startmsf >> 8);
	    cmd[5] = (uae_u8)(startmsf >> 0);
	    cmd[9] = 0x40;
	} else {
#endif
	    cmd[0] = 0x47;
	    cmd[3] = (uae_u8)(startmsf >> 16);
	    cmd[4] = (uae_u8)(startmsf >> 8);
	    cmd[5] = (uae_u8)(startmsf >> 0);
	    cmd[6] = (uae_u8)(endmsf >> 16);
	    cmd[7] = (uae_u8)(endmsf >> 8);
	    cmd[8] = (uae_u8)(endmsf >> 0);
#if 0
	}
#endif
        return device_func[DF_SCSI]->exec_out (unitnum, cmd, sizeof (cmd)) == 0 ? 0 : 1;
    }
    return device_func[DF_IOCTL]->play (unitnum, startmsf, endmsf, scan);
}

/* read qcode */
uae_u8 *sys_command_qcode (int mode, int unitnum)
{
    if (mode == DF_SCSI || !ioctl) {
	uae_u8 cmd[10] = {0x42,2,0x40,1,0,0,0,DEVICE_SCSI_BUFSIZE>>8,DEVICE_SCSI_BUFSIZE&0xff,0};
	return  device_func[DF_SCSI]->exec_in (unitnum, cmd, sizeof (cmd), 0);
    }
    return device_func[DF_IOCTL]->qcode (unitnum);
};

/* read table of contents */
uae_u8 *sys_command_toc (int mode, int unitnum)
{
    if (mode == DF_SCSI || !ioctl) {
	uae_u8 cmd [10] = { 0x43,0,2,0,0,0,1,DEVICE_SCSI_BUFSIZE>>8,DEVICE_SCSI_BUFSIZE&0xFF,0};
        return device_func[DF_SCSI]->exec_in (unitnum, cmd, sizeof(cmd), 0);
    }
    return device_func[DF_IOCTL]->toc (unitnum);
}

/* read one sector */
uae_u8 *sys_command_read (int mode, int unitnum, int offset)
{
    if (mode == DF_SCSI || !ioctl) {
	uae_u8 cmd[12] = { 0xbe, 0, 0, 0, 0, 0, 0, 0, 1, 0x10, 0, 0 };
	cmd[3] = (uae_u8)(offset >> 16);
	cmd[4] = (uae_u8)(offset >> 8);
	cmd[5] = (uae_u8)(offset >> 0);
        return device_func[DF_SCSI]->exec_in (unitnum, cmd, sizeof (cmd), 0);
    }
    return device_func[DF_IOCTL]->read (unitnum, offset);
}

struct device_info *sys_command_info (int mode, int unitnum, struct device_info *di)
{
    if (mode == DF_SCSI || !ioctl)
	return device_func[DF_SCSI]->info (unitnum, di);
    else
	return device_func[DF_IOCTL]->info (unitnum, di);
}

int sys_command_scsi_direct (int unitnum, uaecptr request)
{
    return device_func[DF_SCSI]->exec_direct (unitnum, request);
}

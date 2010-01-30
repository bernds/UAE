 /*
  * UAE - The Un*x Amiga Emulator
  *
  * Floppy disk emulation
  *
  * Copyright 1995 Bernd Schmidt, Hannu Rummukainen
  */

#include <stdio.h>

#include "sysconfig.h"
#include "sysdeps.h"

#include "config.h"
#include "options.h"
#include "threaddep/penguin.h"
#include "memory.h"
#include "events.h"
#include "custom.h"
#include "ersatz.h"
#include "disk.h"
#include "gui.h"
#include "zfile.h"
#include "autoconf.h"
#include "readcpu.h"
#include "newcpu.h"
#include "xwin.h"
#include "osemu.h"
#include "execlib.h"

#define FLOPPY_GAP_LEN 360
/* writable track length with normal 2us bitcell/300RPM motor */
#define FLOPPY_WRITE_LEN 6250
/* (cycles/bitcell) << 8, normal = ((2us/280ns)<<8) = ~1830 */
#define NORMAL_FLOPPY_SPEED 1830

static int floppy_speed = NORMAL_FLOPPY_SPEED;

#undef DISK_DEBUG

/* UAE-1ADF (ADF_EXT2)
 * W	reserved
 * W	number of tracks (default 2*80=160)
 *
 * W	reserved
 * W	type, 0=normal AmigaDOS track, 1 = raw MFM
 * L	available space for track in bytes
 * L	track length in bits
 */

static int side, direction, step, writing;
static uae_u8 selected = 15;
static int dskready;

static uae_u8 *writebuffer[544 * 11];

typedef enum { TRACK_AMIGADOS, TRACK_RAW, TRACK_RAW1 } image_tracktype;
typedef struct {
    uae_u16 len;
    uae_u32 offs;
    int bitlen;
    unsigned int sync;
    image_tracktype type;
} trackid;

#define MAX_TRACKS 328

typedef enum { ADF_NORMAL, ADF_EXT1, ADF_EXT2 } drive_filetype;
typedef struct {
    FILE *diskfile;
    drive_filetype filetype;
    trackid trackdata[MAX_TRACKS];
    int buffered_cyl, buffered_side;
    int cyl;
    int motoroff;
    int state;
    int disabled;
    int wrprot;
    uae_u16 bigmfmbuf[0x4000];
    int mfmpos;
    int mfmpos_hsync;
    int tracklen;
    int trackspeed;
    int dmalen;
    int num_tracks, num_secs;
    /* dskchange is set to a small number when a disk is removed. At a step
     * impulse, it is reset if there is a new disk in the drive or counted
     * down to 1 if the drive contains a disk. DISK_check_change will only
     * insert a new disk when dskchange is 1, or if the dskchange_time timer
     * expires. The last condition can happen if AmigaOS isn't running and a
     * game/demo doesn't use the step motor to check for a new disk.
     */
    int dskchange;
    int dskchange_time;
} drive;

drive floppy[4];

/* code for track display */
static void update_drive_gui (drive * drv)
{
    int num;
#if 0
    num = drv - &floppy[0];
    if (drv->state == gui_data.drive_motor[num]
	&& drv->cyl == gui_data.drive_track[num]
	&& ((writing && gui_data.drive_writing[num])
	    || (!writing && !gui_data.drive_writing[num]))) {
	return;
    }
    gui_data.drive_motor[num] = drv->state;
    gui_data.drive_track[num] = drv->cyl;
    if (!gui_data.drive_writing[num])
	gui_data.drive_writing[num] = writing;
    gui_led (num + 1, gui_data.drive_motor[num]);
#endif
}

static void drive_fill_bigbuf (drive * drv);

FILE *DISK_validate_filename (const char *fname, int leave_open, int *wrprot)
{
    FILE *f = zfile_open (fname, "r+b");
    if (f) {
	if (wrprot)
	    *wrprot = 0;
    } else {
	if (wrprot)
	    *wrprot = 1;
	f = zfile_open (fname, "rb");
    }
    if (!leave_open)
	zfile_close (f);
    return f;
}

static int drive_insert (drive * drv, int dnum, const char *fname)
{
    unsigned char buffer[2 + 2 + 4 + 4];
    trackid *tid;

    drv->diskfile = DISK_validate_filename (fname, 1, &drv->wrprot);
    if (drv->diskfile == 0) {
	drv->tracklen = FLOPPY_WRITE_LEN * 2 * 8;
	drv->trackspeed = floppy_speed;
	return 0;
    }
    strncpy (currprefs.df[dnum], fname, 255);
    currprefs.df[dnum][255] = 0;
    strncpy (changed_prefs.df[dnum], fname, 255);
    changed_prefs.df[dnum][255] = 0;
    gui_filename (dnum, fname);

    fread (buffer, sizeof (char), 8, drv->diskfile);
    if (strncmp ((char *) buffer, "UAE-1ADF", 8) == 0) {
	int offs;
	int i;

	fread (buffer, 1, 4, drv->diskfile);
	drv->num_tracks = buffer[2] * 256 + buffer[3];
	drv->filetype = ADF_EXT2;
	drv->num_secs = 11;
	offs = 8 + drv->num_tracks * (2 + 2 + 4 + 4);

	for (i = 0; i < drv->num_tracks; i++) {
	    tid = &drv->trackdata[i];
	    fread (buffer, 2 + 2 + 4 + 4, 1, drv->diskfile);
	    tid->type = buffer[2] * 256 + buffer[3];
	    tid->len = buffer[5] * 65536 + buffer[6] * 256 + buffer[7];
	    tid->bitlen = buffer[9] * 65536 + buffer[10] * 256 + buffer[11];
	    tid->offs = offs;
	    if (tid->type == TRACK_AMIGADOS) {
		if (tid->len > 20000)
		    drv->num_secs = 22;
	    }
	    offs += tid->len;
	}
    } else if (strncmp ((char *) buffer, "UAE--ADF", 8) == 0) {
	int offs = 160 * 4 + 8;
	int i;

	drv->filetype = ADF_EXT1;
	drv->num_tracks = 160;
	drv->num_secs = 11;

	drv->wrprot = 1;	/* write to adf_ext1 not implemented */
	for (i = 0; i < 160; i++) {
	    tid = &drv->trackdata[i];
	    fread (buffer, 4, 1, drv->diskfile);
	    tid->sync = buffer[0] * 256 + buffer[1];
	    tid->len = buffer[2] * 256 + buffer[3];
	    tid->offs = offs;
	    if (tid->sync == 0) {
		tid->type = TRACK_AMIGADOS;
		tid->bitlen = 0;
	    } else {
		tid->type = TRACK_RAW1;
		tid->bitlen = tid->len * 8;
	    }
	    offs += tid->len;
	}
    } else {
	int i;

	drv->filetype = ADF_NORMAL;

	fseek (drv->diskfile, 0, SEEK_END);
	i = ftell (drv->diskfile);
	fseek (drv->diskfile, 0, SEEK_SET);

	/* High-density disk? */
	if (i >= 160 * 22 * 512)
	    drv->num_tracks = i / (512 * (drv->num_secs = 22));
	else
	    drv->num_tracks = i / (512 * (drv->num_secs = 11));

	if (drv->num_tracks > MAX_TRACKS)
	    write_log ("Your diskfile is too big!\n");
	for (i = 0; i < drv->num_tracks; i++) {
	    tid = &drv->trackdata[i];
	    tid->type = TRACK_AMIGADOS;
	    tid->len = 512 * drv->num_secs;
	    tid->bitlen = 0;
	    tid->offs = i * 512 * drv->num_secs;
	}
    }
    drv->buffered_side = 2;	/* will force read */
    drive_fill_bigbuf (drv);
    return 1;
}

static int drive_empty (drive * drv)
{
    return drv->diskfile == 0;
}

static void drive_step (drive * drv)
{
    if (!drive_empty (drv))
	drv->dskchange = 0;
    else if (drv->dskchange > 1) {
/*	printf("Stepping...\n");
	drv->dskchange--;*/
    }
    if (direction) {
	if (drv->cyl)
	    drv->cyl--;
    } else {
	if (drv->cyl < 83)
	    drv->cyl++;
    }

#if 0 && defined DISK_DEBUG
    write_log ("track %d\n", drv->cyl);
#endif
}

static int drive_track0 (drive * drv)
{
    return drv->cyl == 0;
}

static int drive_writeprotected (drive * drv)
{
    return drv->wrprot || drv->diskfile == NULL;
}

static int drive_running (drive * drv)
{
    return !drv->motoroff;
}

static void drive_motor (drive * drv, int off)
{
    drv->motoroff = off;
}

static void read_floppy_data (drive * drv, int track, int offset, unsigned char *dst, int len)
{
    trackid *ti;

    ti = &drv->trackdata[track];
    fseek (drv->diskfile, ti->offs + offset, SEEK_SET);
    fread (dst, 1, len, drv->diskfile);
}

/* Megalomania does not like zero MFM words... */
static void mfmcode (uae_u16 * mfm, int words)
{
    uae_u16 v;
    uae_u32 lv;
    int i;

    while (words--) {
	lv = (mfm[0] << 16) | mfm[1];
	v = mfm[0];
	for (i = 30; i >= 16; i -= 2) {
	    if (!(lv & (1 << i)) && !(lv & (1 << (i - 2))))
		v |= 1 << (i + 1 - 16);
	}
	*mfm++ = v;
    }
}

static void drive_fill_bigbuf (drive * drv)
{
    int tr = drv->cyl * 2 + side;
    trackid *ti = drv->trackdata + tr;

    if (!drv->diskfile || tr >= drv->num_tracks) {
	drv->tracklen = FLOPPY_WRITE_LEN * 2 * 8;
	drv->trackspeed = floppy_speed;
	memset (drv->bigmfmbuf, 0xaa, FLOPPY_WRITE_LEN * 2);
	return;
    }
    if (drv->buffered_cyl == drv->cyl && drv->buffered_side == side)
	return;

    if (ti->type == TRACK_AMIGADOS) {
	/* Normal AmigaDOS format track */
	int sec;
	drv->tracklen = (drv->num_secs * 544 + FLOPPY_GAP_LEN) * 2 * 8;
	memset (drv->bigmfmbuf, 0xaa, FLOPPY_GAP_LEN * 2);

	for (sec = 0; sec < drv->num_secs; sec++) {
	    uae_u8 secbuf[544];
	    int i;
	    uae_u16 *mfmbuf = drv->bigmfmbuf + 544 * sec + FLOPPY_GAP_LEN;
	    uae_u32 deven, dodd;
	    uae_u32 hck = 0, dck = 0;

	    secbuf[0] = secbuf[1] = 0x00;
	    secbuf[2] = secbuf[3] = 0xa1;
	    secbuf[4] = 0xff;
	    secbuf[5] = tr;
	    secbuf[6] = sec;
	    secbuf[7] = drv->num_secs - sec;

	    for (i = 8; i < 24; i++)
		secbuf[i] = 0;

	    read_floppy_data (drv, tr, sec * 512, &secbuf[32], 512);

	    mfmbuf[0] = mfmbuf[1] = 0xaaaa;
	    mfmbuf[2] = mfmbuf[3] = 0x4489;

	    deven = ((secbuf[4] << 24) | (secbuf[5] << 16)
		     | (secbuf[6] << 8) | (secbuf[7]));
	    dodd = deven >> 1;
	    deven &= 0x55555555;
	    dodd &= 0x55555555;

	    mfmbuf[4] = dodd >> 16;
	    mfmbuf[5] = dodd;
	    mfmbuf[6] = deven >> 16;
	    mfmbuf[7] = deven;

	    for (i = 8; i < 48; i++)
		mfmbuf[i] = 0xaaaa;
	    for (i = 0; i < 512; i += 4) {
		deven = ((secbuf[i + 32] << 24) | (secbuf[i + 33] << 16)
			 | (secbuf[i + 34] << 8) | (secbuf[i + 35]));
		dodd = deven >> 1;
		deven &= 0x55555555;
		dodd &= 0x55555555;
		mfmbuf[(i >> 1) + 32] = dodd >> 16;
		mfmbuf[(i >> 1) + 33] = dodd;
		mfmbuf[(i >> 1) + 256 + 32] = deven >> 16;
		mfmbuf[(i >> 1) + 256 + 33] = deven;
	    }

	    for (i = 4; i < 24; i += 2)
		hck ^= (mfmbuf[i] << 16) | mfmbuf[i + 1];

	    deven = dodd = hck;
	    dodd >>= 1;
	    mfmbuf[24] = dodd >> 16;
	    mfmbuf[25] = dodd;
	    mfmbuf[26] = deven >> 16;
	    mfmbuf[27] = deven;

	    for (i = 32; i < 544; i += 2)
		dck ^= (mfmbuf[i] << 16) | mfmbuf[i + 1];

	    deven = dodd = dck;
	    dodd >>= 1;
	    mfmbuf[28] = dodd >> 16;
	    mfmbuf[29] = dodd;
	    mfmbuf[30] = deven >> 16;
	    mfmbuf[31] = deven;
	    mfmcode (mfmbuf + 4, 544 - 4);
	}
#ifdef DISK_DEBUG
	write_log ("amigados read track %d\n", tr);
#endif
    } else {
	int i;
	int base_offset = ti->type == TRACK_RAW ? 0 : 1;
	drv->tracklen = ti->bitlen + 16 * base_offset;
	read_floppy_data (drv, tr, 0, (char *) (drv->bigmfmbuf + base_offset), (ti->bitlen + 7) / 8);
	drv->bigmfmbuf[0] = ti->sync;
	for (i = base_offset; i < drv->tracklen / 16; i++) {
	    uae_u16 *mfm = drv->bigmfmbuf + i;
	    uae_u8 *data = (uae_u8 *) mfm;
	    *mfm = 256 * *data + *(data + 1);
	}
#if 0 && defined DISK_DEBUG
	write_log ("rawtrack %d\n", tr);
#endif
    }
    drv->buffered_side = side;
    drv->buffered_cyl = drv->cyl;
    drv->trackspeed = floppy_speed * drv->tracklen / (2 * 8 * FLOPPY_WRITE_LEN);
}

/* Update ADF_EXT2 track header */
static void diskfile_update (drive * drv, int len, uae_u8 type)
{
    trackid *ti;
    int track;
    uae_u8 buf[2 + 2 + 4 + 4], *zerobuf;

    if (drv->filetype != ADF_EXT2)
	return;

    track = drv->cyl * 2 + side;
    ti = &drv->trackdata[track];
    ti->bitlen = len;
    fseek (drv->diskfile, 8 + 4 + (2 + 2 + 4 + 4) * track, SEEK_SET);
    memset (buf, 0, sizeof buf);
    ti->type = type;
    buf[3] = ti->type;
    do_put_mem_long ((uae_u32 *) (buf + 4), ti->len);
    do_put_mem_long ((uae_u32 *) (buf + 8), ti->bitlen);
    fwrite (buf, sizeof (buf), 1, drv->diskfile);
    if (ti->len > (len + 7) / 8) {
	zerobuf = malloc (ti->len);
	memset (zerobuf, 0, ti->len);
	fseek (drv->diskfile, ti->offs, SEEK_SET);
	fwrite (zerobuf, 1, ti->len, drv->diskfile);
	free (zerobuf);
    }
#ifdef DISK_DEBUG
    write_log ("track %d, raw track length %d written (total size %d)\n", track, (ti->bitlen + 7) / 8, ti->len);
#endif
}

#define MFMMASK 0x55555555
static uae_u32 getmfmlong (uae_u16 * mbuf)
{
    return ((*mbuf << 16) | *(mbuf + 1)) & MFMMASK;
}

static int drive_write_adf_amigados (drive * drv)
{
    int i, secwritten = 0;
    int length = 2 * FLOPPY_WRITE_LEN;
    uae_u32 odd, even, chksum, id, dlong;
    uae_u8 *secdata;
    uae_u8 secbuf[544];
    uae_u16 *mbuf = drv->bigmfmbuf;
    uae_u16 *mend = mbuf + length;
    char sectable[11];

    memset (sectable, 0, sizeof (sectable));
    memcpy (mbuf + FLOPPY_WRITE_LEN, mbuf, FLOPPY_WRITE_LEN * sizeof (uae_u16));
    mend -= (4 + 16 + 8 + 512);
    while (secwritten < 11) {
	int trackoffs;

	do {
	    while (*mbuf++ != 0x4489) {
		if (mbuf >= mend)
		    return 1;
	    }
	} while (*mbuf++ != 0x4489);

	odd = getmfmlong (mbuf);
	even = getmfmlong (mbuf + 2);
	mbuf += 4;
	id = (odd << 1) | even;

	trackoffs = (id & 0xff00) >> 8;
	if (trackoffs > 10) {
	    if (drv->filetype == ADF_EXT2)
		return 2;
	    write_log ("Disk write: weird sector number %d\n", trackoffs);
	    continue;
	}
	chksum = odd ^ even;
	for (i = 0; i < 4; i++) {
	    odd = getmfmlong (mbuf);
	    even = getmfmlong (mbuf + 8);
	    mbuf += 2;

	    dlong = (odd << 1) | even;
	    if (dlong) {
		if (drv->filetype == ADF_EXT2)
		    return 6;
		secwritten = -200;
	    }
	    chksum ^= odd ^ even;
	}			/* could check here if the label is nonstandard */
	mbuf += 8;
	odd = getmfmlong (mbuf);
	even = getmfmlong (mbuf + 2);
	mbuf += 4;
	if (((odd << 1) | even) != chksum || ((id & 0x00ff0000) >> 16) != drv->cyl * 2 + side) {
	    if (drv->filetype == ADF_EXT2)
		return 3;
	    write_log ("Disk write: checksum error on sector %d header\n", trackoffs);
	    continue;
	}
	odd = getmfmlong (mbuf);
	even = getmfmlong (mbuf + 2);
	mbuf += 4;
	chksum = (odd << 1) | even;
	secdata = secbuf + 32;
	for (i = 0; i < 128; i++) {
	    odd = getmfmlong (mbuf);
	    even = getmfmlong (mbuf + 256);
	    mbuf += 2;
	    dlong = (odd << 1) | even;
	    *secdata++ = dlong >> 24;
	    *secdata++ = dlong >> 16;
	    *secdata++ = dlong >> 8;
	    *secdata++ = dlong;
	    chksum ^= odd ^ even;
	}
	mbuf += 256;
	if (chksum) {
	    if (drv->filetype == ADF_EXT2)
		return 4;
	    write_log ("Disk write: sector %d, data checksum error\n", trackoffs);
	    continue;
	}
	sectable[trackoffs] = 1;
	secwritten++;
	memcpy (writebuffer + trackoffs * 512, secbuf + 32, 512);
    }
    if (drv->filetype == ADF_EXT2 && (secwritten == 0 || secwritten < 0))
	return 5;
    if (secwritten == 0)
	write_log ("Disk write in unsupported format\n");
    if (secwritten < 0)
	write_log ("Disk write: sector labels ignored\n");

    diskfile_update (drv, 11 * 512 * 8, TRACK_AMIGADOS);
    for (i = 0; i < 11; i++) {
	fseek (drv->diskfile, drv->trackdata[drv->cyl * 2 + side].offs + i * 512, SEEK_SET);
	fwrite (writebuffer + i * 512, sizeof (uae_u8), 512, drv->diskfile);
    }

    return 0;
}

/* write raw track to disk file */
static int drive_write_ext2 (drive * drv)
{
    trackid *ti;
    int len, track, i;

    track = drv->cyl * 2 + side;
    ti = &drv->trackdata[track];
    len = (drv->tracklen + 7) / 8;
    if (len > ti->len) {
	write_log ("disk raw write: image file's track %d is too small (%d < %d)!\n", track, ti->len, len);
	return 0;
    }
    diskfile_update (drv, drv->tracklen, TRACK_RAW);
    for (i = 0; i < drv->trackdata[track].len / 2; i++) {
	uae_u16 *mfm = drv->bigmfmbuf + i;
	uae_u8 *data = (uae_u8 *) mfm;
	*mfm = 256 * *data + *(data + 1);
    }
    fseek (drv->diskfile, ti->offs, SEEK_SET);
    fwrite (drv->bigmfmbuf, 1, len, drv->diskfile);
    return 1;
}

static void drive_write_data (drive * drv)
{
    int ret;
    if (drive_writeprotected (drv))
	return;
    switch (drv->filetype) {
    case ADF_NORMAL:
	drive_write_adf_amigados (drv);
	break;
    case ADF_EXT1:
	write_log ("writing to ADF_EXT1 not supported\n");
	return;
    case ADF_EXT2:
	ret = drive_write_adf_amigados (drv);
	if (ret) {
	    /*write_log("not an amigados track (error %d), writing as raw track\n",ret); */
	    drive_write_ext2 (drv);
	}
	break;
    }
    drv->buffered_side = 2;	/* will force read */
}

static void drive_eject (drive * drv)
{
    if (!drive_empty (drv))
	zfile_close (drv->diskfile);

    drv->dskchange = 4;
    drv->dskchange_time = 20;
/*    printf("setting changed bit %d\n", drv-floppy);*/
    drv->diskfile = 0;
}

/* We use this function if we have no Kickstart ROM.
 * No error checking - we trust our luck. */
void DISK_ersatz_read (int tr, int sec, uaecptr dest)
{
    uae_u8 *dptr = get_real_address (dest);
    fseek (floppy[0].diskfile, floppy[0].trackdata[tr].offs + sec * 512, SEEK_SET);
    fread (dptr, 1, 512, floppy[0].diskfile);
}

void disk_eject (int num)
{
    gui_filename (num, "");
    drive_eject (floppy + num);
    *currprefs.df[num] = *changed_prefs.df[num] = 0;
}

void disk_insert (int num, const char *name)
{
    /* just to be sure */
    drive_eject (floppy + num);
    drive_insert (floppy + num, num, name);
}

void DISK_check_change (void)
{
    int i;
    static int count = 0;
    count++;

    for (i = 0; i < 4; i++) {
	if (strcmp (currprefs.df[i], changed_prefs.df[i]) != 0) {
	    if (currprefs.df[i][0] != '\0') {
		drive_eject (floppy + i);
		currprefs.df[i][0] = '\0';
		gui_filename (i, "");
	    } else if (floppy[i].dskchange == 1) {
		/* In theory, it should work without the dskchange test.
		 * In practice, it doesn't. */
		drive_insert (floppy + i, i, changed_prefs.df[i]);
	    } else if (floppy[i].dskchange > 1 && floppy[i].dskchange_time > 0) {
		/* Force the dskchange bit to go to 1 after a given timeout */
		if (--floppy[i].dskchange_time == 0) {
		    floppy[i].dskchange = 1;
		}
	    }
	}
    }
}

int disk_empty (int num)
{
    return drive_empty (floppy + num);
}

void DISK_select (uae_u8 data)
{
    int step_pulse;
    int dr;

    if (selected != ((data >> 3) & 15))
	dskready = 0;
    selected = (data >> 3) & 15;
    side = 1 - ((data >> 2) & 1);

    direction = (data >> 1) & 1;
    step_pulse = data & 1;
    if (step != step_pulse) {
	step = step_pulse;
	if (step == 0) {
	    for (dr = 0; dr < 4; dr++) {
		if (!(selected & (1 << dr))) {
		    drive_step (floppy + dr);
		}
	    }
	}
    }
    for (dr = 0; dr < 4; dr++) {
	if (!(selected & (1 << dr))) {
	    drive_motor (floppy + dr, data >> 7);
	}
    }
    for (dr = 0; dr < 4; dr++) {
	floppy[dr].state = (!(selected & (1 << dr))) | !floppy[dr].motoroff;
	update_drive_gui (floppy + dr);
    }
}

uae_u8 DISK_status (void)
{
    uae_u8 st = 0x3c;
    int dr;

    for (dr = 0; dr < 4; dr++) {
	drive *drv = floppy + dr;
	if (!(selected & (1 << dr))) {
	    if (drive_running (drv)) {
		if (dskready)
		    st &= ~0x20;
		dskready = 1;
	    } else if (!drv->disabled) {
		st &= ~0x20;	/* report drive ID */
	    }

	    if (drive_track0 (drv)) {
		st &= ~0x10;
	    }
	    if (drive_writeprotected (drv)) {
		st &= ~8;
	    }
	    if (drv->dskchange) {
		/*printf("changed bit set: %d\n",dr); */
		st &= ~0x4;
		if (drv->dskchange > 1)
		    drv->dskchange--;
	    }
	}
    }
    return st;
}

#define MAX_DISK_WORDS_PER_LINE 50	/* depends on floppy_speed */

#define DISK_INDEXSYNC 1
#define DISK_WORDSYNC 2

static uae_u16 dskbytr_tab[MAX_DISK_WORDS_PER_LINE * 2 + 1];
static uae_u8 dskbytr_cycle[MAX_DISK_WORDS_PER_LINE * 2 + 1];
static uae_u16 dskdatr_tab[MAX_DISK_WORDS_PER_LINE + 1];
static uae_u8 dskdatr_cycle[MAX_DISK_WORDS_PER_LINE + 1];
static uae_u32 dma_tab[MAX_DISK_WORDS_PER_LINE + 1];
static uae_u8 disk_sync[MAXHPOS];
static int disk_sync_cycle;
static int indexsync_last_cycle;
static int dskdmaen, dsklength;
static uae_u16 dsksync;
static uae_u32 dskpt;

static void disk_dmafinished (void)
{
    INTREQ (0x8002);
    dskdmaen = 0;
}

static void disk_events (int last)
{
    eventtab[ev_disk].active = 0;
    for (disk_sync_cycle = last; disk_sync_cycle < MAXHPOS; disk_sync_cycle++) {
	if (disk_sync[disk_sync_cycle]) {
	    eventtab[ev_disk].oldcycles = cycles;
	    eventtab[ev_disk].evtime = cycles + (disk_sync_cycle - last);
	    eventtab[ev_disk].active = 1;
	    events_schedule ();
	    return;
	}
    }
}

extern void cia_diskindex (void);

void DISK_handler (void)
{
    eventtab[ev_disk].active = 0;
    if (disk_sync[disk_sync_cycle] & DISK_WORDSYNC) {
	INTREQ (0x9000);
	indexsync_last_cycle = disk_sync_cycle + ((floppy_speed + 255) >> 8);
    }
    if (disk_sync[disk_sync_cycle] & DISK_INDEXSYNC) {
	cia_diskindex ();
    }
    disk_sync[disk_sync_cycle] = 0;
    disk_events (disk_sync_cycle);
}

static int syncfound, dmaon, word, bitoffset;

/* Always carried through to the next line.  */
static int disk_hpos;

/* emulate disk write dma for full horizontal line */
static void disk_doupdate_write (drive * drv)
{
    int hpos = disk_hpos;
    while (hpos < (MAXHPOS << 8)) {
	drv->mfmpos++;
	drv->mfmpos %= drv->tracklen;
	if (!drv->mfmpos) {
	    disk_sync[hpos >> 8] |= DISK_INDEXSYNC;
	    disk_events (0);
	}
	if ((dmacon & 0x210) == 0x210 && dskdmaen == 3) {
	    bitoffset++;
	    bitoffset &= 15;
	    if (!bitoffset) {
		drv->bigmfmbuf[drv->mfmpos >> 4] = get_word (dskpt);
		dskpt += 2;
		dsklength--;
		if (dsklength == 0) {
		    disk_dmafinished ();
		    drive_write_data (drv);
		}
	    }
	}
	hpos += drv->trackspeed;
    }
    disk_hpos = hpos - (MAXHPOS << 8);
}

/* get one bit from MFM bit stream */
static uae_u16 getonebit (uae_u16 * mfmbuf, int mfmpos, uae_u16 word)
{
    uae_u16 *buf;

    buf = &mfmbuf[mfmpos >> 4];
    word <<= 1;
    word |= (buf[0] & (1 << (15 - (mfmpos & 15)))) ? 1 : 0;
    return word;
}

/* emulate disk read dma for full horizontal line */
static void disk_doupdate_read (drive * drv)
{
    int hpos = disk_hpos;
    int is_sync = 0, dmaon;
    int j = 0, l = 0, k = 0;

    while (hpos < (MAXHPOS << 8)) {
	word = getonebit (drv->bigmfmbuf, drv->mfmpos, word);
	drv->mfmpos++;
	drv->mfmpos %= drv->tracklen;
	if (!drv->mfmpos) {
	    disk_sync[hpos >> 8] |= DISK_INDEXSYNC;
	    is_sync = 1;
	}
	bitoffset++;
	bitoffset &= 15;
	if (bitoffset == 0 && (!(adkcon & 0x400) || syncfound))
	    dmaon = 1;
	else
	    dmaon = 0;
	if (word == dsksync) {
	    if ((adkcon & 0x400) == 0x400 && ! syncfound) {
		syncfound = 1;
		bitoffset = 0;
	    }
	    disk_sync[hpos >> 8] |= DISK_WORDSYNC;
	    is_sync = 1;
	}
	if (dmaon) {
	    dma_tab[j++] = word;
	    if (j == MAX_DISK_WORDS_PER_LINE - 1) {
		write_log ("Bug: Disk DMA buffer overflow!\n");
		j--;
	    }
	}
	if (bitoffset == 0 || bitoffset == 8) {
	    dskbytr_tab[k] = (word & 0xff) | 0x8000;
	    dskbytr_cycle[k++] = hpos >> 8;
	    if (bitoffset == 0) {
		dskdatr_tab[l] = word;
		dskdatr_cycle[l++] = hpos >> 8;
	    }
	}
	hpos += drv->trackspeed;
    }
    dma_tab[j] = 0xffffffff;
    if (is_sync)
	disk_events (0);
    dskbytr_cycle[k] = 255;
    dskdatr_cycle[l] = 255;

    disk_hpos = hpos - (MAXHPOS << 8);
}

/* disk DMA fetch happens on real Amiga at the beginning of next horizontal line
   (cycles 9, 11 and 13 according to hardware manual) We transfer all DMA'd
   data at cycle 0. I don't think any program cares about this small difference.
*/
static void dodmafetch (void)
{
    int i;

    i = 0;
    while (dma_tab[i] != 0xffffffff && dskdmaen == 2 && (dmacon & 0x210) == 0x210) {
	put_word (dskpt, dma_tab[i]);
	dskpt += 2;
	dsklength--;
	if (dsklength == 0) {
	    disk_dmafinished ();
	    break;
	}
	i++;
    }
    dma_tab[0] = 0xffffffff;
}

static void DISK_start (void)
{
    int dr;

    for (dr = 0; dr < 4; dr++) {
	drive *drv = &floppy[dr];
	if (!(selected & (1 << dr))) {
	    int tr = drv->cyl * 2 + side;
	    trackid *ti = drv->trackdata + tr;

	    drive_fill_bigbuf (drv);
	    if (dskdmaen == 3) {
		drv->tracklen = FLOPPY_WRITE_LEN * 8 * 2;
		drv->trackspeed = floppy_speed;
	    }
	    /* Ugh.  A nasty hack.  Assume ADF_EXT1 tracks are always read
	       from the start.  */
	    if (ti->type == TRACK_RAW1)
	        drv->mfmpos = 0;
	    drv->mfmpos %= drv->tracklen;
	}
    }
    syncfound = 0;
    dmaon = 0;
    word = 0;
    bitoffset = 0;
    dma_tab[0] = 0xffffffff;
}

static int linecounter;

void DISK_update (void)
{
    int dr;
    
    for (dr = 0; dr < 4; dr++) {
	drive *drv = &floppy[dr];
	drv->mfmpos_hsync = drv->mfmpos;
    }
    if (linecounter) {
	linecounter--;
	if (! linecounter)
	    disk_dmafinished ();
	return;
    }

    dodmafetch ();
    indexsync_last_cycle = -1;

    for (dr = 0; dr < 4; dr++) {
	drive *drv = &floppy[dr];
	if (drv->motoroff)
	    continue;
	if (selected & (1 << dr)) {
	    drv->mfmpos += (MAXHPOS << 8) / drv->trackspeed;
	    drv->mfmpos %= drv->tracklen;
	    continue;
	}
	if (dskdmaen == 3)
	    disk_doupdate_write (drv);
	else
	    disk_doupdate_read (drv);
	return;
    }
}

void DSKLEN (uae_u16 v, int hpos)
{
    if (v & 0x8000) {
	dskdmaen = dskdmaen == 1 ? 2 : 1;
    } else {
	/* Megalomania does this */
#if 0
	if (dskdmaen == 2)
	    write_log ("warning: Disk read DMA aborted, %d words left\n", dsklength);
#endif
	if (dskdmaen == 3)
	    write_log ("warning: Disk write DMA aborted, %d words left\n", dsklength);
	dskdmaen = 0;
    }
    dsklength = v & 0x3fff;
    if (dskdmaen <= 1)
	return;
    if (v & 0x4000)
	dskdmaen = 3;
#ifdef DISK_DEBUG
    {
	int dr;
	for (dr = 0; dr < 4; dr++) {
	    drive *drv = &floppy[dr];
	    if (drv->motoroff)
		continue;
	    if ((selected & (1 << dr)) == 0)
		break;
	}
	if (dr == 4)
	    write_log ("disk DMA started, but no drive ready!\n");
	else
	    write_log ("disk %s DMA started for %d words (%08.8X) sync=%04.4X track %d\n",
		       dskdmaen == 3 ? "write" : "read", dsklength, dskpt,
		       (adkcon & 0x400) ? dsksync : 0xffff,
		       floppy[dr].cyl * 2 + side);
    }
#endif
    /* Megalomania does not work if DSKLEN write is delayed to next horizontal line,
       also it seems some copy protections require this fix */
    DISK_start ();

    /* Try to make floppy access from Kickstart faster.  */
    if (dskdmaen != 2)
	return;
    {
	int dr;
	uaecptr pc = m68k_getpc ();
	if ((pc & 0xF80000) != 0xF80000)
	    return;
	for (dr = 0; dr < 4; dr++) {
	    drive *drv = &floppy[dr];
	    if (drv->motoroff)
		continue;
	    if ((selected & (1 << dr)) == 0) {
		int pos = drv->mfmpos & ~15;
		int i;

		if (adkcon & 0x400) {
		    for (i = 0; i < drv->tracklen; i += 16) {
			pos += 16;
			pos %= drv->tracklen;
			if (drv->bigmfmbuf[pos >> 4] == dsksync)
			    break;
		    }
		    if (i >= drv->tracklen)
			return;
		}
		while (dsklength-- > 0) {
		    put_word (dskpt, drv->bigmfmbuf[pos >> 4]);
		    dskpt += 2;
		    pos += 16;
		    pos %= drv->tracklen;
		}
		INTREQ (0x9000);
		linecounter = 2;
		dskdmaen = 0;
		return;
	    }
	}
    }
}

/* this unoptimized code should be enough (DSKBYTR and DSKDATR are used very rarely) */

uae_u16 DSKBYTR (int hpos)
{
    uae_u16 v;
    int i;

    i = 0;
    while (hpos > dskbytr_cycle[i])
	i++;
    v = dskbytr_tab[i];
    if (indexsync_last_cycle >= 0 && indexsync_last_cycle <= hpos)
	v |= 0x1000;
    dskbytr_tab[i] &= ~0x8000;
    if (dskdmaen && (dmacon & 0x210) == 0x210)
	v |= 0x4000;
    if (dskdmaen == 3)
	v |= 0x2000;
    return v;
}

uae_u16 DSKDATR (int hpos)
{
    int i;

    i = 0;
    while (hpos > dskdatr_cycle[i])
	i++;
    return dskdatr_tab[i];
}

void DSKSYNC (uae_u16 v)
{
    dsksync = v;
}

void DSKDAT (uae_u16 v)
{
    static int count = 0;
    if (count < 5) {
	count++;
	write_log ("DSKDAT written. Not good.");
	if (count == 5)
	    write_log ("(further messages suppressed)");

	write_log ("\n");
    }
}
void DSKPTH (uae_u16 v)
{
    dskpt = (dskpt & 0xffff) | ((uae_u32) v << 16);
}

void DSKPTL (uae_u16 v)
{
    dskpt = (dskpt & ~0xffff) | (v);
}

void DISK_init (void)
{
    int i;

    dma_tab[0] = 0xffffffff;
    dskbytr_cycle[0] = 255;
    dskdatr_cycle[0] = 255;
    indexsync_last_cycle = -1;

    for (i = 0; i < 4; i++)
	if (!drive_insert (floppy + i, i, currprefs.df[i]))
	    disk_eject (i);

    if (disk_empty (0))
	write_log ("No disk in drive 0.\n");
}

void dumpdisk (void)
{
    int i;
    for (i = 0; i < 4; i++) {
	drive *drv = &floppy[i];
	printf ("Drive %d: motor %s cylinder %2d selected %d\n", i, drv->motoroff ? "off" : " on", drv->cyl, selected & (1 << i));
    }
}

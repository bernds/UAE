/*

  FDI to raw bit stream converter
  Copyright (c) 2001 by Toni Wilen <twilen@arabuusimiehet.com>

  FDI format created by Vincent "ApH" Joguin

  Tiny changes - function type fixes, multiple drives, addition of
  get_last_head and C++ callability - by Thomas Harte, 2001,
  T.Harte@excite.co.uk


  This program is free software; you can redistribute it and/or modify it
  under the terms of the GNU General Public License as published by the Free
  Software Foundation; either version 2 of the License, or (at your option)
  any later version.

  This program is distributed in the hope that it will be useful, but WITHOUT
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
  more details.

  You should have received a copy of the GNU General Public License along
  with this program; if not, write to the Free Software Foundation, Inc.,
  59 Temple Place - Suite 330, Boston, MA  02111-1307, USA

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* IF UAE */
#include "sysconfig.h"
#include "sysdeps.h"
#include "zfile.h"
/* ELSE */
//#include "types.h"

#include "fdi2raw.h"

//#define DEBUG
//#define VERBOSE

#include <assert.h>

#ifdef DEBUG
static char *datalog(uae_u8 *src, int len)
{
	static char buf[1000];
	static int offset;
	int i = 0, offset2;

	offset2 = offset;
	buf[offset++]='\'';
	while(len--) {
		sprintf (buf + offset, "%02.2X", src[i]);
		offset += 2;
		i++;
		if (i > 10) break;
	}
	buf[offset++]='\'';
	buf[offset++] = 0;
	if (offset >= 900) offset = 0;
	return buf + offset2;
}
#else
static char *datalog(uae_u8 *src, int len) { return ""; }
#endif

#ifdef DEBUG
#define debuglog printf
#else
#define debuglog
#endif
#ifdef VERBOSE
#define outlog printf
#else
#define outlog
#endif

#define MAX_SRC_BUFFER 20000
#define MAX_DST_BUFFER 40000
#define MAX_MFM_SYNC_BUFFER 60000

struct fdi {
	uae_u8 *track_src_buffer;
	uae_u8 *track_src;
	int track_src_len;
	uae_u8 *track_dst_buffer;
	uae_u8 *track_dst;
	uae_u8 track_len;
	uae_u8 track_type;
	int current_track;
	int last_track;
	int last_head;
	int rotation_speed;
	int bit_rate;
	int disk_type;
	int write_protect;
	int err;
	uae_u8 header[512];
	int track_offsets[181];
	struct zfile *file;
	int out;
	int mfmsync_offset;
	int *mfmsync_buffer;
	/* sector described only */
	int index_offset;
	int encoding_type;
	/* bit handling */
	int nextdrop;
};

#define get_u32(x) ((((x)[0])<<24)|(((x)[1])<<16)|(((x)[2])<<8)|((x)[3]))

static int decode_raw_track (FDI *fdi)
{
	int size = get_u32(fdi->track_src);
	memcpy (fdi->track_dst, fdi->track_src, (size + 7) >> 3);
	fdi->track_src += (size + 7) >> 3;
	return size;
}

/* unknown track */
static void zxx (FDI *fdi)
{
	outlog ("track %d: unknown track type 0x%02.2X\n", fdi->current_track, fdi->track_type);
//	return -1;
}
/* unsupported track */
static void zyy (FDI *fdi)
{
	outlog ("track %d: unsupported track type 0x%02.2X\n", fdi->current_track, fdi->track_type);
//	return -1;
}
/* empty track */
static void track_empty (FDI *fdi)
{
//	return 0;
}

/* unknown sector described type */
static void dxx (FDI *fdi)
{
	outlog ("\ntrack %d: unknown sector described type 0x%02.2X\n", fdi->current_track, fdi->track_type);
	fdi->err = 1;
}
/* unsupported sector described type */
static void dyy (FDI *fdi)
{
	outlog ("\ntrack %d: unsupported sector described 0x%02.2X\n", fdi->current_track, fdi->track_type);
	fdi->err = 1;
}
/* add position of mfm sync bit */
static void add_mfm_sync_bit (FDI *fdi)
{
	if (fdi->nextdrop) {
		fdi->nextdrop = 0;
		return;
	}
	fdi->mfmsync_buffer[fdi->mfmsync_offset++] = fdi->out;
	if (fdi->out == 0) {
		outlog ("illegal position for mfm sync bit, offset=%d\n",fdi->out);
		fdi->err = 1;
	}
	if (fdi->mfmsync_offset >= MAX_MFM_SYNC_BUFFER) {
		fdi->mfmsync_offset = 0;
		outlog ("mfmsync buffer overflow\n");
		fdi->err = 1;
	}
	fdi->out++;
}

#define BIT_BYTEOFFSET ((fdi->out) >> 3)
#define BIT_BITOFFSET (7-((fdi->out)&7))

/* add one bit */
static void bit_add (FDI *fdi, int bit)
{
	if (fdi->nextdrop) {
		fdi->nextdrop = 0;
		return;
	}
	fdi->track_dst[BIT_BYTEOFFSET] &= ~(1 << BIT_BITOFFSET);
	if (bit) 
		fdi->track_dst[BIT_BYTEOFFSET] |= (1 << BIT_BITOFFSET);
	fdi->out++;
	if (fdi->out >= MAX_DST_BUFFER * 8) {
		outlog ("destination buffer overflow\n");
		fdi->err = 1;
		fdi->out = 1;
	}
}
/* add bit and mfm sync bit */
static void bit_mfm_add (FDI *fdi, int bit)
{
	add_mfm_sync_bit (fdi);
	bit_add (fdi, bit);
}
/* remove following bit */
static void bit_drop_next (FDI *fdi)
{
	if (fdi->nextdrop > 0) {
		outlog("multiple bit_drop_next() called");
	} else if (fdi->nextdrop < 0) {
		fdi->nextdrop = 0;
		debuglog(":DNN:");
		return;
	}
	debuglog(":DN:");
	fdi->nextdrop = 1;
}

/* ignore next bit_drop_next() */
static void bit_dedrop (FDI *fdi)
{
	if (fdi->nextdrop) {
		outlog("bit_drop_next called before bit_dedrop");
	}
	fdi->nextdrop = -1;
	debuglog(":BDD:");
}

/* add one byte */
static void byte_add (FDI *fdi, uae_u8 v)
{
	int i;
	for (i = 7; i >= 0; i--)
		bit_add (fdi, v & (1 << i));
}
/* add one word */
static void word_add (FDI *fdi, uae_u16 v)
{
	byte_add (fdi, (uae_u8)(v >> 8));
	byte_add (fdi, (uae_u8)v);
}
/* add one byte and mfm encode it */
static void byte_mfm_add (FDI *fdi, uae_u8 v)
{
	int i;
	for (i = 7; i >= 0; i--)
		bit_mfm_add (fdi, v & (1 << i));
}
/* add multiple bytes and mfm encode them */
static void bytes_mfm_add (FDI *fdi, uae_u8 v, int len)
{
	int i;
	for (i = 0; i < len; i++) byte_mfm_add (fdi, v);
}
/* add one mfm encoded word and re-mfm encode it */
static void word_post_mfm_add (FDI *fdi, uae_u16 v)
{
	int i;
	for (i = 14; i >= 0; i -= 2)
		bit_mfm_add (fdi, v & (1 << i));
}

/* bit 0 */
static void s00(FDI *fdi) { bit_add (fdi, 0); }
/* bit 1*/
static void s01(FDI *fdi) { bit_add (fdi, 1); }
/* 4489 */
static void s02(FDI *fdi) { word_add (fdi, 0x4489); }
/* 5224 */
static void s03(FDI *fdi) { word_add (fdi, 0x5224); }
/* mfm sync bit */
static void s04(FDI *fdi) { add_mfm_sync_bit (fdi); }
/* RLE MFM-encoded data */
static void s08(FDI *fdi)
{
	int bytes = *fdi->track_src++;
	uae_u8 byte = *fdi->track_src++;
	if (bytes == 0) bytes = 256;
	debuglog ("s08:len=%d,data=%02.2X",bytes,byte);
	while(bytes--) byte_add (fdi, byte);
}
/* RLE MFM-decoded data */
static void s09(FDI *fdi)
{
	int bytes = *fdi->track_src++;
	uae_u8 byte = *fdi->track_src++;
	if (bytes == 0) bytes = 256;
	bit_drop_next (fdi);
	debuglog ("s09:len=%d,data=%02.2X",bytes,byte);
	while(bytes--) byte_mfm_add (fdi, byte);
}
/* MFM-encoded data */
static void s0a(FDI *fdi)
{
	int i, bits = (fdi->track_src[0] << 8) | fdi->track_src[1];
	uae_u8 b;
	fdi->track_src += 2;
	debuglog ("s0a:bits=%d,data=%s", bits, datalog(fdi->track_src, (bits + 7) / 8));
	while (bits >= 8) {
		byte_add (fdi, *fdi->track_src++);
		bits -= 8;
	}
	if (bits > 0) {
		i = 7;
		b = *fdi->track_src++;
		while (bits--) {
			bit_add (fdi, b & (1 << i));
			i--;
		}
	}
}
/* MFM-encoded data */
static void s0b(FDI *fdi)
{
	int i, bits = ((fdi->track_src[0] << 8) | fdi->track_src[1]) + 65536;
	uae_u8 b;
	fdi->track_src += 2;
	debuglog ("s0b:bits=%d,data=%s", bits, datalog(fdi->track_src, (bits + 7) / 8));
	while (bits >= 8) {
		byte_add (fdi, *fdi->track_src++);
		bits -= 8;
	}
	if (bits > 0) {
		i = 7;
		b = *fdi->track_src++;
		while (bits--) {
			bit_add (fdi, b & (1 << i));
			i--;
		}
	}
}
/* MFM-decoded data */
static void s0c(FDI *fdi)
{
	int i, bits = (fdi->track_src[0] << 8) | fdi->track_src[1];
	uae_u8 b;
	fdi->track_src += 2;
	bit_drop_next (fdi);
	debuglog ("s0c:bits=%d,data=%s", bits, datalog(fdi->track_src, (bits + 7) / 8));
	while (bits >= 8) {
		byte_mfm_add (fdi, *fdi->track_src++);
		bits -= 8;
	}
	if (bits > 0) {
		i = 7;
		b = *fdi->track_src++;
		while(bits--) {
			bit_mfm_add (fdi, b & (1 << i));
			i--;
		}
	}
}
/* MFM-decoded data */
static void s0d(FDI *fdi)
{
	int i, bits = ((fdi->track_src[0] << 8) | fdi->track_src[1]) + 65536;
	uae_u8 b;
	fdi->track_src += 2;
	bit_drop_next (fdi);
	debuglog ("s0d:bits=%d,data=%s", bits, datalog(fdi->track_src, (bits + 7) / 8));
	while (bits >= 8) {
		byte_mfm_add (fdi, *fdi->track_src++);
		bits -= 8;
	}
	if (bits > 0) {
		i = 7;
		b = *fdi->track_src++;
		while(bits--) {
			bit_mfm_add (fdi, b & (1 << i));
			i--;
		}
	}
}

/* ***** */
/* AMIGA */
/* ***** */

/* just for testing integrity of Amiga sectors */

#define MFMMASK 0x55555555
static uae_u32 getmfmlong (uae_u16 * mbuf)
{
	return ((*mbuf << 16) | *(mbuf + 1)) & MFMMASK;
}

static int amiga_check_track (FDI *fdi)
{
	int i, secwritten = 0;
	int fwlen = fdi->out / 8;
	int length = 2 * fwlen;
	int drvsec = 11;
	uae_u32 odd, even, chksum, id, dlong;
	uae_u8 *secdata;
	uae_u8 secbuf[544];
	uae_u16 bigmfmbuf[30000];
	uae_u16 *mbuf = bigmfmbuf;
	uae_u16 *mend = mbuf + length;
	char sectable[22];
	uae_u8 *raw = fdi->track_dst_buffer;
	int slabel;

	for (i = 0; i < (fdi->out + 15) / 16; i++)
		bigmfmbuf[i] = raw[i * 2 + 0] * 256 + raw[i * 2 + 1];

	memset (sectable, 0, sizeof (sectable));
	//memcpy (mbuf + fwlen, mbuf, fwlen * sizeof (uae_u16));
	mend -= (4 + 16 + 8 + 512);

	while (secwritten < drvsec) {
		int trackoffs;

		do {
			while (*mbuf++ != 0x4489) {
				if (mbuf >= mend) return 1;
			}
		} while (*mbuf++ != 0x4489);

		odd = getmfmlong (mbuf);
		even = getmfmlong (mbuf + 2);
		mbuf += 4;
		id = (odd << 1) | even;

		trackoffs = (id & 0xff00) >> 8;
		if (trackoffs + 1 > drvsec) {
			debuglog("illegal sector offset %d\n",trackoffs);
			continue;
		}
		chksum = odd ^ even;
		slabel = 0;
		for (i = 0; i < 4; i++) {
			odd = getmfmlong (mbuf);
			even = getmfmlong (mbuf + 8);
			mbuf += 2;

			dlong = (odd << 1) | even;
			if (dlong) slabel = 1;
			chksum ^= odd ^ even;
		}
		mbuf += 8;
		odd = getmfmlong (mbuf);
		even = getmfmlong (mbuf + 2);
		mbuf += 4;
		if (((odd << 1) | even) != chksum) {
			debuglog("sector header crc error\n");
			continue;
		}
		if (((id & 0x00ff0000) >> 16) != (uae_u32)fdi->current_track) {
			debuglog("illegal track number %d <> %d\n",fdi->current_track,(id & 0x00ff0000) >> 16);
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
			*secdata++ = (uae_u8) (dlong >> 24);
			*secdata++ = (uae_u8) (dlong >> 16);
			*secdata++ = (uae_u8) (dlong >> 8);
			*secdata++ = (uae_u8) dlong;
			chksum ^= odd ^ even;
		}
		mbuf += 256;
		if (chksum) {
			debuglog("sector %d data checksum error",trackoffs);
		} else {
			debuglog("sector %d ok ",trackoffs);
			if (slabel) debuglog("(non-empty sector header)");
		}
		debuglog("\n");
		sectable[trackoffs] = 1;
		secwritten++;
	}	
	return 0;
}

static void amiga_data_raw (FDI *fdi, uae_u8 *secbuf, uae_u8 *crc, int len)
{
	int i;
	uae_u8 crcbuf[4];

	if (!crc) {
		memset (crcbuf, 0, 4);
	} else {
		memcpy (crcbuf, crc ,4);
	}
	for (i = 0; i < 4; i++)
		byte_mfm_add (fdi, crcbuf[i]);
	for (i = 0; i < len; i++)
		byte_mfm_add (fdi, secbuf[i]);
}

static void amiga_data (FDI *fdi, uae_u8 *secbuf)
{
	uae_u16 mfmbuf[4 + 512];
	uae_u32 dodd, deven, dck;
	int i;

	for (i = 0; i < 512; i += 4) {
		deven = ((secbuf[i + 0] << 24) | (secbuf[i + 1] << 16)
		 | (secbuf[i + 2] << 8) | (secbuf[i + 3]));
		dodd = deven >> 1;
		deven &= 0x55555555;
		dodd &= 0x55555555;
		mfmbuf[(i >> 1) + 4] = (uae_u16) (dodd >> 16);
		mfmbuf[(i >> 1) + 5] = (uae_u16) dodd;
		mfmbuf[(i >> 1) + 256 + 4] = (uae_u16) (deven >> 16);
		mfmbuf[(i >> 1) + 256 + 5] = (uae_u16) deven;
	}
	dck = 0;
	for (i = 4; i < 4 + 512; i += 2)
		dck ^= (mfmbuf[i] << 16) | mfmbuf[i + 1];
	deven = dodd = dck;
	dodd >>= 1;
	deven &= 0x55555555;
	dodd &= 0x55555555;
	mfmbuf[0] = (uae_u16) (dodd >> 16);
	mfmbuf[1] = (uae_u16) dodd;
	mfmbuf[2] = (uae_u16) (deven >> 16);
	mfmbuf[3] = (uae_u16) deven;

	for (i = 0; i < 4 + 512; i ++)
		word_post_mfm_add (fdi, mfmbuf[i]);
}

static void amiga_sector_header (FDI *fdi, uae_u8 *header, uae_u8 *data, int sector, int untilgap)
{
	uae_u8 headerbuf[4], databuf[16];
	uae_u32 deven, dodd, hck;
	uae_u16 mfmbuf[24];
	int i;

	byte_mfm_add (fdi, 0);
	byte_mfm_add (fdi, 0);
	word_add (fdi, 0x4489);
	word_add (fdi, 0x4489);
	if (header) {
		memcpy (headerbuf, header, 4);
	} else {
		headerbuf[0] = 0xff;
		headerbuf[1] = (uae_u8)fdi->current_track;
		headerbuf[2] = (uae_u8)sector;
		headerbuf[3] = (uae_u8)untilgap;
	}
	if (data)
		memcpy (databuf, data, 16);
	else
		memset (databuf, 0, 16);

	deven = ((headerbuf[0] << 24) | (headerbuf[1] << 16)
		| (headerbuf[2] << 8) | (headerbuf[3]));
	dodd = deven >> 1;
	deven &= 0x55555555;
	dodd &= 0x55555555;
	mfmbuf[0] = (uae_u16) (dodd >> 16);
	mfmbuf[1] = (uae_u16) dodd;
	mfmbuf[2] = (uae_u16) (deven >> 16);
	mfmbuf[3] = (uae_u16) deven;
	for (i = 0; i < 16; i += 4) {
		deven = ((databuf[i] << 24) | (databuf[i + 1] << 16)
			| (databuf[i + 2] << 8) | (databuf[i + 3]));
		dodd = deven >> 1;
		deven &= 0x55555555;
		dodd &= 0x55555555;
		mfmbuf[(i >> 1) + 0 + 4] = (uae_u16) (dodd >> 16);
		mfmbuf[(i >> 1) + 0 + 5] = (uae_u16) dodd;
		mfmbuf[(i >> 1) + 8 + 4] = (uae_u16) (deven >> 16);
		mfmbuf[(i >> 1) + 8 + 5] = (uae_u16) deven;
	}
	hck = 0;
	for (i = 0; i < 4 + 16; i += 2)
		hck ^= (mfmbuf[i] << 16) | mfmbuf[i + 1];
	deven = dodd = hck;
	dodd >>= 1;
	deven &= 0x55555555;
	dodd &= 0x55555555;
	mfmbuf[20] = (uae_u16) (dodd >> 16);
	mfmbuf[21] = (uae_u16) dodd;
	mfmbuf[22] = (uae_u16) (deven >> 16);
	mfmbuf[23] = (uae_u16) deven;

	for (i = 0; i < 4 + 16 + 4; i ++)
		word_post_mfm_add (fdi, mfmbuf[i]);
}

/* standard super-extended Amiga sector header */
static void s20(FDI *fdi)
{
	bit_drop_next (fdi);
	debuglog ("s20:header=%s,data=%s", datalog(fdi->track_src, 4), datalog(fdi->track_src + 4, 16)); 
	amiga_sector_header (fdi, fdi->track_src, fdi->track_src + 4, 0, 0);
	fdi->track_src += 4 + 16;
}
/* standard extended Amiga sector header */
static void s21(FDI *fdi)
{
	bit_drop_next (fdi);
	debuglog ("s21:header=%s", datalog(fdi->track_src, 4)); 
	amiga_sector_header (fdi, fdi->track_src, 0, 0, 0);
	fdi->track_src += 4;
}
/* standard Amiga sector header */
static void s22(FDI *fdi)
{
	bit_drop_next (fdi);
	debuglog("s22:sector=%d,untilgap=%d", fdi->track_src[0], fdi->track_src[1]);
	amiga_sector_header (fdi, 0, 0, fdi->track_src[0], fdi->track_src[1]);
	fdi->track_src += 2;
}
/* standard 512-byte, CRC-correct Amiga data */
static void s23(FDI *fdi)
{
	debuglog("s23:data=%s", datalog (fdi->track_src, 512));
	amiga_data (fdi, fdi->track_src);
	fdi->track_src += 512;
}
/* not-decoded, 128*2^x-byte, CRC-correct Amiga data */
static void s24(FDI *fdi)
{
	int shift = *fdi->track_src++;
	debuglog("s24:shift=%d,data=%s", shift, datalog (fdi->track_src, 128 << shift));
	amiga_data_raw (fdi, fdi->track_src, 0, 128 << shift);
	fdi->track_src += 128 << shift;
}
/* not-decoded, 128*2^x-byte, CRC-incorrect Amiga data */
static void s25(FDI *fdi)
{
	int shift = *fdi->track_src++;
	debuglog("s25:shift=%d,crc=%s,data=%s", shift, datalog (fdi->track_src, 4), datalog (fdi->track_src + 4, 128 << shift));
	amiga_data_raw (fdi, fdi->track_src + 4, fdi->track_src, 128 << shift);
	fdi->track_src += 4 + (128 << shift);
}
/* standard extended Amiga sector */
static void s26(FDI *fdi)
{
	s21 (fdi);
	debuglog("s26:data=%s", datalog (fdi->track_src, 512));
	amiga_data (fdi, fdi->track_src);
	fdi->track_src += 512;
}
/* standard short Amiga sector */
static void s27(FDI *fdi)
{
	s22 (fdi);
	debuglog("s27:data=%s", datalog (fdi->track_src, 512));
	amiga_data (fdi, fdi->track_src);
	fdi->track_src += 512;
}

/* *** */
/* IBM */
/* *** */

static uae_u16 ibm_crc (uae_u8 byte, int reset)
{
	static uae_u16 crc;
	int i;

	if (reset) crc = 0xcdb4;
	for (i = 0; i < 8; i++) {
		if (crc & 0x8000) {
			crc <<= 1;
			if (!(byte & 0x80)) crc ^= 0x1021;
		} else {
			crc <<= 1;
			if (byte & 0x80) crc ^= 0x1021;
		}
		byte <<= 1;
	}
	return crc;
}

static void ibm_data (FDI *fdi, uae_u8 *data, uae_u8 *crc, int len)
{
	int i;
	uae_u8 crcbuf[2];
	uae_u16 crcv;

	word_add (fdi, 0x4489);
	word_add (fdi, 0x4489);
	word_add (fdi, 0x4489);
	byte_mfm_add (fdi, 0xfb);
	ibm_crc (0xfb, 1);
	for (i = 0; i < len; i++) {
		byte_mfm_add (fdi, data[i]);
		crcv = ibm_crc (data[i], 0);
	}
	if (!crc) {
		crc = crcbuf;
		crc[0] = (uae_u8)(crcv >> 8);
		crc[1] = (uae_u8)crcv;
	}
	byte_mfm_add (fdi, crc[0]);
	byte_mfm_add (fdi, crc[1]);
}

static void ibm_sector_header (FDI *fdi, uae_u8 *data, uae_u8 *crc, int secnum, int pre)
{
	uae_u8 secbuf[5];
	uae_u8 crcbuf[2];
	uae_u16 crcv;
	int i;

	if (pre)
		bytes_mfm_add (fdi, 0, 12);
	word_add (fdi, 0x4489);
	word_add (fdi, 0x4489);
	word_add (fdi, 0x4489);
	secbuf[0] = 0xfe;
	if (secnum >= 0) {
		secbuf[1] = (uae_u8)(fdi->current_track/2);
		secbuf[2] = (uae_u8)(fdi->current_track%2);
		secbuf[3] = (uae_u8)secnum;
		secbuf[4] = 2;
	} else {
		memcpy (secbuf + 1, data, 4);
	}
	ibm_crc (secbuf[0], 1);
	ibm_crc (secbuf[1], 0);
	ibm_crc (secbuf[2], 0);
	ibm_crc (secbuf[3], 0);
	crcv = ibm_crc (secbuf[4], 0);
	if (crc) {
		memcpy (crcbuf, crc, 2);
	} else {
		crcbuf[0] = (uae_u8)(crcv >> 8);
		crcbuf[1] = (uae_u8)crcv;
	}
	/* data */
	for (i = 0;i < 5; i++)
		byte_mfm_add (fdi, secbuf[i]);
	/* crc */
	byte_mfm_add (fdi, crcbuf[0]);
	byte_mfm_add (fdi, crcbuf[1]);
}

/* standard IBM index address mark */
static void s10(FDI *fdi)
{
	bit_drop_next (fdi);
	bytes_mfm_add (fdi, 0, 12);
	word_add (fdi, 0x5224);
	word_add (fdi, 0x5224);
	word_add (fdi, 0x5224);
	byte_mfm_add (fdi, 0xfc);
}
/* standard IBM pre-gap */
static void s11(FDI *fdi)
{
	bit_drop_next (fdi);
	bytes_mfm_add (fdi, 0x4e, 78);
	bit_dedrop (fdi);
	s10 (fdi);
	bytes_mfm_add (fdi, 0x4e, 50);
}
/* standard ST pre-gap */
static void s12(FDI *fdi)
{
	bit_drop_next (fdi);
	bytes_mfm_add (fdi, 0x4e, 78);
}
/* standard extended IBM sector header */
static void s13(FDI *fdi)
{
	bit_drop_next (fdi);
	debuglog ("s13:header=%s", datalog (fdi->track_src, 4));
	ibm_sector_header (fdi, fdi->track_src, 0, -1, 1);
	fdi->track_src += 4;
}
/* standard mini-extended IBM sector header */
static void s14(FDI *fdi)
{
	debuglog ("s14:header=%s", datalog (fdi->track_src, 4));
	ibm_sector_header (fdi, fdi->track_src, 0, -1, 0);
	fdi->track_src += 4;
}
/* standard short IBM sector header */
static void s15(FDI *fdi)
{
	bit_drop_next (fdi);
	debuglog ("s15:sector=%d", *fdi->track_src);
	ibm_sector_header (fdi, 0, 0, *fdi->track_src++, 1);
}
/* standard mini-short IBM sector header */
static void s16(FDI *fdi)
{
	debuglog ("s16:track=%d", *fdi->track_src);
	ibm_sector_header (fdi, 0, 0, *fdi->track_src++, 0);
}
/* standard CRC-incorrect mini-extended IBM sector header */
static void s17(FDI *fdi)
{
	debuglog ("s17:header=%s,crc=%s", datalog (fdi->track_src, 4), datalog (fdi->track_src + 4, 2));
	ibm_sector_header (fdi, fdi->track_src, fdi->track_src + 4, -1, 0);
	fdi->track_src += 4 + 2;
}
/* standard CRC-incorrect mini-short IBM sector header */
static void s18(FDI *fdi)
{
	debuglog ("s18:sector=%d,header=%s", *fdi->track_src, datalog (fdi->track_src + 1, 4));
	ibm_sector_header (fdi, 0, fdi->track_src + 1, *fdi->track_src, 0);
	fdi->track_src += 1 + 4;
}
/* standard 512-byte CRC-correct IBM data */
static void s19(FDI *fdi)
{
	debuglog ("s19:data=%s", datalog (fdi->track_src , 512));
	ibm_data (fdi, fdi->track_src, 0, 512);
	fdi->track_src += 512;
}
/* standard 128*2^x-byte-byte CRC-correct IBM data */
static void s1a(FDI *fdi)
{
	int shift = *fdi->track_src++;
	debuglog ("s1a:shift=%d,data=%s", shift, datalog (fdi->track_src , 128 << shift));
	ibm_data (fdi, fdi->track_src, 0, 128 << shift);
	fdi->track_src += 128 << shift;
}
/* standard 128*2^x-byte-byte CRC-incorrect IBM data */
static void s1b(FDI *fdi)
{
	int shift = *fdi->track_src++;
	debuglog ("s1b:shift=%d,crc=%s,data=%s", shift, datalog (fdi->track_src + (128 << shift), 2), datalog (fdi->track_src , 128 << shift));
	ibm_data (fdi, fdi->track_src, fdi->track_src + (128 << shift), 128 << shift);
	fdi->track_src += (128 << shift) + 2;
}
/* standard extended IBM sector */
static void s1c(FDI *fdi)
{
	int shift = fdi->track_src[3];
	s13 (fdi);
	bytes_mfm_add (fdi, 0x4e, 22);
	bytes_mfm_add (fdi, 0x00, 12);
	ibm_data (fdi, fdi->track_src, 0, 128 << shift);
	fdi->track_src += 128 << shift;
}
/* standard short IBM sector */
static void s1d(FDI *fdi)
{
	s15 (fdi);
	bytes_mfm_add (fdi, 0x4e, 22);
	bytes_mfm_add (fdi, 0x00, 12);
	s19 (fdi);
}

/* end marker */
static void sff(FDI *fdi)
{
}

typedef void (*decode_described_track_func)(FDI*);

static decode_described_track_func decode_sectors_described_track[] =
{
	s00,s01,s02,s03,s04,dxx,dxx,dxx,s08,s09,s0a,s0b,s0c,s0d,dxx,dxx, /* 00-0F */
	s10,s11,s12,s13,s14,s15,s16,s17,s18,s19,s1a,s1b,s1c,s1d,dxx,dxx, /* 10-1F */
	s20,s21,s22,s23,s24,s25,s26,s27,dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx, /* 20-2F */
	dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx, /* 30-3F */
	dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx, /* 40-4F */
	dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx, /* 50-5F */
	dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx, /* 60-6F */
	dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx, /* 70-7F */
	dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx, /* 80-8F */
	dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx, /* 90-9F */
	dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx, /* A0-AF */
	dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx, /* B0-BF */
	dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx, /* C0-CF */
	dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx, /* D0-DF */
	dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx, /* E0-EF */
	dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx,dxx,sff  /* F0-FF */
};

static void track_amiga (struct fdi *fdi, int first_sector, int max_sector)
{
	int i;

	bit_add (fdi, 0);
	bit_drop_next (fdi);
	for (i = 0; i < max_sector; i++) {
		amiga_sector_header (fdi, 0, 0, first_sector, max_sector - i);
		amiga_data (fdi, fdi->track_src + first_sector * 512);
		first_sector++;
		if (first_sector >= max_sector) first_sector = 0;
	}
	bytes_mfm_add (fdi, 0, 260); /* gap */
}
static void track_atari_st (struct fdi *fdi, int max_sector)
{
	int i, gap3;
	uae_u8 *p = fdi->track_src;

	switch (max_sector)
		{
		case 9:
		gap3 = 40;
		break;
		case 10:
		gap3 = 24;
		break;
	}
	s15 (fdi);
	for (i = 0; i < max_sector; i++) {
		byte_mfm_add (fdi, 0x4e);
		byte_mfm_add (fdi, 0x4e);
		ibm_sector_header (fdi, 0, 0, fdi->current_track, 1);
		ibm_data (fdi, p + i * 512, 0, 512);
		bytes_mfm_add (fdi, 0x4e, gap3);
	}
	bytes_mfm_add (fdi, 0x4e, 660 - gap3);
	fdi->track_src += fdi->track_len * 256;
}
static void track_pc (struct fdi *fdi, int max_sector)
{
	int i, gap3;
	uae_u8 *p = fdi->track_src;

	switch (max_sector)
		{
		case 8:
		gap3 = 116;
		break;
		case 9:
		gap3 = 54;
		break;
		default:
		gap3 = 100; /* fixme */
		break;
	}
	s11 (fdi);
	for (i = 0; i < max_sector; i++) {
		byte_mfm_add (fdi, 0x4e);
		byte_mfm_add (fdi, 0x4e);
		ibm_sector_header (fdi, 0, 0, fdi->current_track, 1);
		ibm_data (fdi, p + i * 512, 0, 512);
		bytes_mfm_add (fdi, 0x4e, gap3);
	}
	bytes_mfm_add (fdi, 0x4e, 600 - gap3);
	fdi->track_src += fdi->track_len * 256;
}

/* amiga dd */
static void track_amiga_dd (struct fdi *fdi)
{
	uae_u8 *p = fdi->track_src;
	track_amiga (fdi, fdi->track_len >> 4, 11);
	fdi->track_src = p + (fdi->track_len & 15) * 512;
}
/* amiga hd */
static void track_amiga_hd (struct fdi *fdi)
{
	uae_u8 *p = fdi->track_src;
	track_amiga (fdi, 0, 22);
	fdi->track_src = p + fdi->track_len * 256;
}
/* atari st 9 sector */
static void track_atari_st_9 (struct fdi *fdi)
{
	track_atari_st (fdi, 9);
}
/* atari st 10 sector */
static void track_atari_st_10 (struct fdi *fdi)
{
	track_atari_st (fdi, 10);
}
/* pc 8 sector */
static void track_pc_8 (struct fdi *fdi)
{
	track_pc (fdi, 8);
}
/* pc 9 sector */
static void track_pc_9 (struct fdi *fdi)
{
	track_pc (fdi, 9);
}
/* pc 15 sector */
static void track_pc_15 (struct fdi *fdi)
{
	track_pc (fdi, 15);
}
/* pc 18 sector */
static void track_pc_18 (struct fdi *fdi)
{
	track_pc (fdi, 18);
}
/* pc 36 sector */
static void track_pc_36 (struct fdi *fdi)
{
	track_pc (fdi, 36);
}

typedef void (*decode_normal_track_func)(FDI*);

static decode_normal_track_func decode_normal_track[] =
{ 
	track_empty, /* 0 */
	track_amiga_dd, track_amiga_hd, /* 1-2 */
	track_atari_st_9, track_atari_st_10, /* 3-4 */
	track_pc_8, track_pc_9, track_pc_15, track_pc_18, track_pc_36, /* 5-9 */
	zxx,zxx,zxx,zxx,zxx /* A-F */
};

static void fix_mfm_sync (FDI *fdi)
{
	int i, pos, off1, off2, off3, mask1, mask2, mask3;

	for (i = 0; i < fdi->mfmsync_offset; i++) {
		pos = fdi->mfmsync_buffer[i];
		off1 = (pos - 1) >> 3;
		off2 = (pos + 1) >> 3;
		off3 = pos >> 3;
		mask1 = 1 << (7 - ((pos - 1) & 7));
		mask2 = 1 << (7 - ((pos + 1) & 7));
		mask3 = 1 << (7 - (pos & 7));
		if (!(fdi->track_dst[off1] & mask1) && !(fdi->track_dst[off2] & mask2))
			fdi->track_dst[off3] |= mask3;
		else
			fdi->track_dst[off3] &= ~mask3;
	}
}

static int handle_sectors_described_track (FDI *fdi)
{
	int oldout;
	uae_u8 *start_src = fdi->track_src ;
	fdi->encoding_type = *fdi->track_src++;
	fdi->index_offset = get_u32(fdi->track_src);
	fdi->index_offset >>= 8;
	fdi->track_src += 3;
	outlog ("sectors_described, index offset: %d\n",fdi->index_offset);

	do {
		fdi->track_type = *fdi->track_src++;
		outlog ("%06.6X %06.6X %02.2X:",fdi->track_src - start_src + 0x200, fdi->out/8, fdi->track_type);
		oldout = fdi->out;
		decode_sectors_described_track[fdi->track_type](fdi);
		outlog(" %d\n", fdi->out - oldout);
		oldout = fdi->out;
		if (fdi->out < 0 || fdi->err) {
			outlog ("\nin %d bytes, out %d bits\n", fdi->track_src - fdi->track_src_buffer, fdi->out);
			return -1;
		}
		if (fdi->track_src - fdi->track_src_buffer >= fdi->track_src_len) {
			outlog ("source buffer overrun, previous type: %02.2X\n", fdi->track_type);
			return -1;
		}
	} while (fdi->track_type != 0xff);
	outlog("\n");
	fix_mfm_sync (fdi);
	return fdi->out;
}

static unsigned char fdiid[]={"Formatted Disk Image file"};
static int bit_rate_table[16] = { 125,150,250,300,500,1000 };

void fdi2raw_header_free (FDI *fdi)
{
	free (fdi->mfmsync_buffer);
	fdi->mfmsync_buffer = 0;
	free (fdi->track_src_buffer);
	fdi->track_src_buffer = 0;
	free (fdi->track_dst_buffer);
	fdi->track_dst_buffer = 0;
	memset (fdi, 0, sizeof (FDI));
}

int fdi2raw_get_last_track (FDI *fdi)
{
	return fdi->last_track;
}

int fdi2raw_get_last_head (FDI *fdi)
{
	return fdi->last_head;
}

int fdi2raw_get_rotation (FDI *fdi)
{
	return fdi->rotation_speed;
}

int fdi2raw_get_bit_rate (FDI *fdi)
{
	return fdi->bit_rate;
}

int fdi2raw_get_type (FDI *fdi)
{
	return fdi->disk_type;
}

int fdi2raw_get_write_protect (FDI *fdi)
{
	return fdi->write_protect;
}

FDI *fdi2raw_header(struct zfile *f)
{
	int i, offset, oldseek;
	uae_u8 type, size;
	FDI *fdi;

	fdi = (FDI *)malloc(sizeof(FDI));

	fdi->file = f;
	oldseek = zfile_ftell (fdi->file);
	zfile_fseek (fdi->file, 0, SEEK_SET);
	zfile_fread (fdi->header, 512, 1, fdi->file);
	zfile_fseek (fdi->file, oldseek, SEEK_SET);
	if (memcmp (fdiid, fdi->header, strlen (fdiid)) ) { free(fdi); return NULL;}
	if (fdi->header[140] != 1 || fdi->header[141] != 0) {free(fdi); return NULL;}

	fdi->mfmsync_buffer = malloc (MAX_MFM_SYNC_BUFFER * sizeof(int));
	fdi->track_dst_buffer = malloc (MAX_DST_BUFFER);
	fdi->track_src_buffer = malloc (MAX_SRC_BUFFER);
	
	fdi->last_track = ((fdi->header[142] << 8) + fdi->header[143]) + 1;
	fdi->last_track *= fdi->header[144] + 1;
	fdi->last_head = fdi->header[144];
	fdi->disk_type = fdi->header[145];
	fdi->rotation_speed = fdi->header[146] + 128;
	fdi->write_protect = fdi->header[147] & 1;
	outlog ("last_track=%d rotation_speed=%d\n",fdi->last_track,fdi->rotation_speed);

	offset = 512;
	for (i = 0; i < fdi->last_track; i++) {
		fdi->track_offsets[i] = offset;
		type = fdi->header[152 + i * 2];
		size = fdi->header[152 + i * 2 + 1];
		if (type == 1) offset += (size & 15) * 512; else offset += size * 256;
	}
	fdi->track_offsets[i] = offset;

	return fdi;
}


uae_u8 *fdi2raw_read_track(FDI *fdi, int track, int *len)
{
	uae_u8 *p;
	int outlen;

	*len = -1;
	fdi->err = 0;
	fdi->track_src_len = fdi->track_offsets[track + 1] - fdi->track_offsets[track];
	memset (fdi->track_src_buffer, 0, MAX_SRC_BUFFER);
	zfile_fseek (fdi->file, fdi->track_offsets[track], SEEK_SET);
	zfile_fread (fdi->track_src_buffer, fdi->track_src_len, 1, fdi->file);
	memset (fdi->track_dst_buffer, 0, MAX_DST_BUFFER);

	fdi->current_track = track;
	fdi->track_src = fdi->track_src_buffer;
	fdi->track_dst = fdi->track_dst_buffer;
	p = fdi->header + 152 + fdi->current_track * 2;
	fdi->track_type = *p++;
	fdi->track_len = *p++;
	fdi->bit_rate = 0;
	fdi->out = 0;
	fdi->mfmsync_offset = 0;

	if ((fdi->track_type & 0xf0) == 0xf0 || (fdi->track_type & 0xf0) == 0xe0)
		fdi->bit_rate = bit_rate_table[fdi->track_type & 0x0f];
	else
		fdi->bit_rate = 250;

	outlog ("track %d: srclen: %d track_type: %02.2X, bitrate: %d\n",fdi->current_track, fdi->track_src_len, fdi->track_type, fdi->bit_rate);

	if ((fdi->track_type & 0xf0) == 0xf0) {

		outlen = decode_raw_track (fdi);

	} else if ((fdi->track_type & 0xf0) == 0xe0) {

		outlen = handle_sectors_described_track (fdi);

	} else if ((fdi->track_type & 0xf0)) {

		zxx (fdi);
		outlen = -1;
		
	} else if (fdi->track_type < 0x10) {

		decode_normal_track[fdi->track_type](fdi);
		fix_mfm_sync (fdi);
		outlen = fdi->out;

	} else {

		zxx (fdi);
		outlen = -1;

	}

	if (fdi->err) outlen = -1;
	*len = outlen;
	return fdi->track_dst_buffer;
}


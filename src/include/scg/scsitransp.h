/* @(#)scsitransp.h	1.17 98/11/02 Copyright 1995 J. Schilling */
/*
 *	Definitions for commands that use functions from scsitransp.c
 *
 *	Copyright (c) 1995 J. Schilling
 */
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; see the file COPYING.  If not, write to
 * the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef	_SCSITRANSP_H
#define	_SCSITRANSP_H

#include "scg/standard.h"

typedef struct {
	int	scsibus;	/* SCSI bus #    for next I/O		*/
	int	target;		/* SCSI target # for next I/O		*/
	int	lun;		/* SCSI lun #    for next I/O		*/

	int	kdebug;		/* Kernel debug value for next I/O	*/
	int	debug;		/* Debug value for SCSI library		*/
	int	silent;		/* Be silent if value > 0		*/
	int	verbose;	/* Be verbose if value > 0		*/
	int	disre_disable;
	int	deftimeout;
	int	noparity;	/* Do not use SCSI parity fo next I/O	*/
	int	dev;		/* from scsi_cdr.c			*/
	struct timeval	*cmdstart;
	struct timeval	*cmdstop;
	const char	**nonstderrs;
	char	*cmdname;
	struct scg_cmd *scmd;
	struct scsi_inquiry *inq;
	struct scsi_capacity *cap;

        /* the rest is _private_ - don't touch it! */
        struct SCSIPrivate *private;
} SCSI;

/*
 * From scsitransp.c:
 */
extern	int	scsi_open		__PR((SCSI *scgp, char *device, int busno, int tgt, int tlun));
extern	BOOL	scsi_havebus		__PR((SCSI *scgp, int));
extern	int	scsi_fileno		__PR((SCSI *scgp, int, int, int));
extern	int	scsi_isatapi		__PR((SCSI *scgp));
extern	int	scsireset		__PR((SCSI *scgp));
extern	void	*scsi_getbuf		__PR((SCSI *scgp, long));
extern	long	scsi_bufsize		__PR((SCSI *scgp, long));
extern	void	scsi_setnonstderrs	__PR((SCSI *scgp, const char **));
extern	int	scsicmd			__PR((SCSI *scgp));
extern	int	scsigetresid		__PR((SCSI *scgp));
extern	void	scsiprinterr		__PR((SCSI *scgp));
extern	void	scsiprintcdb		__PR((SCSI *scgp));
extern	void	scsiprintwdata		__PR((SCSI *scgp));
extern	void	scsiprintrdata		__PR((SCSI *scgp));
extern	void	scsiprintresult		__PR((SCSI *scgp));
extern	void	scsiprintstatus		__PR((SCSI *scgp));
extern	void	scsiprbytes		__PR((char *, unsigned char *, int));
extern	void	scsiprsense		__PR((unsigned char *, int));
extern	int	scsi_sense_key		__PR((SCSI *scgp));
extern	int	scsi_sense_code		__PR((SCSI *scgp));
extern	int	scsi_sense_qual		__PR((SCSI *scgp));
#ifdef	_SCSIREG_H
extern	void	scsiprintdev		__PR((struct scsi_inquiry *));
#endif

/*
 * From scsierrmsg.c:
 */
extern	const char	*scsisensemsg	__PR((int, int, int,
						const char **, char *));
#ifdef	_SCGIO_H
extern	void		scsierrmsg	__PR((SCSI *scgp, struct scsi_sense *,
						struct scsi_status *,
						int, const char **));
#endif

#endif	/* _SCSITRANSP_H */

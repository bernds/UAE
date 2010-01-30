/* @(#)utypes.h	1.3 98/06/20 Copyright 1997 J. Schilling */
/*
 *	Definitions for some user defined types
 *
 *	Copyright (c) 1997 J. Schilling
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

#ifndef	_UTYPES_H
#define	_UTYPES_H

/*
 * Several unsigned cardinal types
 */
typedef	unsigned long	Ulong;
typedef	unsigned int	Uint;
typedef	unsigned short	Ushort;
typedef	unsigned char	Uchar;

/*
 * This is a definition for a compiler dependant 64 bit type.
 * It currently is silently a long if the compiler does not
 * support it. Check if this is the right way.
 */
#ifndef	NO_LONGLONG
#	if	defined(HAVE_LONGLONG)
#		define	USE_LONGLONG
#	endif
#endif

#ifdef	USE_LONGLONG

typedef	long long		Llong;
typedef	unsigned long long	Ullong;

#else

typedef	long			Llong;
typedef	unsigned long		Ullong;

#endif

/*
 * The IBM AIX C-compiler seems to be the only compiler on the world
 * which does not allow to use unsigned char bit fields as a hint
 * for packed bit fields. Define a pesical type to avoid warnings.
 * The packed attribute is honored wit unsigned int in this case too.
 */
#ifdef	_AIX

typedef unsigned int	Ucbit;

#else

typedef unsigned char	Ucbit;

#endif

#endif	/* _UTYPES_H */

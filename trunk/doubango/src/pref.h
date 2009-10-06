/*
	** Copyright (C) 2009 Doubango Telecom <imsframework(at)gmail.com> 

	This file is part of 'Doubango IMS Client Framework'.

    'Doubango IMS Client Framework' is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'Doubango IMS Client Framework' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with 'Doubango IMS Client Framework'.  If not, see <http://www.gnu.org/licenses/>.

	** For commercial use, you must purchase a proprietary license from Doubango Telecom.
*/

/* Copyright (C) 2009 Doubango Telecom
*
* Authors:	F0076RTY <imsframework@gmail.com>
*
*/

#ifndef __DOUBANGO_PREF_H__
#define __DOUBANGO_PREF_H__

#define PREF_NAMESPACE_USE		1
#define PREF_NAMESPACE_VALUE	dgo

#if PREF_NAMESPACE_USE 
#	define PREF_NAMESPACE_START namespace PREF_NAMESPACE_VALUE {
#	define PREF_NAMESPACE_END	} 
#else
#	define PREF_NAMESPACE_START
#	define PREF_NAMESPACE_END
#endif

#endif /* __DOUBANGO_PREF_H__ */
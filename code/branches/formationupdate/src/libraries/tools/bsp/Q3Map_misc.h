/*
===========================================================================
Copyright (C) 2008 Daniel Örstadius
Copyright (C) 2009 Jared Prince

This file is part of bsp-renderer source code.

bsp-renderer is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

bsp-renderer is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with bsp-renderer.  If not, see <http://www.gnu.org/licenses/>.

*/

// misc.h -- various definitions and functions

#ifndef _MISC_H
#define _MISC_H

#define DELETE_POINTER(x) if(x) { delete x		; x=0 ; }
#define DELETE_ARRAY(x)		if(x) { delete [] x ; x=0 ; }

typedef float QVECTOR[3];

#endif /* _MISC_H */

/*
 *   ORXONOX - the hottest 3D action shooter ever to exist
 *
 *
 *   License notice:
 *
 *   This program is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU General Public License
 *   as published by the Free Software Foundation; either version 2
 *   of the License, or (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 *   Author:
 *      Fabian 'x3n' Landau
 *   Co-authors:
 *      ...
 *
 *   Inspiration: MultiType by Benjamin Grauer
 */

#ifndef _MultiType_H__
#define _MultiType_H__

#include "UtilPrereqs.h"

enum _UtilExport MultiType
{
    MT_null,
    MT_void,
    MT_int,
    MT_uint,
    MT_char,
    MT_uchar,
    MT_short,
    MT_ushort,
    MT_long,
    MT_ulong,
    MT_float,
    MT_double,
    MT_longdouble,
    MT_bool,
    MT_constchar,
    MT_string,
    MT_xmlelement,
    MT_vector2,
    MT_vector3,
    MT_colourvalue,
    MT_quaternion,
    MT_degree,
    MT_radian
};

union _UtilExport MultiTypeValue
{
    void*           void_;
    int             int_;
    unsigned int    uint_;
    char            char_;
    unsigned char   uchar_;
    short           short_;
    unsigned short  ushort_;
    long            long_;
    unsigned long   ulong_;
    float           float_;
    double          double_;
    long double     longdouble_;
    bool            bool_;
};

#endif /* _MultiType_H__ */

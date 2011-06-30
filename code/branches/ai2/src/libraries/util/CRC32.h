/*
 *   ORXONOX - the hottest 3D action shooter ever to exist
 *                    > www.orxonox.net <
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
 *      ...
 *   Co-authors:
 *      ...
 *
 */

#ifndef _Util_CRC_H__
#define _Util_CRC_H__

#include "UtilPrereqs.h"

namespace orxonox
{
    const unsigned int UTIL_CRC32POLY = 0x04C11DB7; /* CRC-32 Polynom */

    _UtilExport void calcCRCBit(uint32_t &crc32, int bit);

    _UtilExport uint32_t calcCRC(unsigned char *data, unsigned int dataLength);
}

#endif /* _Util_CRC_H__ */

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

#include "CRC32.h"

namespace orxonox
{
    void calcCRCBit(uint32_t &crc32, int bit)
    {
        int hbit = (crc32 & 0x80000000) ? 1 : 0;
        if (hbit != bit)
            crc32 = (crc32 << 1) ^ UTIL_CRC32POLY;
        else
            crc32 = crc32 << 1;
      }

    uint32_t calcCRC(unsigned char *data, unsigned int dataLength)
    {
        uint32_t crc32 = 0;
        for(unsigned int i = 0; i < dataLength; i++)
        {
            calcCRCBit(crc32, (data[i] & 0x1) >> 0); // 1st bit
            calcCRCBit(crc32, (data[i] & 0x2) >> 1); // 2nd bit
            calcCRCBit(crc32, (data[i] & 0x3) >> 2); // 3rd bit
            calcCRCBit(crc32, (data[i] & 0x4) >> 3); // 4th bit
            calcCRCBit(crc32, (data[i] & 0x5) >> 4); // 5th bit
            calcCRCBit(crc32, (data[i] & 0x6) >> 5); // 6th bit
            calcCRCBit(crc32, (data[i] & 0x7) >> 6); // 7th bit
            calcCRCBit(crc32, (data[i] & 0x8) >> 7); // 8th bit
        }
        return crc32;
    }
}


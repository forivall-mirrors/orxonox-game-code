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
 *      Nicolas Perrenoud <nicolape_at_ee.ethz.ch>
 *   Co-authors:
 *      ...
 *
 */

#ifndef _AudioBuffer_H__
#define _AudioBuffer_H__

#include "AudioPrereqs.h"

#include <string>
#include <AL/al.h>

namespace audio
{
    class _AudioExport AudioBuffer
    {
    public:
        AudioBuffer(std::string fileName);
        ~AudioBuffer();

    private:
        // Buffers hold sound data.
        ALuint buffer;
        // Identifier
        std::string name;
        // True if AL was able to load data
        ALboolean loaded;

        ALenum format;
        ALsizei size;
        ALvoid* data;
        ALsizei freq;
        ALboolean loop;
    };
}

#endif /* _AudioBuffer_H__ */

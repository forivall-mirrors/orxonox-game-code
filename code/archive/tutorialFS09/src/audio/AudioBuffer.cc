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

#include "AudioBuffer.h"

namespace orxonox
{
    AudioBuffer::AudioBuffer(std::string filename)
    {
        // Load wav data into buffers.
        alGenBuffers(1, &buffer);

        if (alGetError() != AL_NO_ERROR)
            loaded=AL_FALSE;

        //FIXME deprecated; seems unneeded
//        alutLoadWAVFile((ALbyte*)filename.c_str(), &format, &data, &size, &freq, &loop);
        alBufferData(buffer, format, data, size, freq);
        //FIXME deprecated; seems unneeded
//        alutUnloadWAV(format, data, size, freq);

        // Do another error check and return.
        if (alGetError() != AL_NO_ERROR)
            loaded=AL_FALSE;
        else
              loaded=AL_TRUE;
    }

    AudioBuffer::~AudioBuffer()
    {

    }
}

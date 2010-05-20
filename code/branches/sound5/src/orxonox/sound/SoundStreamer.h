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
 *      Erwin 'vaiursch' Herrsche
 *   Co-authors:
 *
 */

#ifndef _SoundStreamer_H__
#define _SoundStreamer_H__

#include "sound/SoundPrereqs.h"

#include <string>
#include <vorbis/vorbisfile.h>
#include <OgreDataStream.h>
#include "core/CorePrereqs.h"

namespace orxonox
{
    class _OrxonoxExport SoundStreamer // class for stream threads
    {
    public:
        void operator()(ALuint audioSource, DataStreamPtr dataStream, OggVorbis_File* vf, int current_section);
    };
}

#endif /* _SoundStreamer_H__ */

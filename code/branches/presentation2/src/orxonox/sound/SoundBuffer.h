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
 *      Reto Grieder
 *   Co-authors:
 *      ...
 *
 */

#ifndef _SoundBuffer_H__
#define _SoundBuffer_H__

#include "OrxonoxPrereqs.h"
#include <boost/shared_ptr.hpp>

namespace orxonox
{
    /*
    @brief
        The SoundBuffer class is to be used for sounds with position and orientation.
    */
    class _OrxonoxExport SoundBuffer
    {
        friend class SoundManager;

    public:
        SoundBuffer(shared_ptr<ResourceInfo> fileInfo);
        ~SoundBuffer();

        inline ALuint getBuffer()
            { return this->audioBuffer_; }

        unsigned int getSize() const;

        shared_ptr<ResourceInfo> getFileInfo() const
            { return this->fileInfo_; }

        void setPooling(bool val)
            { this->bPooling_ = true; }
        bool getPooling() const
            { return this->bPooling_; }

    private:
        void loadStandard(DataStreamPtr dataStream);
        void loadOgg(DataStreamPtr dataStream);

        shared_ptr<ResourceInfo> fileInfo_;
        ALuint audioBuffer_;
        std::list<shared_ptr<SoundBuffer> >::iterator poolIterator_;
        bool bPooling_;
    };
}

#endif /* _SoundBuffer_H__ */

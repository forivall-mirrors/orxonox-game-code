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
        // Make sure nobody deletes an instance (using smart pointers)
        template <class T>
        friend void boost::checked_delete(T*);

    public:
        inline ALuint getBuffer()
            { return this->audioBuffer_; }

        unsigned int getSize() const;

        const std::string& getFilename() const
            { return this->filename_; }

    private:
        SoundBuffer(const std::string& filename, std::list<shared_ptr<SoundBuffer> >::iterator poolIterator);
        ~SoundBuffer();
        void loadStandard(const shared_ptr<ResourceInfo>& fileInfo, DataStreamPtr dataStream);
        void loadOgg(const shared_ptr<ResourceInfo>& fileInfo, DataStreamPtr dataStream);

        std::string filename_;
        ALuint audioBuffer_;
        std::list<shared_ptr<SoundBuffer> >::iterator poolIterator_;
    };
}

#endif /* _SoundBuffer_H__ */

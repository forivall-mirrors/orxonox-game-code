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
 *       Erwin 'vaiursch' Herrsche
 *   Co-authors:
 *      ...
 *
 */
#ifndef _SOUNDBASE_H__
#define _SOUNDBASE_H__

#include <AL/al.h>
#include <AL/alut.h>
#include <string>

namespace orxonox
{
    class SoundManager;
    class WorldEntity;
    /**
     * The SoudBase class is the base class for all sound file loader classes.
     * It server as main interface to the OpenAL library.
     *
     */
    class SoundBase
    {
    public:
        SoundBase();
        SoundBase(WorldEntity* entity);

        void attachToEntity(WorldEntity* entity);
        void update();
        void play(bool loop);
        void stop();
        void pause();

        bool isPlaying();
        bool isPaused();
        bool isStopped();

        bool loadFile(std::string filename);

    private:
        ALuint source_;
        ALuint buffer_;
        WorldEntity* entity_;

        ALint getSourceState();
    }; // class SoundBase
} // namepsace orxonox

#endif // _SOUNDBASE_H__

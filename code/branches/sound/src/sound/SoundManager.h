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
 */
#ifndef _SOUNDMANGER_H__
#define _SOUNDMANGER_H__

#include <AL/al.h>
#include <AL/alc.h>

#include <orxonox/objects/Tickable.h>

namespace orxonox
{
    class SoundBase;

    /**
     * The SoundManager class manages the OpenAL device, context and listener
     * position. It has a list of all SoundBase objects and calls their update
     * function every tick. It is a singleton.
     *
     */
    class SoundManager : public Tickable 
    {
    public:
        SoundManager();
        void addSound(SoundBase* sound);
        void removeSound(SoundBase* sound);

        virtual void tick(float dt);

    private:
        std::list<SoundBase*> soundlist_;

    }; // class SoundManager
} // namespace orxonox

#endif // _SOUNDMANAGER_H__

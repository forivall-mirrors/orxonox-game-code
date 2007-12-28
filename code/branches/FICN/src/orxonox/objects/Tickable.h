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
 */

/*!
    @file Tickable.h
    @brief Definition of the Tickable interface.

    The Tickable interface provides a tick(dt) function, that gets called every frame.
    float dt is the time since the last frame in seconds.

    Attention:
    Classes derived from a Tickable that want to have a tick(dt) function on their part, MUST call the
    parent::tick(dt) function explicit in their implementation of tick(dt) because it's a virtual function.
*/

#ifndef _Tickable_H__
#define _Tickable_H__

#include <OgreFrameListener.h>

#include "../OrxonoxPrereqs.h"

#include "../core/CoreIncludes.h"

namespace orxonox
{
    class TickFrameListener; // Forward declaration

    //! The Tickable interface provides a tick(dt) function, that gets called every frame.
    class Tickable : virtual public OrxonoxClass
    {
        public:
            /**
                @brief Gets called every frame.
                @param dt The time since the last frame in seconds
            */
            virtual void tick(float dt) = 0;

        protected:
            /**
                @brief Constructor: Registers the object in the Tickable-list
            */
            Tickable() { RegisterRootObject(Tickable); }
    };

    //! The TickFrameListener calls the tick(dt) function of all Tickables every frame.
    class TickFrameListener : public Ogre::FrameListener
    {
        private:
            /** @brief Gets called before a frame gets rendered. */
            bool frameStarted(const Ogre::FrameEvent &evt)
            {
                // Iterate through all Tickables and call their tick(dt) function
                for (Iterator<Tickable> it = ObjectList<Tickable>::start(); it; )
                    (it++)->tick(evt.timeSinceLastFrame);

                return FrameListener::frameStarted(evt);
            }
    };
}

#endif /* _Tickable_H__ */

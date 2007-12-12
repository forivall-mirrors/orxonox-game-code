/*!
    @file Tickable.h
    @brief Definition of the Tickable interface.

    The Tickable interface provides a tick(dt) function, that gets called every frame.
    float dt is the time since the last frame.

    Attention:
    Classes derived from a Tickable that want to have a tick(dt) function on their part, MUST call the
    parent::tick(dt) function explicit in their implementation of tick(dt) because it's a virtual function.
*/

#ifndef _Tickable_H__
#define _Tickable_H__

#include "../core/CoreIncludes.h"
#include "OgreFrameListener.h"

namespace orxonox
{
    class TickFrameListener; // Forward declaration

    //! The Tickable interface provides a tick(dt) function, that gets called every frame.
    class Tickable : virtual public OrxonoxClass
    {
        public:
            /**
                @brief Gets called every frame.
                @param dt The time since the last frame
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
                for (Iterator<Tickable> it = ObjectList<Tickable>::start(); it; ++it)
                    it->tick(evt.timeSinceLastFrame);

                return FrameListener::frameStarted(evt);
            }
    };
}

#endif

#ifndef _Tickable_H__
#define _Tickable_H__

#include <OgreFrameListener.h>
#include "../core/IdentifierIncludes.h"

namespace orxonox
{
    class TickFrameListener;

    class Tickable : virtual public OrxonoxClass
    {
        public:
            virtual void tick(float dt) = 0;

        protected:
            Tickable() { RegisterRootObject(Tickable); }
    };

    class TickFrameListener : public Ogre::FrameListener
    {
        private:
            bool frameStarted(const Ogre::FrameEvent &evt)
            {
                for (Iterator<Tickable> it = ObjectList<Tickable>::start(); it; ++it)
                    it->tick(evt.timeSinceLastFrame);

                return FrameListener::frameStarted(evt);
            }
    };
}

#endif

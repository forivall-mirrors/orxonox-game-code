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

#include "OrxonoxStableHeaders.h"
#include "GSIO.h"

#include <OgreFrameListener.h>
#include <OgreRoot.h>
#include <OgreTimer.h>

#include "core/ConsoleCommand.h"
#include "core/TclThreadManager.h"
#include "GraphicsEngine.h"

namespace orxonox
{
    GSIO::GSIO()
        : GameState("io")
        //, timer_(0)
    {
    }

    GSIO::~GSIO()
    {
    }

    void GSIO::enter()
    {
    }

    void GSIO::leave()
    {
    }

    void GSIO::ticked(float dt, uint64_t time)
    {
        this->tickChild(dt, time);
        //Ogre::Root& ogreRoot = Ogre::Root::getSingleton();

        //unsigned long frameCount = 0;

        //const unsigned long refreshTime = (unsigned long)(debugRefreshTime_ * 1000000.0f);
        //unsigned long refreshStartTime = 0;
        //unsigned long tickTime = 0;
        //unsigned long oldFrameCount = 0;

        //unsigned long timeBeforeTick = 0;
        //unsigned long timeBeforeTickOld = 0;
        //unsigned long timeAfterTick = 0;

        //// TODO: Update time in seconds every 7 seconds to avoid any overflow (7 secs is very tight)

        //COUT(3) << "Orxonox: Starting the main loop." << std::endl;

        //try
        //{
        //    timer_->reset();
        //    while (!this->hasScheduledTransition())
        //    {
        //        // get current time
        //        timeBeforeTickOld = timeBeforeTick;
        //        timeBeforeTick    = timer_->getMicroseconds();
        //        float dt = (timeBeforeTick - timeBeforeTickOld) / 1000000.0;

        //        TclThreadManager::getInstance().tick(dt);

        //        this->tickChild(dt);

        //        // get current time once again
        //        timeAfterTick = timer_->getMicroseconds();

        //        tickTime += timeAfterTick - timeBeforeTick;
        //        if (timeAfterTick > refreshStartTime + refreshTime)
        //        {
        //            GraphicsEngine::getInstance().setAverageTickTime(
        //                (float)tickTime * 0.001 / (frameCount - oldFrameCount));
        //            float avgFPS = (float)(frameCount - oldFrameCount) / (timeAfterTick - refreshStartTime) * 1000000.0;
        //            GraphicsEngine::getInstance().setAverageFramesPerSecond(avgFPS);

        //            oldFrameCount = frameCount;
        //            tickTime = 0;
        //            refreshStartTime = timeAfterTick;
        //        }

        //        // don't forget to call _fireFrameStarted in ogre to make sure
        //        // everything goes smoothly
        //        Ogre::FrameEvent evt;
        //        evt.timeSinceLastFrame = dt;
        //        evt.timeSinceLastEvent = dt; // note: same time, but shouldn't matter anyway
        //        ogreRoot._fireFrameStarted(evt);

        //        // again, just to be sure ogre works fine
        //        ogreRoot._fireFrameEnded(evt); // note: uses the same time as _fireFrameStarted

        //        ++frameCount;
        //    }
        //}
        //catch (std::exception& ex)
        //{
        //    // something went wrong.
        //    COUT(1) << ex.what() << std::endl;
        //    COUT(1) << "Main loop was stopped by an unhandled exception. Shutting down." << std::endl;
        //}
    }
}

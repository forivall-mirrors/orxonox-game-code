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
#include "GSGraphics.h"

#include <OgreFrameListener.h>
#include <OgreRoot.h>
#include <OgreWindowEventUtilities.h>
#include <OgreRenderWindow.h>

#include "core/ConsoleCommand.h"
#include "core/ConfigValueIncludes.h"
#include "core/input/InputManager.h"
#include "overlays/console/InGameConsole.h"
#include "gui/GUIManager.h"
#include "GraphicsEngine.h"

namespace orxonox
{
    GSGraphics::GSGraphics()
        : GameState("graphics")
        , ogreRoot_(0)
        , graphicsEngine_(0)
        , inputManager_(0)
        , console_(0)
        , guiManager_(0)
        , frameCount_(0)
        , statisticsRefreshCycle_(0)
        , statisticsStartTime_(0)
        , statisticsStartCount_(0)
        , tickTime_(0)
    {
    }

    GSGraphics::~GSGraphics()
    {
    }

    void GSGraphics::setConfigValues()
    {
        SetConfigValue(statisticsRefreshCycle_, 200000).description("Sets the time in microseconds interval at which average fps, etc. get updated.");
    }

    void GSGraphics::enter()
    {
        setConfigValues();

        this->ogreRoot_ = Ogre::Root::getSingletonPtr();
        this->graphicsEngine_ = GraphicsEngine::getInstancePtr();

        graphicsEngine_->loadRenderer();    // creates the render window

        // TODO: Spread this so that this call only initialises things needed for the Console and GUI
        graphicsEngine_->initialiseResources();

        // Calls the InputManager which sets up the input devices.
        // The render window width and height are used to set up the mouse movement.
        inputManager_ = new InputManager();
        inputManager_->initialise(graphicsEngine_->getWindowHandle(),
            graphicsEngine_->getWindowWidth(), graphicsEngine_->getWindowHeight(), true);

        // Load the InGameConsole
        console_ = new InGameConsole();
        console_->initialise();

        // load the CEGUI interface
        guiManager_ = new GUIManager();
        guiManager_->initialise();

        // reset frame counter
        this->frameCount_ = 0;
        this->tickTime_ = 0;
        statisticsStartTime_ = 0;
        statisticsStartCount_ = 0;
    }

    void GSGraphics::leave()
    {
        delete this->guiManager_;

        delete this->console_;

        delete this->inputManager_;

        this->ogreRoot_->detachRenderTarget(GraphicsEngine::getInstance().getRenderWindow());
        delete GraphicsEngine::getInstance().getRenderWindow();
        //this->ogreRoot_->shutdown
        // TODO: destroy render window
    }

    /**
        Main loop of the orxonox game.
        We use the Ogre::Timer to measure time since it uses the most precise
        method an a platform (however the windows timer lacks time when under
        heavy kernel load!).
        There is a simple mechanism to measure the average time spent in our
        ticks as it may indicate performance issues.
        A note about the Ogre::FrameListener: Even though we don't use them,
        they still get called. However, the delta times are not correct (except
        for timeSinceLastFrame, which is the most important). A little research
        as shown that there is probably only one FrameListener that doesn't even
        need the time. So we shouldn't run into problems.
    */
    void GSGraphics::ticked(const Clock& time)
    {
        uint64_t timeBeforeTick = time.getRealMicroseconds();
        float dt = time.getDeltaTime();

        this->inputManager_->tick(dt);
        // tick console
        this->console_->tick(dt);
        this->tickChild(time);
        
        uint64_t timeAfterTick = time.getRealMicroseconds();

        tickTime_ += (unsigned int)(timeAfterTick - timeBeforeTick);
        if (timeAfterTick > statisticsStartTime_ + statisticsRefreshCycle_)
        {
            GraphicsEngine::getInstance().setAverageTickTime(
                (float)tickTime_ * 0.001f / (frameCount_ - statisticsStartCount_));
            float avgFPS = (float)(frameCount_ - statisticsStartCount_)
                / (timeAfterTick - statisticsStartTime_) * 1000000.0;
            GraphicsEngine::getInstance().setAverageFramesPerSecond(avgFPS);

            tickTime_ = 0;
            statisticsStartCount_ = frameCount_;
            statisticsStartTime_  = timeAfterTick;
        }

        // don't forget to call _fireFrameStarted in ogre to make sure
        // everything goes smoothly
        Ogre::FrameEvent evt;
        evt.timeSinceLastFrame = dt;
        evt.timeSinceLastEvent = dt; // note: same time, but shouldn't matter anyway
        ogreRoot_->_fireFrameStarted(evt);

        // Pump messages in all registered RenderWindows
        // This calls the WindowEventListener objects.
        Ogre::WindowEventUtilities::messagePump();
        // make sure the window stays active even when not focused
        // (probably only necessary on windows)
        GraphicsEngine::getInstance().setWindowActivity(true);

        // render
        ogreRoot_->_updateAllRenderTargets();

        // again, just to be sure ogre works fine
        ogreRoot_->_fireFrameEnded(evt); // note: uses the same time as _fireFrameStarted

        ++frameCount_;
    }
}

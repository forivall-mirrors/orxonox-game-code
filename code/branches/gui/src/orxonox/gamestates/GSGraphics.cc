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
#include <OgreTimer.h>
#include <OgreWindowEventUtilities.h>

#include "core/ConsoleCommand.h"
#include "core/ConfigValueIncludes.h"
#include "core/input/InputManager.h"
#include "core/Core.h"
#include "overlays/console/InGameConsole.h"
#include "gui/GUIManager.h"
#include "GraphicsEngine.h"

namespace orxonox
{
    GSGraphics::GSGraphics()
        : GameState("graphics")
        , timer_(0)
        , bAbort_(false)
        , debugRefreshTime_(0.0f)
        , inputManager_(0)
        , console_(0)
        , guiManager_(0)
    {
    }

    GSGraphics::~GSGraphics()
    {
    }

    void GSGraphics::setConfigValues()
    {
        SetConfigValue(debugRefreshTime_, 0.2).description("Sets the time interval at which average fps, etc. get updated.");
    }

    void GSGraphics::enter()
    {
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

        // use the ogre timer class to measure time.
        timer_ = new Ogre::Timer();

        // add console commands
        FunctorMember<GSGraphics>* functor = createFunctor(&GSGraphics::exitGame);
        functor->setObject(this);
        CommandExecutor::addConsoleCommandShortcut(createConsoleCommand(functor, "exit"));
    }

    void GSGraphics::leave()
    {
        delete this->timer_;

        delete this->guiManager_;

        delete this->console_;

        delete this->inputManager_;

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
    bool GSGraphics::tick(float dt)
    {
        // note: paramter 'dt' is of no meaning

        Ogre::Root& ogreRoot = Ogre::Root::getSingleton();

        unsigned long frameCount = 0;

        const unsigned long refreshTime = (unsigned long)(debugRefreshTime_ * 1000000.0f);
        unsigned long refreshStartTime = 0;
        unsigned long tickTime = 0;
        unsigned long oldFrameCount = 0;

        unsigned long timeBeforeTick = 0;
        unsigned long timeBeforeTickOld = 0;
        unsigned long timeAfterTick = 0;

        // TODO: Update time in seconds every 7 seconds to avoid any overflow (7 secs is very tight)

        COUT(3) << "Orxonox: Starting the main loop." << std::endl;

        try
        {
            timer_->reset();
            while (!bAbort_)
            {
                // get current time
                timeBeforeTickOld = timeBeforeTick;
                timeBeforeTick    = timer_->getMicroseconds();
                float dt = (timeBeforeTick - timeBeforeTickOld) / 1000000.0;


                // tick the core (needs real time for input and tcl thread management)
                // TODO: ticks of InputManager and tcl thread manager have to be separated.
                Core::tick(dt);

                // tick child state
                if (this->getActiveChild())
                    this->getActiveChild()->tick(dt);

                // tick console
                this->console_->tick(dt);

                // get current time once again
                timeAfterTick = timer_->getMicroseconds();

                tickTime += timeAfterTick - timeBeforeTick;
                if (timeAfterTick > refreshStartTime + refreshTime)
                {
                    GraphicsEngine::getInstance().setAverageTickTime(
                        (float)tickTime * 0.001 / (frameCount - oldFrameCount));
                    float avgFPS = (float)(frameCount - oldFrameCount) / (timeAfterTick - refreshStartTime) * 1000000.0;
                    GraphicsEngine::getInstance().setAverageFramesPerSecond(avgFPS);

                    oldFrameCount = frameCount;
                    tickTime = 0;
                    refreshStartTime = timeAfterTick;
                }

                // don't forget to call _fireFrameStarted in ogre to make sure
                // everything goes smoothly
                Ogre::FrameEvent evt;
                evt.timeSinceLastFrame = dt;
                evt.timeSinceLastEvent = dt; // note: same time, but shouldn't matter anyway
                ogreRoot._fireFrameStarted(evt);

                // Pump messages in all registered RenderWindows
                // This calls the WindowEventListener objects.
                Ogre::WindowEventUtilities::messagePump();
                // make sure the window stays active even when not focused
                // (probably only necessary on windows)
                GraphicsEngine::getInstance().setWindowActivity(true);

                // render
                ogreRoot._updateAllRenderTargets();

                // again, just to be sure ogre works fine
                ogreRoot._fireFrameEnded(evt); // note: uses the same time as _fireFrameStarted

                ++frameCount;
            }
        }
        catch (std::exception& ex)
        {
            // something went wrong.
            COUT(1) << ex.what() << std::endl;
            COUT(1) << "Main loop was stopped by an unhandled exception. Shutting down." << std::endl;
        }        return true;
    }
}

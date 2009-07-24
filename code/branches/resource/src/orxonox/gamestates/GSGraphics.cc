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
 *      Benjamin Knecht
 *
 */

/**
@file
@brief
    Implementation of Graphics GameState class.
 */

#include "GSGraphics.h"

#include <boost/filesystem.hpp>
#include <OgreRenderWindow.h>

#include "util/Convert.h"
#include "core/Clock.h"
#include "core/CommandExecutor.h"
#include "core/ConsoleCommand.h"
#include "core/Core.h"
#include "core/Game.h"
#include "core/GameMode.h"
#include "core/GraphicsManager.h"
#include "core/GUIManager.h"
#include "core/input/InputManager.h"
#include "core/input/KeyBinder.h"
#include "core/input/InputState.h"
#include "core/Loader.h"
#include "core/XMLFile.h"
#include "overlays/console/InGameConsole.h"
#include "sound/SoundManager.h"

// HACK:
#include "overlays/map/Map.h"

namespace orxonox
{
    DeclareGameState(GSGraphics, "graphics", true, true);

    GSGraphics::GSGraphics(const GameStateConstrParams& params)
        : GameState(params)
        , inputManager_(0)
        , console_(0)
        , guiManager_(0)
        , graphicsManager_(0)
        , soundManager_(0)
        , masterKeyBinder_(0)
        , masterInputState_(0)
        , debugOverlay_(0)
    {
    }

    GSGraphics::~GSGraphics()
    {
    }

    /**
    @brief
        This function is called when we enter this game state.

        Since graphics is very important for our game this function does quite a lot:
        \li starts graphics manager
        \li loads debug overlay
        \li manages render window
        \li creates input manager
        \li loads master key bindings
        \li loads the SoundManager
        \li loads ingame console
        \li loads GUI interface (GUIManager)
        \li creates console command to toggle GUI
    */
    void GSGraphics::activate()
    {
        // Load OGRE, CEGUI and OIS
        Core::getInstance().loadGraphics();

        // Load OGRE including the render window
        this->graphicsManager_ = new GraphicsManager();

        // load debug overlay
        COUT(3) << "Loading Debug Overlay..." << std::endl;
        this->debugOverlay_ = new XMLFile((Core::getMediaPath() / "overlay" / "debug.oxo").string());
        Loader::open(debugOverlay_);

        // The render window width and height are used to set up the mouse movement.
        size_t windowHnd = 0;
        Ogre::RenderWindow* renderWindow = GraphicsManager::getInstance().getRenderWindow();
        renderWindow->getCustomAttribute("WINDOW", &windowHnd);

        // Calls the InputManager which sets up the input devices.
        inputManager_ = new InputManager(windowHnd);

        // load master key bindings
        masterInputState_ = InputManager::getInstance().createInputState("master", true);
        masterKeyBinder_ = new KeyBinder();
        masterKeyBinder_->loadBindings("masterKeybindings.ini");
        masterInputState_->setKeyHandler(masterKeyBinder_);

        // Load the SoundManager
        soundManager_ = new SoundManager();

        // Load the InGameConsole
        console_ = new InGameConsole();
        console_->initialise();

        // load the CEGUI interface
        guiManager_ = new GUIManager(renderWindow);

        // add console command to toggle GUI
        FunctorMember<GSGraphics>* functor = createFunctor(&GSGraphics::toggleGUI);
        functor->setObject(this);
        this->ccToggleGUI_ = createConsoleCommand(functor, "toggleGUI");
        CommandExecutor::addConsoleCommandShortcut(this->ccToggleGUI_);

        // enable master input
        InputManager::getInstance().enterState("master");
    }

    /**
    @brief
        This function is called when the game state is left

        Created references, input states and console commands are deleted.
    */
    void GSGraphics::deactivate()
    {
/*
        if (this->ccToggleGUI_)
        {
            delete this->ccToggleGUI_;
            this->ccToggleGUI_ = 0;
        }
*/

        masterInputState_->setHandler(0);
        InputManager::getInstance().destroyState("master");
        delete this->masterKeyBinder_;

        delete this->guiManager_;
        delete this->console_;

        Loader::unload(this->debugOverlay_);
        delete this->debugOverlay_;

        delete this->soundManager_;

        delete this->inputManager_;
        this->inputManager_ = 0;

        // HACK:
        Map::hackDestroyMap();

        delete graphicsManager_;

        // Unload OGRE, CEGUI and OIS
        Core::getInstance().loadGraphics();
    }

    /**
    @brief
        Toggles the visibility of the current GUI

        This function just executes a Lua function in the main script of the GUI by accessing the GUIManager.
        For more details on this function check out the Lua code.
    */
    void GSGraphics::toggleGUI()
    {
        GUIManager::getInstance().executeCode("toggleGUI()");
    }

    /**
    @note
        A note about the Ogre::FrameListener: Even though we don't use them,
        they still get called. However, the delta times are not correct (except
        for timeSinceLastFrame, which is the most important). A little research
        as shown that there is probably only one FrameListener that doesn't even
        need the time. So we shouldn't run into problems.
    */
    void GSGraphics::update(const Clock& time)
    {
        if (this->getActivity().topState)
        {
            // This state can not 'survive' on its own.
            // Load a user interface therefore
            Game::getInstance().requestState("mainMenu");
        }

        uint64_t timeBeforeTick = time.getRealMicroseconds();

        this->inputManager_->update(time);
        this->console_->update(time);

        uint64_t timeAfterTick = time.getRealMicroseconds();

        // Also add our tick time
        Game::getInstance().addTickTime(timeAfterTick - timeBeforeTick);

        // Process gui events
        this->guiManager_->update(time);
        // Render
        this->graphicsManager_->update(time);
    }
}

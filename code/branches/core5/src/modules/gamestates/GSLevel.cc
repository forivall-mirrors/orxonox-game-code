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
 *      Fabian 'x3n' Landau
 *      Benjamin Knecht
 *
 */

#include "GSLevel.h"

#include <OgreCompositorManager.h>

#include "core/input/InputManager.h"
#include "core/input/InputState.h"
#include "core/input/KeyBinder.h"
#include "core/Clock.h"
#include "core/ConsoleCommand.h"
#include "core/ConfigValueIncludes.h"
#include "core/CoreIncludes.h"
#include "core/Game.h"
#include "core/GameMode.h"
#include "core/Core.h"
#include "core/GraphicsManager.h"
#include "core/GUIManager.h"
#include "core/Loader.h"
#include "core/XMLFile.h"

#include "tools/interfaces/Tickable.h"
#include "CameraManager.h"
#include "LevelManager.h"
#include "PlayerManager.h"
#include "infos/HumanPlayer.h"

namespace orxonox
{
    DeclareGameState(GSLevel, "level", false, false);
    SetConsoleCommand(GSLevel, showIngameGUI, true);

    XMLFile* GSLevel::startFile_s = NULL;

    GSLevel::GSLevel(const GameStateInfo& info)
        : GameState(info)
        , keyBinder_(0)
        , gameInputState_(0)
        , guiMouseOnlyInputState_(0)
        , guiKeysOnlyInputState_(0)
        , cameraManager_(0)
    {
        RegisterObject(GSLevel);

        this->ccKeybind_ = 0;
        this->ccTkeybind_ = 0;
    }

    GSLevel::~GSLevel()
    {
    }

    void GSLevel::setConfigValues()
    {
        SetConfigValue(keyDetectorCallbackCode_, "KeybindBindingStringKeyName=");
    }

    void GSLevel::activate()
    {
        setConfigValues();

        if (GameMode::showsGraphics())
        {
            gameInputState_ = InputManager::getInstance().createInputState("game");
            keyBinder_ = new KeyBinder();
            keyBinder_->loadBindings("keybindings.ini");
            gameInputState_->setHandler(keyBinder_);

            guiMouseOnlyInputState_ = InputManager::getInstance().createInputState("guiMouseOnly");
            guiMouseOnlyInputState_->setMouseHandler(GUIManager::getInstancePtr());

            guiKeysOnlyInputState_ = InputManager::getInstance().createInputState("guiKeysOnly");
            guiKeysOnlyInputState_->setKeyHandler(GUIManager::getInstancePtr());

            // create the global CameraManager
            this->cameraManager_ = new CameraManager(GraphicsManager::getInstance().getViewport());
        }

        this->playerManager_ = new PlayerManager();

        this->scope_GSLevel_ = new Scope<ScopeID::GSLevel>();

        if (GameMode::isMaster())
        {
            this->loadLevel();
        }

        if (GameMode::showsGraphics())
        {
            // keybind console command
            ccKeybind_ = createConsoleCommand(createFunctor(&GSLevel::keybind, this), "keybind");
            CommandExecutor::addConsoleCommandShortcut(ccKeybind_);
            ccTkeybind_ = createConsoleCommand(createFunctor(&GSLevel::tkeybind, this), "tkeybind");
            CommandExecutor::addConsoleCommandShortcut(ccTkeybind_);
            // set our console command as callback for the key detector
            InputManager::getInstance().setKeyDetectorCallback(std::string("keybind ") + keyDetectorCallbackCode_);

            // level is loaded: we can start capturing the input
            InputManager::getInstance().enterState("game");
            
            // connect the HumanPlayer to the game
            this->playerManager_->clientConnected(0);
        }
    }

    void GSLevel::showIngameGUI(bool show)
    {
        if (show)
        {
            GUIManager::getInstance().showGUI("inGameTest");
            GUIManager::getInstance().executeCode("showCursor()");
            InputManager::getInstance().enterState("guiMouseOnly");
        }
        else
        {
            GUIManager::getInstance().executeCode("hideGUI(\"inGameTest\")");
            GUIManager::getInstance().executeCode("hideCursor()");
            InputManager::getInstance().leaveState("guiMouseOnly");
        }
    }

    void GSLevel::deactivate()
    {
/*
        // destroy console commands
        if (this->ccKeybind_)
        {
            delete this->ccKeybind_;
            this->ccKeybind_ = 0;
        }
        if (this->ccTkeybind_)
        {
            delete this->ccTkeybind_;
            this->ccTkeybind_ = 0;
        }
*/

        if (GameMode::showsGraphics())
        {
            // disconnect the HumanPlayer
            this->playerManager_->clientDisconnected(0);
            
            // unload all compositors (this is only necessary because we don't yet destroy all resources!)
            Ogre::CompositorManager::getSingleton().removeAll();
        }

        // this call will delete every BaseObject!
        // But currently this will call methods of objects that exist no more
        // The only 'memory leak' is the ParticleSpawer. They would be deleted here
        // and call a sceneNode method that has already been destroy by the corresponding space ship.
        //Loader::close();

        if (GameMode::showsGraphics())
        {
            InputManager::getInstance().leaveState("game");
        }

        if (GameMode::isMaster())
            this->unloadLevel();

        if (this->cameraManager_)
        {
            delete this->cameraManager_;
            this->cameraManager_ = 0;
        }

        if (this->playerManager_)
        {
            this->playerManager_->destroy();
            this->playerManager_ = 0;
        }

        if (this->scope_GSLevel_)
        {
            delete this->scope_GSLevel_;
            this->scope_GSLevel_ = NULL;
        }

        if (GameMode::showsGraphics())
        {
            gameInputState_->setHandler(0);
            guiMouseOnlyInputState_->setHandler(0);
            guiKeysOnlyInputState_->setHandler(0);
            InputManager::getInstance().destroyState("game");
            InputManager::getInstance().destroyState("guiKeysOnly");
            InputManager::getInstance().destroyState("guiMouseOnly");
            if (this->keyBinder_)
            {
                this->keyBinder_->destroy();
                this->keyBinder_ = 0;
            }
        }
    }

    void GSLevel::update(const Clock& time)
    {
        // Note: Temporarily moved to GSGraphics.
        //// Call the scene objects
        //for (ObjectList<Tickable>::iterator it = ObjectList<Tickable>::begin(); it; ++it)
        //    it->tick(time.getDeltaTime() * this->timeFactor_);
    }

    void GSLevel::loadLevel()
    {
        // call the loader
        COUT(0) << "Loading level..." << std::endl;
        startFile_s = new XMLFile(LevelManager::getInstance().getDefaultLevel());
        Loader::open(startFile_s);
    }

    void GSLevel::unloadLevel()
    {
        Loader::unload(startFile_s);

        delete startFile_s;
    }

    void GSLevel::keybind(const std::string &command)
    {
        this->keybindInternal(command, false);
    }

    void GSLevel::tkeybind(const std::string &command)
    {
        this->keybindInternal(command, true);
    }

    /**
    @brief
        Assigns a command string to a key/button/axis. The name is determined via KeyDetector.
    @param command
        Command string that can be executed by the CommandExecutor
        OR: Internal string "KeybindBindingStringKeyName=" used for the second call to identify
        the key/button/axis that has been activated. This is configured above in activate().
    */
    void GSLevel::keybindInternal(const std::string& command, bool bTemporary)
    {
        if (GameMode::showsGraphics())
        {
            static std::string bindingString = "";
            static bool bTemporarySaved = false;
            static bool bound = true;
            // note: We use a long name to make 'sure' that the user doesn't use it accidentally.
            // Howerver there will be no real issue if it happens anyway.
            if (command.find(keyDetectorCallbackCode_) != 0)
            {
                if (bound)
                {
                    COUT(0) << "Press any button/key or move a mouse/joystick axis" << std::endl;
                    InputManager::getInstance().enterState("detector");
                    bindingString = command;
                    bTemporarySaved = bTemporary;
                    bound = false;
                }
                //else:  We're still in a keybind command. ignore this call.
            }
            else
            {
                if (!bound)
                {
                    // user has pressed the key
                    std::string name = command.substr(this->keyDetectorCallbackCode_.size());
                    COUT(0) << "Binding string \"" << bindingString << "\" on key '" << name << "'" << std::endl;
                    this->keyBinder_->setBinding(bindingString, name, bTemporarySaved);
                    InputManager::getInstance().leaveState("detector");
                    bound = true;
                }
                // else: A key was pressed within the same tick, ignore it.
            }
        }
    }
}

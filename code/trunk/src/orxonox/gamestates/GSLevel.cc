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
 *
 */

#include "OrxonoxStableHeaders.h"
#include "GSLevel.h"

#include "core/input/InputManager.h"
#include "core/input/SimpleInputState.h"
#include "core/input/KeyBinder.h"
#include "core/Loader.h"
#include "core/XMLFile.h"
#include "core/CommandExecutor.h"
#include "core/ConsoleCommand.h"
#include "core/CommandLine.h"
#include "core/ConfigValueIncludes.h"
#include "core/CoreIncludes.h"
#include "core/Core.h"
#include "objects/Tickable.h"
#include "objects/Radar.h"
#include "CameraManager.h"
#include "LevelManager.h"
#include "PlayerManager.h"

namespace orxonox
{
    SetCommandLineArgument(level, "presentation.oxw").shortcut("l");

    GSLevel::GSLevel()
//        : GameState<GSGraphics>(name)
        : keyBinder_(0)
        , inputState_(0)
        , radar_(0)
        , startFile_(0)
        , cameraManager_(0)
        , levelManager_(0)
    {
        RegisterObject(GSLevel);

        this->ccKeybind_ = 0;
        this->ccTkeybind_ = 0;

        setConfigValues();
    }

    GSLevel::~GSLevel()
    {
    }

    void GSLevel::setConfigValues()
    {
        SetConfigValue(keyDetectorCallbackCode_, "KeybindBindingStringKeyName=");
    }

    void GSLevel::enter(Ogre::Viewport* viewport)
    {
        if (Core::showsGraphics())
        {
            inputState_ = InputManager::getInstance().createInputState<SimpleInputState>("game", 20);
            keyBinder_ = new KeyBinder();
            keyBinder_->loadBindings("keybindings.ini");
            inputState_->setHandler(keyBinder_);

            // create the global CameraManager
            assert(viewport);
            this->cameraManager_ = new CameraManager(viewport);

            // Start the Radar
            this->radar_ = new Radar();
        }

        this->playerManager_ = new PlayerManager();

        if (Core::isMaster())
        {
            // create the global LevelManager
            this->levelManager_ = new LevelManager();

            this->loadLevel();
        }

        if (Core::showsGraphics())
        {
            // TODO: insert slomo console command with
            // .accessLevel(AccessLevel::Offline).defaultValue(0, 1.0).axisParamIndex(0).isAxisRelative(false);

            // keybind console command
            FunctorMember<GSLevel>* functor1 = createFunctor(&GSLevel::keybind);
            functor1->setObject(this);
            ccKeybind_ = createConsoleCommand(functor1, "keybind");
            CommandExecutor::addConsoleCommandShortcut(ccKeybind_);
            FunctorMember<GSLevel>* functor2 = createFunctor(&GSLevel::tkeybind);
            functor2->setObject(this);
            ccTkeybind_ = createConsoleCommand(functor2, "tkeybind");
            CommandExecutor::addConsoleCommandShortcut(ccTkeybind_);
            // set our console command as callback for the key detector
            InputManager::getInstance().setKeyDetectorCallback(std::string("keybind ") + keyDetectorCallbackCode_);

            // level is loaded: we can start capturing the input
            InputManager::getInstance().requestEnterState("game");
        }
    }

    void GSLevel::leave()
    {
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

        // this call will delete every BaseObject!
        // But currently this will call methods of objects that exist no more
        // The only 'memory leak' is the ParticleSpawer. They would be deleted here
        // and call a sceneNode method that has already been destroy by the corresponding space ship.
        //Loader::close();

        if (Core::showsGraphics())
            InputManager::getInstance().requestLeaveState("game");

        if (Core::isMaster())
            this->unloadLevel();

        if (this->radar_)
        {
            delete this->radar_;
            this->radar_ = 0;
        }

        if (this->cameraManager_)
        {
            delete this->cameraManager_;
            this->cameraManager_ = 0;
        }

        if (this->levelManager_)
        {
            delete this->levelManager_;
            this->levelManager_ = 0;
        }

        if (this->playerManager_)
        {
            delete this->playerManager_;
            this->playerManager_ = 0;
        }

        if (Core::showsGraphics())
        {
            inputState_->setHandler(0);
            InputManager::getInstance().requestDestroyState("game");
            if (this->keyBinder_)
            {
                delete this->keyBinder_;
                this->keyBinder_ = 0;
            }
        }
    }

    void GSLevel::ticked(const Clock& time)
    {
        // Commented by 1337: Temporarily moved to GSGraphics.
        //// Call the scene objects
        //for (ObjectList<Tickable>::iterator it = ObjectList<Tickable>::begin(); it; ++it)
        //    it->tick(time.getDeltaTime() * this->timeFactor_);
    }

    void GSLevel::loadLevel()
    {
        // call the loader
        COUT(0) << "Loading level..." << std::endl;
        std::string levelName;
        CommandLine::getValue("level", &levelName);
        startFile_ = new XMLFile(Core::getMediaPathString() + "levels" + CP_SLASH + levelName);
        Loader::open(startFile_);
    }

    void GSLevel::unloadLevel()
    {
        //////////////////////////////////////////////////////////////////////////////////////////
        // TODO // TODO // TODO // TODO // TODO // TODO // TODO // TODO // TODO // TODO // TODO //
        //////////////////////////////////////////////////////////////////////////////////////////
        // Loader::unload(startFile_); // TODO: REACTIVATE THIS IF LOADER::UNLOAD WORKS PROPERLY /
        //////////////////////////////////////////////////////////////////////////////////////////

        delete this->startFile_;
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
        the key/button/axis that has been activated. This is configured above in enter().
    */
    void GSLevel::keybindInternal(const std::string& command, bool bTemporary)
    {
        if (Core::showsGraphics())
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
                    InputManager::getInstance().requestEnterState("detector");
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
                    InputManager::getInstance().requestLeaveState("detector");
                    bound = true;
                }
                // else: A key was pressed within the same tick, ignore it.
            }
        }
    }
}

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
#include "core/ConfigValueIncludes.h"
#include "core/CoreIncludes.h"
#include "core/Core.h"
//#include "objects/Backlight.h"
#include "objects/Tickable.h"
#include "objects/Radar.h"
//#include "tools/ParticleInterface.h"
#include "CameraManager.h"
#include "LevelManager.h"
#include "Settings.h"

namespace orxonox
{
    GSLevel::GSLevel()
//        : GameState<GSGraphics>(name)
        : timeFactor_(1.0f)
        , keyBinder_(0)
        , inputState_(0)
        , radar_(0)
        , startFile_(0)
        , cameraManager_(0)
        , levelManager_(0)
    {
        RegisterObject(GSLevel);
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

        if (Core::isMaster())
        {
            // create the global LevelManager
            this->levelManager_ = new LevelManager();

            // reset game speed to normal
            timeFactor_ = 1.0f;

            this->loadLevel();
        }

        if (Core::showsGraphics())
        {
            // TODO: insert slomo console command with
            // .accessLevel(AccessLevel::Offline).defaultValue(0, 1.0).axisParamIndex(0).isAxisRelative(false);

            // keybind console command
            FunctorMember<GSLevel>* functor1 = createFunctor(&GSLevel::keybind);
            functor1->setObject(this);
            CommandExecutor::addConsoleCommandShortcut(createConsoleCommand(functor1, "keybind"));
            FunctorMember<GSLevel>* functor2 = createFunctor(&GSLevel::tkeybind);
            functor2->setObject(this);
            CommandExecutor::addConsoleCommandShortcut(createConsoleCommand(functor2, "tkeybind"));
            // set our console command as callback for the key detector
            InputManager::getInstance().setKeyDetectorCallback(std::string("keybind ") + keyDetectorCallbackCode_);

            // level is loaded: we can start capturing the input
            InputManager::getInstance().requestEnterState("game");
        }

        if (Core::isMaster())
        {
            // time factor console command
            FunctorMember<GSLevel>* functor = createFunctor(&GSLevel::setTimeFactor);
            functor->setObject(this);
            CommandExecutor::addConsoleCommandShortcut(createConsoleCommand(functor, "setTimeFactor")).accessLevel(AccessLevel::Offline).defaultValue(0, 1.0);;
        }
    }

    void GSLevel::leave()
    {
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
            delete this->radar_;

        if (this->cameraManager_)
            delete this->cameraManager_;

        if (this->levelManager_)
            delete this->levelManager_;

        if (Core::showsGraphics())
        {
            inputState_->setHandler(0);
            InputManager::getInstance().requestDestroyState("game");
            if (this->keyBinder_)
                delete this->keyBinder_;
        }
    }

    void GSLevel::ticked(const Clock& time)
    {
        // Commented by 1337: Temporarily moved to GSGraphics.
        //// Call the scene objects
        //for (ObjectList<Tickable>::iterator it = ObjectList<Tickable>::begin(); it; ++it)
        //    it->tick(time.getDeltaTime() * this->timeFactor_);
    }

    /**
    @brief
        Changes the speed of Orxonox
    */
    void GSLevel::setTimeFactor(float factor)
    {
/*
        float change = factor / this->timeFactor_;
*/
        this->timeFactor_ = factor;
/*
        for (ObjectList<ParticleInterface>::iterator it = ObjectList<ParticleInterface>::begin(); it; ++it)
            it->setSpeedFactor(it->getSpeedFactor() * change);

        for (ObjectList<Backlight>::iterator it = ObjectList<Backlight>::begin(); it; ++it)
            it->setTimeFactor(timeFactor_);
*/
    }

    void GSLevel::loadLevel()
    {
        // call the loader
        COUT(0) << "Loading level..." << std::endl;
        startFile_ = new XMLFile(Settings::getDataPath() + "levels/sample2.oxw");
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

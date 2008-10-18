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

#include <OgreSceneManager.h>
#include <OgreRoot.h>
#include "core/input/InputManager.h"
#include "core/input/SimpleInputState.h"
#include "core/input/KeyBinder.h"
#include "core/Loader.h"
#include "core/CommandExecutor.h"
#include "core/ConsoleCommand.h"
#include "core/ConfigValueIncludes.h"
#include "core/CoreIncludes.h"
//#include "objects/Backlight.h"
#include "objects/Tickable.h"
#include "objects/Radar.h"
//#include "tools/ParticleInterface.h"
#include "Settings.h"
#include "GraphicsEngine.h"

namespace orxonox
{
    GSLevel::GSLevel(const std::string& name)
        : GameState<GSGraphics>(name)
        , timeFactor_(1.0f)
        , sceneManager_(0)
        , keyBinder_(0)
        , inputState_(0)
        , radar_(0)
        , startLevel_(0)
        , hud_(0)
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

    void GSLevel::enter()
    {
        inputState_ = InputManager::getInstance().createInputState<SimpleInputState>("game", 20);
        keyBinder_ = new KeyBinder();
        keyBinder_->loadBindings("keybindings.ini");
        inputState_->setHandler(keyBinder_);

        // create Ogre SceneManager for the level
        this->sceneManager_ = Ogre::Root::getSingleton().createSceneManager(Ogre::ST_GENERIC, "LevelSceneManager");
        COUT(4) << "Created SceneManager: " << sceneManager_->getName() << std::endl;

        // temporary hack
        GraphicsEngine::getInstance().setLevelSceneManager(this->sceneManager_);

        // Start the Radar
        this->radar_ = new Radar();

        // Load the HUD
        COUT(3) << "Orxonox: Loading HUD" << std::endl;
        hud_ = new Level(Settings::getDataPath() + "overlay/hud.oxo");
        Loader::load(hud_);

        // reset game speed to normal
        timeFactor_ = 1.0f;

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
    }

    void GSLevel::leave()
    {
        Loader::unload(hud_);
        delete this->hud_;

        // this call will delete every BaseObject!
        // But currently this will call methods of objects that exist no more
        // The only 'memory leak' is the ParticleSpawer. They would be deleted here
        // and call a sceneNode method that has already been destroy by the corresponding space ship.
        //Loader::close();

        delete this->radar_;

        Ogre::Root::getSingleton().destroySceneManager(this->sceneManager_);

        inputState_->setHandler(0);
        InputManager::getInstance().requestDestroyState("game");
        delete this->keyBinder_;
    }

    void GSLevel::ticked(const Clock& time)
    {
        // Call the scene objects
        for (ObjectList<Tickable>::iterator it = ObjectList<Tickable>::begin(); it; ++it)
            it->tick(time.getDeltaTime() * this->timeFactor_);
    }

    /**
    @brief
        Changes the speed of Orxonox
    */
    void GSLevel::setTimeFactor(float factor)
    {
        float change = factor / this->timeFactor_;
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
        startLevel_ = new Level(Settings::getDataPath() + "levels/sample2.oxw");
        Loader::open(startLevel_);
    }

    void GSLevel::unloadLevel()
    {
        Loader::unload(startLevel_);
        delete this->startLevel_;
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

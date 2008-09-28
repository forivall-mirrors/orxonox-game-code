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
#include "objects/Backlight.h"
#include "objects/Tickable.h"
#include "objects/Radar.h"
#include "tools/ParticleInterface.h"
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
    }

    GSLevel::~GSLevel()
    {
    }

    void GSLevel::enter()
    {
        inputState_ = InputManager::getInstance().createInputState<SimpleInputState>("game", 20);
        keyBinder_ = new KeyBinder();
        keyBinder_->loadBindings();
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
        for (ObjectList<ParticleInterface>::iterator it = ObjectList<ParticleInterface>::begin(); it; ++it)
            it->setSpeedFactor(it->getSpeedFactor() * change);

        for (ObjectList<Backlight>::iterator it = ObjectList<Backlight>::begin(); it; ++it)
            it->setTimeFactor(timeFactor_);
    }

    void GSLevel::loadLevel()
    {
        // call the loader
        COUT(0) << "Loading level..." << std::endl;
        startLevel_ = new Level(Settings::getDataPath() + "levels/tutorial.oxw");
        Loader::open(startLevel_);
    }

    void GSLevel::unloadLevel()
    {
        Loader::unload(startLevel_);
        delete this->startLevel_;
    }
}

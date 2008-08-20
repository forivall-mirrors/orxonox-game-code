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

#include "core/ConsoleCommand.h"
#include "core/input/InputManager.h"
#include "core/input/SimpleInputState.h"
#include "core/input/KeyBinder.h"
#include "core/Loader.h"
#include "core/CommandLine.h"
#include "overlays/console/InGameConsole.h"
#include "gui/GUIManager.h"
#include "objects/Backlight.h"
#include "tools/ParticleInterface.h"
#include "Radar.h"
#include "Settings.h"
#include "GraphicsEngine.h"

namespace orxonox
{
    SetCommandLineArgument(port, 55556).setShortcut("p");
    SetCommandLineArgument(ip, std::string("127.0.0.0"));

    GSLevel::GSLevel()
        : GameState("level")
        , timefactor_(1.0f)
        , keyBinder_(0)
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
        keyBinder_ = new KeyBinder();
        keyBinder_->loadBindings();
        InputManager::getInstance().createInputState<SimpleInputState>("game", 20)->setHandler(keyBinder_);

        // create Ogre SceneManager for the level
        GraphicsEngine::getInstance().createNewScene();

        // Start the Radar
        this->radar_ = new Radar();

        // Load the HUD
        COUT(3) << "Orxonox: Loading HUD" << std::endl;
        hud_ = new Level(Settings::getDataPath() + "overlay/hud.oxo");
        Loader::load(hud_);

        // call the loader
        COUT(0) << "Loading level..." << std::endl;
        startLevel_ = new Level(Settings::getDataPath() + "levels/sample.oxw");
        Loader::open(startLevel_);

        // add console commands
        FunctorMember<GSLevel>* functor = createFunctor(&GSLevel::setTimeFactor);
        functor->setObject(this);
        CommandExecutor::addConsoleCommandShortcut(createConsoleCommand(functor, "setTimeFactor"));

        // level is loaded: we can start capturing the input
        InputManager::getInstance().requestEnterState("game");
    }

    void GSLevel::leave()
    {
        InputManager::getInstance().requestLeaveState("game");

        // TODO: Remove and destroy console command

        Loader::unload(startLevel_);
        delete this->startLevel_;

        Loader::unload(hud_);
        delete this->hud_;

        // this call will delete every BaseObject!
        // But currently this will call methods of objects that exist no more
        // The only 'memory leak' is the ParticleSpawer. They would be deleted here
        // and call a sceneNode method that has already been destroy by the corresponding space ship.
        //Loader::close();

        delete this->radar_;

        // TODO: delete SceneManager

        InputManager::getInstance().destroyState("game");
        delete this->keyBinder_;
    }

    bool GSLevel::tick(float dt)
    {
        // Call those objects that need the real time
        for (Iterator<TickableReal> it = ObjectList<TickableReal>::start(); it; ++it)
            it->tick(dt);
        // Call the scene objects
        for (Iterator<Tickable> it = ObjectList<Tickable>::start(); it; ++it)
            it->tick(dt * this->timefactor_);

        // TODO: split file into server/client/standalone
        // call server/client with normal dt
        //if (client_g)
        //    client_g->tick(dt * this->timefactor_);
        //if (server_g)
        //    server_g->tick(dt * this->timefactor_);

        return true;
    }

    /**
    @brief
        Changes the speed of Orxonox
    */
    void GSLevel::setTimeFactor(float factor)
    {
        float change = factor / this->timefactor_;
        this->timefactor_ = factor;
        for (Iterator<ParticleInterface> it = ObjectList<ParticleInterface>::begin(); it; ++it)
            it->setSpeedFactor(it->getSpeedFactor() * change);

        for (Iterator<Backlight> it = ObjectList<Backlight>::begin(); it; ++it)
            it->setTimeFactor(timefactor_);
    }
}

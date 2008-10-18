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
#include "GSDedicated.h"

#include <OgreRoot.h>
#include <OgreSceneManager.h>
#include "core/ConsoleCommand.h"
#include "core/CommandLine.h"
#include "core/Loader.h"
#include "network/Server.h"
#include "objects/Tickable.h"
#include "GraphicsEngine.h"
#include "Settings.h"

namespace orxonox
{
    GSDedicated::GSDedicated()
        : GameState<GSRoot>("dedicated")
        , timeFactor_(0)
        , server_(0)
        , sceneManager_(0)
        , startLevel_(0)
    {
    }

    GSDedicated::~GSDedicated()
    {
    }

    void GSDedicated::enter()
    {
        Settings::_getInstance().setHasServer(true);

        // create Ogre SceneManager for the level
        this->sceneManager_ = Ogre::Root::getSingleton().createSceneManager(Ogre::ST_GENERIC, "LevelSceneManager");
        COUT(4) << "Created SceneManager: " << sceneManager_->getName() << std::endl;

        // temporary hack
        GraphicsEngine::getInstance().setLevelSceneManager(this->sceneManager_);

        // reset game speed to normal
        timeFactor_ = 1.0f;

        int serverPort = CommandLine::getArgument<int>("port")->getValue();
        this->server_ = new network::Server(serverPort);

        // call the loader
        COUT(0) << "Loading level..." << std::endl;
        startLevel_ = new Level(Settings::getDataPath() + "levels/sample.oxw");
        Loader::open(startLevel_);

        server_->open();

        // add console commands
        FunctorMember01<GSDedicated, float>* functor = createFunctor(&GSDedicated::setTimeFactor);
        functor->setObject(this);
        CommandExecutor::addConsoleCommandShortcut(createConsoleCommand(functor, "setTimeFactor")).accessLevel(AccessLevel::Offline).defaultValue(0, 1.0);;
    }

    void GSDedicated::leave()
    {
        // TODO: Remove and destroy console command

        Loader::unload(startLevel_);
        delete this->startLevel_;

        this->server_->close();
        delete this->server_;

        Ogre::Root::getSingleton().destroySceneManager(this->sceneManager_);

        Settings::_getInstance().setHasServer(false);
    }

    void GSDedicated::ticked(const Clock& time)
    {
        // Call the scene objects
        for (ObjectList<Tickable>::iterator it = ObjectList<Tickable>::begin(); it; ++it)
            it->tick(time.getDeltaTime() * this->timeFactor_);

        server_->tick(time.getDeltaTime());
        this->tickChild(time);
    }

    /**
    @brief
        Changes the speed of Orxonox
    */
    void GSDedicated::setTimeFactor(float factor)
    {
        this->timeFactor_ = factor;
    }
}

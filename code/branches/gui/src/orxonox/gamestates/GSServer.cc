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
#include "GSServer.h"

#include "core/ConsoleCommand.h"
#include "core/input/InputManager.h"
#include "core/CommandLine.h"
#include "network/Server.h"

namespace orxonox
{
    SetCommandLineArgument(port, 55556).setShortcut("p").setInformation("0-65535");

    GSServer::GSServer()
        : GSLevel("server")
        , server_(0)
    {
    }

    GSServer::~GSServer()
    {
    }

    void GSServer::enter()
    {
        GSLevel::enter();

        int serverPort = CommandLine::getArgument<int>("port")->getValue();
        this->server_ = network::Server::createSingleton(serverPort);

        this->loadLevel();

        server_->open();

        // add console commands
        FunctorMember<GSLevel>* functor = createFunctor(&GSLevel::setTimeFactor);
        functor->setObject(this);
        CommandExecutor::addConsoleCommandShortcut(createConsoleCommand(functor, "setTimeFactor"));

        // level is loaded: we can start capturing the input
        InputManager::getInstance().requestEnterState("game");
    }

    void GSServer::leave()
    {
        InputManager::getInstance().requestLeaveState("game");

        // TODO: Remove and destroy console command

        this->unloadLevel();

        this->server_->close();
        // TODO: destroy server

        GSLevel::leave();
    }

    void GSServer::ticked(float dt)
    {
        GSLevel::ticked(dt);
        server_->tick(dt);
        this->tickChild(dt);
    }
}

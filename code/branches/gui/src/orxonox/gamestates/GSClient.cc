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
#include "GSClient.h"

#include "core/input/InputManager.h"
#include "core/CommandLine.h"
#include "network/Client.h"

namespace orxonox
{
    SetCommandLineArgument(ip, "127.0.0.0").setInformation("#.#.#.#");

    GSClient::GSClient()
        : GSLevel("client")
        , client_(0)
    {
    }

    GSClient::~GSClient()
    {
    }

    void GSClient::enter()
    {
        GSLevel::enter();

        int serverPort = CommandLine::getArgument<int>("port")->getValue();
        std::string serverIP = CommandLine::getArgument<std::string>("ip")->getValue();
        this->client_ = network::Client::createSingleton(serverIP, serverPort);

        if(!client_->establishConnection())
            ThrowException(InitialisationFailed, "Could not establish connection with server.");

        client_->tick(0);

        // level is loaded: we can start capturing the input
        InputManager::getInstance().requestEnterState("game");
    }

    void GSClient::leave()
    {
        InputManager::getInstance().requestLeaveState("game");

        // TODO: How do we unload the level in client mode?

        client_->closeConnection();

        // TODO: destroy client

        GSLevel::leave();
    }

    void GSClient::ticked(const Clock& time)
    {
        GSLevel::ticked(time);
        client_->tick(time.getDeltaTime());

        this->tickChild(time);
    }
}

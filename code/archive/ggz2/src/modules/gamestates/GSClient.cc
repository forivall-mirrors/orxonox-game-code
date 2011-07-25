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
 *      Adrian Friedli
 *
 */

#include "GSClient.h"
#include "SpecialConfig.h"

#include "util/Exception.h"
#include "core/Clock.h"
#include "core/CommandLine.h"
#include "core/Game.h"
#include "core/GameMode.h"
#include "network/Client.h"
#ifdef GGZMOD_FOUND
#include "GGZClient.h"
#endif /* GGZMOD_FOUND */

namespace orxonox
{
    DeclareGameState(GSClient, "client", false, true);

    SetCommandLineArgument(ip, "127.0.0.1").information("Sever IP as string in the form #.#.#.#");

    GSClient::GSClient(const GameStateInfo& info)
        : GameState(info)
        , client_(0)
        , ggzClient(0)
    {
    }

    GSClient::~GSClient()
    {
    }

    void GSClient::activate()
    {
        GameMode::setIsClient(true);

#ifdef GGZMOD_FOUND
        if (GGZClient::isActive())
        {
            this->ggzClient = new GGZClient(this);
            return;
        }
#endif /* GGZMOD_FOUND */

        this->connect(CommandLine::getValue("ip").getString(), CommandLine::getValue("port"));
    }

    void GSClient::connect(const std::string& address, int port)
    {
        this->client_ = new Client(address, port);

        if(!client_->establishConnection())
            ThrowException(InitialisationFailed, "Could not establish connection with server.");

        client_->update(Game::getInstance().getGameClock());
    }

    void GSClient::deactivate()
    {
        client_->closeConnection();

#ifdef GGZMOD_FOUND
        if (ggzClient)
        {
            delete ggzClient;
        }
#endif /* GGZMOD_FOUND */

        // destroy client
        delete this->client_;

        GameMode::setIsClient(false);
    }

    void GSClient::update(const Clock& time)
    {
        if (client_)
        {
            client_->update(time);
        }
    }
}

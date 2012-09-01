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

#include "GSClient.h"

#include "util/Debug.h"
#include "util/Exception.h"
#include "core/CommandLineParser.h"
#include "core/Game.h"
#include "core/GameMode.h"
#include "network/Client.h"

namespace orxonox
{
    DeclareGameState(GSClient, "client", false, false);

    SetCommandLineArgument(ip, "127.0.0.1").information("Sever IP as string in the form #.#.#.#");

    GSClient::GSClient(const GameStateInfo& info)
        : GameState(info)
        , client_(0)
    {
    }

    GSClient::~GSClient()
    {
    }

    void GSClient::activate()
    {
        GameMode::setIsClient(true);

        this->client_ = new Client(CommandLineParser::getValue("ip").getString(), CommandLineParser::getValue("port"));

        if(!client_->establishConnection())
        {
            delete this->client_;
            ThrowException(InitialisationFailed, "Could not establish connection with server.");
        }

        client_->update(Game::getInstance().getGameClock());
    }

    void GSClient::deactivate()
    {
        client_->closeConnection();

        // destroy client
        delete this->client_;

        GameMode::setIsClient(false);
    }

    void GSClient::update(const Clock& time)
    {
        client_->update(time);
    }
}

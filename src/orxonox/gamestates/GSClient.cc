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

#include "util/Exception.h"
#include "core/commandline/CommandLineParser.h"
#include "core/Game.h"
#include "core/GameMode.h"
#include "network/Client.h"

namespace orxonox
{
    DeclareGameState(GSClient, "client", false, false);

    SetCommandLineArgument(dest, "127.0.0.1").information("Server hostname/IP (IP in the form of #.#.#.#)");

    GSClient::GSClient(const GameStateInfo& info)
        : GameState(info)
    {
    }

    GSClient::~GSClient()
    {
    }

    void GSClient::activate()
    {
        orxout(user_status) << "Starting client" << endl;

        GameMode::setIsClient(true);

//         this->client_ = new Client();
//         this->client_->setDestination(CommandLineParser::getValue("dest").getString(), CommandLineParser::getValue("port") );

        if( !Client::getInstance()->establishConnection() )
        {
            ThrowException(InitialisationFailed, "Could not establish connection with server.");
        }

        Client::getInstance()->update(Game::getInstance().getGameClock());
    }

    void GSClient::deactivate()
    {
        Client::getInstance()->closeConnection();

        // destroy client
//         delete this->client_;

        GameMode::setIsClient(false);
    }

    void GSClient::update(const Clock& time)
    {
        Client::getInstance()->update(time);
    }
}

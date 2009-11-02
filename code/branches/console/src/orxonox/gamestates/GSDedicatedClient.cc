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
 *      Oliver Scheuss
 *
 */

#include "GSDedicatedClient.h"

#include "util/Debug.h"
#include "util/Exception.h"
#include "core/CommandLine.h"
#include "core/Game.h"
#include "core/GameMode.h"
#include "network/Client.h"

namespace orxonox
{
    DeclareGameState(GSDedicatedClient, "dedicatedClient", false, false);
    
    GSDedicatedClient::GSDedicatedClient(const GameStateInfo& info)
        : GameState(info)
        , client_(0)
    {
    }

    GSDedicatedClient::~GSDedicatedClient()
    {
    }

    void GSDedicatedClient::activate()
    {
        GameMode::setIsClient(true);

        this->client_ = new Client(CommandLine::getValue("ip").getString(), CommandLine::getValue("port"));
        COUT(0) << "Loading scene in client mode" << std::endl;

        if( !client_->establishConnection() )
            ThrowException(InitialisationFailed, "Could not establish connection with server.");
            
        client_->update(Game::getInstance().getGameClock());
    }

    void GSDedicatedClient::deactivate()
    {
        if (this->client_)
        {
            this->client_->closeConnection();
            delete this->client_;
        }

        GameMode::setIsClient(false);
    }

    void GSDedicatedClient::update(const Clock& time)
    {
        client_->update(time);
    }
}

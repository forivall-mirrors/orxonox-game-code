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
 *      Fabian 'x3n' Landau
 *   Co-authors:
 *      ...
 *
 */

#include "OrxonoxStableHeaders.h"
#include "Gametype.h"

#include "core/CoreIncludes.h"
#include "objects/infos/PlayerInfo.h"

namespace orxonox
{
    CreateUnloadableFactory(Gametype);

    Gametype::Gametype()
    {
        RegisterObject(Gametype);

        this->getConnectedClients();

        COUT(0) << "created Gametype" << std::endl;
    }

    Gametype* Gametype::getCurrentGametype()
    {
        for (ObjectList<Gametype>::iterator it = ObjectList<Gametype>::begin(); it != ObjectList<Gametype>::end(); ++it)
            return (*it);

        return 0;
    }

    void Gametype::clientConnected(unsigned int clientID)
    {
        COUT(0) << "client connected" << std::endl;

        PlayerInfo* player = new PlayerInfo();
        player->setClientID(clientID);
    }

    void Gametype::clientDisconnected(unsigned int clientID)
    {
        COUT(0) << "client disconnected" << std::endl;
    }

    void Gametype::addPlayer(PlayerInfo* player)
    {
        this->players_.insert(player);
        this->playerJoined(player);
    }

    void Gametype::removePlayer(PlayerInfo* player)
    {
        this->players_.erase(player);
        this->playerLeft(player);
    }

    void Gametype::playerJoined(PlayerInfo* player)
    {
        COUT(0) << "player " << player->getName() << " joined" << std::endl;
    }

    void Gametype::playerLeft(PlayerInfo* player)
    {
        COUT(0) << "player " << player->getName() << " left" << std::endl;
    }
}

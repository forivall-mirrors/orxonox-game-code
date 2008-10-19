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
#include "core/ConsoleCommand.h"
#include "objects/infos/PlayerInfo.h"

#include "network/Host.h"

namespace orxonox
{
    SetConsoleCommand(Gametype, listPlayers, true);

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

    PlayerInfo* Gametype::getClient(unsigned int clientID)
    {
        Gametype* gametype = Gametype::getCurrentGametype();
        if (gametype)
        {
            std::map<unsigned int, PlayerInfo*>::const_iterator it = gametype->clients_.find(clientID);
            if (it != gametype->clients_.end())
                return it->second;
        }
        else
        {
            for (ObjectList<PlayerInfo>::iterator it = ObjectList<PlayerInfo>::begin(); it != ObjectList<PlayerInfo>::end(); ++it)
                if (it->getClientID() == clientID)
                    return (*it);
        }
        return 0;
    }

    void Gametype::listPlayers()
    {
        Gametype* gametype = Gametype::getCurrentGametype();

        if (gametype)
        {
            for (std::set<PlayerInfo*>::const_iterator it = gametype->players_.begin(); it != gametype->players_.end(); ++it)
                COUT(0) << "ID: " << (*it)->getClientID() << ", Name: " << (*it)->getName() << std::endl;
        }
        else
        {
            for (ObjectList<PlayerInfo>::iterator it = ObjectList<PlayerInfo>::begin(); it != ObjectList<PlayerInfo>::end(); ++it)
                COUT(0) << "ID: " << (*it)->getClientID() << ", Name: " << (*it)->getName() << std::endl;
        }
    }

    void Gametype::clientConnected(unsigned int clientID)
    {
        COUT(0) << "client connected" << std::endl;

        // create new PlayerInfo instance
        PlayerInfo* player = new PlayerInfo();
        player->setClientID(clientID);

        // add to clients-map
        assert(!this->clients_[clientID]);
        this->clients_[clientID] = player;
    }

    void Gametype::clientDisconnected(unsigned int clientID)
    {
        COUT(0) << "client disconnected" << std::endl;

        // remove from clients-map
        PlayerInfo* player = this->clients_[clientID];
        this->clients_.erase(clientID);

        // delete PlayerInfo instance
        delete player;
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
        std::string message = player->getName() + " entered the game";
        COUT(0) << message << std::endl;
        network::Host::Broadcast(message);
    }

    void Gametype::playerLeft(PlayerInfo* player)
    {
        std::string message = player->getName() + " left the game";
        COUT(0) << message << std::endl;
        network::Host::Broadcast(message);
    }

    void Gametype::playerChangedName(PlayerInfo* player)
    {
        if (this->players_.find(player) != this->players_.end())
        {
            if (player->getName() != player->getOldName())
            {
                std::string message = player->getOldName() + " changed name to " + player->getName();
                COUT(0) << message << std::endl;
                network::Host::Broadcast(message);
            }
        }
    }
}

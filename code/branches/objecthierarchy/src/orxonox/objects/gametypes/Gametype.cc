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
#include "objects/worldentities/pawns/Spectator.h"

#include "network/Host.h"

namespace orxonox
{
    CreateUnloadableFactory(Gametype);

    Gametype::Gametype()
    {
        RegisterObject(Gametype);

        this->defaultPawn_ = Class(Spectator);

        COUT(0) << "created Gametype" << std::endl;
    }

    void Gametype::addPlayer(PlayerInfo* player)
    {
        this->players_.insert(player);
        this->playerJoined(player);

        ControllableEntity* newpawn = this->defaultPawn_.fabricate();
        player->startControl(newpawn);
    }

    void Gametype::removePlayer(PlayerInfo* player)
    {
        if (this->players_.find(player) != this->players_.end())
        {
            player->stopControl();
            this->players_.erase(player);
            this->playerLeft(player);
        }
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

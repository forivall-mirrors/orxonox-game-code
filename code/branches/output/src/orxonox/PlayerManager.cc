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

#include "PlayerManager.h"

#include "core/CoreIncludes.h"
#include "core/GameMode.h"
#include "util/ScopedSingletonManager.h"

#include "Level.h"
#include "LevelManager.h"
#include "infos/HumanPlayer.h"

namespace orxonox
{
    ManageScopedSingleton(PlayerManager, ScopeID::Root, false);

    PlayerManager::PlayerManager()
    {
        RegisterRootObject(PlayerManager);

//         this->getConnectedClients();
    }

    PlayerManager::~PlayerManager()
    {
    }

    void PlayerManager::clientConnected(unsigned int clientID)
    {
        if (GameMode::isMaster())
        {
            if (clientID != 0)
                COUT(3) << "client connected" << std::endl;

            // create new HumanPlayer instance
            HumanPlayer* player = new HumanPlayer(0);
            player->setClientID(clientID);

            // add to clients-map
            assert(!this->clients_[clientID]);
            this->clients_[clientID] = player;

            if (LevelManager::exists() && LevelManager::getInstance().getActiveLevel())
                LevelManager::getInstance().getActiveLevel()->playerEntered(player);
        }
    }

    void PlayerManager::clientDisconnected(unsigned int clientID)
    {
        if (GameMode::isMaster())
        {
            if (clientID != 0)
                COUT(3) << "client disconnected" << std::endl;

            // remove from clients-map
            PlayerInfo* player = this->clients_[clientID];
            this->clients_.erase(clientID);

            if (LevelManager::exists() && LevelManager::getInstance().getActiveLevel())
                LevelManager::getInstance().getActiveLevel()->playerLeft(player);

            // delete PlayerInfo instance
            if (player)
                player->destroy();
        }
    }

    void PlayerManager::disconnectAllClients()
    {
        for( std::map<unsigned int, PlayerInfo*>::iterator it = this->clients_.begin(); it != this->clients_.end(); )
            this->clientDisconnected( (it++)->first );
    }


    PlayerInfo* PlayerManager::getClient(unsigned int clientID) const
    {
        if (GameMode::isMaster())
        {
            std::map<unsigned int, PlayerInfo*>::const_iterator it = this->clients_.find(clientID);
            if (it != this->clients_.end())
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
}

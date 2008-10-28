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

#include <cstdlib>
#include <ctime>

#include "core/CoreIncludes.h"
#include "objects/infos/PlayerInfo.h"
#include "objects/worldentities/pawns/Spectator.h"
#include "objects/worldentities/SpawnPoint.h"

#include "network/Host.h"

namespace orxonox
{
    CreateUnloadableFactory(Gametype);

    Gametype::Gametype(BaseObject* creator) : BaseObject(creator)
    {
        RegisterObject(Gametype);

        this->defaultControllableEntity_ = Class(Spectator);

        this->bStarted_ = false;
        this->bEnded_ = false;
        this->bAutoStart_ = false;
        this->bForceSpawn_ = false;

        this->initialStartCountdown_ = 3;
        this->startCountdown_ = 0;
        this->bStartCountdownRunning_ = false;

        COUT(0) << "created Gametype" << std::endl;
    }

    void Gametype::tick(float dt)
    {
        if (this->bStartCountdownRunning_ && !this->bStarted_)
            this->startCountdown_ -= dt;

        if (!this->bStarted_)
            this->checkStart();

        this->assignDefaultPawnsIfNeeded();
        this->spawnDeadPlayersIfRequested();
    }

    void Gametype::start()
    {
        COUT(0) << "game started" << std::endl;
        this->bStarted_ = true;

        this->spawnPlayersIfRequested();
    }

    void Gametype::end()
    {
        COUT(0) << "game ended" << std::endl;
        this->bEnded_ = true;
    }

    void Gametype::playerEntered(PlayerInfo* player)
    {
        this->players_.insert(player);

        std::string message = player->getName() + " entered the game";
        COUT(0) << message << std::endl;
        network::Host::Broadcast(message);
    }

    void Gametype::playerLeft(PlayerInfo* player)
    {
        std::set<PlayerInfo*>::iterator it = this->players_.find(player);
        if (it != this->players_.end())
        {
            this->players_.erase(it);

            std::string message = player->getName() + " left the game";
            COUT(0) << message << std::endl;
            network::Host::Broadcast(message);
        }
    }

    void Gametype::playerSwitched(PlayerInfo* player, Gametype* newgametype)
    {
    }

    void Gametype::playerSwitchedBack(PlayerInfo* player, Gametype* oldgametype)
    {
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

    void Gametype::pawnPreSpawn(Pawn* pawn)
    {
    }

    void Gametype::pawnPostSpawn(Pawn* pawn)
    {
    }

    void Gametype::pawnKilled(Pawn* victim, Pawn* killer)
    {
    }

    void Gametype::playerScored(PlayerInfo* player)
    {
    }

    SpawnPoint* Gametype::getBestSpawnPoint(PlayerInfo* player) const
    {
        if (this->spawnpoints_.size() > 0)
        {
            srand(time(0));
            rnd();

            unsigned int randomspawn = (unsigned int)rnd(this->spawnpoints_.size());
            unsigned int index = 0;
            for (std::set<SpawnPoint*>::const_iterator it = this->spawnpoints_.begin(); it != this->spawnpoints_.end(); ++it)
            {
                if (index == randomspawn)
                    return (*it);

                ++index;
            }
        }
        return 0;
    }

    void Gametype::assignDefaultPawnsIfNeeded() const
    {
        for (std::set<PlayerInfo*>::const_iterator it = this->players_.begin(); it != this->players_.end(); ++it)
        {
            if (!(*it)->getControllableEntity() && (!(*it)->isReadyToSpawn() || !this->bStarted_))
            {
                SpawnPoint* spawn = this->getBestSpawnPoint(*it);
                if (spawn)
                {
                    // force spawn at spawnpoint with default pawn
                    ControllableEntity* entity = this->defaultControllableEntity_.fabricate(spawn);
                    spawn->spawn(entity);
                    (*it)->startControl(entity);
                }
                else
                {
                    COUT(1) << "Error: No SpawnPoints in current Gametype" << std::endl;
                    abort();
                }
            }
        }
    }

    void Gametype::checkStart()
    {
        if (!this->bStarted_)
        {
            if (this->bStartCountdownRunning_)
            {
                if (this->startCountdown_ <= 0)
                {
                    this->bStartCountdownRunning_ = false;
                    this->startCountdown_ = 0;
                    this->start();
                }
            }
            else if (this->players_.size() > 0)
            {
                if (this->bAutoStart_)
                {
                    this->start();
                }
                else
                {
                    bool allplayersready = true;
                    for (std::set<PlayerInfo*>::iterator it = this->players_.begin(); it != this->players_.end(); ++it)
                    {
                        if (!(*it)->isReadyToSpawn())
                            allplayersready = false;
                    }
                    if (allplayersready)
                    {
                        this->startCountdown_ = this->initialStartCountdown_;
                        this->bStartCountdownRunning_ = true;
                    }
                }
            }
        }
    }

    void Gametype::spawnPlayersIfRequested()
    {
        for (std::set<PlayerInfo*>::iterator it = this->players_.begin(); it != this->players_.end(); ++it)
            if ((*it)->isReadyToSpawn() || this->bForceSpawn_)
                this->spawnPlayer(*it);
    }

    void Gametype::spawnDeadPlayersIfRequested()
    {
        for (std::set<PlayerInfo*>::iterator it = this->players_.begin(); it != this->players_.end(); ++it)
            if (!(*it)->getControllableEntity())
                if ((*it)->isReadyToSpawn() || this->bForceSpawn_)
                    this->spawnPlayer(*it);
    }

    void Gametype::spawnPlayer(PlayerInfo* player)
    {
        SpawnPoint* spawnpoint = this->getBestSpawnPoint(player);
        if (spawnpoint)
        {
            player->startControl(spawnpoint->spawn());
        }
        else
        {
            COUT(1) << "Error: No SpawnPoints in current Gametype" << std::endl;
            abort();
        }
    }
}

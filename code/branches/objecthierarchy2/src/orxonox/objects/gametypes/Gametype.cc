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
#include "core/ConfigValueIncludes.h"
#include "objects/infos/PlayerInfo.h"
#include "objects/worldentities/pawns/Spectator.h"
#include "objects/worldentities/SpawnPoint.h"

#include "network/Host.h"

namespace orxonox
{
    CreateUnloadableFactory(Gametype);

    Gametype::Gametype(BaseObject* creator) : BaseObject(creator), gtinfo_(creator)
    {
        RegisterObject(Gametype);

        this->defaultControllableEntity_ = Class(Spectator);

        this->bAutoStart_ = false;
        this->bForceSpawn_ = false;

        this->initialStartCountdown_ = 3;

        this->setConfigValues();
    }

    void Gametype::setConfigValues()
    {
        SetConfigValue(initialStartCountdown_, 3.0f);
    }

    void Gametype::tick(float dt)
    {
        SUPER(Gametype, tick, dt);

        if (this->gtinfo_.bStartCountdownRunning_ && !this->gtinfo_.bStarted_)
            this->gtinfo_.startCountdown_ -= dt;

        if (!this->gtinfo_.bStarted_)
            this->checkStart();
        else
            this->spawnDeadPlayersIfRequested();

        this->assignDefaultPawnsIfNeeded();
    }

    void Gametype::start()
    {
        COUT(0) << "game started" << std::endl;
        this->gtinfo_.bStarted_ = true;

        this->spawnPlayersIfRequested();
    }

    void Gametype::end()
    {
        COUT(0) << "game ended" << std::endl;
        this->gtinfo_.bEnded_ = true;
    }

    void Gametype::playerEntered(PlayerInfo* player)
    {
        this->players_[player] = PlayerState::Joined;

        std::string message = player->getName() + " entered the game";
        COUT(0) << message << std::endl;
        Host::Broadcast(message);
    }

    void Gametype::playerLeft(PlayerInfo* player)
    {
        std::map<PlayerInfo*, PlayerState::Enum>::iterator it = this->players_.find(player);
        if (it != this->players_.end())
        {
            this->players_.erase(it);

            std::string message = player->getName() + " left the game";
            COUT(0) << message << std::endl;
            Host::Broadcast(message);
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
                Host::Broadcast(message);
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

    void Gametype::assignDefaultPawnsIfNeeded()
    {
        for (std::map<PlayerInfo*, PlayerState::Enum>::iterator it = this->players_.begin(); it != this->players_.end(); ++it)
        {
            if (!it->first->getControllableEntity())
            {
                it->second = PlayerState::Dead;

                if (!it->first->isReadyToSpawn() || !this->gtinfo_.bStarted_)
                {
                    SpawnPoint* spawn = this->getBestSpawnPoint(it->first);
                    if (spawn)
                    {
                        // force spawn at spawnpoint with default pawn
                        ControllableEntity* entity = this->defaultControllableEntity_.fabricate(spawn);
                        spawn->spawn(entity);
                        it->first->startControl(entity);
                        it->second = PlayerState::Dead;
                    }
                    else
                    {
                        COUT(1) << "Error: No SpawnPoints in current Gametype" << std::endl;
                        abort();
                    }
                }
            }
        }
    }

    void Gametype::checkStart()
    {
        if (!this->gtinfo_.bStarted_)
        {
            if (this->gtinfo_.bStartCountdownRunning_)
            {
                if (this->gtinfo_.startCountdown_ <= 0)
                {
                    this->gtinfo_.bStartCountdownRunning_ = false;
                    this->gtinfo_.startCountdown_ = 0;
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
                    bool hashumanplayers = false;
                    for (std::map<PlayerInfo*, PlayerState::Enum>::iterator it = this->players_.begin(); it != this->players_.end(); ++it)
                    {
                        if (!it->first->isReadyToSpawn())
                            allplayersready = false;
                        if (it->first->isHumanPlayer())
                            hashumanplayers = true;
                    }
                    if (allplayersready && hashumanplayers)
                    {
                        this->gtinfo_.startCountdown_ = this->initialStartCountdown_;
                        this->gtinfo_.bStartCountdownRunning_ = true;
                    }
                }
            }
        }
    }

    void Gametype::spawnPlayersIfRequested()
    {
        for (std::map<PlayerInfo*, PlayerState::Enum>::iterator it = this->players_.begin(); it != this->players_.end(); ++it)
            if (it->first->isReadyToSpawn() || this->bForceSpawn_)
                this->spawnPlayer(it->first);
    }

    void Gametype::spawnDeadPlayersIfRequested()
    {
        for (std::map<PlayerInfo*, PlayerState::Enum>::iterator it = this->players_.begin(); it != this->players_.end(); ++it)
            if (it->second == PlayerState::Dead)
                if (it->first->isReadyToSpawn() || this->bForceSpawn_)
                    this->spawnPlayer(it->first);
    }

    void Gametype::spawnPlayer(PlayerInfo* player)
    {
        SpawnPoint* spawnpoint = this->getBestSpawnPoint(player);
        if (spawnpoint)
        {
            player->startControl(spawnpoint->spawn());
            this->players_[player] = PlayerState::Alive;
        }
        else
        {
            COUT(1) << "Error: No SpawnPoints in current Gametype" << std::endl;
            abort();
        }
    }
}

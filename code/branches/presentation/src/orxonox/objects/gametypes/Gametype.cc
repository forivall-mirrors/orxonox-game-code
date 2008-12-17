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
#include "core/Loader.h"
#include "core/XMLFile.h"
#include "objects/infos/PlayerInfo.h"
#include "objects/infos/Bot.h"
#include "objects/worldentities/pawns/Spectator.h"
#include "objects/worldentities/SpawnPoint.h"
#include "objects/worldentities/Camera.h"
#include "Settings.h"

#include "network/Host.h"

namespace orxonox
{
    CreateUnloadableFactory(Gametype);

    Gametype::Gametype(BaseObject* creator) : BaseObject(creator), gtinfo_(creator)
    {
        RegisterObject(Gametype);

        this->setGametype(this);

        this->defaultControllableEntity_ = Class(Spectator);

        this->bAutoStart_ = false;
        this->bForceSpawn_ = false;
        this->numberOfBots_ = 0;

        this->initialStartCountdown_ = 3;

        this->setConfigValues();

        this->addBots(this->numberOfBots_);

        this->statsOverlay_ = 0;

        setConfigValues();

        // load the corresponding score board
        //this->statsOverlay_ = new XMLFile(Settings::getDataPath() + "overlay/" + this->statsOverlayName_);
        //Loader::open(statsOverlay_);
        //this->setGametype(this);
    }

    void Gametype::setConfigValues()
    {
        SetConfigValue(initialStartCountdown_, 3.0f);
        SetConfigValue(bAutoStart_, false);
        SetConfigValue(bForceSpawn_, false);
        SetConfigValue(numberOfBots_, 0);
        SetConfigValue(statsOverlayName_, "stats.oxo");
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
        if (victim && victim->getPlayer())
        {
            std::string message;
            if (killer)
            {
                if (killer->getPlayer())
                    message = victim->getPlayer()->getName() + " was killed by " + killer->getPlayer()->getName();
                else
                    message = victim->getPlayer()->getName() + " was killed";
            }
            else
                message = victim->getPlayer()->getName() + " died";

            COUT(0) << message << std::endl;
            Host::Broadcast(message);
        }

        if (victim && victim->getPlayer())
        {
            std::map<PlayerInfo*, PlayerState::Enum>::iterator it = this->players_.find(victim->getPlayer());
            if (it != this->players_.end())
            {
                it->second = PlayerState::Dead;

                ControllableEntity* entity = this->defaultControllableEntity_.fabricate(victim->getCreator());
                if (victim->getCamera())
                {
                    entity->setPosition(victim->getCamera()->getWorldPosition());
                    entity->setOrientation(victim->getCamera()->getWorldOrientation());
                }
                else
                {
                    entity->setPosition(victim->getWorldPosition());
                    entity->setOrientation(victim->getWorldOrientation());
                }
                it->first->startControl(entity);
            }
            else
                COUT(2) << "Warning: Killed Pawn was not in the playerlist" << std::endl;
        }
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

    void Gametype::addBots(unsigned int amount)
    {
        for (unsigned int i = 0; i < amount; ++i)
            new Bot(this);
    }

    void Gametype::killBots(unsigned int amount)
    {
        unsigned int i = 0;
        for (ObjectList<Bot>::iterator it = ObjectList<Bot>::begin(); (it != ObjectList<Bot>::end()) && ((amount == 0) || (i < amount)); )
        {
            if (it->getGametype() == this)
            {
                delete (*(it++));
                ++i;
            }
        }
    }
}

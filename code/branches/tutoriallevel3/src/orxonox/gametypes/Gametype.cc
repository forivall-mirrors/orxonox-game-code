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

#include "Gametype.h"

#include "util/Math.h"
#include "core/Core.h"
#include "core/CoreIncludes.h"
#include "core/ConfigValueIncludes.h"
#include "core/GameMode.h"
#include "core/command/ConsoleCommand.h"

#include "infos/PlayerInfo.h"
#include "infos/Bot.h"
#include "graphics/Camera.h"
#include "worldentities/ControllableEntity.h"
#include "worldentities/SpawnPoint.h"
#include "worldentities/pawns/Spectator.h"
#include "worldentities/pawns/Pawn.h"
#include "overlays/OverlayGroup.h"

namespace orxonox
{
    CreateUnloadableFactory(Gametype);

    Gametype::Gametype(BaseObject* creator) : BaseObject(creator)
    {
        RegisterObject(Gametype);

        this->gtinfo_ = new GametypeInfo(creator);

        this->setGametype(SmartPtr<Gametype>(this, false));

        this->defaultControllableEntity_ = Class(Spectator);
        
        this->bAutoStart_ = false;
        this->bForceSpawn_ = false;
        this->numberOfBots_ = 0;

        this->timeLimit_ = 0;
        this->time_ = 0;
        this->timerIsActive_ = false;

        this->initialStartCountdown_ = 3;

        this->setConfigValues();

        // load the corresponding score board
        if (GameMode::showsGraphics() && !this->scoreboardTemplate_.empty())
        {
            this->scoreboard_ = new OverlayGroup(this);
            this->scoreboard_->addTemplate(this->scoreboardTemplate_);
            this->scoreboard_->setGametype(this);
        }
        else
            this->scoreboard_ = 0;

        /* HACK HACK HACK */
        this->dedicatedAddBots_ = createConsoleCommand( "dedicatedAddBots", createExecutor( createFunctor(&Gametype::addBots, this) ) );
        this->dedicatedKillBots_ = createConsoleCommand( "dedicatedKillBots", createExecutor( createFunctor(&Gametype::killBots, this) ) );
        /* HACK HACK HACK */
    }

    Gametype::~Gametype()
    {
        if (this->isInitialized())
        {
            this->gtinfo_->destroy();
            if( this->dedicatedAddBots_ )
                delete this->dedicatedAddBots_;
            if( this->dedicatedKillBots_ )
                delete this->dedicatedKillBots_;
        }
    }

    void Gametype::setConfigValues()
    {
        SetConfigValue(initialStartCountdown_, 3.0f);
        SetConfigValue(bAutoStart_, false);
        SetConfigValue(bForceSpawn_, false);
        SetConfigValue(numberOfBots_, 0);
        SetConfigValue(scoreboardTemplate_, "defaultScoreboard");
    }

    void Gametype::tick(float dt)
    {
        SUPER(Gametype, tick, dt);

        //count timer
        if (timerIsActive_)
        {
            if (this->timeLimit_ == 0)
                this->time_ += dt;
            else
                this->time_ -= dt;
        }

        if (this->gtinfo_->isStartCountdownRunning() && !this->gtinfo_->hasStarted())
            this->gtinfo_->countdownStartCountdown(dt);

        if (!this->gtinfo_->hasStarted())
        {
            for (std::map<PlayerInfo*, Player>::iterator it = this->players_.begin(); it != this->players_.end(); ++it)
            {
                // Inform the GametypeInfo that the player is ready to spawn.
                if(it->first->isHumanPlayer() && it->first->isReadyToSpawn())
                    this->gtinfo_->playerReadyToSpawn(it->first);
            }
                    
            this->checkStart();
        }
        else if (!this->gtinfo_->hasEnded())
            this->spawnDeadPlayersIfRequested();

        this->assignDefaultPawnsIfNeeded();
    }

    void Gametype::start()
    {
        this->addBots(this->numberOfBots_);

        this->gtinfo_->start();

        this->spawnPlayersIfRequested();
    }

    void Gametype::end()
    {
        this->gtinfo_->end();

        for (std::map<PlayerInfo*, Player>::iterator it = this->players_.begin(); it != this->players_.end(); ++it)
        {
            if (it->first->getControllableEntity())
            {
                ControllableEntity* oldentity = it->first->getControllableEntity();

                ControllableEntity* entity = this->defaultControllableEntity_.fabricate(oldentity);
                if (oldentity->getCamera())
                {
                    entity->setPosition(oldentity->getCamera()->getWorldPosition());
                    entity->setOrientation(oldentity->getCamera()->getWorldOrientation());
                }
                else
                {
                    entity->setPosition(oldentity->getWorldPosition());
                    entity->setOrientation(oldentity->getWorldOrientation());
                }

                it->first->startControl(entity);
            }
            else
                this->spawnPlayerAsDefaultPawn(it->first);
        }
    }

    void Gametype::playerEntered(PlayerInfo* player)
    {
        this->players_[player].state_ = PlayerState::Joined;
        this->gtinfo_->playerEntered(player);
    }

    bool Gametype::playerLeft(PlayerInfo* player)
    {
        std::map<PlayerInfo*, Player>::iterator it = this->players_.find(player);
        if (it != this->players_.end())
        {
            this->players_.erase(it);
            return true;
        }
        return false;
    }

    void Gametype::playerSwitched(PlayerInfo* player, Gametype* newgametype)
    {
    }

    void Gametype::playerSwitchedBack(PlayerInfo* player, Gametype* oldgametype)
    {
    }

    bool Gametype::playerChangedName(PlayerInfo* player)
    {
        if (this->players_.find(player) != this->players_.end())
        {
            if (player->getName() != player->getOldName())
            {
                return true;
            }
        }
        return false;
    }

    void Gametype::pawnPreSpawn(Pawn* pawn)
    {
    }

    void Gametype::pawnPostSpawn(Pawn* pawn)
    {
    }

    void Gametype::playerPreSpawn(PlayerInfo* player)
    {
    }

    void Gametype::playerPostSpawn(PlayerInfo* player)
    {
    }

    void Gametype::playerStartsControllingPawn(PlayerInfo* player, Pawn* pawn)
    {
    }

    void Gametype::playerStopsControllingPawn(PlayerInfo* player, Pawn* pawn)
    {
    }

    bool Gametype::allowPawnHit(Pawn* victim, Pawn* originator)
    {
        return true;
    }

    bool Gametype::allowPawnDamage(Pawn* victim, Pawn* originator)
    {
        return true;
    }

    bool Gametype::allowPawnDeath(Pawn* victim, Pawn* originator)
    {
        return true;
    }

    void Gametype::pawnKilled(Pawn* victim, Pawn* killer)
    {
        if (victim && victim->getPlayer())
        {
            std::map<PlayerInfo*, Player>::iterator it = this->players_.find(victim->getPlayer());
            if (it != this->players_.end())
            {
                it->second.state_ = PlayerState::Dead;
                it->second.killed_++;

                // Reward killer
                if (killer && killer->getPlayer())
                {
                    std::map<PlayerInfo*, Player>::iterator it = this->players_.find(killer->getPlayer());
                    if (it != this->players_.end())
                    {
                        it->second.frags_++;

                        if (killer->getPlayer()->getClientID() != NETWORK_PEER_ID_UNKNOWN)
                            this->gtinfo_->sendKillMessage("You killed " + victim->getPlayer()->getName(), killer->getPlayer()->getClientID());
                        if (victim->getPlayer()->getClientID() != NETWORK_PEER_ID_UNKNOWN)
                            this->gtinfo_->sendDeathMessage("You were killed by " + killer->getPlayer()->getName(), victim->getPlayer()->getClientID());
                    }
                }

                if(victim->getPlayer()->isHumanPlayer())
                    this->gtinfo_->pawnKilled(victim->getPlayer());

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
        std::map<PlayerInfo*, Player>::iterator it = this->players_.find(player);
        if (it != this->players_.end())
            it->second.frags_++;
    }

    int Gametype::getScore(PlayerInfo* player) const
    {
        std::map<PlayerInfo*, Player>::const_iterator it = this->players_.find(player);
        if (it != this->players_.end())
            return it->second.frags_;
        else
            return 0;
    }

    SpawnPoint* Gametype::getBestSpawnPoint(PlayerInfo* player) const
    {
        if (this->spawnpoints_.size() > 0)
        {
            SpawnPoint* fallbackSpawnPoint = NULL;
            unsigned int randomspawn = static_cast<unsigned int>(rnd(static_cast<float>(this->spawnpoints_.size())));
            unsigned int index = 0;
            std::set<SpawnPoint*> activeSpawnPoints = this->spawnpoints_;
            for (std::set<SpawnPoint*>::const_iterator it = this->spawnpoints_.begin(); it != this->spawnpoints_.end(); ++it)
            {
                if (index == randomspawn)
                    fallbackSpawnPoint = (*it);

                if (!(*it)->isActive())
                    activeSpawnPoints.erase(*it);

                ++index;
            }

            randomspawn = static_cast<unsigned int>(rnd(static_cast<float>(this->spawnpoints_.size())));
            index = 0;
            for (std::set<SpawnPoint*>::const_iterator it = activeSpawnPoints.begin(); it != activeSpawnPoints.end(); ++it)
            {
                if (index == randomspawn)
                    return (*it);

                ++index;
            }

            return fallbackSpawnPoint;
        }
        return 0;
    }

    void Gametype::assignDefaultPawnsIfNeeded()
    {
        for (std::map<PlayerInfo*, Player>::iterator it = this->players_.begin(); it != this->players_.end(); ++it)
        {
            if (!it->first->getControllableEntity())
            {
                it->second.state_ = PlayerState::Dead;

                if (!it->first->isReadyToSpawn() || !this->gtinfo_->hasStarted())
                {
                    this->spawnPlayerAsDefaultPawn(it->first);
                    it->second.state_ = PlayerState::Dead;
                }
            }
        }
    }

    void Gametype::checkStart()
    {
        if (!this->gtinfo_->hasStarted())
        {
            if (this->gtinfo_->isStartCountdownRunning())
            {
                if (this->gtinfo_->getStartCountdown() <= 0.0f)
                {
                    this->gtinfo_->stopStartCountdown();
                    this->gtinfo_->setStartCountdown(0.0f);;
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
                    for (std::map<PlayerInfo*, Player>::iterator it = this->players_.begin(); it != this->players_.end(); ++it)
                    {
                        if (!it->first->isReadyToSpawn())
                            allplayersready = false;
                        if (it->first->isHumanPlayer())
                            hashumanplayers = true;
                    }
                    if (allplayersready && hashumanplayers)
                    {
                        // If in developer's mode, there is no start countdown.
                        if(Core::getInstance().inDevMode())
                            this->start();
                        else
                            this->gtinfo_->setStartCountdown(this->initialStartCountdown_);
                        this->gtinfo_->startStartCountdown();
                    }
                }
            }
        }
    }

    void Gametype::spawnPlayersIfRequested()
    {
        for (std::map<PlayerInfo*, Player>::iterator it = this->players_.begin(); it != this->players_.end(); ++it)
        {
            if (it->first->isReadyToSpawn() || this->bForceSpawn_)
                this->spawnPlayer(it->first);
        }
    }

    void Gametype::spawnDeadPlayersIfRequested()
    {
        for (std::map<PlayerInfo*, Player>::iterator it = this->players_.begin(); it != this->players_.end(); ++it)
            if (it->second.state_ == PlayerState::Dead)
                if (it->first->isReadyToSpawn() || this->bForceSpawn_)
                    this->spawnPlayer(it->first);
    }

    void Gametype::spawnPlayer(PlayerInfo* player)
    {
        SpawnPoint* spawnpoint = this->getBestSpawnPoint(player);
        if (spawnpoint)
        {
            this->playerPreSpawn(player);
            player->startControl(spawnpoint->spawn());
            this->players_[player].state_ = PlayerState::Alive;

            if(player->isHumanPlayer())
                this->gtinfo_->playerSpawned(player);
            
            this->playerPostSpawn(player);
        }
        else
        {
            COUT(1) << "Error: No SpawnPoints in current Gametype" << std::endl;
            abort();
        }
    }

    void Gametype::spawnPlayerAsDefaultPawn(PlayerInfo* player)
    {
        SpawnPoint* spawn = this->getBestSpawnPoint(player);
        if (spawn)
        {
            // force spawn at spawnpoint with default pawn
            ControllableEntity* entity = this->defaultControllableEntity_.fabricate(spawn);
            spawn->spawn(entity);
            player->startControl(entity);
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
            this->botclass_.fabricate(this);
    }

    void Gametype::killBots(unsigned int amount)
    {
        unsigned int i = 0;
        for (ObjectList<Bot>::iterator it = ObjectList<Bot>::begin(); (it != ObjectList<Bot>::end()) && ((amount == 0) || (i < amount)); )
        {
            if (it->getGametype() == this)
            {
                (it++)->destroy();
                ++i;
            }
            else
                ++it;
        }
    }

    void Gametype::addTime(float t)
    {
        if (this->timeLimit_ == 0)
          this->time_ -= t;
        else
          this->time_ += t;
    }

    void Gametype::removeTime(float t)
    {
        if (this->timeLimit_ == 0)
          this->time_ += t;
        else
          this->time_ -= t;
    }

    void Gametype::resetTimer()
    {
        this->resetTimer(timeLimit_);
    }

    void Gametype::resetTimer(float t)
    {
        this->timeLimit_ = t;
        this->time_ = t;
    }
}

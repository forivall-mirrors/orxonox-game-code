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
 *      Johannes Ritz
 *   Co-authors:
 *      ...
 *
 */

#include "LastManStanding.h"

#include "core/CoreIncludes.h"
#include "network/Host.h"
#include "infos/PlayerInfo.h"
#include "worldentities/pawns/Pawn.h"
#include "core/ConfigValueIncludes.h"

namespace orxonox
{
    CreateUnloadableFactory(LastManStanding);

    LastManStanding::LastManStanding(BaseObject* creator) : Gametype(creator)
    {
        RegisterObject(LastManStanding);
        this->bForceSpawn_=true;
        this->lives=4;
        this->playersAlive=0;
        this->timeRemaining=20.0f;
    }

    void LastManStanding::spawnDeadPlayersIfRequested()
    {
        for (std::map<PlayerInfo*, Player>::iterator it = this->players_.begin(); it != this->players_.end(); ++it)
            if (it->second.state_ == PlayerState::Dead)
            {
                bool alive = (0<playerLives_[it->first]);
                if (alive&&(it->first->isReadyToSpawn() || this->bForceSpawn_))
                    this->spawnPlayer(it->first);
             }
    }


    void LastManStanding::setConfigValues()
    {
        SetConfigValue(lives, 4);
        SetConfigValue(timeRemaining, 20.0f);
    }

    bool LastManStanding::allowPawnDamage(Pawn* victim, Pawn* originator)
    {
        if (originator && originator->getPlayer())// only for safety
        {
            this->timeToAct_[originator->getPlayer()]=timeRemaining;
        }

        return true;
    }

    bool LastManStanding::allowPawnDeath(Pawn* victim, Pawn* originator)
    {
        if (!victim||!victim->getPlayer())// only for safety
            return true;
        playerLives_[victim->getPlayer()]=playerLives_[victim->getPlayer()]-1;
        if (playerLives_[victim->getPlayer()]<=0)//if player lost all lives
        {
            this->playersAlive--;
            const std::string& message = victim->getPlayer()->getName() + " has lost all lives";
            COUT(0) << message << std::endl;
            Host::Broadcast(message);
        }
        return true;
    }

    void LastManStanding::start()
    {
        Gametype::start();

        std::string message("Try to survive!");
        COUT(0) << message << std::endl;
        Host::Broadcast(message);
    }

    void LastManStanding::end()
    {
        Gametype::end();
        
        for (std::map<PlayerInfo*, int>::iterator it = this->playerLives_.begin(); it != this->playerLives_.end(); ++it)
        {
            if (it->first->getClientID() == CLIENTID_UNKNOWN)
                continue;

            if (it->second > 0)
                this->gtinfo_->sendAnnounceMessage("You have won the match!", it->first->getClientID());
            else
                this->gtinfo_->sendAnnounceMessage("You have lost the match!", it->first->getClientID());
        }
    }

    void LastManStanding::playerEntered(PlayerInfo* player)
    {
        if (!player)// only for safety
            return;
        Gametype::playerEntered(player);

        playerLives_[player]=lives;
        this->playersAlive++;
        this->timeToAct_[player]=timeRemaining;
        const std::string& message = player->getName() + " entered the game";
        COUT(0) << message << std::endl;
        Host::Broadcast(message);
    }

    bool LastManStanding::playerLeft(PlayerInfo* player)
    {
        bool valid_player = Gametype::playerLeft(player);

        if (valid_player)
        {
            this->playersAlive--;
            //this->playerLives_[player].erase (player); not necessary?
            //
            const std::string& message = player->getName() + " left the game";
            COUT(0) << message << std::endl;
            Host::Broadcast(message);
        }

        return valid_player;
    }

    bool LastManStanding::playerChangedName(PlayerInfo* player)
    {
        bool valid_player = Gametype::playerChangedName(player);

        if (valid_player)
        {
            const std::string& message = player->getOldName() + " changed name to " + player->getName();
            COUT(0) << message << std::endl;
            Host::Broadcast(message);
        }

        return valid_player;
    }

    void LastManStanding::pawnKilled(Pawn* victim, Pawn* killer)
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

        Gametype::pawnKilled(victim, killer);
    }

    const int LastManStanding::playerGetLives(PlayerInfo* player)
    {
        if (player)
            return  playerLives_[player];
        else
            return 0;
    }

    void LastManStanding::killPlayer(PlayerInfo* player)
    {
        if(!player)
            return;
        std::map<PlayerInfo*, Player>::iterator it = this->players_.find(player);//!!!!!!!!!!!
        if (it != this->players_.end())
        {
            it->second.state_ = PlayerState::Dead;//-------------killpart
            it->second.killed_++;

            playerLives_[player]=playerLives_[player]-1;//-----------datapart
            if (playerLives_[player]<=0)//if player lost all lives
            {
                this->playersAlive--;
                const std::string& message = player->getName() + " is out";
                COUT(0) << message << std::endl;
                Host::Broadcast(message);
            }
            this->timeToAct_[player]=timeRemaining+3.0f;//reset timer
        }
    }
    

    void LastManStanding::tick(float dt)
    {
        SUPER(LastManStanding, tick, dt);
        if(!this->hasEnded())
        {
            if ((this->hasStarted()&&(playersAlive==1)))//last player remaining
            {
            this->end();
            }
            for (std::map<PlayerInfo*, float>::iterator it = this->timeToAct_.begin(); it != this->timeToAct_.end(); ++it)
            {        
                it->second-=dt;
                if (it->second<0.0f)
                {
                    it->second=timeRemaining+3.0f;//reset timer
                    if (playerGetLives(it->first)>0)
                        this->killPlayer(it->first);
                }
            }
        }
    }

}

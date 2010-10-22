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
#include "util/Convert.h"

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
        this->setHUDTemplate("LastmanstandingHUD");
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
        //Update: EachPlayer's "Players in Game"-HUD
        for (std::map<PlayerInfo*, Player>::iterator it = this->players_.begin(); it != this->players_.end(); ++it)
        {
            if (it->first->getClientID() == CLIENTID_UNKNOWN)
                continue;
            const std::string& message1 = "Remaining Players: "+ multi_cast<std::string>(playersAlive);
            this->gtinfo_->sendStaticMessage(message1,it->first->getClientID(),ColourValue(1.0f, 1.0f, 0.5f));
        }
        
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
            //Update: EachPlayer's "Players in Game"-HUD
            for (std::map<PlayerInfo*, Player>::iterator it = this->players_.begin(); it != this->players_.end(); ++it)
            {
                if (it->first->getClientID() == CLIENTID_UNKNOWN)
                    continue;
                const std::string& message1 = "Remaining Players: "+ multi_cast<std::string>(playersAlive);
                this->gtinfo_->sendStaticMessage(message1,it->first->getClientID(),ColourValue(1.0f, 1.0f, 0.5f));
            }
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

    void LastManStanding::playerStartsControllingPawn(PlayerInfo* player, Pawn* pawn)
    {
        if (!player)
            return;
        //Update: Individual Players "lifes"-HUD
        std::map<PlayerInfo*, Player>::iterator it2 = this->players_.find(player);
        if (it2 != this->players_.end())
        {
            const std::string& message = "Your Lives: " +multi_cast<std::string>(playerLives_[player]);
            this->gtinfo_->sendFadingMessage(message,it2->first->getClientID());

        }
    }

    void LastManStanding::playerStopsControllingPawn(PlayerInfo* player, Pawn* pawn)
    {
        //Update: EachPlayer's "Players in Game"-HUD
        for (std::map<PlayerInfo*, Player>::iterator it = this->players_.begin(); it != this->players_.end(); ++it)
        {
            if (it->first->getClientID() == CLIENTID_UNKNOWN)
                continue;
            const std::string& message1 = "Remaining Players : "+ multi_cast<std::string>(playersAlive);
            this->gtinfo_->sendStaticMessage(message1,it->first->getClientID(),ColourValue(1.0f, 1.0f, 0.5f));
        }
    
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
    /*BUG-Description:
    *There are two ways for a player to be killed: Either receiving damage, or through the following function.
    *The function works fine - until the last call, when a player looses his last live. Than the kill part
    *(it->second.state_ = PlayerState::Dead;) somehow isn't executed:
    *The player isn't killed (he doesn't leave play). Although the corresponding code is reached.
    *
    *How to reproduce this bug: Start a new Lastmanstanding-Match. Immdiately add 8 bots(before actually entering the level).
    *Just fly around and wait. Don't shoot, since only passive behaviour triggers the killPlayer-Function.
    */
    void LastManStanding::killPlayer(PlayerInfo* player)
    {
        if(!player)
            return;
        std::map<PlayerInfo*, Player>::iterator it = this->players_.find(player);
        if (it != this->players_.end())
        {
            if (playerLives_[player]<=1)//if player lost all lives
            {
                this->playersAlive--;
                const std::string& message = player->getName() + " is out";
                COUT(0) << message << std::endl;
                Host::Broadcast(message);
                playerLives_[player]=playerLives_[player]-1;//-----------datapart
                it->second.killed_++;
                it->second.state_ = PlayerState::Dead;//-------------killpart
            }
            else
            {
                playerLives_[player]=playerLives_[player]-1;//-----------datapart
                it->second.killed_++;
                it->second.state_ = PlayerState::Dead;//-------------killpart

                this->timeToAct_[player]=timeRemaining+3.0f;//reset timer
            }

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

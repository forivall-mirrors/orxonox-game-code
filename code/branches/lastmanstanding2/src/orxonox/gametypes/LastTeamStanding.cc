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

#include "LastTeamStanding.h"

#include "core/CoreIncludes.h"
#include "network/Host.h"
#include "infos/PlayerInfo.h"
#include "worldentities/pawns/Pawn.h"
#include "core/ConfigValueIncludes.h"
#include "util/Convert.h"
//TODO: teamsAlive instead of playersAlive

namespace orxonox
{
    CreateUnloadableFactory(LastTeamStanding);

    LastTeamStanding::LastTeamStanding(BaseObject* creator) : TeamDeathmatch(creator)
    {
        RegisterObject(LastTeamStanding);
        this->bForceSpawn_=true;
        this->lives=4;
        this->teamsAlive=0;
        this->timeRemaining=15.0f;
        this->respawnDelay=4.0f;
        this->bNoPunishment=false;
        this->bHardPunishment=false;
        this->punishDamageRate=0.4f;
        this->setHUDTemplate("LastTeamStandingHUD");
        this->eachTeamsPlayers = new int[teams_];
    }
    
    void LastTeamStanding::~LastTeamStanding()
    {
        delete[] this->eachTeamsPlayers;
    }   

    void LastTeamStanding::spawnDeadPlayersIfRequested()
    {
        for (std::map<PlayerInfo*, Player>::iterator it = this->players_.begin(); it != this->players_.end(); ++it)
            if (it->second.state_ == PlayerState::Dead)
            {
                bool alive = (0<playerLives_[it->first]&&(inGame_[it->first]));
                if (alive&&(it->first->isReadyToSpawn() || this->bForceSpawn_))
                {
                    this->spawnPlayer(it->first);
                }
            }
    }


    void LastTeamStanding::setConfigValues()
    {
        SetConfigValue(lives, 4);
        SetConfigValue(timeRemaining, 15.0f);
        SetConfigValue(respawnDelay, 4.0f);
        SetConfigValue(bNoPunishment, false);
        SetConfigValue(bHardPunishment, false);
    }

    bool LastTeamStanding::allowPawnDamage(Pawn* victim, Pawn* originator)
    {
        bool allow = TeamDeathmatch::allowPawnDamage(victim, originator);
        if(!allow) {return allow;}
        if (originator && originator->getPlayer())// only for safety
        {
            this->timeToAct_[originator->getPlayer()]=timeRemaining;

            std::map<PlayerInfo*, Player>::iterator it = this->players_.find(originator->getPlayer());
            if (it != this->players_.end())
            {
                if (it->first->getClientID()== CLIENTID_UNKNOWN)
                    return true;
                const std::string& message = ""; // resets Camper-Warning-message
                this->gtinfo_->sendFadingMessage(message,it->first->getClientID());
            }    
        }
        return allow;
    }

    bool LastTeamStanding::allowPawnDeath(Pawn* victim, Pawn* originator)
    {
        if (!victim||!victim->getPlayer())// only for safety
            return true;
        bool allow =TeamDeathmatch::allowPawnDeath(victim, originator);
        if(!allow) {return allow;}
        playerLives_[victim->getPlayer()]=playerLives_[victim->getPlayer()]-1;
        this->inGame_[victim->getPlayer()]=false;//if player dies he, isn't allowed to respawn immediately
        if (playerLives_[victim->getPlayer()]<=0)//if player lost all lives
        {
            this->eachTeamsPlayers[getTeam(victim->getPlayer())]--;
            if(eachTeamsPlayers[getTeam(victim->getPlayer())]==0)//last team member died
                this->teamsAlive--;
            const std::string& message = victim->getPlayer()->getName() + " has lost all lives";
            COUT(0) << message << std::endl;
            Host::Broadcast(message);
        }

        return allow;
    }

    int LastTeamStanding::getMinLives()
    {
        int min=lives;
        for (std::map<PlayerInfo*, int>::iterator it = this->playerLives_.begin(); it != this->playerLives_.end(); ++it)
        {
            if (it->second<=0)
                continue;
            if (it->second<lives)
                min=it->second;
        }
        return min;
    }

    void LastTeamStanding::end()
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

    int LastTeamStanding::playerGetLives(PlayerInfo* player)
    {
        if (player)
            return  playerLives_[player];
        else
            return 0;
    }
    
    int LastTeamStanding::getNumTeamsAlive() const
    {
        return this->teamsAlive;
    }

    void LastTeamStanding::playerEntered(PlayerInfo* player)
    {
        if (!player)// only for safety
            return;
        Deathmatch::playerEntered(player);
        if (teamsAlive<=1)
            playerLives_[player]=lives;
        else
            playerLives_[player]=getMinLives();//new players only get minimum of lives
            
        if(this->eachTeamsPlayers[getTeam(player)]==0)
            this->teamsAlive++;
        this->eachTeamsPlayers[getTeam(player)]++;
        
        this->timeToAct_[player]=timeRemaining;
        this->playerDelayTime_[player]=respawnDelay;
        this->inGame_[player]=true;
    }

    bool LastTeamStanding::playerLeft(PlayerInfo* player)
    {
        bool valid_player = Deathmatch::playerLeft(player);
        if (valid_player)
        {
            this->eachTeamsPlayers[getTeam(player)]--;
            if(this->eachTeamsPlayers[getTeam(player)]==0)
                 this->teamsAlive--;
            this->playerLives_.erase (player);
            this->playerDelayTime_.erase (player);
            this->inGame_.erase (player);
            this->timeToAct_.erase(player);
        }

        return valid_player;
    }

    void LastTeamStanding::playerStartsControllingPawn(PlayerInfo* player, Pawn* pawn)
    {
        if (!player)
            return;
        TeamDeathmatch::playerStartsControllingPawn(player,pawn);
        
        this->timeToAct_[player]=timeRemaining+3.0f+respawnDelay;//reset timer
        this->playerDelayTime_[player]=respawnDelay;
        
        std::map<PlayerInfo*, Player>::iterator it = this->players_.find(player);
        if (it != this->players_.end())
        {
            if (it->first->getClientID()== CLIENTID_UNKNOWN)
                return;
            const std::string& message = ""; // resets Camper-Warning-message
            this->gtinfo_->sendFadingMessage(message,it->first->getClientID());
        }  
    }

    void LastTeamStanding::punishPlayer(PlayerInfo* player)
    {
        if(!player)
            return;
        if(bNoPunishment)
            return;
        std::map<PlayerInfo*, Player>::iterator it = this->players_.find(player);
        if (it != this->players_.end())
        {
            if(!player->getControllableEntity())
                return;
            Pawn* pawn = dynamic_cast<Pawn*>(player->getControllableEntity());
            if(!pawn)
                return;
            if(bHardPunishment)
            {
                pawn->kill();
                this->timeToAct_[player]=timeRemaining+3.0f+respawnDelay;//reset timer
            }
            else
            {
                float damage=pawn->getMaxHealth()*punishDamageRate*0.5;//TODO: Factor 0.5 is hard coded. Where is the ratio between MaxHealth actually defined?
                pawn->removeHealth(damage);
                this->timeToAct_[player]=timeRemaining;//reset timer
            }
        }
    }

    void LastTeamStanding::tick(float dt)
    {
        SUPER(LastTeamStanding, tick, dt);
        if(this->hasStarted()&&(!this->hasEnded()))
        {
            if ((this->hasStarted()&&(teamsAlive<=1)))//last team remaining
            {
                this->end();
            }
            for (std::map<PlayerInfo*, float>::iterator it = this->timeToAct_.begin(); it != this->timeToAct_.end(); ++it)
            {   
                if (playerGetLives(it->first)<=0)//Players without lives shouldn't be affected by time.
                    continue;     
                it->second-=dt;//Decreases punishment time.
                if (!inGame_[it->first])//Manages respawn delay - player is forced to respawn after the delaytime is used up. 
                {
                    playerDelayTime_[it->first]-=dt;
                    if (playerDelayTime_[it->first]<=0)
                    this->inGame_[it->first]=true;

                    if (it->first->getClientID()== CLIENTID_UNKNOWN)
                        continue;
                    int output=1+playerDelayTime_[it->first];
                    const std::string& message = "Respawn in " +multi_cast<std::string>(output)+ " seconds." ;//Countdown
                    this->gtinfo_->sendFadingMessage(message,it->first->getClientID());
                }
                else if (it->second<0.0f)
                {
                    it->second=timeRemaining+3.0f;//reset punishment-timer
                    if (playerGetLives(it->first)>0)
                    {
                        this->punishPlayer(it->first);
                        if (it->first->getClientID()== CLIENTID_UNKNOWN)
                            return;
                        const std::string& message = ""; // resets Camper-Warning-message
                        this->gtinfo_->sendFadingMessage(message,it->first->getClientID());
                    }
                }
                else if (it->second<timeRemaining/5)//Warning message
                {
                    if (it->first->getClientID()== CLIENTID_UNKNOWN)
                        continue;
                    const std::string& message = "Camper Warning! Don't forget to shoot.";
                    this->gtinfo_->sendFadingMessage(message,it->first->getClientID());
                }
            }
        }
    }

}

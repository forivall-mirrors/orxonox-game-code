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
 *     Mauro Salomon
 *   Co-authors:
 *      ...
 *
 */

#include "SpaceRace.h"


#include "items/Engine.h"

#include "core/CoreIncludes.h"
#include "chat/ChatManager.h"
#include "util/Convert.h"
#include "util/Math.h"

#include "items/Engine.h"

namespace orxonox
{
    CreateUnloadableFactory(SpaceRace);

    SpaceRace::SpaceRace(BaseObject* creator) : Gametype(creator)
    {
        RegisterObject(SpaceRace);
        
        this->bTimeIsUp_ = false;
        this->numberOfBots_ = 0;
        this->cantMove_=false;
        
    }
       
    
  // void SpaceRace::SetConfigValues(){
    //SUPER(Gametype,setConfigValues);
    //this->Gametype::SetConfigValue(initialStartCountdown_, 3.0f);}

    void SpaceRace::end()
    {
        this->Gametype::end();

        if (this->bTimeIsUp_)
        {
            this->clock_.capture();
            int s = this->clock_.getSeconds();
            int ms = static_cast<int>(this->clock_.getMilliseconds()-1000*s);
            const std::string& message = multi_cast<std::string>(s) + "." + multi_cast<std::string>(ms) + " seconds !!\n"
                        + "You didn't reach the check point  before the time limit. You lose!";
            const_cast<GametypeInfo*>(this->getGametypeInfo())->sendAnnounceMessage(message);
            ChatManager::message(message);
        }
        else
        {
            this->clock_.capture();
            int s = this->clock_.getSeconds();
            int ms = static_cast<int>(this->clock_.getMilliseconds()-1000*s);
            const std::string& message = "You win!! You have reached the last check point after "+ multi_cast<std::string>(s)
                        + "." + multi_cast<std::string>(ms) + " seconds.";
            const_cast<GametypeInfo*>(this->getGametypeInfo())->sendAnnounceMessage(message);
            ChatManager::message(message);

            float time = this->clock_.getSecondsPrecise();
            this->scores_.insert(time);
            std::set<float>::iterator it;
            

        }
    }

    void SpaceRace::start()
    {

        this->spawnPlayersIfRequested();
        Gametype::checkStart(); 
        this->cantMove_=true; 
        
        for(ObjectList<Engine>::iterator it = ObjectList<Engine>::begin(); it; ++it) 
        {
            it->setActive(false);
            
        } 
        this->addBots(this->numberOfBots_); 
    }
    
    void SpaceRace::tick(float dt)
    {
        SUPER(SpaceRace,tick,dt);
    
        if(!this->isStartCountdownRunning() && this->cantMove_)
        {
            for(ObjectList<Engine>::iterator it = ObjectList<Engine>::begin(); it; ++it) 
            { 
                it->setActive(true);
                
            }
            this->cantMove_= false;
            
            std::string message("The match has started! Reach the check points as quickly as possible!");
            const_cast<GametypeInfo*>(this->getGametypeInfo())->sendAnnounceMessage(message);
            ChatManager::message(message);            
        }
   
    }

    
    
    void SpaceRace::newCheckpointReached(SpaceRaceManager* p, int index,PlayerInfo* pl)
    {
        this->checkpointReached_[pl]=index;
        this->clock_.capture();
        int s = this->clock_.getSeconds();
        int ms = static_cast<int>(this->clock_.getMilliseconds()-1000*s);
        const std::string& message = "Checkpoint " + multi_cast<std::string>(index)
            + " reached after " + multi_cast<std::string>(s) + "." + multi_cast<std::string>(ms)
            + " seconds.";// Message is too long for a normal screen.
        const_cast<GametypeInfo*>(this->getGametypeInfo())->sendAnnounceMessage(message);
        ChatManager::message(message);
    }
   
    void SpaceRace::newCheckpointReached(RaceCheckPoint* p, PlayerInfo* pl)
    {   
        int index = p->getCheckpointIndex();
        this->checkpointReached_[pl]=index;
        this->clock_.capture();
        int s = this->clock_.getSeconds();
        int ms = static_cast<int>(this->clock_.getMilliseconds()-1000*s);
        const std::string& message = "Checkpoint " + multi_cast<std::string>(index)
            + " reached after " + multi_cast<std::string>(s) + "." + multi_cast<std::string>(ms)
            + " seconds.";
        const_cast<GametypeInfo*>(this->getGametypeInfo())->sendAnnounceMessage(message);
        ChatManager::message(message);
    }
        
    
    void SpaceRace::playerEntered(PlayerInfo* player)
    {
        Gametype::playerEntered(player);
    
        this->checkpointReached_[player]=-1;
        //this->playersAlive_++;
    }
    
    bool SpaceRace::playerLeft(PlayerInfo* player)
    {
        return Gametype::playerLeft(player);
        // bool valid_player = true;
        //if (valid_player)
       // {
        //    this->playersAlive_--;
        //}

       // return valid_player;
    }
    
    bool SpaceRace::allowPawnHit(Pawn* victim, Pawn* originator)
    {
        return false;
    }

    bool SpaceRace::allowPawnDamage(Pawn* victim, Pawn* originator)
    {
        return false;
    }

    bool SpaceRace::allowPawnDeath(Pawn* victim, Pawn* originator)
    {
        return false;
    }
}

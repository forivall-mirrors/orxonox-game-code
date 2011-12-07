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

#include "SpaceRaceManager.h"
#include "SpaceRace.h"
#include "infos/PlayerInfo.h"

#include "core/XMLPort.h"

#include "core/CoreIncludes.h"

#include "util/Convert.h"
#include "util/Math.h"

namespace orxonox
{
    CreateFactory(SpaceRaceManager);

    SpaceRaceManager::SpaceRaceManager(BaseObject* creator) : BaseObject(creator)
    {
        RegisterObject(SpaceRaceManager);
         
         for (size_t i = 0; i < this->checkpoints_.size(); ++i)
                this->setNext(this->checkpoints_[i]);
          
    }

   SpaceRaceManager::~SpaceRaceManager()
    {
        if (this->isInitialized())
        {
            for (size_t i = 0; i < this->checkpoints_.size(); ++i)
                this->checkpoints_[i]->destroy();
        }
    }
    
     void SpaceRaceManager::addCheckpoint(RaceCheckPoint* checkpoint)
    {
        this->checkpoints_.push_back(checkpoint);
    }

    RaceCheckPoint* SpaceRaceManager::getCheckpoint(unsigned int index) const
    {
        if (index < this->checkpoints_.size())
            return this->checkpoints_[index];
        else
            return 0;
    }
    
    int SpaceRaceManager::getIndex(RaceCheckPoint* r) 
    {
        
        	 for (size_t i = 0; i < this->checkpoints_.size(); ++i)
                if (this->checkpoints_[i]==r){return i;}
           
          
        
            return -1;
    }
    
     void SpaceRaceManager::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(SpaceRaceManager, XMLPort, xmlelement, mode);

        //XMLPortParam(WaypointController, "accuracy", setAccuracy, getAccuracy, xmlelement, mode).defaultValues(100.0f);
        XMLPortObject(SpaceRaceManager, RaceCheckPoint, "checkpoints", addCheckpoint, getCheckpoint,  xmlelement, mode);
    }
    
    void SpaceRaceManager::tick(float dt)
    {
    SUPER(SpaceRaceManager,tick,dt);
     SpaceRace* gametype = orxonox_cast<SpaceRace*>(this->getGametype().get());
     gametype->setV(this);
         
         
    	 for (size_t i = 0; i < this->checkpoints_.size(); ++i){
                if(this->checkpoints_[i]->reached_!=NULL)
                this->checkpointReached(this->checkpoints_[i],this->checkpoints_[i]->reached_);
          
     }
    }
    
    
    void SpaceRaceManager::checkpointReached(RaceCheckPoint* check, PlayerInfo* player){
    	 SpaceRace* gametype = orxonox_cast<SpaceRace*>(this->getGametype().get());
        assert(gametype);
        
        //if(gametype->getCheckpointReached(player)==-1) {orxout()<<"index -1"<<endl;}
   
        	 bool b =false;	
        	//DistanceMultiTrigger::fire(bIsTriggered,player);
        	int index=gametype->getCheckpointReached(player);
        	if (index>-1){
        	RaceCheckPoint* tmp= this->getCheckpoint(index);
        	Vector3 v= tmp->getNextcheckpoint();
       
       // orxout()<<"index not -1"<<endl;
        if (this->getCheckpoint(v.x)==check){
        	b=true;
        }	
       
       if (this->getCheckpoint(v.y)==check){
        	b=true;
        }	
       if (this->getCheckpoint(v.z)==check){
        	b=true;
        }	}
        else{b=(this->getIndex(check)==0);}
        	
        if (gametype && b)
        {
            gametype->clock_.capture();
            float time = gametype->clock_.getSecondsPrecise();
            if (check->bTimeLimit_!=0 && time > check->bTimeLimit_)
            {
                gametype->timeIsUp();
                gametype->end();
            }
            else if (check->getLast())
                gametype->end();
            else
            {
                gametype->newCheckpointReached(check,player);
                check->setRadarObjectColour(ColourValue::Green); //sets the radar colour of the checkpoint to green if it is reached, else it is red.
            }
        }
    	check->reached_=NULL;
    }
    
    
    
    void SpaceRaceManager::setNext(RaceCheckPoint* check){
    	
    	Vector3 v=check->getNextcheckpoint();
    		check->setNext(0,this->getCheckpoint(v.x));
    	check->setNext(1,this->getCheckpoint(v.y));
    	check->setNext(2,this->getCheckpoint(v.z));
    	}
    
    
    }




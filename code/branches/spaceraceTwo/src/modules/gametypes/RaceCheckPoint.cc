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
 *      Mauro Salomon
 *   Co-authors:
 *      ...
 *
 */

#include "RaceCheckPoint.h"

#include "util/Convert.h"
#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "chat/ChatManager.h"

#include "SpaceRace.h"

namespace orxonox
{
    CreateFactory(RaceCheckPoint);
    
     

    RaceCheckPoint::RaceCheckPoint(BaseObject* creator): DistanceMultiTrigger(creator), RadarViewable(creator, static_cast<WorldEntity*>(this))
    {
        RegisterObject(RaceCheckPoint);
        this->setDistance(100);
        this->setBeaconMode("off");
        this->setBroadcast(false);
        this->setSimultaneousTriggerers(100);
	
           
        this->bCheckpointIndex_ = 0;
        //this->bIsLast_ = false;
        this->bTimeLimit_ = 0;
        this->isVisible_=true;

        this->setRadarObjectColour(ColourValue::Blue);
        this->setRadarObjectShape(RadarViewable::Triangle);
        this->setRadarVisibility(true);
    
    //this->addTarget("WorldEntity");
    
    
    }
    

   RaceCheckPoint::~RaceCheckPoint()
    {
    
    	 if (this->isInitialized())
        {
            //for (size_t i = 0; i < 3; ++i)
            //   this->nextcheckpoints_[i]->destroy();
        }
       //nextcheckpoints_.destroy;
    }

    void RaceCheckPoint::tick(float dt)
    {
        SUPER(RaceCheckPoint, tick, dt);

        SpaceRace* gametype = orxonox_cast<SpaceRace*>(this->getGametype().get());
        assert(gametype);
        if(this->isVisible_){this->setRadarVisibility(true);}
        else{this->setRadarVisibility(false);}
        
      
      
       
        /*this->setRadarVisibility(false);
        Vector3 v =Vector3(0,0,0);
        int j=0;
        for (std::map<PlayerInfo*, Player>::iterator it = gametype->players_.begin(); it != gametype->players_.end(); ++it)
        {
        	j=gametype->getCheckpointReached(it->first);
        	RaceCheckPoint* r=SpaceRaceManager::getCheckpoint(j);
        	v=r->getNextcheckpoint();
        	for(int i=1;i<4;i++){
        	if (this->getCheckpointIndex() == v[i])
           	 this->setRadarVisibility(true);
           	 }*/
        //}     
    }

    void RaceCheckPoint::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(RaceCheckPoint, XMLPort, xmlelement, mode);
	Vector3 v= Vector3(0,0,0);
        XMLPortParam(RaceCheckPoint, "checkpointindex", setCheckpointIndex, getCheckpointIndex, xmlelement, mode).defaultValues(0);
        XMLPortParam(RaceCheckPoint, "islast", setLast, getLast, xmlelement, mode).defaultValues(false);
        XMLPortParam(RaceCheckPoint, "timelimit", setTimelimit, getTimeLimit, xmlelement, mode).defaultValues(0);
    XMLPortParamTemplate(RaceCheckPoint, "nextcheckpoints", setNextcheckpoint, getNextcheckpoint, xmlelement, mode,const Vector3&).defaultValues(v);
    }

	void RaceCheckPoint::fire(bool bIsTriggered,BaseObject* player)
    {
    	
    	//bool b= bIsTriggered;
    	//PlayerInfo* pl= player;
    	DistanceMultiTrigger::fire((bool)bIsTriggered,player);
        
   //SUPER(RaceCheckPoint,fire,bIsTriggered,player);
		

        SpaceRace* gametype = orxonox_cast<SpaceRace*>(this->getGametype().get());
        assert(gametype);
        
        	 PlayerInfo* player2 = (PlayerInfo*)player;
        assert(player2);
        	//DistanceMultiTrigger::fire(bIsTriggered,player);
        	
        	
        	
       
        	
        if (gametype && this->getCheckpointIndex() == gametype->getCheckpointReached(player2) && bIsTriggered)
        {
            gametype->clock_.capture();
            float time = gametype->clock_.getSecondsPrecise();
            if (this->bTimeLimit_!=0 && time > this->bTimeLimit_)
            {
                gametype->timeIsUp();
                gametype->end();
            }
            else if (this->getLast())
                gametype->end();
            else
            {
                gametype->newCheckpointReached(this,player2);
                this->setRadarObjectColour(ColourValue::Green); //sets the radar colour of the checkpoint to green if it is reached, else it is red.
            }
        }
    }

    void RaceCheckPoint::setTimelimit(float timeLimit)
    {
        this->bTimeLimit_ = timeLimit;
        if (this->bTimeLimit_ != 0)
        {
            SpaceRace* gametype = orxonox_cast<SpaceRace*>(this->getGametype().get());
            assert(gametype);
            if (gametype)
            {
                const std::string& message =  "You have " + multi_cast<std::string>(this->bTimeLimit_)
                            + " seconds to reach the check point " + multi_cast<std::string>(this->bCheckpointIndex_+1);
                const_cast<GametypeInfo*>(gametype->getGametypeInfo())->sendAnnounceMessage(message);
                ChatManager::message(message);
            }
        }
    }

}

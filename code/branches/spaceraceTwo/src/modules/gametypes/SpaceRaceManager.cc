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
 *     Celine Eggenberger
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
         
        this->firstcheckpointvisible_=false;
         
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
            if (this->checkpoints_[i]==r) {return i;}
           
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
     
        if(this->checkpoints_[0] != NULL && !this->firstcheckpointvisible_)
        {
            this->checkpoints_[0]->setRadarVisibility(true);
            this->firstcheckpointvisible_=false;
        }
         
        for (size_t i = 0; i < this->checkpoints_.size(); ++i)
        {
            if(this->checkpoints_[i]->reached_!=NULL)
                this->checkpointReached(this->checkpoints_[i],this->checkpoints_[i]->reached_);
        }
    }
    
    
    void SpaceRaceManager::checkpointReached(RaceCheckPoint* check, PlayerInfo* player)
    {
        SpaceRace* gametype = orxonox_cast<SpaceRace*>(this->getGametype().get());
        assert(gametype);
        
        bool b =false;    
            
        int index=gametype->getCheckpointReached(player);
        Vector3 v=Vector3 (-1,-1,-1);
        if (index>-1)
        {
            RaceCheckPoint* tmp= this->getCheckpoint(index);
            v= tmp->getNextcheckpoint();
       
            if (this->getCheckpoint(v.x) == check)
            {
                b = true;
            }    
       
            if (this->getCheckpoint(v.y) == check)
            {
                b = true;
            }    
            if (this->getCheckpoint(v.z) == check)
            {
                b = true;
            }    
        }
        else
        {
            b = (this->getIndex(check) == 0);
        }
            
        if (gametype && b)
        {
            gametype->clock_.capture();
            float time = gametype->clock_.getSecondsPrecise();
            if (check->getTimeLimit()!=0 && time > check->getTimeLimit())
            {
                gametype->timeIsUp();
                gametype->end();
            }
            else if (check->getLast())
                gametype->end();
            else
            {
                if (index > -1)this->setRadVis(player,false);
                gametype->newCheckpointReached(check,player);
                check->setRadarObjectColour(ColourValue::Green); //sets the radar colour of the checkpoint to green if it is reached, else it is red.
                
                this->setRadVis(player, true);
            }
        }
        check->reached_=NULL;
    }
    
    void SpaceRaceManager::setRadVis(PlayerInfo* player, bool b)
    {
        SpaceRace* gametype = orxonox_cast<SpaceRace*>(this->getGametype().get());
        assert(gametype);
        int index = gametype->getCheckpointReached(player);
        Vector3 v = Vector3(-1,-1,-1);
        RaceCheckPoint* tmp = this->getCheckpoint(index);
        v = tmp->getNextcheckpoint();
    
        if(v.x > -1)
        {
            this->getCheckpoint(v.x)->setRadarVisibility(b);
            this->getCheckpoint(v.x)->settingsChanged();
        }
        if(v.y > -1)
        {
            this->getCheckpoint(v.y)->setRadarVisibility(b);
            this->getCheckpoint(v.y)->settingsChanged();
        }
        if(v.z > -1)
        {
            this->getCheckpoint(v.z)->setRadarVisibility(b);
            this->getCheckpoint(v.z)->settingsChanged();
        }
    }
    
}

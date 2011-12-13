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

#include <infos/PlayerInfo.h>
#include <worldentities/ControllableEntity.h>

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
        this->bTimeLimit_ = 0;

        this->setRadarObjectColour(ColourValue::Blue);
        this->setRadarObjectShape(RadarViewable::Triangle);
        this->setRadarVisibility(false);
        this->settingsChanged();
        this->reached_=NULL;
        //this->addTarget("WorldEntity");
    }
    

   RaceCheckPoint::~RaceCheckPoint()
   {
    
   }

    void RaceCheckPoint::tick(float dt)
    {
        SUPER(RaceCheckPoint, tick, dt);

        SpaceRace* gametype = orxonox_cast<SpaceRace*>(this->getGametype().get());
        assert(gametype);
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
        DistanceMultiTrigger::fire((bool)bIsTriggered,player);
        
        SpaceRace* gametype = orxonox_cast<SpaceRace*>(this->getGametype().get());
        assert(gametype);
        ControllableEntity* entity = (ControllableEntity*) player;
     
        PlayerInfo* player2 = entity->getPlayer();
     
        if(bIsTriggered)
            this->reached_=player2;
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

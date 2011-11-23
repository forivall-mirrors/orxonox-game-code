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
         
    	
     
    }
}



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

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "SpaceRace.h"

namespace orxonox
{
    CreateFactory(RaceCheckPoint);
    
    RaceCheckPoint::RaceCheckPoint(BaseObject* creator):
	DistanceTrigger(creator),
	RadarViewable(creator, static_cast<WorldEntity*>(this))
    {
	RegisterObject(RaceCheckPoint);
	
	this->bCheckpointIndex_ = 0;
	this->bIsLast_ = false;
	
	this->setRadarObjectColour(ColourValue::Red);
        this->setRadarObjectShape(RadarViewable::Dot);
        this->setRadarVisibility(false);
    }
    
    RaceCheckPoint::~RaceCheckPoint()
    {
    }
    
    void RaceCheckPoint::tick(float dt)
    {
	Trigger::tick(dt);
	
	SpaceRace* gametype = orxonox_cast<SpaceRace*>(this->getGametype().get());
	if (this->getCheckpointIndex() == gametype->getCheckpointsReached()) this->setRadarVisibility(true);
	else  this->setRadarVisibility(false);
    }

    
    void RaceCheckPoint::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
	SUPER(RaceCheckPoint, XMLPort, xmlelement, mode);
	
	XMLPortParam(RaceCheckPoint, "checkpointindex", setCheckpointIndex, getCheckpointIndex, xmlelement, mode).defaultValues(0);
	XMLPortParam(RaceCheckPoint, "islast", setLast, getLast, xmlelement, mode).defaultValues(false);
    }
    
    void RaceCheckPoint::triggered(bool bIsTriggered)
    {
	DistanceTrigger::triggered(bIsTriggered);
	
	SpaceRace* gametype = orxonox_cast<SpaceRace*>(this->getGametype().get());
	if (gametype)
	{
	    if (this->getCheckpointIndex() == gametype->getCheckpointsReached() && bIsTriggered)
	    {
		if (this->getLast())
		{
		  gametype->end();
		}
		else
		{
		  gametype->newCheckpointReached();
		  this->setRadarObjectColour(ColourValue::Green); //sets the radar colour of the checkpoint to green if it is reached, else it is red.
		}
	    }
	}
    }
    
}

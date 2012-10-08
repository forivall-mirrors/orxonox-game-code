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

#include "OldRaceCheckPoint.h"

#include "util/Convert.h"
#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "chat/ChatManager.h"

#include "OldSpaceRace.h"

namespace orxonox
{
    CreateFactory(OldRaceCheckPoint);

    OldRaceCheckPoint::OldRaceCheckPoint(BaseObject* creator): DistanceTrigger(creator), RadarViewable(creator, static_cast<WorldEntity*>(this))
    {
        RegisterObject(OldRaceCheckPoint);

        this->bCheckpointIndex_ = 0;
        this->bIsLast_ = false;
        this->bTimeLimit_ = 0;

        this->setRadarObjectColour(ColourValue::Blue);
        this->setRadarObjectShape(RadarViewable::Triangle);
        this->setRadarVisibility(false);
    }

    OldRaceCheckPoint::~OldRaceCheckPoint()
    {
    }

    void OldRaceCheckPoint::tick(float dt)
    {
        SUPER(OldRaceCheckPoint, tick, dt);

        OldSpaceRace* gametype = orxonox_cast<OldSpaceRace*>(this->getGametype().get());
        assert(gametype);
        if (this->getCheckpointIndex() == gametype->getCheckpointsReached())
            this->setRadarVisibility(true);
        else
            this->setRadarVisibility(false);
    }

    void OldRaceCheckPoint::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(OldRaceCheckPoint, XMLPort, xmlelement, mode);

        XMLPortParam(OldRaceCheckPoint, "checkpointindex", setCheckpointIndex, getCheckpointIndex, xmlelement, mode).defaultValues(0);
        XMLPortParam(OldRaceCheckPoint, "islast", setLast, getLast, xmlelement, mode).defaultValues(false);
        XMLPortParam(OldRaceCheckPoint, "timelimit", setTimelimit, getTimeLimit, xmlelement, mode).defaultValues(0);
    }

    void OldRaceCheckPoint::triggered(bool bIsTriggered)
    {
        DistanceTrigger::triggered(bIsTriggered);

        OldSpaceRace* gametype = orxonox_cast<OldSpaceRace*>(this->getGametype().get());
        if (gametype && this->getCheckpointIndex() == gametype->getCheckpointsReached() && bIsTriggered)
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
                gametype->newCheckpointReached();
                this->setRadarObjectColour(ColourValue::Green); //sets the radar colour of the checkpoint to green if it is reached, else it is red.
            }
        }
    }

    void OldRaceCheckPoint::setTimelimit(float timeLimit)
    {
        this->bTimeLimit_ = timeLimit;
        if (this->bTimeLimit_ != 0)
        {
            OldSpaceRace* gametype = orxonox_cast<OldSpaceRace*>(this->getGametype().get());
            if (gametype)
            {
                const std::string& message =  "You have " + multi_cast<std::string>(this->bTimeLimit_)
                            + " seconds to reach the check point " + multi_cast<std::string>(this->bCheckpointIndex_+1);
                gametype->getGametypeInfo()->sendAnnounceMessage(message);
                ChatManager::message(message);
            }
        }
    }

}

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
    RegisterClass(RaceCheckPoint);

    RaceCheckPoint::RaceCheckPoint(Context* context) : DistanceMultiTrigger(context),
            RadarViewable(this, static_cast<WorldEntity*> (this))
    {
        RegisterObject(RaceCheckPoint);
        this->setDistance(100);
        this->setBeaconMode("off");
        this->setBroadcast(false);
        this->setSimultaneousTriggerers(100);

        this->setRadarObjectColour(ColourValue::Blue);
        this->setRadarObjectShape(RadarViewable::Triangle);
        this->setRadarVisibility(false);
        this->settingsChanged();

        this->checkpointIndex_ = 0;
        this->bIsLast_ = false;
        this->timeLimit_ = 0;
    }

    RaceCheckPoint::~RaceCheckPoint()
    {

    }

    void RaceCheckPoint::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(RaceCheckPoint, XMLPort, xmlelement, mode);

        XMLPortParam(RaceCheckPoint, "checkpointindex", setCheckpointIndex, getCheckpointIndex, xmlelement, mode).defaultValues(0);
        XMLPortParam(RaceCheckPoint, "islast", setLast, isLast, xmlelement, mode).defaultValues(false);
        XMLPortParam(RaceCheckPoint, "timelimit", setTimelimit, getTimeLimit, xmlelement, mode).defaultValues(0);
        XMLPortParam(RaceCheckPoint, "nextcheckpoints", setNextCheckpointsAsVector3, getNextCheckpointsAsVector3, xmlelement, mode);
    }

    void RaceCheckPoint::fire(bool bIsTriggered, BaseObject* originator)
    {
        DistanceMultiTrigger::fire(bIsTriggered, originator);

        if (bIsTriggered)
        {
            ControllableEntity* entity = orxonox_cast<ControllableEntity*>(originator);
            if (entity)
                this->players_.push_back(entity->getPlayer());
        }
    }

    void RaceCheckPoint::setTimelimit(float timeLimit)
    {
        this->timeLimit_ = timeLimit;
        if (this->timeLimit_ != 0)
        {
            std::string message = "You have " + multi_cast<std::string>(this->timeLimit_)
            + " seconds to reach the check point " + multi_cast<std::string>(this->checkpointIndex_ + 1);
            this->getGametype()->getGametypeInfo()->sendAnnounceMessage(message);
            ChatManager::message(message);
        }
    }

    void RaceCheckPoint::setNextCheckpointsAsVector3(const Vector3& checkpoints)
    {
        this->nextCheckpoints_.clear();

        if (checkpoints.x > -0.5)
            this->nextCheckpoints_.insert(static_cast<int>(checkpoints.x + 0.5)); // the red number has the type double and for the cast (to int) is added 0.5 so that the cast works correctly
        if (checkpoints.y > -0.5)
            this->nextCheckpoints_.insert(static_cast<int>(checkpoints.y + 0.5));
        if (checkpoints.z > -0.5)
            this->nextCheckpoints_.insert(static_cast<int>(checkpoints.z + 0.5));
    }

    PlayerInfo* RaceCheckPoint::getPlayer(unsigned int clientID) const
    {
        if (players_.size() > 0)
        {
            for (size_t i = 0; i < players_.size(); i++)
            {
                if (this->players_[i]->getClientID() == clientID)
                {
                    return players_[i];
                }
            }
        }
        return NULL;
    }

    bool RaceCheckPoint::playerWasHere(PlayerInfo* player) const
    {
        if (players_.size() > 0)
        {
            for (size_t i = 0; i < players_.size(); i++)
            {
                if (this->players_[i] == player)
                {
                    return true;
                }
            }
        }
        return false;
    }

    Vector3 RaceCheckPoint::getNextCheckpointsAsVector3()
    {
        Vector3 checkpoints(-1,-1,-1); int count=0;
        for (std::set<int>::iterator it= nextCheckpoints_.begin();it!=nextCheckpoints_.end(); it++ )
        {
            switch (count)
            {
                case 0: checkpoints.x = static_cast<Ogre::Real>(*it); break;
                case 1: checkpoints.y = static_cast<Ogre::Real>(*it); break;
                case 2: checkpoints.z = static_cast<Ogre::Real>(*it); break;
            }
            ++count;
        }
        return checkpoints;
    }

}

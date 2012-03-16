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
 *      Aurelian Jaggi
 *   Co-authors:
 *      ...
 *
 */

/**
    @file CheckPoint.cc
    @brief Implementation of the CheckPoint class.
    @ingroup NormalTrigger
*/

#include "CheckPoint.h"

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "gametypes/Asteroids.h"
#include "worldentities/pawns/Pawn.h"

namespace orxonox
{
    CreateFactory(CheckPoint);

    CheckPoint::CheckPoint(BaseObject* creator)
        : DistanceTrigger(creator)
        , RadarViewable(creator, static_cast<WorldEntity*>(this))
    {
        RegisterObject(CheckPoint);

        this->setStayActive(true);
        this->setDistance(50);
        this->bIsFirst_ = false;
        this->bIsDestination_ = false;

        this->setRadarObjectColour(ColourValue::Green);
        this->setRadarObjectShape(RadarViewable::Dot);
        this->setRadarVisibility(false);

        this->notifyMaskUpdate();
    }

    CheckPoint::~CheckPoint()
    {
    }

    void CheckPoint::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(CheckPoint, XMLPort, xmlelement, mode);

        XMLPortParam(CheckPoint, "isfirst", setFirst, getFirst, xmlelement, mode).defaultValues(false);
        XMLPortParam(CheckPoint, "isdestination", setDestination, getDestination, xmlelement, mode).defaultValues(false);
        XMLPortParam(CheckPoint, "addtime", setAddTime, getAddTime, xmlelement, mode).defaultValues(30);
    }

    void CheckPoint::changedActivity()
    {
        SUPER(CheckPoint, changedActivity);

        if (this->BaseObject::isActive())
        {
            this->setRadarVisibility(true);
        }
        else
        {
            this->setRadarVisibility(false);
        }
    }

    void CheckPoint::triggered(bool bIsTriggered)
    {
        DistanceTrigger::triggered(bIsTriggered);

        Asteroids* gametype = orxonox_cast<Asteroids*>(this->getGametype().get());
        if (gametype)
        {
            gametype->addTime(addTime_);
            this->setRadarVisibility(false);

            if (bIsTriggered)
            {
                if (bIsFirst_)
                {
                    gametype->setTimeLimit(addTime_);
                    gametype->firstCheckpointReached(true);
                }

                if (bIsDestination_)
                {
                    const_cast<GametypeInfo*>(gametype->getGametypeInfo())->sendAnnounceMessage("Congratulations - you have won the match!");
                    gametype->end();
                }

                if (!bIsFirst_ && !bIsDestination_)
                {
                    const_cast<GametypeInfo*>(gametype->getGametypeInfo())->sendAnnounceMessage("Checkpoint reached");
                }
            }
        }
    }

    void CheckPoint::notifyMaskUpdate()
    {
        this->targetMask_.exclude(Class(BaseObject));
        this->targetMask_.include(Class(Pawn));
    }
}

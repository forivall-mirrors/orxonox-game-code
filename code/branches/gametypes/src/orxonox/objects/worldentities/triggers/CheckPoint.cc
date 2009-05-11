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
 *      Aurelian
 *   Co-authors:
 *      ...
 *
 */

#include "OrxonoxStableHeaders.h"
#include "CheckPoint.h"
#include "objects/gametypes/Asteroids.h"

#include <OgreNode.h>

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"

#include "orxonox/objects/worldentities/ControllableEntity.h"

namespace orxonox
{
  CreateFactory(CheckPoint);

  CheckPoint::CheckPoint(BaseObject* creator) : DistanceTrigger(creator)
  {
    RegisterObject(CheckPoint);

    this->setStayActive(true);
    this->setDistance(50);
    this->bIsFirst_ = false;
    this->bIsDestination_ = false;
    this->setVisible(true);
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
  
  void CheckPoint::triggered(bool bIsTriggered)
  {
    DistanceTrigger::triggered(bIsTriggered);

    Asteroids* gametype = dynamic_cast<Asteroids*>(this->getGametype());
    if (gametype)
    { 
        gametype->addTime(addTime_);

        if (bIsTriggered && bIsFirst_)
        {
            gametype->firstCheckpointReached(true);
        }
     
        if (bIsTriggered && bIsDestination_)
        {
            gametype->end();
        }
     }
  }
}

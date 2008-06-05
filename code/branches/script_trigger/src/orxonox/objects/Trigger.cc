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
 *      Benjamin Knecht
 *   Co-authors:
 *      ...
 *
 */

#include "Trigger.h"

#include "core/CoreIncludes.h"

namespace orxonox
{
  CreateFactory(Trigger);

  Trigger::Trigger()
  {
    RegisterObject(Trigger);

    targetMask_.exclude(Class(BaseObject));
  }

  Trigger::~Trigger()
  {
  }

  bool Trigger::isTriggered()
  {
    return this->isTriggered(this->mode_);
  }

  bool Trigger::isTriggered(TriggerMode mode)
  {
    switch(mode)
    {
      case TM_EventTriggerAnd:
        return checkAnd();
        break;
      case TM_EventTriggerOr:
        return checkOr();
        break;
      case TM_DelayTrigger:
        return checkDelay();
        break;
      case TM_DistanceTrigger:
        return checkDistance();
        break;
      case TM_DistanceEventTriggerAnd:
        if (checkDistance() && checkAnd())
          return true;
        else
          return false;
        break;
      case TM_DistanceEventTriggerOr:
        if (checkDistance() && checkOr())
          return true;
        else
          return false;
        break;
      default:
        return false;
        break;
    }
  }

  void Trigger::addTrigger(Trigger* trig)
  {
    if (this != trig)
      this->triggers_.insert(trig);
  }

  void Trigger::addTargets(std::string targets)
  {
    Identifier* targetId = ID(targets);
    targetMask_.include(targetId);
    // trigger shouldn't react on itself or other triggers
    targetMask_.exclude(Class(Trigger), true);
  }

  bool Trigger::checkAnd()
  {
    std::set<Trigger*>::iterator it;
    for(it = this->triggers_.begin(); it != this->triggers_.end(); it++)
    {
      if(!((*it)->isTriggered()))
        return false;
    }
    return true;
  }

  bool Trigger::checkOr()
  {
    std::set<Trigger*>::iterator it;
    for(it = this->triggers_.begin(); it != this->triggers_.end(); it++)
    {
      if((*it)->isTriggered())
        return true;
    }
    return false;
  }

  bool Trigger::checkDelay()
  {
    if (triggingTime_ < actualTime_)
      return true;
    else
      return false;
  }

  bool Trigger::checkDistance()
  {
    // Iterate through all WorldEntities
    for(Iterator<WorldEntity> it = ObjectList<WorldEntity>::begin(); it; it++)
    {
      Vector3 distanceVec = it->getNode()->getWorldPosition() - this->getNode()->getWorldPosition();
      if (distanceVec.length() < radius_)
        return true;
    }
    return false;

  }

}

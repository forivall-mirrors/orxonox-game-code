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

#include "core/Debug.h"
#include <OgreBillboard.h>

#include "core/CoreIncludes.h"
#include "core/ConsoleCommand.h"

namespace orxonox
{

  ConsoleCommand(Trigger, setVisibility, AccessLevel::Debug, false).setDefaultValues(0);

  CreateFactory(Trigger);

  Trigger::Trigger()
  {
    RegisterObject(Trigger);

    targetMask_.exclude(Class(BaseObject));

    //testing
    mode_ = TM_EventTriggerAND;
    bActive_ = true;
    triggingTime_ = 100;

    debugBillboard_.setBillboardSet("Examples/Flare", ColourValue(1.0, 0.0, 0.0), 1);
    this->getNode()->attachObject(debugBillboard_.getBillboardSet());
  }

  Trigger::~Trigger()
  {
  }

  bool Trigger::isTriggered()
  {
    return this->isTriggered(this->mode_);
  }

  void Trigger::setVisibility(int bVisible)
  {
    if(bVisible)
      this->setScale(2,2,2);
    else
      this->setScale(0,0,0);
  }

  void Trigger::tick(float dt)
  {
    //COUT(0) << "Scale: " << this->getScale() << std::endl;
    if(bActive_)
    {
      //this->actualTime_ += dt;
      if(this->isTriggered())
      {
        this->debugBillboard_.getBillboardSet()->getBillboard(0)->setColour(ColourValue(0.0, 1.0, 0.0));
      }
    }
  }

  bool Trigger::isTriggered(TriggerMode mode)
  {
    if( children_.size() != 0 )
    {
      switch(mode)
      {
        case TM_EventTriggerAND:
          return checkAnd();
          break;
        case TM_EventTriggerOR:
          return checkOr();
          break;
        case TM_EventTriggerXOR:
          return checkXor();
          break;
        case TM_EventTriggerNOT:
          return checkNot();
          break;
        default:
          return false;
          break;
      }
    }
    return true;
  }

  void Trigger::XMLPort(Element& xmlelement, XMLPort::Mode mode)
  {
    WorldEntity::XMLPort(xmlelement, mode);
  }

  void Trigger::addTrigger(Trigger* trig)
  {
    if (this != trig)
      this->children_.insert(trig);
  }

  void Trigger::addTargets(std::string targets)
  {
    Identifier* targetId = ID(targets);
    targetMask_.include(targetId);
    // trigger shouldn't react on itself or other triggers
    targetMask_.exclude(Class(Trigger), true);
  }

  void Trigger::removeTargets(std::string targets)
  {
    Identifier* targetId = ID(targets);
    targetMask_.exclude(targetId);
  }

  bool Trigger::checkAnd()
  {
    std::set<Trigger*>::iterator it;
    for(it = this->children_.begin(); it != this->children_.end(); it++)
    {
      if(!((*it)->isTriggered()))
        return false;
    }
    return true;
  }

  bool Trigger::checkOr()
  {
    std::set<Trigger*>::iterator it;
    for(it = this->children_.begin(); it != this->children_.end(); it++)
    {
      if((*it)->isTriggered())
        return true;
    }
    return false;
  }

  bool Trigger::checkNot()
  {
    std::set<Trigger*>::iterator it;
    for(it = this->children_.begin(); it != this->children_.end(); it++)
    {
      if((*it)->isTriggered())
        return false;
    }
    return true;
  }

  bool Trigger::checkXor()
  {
    std::set<Trigger*>::iterator it;
    bool test = false;
    for(it = this->children_.begin(); it != this->children_.end(); it++)
    {
      if(test && (*it)->isTriggered())
        return false;
      if((*it)->isTriggered())
        test = true;
    }
    return test;
  }

  /*bool Trigger::checkDelay()
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

  }*/

}

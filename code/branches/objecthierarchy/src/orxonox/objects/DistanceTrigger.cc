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

#include "OrxonoxStableHeaders.h"
#include "DistanceTrigger.h"

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"

namespace orxonox {

  CreateFactory(DistanceTrigger);

  DistanceTrigger::DistanceTrigger()
  {
    RegisterObject(DistanceTrigger);

    targetMask_.exclude(Class(BaseObject));
  }

  DistanceTrigger::~DistanceTrigger()
  {
  }

  void DistanceTrigger::addTarget(Ogre::Node* targetNode)
  {
    this->targetSet_.insert(targetNode);
  }

  void DistanceTrigger::removeTarget(Ogre::Node* targetNode)
  {
    int returnval = this->targetSet_.erase(targetNode);
    if (returnval == 0)
    {
      COUT(2) << "Warning: Node " << targetNode << " did not exist in targetSet of trigger " << this << " !" << std::endl;
      COUT(4) << "Content of targetSet of trigger " << this << " :" << std::endl;
      std::set<Ogre::Node*>::iterator it;
      for(it = this->targetSet_.begin(); it != this->targetSet_.end(); it++)
      {
        COUT(4) << *it << std::endl;
      }
      COUT(4) << "End of targetSet of trigger " << this << std::endl;
    }
  }

  void DistanceTrigger::addTargets(std::string targets)
  {
    Identifier* targetId = ID(targets);
    targetMask_.include(targetId);
    // trigger shouldn't react on itself or other triggers
    targetMask_.exclude(Class(Trigger), true);

  }

  void DistanceTrigger::removeTargets(std::string targets)
  {
    Identifier* targetId = ID(targets);
    targetMask_.exclude(targetId);
  }

  bool DistanceTrigger::checkDistance()
  {
    // Iterate through all WorldEntities
    for(Iterator<WorldEntity> it = ObjectList<WorldEntity>::begin(); it; it++)
    {
      // check if WorldEntity is a target
      if(targetMask_.isIncluded(it->getIdentifier()))
      {
        Vector3 distanceVec = it->getNode()->getWorldPosition() - this->getNode()->getWorldPosition();
        if (distanceVec.length() < this->distance_)
        {
          return true;
        }
      }
    }
    return false;

  }

  void DistanceTrigger::setDistance(float dist)
  {
    this->distance_ = dist;
  }

  void DistanceTrigger::XMLPort(Element& xmlelement, XMLPort::Mode mode)
  {
    Trigger::XMLPort(xmlelement, mode);

    XMLPortParamLoadOnly(DistanceTrigger, "distance", setDistance, xmlelement, mode);
    XMLPortParamLoadOnly(DistanceTrigger, "target", addTargets, xmlelement, mode);
  }

  bool DistanceTrigger::isTriggered(TriggerMode mode)
  {
    if(Trigger::isTriggered(mode))
      return checkDistance();
    else
      return false;
  }
}

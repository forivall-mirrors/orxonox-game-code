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

#include <OgreNode.h>

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"

namespace orxonox
{
  CreateFactory(DistanceTrigger);

  DistanceTrigger::DistanceTrigger(BaseObject* creator) : PlayerTrigger(creator)
  {
    RegisterObject(DistanceTrigger);

    this->distance_ = 100;
    this->targetMask_.exclude(Class(BaseObject));
    this->player_ = NULL;
  }

  DistanceTrigger::~DistanceTrigger()
  {
  }

  void DistanceTrigger::XMLPort(Element& xmlelement, XMLPort::Mode mode)
  {
    SUPER(DistanceTrigger, XMLPort, xmlelement, mode);

    XMLPortParam(DistanceTrigger, "distance", setDistance, getDistance, xmlelement, mode).defaultValues(100.0f);
    XMLPortParamLoadOnly(DistanceTrigger, "target", addTargets, xmlelement, mode).defaultValues("ControllableEntity");
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
      for (it = this->targetSet_.begin(); it != this->targetSet_.end(); ++it)
      {
        COUT(4) << *it << std::endl;
      }
      COUT(4) << "End of targetSet of trigger " << this << std::endl;
    }
  }

  void DistanceTrigger::addTargets(const std::string& targets)
  {
    Identifier* targetId = ClassByString(targets);
    if (!targetId)
    {
        COUT(1) << "Error: \"" << targets << "\" is not a valid class name to include in ClassTreeMask (in " << this->getName() << ", class " << this->getIdentifier()->getName() << ")" << std::endl;
        return;
    }

    this->targetMask_.include(targetId);

    // trigger shouldn't react on itself or other triggers
    this->targetMask_.exclude(Class(Trigger), true);

    // we only want WorldEntities
    ClassTreeMask WEMask;
    WEMask.include(Class(WorldEntity));
    this->targetMask_ *= WEMask;
  }

  void DistanceTrigger::removeTargets(const std::string& targets)
  {
    Identifier* targetId = ClassByString(targets);
    this->targetMask_.exclude(targetId);
  }

  bool DistanceTrigger::checkDistance()
  {
    // Iterate through all objects
    for (ClassTreeMaskObjectIterator it = this->targetMask_.begin(); it != this->targetMask_.end(); ++it)
    {
      WorldEntity* entity = dynamic_cast<WorldEntity*>(*it);
      if (!entity)
        continue;

      Vector3 distanceVec = entity->getWorldPosition() - this->getWorldPosition();
      if (distanceVec.length() < this->distance_)
      {
        this->player_ = dynamic_cast<ControllableEntity*>(entity);
        return true;
      }
    }
    return false;

  }
  
  ControllableEntity* DistanceTrigger::getTriggeringPlayer(void) const
  {
    return this->player_;
  }

  bool DistanceTrigger::isTriggered(TriggerMode mode)
  {
    if (Trigger::isTriggered(mode))
      return checkDistance();
    else
      return false;
  }
}

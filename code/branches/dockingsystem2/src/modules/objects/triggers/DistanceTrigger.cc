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

/**
    @file DistanceTrigger.cc
    @brief Implementation of the DistanceTrigger class.
    @ingroup NormalTrigger
*/

#include "DistanceTrigger.h"

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "worldentities/pawns/Pawn.h"
#include "DistanceTriggerBeacon.h"

namespace orxonox
{
    
  /*static*/ const std::string DistanceTrigger::beaconModeOff_s = "off";
  /*static*/ const std::string DistanceTrigger::beaconModeIdentify_s = "identify";
  /*static*/ const std::string DistanceTrigger::beaconModeExlcude_s = "exclude";
    
  CreateFactory(DistanceTrigger);

  DistanceTrigger::DistanceTrigger(BaseObject* creator) : Trigger(creator), beaconMask_(NULL)
  {
    RegisterObject(DistanceTrigger);

    this->distance_ = 100;
    this->targetMask_.exclude(Class(BaseObject));
    this->targetName_ = "";
  }

  DistanceTrigger::~DistanceTrigger()
  {
    if(this->beaconMask_ != NULL)
      delete this->beaconMask_;
  }

  void DistanceTrigger::XMLPort(Element& xmlelement, XMLPort::Mode mode)
  {
    SUPER(DistanceTrigger, XMLPort, xmlelement, mode);

    XMLPortParam(DistanceTrigger, "distance", setDistance, getDistance, xmlelement, mode).defaultValues(100.0f);
    XMLPortParamLoadOnly(DistanceTrigger, "target", addTargets, xmlelement, mode).defaultValues("Pawn");
    XMLPortParam(DistanceTrigger, "beaconMode", setBeaconMode, getBeaconMode, xmlelement, mode);
    XMLPortParam(DistanceTrigger, "targetname", setTargetName, getTargetName, xmlelement, mode);
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

    //! Checks whether the target is (or is derived from) a ControllableEntity.
    Identifier* pawnId = Class(Pawn);
    Identifier* distanceTriggerBeaconId = Class(DistanceTriggerBeacon);
    if(targetId->isA(pawnId) || targetId->isA(distanceTriggerBeaconId))
    {
      this->setForPlayer(true);
    }

    if (!targetId)
    {
        COUT(1) << "Error: \"" << targets << "\" is not a valid class name to include in ClassTreeMask (in " << this->getName() << ", class " << this->getIdentifier()->getName() << ')' << std::endl;
        return;
    }

    this->targetMask_.include(targetId);

    // trigger shouldn't react on itself or other triggers
    this->targetMask_.exclude(Class(Trigger), true);

    // we only want WorldEntities
    ClassTreeMask WEMask;
    WEMask.include(Class(WorldEntity));
    this->targetMask_ *= WEMask;

    this->notifyMaskUpdate();
  }

  void DistanceTrigger::removeTargets(const std::string& targets)
  {
    Identifier* targetId = ClassByString(targets);
    this->targetMask_.exclude(targetId);
  }

  bool DistanceTrigger::checkDistance()
  {
    // Check for new objects that are in range
    ClassTreeMask targetMask = this->targetMask_;
    if(this->beaconMode_ == distanceTriggerBeaconMode::identify)
     targetMask = *this->beaconMask_;
    
    // Iterate through all objects
    for (ClassTreeMaskObjectIterator it = targetMask.begin(); it != targetMask.end(); ++it)
    {
      WorldEntity* entity = orxonox_cast<WorldEntity*>(*it);
      if (!entity)
        continue;
      
       // If the DistanceTrigger is in identify mode and the DistanceTriggerBeacon attached to the object has the wrong name we ignore it.
      if(this->beaconMode_ == distanceTriggerBeaconMode::identify)
      {
        if(entity->getName() != this->targetName_)
            continue;
        // If the object, the DistanceTriggerBeacon is attached to, is not a target of this DistanceMultiTrigger.
        else if(this->targetMask_.isExcluded(entity->getParent()->getIdentifier()))
            continue;
      }
    
      // If the DistanceTrigger is in exclude mode and the DistanceTriggerBeacon attached to the object has the right name, we ignore it.
      if(this->beaconMode_ == distanceTriggerBeaconMode::exclude)
      {
        
        const std::set<WorldEntity*> attached = entity->getAttachedObjects();
        bool found = false;
        for(std::set<WorldEntity*>::const_iterator it = attached.begin(); it != attached.end(); it++)
        {
          if((*it)->isA(ClassIdentifier<DistanceTriggerBeacon>::getIdentifier()) && static_cast<DistanceTriggerBeacon*>(*it)->getName() == this->targetName_)
          {
            found = true;
            break;
          }
        }
        if(found)
          continue;
      }

      Vector3 distanceVec = entity->getWorldPosition() - this->getWorldPosition();
      if (distanceVec.length() < this->distance_)
      {

        // If the target is a player (resp. is a, or is derived from a, Pawn) the triggeringPlayer is set to the target entity.
        if(this->isForPlayer())
        {

          // Change the entity to the parent of the DistanceTriggerBeacon (if in identify-mode), which is the entity to which the beacon is attached.
          if(this->beaconMode_ == distanceTriggerBeaconMode::identify)
            entity = entity->getParent();

          Pawn* player = orxonox_cast<Pawn*>(entity);
          this->setTriggeringPlayer(player);
        }

        return true;
      }
    }

    return false;
  }
  
  /**
  @brief
    Set the beacon mode.
  @param mode
    The mode as an enum.
  */
  void DistanceTrigger::setBeaconModeDirect(distanceTriggerBeaconMode::Value mode)
  {
    this->beaconMode_ = mode;
    if(this->beaconMode_ == distanceTriggerBeaconMode::identify && this->beaconMask_ == NULL)
    {
      this->beaconMask_ = new ClassTreeMask();
      this->beaconMask_->exclude(Class(BaseObject));
      this->beaconMask_->include(Class(DistanceTriggerBeacon));
    }
  }
    
  /**
  @brief
    Get the beacon mode.
  @return
    Returns the mode as a string.
  */
  const std::string& DistanceTrigger::getBeaconMode(void) const
  {
    switch(this->getBeaconModeDirect())
    {
      case distanceTriggerBeaconMode::off :
        return DistanceTrigger::beaconModeOff_s;
      case distanceTriggerBeaconMode::identify:
        return DistanceTrigger::beaconModeIdentify_s;
      case distanceTriggerBeaconMode::exclude:
        return DistanceTrigger::beaconModeExlcude_s;
      default :
        assert(0); // This is impossible.
        return BLANKSTRING;
    }
  }
    
  /**
  @brief
    Set the beacon mode.
  @param mode
    The mode as a string.
  */
  void DistanceTrigger::setBeaconMode(const std::string& mode)
  {
    if(mode == DistanceTrigger::beaconModeOff_s)
      this->setBeaconModeDirect(distanceTriggerBeaconMode::off);
    else if(mode == DistanceTrigger::beaconModeIdentify_s)
      this->setBeaconModeDirect(distanceTriggerBeaconMode::identify);
    else if(mode == DistanceTrigger::beaconModeExlcude_s)
      this->setBeaconModeDirect(distanceTriggerBeaconMode::exclude);
    else
      COUT(1) << "Invalid beacon mode in DistanceTrigger." << endl;
  }

  bool DistanceTrigger::isTriggered(TriggerMode::Value mode)
  {
    if (Trigger::isTriggered(mode))
    {
      return checkDistance();
    }
    else
      return false;
  }
}

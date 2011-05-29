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

    /**
    @brief
        Constructor. Registers and initializes the object.
    @param creator
        The creator of this trigger.
    */
    DistanceTrigger::DistanceTrigger(BaseObject* creator) : Trigger(creator)
    {
        RegisterObject(DistanceTrigger);

        this->distance_ = 100;
        this->targetMask_.exclude(Class(BaseObject));
        this->targetName_ = "";
        this->beaconMask_.exclude(Class(BaseObject));
        this->beaconMask_.include(Class(DistanceTriggerBeacon));
    }

    /**
    @brief
        Destructor.
    */
    DistanceTrigger::~DistanceTrigger()
    {

    }

    /**
    @brief
        Method for creating a DistanceTrigger object through XML.
    */
    void DistanceTrigger::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(DistanceTrigger, XMLPort, xmlelement, mode);

        XMLPortParam(DistanceTrigger, "distance", setDistance, getDistance, xmlelement, mode).defaultValues(100.0f);
        XMLPortParamLoadOnly(DistanceTrigger, "target", addTarget, xmlelement, mode).defaultValues("Pawn");
        XMLPortParam(DistanceTrigger, "beaconMode", setBeaconMode, getBeaconMode, xmlelement, mode);
        XMLPortParam(DistanceTrigger, "targetname", setTargetName, getTargetName, xmlelement, mode);
    }

    /**
    @brief
        Add some target to the DistanceTrigger.
    @param targetStr
        The target class name as a string.
    */
    void DistanceTrigger::addTarget(const std::string& targetStr)
    {
        Identifier* targetId = ClassByString(targetStr);

        // Checks whether the target is (or is derived from) a Pawn and if so set the PlayerTrigger aspect of this trigger to be for the player, meaning, that from this Trigger one can derive the Pawn that caused it to trigger.
        Identifier* pawnId = Class(Pawn);
        if(targetId->isA(pawnId))
            this->setForPlayer(true);

        if (targetId == NULL)
        {
            COUT(1) << "Error: \"" << targetStr << "\" is not a valid class name to include in ClassTreeMask (in " << this->getName() << ", class " << this->getIdentifier()->getName() << ')' << std::endl;
            return;
        }

        // Add the target to the target mask.
        this->targetMask_.include(targetId);

        // The DistanceTrigger shouldn't react to itself or other triggers.
        this->targetMask_.exclude(Class(TriggerBase), true);

        // We only want WorldEntities (since only they have a position)
        ClassTreeMask WEMask;
        WEMask.include(Class(WorldEntity));
        this->targetMask_ *= WEMask;

        this->notifyMaskUpdate(); // Inform interested parties that the target mask has been updated.
    }

    /**
    @brief
        Remove some target from the DistanceTrigger.
    @param targetStr
        The target class name as a string.
    */
    void DistanceTrigger::removeTarget(const std::string& targetStr)
    {
        Identifier* targetId = ClassByString(targetStr);
        this->targetMask_.exclude(targetId);
    }

    /**
    @brief
        Check, whether there are entities that are targets of this DistanceTrigger in its range.
    @return
        Returns true if there are valid entities in its range.
    */
    bool DistanceTrigger::checkDistance()
    {
        // Check whether there is a cached object, it still exists and whether it is still in range, if so nothing further needs to be done.
        if(this->cache_.get() != NULL)
        {
            if((this->cache_.get()->getWorldPosition() - this->getWorldPosition()).length() < this->distance_)
                return true;
            else
                this->cache_.reset();
        }
        
        // Check for new objects that are in range
        ClassTreeMask targetMask = this->targetMask_;
        // If we are in identify-mode another target mask has to be applies to find the DistanceTriggerBeacons.
        if(this->beaconMode_ == distanceTriggerBeaconMode::identify)
            targetMask = this->beaconMask_;

        // Iterate through all objects that are targets of the DistanceTrigger.
        for (ClassTreeMaskObjectIterator it = targetMask.begin(); it != targetMask.end(); ++it)
        {
            WorldEntity* entity = static_cast<WorldEntity*>(*it);

            // If the DistanceTrigger is in identify-mode and the DistanceTriggerBeacon attached to the object has the wrong name we ignore it.
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

            // Check if the entity is in range.
            Vector3 distanceVec = entity->getWorldPosition() - this->getWorldPosition();
            if (distanceVec.length() < this->distance_)
            {
                // If the target is a player (resp. is a, or is derived from a, Pawn) the triggeringPlayer is set to the target entity.
                if(this->isForPlayer())
                {
                    // Change the entity to the parent of the DistanceTriggerBeacon (if in identify-mode), which is the entity to which the beacon is attached.
                    if(this->beaconMode_ == distanceTriggerBeaconMode::identify)
                        entity = entity->getParent();

                    Pawn* pawn = orxonox_cast<Pawn*>(entity);
                    if(pawn != NULL)
                        this->setTriggeringPawn(pawn);
                    else
                        CCOUT(2) << "Pawn was NULL." << endl;
                }
                
                // Add the entity to the cache.
                this->cache_ = WeakPtr<WorldEntity>(entity);

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

    /**
    @brief
        Check whether the DistanceTrigger is triggered.
        It is triggered if it is triggered according only to its mode (i.e. its sub-triggers) and if a target is in range.
    @param mode
        The mode for which it is tested, whether the DistanceTrigger is triggered.
    @return
        Returns true if it is triggered ,false if not.
    */
    bool DistanceTrigger::isTriggered(TriggerMode::Value mode)
    {
        if (Trigger::isTriggered(mode))
            return checkDistance();
        else
            return false;
    }
}

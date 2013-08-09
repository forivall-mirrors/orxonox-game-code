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
 *      Damian 'Mozork' Frick
 *   Co-authors:
 *      ...
 *
*/

/**
    @file DistanceMultiTrigger.cc
    @brief Implementation of the DistanceMultiTrigger class.
    @ingroup MultiTrigger
*/

#include "DistanceMultiTrigger.h"

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"

#include "DistanceTriggerBeacon.h"

namespace orxonox
{

    /*static*/ const std::string DistanceMultiTrigger::beaconModeOff_s = "off";
    /*static*/ const std::string DistanceMultiTrigger::beaconModeIdentify_s = "identify";
    /*static*/ const std::string DistanceMultiTrigger::beaconModeExlcude_s = "exclude";
    
    CreateFactory(DistanceMultiTrigger);

    /**
    @brief
        Default Constructor. Registers the object and initializes default values.
    */
    DistanceMultiTrigger::DistanceMultiTrigger(Context* context) : MultiTrigger(context)
    {
        RegisterObject(DistanceMultiTrigger);

        this->distance_ = 100.0f;
        this->setBeaconModeDirect(distanceMultiTriggerBeaconMode::off);
        this->targetName_ = "";
        this->beaconMask_.exclude(Class(BaseObject));
        this->beaconMask_.include(Class(DistanceTriggerBeacon));
    }

    /**
    @brief
        Destructor.
    */
    DistanceMultiTrigger::~DistanceMultiTrigger()
    {

    }

    /**
    @brief
        Method for creating a DistanceMultiTrigger object through XML.
    */
    void DistanceMultiTrigger::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(DistanceMultiTrigger, XMLPort, xmlelement, mode);

        XMLPortParam(DistanceMultiTrigger, "distance", setDistance, getDistance, xmlelement, mode);
        XMLPortParam(DistanceMultiTrigger, "beaconMode", setBeaconMode, getBeaconMode, xmlelement, mode);
        XMLPortParam(DistanceMultiTrigger, "targetname", setTargetName, getTargetName, xmlelement, mode);
    }

    /**
    @brief
        This method is called by the MultiTrigger to get information about new trigger events that need to be looked at.

        In this implementation we iterate through all possible objects and check whether the fact that they are in range or not has changed and fire and hand a state ofer to the MultiTrigger if so.
    @return
        Returns a pointer to a queue of MultiTriggerState pointers, containing all the necessary information to decide whether these states should indeed become new states of the MultiTrigger.
    */
    std::queue<MultiTriggerState*>* DistanceMultiTrigger::letTrigger(void)
    {

        std::queue<MultiTriggerState*>* queue = NULL;

        // Check for objects that were in range but no longer are. Iterate through all objects, that are in range.
        for(std::map<WorldEntity*, WeakPtr<WorldEntity>* >::iterator it = this->range_.begin(); it != this->range_.end(); )
        {
            WorldEntity* entity = it->second->get();
            WorldEntity* key = it->first;
            it++; // Incrementing the iterator in advance, since we don't need the current anymore and we potentially are going to delete the current element thus invalidating the iterator.
            // If the entity no longer exists.
            if(entity == NULL)
            {
                this->removeFromRange(key);
                continue;
            }

            Vector3 distanceVec = entity->getWorldPosition() - this->getWorldPosition();
            // If the object is no longer in range.
            if (distanceVec.length() > this->distance_)
            {
                // If for some reason the entity could not be removed.
                if(!this->removeFromRange(key))
                    continue;

                // If no queue has been created, yet.
                if(queue == NULL)
                    queue = new std::queue<MultiTriggerState*>();

                // Create a state and append it to the queue.
                MultiTriggerState* state = new MultiTriggerState;
                state->bTriggered = false; // Because the entity went out of range.
                state->originator = entity;
                queue->push(state);
            }
        }

        // Check for new objects that are in range
        ClassTreeMask targetMask = this->getTargetMask();
        // If we are in identify-mode another target mask has to be applies to find the DistanceTriggerBeacons.
        if(this->beaconMode_ == distanceMultiTriggerBeaconMode::identify)
            targetMask = this->beaconMask_;

        // Iterate through all objects that are targets of the DistanceMultiTrigger.
        for(ClassTreeMaskObjectIterator it = targetMask.begin(); it != targetMask.end(); ++it)
        {
            WorldEntity* entity = static_cast<WorldEntity*>(*it);

            // If the DistanceMultiTrigger is in identify-mode and the DistanceTriggerBeacon attached to the object has the wrong name we ignore it.
            if(this->beaconMode_ == distanceMultiTriggerBeaconMode::identify)
            {
                if(entity->getName() != this->targetName_)
                    continue;
                // If the object, the DistanceTriggerBeacon is attached to, is not a target of this DistanceMultiTrigger.
                else if(this->getTargetMask().isExcluded(entity->getParent()->getIdentifier()))
                    continue;
            }
            
            // If the DistanceMultiTrigger is in exclude mode and the DistanceTriggerBeacon attached to the object has the right name, we ignore it.
            if(this->beaconMode_ == distanceMultiTriggerBeaconMode::exclude)
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
            // If the object is in range.
            if (distanceVec.length() <= this->distance_)
            {
                // Add the object to the objects that are in range.
                // Objects that already are in range are not added twice, because in a map (this->range_) each key can only exist once and thus addToRange() will reject all attempts of duplicate entries.
                if(!this->addToRange(entity))
                    continue;

                // Change the entity to the parent of the DistanceTriggerBeacon (if in identify-mode), which is the entity to which the beacon is attached.
                if(this->beaconMode_ == distanceMultiTriggerBeaconMode::identify)
                    entity = entity->getParent();

                // If no queue has been created, yet.
                if(queue == NULL)
                    queue = new std::queue<MultiTriggerState*>();

                // Create a state and append it to the queue.
                MultiTriggerState* state = new MultiTriggerState;
                state->bTriggered = true; // Because the entity came into range.
                state->originator = entity;
                queue->push(state);
            }
        }

        return queue;
    }
    
    /**
    @brief
        Set the beacon mode.
    @param mode
        The mode as an enum.
    */
    void DistanceMultiTrigger::setBeaconModeDirect(distanceMultiTriggerBeaconMode::Value mode)
    {
        this->beaconMode_ = mode;
    }
    
    /**
    @brief
        Get the beacon mode.
    @return
        Returns the mode as a string.
    */
    const std::string& DistanceMultiTrigger::getBeaconMode(void) const
    {
        switch(this->getBeaconModeDirect())
        {
            case distanceMultiTriggerBeaconMode::off :
                return DistanceMultiTrigger::beaconModeOff_s;
            case distanceMultiTriggerBeaconMode::identify:
                return DistanceMultiTrigger::beaconModeIdentify_s;
            case distanceMultiTriggerBeaconMode::exclude:
                return DistanceMultiTrigger::beaconModeExlcude_s;
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
    void DistanceMultiTrigger::setBeaconMode(const std::string& mode)
    {
        if(mode == DistanceMultiTrigger::beaconModeOff_s)
            this->setBeaconModeDirect(distanceMultiTriggerBeaconMode::off);
        else if(mode == DistanceMultiTrigger::beaconModeIdentify_s)
            this->setBeaconModeDirect(distanceMultiTriggerBeaconMode::identify);
        else if(mode == DistanceMultiTrigger::beaconModeExlcude_s)
            this->setBeaconModeDirect(distanceMultiTriggerBeaconMode::exclude);
        else
            orxout(internal_error, context::triggers) << "Invalid beacon mode in DistanceMultiTrigger." << endl;
    }

    /**
    @brief
        Add a given entity to the entities, that currently are in range of the DistanceMultiTrigger.
    @param entity
        A pointer to the entity.
    @return
        Returns true if successful, false if not.
    */
    bool DistanceMultiTrigger::addToRange(WorldEntity* entity)
    {
        WeakPtr<WorldEntity>* weakptr = new WeakPtr<WorldEntity>(entity);
        std::pair<std::map<WorldEntity*, WeakPtr<WorldEntity>* >::iterator, bool> pair = this->range_.insert(std::pair<WorldEntity*, WeakPtr<WorldEntity>* >(entity, weakptr));

        if(!pair.second)
        {
            delete weakptr;
            return false;
        }

        return true;
    }

    /**
    @brief
        Remove a given entity from the set of entities, that currently are in range of the DistanceMultiTrigger.
    @param entity
        A pointer ot the entity.
    @return
        Returns true if successful.
    */
    bool DistanceMultiTrigger::removeFromRange(WorldEntity* entity)
    {
        WeakPtr<WorldEntity>* weakptr = this->range_.find(entity)->second;
        bool erased = this->range_.erase(entity) > 0;
        if(erased)
            delete weakptr;
        return erased;
    }

}

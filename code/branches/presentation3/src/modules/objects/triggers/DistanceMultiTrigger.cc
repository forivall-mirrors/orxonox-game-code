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
*/

#include "DistanceMultiTrigger.h"

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "DistanceTriggerBeacon.h"

namespace orxonox
{
    
    CreateFactory(DistanceMultiTrigger);

    /**
    @brief
        Default Constructor. Registers the object and initializes default values. 
    */
    DistanceMultiTrigger::DistanceMultiTrigger(BaseObject* creator) : MultiTrigger(creator)
    {
        RegisterObject(DistanceMultiTrigger);
        
        this->distance_ = 100.0f;
        this->targetName_ = BLANKSTRING;
        this->singleTargetMode_ = false;
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
        XMLPortParam(DistanceMultiTrigger, "targetname", setTargetName, getTargetName, xmlelement, mode);
    }

    /**
    @brief
        This method is called by the MultiTrigger to get information about new trigger events that need to be looked at.

        In this implementation we iterate through all possible objects and check whether the fact that they are in range or not has changed and fire and hand a state ofer to the MultiTrigger if so.
    */
    std::queue<MultiTriggerState*>* DistanceMultiTrigger::letTrigger(void)
    {

        std::queue<MultiTriggerState*>* queue = NULL;

        // Check for objects that were in range but no longer are. Iterate through all objects, that are in range.
        for(std::map<WorldEntity*, WeakPtr<WorldEntity>* >::iterator it = this->range_.begin(); it != this->range_.end(); )
        {
            WorldEntity* entity = it->second->get();
            WorldEntity* key = it->first;
            if(entity == NULL)
            {
                ++it;
                this->removeFromRange(key);
                continue;
            }
            
            Vector3 distanceVec = entity->getWorldPosition() - this->getWorldPosition();
            // If the object is no longer in range.
            if (distanceVec.length() > this->distance_)
            {
                if(!this->removeFromRange(key))
                {
                    ++it;
                    continue;
                }

                // If no queue has been created, yet.
                if(queue == NULL)
                    queue = new std::queue<MultiTriggerState*>();

                // Create a state and append it to the queue.
                MultiTriggerState* state = new MultiTriggerState;
                state->bTriggered = false;
                state->originator = entity;
                queue->push(state);
            }
            else
                ++it;
        }

        ClassTreeMask& targetMask = this->getTargetMask();

        // Check for new objects that are in range
        for(ClassTreeMaskObjectIterator it = targetMask.begin(); it != targetMask.end(); ++it)
        {
            WorldEntity* entity = static_cast<WorldEntity*>(*it);
            if (entity == NULL) //If the object is no WorldEntity or is already in range.
                continue;

            // If the DistanceMultiTrigger is in single-target-mode.
            if(this->singleTargetMode_)
            {
                // If the object that is a target is no DistanceTriggerBeacon, then the DistanceMultiTrigger can't be in single-target-mode.
                if(!(*it)->isA(ClassIdentifier<DistanceTriggerBeacon>::getIdentifier()))
                    this->singleTargetMode_ = false;
                // If the target name and the name of the DistancTriggreBeacon don't match.
                else if(entity->getName().compare(this->targetName_) != 0)
                    continue;
            }

            Vector3 distanceVec = entity->getWorldPosition() - this->getWorldPosition();
            // If the object is in range.
            if (distanceVec.length() <= this->distance_)
            {
                // Add the object to the objects that are in range.
                if(!this->addToRange(entity))
                    continue;

                // Change the entity to the parent of the DistanceTriggerBeacon (if in single-target-mode), which is the entity to which the beacon is attached.
                if(this->singleTargetMode_)
                    entity = entity->getParent();
                
                // If no queue has been created, yet.
                if(queue == NULL)
                    queue = new std::queue<MultiTriggerState*>();

                // Create a state and append it to the queue.
                MultiTriggerState* state = new MultiTriggerState;
                state->bTriggered = true;
                state->originator = entity;
                queue->push(state);
            }
        }
        
        return queue;
    }
    
}

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

#include "DistanceMultiTrigger.h"

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"

namespace orxonox
{
    
    CreateFactory(DistanceMultiTrigger);
    
    DistanceMultiTrigger::DistanceMultiTrigger(BaseObject* creator) : MultiTrigger(creator)
    {
        RegisterObject(DistanceMultiTrigger);
        
        this->distance_ = 100.0f;
    }
    
    DistanceMultiTrigger::~DistanceMultiTrigger()
    {
        
    }
    
    void DistanceMultiTrigger::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(DistanceMultiTrigger, XMLPort, xmlelement, mode);

        XMLPortParam(DistanceMultiTrigger, "distance", setDistance, getDistance, xmlelement, mode).defaultValues(100.0f);
    }
    
    std::queue<MultiTriggerState*>* DistanceMultiTrigger::letTrigger(void)
    {
        ClassTreeMask targetMask = this->getTargetMask();
        
        std::queue<MultiTriggerState*>* queue = NULL;
        // Check for new objects that are in range
        for(ClassTreeMaskObjectIterator it = targetMask.begin(); it != targetMask.end(); ++it)
        {
            WorldEntity* entity = orxonox_cast<WorldEntity*>(*it);
            if (entity == NULL || this->inRange(entity)) //If the object is no WorldEntity or is already in range.
                continue;

            Vector3 distanceVec = entity->getWorldPosition() - this->getWorldPosition();
            if (distanceVec.length() < this->distance_)
            {
                if(!this->addToRange(entity))
                    continue;
                
                if(queue == NULL)
                {
                    queue = new std::queue<MultiTriggerState*>();
                }
                MultiTriggerState* state = new MultiTriggerState;
                state->bTriggered = true;
                state->originator = entity;
                queue->push(state);
            }
        }
        
        for(std::set<WorldEntity*>::iterator it = this->range_.begin(); it != this->range_.end(); )
        {
            Vector3 distanceVec = (*it)->getWorldPosition() - this->getWorldPosition();
            if (distanceVec.length() >= this->distance_)
            {
                WorldEntity* temp = *(it++);
                if(!this->removeFromRange(temp))
                    continue;
                
                if(queue == NULL)
                {
                    queue = new std::queue<MultiTriggerState*>();
                }
                MultiTriggerState* state = new MultiTriggerState;
                state->bTriggered = false;
                state->originator = temp;
                queue->push(state);
            }
            else
                ++it;
        }
        
        return queue;
    }
    
}

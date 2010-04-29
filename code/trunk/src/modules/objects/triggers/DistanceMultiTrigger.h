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

#ifndef _DistanceMultiTrigger_H__
#define _DistanceMultiTrigger_H__

#include "objects/ObjectsPrereqs.h"

#include "worldentities/WorldEntity.h"
#include <set>

#include "MultiTrigger.h"

namespace orxonox
{

    class _ObjectsExport DistanceMultiTrigger : public MultiTrigger
    {
        
        public:
            DistanceMultiTrigger(BaseObject* creator);
            ~DistanceMultiTrigger();
            
            void XMLPort(Element& xmlelement, XMLPort::Mode mode);
            
            inline void setDistance(float distance)
                { this->distance_ = distance; }
            inline float getDistance() const
                { return this->distance_; }
                
        protected:
            virtual std::queue<MultiTriggerState*>* letTrigger(void);
            
            inline bool inRange(WorldEntity* entity)
                { return this->range_.find(entity) != this->range_.end(); }
            bool addToRange(WorldEntity* entity)
                { std::pair<std::set<WorldEntity*>::iterator, bool> pair = this->range_.insert(entity); return pair.second; }
            bool removeFromRange(WorldEntity* entity)
                { return this->range_.erase(entity) > 0; }
                
        private:
            float distance_;
            std::set<WorldEntity*> range_;
        
    };
    
}

#endif // _DistanceMultiTrigger_H__

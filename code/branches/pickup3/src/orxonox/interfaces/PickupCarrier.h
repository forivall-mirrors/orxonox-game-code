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
    @file
    @brief Definition of the PickupCarrier class.
*/

#ifndef _PickupCarrier_H__
#define _PickupCarrier_H__

#include "OrxonoxPrereqs.h"
#include "core/OrxonoxClass.h"
#include "Pickupable.h"

#include <set>
#include <list>

namespace orxonox
{

    class _OrxonoxExport PickupCarrier : virtual public OrxonoxClass
    {
        friend class Pickupable;
        
        public:
            PickupCarrier();
            virtual ~PickupCarrier() {}
            
            //TODO: Secure uniqueness of each item in the set, if neccessary, check.
            inline bool pickup(Pickupable* pickup)
                {
                    bool pickedUp = this->pickups_.insert(pickup).second;
                    if(pickedUp) pickup->pickedUp(this);
                    return pickedUp;
                }
                
            inline bool drop(Pickupable* pickup)
                { 
                   bool dropped = this->pickups_.erase(pickup) == 1;
                   if(dropped)
                   {
                       pickup->dropped();
                        //TODO: Create Spawner.
                   }
                   return dropped;
                }
                
            inline bool isTarget(Pickupable* pickup)
                {
                    if(pickup->isTarget(this))
                        return true;
                    const std::list<PickupCarrier*>* children = this->getChildren();
                    for(std::list<PickupCarrier*>::const_iterator it = children->begin(); it != children->end(); it++)
                    {
                        if((*it)->isTarget(pickup))
                            return true;
                    }
                    
                    return false;
                }
            
        protected:            
            //TODO: Good return type?
            virtual const std::list<PickupCarrier*>* getChildren(void) = 0;
            virtual PickupCarrier* getParent(void) = 0;
        
        private:
            std::set<Pickupable*> pickups_;
            
        
    };
    
}

#endif /* _PickupCarrier_H__ */

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

#include <list>
#include <set>
#include "Pickupable.h"
#include "core/Identifier.h"

#include "core/OrxonoxClass.h"

namespace orxonox
{
    
    class Pickup;
    class HealthPickup;
    class MetaPickup;

    /**
    @brief
        The PickupCarrier interface provides the means, for any class implementing it, to possess Pickupables.
    @author
        Damian 'Mozork' Frick
    */
    class _OrxonoxExport PickupCarrier : virtual public OrxonoxClass
    {
        friend class Pickupable; //!< The Pickupable has full acces to its PickupCarrier.
        //TODO: Ugly workaround.
        friend class Pickup;
        friend class HealthPickup;
        friend class MetaPickup;
        
        public:
            PickupCarrier(); //!< Constructor.
            virtual ~PickupCarrier(); //!< Destructor.
            
            /**
            @brief Can be called to pick up a Pickupable.
            @param pickup A pointer to the Pickupable.
            @return Returns true if the Pickupable was picked up, false if not.
            */
            bool pickup(Pickupable* pickup)
                {
                    bool pickedUp = this->pickups_.insert(pickup).second;
                    if(pickedUp)
                    {
                        COUT(4) << "Picked up Pickupable " << pickup->getIdentifier()->getName() << "(&" << pickup << ")." << std::endl;
                        pickup->pickedUp(this);
                    }
                    return pickedUp;
                }
                
            /**
            @brief Can be called to drop a Pickupable.
            @param pickup A pointer to the Pickupable.
            @param drop If the Pickupable should just be removed from the PickupCarrier without further action, this can be set to false. true is default.
            @return Returns true if the Pickupable has been dropped, false if not.
            */
            bool drop(Pickupable* pickup, bool drop = true)
                { 
                    bool dropped = this->pickups_.erase(pickup) == 1;
                    if(dropped && drop)
                    {
                        COUT(4) << "Dropping Pickupable " << pickup->getIdentifier()->getName() << "(&" << pickup << ")." << std::endl;
                        pickup->dropped();
                    }
                    return dropped;
                }
                
            /**
            @brief Can be used to check whether the PickupCarrier or a child of his is a target ot the input Pickupable.
            @param pickup A pointer to the Pickupable.
            @return Returns true if the PickupCarrier or one of its children is a target, false if not.
            */
            //TODO: Use?
            bool isTarget(const Pickupable* pickup)
                {
                    if(pickup->isTarget(this)) //!< If the PickupCarrier itself is a target.
                        return true;
                    
                    //! Go recursively through all children to check whether they are a target.
                    std::list<PickupCarrier*>* children = this->getCarrierChildren();
                    for(std::list<PickupCarrier*>::const_iterator it = children->begin(); it != children->end(); it++)
                    {
                        if((*it)->isTarget(pickup))
                            return true;
                    }
                    
                    children->clear();
                    delete children;
                    
                    return false;
                }
                
            /**
            @brief Get the carrier that is both a child of the PickupCarrier (or the PickupCarrier itself) and a target of the input Pickupable.
            @param pickup A pounter to the Pickupable.
            @return Returns a pointer to the PickupCarrier that is the target of the input Pickupable.
            */
            PickupCarrier* getTarget(const Pickupable* pickup)
                {
                    if(!this->isTarget(pickup))
                        return NULL;
                    
                    if(pickup->isTarget(this)) //!< If the PickupCarrier itself is a target.
                        return this;
                    
                    //! Go recursively through all children to check whether they are the target.
                    std::list<PickupCarrier*>* children = this->getCarrierChildren();
                    for(std::list<PickupCarrier*>::iterator it = children->begin(); it != children->end(); it++)
                    {
                        if(pickup->isTarget(*it))
                            return *it;
                    }
                    
                    children->clear();
                    delete children;
                    
                    return NULL;
                }
            
        protected:        
            /**
            @brief Get all direct children of this PickupSpawner.
                   This method needs to be implemented by any direct derivative class of PickupCarrier.
            @return Returns a pointer to a list of all direct children. 
            */
            //TODO: Good return type? Maybe not const and destroyed in isTarget...
            virtual std::list<PickupCarrier*>* getCarrierChildren(void) = 0;
            /**
            @brief Get the parent of this PickupSpawner
                   This method needs to be implemented by any direct derivative class of PickupCarrier.
            @return Returns a pointer to the parent.
            */
            virtual PickupCarrier* getCarrierParent(void) = 0;
            /**
            @brief Get the (absolute) position of the PickupCarrier.
                   This method needs to be implemented by any direct derivative class of PickupCarrier.
            @return Returns the position as a Vector3.
            */
            virtual const Vector3& getCarrierPosition(void) = 0;
                            
            //TODO: Remove.
            std::set<Pickupable*>& getPickups(void)
                { return this->pickups_; }
        
        private:
            std::set<Pickupable*> pickups_; //!< The list of Pickupables carried by this PickupCarrier.
            
    };
}

#endif /* _PickupCarrier_H__ */

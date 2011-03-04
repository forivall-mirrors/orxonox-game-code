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
    @file PickupCarrier.cc
    @brief Implementation of the PickupCarrier class.
*/

#include "PickupCarrier.h"

#include "core/CoreIncludes.h"
#include "core/Identifier.h"

#include "Pickupable.h"

namespace orxonox {

    /**
    @brief
        Constructor. Registers the object.
    */
    PickupCarrier::PickupCarrier()
    {
        RegisterRootObject(PickupCarrier);
    }

    /**
    @brief
        Destructor.
    */
    PickupCarrier::~PickupCarrier()
    {

    }

    /**
    @brief
        Is called before the PickupCarrier is effectively destroyed.
    */
    void PickupCarrier::preDestroy(void)
    {
        std::set<Pickupable*>::iterator it = this->pickups_.begin();
        std::set<Pickupable*>::iterator temp;
        while(it != this->pickups_.end())
        {
            (*it)->carrierDestroyed();
            temp = it;
            it = this->pickups_.begin();
            if(it == temp) // Infinite loop avoidance, in case the pickup wasn't removed from the carrier somewhere in the carrierDestroy() procedure.
            {
                COUT(2) << "Oops. In a PickupCarrier, while cleaning up, a Pickupable (&" << (*temp) << ") didn't unregister itself as it should have." << std::endl;;
                it++;
            }
        }

        this->pickups_.clear();
    }

    /**
    @brief
        Can be used to check whether the PickupCarrier or a child of his is a target ot the input Pickupable.
    @param pickup
        A pointer to the Pickupable.
    @return
        Returns true if the PickupCarrier or one of its children is a target, false if not.
    */
    bool PickupCarrier::isTarget(const Pickupable* pickup) const
    {
        if(pickup->isTarget(this)) // If the PickupCarrier itself is a target.
            return true;

        // Go recursively through all children to check whether they are a target.
        std::vector<PickupCarrier*>* children = this->getCarrierChildren();
        for(std::vector<PickupCarrier*>::const_iterator it = children->begin(); it != children->end(); it++)
        {
            if((*it)->isTarget(pickup))
                return true;
        }

        children->clear();
        delete children;

        return false;
    }

    /**
    @brief
        Get the carrier that is both a child of the PickupCarrier (or the PickupCarrier itself) and a target of the input Pickupable.
    @param pickup
        A pounter to the Pickupable.
    @return
        Returns a pointer to the PickupCarrier that is the target of the input Pickupable.
    */
    PickupCarrier* PickupCarrier::getTarget(const Pickupable* pickup)
    {
        if(!this->isTarget(pickup))
            return NULL;

        if(pickup->isTarget(this)) // If the PickupCarrier itself is a target.
            return this;

        // Go recursively through all children to check whether they are the target.
        std::vector<PickupCarrier*>* children = this->getCarrierChildren();
        for(std::vector<PickupCarrier*>::iterator it = children->begin(); it != children->end(); it++)
        {
            if(pickup->isTarget(*it))
                return *it;
        }

        children->clear();
        delete children;

        return NULL;
    }

    /**
    @brief
        Adds a Pickupable to the list of pickups that are carried by this PickupCarrier.
    @param pickup
        A pointer to the pickup to be added.
    @return
        Returns true if successfull, false if the Pickupable was already present.
    */
    bool PickupCarrier::addPickup(Pickupable* pickup)
    {
        COUT(4) << "Adding Pickupable (&" << pickup << ") to PickupCarrier (&" << this << ")" << std::endl;
        return this->pickups_.insert(pickup).second;
    }

    /**
    @brief
        Removes a Pickupable from the list of pickups that are carried by this PickupCarrier.
    @param pickup
        A pointer to the pickup to be removed.
    @return
        Returns true if successfull, false if the Pickupable was not present in the list.
    */
    bool PickupCarrier::removePickup(Pickupable* pickup)
    {
        COUT(4) << "Removing Pickupable (&" << pickup << ") from PickupCarrier (&" << this << ")" << std::endl;
        return this->pickups_.erase(pickup) == 1;
    }

}

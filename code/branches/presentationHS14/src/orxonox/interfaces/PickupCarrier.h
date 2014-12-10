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
    @file PickupCarrier.h
    @brief Definition of the PickupCarrier class.
    @ingroup Pickup
*/

#ifndef _PickupCarrier_H__
#define _PickupCarrier_H__

#include "OrxonoxPrereqs.h"

#include <set>
#include <vector>

#include "core/class/OrxonoxInterface.h"

namespace orxonox
{

    // Forward-declarations.
    class MetaPickup;

    /**
    @brief
        The PickupCarrier interface provides the means, for any class implementing it, to possess @ref orxonox::Pickupable "Pickupables".

        For a class to use the PickupCarrier interface it must implement the follwing three methods:
        - <b>getCarrierPosition()</b> The getCarrierPosition() method returns the absolute position (in space) of the PickupCarrier.

        Different PickupCarriers are structured hierarchically, a pickup can be picked up by a PickupCarrier that can't really carry that particular pickup but one of its children (or one of their children) can, and thus it gets "handed down" until it is at the right place.
        But this structure has to be established first.
        - <b>getCarrierChildren()</b> To this end a PickupCarrier needs to implement getCarrierChildren() which returns a list of its direct PickupCarrier children. If you need an example, have a look at @ref orxonox::Pawn "Pawn" and @ref orxonox::Engine "Engine".
        - <b>getCarrierParent()</b> This is the method in the other direction. It returns the parent of this PickupCarrier, or NULL if the PickupCarrier is a root node in this hierarchy.

    @author
        Damian 'Mozork' Frick

    @ingroup Pickup
    */
    class _OrxonoxExport PickupCarrier : virtual public OrxonoxInterface
    {
        // So that the different Pickupables have full access to their PickupCarrier.
        friend class Pickupable;
        friend class PickupManager;
        // Friends.
        friend class MetaPickup;

        public:
            PickupCarrier(); //!< Constructor.
            virtual ~PickupCarrier(); //!< Destructor.
            void preDestroy(void); //!< Is called before the PickupCarrier is effectively destroyed.

            bool isTarget(const Pickupable* pickup) const; //!< Can be used to check whether the PickupCarrier or a child of his is a target ot the input Pickupable.
            PickupCarrier* getTarget(const Pickupable* pickup); //!< Get the carrier that is both a child of the PickupCarrier (or the PickupCarrier itself) and a target of the input Pickupable.

            /**
            @brief Get the (absolute) position of the PickupCarrier.
                   This method needs to be implemented by any direct derivative class of PickupCarrier.
            @return Returns the position as a Vector3.
            */
            virtual const Vector3& getCarrierPosition(void) const = 0;

        protected:
            /**
            @brief Get all direct children of this PickupSpawner.
                   This method needs to be implemented by any direct derivative class of PickupCarrier.
                   The returned list will be deleted by the methods calling this function.
            @return Returns a pointer to a list of all direct children.
            */
            virtual std::vector<PickupCarrier*>* getCarrierChildren(void) const = 0;
            /**
            @brief Get the parent of this PickupSpawner.
                   This method needs to be implemented by any direct derivative class of PickupCarrier.
            @return Returns a pointer to the parent.
            */
            virtual PickupCarrier* getCarrierParent(void) const = 0;

            /**
            @brief Get all Pickupables this PickupCarrier has.
            @return  Returns the set of all Pickupables this PickupCarrier has.
            */
            std::set<Pickupable*>& getPickups(void)
                { return this->pickups_; }

        private:
            std::set<Pickupable*> pickups_; //!< The list of Pickupables carried by this PickupCarrier.

            bool addPickup(Pickupable* pickup); //!< Adds a Pickupable to the list of pickups that are carried by this PickupCarrier.
            bool removePickup(Pickupable* pickup); //!< Removes a Pickupable from the list of pickups that are carried by this PickupCarrier.

    };
}

#endif /* _PickupCarrier_H__ */

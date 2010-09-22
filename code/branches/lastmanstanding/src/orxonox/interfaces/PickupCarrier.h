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

#include <list>
#include <set>
#include "Pickupable.h"
#include "core/Identifier.h"
#include "core/WeakPtr.h"

#include "core/OrxonoxClass.h"

namespace orxonox
{

    //! Forward-declarations.
    class PickupManager;
    class Pickup;
    class HealthPickup;
    class InvisiblePickup;
    class MetaPickup;
    class DronePickup;
    class SpeedPickup;

    /**
    @brief
        The PickupCarrier interface provides the means, for any class implementing it, to possess Pickupables.
    @author
        Damian 'Mozork' Frick
    */
    class _OrxonoxExport PickupCarrier : virtual public OrxonoxClass
    {
        //! So that the different Pickupables have full access to their PickupCarrier.
        friend class Pickupable;
        friend class PickupManager;
        //! Friends.
        friend class Pickup;
        friend class HealthPickup;
        friend class InvisiblePickup;
        friend class MetaPickup;
        friend class DronePickup;
        friend class SpeedPickup;

        public:
            PickupCarrier(); //!< Constructor.
            virtual ~PickupCarrier(); //!< Destructor.
            void preDestroy(void);

            /**
            @brief Can be used to check whether the PickupCarrier or a child of his is a target ot the input Pickupable.
            @param pickup A pointer to the Pickupable.
            @return Returns true if the PickupCarrier or one of its children is a target, false if not.
            */
            bool isTarget(const Pickupable* pickup)
                {
                    if(pickup->isTarget(this)) //!< If the PickupCarrier itself is a target.
                        return true;

                    //! Go recursively through all children to check whether they are a target.
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
            @brief Get the (absolute) position of the PickupCarrier.
                   This method needs to be implemented by any direct derivative class of PickupCarrier.
            @return Returns the position as a Vector3.
            */
            virtual const Vector3& getCarrierPosition(void) = 0;

        protected:
            /**
            @brief Get all direct children of this PickupSpawner.
                   This method needs to be implemented by any direct derivative class of PickupCarrier.
                   The returned list will be deleted by the methods calling this function.
            @return Returns a pointer to a list of all direct children.
            */
            virtual std::vector<PickupCarrier*>* getCarrierChildren(void) = 0;
            /**
            @brief Get the parent of this PickupSpawner
                   This method needs to be implemented by any direct derivative class of PickupCarrier.
            @return Returns a pointer to the parent.
            */
            virtual PickupCarrier* getCarrierParent(void) = 0;

            /**
            @brief Get all Pickupables this PickupCarrier has.
            @return  Returns the set of all Pickupables this PickupCarrier has.
            */
            std::set<Pickupable*>& getPickups(void)
                { return this->pickups_; }

        private:
            std::set<Pickupable*> pickups_; //!< The list of Pickupables carried by this PickupCarrier.

            /**
            @brief Adds a Pickupable to the list of pickups that are carried by this PickupCarrier.
            @param pickup A pointer to the pickup to be added.
            @return Returns true if successfull, false if the Pickupable was already present.
            */
            bool addPickup(Pickupable* pickup)
                {
                    COUT(4) << "Adding Pickupable (&" << pickup << ") to PickupCarrier (&" << this << ")" << std::endl;
                    return this->pickups_.insert(pickup).second;
                }

            /**
            @brief Removes a Pickupable from the list of pickups that are carried by thsi PickupCarrier.
            @param pickup A pointer to the pickup to be removed.
            @return Returns true if successfull, false if the Pickupable was not present in the list.
            */
            bool removePickup(Pickupable* pickup)
                {
                    COUT(4) << "Removing Pickupable (&" << pickup << ") from PickupCarrier (&" << this << ")" << std::endl;
                    return this->pickups_.erase(pickup) == 1;
                }

    };
}

#endif /* _PickupCarrier_H__ */

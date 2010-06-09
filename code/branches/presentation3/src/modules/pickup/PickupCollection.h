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
    @file PickupCollection.h
    @brief Declaration of PickupCollection.
*/

#ifndef _PickupCollection_H__
#define _PickupCollection_H__

#include "PickupPrereqs.h"

#include "interfaces/Pickupable.h"
#include "core/BaseObject.h"

#include <list>

namespace orxonox
{

    /**
    @brief
        The PickupCollection combines different Pickupables to a coherent, single pickup and makes the seem (from the outside looking in) just as if they were just one Pickupable.
    @author
        Damian 'Mozork' Frick
    */
    class _PickupExport PickupCollection : public Pickupable, public BaseObject
    {

        public:

            PickupCollection(BaseObject* creator); //!< Default Constructor.
            virtual ~PickupCollection(); //!< Destructor.

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode); //!< Creates an instance of this Class through XML.

            virtual void changedUsed(void); //!< Is called when the pickup has transited from used to unused or the other way around.
            virtual void changedCarrier(void); //!< Is called when the pickup has changed its PickupCarrier.
            virtual void changedPickedUp(void); //!< Is called when the pickup has transited from picked up to dropped or the other way around.

            virtual void clone(OrxonoxClass*& item); //!< Creates a duplicate of the input OrxonoxClass.

            virtual bool isTarget(PickupCarrier* carrier) const; //!< Get whether a given class, represented by the input Identifier, is a target of this PickupCollection.

            virtual const PickupIdentifier* getPickupIdentifier(void); //!< Get the PickupIdentifier of this PickupCollection.

            bool addPickupable(Pickupable* pickup); //!< Add the input Pickupable to list of Pickupables combined by this PickupCollection.
            const Pickupable* getPickupable(unsigned int index); //!< Get the Pickupable at the given index.

        protected:
            void initializeIdentifier(void); //!< Initializes the PickupIdentifier for this pickup.

            virtual bool createSpawner(void); //!< Facilitates the creation of a PickupSpawner upon dropping of the Pickupable.

            PickupCollectionIdentifier* pickupCollectionIdentifier_; //!< The PickupCollectionIdentifier of this PickupCollection. Is used to distinguish different PickupCollections amongst themselves.

        private:

            std::vector<WeakPtr<Pickupable> > pickups_; //!< The list of the pointers of all the Pickupables this PickupCollection consists of. They are weak pointers to facilitate testing, whether the pointers are still valid.

    };

}

#endif /* _PickupCollection_H__ */
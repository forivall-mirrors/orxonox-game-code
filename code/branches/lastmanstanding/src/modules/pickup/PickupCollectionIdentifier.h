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
    @file PickupCollectionIdentifier.h
    @brief Declaration of PickupCollectionIdentifier.
    @ingroup Pickup
*/

#ifndef _PickupCollectionIdentifier_H__
#define _PickupCollectionIdentifier_H_

#include "PickupPrereqs.h"

#include "pickup/PickupIdentifier.h"

#include <set>

namespace orxonox
{

    /**
    @brief
        The PickupCollectionIdentifier is the specialization of the PickupIdentifier for the PickupCollection class.
        It identifies PickupCollections based on the different Pickupables they consist of.
        Pickupables can be added to the PickupCollectionIdentifier via the addPickup method.
    @author
        Damian 'Mozork' Frick
    */
    class _PickupExport PickupCollectionIdentifier : public PickupIdentifier
    {

        public:
            PickupCollectionIdentifier(Pickupable* pickup); //!< Constructor.
            ~PickupCollectionIdentifier(); //!< Destructor.

            virtual int compare(const PickupIdentifier* identifier) const; //!< Compares a PickupCollectionIdentifier with a PickupIdentifier.

            void addPickup(const PickupIdentifier* identifier); //!< Add a Pickupable to the PickupCollectionIdentifier.

        private:
            std::set<const PickupIdentifier*, PickupIdentifierCompare> identifiers_; //!< The set of PickupIdentifiers of the Pickupables the PickupCollection with this PickupCollectionIdentifier consists of, ordered by the rule set by PickupIdentifierCompare.

    };

}

#endif // _PickupCollectionIdentifier_H_

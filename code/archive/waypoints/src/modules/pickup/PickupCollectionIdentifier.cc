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
    @file PickupCollectionIdentifier.cc
    @brief Implementation of PickupCollectionIdentifier.
*/

#include "core/CoreIncludes.h"

#include "PickupCollectionIdentifier.h"

namespace orxonox
{

    /**
    @brief
        Constructor. Registers the object.
    */
    PickupCollectionIdentifier::PickupCollectionIdentifier(Pickupable* pickup) : PickupIdentifier(pickup)
    {
        RegisterObject(PickupCollectionIdentifier);
    }

    /**
    @brief
        Destructor.
    */
    PickupCollectionIdentifier::~PickupCollectionIdentifier()
    {

    }

    /**
    @brief
        Compares a PickupCollectionIdentifier with a PickupIdentifier and returns 0 if a == b, <0 if a < b and >0 if a > b for a.compare(b), where a is a PickupCollectionIdentifier and b is just some PickupIdentifier.
    @param identifier
        Pointer to the second PickupIdentifier, b.
    @return
        Returns an integer. 0 if the two compared PickupIdentifiers are the same, <0 if a < b and >0 if a > b.
    */
    int PickupCollectionIdentifier::compare(const PickupIdentifier* identifier) const
    {
        assert(identifier);

        // Slight un-niceity to cast the const PickupIdentifier to a const PickupCollectionIdentifier, but since we cast to a const, there is no harm done.
        PickupIdentifier* temp = const_cast<PickupIdentifier*>(identifier);
        const PickupCollectionIdentifier* collectionIdentifier = orxonox_cast<PickupCollectionIdentifier*>(temp);

        // If the input PickupIdentifier 'identifier' is no PickupCollectionIdentifier then just the two PickupIdentifiers are compared.
        if(collectionIdentifier == NULL)
        {
            return this->PickupIdentifier::compare(identifier);
        }

        // If the number of Pickupables each of the two PickupCollectionIdentifiers contain differ, the one with less is considered smaller.
        if(this->identifiers_.size() != collectionIdentifier->identifiers_.size())
            return this->identifiers_.size()-collectionIdentifier->identifiers_.size();

        // Compare the Pickupables of the two PickupCollectionIdentifiers one after the other. the one with the first 'smaller' one is considered smaller.
        std::set<const PickupIdentifier*, PickupIdentifierCompare>::const_iterator it2 = collectionIdentifier->identifiers_.begin();
        for(std::set<const PickupIdentifier*, PickupIdentifierCompare>::const_iterator it = this->identifiers_.begin(); it != this->identifiers_.end(); it++)
        {

            if((*it)->compare(*it2) < 0)
                return -1;
            if((*it2)->compare(*it) < 0)
                return 1;
        }

        // This means they are equal.
        return 0;
    }

    /**
    @brief
        Add a Pickupable to the PickupCollectionIdentifier.
    @param identifier
        A pointer to the PickupIdentifier of the Pickupable to be added.
    */
    void PickupCollectionIdentifier::addPickup(const PickupIdentifier* identifier)
    {
        this->identifiers_.insert(identifier);
    }

}


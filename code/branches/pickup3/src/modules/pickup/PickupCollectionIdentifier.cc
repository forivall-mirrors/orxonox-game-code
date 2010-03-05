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
 *      ...
 *   Co-authors:
 *      ...
 *
*/

#include "PickupCollectionIdentifier.h"

#include "core/CoreIncludes.h"

namespace orxonox
{
    
    PickupCollectionIdentifier::PickupCollectionIdentifier(void)
    {
        RegisterObject(PickupCollectionIdentifier);
    }
    
    PickupCollectionIdentifier::~PickupCollectionIdentifier()
    {
        
    }

    int PickupCollectionIdentifier::compare(const PickupIdentifier& identifier) const
    {
        PickupIdentifier* temp = const_cast<PickupIdentifier*>(&identifier);
        const PickupCollectionIdentifier* collectionIdentifier = dynamic_cast<PickupCollectionIdentifier*>(temp);
        if(collectionIdentifier == NULL)
        {
            return this->compare(identifier);
        }
        
        if(this->identifiers_.size() != collectionIdentifier->identifiers_.size())
            return this->identifiers_.size()-collectionIdentifier->identifiers_.size();
        
        std::set<const PickupIdentifier*, PickupIdentifierCompare>::const_iterator it2 = collectionIdentifier->identifiers_.begin();
        for(std::set<const PickupIdentifier*, PickupIdentifierCompare>::const_iterator it = this->identifiers_.begin(); it != this->identifiers_.end(); it++)
        {
            
            if((*it)->compare(**it2) < 0)
                return -1;
            if((*it2)->compare(**it) < 0)
                return 1;
        }
        
        return 0;
    }
    
    void PickupCollectionIdentifier::addPickup(const PickupIdentifier* identifier)
    {
        this->identifiers_.insert(identifier);
    }
    
}


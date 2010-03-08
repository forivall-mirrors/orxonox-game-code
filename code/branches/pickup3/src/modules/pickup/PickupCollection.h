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

#ifndef _PickupCollection_H__
#define _PickupCollection_H__

#include "PickupPrereqs.h"

#include "interfaces/Pickupable.h"
#include "core/BaseObject.h"
#include "core/XMLPort.h"

#include <list>

namespace orxonox
{

    /**
    @brief
        The PickupCollection combines different Pickupables to a coherent, single pickup and makes the seem (from the outside looking in) just as if they were just one Pickupable.
    @author
        Damian 'Mozork' Frick
    */
    //TODO: Isn't private inheritance sufficient?
    class _PickupExport PickupCollection : public Pickupable, public BaseObject
    {
        
        public:
            
            PickupCollection(BaseObject* creator);
            virtual ~PickupCollection();
            
            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);

            virtual void changedUsed(void);
            
            virtual void changedCarrier(void);
            
            virtual void clone(OrxonoxClass*& item);
            
            virtual const PickupIdentifier* getPickupIdentifier(void);
            
            bool addPickupable(Pickupable* pickup);
            const Pickupable* getPickupable(unsigned int index);
            
        protected:
            void initializeIdentifier(void);
            
            virtual bool createSpawner(const Vector3& position); //!< Facilitates the creation of a PickupSpawner upon dropping of the Pickupable.
            
            PickupCollectionIdentifier* pickupCollectionIdentifier_;
            
        private:
            
            std::vector<Pickupable*> pickups_;
        
    };
    
}

#endif /* _PickupCollection_H__ */
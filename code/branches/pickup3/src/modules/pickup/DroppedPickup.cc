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
 *      Daniel 'Huty' Haggenmueller
 *   Co-authors:
 *      Damian 'Mozork' Frick
 *
 */

/**
    @file
    @brief Implementation of the DroppedPickup class.
*/

#include "DroppedPickup.h"

#include "core/CoreIncludes.h"
#include "interfaces/Pickupable.h"
#include "graphics/Model.h"

namespace orxonox
{
    /**
    @brief
        Default constructor. Registers object and sets default values.
    */
    DroppedPickup::DroppedPickup(BaseObject* creator) : PickupSpawner(creator)
    {
        RegisterObject(DroppedPickup);
        
        this->initialize();
    }

    /**
    @brief
        Constructor. Registers the object and sets values.
    @param creator
        The creator of the DroppedPickup.
    @param pickup
        The Pickupable that was dropped.
    @param position
        The position at which the DroppedPickup should be created.
    @param triggerDistance
        The distance at which the PickupSpawner triggers. Default is 10.
    */
    DroppedPickup::DroppedPickup(BaseObject* creator, Pickupable* pickup, const Vector3& position, float triggerDistance) : PickupSpawner(creator, pickup, triggerDistance, 0, 1)
    {   
        RegisterObject(DroppedPickup);
        
        this->initialize();
        
        this->setPosition(position);
    }

    /**
    @brief
        Destructor.
    */
    DroppedPickup::~DroppedPickup()
    {
        if(this->gotPickedUp_ && this->pickup_ != NULL)
        {
            this->pickup_ = NULL;
        }
    }
    
    /**
    @brief
        Initializes the member variables of the object.
    */
    void DroppedPickup::initialize(void)
    {
        this->gotPickedUp_ = false;
    }

    /**
    @brief
        Creates the Pickupable that is going to get picked up.
        In the case of the DroppedItem it is the one and only Pickupable that was dropped. No additional Pickupables of the same type are created.
    */
    Pickupable* DroppedPickup::getPickup(void)
    {
        return this->pickup_;
    }

}

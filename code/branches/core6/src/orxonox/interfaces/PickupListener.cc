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
    @file PickupListener.cc
    @brief Implementation of the PickupListener class.
*/

#include "OrxonoxPrereqs.h"

#include "core/CoreIncludes.h"

#include "PickupListener.h"

namespace orxonox
{

    /**
    @brief
        Constructor. Registers the object.
    */
    PickupListener::PickupListener()
    {
        RegisterObject(PickupListener);
    }

    /**
    @brief
        Destructor.
    */
    PickupListener::~PickupListener()
    {

    }

    /**
    @brief
        Static method that is used to notify the PickupListener itself of the change in the used status of a Pickupable.
        This is called in Pickupable.
    @param pickup
        The Pickupable that has changed its used status.
    @param used
        Thse value the status has changed to.
    */
    /*static*/ void PickupListener::broadcastPickupChangedUsed(Pickupable* pickup, bool used)
    {
        assert(pickup);

        // Iterate through all PickupListeners and notify them by calling the method they overloaded.
        for(ObjectList<PickupListener>::iterator it = ObjectList<PickupListener>::begin(); it != ObjectList<PickupListener>::end(); ++it)
            it->pickupChangedUsed(pickup, used);
    }

    /**
    @brief
        Static method that is used to notify the PickupListener itself of the change in the pickedUp status of a Pickupable.
        This is called in Pickupable.
    @param pickup
        The Pickupable that has changed its pickedUp status.
    @param pickedUp
        Thse value the status has changed to.
    */
    /*static*/ void PickupListener::broadcastPickupChangedPickedUp(Pickupable* pickup, bool pickedUp)
    {
        assert(pickup);

        // Iterate through all PickupListeners and notify them by calling the method they overloaded.
        for(ObjectList<PickupListener>::iterator it = ObjectList<PickupListener>::begin(); it != ObjectList<PickupListener>::end(); ++it)
            it->pickupChangedPickedUp(pickup, pickedUp);
    }

}

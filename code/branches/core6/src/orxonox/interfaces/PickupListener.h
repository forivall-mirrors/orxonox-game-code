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
    @file PickupListener.h
    @brief Definition of the PickupListener class.
    @ingroup Pickup
*/


#ifndef _PickupListener_H__
#define _PickupListener_H__

#include "Pickupable.h"

#include "core/class/OrxonoxInterface.h"

namespace orxonox
{

    /**
    @brief
        The PickupListener class facilitates the flow of information regarding the picking up, dropping, using and unusing of @ref orxonox::Pickupable "Pickupables" to interested parties (such as the @ref orxonox::PickupManager "PickupManager").

        All you need to to do be notified is to inherit from PickupListener and implement the two methods <code>pickupChangedUsed()</code> and <code>pickupChangedPickedUp()</code>.

    @author
        Damian 'Mozork' Frick

    @ingroup Pickup
    */
    class _OrxonoxExport PickupListener : virtual public OrxonoxInterface
    {
        public:
            PickupListener();
            virtual ~PickupListener();

            static void broadcastPickupChangedUsed(Pickupable* pickup, bool used); //!< Static method that is used to notify the PickupListener itself of the change in the used status of a Pickupable.
            static void broadcastPickupChangedPickedUp(Pickupable* pickup, bool pickedUp); //!< Static method that is used to notify the PickupListener itself of the change in the pickedUp status of a Pickupable.

            /**
            @brief Method that can be overloaded by any PickupListener to be notified of a change in the used status of a Pickupable.
            @param pickup The Pickupable, whose used status has changed.
            @param used The value the used status has changed to.
            */
            virtual void pickupChangedUsed(Pickupable* pickup, bool used) {}
            /**
            @brief Method that can be overloaded by any PickupListener to be notified of a change in the pickedUp status of a Pickupable.
            @param pickup The Pickupable, whose pickedUp status has changed.
            @param pickedUp The value the pickedUp status has changed to.
            */
            virtual void pickupChangedPickedUp(Pickupable* pickup, bool pickedUp) {}
    };

}

#endif // _PickupListener_H__

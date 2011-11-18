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
    @file PartyMember.cc
    @brief Implementation of the PartyMember class.
*/

#include "OrxonoxPrereqs.h"

#include "core/CoreIncludes.h"

#include "PartyMember.h"

namespace orxonox
{

    /**
    @brief
        Constructor. Registers the object.
    */
    PartyMember::PartyMember()
    {
        RegisterRootObject(PartyMember);
        this->team_ = -1;
    }

    /**
    @brief
        Destructor.
    */
    PartyMember::~PartyMember()
    {

    }

    /**
    @brief
        Static method that is used to notify the PartyMember itself of the change in the used status of a Pickupable.
        This is called in Pickupable.
    @param pickup
        The Pickupable that has changed its used status.
    @param used
        Thse value the status has changed to.
    */

}

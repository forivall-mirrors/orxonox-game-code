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
    @file MultiTriggerContainer.cc
    @brief Implementation of the MultiTriggerContainer class.
    @ingroup MultiTrigger
*/

#include "MultiTriggerContainer.h"

#include "core/CoreIncludes.h"

#include "worldentities/pawns/Pawn.h"

namespace orxonox
{

    CreateUnloadableFactory(MultiTriggerContainer);

    /**
    @brief
        Default constructor. Registers the object and creates an empty container.
    @param creator
        The creator.
    */
    MultiTriggerContainer::MultiTriggerContainer(BaseObject* creator) : BaseObject(creator), originator_(NULL), data_(NULL)
    {
        RegisterObject(MultiTriggerContainer);
    }

    /**
    @brief
        Constructor. Registers the object and sets the input values.
    @param creator
        The creator.
    @param originator
        A pointer to the originator of the Event, i.e. the MultiTrigger that fired the Event. (or is about to fire)
    @param data
        A pointer to the data that should be sent with the container.
    */
    MultiTriggerContainer::MultiTriggerContainer(BaseObject* creator, MultiTrigger* originator, BaseObject* data) : BaseObject(creator), originator_(originator), data_(data)
    {
        RegisterObject(MultiTriggerContainer);

        Pawn* pawn = orxonox_cast<Pawn*>(data);
        if(pawn != NULL)
        {
            this->setForPlayer(true);
            this->setTriggeringPlayer(pawn);
        }
    }

    /**
    @brief
        Destructor.
    */
    MultiTriggerContainer::~MultiTriggerContainer()
    {

    }


}

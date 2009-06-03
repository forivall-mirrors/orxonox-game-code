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
    @file
    @brief
    Implementation of the PlayerTrigger class.
*/

#include "PlayerTrigger.h"

#include "core/CoreIncludes.h"

namespace orxonox
{
    /**
    @brief
        Constructor. Registers the object and initializes defaults.
    */
    PlayerTrigger::PlayerTrigger(BaseObject* creator) : Trigger(creator)
    {
        RegisterObject(PlayerTrigger);

        this->player_ = NULL;
        this->isForPlayer_ = true;
    }

    /**
    @brief
        Destructor.
    */
    PlayerTrigger::~PlayerTrigger()
    {
    }

    /**
    @brief
        Method for creating a QuestEffectBeacon object through XML.
    */
    void PlayerTrigger::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(PlayerTrigger, XMLPort, xmlelement, mode);
    }
}

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
 *      ...
 *
 */

/**
    @file
    @brief Implementation of HealthImmediate.
*/

#include "HealthImmediate.h"

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "worldentities/pawns/Pawn.h"

namespace orxonox
{
    CreateFactory(HealthImmediate);

    HealthImmediate::HealthImmediate(BaseObject* creator) : PassiveItem(creator)
    {
        RegisterObject(HealthImmediate);

        this->recoveredHealth_ = 0;
    }
    HealthImmediate::~HealthImmediate()
    {
    }

    void HealthImmediate::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(HealthImmediate, XMLPort, xmlelement, mode);

        XMLPortParam(HealthImmediate, "recoveredHealth", setRecoveredHealth, getRecoveredHealth, xmlelement, mode);
    }

    bool HealthImmediate::pickedUp(Pawn* pawn)
    {
        float maxH = pawn->getMaxHealth();
        float curH = pawn->getHealth();

        if (curH < maxH)
        {
            pawn->addHealth(this->recoveredHealth_);
            this->destroy();
        }

        return (curH < maxH);
    }
}

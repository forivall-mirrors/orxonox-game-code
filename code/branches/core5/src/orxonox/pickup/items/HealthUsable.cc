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
    @brief Implementation of HealthUsable.
*/

#include "HealthUsable.h"

#include "util/Math.h"
#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "pickup/DroppedItem.h"
#include "worldentities/pawns/Pawn.h"

namespace orxonox
{
    CreateFactory(HealthUsable);

    /**
        @brief Constructor
        @param creator Object that created this item.
    */
    HealthUsable::HealthUsable(BaseObject* creator) : UsableItem(creator)
    {
        RegisterObject(HealthUsable);

        this->recoveredHealth_ = 0;
    }
    //! Deconstructor
    HealthUsable::~HealthUsable()
    {
    }
    /**
        @brief XMLPort for Jump.
        @param xmlelement Element of the XML-file.
        @param mode XMLPort mode to use.
    */
    void HealthUsable::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(HealthUsable, XMLPort, xmlelement, mode);

        XMLPortParam(HealthUsable, "recoveredHealth", setRecoveredHealth, getRecoveredHealth, xmlelement, mode);
    }
    /**
        @brief Called when the item is used, makes the user "jump".
        @param pawn Pawn which used te item.
    */
    void HealthUsable::used(Pawn* pawn)
    {
        float maxH = pawn->getMaxHealth();
        float curH = pawn->getHealth();

        if (curH < maxH) {
            pawn->addHealth(this->recoveredHealth_);

            this->removeFrom(pawn);
            this->destroy();
        }
    }
    /**
        @brief Called when the item is picked up.
        @param pawn Pawn which picked up the item.
    */
    bool HealthUsable::pickedUp(Pawn* pawn)
    {
        return this->addTo(pawn);
    }
    /**
        @brief Called when the item is dropped, creates a DroppedItem behind the pawn.
        @param pawn Pawn which dropped the item.
    */
    bool HealthUsable::dropped(Pawn* pawn)
    {
        DroppedItem::createDefaultDrop(this, pawn, ColourValue(1.0f, 0.0f, 0.0f), 30.0f);
        return this->removeFrom(pawn);
    }
}

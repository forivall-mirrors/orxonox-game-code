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
    @brief Implementation of Jump.
*/

#include "Jump.h"
#include "DroppedItem.h"

#include "objects/worldentities/pawns/Pawn.h"

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "core/Core.h"

namespace orxonox
{
    CreateFactory(Jump);

    /**
        @brief Constructor
        @param creator Object that created this item.
    */
    Jump::Jump(BaseObject* creator) : UsableItem(creator)
    {
        RegisterObject(Jump);

        this->velocity_ = Vector3(0.0f, 0.0f, 0.0f);
        this->jumpsAvailable_ = 1;
    }
    //! Deconstructor
    Jump::~Jump()
    {
    }
    /**
        @brief XMLPort for Jump.
        @param xmlelement Element of the XML-file.
        @param mode XMLPort mode to use.
    */
    void Jump::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(Jump, XMLPort, xmlelement, mode);

        XMLPortParam(Jump, "velocity", setVelocity, getVelocity, xmlelement, mode);
        XMLPortParam(Jump, "jumpsAvailable", setJumpsAvailable, getJumpsAvailable, xmlelement, mode);
    }
    /**
        @brief Called when the item is used, makes the user "jump".
        @param pawn Pawn which used te item.
    */
    void Jump::used(Pawn* pawn)
    {
        if (this->jumpsAvailable_ > 0){
            pawn->setVelocity(pawn->getVelocity() + pawn->getOrientation() * this->velocity_);
        }

        this->jumpsAvailable_--;
        if (this->jumpsAvailable_ <= 0)
        {
            this->removeFrom(pawn);
            delete this;
        }
    }
    /**
        @brief Called when the item is picked up.
        @param pawn Pawn which picked up the item.
    */
    bool Jump::pickedUp(Pawn* pawn)
    {
        return this->addTo(pawn);
    }
    /**
        @brief Called when the item is dropped, creates a DroppedItem behind the pawn.
        @param pawn Pawn which dropped the item.
    */
    bool Jump::dropped(Pawn* pawn)
    {
        DroppedItem::createDefaultDrop(this, pawn, ColourValue(1.0f, 0.0f, 0.0f), 5.0f);
        return this->removeFrom(pawn);
    }
}
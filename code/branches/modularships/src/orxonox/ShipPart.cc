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
 *      Noe Pedrazzini
 *   Co-authors:
 *      ...
 *
 */

#include "ShipPart.h"

#include <algorithm>

#include "core/CoreIncludes.h"
#include "core/GameMode.h"
#include "core/XMLPort.h"
#include "network/NetworkFunction.h"
#include "items/Item.h"
#include "worldentities/pawns/Pawn.h"
#include "gametypes/Gametype.h"
#include "worldentities/pawns/ModularSpaceShip.h"


namespace orxonox
{
    RegisterClass(ShipPart);

    ShipPart::ShipPart(Context* context)
        : Item(context)
    {
        RegisterObject(ShipPart);
    }

    ShipPart::~ShipPart()
    {

    }


    /**
    @brief
        Add a StaticEntity to the ShipPart.
    @param engine
        A pointer to the StaticEntity to be added.
    */
    void ShipPart::addEntity(StaticEntity* entity)
    {
        OrxAssert(entity != NULL, "The Entity cannot be NULL.");
        this->entityList_.push_back(entity);
        //part->addToSpaceShip(this); //FIXME: (noep) add
    }

    /**
    @brief
        Get the i-th StaticEntity of the ShipPart.
    @return
        Returns a pointer to the i-the StaticEntity. NULL if there is no StaticEntity with that index.
    */
    StaticEntity* ShipPart::getEntity(unsigned int index)
    {
        if(this->entityList_.size() >= index)
            return NULL;
        else
            return this->entityList_[index];
    }

    void ShipPart::setDamageAbsorption(float value)
    {
        this->damageAbsorption_ = value;
    }

    /**
    @brief
        Sets the health of the ShipPart.
    */
    void ShipPart::setHealth(float health)
    {
        this->health_ = health;
    }

    /**
    @brief
        Handles a received hit.
    */
    void ShipPart::handleHit(float damage, float healthdamage, float shielddamage, Pawn* originator)
    {
        if (parent_->getGametype() && parent_->getGametype()->allowPawnDamage(parent_, originator))
        {
            if (shielddamage >= parent_->getShieldHealth())
            {
                parent_->setShieldHealth(0);
                this->setHealth(this->health_ - (healthdamage + damage) * this->damageAbsorption_);
                parent_->setHealth(parent_->getHealth() - (healthdamage + damage) * (1 - this->damageAbsorption_));
            }
            else
            {
                parent_->setShieldHealth(parent_->getShieldHealth() - shielddamage);

                // remove remaining shieldAbsorpton-Part of damage from shield
                shielddamage = damage * parent_->getShieldAbsorption();
                shielddamage = std::min(parent_->getShieldHealth(),shielddamage);
                parent_->setShieldHealth(parent_->getShieldHealth() - shielddamage);

                // set remaining damage to health
                this->setHealth(this->health_ - ((damage - shielddamage) - healthdamage) * this->damageAbsorption_);
                parent_->setHealth(parent_->getHealth() - ((damage - shielddamage) - healthdamage) * (1- this->damageAbsorption_));
            }
        }
    }


    /**
    @brief
        Adds the ShipPart to the input SpaceShip.
    @param ship
        A pointer to the SpaceShip to which the ShipPart is added.
    */
    /*void ShipPart::addToSpaceShip(ModularSpaceShip* ship)
    {
        this->parent_ = ship;

        if (ship)
        {
            this->parentID_ = ship->getObjectID();
            if (!ship->hasShipPart(this))
                ship->addShipPart(this);
        }
    }*/

}

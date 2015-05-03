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
#include "Item.h"
#include "worldentities/pawns/Pawn.h"
#include "worldentities/pawns/ModularSpaceShip.h"
#include "gametypes/Gametype.h"
#include "worldentities/StaticEntity.h"
#include "items/PartDestructionEvent.h"
#include "worldentities/BigExplosion.h"
#include "chat/ChatManager.h"


namespace orxonox
{
    RegisterClass(ShipPart);

    ShipPart::ShipPart(Context* context)
        : Item(context), parent_(NULL)
    {
        RegisterObject(ShipPart);
        this->eventExecution_ = true;
        this->healthMem_ = 100;
    }

    ShipPart::~ShipPart()
    {
        if (this->parent_)
        {
            // Remove this ShipPart from the parent.
            this->parent_->removeShipPart(this);
        }
    }

    void ShipPart::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(ShipPart, XMLPort, xmlelement, mode);

        XMLPortParam(ShipPart, "health", setHealth, getHealth, xmlelement, mode).defaultValues(100);
        XMLPortParam(ShipPart, "maxhealth", setMaxHealth, getMaxHealth, xmlelement, mode).defaultValues(200);
        XMLPortParam(ShipPart, "initialhealth", setInitialHealth, getInitialHealth, xmlelement, mode).defaultValues(100);

        XMLPortParam(ShipPart, "damageabsorption", setDamageAbsorption, getDamageAbsorption, xmlelement, mode).defaultValues(0.5);

        XMLPortParamTemplate(ShipPart, "explosionposition", setExplosionPosition, getExplosionPosition, xmlelement, mode, Vector3);

        XMLPortObject(ShipPart, PartDestructionEvent, "destructionevents", addDestructionEvent, getDestructionEvent, xmlelement, mode);
    }

    /**
    @brief
        Is called when the ShipPart dies.
        Executes PartDestructionEvents.
        Tells the ModularSpaceShip to remove this ShipPart.
    */
    void ShipPart::death()
    {
        this->explode();

        if(eventExecution_)
        {
            // Execute all destruction events
            for (unsigned int i = 0; i < this->eventList_.size(); i++)
            {
                this->getDestructionEvent(i)->execute();
            }
        }

        this->destroyLater();
    }

    void ShipPart::explode()
    {
        BigExplosion* chunk = new BigExplosion(this->getContext());
        chunk->setPosition(this->parent_->getPosition() + this->parent_->getOrientation() * (this->explosionPosition_));
        //chunk->setPosition(this->parent_->getPosition() + this->parent_->getOrientation() * Vector3(this->entityList_[0]->getLocalInertia()));
        chunk->setVelocity(this->parent_->getVelocity());

        // this->explosionSound_->setPosition(this->parent_->getPosition());
        // this->explosionSound_->play();
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

    /**
    @brief
        Check whether the ShipPart has a particular Entity.
    @param engine
        A pointer to the Entity to be checked.
    */
    bool ShipPart::hasEntity(StaticEntity* entity) const
    {
        for(unsigned int i = 0; i < this->entityList_.size(); i++)
        {
            if(this->entityList_[i] == entity)
                return true;
        }
        return false;
    }

    /**
    @brief
        Add a PartDestructionEvent to the ShipPart.
    @param engine
        A pointer to the PartDestructionEvent to be added.
    */
    void ShipPart::addDestructionEvent(PartDestructionEvent* event)
    {
        OrxAssert(event != NULL, "The PartDestructionEvent cannot be NULL.");
        event->setParent(this);
        this->eventList_.push_back(event);
    }

    /**
    @brief
        Get the i-th PartDestructionEvent of the ShipPart.
    @return
        Returns a pointer to the i-the PartDestructionEvent. NULL if there is no PartDestructionEvent with that index.
    */
    PartDestructionEvent* ShipPart::getDestructionEvent(unsigned int index)
    {
        if(this->eventList_.size() <= index)
            return NULL;
        else
            return this->eventList_[index];
    }

    void ShipPart::setDamageAbsorption(float value)
    {
        this->damageAbsorption_ = value;
    }

    void ShipPart::setParent(ModularSpaceShip* ship)
    {
        this->parent_ = ship;
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
        if (this->health_ < 0)
            this->death();

        // (Ugly) Chatoutput of health, until a GUI for modularspaceships-shipparts is implemented.
        if ((this->health_ < 0.2 * this->maxHealth_) && (this->healthMem_ == 40))
        {
            this->healthMem_ = 20;
            ChatManager::message("ShipPart " + this->getName() + " remaining health is 20%!");
            return;
        }
        if ((this->health_ < 0.4 * this->maxHealth_) && (this->healthMem_ == 60))
        {
            this->healthMem_ = 40;
            ChatManager::message("ShipPart " + this->getName() + " remaining health is 40%!");
            return;
        }
        if ((this->health_ < 0.6 * this->maxHealth_) && (this->healthMem_ == 80))
        {
            this->healthMem_ = 60;
            ChatManager::message("ShipPart " + this->getName() + " remaining health is 60%!");
            return;
        }
        if ((this->health_ < 0.8 * this->maxHealth_) && (this->healthMem_ == 100))
        {
            this->healthMem_ = 80;
            ChatManager::message("ShipPart " + this->getName() + " remaining health is 80%!");
            return;
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

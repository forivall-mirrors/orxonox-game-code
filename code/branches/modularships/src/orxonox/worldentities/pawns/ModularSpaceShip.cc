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
 *      Fabian 'x3n' Landau
 *   Co-authors:
 *      Noe Pedrazzini
 *
 */

#include "ModularSpaceShip.h"

#include <BulletDynamics/Dynamics/btRigidBody.h>

#include "core/CoreIncludes.h"
#include "core/config/ConfigValueIncludes.h"
#include "core/Template.h"
#include "core/XMLPort.h"
#include "util/Math.h"
#include "gametypes/Gametype.h"

#include "items/ShipPart.h"
#include "worldentities/StaticEntity.h"
#include <BulletCollision/CollisionShapes/btCollisionShape.h>



namespace orxonox
{
    RegisterClass(ModularSpaceShip);

    ModularSpaceShip::ModularSpaceShip(Context* context) : SpaceShip(context)
    {
        RegisterObject(ModularSpaceShip);

        this->registerVariables();

    }

    ModularSpaceShip::~ModularSpaceShip()
    {
        if (this->isInitialized())
        {

        }
    }

    void ModularSpaceShip::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(ModularSpaceShip, XMLPort, xmlelement, mode);
        XMLPortObject(ModularSpaceShip, ShipPart, "parts", addShipPart, getShipPart, xmlelement, mode);
    }

    void ModularSpaceShip::registerVariables()
    {
        return;
    }

    void ModularSpaceShip::updatePartAssignment()
    {
        // iterate through all attached objects
        for (unsigned int i=0; i < 100; i++)
        {
            if (this->getAttachedObject(i) == NULL)
                break;
            // iterate through all attached parts
            for(unsigned int j = 0; j < this->partList_.size(); j++)
            {
                // if the name of the part matches the name of the object, add the object to that parts entitylist (unless it was already done).
                if((this->partList_[j]->getName() == this->getAttachedObject(i)->getName()) && !this->partList_[j]->hasEntity(orxonox_cast<StaticEntity*>(this->getAttachedObject(i))))
                {
                    // The Entity is added to the part's entityList_
                    this->partList_[j]->addEntity(orxonox_cast<StaticEntity*>(this->getAttachedObject(i)));
                    // An entry in the partMap_ is created, assigning the part to the entity.
                    this->addPartEntityAssignment((StaticEntity*)(this->getAttachedObject(i)), this->partList_[j]);
                    orxout() << "A matching part-entity-pair with name " << this->partList_[j]->getName() << " was found!" << endl;
                    this->partList_[j]->printEntities(); // FIXME: (noep) remove debug
                }
            }
        }

        orxout() << "List of all assignments:" << endl;
        for (std::map<StaticEntity*, ShipPart*>::const_iterator it = this->partMap_.begin(); it != this->partMap_.end(); ++it)
                {
                    orxout() << "Entity: " << it->first << "   Part: " << it->second << endl;
                }
    }

    void ModularSpaceShip::attach(WorldEntity* object)
    {
        SpaceShip::attach(object);
        this->updatePartAssignment();
    }

    void ModularSpaceShip::addPartEntityAssignment(StaticEntity* entity, ShipPart* part)
    {
        if (!entity || !part)
            return;

        if (this->partMap_.find(entity) != this->partMap_.end())
                {
                    orxout(internal_warning) << "Assigning an Entity to multiple parts is not yet supported." << endl;
                    return;
                }

        this->partMap_[entity] = part;
        orxout() << "New entity-part assignment created!" << endl;
    }

    /**
    @brief
        Get the ShipPart an attached entity belongs to.
    @param entity
        The entity to be searched.
    @return
        Returns a pointer to the ShipPart the entity belongs to.
    */
    ShipPart* ModularSpaceShip::getPartOfEntity(StaticEntity* entity) const
    {
        for (std::map<StaticEntity*, ShipPart*>::const_iterator it = this->partMap_.begin(); it != this->partMap_.end(); ++it)
        {
            if (it->first == entity)
                return it->second;
        }
        return NULL;
    }

    //FIXME: (noep) finish
    // void ModularSpaceShip::attach

    void ModularSpaceShip::damage(float damage, float healthdamage, float shielddamage, Pawn* originator, const btCollisionShape* cs)
    {
        orxout() << "Mdamage(): Collision detected on " << this->getRadarName() << ", btCS*: " << cs << endl;
        orxout() << "UserPtr of said collisionShape: " << cs->getUserPointer() << endl;


            // Print all attached objects & parts
        /*
        orxout() << "  " << this->getName() << " has the following Objects attached:" << endl;

        for (int i=0; i<50; i++)
        {
            if (this->getAttachedObject(i)==NULL)
                break;
            orxout() << " " << i << ": " << this->getAttachedObject(i) << " (" << this->getAttachedObject(i)->getName() << ")";
            orxout() << endl;
        }

        orxout() << "  Attached ShipParts:" << endl;
        for(unsigned int i=0; i < this->partList_.size(); i++)
        {
            orxout() << "  " << i << ": " << this->partList_[i] << " (" << this->partList_[i]->getName() << ")" << endl;
        }*/


        //int collisionShapeIndex = this->isMyCollisionShape(cs);
        //orxout() << collisionShapeIndex << endl;

        //orxout() << "ShipPart of Entity " << cs->getUserPointer() << ": " << this->getPartOfEntity((StaticEntity*)(cs->getUserPointer())) << endl;

        orxout() << "CP_start" << endl;

        if (this->getPartOfEntity((StaticEntity*)(cs->getUserPointer())) != NULL)
            this->getPartOfEntity((StaticEntity*)(cs->getUserPointer()))->handleHit(damage, healthdamage, shielddamage, originator);
        else
            SpaceShip::damage(damage, healthdamage, shielddamage, originator, cs);

        orxout() << "CP_end" << endl;

        /*
        // Applies multiplier given by the DamageBoost Pickup.
        if (originator)
            damage *= originator->getDamageMultiplier();

        if (this->getGametype() && this->getGametype()->allowPawnDamage(this, originator))
        {
            if (shielddamage >= this->getShieldHealth())
            {
                this->setShieldHealth(0);
                this->setHealth(this->health_ - (healthdamage + damage));
            }
            else
            {
                this->setShieldHealth(this->shieldHealth_ - shielddamage);

                // remove remaining shieldAbsorpton-Part of damage from shield
                shielddamage = damage * this->shieldAbsorption_;
                shielddamage = std::min(this->getShieldHealth(),shielddamage);
                this->setShieldHealth(this->shieldHealth_ - shielddamage);

                // set remaining damage to health
                this->setHealth(this->health_ - (damage - shielddamage) - healthdamage);
            }

            this->lastHitOriginator_ = originator;
        }*/
    }

    /**
    @brief
        Add a ShipPart to the SpaceShip.
    @param engine
        A pointer to the ShipPart to be added.
    */
    void ModularSpaceShip::addShipPart(ShipPart* part)
    {
        OrxAssert(part != NULL, "The ShipPart cannot be NULL.");
        this->partList_.push_back(part);
        part->setParent(this);
        //part->addToSpaceShip(this); //FIXME: (noep) add
        this->updatePartAssignment();
    }

    /**
    @brief
        Get the i-th ShipPart of the SpaceShip.
    @return
        Returns a pointer to the i-the ShipPart. NULL if there is no ShipPart with that index.
    */
    ShipPart* ModularSpaceShip::getShipPart(unsigned int index)
    {
        if(this->partList_.size() >= index)
            return NULL;
        else
            return this->partList_[index];
    }

    /**
    @brief
        Check whether the SpaceShip has a particular Engine.
    @param engine
        A pointer to the Engine to be checked.
    */
    bool ModularSpaceShip::hasShipPart(ShipPart* part) const
    {
        for(unsigned int i = 0; i < this->partList_.size(); i++)
        {
            if(this->partList_[i] == part)
                return true;
        }
        return false;
    }

    void ModularSpaceShip::removeShipPart(ShipPart* part)
    {
        // Remove the part from the partList_
        std::vector<ShipPart*>::iterator it = this->partList_.begin();
        for(unsigned int i = 0; i < this->partList_.size(); i++)
        {
            if(this->partList_[i] == part)
                this->partList_.erase(it);
            it++;
        }
        // Remove the part-entity assignment and detach the Entity of this ShipPart
        for (std::map<StaticEntity*, ShipPart*>::iterator itt = this->partMap_.begin(); itt != this->partMap_.end(); ++itt)
        {
            if (itt->second == part)
            {
                this->detach(itt->first);
                this->partMap_.erase(itt);
            }
        }
    }

}

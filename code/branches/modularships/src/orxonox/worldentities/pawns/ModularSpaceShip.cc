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

#include "ShipPart.h"



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

    }

    //FIXME: (noep) finish
    // void ModularSpaceShip::attach

    void ModularSpaceShip::damage(float damage, float healthdamage, float shielddamage, Pawn* originator, const btCollisionShape* cs)
    {
        orxout() << "Mdamage(): Collision detected on " << this->getRadarName() << ", btCS*: " << cs << endl;
        orxout() << "Attached parts:" << endl;
        for(unsigned int i=0; i < this->partList_.size(); i++)
        {
            orxout() << "  " << i << ": " << this->partList_[i] << " (" << this->partList_[i]->getName() << ")" << endl;
        }

        int collisionShapeIndex = this->isMyCollisionShape(cs);
        orxout() << collisionShapeIndex << endl;

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
        }
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

}

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
 *      ...
 *
 */

#include "OrxonoxStableHeaders.h"
#include "Pawn.h"

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "util/Math.h"
#include "objects/infos/PlayerInfo.h"
#include "objects/gametypes/Gametype.h"


namespace orxonox
{
    CreateFactory(Pawn);

    Pawn::Pawn(BaseObject* creator) : ControllableEntity(creator)
    {
        RegisterObject(Pawn);

        this->bAlive_ = false;

        this->health_ = 0;
        this->maxHealth_ = 0;
        this->initialHealth_ = 0;

        this->lastHitOriginator_ = 0;

        //WeaponSystem
        weaponSystem_ = new WeaponSystem(this);
        /*
        WeaponSet * weaponSet1 = new WeaponSet(this,1);
        this->weaponSystem_->attachWeaponSet(weaponSet1);
        //totally bad solution...
        weaponSet1->setParentWeaponSystem(weaponSystem_);
        */

        this->registerVariables();
    }

    Pawn::~Pawn()
    {
    }

    void Pawn::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(Pawn, XMLPort, xmlelement, mode);

        XMLPortParam(Pawn, "health", setHealth, getHealht, xmlelement, mode).defaultValues(100);
        XMLPortParam(Pawn, "maxhealth", setMaxHealth, getMaxHealth, xmlelement, mode).defaultValues(200);
        XMLPortParam(Pawn, "initialhealth", setInitialHealth, getInitialHealth, xmlelement, mode).defaultValues(100);

        XMLPortObject(Pawn, WeaponSlot, "weaponslots", setWeaponSlot, getWeaponSlot, xmlelement, mode);
        XMLPortObject(Pawn, WeaponSet, "weaponsets", setWeaponSet, getWeaponSet, xmlelement, mode);
        XMLPortObject(Pawn, WeaponPack, "weapons", setWeaponPack, getWeaponPack, xmlelement, mode);

        }

    void Pawn::registerVariables()
    {
        REGISTERDATA(this->bAlive_, network::direction::toclient);
        REGISTERDATA(this->health_, network::direction::toclient);
    }

    void Pawn::tick(float dt)
    {
        SUPER(Pawn, tick, dt);

        if (this->health_ <= 0)
            this->death();
    }

    void Pawn::setHealth(float health)
    {
        this->health_ = min(health, this->maxHealth_);
    }

    void Pawn::damage(float damage, Pawn* originator)
    {
        this->setHealth(this->health_ - damage);
        this->lastHitOriginator_ = originator;

        // play damage effect
    }

    void Pawn::hit(Pawn* originator, const Vector3& force, float damage)
    {
        this->damage(damage, originator);
        this->setVelocity(this->getVelocity() + force);

        // play hit effect
    }

    void Pawn::kill()
    {
        this->damage(this->health_);
        this->death();
    }

    void Pawn::spawn()
    {
        // play spawn effect
    }

    void Pawn::death()
    {
        this->bAlive_ = false;
        if (this->getGametype())
            this->getGametype()->pawnKilled(this, this->lastHitOriginator_);
        if (this->getPlayer())
            this->getPlayer()->stopControl(this);

        delete this;

        // play death effect
    }

    void Pawn::fire(WeaponMode::Enum fireMode)
    {
COUT(0) << "Pawn::fire" << std::endl;
        if (this->weaponSystem_)
            this->weaponSystem_->fire(fireMode);
    }

    void Pawn::postSpawn()
    {
        this->setHealth(this->initialHealth_);
        this->spawn();
    }



    void Pawn::setWeaponSlot(WeaponSlot * wSlot)
    {   
COUT(0) << "Pawn::setWeaponSlot" << std::endl;
        this->weaponSystem_->attachWeaponSlot(wSlot);   }
    WeaponSlot * Pawn::getWeaponSlot(unsigned int index) const
    {   return this->weaponSystem_->getWeaponSlotPointer(index);    }
    
    void Pawn::setWeaponPack(WeaponPack * wPack)
    {   
COUT(0) << "Pawn::setWeaponPack" << std::endl;
        wPack->setParentWeaponSystem(this->weaponSystem_);
        wPack->setParentWeaponSystemToAllWeapons(this->weaponSystem_);
        this->weaponSystem_->attachWeaponPack( wPack,wPack->getFireMode() );
        wPack->attachNeededMunitionToAllWeapons();
    }

    WeaponPack * Pawn::getWeaponPack(unsigned int firemode) const
    {   return this->weaponSystem_->getWeaponPackPointer(firemode);    }
    
    void Pawn::setWeaponSet(WeaponSet * wSet)
    {   
COUT(0) << "Pawn::setWeaponSet" << std::endl;
        this->weaponSystem_->attachWeaponSet(wSet);   }
    WeaponSet * Pawn::getWeaponSet(unsigned int index) const
    {   return this->weaponSystem_->getWeaponSetPointer(index);    }

}

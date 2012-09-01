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
 *      Joel Smely
 *   Co-authors:
 *      simonmie
 *
 */

/**
    @file LightningGun.h
    @brief Implementation of the LightningGun class.
*/

#include "LightningGun.h"

#include "core/CoreIncludes.h"
#include "weaponsystem/Weapon.h"
#include "weaponsystem/WeaponPack.h"
#include "weaponsystem/WeaponSystem.h"
#include "worldentities/pawns/Pawn.h"

#include "weapons/projectiles/LightningGunProjectile.h"

namespace orxonox
{
    CreateFactory(LightningGun);

    LightningGun::LightningGun(BaseObject* creator) : WeaponMode(creator)
    {
        RegisterObject(LightningGun);

        this->reloadTime_ = 1.0f;
        this->damage_ = 0.0f;
        this->speed_ = 700.0f;

        this->setMunitionName("LaserMunition");
        this->setDefaultSound("sounds/Weapon_LightningGun.ogg");
    }

    LightningGun::~LightningGun()
    {
    }

    /**
    @brief
        Fires the weapon. Creates a projectile and fires it.
    */
    void LightningGun::fire()
    {
        LightningGunProjectile* projectile = new LightningGunProjectile(this);
        projectile->setMaterial("Flares/LightningBall_");

        this->computeMuzzleParameters(this->getWeapon()->getWeaponPack()->getWeaponSystem()->getPawn()->getAimPosition());
        projectile->setOrientation(this->getMuzzleOrientation());
        projectile->setPosition(this->getMuzzlePosition());
        projectile->setVelocity(this->getWeapon()->getWeaponPack()->getWeaponSystem()->getPawn()->getVelocity() + this->getMuzzleDirection() * this->speed_);

        projectile->setShooter(this->getWeapon()->getWeaponPack()->getWeaponSystem()->getPawn());
        projectile->setDamage(this->getDamage());
        projectile->setShieldDamage(this->getShieldDamage());
        projectile->setHealthDamage(this->getHealthDamage());
    }
}

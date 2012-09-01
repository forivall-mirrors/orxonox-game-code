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
 *      ...
 *
 */

#include "LightningGun.h"

#include "core/CoreIncludes.h"
#include "weapons/projectiles/LightningGunProjectile.h"
#include "weaponsystem/Weapon.h"
#include "weaponsystem/WeaponPack.h"
#include "weaponsystem/WeaponSystem.h"
#include "worldentities/pawns/Pawn.h"

namespace orxonox
{
    CreateFactory(LightningGun);

    LightningGun::LightningGun(BaseObject* creator) : WeaponMode(creator)
    {
        RegisterObject(LightningGun);

        this->reloadTime_ = 1;
        this->damage_ = 100;
        this->speed_ = 150;

        this->setMunitionName("LaserMunition");
        this->setDefaultSound("sounds/Weapon_LightningGun.ogg");
    }

    LightningGun::~LightningGun()
    {
    }

    void LightningGun::fire()
    {
        LightningGunProjectile* projectile = new LightningGunProjectile(this);
        projectile->setMaterial("Flares/LightningBall_");

        this->computeMuzzleParameters(this->getWeapon()->getWeaponPack()->getWeaponSystem()->getPawn()->getAimPosition());
        projectile->setOrientation(this->getMuzzleOrientation());
        projectile->setPosition(this->getMuzzlePosition());
        projectile->setVelocity(this->getMuzzleDirection() * this->speed_);
        projectile->setAcceleration(this->getMuzzleDirection() * 1000);

        projectile->setOwner(this->getWeapon()->getWeaponPack()->getWeaponSystem()->getPawn());
        projectile->setDamage(this->getDamage());
    }
}

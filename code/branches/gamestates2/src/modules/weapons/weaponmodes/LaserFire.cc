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
 *      Martin Polak
 *   Co-authors:
 *      ...
 *
 */

#include "LaserFire.h"

#include "core/CoreIncludes.h"
#include "weapons/projectiles/ParticleProjectile.h"
#include "weaponsystem/Weapon.h"
#include "weaponsystem/WeaponPack.h"
#include "weaponsystem/WeaponSystem.h"

namespace orxonox
{
    CreateFactory(LaserFire);

    LaserFire::LaserFire(BaseObject* creator) : WeaponMode(creator)
    {
        RegisterObject(LaserFire);

        this->reloadTime_ = 0.25;
        this->damage_ = 15;
        this->speed_ = 1250;

        this->setMunitionName("LaserMunition");
    }

    void LaserFire::fire()
    {
        ParticleProjectile* projectile = new ParticleProjectile(this);

        projectile->setOrientation(this->getMuzzleOrientation());
        projectile->setPosition(this->getMuzzlePosition());
        projectile->setVelocity(this->getMuzzleDirection() * this->speed_);

        projectile->setOwner(this->getWeapon()->getWeaponPack()->getWeaponSystem()->getPawn());
        projectile->setDamage(this->getDamage());
    }
}

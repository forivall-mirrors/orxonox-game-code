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

#include "OrxonoxStableHeaders.h"
#include "LaserGun.h"

#include "core/CoreIncludes.h"

#include "objects/weaponSystem/Munition.h"
#include "objects/weaponSystem/projectiles/ParticleProjectile.h"
#include "objects/weaponSystem/WeaponSystem.h"

namespace orxonox
{
    CreateFactory(LaserGun);

    LaserGun::LaserGun(BaseObject* creator) : Weapon(creator)
    {
        RegisterObject(LaserGun);

        this->speed_ = 1250;

    }

    LaserGun::~LaserGun()
    {
    }

    void LaserGun::reloadBullet()
    {
        this->bulletTimer(this->bulletLoadingTime_);
    }

    void LaserGun::reloadMagazine()
    {
        this->magazineTimer(this->magazineLoadingTime_);
    }

    void LaserGun::takeBullets()
    {
        this->munition_->removeBullets(1);
    }

    void LaserGun::takeMagazines()
    {
        this->munition_->removeMagazines(1);
    }

    void LaserGun::createProjectile()
    {
        BillboardProjectile* projectile = new ParticleProjectile(this);
        projectile->setOrientation(this->getWorldOrientation());
        projectile->setPosition(this->getWorldPosition());
        projectile->setVelocity(this->getWorldOrientation() * WorldEntity::FRONT * this->speed_);
        projectile->setOwner(this->getWeaponSystem()->getPawn());
    }
}

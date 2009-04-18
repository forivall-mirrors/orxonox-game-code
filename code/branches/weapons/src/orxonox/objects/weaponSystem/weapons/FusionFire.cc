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
#include "FusionFire.h"

#include "core/CoreIncludes.h"
#include "objects/weaponSystem/projectiles/BillboardProjectile.h"

#include "objects/weaponSystem/Weapon.h"
#include "objects/weaponSystem/WeaponPack.h"
#include "objects/weaponSystem/WeaponSystem.h"

namespace orxonox
{
    CreateFactory(FusionFire);

    FusionFire::FusionFire(BaseObject* creator) : WeaponMode(creator)
    {
        RegisterObject(FusionFire);

        this->reloadTime_ = 1.0;
        this->bParallelReload_ = false;
        this->damage_ = 40;
        this->speed_ = 1250;

        this->setMunitionName("FusionMunition");
    }

    void FusionFire::fire()
    {
        BillboardProjectile* projectile = new BillboardProjectile(this);

        projectile->setOrientation(this->getMuzzleOrientation());
        projectile->setPosition(this->getMuzzlePosition());
        projectile->setVelocity(this->getMuzzleDirection() * this->speed_);

        projectile->setOwner(this->getWeapon()->getWeaponPack()->getWeaponSystem()->getPawn());
        projectile->setDamage(this->getDamage());
        projectile->setColour(ColourValue(1.0f, 0.7f, 0.3f, 1.0f));
    }
}

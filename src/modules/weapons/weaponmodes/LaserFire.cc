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
 *      simonmie
 *
 */

/**
    @file LaserFire.h
    @brief Implementation of the LaserFire class.
*/

#include "LaserFire.h"

#include "core/CoreIncludes.h"

#include "weaponsystem/Weapon.h"
#include "weaponsystem/WeaponPack.h"
#include "weaponsystem/WeaponSystem.h"
#include "worldentities/pawns/Pawn.h"

#include "weapons/projectiles/ParticleProjectile.h"

namespace orxonox
{
    RegisterClass(LaserFire);

    LaserFire::LaserFire(Context* context) : WeaponMode(context)
    {
        RegisterObject(LaserFire);

        this->reloadTime_ = 0.25f;
        this->damage_ = 0.0f;
        this->speed_ = 750.0f;

        this->setMunitionName("LaserMunition");
    }

    /**
    @brief
        Fires the weapon. Creates a projectile and fires it.
    */
    void LaserFire::fire()
    {
        ParticleProjectile* projectile = new ParticleProjectile(this->getContext());

	this->computeMuzzleParameters(this->getWeapon()->getWeaponPack()->getWeaponSystem()->getPawn()->getAimPosition());
        projectile->setOrientation(this->getMuzzleOrientation());
        projectile->setPosition(this->getMuzzlePosition());
        projectile->setVelocity(this->getMuzzleDirection() * this->speed_);

        projectile->setShooter(this->getWeapon()->getWeaponPack()->getWeaponSystem()->getPawn());
        projectile->setDamage(this->getDamage());
        projectile->setShieldDamage(this->getShieldDamage());
        projectile->setHealthDamage(this->getHealthDamage());
    }
}

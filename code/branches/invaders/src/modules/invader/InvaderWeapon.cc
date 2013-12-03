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
 *      Florian Zinggeler
 *   Co-authors:
 *      --
 *
 */

/**
    @file InvaderWeapon.h
    @brief Implementation of the InvaderWeapon class.
*/

#include "InvaderWeapon.h"

#include "core/CoreIncludes.h"
// #include "core/XMLPort.h"
// #include "core/command/Executor.h"

#include "graphics/Model.h"
#include "weaponsystem/Weapon.h"
#include "weaponsystem/WeaponPack.h"
#include "weaponsystem/WeaponSystem.h"
#include "worldentities/WorldEntity.h"
#include "worldentities/pawns/Pawn.h"

#include "weapons/projectiles/Projectile.h"
#include "weapons/MuzzleFlash.h"

namespace orxonox
{
    RegisterClass(InvaderWeapon);

    InvaderWeapon::InvaderWeapon(Context* context) : HsW01(context)
    {
        RegisterObject(InvaderWeapon);
    }

    InvaderWeapon::~InvaderWeapon(){}

    void InvaderWeapon::shot()
    {
        assert( this->getWeapon() && this->getWeapon()->getWeaponPack() && this->getWeapon()->getWeaponPack()->getWeaponSystem() && this->getWeapon()->getWeaponPack()->getWeaponSystem()->getPawn() );

        // Create the projectile.
        Projectile* projectile = new Projectile(this->getContext());
        Model* model = new Model(projectile->getContext());
        model->setMeshSource(mesh_);
        model->setCastShadows(false);
        projectile->attach(model);
        model->setScale(5);

        this->computeMuzzleParameters(this->getWeapon()->getWeaponPack()->getWeaponSystem()->getPawn()->getAimPosition());
        // only shoot in foreward direction
        projectile->setOrientation(Quaternion(sqrt(0.5),0,sqrt(0.5),0));
        projectile->setPosition(this->getMuzzlePosition());
        // only shoot in foreward direction
        projectile->setVelocity(Vector3(1, 0, 0) * 2000);

        projectile->setShooter(this->getWeapon()->getWeaponPack()->getWeaponSystem()->getPawn());
        projectile->setDamage(this->getDamage());
        projectile->setShieldDamage(this->getShieldDamage());
        projectile->setHealthDamage(this->getHealthDamage());

        // Display the muzzle flash.
        this->InvaderWeapon::muzzleflash();
    }
}

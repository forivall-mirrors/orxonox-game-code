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
 *      Oliver Scheuss
 *   Co-authors:
 *      simonmie
 *
 */

/**
    @file RocketFire.h
    @brief Implementation of the RocketFire class.
*/

#include "RocketFire.h"

#include "core/CoreIncludes.h"
#include "util/Math.h"

#include "weaponsystem/Weapon.h"
#include "weaponsystem/WeaponPack.h"
#include "weaponsystem/WeaponSystem.h"
#include "worldentities/pawns/Pawn.h"

#include "weapons/projectiles/Rocket.h"

namespace orxonox
{
    RegisterClass(RocketFire);

    RocketFire::RocketFire(Context* context) : WeaponMode(context)
    {
        RegisterObject(RocketFire);

        this->reloadTime_ = 0.20f;
        this->bParallelReload_ = false;
        this->damage_ = 0.0f;
        this->speed_ = 500.0f;

        this->setMunitionName("RocketMunition");
        // The firing sound of the Rocket is played in Rocket.cc (because of OpenAl sound positioning)
    }

    RocketFire::~RocketFire()
    {
    }

    /**
    @brief
        Fires the weapon. Creates the Rocket and fires it.
    */
    void RocketFire::fire()
    {
        Rocket* rocket = new Rocket(this->getContext());

        this->computeMuzzleParameters(this->getWeapon()->getWeaponPack()->getWeaponSystem()->getPawn()->getAimPosition());
        rocket->setOrientation(this->getMuzzleOrientation());
        rocket->setPosition(this->getMuzzlePosition());
        rocket->setVelocity(this->getMuzzleDirection() * this->speed_);
        rocket->scale(2);

        rocket->setShooter(this->getWeapon()->getWeaponPack()->getWeaponSystem()->getPawn());
        rocket->setDamage(this->getDamage());
        rocket->setShieldDamage(this->getShieldDamage());
        rocket->setHealthDamage(this->getHealthDamage());
    }
}

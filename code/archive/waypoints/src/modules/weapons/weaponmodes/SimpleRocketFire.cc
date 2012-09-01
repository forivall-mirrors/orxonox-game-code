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
 *      Gabriel Nadler
 *   Co-authors:
 *      Oliver Scheuss
 *      simonmie
 *
 */

/**
    @file BasicProjectile.h
    @brief Implementation of the BasicProjectile class.
*/

#include "SimpleRocketFire.h"

#include "core/CoreIncludes.h"
#include "util/Math.h"

#include "weaponsystem/Weapon.h"
#include "weaponsystem/WeaponPack.h"
#include "weaponsystem/WeaponSystem.h"
#include "worldentities/pawns/Pawn.h"
#include "sound/WorldSound.h"

#include "weapons/RocketController.h"
#include "weapons/projectiles/SimpleRocket.h"

namespace orxonox
{

    CreateFactory(SimpleRocketFire);

    SimpleRocketFire::SimpleRocketFire(BaseObject* creator) : WeaponMode(creator)
    {
        RegisterObject(SimpleRocketFire);

        this->reloadTime_ = 1.0f;
        this->bParallelReload_ = false;
        this->damage_ = 0.0f;
        this->speed_ = 500.0f;

        this->setMunitionName("RocketMunition");
        this->setDefaultSoundWithVolume("sounds/Rocket_launch.ogg",0.4f);
        // The firing sound of the Rocket is played in Rocket.cc (because of OpenAl sound positioning)
    }

    SimpleRocketFire::~SimpleRocketFire()
    {
    }

    /**
    @brief
        Fires the weapon. Creates the SimpleRocket and a RocketController to steer it and fires it.
    */
    void SimpleRocketFire::fire()
    {
        RocketController* controller = new RocketController(this);
        SimpleRocket* rocket = controller->getRocket();
        this->computeMuzzleParameters(this->getWeapon()->getWeaponPack()->getWeaponSystem()->getPawn()->getAimPosition());
        rocket->setOrientation(this->getWeapon()->getWeaponPack()->getWeaponSystem()->getPawn()->getWorldOrientation());
        rocket->setPosition(this->getMuzzlePosition());
        rocket->setVelocity(this->getMuzzleDirection()*this->speed_);
        rocket->setShooter(this->getWeapon()->getWeaponPack()->getWeaponSystem()->getPawn());

        rocket->setDamage(this->damage_);
        rocket->setShieldDamage(this->getShieldDamage());
        rocket->setHealthDamage(this->getHealthDamage());

        WorldEntity* pawn = static_cast<ControllableEntity*>(this->getWeapon()->getWeaponPack()->getWeaponSystem()->getPawn())->getTarget();
        if (pawn) controller->setTarget(pawn);
    }
}

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
 *      ...
 *
 */

#include "SimpleRocketFire.h"

#include "util/Math.h"
#include "core/CoreIncludes.h"
#include "weapons/RocketController.h"

#include "weaponsystem/Weapon.h"
#include "weaponsystem/WeaponPack.h"
#include "weaponsystem/WeaponSystem.h"
#include "worldentities/pawns/Pawn.h"
#include "sound/WorldSound.h"

namespace orxonox
{

    CreateFactory(SimpleRocketFire);

    SimpleRocketFire::SimpleRocketFire(BaseObject* creator) : WeaponMode(creator)
    {
        RegisterObject(SimpleRocketFire);

        this->reloadTime_ = 1;
        this->bParallelReload_ = false;
        this->damage_ = 100;
        this->speed_ = 500;

        this->setMunitionName("RocketMunition");
        this->setDefaultSoundWithVolume("sounds/Rocket_launch.ogg",0.4f);
        // The firing sound of the Rocket is played in Rocket.cc (because of OpenAl sound positioning)
    }

    SimpleRocketFire::~SimpleRocketFire()
    {
    }

    void SimpleRocketFire::fire()
    {
        RocketController* con = new RocketController(this);
        SimpleRocket* rocket = con->getRocket();
        this->computeMuzzleParameters(this->getWeapon()->getWeaponPack()->getWeaponSystem()->getPawn()->getAimPosition());
        rocket->setOrientation(this->getWeapon()->getWeaponPack()->getWeaponSystem()->getPawn()->getWorldOrientation());
        rocket->setPosition(this->getMuzzlePosition());
        rocket->setVelocity(this->getMuzzleDirection()*this->speed_);
        rocket->setOwner(this->getWeapon()->getWeaponPack()->getWeaponSystem()->getPawn());
        rocket->setDamage(this->damage_);
        WorldEntity* pawnn=static_cast<ControllableEntity*>(this->getWeapon()->getWeaponPack()->getWeaponSystem()->getPawn())->getTarget();
        if (pawnn) con->setTarget(pawnn);
    }
}

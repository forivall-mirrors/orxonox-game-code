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

namespace orxonox
{
    CreateFactory(SimpleRocketFire);

    SimpleRocketFire::SimpleRocketFire(BaseObject* creator) : WeaponMode(creator)
    {
        RegisterObject(SimpleRocketFire);

        this->reloadTime_ = 0.20f;
        this->bParallelReload_ = false;
        this->damage_ = 100;
        this->speed_ = 50;

        this->setMunitionName("LaserMunition");
        // The firing sound of the Rocket is played in Rocket.cc (because of OpenAl sound positioning)
    }

    SimpleRocketFire::~SimpleRocketFire()
    {
    }

    void SimpleRocketFire::fire()
    {
        //SimpleRocket* rocket = new SimpleRocket(this);
		RocketController* con = new RocketController(this);
		SimpleRocket* rocket = con->getRocket();
		this->computeMuzzleParameters(this->getWeapon()->getWeaponPack()->getWeaponSystem()->getPawn()->getAimPosition());
		rocket->setOrientation(this->getMuzzleOrientation());
		Vector3 pos = this->getMuzzlePosition() + (this->getMuzzleOrientation()* Vector3(-2, -3, -2));
        rocket->setPosition(pos);
		rocket->setVelocity(this->getMuzzleDirection()*this->speed_);
		//Ogre::Vector3 vec = this->getMuzzlePosition();
		//vec.normalise();
		//rocket->setAcceleration(100*vec);
		rocket->setOwner(this->getWeapon()->getWeaponPack()->getWeaponSystem()->getPawn());
		//con->getRocket()->setPosition(this->getMuzzlePosition());
	//con->setControllableEntity(dynamic_cast<rocket*>(ControllableEntity));
	//con->getControllableEntity()->setPosition(this->getMuzzlePosition());

        /* this->computeMuzzleParameters(this->getWeapon()->getWeaponPack()->getWeaponSystem()->getPawn()->getAimPosition());
        rocket->setOrientation(this->getMuzzleOrientation());
        rocket->setPosition(this->getMuzzlePosition());
		rocket->scale(10);
        rocket->setVelocity(this->getMuzzleDirection() * this->speed_);
        rocket->setOwner(this->getWeapon()->getWeaponPack()->getWeaponSystem()->getPawn());
        rocket->setDamage(this->getDamage()); 
        rocket->scale(2);
		*/
    }
}

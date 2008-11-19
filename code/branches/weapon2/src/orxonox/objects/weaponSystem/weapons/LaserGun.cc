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


#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "util/Debug.h"

#include "LaserGun.h"


namespace orxonox
{
    LaserGun::LaserGun(BaseObject* creator) : Weapon(creator)
    {
        RegisterObject(LaserGun);

        //set weapon properties here
        //this->setLoadingTime(0.5);

        //Hack --> will be loaded by XML
        this->attachNeededMunition("LaserGunMunition");
    }

    LaserGun::~LaserGun()
    {
    }

    void LaserGun::fire()
    {
        if ( this->getBulletReadyToShoot() && this->getMagazineReadyToShoot() )
        {



            //take munition
            this->getAttachedMunition()->removeBullets(1,this);
            Weapon::bulletTimer();
            this->setBulletReadyToShoot(false);

            //create projectile
            BillboardProjectile* projectile = new ParticleProjectile(this,this);
            //projectile->setColour(this->getProjectileColour());
        }
        else
        {
            //actions, when weapon is not reloaded if there are some
        }
    }

    void LaserGun::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {

    }



}

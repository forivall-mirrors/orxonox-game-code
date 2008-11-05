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


namespace orxonox
{
    LaserGun::LaserGun(BaseObject* creator) : Weapon(creator)
    {
        RegisterObject(LaserGun);

        this->attachNeededMunition();

        //set weapon properties here
        this->projectileColor_ = ColourValue(1.0, 1.0, 0.5)
        this->loadingTime_ = 0.5;
    }

    LaserGun::~LaserGun()
    {
    }

    LaserGun::fire()
    {
        if { this->weaponReadyToShoot_ }
        {
            this->weaponReadyToShoot_ = false;
            //take munition
            //this->pointerToMunition_->

            this->reloadTimer_.setTimer( loadingTime_ , false , this , &this->reloaded );

            BillboardProjectile* projectile = new ParticleProjectile(this);
            projectile->setColour(this->getProjectileColour());
        }
        else
        {
            //actions, when weapon is not reloaded
        }
    }

    void LaserGun::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {

    }

    ColorValue LaserGun::getProjectileColor()
    {
        return projectileColor_;
    }

    void attachNeededMunition(Munition *pointerToMunition)
    {
        //if munition type already exist attach it, else create a new one of this type and attach it to the weapon and to the WeaponSystem
        if ( this->parentWeaponSystem_->munitionSet_[laserGunMunition] )
            this->pointerToMunition_ = pointerToMunition;
        else
        {
            this->pointerToMunition_ = new LaserGunMunition;

        }
    }
}

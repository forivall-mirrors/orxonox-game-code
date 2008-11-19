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

#include "Weapon.h"

namespace orxonox
{
    Weapon::Weapon(BaseObject* creator) : BaseObject(creator)
    {
        RegisterObject(Weapon);
        this->bulletReadyToShoot_ = true;
        this->magazineReadyToShoot_ = true;
        this->setParentWeaponSystem();
    }

    Weapon::~Weapon()
    {
    }


    void Weapon::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(Weapon, XMLPort, xmlelement, mode);
    }

    void Weapon::fire()
    {

    }


    void Weapon::bulletTimer()
    {
        this->bulletReloadTimer_.setTimer( this->bulletLoadingTime_ , false , this , createExecutor(createFunctor(&Weapon::bulletReloaded)));
    }
    void Weapon::magazineTimer()
    {
        this->magazineReloadTimer_.setTimer( this->magazineLoadingTime_ , false , this , createExecutor(createFunctor(&Weapon::magazineReloaded)));
    }

    void Weapon::bulletReloaded()
    { this->bulletReadyToShoot_ = true; }

    void Weapon::magazineReloaded()
    { this->magazineReadyToShoot_ = true; }


    void Weapon::attachNeededMunition(std::string munitionName)
    {
        //if munition type already exists attach it, else create a new one of this type and attach it to the weapon and to the WeaponSystem
        if ( this->parentWeaponSystem_->getMunitionType(munitionName) )
            this->munition_ = this->parentWeaponSystem_->getMunitionType(munitionName);
        else
        {
            //create new munition with identifier
            this->munitionIdentifier_ = ClassByString(munitionName);
            this->munition_ = this->munitionIdentifier_.fabricate(this);
            this->parentWeaponSystem_->setNewMunition(munitionName, this->munition_);
        }
    }


    /*get and set functions
     *
     */
    void Weapon::setParentWeaponSystem()
    {   this->parentWeaponSystem_ = this->parentWeaponSlot_->getParentWeaponSet()->getParentWeaponSystem(); }

    Munition * Weapon::getAttachedMunition()
    {   return this->munition_; }

    void Weapon::setBulletLoadingTime(float loadingTime)
    {   this->bulletLoadingTime_ = loadingTime;   }

    float Weapon::getBulletLoadingTime()
    {   return this->bulletLoadingTime_;  }

    void Weapon::setMagazineLoadingTime(float loadingTime)
    {   this->magazineLoadingTime_ = loadingTime;   }

    float Weapon::getMagazineLoadingTime()
    {   return this->magazineLoadingTime_;  }

    void Weapon::setBulletReadyToShoot(bool b)
    {   this->bulletReadyToShoot_ = b;   }

    bool Weapon::getBulletReadyToShoot()
    {   return this->bulletReadyToShoot_;    }

    void Weapon::setMagazineReadyToShoot(bool b)
    {   this->magazineReadyToShoot_ = b;   }

    bool Weapon::getMagazineReadyToShoot()
    {   return this->magazineReadyToShoot_;    }

    Timer<Weapon> * Weapon::getBulletTimer()
    {   return &this->bulletReloadTimer_;   }

    Timer<Weapon> * Weapon::getMagazineTimer()
    {   return &this->magazineReloadTimer_;   }
}

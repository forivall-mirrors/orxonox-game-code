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
    CreateFactory(Weapon);

    Weapon::Weapon(BaseObject* creator) : BaseObject(creator)
    {
        RegisterObject(Weapon);
        this->bulletReadyToShoot_ = true;
        this->magazineReadyToShoot_ = true;
        this->parentWeaponSystem_ = 0;
        this->parentWeaponSlot_ = 0;
        this->munition_ = 0;
    }

    Weapon::~Weapon()
    {
    }


    void Weapon::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(Weapon, XMLPort, xmlelement, mode);
        XMLPortParam(Weapon, "munitionType", setMunitionType, getMunitionType, xmlelement, mode);
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
        if (this->parentWeaponSystem_)
        {
            Munition* munition = this->parentWeaponSystem_->getMunitionType(munitionName);
            if ( munition )
                this->munition_ = munition;
            else
            {
                //create new munition with identifier
COUT(0) << "Weapon::attachNeededMunition, create new Munition of Type " << munitionName << std::endl;
                this->munitionIdentifier_ = ClassByString(munitionName);
                this->munition_ = this->munitionIdentifier_.fabricate(this);
                this->parentWeaponSystem_->setNewMunition(munitionName, this->munition_);
            }
        }
    }


    /*get and set functions
     *
     */

    void Weapon::setMunitionType(std::string munitionType)
    {   this->munitionType_ = munitionType; }

    std::string Weapon::getMunitionType()
    {   return this->munitionType_;  }

    Munition * Weapon::getAttachedMunition(std::string munitionType)
    {   
COUT(0) << "Weapon::getAttachedMunition, parentWeaponSystem_="<< this->parentWeaponSystem_ << std::endl;   
        this->parentWeaponSystem_->getMunitionType(munitionType);
COUT(0) << "Weapon::getAttachedMunition, munition_="<< this->munition_ << std::endl;
return this->munition_; }

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

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

    Weapon::Weapon(BaseObject* creator) : StaticEntity(creator)
    {
        RegisterObject(Weapon);
        this->bulletReadyToShoot_ = true;
        this->magazineReadyToShoot_ = true;
        this->parentWeaponSystem_ = 0;
        this->attachedToWeaponSlot_ = 0;
        this->munition_ = 0;
        this->bulletLoadingTime_ = 0;
        this->magazineLoadingTime_ = 0;
        this->bReloading_ = false;

        this->setObjectMode(0x0);
    }

    Weapon::~Weapon()
    {
    }


    void Weapon::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(Weapon, XMLPort, xmlelement, mode);
        XMLPortParam(Weapon, "munitionType", setMunitionType, getMunitionType, xmlelement, mode);
        XMLPortParam(Weapon, "bulletLoadingTime", setBulletLoadingTime, getBulletLoadingTime, xmlelement, mode);
        XMLPortParam(Weapon, "magazineLoadingTime", setMagazineLoadingTime, getMagazineLoadingTime, xmlelement, mode);
    }

    void Weapon::setWeapon()
    {
        this->munition_->fillBullets();
        this->munition_->fillMagazines();
    }


    void Weapon::fire()
    {
//COUT(0) << "LaserGun::fire, this=" << this << std::endl;
        if ( this->bulletReadyToShoot_ && this->magazineReadyToShoot_ && !this->bReloading_)
        {
//COUT(0) << "LaserGun::fire - ready to shoot" << std::endl;
//COUT(0) << "LaserGun::fire - bullets" << this->munition_->bullets() << std::endl;
            this->bulletReadyToShoot_ = false;
            if ( this->munition_->bullets() > 0)
            {
                //shoot
                this->takeBullets();
                this->createProjectile();
            }
            //if there are no bullets, but magazines
            else if ( this->munition_->magazines() > 0 && this->munition_->bullets() == 0 )
            {
//COUT(0) << "LaserGun::fire - no bullets" << std::endl;
                this->takeMagazines();
            }
            else
            {
//COUT(0) << "LaserGun::fire - no magazines" << std::endl;
                //actions
            }
        }
        else
        {
//COUT(0) << "LaserGun::fire - weapon not reloaded - bullets remaining:" << this->munition_->bullets() << std::endl;
            //actions
        }

    }


    void Weapon::bulletTimer(float bulletLoadingTime)
    {
//COUT(0) << "Weapon::bulletTimer started" << std::endl;
        this->bReloading_ = true;
        this->bulletReloadTimer_.setTimer( bulletLoadingTime , false , this , createExecutor(createFunctor(&Weapon::bulletReloaded)));
    }
    void Weapon::magazineTimer(float magazineLoadingTime)
    {
//COUT(0) << "Weapon::magazineTimer started" << std::endl;
        this->bReloading_ = true;
        this->magazineReloadTimer_.setTimer( magazineLoadingTime , false , this , createExecutor(createFunctor(&Weapon::magazineReloaded)));
    }

    void Weapon::bulletReloaded()
    {
        this->bReloading_ = false;
        this->bulletReadyToShoot_ = true;
    }

    void Weapon::magazineReloaded()
    {
        this->bReloading_ = false;
        this->munition_->fillBullets();
        this->magazineReadyToShoot_ = true;
        this->bulletReadyToShoot_ = true;
    }


    void Weapon::attachNeededMunition(std::string munitionName)
    {
//COUT(0) << "Weapon::attachNeededMunition, parentWeaponSystem=" << this->parentWeaponSystem_ << std::endl;
        //if munition type already exists attach it, else create a new one of this type and attach it to the weapon and to the WeaponSystem
        if (this->parentWeaponSystem_)
        {
//COUT(0) << "Weapon::attachNeededMunition " << munitionName << std::endl;
            Munition* munition = this->parentWeaponSystem_->getMunitionType(munitionName);
            if ( munition )
                this->munition_ = munition;
            else
            {
                //create new munition with identifier
//COUT(0) << "Weapon::attachNeededMunition, create new Munition of Type " << munitionName << std::endl;
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

    const std::string Weapon::getMunitionType()
    {   return this->munitionType_;  }

    void Weapon::setBulletLoadingTime(float loadingTime)
    {   this->bulletLoadingTime_ = loadingTime; }

    const float Weapon::getBulletLoadingTime()
    {   return this->bulletLoadingTime_;  }

    void Weapon::setMagazineLoadingTime(float loadingTime)
    {   this->magazineLoadingTime_ = loadingTime; }

    const float Weapon::getMagazineLoadingTime()
    {   return this->magazineLoadingTime_;  }


    Munition * Weapon::getAttachedMunition(std::string munitionType)
    {
//COUT(0) << "Weapon::getAttachedMunition, parentWeaponSystem_="<< this->parentWeaponSystem_ << std::endl;
        this->munition_ = this->parentWeaponSystem_->getMunitionType(munitionType);
//COUT(0) << "Weapon::getAttachedMunition, munition_="<< this->munition_ << std::endl;
        return this->munition_;
    }

    void Weapon::takeBullets() { };
    void Weapon::createProjectile() { };
    void Weapon::takeMagazines() { };

}

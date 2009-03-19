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
        this->bulletLoadingTime_ = 0;
        this->magazineLoadingTime_ = 0;
        this->bReloading_ = false;
        this->bulletAmount_= 0;
        this->magazineAmount_ = 0;
        this->munition_ = 0;
        this->unlimitedMunition_ = false;
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
        XMLPortParam(Weapon, "bSharedMunition", setSharedMunition, getSharedMunition, xmlelement, mode);
        XMLPortParam(Weapon, "bullets", setBulletAmount, getBulletAmount, xmlelement, mode);
        XMLPortParam(Weapon, "magazines", setMagazineAmount, getMagazineAmount, xmlelement, mode);
        XMLPortParam(Weapon, "unlimitedMunition", setUnlimitedMunition, getUnlimitedMunition, xmlelement, mode);
    }

    void Weapon::setWeapon()
    {
        this->munition_->fillBullets();
        this->munition_->fillMagazines();
    }

    void Weapon::setMunition()
    {
        this->munition_->setMaxBullets(this->bulletAmount_);
        this->munition_->setMaxMagazines(this->magazineAmount_);
    }

    void Weapon::fire()
    {
COUT(0) << "Weapon::fire" << std::endl;
        if ( this->bulletReadyToShoot_ && this->magazineReadyToShoot_ && !this->bReloading_)
        {
COUT(0) << "Weapon::fire 2" << std::endl;
            this->bulletReadyToShoot_ = false;
            if ( this->unlimitedMunition_== true )
            {
COUT(0) << "Weapon::fire 3" << std::endl;
                //shoot
                this->reloadBullet();
                this->createProjectile();
            }
            else
            {
COUT(0) << "Weapon::fire 4" << std::endl;
                if ( this->munition_->bullets() > 0)
                {
COUT(0) << "Weapon::fire 5" << std::endl;
                    //shoot
                    this->takeBullets();
                    this->reloadBullet();
                    this->createProjectile();
                }
                //if there are no bullets, but magazines
                else if ( this->munition_->magazines() > 0 && this->munition_->bullets() == 0 )
                {
COUT(0) << "Weapon::fire 6" << std::endl;
                    this->takeMagazines();
                    this->reloadMagazine();
                }
                else
                {
                    //no magazines
                }
            }
        }
        else
        {
COUT(0) << "Weapon::fire not reloaded" << std::endl;
            //weapon not reloaded
        }

    }

    /*
    * weapon reloading
    */
    void Weapon::bulletTimer(float bulletLoadingTime)
    {
        this->bReloading_ = true;
        this->bulletReloadTimer_.setTimer( bulletLoadingTime , false , this , createExecutor(createFunctor(&Weapon::bulletReloaded)));
    }
    void Weapon::magazineTimer(float magazineLoadingTime)
    {
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
        /*  if munition type already exists attach it, else create a new one of this type and attach it to the weapon and to the WeaponSystem
        *   if the weapon shares one munitionType put it into sharedMunitionSet else to munitionSet
        */
        if (this->parentWeaponSystem_)
        {
            if (this->bSharedMunition_ == false)
            {
                this->munitionIdentifier_ = ClassByString(munitionName);
                this->munition_ = this->munitionIdentifier_.fabricate(this);
                this->parentWeaponSystem_->setNewMunition(munitionName, this->munition_);
            }
            else
            {
                //getMunitionType returns 0 if there is no such munitionType
                Munition* munition = this->parentWeaponSystem_->getMunitionType(munitionName);
                if ( munition )
                    this->munition_ = munition;
                else
                {
                    //create new munition with identifier because there is no such munitionType
                    this->munitionIdentifier_ = ClassByString(munitionName);
                    this->munition_ = this->munitionIdentifier_.fabricate(this);
                    this->parentWeaponSystem_->setNewSharedMunition(munitionName, this->munition_);
                }
            }
            this->setMunition();
        }
    }


    Munition * Weapon::getAttachedMunition(std::string munitionType)
    {
        this->munition_ = this->parentWeaponSystem_->getMunitionType(munitionType);
        return this->munition_;
    }


    //these function are defined in the weapon classes
    void Weapon::takeBullets() { };
    void Weapon::createProjectile() { };
    void Weapon::takeMagazines() { };
    void Weapon::reloadBullet() { };
    void Weapon::reloadMagazine() { };


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

    void Weapon::setSharedMunition(bool bSharedMunition)
    {   this->bSharedMunition_ = bSharedMunition; }

    const bool Weapon::getSharedMunition()
    {   return this->bSharedMunition_;  }

    void Weapon::setBulletAmount(unsigned int amount)
    {   this->bulletAmount_ = amount; }

    const unsigned int Weapon::getBulletAmount()
    {   return this->bulletAmount_;  }

    void Weapon::setMagazineAmount(unsigned int amount)
    {   this->magazineAmount_ = amount; }

    const unsigned int Weapon::getMagazineAmount()
    {   return this->magazineAmount_;  }

    void Weapon::setUnlimitedMunition(bool unlimitedMunition)
    {   this->unlimitedMunition_ = unlimitedMunition;   }

    const bool Weapon::getUnlimitedMunition()
    {   return this->unlimitedMunition_;    }

}

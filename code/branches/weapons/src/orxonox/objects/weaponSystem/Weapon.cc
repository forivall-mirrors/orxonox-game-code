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
#include "Weapon.h"

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"

#include "Munition.h"
#include "WeaponSystem.h"

namespace orxonox
{
    CreateFactory(Weapon);

    Weapon::Weapon(BaseObject* creator) : StaticEntity(creator)
    {
        RegisterObject(Weapon);

        this->bulletReadyToShoot_ = true;
        this->magazineReadyToShoot_ = true;
        this->weaponSystem_ = 0;
        this->weaponPack_ = 0;
        this->weaponSlot_ = 0;
        this->bulletLoadingTime_ = 0;
        this->magazineLoadingTime_ = 0;
        this->bReloading_ = false;
        this->bulletAmount_= 0;
        this->magazineAmount_ = 0;
        this->munition_ = 0;
        this->unlimitedMunition_ = false;
        this->setObjectMode(0x0);

COUT(0) << "+Weapon" << std::endl;
    }

    Weapon::~Weapon()
    {
COUT(0) << "~Weapon" << std::endl;
    }

    void Weapon::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(Weapon, XMLPort, xmlelement, mode);
        XMLPortParam(Weapon, "munitionType", setMunitionType, getMunitionType, xmlelement, mode);
        XMLPortParam(Weapon, "bulletLoadingTime", setBulletLoadingTime, getBulletLoadingTime, xmlelement, mode);
        XMLPortParam(Weapon, "magazineLoadingTime", setMagazineLoadingTime, getMagazineLoadingTime, xmlelement, mode);
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
        if ( this->bulletReadyToShoot_ && this->magazineReadyToShoot_ && !this->bReloading_)
        {
            this->bulletReadyToShoot_ = false;
            if ( this->unlimitedMunition_== true )
            {
                //shoot
                this->reloadBullet();
                this->createProjectile();
            }
            else
            {
                if ( this->munition_->bullets() > 0)
                {
                    //shoot and reload
                    this->takeBullets();
                    this->reloadBullet();
                    this->createProjectile();
                }
                //if there are no bullets, but magazines
                else if ( this->munition_->magazines() > 0 && this->munition_->bullets() == 0 )
                {
                    //reload magazine
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
            //weapon not reloaded
        }

    }


    //weapon reloading
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
    }



    void Weapon::attachNeededMunition(const std::string& munitionName)
    {
        /*  if munition type already exists attach it, else create a new one of this type and attach it to the weapon and to the WeaponSystem
        */
        if (this->weaponSystem_)
        {
            //getMunitionType returns 0 if there is no such munitionType
            Munition* munition = this->weaponSystem_->getMunitionType(munitionName);
            if ( munition )
            {
                this->munition_ = munition;
                this->setMunition();
            }
            else
            {
                //create new munition with identifier because there is no such munitionType
                this->munitionIdentifier_ = ClassByString(munitionName);
                this->munition_ = this->munitionIdentifier_.fabricate(this);
                this->weaponSystem_->setNewMunition(munitionName, this->munition_);
                this->setMunition();
            }
        }
    }


    Munition * Weapon::getAttachedMunition(const std::string& munitionType)
    {
        this->munition_ = this->weaponSystem_->getMunitionType(munitionType);
        return this->munition_;
    }


    //these function are defined in the weapon classes
    void Weapon::takeBullets() { };
    void Weapon::createProjectile() { };
    void Weapon::takeMagazines() { };
    void Weapon::reloadBullet() { };
    void Weapon::reloadMagazine() { };


    //get and set functions for XMLPort
    void Weapon::setMunitionType(const std::string& munitionType)
    {   this->munitionType_ = munitionType; }

    const std::string& Weapon::getMunitionType() const
    {   return this->munitionType_;  }

    void Weapon::setBulletLoadingTime(float loadingTime)
    {   this->bulletLoadingTime_ = loadingTime; }

    const float Weapon::getBulletLoadingTime() const
    {   return this->bulletLoadingTime_;  }

    void Weapon::setMagazineLoadingTime(float loadingTime)
    {   this->magazineLoadingTime_ = loadingTime; }

    const float Weapon::getMagazineLoadingTime() const
    {   return this->magazineLoadingTime_;  }

    void Weapon::setBulletAmount(unsigned int amount)
    {   this->bulletAmount_ = amount; }

    const unsigned int Weapon::getBulletAmount() const
    {   return this->bulletAmount_;  }

    void Weapon::setMagazineAmount(unsigned int amount)
    {   this->magazineAmount_ = amount; }

    const unsigned int Weapon::getMagazineAmount() const
    {   return this->magazineAmount_;   }

    void Weapon::setUnlimitedMunition(bool unlimitedMunition)
    {   this->unlimitedMunition_ = unlimitedMunition;   }

    const bool Weapon::getUnlimitedMunition() const
    {   return this->unlimitedMunition_;    }

}

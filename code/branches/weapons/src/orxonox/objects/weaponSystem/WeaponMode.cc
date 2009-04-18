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
 *      Fabian 'x3n' Landau
 *   Co-authors:
 *      ...
 *
 */

#include "OrxonoxStableHeaders.h"
#include "WeaponMode.h"

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"

#include "Munition.h"
#include "Weapon.h"
#include "WeaponPack.h"
#include "WeaponSystem.h"

namespace orxonox
{
    WeaponMode::WeaponMode(BaseObject* creator) : BaseObject(creator)
    {
        RegisterObject(WeaponMode);

        this->weapon_ = 0;
        this->mode_ = WeaponSystem::WEAPON_MODE_UNASSIGNED;

        this->munition_ = 0;
        this->initialMunition_ = 0;
        this->initialMagazines_ = 0;
        this->munitionPerShot_ = 1;

        this->reloadTime_ = 0.25;
        this->bReloading_ = false;
        this->bAutoReload_ = true;
        this->bParallelReload_ = true;

        this->reloadTimer_.setTimer(0.0f, false, this, createExecutor(createFunctor(&WeaponMode::reloaded)));
        this->reloadTimer_.stopTimer();

        this->damage_ = 0;
        this->muzzleOffset_ = Vector3::ZERO;

COUT(0) << "+WeaponMode" << std::endl;
    }

    WeaponMode::~WeaponMode()
    {
COUT(0) << "~WeaponMode" << std::endl;
    }

    void WeaponMode::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(WeaponMode, XMLPort, xmlelement, mode);

        XMLPortParam(WeaponMode, "mode",             setMode,             getMode,             xmlelement, mode);

        XMLPortParam(WeaponMode, "munitiontype",     setMunitionName,     getMunitionName,     xmlelement, mode);
        XMLPortParam(WeaponMode, "initialmunition",  setInitialMunition,  getInitialMunition,  xmlelement, mode);
        XMLPortParam(WeaponMode, "initialmagazines", setInitialMagazines, getInitialMagazines, xmlelement, mode);
        XMLPortParam(WeaponMode, "munitionpershot",  setMunitionPerShot,  getMunitionPerShot,  xmlelement, mode);

        XMLPortParam(WeaponMode, "reloadtime",       setReloadTime,       getReloadTime,       xmlelement, mode);
        XMLPortParam(WeaponMode, "autoreload",       setAutoReload,       getAutoReload,       xmlelement, mode).description("If true, the weapon reloads the magazine automatically");
        XMLPortParam(WeaponMode, "parallelreload",   setParallelReload,   getParallelReload,   xmlelement, mode).description("If true, the weapon reloads in parallel to the magazine reloading");

        XMLPortParam(WeaponMode, "damage",           setDamage,           getDamage,           xmlelement, mode);
        XMLPortParam(WeaponMode, "muzzleoffset",     setMuzzleOffset,     getMuzzleOffset,     xmlelement, mode);
    }

    bool WeaponMode::fire(float* reloadTime)
    {
        (*reloadTime) = this->reloadTime_;

        if (!this->bReloading_ && this->munition_ && this->munition_->takeMunition(this->munitionPerShot_, this))
        {
            float reloadtime = this->reloadTime_;

            if (this->bAutoReload_ && this->munition_->needReload(this))
            {
                if (this->munition_->reload(this))
                {
                    if (!this->bParallelReload_)
                        reloadtime += this->munition_->getReloadTime();
                }
            }

            this->bReloading_ = true;
            this->reloadTimer_.setInterval(reloadtime);
            this->reloadTimer_.startTimer();

            this->fire();

            return true;
        }
        else
        {
            return false;
        }
    }

    bool WeaponMode::reload()
    {
        if (this->munition_ && this->munition_->reload(this))
        {
            if (!this->bParallelReload_)
            {
                this->bReloading_ = true;
                this->reloadTimer_.setInterval(this->reloadTime_ + this->munition_->getReloadTime());
                this->reloadTimer_.startTimer();
            }

            return true;
        }

        return false;
    }

    void WeaponMode::setMunitionType(Identifier* identifier)
    {
        this->munitionname_ = identifier->getName();
        this->munitiontype_ = identifier;
        this->updateMunition();
    }

    void WeaponMode::setMunitionName(const std::string& munitionname)
    {
        this->munitionname_ = munitionname;
        this->munitiontype_ = ClassByString(this->munitionname_);
        this->updateMunition();
    }

    void WeaponMode::updateMunition()
    {
        if (this->munitiontype_ && this->weapon_ && this->weapon_->getWeaponPack() && this->weapon_->getWeaponPack()->getWeaponSystem())
        {
            this->munition_ = this->weapon_->getWeaponPack()->getWeaponSystem()->getMunition(&this->munitiontype_);

            if (this->munition_)
            {
                // Add the initial magazines
                this->munition_->addMagazines(this->initialMagazines_);

                // Maybe we have to reload (if this munition is used the first time or if there weren't any magazines available before)
                if (this->munition_->needReload(this))
                    this->munition_->reload(this, false);

                // Add the initial munition
                if (this->initialMunition_ > 0 && this->munition_->getNumMunitionInCurrentMagazine(this) == this->munition_->getMaxMunitionPerMagazine())
                {
                    // The current magazine is still full, so let's just add another magazine to
                    // the stack and reduce the current magazine to the given amount of munition

                    unsigned int initialmunition = this->initialMunition_;
                    if (initialmunition > this->munition_->getMaxMunitionPerMagazine())
                        initialmunition = this->munition_->getMaxMunitionPerMagazine();

                    this->munition_->takeMunition(this->munition_->getMaxMunitionPerMagazine() - initialmunition, this);
                    this->munition_->addMagazines(1);
                }
                else
                {
                    // The current magazine isn't full, add the munition directly

                    this->munition_->addMunition(this->initialMunition_);
                }
            }
        }
        else
            this->munition_ = 0;
    }

    void WeaponMode::reloaded()
    {
        this->bReloading_ = false;
    }

    Vector3 WeaponMode::getMuzzlePosition() const
    {
        if (this->weapon_)
            return (this->weapon_->getWorldPosition() + this->muzzleOffset_);
        else
            return this->muzzleOffset_;
    }

    const Quaternion& WeaponMode::getMuzzleOrientation() const
    {
        if (this->weapon_)
            return this->weapon_->getWorldOrientation();
        else
            return Quaternion::IDENTITY;
    }

    Vector3 WeaponMode::getMuzzleDirection() const
    {
        if (this->weapon_)
            return (this->weapon_->getWorldOrientation() * WorldEntity::FRONT);
        else
            return WorldEntity::FRONT;
    }

/*
    WeaponMode::WeaponMode(BaseObject* creator) : StaticEntity(creator)
    {
        RegisterObject(WeaponMode);

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
COUT(0) << "+WeaponMode" << std::endl;
    }

    WeaponMode::~WeaponMode()
    {
COUT(0) << "~WeaponMode" << std::endl;
        if (this->isInitialized() && this->weaponPack_)
            this->weaponPack_->removeWeapon(this);
    }

    void WeaponMode::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(WeaponMode, XMLPort, xmlelement, mode);

        XMLPortParam(WeaponMode, "munitionType", setMunitionType, getMunitionType, xmlelement, mode);
        XMLPortParam(WeaponMode, "bulletLoadingTime", setBulletLoadingTime, getBulletLoadingTime, xmlelement, mode);
        XMLPortParam(WeaponMode, "magazineLoadingTime", setMagazineLoadingTime, getMagazineLoadingTime, xmlelement, mode);
        XMLPortParam(WeaponMode, "bullets", setBulletAmount, getBulletAmount, xmlelement, mode);
        XMLPortParam(WeaponMode, "magazines", setMagazineAmount, getMagazineAmount, xmlelement, mode);
        XMLPortParam(WeaponMode, "unlimitedMunition", setUnlimitedMunition, getUnlimitedMunition, xmlelement, mode);
    }

    void WeaponMode::setWeapon()
    {
        this->munition_->fillBullets();
        this->munition_->fillMagazines();
    }

    void WeaponMode::setMunition()
    {
        this->munition_->setMaxBullets(this->bulletAmount_);
        this->munition_->setMaxMagazines(this->magazineAmount_);
    }

    void WeaponMode::fire()
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
    void WeaponMode::bulletTimer(float bulletLoadingTime)
    {
        this->bReloading_ = true;
        this->bulletReloadTimer_.setTimer( bulletLoadingTime , false , this , createExecutor(createFunctor(&WeaponMode::bulletReloaded)));
    }
    void WeaponMode::magazineTimer(float magazineLoadingTime)
    {
        this->bReloading_ = true;
        this->magazineReloadTimer_.setTimer( magazineLoadingTime , false , this , createExecutor(createFunctor(&WeaponMode::magazineReloaded)));
    }

    void WeaponMode::bulletReloaded()
    {
        this->bReloading_ = false;
        this->bulletReadyToShoot_ = true;
    }

    void WeaponMode::magazineReloaded()
    {
        this->bReloading_ = false;
        this->munition_->fillBullets();
    }



    void WeaponMode::attachNeededMunition(const std::string& munitionName)
    {
        //  if munition type already exists attach it, else create a new one of this type and attach it to the weapon and to the WeaponSystem
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


    Munition * WeaponMode::getAttachedMunition(const std::string& munitionType)
    {
        this->munition_ = this->weaponSystem_->getMunitionType(munitionType);
        return this->munition_;
    }
*/
}

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

#include "WeaponMode.h"

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "controllers/Controller.h"
#include "worldentities/pawns/Pawn.h"

#include "Munition.h"
#include "Weapon.h"
#include "WeaponPack.h"
#include "WeaponSystem.h"
#include "WeaponSlot.h"

#include "sound/WorldSound.h"

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

        this->reloadTimer_.setTimer(0.0f, false, createExecutor(createFunctor(&WeaponMode::reloaded, this)));
        this->reloadTimer_.stopTimer();

        this->damage_ = 0;
        this->healthdamage_ = 0;
        this->shielddamage_ = 0;

        this->muzzleOffset_ = Vector3::ZERO;
        this->muzzlePosition_ = Vector3::ZERO;
        this->muzzleOrientation_ = Quaternion::IDENTITY;

        if( GameMode::isMaster() )
        {
            this->defSndWpnFire_ = new WorldSound(this);
            this->defSndWpnFire_->setLooping(false);
            this->bSoundAttached_ = false;
        }
        else
            this->defSndWpnFire_ = 0;
    }

    WeaponMode::~WeaponMode()
    {
        if (this->isInitialized())
        {
            if (this->defSndWpnFire_)
                this->defSndWpnFire_->destroy();
        }
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
        XMLPortParam(WeaponMode, "healthdamage",     setHealthDamage,     getHealthDamage,     xmlelement, mode);
        XMLPortParam(WeaponMode, "shielddamage",     setShieldDamage,     getShieldDamage,     xmlelement, mode);
        XMLPortParam(WeaponMode, "muzzleoffset",     setMuzzleOffset,     getMuzzleOffset,     xmlelement, mode);
    }

    bool WeaponMode::fire(float* reloadTime)
    {
        (*reloadTime) = this->reloadTime_;
        if( !this->bSoundAttached_ && GameMode::isMaster() )
        {
            assert(this->getWeapon());
            this->getWeapon()->attach(this->defSndWpnFire_);
            this->bSoundAttached_ = true;
        }

        if (!this->bReloading_ && this->munition_ && this->munition_->takeMunition(this->munitionPerShot_, this))
        {
            float reloadtime = this->reloadTime_;

            if (this->bAutoReload_ && this->munition_->needReload(this))
            {
                if (this->munition_->reload(this))
                {
                    if (this->bParallelReload_)
                        reloadtime = std::max(this->reloadTime_, this->munition_->getReloadTime());
                    else
                        reloadtime = this->reloadTime_ + this->munition_->getReloadTime();
                }
            }

            this->bReloading_ = true;
            this->reloadTimer_.setInterval(reloadtime);
            this->reloadTimer_.startTimer();

            if( this->defSndWpnFire_ && !(this->defSndWpnFire_->isPlaying()))
            {
                this->defSndWpnFire_->play();
            }

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
        Identifier* identifier = ClassByString(this->munitionname_);
        if (identifier)
            this->munitiontype_ = identifier;
        else
            COUT(2) << "Warning: No munition class defined in WeaponMode " << this->getName() << std::endl;
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
        if( this->defSndWpnFire_ && this->defSndWpnFire_->isPlaying())
        {
            this->defSndWpnFire_->stop();
        }
        this->bReloading_ = false;
    }

    void WeaponMode::computeMuzzleParameters(const Vector3& target)
    {
        if (this->weapon_)
        {
            this->muzzlePosition_ = this->weapon_->getWorldPosition() + this->weapon_->getWorldOrientation() * this->muzzleOffset_;

            Vector3 muzzleDirection;
            muzzleDirection = target - this->muzzlePosition_;
//             COUT(0) << "muzzleDirection " << muzzleDirection << endl;
            this->muzzleOrientation_ = (this->weapon_->getWorldOrientation() * WorldEntity::FRONT).getRotationTo(muzzleDirection) * this->weapon_->getWorldOrientation();
        }
        else
        {
            this->muzzlePosition_ = this->muzzleOffset_;
            this->muzzleOrientation_ = Quaternion::IDENTITY;
        }
    }

    Vector3 WeaponMode::getMuzzleDirection() const
    {
        if (this->weapon_)
            return (this->getMuzzleOrientation() * WorldEntity::FRONT);
        else
            return WorldEntity::FRONT;
    }

    void WeaponMode::setDefaultSound(const std::string& soundPath)
    {
        if( this->defSndWpnFire_ )
            this->defSndWpnFire_->setSource(soundPath);
    }

    const std::string& WeaponMode::getDefaultSound()
    {
        if( this->defSndWpnFire_ )
            return this->defSndWpnFire_->getSource();
        else
            return BLANKSTRING;
    }

    void WeaponMode::setDefaultSoundWithVolume(const std::string& soundPath, const float soundVolume){
        if (this->defSndWpnFire_) {
            this->defSndWpnFire_->setSource(soundPath);
            this->defSndWpnFire_->setVolume(soundVolume);
        }
    }

}

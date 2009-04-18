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

#ifndef _WeaponMode_H__
#define _WeaponMode_H__

#include "OrxonoxPrereqs.h"
#include "core/BaseObject.h"

#include "tools/Timer.h"
#include "core/Identifier.h"
#include "util/Math.h"

namespace orxonox
{
    class _OrxonoxExport WeaponMode : public BaseObject
    {
        public:
            WeaponMode(BaseObject* creator);
            virtual ~WeaponMode();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);

            bool fire(float* reloadTime);
            bool reload();


            // Munition
            inline Munition* getMunition() const
                { return this->munition_; }

            void setMunitionType(Identifier* identifier);
            inline Identifier* getMunitionType() const
                { return this->munitiontype_; }

            void setMunitionName(const std::string& munitionname);
            inline const std::string& getMunitionName() const
                { return this->munitionname_; }

            inline void setInitialMunition(unsigned int amount)
                { this->initialMunition_ = amount; }
            inline unsigned int getInitialMunition() const
                { return this->initialMunition_; }

            inline void setInitialMagazines(unsigned int amount)
                { this->initialMagazines_ = amount; }
            inline unsigned int getInitialMagazines() const
                { return this->initialMagazines_; }

            inline void setMunitionPerShot(unsigned int amount)
                { this->munitionPerShot_ = amount; }
            inline unsigned int getMunitionPerShot() const
                { return this->munitionPerShot_; }


            // Reloading
            inline void setReloadTime(float time)
                { this->reloadTime_ = time; }
            inline float getReloadTime() const
                { return this->reloadTime_; }

            inline void setAutoReload(bool autoreload)
                { this->bAutoReload_ = autoreload; }
            inline bool getAutoReload() const
                { return this->bAutoReload_; }

            inline void setParallelReload(bool parallelreload)
                { this->bParallelReload_ = parallelreload; }
            inline bool getParallelReload() const
                { return this->bParallelReload_; }


            // Fire
            inline void setDamage(float damage)
                { this->damage_ = damage; }
            inline float getDamage() const
                { return this->damage_; }

            inline void setMuzzleOffset(const Vector3& offset)
                { this->muzzleOffset_ = offset; }
            inline const Vector3& getMuzzleOffset() const
                { return this->muzzleOffset_; }

            Vector3 getMuzzlePosition() const;
            const Quaternion& getMuzzleOrientation() const;
            Vector3 getMuzzleDirection() const;


            // Weapon
            inline void setWeapon(Weapon* weapon)
                { this->weapon_ = weapon; this->updateMunition(); }
            inline Weapon* getWeapon() const
                { return this->weapon_; }

            inline void setMode(unsigned int mode)
                { this->mode_ = mode; }
            inline unsigned int getMode() const
                { return this->mode_; }

        protected:
            virtual void fire() = 0;

            unsigned int initialMunition_;
            unsigned int initialMagazines_;
            unsigned int munitionPerShot_;

            float reloadTime_;
            bool bAutoReload_;
            bool bParallelReload_;

            float damage_;
            Vector3 muzzleOffset_;

        private:
            void updateMunition();
            void reloaded();

            Weapon* weapon_;
            unsigned int mode_;

            Munition* munition_;
            SubclassIdentifier<Munition> munitiontype_;
            std::string munitionname_;

            Timer<WeaponMode> reloadTimer_;
            bool bReloading_;
/*
            virtual void fire();
            void attachNeededMunition(const std::string& munitionType);
            Munition * getAttachedMunition(const std::string& munitiontype);

            //reloading
            void bulletTimer(float bulletLoadingTime);
            void magazineTimer(float magazineLoadingTime);
            void bulletReloaded();
            void magazineReloaded();

            //get and set functions for XMLPort
            void setMunitionType(const std::string& munitionType)
                {   this->munitionType_ = munitionType; }
            std::string& getMunitionType() const
                {   return this->munitionType_;  }

            void setBulletLoadingTime(float loadingTime)
                {   this->bulletLoadingTime_ = loadingTime; }
            float getBulletLoadingTime() const
                {   return this->bulletLoadingTime_;  }

            void setMagazineLoadingTime(float loadingTime)
                {   this->magazineLoadingTime_ = loadingTime; }
            float getMagazineLoadingTime() const
                {   return this->magazineLoadingTime_;  }

            void setBulletAmount(unsigned int amount)
                {   this->bulletAmount_ = amount; }
            unsigned int getBulletAmount() const
                {   return this->bulletAmount_;  }

            void setMagazineAmount(unsigned int amount)
                {   this->magazineAmount_ = amount; }
            unsigned int getMagazineAmount() const
                {   return this->magazineAmount_;   }

            void setUnlimitedMunition(bool unlimitedMunition)
                {   this->unlimitedMunition_ = unlimitedMunition;   }
            bool getUnlimitedMunition() const
                {   return this->unlimitedMunition_;    }

            //weapon actions
            //these function are defined in the weapon classes
            virtual void takeBullets() {}
            virtual void takeMagazines() {}
            virtual void createProjectile() {}
            virtual void reloadBullet() {}
            virtual void reloadMagazine() {}

            //manually set or reset
            virtual void setWeapon();
            virtual void setMunition();

            inline void setWeaponSystem(WeaponSystem *weaponSystem)
                { this->weaponSystem_ = weaponSystem; };
            inline WeaponSystem * getWeaponSystem() const
                { return this->weaponSystem_; };

            inline void setWeaponPack(WeaponPack *weaponPack)
                { this->weaponPack_ = weaponPack; };
            inline WeaponPack * getWeaponPack() const
                { return this->weaponPack_; };

            inline void setWeaponSlot(WeaponSlot * wSlot)
                { this->weaponSlot_ = wSlot; }
            inline WeaponSlot * getWeaponSlot() const
                { return this->weaponSlot_; }

        protected:
            bool bReloading_;
            bool bulletReadyToShoot_;
            bool magazineReadyToShoot_;
            bool unlimitedMunition_;
            float bulletLoadingTime_;
            float magazineLoadingTime_;
            unsigned int bulletAmount_;
            unsigned int magazineAmount_;
            std::string munitionType_;

            WeaponSlot * weaponSlot_;
            Munition * munition_;
            WeaponSystem * weaponSystem_;
            WeaponPack* weaponPack_;

            SubclassIdentifier<Munition> munitionIdentifier_;

            Timer<WeaponMode> bulletReloadTimer_;
            Timer<WeaponMode> magazineReloadTimer_;
*/
    };
}

#endif /* _WeaponMode_H__ */

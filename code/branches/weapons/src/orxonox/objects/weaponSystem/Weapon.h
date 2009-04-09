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

#ifndef _Weapon_H__
#define _Weapon_H__

#include "OrxonoxPrereqs.h"
#include "objects/worldentities/StaticEntity.h"

#include "tools/Timer.h"
#include "core/Identifier.h"

namespace orxonox
{
    class _OrxonoxExport Weapon : public StaticEntity
    {
        public:
            Weapon(BaseObject* creator);
            virtual ~Weapon();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);

            virtual void fire();
            void attachNeededMunition(const std::string& munitionType);
            Munition * getAttachedMunition(const std::string& munitiontype);

            //reloading
            void bulletTimer(float bulletLoadingTime);
            void magazineTimer(float magazineLoadingTime);
            void bulletReloaded();
            void magazineReloaded();

            //XMLPort functions
            virtual void setMunitionType(const std::string& munitionType);
            virtual const std::string& getMunitionType() const;
            virtual void setBulletLoadingTime(float loadingTime);
            virtual const float getBulletLoadingTime() const;
            virtual void setMagazineLoadingTime(float loadingTime);
            virtual const float getMagazineLoadingTime() const;
            virtual void setBulletAmount(unsigned int amount);
            virtual const unsigned int getBulletAmount() const;
            virtual void setMagazineAmount(unsigned int amount);
            virtual const unsigned int getMagazineAmount() const;
            virtual void setUnlimitedMunition(bool unlimitedMunition);
            virtual const bool getUnlimitedMunition() const;

            //weapon actions
            virtual void takeBullets();
            virtual void takeMagazines();
            virtual void createProjectile();
            virtual void reloadBullet();
            virtual void reloadMagazine();

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

            Timer<Weapon> bulletReloadTimer_;
            Timer<Weapon> magazineReloadTimer_;
    };
}

#endif /* _Weapon_H__ */

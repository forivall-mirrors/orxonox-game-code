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

#include "core/BaseObject.h"
#include "tools/BillboardSet.h"
#include "tools/Timer.h"
#include "core/Identifier.h"

#include "WeaponSystem.h"
#include "Munition.h"

#include "objects/worldentities/StaticEntity.h"

namespace orxonox
{
    class _OrxonoxExport Weapon : public StaticEntity
    {
        public:
            Weapon(BaseObject* creator);
            virtual ~Weapon();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);

            virtual void fire();
            void attachNeededMunition(std::string munitionType);
            Munition * getAttachedMunition(std::string munitiontype);

            //reloading
            void bulletTimer(float bulletLoadingTime);
            void magazineTimer(float magazineLoadingTime);
            void bulletReloaded();
            void magazineReloaded();

            virtual void setMunitionType(std::string munitionType);
            virtual const std::string getMunitionType();
            virtual void setBulletLoadingTime(float loadingTime);
            virtual const float getBulletLoadingTime();
            virtual void setMagazineLoadingTime(float loadingTime);
            virtual const float getMagazineLoadingTime();
            virtual void setSharedMunition(bool bSharedMunition);
            virtual const bool getSharedMunition();

            virtual void takeBullets();
            virtual void takeMagazines();
            virtual void createProjectile();

            inline void setParentWeaponSystem(WeaponSystem *parentWeaponSystem)
                { this->parentWeaponSystem_=parentWeaponSystem; };
            inline WeaponSystem * getParentWeaponSystem()
                { return this->parentWeaponSystem_; };

            inline void setAttachedToWeaponSlot(WeaponSlot * wSlot)
                { this->attachedToWeaponSlot_ = wSlot; }
            inline WeaponSlot * getAttachedToWeaponSlot()
                { return this->attachedToWeaponSlot_; }

            virtual void setWeapon();

        private:

        protected:
            bool bReloading_;
            bool bulletReadyToShoot_;
            bool magazineReadyToShoot_;
            bool bSharedMunition_;
            float bulletLoadingTime_;
            float magazineLoadingTime_;
            std::string munitionType_;

            WeaponSlot * attachedToWeaponSlot_;
            Munition * munition_;
            WeaponSystem * parentWeaponSystem_;

            SubclassIdentifier<Munition> munitionIdentifier_;

            Timer<Weapon> bulletReloadTimer_;
            Timer<Weapon> magazineReloadTimer_;
    };
}

#endif /* _Weapon_H__ */

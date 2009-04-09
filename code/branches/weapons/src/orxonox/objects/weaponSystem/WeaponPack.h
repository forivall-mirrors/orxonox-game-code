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


#ifndef _WeaponPack_H__
#define _WeaponPack_H__

#include "OrxonoxPrereqs.h"

#include <vector>

#include "core/BaseObject.h"

namespace orxonox
{
    class _OrxonoxExport WeaponPack : public BaseObject
    {
        public:
            WeaponPack(BaseObject* creator);
            virtual ~WeaponPack();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);

            void fire();

            Weapon * getWeaponPointer(unsigned int n) const;
            int getSize() const;

            void setFireMode(unsigned int firemode);
            unsigned int getFireMode() const;

            void addWeapon(Weapon * weapon);
            const Weapon * getWeapon(unsigned int index) const;

            void attachNeededMunitionToAllWeapons();

            //functions with effect to all weapons of the weaponPack
            //functions needed for creating Pointer to the right objects (-->Pawn.cc)
            inline void setWeaponSystem(WeaponSystem *weaponSystem)
                { this->weaponSystem_ = weaponSystem; this->setWeaponSystemToAllWeapons(weaponSystem); }
            inline WeaponSystem * getWeaponSystem() const
                { return this->weaponSystem_; }

        private:
            void setWeaponSystemToAllWeapons(WeaponSystem * weaponSystem);

            std::vector<Weapon *> weapons_;
            WeaponSystem *weaponSystem_;
            unsigned int firemode_;
    };
}

#endif /* _WeaponPack_H__ */

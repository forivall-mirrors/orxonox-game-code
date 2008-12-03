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

#include "core/BaseObject.h"

#include "Weapon.h"


namespace orxonox
{
    class _OrxonoxExport WeaponPack : public BaseObject
    {
        public:
            WeaponPack(BaseObject* creator);
            virtual ~WeaponPack();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);

            void fire();

            Weapon * getWeaponPointer(unsigned int n);
            int getSize();
            void attachWeapon(Weapon *weapon);

            void setFireMode(unsigned int firemode);
            unsigned int getFireMode();

            void addWeapon(Weapon * weapon);
            Weapon * getWeapon(unsigned int index);

            inline void setParentWeaponSystem(WeaponSystem *parentWeaponSystem)
                { parentWeaponSystem_=parentWeaponSystem; }
            inline WeaponSystem * getParentWeaponSystem()
                { return parentWeaponSystem_; }

        private:
            std::vector<Weapon *> weapons_;
            WeaponSystem *parentWeaponSystem_;
            unsigned int firemode_;
    };
}

#endif /* _WeaponPack_H__ */

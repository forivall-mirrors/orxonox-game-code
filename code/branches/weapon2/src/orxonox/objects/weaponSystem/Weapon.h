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

namespace orxonox
{
    class _OrxonoxExport Weapon : public BaseObject
    {
        public:
            Weapon(BaseObject* creator);
            virtual ~Weapon();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);

            virtual void fire();
            void reloaded();
            virtual void setParentWeaponSystem();
            void attachNeededMunition(Munition *munitionPointer);

            inline void setParentWeaponSlot(WeaponSlot *parentWeaponSlot)
                { parentWeaponSlot_=parentWeaponSlot; };
            inline WeaponSlot * getParentWeaponSlot()
                { return parentWeaponSlot_; };




        private:
            bool weaponReadyToShoot_;
            float loadingTime_;
            Munition *pointerToMunition_;
            WeaponSlot *parentWeaponSlot_;
            WeaponSystem *parentWeaponSystem_;
            Timer<Weapon> reloadTimer_;


    };
}

#endif /* _Weapon_H__ */

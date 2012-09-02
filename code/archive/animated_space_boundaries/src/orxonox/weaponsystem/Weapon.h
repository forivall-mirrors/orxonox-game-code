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

#ifndef _Weapon_H__
#define _Weapon_H__

#include "OrxonoxPrereqs.h"

#include <map>
#include "tools/Timer.h"
#include "worldentities/StaticEntity.h"

namespace orxonox
{
    class _OrxonoxExport Weapon : public StaticEntity
    {
        public:
            Weapon(BaseObject* creator);
            virtual ~Weapon();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);

            void fire(unsigned int mode);
            void reload();

            void addWeaponmode(WeaponMode* weaponmode);
            WeaponMode* getWeaponmode(unsigned int index) const;

            inline void setWeaponPack(WeaponPack * weaponPack)
                { this->weaponPack_ = weaponPack; this->notifyWeaponModes(); }
            inline WeaponPack * getWeaponPack() const
                { return this->weaponPack_; }

            inline void setWeaponSlot(WeaponSlot * wSlot)
                { this->weaponSlot_ = wSlot; }
            inline WeaponSlot * getWeaponSlot() const
                { return this->weaponSlot_; }

        private:
            void reloaded();
            void notifyWeaponModes();

            WeaponPack* weaponPack_;
            WeaponSlot* weaponSlot_;
            std::multimap<unsigned int, WeaponMode*> weaponmodes_;

            Timer reloadTimer_;
            bool bReloading_;
            unsigned int reloadingWeaponmode_;
    };
}

#endif /* _Weapon_H__ */

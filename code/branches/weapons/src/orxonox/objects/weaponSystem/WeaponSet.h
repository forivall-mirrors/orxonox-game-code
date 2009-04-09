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


#ifndef _WeaponSet_H__
#define _WeaponSet_H__

#include "OrxonoxPrereqs.h"

#include <vector>

#include "core/BaseObject.h"

namespace orxonox
{
    class _OrxonoxExport WeaponSet : public BaseObject
    {
        public:
            WeaponSet(BaseObject* creator, int k = 0);
            virtual ~WeaponSet();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);

            void attachWeaponPack(WeaponPack *wPack);
            void fire();

            inline void setFireMode(const unsigned int firemode)
                { this->firemode_ = firemode; }
            inline unsigned int getFireMode() const
                { return this->firemode_; }

            inline void setWeaponSystem(WeaponSystem *weaponSystem)
                { this->weaponSystem_ = weaponSystem; }
            inline WeaponSystem * getWeaponSystem() const
                { return this->weaponSystem_; }

        private:
            WeaponSystem *weaponSystem_;
            std::vector<WeaponSlot *> setWeaponSlots_;
            unsigned int firemode_;
            WeaponPack * attachedWeaponPack_;
    };
}

#endif /* _WeaponSet_H__ */

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

#include "core/BaseObject.h"

#include "WeaponSlot.h"


namespace orxonox
{
    class _OrxonoxExport WeaponSet : public BaseObject
    {
        public:
            WeaponSet(int k);
            virtual ~WeaponSet();

            virtual void WeaponSet::XMLPort(Element& xmlelement, XMLPort::Mode mode);

            void WeaponSet::attachWeaponSlot(WeaponSlot *wSlot);
            void WeaponSet::fire();
            WeaponSlot * getWeaponSlotPointer(int n);

            inline void WeaponSet::setParentWeaponSystem(WeaponSystem *parentWeaponSystem)
                { parentWeaponSystem_=parentWeaponSystem; }
            inline WeaponSystem * getParentWeaponSystem()
                { return parentWeaponSystem_; }

        private:
            std::vector<WeaponSlot *> weaponSlots_;
            WeaponSystem *parentWeaponSystem_;
    };
}

#endif /* _WeaponSystem_H__ */

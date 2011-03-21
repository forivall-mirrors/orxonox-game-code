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


#ifndef _WeaponSystem_H__
#define _WeaponSystem_H__

#include "OrxonoxPrereqs.h"

#include <map>
#include <set>
#include <vector>
#include "core/BaseObject.h"

namespace orxonox
{
    class _OrxonoxExport WeaponSystem : public BaseObject
    {
        public:
            WeaponSystem(BaseObject* creator);
            virtual ~WeaponSystem();

            // adding and removing WeaponSlots
            void addWeaponSlot(WeaponSlot * wSlot);
            void removeWeaponSlot(WeaponSlot * wSlot);
            WeaponSlot * getWeaponSlot(unsigned int index) const;

            // adding and removing WeaponSets
            bool addWeaponSet(WeaponSet * wSet);
            bool addWeaponSet(WeaponSet * wSet, unsigned int firemode);
            void removeWeaponSet(WeaponSet * wSet);
            WeaponSet * getWeaponSet(unsigned int index) const;

            // adding and removing WeaponPacks
            bool canAddWeaponPack(WeaponPack * wPack);
            bool addWeaponPack(WeaponPack * wPack);
            void removeWeaponPack(WeaponPack * wPack);
            WeaponPack * getWeaponPack(unsigned int index) const;

            // configure slots and firemodes
            bool swapWeaponSlots(WeaponSlot * wSlot1, WeaponSlot * wSlot2);
            void changeWeaponmode(WeaponPack * wPack, WeaponSet * wSet, unsigned int weaponmode);

            void fire(unsigned int firemode);
            void reload();

            Munition * getMunition(SubclassIdentifier<Munition> * identifier);

            inline void setPawn(Pawn * pawn)
                { this->pawn_ = pawn; }
            inline Pawn * getPawn() const
                { return this->pawn_; }

            inline int getWeaponSlotSize() const
                { return this->weaponSlots_.size(); }

            static inline unsigned int getFiremodeMask(unsigned int firemode)
                { return (0x1 << firemode); }

            static const unsigned int MAX_FIRE_MODES = 8;
            static const unsigned int FIRE_MODE_UNASSIGNED = static_cast<unsigned int>(-1);

            static const unsigned int MAX_WEAPON_MODES = 8;
            static const unsigned int WEAPON_MODE_UNASSIGNED = static_cast<unsigned int>(-1);

        private:
            std::map<unsigned int, WeaponSet *> weaponSets_;
            std::vector<WeaponSlot *> weaponSlots_;
            std::vector<WeaponPack *> weaponPacks_;
            std::map<Identifier *, Munition *> munitions_;
            Pawn * pawn_;
    };
}

#endif /* _WeaponSystem_H__ */

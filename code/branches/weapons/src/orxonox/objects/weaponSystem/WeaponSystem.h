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

#include <set>
#include <map>

#include "core/BaseObject.h"

namespace orxonox
{
    const unsigned int MAX_FIRE_MODES = 8;

    class _OrxonoxExport WeaponSystem : public BaseObject
    {
        public:
            WeaponSystem(BaseObject* creator);
            virtual ~WeaponSystem();

            void attachWeaponSlot(WeaponSlot * wSlot);
            WeaponSlot * getWeaponSlot(unsigned int index) const;

            void attachWeaponSet(WeaponSet * wSet);
            WeaponSet * getWeaponSet(unsigned int index) const;

            void attachWeaponPack(WeaponPack * wPack, unsigned int wSetNumber);
            WeaponPack * getWeaponPack(unsigned int index) const;

            void fire(unsigned int firemode);

            void setNewMunition(const std::string& munitionType, Munition * munitionToAdd);
            void setNewSharedMunition(const std::string& munitionType, Munition * munitionToAdd);
            Munition * getMunitionType(const std::string& munitionType) const;

            inline void setPawn(Pawn * pawn)
                { this->pawn_ = pawn; }
            inline Pawn * getPawn() const
                { return this->pawn_; }

            inline int getWeaponSlotSize() const
                { return this->weaponSlots_.size(); }

            static inline unsigned int getMaxFireModes()
                { return MAX_FIRE_MODES; }
            static inline unsigned int getFireModeMask(unsigned int firemode)
                { return (0x1 << firemode); }

        private:
            std::map<unsigned int, WeaponSet *> weaponSets_;
            std::set<WeaponSlot *> weaponSlots_;
            std::set<WeaponPack *> weaponPacks_;
            std::map<std::string, Munition *> munitionSet_;
            Pawn * pawn_;
    };
}

#endif /* _WeaponSystem_H__ */

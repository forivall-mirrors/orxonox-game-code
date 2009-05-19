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

#include "core/BaseObject.h"

#include "WeaponSet.h"
#include "WeaponPack.h"

namespace orxonox
{

    class _OrxonoxExport WeaponSystem : public BaseObject
    {
        public:
            WeaponSystem(BaseObject* creator);
            virtual ~WeaponSystem();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);

            void attachWeaponSlot(WeaponSlot *wSlot);
            void attachWeaponSet(WeaponSet *wSet);
            //void fire();
            void fire(WeaponMode::Enum fireMode);
            void attachWeaponPack(WeaponPack * wPack, unsigned int firemode);
            WeaponSet * getWeaponSetPointer(unsigned int n);
            WeaponSlot * getWeaponSlotPointer(unsigned int n);
            WeaponPack * getWeaponPackPointer(unsigned int n);
            void setNewMunition(std::string munitionType, Munition * munitionToAdd);
            void setNewSharedMunition(std::string munitionType, Munition * munitionToAdd);
            Munition * getMunitionType(std::string munitionType);

            inline void setParentPawn(Pawn *parentPawn)
                { parentPawn_=parentPawn; }
            inline Pawn * getParentPawn()
                { return parentPawn_; }

            inline int getWeaponSlotSize()
                { return this->weaponSlots_.size(); }

        private:
            std::vector<WeaponSet *> weaponSets_;
            std::vector<WeaponSlot *> weaponSlots_;
            std::vector<WeaponPack *> weaponPacks_;
            std::map<std::string, Munition *> munitionSet_;
            Pawn *parentPawn_;
    };
}

#endif /* _WeaponSystem_H__ */

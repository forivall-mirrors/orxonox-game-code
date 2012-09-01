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

#ifndef _WeaponSlot_H__
#define _WeaponSlot_H__

#include "OrxonoxPrereqs.h"
#include "worldentities/StaticEntity.h"

namespace orxonox
{
        /**
        @brief
            The a WeaponSlot defines where a @ref orxonox::Weapon "Weapon" is placed on a pawn. (A WeaponSlot is a StaticEntity)
            In a WeaponSlot there can be only one "Weapon", but a Weapon can have several @ref orxonox::WeaponMode "WeaponModes".
            A WeaponMode is what one intuitively imagines as weapon. (E.g. RocketFire, LightningGun, LaserFire are weaponmodes)

            A WeaponSlot is created in XML (in a weaponsettings file), which can be done in the following fashion:
            @code
            <weaponslots>
              <WeaponSlot position="-15.0,-1.5,0" />
              <WeaponSlot position=" 15.0,-1.5,0" />
              <WeaponSlot position="    0,   0,0" />
            </weaponslots>

        @author
            Martin Polak

        @ingroup Weaponsystem
    */
    class _OrxonoxExport WeaponSlot : public StaticEntity
    {
        public:
            WeaponSlot(BaseObject* creator);
            virtual ~WeaponSlot();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);

            void attachWeapon(Weapon * weapon);
            void removeWeapon();
            Weapon * getWeapon() const
                { return this->weapon_; }

            inline bool isOccupied() const
                { return (this->weapon_ != 0); }

            inline void setWeaponSystem(WeaponSystem * weaponSystem)
                { this->weaponSystem_ = weaponSystem; }
            inline WeaponSystem * getWeaponSystem() const
                { return this->weaponSystem_; }


        private:
            WeaponSystem * weaponSystem_;
            Weapon * weapon_;
    };
}

#endif /* _WeaponSlot_H__ */

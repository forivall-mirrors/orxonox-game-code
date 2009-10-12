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

#include "WeaponSlot.h"

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"

#include "Weapon.h"
#include "WeaponSystem.h"

namespace orxonox
{
    CreateFactory(WeaponSlot);

    WeaponSlot::WeaponSlot(BaseObject* creator) : StaticEntity(creator)
    {
        RegisterObject(WeaponSlot);

        this->weaponSystem_ = 0;
        this->weapon_ = 0;

        this->setSyncMode(0x0);
    }

    WeaponSlot::~WeaponSlot()
    {
        if (this->isInitialized() && this->weaponSystem_)
            this->weaponSystem_->removeWeaponSlot(this);
    }

    void WeaponSlot::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(WeaponSlot, XMLPort, xmlelement, mode);

        // In the future, there might be parameters like allowed weapon types or max size of the weapon
    }

    void WeaponSlot::attachWeapon(Weapon *weapon)
    {
        if (this->weapon_)
            this->removeWeapon();

        this->weapon_ = weapon;

        if (this->weapon_)
        {
            this->weapon_->setWeaponSlot(this);
            this->weapon_->setPosition(this->getPosition());
        }
    }

    void WeaponSlot::removeWeapon()
    {
        if (this->weapon_)
        {
            this->weapon_->setWeaponSlot(0);
            this->weapon_ = 0;
        }
    }
}

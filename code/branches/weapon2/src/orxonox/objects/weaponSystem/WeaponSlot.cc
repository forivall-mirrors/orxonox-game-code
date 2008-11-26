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

#include "OrxonoxStableHeaders.h"

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "util/Debug.h"

#include "WeaponSlot.h"


namespace orxonox
{
    WeaponSlot::WeaponSlot(BaseObject* creator) : PositionableEntity(creator)
    {
        RegisterObject(WeaponSlot);

        this->unlimitedAmmo_ = false;
        this->attachedWeapon_ = 0;
        this->setObjectMode(0x0);
    }

    WeaponSlot::~WeaponSlot()
    {
    }


    /*sets the munition type
     *unlimited: true
     *limited:   false  (in this case there will be munition)
     */
    void WeaponSlot::setAmmoType(bool isUnlimited)
    {
        unlimitedAmmo_ = isUnlimited;
    }


    void WeaponSlot::fire()
    {
COUT(0) << "WeaponSlot::fire" << std::endl;
        this->attachedWeapon_->fire();
    }


    //XMLPort functions
    void WeaponSlot::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(WeaponSlot, XMLPort, xmlelement, mode);
        XMLPortObject(WeaponSlot, Weapon, "attached-weapon", attachWeapon, getAttachedWeapon, xmlelement, mode);
    }

    void WeaponSlot::attachWeapon(Weapon *weaponPointer)
    {   this->attachedWeapon_ = weaponPointer;   }

    Weapon * WeaponSlot::getAttachedWeapon(unsigned int index) const
    {   return this->attachedWeapon_;   }


}

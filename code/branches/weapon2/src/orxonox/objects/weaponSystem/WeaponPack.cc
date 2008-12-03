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
 *      ... *
 */

#include "OrxonoxStableHeaders.h"

#include <vector>

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "util/Debug.h"

#include "WeaponPack.h"
#include "objects/worldentities/pawns/Pawn.h"

namespace orxonox
{
    WeaponPack::WeaponPack(BaseObject* creator) : BaseObject(creator)
    {
        RegisterObject(WeaponPack);

        this->parentWeaponSystem_ = 0;
        this->firemode_ = 1;

    }

    WeaponPack::~WeaponPack()
    {
    }

    int WeaponPack::getSize()
    {
        return this->weapons_.size();
    }

    void WeaponPack::attachWeapon(Weapon *weapon)
    {
        this->weapons_.push_back(weapon);
    }

    void WeaponPack::fire()
    {
        for (int i=0; i < (int) this->weapons_.size(); i++)
        {
COUT(0) << "WeaponPack::fire"<< i << std::endl;
            this->weapons_[i]->fire();
        }
    }

    Weapon * WeaponPack::getWeaponPointer(unsigned int n)
    {
        return this->weapons_[n];
    }

    void WeaponPack::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        XMLPortObject(WeaponPack, Weapon, "", addWeapon, getWeapon, xmlelement, mode);
        XMLPortParam(WeaponPack, "firemode", setFireMode, getFireMode, xmlelement, mode);
    }

    void WeaponPack::setFireMode(unsigned int firemode)
    {
        this->firemode_ = firemode;
    }

    unsigned int WeaponPack::getFireMode()
    {
        return this->firemode_;
    }

    void WeaponPack::addWeapon(Weapon * weapon)
    {
        this->weapons_.push_back(weapon);
    }
    
    Weapon * WeaponPack::getWeapon(unsigned int index)
    {
        return weapons_[index];
    }
}

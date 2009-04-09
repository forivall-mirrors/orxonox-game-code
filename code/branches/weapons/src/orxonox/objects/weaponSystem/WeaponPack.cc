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
#include "WeaponPack.h"

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "objects/worldentities/pawns/Pawn.h"

#include "Weapon.h"
#include "WeaponSlot.h"
#include "WeaponSystem.h"

namespace orxonox
{
    CreateFactory(WeaponPack);

    WeaponPack::WeaponPack(BaseObject* creator) : BaseObject(creator)
    {
        RegisterObject(WeaponPack);

        this->weaponSystem_ = 0;

COUT(0) << "+WeaponPack" << std::endl;
    }

    WeaponPack::~WeaponPack()
    {
COUT(0) << "~WeaponPack" << std::endl;

        if (this->isInitialized() && this->weaponSystem_)
            this->weaponSystem_->removeWeaponPack(this);
    }

    void WeaponPack::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(WeaponPack, XMLPort, xmlelement, mode);

        XMLPortObject(WeaponPack, Weapon, "", addWeapon, getWeapon, xmlelement, mode);
    }

    void WeaponPack::fire(unsigned int weaponmode)
    {
        for (std::set<Weapon *>::iterator it = this->weapons_.begin(); it != this->weapons_.end(); ++it)
            (*it)->fire();
    }

    void WeaponPack::addWeapon(Weapon * weapon)
    {
        if (!weapon)
            return;

        this->weapons_.insert(weapon);
        weapon->setWeaponPack(this);
    }

    Weapon * WeaponPack::getWeapon(unsigned int index) const
    {
        unsigned int i = 0;

        for (std::set<Weapon *>::const_iterator it = this->weapons_.begin(); it != this->weapons_.end(); ++it)
        {
            if (i == index)
                return (*it);
            ++i;
        }

        return 0;
    }

    void WeaponPack::setWeaponSystemToAllWeapons(WeaponSystem * weaponSystem)
    {
        for (std::set<Weapon *>::const_iterator it = this->weapons_.begin(); it != this->weapons_.end(); ++it)
            (*it)->setWeaponSystem(weaponSystem);
    }

    void WeaponPack::attachNeededMunitionToAllWeapons()
    {
        for (std::set<Weapon *>::const_iterator it = this->weapons_.begin(); it != this->weapons_.end(); ++it)
        {
            (*it)->attachNeededMunition((*it)->getMunitionType());
            (*it)->setWeapon();
        }
    }
}

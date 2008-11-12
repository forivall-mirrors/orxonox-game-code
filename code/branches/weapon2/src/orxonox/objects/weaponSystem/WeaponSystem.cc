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

#include <vector>

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "util/Debug.h"

#include "WeaponSystem.h"

/* WEAPONSYSTEM
 * creates the WeaponSystem and the ability to use weapons and munition
 * loads the weapon the whole weaponSystem setting from an xml file
 *
 */

namespace orxonox
{

    WeaponSystem::WeaponSystem(BaseObject* creator) : BaseObject(creator)
    {
        RegisterObject(WeaponSystem);

        this->activeWeaponSet_ = 0;
        this->parentSpaceShip_ = 0;
        this->attachedMunition_ =0;
    }

    WeaponSystem::~WeaponSystem()
    {
    }

    void WeaponSystem::attachWeaponSet(WeaponSet *wSet)
    {
        this->weaponSets_.push_back(wSet);
        wSet->setParentWeaponSystem(this);
    }

/*
    //the first weaponSet is at n=0
    void WeaponSystem::setActiveWeaponSet(unsigned int n)
    {
        if (n < this->weaponSets_.size())
            this->activeWeaponSet_ = this->weaponSets_[n];
        else
            this->activeWeaponSet_ = this->weaponSets_[0];
    }
*/

    //n is the n'th weaponSet, starting with zero
    //SpaceShip.cc only needs to have the keybinding to a specific Set-number n
    void WeaponSystem::fire(WeaponMode::Enum n)
    {
        if (n < (int)this->weaponSets_.size())
            this->weaponSets_[n]->fire();
    }


    WeaponSet * WeaponSystem::getWeaponSetPointer(unsigned int n)
    {
        if (n < this->weaponSets_.size())
            return this->weaponSets_[n];
        else
            return 0;
    }

    void WeaponSystem::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {

    }

    Munition * WeaponSystem::getAttachedMunitionPointer()
    {
        return this->attachedMunition_;
    }

    void WeaponSystem::addMunitionType(Munition *munitionPointer)
    {


        if (munitionPointer != NULL)  //gewährleiste, dass munitionPointer auf etwas sinnvolles zeigt
            this->attachedMunition_ = munitionPointer;
        else
            ;//was?

    }


}

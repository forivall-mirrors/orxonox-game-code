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


/* WeaponSystem
 *
 *  www.orxonox.net/wiki/WeaponSystem
 */

namespace orxonox
{
    CreateFactory(WeaponSystem);

    WeaponSystem::WeaponSystem(BaseObject* creator) : BaseObject(creator)
    {
        RegisterObject(WeaponSystem);

        this->parentPawn_ = 0;
    }

    WeaponSystem::~WeaponSystem()
    {
    }

    void WeaponSystem::attachWeaponPack(WeaponPack *wPack, unsigned int firemode)
    {
        if (firemode < this->weaponSets_.size())
            this->weaponSets_[firemode]->attachWeaponPack(wPack);
        this->weaponPacks_.push_back(wPack);
    }

    void WeaponSystem::attachWeaponSlot(WeaponSlot *wSlot)
    {
        wSlot->setParentWeaponSystem(this);
        this->weaponSlots_.push_back(wSlot);
    }

    void WeaponSystem::attachWeaponSet(WeaponSet *wSet)
    {
        wSet->setParentWeaponSystem(this);
        this->weaponSets_.push_back(wSet);
    }

    void WeaponSystem::setNewMunition(std::string munitionType, Munition * munitionToAdd)
    {
        this->munitionSet_[munitionType] = munitionToAdd;
    }


    //returns the Pointer to the munitionType, if this munitionType doesn't exist returns 0, see Weapon::attachNeededMunition
    Munition * WeaponSystem::getMunitionType(std::string munitionType)
    {
        std::map<std::string, Munition *>::const_iterator it = this->munitionSet_.find(munitionType);
        if (it != this->munitionSet_.end())
            return it->second;
        else
            return 0;
    }


    //n is the n'th weaponSet, starting with zero
    //SpaceShip.cc only needs to have the keybinding to a specific Set-number n (=firemode)
    //in future this could be well defined and not only for 3 different WeaponModes
    void WeaponSystem::fire(WeaponMode::Enum n)
    {
        int set = 0;
        switch (n)
        {
            case WeaponMode::fire:
                set = 0;
                break;
            case WeaponMode::altFire:
                set = 1;
                break;
            case WeaponMode::altFire2:
                set = 2;
                break;
        }
        if (set < (int)this->weaponSets_.size())
            this->weaponSets_[set]->fire();
    }


    WeaponSet * WeaponSystem::getWeaponSetPointer(unsigned int n)
    {
        if (n < this->weaponSets_.size())
            return this->weaponSets_[n];
        else
            return 0;
    }

    WeaponSlot * WeaponSystem::getWeaponSlotPointer(unsigned int n)
    {
        if (n < this->weaponSlots_.size())
            return this->weaponSlots_[n];
        else
            return 0;
    }

    WeaponPack * WeaponSystem::getWeaponPackPointer(unsigned int n)
    {
        if (n < this->weaponPacks_.size())
            return this->weaponPacks_[n];
        else
            return 0;
    }

    void WeaponSystem::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(WeaponSystem, XMLPort, xmlelement, mode);
    }

}

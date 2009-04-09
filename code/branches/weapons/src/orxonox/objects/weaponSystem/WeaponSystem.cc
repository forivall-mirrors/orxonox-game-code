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
#include "WeaponSystem.h"

#include "core/CoreIncludes.h"
#include "objects/worldentities/pawns/Pawn.h"

#include "WeaponSlot.h"
#include "WeaponPack.h"
#include "WeaponSet.h"

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

        this->pawn_ = 0;
COUT(0) << "+WeaponSystem" << std::endl;
    }

    WeaponSystem::~WeaponSystem()
    {
COUT(0) << "~WeaponSystem" << std::endl;
        if (this->isInitialized())
        {
            if (this->pawn_)
                this->pawn_->setWeaponSystem(0);
        }
    }

    void WeaponSystem::attachWeaponSet(WeaponSet *wSet)
    {
        if (!wSet)
            return;

        this->weaponSets_[wSet->getFireMode()] = wSet;
        wSet->setWeaponSystem(this);
    }

    WeaponSet * WeaponSystem::getWeaponSet(unsigned int index) const
    {
        unsigned int i = 0;
        for (std::map<unsigned int, WeaponSet*>::const_iterator it = this->weaponSets_.begin(); it != this->weaponSets_.end(); ++it)
        {
            ++i;
            if (i > index)
                return it->second;
        }
        return 0;
    }

    void WeaponSystem::attachWeaponSlot(WeaponSlot *wSlot)
    {
        if (!wSlot)
            return;

        this->weaponSlots_.insert(wSlot);
        wSlot->setWeaponSystem(this);
    }

    WeaponSlot * WeaponSystem::getWeaponSlot(unsigned int index) const
    {
        unsigned int i = 0;
        for (std::set<WeaponSlot*>::iterator it = this->weaponSlots_.begin(); it != this->weaponSlots_.end(); ++it)
        {
            ++i;
            if (i > index)
                return (*it);
        }
        return 0;
    }

    void WeaponSystem::attachWeaponPack(WeaponPack *wPack, unsigned int wSetNumber)
    {
        if (!wPack)
            return;

        std::map<unsigned int, WeaponSet *>::iterator it = this->weaponSets_.find(wSetNumber);
        if (it != this->weaponSets_.end() && it->second)
            it->second->attachWeaponPack(wPack);

        this->weaponPacks_.insert(wPack);
        wPack->setWeaponSystem(this);
        wPack->attachNeededMunitionToAllWeapons(); // TODO - what is this?
    }

    WeaponPack * WeaponSystem::getWeaponPack(unsigned int index) const
    {
        unsigned int i = 0;
        for (std::set<WeaponPack*>::iterator it = this->weaponPacks_.begin(); it != this->weaponPacks_.end(); ++it)
        {
            ++i;
            if (i > index)
                return (*it);
        }
        return 0;
   }

    void WeaponSystem::setNewMunition(const std::string& munitionType, Munition * munitionToAdd)
    {
        this->munitionSet_[munitionType] = munitionToAdd;
    }


    //returns the Pointer to the munitionType, if this munitionType doesn't exist returns 0, see Weapon::attachNeededMunition
    Munition * WeaponSystem::getMunitionType(const std::string& munitionType) const
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
    void WeaponSystem::fire(unsigned int firemode)
    {
        std::map<unsigned int, WeaponSet *>::iterator it = this->weaponSets_.find(firemode);
        if (it != this->weaponSets_.end() && it->second)
            it->second->fire();
    }
}

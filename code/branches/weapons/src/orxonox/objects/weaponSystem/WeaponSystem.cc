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
#include "Weapon.h"

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

            for (std::map<unsigned int, WeaponSet*>::iterator it = this->weaponSets_.begin(); it != this->weaponSets_.end(); )
                delete (it++)->second;

            for (std::set<WeaponPack*>::iterator it = this->weaponPacks_.begin(); it != this->weaponPacks_.end(); )
                delete (*(it++));

            for (std::set<WeaponSlot*>::iterator it = this->weaponSlots_.begin(); it != this->weaponSlots_.end(); )
                delete (*(it++));
        }
    }

    void WeaponSystem::addWeaponSlot(WeaponSlot * wSlot)
    {
        if (!wSlot)
            return;

        this->weaponSlots_.insert(wSlot);
        wSlot->setWeaponSystem(this);
    }

    void WeaponSystem::removeWeaponSlot(WeaponSlot * wSlot)
    {
        if (!wSlot)
            return;

        if (wSlot->getWeapon())
            this->removeWeaponPack(wSlot->getWeapon()->getWeaponPack());

        this->weaponSlots_.erase(wSlot);
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

    bool WeaponSystem::addWeaponSet(WeaponSet * wSet)
    {
        if (wSet)
            return this->addWeaponSet(wSet, wSet->getDesiredFiremode());
        else
            return false;
    }

    bool WeaponSystem::addWeaponSet(WeaponSet * wSet, unsigned int firemode)
    {
        if (!wSet || firemode >= WeaponSystem::MAX_FIRE_MODES)
            return false;

        std::map<unsigned int, WeaponSet*>::const_iterator it = this->weaponSets_.find(firemode);
        if (it == this->weaponSets_.end())
        {
            this->weaponSets_[firemode] = wSet;
            wSet->setWeaponSystem(this);
            return true;
        }

        return false;
    }

    void WeaponSystem::removeWeaponSet(WeaponSet * wSet)
    {
        for (std::map<unsigned int, WeaponSet*>::iterator it = this->weaponSets_.begin(); it != this->weaponSets_.end(); )
        {
            if (it->second == wSet)
                this->weaponSets_.erase(it++);
            else
                ++it;
        }
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

    bool WeaponSystem::canAddWeaponPack(WeaponPack * wPack)
    {
        if (!wPack)
            return false;

        unsigned int freeSlots = 0;
        for (std::set<WeaponSlot*>::iterator it = this->weaponSlots_.begin(); it != this->weaponSlots_.end(); ++it)
        {
            if (!(*it)->isOccupied())
                ++freeSlots;
        }

        return (freeSlots >= wPack->getNumWeapons());
    }

    bool WeaponSystem::addWeaponPack(WeaponPack * wPack)
    {
        if (!this->canAddWeaponPack(wPack))
            return false;

        // Attach all weapons to the first free slots (and to the Pawn)
        unsigned int i = 0;
        for (std::set<WeaponSlot*>::iterator it = this->weaponSlots_.begin(); it != this->weaponSlots_.end(); ++it)
        {
            if (!(*it)->isOccupied() && i < wPack->getNumWeapons())
            {
                Weapon* weapon = wPack->getWeapon(i);
                (*it)->attachWeapon(weapon);
                this->getPawn()->attach(weapon);
                ++i;
            }
        }

        // Assign the desired weaponmode to the firemodes
        for (std::map<unsigned int, WeaponSet *>::iterator it = this->weaponSets_.begin(); it != this->weaponSets_.end(); ++it)
        {
            unsigned int weaponmode = wPack->getDesiredWeaponmode(it->first);
            if (weaponmode != WeaponSystem::WEAPON_MODE_UNASSIGNED)
                it->second->setWeaponmodeLink(wPack, weaponmode);
        }

        this->weaponPacks_.insert(wPack);
        wPack->setWeaponSystem(this);
        wPack->attachNeededMunitionToAllWeapons(); // TODO - what is this?

        return true;
    }

    void WeaponSystem::removeWeaponPack(WeaponPack * wPack)
    {
        // Remove all weapons from their WeaponSlot
        unsigned int i = 0;
        Weapon* weapon = 0;
        while (weapon = wPack->getWeapon(i++))
            weapon->getWeaponSlot()->removeWeapon();

        // Remove all added links from the WeaponSets
        for (std::map<unsigned int, WeaponSet *>::iterator it = this->weaponSets_.begin(); it != this->weaponSets_.end(); ++it)
            it->second->removeWeaponmodeLink(wPack);

        // Remove the WeaponPack from the WeaponSystem
        this->weaponPacks_.erase(wPack);
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

    bool WeaponSystem::swapWeaponSlots(WeaponSlot * wSlot1, WeaponSlot * wSlot2)
    {
        // TODO
    }

    void WeaponSystem::changeWeaponmode(WeaponPack * wPack, WeaponSet * wSet, unsigned int weaponmode)
    {
        if (!wPack || !wSet)
            return;

        // Check if the WeaponPack belongs to this WeaponSystem
        std::set<WeaponPack *>::iterator it1 = this->weaponPacks_.find(wPack);
        if (it1 == this->weaponPacks_.end())
            return;

        // Check if the WeaponSet belongs to this WeaponSystem
        bool foundWeaponSet = false;
        for (std::map<unsigned int, WeaponSet *>::iterator it2 = this->weaponSets_.begin(); it2 != this->weaponSets_.end(); ++it2)
        {
            if (it2->second == wSet)
            {
                foundWeaponSet = true;
                break;
            }
        }
        if (!foundWeaponSet)
            return;

        // Finally set the link between firemode and weaponmode
        wSet->setWeaponmodeLink(wPack, weaponmode);
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

    void WeaponSystem::fire(unsigned int firemode)
    {
        std::map<unsigned int, WeaponSet *>::iterator it = this->weaponSets_.find(firemode);
        if (it != this->weaponSets_.end() && it->second)
            it->second->fire();
    }
}

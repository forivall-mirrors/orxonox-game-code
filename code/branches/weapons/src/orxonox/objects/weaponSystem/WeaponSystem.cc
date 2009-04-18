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
#include "Munition.h"

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

            while (!this->weaponSets_.empty())
                delete (this->weaponSets_.begin()->second);

            while (!this->weaponPacks_.empty())
                delete (*this->weaponPacks_.begin());

            while (!this->weaponSlots_.empty())
                delete (*this->weaponSlots_.begin());

            while (!this->munitions_.empty())
                { delete (this->munitions_.begin()->second); this->munitions_.erase(this->munitions_.begin()); }
        }
    }

    void WeaponSystem::addWeaponSlot(WeaponSlot * wSlot)
    {
        if (!wSlot)
            return;

        this->weaponSlots_.push_back(wSlot);
        wSlot->setWeaponSystem(this);
    }

    void WeaponSystem::removeWeaponSlot(WeaponSlot * wSlot)
    {
        if (!wSlot)
            return;

        if (wSlot->getWeapon())
            this->removeWeaponPack(wSlot->getWeapon()->getWeaponPack());

        for (std::vector<WeaponSlot*>::iterator it = this->weaponSlots_.begin(); it != this->weaponSlots_.end(); ++it)
        {
            if ((*it) == wSlot)
            {
                this->weaponSlots_.erase(it);
                break;
            }
        }
    }

    WeaponSlot * WeaponSystem::getWeaponSlot(unsigned int index) const
    {
        unsigned int i = 0;
        for (std::vector<WeaponSlot*>::const_iterator it = this->weaponSlots_.begin(); it != this->weaponSlots_.end(); ++it)
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
        for (std::vector<WeaponSlot*>::iterator it = this->weaponSlots_.begin(); it != this->weaponSlots_.end(); ++it)
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
        for (std::vector<WeaponSlot*>::iterator it = this->weaponSlots_.begin(); it != this->weaponSlots_.end(); ++it)
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
        if (!wSlot1 || !wSlot2)
            return false;

        Weapon* weapon1 = wSlot1->getWeapon();
        Weapon* weapon2 = wSlot2->getWeapon();

        wSlot1->attachWeapon(weapon2);
        wSlot2->attachWeapon(weapon1);

        return true;
        // In the future, certain weapons might not fit to some slots. Swapping would then be
        // impossible and the returnvalue would be false.
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

    void WeaponSystem::fire(unsigned int firemode)
    {
        std::map<unsigned int, WeaponSet *>::iterator it = this->weaponSets_.find(firemode);
        if (it != this->weaponSets_.end() && it->second)
            it->second->fire();
    }

    void WeaponSystem::reload()
    {
        for (std::map<unsigned int, WeaponSet *>::iterator it = this->weaponSets_.begin(); it != this->weaponSets_.end(); ++it)
            it->second->reload();
    }

    Munition * WeaponSystem::getMunition(SubclassIdentifier<Munition> * identifier)
    {
        if (!identifier || !identifier->getIdentifier())
            return 0;

        std::map<Identifier *, Munition *>::iterator it = this->munitions_.find(identifier->getIdentifier());
        if (it != this->munitions_.end())
        {
            return it->second;
        }
        else if (identifier->getIdentifier()->isA(Class(Munition)))
        {
            Munition* munition = identifier->fabricate(this);
            this->munitions_[identifier->getIdentifier()] = munition;
            return munition;
        }
        else
        {
            return 0;
        }
    }
}

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

#include "WeaponPack.h"

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"

#include "Weapon.h"
#include "WeaponSystem.h"
#include "DefaultWeaponmodeLink.h"

namespace orxonox
{
    CreateFactory(WeaponPack);

    WeaponPack::WeaponPack(BaseObject* creator) : BaseObject(creator)
    {
        RegisterObject(WeaponPack);

        this->weaponSystem_ = 0;
    }

    WeaponPack::~WeaponPack()
    {
        if (this->isInitialized())
        {
            if( this->weaponSystem_ )
                this->weaponSystem_->removeWeaponPack(this);

            while (!this->weapons_.empty())
                (*this->weapons_.begin())->destroy();

            for (std::set<DefaultWeaponmodeLink*>::iterator it = this->links_.begin(); it != this->links_.end(); )
                (*(it++))->destroy();
        }
    }

    void WeaponPack::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(WeaponPack, XMLPort, xmlelement, mode);

        XMLPortObjectExtended(WeaponPack, Weapon, "", addWeapon, getWeapon, xmlelement, mode, false, false);
        XMLPortObject(WeaponPack, DefaultWeaponmodeLink, "links", addDefaultWeaponmodeLink, getDefaultWeaponmodeLink, xmlelement, mode);
    }

    void WeaponPack::fire(unsigned int weaponmode)
    {
        for (std::vector<Weapon *>::iterator it = this->weapons_.begin(); it != this->weapons_.end(); ++it)
            (*it)->fire(weaponmode);
    }

    void WeaponPack::reload()
    {
        for (std::vector<Weapon *>::iterator it = this->weapons_.begin(); it != this->weapons_.end(); ++it)
            (*it)->reload();
    }

    void WeaponPack::addWeapon(Weapon * weapon)
    {
        if (!weapon)
            return;

        this->weapons_.push_back(weapon);
        weapon->setWeaponPack(this);
    }

    void WeaponPack::removeWeapon(Weapon * weapon)
    {
        if (!weapon)
            return;

        std::vector<Weapon*>::iterator it = std::find(this->weapons_.begin(), this->weapons_.end(), weapon);
        assert(it != this->weapons_.end());
        this->weapons_.erase(it);
        weapon->setWeaponPack(0);
    }

    Weapon * WeaponPack::getWeapon(unsigned int index) const
    {
        unsigned int i = 0;

        for (std::vector<Weapon *>::const_iterator it = this->weapons_.begin(); it != this->weapons_.end(); ++it)
        {
            if (i == index)
                return (*it);
            ++i;
        }

        return 0;
    }

    void WeaponPack::addDefaultWeaponmodeLink(DefaultWeaponmodeLink* link)
    {
        this->links_.insert(link);
    }

    DefaultWeaponmodeLink* WeaponPack::getDefaultWeaponmodeLink(unsigned int index) const
    {
        unsigned int i = 0;
        for (std::set<DefaultWeaponmodeLink*>::const_iterator it = this->links_.begin(); it != this->links_.end(); ++it)
        {
            if (i == index)
                return (*it);

            ++i;
        }
        return 0;
    }

    unsigned int WeaponPack::getDesiredWeaponmode(unsigned int firemode) const
    {
        for (std::set<DefaultWeaponmodeLink*>::const_iterator it = this->links_.begin(); it != this->links_.end(); ++it)
            if ((*it)->getFiremode() == firemode)
                return (*it)->getWeaponmode();

        return WeaponSystem::WEAPON_MODE_UNASSIGNED;
    }

    void WeaponPack::notifyWeapons()
    {
        for (std::vector<Weapon *>::const_iterator it = this->weapons_.begin(); it != this->weapons_.end(); ++it)
            (*it)->setWeaponPack(this);
    }
}

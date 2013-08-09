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
 *      Fabian 'x3n' Landau
 *   Co-authors:
 *      ...
 *
 */

#include "Weapon.h"

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"

#include "WeaponMode.h"
#include "WeaponPack.h"
#include "WeaponSystem.h"

namespace orxonox
{
    CreateFactory(Weapon);

    Weapon::Weapon(Context* context) : StaticEntity(context)
    {
        RegisterObject(Weapon);

        this->weaponPack_ = 0;
        this->weaponSlot_ = 0;
        this->bReloading_ = false;
        this->reloadingWeaponmode_ = WeaponSystem::WEAPON_MODE_UNASSIGNED;

        this->reloadTimer_.setTimer(0.0f, false, createExecutor(createFunctor(&Weapon::reloaded, this)));
        this->reloadTimer_.stopTimer();
    }

    Weapon::~Weapon()
    {
        if (this->isInitialized())
        {
            if (this->weaponPack_)
                this->weaponPack_->removeWeapon(this);

            for (std::multimap<unsigned int, WeaponMode*>::iterator it = this->weaponmodes_.begin(); it != this->weaponmodes_.end(); ++it)
                it->second->destroy();
        }
    }

    void Weapon::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(Weapon, XMLPort, xmlelement, mode);

        XMLPortObject(Weapon, WeaponMode, "", addWeaponmode, getWeaponmode, xmlelement, mode);
    }

    void Weapon::addWeaponmode(WeaponMode* weaponmode)
    {
        if (!weaponmode)
            return;

        this->weaponmodes_.insert(std::pair<unsigned int, WeaponMode*>(weaponmode->getMode(), weaponmode));
        weaponmode->setWeapon(this);
    }

    WeaponMode* Weapon::getWeaponmode(unsigned int index) const
    {
        unsigned int i = 0;
        for (std::multimap<unsigned int, WeaponMode*>::const_iterator it = this->weaponmodes_.begin(); it != this->weaponmodes_.end(); ++it)
        {
            if (i == index)
                return it->second;

            ++i;
        }
        return 0;
    }

    void Weapon::fire(unsigned int mode)
    {
        // To avoid firing with more than one mode at the same time, we lock the weapon (reloading) for
        // all modes except the one which is currently reloading.
        //
        // Example:
        // WeaponMode A -> mode 0
        // WeaponMode B -> mode 0
        // WeaponMode C -> mode 1
        //
        // -> A and B can fire at the same time, but C has to wait until both (A and B) have reloaded
        // -> If C fires, A and B have to wait until C has reloaded
        //
        // Note: The reloading of each WeaponMode is internally handled by each A, B and C.
        //       The reloading of the weapon is only performed to avoid firing with different modes at the same time.
        if (this->bReloading_ && this->reloadingWeaponmode_ != mode)
            return;

        std::multimap<unsigned int, WeaponMode*>::iterator start = this->weaponmodes_.lower_bound(mode);
        std::multimap<unsigned int, WeaponMode*>::iterator end   = this->weaponmodes_.upper_bound(mode);

        for (std::multimap<unsigned int, WeaponMode*>::iterator it = start; it != end; ++it)
        {
            float reloading_time = 0;
            if (it->second->fire(&reloading_time))
            {
                this->bReloading_ = true;
                this->reloadingWeaponmode_ = mode;

                this->reloadTimer_.setInterval(reloading_time);
                this->reloadTimer_.startTimer();
            }
        }
    }

    void Weapon::reload()
    {
        for (std::multimap<unsigned int, WeaponMode*>::iterator it = this->weaponmodes_.begin(); it != this->weaponmodes_.end(); ++it)
            it->second->reload();
    }

    void Weapon::reloaded()
    {
        this->bReloading_ = false;
        this->reloadingWeaponmode_ = WeaponSystem::WEAPON_MODE_UNASSIGNED;
    }

    void Weapon::notifyWeaponModes()
    {
        for (std::multimap<unsigned int, WeaponMode*>::iterator it = this->weaponmodes_.begin(); it != this->weaponmodes_.end(); ++it)
            it->second->setWeapon(this);
    }
}

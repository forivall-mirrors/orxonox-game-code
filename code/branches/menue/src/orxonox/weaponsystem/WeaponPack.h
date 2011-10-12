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


#ifndef _WeaponPack_H__
#define _WeaponPack_H__

#include "OrxonoxPrereqs.h"

#include <set>
#include "core/BaseObject.h"

namespace orxonox
{
    class _OrxonoxExport WeaponPack : public BaseObject
    {
        public:
            WeaponPack(BaseObject* creator);
            virtual ~WeaponPack();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);

            void fire(unsigned int weaponmode);
            void reload();

            void addWeapon(Weapon * weapon);
            void removeWeapon(Weapon * weapon);
            Weapon * getWeapon(unsigned int index) const;

            inline size_t getNumWeapons() const
                { return this->weapons_.size(); }

            void addDefaultWeaponmodeLink(DefaultWeaponmodeLink* link);
            DefaultWeaponmodeLink* getDefaultWeaponmodeLink(unsigned int index) const;

            unsigned int getDesiredWeaponmode(unsigned int firemode) const;

            inline void setWeaponSystem(WeaponSystem * weaponSystem)
                { this->weaponSystem_ = weaponSystem; this->notifyWeapons(); }
            inline WeaponSystem * getWeaponSystem() const
                { return this->weaponSystem_; }

        private:
            void notifyWeapons();

            std::vector<Weapon *> weapons_;
            std::set<DefaultWeaponmodeLink *> links_;
            WeaponSystem * weaponSystem_;
    };
}

#endif /* _WeaponPack_H__ */

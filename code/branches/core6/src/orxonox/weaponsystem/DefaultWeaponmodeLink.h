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
 *      Fabian 'x3n' Landau
 *   Co-authors:
 *      ...
 *
 */


#ifndef _DefaultWeaponmodeLink_H__
#define _DefaultWeaponmodeLink_H__

#include "OrxonoxPrereqs.h"
#include "core/BaseObject.h"

namespace orxonox
{
    class _OrxonoxExport DefaultWeaponmodeLink : public BaseObject
    {
        public:
            DefaultWeaponmodeLink(Context* context);
            virtual ~DefaultWeaponmodeLink();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);

            inline void setFiremode(const unsigned int firemode)
                { this->firemode_ = firemode; }
            inline unsigned int getFiremode() const
                { return this->firemode_; }

            inline void setWeaponmode(const unsigned int weaponmode)
                { this->weaponmode_ = weaponmode; }
            inline unsigned int getWeaponmode() const
                { return this->weaponmode_; }

        private:
            unsigned int firemode_;
            unsigned int weaponmode_;
    };
}

#endif /* _DefaultWeaponmodeLink_H__ */

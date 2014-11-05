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
 *      Florian Zinggeler
 *   Co-authors:
 *      ...
 *
 */

/**
    @file InvaderWeaponEnemy.h
    @brief Definition of the InvaderWeaponEnemy class.
*/

#ifndef _InvaderWeaponEnemy_H__
#define _InvaderWeaponEnemy_H__

// #include "weapons/weaponmodes/HsW01.h"
// #include "weapons/WeaponsPrereqs.h"
#include "invader/InvaderWeapon.h"
#include "tools/Timer.h"

namespace orxonox
{
    class _InvaderExport InvaderWeaponEnemy : public InvaderWeapon
    {
        public:
            InvaderWeaponEnemy(Context* context);
        protected:
            virtual void shot();
    };
}

#endif /* _InvaderWeaponEnemy_H__ */

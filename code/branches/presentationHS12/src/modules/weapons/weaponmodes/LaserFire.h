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

/**
    @file LaserFire.h
    @brief Definition of the LaserFire class.
*/

#ifndef _LaserFire_H__
#define _LaserFire_H__

#include "weapons/WeaponsPrereqs.h"
#include "weaponsystem/WeaponMode.h"

namespace orxonox
{

    /**
    @brief
        Shoots a particle laser.
    @author
        Martin Polak
    @ingroup WeaponsWeaponModes
    */
    class _WeaponsExport LaserFire : public WeaponMode
    {
        public:
            LaserFire(BaseObject* creator);
            virtual ~LaserFire() {}

            virtual void fire();

        private:
            float speed_; //!< The speed of the fired projectile.
    };
}

#endif /* _LaserFire_H__ */

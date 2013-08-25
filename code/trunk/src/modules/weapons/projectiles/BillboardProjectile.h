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

/**
    @file BillboardProjectile.h
    @brief Definition of the BillboardProjectile class.
*/

#ifndef _BillboardProjectile_H__
#define _BillboardProjectile_H__

#include "weapons/WeaponsPrereqs.h"

#include "util/Math.h"
#include "tools/BillboardSet.h"
#include "Projectile.h"

namespace orxonox
{

    /**
    @brief
        A BillboardProjectile is a projectile that is represented by a Billboard.

    @author
        Fabian 'x3n' Landau
    @ingroup WeaponsProjectiles
    */
    class _WeaponsExport BillboardProjectile : public Projectile
    {
        public:
            BillboardProjectile(Context* context);
            virtual ~BillboardProjectile();

            virtual void setColour(const ColourValue& colour);
            virtual void setMaterial(const std::string& material);
            virtual void changedVisibility();

        private:
            BillboardSet billboard_; //!< The billboard that represents the projectile.
    };
}

#endif /* _BillboardProjectile_H__ */

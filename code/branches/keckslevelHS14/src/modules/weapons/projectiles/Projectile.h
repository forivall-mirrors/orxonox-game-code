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
 *      simonmie
 *
 */

/**
    @file Projectile.h
    @brief Definition of the Projectile class.
*/

#ifndef _Projectile_H__
#define _Projectile_H__

#include "weapons/WeaponsPrereqs.h"

#include "tools/Timer.h"
#include "worldentities/MovableEntity.h"

#include "BasicProjectile.h"

namespace orxonox
{

    /**
    @brief
        Represents all 'standard' projectiles.

    @author
        Fabian 'x3n' Landau
    @author
        Simon Miescher
    @ingroup WeaponsProjectiles
    */
    class _WeaponsExport Projectile : public MovableEntity, public BasicProjectile
    {
        public:
            Projectile(Context* context);
            virtual ~Projectile();

            void setConfigValues();

            virtual void tick(float dt);
            virtual bool collidesAgainst(WorldEntity* otherObject, btManifoldPoint& contactPoint);

        private:
            float lifetime_; //!< The time the projectile exists.
            Timer destroyTimer_; //!< Timer to destroy the projectile after its lifetime has run out.
    };
}

#endif /* _Projectile_H__ */

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
    @file ParticleProjectile.h
    @brief Definition of the ParticleProjectile class.
*/

#ifndef _ParticleProjectile_H__
#define _ParticleProjectile_H__

#include "weapons/WeaponsPrereqs.h"
#include "BillboardProjectile.h"

namespace orxonox
{

    /**
    @brief
        A projectile that is represented by particles.
    @author
        Fabian 'x3n' Landau
    @ingroup WeaponsProjectiles
    */
    class _WeaponsExport ParticleProjectile : public BillboardProjectile
    {
        public:
            ParticleProjectile(Context* context);
            virtual ~ParticleProjectile();
            virtual void changedVisibility();

        private:
            ParticleInterface* particles_; //!< The particles.
    };
}

#endif /* _ParticleProjectile_H__ */

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
 *      Joel Smely
 *   Co-authors:
 *      ...
 *
 */

/**
    @file LightningGunProjectile.h
    @brief Definition of the LightningGunProjectile class.
*/

#ifndef _LightningGunProjectile_H__
#define _LightningGunProjectile_H__

#include "weapons/WeaponsPrereqs.h"

#include <string>
#include "tools/Timer.h"
#include "BillboardProjectile.h"

namespace orxonox
{

    /**
    @brief
        The LightningGunProjectile is a projectile that is represented by a looped series of billboards.
        
    @author
        Joel Smely
    @ingroup WeaponsProjectiles
    */
    class _WeaponsExport LightningGunProjectile : public BillboardProjectile
    {
        public:
            LightningGunProjectile(Context* context);
            virtual ~LightningGunProjectile() {}

            virtual void setMaterial(const std::string& material);

        private:
            void registerVariables();
            void changeTexture();
            
            unsigned int textureIndex_; //!< The current index of the texture. (i.e. the index of the currently displayed texture)
            unsigned int maxTextureIndex_; //!< The maximal index.
            Timer textureTimer_; //!< A timer that loops and changes textures each time it expires.
            std::string materialBase_; //!< The base name of the material.
    };
}

#endif /* _LightningGunProjectile_H__ */

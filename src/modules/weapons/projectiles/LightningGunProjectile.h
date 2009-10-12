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

#ifndef _LightningGunProjectile_H__
#define _LightningGunProjectile_H__

#include "weapons/WeaponsPrereqs.h"

#include <string>
#include "tools/Timer.h"
#include "BillboardProjectile.h"

namespace orxonox
{
    class _WeaponsExport LightningGunProjectile : public BillboardProjectile
    {
        public:
            LightningGunProjectile(BaseObject* creator);
            virtual ~LightningGunProjectile() {}

            virtual void setMaterial(const std::string& material);

        protected:
            void changeTexture();
            unsigned int textureIndex_;
            unsigned int maxTextureIndex_;
            Timer textureTimer_;
            std::string materialBase_;
      private:
            void registerVariables();
    };
}

#endif /* _LightningGunProjectile_H__ */

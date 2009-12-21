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

#include "LightningGunProjectile.h"

#include "util/Convert.h"
#include "core/CoreIncludes.h"

namespace orxonox
{
    CreateFactory(LightningGunProjectile);

    LightningGunProjectile::LightningGunProjectile(BaseObject* creator) : BillboardProjectile(creator)
    {
        RegisterObject(LightningGunProjectile);

        this->textureIndex_ = 1;
        this->maxTextureIndex_ = 8;
        this->textureTimer_.setTimer(0.01f, true, createExecutor(createFunctor(&LightningGunProjectile::changeTexture, this)));

        registerVariables();
    }

    void LightningGunProjectile::registerVariables()
    {
        registerVariable(this->materialBase_);
    }

    void LightningGunProjectile::setMaterial(const std::string& material)
    {
        this->materialBase_ = material;    

        BillboardProjectile::setMaterial(material + multi_cast<std::string>(this->textureIndex_));
    }

    void LightningGunProjectile::changeTexture()
    {
        this->textureIndex_++;
        if (this->textureIndex_ > this->maxTextureIndex_)
            this->textureIndex_ = 1;

        this->setMaterial(this->materialBase_);
    }
}

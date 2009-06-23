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

#include "OrxonoxStableHeaders.h"
#include "ParticleProjectile.h"

#include <OgreParticleSystem.h>
#include <OgreParticleEmitter.h>

#include "core/Core.h"
#include "core/CoreIncludes.h"
#include "core/ConfigValueIncludes.h"
#include "objects/Scene.h"

namespace orxonox
{
    CreateFactory(ParticleProjectile);

    ParticleProjectile::ParticleProjectile(BaseObject* creator) : BillboardProjectile(creator)
    {
        RegisterObject(ParticleProjectile);

        if (Core::showsGraphics())
        {
            this->particles_ = new ParticleInterface(this->getScene()->getSceneManager(), "Orxonox/shot3_small", LODParticle::normal);
            this->attachOgreObject(this->particles_->getParticleSystem());
            this->particles_->setKeepParticlesInLocalSpace(0);

            this->particles_->getAllEmitters()->setDirection(-WorldEntity::FRONT);
        }
        else
            this->particles_ = 0;
    }

    ParticleProjectile::~ParticleProjectile()
    {
        if (this->isInitialized() && this->particles_)
        {
            this->detachOgreObject(this->particles_->getParticleSystem());
            delete this->particles_;
        }
    }

    void ParticleProjectile::changedVisibility()
    {
        SUPER(ParticleProjectile, changedVisibility);

        if (this->particles_)
            this->particles_->setEnabled(this->isVisible());
    }
}

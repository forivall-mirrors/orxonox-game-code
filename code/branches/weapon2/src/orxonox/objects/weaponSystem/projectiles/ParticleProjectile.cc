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

#include "../../worldentities/pawns/SpaceShip.h"
#include "core/CoreIncludes.h"
#include "core/ConfigValueIncludes.h"

namespace orxonox
{
    CreateFactory(ParticleProjectile);

    ParticleProjectile::ParticleProjectile(BaseObject* creator) : BillboardProjectile(creator)
    {
        RegisterObject(ParticleProjectile);

        this->particles_ = new ParticleInterface(this->getSceneManager(), "Orxonox/shot2", LODParticle::normal);
        this->particles_->addToSceneNode(this->getNode());
        this->particles_->setKeepParticlesInLocalSpace(true);

        /*
        if (this->owner_)
        {
        }
//        else
//        {
//            this->particles_ = 0;
//        }
        */

        this->setConfigValues();
    }

    ParticleProjectile::~ParticleProjectile()
    {
        if (this->isInitialized() && this->particles_)
            delete this->particles_;
    }

    void ParticleProjectile::setConfigValues()
    {
        //SetConfigValue(speed_, 5000.0).description("The speed of a projectile in units per second").callback((Projectile*)this, &ParticleProjectile::speedChanged);
    }

    void ParticleProjectile::changedVisibility()
    {
        SUPER(ParticleProjectile, changedVisibility);
        this->particles_->setEnabled(this->isVisible());
    }

    bool ParticleProjectile::create(){
      if(!Projectile::create())
        return false;
      this->particles_->getAllEmitters()->setDirection(-this->getOrientation()*Vector3(1,0,0));
      return true;
    }
}

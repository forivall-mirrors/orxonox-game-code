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
#include "Projectile.h"

#include <OgreBillboard.h>

#include "core/CoreIncludes.h"
#include "core/Executor.h"
#include "core/ConfigValueIncludes.h"
#include "core/Iterator.h"
#include "tools/ParticleInterface.h"

#include "SpaceShipAI.h"
#include "ParticleSpawner.h"
#include "Model.h"
#include "Settings.h"

namespace orxonox
{
    float Projectile::speed_s = 5000;

    Projectile::Projectile(Weapon* owner) : owner_(owner)
    {
        RegisterObject(Projectile);

        this->setConfigValues();
        this->explosionTemplateName_ = "Orxonox/explosion3";
        this->smokeTemplateName_ = "Orxonox/smoke4";

        this->setStatic(false);
        this->translate(Vector3(55, 0, 0), Ogre::Node::TS_LOCAL);

        if (this->owner_)
        {
            this->setOrientation(this->owner_->getOrientation());
            this->setPosition(this->owner_->getPosition());
            this->setVelocity(this->owner_->getInitialDir() * this->speed_);
        }

        if(!orxonox::Settings::isClient()) //only if not on client
          this->destroyTimer_.setTimer(this->lifetime_, false, this, createExecutor(createFunctor(&Projectile::destroyObject)));
    }

    Projectile::~Projectile()
    {
    }

    void Projectile::setConfigValues()
    {
        SetConfigValue(damage_, 15.0).description("The damage caused by the projectile");
        SetConfigValue(lifetime_, 4.0).description("The time in seconds a projectile stays alive");
        SetConfigValue(speed_, 5000.0).description("The speed of a projectile in units per second").callback(this, &Projectile::speedChanged);
    }

    void Projectile::speedChanged()
    {
        Projectile::speed_s = this->speed_;
        if (this->owner_)
            this->setVelocity(this->owner_->getInitialDir() * this->speed_);
    }

    void Projectile::tick(float dt)
    {
        SUPER(Projectile, tick, dt);

        if (!this->isActive())
            return;

        float radius;
        for (ObjectList<Model>::iterator it = ObjectList<Model>::begin(); it; ++it)
        {
            if ((*it) != this->owner_)
            {
                radius = it->getScale3D().x * 3.0;

                if (this->getPosition().squaredDistance(it->getPosition()) <= (radius*radius))
                {
                    // hit
                    if (it->isA(Class(SpaceShipAI)))
                        ((SpaceShipAI*)(*it))->damage(this->damage_);
                    ParticleSpawner* explosion = new ParticleSpawner(this->explosionTemplateName_, LODParticle::low, 2.0);
                    explosion->setPosition(this->getPosition());
                    explosion->create();
                    ParticleSpawner* smoke = new ParticleSpawner(this->smokeTemplateName_, LODParticle::normal, 2.0, 0.0);
                    smoke->setPosition(this->getPosition());
//                    smoke->getParticleInterface()->setSpeedFactor(3.0);
                    smoke->create();
                    delete this;
                    return;
                }
            }
        }
    }

    void Projectile::destroyObject()
    {
        delete this;
    }

    bool Projectile::create(){
      return WorldEntity::create();
    }
}

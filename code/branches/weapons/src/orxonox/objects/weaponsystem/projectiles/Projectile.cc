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

#include "objects/worldentities/Model.h"
#include "objects/worldentities/ParticleSpawner.h"
#include "objects/collisionshapes/SphereCollisionShape.h"
#include "core/GameMode.h"

namespace orxonox
{
    Projectile::Projectile(BaseObject* creator) : MovableEntity(creator)
    {
        RegisterObject(Projectile);

        this->setConfigValues();
        this->bDestroy_ = false;
        this->owner_ = 0;

        // Get notification about collisions

        if (GameMode::isMaster())
        {
            this->enableCollisionCallback();

            this->setCollisionType(Kinematic);

            SphereCollisionShape* shape = new SphereCollisionShape(this);
            shape->setRadius(10);
            this->attachCollisionShape(shape);

            this->destroyTimer_.setTimer(this->lifetime_, false, this, createExecutor(createFunctor(&Projectile::destroyObject)));
        }
    }

    Projectile::~Projectile()
    {
    }

    void Projectile::setConfigValues()
    {
        SetConfigValue(damage_, 15.0).description("The damage caused by the projectile");
        SetConfigValue(lifetime_, 4.0).description("The time in seconds a projectile stays alive");
    }


    void Projectile::tick(float dt)
    {
        SUPER(Projectile, tick, dt);

        if (!this->isActive())
            return;

        if (this->bDestroy_)
            delete this;
    }

    void Projectile::destroyObject()
    {
        if (GameMode::isMaster())
            delete this;
    }

    bool Projectile::collidesAgainst(WorldEntity* otherObject, btManifoldPoint& contactPoint)
    {
        if (!this->bDestroy_ && GameMode::isMaster())
        {
            if (otherObject == this->owner_)
                return true;

            this->bDestroy_ = true;

            if (this->owner_)
            {
                {
                    ParticleSpawner* effect = new ParticleSpawner(this->owner_->getCreator());
                    effect->setPosition(this->getPosition());
                    effect->setOrientation(this->getOrientation());
                    effect->setDestroyAfterLife(true);
                    effect->setSource("Orxonox/explosion3");
                    effect->setLifetime(2.0f);
                }
                {
                    ParticleSpawner* effect = new ParticleSpawner(this->owner_->getCreator());
                    effect->setPosition(this->getPosition());
                    effect->setOrientation(this->getOrientation());
                    effect->setDestroyAfterLife(true);
                    effect->setSource("Orxonox/smoke4");
                    effect->setLifetime(3.0f);
                }
            }

            Pawn* victim = dynamic_cast<Pawn*>(otherObject);
            if (victim)
                victim->damage(this->damage_, this->owner_);
        }
        return false;
    }

    void Projectile::destroyedPawn(Pawn* pawn)
    {
        if (this->owner_ == pawn)
            this->owner_ = 0;
    }
}

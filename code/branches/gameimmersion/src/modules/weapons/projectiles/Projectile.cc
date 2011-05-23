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

#include "Projectile.h"

#include "core/CoreIncludes.h"
#include "core/ConfigValueIncludes.h"
#include "core/GameMode.h"
#include "core/command/Executor.h"
#include "objects/collisionshapes/SphereCollisionShape.h"
#include "worldentities/pawns/Pawn.h"
#include "graphics/ParticleSpawner.h"

namespace orxonox
{
    CreateFactory(Projectile);

    Projectile::Projectile(BaseObject* creator) : MovableEntity(creator), BasicProjectile()
    {
        RegisterObject(Projectile);

        this->setConfigValues();
//        this->bDestroy_ = false;
//        this->owner_ = 0;
//        this->damage_ = 115;
///////////////////me
//        this->healthdamage_ = 0;
//        this->shielddamage_ = 0;
///////////////////end me

        // Get notification about collisions

        if (GameMode::isMaster())
        {
            this->setMass(1.0);
            this->enableCollisionCallback();
            this->setCollisionResponse(false);
            this->setCollisionType(Kinematic);

            SphereCollisionShape* shape = new SphereCollisionShape(this);
            shape->setRadius(20);
            this->attachCollisionShape(shape);

            this->destroyTimer_.setTimer(this->lifetime_, false, createExecutor(createFunctor(&Projectile::destroyObject, this)));
        }
    }

    Projectile::~Projectile()
    {
    }

    void Projectile::setConfigValues()
    {
        SetConfigValue(lifetime_, 4.0).description("The time in seconds a projectile stays alive");
    }


    void Projectile::tick(float dt)
    {
        SUPER(Projectile, tick, dt);

        if (!this->isActive())
            return;

        if (this->getBDestroy())
            this->destroy(); // TODO: use a scheduler instead of deleting the object right here in tick()
    }

    void Projectile::destroyObject()
    {
        if (GameMode::isMaster())
            this->destroy();
    }

    bool Projectile::collidesAgainst(WorldEntity* otherObject, btManifoldPoint& contactPoint)
    {
        return BasicProjectile::basicCollidesAgainst(otherObject,contactPoint,this->getOwner(),this);
    }

//////////////////////////me edit
/*    bool Projectile::collidesAgainst(WorldEntity* otherObject, btManifoldPoint& contactPoint)
    {
        if (!this->bDestroy_ && GameMode::isMaster())
        {
            if (otherObject == this->owner_)
                return false;

            this->bDestroy_ = true;

            Pawn* victim = orxonox_cast<Pawn*>(otherObject); //if otherObject isn't a Pawn, then victim is NULL

            if (this->owner_)
            {
                if (!victim || (victim && !victim->hasShield())) //same like below
                {
                    ParticleSpawner* effect = new ParticleSpawner(this->owner_->getCreator());
                    effect->setPosition(this->getPosition());
                    effect->setOrientation(this->getOrientation());
                    effect->setDestroyAfterLife(true);
                    effect->setSource("Orxonox/explosion3");
                    effect->setLifetime(2.0f);
                }
                if (!victim || (victim && !victim->hasShield())) //same like above
                {
                    ParticleSpawner* effect = new ParticleSpawner(this->owner_->getCreator());
                    effect->setPosition(this->getPosition());
                    effect->setOrientation(this->getOrientation());
                    effect->setDestroyAfterLife(true);
                    effect->setSource("Orxonox/smoke4");
                    effect->setLifetime(3.0f);
                }
                if (victim && victim->hasShield())
                {
                    ParticleSpawner* effect = new ParticleSpawner(this->owner_->getCreator());
                    effect->setPosition(this->getPosition());
                    effect->setOrientation(this->getOrientation());
                    effect->setDestroyAfterLife(true);
                    effect->setSource("Orxonox/engineglow");
                    effect->setLifetime(0.5f);
                }
            }

            if (victim)
            {
                victim->hit(this->owner_, contactPoint, this->damage_, this->healthdamage_, this->shielddamage_);
                victim->startReloadCountdown();
            }
        }
        return false;
    }
//////////////////////////////////////////////////////////////////////end edit
*/
    void Projectile::setOwner(Pawn* owner)
    {
        this->owner_ = owner;
    }
}

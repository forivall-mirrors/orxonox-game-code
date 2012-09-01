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
    @brief Implementation of the Projectile class.
*/

#include "Projectile.h"

#include "core/ConfigValueIncludes.h"
#include "core/CoreIncludes.h"
#include "core/GameMode.h"
#include "core/command/Executor.h"

#include "objects/collisionshapes/SphereCollisionShape.h"
#include "worldentities/pawns/Pawn.h"

namespace orxonox
{
    CreateFactory(Projectile);

    Projectile::Projectile(BaseObject* creator) : MovableEntity(creator), BasicProjectile()
    {
        RegisterObject(Projectile);

        this->setConfigValues();

        // Get notification about collisions
        if (GameMode::isMaster())
        {
            this->setMass(1.0f);
            this->enableCollisionCallback();
            this->setCollisionResponse(false);
            this->setCollisionType(Kinematic);

            SphereCollisionShape* shape = new SphereCollisionShape(this);
            shape->setRadius(20.0f);
            this->attachCollisionShape(shape);

            this->destroyTimer_.setTimer(this->lifetime_, false, createExecutor(createFunctor(&BasicProjectile::destroyObject, this)));
        }
    }

    Projectile::~Projectile()
    {
    }

    void Projectile::setConfigValues()
    {
        SetConfigValue(lifetime_, 4.0f).description("The time in seconds a projectile stays alive");
    }


    void Projectile::tick(float dt)
    {
        SUPER(Projectile, tick, dt);

        if (!this->isActive())
            return;

        this->destroyCheck();
    }

    bool Projectile::collidesAgainst(WorldEntity* otherObject, btManifoldPoint& contactPoint)
    {
        return this->processCollision(otherObject, contactPoint);
    }

}

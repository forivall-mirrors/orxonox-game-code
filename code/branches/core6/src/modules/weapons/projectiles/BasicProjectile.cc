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
 *      simonmie
 *   Co-authors:
 *      ...
 *
 */

/**
    @file BasicProjectile.h
    @brief Implementation of the BasicProjectile class.
*/

#include "BasicProjectile.h"

#include "core/CoreIncludes.h"
#include "core/GameMode.h"
#include "core/command/Executor.h"

#include "graphics/ParticleSpawner.h"

namespace orxonox
{
    RegisterClassNoArgs(BasicProjectile);

    /**
    @brief
        Constructor. Registers the object and initializes some default values.
    */
    BasicProjectile::BasicProjectile()
    {
        RegisterRootObject(BasicProjectile);// Register the BasicProjectile class to the core

        this->bDestroy_ = false;

        // Default damage must be zero, otherwise it would be above zero if no settings are made in the weaponsettings xml file.
        // same thing for all weaponmodes files
        this->damage_ = 0.0f;
        this->healthdamage_ = 0.0f;
        this->shielddamage_ = 0.0f;
    }

    BasicProjectile::~BasicProjectile()
    {
    }

    /**
    @brief
        The function called when a projectile hits another thing.
        Calls the hit-function, starts the reload countdown, displays visual hit effects defined in Pawn.
        Needs to be called in the collidesAgainst() function by every Class directly inheriting from BasicProjectile.
    @param otherObject
        A pointer to the object the Projectile has collided against.
    @param contactPoint
        A btManifoldPoint indicating the point of contact/impact.
    @return
        Returns true if the collision resulted in a successful hit.
    @see Pawn.h
    */
    bool BasicProjectile::processCollision(WorldEntity* otherObject, btManifoldPoint& contactPoint)
    {
        if (!this->bDestroy_ && GameMode::isMaster())
        {
            if (otherObject == this->getShooter()) // Prevents you from shooting yourself
                return false;

            this->bDestroy_ = true; // If something is hit, the object is destroyed and can't hit something else.
                                    // The projectile is destroyed by its tick()-function (in the following tick).

            Pawn* victim = orxonox_cast<Pawn*>(otherObject); // If otherObject isn't a Pawn, then victim is NULL

            WorldEntity* entity = orxonox_cast<WorldEntity*>(this);
            assert(entity); // The projectile must not be a WorldEntity.

            // If visual effects after destruction cause problems, put this block below the effects code block
            if (victim)
            {
                victim->hit(this->getShooter(), contactPoint, this->getDamage(), this->getHealthDamage(), this->getShieldDamage());
                victim->startReloadCountdown();
            }

            // Visual effects for being hit, depending on whether the shield is hit or not
            if (this->getShooter()) // If the owner does not exist (anymore?), no effects are displayed.
            {
                // Damping and explosion effect is only played if the victim is no Pawn (see cast above)
                // or if the victim is a Pawn, has no shield left, is still alive and any damage goes to the health
                if (!victim || (victim && !victim->hasShield() && victim->getHealth() > 0.0f && (this->getDamage() > 0.0f || this->getHealthDamage() > 0.0f)))
                {
                    {
                        ParticleSpawner* effect = new ParticleSpawner(this->getShooter()->getContext());
                        effect->setPosition(entity->getPosition());
                        effect->setOrientation(entity->getOrientation());
                        effect->setDestroyAfterLife(true);
                        effect->setSource("Orxonox/explosion3");
                        effect->setLifetime(2.0f);
                    }
                    // Second effect with same condition
                    {
                        ParticleSpawner* effect = new ParticleSpawner(this->getShooter()->getContext());
                        effect->setPosition(entity->getPosition());
                        effect->setOrientation(entity->getOrientation());
                        effect->setDestroyAfterLife(true);
                        effect->setSource("Orxonox/smoke4");
                        effect->setLifetime(3.0f);
                    }
                }

                // victim->isAlive() is not false until the next tick, so getHealth() > 0 is used instead
                if (victim && victim->hasShield() && (this->getDamage() > 0.0f || this->getShieldDamage() > 0.0f) && victim->getHealth() > 0.0f)
                {
                    ParticleSpawner* effect = new ParticleSpawner(this->getShooter()->getContext());
                    effect->setDestroyAfterLife(true);
                    effect->setSource("Orxonox/Shield");
                    effect->setLifetime(0.5f);
                    victim->attach(effect);
                }
            }
            return true;
        }
        return false;
    }

    /**
    @brief
        Check whether the projectile needs to be destroyed and destroys it if so.
        Needs to be called in the tick() by every Class directly inheriting from BasicProjectile, to make sure the projectile is destroyed after it has hit something.
    */
    void BasicProjectile::destroyCheck(void)
    {
        if(GameMode::isMaster() && this->bDestroy_)
            this->destroy();
    }

    /**
    @brief
        Destroys the object.
    */
    void BasicProjectile::destroyObject(void)
    {
        if(GameMode::isMaster())
            this->destroy();
    }
}

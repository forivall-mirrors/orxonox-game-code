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

#include "BasicProjectile.h"

#include "core/CoreIncludes.h"
#include "core/ConfigValueIncludes.h"
#include "core/GameMode.h"
#include "core/command/Executor.h"
#include "objects/collisionshapes/SphereCollisionShape.h"
#include "worldentities/pawns/Pawn.h"
#include "graphics/ParticleSpawner.h"
#include "core/OrxonoxClass.h"

namespace orxonox
{
    /**
    @brief
        Constructor. Registers the object and initializes some default values.
    */
    BasicProjectile::BasicProjectile() : OrxonoxClass()
    {
        RegisterRootObject(BasicProjectile);// - register the BasicProjectile class to the core

        this->bDestroy_ = false;

        this->damage_ = 0;
        this->healthdamage_ = 0;
        this->shielddamage_ = 0;
    }

    BasicProjectile::~BasicProjectile()
    {
    }

    bool BasicProjectile::basicCollidesAgainst(WorldEntity* otherObject, btManifoldPoint& contactPoint, Pawn* owner, BasicProjectile* this_)
    {
        if (!this_->getBDestroy() && GameMode::isMaster())
        {
            if (otherObject == /*this->*/owner/*_*/) //prevents you from shooting yourself
                return false;

            this_->setBDestroy(true); //if something is hit, the object is destroyed and can't hit something else
// instead of returning false, bDestroy is returned

            Pawn* victim = orxonox_cast<Pawn*>(otherObject); //if otherObject isn't a Pawn, then victim is NULL

            WorldEntity* entity = orxonox_cast<WorldEntity*>(this_);
            assert(entity); //entity must not be null


            // if visual effects after destruction cause problems, put this block below the effects code block
            if (victim)
            {
                victim->hit(/*this->*/owner/*_*/, contactPoint, this_->getDamage(), this_->getHealthDamage(), this_->getShieldDamage());
                victim->startReloadCountdown();
            }

            // visual effects for being hit, depending on whether the shield is hit or not
            if (/*this->*/owner/*_*/) //if the owner does not exist (anymore??), no effects are displayed.
            {
                if (!victim || (victim && !victim->hasShield())) //same like below
                {
                    {
                        ParticleSpawner* effect = new ParticleSpawner(/*this->*/owner/*_*/->getCreator());
                        effect->setPosition(entity->getPosition());
                        effect->setOrientation(entity->getOrientation());
                        effect->setDestroyAfterLife(true);
                        effect->setSource("Orxonox/explosion3");
                        effect->setLifetime(2.0f);
                    }
                        // second effect with same condition
                    {
                        ParticleSpawner* effect = new ParticleSpawner(/*this->*/owner/*_*/->getCreator());
                        effect->setPosition(entity->getPosition());
                        effect->setOrientation(entity->getOrientation());
                        effect->setDestroyAfterLife(true);
                        effect->setSource("Orxonox/smoke4");
                        effect->setLifetime(3.0f);
                    }
                }
                        // victim->isAlive() is not false until the next tick, so getHealth() is used instead
                if (victim && victim->hasShield() && (this_->getDamage() > 0 || this_->getShieldDamage() > 0) && victim->getHealth() > 0)
                {
                    ParticleSpawner* effect = new ParticleSpawner(/*this->*/owner/*_*/->getCreator());
                    effect->setPosition(entity->getPosition());
                    effect->setOrientation(entity->getOrientation());
                    effect->setDestroyAfterLife(true);
                    effect->setSource("Orxonox/Shield");
                    effect->setLifetime(0.5f);
                }
            }

//            if (victim)
//            {
//                victim->hit(/*this->*/owner/*_*/, contactPoint, this_->getDamage(), this_->getHealthDamage(), this_->getShieldDamage());
//                victim->startReloadCountdown();
//            }
        }
        return false;
    }

/*    void BasicProjectile::destroyObject()
    {
        if (GameMode::isMaster())
            this->destroy();
    }
*/
}

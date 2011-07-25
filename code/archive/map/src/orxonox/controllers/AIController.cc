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

#include "AIController.h"

#include "util/Math.h"
#include "core/CoreIncludes.h"
#include "core/Executor.h"
#include "worldentities/ControllableEntity.h"

namespace orxonox
{
    static const float ACTION_INTERVAL = 1.0f;

    CreateFactory(AIController);

    AIController::AIController(BaseObject* creator) : ArtificialController(creator)
    {
        RegisterObject(AIController);

        this->actionTimer_.setTimer(ACTION_INTERVAL, true, createExecutor(createFunctor(&AIController::action, this)));
    }

    AIController::~AIController()
    {
    }

    void AIController::action()
    {
        float random;
        float maxrand = 100.0f / ACTION_INTERVAL;

        // search enemy
        random = rnd(maxrand);
        if (random < 15 && (!this->target_))
            this->searchNewTarget();

        // forget enemy
        random = rnd(maxrand);
        if (random < 5 && (this->target_))
            this->forgetTarget();

        // next enemy
        random = rnd(maxrand);
        if (random < 10 && (this->target_))
            this->searchNewTarget();

        // fly somewhere
        random = rnd(maxrand);
        if (random < 50 && (!this->bHasTargetPosition_ && !this->target_))
            this->searchRandomTargetPosition();

        // stop flying
        random = rnd(maxrand);
        if (random < 10 && (this->bHasTargetPosition_ && !this->target_))
            this->bHasTargetPosition_ = false;

        // fly somewhere else
        random = rnd(maxrand);
        if (random < 30 && (this->bHasTargetPosition_ && !this->target_))
            this->searchRandomTargetPosition();

        // shoot
        random = rnd(maxrand);
        if (random < 75 && (this->target_ && !this->bShooting_))
            this->bShooting_ = true;

        // stop shooting
        random = rnd(maxrand);
        if (random < 25 && (this->bShooting_))
            this->bShooting_ = false;
    }

    void AIController::tick(float dt)
    {
        if (!this->isActive())
            return;

        if (this->target_)
            this->aimAtTarget();

        if (this->bHasTargetPosition_)
            this->moveToTargetPosition();

        if (this->getControllableEntity() && this->bShooting_ && this->isCloseAtTarget(1000) && this->isLookingAtTarget(Ogre::Math::PI / 20.0f))
            this->getControllableEntity()->fire(0);

        SUPER(AIController, tick, dt);
    }

}

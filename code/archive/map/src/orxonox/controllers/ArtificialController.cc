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

#include "ArtificialController.h"

#include "core/CoreIncludes.h"
#include "worldentities/ControllableEntity.h"
#include "worldentities/pawns/Pawn.h"
#include "worldentities/pawns/TeamBaseMatchBase.h"
#include "gametypes/TeamDeathmatch.h"
#include "controllers/WaypointPatrolController.h"

namespace orxonox
{
    ArtificialController::ArtificialController(BaseObject* creator) : Controller(creator)
    {
        RegisterObject(ArtificialController);

        this->target_ = 0;
        this->bShooting_ = false;
        this->bHasTargetPosition_ = false;
        this->targetPosition_ = Vector3::ZERO;

        this->target_.setCallback(createFunctor(&ArtificialController::targetDied, this));
    }

    ArtificialController::~ArtificialController()
    {
    }

    void ArtificialController::moveToPosition(const Vector3& target)
    {
        if (!this->getControllableEntity())
            return;

        Vector2 coord = get2DViewdirection(this->getControllableEntity()->getPosition(), this->getControllableEntity()->getOrientation() * WorldEntity::FRONT, this->getControllableEntity()->getOrientation() * WorldEntity::UP, target);
        float distance = (target - this->getControllableEntity()->getPosition()).length();

        if (this->target_ || distance > 10)
        {
            // Multiply with 0.8 to make them a bit slower
            this->getControllableEntity()->rotateYaw(-0.8f * sgn(coord.x) * coord.x*coord.x);
            this->getControllableEntity()->rotatePitch(0.8f * sgn(coord.y) * coord.y*coord.y);
        }

        if (this->target_ && distance < 200 && this->getControllableEntity()->getVelocity().squaredLength() > this->target_->getVelocity().squaredLength())
            this->getControllableEntity()->moveFrontBack(-0.5f); // They don't brake with full power to give the player a chance
        else
            this->getControllableEntity()->moveFrontBack(0.8f);
    }

    void ArtificialController::moveToTargetPosition()
    {
        this->moveToPosition(this->targetPosition_);
    }

    void ArtificialController::setTargetPosition(const Vector3& target)
    {
        this->targetPosition_ = target;
        this->bHasTargetPosition_ = true;
    }

    void ArtificialController::searchRandomTargetPosition()
    {
        this->targetPosition_ = Vector3(rnd(-2000,2000), rnd(-2000,2000), rnd(-2000,2000));
        this->bHasTargetPosition_ = true;
    }

    void ArtificialController::setTarget(Pawn* target)
    {
        this->target_ = target;

        if (target)
            this->targetPosition_ = target->getPosition();
    }

    void ArtificialController::searchNewTarget()
    {
        if (!this->getControllableEntity())
            return;

        this->targetPosition_ = this->getControllableEntity()->getPosition();
        this->forgetTarget();

        for (ObjectList<Pawn>::iterator it = ObjectList<Pawn>::begin(); it; ++it)
        {
            if (ArtificialController::sameTeam(this->getControllableEntity(), static_cast<ControllableEntity*>(*it), this->getGametype()))
                continue;

            if (static_cast<ControllableEntity*>(*it) != this->getControllableEntity())
            {
                float speed = this->getControllableEntity()->getVelocity().length();
                Vector3 distanceCurrent = this->targetPosition_ - this->getControllableEntity()->getPosition();
                Vector3 distanceNew = it->getPosition() - this->getControllableEntity()->getPosition();
                if (!this->target_ || it->getPosition().squaredDistance(this->getControllableEntity()->getPosition()) * (1.5f + acos((this->getControllableEntity()->getOrientation() * WorldEntity::FRONT).dotProduct(distanceNew) / speed / distanceNew.length()) / (2 * Ogre::Math::PI))
                        < this->targetPosition_.squaredDistance(this->getControllableEntity()->getPosition()) * (1.5f + acos((this->getControllableEntity()->getOrientation() * WorldEntity::FRONT).dotProduct(distanceCurrent) / speed / distanceCurrent.length()) / (2 * Ogre::Math::PI)) + rnd(-250, 250))
                {
                    this->target_ = (*it);
                    this->targetPosition_ = it->getPosition();
                }
            }
        }
    }

    void ArtificialController::forgetTarget()
    {
        this->target_ = 0;
        this->bShooting_ = false;
    }

    void ArtificialController::aimAtTarget()
    {
        if (!this->target_ || !this->getControllableEntity())
            return;

        static const float hardcoded_projectile_speed = 1250;

        this->targetPosition_ = getPredictedPosition(this->getControllableEntity()->getPosition(), hardcoded_projectile_speed, this->target_->getPosition(), this->target_->getVelocity());
        this->bHasTargetPosition_ = (this->targetPosition_ != Vector3::ZERO);

        Pawn* pawn = dynamic_cast<Pawn*>(this->getControllableEntity());
        if (pawn)
            pawn->setAimPosition(this->targetPosition_);
    }

    bool ArtificialController::isCloseAtTarget(float distance) const
    {
        if (!this->getControllableEntity())
            return false;

        if (!this->target_)
            return (this->getControllableEntity()->getPosition().squaredDistance(this->targetPosition_) < distance*distance);
        else
            return (this->getControllableEntity()->getPosition().squaredDistance(this->target_->getPosition()) < distance*distance);
    }

    bool ArtificialController::isLookingAtTarget(float angle) const
    {
        if (!this->getControllableEntity())
            return false;

        return (getAngle(this->getControllableEntity()->getPosition(), this->getControllableEntity()->getOrientation() * WorldEntity::FRONT, this->targetPosition_) < angle);
    }

    void ArtificialController::abandonTarget(Pawn* target)
    {
        if (target == this->target_)
            this->targetDied();
    }

    void ArtificialController::targetDied()
    {
        this->forgetTarget();
        this->searchRandomTargetPosition();
    }

    bool ArtificialController::sameTeam(ControllableEntity* entity1, ControllableEntity* entity2, Gametype* gametype)
    {
        if (entity1 == entity2)
            return true;

        int team1 = -1;
        int team2 = -1;

        if (entity1->getXMLController())
        {
            WaypointPatrolController* wpc = orxonox_cast<WaypointPatrolController*>(entity1->getXMLController());
            if (wpc)
                team1 = wpc->getTeam();
        }
        if (entity2->getXMLController())
        {
            WaypointPatrolController* wpc = orxonox_cast<WaypointPatrolController*>(entity2->getXMLController());
            if (wpc)
                team2 = wpc->getTeam();
        }

        TeamDeathmatch* tdm = orxonox_cast<TeamDeathmatch*>(gametype);
        if (tdm)
        {
            if (entity1->getPlayer())
                team1 = tdm->getTeam(entity1->getPlayer());

            if (entity2->getPlayer())
                team2 = tdm->getTeam(entity2->getPlayer());
        }

        TeamBaseMatchBase* base = 0;
        base = orxonox_cast<TeamBaseMatchBase*>(entity1);
        if (base)
        {
            switch (base->getState())
            {
                case BaseState::ControlTeam1:
                    team1 = 0;
                    break;
                case BaseState::ControlTeam2:
                    team1 = 1;
                    break;
                case BaseState::Uncontrolled:
                default:
                    team1 = -1;
            }
        }
        base = orxonox_cast<TeamBaseMatchBase*>(entity2);
        if (base)
        {
            switch (base->getState())
            {
                case BaseState::ControlTeam1:
                    team2 = 0;
                    break;
                case BaseState::ControlTeam2:
                    team2 = 1;
                    break;
                case BaseState::Uncontrolled:
                default:
                    team2 = -1;
            }
        }

        return (team1 == team2 && team1 != -1);
    }
}

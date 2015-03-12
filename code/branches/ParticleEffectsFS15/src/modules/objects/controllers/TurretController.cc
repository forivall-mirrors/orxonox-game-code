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
 *      Martin Mueller
 *   Co-authors:
 *      ...
 *
 */

#include "TurretController.h"
#include "worldentities/pawns/Pawn.h"
#include "objects/Turret.h"

 namespace orxonox
 {
    RegisterClass(TurretController);

    /**
        @brief 
        Sets default values for all variables.

        @param context
        The context
    */
    TurretController::TurretController(Context* context) : ArtificialController(context)
    {
        RegisterObject(TurretController);

        this->once_ = false;

    }

    /**
        @brief 
        Destructor. Nothing to see here.
    */
    TurretController::~TurretController()
    {

    }

    /**
        @brief 
        Searches a valid target for the turret to aim at.

        Loops through all pawns and tests, if it is in range. Scores every pawn and chooses the best one (the one with the lowest score).
        If the turret has a parent, try to aim at the same target the parent has, if there is one.

        @see targetScore
        The function that scores the pawns.
    */
    void TurretController::searchTarget()
    {
        Turret* turret = orxonox_cast<Turret*>(this->getControllableEntity());

        //The controller might find a target before teams are set, so we need to check again here.
        if(this->target_ && turret->isInRange(target_) != -1.f && !FormationController::sameTeam(turret, this->target_, this->getGametype()))
        {
            return;
        }
        else
        {
            this->forgetTarget();
            turret->setTarget(0);
        }


        ControllableEntity* parent = orxonox_cast<ControllableEntity*>(turret->getParent());
        if(parent)
        {
            Pawn* parenttarget = orxonox_cast<Pawn*>(parent->getTarget());
            if(parenttarget && turret->isInRange(parenttarget))
            {
                this->setTarget(parenttarget);
                turret->setTarget(parenttarget);
                return;
            }
        }

        float minScore = FLT_MAX;
        float tempScore;
        Pawn* minScorePawn = 0;

        for (ObjectList<Pawn>::iterator it = ObjectList<Pawn>::begin(); it != ObjectList<Pawn>::end(); ++it)
        {
            Pawn* entity = orxonox_cast<Pawn*>(*it);
            if (!entity || FormationController::sameTeam(this->getControllableEntity(), entity, this->getGametype()))
                continue;
            tempScore = turret->isInRange(entity);
            if(tempScore != -1.f)
            {
                if(tempScore < minScore)
                {
                    minScore = tempScore;
                    minScorePawn = entity;
                }
            }
        }
        this->setTarget(minScorePawn);
        turret->setTarget(minScorePawn);
    }

    /**
        @brief 
        Tests, if the turret is looking at the target, with a specified tolerance

        This uses the world position as opposed to the local position in the old version.

        @param angle
        The tolerance, in radians
    */
    bool TurretController::isLookingAtTargetNew(float angle) const
    {
        return (getAngle(this->getControllableEntity()->getWorldPosition(), this->getControllableEntity()->getWorldOrientation() * WorldEntity::FRONT, this->target_->getWorldPosition()) < angle);
    }

    /**
        @brief 
        Scores a pawn as a target, based on distance and health.

        The more health and distance a pawn has, the higher the score. This means lower equals better.

        @param pawn
        The pawn to score

        @param distance
        The distance. Can be squared or normed, doesn't matter as long as all are treated the same.
    */   
    float TurretController::targetScore(Pawn* pawn, float distance) const
    {
        return pawn->getHealth()/pawn->getMaxHealth() + distance;
    }

    /**
        @brief
        Does all the controlling of the turret.

        If the turret has a parent, copies the team from there, if it's not already set.
        Other actions are: Search a target. If a target has been found, aim and shoot at it.
    */
    void TurretController::tick(float dt)
    {
        if (!this->isActive() || !this->getControllableEntity())
            return;


        ControllableEntity* parent = orxonox_cast<ControllableEntity*> (this->getControllableEntity()->getParent());
        if(this->getTeam() != -1 && !this->once_ && parent)
        {
            orxout(internal_warning) << "TurretController: Team already set, may result in undesired behaviour. Will get overridden by the parent's team." << endl;
        }

        if(!this->once_)
            this->once_ = true;
     
        //Teams aren't set immediately, after creation, so we have to check every tick...
        if(parent)
        {
            Controller* parentcontroller = parent->getController();
            if(parentcontroller)
            {
                this->setTeam(parentcontroller->getTeam());
            }
            else
            {
                this->setTeam(parent->getTeam());
            }
            this->getControllableEntity()->setTeam(parent->getTeam());
        }

        this->searchTarget();
        if(this->target_)
        {
            Turret* turret = orxonox_cast<Turret*> (this->getControllableEntity());
            this->aimAtTarget();
            turret->aimAtPosition(target_->getWorldPosition());
            if(this->isLookingAtTargetNew(Degree(5).valueRadians()))
            {
                this->getControllableEntity()->fire(0);
            }
        }
    }
 }

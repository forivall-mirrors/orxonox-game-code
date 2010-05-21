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
#include "core/XMLPort.h"
#include "worldentities/ControllableEntity.h"
#include "worldentities/pawns/Pawn.h"
#include "worldentities/pawns/TeamBaseMatchBase.h"
#include "gametypes/TeamDeathmatch.h"
#include "controllers/WaypointPatrolController.h"
#include "controllers/DroneController.h"
#include "util/Math.h"
#include "core/ConsoleCommand.h"

namespace orxonox
{
    SetConsoleCommand(ArtificialController, formationflight, true);//.defaultValues(0, true);
    SetConsoleCommand(ArtificialController, masteraction, true);

    static const unsigned int MAX_FORMATION_SIZE = 7;
    static const int FORMATION_LENGTH =  10;
    static const int FORMATION_WIDTH =  110;
    static const int FREEDOM_COUNT = 4; //seconds the slaves in a formation will be set free when master attacks an enemy
    static const float SPEED_MASTER = 0.6f;
    static const float ROTATEFACTOR_MASTER = 0.2f;
    static const float SPEED_FREE = 0.8f;
    static const float ROTATEFACTOR_FREE = 0.8f;

    ArtificialController::ArtificialController(BaseObject* creator) : Controller(creator)
    {
        RegisterObject(ArtificialController);

        this->target_ = 0;
        this->formationFlight_ = true;
        this->myMaster_ = 0;
        this->freedomCount_ = 0;
	this->team_ = -1;
	this->state_ = FREE;
        this->specificMasterAction_ = NONE;
        this->specificMasterActionHoldCount_  = 0;
        this->bShooting_ = false;
        this->bHasTargetPosition_ = false;
        this->targetPosition_ = Vector3::ZERO;

        this->target_.setCallback(createFunctor(&ArtificialController::targetDied, this));
    }

    ArtificialController::~ArtificialController()
    {
    }

    void ArtificialController::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(ArtificialController, XMLPort, xmlelement, mode);

        XMLPortParam(ArtificialController, "team", setTeam, getTeam, xmlelement, mode).defaultValues(-1);
        XMLPortParam(ArtificialController, "formation", setFormationFlight, getFormationFlight, xmlelement, mode).defaultValues(true);
    }

    //activate/deactivate formationflight
    void ArtificialController::formationflight(bool form)
    {
        for (ObjectList<Pawn>::iterator it = ObjectList<Pawn>::begin(); it; ++it)
        {
            if (!it->getController())
                continue;

            ArtificialController *aiController = static_cast<ArtificialController*>(it->getController());

            if(aiController)
            {
                aiController->formationFlight_ = form;
            }
        }
    }
    //get all masters to do this action
    void ArtificialController::masteraction(int action) 
    {
        for (ObjectList<Pawn>::iterator it = ObjectList<Pawn>::begin(); it; ++it)
        {
            if (!it->getController())
                continue;

            ArtificialController *aiController = static_cast<ArtificialController*>(it->getController());

            if(aiController || aiController->state_ == MASTER)
            {
                aiController->specificMasterAction_ = TURN180;
            }
        }
    }

// gets called when Bot dies
    void ArtificialController::changedControllableEntity()
    {
        if(!getControllableEntity()) 
        {
        if (this->state_ == SLAVE) unregisterSlave();
         if (this->state_ == MASTER) setNewMasterWithinFormation();
        this->slaves_.clear();
        this->state_ = FREE;

        }
    }

    void ArtificialController::moveToPosition(const Vector3& target)
    {
        if (!this->getControllableEntity())
            return;

        Vector2 coord = get2DViewdirection(this->getControllableEntity()->getPosition(), this->getControllableEntity()->getOrientation() * WorldEntity::FRONT, this->getControllableEntity()->getOrientation() * WorldEntity::UP, target);
        float distance = (target - this->getControllableEntity()->getPosition()).length();


        if(this->state_ == FREE)
        {
            if (this->target_ || distance > 10)
            {
                // Multiply with 0.8 to make them a bit slower
                this->getControllableEntity()->rotateYaw(-1.0f * ROTATEFACTOR_FREE * sgn(coord.x) * coord.x*coord.x);
                this->getControllableEntity()->rotatePitch(ROTATEFACTOR_FREE * sgn(coord.y) * coord.y*coord.y);
            }

            if (this->target_ && distance < 200 && this->getControllableEntity()->getVelocity().squaredLength() > this->target_->getVelocity().squaredLength())
            {
              this->getControllableEntity()->moveFrontBack(-0.05f); // They don't brake with full power to give the player a chance
            } else this->getControllableEntity()->moveFrontBack(SPEED_FREE);
        }



        if(this->state_ == MASTER)
        {
            if (this->target_ || distance > 10)
            {
                if (this->specificMasterAction_ == NONE)
                {
                this->getControllableEntity()->rotateYaw(-1.0f * ROTATEFACTOR_MASTER * sgn(coord.x) * coord.x*coord.x);
                this->getControllableEntity()->rotatePitch(ROTATEFACTOR_MASTER * sgn(coord.y) * coord.y*coord.y);
                } else if (this->specificMasterAction_ == TURN180)
                {
                this->getControllableEntity()->rotateYaw(-1.0f * sgn(coord.x) * coord.x*coord.x);
                this->getControllableEntity()->rotatePitch(sgn(coord.y) * coord.y*coord.y);
                }

            }

            if (this->target_ && distance < 200 && this->getControllableEntity()->getVelocity().squaredLength() > this->target_->getVelocity().squaredLength())
            {
                this->getControllableEntity()->moveFrontBack(-0.05f);
            } else this->getControllableEntity()->moveFrontBack(SPEED_MASTER);
        }



        if(this->state_ == SLAVE)
        {

           this->getControllableEntity()->rotateYaw(-2.0f * ROTATEFACTOR_MASTER * sgn(coord.x) * coord.x*coord.x);
           this->getControllableEntity()->rotatePitch(2.0f * ROTATEFACTOR_MASTER * sgn(coord.y) * coord.y*coord.y);

            if (distance < 300)
            {
                if (distance < 40)
                {
                    this->getControllableEntity()->moveFrontBack(0.8f*SPEED_MASTER);
                } else this->getControllableEntity()->moveFrontBack(1.2f*SPEED_MASTER);

            } else {
                this->getControllableEntity()->moveFrontBack(1.2f*SPEED_MASTER + distance/300.0f);
            }
        }
    }

    void ArtificialController::moveToTargetPosition()
    {
        this->moveToPosition(this->targetPosition_);
    }

    int ArtificialController::getState()
    {
        return this->state_;
    }

    void ArtificialController::unregisterSlave() {
        if(myMaster_)
        {
            std::vector<ArtificialController*>::iterator it = std::find(myMaster_->slaves_.begin(), myMaster_->slaves_.end(), this);
            if( it != myMaster_->slaves_.end() )
                myMaster_->slaves_.erase(it);
        }
    }

    void ArtificialController::searchNewMaster()
    {

        if (!this->getControllableEntity())
            return;

        this->targetPosition_ = this->getControllableEntity()->getPosition();
        this->forgetTarget();
        int teamSize = 0;
        //go through all pawns
        for (ObjectList<Pawn>::iterator it = ObjectList<Pawn>::begin(); it; ++it)
        {

            //same team?
            if (!ArtificialController::sameTeam(this->getControllableEntity(), static_cast<ControllableEntity*>(*it), this->getGametype()))
                continue;

            //has it an ArtificialController?
            if (!it->getController())
                continue;

            //is pawn oneself?
            if (static_cast<ControllableEntity*>(*it) == this->getControllableEntity())
                continue;

            teamSize++;

            ArtificialController *newMaster = static_cast<ArtificialController*>(it->getController());

            //is it a master?
            if (!newMaster || newMaster->getState() != MASTER)
                continue;

            float distance = (it->getPosition() - this->getControllableEntity()->getPosition()).length();

            // is pawn in range?
            if (distance < 5000)
            {
                if(newMaster->slaves_.size() > MAX_FORMATION_SIZE) continue;

                for(std::vector<ArtificialController*>::iterator itSlave = this->slaves_.begin(); itSlave != this->slaves_.end(); itSlave++)
                {
                    (*itSlave)->myMaster_ = newMaster;
                    newMaster->slaves_.push_back(*itSlave);
                }
                this->slaves_.clear();
                this->state_ = SLAVE;

                this->myMaster_ = newMaster;
                newMaster->slaves_.push_back(this);

                break;
            }
        }//for
        //hasn't encountered any masters in range? -> become a master
        if (state_ != SLAVE  && teamSize != 0) state_ = MASTER;//master encounters master? ->done

    }

    void ArtificialController::commandSlaves() {

        Quaternion orient = this->getControllableEntity()->getOrientation();
        Vector3 dest = this->getControllableEntity()->getPosition();

        // 1 slave: follow
        if (this->slaves_.size() == 1)
        {
            dest += 4*orient*WorldEntity::BACK;
            this->slaves_.front()->setTargetPosition(dest);
        }
        else 
        {
            dest += 1.0f*orient*WorldEntity::BACK;
            Vector3 pos = Vector3::ZERO;
            int i = 1;

            for(std::vector<ArtificialController*>::iterator it = slaves_.begin(); it != slaves_.end(); it++)
            {
                pos = Vector3::ZERO;
                if (i <= 1) pos += dest  + FORMATION_WIDTH*WorldEntity::LEFT;
                if (i == 2) pos += dest  + FORMATION_WIDTH*WorldEntity::RIGHT;
                if (i == 3) pos += dest  + FORMATION_WIDTH*WorldEntity::UP;
                if (i >= 4)
                {
                    pos += dest  + FORMATION_WIDTH*WorldEntity::DOWN;
                    i = 1;
                    dest += FORMATION_LENGTH*orient*WorldEntity::BACK;
                    (*it)->setTargetPosition(pos);
                    continue;
                }
                i++;
                (*it)->setTargetPosition(pos);
            }
        }
    }

    // binds slaves to new Master within formation
    void ArtificialController::setNewMasterWithinFormation()
    {

        if (this->slaves_.empty())
            return;

        ArtificialController *newMaster = this->slaves_.back();
        this->slaves_.pop_back();

        if(!newMaster) return;
        newMaster->state_ = MASTER;
        newMaster->slaves_ = this->slaves_;

        this->slaves_.clear();
        this->state_ = SLAVE;
        this->myMaster_ = newMaster;

        for(std::vector<ArtificialController*>::iterator it = newMaster->slaves_.begin(); it != newMaster->slaves_.end(); it++)
        {
            (*it)->myMaster_ = newMaster;
        }

    }

    void ArtificialController::freeSlaves()
    {
        for(std::vector<ArtificialController*>::iterator it = slaves_.begin(); it != slaves_.end(); it++)
        {
            (*it)->state_ = FREE;
        }
        this->slaves_.clear();
    }

    void ArtificialController::forceFreeSlaves()
    {
        for(std::vector<ArtificialController*>::iterator it = slaves_.begin(); it != slaves_.end(); it++)
        {
            (*it)->state_ = FREE;
            (*it)->forceFreedom();
            (*it)->targetPosition_ = this->targetPosition_;
            (*it)->bShooting_ = true;
            (*it)->getControllableEntity()->fire(0);// fire once for fun
        }
    }

    void ArtificialController::loseMasterState()
    {
        this->freeSlaves();
        this->state_ = FREE;
    }

    void ArtificialController::forceFreedom()
    {
        this->freedomCount_ = FREEDOM_COUNT;
    }

    bool ArtificialController::forcedFree()
    {
        if(this->freedomCount_ > 0) 
        {
            this->freedomCount_--;
            return true;
        } else return false;
    }

    void ArtificialController::specificMasterActionHold()
    {
        if (specificMasterActionHoldCount_ == 0) 
         {
            this->specificMasterAction_ = NONE;
            this->searchNewTarget();
            COUT(0) << "~action end" << std::endl;
         }
        else specificMasterActionHoldCount_--;
    }

    void ArtificialController::turn180()
    {
        COUT(0) << "~turn" << std::endl;

        Quaternion orient = this->getControllableEntity()->getOrientation();

        this->setTargetPosition(this->getControllableEntity()->getPosition() + 500.0f*orient*WorldEntity::BACK);
        this->specificMasterActionHoldCount_ = 2;
        this->specificMasterAction_  =  HOLD;
    }

    void ArtificialController::spin()
    {
        this->specificMasterAction_  =  NONE;
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

        Controller* controller = 0;
        if (entity1->getController())
            controller = entity1->getController();
        else
            controller = entity1->getXMLController();
        if (controller)
        {
            ArtificialController* ac = orxonox_cast<ArtificialController*>(controller);
            if (ac)
                team1 = ac->getTeam();
        }

        if (entity1->getController())
            controller = entity1->getController();
        else
            controller = entity1->getXMLController();
        if (controller)
        {
            ArtificialController* ac = orxonox_cast<ArtificialController*>(controller);
            if (ac)
                team2 = ac->getTeam();
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

        DroneController* droneController = 0;
        droneController = orxonox_cast<DroneController*>(entity1->getController());
        if (droneController && static_cast<ControllableEntity*>(droneController->getOwner()) == entity2)
            return true;
        droneController = orxonox_cast<DroneController*>(entity2->getController());
        if (droneController && static_cast<ControllableEntity*>(droneController->getOwner()) == entity1)
            return true;

        return (team1 == team2 && team1 != -1);
    }
}

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
 *      Dominik Solenicki
 *
 */

#include "ArtificialController.h"

#include <vector>
#include <climits>

#include "util/Math.h"
#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "core/command/ConsoleCommand.h"
#include "worldentities/ControllableEntity.h"
#include "worldentities/pawns/Pawn.h"
#include "worldentities/pawns/TeamBaseMatchBase.h"
#include "gametypes/TeamDeathmatch.h"
#include "gametypes/Dynamicmatch.h"
#include "controllers/WaypointPatrolController.h"
#include "controllers/NewHumanController.h"
#include "controllers/DroneController.h"

namespace orxonox
{
    SetConsoleCommand("ArtificialController", "formationflight",  &ArtificialController::formationflight);
    SetConsoleCommand("ArtificialController", "masteraction",     &ArtificialController::masteraction);
    SetConsoleCommand("ArtificialController", "followme",         &ArtificialController::followme);
    SetConsoleCommand("ArtificialController", "passivebehaviour", &ArtificialController::passivebehaviour);
    SetConsoleCommand("ArtificialController", "formationsize",    &ArtificialController::formationsize);

    static const unsigned int STANDARD_MAX_FORMATION_SIZE = 7;
    static const int RADIUS_TO_SEARCH_FOR_MASTERS = 5000;
    static const int FORMATION_LENGTH =  130;
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
        this->formationFlight_ = false;
        this->passive_ = false;
        this->maxFormationSize_ = STANDARD_MAX_FORMATION_SIZE;
        this->myMaster_ = 0;
        this->freedomCount_ = 0;
        this->team_ = -1;
        this->state_ = FREE;
        this->specificMasterAction_ = NONE;
        this->specificMasterActionHoldCount_  = 0;
        this->bShooting_ = false;
        this->bHasTargetPosition_ = false;
	this->bHasTargetOrientation_=false;
        this->speedCounter_ = 0.2f;
        this->targetPosition_ = Vector3::ZERO;

        this->target_.setCallback(createFunctor(&ArtificialController::targetDied, this));
    }

    ArtificialController::~ArtificialController()
    {
        if (this->isInitialized())
        {
            this->removeFromFormation();

            for (ObjectList<ArtificialController>::iterator it = ObjectList<ArtificialController>::begin(); it; ++it)
            {
                if (*it != this)
                {
                    if (it->myMaster_ == this)
                    {
                        orxout(internal_error) << this << " is still master in " << (*it) << endl;
                        it->myMaster_ = 0;
                    }

                    while (true)
                    {
                        std::vector<ArtificialController*>::iterator it2 = std::find(it->slaves_.begin(), it->slaves_.end(), this);
                        if (it2 != it->slaves_.end())
                        {
                            orxout(internal_error) << this << " is still slave in " << (*it) << endl;
                            it->slaves_.erase(it2);
                        }
                        else
                            break;
                    }
                }
            }
        }
    }

    void ArtificialController::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(ArtificialController, XMLPort, xmlelement, mode);

        XMLPortParam(ArtificialController, "team", setTeam, getTeam, xmlelement, mode).defaultValues(-1);
        XMLPortParam(ArtificialController, "formationFlight", setFormationFlight, getFormationFlight, xmlelement, mode).defaultValues(false);
        XMLPortParam(ArtificialController, "formationSize", setFormationSize, getFormationSize, xmlelement, mode).defaultValues(STANDARD_MAX_FORMATION_SIZE);
        XMLPortParam(ArtificialController, "passive", setPassive, getPassive, xmlelement, mode).defaultValues(false);
    }

// Documentation only here to get a faster overview for creating a useful documentation...

    /**
        @brief Activates / deactivates formationflight behaviour
        @param form activate formflight if form is true
    */
    void ArtificialController::formationflight(const bool form)
    {
        for (ObjectList<Pawn>::iterator it = ObjectList<Pawn>::begin(); it; ++it)
        {
            Controller* controller = 0;

            if (it->getController())
                controller = it->getController();
            else if (it->getXMLController())
                controller = it->getXMLController();

            if (!controller)
                continue;

            ArtificialController *aiController = orxonox_cast<ArtificialController*>(controller);

            if (aiController)
            {
                aiController->formationFlight_ = form;
                if (!form)
                {
                    aiController->removeFromFormation();
                }
            }
        }
    }

    /**
        @brief Get all masters to do a "specific master action"
        @param action which action to perform (integer, so it can be called with a console command (tmp solution))
    */
    void ArtificialController::masteraction(const int action)
    {
        for (ObjectList<Pawn>::iterator it = ObjectList<Pawn>::begin(); it; ++it)
        {
            Controller* controller = 0;

            if (it->getController())
                controller = it->getController();
            else if (it->getXMLController())
                controller = it->getXMLController();

            if (!controller)
                continue;

            ArtificialController *aiController = orxonox_cast<ArtificialController*>(controller);

            if(aiController && aiController->state_ == MASTER)
            {
                if (action == 1)
                    aiController->spinInit();
                if (action == 2)
                    aiController->turn180Init();
            }
        }
    }

    /**
        @brief A human player gets followed by its nearest master. Initiated by console command, so far intended for demonstration puproses (possible future pickup).
    */
    void ArtificialController::followme()
    {

        Pawn *humanPawn = NULL;
        NewHumanController *currentHumanController = NULL;
        std::vector<ArtificialController*> allMasters;

        for (ObjectList<Pawn>::iterator it = ObjectList<Pawn>::begin(); it; ++it)
        {
            Controller* controller = 0;

            if (it->getController())
                controller = it->getController();
            else if (it->getXMLController())
                controller = it->getXMLController();

            if (!controller)
                continue;

            currentHumanController = orxonox_cast<NewHumanController*>(controller);

            if(currentHumanController) humanPawn = *it;

            ArtificialController *aiController = orxonox_cast<ArtificialController*>(controller);

            if(aiController && aiController->state_ == MASTER)
                allMasters.push_back(aiController);

        }

        if((humanPawn != NULL) && (allMasters.size() != 0))
        {
                float posHuman = humanPawn->getPosition().length();
                float distance = 0.0f;
                float minDistance = FLT_MAX;
                int index = 0;
                int i = 0;

                for(std::vector<ArtificialController*>::iterator it = allMasters.begin(); it != allMasters.end(); it++, i++)
                    {
                        if (!ArtificialController::sameTeam((*it)->getControllableEntity(), humanPawn, (*it)->getGametype())) continue;
                        distance = posHuman - (*it)->getControllableEntity()->getPosition().length();
                        if(distance < minDistance) index = i;
                    }
                allMasters[index]->followInit(humanPawn);
            }

    }

    /**
        @brief Sets shooting behaviour of pawns.
        @param passive if true, bots won't shoot.
    */
    void ArtificialController::passivebehaviour(const bool passive)
    {
        for (ObjectList<Pawn>::iterator it = ObjectList<Pawn>::begin(); it; ++it)
        {
            Controller* controller = 0;

            if (it->getController())
                controller = it->getController();
            else if (it->getXMLController())
                controller = it->getXMLController();

            if (!controller)
                continue;

            ArtificialController *aiController = orxonox_cast<ArtificialController*>(controller);

            if(aiController)
            {
                aiController->passive_ = passive;
            }
        }
    }


    /**
        @brief Sets maximal formation size
        @param size maximal formation size.
    */
    void ArtificialController::formationsize(const int size)
    {
        for (ObjectList<Pawn>::iterator it = ObjectList<Pawn>::begin(); it; ++it)
        {
            Controller* controller = 0;

            if (it->getController())
                controller = it->getController();
            else if (it->getXMLController())
                controller = it->getXMLController();

            if (!controller)
                continue;

            ArtificialController *aiController = orxonox_cast<ArtificialController*>(controller);

            if(aiController)
            {
                aiController->maxFormationSize_ = size;
            }
        }
    }

    /**
        @brief Gets called when ControllableEntity is being changed. Resets the bot when it dies.
    */
    void ArtificialController::changedControllableEntity()
    {
        if (!this->getControllableEntity())
            this->removeFromFormation();
    }

    void ArtificialController::removeFromFormation()
    {
        if (this->state_ == SLAVE || this->myMaster_) // slaves can also be temporary free, so check if myMaster_ is set
            this->unregisterSlave();
        else if (this->state_ == MASTER)
            this->setNewMasterWithinFormation();
    }

    void ArtificialController::moveToPosition(const Vector3& target)
    {
        if (!this->getControllableEntity())
            return;

        // Slave uses special movement if its master is in FOLLOW mode
        if(this->state_ == SLAVE && this->myMaster_ && this->myMaster_->specificMasterAction_ == FOLLOW)
        {
//             this->followForSlaves(target);
//             return;
        }

        Vector2 coord = get2DViewdirection(this->getControllableEntity()->getPosition(), this->getControllableEntity()->getOrientation() * WorldEntity::FRONT, this->getControllableEntity()->getOrientation() * WorldEntity::UP, target);
        float distance = (target - this->getControllableEntity()->getPosition()).length();


        if(this->state_ == FREE)
        {
            if (this->target_ || distance > 10)
            {
                // Multiply with ROTATEFACTOR_FREE to make them a bit slower
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
                this->getControllableEntity()->rotateYaw(-1.0f * ROTATEFACTOR_MASTER * sgn(coord.x) * coord.x*coord.x);
                this->getControllableEntity()->rotatePitch(ROTATEFACTOR_MASTER * sgn(coord.y) * coord.y*coord.y);
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
		 if (bHasTargetOrientation_)
		    {
			copyTargetOrientation();
		    }
                if (distance < 40)
                {
                    this->getControllableEntity()->moveFrontBack(0.8f*SPEED_MASTER);
		   
                } else this->getControllableEntity()->moveFrontBack(1.2f*SPEED_MASTER);

            } else {
                this->getControllableEntity()->moveFrontBack(1.2f*SPEED_MASTER + distance/300.0f);
            }
        }

        if (distance < 10)
        {
            this->positionReached();
	    bHasTargetOrientation_=false;
        }
    }

    void ArtificialController::moveToTargetPosition()
    {
        this->moveToPosition(this->targetPosition_);
    }

    void ArtificialController::copyOrientation(const Quaternion& orient)
    {
	//roll angle in radian, difference between master and slave
	float diff=orient.getRoll().valueRadians()-(this->getControllableEntity()->getOrientation().getRoll().valueRadians());
	if ((diff<math::twoPi && diff>math::pi) || diff>(math::pi)*3)
	{
		diff=diff-math::twoPi;
	}
	this->getControllableEntity()->rotateRoll(1.0f*ROTATEFACTOR_MASTER*diff);
    }

    void ArtificialController::copyTargetOrientation()
    {
	if (bHasTargetOrientation_)
  	{
		copyOrientation(targetOrientation_);
	}
    }

    /**
        @brief Unregisters a slave from its master. Initiated by a slave.
    */
    void ArtificialController::unregisterSlave()
    {
        if (this->myMaster_)
        {
            std::vector<ArtificialController*>::iterator it = std::find(this->myMaster_->slaves_.begin(), this->myMaster_->slaves_.end(), this);
            if (it != this->myMaster_->slaves_.end())
                this->myMaster_->slaves_.erase(it);
        }

        this->myMaster_ = 0;
        this->state_ = FREE;
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
            Controller* controller = 0;

            if (it->getController())
                controller = it->getController();
            else if (it->getXMLController())
                controller = it->getXMLController();

            if (!controller)
                continue;

            //is pawn oneself?
            if (orxonox_cast<ControllableEntity*>(*it) == this->getControllableEntity())
                continue;

            teamSize++;

            ArtificialController *newMaster = orxonox_cast<ArtificialController*>(controller);

            //is it a master?
            if (!newMaster || newMaster->state_ != MASTER)
                continue;

            float distance = (it->getPosition() - this->getControllableEntity()->getPosition()).length();

            // is pawn in range?
            if (distance < RADIUS_TO_SEARCH_FOR_MASTERS)
            {
                if(newMaster->slaves_.size() > this->maxFormationSize_) continue;

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
        }

        if (this->state_ != SLAVE  && teamSize != 0)
        {
            this->state_ = MASTER;
            this->myMaster_ = 0;
        }
    }
 /**
        @brief Commands the slaves of a master into a formation. Sufficiently fast not to be called within tick. Initiated by a master.
    */

void ArtificialController::commandSlaves()
    {
        if(this->state_ != MASTER) return;

        Quaternion orient = this->getControllableEntity()->getOrientation();
        Vector3 dest = this->getControllableEntity()->getPosition();

        // 1 slave: follow
        if (this->slaves_.size() == 1)
        {
            dest += 4*orient*WorldEntity::BACK;
            this->slaves_.front()->setTargetPosition(dest);
        }
        else
	// formation:
        {
            dest += 1.0f*orient*WorldEntity::BACK;
            Vector3 pos = Vector3::ZERO;
	    bool left=true;
            int i = 1;
	    
            for(std::vector<ArtificialController*>::iterator it = slaves_.begin(); it != slaves_.end(); it++)
            {
                pos = Vector3::ZERO;
		if (left)
		{
                    pos+=dest+i*FORMATION_WIDTH*(orient*WorldEntity::LEFT);
		} else
		{
		    pos+=dest+i*FORMATION_WIDTH*(orient*WorldEntity::RIGHT);
		    i++;
		    dest+=FORMATION_LENGTH*(orient*WorldEntity::BACK);
		}		
		(*it)->setTargetOrientation(orient);
                (*it)->setTargetPosition(pos);
		left=!left;
            }
        }
    }

    /**
        @brief Sets a new master within the formation. Called by a master.
    */
    void ArtificialController::setNewMasterWithinFormation()
    {
        if(this->state_ != MASTER) return;

        if (!this->slaves_.empty())
        {
            ArtificialController *newMaster = this->slaves_.back();
            this->slaves_.pop_back();

            newMaster->state_ = MASTER;
            newMaster->slaves_ = this->slaves_;
            newMaster->myMaster_ = 0;

            for(std::vector<ArtificialController*>::iterator it = newMaster->slaves_.begin(); it != newMaster->slaves_.end(); it++)
            {
                (*it)->myMaster_ = newMaster;
            }
        }

        this->slaves_.clear();
        this->specificMasterAction_ = NONE;
        this->state_ = FREE;
    }

    /**
        @brief Frees all slaves form a master. Initiated by a master.
    */
    void ArtificialController::freeSlaves()
    {
        if(this->state_ != MASTER) return;

        for(std::vector<ArtificialController*>::iterator it = slaves_.begin(); it != slaves_.end(); it++)
        {
            (*it)->state_ = FREE;
            (*it)->myMaster_ = 0;
        }
        this->slaves_.clear();
    }

    /**
        @brief Master sets its slaves free for @ref FREEDOM_COUNT seconds.
    */
    void ArtificialController::forceFreeSlaves()
    {
        if(this->state_ != MASTER) return;

        for(std::vector<ArtificialController*>::iterator it = slaves_.begin(); it != slaves_.end(); it++)
        {
            (*it)->state_ = FREE;
            (*it)->forceFreedom();
            (*it)->targetPosition_ = this->targetPosition_;
            (*it)->bShooting_ = true;
//             (*it)->getControllableEntity()->fire(0);// fire once for fun
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

    /**
        @brief Checks wether caller has been forced free, decrements time to stay forced free.
        @return true if forced free.
    */
    bool ArtificialController::forcedFree()
    {
        if(this->freedomCount_ > 0)
        {
            this->freedomCount_--;
            return true;
        } else return false;
    }

    /**
        @brief Used to continue a "specific master action" for a certain time and resuming normal behaviour after.
    */
    void ArtificialController::specificMasterActionHold()
    {
        if(this->state_ != MASTER) return;

        if (specificMasterActionHoldCount_ == 0)
         {
            this->specificMasterAction_ = NONE;
            this->searchNewTarget();
         }
        else specificMasterActionHoldCount_--;
    }

    /**
        @brief Master initializes a 180 degree turn. Leads to a "specific master action".
    */
    void ArtificialController::turn180Init()
    {
        if(this->state_ != MASTER) return;

        Quaternion orient = this->getControllableEntity()->getOrientation();

        this->setTargetPosition(this->getControllableEntity()->getPosition() + 1000.0f*orient*WorldEntity::BACK);

        this->specificMasterActionHoldCount_ = 4;

        this->specificMasterAction_ = TURN180;
    }

    /**
        @brief Execute the 180 degree turn. Called within tick.
    */
    void ArtificialController::turn180()
    {
            Vector2 coord = get2DViewdirection(this->getControllableEntity()->getPosition(), this->getControllableEntity()->getOrientation() * WorldEntity::FRONT, this->getControllableEntity()->getOrientation() * WorldEntity::UP, this->targetPosition_);

            this->getControllableEntity()->rotateYaw(-2.0f * sgn(coord.x) * coord.x*coord.x);
            this->getControllableEntity()->rotatePitch(2.0f * sgn(coord.y) * coord.y*coord.y);

            this->getControllableEntity()->moveFrontBack(SPEED_MASTER);
    }

    /**
        @brief Master initializes a spin around its looking direction axis. Leads to a "specific master action".
    */
    void ArtificialController::spinInit()
    {
        if(this->state_ != MASTER) return;
        this->specificMasterAction_ = SPIN;
        this->specificMasterActionHoldCount_ = 10;
    }

    /**
        @brief Execute the spin. Called within tick.
    */
    void ArtificialController::spin()
    {
            this->moveToTargetPosition();
            this->getControllableEntity()->rotateRoll(0.8f);
    }

    /**
        @brief Master begins to follow a pawn. Is a "specific master action".
        @param pawn pawn to follow.
        @param always follows pawn forever if true (false if omitted).
        @param secondsToFollow seconds to follow the pawn if always is false. Will follow pawn 100 seconds if omitted (set in header).
    */
    void ArtificialController::followInit(Pawn* pawn, const bool always, const int secondsToFollow)
    {
        if (pawn == NULL || this->state_ != MASTER)
            return;
        this->specificMasterAction_  =  FOLLOW;

        this->setTarget(pawn);
        if (!always)
            this->specificMasterActionHoldCount_ = secondsToFollow;
        else
            this->specificMasterActionHoldCount_ = INT_MAX; //for now...

    }


    /**
        @brief Master begins to follow a randomly chosen human player of the same team. Is a "specific master action".
    */
    void ArtificialController::followRandomHumanInit()
    {

        Pawn *humanPawn = NULL;
        NewHumanController *currentHumanController = NULL;

        for (ObjectList<Pawn>::iterator it = ObjectList<Pawn>::begin(); it; ++it)
        {
            if (!it->getController())
                continue;

            currentHumanController = orxonox_cast<NewHumanController*>(it->getController());
            if(currentHumanController)
            {
                if (!ArtificialController::sameTeam(this->getControllableEntity(), *it, this->getGametype())) continue;
                humanPawn = *it;
                break;
            }
        }

        if((humanPawn != NULL))
                this->followInit(humanPawn);
    }

    /**
        @brief Master follows target with adjusted speed. Called within tick.
    */
    void ArtificialController::follow()
    {
        if (this->target_)
            this->moveToPosition(this->target_->getPosition());
        else
            this->specificMasterActionHoldCount_ = 0;
/*
        if (!this->getControllableEntity())
            return;

        float distance = (this->target_->getPosition() - this->getControllableEntity()->getPosition()).length();

        Vector2 coord = get2DViewdirection(this->getControllableEntity()->getPosition(), this->getControllableEntity()->getOrientation() * WorldEntity::FRONT, this->getControllableEntity()->getOrientation() * WorldEntity::UP, this->target_->getPosition());


        this->getControllableEntity()->rotateYaw(-0.8f * sgn(coord.x) * coord.x*coord.x);
        this->getControllableEntity()->rotatePitch(0.8f * sgn(coord.y) * coord.y*coord.y);

        float speedDiv = this->getControllableEntity()->getVelocity().squaredLength() - this->target_->getVelocity().squaredLength();

orxout() << "~follow distance: " << distance << "SpeedCounter: " << this->speedCounter_ << "~speedDiv: " << speedDiv << endl;
        if (distance < 800)
        {
            if (distance < 200)
            {
                this->speedCounter_ -= 0.5f;
                if(this->speedCounter_ < 0) this->speedCounter_ = 0.0f;
                this->getControllableEntity()->moveFrontBack(speedCounter_);
            } else {
                if(speedDiv < 0)
                    this->speedCounter_ +=  0.01f;
                else
                    this->speedCounter_ -= 0.05f;
                this->getControllableEntity()->moveFrontBack(speedCounter_);
            }

        } else {
            this->speedCounter_ += 0.05f;
            this->getControllableEntity()->moveFrontBack(speedCounter_ + distance/300.0f);
        }
//         if (this->getControllableEntity()->getVelocity().squaredLength() > 50.0f) this->speedCounter_ = 0;

*/
    }


    /**
        @brief Slave moving behaviour when master is following a pawn, gets redirected from moveToPosition(const Vector3& target)). Called within tick.
    */
    void ArtificialController::followForSlaves(const Vector3& target)
    {

/*
        if (!this->getControllableEntity() && !this->myMaster_ && this->myMaster_->state_ != FOLLOW && !this->myMaster_->target_)
            return;

        float distance = (target - this->getControllableEntity()->getPosition()).length();

        Vector2 coord = get2DViewdirection(this->getControllableEntity()->getPosition(), this->getControllableEntity()->getOrientation() * WorldEntity::FRONT, this->getControllableEntity()->getOrientation() * WorldEntity::UP, target);


        this->getControllableEntity()->rotateYaw(-0.8f * sgn(coord.x) * coord.x*coord.x);
        this->getControllableEntity()->rotatePitch(0.8f * sgn(coord.y) * coord.y*coord.y);


        float speedDiv = this->getControllableEntity()->getVelocity().squaredLength() - this->myMaster_->target_->getVelocity().squaredLength();


         if (distance < 800)
        {
            if (distance < 200)
            {
                this->speedCounter_ -= 5.0f;
                if(this->speedCounter_ < 0) this->speedCounter_ = 0.0f;
                this->getControllableEntity()->moveFrontBack(speedCounter_);
            } else {
                if(speedDiv < 0)
                    this->speedCounter_ +=  0.01f;
                else
                    this->speedCounter_ -= 0.05f;
                this->getControllableEntity()->moveFrontBack(speedCounter_);
            }

        } else {
            this->speedCounter_ += 0.05f;
            this->getControllableEntity()->moveFrontBack(speedCounter_ + distance/300.0f);
        }
//         if (this->getControllableEntity()->getVelocity().squaredLength() > 50.0f) this->speedCounter_ = 0;
*/
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

    void ArtificialController::setTargetOrientation(const Quaternion& orient)
    {
	this->targetOrientation_=orient;	
	this->bHasTargetOrientation_=true;
    }

    void ArtificialController::setTargetOrientation(Pawn* target)
    {
	if (target)
	    setTargetOrientation(target->getOrientation());
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

            /* So AI won't choose invisible Spaceships as target */
            if (!it->getRadarVisibility())
                continue;

            if (static_cast<ControllableEntity*>(*it) != this->getControllableEntity())
            {
                float speed = this->getControllableEntity()->getVelocity().length();
                Vector3 distanceCurrent = this->targetPosition_ - this->getControllableEntity()->getPosition();
                Vector3 distanceNew = it->getPosition() - this->getControllableEntity()->getPosition();
                if (!this->target_ || it->getPosition().squaredDistance(this->getControllableEntity()->getPosition()) * (1.5f + acos((this->getControllableEntity()->getOrientation() * WorldEntity::FRONT).dotProduct(distanceNew) / speed / distanceNew.length()) / math::twoPi)
                        < this->targetPosition_.squaredDistance(this->getControllableEntity()->getPosition()) * (1.5f + acos((this->getControllableEntity()->getOrientation() * WorldEntity::FRONT).dotProduct(distanceCurrent) / speed / distanceCurrent.length()) / math::twoPi) + rnd(-250, 250))
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

        Pawn* pawn = orxonox_cast<Pawn*>(this->getControllableEntity());
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

        if (entity2->getController())
            controller = entity2->getController();
        else
            controller = entity2->getXMLController();
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
        DroneController* droneController1 = orxonox_cast<DroneController*>(entity1->getController());
        DroneController* droneController2 = orxonox_cast<DroneController*>(entity2->getController());
        if (droneController1 && droneController2 && droneController1->getOwner() == droneController2->getOwner())
            return true;

        Dynamicmatch* dynamic = orxonox_cast<Dynamicmatch*>(gametype);
        if (dynamic)
        {
            if (dynamic->notEnoughPigs||dynamic->notEnoughKillers||dynamic->notEnoughChasers) {return false;}

            if (entity1->getPlayer())
                team1 = dynamic->getParty(entity1->getPlayer());

            if (entity2->getPlayer())
                team2 = dynamic->getParty(entity2->getPlayer());

            if (team1 ==-1 ||team2 ==-1 ) {return false;}
            else if (team1 == dynamic->chaser && team2 != dynamic->chaser) {return false;}
            else if (team1 == dynamic->piggy && team2 == dynamic->chaser) {return false;}
            else if (team1 == dynamic->killer && team2 == dynamic->chaser) {return false;}
            else return true;
        }

        return (team1 == team2 && team1 != -1);
    }
}

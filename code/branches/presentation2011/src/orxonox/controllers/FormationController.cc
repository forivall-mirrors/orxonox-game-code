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

#include <climits>
#include "controllers/FormationController.h"

#include "core/CoreIncludes.h"

#include "core/XMLPort.h"
#include "core/command/ConsoleCommand.h"

#include "worldentities/ControllableEntity.h"
#include "worldentities/pawns/Pawn.h"
#include "worldentities/pawns/TeamBaseMatchBase.h"
#include "gametypes/TeamDeathmatch.h"
#include "gametypes/Dynamicmatch.h"
#include "gametypes/Mission.h"
#include "gametypes/Gametype.h"
#include "controllers/WaypointPatrolController.h"
#include "controllers/NewHumanController.h"
#include "controllers/DroneController.h"


namespace orxonox
{

  SetConsoleCommand("FormationController", "formationflight",  &FormationController::formationflight);
  SetConsoleCommand("FormationController", "masteraction",     &FormationController::masteraction);
  SetConsoleCommand("FormationController", "followme",         &FormationController::followme);
  SetConsoleCommand("FormationController", "passivebehaviour", &FormationController::passivebehaviour);
  SetConsoleCommand("FormationController", "formationsize",    &FormationController::formationsize);




  static const unsigned int STANDARD_MAX_FORMATION_SIZE = 9;
  static const int RADIUS_TO_SEARCH_FOR_MASTERS = 5000;
  static const int FORMATION_LENGTH =  110;
  static const int FORMATION_WIDTH =  110;
  static const int FREEDOM_COUNT = 4; //seconds the slaves in a formation will be set free when master attacks an enemy
  static const float SPEED_MASTER = 0.6f;
  static const float ROTATEFACTOR_MASTER = 0.2f;
  static const float SPEED_FREE = 0.8f;
  static const float ROTATEFACTOR_FREE = 0.8f;

  FormationController::FormationController(BaseObject* creator) : Controller(creator)
  {
        RegisterObject(FormationController);

        this->target_ = 0;
        this->formationFlight_ = false;
        this->passive_ = false;
        this->maxFormationSize_ = STANDARD_MAX_FORMATION_SIZE;
        this->myMaster_ = 0;
        this->freedomCount_ = 0;

        this->state_ = FREE;
        this->formationMode_ = NORMAL;
        this->specificMasterAction_ = NONE;
        this->specificMasterActionHoldCount_  = 0;
        this->bShooting_ = false;
        this->bHasTargetPosition_ = false;
	this->bHasTargetOrientation_=false;
        this->speedCounter_ = 0.2f;
        this->targetPosition_ = Vector3::ZERO;
        this->team_=-1;
        this->target_.setCallback(createFunctor(&FormationController::targetDied, this));
  }

  FormationController::~FormationController()
  {
    if (this->isInitialized())
        {
            this->removeFromFormation();

            for (ObjectList<FormationController>::iterator it = ObjectList<FormationController>::begin(); it; ++it)
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
                        std::vector<FormationController*>::iterator it2 = std::find(it->slaves_.begin(), it->slaves_.end(), this);
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

  void FormationController::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(FormationController, XMLPort, xmlelement, mode);

        XMLPortParam(FormationController, "team", setTeam, getTeam, xmlelement, mode).defaultValues(-1);
        XMLPortParam(FormationController, "formationFlight", setFormationFlight, getFormationFlight, xmlelement, mode).defaultValues(false);
        XMLPortParam(FormationController, "formationSize", setFormationSize, getFormationSize, xmlelement, mode).defaultValues(STANDARD_MAX_FORMATION_SIZE);
        XMLPortParam(FormationController, "passive", setPassive, getPassive, xmlelement, mode).defaultValues(false);
    }



  /**
        @brief Activates / deactivates formationflight behaviour
        @param form activate formflight if form is true
    */
  void FormationController::formationflight(const bool form)
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

            FormationController *aiController = orxonox_cast<FormationController*>(controller);

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
    void FormationController::masteraction(const int action)
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

            FormationController *aiController = orxonox_cast<FormationController*>(controller);

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
        @brief Sets shooting behaviour of pawns.
        @param passive if true, bots won't shoot.
    */
    void FormationController::passivebehaviour(const bool passive)
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

            FormationController *aiController = orxonox_cast<FormationController*>(controller);

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
    void FormationController::formationsize(const int size)
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

            FormationController *aiController = orxonox_cast<FormationController*>(controller);

            if(aiController)
            {
                aiController->maxFormationSize_ = size;
            }
        }
    }

  void FormationController::removeFromFormation()
    {
        if (this->state_ == SLAVE || this->myMaster_) // slaves can also be temporary free, so check if myMaster_ is set
            this->unregisterSlave();
        else if (this->state_ == MASTER)
            this->setNewMasterWithinFormation();
    }

    void FormationController::moveToPosition(const Vector3& target)
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
                if (distance < 100)
                {   //linear speed reduction
                    this->getControllableEntity()->moveFrontBack(distance/100.0f*0.4f*SPEED_MASTER);
		   
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



  void FormationController::moveToTargetPosition()
    {
        this->moveToPosition(this->targetPosition_);
    }

  //copy the Roll orientation of given Quaternion.
  void FormationController::copyOrientation(const Quaternion& orient)
    {
        //roll angle difference in radian
        float diff=orient.getRoll(false).valueRadians()-(this->getControllableEntity()->getOrientation().getRoll(false).valueRadians());
        while(diff>math::twoPi) diff-=math::twoPi;
        while(diff<-math::twoPi) diff+=math::twoPi;
        this->getControllableEntity()->rotateRoll(diff*ROTATEFACTOR_MASTER);
    }

    void FormationController::copyTargetOrientation()
    {
        if (bHasTargetOrientation_)
        {
            copyOrientation(targetOrientation_);
        }
    }


   /**
        @brief Unregisters a slave from its master. Initiated by a slave.
    */
    void FormationController::unregisterSlave()
    {
        if (this->myMaster_)
        {
            std::vector<FormationController*>::iterator it = std::find(this->myMaster_->slaves_.begin(), this->myMaster_->slaves_.end(), this);
            if (it != this->myMaster_->slaves_.end())
                this->myMaster_->slaves_.erase(it);
        }

        this->myMaster_ = 0;
        this->state_ = FREE;
    }

    void FormationController::searchNewMaster()
    {
        if (this->state_==SLAVE) 
           return;
        if (!this->getControllableEntity())
            return;

        this->targetPosition_ = this->getControllableEntity()->getPosition();
        this->forgetTarget();
        int teamSize = 0;
        //go through all pawns
        for (ObjectList<Pawn>::iterator it = ObjectList<Pawn>::begin(); it; ++it)
        {
           
            //same team?
            Gametype* gt=this->getGametype();
            if (!gt)
            {
                gt=it->getGametype();
            }
            if (!FormationController::sameTeam(this->getControllableEntity(), static_cast<ControllableEntity*>(*it),gt))
                continue;

            //has it an FormationController?
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

            FormationController *newMaster = orxonox_cast<FormationController*>(controller);

            //is it a master?
            if (!newMaster || newMaster->state_ != MASTER)
                continue;

            float distance = (it->getPosition() - this->getControllableEntity()->getPosition()).length();

            // is pawn in range?
            if (distance < RADIUS_TO_SEARCH_FOR_MASTERS)
            {
                if(newMaster->slaves_.size() > this->maxFormationSize_) continue;

                for(std::vector<FormationController*>::iterator itSlave = this->slaves_.begin(); itSlave != this->slaves_.end(); itSlave++)
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

void FormationController::commandSlaves()
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
	    
            for(std::vector<FormationController*>::iterator it = slaves_.begin(); it != slaves_.end(); it++)
            {
                pos = Vector3::ZERO;
                if (left)
                {
                    pos+=dest+i*FORMATION_WIDTH*(orient*WorldEntity::LEFT);
                } else{
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
    void FormationController::setNewMasterWithinFormation()
    {
        if(this->state_ != MASTER) return;

        if (!this->slaves_.empty())
        {
            FormationController *newMaster = this->slaves_.back();
            this->slaves_.pop_back();

            newMaster->state_ = MASTER;
            newMaster->slaves_ = this->slaves_;
            newMaster->myMaster_ = 0;

            for(std::vector<FormationController*>::iterator it = newMaster->slaves_.begin(); it != newMaster->slaves_.end(); it++)
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
    void FormationController::freeSlaves()
    {
        if(this->state_ != MASTER) return;

        for(std::vector<FormationController*>::iterator it = slaves_.begin(); it != slaves_.end(); it++)
        {
            (*it)->state_ = FREE;
            (*it)->myMaster_ = 0;
        }
        this->slaves_.clear();
    }

    /**
        @brief Master sets its slaves free for @ref FREEDOM_COUNT seconds.
    */
    void FormationController::forceFreeSlaves()
    {
        if(this->state_ != MASTER) return;

        for(std::vector<FormationController*>::iterator it = slaves_.begin(); it != slaves_.end(); it++)
        {
            (*it)->state_ = FREE;
            (*it)->forceFreedom();
            (*it)->targetPosition_ = this->targetPosition_;
            (*it)->bShooting_ = true;
//             (*it)->getControllableEntity()->fire(0);// fire once for fun
        }
    }

    void FormationController::loseMasterState()
    {
        this->freeSlaves();
        this->state_ = FREE;
    }


    void FormationController::forceFreedom()
    {
        this->freedomCount_ = FREEDOM_COUNT;
    }

    /**
        @brief Checks wether caller has been forced free, decrements time to stay forced free.
        @return true if forced free.
    */
    bool FormationController::forcedFree()
    {
        if(this->freedomCount_ > 0)
        {
            this->freedomCount_--;
            return true;
        } else return false;
    }


    /**
        @brief Call to take the lead of formation (if free, become slave of nearest formation, then, if Slave, become Master)
    */
    void FormationController::takeLeadOfFormation()
    {
        if (!this->getControllableEntity() || this->state_==MASTER)
            return;

        //search new Master, then take lead
        if (this->state_==FREE && this->myMaster_==0)
        {
          searchNewMaster();
        }

        if (this->state_==SLAVE)  //become master of this formation
        {   
            this->slaves_=this->myMaster_->slaves_;
            this->myMaster_->slaves_.clear();
            this->myMaster_->state_=SLAVE;
            this->myMaster_->myMaster_=this;
            
            //delete myself in slavelist
            std::vector<FormationController*>::iterator it2 = std::find(this->slaves_.begin(), this->slaves_.end(), this);
            if (it2 != this->slaves_.end())
            {
                 this->slaves_.erase(it2);
            }
            //add previous master
            this->slaves_.push_back(this->myMaster_);
            //set this as new master
            for(std::vector<FormationController*>::iterator it = slaves_.begin(); it != slaves_.end(); it++)
            {
                 (*it)->myMaster_=this;
            }
            this->myMaster_=0;
            this->state_=MASTER;
        }
        /*/debug
        if (this->state_==SLAVE)
           {orxout(debug_output) << this << " is slave "<< endl;}
        else if (this->state_==MASTER)
           {orxout(debug_output) << this << " is now a master of "<<this->slaves_.size()<<" slaves."<< endl;}
        if (this->state_==FREE)
           {orxout(debug_output) << this << " is free "<< endl;}*/
    }
    /**
      @brief if called, half of the formation will attack the originator
    */
    void FormationController::masterAttacked(Pawn* originator)
    {
       if (this->state_!=MASTER) return;
       unsigned int i=0;
       for(std::vector<FormationController*>::reverse_iterator it = slaves_.rbegin(); it != slaves_.rend(); it++)
       {
           if ((*it)->state_!=FREE)
           {
               (*it)->state_=FREE;
               (*it)->forceFreedom();
               (*it)->target_=originator;
           }
           i++;
           if (i>=slaves_.size()/2) break; //half the formation should attack.
       }
    }     


    /**
      @brief Sets the new mode. If master, set it for all slaves.
    */
    void FormationController::setFormationMode(FormationMode val)
    {
        this->formationMode_ = val;
        if (this->state_ == MASTER)
        {
            for(std::vector<FormationController*>::iterator it = slaves_.begin(); it != slaves_.end(); it++)
            {
                 (*it)->formationMode_ = val;
                 if (val == ATTACK)
                     (*it)->forgetTarget();
            }
        }
    }

    /**
        @brief Used to continue a "specific master action" for a certain time and resuming normal behaviour after.
    */
    void FormationController::specificMasterActionHold()
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
    void FormationController::turn180Init()
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
    void FormationController::turn180()
    {
            Vector2 coord = get2DViewdirection(this->getControllableEntity()->getPosition(), this->getControllableEntity()->getOrientation() * WorldEntity::FRONT, this->getControllableEntity()->getOrientation() * WorldEntity::UP, this->targetPosition_);

            this->getControllableEntity()->rotateYaw(-2.0f * sgn(coord.x) * coord.x*coord.x);
            this->getControllableEntity()->rotatePitch(2.0f * sgn(coord.y) * coord.y*coord.y);

            this->getControllableEntity()->moveFrontBack(SPEED_MASTER);
    }

    /**
        @brief Master initializes a spin around its looking direction axis. Leads to a "specific master action".
    */
    void FormationController::spinInit()
    {
        if(this->state_ != MASTER) return;
        this->specificMasterAction_ = SPIN;
        this->specificMasterActionHoldCount_ = 10;
    }

    /**
        @brief Execute the spin. Called within tick.
    */
    void FormationController::spin()
    {
            this->moveToTargetPosition();
            this->getControllableEntity()->rotateRoll(0.8f);
    }

  /**
        @brief A human player gets followed by its nearest master. Initiated by console command, so far intended for demonstration puproses (possible future pickup).
    */
    void FormationController::followme()
    {

        Pawn *humanPawn = NULL;
        NewHumanController *currentHumanController = NULL;
        std::vector<FormationController*> allMasters;

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

            FormationController *aiController = orxonox_cast<FormationController*>(controller);

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

                for(std::vector<FormationController*>::iterator it = allMasters.begin(); it != allMasters.end(); it++, i++)
                    {
                        if (!FormationController::sameTeam((*it)->getControllableEntity(), humanPawn, (*it)->getGametype())) continue;
                        distance = posHuman - (*it)->getControllableEntity()->getPosition().length();
                        if(distance < minDistance) index = i;
                    }
                allMasters[index]->followInit(humanPawn);
            }

    }





    /**
        @brief Master begins to follow a pawn. Is a "specific master action".
        @param pawn pawn to follow.
        @param always follows pawn forever if true (false if omitted).
        @param secondsToFollow seconds to follow the pawn if always is false. Will follow pawn 100 seconds if omitted (set in header).
    */
    void FormationController::followInit(Pawn* pawn, const bool always, const int secondsToFollow)
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
    void FormationController::followRandomHumanInit()
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
                if (!FormationController::sameTeam(this->getControllableEntity(), *it, this->getGametype())) continue;
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
    void FormationController::follow()
    {
        if (this->target_)
            this->moveToPosition(this->target_->getPosition());
        else
            this->specificMasterActionHoldCount_ = 0;
    }


  void FormationController::setTargetPosition(const Vector3& target)
    {
        this->targetPosition_ = target;
        this->bHasTargetPosition_ = true;
    }

    void FormationController::searchRandomTargetPosition()
    {
        this->targetPosition_ = Vector3(rnd(-2000,2000), rnd(-2000,2000), rnd(-2000,2000));
        this->bHasTargetPosition_ = true;
    }

    void FormationController::setTargetOrientation(const Quaternion& orient)
    {
        this->targetOrientation_=orient;	
        this->bHasTargetOrientation_=true;
    }

    void FormationController::setTargetOrientation(Pawn* target)
    {
        if (target)
            setTargetOrientation(target->getOrientation());
    }

    void FormationController::setTarget(Pawn* target)
    {
        this->target_ = target;

        if (target)
            this->targetPosition_ = target->getPosition();
    }

    void FormationController::searchNewTarget()
    {
        if (!this->getControllableEntity())
            return;

        this->targetPosition_ = this->getControllableEntity()->getPosition();
        this->forgetTarget();

        for (ObjectList<Pawn>::iterator it = ObjectList<Pawn>::begin(); it; ++it)
        {
            if (FormationController::sameTeam(this->getControllableEntity(), static_cast<ControllableEntity*>(*it), this->getGametype()))
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

  void FormationController::forgetTarget()
    {
        this->target_ = 0;
        this->bShooting_ = false;
    }

   void FormationController::targetDied()
    {
        this->forgetTarget();
        this->searchRandomTargetPosition();
    }

  bool FormationController::sameTeam(ControllableEntity* entity1, ControllableEntity* entity2, Gametype* gametype)
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
            FormationController* ac = orxonox_cast<FormationController*>(controller);
            if (ac)
                team1 = ac->getTeam();
        }

        if (entity2->getController())
            controller = entity2->getController();
        else
            controller = entity2->getXMLController();
        if (controller)
        {
            FormationController* ac = orxonox_cast<FormationController*>(controller);
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

        Mission* miss = orxonox_cast<Mission*>(gametype);
        if (miss)
        {
            if (entity1->getPlayer())
                team1 = miss->getTeam(entity1->getPlayer());

            if (entity2->getPlayer())
                team2 = miss->getTeam(entity2->getPlayer());
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

    void FormationController::absoluteMoveToPosition(const Vector3& target)
    {
        float minDistance = 40.0f;
        if (!this->getControllableEntity())
            return;

        Vector2 coord = get2DViewdirection(this->getControllableEntity()->getPosition(), this->getControllableEntity()->getOrientation() * WorldEntity::FRONT, this->getControllableEntity()->getOrientation() * WorldEntity::UP, target);
        float distance = (target - this->getControllableEntity()->getPosition()).length();

            if (this->target_ || distance > minDistance)
            {
                // Multiply with ROTATEFACTOR_FREE to make them a bit slower
                this->getControllableEntity()->rotateYaw(-1.0f * ROTATEFACTOR_FREE * sgn(coord.x) * coord.x*coord.x);
                this->getControllableEntity()->rotatePitch(ROTATEFACTOR_FREE * sgn(coord.y) * coord.y*coord.y);
                this->getControllableEntity()->moveFrontBack(SPEED_FREE);
            }


        if (distance < minDistance)
        {
            this->positionReached();
        }
    }

}

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

#include "AIController.h"

#include "util/Math.h"
#include "core/CoreIncludes.h"
#include "core/command/Executor.h"
#include "worldentities/ControllableEntity.h"
#include "worldentities/pawns/Pawn.h"

namespace orxonox
{
    const float AIController::ACTION_INTERVAL = 1.0f;

    RegisterClass(AIController);

    AIController::AIController(Context* context) : ArtificialController(context)
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

        if (this->state_ == FREE)
        {
            
            if (this->formationFlight_)
            {

                //changed order -> searchNewMaster MUSTN'T be called in SLAVE-state (bugfix for internal-error messages at quit)
                random = rnd(maxrand);
                if (random < 90 && (((!this->target_) || (random < 50 && this->target_)) && !this->forcedFree()))
                       this->searchNewMaster();

                // return to Master after being forced free
                if (this->freedomCount_ == 1)
                {
                    this->state_ = SLAVE;
                    this->freedomCount_ = 0;
                }
            }

            this->defaultBehaviour(maxrand);

        }

        if (this->state_ == SLAVE && this->formationMode_ == ATTACK) 
        {
            // search enemy
            random = rnd(maxrand);
            if (random < (botlevel_*100) && (!this->target_))
                this->searchNewTarget();

            // next enemy
            random = rnd(maxrand);
            if (random < (botlevel_*30) && (this->target_))
                this->searchNewTarget();

            // shoot
            random = rnd(maxrand);
            if (!(this->passive_) && random < (botlevel_*100) && (this->target_ && !this->bShooting_))
                this->bShooting_ = true;

            // stop shooting
            random = rnd(maxrand);
            if (random < (1-botlevel_)*50 && (this->bShooting_))
                this->bShooting_ = false;

        }

        if (this->state_ == MASTER)
        {
            this->commandSlaves();

            if  (this->specificMasterAction_ != NONE)
                    this->specificMasterActionHold();

            else {

                 // make 180 degree turn - a specific Master Action
                random = rnd(1000.0f);
                if (random < 5)
                   this->turn180Init();

                // spin around - a specific Master Action
                random = rnd(1000.0f);
                if (random < 5)
                   this->spinInit();

                /*// follow a randomly chosen human - a specific Master Action
                random = rnd(1000.0f);
                if (random < 1)
                   this->followRandomHumanInit();
*/
                 // lose master status (only if less than 4 slaves in formation)
                random = rnd(maxrand);
                if(random < 15/(this->slaves_.size()+1) && this->slaves_.size() < 4 )
                   this->loseMasterState();

                // look out for outher masters if formation is small
                random = rnd(maxrand);
                if(this->slaves_.size() < 3 && random < 20)
                    this->searchNewMaster();

                this->defaultBehaviour(maxrand);

            }
        }

    }

    void AIController::tick(float dt)
    {
        if (!this->isActive())
            return;

        float random;
        float maxrand = 100.0f / ACTION_INTERVAL;
        ControllableEntity* controllable = this->getControllableEntity();
        //DOES: Either move to the waypoint or search for a Point of interest
        if (controllable && this->mode_ == DEFAULT)// bot is ready to move to a target
        {
            if (this->waypoints_.size() > 0 ) //Waypoint functionality.
            {
                WorldEntity* wPoint = this->waypoints_[this->waypoints_.size()-1];
                if(wPoint)
                {
                    this->moveToPosition(wPoint->getWorldPosition()); //BUG ?? sometime wPoint->getWorldPosition() causes crash
                    if (wPoint->getWorldPosition().squaredDistance(controllable->getPosition()) <= this->squaredaccuracy_)
                        this->waypoints_.pop_back(); // if goal is reached, remove it from the list
                }
                else
                    this->waypoints_.pop_back(); // remove invalid waypoints

            }
            else if(this->defaultWaypoint_ && ((this->defaultWaypoint_->getPosition()-controllable->getPosition()).length()  > 200.0f))
            {
                this->moveToPosition(this->defaultWaypoint_->getPosition()); // stay within a certain range of the defaultWaypoint_
                random = rnd(maxrand);
            }
        }

        if (this->mode_ == DEFAULT)
        {
            if (this->state_ == MASTER)
            {
                if (this->specificMasterAction_ ==  NONE)
                {
                    if (this->target_)
                    {
                        if (!this->target_->getRadarVisibility()) /* So AI won't shoot invisible Spaceships */
                            this->forgetTarget();
                        else
                        {
                            this->aimAtTarget();
                            random = rnd(maxrand);
                            if(this->botlevel_*70 > random && !this->isCloseAtTarget(100))
                                this->follow();  //If a bot is shooting a player, it shouldn't let him go away easily.
                        }
                    }

                    if (this->bHasTargetPosition_)
                        this->moveToTargetPosition();
                    this->doFire();
                }

                if (this->specificMasterAction_  == TURN180)
                    this->turn180();

                if (this->specificMasterAction_ == SPIN)
                    this->spin();
                if (this->specificMasterAction_ == FOLLOW)
                    this->follow();
            }

            if (this->state_ == SLAVE && this->formationMode_ != ATTACK)
            {
                if (this->bHasTargetPosition_)
                    this->moveToTargetPosition();
            }

            if (this->state_ == FREE || (this->state_==SLAVE && this->formationMode_ == ATTACK) )
            {
                if (this->target_)
                {
                    if (!this->target_->getRadarVisibility()) /* So AI won't shoot invisible Spaceships */
                        this->forgetTarget();
                    else this->aimAtTarget();
                }

                if (this->bHasTargetPosition_)
                    this->moveToTargetPosition();

                    this->doFire();
            }
        }
        else if (this->mode_ == ROCKET)//Rockets do not belong to a group of bots -> bot states are not relevant.
        {   //Vector-implementation: mode_.back() == ROCKET;
            if(controllable)
            {//Check wether the bot is controlling the rocket and if the timeout is over.
                if(controllable->getIdentifier() == ClassByString("Rocket"))

                {
                    this->follow();
                    this->timeout_ -= dt;
                    if((timeout_< 0)||(!target_))//Check if the timeout is over or target died.
                    {
                       controllable->fire(0);//kill the rocket
                       this->setPreviousMode();//get out of rocket mode
                    }
                }
                else
                    this->setPreviousMode();//no rocket entity -> get out of rocket mode
            }
            else
                this->setPreviousMode();//If bot dies -> getControllableEntity == NULL -> get out of ROCKET mode
        }//END_OF ROCKET MODE

        SUPER(AIController, tick, dt);
    }
//**********************************************NEW
    void AIController::defaultBehaviour(float maxrand)
    {       float random;
            // search enemy
            random = rnd(maxrand);
            if (random < (botlevel_* 100) && (!this->target_))
                this->searchNewTarget();

            // forget enemy
            random = rnd(maxrand);
            if (random < ((1-botlevel_)*20) && (this->target_))
                this->forgetTarget();

            // next enemy
            random = rnd(maxrand);
            if (random < (botlevel_*30) && (this->target_))
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

            if (this->state_ == MASTER) // master: shoot
            {
                random = rnd(maxrand);
                if (!(this->passive_) && random < (100*botlevel_) && (this->target_ && !this->bShooting_))
                {
                    this->bShooting_ = true;
                    this->forceFreeSlaves();
                }
            }
            else
            {
                // shoot
                random = rnd(maxrand);
                if (!(this->passive_) && random < (botlevel_*100) && (this->target_ && !this->bShooting_))
                    this->bShooting_ = true;
            }

            // stop shooting
            random = rnd(maxrand);
            if (random < ((1 - botlevel_)*50) && (this->bShooting_))
                this->bShooting_ = false;

            // boost
            random = rnd(maxrand);
            if (random < botlevel_*50 )
                this->boostControl();

            // update Checkpoints
            /*random = rnd(maxrand);
            if (this->defaultWaypoint_ && random > (maxrand-10))
                this->manageWaypoints();
            else //if(random > maxrand-10) //CHECK USABILITY!!*/
            if (this->waypoints_.size() == 0 )
                this->manageWaypoints();
    }

}

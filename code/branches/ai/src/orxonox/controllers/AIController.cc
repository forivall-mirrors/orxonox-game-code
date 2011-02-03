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

//Todo: Bot soll pickupspawner besuchen können, falls Pickup vorhanden
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

        if (this->state_ == FREE)
        {

            if (this->formationFlight_)
            {
                // return to Master after being forced free
                if (this->freedomCount_ == 1)
                {
                this->state_ = SLAVE;
                this->freedomCount_ = 0;
                }

                random = rnd(maxrand);
                if (random < 90 && (((!this->target_) || (random < 50 && this->target_)) && !this->forcedFree()))
                    this->searchNewMaster();
            }

            // search enemy
            random = rnd(maxrand);
            if (random < (15 + botlevel_* 20) && (!this->target_))
                this->searchNewTarget();

            // forget enemy
            random = rnd(maxrand);
            if (random < ((1-botlevel_)*5) && (this->target_))
                this->forgetTarget();

            // next enemy
            random = rnd(maxrand);
            if (random < ((1-botlevel_)*10) && (this->target_))
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
            if (!(this->passive_) && random < (75 + botlevel_*25) && (this->target_ && !this->bShooting_))
                this->bShooting_ = true;

            // stop shooting
            random = rnd(maxrand);
            if (random < ((1 - botlevel_)*25) && (this->bShooting_))
                this->bShooting_ = false;

        }

        if (this->state_ == SLAVE)
        {

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

                // follow a randomly chosen human - a specific Master Action
                random = rnd(1000.0f);
                if (random < 1)
                   this->followRandomHumanInit();

                 // lose master status (only if less than 4 slaves in formation)
                random = rnd(maxrand);
                if(random < 15/(this->slaves_.size()+1) && this->slaves_.size() < 4 )
                   this->loseMasterState();

                // look out for outher masters if formation is small
                random = rnd(maxrand);
                if(this->slaves_.size() < 3 && random < 20)
                    this->searchNewMaster();

                // search enemy
                random = rnd(maxrand);
                if (random < (botlevel_)*25 && (!this->target_))
                    this->searchNewTarget();

                // forget enemy
                random = rnd(maxrand);
                if (random < (1-botlevel_)*6 && (this->target_))
                    this->forgetTarget();

                // next enemy
                random = rnd(maxrand);
                if (random < 10 && (this->target_))
                    this->searchNewTarget();

                // fly somewhere
                random = rnd(maxrand);
                if (random < 50 && (!this->bHasTargetPosition_ && !this->target_))
                    this->searchRandomTargetPosition();

                // fly somewhere else
                random = rnd(maxrand);
                if (random < 30 && (this->bHasTargetPosition_ && !this->target_))
                    this->searchRandomTargetPosition();

                // shoot
                random = rnd(maxrand);
                if (!(this->passive_) && random < 25*(botlevel_)+1 && (this->target_ && !this->bShooting_))
                {
                    this->bShooting_ = true;
                    this->forceFreeSlaves();
                }

                // stop shooting
                random = rnd(maxrand);
                if (random < ( (1- botlevel_)*25 ) && (this->bShooting_))
                    this->bShooting_ = false;

            }
        }

    }

    void AIController::tick(float dt)
    {
        float random;
        float maxrand = 100.0f / ACTION_INTERVAL;
	
        if (!this->isActive())
            return;

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
		        if(this->botlevel_*100 > random)
                            this->follow();//If a bot is shooting a player, it shouldn't let him go away easily.
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

        if (this->state_ == SLAVE)
        {
            if (this->bHasTargetPosition_)
                this->moveToTargetPosition();
        }

        if (this->state_ == FREE)
        {
            if (this->target_)
            {
                if (!this->target_->getRadarVisibility()) /* So AI won't shoot invisible Spaceships */
                    this->forgetTarget();
                else
		{
		    this->aimAtTarget();
		    random = rnd(maxrand);
		    if(this->botlevel_*100 > random)
                        this->follow();//If a bot is shooting a player, it shouldn't let him go away easily.
		}
            }

            if (this->bHasTargetPosition_)
                this->moveToTargetPosition();

            this->doFire();
        }

        SUPER(AIController, tick, dt);
    }

}

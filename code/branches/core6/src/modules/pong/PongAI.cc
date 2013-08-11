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

/**
    @file PongAI.cc
    @brief Implementation of the PongAI class.
*/

#include "PongAI.h"

#include "core/CoreIncludes.h"
#include "core/config/ConfigValueIncludes.h"
#include "core/command/Executor.h"
#include "tools/Timer.h"

#include "worldentities/ControllableEntity.h"

#include "PongBall.h"

namespace orxonox
{
    RegisterUnloadableClass(PongAI);

    const static float MAX_REACTION_TIME = 0.4f;

    /**
    @brief
        Constructor. Registers and initializes the object.
    */
    PongAI::PongAI(Context* context) : Controller(context)
    {
        RegisterObject(PongAI);

        this->ball_ = 0;
        this->ballDirection_ = Vector2::ZERO;
        this->ballEndPosition_ = 0;
        this->randomOffset_ = 0;
        this->bChangedRandomOffset_ = false;
        this->relHysteresisOffset_ = 0.02f;
        this->strength_ = 0.5f;
        this->movement_ = 0;
        this->oldMove_ = 0;
        this->bOscillationAvoidanceActive_ = false;

        this->setConfigValues();
    }

    /**
    @brief
        Destructor. Cleans up the list fo reaction timers.
    */
    PongAI::~PongAI()
    {
        for (std::list<std::pair<Timer*, char> >::iterator it = this->reactionTimers_.begin(); it != this->reactionTimers_.end(); ++it)
            it->first->destroy();
    }

    /**
    @brief
        Sets config values.
    */
    void PongAI::setConfigValues()
    {
        // Sets the strength of the PongAi as a config value.
        SetConfigValue(strength_, 0.5).description("A value from 0 to 1 where 0 is weak and 1 is strong.");
    }

    /**
    @brief
        Is called each tick.
        Implements the behavior of the PongAI (i.e. its intelligence).
    @param dt
        The time that has elapsed since the last tick.
    */
    void PongAI::tick(float dt)
    {
        // If either the ball, or the controllable entity (i.e. the bat) don't exist (or aren't set).
        if (this->ball_  == NULL || this->getControllableEntity() == NULL)
            return;

        Vector3 mypos = this->getControllableEntity()->getPosition();
        Vector3 ballpos = this->ball_->getPosition();
        Vector3 ballvel = this->ball_->getVelocity();
        float hysteresisOffset = this->relHysteresisOffset_ * this->ball_->getFieldDimension().y;

        char move = 0;
        bool delay = false;

        // Check in which direction the ball is flying
        if ((mypos.x > 0 && ballvel.x < 0) || (mypos.x < 0 && ballvel.x > 0))
        {
            // The ball is flying away
            this->ballDirection_.x = -1;
            this->ballDirection_.y = 0;
            this->bOscillationAvoidanceActive_ = false;

            // Move to the middle
            if (mypos.z > hysteresisOffset)
                move = 1;
            else if (mypos.z < -hysteresisOffset)
                move = -1;
        }
        else if (ballvel.x == 0)
        {
            // The ball is standing still
            this->ballDirection_.x = 0;
            this->ballDirection_.y = 0;
            this->bOscillationAvoidanceActive_ = false;
        }
        else
        {
            // The ball is approaching
            if (this->ballDirection_.x != 1)
            {
                // The ball just started to approach, initialize all values
                this->ballDirection_.x = 1;
                this->ballDirection_.y = sgn(ballvel.z);
                this->ballEndPosition_ = 0;
                this->randomOffset_ = 0;
                this->bChangedRandomOffset_ = false;

                this->calculateRandomOffset();
                this->calculateBallEndPosition();
                delay = true;
                this->bOscillationAvoidanceActive_ = false;
            }

            if (this->ballDirection_.y != sgn(ballvel.z))
            {
                // The ball just bounced from a bound, recalculate the predicted end position
                this->ballDirection_.y = sgn(ballvel.z);

                this->calculateBallEndPosition();
                delay = true;
                this->bOscillationAvoidanceActive_ = false;
            }

            // If the ball is close enough, calculate another random offset to accelerate the ball
            if (!this->bChangedRandomOffset_)
            {
                float timetohit = (-ballpos.x + this->ball_->getFieldDimension().x / 2 * sgn(ballvel.x)) / ballvel.x;
                if (timetohit < 0.05)
                {
                    this->bChangedRandomOffset_ = true;
                    if (rnd() < this->strength_)
                        this->calculateRandomOffset();
                }
            }

            // Move to the predicted end position with an additional offset (to hit the ball with the side of the bat)
            if (!this->bOscillationAvoidanceActive_)
            {
                float desiredZValue = this->ballEndPosition_ + this->randomOffset_;

                if (mypos.z > desiredZValue + hysteresisOffset * (this->randomOffset_ < 0))
                    move = 1;
                else if (mypos.z < desiredZValue - hysteresisOffset * (this->randomOffset_ > 0))
                    move = -1;
            }

            if (move != 0 && this->oldMove_ != 0 && move != this->oldMove_ && !delay)
            {
                // We had to correct our position because we moved too far
                // (and delay is false, so we're not in the wrong place because of a new end-position prediction)
                if (fabs(mypos.z - this->ballEndPosition_) < 0.5 * this->ball_->getBatLength() * this->ball_->getFieldDimension().y)
                {
                    // We're not at the right position, but we still hit the ball, so just stay there to avoid oscillation
                    move = 0;
                    this->bOscillationAvoidanceActive_ = true;
                }
            }
        }

        this->oldMove_ = move;
        this->move(move, delay);
        this->getControllableEntity()->moveFrontBack(this->movement_);
    }

    /**
    @brief
        Calculates the random offset, that accounts for random errors the AI makes in order to be beatable.
        The higher the strength of the AI, the smaller the (expected value of the) error.
        The result of this method is stored in this->randomOffset_.
    */
    void PongAI::calculateRandomOffset()
    {
        // Calculate the exponent for the position-formula
        float exp = pow(10, 1 - 2*this->strength_); // strength: 0   -> exp = 10
                                                    // strength: 0.5 -> exp = 1
                                                    // strength: 1   -> exp = 0.1

        // Calculate the relative position where to hit the ball with the bat
        float position = pow(rnd(), exp); // exp > 1 -> position is more likely a small number
                                          // exp < 1 -> position is more likely a large number

        // The position shouldn't be larger than 0.5 (50% of the bat-length from the middle is the end)
        position *= 0.48f;

        // Both sides are equally probable
        position *= rndsgn();

        // Calculate the offset in world units
        this->randomOffset_ = position * this->ball_->getBatLength() * this->ball_->getFieldDimension().y;
    }

    /**
    @brief
        Calculate the end position the ball will be in.
        The result of this calculation is stored in this->ballEndPosition_.
    */
    void PongAI::calculateBallEndPosition()
    {
        Vector3 position = this->ball_->getPosition();
        Vector3 velocity = this->ball_->getVelocity();
        Vector3 acceleration = this->ball_->getAcceleration();
        Vector2 dimension = this->ball_->getFieldDimension();

        // Calculate bounces. The number of predicted bounces is limited by the AIs strength
        for (float limit = -0.05f; limit < this->strength_ || this->strength_ > 0.99f; limit += 0.4f)
        {
            // calculate the time until the ball reaches the other side
            float totaltime = (-position.x + dimension.x / 2 * sgn(velocity.x)) / velocity.x;

            // calculate wall bounce position (four possible solutions of the equation: pos.z + vel.z*t + acc.z/2*t^2 = +/- dim.z/2)
            float bouncetime = totaltime;
            bool bUpperWall = false;

            if (acceleration.z == 0)
            {
                if (velocity.z > 0)
                {
                    bUpperWall = true;
                    bouncetime = (dimension.y/2 - position.z) / velocity.z;
                }
                else if (velocity.z < 0)
                {
                    bUpperWall = false;
                    bouncetime = (-dimension.y/2 - position.z) / velocity.z;
                }
            }
            else
            {
                // upper wall
                float temp = velocity.z*velocity.z + 2*acceleration.z*(dimension.y/2 - position.z);
                if (temp >= 0)
                {
                    float t1 = (sqrt(temp) - velocity.z) / acceleration.z;
                    float t2 = (sqrt(temp) + velocity.z) / acceleration.z * (-1);
                    if (t1 > 0 && t1 < bouncetime)
                    {
                        bouncetime = t1;
                        bUpperWall = true;
                    }
                    if (t2 > 0 && t2 < bouncetime)
                    {
                        bouncetime = t2;
                        bUpperWall = true;
                    }
                }
                // lower wall
                temp = velocity.z*velocity.z - 2*acceleration.z*(dimension.y/2 + position.z);
                if (temp >= 0)
                {
                    float t1 = (sqrt(temp) - velocity.z) / acceleration.z;
                    float t2 = (sqrt(temp) + velocity.z) / acceleration.z * (-1);
                    if (t1 > 0 && t1 < bouncetime)
                    {
                        bouncetime = t1;
                        bUpperWall = false;
                    }
                    if (t2 > 0 && t2 < bouncetime)
                    {
                        bouncetime = t2;
                        bUpperWall = false;
                    }
                }
            }

            if (bouncetime < totaltime)
            {
                // Calculate a random prediction error, based on the vertical speed of the ball and the strength of the AI
                float randomErrorX = rnd(-1, 1) * dimension.y * (velocity.z / velocity.x / PongBall::MAX_REL_Z_VELOCITY) * (1 - this->strength_);
                float randomErrorZ = rnd(-1, 1) * dimension.y * (velocity.z / velocity.x / PongBall::MAX_REL_Z_VELOCITY) * (1 - this->strength_);

                // ball bounces after <bouncetime> seconds, update the position and continue
                velocity.z = velocity.z + acceleration.z * bouncetime;

                if (bUpperWall)
                {
                    position.z = dimension.y / 2;
                    velocity.z = -fabs(velocity.z) + fabs(randomErrorZ);
                }
                else
                {
                    position.z = -dimension.y / 2;
                    velocity.z = fabs(velocity.z) - fabs(randomErrorZ);
                }

                position.x = position.x + velocity.x * bouncetime + randomErrorX;
                this->ballEndPosition_ = position.z;
            }
            else
            {
                // ball doesn't bounce, calculate the end position and return
                // calculate end-height: current height + slope * distance incl. acceleration
                this->ballEndPosition_ = position.z + velocity.z * totaltime + acceleration.z / 2 * totaltime * totaltime;
                return;
            }
        }
    }

    /**
    @brief
        Determine the movement the AI will undertake. (Either -1, 0 or 1)
        The result of this calculation is stored in this->movement_;
    @param direction
        The current direction of movement.
    @param bUseDelay
        The time by which this move is delayed. (Again, to make the AI less efficient)
    */
    void PongAI::move(char direction, bool bUseDelay)
    {
        // The current direction is either what we're doing right now (movement_) or what is last in the queue
        char currentDirection = this->movement_;
        if (this->reactionTimers_.size() > 0)
            currentDirection = this->reactionTimers_.back().second;

        // Only add changes of direction
        if (direction == currentDirection)
            return;

        if (bUseDelay)
        {
            // Calculate delay
            float delay = MAX_REACTION_TIME * (1 - this->strength_);

            // Add a new Timer
            Timer* timer = new Timer(delay, false, createExecutor(createFunctor(&PongAI::delayedMove, this)));
            this->reactionTimers_.push_back(std::pair<Timer*, char>(timer, direction));
        }
        else
        {
            this->movement_ = direction;
        }
    }

    /**
    @brief
        Is called, when a delayed move takes effect.
    */
    void PongAI::delayedMove()
    {
        // Get the new movement direction from the timer list
        this->movement_ = this->reactionTimers_.front().second;

        // Destroy the timer and remove it from the list
        Timer* timer = this->reactionTimers_.front().first;
        timer->destroy();

        this->reactionTimers_.pop_front();
    }
}

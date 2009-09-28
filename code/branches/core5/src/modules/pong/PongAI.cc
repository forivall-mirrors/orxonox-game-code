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

#include "PongAI.h"

#include "core/CoreIncludes.h"
#include "core/ConfigValueIncludes.h"
#include "tools/Timer.h"
#include "worldentities/ControllableEntity.h"
#include "PongBall.h"

namespace orxonox
{
    CreateUnloadableFactory(PongAI);

    const static float MAX_REACTION_TIME = 0.4f;

    PongAI::PongAI(BaseObject* creator) : Controller(creator)
    {
        RegisterObject(PongAI);

        this->ball_ = 0;
        this->ballDirection_ = Vector2::ZERO;
        this->ballEndPosition_ = 0;
        this->randomOffset_ = 0;
        this->relHysteresisOffset_ = 0.02f;
        this->strength_ = 0.5f;
        this->movement_ = 0;
        this->oldMove_ = 0;
        this->bOscillationAvoidanceActive_ = false;

        this->setConfigValues();
    }

    PongAI::~PongAI()
    {
        for (std::list<std::pair<Timer*, char> >::iterator it = this->reactionTimers_.begin(); it != this->reactionTimers_.end(); ++it)
            (*it).first->destroy();
    }

    void PongAI::setConfigValues()
    {
        SetConfigValue(strength_, 0.5).description("A value from 0 to 1 where 0 is weak and 1 is strong.");
    }

    void PongAI::tick(float dt)
    {
        if (!this->ball_ || !this->getControllableEntity())
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
                // The ball just startet to approach, initialize all values
                this->ballDirection_.x = 1;
                this->ballDirection_.y = sgn(ballvel.z);
                this->ballEndPosition_ = 0;
                this->randomOffset_ = 0;

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
                // (and delay ist false, so we're not in the wrong place because of a new end-position prediction)
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

    void PongAI::calculateRandomOffset()
    {
        // Calculate the exponent for the position-formula
        float exp = pow(10, 1 - 2*this->strength_); // strength: 0   -> exp = 10
                                                    // strength: 0.5 -> exp = 1
                                                    // strength: 1   -> exp = 0.1

        // Calculate the relative position where to hit the ball with the bat
        float position = pow(rnd(), exp); // exp > 1 -> position is more likely a small number
                                          // exp < 1 -> position is more likely a large number

        // The position shouln't be larger than 0.5 (50% of the bat-length from the middle is the end)
        position *= 0.48f;

        // Both sides are equally probable
        position *= rndsgn();

        // Calculate the offset in world units
        this->randomOffset_ = position * this->ball_->getBatLength() * this->ball_->getFieldDimension().y;
    }

    void PongAI::calculateBallEndPosition()
    {
        Vector3 position = this->ball_->getPosition();
        Vector3 velocity = this->ball_->getVelocity();
        Vector2 dimension = this->ball_->getFieldDimension();

        // calculate end-height: current height + slope * distance
        this->ballEndPosition_ = position.z + velocity.z / velocity.x * (-position.x + dimension.x / 2 * sgn(velocity.x));

        // Calculate bounces
        for (float limit = 0.35f; limit < this->strength_ || this->strength_ > 0.99f; limit += 0.4f)
        {
            // Calculate a random prediction error, based on the vertical speed of the ball and the strength of the AI
            float randomError = rnd(-1, 1) * dimension.y * (velocity.z / velocity.x / PongBall::MAX_REL_Z_VELOCITY) * (1 - this->strength_);

            // Bounce from the lower bound
            if (this->ballEndPosition_ > dimension.y / 2)
            {
                // Mirror the predicted position at the upper bound and add some random error
                this->ballEndPosition_ = dimension.y - this->ballEndPosition_ + randomError;
                continue;
            }
            // Bounce from the upper bound
            if (this->ballEndPosition_ < -dimension.y / 2)
            {
                // Mirror the predicted position at the lower bound and add some random error
                this->ballEndPosition_ = -dimension.y - this->ballEndPosition_ + randomError;
                continue;
            }
            // No bounce - break
            break;
        }
    }

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

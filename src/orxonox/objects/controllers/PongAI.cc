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

#include "OrxonoxStableHeaders.h"
#include "PongAI.h"

#include "core/CoreIncludes.h"
#include "core/ConfigValueIncludes.h"
#include "objects/worldentities/ControllableEntity.h"
#include "objects/worldentities/PongBall.h"
#include "tools/Timer.h"

namespace orxonox
{
    CreateUnloadableFactory(PongAI);

    const static float MAX_REACTION_TIME = 0.4;

    PongAI::PongAI(BaseObject* creator) : Controller(creator)
    {
        RegisterObject(PongAI);

        this->ball_ = 0;
        this->ballDirection_ = Vector2::ZERO;
        this->ballEndPosition_ = 0;
        this->randomOffset_ = 0;
        this->relHysteresisOffset_ = 0.02;
        this->strength_ = 0.5;
        this->movement_ = 0;

        this->setConfigValues();
    }

    PongAI::~PongAI()
    {
        for (std::list<std::pair<Timer<PongAI>*, char> >::iterator it = this->reactionTimers_.begin(); it != this->reactionTimers_.end(); ++it)
            delete (*it).first;
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

        // Check in which direction the ball is flying
        if ((mypos.x > 0 && ballvel.x < 0) || (mypos.x < 0 && ballvel.x > 0))
        {
            // The ball is flying away
            this->ballDirection_.x = -1;
            this->ballDirection_.y = 0;

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
            }

            if (this->ballDirection_.y != sgn(ballvel.z))
            {
                // The ball just bounced from a bound, recalculate the predicted end position
                this->ballDirection_.y = sgn(ballvel.z);

                this->calculateBallEndPosition();
            }

            // Move to the predicted end position with an additional offset (to hit the ball with the side of the bat)
            float desiredZValue = this->ballEndPosition_ + this->randomOffset_;

            if (mypos.z > desiredZValue + hysteresisOffset * (this->randomOffset_ < 0))
                move = 1;
            else if (mypos.z < desiredZValue - hysteresisOffset * (this->randomOffset_ > 0))
                move = -1;
        }

        this->move(move);
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
        position *= 0.48;

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
        for (float limit = 0.35; limit < this->strength_ || this->strength_ > 0.99; limit += 0.4)
        {
            // Bounce from the upper bound
            if (this->ballEndPosition_ > dimension.y / 2)
            {
                // Mirror the predicted position at the upper bound and add some random error
                this->ballEndPosition_ = dimension.y - this->ballEndPosition_ + (rnd(-1, 1) * dimension.y * (1 - this->strength_));
                continue;
            }
            // Bounce from the upper bound
            if (this->ballEndPosition_ < -dimension.y / 2)
            {
                // Mirror the predicted position at the lower bound and add some random error
                this->ballEndPosition_ = -dimension.y - this->ballEndPosition_ + (rnd(-1, 1) * dimension.y * (1 - this->strength_));
                continue;
            }
            // No bounce - break
            break;
        }
    }

    void PongAI::move(char direction)
    {
        // The current direction is either what we're doing right now (movement_) or what is last in the queue
        char currentDirection = this->movement_;
        if (this->reactionTimers_.size() > 0)
            currentDirection = this->reactionTimers_.back().second;

        // Only add changes of direction
        if (direction == currentDirection)
            return;

        // Calculate delay, but only to change direction or start moving (stop works without delay)
        if (direction != 0)
        {
            float delay = MAX_REACTION_TIME * (1 - this->strength_);

            // Add a new Timer
            Timer<PongAI>* timer = new Timer<PongAI>(delay, false, this, createExecutor(createFunctor(&PongAI::delayedMove)));
            this->reactionTimers_.push_back(std::pair<Timer<PongAI>*, char>(timer, direction));
        }
        else
        {
            this->movement_ = 0;
        }
    }

    void PongAI::delayedMove()
    {
        // Get the new movement direction from the timer list
        this->movement_ = this->reactionTimers_.front().second;

        // Destroy the timer and remove it from the list
        Timer<PongAI>* timer = this->reactionTimers_.front().first;
        delete timer;

        this->reactionTimers_.pop_front();
    }
}

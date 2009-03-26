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

namespace orxonox
{
    CreateUnloadableFactory(PongAI);

    PongAI::PongAI(BaseObject* creator) : Controller(creator)
    {
        RegisterObject(PongAI);

        this->ball_ = 0;
        this->randomOffset_ = 0;
        this->relHysteresisOffset_ = 0.02;
        this->strength_ = 0.5;

        this->setConfigValues();
    }

    void PongAI::setConfigValues()
    {
        SetConfigValue(strength_, 0.5).description("A value from 0 to 1 where 0 is weak and 1 is strong.");
    }

    void PongAI::tick(float dt)
    {
        if (!this->ball_ || !this->getControllableEntity())
            return;

        ControllableEntity* bat = this->getControllableEntity();

        Vector3 mypos = bat->getPosition();
        Vector3 ballpos = this->ball_->getPosition();
        Vector3 ballvel = this->ball_->getVelocity();
        float hysteresisOffset = this->relHysteresisOffset_ * this->ball_->getFieldDimension().y;

        // Check in which direction the ball is flying
        if ((mypos.x > 0 && ballvel.x < 0) || (mypos.x < 0 && ballvel.x > 0))
        {
            // Ball is flying away
            this->calculateRandomOffset();

            if (mypos.z > hysteresisOffset)
                bat->moveFrontBack(1);
            else if (mypos.z < -hysteresisOffset)
                bat->moveFrontBack(-1);
        }
        else if (ballvel.x == 0)
        {
            // Ball is standing still
            this->calculateRandomOffset();
        }
        else
        {
            // Ball is approaching
            float desiredZValue = ballpos.z + this->randomOffset_;

            if (mypos.z > desiredZValue + hysteresisOffset)
                bat->moveFrontBack(1);
            else if (mypos.z < desiredZValue - hysteresisOffset)
                bat->moveFrontBack(-1);
        }
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
        position *= 0.45;

        // Both sides are equally probable
        position *= sgn(rnd(-1,1));

        // Calculate the offset in world units
        this->randomOffset_ = position * this->ball_->getBatLength() * this->ball_->getFieldDimension().y;
    }
}

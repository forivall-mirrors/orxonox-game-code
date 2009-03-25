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
        this->randomOffset_ = rnd(-0.45, 0.45) * this->ball_->getBatLength() * this->ball_->getFieldDimension().y;
    }
}

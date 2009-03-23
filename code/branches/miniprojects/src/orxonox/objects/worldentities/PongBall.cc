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
#include "PongBall.h"

#include "core/CoreIncludes.h"
#include "objects/worldentities/PongBat.h"
#include "objects/gametypes/Gametype.h"

namespace orxonox
{
    CreateFactory(PongBall);

    PongBall::PongBall(BaseObject* creator) : MovableEntity(creator)
    {
        RegisterObject(PongBall);

        this->speed_ = 0;
        this->bat_ = 0;
    }

    void PongBall::tick(float dt)
    {
        SUPER(PongBall, tick, dt);

        if (Core::isMaster())
        {
            Vector3 position = this->getPosition();
            Vector3 velocity = this->getVelocity();

            if (position.z > this->fieldHeight_ / 2 || position.z < -this->fieldHeight_ / 2)
            {
                velocity.z = -velocity.z;

                if (position.z > this->fieldHeight_ / 2)
                    position.z = this->fieldHeight_ / 2;
                if (position.z < -this->fieldHeight_ / 2)
                    position.z = -this->fieldHeight_ / 2;
            }

            if (position.x > this->fieldWidth_ / 2 || position.x < -this->fieldWidth_ / 2)
            {
                velocity.x = -velocity.x;
                float distance = 0;

                if (position.x > this->fieldWidth_ / 2)
                {
                    position.x = this->fieldWidth_ / 2;
                    if (this->bat_ && this->bat_[1])
                    {
                        distance = (position.z - this->bat_[1]->getPosition().z) / (this->fieldHeight_ * this->batlength_ / 2);
                        if (this->getGametype() && this->bat_[0] && fabs(distance) > 1)
                        {
                            this->getGametype()->playerScored(this->bat_[0]->getPlayer());
                            return;
                        }
                    }
                }
                if (position.x < -this->fieldWidth_ / 2)
                {
                    position.x = -this->fieldWidth_ / 2;
                    if (this->bat_ && this->bat_[0])
                    {
                        distance = (position.z - this->bat_[0]->getPosition().z) / (this->fieldHeight_ * this->batlength_ / 2);
                        if (this->getGametype() && this->bat_[1] && fabs(distance) > 1)
                        {
                            this->getGametype()->playerScored(this->bat_[1]->getPlayer());
                            return;
                        }
                    }
                }

                velocity.z = distance * distance * sgn(distance) * 1.5 * this->speed_;
            }

            if (velocity != this->getVelocity())
                this->setVelocity(velocity);
            if (position != this->getPosition())
                this->setPosition(position);
        }
    }

    void PongBall::setSpeed(float speed)
    {
        if (speed != this->speed_)
        {
            this->speed_ = speed;

            Vector3 velocity = this->getVelocity();
            if (velocity.x != 0)
                velocity.x = sgn(velocity.x) * this->speed_;
            else
                velocity.x = this->speed_ * sgn(rnd(-1,1));

            this->setVelocity(velocity);
        }
    }
}

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

#include "PongBall.h"

#include "core/CoreIncludes.h"
#include "core/GameMode.h"
#include "gametypes/Gametype.h"
#include "PongBat.h"

namespace orxonox
{
    CreateFactory(PongBall);

    const float PongBall::MAX_REL_Z_VELOCITY = 1.5;

    PongBall::PongBall(BaseObject* creator)
        : MovableEntity(creator)
    {
        RegisterObject(PongBall);

        this->speed_ = 0;
        this->accelerationFactor_ = 1.0f;
        this->bat_ = 0;
        this->batID_ = new unsigned int[2];
        this->batID_[0] = OBJECTID_UNKNOWN;
        this->batID_[1] = OBJECTID_UNKNOWN;
        this->relMercyOffset_ = 0.05f;

        this->registerVariables();
    }

    PongBall::~PongBall()
    {
    }

    void PongBall::registerVariables()
    {
        registerVariable( this->fieldWidth_ );
        registerVariable( this->fieldHeight_ );
        registerVariable( this->batlength_ );
        registerVariable( this->speed_ );
        registerVariable( this->relMercyOffset_ );
        registerVariable( this->batID_[0] );
        registerVariable( this->batID_[1], VariableDirection::ToClient, new NetworkCallback<PongBall>( this, &PongBall::applyBats) );
    }

    void PongBall::tick(float dt)
    {
        SUPER(PongBall, tick, dt);

        Vector3 position = this->getPosition();
        Vector3 velocity = this->getVelocity();
        Vector3 acceleration = this->getAcceleration();

        if (position.z > this->fieldHeight_ / 2 || position.z < -this->fieldHeight_ / 2)
        {
            velocity.z = -velocity.z;
            if (position.z > this->fieldHeight_ / 2)
                position.z = this->fieldHeight_ / 2;
            if (position.z < -this->fieldHeight_ / 2)
                position.z = -this->fieldHeight_ / 2;

            this->fireEvent();
        }

        if (position.x > this->fieldWidth_ / 2 || position.x < -this->fieldWidth_ / 2)
        {
            float distance = 0;

            if (this->bat_)
            {
                if (position.x > this->fieldWidth_ / 2 && this->bat_[1])
                {
                    distance = (position.z - this->bat_[1]->getPosition().z) / (this->fieldHeight_ * (this->batlength_ * 1.10f) / 2);
                    if (fabs(distance) <= 1)
                    {
                        position.x = this->fieldWidth_ / 2;
                        velocity.x = -velocity.x;
                        velocity.z = distance * distance * sgn(distance) * PongBall::MAX_REL_Z_VELOCITY * this->speed_;
                        acceleration = this->bat_[1]->getVelocity() * this->accelerationFactor_ * -1;

                        this->fireEvent();
                    }
                    else if (GameMode::isMaster() && position.x > this->fieldWidth_ / 2 * (1 + this->relMercyOffset_))
                    {
                        if (this->getGametype() && this->bat_[0])
                        {
                            this->getGametype()->playerScored(this->bat_[0]->getPlayer());
                            return;
                        }
                    }
                }
                if (position.x < -this->fieldWidth_ / 2 && this->bat_[0])
                {
                    distance = (position.z - this->bat_[0]->getPosition().z) / (this->fieldHeight_ * (this->batlength_ * 1.10f) / 2);
                    if (fabs(distance) <= 1)
                    {
                        position.x = -this->fieldWidth_ / 2;
                        velocity.x = -velocity.x;
                        velocity.z = distance * distance * sgn(distance) * PongBall::MAX_REL_Z_VELOCITY * this->speed_;
                        acceleration = this->bat_[0]->getVelocity() * this->accelerationFactor_ * -1;

                        this->fireEvent();
                    }
                    else if (GameMode::isMaster() && position.x < -this->fieldWidth_ / 2 * (1 + this->relMercyOffset_))
                    {
                        if (this->getGametype() && this->bat_[1])
                        {
                            this->getGametype()->playerScored(this->bat_[1]->getPlayer());
                            return;
                        }
                    }
                }
            }
        }

        if (acceleration != this->getAcceleration())
            this->setAcceleration(acceleration);
        if (velocity != this->getVelocity())
            this->setVelocity(velocity);
        if (position != this->getPosition())
            this->setPosition(position);
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

    void PongBall::setBats(PongBat** bats)
    {
        this->bat_ = bats;
        this->batID_[0] = this->bat_[0]->getObjectID();
        this->batID_[1] = this->bat_[1]->getObjectID();
    }

    void PongBall::applyBats()
    {
        if (!this->bat_)
            this->bat_ = new PongBat*[2];
        if (this->batID_[0] != OBJECTID_UNKNOWN)
            this->bat_[0] = orxonox_cast<PongBat*>(Synchronisable::getSynchronisable(this->batID_[0]));
        if (this->batID_[1] != OBJECTID_UNKNOWN)
            this->bat_[1] = orxonox_cast<PongBat*>(Synchronisable::getSynchronisable(this->batID_[1]));
    }
}

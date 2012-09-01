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

#include "PongBat.h"

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"

namespace orxonox
{
    CreateFactory(PongBat);

    PongBat::PongBat(BaseObject* creator) : ControllableEntity(creator)
    {
        RegisterObject(PongBat);

        this->movement_ = 0;
        this->bMoveLocal_ = false;
        this->speed_ = 60;
        this->length_ = 0.25;
        this->fieldHeight_ = 100;
        this->bSteadiedPosition_ = false;

        this->registerVariables();
    }

    void PongBat::registerVariables()
    {
        registerVariable(this->speed_);
        registerVariable(this->fieldHeight_);
        registerVariable(this->length_);
    }

    void PongBat::tick(float dt)
    {
        if (this->hasLocalController())
        {
            if (this->movement_ != 0)
            {
                this->movement_ = clamp(this->movement_, -1.0f, 1.0f) * this->speed_;

                if (this->bMoveLocal_)
                    this->setVelocity(this->getOrientation() * Vector3(this->movement_, 0, 0));
                else
                    this->setVelocity(0, 0, this->movement_);

                this->movement_ = 0;
                this->bSteadiedPosition_ = false;
            }
            else if (!this->bSteadiedPosition_)
            {
                // To ensure network synchronicity
                this->setVelocity(0, 0, 0);
                this->setPosition(this->getPosition());
                this->bSteadiedPosition_ = true;
            }
        }

        SUPER(PongBat, tick, dt);

        Vector3 position = this->getPosition();
        if (position.z > this->fieldHeight_ / 2 - this->fieldHeight_ * this->length_ / 2)
            position.z = this->fieldHeight_ / 2 - this->fieldHeight_ * this->length_ / 2;
        if (position.z < -this->fieldHeight_ / 2 + this->fieldHeight_ * this->length_ / 2)
            position.z = -this->fieldHeight_ / 2 + this->fieldHeight_ * this->length_ / 2;
        if (position != this->getPosition())
        {
            this->setPosition(position);
            this->setVelocity( Vector3::ZERO );
        }
    }

    void PongBat::moveFrontBack(const Vector2& value)
    {
        this->bMoveLocal_ = false;
        this->movement_ = -value.x;
    }

    void PongBat::moveRightLeft(const Vector2& value)
    {
        this->bMoveLocal_ = true;
        this->movement_ = value.x;
    }

    void PongBat::changedPlayer()
    {
        this->setVelocity(0, 0, 0);
    }
}

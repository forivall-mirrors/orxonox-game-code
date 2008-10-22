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
#include "Spectator.h"

#include "core/CoreIncludes.h"
#include "objects/worldentities/Model.h"

namespace orxonox
{
    CreateFactory(Spectator);

    Spectator::Spectator()
    {
        RegisterObject(Spectator);

        this->speed_ = 100;
        this->rotationSpeed_ = 3;

        this->yaw_ = 0;
        this->pitch_ = 0;
        this->roll_ = 0;
        this->setHudTemplate("spectatorhud");

        this->setDestroyWhenPlayerLeft(true);

        Model* temp = new Model;
        temp->setMeshSource("assff.mesh");
        this->attach(temp);
    }

    Spectator::~Spectator()
    {
    }

    void Spectator::tick(float dt)
    {
        Vector3 velocity = this->getVelocity();
        velocity.normalise();
        this->setVelocity(velocity * this->speed_);

        // TODO: Check why I have removed *dt (1337)
        this->yaw(Radian(this->yaw_ * this->rotationSpeed_));
        this->pitch(Radian(this->pitch_ * this->rotationSpeed_));
        this->roll(Radian(this->roll_ * this->rotationSpeed_));

        this->yaw_ = this->pitch_ = this->roll_ = 0;

        SUPER(Spectator, tick, dt);

        this->setVelocity(Vector3::ZERO);
    }

    void Spectator::setPlayer(PlayerInfo* player)
    {
        ControllableEntity::setPlayer(player);

        this->setObjectMode(network::direction::toclient);
    }

    void Spectator::moveFrontBack(float value)
    {
        this->setVelocity(this->getVelocity() + value * this->speed_ * WorldEntity::FRONT);
    }

    void Spectator::moveRightLeft(float value)
    {
        this->setVelocity(this->getVelocity() + value * this->speed_ * WorldEntity::RIGHT);
    }

    void Spectator::moveUpDown(float value)
    {
        this->setVelocity(this->getVelocity() + value * this->speed_ * WorldEntity::UP);
    }

    void Spectator::rotateYaw(float value)
    {
        this->yaw_ = value;
    }

    void Spectator::rotatePitch(float value)
    {
        this->pitch_ = value;
    }

    void Spectator::rotateRoll(float value)
    {
        this->roll_ = value;
    }

    void Spectator::fire()
    {
    }
}

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
#include "SpaceShip.h"

#include "BulletDynamics/Dynamics/btRigidBody.h"

#include "util/Math.h"
#include "util/Exception.h"
#include "core/CoreIncludes.h"
#include "core/ConfigValueIncludes.h"
#include "core/XMLPort.h"

namespace orxonox
{
    const float orientationGain = 100;
    CreateFactory(SpaceShip);

    SpaceShip::SpaceShip(BaseObject* creator) : Pawn(creator)
    {
        RegisterObject(SpaceShip);

        this->primaryThrust_  = 100;
        this->auxilaryThrust_ =  30;
        this->rotationThrust_ =  10;

        this->localLinearAcceleration_.setValue(0, 0, 0);
        this->localAngularAcceleration_.setValue(0, 0, 0);

        this->bInvertYAxis_ = false;

        this->setDestroyWhenPlayerLeft(true);

        // SpaceShip is always a physical object per default
        // Be aware of this call: The collision type legality check will not reach derived classes!
        this->setCollisionType(WorldEntity::Dynamic);
        // Get notification about collisions
        this->enableCollisionCallback();

        this->setConfigValues();
        this->registerVariables();
    }

    SpaceShip::~SpaceShip()
    {
    }

    void SpaceShip::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(SpaceShip, XMLPort, xmlelement, mode);

        XMLPortParamVariable(SpaceShip, "primaryThrust",  primaryThrust_,  xmlelement, mode);
        XMLPortParamVariable(SpaceShip, "auxilaryThrust", auxilaryThrust_, xmlelement, mode);
        XMLPortParamVariable(SpaceShip, "rotationThrust", rotationThrust_, xmlelement, mode);
    }

    void SpaceShip::registerVariables()
    {
        registerVariable(this->primaryThrust_,  variableDirection::toclient);
        registerVariable(this->auxilaryThrust_, variableDirection::toclient);
        registerVariable(this->rotationThrust_, variableDirection::toclient);
    }

    void SpaceShip::setConfigValues()
    {
        SetConfigValue(bInvertYAxis_, false).description("Set this to true for joystick-like mouse behaviour (mouse up = ship down).");
    }

    bool SpaceShip::isCollisionTypeLegal(WorldEntity::CollisionType type) const
    {
        if (type != WorldEntity::Dynamic)
        {
            CCOUT(1) << "Error: Cannot tell a SpaceShip not to be dynamic! Ignoring." << std::endl;
            assert(false); // Only in debug mode
            return false;
        }
        else
            return true;
    }

    void SpaceShip::tick(float dt)
    {
        SUPER(SpaceShip, tick, dt);

        if (this->isLocallyControlled())
        {
            this->localLinearAcceleration_.setX(this->localLinearAcceleration_.x() * getMass() * this->auxilaryThrust_);
            this->localLinearAcceleration_.setY(this->localLinearAcceleration_.y() * getMass() * this->auxilaryThrust_);
            if (this->localLinearAcceleration_.z() > 0)
                this->localLinearAcceleration_.setZ(this->localLinearAcceleration_.z() * getMass() * this->auxilaryThrust_);
            else
                this->localLinearAcceleration_.setZ(this->localLinearAcceleration_.z() * getMass() * this->primaryThrust_);
            this->physicalBody_->applyCentralForce(physicalBody_->getWorldTransform().getBasis() * this->localLinearAcceleration_);
            this->localLinearAcceleration_.setValue(0, 0, 0);

            this->localAngularAcceleration_ *= this->getLocalInertia() * this->rotationThrust_;
            this->physicalBody_->applyTorque(physicalBody_->getWorldTransform().getBasis() * this->localAngularAcceleration_);
            this->localAngularAcceleration_.setValue(0, 0, 0);
        }
    }

    void SpaceShip::moveFrontBack(const Vector2& value)
    {
        this->localLinearAcceleration_.setZ(this->localLinearAcceleration_.z() - value.x);
    }

    void SpaceShip::moveRightLeft(const Vector2& value)
    {
        this->localLinearAcceleration_.setX(this->localLinearAcceleration_.x() + value.x);
    }

    void SpaceShip::moveUpDown(const Vector2& value)
    {
        this->localLinearAcceleration_.setY(this->localLinearAcceleration_.y() + value.x);
    }

    void SpaceShip::rotateYaw(const Vector2& value)
    {
        this->localAngularAcceleration_.setY(this->localAngularAcceleration_.y() + value.x);
    }

    void SpaceShip::rotatePitch(const Vector2& value)
    {
        this->localAngularAcceleration_.setX(this->localAngularAcceleration_.x() + value.x);
    }

    void SpaceShip::rotateRoll(const Vector2& value)
    {
        this->localAngularAcceleration_.setZ(this->localAngularAcceleration_.z() + value.x);
    }

    void SpaceShip::fire()
    {
    }
}

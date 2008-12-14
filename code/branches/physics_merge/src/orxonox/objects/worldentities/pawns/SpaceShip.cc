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

        this->zeroDegree_ = 0;

        this->maxSpeed_ = 0;
        this->maxSecondarySpeed_ = 0;
        this->maxRotation_ = 0;
        this->translationAcceleration_ = 0;
        this->rotationAcceleration_ = 0;
        this->translationDamping_ = 0;

        this->yawRotation_ = 0;
        this->pitchRotation_ = 0;
        this->rollRotation_ = 0;

        this->bInvertYAxis_ = false;

        this->setDestroyWhenPlayerLeft(true);

        this->setConfigValues();
        this->registerVariables();
    }

    SpaceShip::~SpaceShip()
    {
    }

    void SpaceShip::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(SpaceShip, XMLPort, xmlelement, mode);

        XMLPortParam(SpaceShip, "maxspeed",          setMaxSpeed,          getMaxSpeed,          xmlelement, mode);
        XMLPortParam(SpaceShip, "maxsecondaryspeed", setMaxSecondarySpeed, getMaxSecondarySpeed, xmlelement, mode);
        XMLPortParam(SpaceShip, "maxrotation",       setMaxRotation,       getMaxRotation,       xmlelement, mode);
        XMLPortParam(SpaceShip, "transacc",          setTransAcc,          getTransAcc,          xmlelement, mode);
        XMLPortParam(SpaceShip, "rotacc",            setRotAcc,            getRotAcc,            xmlelement, mode);
        XMLPortParam(SpaceShip, "transdamp",         setTransDamp,         getTransDamp,         xmlelement, mode);

        if (this->physicalBody_)
        {
            this->physicalBody_->setDamping(0.7, 0.3);
        }
    }

    void SpaceShip::registerVariables()
    {
        registerVariable(this->maxSpeed_,                variableDirection::toclient);
        registerVariable(this->maxSecondarySpeed_,       variableDirection::toclient);
        registerVariable(this->maxRotation_,             variableDirection::toclient);
        registerVariable(this->translationAcceleration_, variableDirection::toclient);
        registerVariable(this->rotationAcceleration_,    variableDirection::toclient);
        registerVariable(this->translationDamping_,      variableDirection::toclient);
    }

    void SpaceShip::setConfigValues()
    {
        SetConfigValue(bInvertYAxis_, false).description("Set this to true for joystick-like mouse behaviour (mouse up = ship down).");
    }

    void SpaceShip::tick(float dt)
    {
        SUPER(SpaceShip, tick, dt);
    }

    void SpaceShip::moveFrontBack(const Vector2& value)
    {
        assert(this->physicalBody_);
        this->physicalBody_->applyCentralForce(physicalBody_->getWorldTransform().getBasis() * btVector3(0.0f, 0.0f, -getMass() * value.x * 100));
    }

    void SpaceShip::moveRightLeft(const Vector2& value)
    {
        this->physicalBody_->applyCentralForce(physicalBody_->getWorldTransform().getBasis() * btVector3(getMass() * value.x * 100, 0.0f, 0.0f));
    }

    void SpaceShip::moveUpDown(const Vector2& value)
    {
        this->physicalBody_->applyCentralForce(physicalBody_->getWorldTransform().getBasis() * btVector3(0.0f, getMass() * value.x * 100, 0.0f));
    }

    void SpaceShip::rotateYaw(const Vector2& value)
    {
        this->physicalBody_->applyTorque(physicalBody_->getWorldTransform().getBasis() * btVector3(0.0f, 1 / this->physicalBody_->getInvInertiaDiagLocal().y() * value.y * orientationGain, 0.0f));
    }

    void SpaceShip::rotatePitch(const Vector2& value)
    {
        this->physicalBody_->applyTorque(physicalBody_->getWorldTransform().getBasis() * btVector3(1 / this->physicalBody_->getInvInertiaDiagLocal().x() * value.y * orientationGain, 0.0f, 0.0f));
    }

    void SpaceShip::rotateRoll(const Vector2& value)
    {
        this->physicalBody_->applyTorque(physicalBody_->getWorldTransform().getBasis() * btVector3(0.0f, 0.0f, -1 / this->physicalBody_->getInvInertiaDiagLocal().z() * value.y * orientationGain));
    }

    void SpaceShip::fire()
    {
    }
}

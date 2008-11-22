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
#include "MovableEntity.h"

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "core/Executor.h"
#include "tools/Timer.h"

namespace orxonox
{
    static const float MAX_RESYNCHRONIZE_TIME = 3.0f;

    CreateFactory(MovableEntity);

    MovableEntity::MovableEntity(BaseObject* creator) : WorldEntity(creator)
    {
        RegisterObject(MovableEntity);

        this->velocity_ = Vector3::ZERO;
        this->acceleration_ = Vector3::ZERO;
        this->rotationAxis_ = Vector3::ZERO;
        this->rotationRate_ = 0;
        this->momentum_ = 0;

        this->overwrite_position_ = Vector3::ZERO;
        this->overwrite_orientation_ = Quaternion::IDENTITY;

        this->registerVariables();
    }

    MovableEntity::~MovableEntity()
    {
    }

    void MovableEntity::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(MovableEntity, XMLPort, xmlelement, mode);

        XMLPortParamTemplate(MovableEntity, "velocity", setVelocity, getVelocity, xmlelement, mode, const Vector3&);
        XMLPortParamTemplate(MovableEntity, "rotationaxis", setRotationAxis, getRotationAxis, xmlelement, mode, const Vector3&);
        XMLPortParamTemplate(MovableEntity, "rotationrate", setRotationRate, getRotationRate, xmlelement, mode, const Degree&);
    }

    void MovableEntity::tick(float dt)
    {
        if (this->isActive())
        {
            this->velocity_ += (dt * this->acceleration_);
            this->node_->translate(dt * this->velocity_);

            this->rotationRate_ += (dt * this->momentum_);
            this->node_->rotate(this->rotationAxis_, this->rotationRate_  * dt);
        }
    }

    void MovableEntity::registerVariables()
    {
        registerVariable(this->velocity_.x, variableDirection::toclient);
        registerVariable(this->velocity_.y, variableDirection::toclient);
        registerVariable(this->velocity_.z, variableDirection::toclient);
  
        registerVariable(this->rotationAxis_.x, variableDirection::toclient);
        registerVariable(this->rotationAxis_.y, variableDirection::toclient);
        registerVariable(this->rotationAxis_.z, variableDirection::toclient);
  
        registerVariable(this->rotationRate_, variableDirection::toclient);
  
        registerVariable(this->overwrite_position_,    variableDirection::toclient, new NetworkCallback<MovableEntity>(this, &MovableEntity::overwritePosition));
        registerVariable(this->overwrite_orientation_, variableDirection::toclient, new NetworkCallback<MovableEntity>(this, &MovableEntity::overwriteOrientation));
    }

    void MovableEntity::overwritePosition()
    {
        this->node_->setPosition(this->overwrite_position_);
    }

    void MovableEntity::overwriteOrientation()
    {
        this->node_->setOrientation(this->overwrite_orientation_);
    }

    void MovableEntity::clientConnected(unsigned int clientID)
    {
        new Timer<MovableEntity>(rnd() * MAX_RESYNCHRONIZE_TIME, false, this, createExecutor(createFunctor(&MovableEntity::resynchronize)), true);
    }

    void MovableEntity::clientDisconnected(unsigned int clientID)
    {
    }

    void MovableEntity::resynchronize()
    {
        this->overwrite_position_ = this->getPosition();
        this->overwrite_orientation_ = this->getOrientation();
    }

    void MovableEntity::setPosition(const Vector3& position)
    {
        this->node_->setPosition(position);
        this->overwrite_position_ = this->node_->getPosition();
    }

    void MovableEntity::translate(const Vector3& distance, Ogre::Node::TransformSpace relativeTo)
    {
        this->node_->translate(distance, relativeTo);
        this->overwrite_position_ = this->node_->getPosition();
    }

    void MovableEntity::setOrientation(const Quaternion& orientation)
    {
        this->node_->setOrientation(orientation);
        this->overwrite_orientation_ = this->node_->getOrientation();
    }

    void MovableEntity::rotate(const Quaternion& rotation, Ogre::Node::TransformSpace relativeTo)
    {
        this->node_->rotate(rotation, relativeTo);
        this->overwrite_orientation_ = this->node_->getOrientation();
    }

    void MovableEntity::yaw(const Degree& angle, Ogre::Node::TransformSpace relativeTo)
    {
        this->node_->yaw(angle, relativeTo);
        this->overwrite_orientation_ = this->node_->getOrientation();
    }

    void MovableEntity::pitch(const Degree& angle, Ogre::Node::TransformSpace relativeTo)
    {
        this->node_->pitch(angle, relativeTo);
        this->overwrite_orientation_ = this->node_->getOrientation();
    }

    void MovableEntity::roll(const Degree& angle, Ogre::Node::TransformSpace relativeTo)
    {
        this->node_->roll(angle, relativeTo);
        this->overwrite_orientation_ = this->node_->getOrientation();
    }

    void MovableEntity::lookAt(const Vector3& target, Ogre::Node::TransformSpace relativeTo, const Vector3& localDirectionVector)
    {
        this->node_->lookAt(target, relativeTo, localDirectionVector);
        this->overwrite_orientation_ = this->node_->getOrientation();
    }

    void MovableEntity::setDirection(const Vector3& direction, Ogre::Node::TransformSpace relativeTo, const Vector3& localDirectionVector)
    {
        this->node_->setDirection(direction, relativeTo, localDirectionVector);
        this->overwrite_orientation_ = this->node_->getOrientation();
    }
}

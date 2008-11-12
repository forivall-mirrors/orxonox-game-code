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
#include "LinearEntity.h"

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "core/Executor.h"
#include "tools/Timer.h"

namespace orxonox
{
    static const float MAX_RESYNCHRONIZE_TIME = 3.0f;

    CreateFactory(LinearEntity);

    LinearEntity::LinearEntity(BaseObject* creator) : MovableEntity(creator)
    {
        RegisterObject(LinearEntity);

        this->velocity_ = Vector3::ZERO;
        this->acceleration_ = Vector3::ZERO;
        this->rotationAxis_ = Vector3::ZERO;
        this->rotationRate_ = 0;
        this->momentum_ = 0;

        this->overwrite_position_ = Vector3::ZERO;
        this->overwrite_orientation_ = Quaternion::IDENTITY;

        this->registerVariables();
    }

    LinearEntity::~LinearEntity()
    {
    }

    void LinearEntity::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(LinearEntity, XMLPort, xmlelement, mode);

        XMLPortParamTemplate(LinearEntity, "velocity", setVelocity, getVelocity, xmlelement, mode, const Vector3&);
        XMLPortParamTemplate(LinearEntity, "rotationaxis", setRotationAxis, getRotationAxis, xmlelement, mode, const Vector3&);
        XMLPortParamTemplate(LinearEntity, "rotationrate", setRotationRate, getRotationRate, xmlelement, mode, const Degree&);
    }

    void LinearEntity::tick(float dt)
    {
        if (this->isActive())
        {
            this->velocity_ += (dt * this->acceleration_);
            this->node_->translate(dt * this->velocity_);

            this->rotationRate_ += (dt * this->momentum_);
            this->node_->rotate(this->rotationAxis_, this->rotationRate_  * dt);
        }
    }

    void LinearEntity::registerVariables()
    {
        REGISTERDATA(this->velocity_.x, network::direction::toclient);
        REGISTERDATA(this->velocity_.y, network::direction::toclient);
        REGISTERDATA(this->velocity_.z, network::direction::toclient);

        REGISTERDATA(this->rotationAxis_.x, network::direction::toclient);
        REGISTERDATA(this->rotationAxis_.y, network::direction::toclient);
        REGISTERDATA(this->rotationAxis_.z, network::direction::toclient);

        REGISTERDATA(this->rotationRate_, network::direction::toclient);

        REGISTERDATA(this->overwrite_position_,    network::direction::toclient, new network::NetworkCallback<LinearEntity>(this, &LinearEntity::overwritePosition));
        REGISTERDATA(this->overwrite_orientation_, network::direction::toclient, new network::NetworkCallback<LinearEntity>(this, &LinearEntity::overwriteOrientation));
    }

    void LinearEntity::overwritePosition()
    {
        this->node_->setPosition(this->overwrite_position_);
    }

    void LinearEntity::overwriteOrientation()
    {
        this->node_->setOrientation(this->overwrite_orientation_);
    }

    void LinearEntity::clientConnected(unsigned int clientID)
    {
        new Timer<LinearEntity>(rnd() * MAX_RESYNCHRONIZE_TIME, false, this, createExecutor(createFunctor(&LinearEntity::resynchronize)), true);
    }

    void LinearEntity::clientDisconnected(unsigned int clientID)
    {
    }

    void LinearEntity::resynchronize()
    {
        this->overwrite_position_ = this->getPosition();
        this->overwrite_orientation_ = this->getOrientation();
    }

    void LinearEntity::setPosition(const Vector3& position)
    {
        this->node_->setPosition(position);
        this->overwrite_position_ = this->node_->getPosition();
    }

    void LinearEntity::translate(const Vector3& distance, Ogre::Node::TransformSpace relativeTo)
    {
        this->node_->translate(distance, relativeTo);
        this->overwrite_position_ = this->node_->getPosition();
    }

    void LinearEntity::setOrientation(const Quaternion& orientation)
    {
        this->node_->setOrientation(orientation);
        this->overwrite_orientation_ = this->node_->getOrientation();
    }

    void LinearEntity::rotate(const Quaternion& rotation, Ogre::Node::TransformSpace relativeTo)
    {
        this->node_->rotate(rotation, relativeTo);
        this->overwrite_orientation_ = this->node_->getOrientation();
    }

    void LinearEntity::yaw(const Degree& angle, Ogre::Node::TransformSpace relativeTo)
    {
        this->node_->yaw(angle, relativeTo);
        this->overwrite_orientation_ = this->node_->getOrientation();
    }

    void LinearEntity::pitch(const Degree& angle, Ogre::Node::TransformSpace relativeTo)
    {
        this->node_->pitch(angle, relativeTo);
        this->overwrite_orientation_ = this->node_->getOrientation();
    }

    void LinearEntity::roll(const Degree& angle, Ogre::Node::TransformSpace relativeTo)
    {
        this->node_->roll(angle, relativeTo);
        this->overwrite_orientation_ = this->node_->getOrientation();
    }

    void LinearEntity::lookAt(const Vector3& target, Ogre::Node::TransformSpace relativeTo, const Vector3& localDirectionVector)
    {
        this->node_->lookAt(target, relativeTo, localDirectionVector);
        this->overwrite_orientation_ = this->node_->getOrientation();
    }

    void LinearEntity::setDirection(const Vector3& direction, Ogre::Node::TransformSpace relativeTo, const Vector3& localDirectionVector)
    {
        this->node_->setDirection(direction, relativeTo, localDirectionVector);
        this->overwrite_orientation_ = this->node_->getOrientation();
    }
}

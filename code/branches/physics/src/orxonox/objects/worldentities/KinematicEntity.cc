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
#include "KinematicEntity.h"

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "core/Executor.h"
#include "tools/Timer.h"

namespace orxonox
{
    static const float MAX_RESYNCHRONIZE_TIME = 3.0f;

    CreateFactory(KinematicEntity);

    KinematicEntity::KinematicEntity(BaseObject* creator) : WorldEntity(creator)
    {
        RegisterObject(KinematicEntity);

        this->velocity_ = Vector3::ZERO;
        this->acceleration_ = Vector3::ZERO;
        this->rotationAxis_ = Vector3::ZERO;
        this->rotationRate_ = 0;
        this->momentum_ = 0;

        this->overwrite_position_ = Vector3::ZERO;
        this->overwrite_orientation_ = Quaternion::IDENTITY;

        this->registerVariables();
    }

    KinematicEntity::~KinematicEntity()
    {
    }

    void KinematicEntity::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(KinematicEntity, XMLPort, xmlelement, mode);

        XMLPortParamTemplate(KinematicEntity, "velocity", setVelocity, getVelocity, xmlelement, mode, const Vector3&);
        XMLPortParamTemplate(KinematicEntity, "rotationaxis", setRotationAxis, getRotationAxis, xmlelement, mode, const Vector3&);
        XMLPortParamTemplate(KinematicEntity, "rotationrate", setRotationRate, getRotationRate, xmlelement, mode, const Degree&);
    }

    void KinematicEntity::tick(float dt)
    {
        if (this->isActive())
        {
            this->velocity_ += (dt * this->acceleration_);
            this->node_->translate(dt * this->velocity_);

            this->rotationRate_ += (dt * this->momentum_);
            this->node_->rotate(this->rotationAxis_, this->rotationRate_  * dt);
        }
    }

    void KinematicEntity::registerVariables()
    {
        REGISTERDATA(this->velocity_.x, network::direction::toclient);
        REGISTERDATA(this->velocity_.y, network::direction::toclient);
        REGISTERDATA(this->velocity_.z, network::direction::toclient);

        REGISTERDATA(this->rotationAxis_.x, network::direction::toclient);
        REGISTERDATA(this->rotationAxis_.y, network::direction::toclient);
        REGISTERDATA(this->rotationAxis_.z, network::direction::toclient);

        REGISTERDATA(this->rotationRate_, network::direction::toclient);

        REGISTERDATA(this->overwrite_position_,    network::direction::toclient, new network::NetworkCallback<KinematicEntity>(this, &KinematicEntity::overwritePosition));
        REGISTERDATA(this->overwrite_orientation_, network::direction::toclient, new network::NetworkCallback<KinematicEntity>(this, &KinematicEntity::overwriteOrientation));
    }

    void KinematicEntity::overwritePosition()
    {
        this->node_->setPosition(this->overwrite_position_);
    }

    void KinematicEntity::overwriteOrientation()
    {
        this->node_->setOrientation(this->overwrite_orientation_);
    }

    void KinematicEntity::clientConnected(unsigned int clientID)
    {
        new Timer<KinematicEntity>(rnd() * MAX_RESYNCHRONIZE_TIME, false, this, createExecutor(createFunctor(&KinematicEntity::resynchronize)), true);
    }

    void KinematicEntity::clientDisconnected(unsigned int clientID)
    {
    }

    void KinematicEntity::resynchronize()
    {
        this->overwrite_position_ = this->getPosition();
        this->overwrite_orientation_ = this->getOrientation();
    }

    void KinematicEntity::setPosition(const Vector3& position)
    {
        this->node_->setPosition(position);
        this->overwrite_position_ = this->node_->getPosition();
    }

    void KinematicEntity::translate(const Vector3& distance, Ogre::Node::TransformSpace relativeTo)
    {
        this->node_->translate(distance, relativeTo);
        this->overwrite_position_ = this->node_->getPosition();
    }

    void KinematicEntity::setOrientation(const Quaternion& orientation)
    {
        this->node_->setOrientation(orientation);
        this->overwrite_orientation_ = this->node_->getOrientation();
    }

    void KinematicEntity::rotate(const Quaternion& rotation, Ogre::Node::TransformSpace relativeTo)
    {
        this->node_->rotate(rotation, relativeTo);
        this->overwrite_orientation_ = this->node_->getOrientation();
    }

    void KinematicEntity::yaw(const Degree& angle, Ogre::Node::TransformSpace relativeTo)
    {
        this->node_->yaw(angle, relativeTo);
        this->overwrite_orientation_ = this->node_->getOrientation();
    }

    void KinematicEntity::pitch(const Degree& angle, Ogre::Node::TransformSpace relativeTo)
    {
        this->node_->pitch(angle, relativeTo);
        this->overwrite_orientation_ = this->node_->getOrientation();
    }

    void KinematicEntity::roll(const Degree& angle, Ogre::Node::TransformSpace relativeTo)
    {
        this->node_->roll(angle, relativeTo);
        this->overwrite_orientation_ = this->node_->getOrientation();
    }

    void KinematicEntity::lookAt(const Vector3& target, Ogre::Node::TransformSpace relativeTo, const Vector3& localDirectionVector)
    {
        this->node_->lookAt(target, relativeTo, localDirectionVector);
        this->overwrite_orientation_ = this->node_->getOrientation();
    }

    void KinematicEntity::setDirection(const Vector3& direction, Ogre::Node::TransformSpace relativeTo, const Vector3& localDirectionVector)
    {
        this->node_->setDirection(direction, relativeTo, localDirectionVector);
        this->overwrite_orientation_ = this->node_->getOrientation();
    }
}

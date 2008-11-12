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
 *      Martin Stypinski
 *
 */

#include "OrxonoxStableHeaders.h"
#include "MovableEntity.h"

#include "objects/Scene.h"

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"

namespace orxonox
{
    MovableEntity::MovableEntity(BaseObject* creator) : WorldEntity(creator)
    {
        RegisterObject(MovableEntity);

        this->registerVariables();
    }

    MovableEntity::~MovableEntity()
    {
    }

    void MovableEntity::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(MovableEntity, XMLPort, xmlelement, mode);
    }

    void MovableEntity::registerVariables()
    {
    }

    void MovableEntity::setPosition(const Vector3& position){
	// Here the code
        if(isDynamic() && bAddedToPhysicalWorld_){
            this->getScene()->getPhysicalWorld()->removeRigidBody(this->physicalBody_);
            this->getScene()->getPhysicalWorld()->addRigidBody(this->physicalBody_);
        }
        this->node_->setPosition(position);
        positionChanged();
    }

    void MovableEntity::translate(const Vector3& distance, Ogre::Node::TransformSpace relativeTo){
        if(isDynamic() && bAddedToPhysicalWorld_){
            this->getScene()->getPhysicalWorld()->removeRigidBody(this->physicalBody_);
            this->getScene()->getPhysicalWorld()->addRigidBody(this->physicalBody_);
        }
        this->node_->translate(distance, relativeTo); 
        translateChanged();
    }

    void MovableEntity::setOrientation(const Quaternion& orientation){
        if(isDynamic() && bAddedToPhysicalWorld_){
            this->getScene()->getPhysicalWorld()->removeRigidBody(this->physicalBody_);
            this->getScene()->getPhysicalWorld()->addRigidBody(this->physicalBody_);
        }
        this->node_->setOrientation(orientation);
        orientationChanged();
    }

    void MovableEntity::rotate(const Quaternion& rotation, Ogre::Node::TransformSpace relativeTo){
        if(isDynamic() && bAddedToPhysicalWorld_){
            this->getScene()->getPhysicalWorld()->removeRigidBody(this->physicalBody_);
            this->getScene()->getPhysicalWorld()->addRigidBody(this->physicalBody_);
        }
        this->node_->rotate(rotation, relativeTo);
        rotateChanged();
    }

    void MovableEntity::yaw(const Degree& angle, Ogre::Node::TransformSpace relativeTo){
        if(isDynamic() && bAddedToPhysicalWorld_){
            this->getScene()->getPhysicalWorld()->removeRigidBody(this->physicalBody_);
            this->getScene()->getPhysicalWorld()->addRigidBody(this->physicalBody_);
        }
        this->node_->yaw(angle, relativeTo);
        yawChanged();
    }

    void MovableEntity::pitch(const Degree& angle, Ogre::Node::TransformSpace relativeTo){
        if(isDynamic() && bAddedToPhysicalWorld_){
            this->getScene()->getPhysicalWorld()->removeRigidBody(this->physicalBody_);
            this->getScene()->getPhysicalWorld()->addRigidBody(this->physicalBody_);
        }
        this->node_->pitch(angle, relativeTo);
        pitchChanged();
    }

    void MovableEntity::roll(const Degree& angle, Ogre::Node::TransformSpace relativeTo){
        if(isDynamic() && bAddedToPhysicalWorld_){
            this->getScene()->getPhysicalWorld()->removeRigidBody(this->physicalBody_);
            this->getScene()->getPhysicalWorld()->addRigidBody(this->physicalBody_);
        }
        this->node_->roll(angle, relativeTo);
        rollChanged();
    }

    void MovableEntity::lookAt(const Vector3& target, Ogre::Node::TransformSpace relativeTo, const Vector3& localDirectionVector){
        if(isDynamic() && bAddedToPhysicalWorld_){
            this->getScene()->getPhysicalWorld()->removeRigidBody(this->physicalBody_);
            this->getScene()->getPhysicalWorld()->addRigidBody(this->physicalBody_);
        }
        this->node_->lookAt(target, relativeTo, localDirectionVector); 
        lookAtChanged();
    }

    void MovableEntity::setDirection(const Vector3& direction, Ogre::Node::TransformSpace relativeTo, const Vector3& localDirectionVector){
        if(isDynamic() && bAddedToPhysicalWorld_){
            this->getScene()->getPhysicalWorld()->removeRigidBody(this->physicalBody_);
            this->getScene()->getPhysicalWorld()->addRigidBody(this->physicalBody_);
        }
        this->node_->setDirection(direction, relativeTo, localDirectionVector); 
        directionChanged();
    }

}

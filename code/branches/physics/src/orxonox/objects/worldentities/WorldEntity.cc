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
#include "WorldEntity.h"

#include <cassert>
#include <OgreSceneManager.h>

#include "BulletCollision/CollisionShapes/btSphereShape.h"

#include "util/Exception.h"
#include "util/Convert.h"
#include "core/CoreIncludes.h"
#include "core/XMLPort.h"

#include "objects/Scene.h"

#include "StaticEntity.h"

namespace orxonox
{
    const Vector3 WorldEntity::FRONT = Vector3::NEGATIVE_UNIT_Z;
    const Vector3 WorldEntity::BACK  = Vector3::UNIT_Z;
    const Vector3 WorldEntity::LEFT  = Vector3::NEGATIVE_UNIT_X;
    const Vector3 WorldEntity::RIGHT = Vector3::UNIT_X;
    const Vector3 WorldEntity::DOWN  = Vector3::NEGATIVE_UNIT_Y;
    const Vector3 WorldEntity::UP    = Vector3::UNIT_Y;

    WorldEntity::WorldEntity(BaseObject* creator) : BaseObject(creator), network::Synchronisable(creator)
    {
        RegisterObject(WorldEntity);

        assert(this->getScene());
        assert(this->getScene()->getRootSceneNode());

        this->node_ = this->getScene()->getRootSceneNode()->createChildSceneNode();

        this->parent_ = 0;
        this->parentID_ = (unsigned int)-1;

        this->node_->setPosition(Vector3::ZERO);
        this->node_->setOrientation(Quaternion::IDENTITY);

        // Default behaviour does not include physics
        this->physicalBody_ = 0;

        this->registerVariables();
    }

    WorldEntity::~WorldEntity()
    {
        if (this->isInitialized())
        {
            this->node_->detachAllObjects();
            if (this->getScene()->getSceneManager())
                this->getScene()->getSceneManager()->destroySceneNode(this->node_->getName());
            
            // Physics is not guaranteed, so check first
            if (this->physicalBody_)
            {
                if (this->physicalBody_->isInWorld())
                    this->getScene()->getPhysicalWorld()->removeRigidBody(this->physicalBody_);
                if (this->physicalBody_->getCollisionShape())
                    delete this->physicalBody_->getCollisionShape();
                delete this->physicalBody_;
            }
        }
    }

    void WorldEntity::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(WorldEntity, XMLPort, xmlelement, mode);

        XMLPortParamTemplate(WorldEntity, "position", setPosition, getPosition, xmlelement, mode, const Vector3&);
        XMLPortParamTemplate(WorldEntity, "orientation", setOrientation, getOrientation, xmlelement, mode, const Quaternion&);
        XMLPortParamLoadOnly(WorldEntity, "lookat", lookAt_xmlport, xmlelement, mode);
        XMLPortParamLoadOnly(WorldEntity, "direction", setDirection_xmlport, xmlelement, mode);
        XMLPortParamLoadOnly(WorldEntity, "yaw", yaw_xmlport, xmlelement, mode);
        XMLPortParamLoadOnly(WorldEntity, "pitch", pitch_xmlport, xmlelement, mode);
        XMLPortParamLoadOnly(WorldEntity, "roll", roll_xmlport, xmlelement, mode);
        XMLPortParamTemplate(WorldEntity, "scale3D", setScale3D, getScale3D, xmlelement, mode, const Vector3&);
        XMLPortParam(WorldEntity, "scale", setScale, getScale, xmlelement, mode);

        XMLPortParam(WorldEntity, "collisionRadius", setCollisionRadius, getCollisionRadius, xmlelement, mode);
        XMLPortParam(WorldEntity, "collisionType", setCollisionTypeStr, getCollisionTypeStr, xmlelement, mode);
        XMLPortParam(WorldEntity, "mass", setMass, getMass, xmlelement, mode);

        if (this->physicalBody_)
            this->getScene()->getPhysicalWorld()->addRigidBody(this->physicalBody_);

        XMLPortObject(WorldEntity, WorldEntity, "attached", attach, getAttachedObject, xmlelement, mode);
    }

    void WorldEntity::registerVariables()
    {
        REGISTERDATA(this->bActive_,  network::direction::toclient, new network::NetworkCallback<WorldEntity>(this, &WorldEntity::changedActivity));
        REGISTERDATA(this->bVisible_, network::direction::toclient, new network::NetworkCallback<WorldEntity>(this, &WorldEntity::changedVisibility));

        REGISTERDATA(this->getScale3D().x, network::direction::toclient);
        REGISTERDATA(this->getScale3D().y, network::direction::toclient);
        REGISTERDATA(this->getScale3D().z, network::direction::toclient);

        REGISTERDATA(this->parentID_, network::direction::toclient, new network::NetworkCallback<WorldEntity>(this, &WorldEntity::updateParent));
    }

    void WorldEntity::updateParent()
    {
        WorldEntity* parent = dynamic_cast<WorldEntity*>(Synchronisable::getSynchronisable(this->parentID_));
        if (parent)
            this->attachToParent(parent);
    }

    void WorldEntity::attach(WorldEntity* object)
    {
        if (object->getParent())
            object->detachFromParent();
        else
        {
            Ogre::Node* parent = object->node_->getParent();
            if (parent)
                parent->removeChild(object->node_);
        }

        this->node_->addChild(object->node_);
        this->children_.insert(object);
        object->parent_ = this;
        object->parentID_ = this->getObjectID();

        // Do the physical connection if required
        //this->attachPhysicalObject(object);
    }

    //void WorldEntity::attachPhysicalObject(WorldEntity* object)
    //{
    //    StaticEntity* staticObject = dynamic_cast<StaticEntity*>(object);
    //    if (staticObject != 0 && this->hasPhysics())
    //    {
    //       btCompoundShape* compoundShape = dynamic_cast<btCompoundShape*>(this->physicalBody_->getCollisionShape());
    //       if (compoundShape == 0)
    //       {
    //            // create a compound shape and add both
    //            compoundShape = new btCompoundShape();
    //            compoundShape->addChildShape(this->physicalBody_->getCollisionShape());
    //            compoundShape->addChildShape(staticObject->getCollisionShape());
    //            this->physicalBody_->setCollisionShape();
    //       }
    //       else
    //       {
    //           compoundShape -> addChildShape(staticObject->getCollisionShape());
    //       }
    //    }
    //}

    void WorldEntity::detach(WorldEntity* object)
    {
        this->node_->removeChild(object->node_);
        this->children_.erase(object);
        object->parent_ = 0;
        object->parentID_ = (unsigned int)-1;

//        this->getScene()->getRootSceneNode()->addChild(object->node_);
    }

    WorldEntity* WorldEntity::getAttachedObject(unsigned int index) const
    {
        unsigned int i = 0;
        for (std::set<WorldEntity*>::const_iterator it = this->children_.begin(); it != this->children_.end(); ++it)
        {
            if (i == index)
                return (*it);
            ++i;
        }
        return 0;
    }

    void WorldEntity::createPhysicalBody()
    {
        // Note: The motion state will be configured in a derived class.
        btRigidBody::btRigidBodyConstructionInfo bodyConstructionInfo(0, this, 0, btVector3(0,0,0));
        this->physicalBody_ = new btRigidBody(bodyConstructionInfo);
    }

    float WorldEntity::getMass()
    {
        if (!this->physicalBody_)
            return 0.0f;

        return 1.0f/this->physicalBody_->getInvMass();
    }

    void WorldEntity::setMass(float mass)
    {
        if (!this->physicalBody_)
            this->createPhysicalBody();

        this->physicalBody_->setMassProps(mass, btVector3(0,0,0));
    }

    void WorldEntity::setCollisionType(WorldEntity::CollisionType type)
    {
        if (!this->physicalBody_)
            this->createPhysicalBody();

        switch (type)
        {
        case Dynamic:
            this->physicalBody_->setCollisionFlags(this->physicalBody_->getCollisionFlags() & !(btCollisionObject::CF_STATIC_OBJECT | btCollisionObject::CF_KINEMATIC_OBJECT));
            break;
        case Kinematic:
            this->physicalBody_->setCollisionFlags(this->physicalBody_->getCollisionFlags() & !btCollisionObject::CF_STATIC_OBJECT | btCollisionObject::CF_KINEMATIC_OBJECT);
            break;
        case Static:
            this->physicalBody_->setCollisionFlags(this->physicalBody_->getCollisionFlags() & !btCollisionObject::CF_KINEMATIC_OBJECT | btCollisionObject::CF_STATIC_OBJECT);
            break;
        }
    }

    WorldEntity::CollisionType WorldEntity::getCollisionType()
    {
        if (!this->physicalBody_)
            ThrowException(Argument, "Cannot retrieve collision type of a non physical object.");

        int flags = this->physicalBody_->getCollisionFlags();
        if (flags & btCollisionObject::CF_STATIC_OBJECT)
            return Static;
        else if (flags & btCollisionObject::CF_KINEMATIC_OBJECT)
            return Kinematic;
        else
            return Dynamic;
    }

    void WorldEntity::setCollisionTypeStr(const std::string& type)
    {
        std::string lower = getLowercase(type);
        if (lower == "dynamic")
            setCollisionType(Dynamic);
        else if (lower == "static")
            setCollisionType(Static);
        else if (lower == "kinematic")
            setCollisionType(Kinematic);
        else
            ThrowException(Argument, std::string("Trying to set an unknown collision type: '") + type + "'.");
    }

    std::string WorldEntity::getCollisionTypeStr()
    {
        switch (this->getCollisionType())
        {
        case Dynamic:
            return "dynamic";
        case Kinematic:
            return "kinematic";
        case Static:
            return "static";
        default:
            ThrowException(Argument, "Encountered unknown collision Type.");
        }
    }

    void WorldEntity::setCollisionRadius(float radius)
    {
        if (!this->physicalBody_)
            createPhysicalBody();

        // destroy old one first
        btCollisionShape* oldShape = this->physicalBody_->getCollisionShape();
        if (oldShape)
            delete oldShape;

        this->physicalBody_->setCollisionShape(new btSphereShape(btScalar(radius)));
    }

    float WorldEntity::getCollisionRadius()
    {
        if (this->physicalBody_)
        {
            btSphereShape* sphere = dynamic_cast<btSphereShape*>(this->physicalBody_->getCollisionShape());
            if (sphere)
                return (float)sphere->getRadius();
        }
        return 0.0f;
    }
}

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
 *      Reto Grieder (physics)
 *   Co-authors:
 *      ...
 *
 */

#include "WorldEntity.h"

#include <OgreBillboardSet.h>
#include <OgreCamera.h>
#include <OgreEntity.h>
#include <OgreParticleSystem.h>
#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <BulletDynamics/Dynamics/btRigidBody.h>
#include <boost/static_assert.hpp>

#include "util/OrxAssert.h"
#include "util/Convert.h"
#include "util/Exception.h"
#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "objects/Scene.h"
#include "objects/collisionshapes/WorldEntityCollisionShape.h"

namespace orxonox
{
    const Vector3 WorldEntity::FRONT = Vector3::NEGATIVE_UNIT_Z;
    const Vector3 WorldEntity::BACK  = Vector3::UNIT_Z;
    const Vector3 WorldEntity::LEFT  = Vector3::NEGATIVE_UNIT_X;
    const Vector3 WorldEntity::RIGHT = Vector3::UNIT_X;
    const Vector3 WorldEntity::DOWN  = Vector3::NEGATIVE_UNIT_Y;
    const Vector3 WorldEntity::UP    = Vector3::UNIT_Y;

    // Be sure we don't do bad conversions
    BOOST_STATIC_ASSERT((int)Ogre::Node::TS_LOCAL  == (int)WorldEntity::Local);
    BOOST_STATIC_ASSERT((int)Ogre::Node::TS_PARENT == (int)WorldEntity::Parent);
    BOOST_STATIC_ASSERT((int)Ogre::Node::TS_WORLD  == (int)WorldEntity::World);

    /**
    @brief
        Creates a new WorldEntity that may immediately be used.
        All the default values are being set here.
    */
    WorldEntity::WorldEntity(BaseObject* creator) : BaseObject(creator), Synchronisable(creator)
    {
        RegisterObject(WorldEntity);

        if (!this->getScene() || !this->getScene()->getRootSceneNode())
            ThrowException(AbortLoading, "Can't create WorldEntity, no scene or no root-scenenode given.");

        this->node_ = this->getScene()->getRootSceneNode()->createChildSceneNode();

        this->parent_ = 0;
        this->parentID_ = OBJECTID_UNKNOWN;
        this->bDeleteWithParent_ = true;

        this->node_->setPosition(Vector3::ZERO);
        this->node_->setOrientation(Quaternion::IDENTITY);


        // Default behaviour does not include physics
        this->physicalBody_   = 0;
        this->bPhysicsActive_ = false;
        this->bPhysicsActiveSynchronised_    = false;
        this->bPhysicsActiveBeforeAttaching_ = false;
        this->collisionShape_ = new WorldEntityCollisionShape(this);
        this->collisionType_             = None;
        this->collisionTypeSynchronised_ = None;
        this->mass_           = 0;
        this->childrenMass_   = 0;
        // Using bullet default values
        this->restitution_    = 0;
        this->angularFactor_  = 1;
        this->linearDamping_  = 0;
        this->angularDamping_ = 0;
        this->friction_       = 0.5;
        this->bCollisionCallbackActive_ = false;
        this->bCollisionResponseActive_ = true;

        this->registerVariables();
    }

    /**
    @brief
        Destroys the WorldEntity AND ALL its children with it.
    */
    WorldEntity::~WorldEntity()
    {
        if (this->isInitialized())
        {
            if (this->parent_)
                this->detachFromParent();

            for (std::set<WorldEntity*>::const_iterator it = this->children_.begin(); it != this->children_.end(); )
            {
                if ((*it)->getDeleteWithParent())
                    delete (*(it++));
                else
                {
                    (*it)->setPosition(this->getWorldPosition());
                    this->detach(*(it++));
                }
            }

            if (this->physicalBody_)
            {
                this->deactivatePhysics();
                delete this->physicalBody_;
            }
            delete this->collisionShape_;

            this->node_->detachAllObjects();
            this->node_->removeAllChildren();

            OrxAssert(this->getScene()->getSceneManager(), "No SceneManager defined in a WorldEntity.");
            this->getScene()->getSceneManager()->destroySceneNode(this->node_->getName());
        }
    }

    void WorldEntity::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(WorldEntity, XMLPort, xmlelement, mode);

        XMLPortParamTemplate(WorldEntity, "position",    setPosition,    getPosition,    xmlelement, mode, const Vector3&);
        XMLPortParamTemplate(WorldEntity, "orientation", setOrientation, getOrientation, xmlelement, mode, const Quaternion&);
        XMLPortParamTemplate(WorldEntity, "scale3D",     setScale3D,     getScale3D,     xmlelement, mode, const Vector3&);
        XMLPortParam        (WorldEntity, "scale",       setScale,       getScale,       xmlelement, mode);
        XMLPortParamLoadOnly(WorldEntity, "lookat",      lookAt_xmlport,       xmlelement, mode);
        XMLPortParamLoadOnly(WorldEntity, "direction",   setDirection_xmlport, xmlelement, mode);
        XMLPortParamLoadOnly(WorldEntity, "yaw",         yaw_xmlport,          xmlelement, mode);
        XMLPortParamLoadOnly(WorldEntity, "pitch",       pitch_xmlport,        xmlelement, mode);
        XMLPortParamLoadOnly(WorldEntity, "roll",        roll_xmlport,         xmlelement, mode);
        XMLPortParam        (WorldEntity, "deletewithparent", setDeleteWithParent, getDeleteWithParent, xmlelement, mode);

        // Physics
        XMLPortParam(WorldEntity, "collisionType",     setCollisionTypeStr,  getCollisionTypeStr,  xmlelement, mode);
        XMLPortParam(WorldEntity, "collisionResponse", setCollisionResponse, hasCollisionResponse, xmlelement, mode);
        XMLPortParam(WorldEntity, "mass",              setMass,              getMass,              xmlelement, mode);
        XMLPortParam(WorldEntity, "restitution",       setRestitution,       getRestitution,       xmlelement, mode);
        XMLPortParam(WorldEntity, "angularFactor",     setAngularFactor,     getAngularFactor,     xmlelement, mode);
        XMLPortParam(WorldEntity, "linearDamping",     setLinearDamping,     getLinearDamping,     xmlelement, mode);
        XMLPortParam(WorldEntity, "angularDamping",    setAngularDamping,    getAngularDamping,    xmlelement, mode);
        XMLPortParam(WorldEntity, "friction",          setFriction,          getFriction,          xmlelement, mode);

        // Other attached WorldEntities
        XMLPortObject(WorldEntity, WorldEntity, "attached", attach, getAttachedObject, xmlelement, mode);
        // Attached collision shapes
        XMLPortObject(WorldEntity, CollisionShape, "collisionShapes", attachCollisionShape, getAttachedCollisionShape, xmlelement, mode);
    }

    void WorldEntity::registerVariables()
    {
        registerVariable(this->mainStateName_,  variableDirection::toclient, new NetworkCallback<WorldEntity>(this, &WorldEntity::changedMainState));

        registerVariable(this->bActive_,        variableDirection::toclient, new NetworkCallback<WorldEntity>(this, &WorldEntity::changedActivity));
        registerVariable(this->bVisible_,       variableDirection::toclient, new NetworkCallback<WorldEntity>(this, &WorldEntity::changedVisibility));

        registerVariable(this->getScale3D(),    variableDirection::toclient, new NetworkCallback<WorldEntity>(this, &WorldEntity::scaleChanged));

        // Physics stuff
        registerVariable(this->mass_,           variableDirection::toclient, new NetworkCallback<WorldEntity>(this, &WorldEntity::massChanged));
        registerVariable(this->restitution_,    variableDirection::toclient, new NetworkCallback<WorldEntity>(this, &WorldEntity::restitutionChanged));
        registerVariable(this->angularFactor_,  variableDirection::toclient, new NetworkCallback<WorldEntity>(this, &WorldEntity::angularFactorChanged));
        registerVariable(this->linearDamping_,  variableDirection::toclient, new NetworkCallback<WorldEntity>(this, &WorldEntity::linearDampingChanged));
        registerVariable(this->angularDamping_, variableDirection::toclient, new NetworkCallback<WorldEntity>(this, &WorldEntity::angularDampingChanged));
        registerVariable(this->friction_,       variableDirection::toclient, new NetworkCallback<WorldEntity>(this, &WorldEntity::frictionChanged));
        registerVariable(this->bCollisionCallbackActive_,
                                                variableDirection::toclient, new NetworkCallback<WorldEntity>(this, &WorldEntity::collisionCallbackActivityChanged));
        registerVariable(this->bCollisionResponseActive_,
                                                variableDirection::toclient, new NetworkCallback<WorldEntity>(this, &WorldEntity::collisionResponseActivityChanged));
        registerVariable((int&)this->collisionTypeSynchronised_,
                                                variableDirection::toclient, new NetworkCallback<WorldEntity>(this, &WorldEntity::collisionTypeChanged));
        registerVariable(this->bPhysicsActiveSynchronised_,
                                                variableDirection::toclient, new NetworkCallback<WorldEntity>(this, &WorldEntity::physicsActivityChanged));

        // Attach to parent if necessary
        registerVariable(this->parentID_,       variableDirection::toclient, new NetworkCallback<WorldEntity>(this, &WorldEntity::networkcallback_parentChanged));
    }

    /**
    @brief
        Network function that object this instance to its correct parent.
    */
    void WorldEntity::networkcallback_parentChanged()
    {
        if (this->parentID_ != OBJECTID_UNKNOWN)
        {
            WorldEntity* parent = dynamic_cast<WorldEntity*>(Synchronisable::getSynchronisable(this->parentID_));
            if (parent)
                this->attachToParent(parent);
        }
    }

    /**
    @brief
        Attaches this object to a parent SceneNode.
    @Remarks
        Only use this method if you know exactly what you're doing!
        Normally, attaching works internally by attaching WE's.
    */
    void WorldEntity::attachToNode(Ogre::SceneNode* node)
    {
        Ogre::Node* parent = this->node_->getParent();
        if (parent)
            parent->removeChild(this->node_);
        node->addChild(this->node_);
    }

    /**
    @brief
        Detaches this object from a parent SceneNode.
    @Remarks
        Only use this method if you know exactly what you're doing!
        Normally, attaching works internally by attaching WE's.
    */
    void WorldEntity::detachFromNode(Ogre::SceneNode* node)
    {
        node->removeChild(this->node_);
//        this->getScene()->getRootSceneNode()->addChild(this->node_);
    }

    /**
    @brief
        Network callback for the collision type. Only change the type if it was valid.
    */
    void WorldEntity::collisionTypeChanged()
    {
        if (this->collisionTypeSynchronised_ != Dynamic &&
            this->collisionTypeSynchronised_ != Kinematic &&
            this->collisionTypeSynchronised_ != Static &&
            this->collisionTypeSynchronised_ != None)
        {
            CCOUT(1) << "Error when collsion Type was received over network. Unknown enum value:" << this->collisionTypeSynchronised_ << std::endl;
        }
        else if (this->collisionTypeSynchronised_ != collisionType_)
        {
            if (this->parent_)
                CCOUT(2) << "Warning: Network connection tried to set the collision type of an attached WE. Ignoring." << std::endl;
            else
                this->setCollisionType(this->collisionTypeSynchronised_);
        }
    }

    //! Network callback for this->bPhysicsActive_
    void WorldEntity::physicsActivityChanged()
    {
        if (this->bPhysicsActiveSynchronised_)
            this->activatePhysics();
        else
            this->deactivatePhysics();
    }

    //! Function sets whether Bullet should issue a callback on collisions
    void WorldEntity::collisionCallbackActivityChanged()
    {
        if (this->hasPhysics())
        {
            if (this->bCollisionCallbackActive_)
                this->physicalBody_->setCollisionFlags(this->physicalBody_->getCollisionFlags() |
                    btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
            else
                this->physicalBody_->setCollisionFlags(this->physicalBody_->getCollisionFlags() &
                    ~btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
        }
    }

    //! Function sets whether Bullet should react itself to a collision
    void WorldEntity::collisionResponseActivityChanged()
    {
        if (this->hasPhysics())
        {
            if (this->bCollisionResponseActive_)
                this->physicalBody_->setCollisionFlags(this->physicalBody_->getCollisionFlags() &
                    ~btCollisionObject::CF_NO_CONTACT_RESPONSE);
            else
                this->physicalBody_->setCollisionFlags(this->physicalBody_->getCollisionFlags() |
                    btCollisionObject::CF_NO_CONTACT_RESPONSE);
        }
    }

    /**
    @brief
        Attaches a child WorldEntity to this object. This calls notifyBeingAttached()
        of the child WE.
    @Note
        The collision shape of the child object gets attached nevertheless. That also means
        that you can change the collision shape of the child and it correctly cascadeds the changes to this instance.
        Be aware of this implication: When implementing attaching of kinematic objects to others, you have to change
        this behaviour because you then might not want to merge the collision shapes.
    */
    void WorldEntity::attach(WorldEntity* object)
    {
        if (object == this)
        {
            COUT(2) << "Warning: Can't attach a WorldEntity to itself." << std::endl;
            return;
        }

        if (!object->notifyBeingAttached(this))
            return;

        this->attachNode(object->node_);
        this->children_.insert(object);

        this->attachCollisionShape(object->collisionShape_);
        // mass
        this->childrenMass_ += object->getMass();
        recalculateMassProps();
    }

    /**
    @brief
        Function gets called when this object is being attached to a new parent.

        This operation is only allowed if the collision types "like" each other.
        - You cannot a attach a non physical object to a physical one.
        - Dynamic object can NOT be attached at all.
        - It is also not possible to attach a kinematic to a dynamic one.
        - Attaching of kinematic objects otherwise is not yet supported.
    */
    bool WorldEntity::notifyBeingAttached(WorldEntity* newParent)
    {
        // check first whether attaching is even allowed
        if (this->hasPhysics())
        {
            if (!newParent->hasPhysics())
            {
                COUT(2) << "Warning: Cannot attach a physical object to a non physical one." << std::endl;
                return false;
            }
            else if (this->isDynamic())
            {
                COUT(2) << "Warning: Cannot attach a dynamic object to a WorldEntity." << std::endl;
                return false;
            }
            else if (this->isKinematic() && newParent->isDynamic())
            {
                COUT(2) << "Warning: Cannot attach a kinematic object to a dynamic one." << std::endl;
                return false;
            }
            else if (this->isKinematic())
            {
                COUT(2) << "Warning: Cannot attach a kinematic object to a static or kinematic one: Not yet implemented." << std::endl;
                return false;
            }
        }

        if (this->isPhysicsActive())
            this->bPhysicsActiveBeforeAttaching_ = true;
        this->deactivatePhysics();

        if (this->parent_)
            this->detachFromParent();

        this->parent_ = newParent;
        this->parentID_ = newParent->getObjectID();

        this->parentChanged();

        // apply transform to collision shape
        this->collisionShape_->setPosition(this->getPosition());
        this->collisionShape_->setOrientation(this->getOrientation());
        // TODO: Scale

        return true;
    }

    /**
    @brief
        Detaches a child WorldEntity from this instance.
    */
    void WorldEntity::detach(WorldEntity* object)
    {
        if (this->children_.find(object) == this->children_.end())
        {
            CCOUT(2) << "Warning: Cannot detach an object that is not a child." << std::endl;
            return;
        }

        // collision shapes
        this->detachCollisionShape(object->collisionShape_);

        // mass
        if (object->getMass() > 0.0f)
        {
            this->childrenMass_ -= object->getMass();
            recalculateMassProps();
        }

        this->detachNode(object->node_);
        this->children_.erase(object);

        object->notifyDetached();
    }

    /**
    @brief
        Function gets called when the object has been detached from its parent.
    */
    void WorldEntity::notifyDetached()
    {
        this->parent_ = 0;
        this->parentID_ = OBJECTID_UNKNOWN;

        this->parentChanged();

        // reset orientation of the collisionShape (cannot be set within a WE usually)
        this->collisionShape_->setPosition(Vector3::ZERO);
        this->collisionShape_->setOrientation(Quaternion::IDENTITY);
        // TODO: Scale

        if (this->bPhysicsActiveBeforeAttaching_)
        {
            this->activatePhysics();
            this->bPhysicsActiveBeforeAttaching_ = false;
        }
    }

    //! Returns an attached object (merely for XMLPort).
    WorldEntity* WorldEntity::getAttachedObject(unsigned int index)
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

    //! Attaches an Ogre::SceneNode to this WorldEntity.
    void WorldEntity::attachNode(Ogre::SceneNode* node)
    {
        Ogre::Node* parent = node->getParent();
        if (parent)
            parent->removeChild(node);
        this->node_->addChild(node);
    }

    //! Detaches an Ogre::SceneNode from this WorldEntity.
    void WorldEntity::detachNode(Ogre::SceneNode* node)
    {
        this->node_->removeChild(node);
//        this->getScene()->getRootSceneNode()->addChild(node);
    }

    //! Attaches an Ogre::MovableObject to this WorldEntity.
    void WorldEntity::attachOgreObject(Ogre::MovableObject* object)
        { this->node_->attachObject(object); }
    void WorldEntity::attachOgreObject(Ogre::BillboardSet* object)
        { this->node_->attachObject(object); }
    void WorldEntity::attachOgreObject(Ogre::Camera* object)
        { this->node_->attachObject(object); }
    void WorldEntity::attachOgreObject(Ogre::Entity* object)
        { this->node_->attachObject(object); }
    void WorldEntity::attachOgreObject(Ogre::ParticleSystem* object)
        { this->node_->attachObject(object); }

    //! Detaches an Ogre::MovableObject from this WorldEntity.
    void WorldEntity::detachOgreObject(Ogre::MovableObject* object)
        { this->node_->detachObject(object); }
    void WorldEntity::detachOgreObject(Ogre::BillboardSet* object)
        { this->node_->detachObject(object); }
    void WorldEntity::detachOgreObject(Ogre::Camera* object)
        { this->node_->detachObject(object); }
    void WorldEntity::detachOgreObject(Ogre::Entity* object)
        { this->node_->detachObject(object); }
    void WorldEntity::detachOgreObject(Ogre::ParticleSystem* object)
        { this->node_->detachObject(object); }

    //! Detaches an Ogre::MovableObject (by string) from this WorldEntity.
    Ogre::MovableObject* WorldEntity::detachOgreObject(const Ogre::String& name)
    {
        return this->node_->detachObject(name);
    }

    //! Attaches a collision Shape to this object (delegated to the internal CompoundCollisionShape)
    void WorldEntity::attachCollisionShape(CollisionShape* shape)
    {
        this->collisionShape_->attach(shape);
        // Note: this->collisionShape_ already notifies us of any changes.
    }

    //! Detaches a collision Shape from this object (delegated to the internal CompoundCollisionShape)
    void WorldEntity::detachCollisionShape(CollisionShape* shape)
    {
        // Note: The collision shapes may not be detached with this function!
        this->collisionShape_->detach(shape);
        // Note: this->collisionShape_ already notifies us of any changes.
    }

    //! Returns an attached collision Shape of this object (delegated to the internal CompoundCollisionShape)
    CollisionShape* WorldEntity::getAttachedCollisionShape(unsigned int index)
    {
        return this->collisionShape_->getAttachedShape(index);
    }

    // Note: These functions are placed in WorldEntity.h as inline functions for the release build.
#ifndef ORXONOX_RELEASE
    const Vector3& WorldEntity::getPosition() const
    {
        return this->node_->getPosition();
    }

    const Quaternion& WorldEntity::getOrientation() const
    {
        return this->node_->getOrientation();
    }

    const Vector3& WorldEntity::getScale3D() const
    {
        return this->node_->getScale();
    }
#endif

    //! Returns the position relative to the root space
    const Vector3& WorldEntity::getWorldPosition() const
    {
        return this->node_->_getDerivedPosition();
    }

    //! Returns the orientation relative to the root space
    const Quaternion& WorldEntity::getWorldOrientation() const
    {
        return this->node_->_getDerivedOrientation();
    }

    //! Returns the scaling applied relative to the root space in 3 coordinates
    const Vector3& WorldEntity::getWorldScale3D() const
    {
        return this->node_->_getDerivedScale();
    }

    /**
    @brief
        Returns the scaling applied relative to the root space in 3 coordinates
    @return
        Returns the scaling if it is uniform, 1.0f otherwise.
    */
    float WorldEntity::getWorldScale() const
    {
        Vector3 scale = this->getWorldScale3D();
        return (scale.x == scale.y && scale.x == scale.z) ? scale.x : 1;
    }

    /**
    @brief
        Sets the three dimensional scaling of this object.
    @Note
        Scaling physical objects has not yet been implemented and is therefore forbidden.
    */
    void WorldEntity::setScale3D(const Vector3& scale)
    {
/*
HACK HACK HACK
        if (bScalePhysics && this->hasPhysics() && scale != Vector3::UNIT_SCALE)
        {
            CCOUT(2) << "Warning: Cannot set the scale of a physical object: Not yet implemented. Ignoring scaling." << std::endl;
            return;
        }
HACK HACK HACK
*/
        this->node_->setScale(scale);

        this->changedScale();
    }

    /**
    @brief
        Translates this WorldEntity by a vector.
    @param relativeTo
        @see WorldEntity::TransformSpace
    */
    void WorldEntity::translate(const Vector3& distance, TransformSpace relativeTo)
    {
        switch (relativeTo)
        {
        case WorldEntity::Local:
            // position is relative to parent so transform downwards
            this->setPosition(this->getPosition() + this->getOrientation() * distance);
            break;
        case WorldEntity::Parent:
            this->setPosition(this->getPosition() + distance);
            break;
        case WorldEntity::World:
            // position is relative to parent so transform upwards
            if (this->node_->getParent())
                setPosition(getPosition() + (node_->getParent()->_getDerivedOrientation().Inverse() * distance)
                    / node_->getParent()->_getDerivedScale());
            else
                this->setPosition(this->getPosition() + distance);
            break;
        }
    }

    /**
    @brief
        Rotates this WorldEntity by a quaternion.
    @param relativeTo
        @see WorldEntity::TransformSpace
    */
    void WorldEntity::rotate(const Quaternion& rotation, TransformSpace relativeTo)
    {
        switch(relativeTo)
        {
        case WorldEntity::Local:
            this->setOrientation(this->getOrientation() * rotation);
            break;
        case WorldEntity::Parent:
            // Rotations are normally relative to local axes, transform up
            this->setOrientation(rotation * this->getOrientation());
            break;
        case WorldEntity::World:
            // Rotations are normally relative to local axes, transform up
            this->setOrientation(this->getOrientation() * this->getWorldOrientation().Inverse()
                * rotation * this->getWorldOrientation());
            break;
        }
    }

    /**
    @brief
        Makes this WorldEntity look a specific target location.
    @param relativeTo
        @see WorldEntity::TransformSpace
    @param localDirectionVector
        The vector which normally describes the natural direction of the object, usually -Z.
    */
    void WorldEntity::lookAt(const Vector3& target, TransformSpace relativeTo, const Vector3& localDirectionVector)
    {
        Vector3 origin(0, 0, 0);
        switch (relativeTo)
        {
        case WorldEntity::Local:
            origin = Vector3::ZERO;
            break;
        case WorldEntity::Parent:
            origin = this->getPosition();
            break;
        case WorldEntity::World:
            origin = this->getWorldPosition();
            break;
        }
        this->setDirection(target - origin, relativeTo, localDirectionVector);
    }

    /**
    @brief
        Makes this WorldEntity look in specific direction.
    @param relativeTo
        @see WorldEntity::TransformSpace
    @param localDirectionVector
        The vector which normally describes the natural direction of the object, usually -Z.
    */
    void WorldEntity::setDirection(const Vector3& direction, TransformSpace relativeTo, const Vector3& localDirectionVector)
    {
        Quaternion savedOrientation(this->getOrientation());
        this->node_->setDirection(direction, static_cast<Ogre::Node::TransformSpace>(relativeTo), localDirectionVector);
        Quaternion newOrientation(this->node_->getOrientation());
        this->node_->setOrientation(savedOrientation);
        this->setOrientation(newOrientation);
    }

    //! Activates physics if the CollisionType is not None.
    void WorldEntity::activatePhysics()
    {
        if (this->isActive() && this->hasPhysics() && !this->isPhysicsActive() && !this->parent_)
        {
            this->getScene()->addPhysicalObject(this);
            this->bPhysicsActive_ = true;
            this->bPhysicsActiveSynchronised_ = true;
        }
    }

    //! Deactivates physics but the CollisionType does not change.
    void WorldEntity::deactivatePhysics()
    {
        if (this->isPhysicsActive())
        {
            this->getScene()->removePhysicalObject(this);
            this->bPhysicsActive_ = false;
            this->bPhysicsActiveSynchronised_ = false;
        }
    }

    //! Tells whether the object has already been added to the Bullet physics World.
    bool WorldEntity::addedToPhysicalWorld() const
    {
        return this->physicalBody_ && this->physicalBody_->isInWorld();
    }

    /**
    @brief
        Sets the CollisionType. This alters the object significantly! @see CollisionType.
    @Note
        Operation does not work on attached WorldEntities.
    */
    void WorldEntity::setCollisionType(CollisionType type)
    {
        if (this->collisionType_ == type)
            return;

        // If we are already attached to a parent, this would be a bad idea..
        if (this->parent_)
        {
            CCOUT(2) << "Warning: Cannot set the collision type of a WorldEntity with a parent." << std::endl;
            return;
        }

        // Check for type legality. Could be StaticEntity or MobileEntity.
        if (!this->isCollisionTypeLegal(type))
            return;

        if (this->isPhysicsActive())
            this->deactivatePhysics();

        bool bReactivatePhysics = true;
        if (this->hasPhysics() && !this->isPhysicsActive())
            bReactivatePhysics = false;

        // Check whether we have to create or destroy.
        if (type != None && this->collisionType_ == None)
        {
/*
HACK HACK HACK
            // Check whether there was some scaling applied.
            if (!this->node_->getScale().positionEquals(Vector3(1, 1, 1), 0.001))
            {
                CCOUT(2) << "Warning: Cannot create a physical body if there is scaling applied to the node: Not yet implemented." << std::endl;
                return;
            }
HACK HACK HACK
*/
            // Create new rigid body
            btRigidBody::btRigidBodyConstructionInfo bodyConstructionInfo(0, this, this->collisionShape_->getCollisionShape());
            this->physicalBody_ = new btRigidBody(bodyConstructionInfo);
            this->physicalBody_->setUserPointer(this);
            this->physicalBody_->setActivationState(DISABLE_DEACTIVATION);
        }
        else if (type == None && this->collisionType_ != None)
        {
            // Destroy rigid body
            assert(this->physicalBody_);
            deactivatePhysics();
            delete this->physicalBody_;
            this->physicalBody_ = 0;
            this->collisionType_ = None;
            this->collisionTypeSynchronised_ = None;
            return;
        }

        // Change type
        switch (type)
        {
        case Dynamic:
            this->physicalBody_->setCollisionFlags(this->physicalBody_->getCollisionFlags() & !btCollisionObject::CF_STATIC_OBJECT & !btCollisionObject::CF_KINEMATIC_OBJECT);
            break;
        case Kinematic:
            this->physicalBody_->setCollisionFlags((this->physicalBody_->getCollisionFlags() & !btCollisionObject::CF_STATIC_OBJECT) | btCollisionObject::CF_KINEMATIC_OBJECT);
            break;
        case Static:
            this->physicalBody_->setCollisionFlags((this->physicalBody_->getCollisionFlags() & !btCollisionObject::CF_KINEMATIC_OBJECT) | btCollisionObject::CF_STATIC_OBJECT);
            break;
        case None:
            assert(false); // Doesn't happen
            return;
        }
        this->collisionType_ = type;
        this->collisionTypeSynchronised_ = type;

        // update mass and inertia tensor
        recalculateMassProps();
        internalSetPhysicsProps();
        collisionCallbackActivityChanged();
        collisionResponseActivityChanged();
        if (bReactivatePhysics)
            activatePhysics();
    }

    //! Sets the CollisionType by string (used for the XMLPort)
    void WorldEntity::setCollisionTypeStr(const std::string& typeStr)
    {
        std::string typeStrLower = getLowercase(typeStr);
        CollisionType type;
        if (typeStrLower == "dynamic")
            type = Dynamic;
        else if (typeStrLower == "static")
            type = Static;
        else if (typeStrLower == "kinematic")
            type = Kinematic;
        else if (typeStrLower == "none")
            type = None;
        else
            ThrowException(ParseError, std::string("Attempting to set an unknown collision type: '") + typeStr + "'.");
        this->setCollisionType(type);
    }

    //! Gets the CollisionType by string (used for the XMLPort)
    std::string WorldEntity::getCollisionTypeStr() const
    {
        switch (this->getCollisionType())
        {
            case Dynamic:
                return "dynamic";
            case Kinematic:
                return "kinematic";
            case Static:
                return "static";
            case None:
                return "none";
            default:
                assert(false);
                return "";
        }
    }

    /**
    @brief
        Recalculates the accumulated child mass and calls recalculateMassProps()
        and notifies the parent of the change.
    @Note
        Called by a child WE
    */
    void WorldEntity::notifyChildMassChanged()
    {
        // Note: CollisionShape changes of a child get handled over the internal CompoundCollisionShape already
        // Recalculate mass
        this->childrenMass_ = 0.0f;
        for (std::set<WorldEntity*>::const_iterator it = this->children_.begin(); it != this->children_.end(); ++it)
            this->childrenMass_ += (*it)->getMass();
        recalculateMassProps();
        // Notify parent WE
        if (this->parent_)
            parent_->notifyChildMassChanged();
    }

    /**
    @brief
        Undertakes the necessary steps to change the collision shape in Bullet, even at runtime.
    @Note
        - called by this->collisionShape_
        - May have a REALLY big overhead when called continuously at runtime, because then we need
          to remove the physical body from Bullet and add it again.
    */
    void WorldEntity::notifyCollisionShapeChanged()
    {
        if (hasPhysics())
        {
            // Bullet doesn't like sudden changes of the collision shape, so we remove and add it again
            if (this->addedToPhysicalWorld())
            {
                this->deactivatePhysics();
                this->physicalBody_->setCollisionShape(this->collisionShape_->getCollisionShape());
                this->activatePhysics();
            }
            else
                this->physicalBody_->setCollisionShape(this->collisionShape_->getCollisionShape());
        }
        recalculateMassProps();
    }

    //! Updates all mass dependent parameters (mass, inertia tensor and child mass)
    void WorldEntity::recalculateMassProps()
    {
        // Store local inertia for faster access. Evaluates to (0,0,0) if there is no collision shape.
        float totalMass = this->mass_ + this->childrenMass_;
        this->collisionShape_->calculateLocalInertia(totalMass, this->localInertia_);
        if (this->hasPhysics())
        {
            if (this->isStatic())
            {
                // Just set everything to zero
                this->physicalBody_->setMassProps(0.0f, btVector3(0, 0, 0));
            }
            else if ((this->mass_ + this->childrenMass_) == 0.0f)
            {
                // Use default values to avoid very large or very small values
                CCOUT(4) << "Warning: Setting the internal physical mass to 1.0 because mass_ is 0.0" << std::endl;
                btVector3 inertia(0, 0, 0);
                this->collisionShape_->calculateLocalInertia(1.0f, inertia);
                this->physicalBody_->setMassProps(1.0f, inertia);
            }
            else
            {
                this->physicalBody_->setMassProps(totalMass, this->localInertia_);
            }
        }
    }

    //! Copies our own parameters for restitution, angular factor, dampings and friction to the bullet rigid body.
    void WorldEntity::internalSetPhysicsProps()
    {
        if (this->hasPhysics())
        {
            this->physicalBody_->setRestitution(this->restitution_);
            this->physicalBody_->setAngularFactor(this->angularFactor_);
            this->physicalBody_->setDamping(this->linearDamping_, this->angularDamping_);
            this->physicalBody_->setFriction(this->friction_);
        }
    }
}

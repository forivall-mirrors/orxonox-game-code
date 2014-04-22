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

#ifndef _WorldEntity_H__
#define _WorldEntity_H__

#include "OrxonoxPrereqs.h"

#ifdef ORXONOX_RELEASE
#  include <OgreSceneNode.h>
#endif
#include <LinearMath/btMotionState.h>

#include "util/Math.h"
#include "util/OgreForwardRefs.h"
#include "core/BaseObject.h"
#include "network/synchronisable/Synchronisable.h"

namespace orxonox
{
    /**
    @brief
        The WorldEntity represents everything that can be put in a Scene at a certain location.

        It is supposed to be the base class of everything you would call an 'object' in a Scene.
        The class itself is abstract which means you cannot use it directly. You may use StaticEntity
        as the simplest derivative or (derived from MobileEntity) MovableEntity and ControllableEntity
        as more advanced ones.

        The basic task of the WorldEntity is provide a location, a direction and a scaling and the possibility
        to create an entire hierarchy of derived objects.
        It is also the basis for the physics interface to the Bullet physics engine.
        Every WorldEntity can have a specific collision type: @see CollisionType
        This would then imply that every scene object could have any collision type. To limit this, you can always
        override this->isCollisionTypeLegal(CollisionType). Return false if the collision type is not supported
        for a specific object.
        There is also support for attaching WorldEntities with physics to each other. Currently, the collision shape
        of both objects simply get merged into one larger shape (for static collision type).
        The physical body that is internally stored and administrated has the following supported properties:
        - Restitution, angular factor, linear damping, angular damping, friction, mass and collision shape.
        You can get more information at the corresponding set function.

        Collision shapes: These are controlled by the internal WorldEntityCollisionShape. @see WorldEntityCollisionShape.
    */
    class _OrxonoxExport WorldEntity : public BaseObject, public Synchronisable, public btMotionState
    {
        friend class Scene;

        public:
            // Define our own transform space enum to avoid Ogre includes here
            /**
            @brief
                Enumeration denoting the spaces which a transform can be relative to.
            */
            enum TransformSpace
            {
                //! Transform is relative to the local space
                Local,
                //! Transform is relative to the space of the parent node
                Parent,
                //! Transform is relative to world space
                World
            };

        public:
            WorldEntity(Context* context);
            virtual ~WorldEntity();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);

            inline const Ogre::SceneNode* getNode() const
                { return this->node_; }

            static const Vector3 FRONT;
            static const Vector3 BACK;
            static const Vector3 LEFT;
            static const Vector3 RIGHT;
            static const Vector3 DOWN;
            static const Vector3 UP;

            virtual void changedActivity(void);
            virtual void changedVisibility(void);

            virtual void setPosition(const Vector3& position) = 0;
            inline void setPosition(float x, float y, float z)
                { this->setPosition(Vector3(x, y, z)); }
            const Vector3& getPosition() const;
            const Vector3& getWorldPosition() const;

            void translate(const Vector3& distance, TransformSpace relativeTo = WorldEntity::Parent);
            inline void translate(float x, float y, float z, TransformSpace relativeTo = WorldEntity::Parent)
                { this->translate(Vector3(x, y, z), relativeTo); }

            virtual inline const Vector3& getVelocity() const
                { return Vector3::ZERO; }

            virtual void setOrientation(const Quaternion& orientation) = 0;
            inline void setOrientation(float w, float x, float y, float z)
                { this->setOrientation(Quaternion(w, x, y, z)); }
            inline void setOrientation(const Vector3& axis, const Radian& angle)
                { this->setOrientation(Quaternion(angle, axis)); }
            inline void setOrientation(const Vector3& axis, const Degree& angle)
                { this->setOrientation(Quaternion(angle, axis)); }
            const Quaternion& getOrientation() const;
            const Quaternion& getWorldOrientation() const;

            void rotate(const Quaternion& rotation, TransformSpace relativeTo = WorldEntity::Local);
            inline void rotate(const Vector3& axis, const Degree& angle, TransformSpace relativeTo = WorldEntity::Local)
                { this->rotate(Quaternion(angle, axis), relativeTo); }

            inline void yaw(const Degree& angle, TransformSpace relativeTo = WorldEntity::Local)
                { this->rotate(Quaternion(angle, Vector3::UNIT_Y), relativeTo); }
            inline void pitch(const Degree& angle, TransformSpace relativeTo = WorldEntity::Local)
                { this->rotate(Quaternion(angle, Vector3::UNIT_X), relativeTo); }
            inline void roll(const Degree& angle, TransformSpace relativeTo = WorldEntity::Local)
                { this->rotate(Quaternion(angle, Vector3::UNIT_Z), relativeTo); }

            void lookAt(const Vector3& target, TransformSpace relativeTo = WorldEntity::Parent, const Vector3& localDirectionVector = Vector3::NEGATIVE_UNIT_Z);
            void setDirection(const Vector3& direction, TransformSpace relativeTo = WorldEntity::Local, const Vector3& localDirectionVector = Vector3::NEGATIVE_UNIT_Z);
            inline void setDirection(float x, float y, float z, TransformSpace relativeTo = WorldEntity::Local, const Vector3& localDirectionVector = Vector3::NEGATIVE_UNIT_Z)
                { this->setDirection(Vector3(x, y, z), relativeTo, localDirectionVector); }

            virtual void setScale3D(const Vector3& scale);
            inline void setScale3D(float x, float y, float z)
                { this->setScale3D(Vector3(x, y, z)); }
            const Vector3& getScale3D() const;
            const Vector3& getWorldScale3D() const;

            inline void setScale(float scale)
                { this->setScale3D(scale, scale, scale); }
            inline float getScale() const
                { Vector3 scale = this->getScale3D(); return (scale.x == scale.y && scale.x == scale.z) ? scale.x : 1; }
            float getWorldScale() const;

            inline void scale3D(const Vector3& scale)
                { this->setScale3D(this->getScale3D() * scale); }
            inline void scale3D(float x, float y, float z)
                { this->scale3D(Vector3(x, y, z)); }
            inline void scale(float scale)

                { this->scale3D(scale, scale, scale); }
            virtual void changedScale() {}

            void attach(WorldEntity* object);
            void detach(WorldEntity* object);
            WorldEntity* getAttachedObject(unsigned int index);
            inline const std::set<WorldEntity*>& getAttachedObjects() const
                { return this->children_; }

            void attachOgreObject(Ogre::MovableObject*  object);
            void attachOgreObject(Ogre::BillboardSet*   object);
            void attachOgreObject(Ogre::Camera*         object);
            void attachOgreObject(Ogre::Entity*         object);
            void attachOgreObject(Ogre::ParticleSystem* object);

            void detachOgreObject(Ogre::MovableObject*  object);
            void detachOgreObject(Ogre::BillboardSet*   object);
            void detachOgreObject(Ogre::Camera*         object);
            void detachOgreObject(Ogre::Entity*         object);
            void detachOgreObject(Ogre::ParticleSystem* object);

            Ogre::MovableObject* detachOgreObject(const Ogre::String& name);

            inline void attachToParent(WorldEntity* parent)
                { parent->attach(this); }
            inline void detachFromParent()
                { if (this->parent_) { this->parent_->detach(this); } }
            inline WorldEntity* getParent() const
                { return this->parent_; }

            void attachNode(Ogre::SceneNode* node);
            void detachNode(Ogre::SceneNode* node);
            void attachToNode(Ogre::SceneNode* node);
            void detachFromNode(Ogre::SceneNode* node);

            inline void setDeleteWithParent(bool value)
                { this->bDeleteWithParent_ = value; }
            inline bool getDeleteWithParent() const
                { return this->bDeleteWithParent_; }

            void notifyChildPropsChanged();

        protected:
            virtual void parentChanged() {}

            Ogre::SceneNode* node_;

        private:
            void registerVariables();

            inline void lookAt_xmlport(const Vector3& target)
                { this->lookAt(target); }
            inline void setDirection_xmlport(const Vector3& direction)
                { this->setDirection(direction); }
            inline void yaw_xmlport(const Degree& angle)
                { this->yaw(angle); }
            inline void pitch_xmlport(const Degree& angle)
                { this->pitch(angle); }
            inline void roll_xmlport(const Degree& angle)
                { this->roll(angle); }

            // network callbacks
            void networkcallback_parentChanged();
            inline void scaleChanged()
                { this->setScale3D(this->getScale3D()); }

            WorldEntity* parent_;
            unsigned int parentID_;
            std::set<WorldEntity*> children_;
            bool bDeleteWithParent_;

            bool bActiveMem_;
            bool bVisibleMem_;


        /////////////
        // Physics //
        /////////////

        public:
            /**
            @brief
                Denotes the possible types of physical objects in a Scene.

                Dynamic:   The object is influenced by its physical environment, like for instance little ball.
                Kinematic: The object can only influence other dynamic objects. It's movement is coordinated by your own saying.
                Static:    Like kinematic but the object is not allowed to move during the simulation.
                None:      The object has no physics at all.
            */
            enum CollisionType
            {
                Dynamic,
                Kinematic,
                Static,
                None
            };

            //! Tells whether the object has any connection to the Bullet physics engine. If hasPhysics() is false, the object may still have a velocity.
            bool hasPhysics()       const { return getCollisionType() != None     ; }
            //! @see CollisionType
            bool isStatic()         const { return getCollisionType() == Static   ; }
            //! @see CollisionType
            bool isKinematic()      const { return getCollisionType() == Kinematic; }
            //! @see CollisionType
            bool isDynamic()        const { return getCollisionType() == Dynamic  ; }
            //! Tells whether physics has been activated (you can temporarily deactivate it)
            bool isPhysicsActive()  const { return this->bPhysicsActive_; }
            bool addedToPhysicalWorld() const;

            void activatePhysics();
            void deactivatePhysics();

            //! Returns the CollisionType. @see CollisionType.
            inline CollisionType getCollisionType() const
                { return this->collisionType_; }
            void setCollisionType(CollisionType type);

            void setCollisionTypeStr(const std::string& type);
            std::string getCollisionTypeStr() const;

            //! Sets the mass of this object. Note that the total mass may be influenced by attached objects!
            inline void setMass(float mass)
                { this->mass_ = mass; recalculateMassProps(); }
            //! Returns the mass of this object without its children.
            inline float getMass() const
                { return this->mass_; }

            //! Returns the total mass of this object with all its attached children.
            inline float getTotalMass() const
                { return this->mass_ + this->childrenMass_; }

            /**
            @brief
                Returns the diagonal elements of the inertia tensor when calculated in local coordinates.
            @note
                The local inertia tensor cannot be set, but is calculated by Bullet according to the collisionShape.
                With compound collision shapes, an approximation is used.
            */
            inline const btVector3& getLocalInertia() const
                { return this->localInertia_; }

            /**
            @brief
                Sets how much reaction is applied in a collision.

                Consider two equal spheres colliding with equal velocities:
                Restitution 1 means that both spheres simply reverse their velocity (no loss of energy)
                Restitution 0 means that both spheres will immediately stop moving
                (maximum loss of energy without violating of the preservation of momentum)
            */
            inline void setRestitution(float restitution)
                { this->restitution_ = restitution; internalSetPhysicsProps(); }
            //! Returns the restitution parameter. @see setRestitution.
            inline float getRestitution() const
                { return this->restitution_; }

            /**
            @brief
                Sets an artificial parameter that tells how much torque is applied when you apply a non-central force.

                Normally the angular factor is 1, which means it's physically 'correct'. However if you have a player
                character that should not rotate when hit sideways, you can set the angular factor to 0.
            */
            inline void setAngularFactor(float angularFactor)
                { this->angularFactor_ = angularFactor; internalSetPhysicsProps(); }
            //! Returns the angular factor. @see setAngularFactor.
            inline float getAngularFactor() const
                { return this->angularFactor_; }

            //! Applies a mass independent damping. Velocities will simply diminish exponentially.
            inline void setLinearDamping(float linearDamping)
                { this->linearDamping_ = linearDamping; internalSetPhysicsProps(); }
            //! Returns the linear damping. @see setLinearDamping.
            inline float getLinearDamping() const
                { return this->linearDamping_; }

            //! Applies a tensor independent rotation damping. Angular velocities will simply diminish exponentially.
            inline void setAngularDamping(float angularDamping)
                { this->angularDamping_ = angularDamping; internalSetPhysicsProps(); }
            //! Returns the angular damping. @see setAngularDamping.
            inline float getAngularDamping() const
                { return this->angularDamping_; }

            //! Applies friction to the object. Friction occurs when two objects collide.
            inline void setFriction(float friction)
                { this->friction_ = friction; internalSetPhysicsProps(); }
            //! Returns the amount of friction applied to the object.
            inline float getFriction() const
                { return this->friction_; }

            void attachCollisionShape(CollisionShape* shape);
            void detachCollisionShape(CollisionShape* shape);
            CollisionShape* getAttachedCollisionShape(unsigned int index);

            void notifyCollisionShapeChanged();
            void notifyChildMassChanged();

            /**
            @brief
                Virtual function that gets called when this object collides with another.
            @param otherObject
                The object this one has collided into.
            @param contactPoint
                Contact point provided by Bullet. Holds more information and can me modified. See return value.
            @return
                Returning false means that no modification to the contactPoint has been made. Return true otherwise!
            @note
                Condition is that enableCollisionCallback() was called.
            */
            virtual inline bool collidesAgainst(WorldEntity* otherObject, btManifoldPoint& contactPoint)
                { return false; } /* With false, Bullet assumes no modification to the collision objects. */

            //! Enables the collidesAgainst(.) function. The object doesn't respond to collision otherwise!
            inline void enableCollisionCallback()
                { this->bCollisionCallbackActive_ = true; this->collisionCallbackActivityChanged(); }
            //! Disables the collidesAgainst(.) function. @see enableCollisionCallback()
            inline void disableCollisionCallback()
                { this->bCollisionCallbackActive_ = false; this->collisionCallbackActivityChanged(); }
            //! Tells whether there could be a collision callback via collidesAgainst(.)
            inline bool isCollisionCallbackActive() const
                { return this->bCollisionCallbackActive_; }

            //! Enables or disables collision response (default is of course on)
            inline void setCollisionResponse(bool value)
                { this->bCollisionResponseActive_ = value; this->collisionResponseActivityChanged(); }
            //! Tells whether there could be a collision response
            inline bool hasCollisionResponse()
                { return this->bCollisionResponseActive_; }

        protected:
            /**
            @brief
                Function checks whether the requested collision type is legal to this object.

                You can override this function in a derived class to constrain the collision to e.g. None or Dynamic.
                A projectile may not prove very useful if there is no physical body. Simply set the CollisionType
                in its constructor and override this method. But be careful that a derived class's virtual functions
                don't yet exist in the constructor if a base class.
            */
            virtual bool isCollisionTypeLegal(CollisionType type) const = 0;

            btRigidBody*  physicalBody_; //!< Bullet rigid body. Everything physical is applied to this instance.

        private:
            void recalculateMassProps();
            void internalSetPhysicsProps();

            bool notifyBeingAttached(WorldEntity* newParent);
            void notifyDetached();

            // network callbacks
            void collisionTypeChanged();
            void physicsActivityChanged();
            void collisionCallbackActivityChanged();
            void collisionResponseActivityChanged();
            //! Network callback workaround to call a function when the value changes.
            inline void massChanged()
                { this->setMass(this->mass_); }
            //! Network callback workaround to call a function when the value changes.
            inline void restitutionChanged()
                { this->setRestitution(this->restitution_); }
            //! Network callback workaround to call a function when the value changes.
            inline void angularFactorChanged()
                { this->setAngularFactor(this->angularFactor_); }
            //! Network callback workaround to call a function when the value changes.
            inline void linearDampingChanged()
                { this->setLinearDamping(this->linearDamping_); }
            //! Network callback workaround to call a function when the value changes.
            inline void angularDampingChanged()
                { this->setAngularDamping(this->angularDamping_); }
            //! Network callback workaround to call a function when the value changes.
            inline void frictionChanged()
                { this->setFriction(this->friction_); }

            CollisionType                collisionType_;                 //!< @see setCollisionType
            CollisionType                collisionTypeSynchronised_;     //!< Network synchronised variable for collisionType_
            bool                         bPhysicsActive_;                //!< @see isPhysicsActive
            bool                         bPhysicsActiveSynchronised_;    //!< Network synchronised variable for bPhysicsActive_
            //! When attaching objects hierarchically this variable tells this object (as child) whether physics was activated before attaching (because the deactivate physics while being attached).
            bool                         bPhysicsActiveBeforeAttaching_;
            WorldEntityCollisionShape*   collisionShape_;                //!< Attached collision shapes go here
            btScalar                     mass_;                          //!< @see setMass
            btVector3                    localInertia_;                  //!< @see getLocalInertia
            btScalar                     restitution_;                   //!< @see setRestitution
            btScalar                     angularFactor_;                 //!< @see setAngularFactor
            btScalar                     linearDamping_;                 //!< @see setLinearDamping
            btScalar                     angularDamping_;                //!< @see setAngularDamping
            btScalar                     friction_;                      //!< @see setFriction
            btScalar                     childrenMass_;                  //!< Sum of all the children's masses
            bool                         bCollisionCallbackActive_;      //!< @see enableCollisionCallback
            bool                         bCollisionResponseActive_;      //!< Tells whether the object should respond to collisions
    };

    // Inline heavily used functions for release builds. In debug, we better avoid including OgreSceneNode here.
#ifdef ORXONOX_RELEASE
    inline const Vector3& WorldEntity::getPosition() const
        { return this->node_->getPosition(); }
    inline const Quaternion& WorldEntity::getOrientation() const
        { return this->node_->getOrientation(); }
    inline const Vector3& WorldEntity::getScale3D() const
        { return this->node_->getScale(); }
#endif

    SUPER_FUNCTION(5, WorldEntity, changedScale, false);
}

#endif /* _WorldEntity_H__ */

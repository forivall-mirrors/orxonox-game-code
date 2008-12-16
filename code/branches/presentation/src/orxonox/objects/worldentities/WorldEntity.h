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

#ifdef _NDEBUG
#include <OgreSceneNode.h>
#else
#include <OgrePrerequisites.h>
#endif
#include "LinearMath/btMotionState.h"

#include "network/synchronisable/Synchronisable.h"
#include "core/BaseObject.h"
#include "util/Math.h"

namespace orxonox
{
    class _OrxonoxExport WorldEntity : public BaseObject, public Synchronisable, public btMotionState
    {
        public:
            WorldEntity(BaseObject* creator);
            virtual ~WorldEntity();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);
            void registerVariables();

            inline const Ogre::SceneNode* getNode() const
                { return this->node_; }

            static const Vector3 FRONT;
            static const Vector3 BACK;
            static const Vector3 LEFT;
            static const Vector3 RIGHT;
            static const Vector3 DOWN;
            static const Vector3 UP;

            virtual void setPosition(const Vector3& position) = 0;
            inline void setPosition(float x, float y, float z)
                { this->setPosition(Vector3(x, y, z)); }
            const Vector3& getPosition() const;
            const Vector3& getWorldPosition() const;

            void translate(const Vector3& distance, TransformSpace::Space relativeTo = TransformSpace::Parent);
            inline void translate(float x, float y, float z, TransformSpace::Space relativeTo = TransformSpace::Parent)
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

            void rotate(const Quaternion& rotation, TransformSpace::Space relativeTo = TransformSpace::Local);
            inline void rotate(const Vector3& axis, const Degree& angle, TransformSpace::Space relativeTo = TransformSpace::Local)
                { this->rotate(Quaternion(angle, axis), relativeTo); }

            inline void yaw(const Degree& angle, TransformSpace::Space relativeTo = TransformSpace::Local)
                { this->rotate(Quaternion(angle, Vector3::UNIT_Y), relativeTo); }
            inline void pitch(const Degree& angle, TransformSpace::Space relativeTo = TransformSpace::Local)
                { this->rotate(Quaternion(angle, Vector3::UNIT_X), relativeTo); }
            inline void roll(const Degree& angle, TransformSpace::Space relativeTo = TransformSpace::Local)
                { this->rotate(Quaternion(angle, Vector3::UNIT_Z), relativeTo); }

            void lookAt(const Vector3& target, TransformSpace::Space relativeTo = TransformSpace::Parent, const Vector3& localDirectionVector = Vector3::NEGATIVE_UNIT_Z);
            void setDirection(const Vector3& direction, TransformSpace::Space relativeTo = TransformSpace::Local, const Vector3& localDirectionVector = Vector3::NEGATIVE_UNIT_Z);
            inline void setDirection(float x, float y, float z, TransformSpace::Space relativeTo = TransformSpace::Local, const Vector3& localDirectionVector = Vector3::NEGATIVE_UNIT_Z)
                { this->setDirection(Vector3(x, y, z), relativeTo, localDirectionVector); }

            virtual void setScale3D(const Vector3& scale);
            inline void setScale3D(float x, float y, float z)
                { this->setScale3D(Vector3(x, y, z)); }
            const Vector3& getScale3D(void) const;
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
            WorldEntity* getAttachedObject(unsigned int index) const;
            inline const std::set<WorldEntity*>& getAttachedObjects() const
                { return this->children_; }

            void attachOgreObject(Ogre::MovableObject* object);
            void detachOgreObject(Ogre::MovableObject* object);
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

            void notifyChildPropsChanged();

        protected:
            Ogre::SceneNode* node_;

        private:
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
            void parentChanged();
            inline void scaleChanged()
                { this->setScale3D(this->getScale3D()); }

            WorldEntity* parent_;
            unsigned int parentID_;
            std::set<WorldEntity*> children_;


        /////////////
        // Physics //
        /////////////

        public:
            enum CollisionType
            {
                Dynamic,
                Kinematic,
                Static,
                None
            };

            bool hasPhysics()       const { return getCollisionType() != None     ; }
            bool isStatic()         const { return getCollisionType() == Static   ; }
            bool isKinematic()      const { return getCollisionType() == Kinematic; }
            bool isDynamic()        const { return getCollisionType() == Dynamic  ; }
            bool isPhysicsActive()  const { return this->bPhysicsActive_; }
            bool addedToPhysicalWorld() const;

            void activatePhysics();
            void deactivatePhysics();

            inline CollisionType getCollisionType() const
                { return this->collisionType_; }
            void setCollisionType(CollisionType type);

            void setCollisionTypeStr(const std::string& type);
            std::string getCollisionTypeStr() const;

            inline void setMass(float mass)
                { this->mass_ = mass; recalculateMassProps(); }
            inline float getMass() const
                { return this->mass_; }

            inline float getTotalMass() const
                { return this->mass_ + this->childrenMass_; }

            inline const btVector3& getLocalInertia() const
                { return this->localInertia_; }

            inline void setRestitution(float restitution)
                { this->restitution_ = restitution; resetPhysicsProps(); }
            inline float getRestitution() const
                { return this->restitution_; }

            inline void setAngularFactor(float angularFactor)
                { this->angularFactor_ = angularFactor; resetPhysicsProps(); }
            inline float getAngularFactor() const
                { return this->angularFactor_; }

            inline void setLinearDamping(float linearDamping)
                { this->linearDamping_ = linearDamping; resetPhysicsProps(); }
            inline float getLinearDamping() const
                { return this->linearDamping_; }

            inline void setAngularDamping(float angularDamping)
                { this->angularDamping_ = angularDamping; resetPhysicsProps(); }
            inline float getAngularDamping() const
                { return this->angularDamping_; }

            inline void setFriction(float friction)
                { this->friction_ = friction; resetPhysicsProps(); }
            inline float getFriction() const
                { return this->friction_; }

            void attachCollisionShape(CollisionShape* shape);
            void detachCollisionShape(CollisionShape* shape);
            CollisionShape* getAttachedCollisionShape(unsigned int index) const;

            inline CompoundCollisionShape* getCollisionShape()
                { return this->collisionShape_; }
            inline btRigidBody* getPhysicalBody()
                { return this->physicalBody_; }

            void notifyCollisionShapeChanged();
            void notifyChildMassChanged();

            virtual inline bool collidesAgainst(WorldEntity* otherObject, btManifoldPoint& contactPoint)
                { return false; } /* With false, Bullet assumes no modification to the collision objects. */

            inline void enableCollisionCallback()
                { this->bCollisionCallbackActive_ = true; this->collisionCallbackActivityChanged(); }
            inline void disableCollisionCallback()
                { this->bCollisionCallbackActive_ = false; this->collisionCallbackActivityChanged(); }
            inline bool isCollisionCallbackActive()
                { return this->bCollisionCallbackActive_; }

        protected:
            virtual bool isCollisionTypeLegal(CollisionType type) const = 0;

            btRigidBody*  physicalBody_;

        private:
            void updateCollisionType();
            void recalculateMassProps();
            void resetPhysicsProps();

            // network callbacks
            void collisionTypeChanged();
            void physicsActivityChanged();
            void collisionCallbackActivityChanged();
            inline void massChanged()
                { this->setMass(this->mass_); }
            inline void restitutionChanged()
                { this->setRestitution(this->restitution_); }
            inline void angularFactorChanged()
                { this->setAngularFactor(this->angularFactor_); }
            inline void linearDampingChanged()
                { this->setLinearDamping(this->linearDamping_); }
            inline void angularDampingChanged()
                { this->setAngularDamping(this->angularDamping_); }
            inline void frictionChanged()
                { this->setFriction(this->friction_); }

            CollisionType                collisionType_;
            CollisionType                collisionTypeSynchronised_;
            bool                         bPhysicsActive_;
            bool                         bPhysicsActiveSynchronised_;
            CompoundCollisionShape*      collisionShape_;
            btScalar                     mass_;
            btVector3                    localInertia_;
            btScalar                     restitution_;
            btScalar                     angularFactor_;
            btScalar                     linearDamping_;
            btScalar                     angularDamping_;
            btScalar                     friction_;
            btScalar                     childrenMass_;
            bool                         bCollisionCallbackActive_;
    };

    // Inline heavily used functions for release builds. In debug, we better avoid including OgreSceneNode here.
#ifdef _NDEBUG
    inline const Vector3& WorldEntity::getPosition() const
        { return this->node_->getPosition(); }
    inline const Quaternion& WorldEntity::getOrientation() const
        { return this->node_->getrOrientation(); }
    inline const Vector3& WorldEntity::getScale3D(void) const
        { return this->node_->getScale(); }
#endif

    SUPER_FUNCTION(5, WorldEntity, changedScale, false);
}

#endif /* _WorldEntity_H__ */

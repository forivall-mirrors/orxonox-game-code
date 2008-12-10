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

#define OGRE_FORCE_ANGLE_TYPES
#include <OgreSceneNode.h>

#include "LinearMath/btMotionState.h"

#include "network/Synchronisable.h"
#include "core/BaseObject.h"
#include "util/Math.h"

namespace orxonox
{
    class _OrxonoxExport WorldEntity : public BaseObject, public network::Synchronisable, public btMotionState
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
            inline const Vector3& getPosition() const
                { return this->node_->getPosition(); }
            inline const Vector3& getWorldPosition() const
                { return this->node_->getWorldPosition(); }

            virtual void translate(const Vector3& distance, Ogre::Node::TransformSpace relativeTo = Ogre::Node::TS_LOCAL) = 0;
            inline void translate(float x, float y, float z, Ogre::Node::TransformSpace relativeTo = Ogre::Node::TS_LOCAL)
                { this->translate(Vector3(x, y, z), relativeTo); }

            virtual void setOrientation(const Quaternion& orientation) = 0;
            inline void setOrientation(float w, float x, float y, float z)
                { this->setOrientation(Quaternion(w, x, y, z)); }
            inline void setOrientation(const Vector3& axis, const Radian& angle)
                { this->setOrientation(Quaternion(angle, axis)); }
            inline void setOrientation(const Vector3& axis, const Degree& angle)
                { this->setOrientation(Quaternion(angle, axis)); }
            inline const Quaternion& getOrientation() const
                { return this->node_->getOrientation(); }
            inline const Quaternion& getWorldOrientation() const
                { return this->node_->getWorldOrientation(); }

            virtual void rotate(const Quaternion& rotation, Ogre::Node::TransformSpace relativeTo = Ogre::Node::TS_LOCAL) = 0;
            inline void rotate(const Vector3& axis, const Degree& angle, Ogre::Node::TransformSpace relativeTo = Ogre::Node::TS_LOCAL)
                { this->rotate(Quaternion(angle, axis), relativeTo); }
            inline void rotate(const Vector3& axis, const Radian& angle, Ogre::Node::TransformSpace relativeTo = Ogre::Node::TS_LOCAL)
                { this->rotate(Quaternion(angle, axis), relativeTo); }

            virtual void yaw(const Degree& angle, Ogre::Node::TransformSpace relativeTo = Ogre::Node::TS_LOCAL) = 0;
            inline void yaw(const Radian& angle, Ogre::Node::TransformSpace relativeTo = Ogre::Node::TS_LOCAL)
                { this->yaw(Degree(angle), relativeTo); }
            virtual void pitch(const Degree& angle, Ogre::Node::TransformSpace relativeTo = Ogre::Node::TS_LOCAL) = 0;
            inline void pitch(const Radian& angle, Ogre::Node::TransformSpace relativeTo = Ogre::Node::TS_LOCAL)
                { this->pitch(Degree(angle), relativeTo); }
            virtual void roll(const Degree& angle, Ogre::Node::TransformSpace relativeTo = Ogre::Node::TS_LOCAL) = 0;
            inline void roll(const Radian& angle, Ogre::Node::TransformSpace relativeTo = Ogre::Node::TS_LOCAL)
                { this->roll(Degree(angle), relativeTo); }

            virtual void lookAt(const Vector3& target, Ogre::Node::TransformSpace relativeTo = Ogre::Node::TS_LOCAL, const Vector3& localDirectionVector = Vector3::NEGATIVE_UNIT_Z) = 0;
            virtual void setDirection(const Vector3& direction, Ogre::Node::TransformSpace relativeTo = Ogre::Node::TS_LOCAL, const Vector3& localDirectionVector = Vector3::NEGATIVE_UNIT_Z) = 0;
            inline void setDirection(float x, float y, float z, Ogre::Node::TransformSpace relativeTo = Ogre::Node::TS_LOCAL, const Vector3& localDirectionVector = Vector3::NEGATIVE_UNIT_Z)
                { this->setDirection(Vector3(x, y, z), relativeTo, localDirectionVector); }

            virtual void setScale3D(const Vector3& scale);
            inline void setScale3D(float x, float y, float z)
                { this->setScale3D(Vector3(x, y, z)); }
            inline const Vector3& getScale3D(void) const
                { return this->node_->getScale(); }

            void setScale(float scale)
                { this->setScale3D(scale, scale, scale); }
            inline float getScale() const
                { Vector3 scale = this->getScale3D(); return (scale.x == scale.y && scale.x == scale.z) ? scale.x : 1; }

            virtual void scale3D(const Vector3& scale);
            inline void scale3D(float x, float y, float z)
                { this->scale3D(Vector3(x, y, z)); }
            inline void scale(float scale)
                { this->scale3D(scale, scale, scale); }

            void attach(WorldEntity* object);
//            void attachAsdf(BlinkingBillboard* object);
            void detach(WorldEntity* object);
            WorldEntity* getAttachedObject(unsigned int index) const;
//            BlinkingBillboard* getAttachedAsdfObject(unsigned int index) const;
            inline const std::set<WorldEntity*>& getAttachedObjects() const
                { return this->children_; }

            inline void attachOgreObject(Ogre::MovableObject* object)
                { this->node_->attachObject(object); }
            inline void detachOgreObject(Ogre::MovableObject* object)
                { this->node_->detachObject(object); }
            inline Ogre::MovableObject* detachOgreObject(const Ogre::String& name)
                { return this->node_->detachObject(name); }

            inline void attachToParent(WorldEntity* parent)
                { parent->attach(this); }
            inline void detachFromParent()
                { if (this->parent_) { this->parent_->detach(this); } }
            inline WorldEntity* getParent() const
                { return this->parent_; }

        protected:
            Ogre::SceneNode* node_;

        private:
            void updateParent();

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

            bool hasPhysics()  const { return getCollisionType() != None     ; }
            bool isStatic()    const { return getCollisionType() == Static   ; }
            bool isKinematic() const { return getCollisionType() == Kinematic; }
            bool isDynamic()   const { return getCollisionType() == Dynamic  ; }
            bool isPhysicsRunning() const;

            inline CollisionType getCollisionType() const
                { return this->collisionType_; }
            void setCollisionType(CollisionType type);

            void setCollisionTypeStr(const std::string& type);
            std::string getCollisionTypeStr() const;

            void setMass(float mass);
            inline float getMass() const
                { return this->mass_; }

            void attachCollisionShape(CollisionShape* shape);
            CollisionShape* getAttachedCollisionShape(unsigned int index) const;

            inline CompoundCollisionShape* getCollisionShape()
                { return this->collisionShape_; }
            inline btRigidBody* getPhysicalBody()
                { return this->physicalBody_; }

        protected:
            virtual bool isCollisionTypeLegal(CollisionType type) const = 0;

            btRigidBody*  physicalBody_;

        private:
            void updateCollisionType();
            void mergeCollisionShape(CollisionShape* shape);
            void internalSetMassProps();
            btVector3 getLocalInertia(btScalar mass) const;
            bool checkPhysics() const;
            void addToPhysicalWorld() const;
            void removeFromPhysicalWorld() const;

            // network callbacks
            void collisionTypeChanged();
            void massChanged();

            CollisionType                collisionType_;
            CollisionType                collisionTypeSynchronised_;
            CompoundCollisionShape*      collisionShape_;
            btScalar                     mass_;
            btScalar                     childMass_;
    };
}

#endif /* _WorldEntity_H__ */

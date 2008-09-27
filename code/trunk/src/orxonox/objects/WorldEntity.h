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

#ifndef _WorldEntity_H__
#define _WorldEntity_H__

#include "OrxonoxPrereqs.h"
#include <OgrePrerequisites.h>

#include <OgreSceneNode.h>
#include "util/Math.h"
#include "network/Synchronisable.h"
#include "core/XMLIncludes.h"
#include "core/BaseObject.h"
#include "Tickable.h"
#include "tools/Mesh.h"

namespace orxonox
{
    class _OrxonoxExport WorldEntity : public BaseObject, public Tickable, public network::Synchronisable
    {
        public:
            WorldEntity();
            virtual ~WorldEntity();

            virtual void tick(float dt);
            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);
            virtual inline bool create(){ return Synchronisable::create(); }

            void attachWorldEntity(WorldEntity* entity);
            const WorldEntity* getAttachedWorldEntity(unsigned int index) const;

            inline Ogre::SceneNode* getNode() const
                { return this->node_; }

            inline void setNode(Ogre::SceneNode* node)
                { this->node_ = node; }

            inline void setPosition(const Vector3& pos)
                { this->node_->setPosition(pos); }
            inline void setPosition(Real x, Real y, Real z)
                { this->node_->setPosition(x, y, z); }
            inline const Vector3& getPosition() const
                { return this->node_->getPosition(); }
            inline const Vector3& getWorldPosition() const
                { return this->node_->getWorldPosition(); }

            inline void translate(const Vector3& d, Ogre::Node::TransformSpace relativeTo=Ogre::Node::TS_PARENT)
                { this->node_->translate(d, relativeTo); }
            inline void translate(Real x, Real y, Real z, Ogre::Node::TransformSpace relativeTo=Ogre::Node::TS_PARENT)
                { this->node_->translate(x, y, z, relativeTo); }
            inline void translate(const Matrix3& axes, const Vector3& move, Ogre::Node::TransformSpace relativeTo=Ogre::Node::TS_PARENT)
                { this->node_->translate(axes, move, relativeTo); }
            inline void translate(const Matrix3& axes, Real x, Real y, Real z, Ogre::Node::TransformSpace relativeTo=Ogre::Node::TS_PARENT)
                { this->node_->translate(axes, x, y, z, relativeTo); }

            inline void yaw(const Radian& angle, Ogre::Node::TransformSpace relativeTo=Ogre::Node::TS_LOCAL)
                { this->node_->yaw(angle, relativeTo); }
            inline void pitch(const Radian& angle, Ogre::Node::TransformSpace relativeTo=Ogre::Node::TS_LOCAL)
                { this->node_->pitch(angle, relativeTo); }
            inline void roll(const Radian& angle, Ogre::Node::TransformSpace relativeTo=Ogre::Node::TS_LOCAL)
                { this->node_->roll(angle, relativeTo); }
            void setYawPitchRoll(const Degree& yaw, const Degree& pitch, const Degree& roll);

            inline void setYaw(const Degree& angle)
                { this->node_->yaw(angle, Ogre::Node::TS_LOCAL); }
            inline void setPitch(const Degree& angle)
                { this->node_->pitch(angle, Ogre::Node::TS_LOCAL); }
            inline void setRoll(const Degree& angle)
                { this->node_->roll(angle, Ogre::Node::TS_LOCAL); }

            inline const Ogre::Quaternion& getOrientation()
              { return this->node_->getOrientation(); }
            inline const Ogre::Quaternion& getWorldOrientation()
              { return this->node_->getWorldOrientation(); }
            inline void setOrientation(const Ogre::Quaternion& quat)
              { this->node_->setOrientation(quat); }
            inline void rotate(const Vector3& axis, const Radian& angle, Ogre::Node::TransformSpace relativeTo=Ogre::Node::TS_LOCAL)
              { this->node_->rotate(axis, angle, relativeTo); }
            inline void setDirectionSimple(Real x, Real y, Real z)
              { this->setDirection(x, y, z); }
            inline void setDirection(Real x, Real y, Real z, Ogre::Node::TransformSpace relativeTo=Ogre::Node::TS_LOCAL, const Vector3& localDirectionVector=Vector3::NEGATIVE_UNIT_Z)
              { this->node_->setDirection(x, y, z, relativeTo, localDirectionVector); }
            inline void setDirection(const Vector3& vec, Ogre::Node::TransformSpace relativeTo=Ogre::Node::TS_LOCAL, const Vector3& localDirectionVector=Vector3::NEGATIVE_UNIT_Z)
              { this->node_->setDirection(vec, relativeTo, localDirectionVector); }
            inline void lookAt(const Vector3& targetPoint, Ogre::Node::TransformSpace relativeTo, const Vector3& localDirectionVector=Vector3::NEGATIVE_UNIT_Z)
              { this->node_->lookAt(targetPoint, relativeTo, localDirectionVector); }

            inline void setScale3D(const Vector3 &scale)
              { this->node_->setScale(scale); }
            inline void setScale3D(Real x, Real y, Real z)
              { this->node_->setScale(x, y, z); }
            inline const Vector3& getScale3D(void) const
              { return this->node_->getScale(); }
            inline void setScale(float scale)
              { this->node_->setScale(scale, scale, scale); }
            inline float getScale() const
              { Vector3 scale = this->getScale3D(); return (scale.x == scale.y && scale.x == scale.z) ? scale.x : 1; }
            inline void scale3D(const Vector3 &scale)
              { this->node_->scale(scale); }
            inline void scale3D(Real x, Real y, Real z)
              { this->node_->scale(x, y, z); }
            inline void scale(Real scale)
              { this->node_->scale(scale, scale, scale); }

            void attachObject(const WorldEntity& obj) const;
            void attachObject(WorldEntity* obj) const;
            inline void attachObject(Ogre::MovableObject* obj) const
              { this->node_->attachObject(obj); }
            inline void attachObject(Mesh& mesh) const
              { this->node_->attachObject(mesh.getEntity()); }
            inline void detachObject(Ogre::MovableObject* obj) const
              { this->node_->detachObject(obj); }
            inline void detachAllObjects() const
              { this->node_->detachAllObjects(); }

            inline void setVelocity(const Vector3& velocity)
                { this->velocity_ = velocity; }
            inline void setVelocity(Real x, Real y, Real z)
                { this->velocity_.x = x; this->velocity_.y = y; this->velocity_.z = z; }
            inline const Vector3& getVelocity() const
                { return this->velocity_; }

            inline void setAcceleration(const Vector3& acceleration)
                { this->acceleration_ = acceleration; }
            inline void setAcceleration(Real x, Real y, Real z)
                { this->acceleration_.x = x; this->acceleration_.y = y; this->acceleration_.z = z; }
            inline const Vector3& getAcceleration() const
                { return this->acceleration_; }

            inline void setRotationAxis(const Vector3& axis)
                { this->rotationAxis_ = axis; this->rotationAxis_.normalise(); }
            inline void setRotationAxis(Real x, Real y, Real z)
                { this->rotationAxis_.x = x; this->rotationAxis_.y = y; this->rotationAxis_.z = z; rotationAxis_.normalise(); }
            inline const Vector3& getRotationAxis() const
                { return this->rotationAxis_; }

//            inline void setRotationRate(const Radian& angle)
//                { this->rotationRate_ = angle; }
            inline void setRotationRate(const Degree& angle)
                { this->rotationRate_ = angle; this->setStatic(angle == Degree(0)); }
            inline const Radian& getRotationRate() const
                { return this->rotationRate_; }

            inline void setMomentum(const Radian& angle)
                { this->momentum_ = angle; }
            inline void setMomentum(const Degree& angle)
                { this->momentum_ = angle; }
            inline const Radian& getMomentum() const
                { return this->momentum_; }

            inline void setStatic(bool bStatic)
                { this->bStatic_ = bStatic; }
            inline bool isStatic() const
                { return this->bStatic_; }

        protected:
            void registerAllVariables();

            Vector3 velocity_;
            Vector3 acceleration_;
            Vector3 rotationAxis_;
            Radian rotationRate_;
            Radian momentum_;

        private:
            static unsigned int worldEntityCounter_s;
            Ogre::SceneNode* node_;
            bool bStatic_;
            std::vector<WorldEntity*> attachedWorldEntities_;
    };
}

#endif /* _WorldEntity_H__ */

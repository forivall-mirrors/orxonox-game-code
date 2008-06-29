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

#include <OgreSceneManager.h>
#include <OgreSceneNode.h>

#include "util/Math.h"
#include "util/XMLIncludes.h"
#include "network/Synchronisable.h"
#include "core/BaseObject.h"
#include "Tickable.h"
#include "../tools/Mesh.h"

namespace orxonox
{
    class _OrxonoxExport WorldEntity : public BaseObject, public Tickable, public network::Synchronisable
    {
        public:
            WorldEntity();
            virtual ~WorldEntity();

            virtual void tick(float dt);
            virtual void loadParams(TiXmlElement* xmlElem);
            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);
            virtual bool create(){ return Synchronisable::create(); }

            void attachWorldEntity(WorldEntity* entity);
            const WorldEntity* getAttachedWorldEntity(unsigned int index) const;

            Ogre::SceneNode* getNode()
                { return this->node_; }

            void setNode(Ogre::SceneNode* node)
                { this->node_ = node; }

            void setPosition(const Vector3& pos)
                { this->node_->setPosition(pos); }
            void setPositionLoader1(const Vector3& pos)
                { this->node_->setPosition(pos); }
            void setPositionLoader2(Real x, Real y, Real z)
                { this->node_->setPosition(x, y, z); }
            void setPosition(Real x, Real y, Real z)
                { this->node_->setPosition(x, y, z); }
            const Vector3& getPosition() const
                { return this->node_->getPosition(); }

            void translate(const Vector3 &d, Ogre::Node::TransformSpace relativeTo=Ogre::Node::TS_PARENT)
                { this->node_->translate(d, relativeTo); }
            void translate(Real x, Real y, Real z, Ogre::Node::TransformSpace relativeTo=Ogre::Node::TS_PARENT)
                { this->node_->translate(x, y, z, relativeTo); }
            void translate(const Matrix3 &axes, const Vector3 &move, Ogre::Node::TransformSpace relativeTo=Ogre::Node::TS_PARENT)
                { this->node_->translate(axes, move, relativeTo); }
            void translate(const Matrix3 &axes, Real x, Real y, Real z, Ogre::Node::TransformSpace relativeTo=Ogre::Node::TS_PARENT)
                { this->node_->translate(axes, x, y, z, relativeTo); }

            void yaw(const Radian &angle, Ogre::Node::TransformSpace relativeTo=Ogre::Node::TS_LOCAL)
                { this->node_->yaw(angle, relativeTo); }
            void pitch(const Radian &angle, Ogre::Node::TransformSpace relativeTo=Ogre::Node::TS_LOCAL)
                { this->node_->pitch(angle, relativeTo); }
            void roll(const Radian &angle, Ogre::Node::TransformSpace relativeTo=Ogre::Node::TS_LOCAL)
                { this->node_->roll(angle, relativeTo); }
            void setYawPitchRoll(const Degree& yaw, const Degree& pitch, const Degree& roll);

            void setYaw(const Degree &angle)
                { this->node_->yaw(angle, Ogre::Node::TS_LOCAL); }
            void setPitch(const Degree &angle)
                { this->node_->pitch(angle, Ogre::Node::TS_LOCAL); }
            void setRoll(const Degree &angle)
                { this->node_->roll(angle, Ogre::Node::TS_LOCAL); }

            const Ogre::Quaternion& getOrientation()
              { return this->node_->getOrientation(); }
            void setOrientation(const Ogre::Quaternion& quat)
              { this->node_->setOrientation(quat); }
            void rotate(const Vector3 &axis, const Radian &angle, Ogre::Node::TransformSpace relativeTo=Ogre::Node::TS_LOCAL)
              { this->node_->rotate(axis, angle, relativeTo); }
            void setDirectionLoader(Real x, Real y, Real z)
              { this->setDirection(x, y, z); }
            void setDirection(Real x, Real y, Real z, Ogre::Node::TransformSpace relativeTo=Ogre::Node::TS_LOCAL, const Vector3 &localDirectionVector=Vector3::NEGATIVE_UNIT_Z)
              { this->node_->setDirection(x, y, z, relativeTo, localDirectionVector); }
            void setDirection(const Vector3 &vec, Ogre::Node::TransformSpace relativeTo=Ogre::Node::TS_LOCAL, const Vector3 &localDirectionVector=Vector3::NEGATIVE_UNIT_Z)
              { this->node_->setDirection(vec, relativeTo, localDirectionVector); }
            void lookAt(const Vector3 &targetPoint, Ogre::Node::TransformSpace relativeTo, const Vector3 &localDirectionVector=Vector3::NEGATIVE_UNIT_Z)
              { this->node_->lookAt(targetPoint, relativeTo, localDirectionVector); }

            void setScale(const Vector3 &scale)
              { this->node_->setScale(scale); }
            void setScale(Real x, Real y, Real z)
              { this->node_->setScale(x, y, z); }
            void setScale(Real scale)
              { this->node_->setScale(scale, scale, scale); }
            void setTotalScale(Real scale)
              { this->node_->setScale(scale, scale, scale); }
            const Vector3& getScale(void) const
              { return this->node_->getScale(); }
            void scale(const Vector3 &scale)
              { this->node_->scale(scale); }
            void scale(Real x, Real y, Real z)
              { this->node_->scale(x, y, z); }
            void scale(Real scale)
              { this->node_->scale(scale, scale, scale); }

            void attachObject(Ogre::MovableObject *obj)
              { this->node_->attachObject(obj); }
            void attachObject(Mesh &mesh)
              { this->node_->attachObject(mesh.getEntity()); }
            void detachObject(Ogre::MovableObject *obj)
              { this->node_->detachObject(obj); }
            void detachAllObjects()
              { this->node_->detachAllObjects(); }

            void setVelocity(const Vector3& velocity)
                { this->velocity_ = velocity; }
            void setVelocity(Real x, Real y, Real z)
                { this->velocity_.x = x; this->velocity_.y = y; this->velocity_.z = z; }
            const Vector3& getVelocity() const
                { return this->velocity_; }

            void setAcceleration(const Vector3& acceleration)
                { this->acceleration_ = acceleration; }
            void setAcceleration(Real x, Real y, Real z)
                { this->acceleration_.x = x; this->acceleration_.y = y; this->acceleration_.z = z; }
            const Vector3& getAcceleration() const
                { return this->acceleration_; }

            void setRotationAxisLoader(const Vector3& axis)
                { this->rotationAxis_ = axis; rotationAxis_.normalise(); }
            void setRotationAxis(const Vector3& axis)
                { this->rotationAxis_ = axis; rotationAxis_.normalise(); }
            void setRotationAxis(Real x, Real y, Real z)
                { this->rotationAxis_.x = x; this->rotationAxis_.y = y; this->rotationAxis_.z = z; rotationAxis_.normalise(); }
            const Vector3& getRotationAxis() const
                { return this->rotationAxis_; }

//            void setRotationRate(const Radian& angle)
//                { this->rotationRate_ = angle; }
            void setRotationRate(const Degree& angle)
                { this->rotationRate_ = angle; this->setStatic(angle == Degree(0)); }
            const Radian& getRotationRate() const
                { return this->rotationRate_; }

            void setMomentum(const Radian& angle)
                { this->momentum_ = angle; }
            void setMomentum(const Degree& angle)
                { this->momentum_ = angle; }
            const Radian& getMomentum() const
                { return this->momentum_; }

            void setStatic(bool bStatic)
                { this->bStatic_ = bStatic; }
            bool isStatic()
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

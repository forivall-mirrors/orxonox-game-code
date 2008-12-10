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
 *      Reto Grieder
 *   Co-authors:
 *      Martin Stypinski
 *
 */

#ifndef _MovableEntity_H__
#define _MovableEntity_H__

#include "OrxonoxPrereqs.h"

#include "WorldEntity.h"
#include "objects/Tickable.h"

namespace orxonox
{
    class _OrxonoxExport MovableEntity : public WorldEntity, public Tickable
    {
        public:
            MovableEntity(BaseObject* creator);
            virtual ~MovableEntity();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);
            virtual void tick(float dt);
            void registerVariables();

            using WorldEntity::setPosition;
            using WorldEntity::translate;
            using WorldEntity::setOrientation;
            using WorldEntity::rotate;
            using WorldEntity::yaw;
            using WorldEntity::pitch;
            using WorldEntity::roll;
            using WorldEntity::lookAt;
            using WorldEntity::setDirection;

            void setPosition(const Vector3& position);
            void translate(const Vector3& distance, Ogre::Node::TransformSpace relativeTo = Ogre::Node::TS_LOCAL);
            void setOrientation(const Quaternion& orientation);
            void rotate(const Quaternion& rotation, Ogre::Node::TransformSpace relativeTo = Ogre::Node::TS_LOCAL);
            void yaw(const Degree& angle, Ogre::Node::TransformSpace relativeTo = Ogre::Node::TS_LOCAL);
            void pitch(const Degree& angle, Ogre::Node::TransformSpace relativeTo = Ogre::Node::TS_LOCAL);
            void roll(const Degree& angle, Ogre::Node::TransformSpace relativeTo = Ogre::Node::TS_LOCAL);
            void lookAt(const Vector3& target, Ogre::Node::TransformSpace relativeTo = Ogre::Node::TS_LOCAL, const Vector3& localDirectionVector = Vector3::NEGATIVE_UNIT_Z);
            void setDirection(const Vector3& direction, Ogre::Node::TransformSpace relativeTo = Ogre::Node::TS_LOCAL, const Vector3& localDirectionVector = Vector3::NEGATIVE_UNIT_Z);

            void setVelocity(const Vector3& velocity);
            inline void setVelocity(float x, float y, float z)
                { this->setVelocity(Vector3(x, y, z)); }
            inline const Vector3& getVelocity() const
                { return this->linearVelocity_; }

            void setAngularVelocity(const Vector3& velocity);
            inline void setAngularVelocity(float x, float y, float z)
                { this->setAngularVelocity(Vector3(x, y, z)); }
            inline const Vector3& getAngularVelocity() const
                { return this->linearAcceleration_; }

            void setAcceleration(const Vector3& acceleration);
            inline void setAcceleration(float x, float y, float z)
                { this->setAcceleration(Vector3(x, y, z)); }
            inline const Vector3& getAcceleration() const
                { return this->linearAcceleration_; }

            void setAngularAcceleration(const Vector3& acceleration);
            inline void setAngularAcceleration(float x, float y, float z)
                { this->setAngularAcceleration(Vector3(x, y, z)); }
            inline const Vector3& getAngularAcceleration() const
                { return this->angularAcceleration_; }

            inline void setRotationRate(Degree rate)
                { this->setAngularVelocity(this->getAngularVelocity().normalisedCopy() * rate.valueRadians()); }
            inline Degree getRotationRate() const
                { return Degree(this->getAngularVelocity().length()); }

            inline void setRotationAxis(const Vector3& axis)
                { this->setAngularVelocity(axis * this->getAngularVelocity().length()); }
            inline Vector3 getRotationAxis() const
                { return this->getAngularVelocity().normalisedCopy(); }

        protected:
            Vector3 linearAcceleration_;
            Vector3 linearVelocity_;
            Vector3 angularAcceleration_;
            Vector3 angularVelocity_;

        private:
            virtual void positionChanged       (bool bContinuous) { }
            virtual void orientationChanged    (bool bContinuous) { }
            virtual void linearVelocityChanged (bool bContinuous) { }
            virtual void angularVelocityChanged(bool bContinuous) { }

            virtual bool isCollisionTypeLegal(WorldEntity::CollisionType type) const;

            // Bullet btMotionState related
            void setWorldTransform(const btTransform& worldTrans);
            void getWorldTransform(btTransform& worldTrans) const;
    };
}

#endif /* _MovableEntity_H__ */

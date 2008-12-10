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
 *      Reto Grieder
 *   Co-authors:
 *      ...
 *
 */

#ifndef _StaticEntity_H__
#define _StaticEntity_H__

#include "OrxonoxPrereqs.h"
#include "WorldEntity.h"

namespace orxonox
{
    class _OrxonoxExport StaticEntity : public WorldEntity
    {
        public:
            StaticEntity(BaseObject* creator);
            virtual ~StaticEntity();

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

        private:
            bool isCollisionTypeLegal(CollisionType type) const;

            inline void positionChanged()
                { this->setPosition(this->getPosition()); }
            inline void orientationChanged()
                { this->setOrientation(this->getOrientation()); }

            // Bullet btMotionState related
            void setWorldTransform(const btTransform& worldTrans);
            void getWorldTransform(btTransform& worldTrans) const;
    };
}

#endif /* _StaticEntity_H__ */

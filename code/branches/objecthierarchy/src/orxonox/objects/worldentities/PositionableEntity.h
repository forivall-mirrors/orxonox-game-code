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

#ifndef _PositionableEntity_H__
#define _PositionableEntity_H__

#include "OrxonoxPrereqs.h"
#include "WorldEntity.h"

namespace orxonox
{
    class _OrxonoxExport PositionableEntity : public WorldEntity
    {
        public:
            PositionableEntity(BaseObject* creator);
            virtual ~PositionableEntity();

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

            inline void setPosition(const Vector3& position)
                { this->node_->setPosition(position); }
            inline void translate(const Vector3& distance, Ogre::Node::TransformSpace relativeTo = Ogre::Node::TS_LOCAL)
                { this->node_->translate(distance, relativeTo); }
            inline void setOrientation(const Quaternion& orientation)
                { this->node_->setOrientation(orientation); }
            inline void rotate(const Quaternion& rotation, Ogre::Node::TransformSpace relativeTo = Ogre::Node::TS_LOCAL)
                { this->node_->rotate(rotation, relativeTo); }
            inline void yaw(const Degree& angle, Ogre::Node::TransformSpace relativeTo = Ogre::Node::TS_LOCAL)
                { this->node_->yaw(angle, relativeTo); }
            inline void pitch(const Degree& angle, Ogre::Node::TransformSpace relativeTo = Ogre::Node::TS_LOCAL)
                { this->node_->pitch(angle, relativeTo); }
            inline void roll(const Degree& angle, Ogre::Node::TransformSpace relativeTo = Ogre::Node::TS_LOCAL)
                { this->node_->roll(angle, relativeTo); }
            inline void lookAt(const Vector3& target, Ogre::Node::TransformSpace relativeTo = Ogre::Node::TS_LOCAL, const Vector3& localDirectionVector = Vector3::NEGATIVE_UNIT_Z)
                { this->node_->lookAt(target, relativeTo, localDirectionVector); }
            inline void setDirection(const Vector3& direction, Ogre::Node::TransformSpace relativeTo = Ogre::Node::TS_LOCAL, const Vector3& localDirectionVector = Vector3::NEGATIVE_UNIT_Z)
                { this->node_->setDirection(direction, relativeTo, localDirectionVector); }
    };
}

#endif /* _PositionableEntity_H__ */

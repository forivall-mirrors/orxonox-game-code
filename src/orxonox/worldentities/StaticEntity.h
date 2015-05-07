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
    /**
    @brief
        The StaticEntity is the simplest derivative of the @ref orxonox::WorldEntity class. This means all StaticEntity instances also have
        a position in space, a mass, a scale, a frication, ... because every StaticEntity is a WorldEntity. You can attach StaticEntities to eachother ike @ref orxonox::WorldEntity WorldEntities.

        In contrast to the MobileEntity the StaticEntity cannot move with respect to the parent to which it is attached. That's why
        it is called StaticEntity. It will keep the same position (always with respect to its parent) forever unless you call the
        function @see setPosition to changee it.
    */

    class _OrxonoxExport StaticEntity : public WorldEntity
    {
        public:
            StaticEntity(Context* context);
            virtual ~StaticEntity();

            using WorldEntity::setPosition;
            using WorldEntity::setOrientation;

            void setPosition(const Vector3& position);
            void setOrientation(const Quaternion& orientation);

        private:
            void registerVariables();
            bool isCollisionTypeLegal(CollisionType type) const;

            // network callbacks
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

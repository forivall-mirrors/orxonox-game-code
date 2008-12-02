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
 *      ...
 *
 */

#ifndef _CollisionShape_H__
#define _CollisionShape_H__

#include "OrxonoxPrereqs.h"

#include "objects/worldentities/StaticEntity.h"

namespace orxonox
{
    class _OrxonoxExport CollisionShape : public StaticEntity
    {
        public:
            CollisionShape(BaseObject* creator);
            virtual ~CollisionShape();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);

            btCollisionShape* getCollisionShape() const { return this->collisionShape_; }
            bool isCompoundShape() const { return this->bIsCompound_; }

            bool hasNoTransform() const;
            virtual btVector3 getTotalScaling();

        protected:
            bool              bIsCompound_;
            btCollisionShape* collisionShape_;

        private:
            virtual void setScale3D(const Vector3& scale);
            virtual void scale3D(const Vector3& scale);

            bool isCollisionTypeLegal(WorldEntity::CollisionType type) const;
    };
}

#endif /* _CollisionShape_H__ */

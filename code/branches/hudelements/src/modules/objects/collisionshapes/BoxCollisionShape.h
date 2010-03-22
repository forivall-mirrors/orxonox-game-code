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

#ifndef _BoxCollisionShape_H__
#define _BoxCollisionShape_H__

#include "objects/ObjectsPrereqs.h"

#include "util/Math.h"
#include "collisionshapes/CollisionShape.h"

namespace orxonox
{
    class _ObjectsExport BoxCollisionShape : public CollisionShape
    {
        public:
            BoxCollisionShape(BaseObject* creator);
            virtual ~BoxCollisionShape();

            void registerVariables();
            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);

            inline void setHalfExtents(const Vector3& extents)
                { this->halfExtents_ = extents; updateShape(); }
            inline const Vector3& getHalfExtents() const
                { return halfExtents_;}

            inline void setWidth(float value)
                { this->halfExtents_.z = value / 2; updateShape(); }
            inline float getWidth() const
                { return this->halfExtents_.z * 2; }

            inline void setHeight(float value)
                { this->halfExtents_.y = value / 2; updateShape(); }
            inline float getHeight() const
                { return this->halfExtents_.y * 2; }

            inline void setLength(float value)
                { this->halfExtents_.x = value / 2; updateShape(); }
            inline float getLength() const
                { return this->halfExtents_.x * 2; }

        private:
            btCollisionShape* createNewShape() const;

            Vector3 halfExtents_;
     };
}

#endif /* _BoxCollisionShape_H__ */

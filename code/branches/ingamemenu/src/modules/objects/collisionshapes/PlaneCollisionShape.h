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
 *      Martin Stypinski
 *   Co-authors:
 *      ...
 *
 */

#ifndef _PlaneCollisionShape_H__
#define _PlaneCollisionShape_H__

#include "objects/ObjectsPrereqs.h"

#include "util/Math.h"
#include "collisionshapes/CollisionShape.h"

namespace orxonox
{
    class _ObjectsExport PlaneCollisionShape : public CollisionShape
    {
        public:
            PlaneCollisionShape(BaseObject* creator);
            virtual ~PlaneCollisionShape();

            void registerVariables();
            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);

            inline void setNormal(const Vector3& normal)
                { this->normal_ = normal; updateShape(); }
            inline const Vector3& getNormal()
                { return normal_;}

            inline void setOffset(float offset)
                { this->offset_ = offset; updateShape(); }
            inline float getOffset()
                { return this->offset_;}

        private:
            btCollisionShape* createNewShape()const;

            Vector3 normal_;
            float   offset_;
     };
}

#endif /* _PlaneCollisionShape_H__ */

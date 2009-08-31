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

#ifndef _SphereCollisionShape_H__
#define _SphereCollisionShape_H__

#include "objects/ObjectsPrereqs.h"
#include "collisionshapes/CollisionShape.h"

namespace orxonox
{
    class _ObjectsExport SphereCollisionShape : public CollisionShape
    {
        public:
            SphereCollisionShape(BaseObject* creator);
            virtual ~SphereCollisionShape();

            void registerVariables();
            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);

            inline void setRadius(float radius)
                { this->radius_ = radius; updateShape(); }
            inline float getRadius() const
                { return this->radius_; }

        private:
            btCollisionShape* createNewShape() const;

            float radius_;
    };
}

#endif /* _SphereCollisionShape_H__ */

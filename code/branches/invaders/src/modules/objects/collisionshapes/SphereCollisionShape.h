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

/**
    @file SphereCollisionShape.h
    @brief Definition of the SphereCollisionShape class.
    @ingroup Collisionshapes
*/

#ifndef _SphereCollisionShape_H__
#define _SphereCollisionShape_H__

#include "objects/ObjectsPrereqs.h"
#include "collisionshapes/CollisionShape.h"

namespace orxonox
{

    /**
    @brief
        Wrapper for the bullet sphere collision shape class btSphereShape.

    @author
        Reto Grieder

    @see btSphereShape
    @ingroup Collisionshapes
    */
    class _ObjectsExport SphereCollisionShape : public CollisionShape
    {
        public:
            SphereCollisionShape(Context* context);
            virtual ~SphereCollisionShape();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);

            /**
            @brief Set the radius of the SphereCollisionShape.
                   If the radius changes, this causes the internal collision shape to be recreated.
            @param radius The radius to be set.
            @return Returns true if the radius has changed, false if not.
            */
            inline bool setRadius(float radius)
                { if(this->radius_ == radius) return false; this->radius_ = radius; updateShape(); return true; }
            /**
            @brief Get the radius of the SphereCollisionShape.
            @return Returns the radius of the SphereCollisionShape.
            */
            inline float getRadius() const
                { return this->radius_; }

            virtual void changedScale(); // Is called when the scale of the SphereCollisionShape has changed.

        private:
            void registerVariables();

            btCollisionShape* createNewShape() const;

            float radius_; //!< The radius of the SphereCollisionShape.
    };
}

#endif /* _SphereCollisionShape_H__ */

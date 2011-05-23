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

/**
    @file PlaneCollisionShape.h
    @brief Definition of the PlaneCollisionShape class.
    @ingroup Collisionshapes
*/

#ifndef _PlaneCollisionShape_H__
#define _PlaneCollisionShape_H__

#include "objects/ObjectsPrereqs.h"

#include "util/Math.h"
#include "collisionshapes/CollisionShape.h"

namespace orxonox
{

    /**
    @brief
        Wrapper for the bullet plane collision shape class btStaticPlaneShape.

    @author
        Martin Stypinski

    @see btStaticPlaneShape
    @ingroup Collisionshapes
    */
    class _ObjectsExport PlaneCollisionShape : public CollisionShape
    {
        public:
            PlaneCollisionShape(BaseObject* creator);
            virtual ~PlaneCollisionShape();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);

            /**
            @brief Set the normal of the PlaneCollisionShape.
                   If the normal changes, this causes the internal collision shape to be recreated.
            @param normal The normal vector to be set.
            @return Returns true if the normal has changed, false if not.
            */
            inline bool setNormal(const Vector3& normal)
                { if(this->normal_ == normal) return false; this->normal_ = normal; updateShape(); return true; }
            /**
            @brief Get the normal of the PlaneCollisionShape.
            @return Returns the normal vector of the PlaneCollisionShape.
            */
            inline const Vector3& getNormal() const
                { return normal_;}

            /**
            @brief Set the offset of the PlaneCollisionShape.
                   If the offset changes, this causes the internal collision shape to be recreated.
            @param offset The offset to be set.
            @return Returns true if the offset has changed, false if not.
            */
            inline bool setOffset(float offset)
                { if(this->offset_ == offset) return false; this->offset_ = offset; updateShape(); return true; }
            /**
            @brief Get the offset of the PlaneCollisionShape.
            @return Returns the offset of the PlaneCollisionShape.
            */
            inline float getOffset() const
                { return this->offset_;}

            virtual void changedScale(); // Is called when the scale of the PlaneCollisionShape has changed.

        private:
            void registerVariables();

            btCollisionShape* createNewShape() const; // Creates a new internal collision shape for the PlaneCollisionShape.

            Vector3 normal_; //!< The normal vector of the PlaneCollisionShape.
            float   offset_; //!< The offset of the PlaneCollisionShape.
     };
}

#endif /* _PlaneCollisionShape_H__ */

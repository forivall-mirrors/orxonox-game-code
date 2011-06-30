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
    @file BoxCollisionShape.h
    @brief Definition of the BoxCollisionShape class.
    @ingroup Collisionshapes
*/

#ifndef _BoxCollisionShape_H__
#define _BoxCollisionShape_H__

#include "objects/ObjectsPrereqs.h"

#include "util/Math.h"
#include "collisionshapes/CollisionShape.h"

namespace orxonox
{

    /**
    @brief
        Wrapper for the bullet box collision shape class btBoxShape.

    @author
        Reto Grieder

    @see btBoxShape
    @ingroup Collisionshapes
    */
    class _ObjectsExport BoxCollisionShape : public CollisionShape
    {
        public:
            BoxCollisionShape(BaseObject* creator);
            virtual ~BoxCollisionShape();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);

            /**
            @brief Set the half extents of the BoxCollisionShape.
                   If the half extent changes, this causes the internal collision shape to be recreated.
            @param extents A vector with the half extents.
                   The x-component is half the length, the y-component is half the height and the z-component is half the width.
            @return Returns true if the half extent has changed, false if not.
            */
            inline bool setHalfExtents(const Vector3& extents)
                { if(this->halfExtents_ == extents) return false; this->halfExtents_ = extents; updateShape(); return true; }
            /**
            @brief Get the half extents of the BoxCollisionShape.
            @return Returns a vector containing the half extents.
            */
            inline const Vector3& getHalfExtents() const
                { return halfExtents_;}

            /**
            @brief Set the width of the BoxCollisionShape.
                   If the width changes, this causes the internal collision shape to be recreated.
            @param value The width to be set.
            @return Returns true if the width has changed, false if not.
            */
            inline bool setWidth(float value)
                { if(this->halfExtents_.z == value/2.0f) return false; this->halfExtents_.z = value / 2.0f; updateShape(); return true; }
            /**
            @brief Get the width of the BoxCollisionShape.
            @return Returns the width of the BoxCollisionShape.
            */
            inline float getWidth() const
                { return this->halfExtents_.z * 2.0f; }

            /**
            @brief Set the height of the BoxCollisionShape.
                   If the height changes, this causes the internal collision shape to be recreated.
            @param value The height to be set.
            @return Returns true if the height has changed, false if not.
            */
            inline bool setHeight(float value)
                { if(this->halfExtents_.y == value/2.0f) return false; this->halfExtents_.y = value / 2.0f; updateShape(); return true; }
            /**
            @brief Get the height of the BoxCollisionShape.
            @return Returns the height of the BoxCollisionShape.
            */
            inline float getHeight() const
                { return this->halfExtents_.y * 2.0f; }

            /**
            @brief Set the length of the BoxCollisionShape.
                   If the length changes, this causes the internal collision shape to be recreated.
            @param value The length to be set.
            @return Returns true if the length has changed, false if not.
            */
            inline bool setLength(float value)
                { if(this->halfExtents_.x == value/2.0f) return false; this->halfExtents_.x = value / 2.0f; updateShape(); return true; }
            /**
            @brief Get the length of the BoxCollisionShape.
            @return Returns the length of the BoxCollisionShape.
            */
            inline float getLength() const
                { return this->halfExtents_.x * 2.0f; }

            virtual void changedScale(); // Is called when the scale of the BoxCollisionShape has changed.

        private:
            void registerVariables();

            btCollisionShape* createNewShape() const; // Creates a new internal collision shape for the BoxCollisionShape.

            Vector3 halfExtents_; //!< The half extents of the BoxCollisionShape. The x-component is half the length, the y-component is half the height and the z-component is half the width.
     };
}

#endif /* _BoxCollisionShape_H__ */

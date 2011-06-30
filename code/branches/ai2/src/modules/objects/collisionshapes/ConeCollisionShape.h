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
    @file ConeCollisionShape.h
    @brief Definition of the ConeCollisionShape class.
    @ingroup Collisionshapes
*/

#ifndef _ConeCollisionShape_H__
#define _ConeCollisionShape_H__

#include "objects/ObjectsPrereqs.h"
#include "collisionshapes/CollisionShape.h"

namespace orxonox
{

    /**
    @brief
        Wrapper for the bullet cone collision shape class btConeShape.

    @author
        Reto Grieder

    @see btConeShape
    @ingroup Collisionshapes
    */
    class _ObjectsExport ConeCollisionShape : public CollisionShape
    {
        public:
            ConeCollisionShape(BaseObject* creator);
            virtual ~ConeCollisionShape();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);

            /**
            @brief Set the radius of the ConeCollisionShape.
                   If the radius changes, this causes the internal collision shape to be recreated.
            @param value The radius to be set.
            @return Returns true if the radius has changed, false if not.
            */
            inline bool setRadius(float value)
                { if(this->radius_ == value) return false; this->radius_ = value; updateShape(); return true; }
            /**
            @brief Get the radius of the ConeCollisionShape.
            @return Returns the radius of the ConeCollisionShape.
            */
            inline float getRadius() const
                { return radius_; }

            /**
            @brief Set the height of the ConeCollisionShape.
                   If the height changes, this causes the internal collision shape to be recreated.
            @param value The height to be set.
            @return Returns true if the height has changed, false if not.
            */
            inline bool setHeight(float value)
                { if(this->height_ == value) return false; this->height_ = value; updateShape(); return true; }
            /**
            @brief Get the height of the ConeCollisionShape.
            @return Returns the height of the ConeCollisionShape.
            */
            inline float getHeight() const
                { return this->height_; }

            virtual void changedScale(); // Is called when the scale of the ConeCollisionShape has changed.

        private:
            void registerVariables();

            btCollisionShape* createNewShape() const; // Creates a new internal collision shape for the ConeCollisionShape.

            float radius_; //!< The radius of the ConeCollisionShape.
            float height_; //!< The height of the ConeCollisionShape.
     };
}

#endif /* _ConeCollisionShape_H__ */

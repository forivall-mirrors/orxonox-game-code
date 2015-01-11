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
 *      Fabian 'x3n' Landau
 *
 */

/**
    @file AbstractRadiusHeightCollisionShape.h
    @brief Definition of the AbstractRadiusHeightCollisionShape class.
    @ingroup Collisionshapes
*/

#ifndef _AbstractRadiusHeightCollisionShape_H__
#define _AbstractRadiusHeightCollisionShape_H__

#include "objects/ObjectsPrereqs.h"
#include "collisionshapes/CollisionShape.h"

namespace orxonox
{

    /**
    @brief
        Wrapper for the bullet collision shapes with radius and height.

    @ingroup Collisionshapes
    */
    class _ObjectsExport AbstractRadiusHeightCollisionShape : public CollisionShape
    {
        public:
            AbstractRadiusHeightCollisionShape(Context* context);

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);

            /**
            @brief Set the radius of the AbstractRadiusHeightCollisionShape.
                   If the radius changes, this causes the internal collision shape to be recreated.
            @param value The radius to be set.
            @return Returns true if the radius has changed, false if not.
            */
            inline bool setRadius(float value)
                { if(this->radius_ == value) return false; this->radius_ = value; updateShape(); return true; }
            /**
            @brief Get the radius of the AbstractRadiusHeightCollisionShape.
            @return Returns the radius of the AbstractRadiusHeightCollisionShape.
            */
            inline float getRadius() const
                { return radius_; }

            /**
            @brief Set the height of the AbstractRadiusHeightCollisionShape.
                   If the height changes, this causes the internal collision shape to be recreated.
            @param value The height to be set.
            @return Returns true if the height has changed, false if not.
            */
            inline bool setHeight(float value)
                { if(this->height_ == value) return false; this->height_ = value; updateShape(); return true; }
            /**
            @brief Get the height of the AbstractRadiusHeightCollisionShape.
            @return Returns the height of the AbstractRadiusHeightCollisionShape.
            */
            inline float getHeight() const
                { return this->height_; }

            virtual void changedScale(); // Is called when the scale of the AbstractRadiusHeightCollisionShape has changed.

        private:
            void registerVariables();

            float radius_; //!< The radius of the AbstractRadiusHeightCollisionShape.
            float height_; //!< The height of the AbstractRadiusHeightCollisionShape.
     };
}

#endif /* _AbstractRadiusHeightCollisionShape_H__ */

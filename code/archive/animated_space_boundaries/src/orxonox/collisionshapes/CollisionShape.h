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
    @file CollisionShape.h
    @brief Definition of the CollisionShape class.
    @ingroup Collisionshapes
*/

#ifndef _CollisionShape_H__
#define _CollisionShape_H__

#include "OrxonoxPrereqs.h"

#include "util/Math.h"
#include "core/BaseObject.h"
#include "network/synchronisable/Synchronisable.h"

namespace orxonox
{

    /**
    @brief
        Wrapper for bullet collision shape class btCollisionShape.

    @author
        Reto Grieder

    @see btCollisionShape
    @ingroup Collisionshapes
    */
    class _OrxonoxExport CollisionShape : public BaseObject, public Synchronisable
    {
        public:
            CollisionShape(BaseObject* creator);
            virtual ~CollisionShape();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);

            /**
            @brief Set the position of the CollisionShape.
                   If the position changes, this causes the parent CompoundCollisionShape (if there is one) to be updated.
            @param position A vector indicating the new position.
            */
            inline void setPosition(const Vector3& position)
                { if(this->position_ == position) return; this->position_ = position; this->updateParent(); }
            /**
            @brief Get the position of the CollisionShape.
            @return Returns the position of the CollisionShape as a vector.
            */
            inline const Vector3& getPosition() const
                { return this->position_; }

            /**
            @brief Set the orientation of the CollisionShape.
                   If the orientation changes, this causes the parent CompoundCollisionShape (if there is one) to be updated.
            @param orientation A quaternion indicating the new orientation.
            */
            inline void setOrientation(const Quaternion& orientation)
                { if(this->orientation_ == orientation) return; this->orientation_ = orientation; this->updateParent(); }
            /**
            @brief Get the orientation of the CollisionShape.
            @return Returns the orientation of the CollisionShape as a quaternion.
            */
            inline const Quaternion& getOrientation() const
                { return this->orientation_; }

            /**
            @brief Rotate the CollisionShape around the y-axis by the specified angle.
                   If the orientation changes, this causes the parent CompoundCollisionShape (if there is one) to be updated.
            @param angle The angle by which the CollisionShape is rotated.
            */
            void yaw(const Degree& angle)
                { this->setOrientation(this->orientation_ * Quaternion(angle, Vector3::UNIT_Y)); }
            /**
            @brief Rotate the CollisionShape around the x-axis by the specified angle.
                   If the orientation changes, this causes the parent CompoundCollisionShape (if there is one) to be updated.
            @param angle The angle by which the CollisionShape is rotated.
            */
            void pitch(const Degree& angle)
                { this->setOrientation(this->orientation_ * Quaternion(angle, Vector3::UNIT_X)); }
            /**
            @brief Rotate the CollisionShape around the z-axis by the specified angle.
                   If the orientation changes, this causes the parent CompoundCollisionShape (if there is one) to be updated.
            @param angle The angle by which the CollisionShape is rotated.
            */
            void roll(const Degree& angle)
                { this->setOrientation(this->orientation_ * Quaternion(angle, Vector3::UNIT_Z)); }

            /**
            @brief Scale the CollisionShape by the input vector.
                   Since the scale is a vector the CollisionShape can be scaled independently in each direction, allowing for linear distortions.
                   If the scale changes, this causes the parent CompoundCollisionShape (if there is one) to be updated.
                   Beware, non-uniform scaling (i.e. distortions) might not be supported by all CollisionShapes.
            @param scale The scaling vector by which the CollisionShape is scaled.
            */
            inline void scale3D(const Vector3& scale)
                { this->setScale3D(this->getScale3D()*scale); }
            void setScale3D(const Vector3& scale); // Set the scale of the CollisionShape.
            /**
            @brief Get the scale of the CollisionShape.
            @return Returns a vector indicating the scale of the CollisionShape.
            */
            inline const Vector3& getScale3D() const
                { return this->scale_; }

            /**
            @brief (Uniformly) scale the CollisionShape by the input scale.
                   If the scale changes, this causes the parent CompoundCollisionShape (if there is one) to be updated.
            @param scale The value by which the CollisionShape is scaled.
            */
            inline void scale(float scale)
                { this->setScale3D(this->getScale3D()*scale); }
            void setScale(float scale); // Set the (uniform) scale of the CollisionShape.
            /**
            @brief Get the (uniform) scale of the CollisionShape.
                   This is only meaningful if the CollisionShape has uniform scaling.
            @return Returns the (uniform) scale of the CollisionShape. Returns 0.0f if the scaling is non-uniform.
            */
            inline float getScale()
                { if(this->hasUniformScaling()) return this->scale_.x; return 0.0f; }

            /**
            @brief Check whether the CollisionShape is uniformly scaled.
            @return Returns true if the CollisionShape is uniformly scaled, false if not.
            */
            inline bool hasUniformScaling()
                { return this->uniformScale_; }
            virtual void changedScale(); // Is called when the scale of the CollisionShape has changed.

            void updateShape(); // Updates the shape.

            void calculateLocalInertia(float mass, btVector3& inertia) const; // Calculates the local inertia of the collision shape.

            /**
            @brief Get the bullet collision shape of this CollisionShape.
            @return Returns a pointer to the bullet collision shape of this CollisionShape.
            */
            inline btCollisionShape* getCollisionShape() const
                { return this->collisionShape_; }

            bool hasTransform() const; // Check whether the CollisionShape has been either moved or rotated or both.

            bool notifyBeingAttached(CompoundCollisionShape* newParent); // Notifies the CollisionShape of being attached to a CompoundCollisionShape.
            void notifyDetached(); // Notifies the CollisionShape of being detached from a CompoundCollisionShape.

        protected:
            virtual void updateParent(); // Updates the CompoundCollisionShape the CollisionShape belongs to, after the CollisionShape has changed.
            virtual void parentChanged(); // Is called when the parentID of the CollisionShape has changed.

            /**
            @brief Create a new bullet collision shape depending on the internal parameters of the specific CollisionShape.
            @return Returns a pointer to the new bullet collision shape.
            */
            virtual btCollisionShape* createNewShape() const = 0;

            btCollisionShape*       collisionShape_; //!< The bullet collision shape of this CollisionShape.
            CompoundCollisionShape* parent_; //!< The CompoundCollisionShape this CollisionShape belongs to, NULL if it doesn't belong to one.
            unsigned int            parentID_; //!< The objectID of the parent of this CollisionShape, which can either be a CompoundCollisionShape or a WorldEntity.

        private:
            void registerVariables();

            Vector3                 position_; //!< The position of the CollisionShape.
            Quaternion              orientation_; //!< The orientation of the CollisionShape.
            Vector3                 scale_; //!< The scale of the CollisionShape.
            bool                    uniformScale_; //!< Whether the scale is uniform.
    };
}

#endif /* _CollisionShape_H__ */

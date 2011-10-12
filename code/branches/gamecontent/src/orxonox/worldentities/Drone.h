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
 *      Oli Scheuss
 *   Co-authors:
 *      ...
 *
 */

#ifndef _Drone_H__
#define _Drone_H__

#include "OrxonoxPrereqs.h"
#include "worldentities/pawns/Pawn.h"
#include "controllers/DroneController.h"

namespace orxonox
{

    /**
    @brief
        Drone, that is made to move upon a specified pattern.
        This class was constructed for the PPS tutorial.
    @author
        Oli Scheuss
    */
    class _OrxonoxExport Drone : public Pawn
    {
        public:
            Drone(BaseObject* creator);
            virtual ~Drone();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode); //!< Method for creating a Drone through XML.
            virtual void tick(float dt); //!< Defines which actions the Drone has to take in each tick.


            virtual void moveFrontBack(const Vector2& value);
            virtual void moveRightLeft(const Vector2& value);
            virtual void moveUpDown(const Vector2& value);

            virtual void rotateYaw(const Vector2& value);
            virtual void rotatePitch(const Vector2& value);
            virtual void rotateRoll(const Vector2& value);

            /**
            @brief Moves the Drone in the Front/Back-direction by the specifed amount.
            @param value  The amount by which the drone is to be moved.
            */
            inline void moveFrontBack(float value)
            { this->moveFrontBack(Vector2(value, 0)); }
            /**
            @brief Moves the Drone in the Right/Left-direction by the specifed amount.
            @param value  The amount by which the drone is to be moved.
            */
            inline void moveRightLeft(float value)
            { this->moveRightLeft(Vector2(value, 0)); }
            /**
            @brief Moves the Drone in the Up/Down-direction by the specifed amount.
            @param value  The amount by which the drone is to be moved.
            */
            inline void moveUpDown(float value)
            { this->moveUpDown(Vector2(value, 0)); }

            /**
            @brief Rotates the Drone around the y-axis by the specifed amount.
            @param value  The amount by which the drone is to be rotated.
            */
            inline void rotateYaw(float value)
            { this->rotateYaw(Vector2(value, 0)); }
            /**
            @brief Rotates the Drone around the x-axis by the specifed amount.
            @param value  The amount by which the drone is to be rotated.
            */
            inline void rotatePitch(float value)
            { this->rotatePitch(Vector2(value, 0)); }
            /**
            @brief Rotates the Drone around the z-axis by the specifed amount.
            @param value  The amount by which the drone is to be rotated.
            */
            inline void rotateRoll(float value)
            { this->rotateRoll(Vector2(value, 0)); }

            /**
            @brief Sets the primary thrust to the input amount.
            @param thrust The amount of thrust.
            */
            inline void setPrimaryThrust( float thrust )
                { this->primaryThrust_=thrust; }
            inline void setAuxilaryThrust( float thrust )
                { this->auxilaryThrust_=thrust; }
            inline void setRotationThrust( float thrust )
                { this->rotationThrust_=thrust; }
            inline void setMaxDistanceToOwner( float distance)
                { this->maxDistanceToOwner_=distance; }
            inline void setMinDistanceToOwner( float distance)
                { this->minDistanceToOwner_=distance; }
            inline void setMaxShootingRange( float distance)
                { this->maxShootingRange_=distance; }


            /**
            @brief Gets the primary thrust to the input amount.
            @return The amount of thrust.
            */
            inline float getPrimaryThrust()
                { return this->primaryThrust_; }
            inline float getAuxilaryThrust()
                { return this->auxilaryThrust_; }
            inline float getRotationThrust()
                { return this->rotationThrust_; }
            inline float getMaxDistanceToOwner()
                { return this->maxDistanceToOwner_; }
            inline float getMinDistanceToOwner()
                { return this->minDistanceToOwner_; }
            inline float getMaxShootingRange()
                { return this->maxShootingRange_; }

        private:
            DroneController *myController_; //!< The controller of the Drone.

            btVector3 localLinearAcceleration_; //!< The linear acceleration that is used to move the Drone the next tick.
            btVector3 localAngularAcceleration_; //!< The linear angular acceleration that is used to move the Drone the next tick.
            float primaryThrust_; //!< The amount of primary thrust. This is just used, when moving forward.
            float auxilaryThrust_; //!< The amount of auxilary thrust. Used for all other movements (except for rotations).
            float rotationThrust_; //!< The amount of rotation thrust. Used for rotations only.
            float maxDistanceToOwner_; //Maximum Distance to owner
            float minDistanceToOwner_; //Minimum Distance to owner
            float maxShootingRange_;
    };

}

#endif /* _Drone_H__ */

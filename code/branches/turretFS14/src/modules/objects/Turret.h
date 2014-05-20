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
 *      Marian Runo, Martin Mueller
 *   Co-authors:
 *      ...
 *
 */

/**
    @brief Definition of the Turret class.
    @ingroup Objects
*/

#ifndef _Turret_H__
#define _Turret_H__

#include "objects/ObjectsPrereqs.h"
#include "worldentities/pawns/Pawn.h"
#include <OgreSceneQuery.h>

namespace orxonox
{
    /**
    @brief
        Creates a turret with limited rotation. The point of this class is to be able to attach
        a turret to a spaceship or a spacestation which is more or less completely autonomous in
        it's behaviour.

        This class also contains a custom local coordinate system, which gets initially rotated through xml, and
        afterwards is updated with the parent's rotation (if there is one). This allows for almost trivialal calculation
        of pitch, yaw and roll through coordinate transformation. (TODO: Ogre should do something like this already, investigate...)
        

    @note
        The rotation isn't limited "physically". You have to call isInRange to find out if the turret is allowed to shoot at a target.
    */
    class _ObjectsExport Turret : public Pawn
    {
        public:
            Turret(Context* context);
            virtual ~Turret();

            virtual void rotatePitch(const Vector2& value);
            virtual void rotateYaw(const Vector2& value);
            virtual void rotateRoll(const Vector2& value);
            virtual float isInRange(const WorldEntity* target);
            virtual void aimAtPosition(const Vector3 &position);

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);
            virtual void tick(float dt);

            /** @brief Sets the maximum distance the turret is allowed to shoot. @param radius The distance*/
            inline void setMaxAttackRadius(float radius)
                { this->maxAttackRadius_ = radius; }

            /** @brief Sets the minimum distance the turret is allowed to shoot. @param radius The distance*/
            inline void setMinAttackRadius(float radius)
                { this->minAttackRadius_ = radius; }

            /** @brief Sets the maximum pitch the turret can have (in both directions). @param pitch The pitch (in one direction)*/
            inline void setMaxPitch(float pitch)
                { this->maxPitch_ = pitch; }

            /** @brief Sets the maximum yaw the turret can have (in both directions). @param yaw The yaw (in one direction)*/
            inline void setMaxYaw(float yaw)
                { this->maxYaw_ = yaw; }

            /** @brief Returns the maximum distance the turret is allowed to shoot. @return The distance */
            inline float getMaxAttackRadius() const
                { return this->maxAttackRadius_; }                

            /** @brief Returns the minimum distance the turret is allowed to shoot. @return The distance */
            inline float getMinAttackRadius() const
                { return this->minAttackRadius_; }   

            /** @brief Returns the maximum pitch the turret can have. @return The pitch */
            inline float getMaxPitch() const
                { return this->maxPitch_; }

            /** @brief Returns the maximum yaw the turret can have. @return The yaw */
            inline float getMaxYaw() const
                { return this->maxYaw_; }

        protected:
            Vector3 startDir_; //!< The initial facing direction, in local coordinates.
            Vector3 localZ_; //!< The local z-axis, includes for the parent's rotation and rotations done in xml.
            Vector3 localY_; //!< The local y-axis, includes for the parent's rotation and rotations done in xml.
            Vector3 localX_; //!< The local x-axis, includes for the parent's rotation and rotations done in xml.      
            Quaternion rotation_; //!< The rotation to be done by the turret.

        private:
            bool once_; //!< Flag for executing code in the tick function only once.

            Vector3 localZStart_; //!< The local z-axis, without the parent's rotation.
            Vector3 localYStart_; //!< The local y-axis, without the parent's rotation.
            Vector3 localXStart_; //!< The local x-axis, without the parent's rotation.
            float maxAttackRadius_; //!< The maximum distance the turret is allowed to shoot.
            float minAttackRadius_; //!< The minimum distance the turret is allowed to shoot.
            Ogre::Real maxPitch_; //!< The maxmium pitch the turret can have (on one side).
            Ogre::Real maxYaw_; //!< The maxmium yaw the turret can have (on one side).
            float rotationThrust_;  //!< The velocity the turret rotates with.
            Ogre::RaySceneQuery* rayTest_; //!< Used to perform a raytest, currently unused @see isInRange

    };
}

#endif

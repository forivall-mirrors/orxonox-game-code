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
 *      Gabriel Nadler
 *   Co-authors:
 *      simonmie
 *
 */

/**
    @file SimpleRocket.h
    @brief Definition of the SimpleRocket class.
*/

#ifndef _SimpleRocket_H__
#define _SimpleRocket_H__

#include "weapons/WeaponsPrereqs.h"

#include "tools/Timer.h"

#include "graphics/ParticleSpawner.h"
#include "interfaces/RadarViewable.h"
#include "worldentities/ControllableEntity.h"

#include "BasicProjectile.h"

namespace orxonox
{
    class ConeCollisionShape;

    /**
    @brief
        SimpleRocket is a target seeking, intelligent rocket. It follows its target until it either hits something or runs out of fuel.
        The steering is done by the RocketController.
    @author
       Gabriel Nadler
    @ingroup WeaponsProjectiles
    */
    class _WeaponsExport SimpleRocket : public ControllableEntity, public BasicProjectile, public RadarViewable
    {
        public:
            SimpleRocket(Context* context);
            virtual ~SimpleRocket();
            virtual void tick(float dt);

            virtual bool collidesAgainst(WorldEntity* otherObject, const btCollisionShape* cs, btManifoldPoint& contactPoint);

            void disableFire(); //!< Method to disable the fire and stop all acceleration

            virtual void moveFrontBack(const Vector2& value){}
            virtual void moveRightLeft(const Vector2& value){}
            virtual void moveUpDown(const Vector2& value){}

            virtual void rotateYaw(const Vector2& value);
            virtual void rotatePitch(const Vector2& value);
            virtual void rotateRoll(const Vector2& value);
            void setDestroy();

            /**
            @brief Moves the SimpleRocket in the Front/Back-direction by the specifed amount.
            @param value  The amount by which the SimpleRocket is to be moved.
            */
            inline void moveFrontBack(float value)
                { this->moveFrontBack(Vector2(value, 0)); }
            /**
            @brief Moves the SimpleRocket in the Right/Left-direction by the specifed amount.
            @param value  The amount by which the SimpleRocket is to be moved.
            */
            inline void moveRightLeft(float value)
                { this->moveRightLeft(Vector2(value, 0)); }
            /**
            @brief Moves the SimpleRocket in the Up/Down-direction by the specifed amount.
            @param value  The amount by which the SimpleRocket is to be moved.
            */
            inline void moveUpDown(float value)
                { this->moveUpDown(Vector2(value, 0)); }

            /**
            @brief Rotates the SimpleRocket around the y-axis by the specifed amount.
            @param value  The amount by which the SimpleRocket is to be rotated.
            */
            inline void rotateYaw(float value)
                { this->rotateYaw(Vector2(value, 0)); }
            /**
            @brief Rotates the SimpleRocket around the x-axis by the specifed amount.
            @param value  The amount by which the SimpleRocket is to be rotated.
            */
            inline void rotatePitch(float value)
                { this->rotatePitch(Vector2(value, 0)); }
            /**
            @brief Rotates the SimpleRocket around the z-axis by the specifed amount.
            @param value  The amount by which the SimpleRocket is to be rotated.
            */
            inline void rotateRoll(float value)
                { this->rotateRoll(Vector2(value, 0)); }

            virtual void setShooter(Pawn* shooter);

            inline bool hasFuel() const
                { return this->fuel_; }


        private:
            static const float FUEL_PERCENTAGE; //!< Percentage of lifetime the rocket has fuel

            Vector3 localAngularVelocity_; //!< Variable to temporarily store accumulated steering command input.
            bool fuel_; //!< Bool is true while the rocket "has fuel"

            WeakPtr<PlayerInfo> player_; //!< The player the SimpleRocket belongs to.
            Timer destroyTimer_; //!< Timer to destroy the projectile after its lifetime has run out.
            float lifetime_; //!< The time the projectile exists.

            ParticleEmitter* fire_; //!< Fire-Emittor
    };

}

#endif /* _SimpleRocket_H__ */

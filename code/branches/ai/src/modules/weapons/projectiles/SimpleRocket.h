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
 *      Oliver Scheuss
 *   Co-authors:
 *      ...
 *
 */

#ifndef _SimpleRocket_H__
#define _SimpleRocket_H__

#include "weapons/WeaponsPrereqs.h"

#include "tools/Timer.h"
#include "worldentities/ControllableEntity.h"
#include "graphics/ParticleSpawner.h"

namespace orxonox
{
    class ConeCollisionShape;

    /**
    @brief
        SimpleRocket, follows direction from a Rocketcontroller, has fuel for 80% of its lifetime, afterwards it's fire disappears.
    @author
       Gabriel Nadler (Original file: Oli Scheuss)
    */
    class _WeaponsExport SimpleRocket : public ControllableEntity
    {
        public:
            SimpleRocket(BaseObject* creator);
            virtual ~SimpleRocket();
            virtual void tick(float dt);
            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode); //!< Method for creating a SimpleRocket through XML.

            virtual bool collidesAgainst(WorldEntity* otherObject, btManifoldPoint& contactPoint);
            void destroyObject();

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
            {
                this->rotatePitch(Vector2(value, 0)); }
            /**
            @brief Rotates the SimpleRocket around the z-axis by the specifed amount.
            @param value  The amount by which the SimpleRocket is to be rotated.
            */
            inline void rotateRoll(float value)
            {
                this->rotateRoll(Vector2(value, 0)); }

            void setOwner(Pawn* owner);
            inline Pawn* getOwner() const
                { return this->owner_; }
            inline bool hasFuel() const
            { return this->fuel_; }

            inline void setDamage(float damage)
                { this->damage_ = damage; }
            inline float getDamage() const
                { return this->damage_; }


        private:
            WeakPtr<Pawn> owner_;
            Vector3 localAngularVelocity_;
            float damage_;
            bool bDestroy_;
            bool fuel_; //!< Bool is true while the rocket "has fuel"


            WeakPtr<PlayerInfo> player_;
            Timer destroyTimer_;
            float lifetime_;
            static const int FUEL_PERCENTAGE=80; //!<Percentage of Lifetime the rocket has fuel

            ParticleEmitter* fire_; //!< Fire-Emittor



    };

}

#endif /* _SimpleRocket_H__ */

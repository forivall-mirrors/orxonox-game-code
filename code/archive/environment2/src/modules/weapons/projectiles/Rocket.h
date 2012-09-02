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

#ifndef _Rocket_H__
#define _Rocket_H__

#include "weapons/WeaponsPrereqs.h"

#include "tools/Timer.h"
#include "worldentities/ControllableEntity.h"

namespace orxonox
{
    class ConeCollisionShape;

    /**
    @brief
        Rocket, that is made to move upon a specified pattern.
        This class was constructed for the PPS tutorial.
    @author
        Oli Scheuss
    */
    class _WeaponsExport Rocket : public ControllableEntity
    {
        public:
            Rocket(BaseObject* creator);
            virtual ~Rocket();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode); //!< Method for creating a Rocket through XML.
            virtual void tick(float dt); //!< Defines which actions the Rocket has to take in each tick.

            virtual bool collidesAgainst(WorldEntity* otherObject, btManifoldPoint& contactPoint);
            void destroyObject();
            void destructionEffect();

            virtual void moveFrontBack(const Vector2& value){}
            virtual void moveRightLeft(const Vector2& value){}
            virtual void moveUpDown(const Vector2& value){}

            virtual void rotateYaw(const Vector2& value);
            virtual void rotatePitch(const Vector2& value);
            virtual void rotateRoll(const Vector2& value);

            /**
            @brief Moves the Rocket in the Front/Back-direction by the specifed amount.
            @param value  The amount by which the Rocket is to be moved.
            */
            inline void moveFrontBack(float value)
            { this->moveFrontBack(Vector2(value, 0)); }
            /**
            @brief Moves the Rocket in the Right/Left-direction by the specifed amount.
            @param value  The amount by which the Rocket is to be moved.
            */
            inline void moveRightLeft(float value)
            { this->moveRightLeft(Vector2(value, 0)); }
            /**
            @brief Moves the Rocket in the Up/Down-direction by the specifed amount.
            @param value  The amount by which the Rocket is to be moved.
            */
            inline void moveUpDown(float value)
            { this->moveUpDown(Vector2(value, 0)); }

            /**
            @brief Rotates the Rocket around the y-axis by the specifed amount.
            @param value  The amount by which the Rocket is to be rotated.
            */
            inline void rotateYaw(float value)
            { this->rotateYaw(Vector2(value, 0)); }
            /**
            @brief Rotates the Rocket around the x-axis by the specifed amount.
            @param value  The amount by which the Rocket is to be rotated.
            */
            inline void rotatePitch(float value)
            { this->rotatePitch(Vector2(value, 0)); }
            /**
            @brief Rotates the Rocket around the z-axis by the specifed amount.
            @param value  The amount by which the Rocket is to be rotated.
            */
            inline void rotateRoll(float value)
            { this->rotateRoll(Vector2(value, 0)); }

            void setOwner(Pawn* owner);
            inline Pawn* getOwner() const
                { return this->owner_; }

            inline void setDamage(float damage)
                { this->damage_ = damage; }
            inline float getDamage() const
                { return this->damage_; }
            virtual void fired(unsigned int firemode);

        private:
            WeakPtr<Pawn> owner_;
            Vector3 localAngularVelocity_;
            float damage_;
            bool bDestroy_;

            WeakPtr<PlayerInfo> player_;
            Timer destroyTimer_;
            float lifetime_;

            WorldSound* defSndWpnEngine_;
            WorldSound* defSndWpnLaunch_;
    };

}

#endif /* _Rocket_H__ */

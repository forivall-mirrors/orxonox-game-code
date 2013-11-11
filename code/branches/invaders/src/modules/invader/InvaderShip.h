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
 *      Florian Zinggeler
 *   Co-authors:
 *      ...
 *
 */

/**
    @file InvaderShip.h
    @brief Declaration of the InvaderShip class.
*/

#ifndef _InvaderShip_H__
#define _InvaderShip_H__

#include "invader/InvaderPrereqs.h"

#include "worldentities/pawns/SpaceShip.h"
#include "graphics/Camera.h"

namespace orxonox
{
    class _InvaderExport InvaderShip : public SpaceShip
    {
        public:
            InvaderShip(Context* context); //!< Constructor. Registers and initializes the object.
            virtual ~InvaderShip() {}

            virtual void tick(float dt);

            virtual void moveFrontBack(const Vector2& value);
            virtual void moveRightLeft(const Vector2& value);

            virtual void boost(bool bBoost); // Starts or stops fireing

            //no rotation!
            virtual void rotateYaw(const Vector2& value){}; 
            virtual void rotatePitch(const Vector2& value){};
            virtual void rotateRoll(const Vector2& value){};

            // *InvaderShip getShip(){return this;}
        private:
            Camera* camera;
            bool isFireing;
            float speed, damping;
            float lastTimeFront, lastTimeLeft;
            struct Velocity
            {
                float x;
                float y;
            } velocity, desiredVelocity;

    };
}

#endif /* _InvaderShip_H__ */

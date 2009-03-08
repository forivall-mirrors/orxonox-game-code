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
 *      Fabian 'x3n' Landau
 *   Co-authors:
 *      ...
 *
 */

#ifndef _Drone_H__
#define _Drone_H__

#include "OrxonoxPrereqs.h"
#include "objects/worldentities/ControllableEntity.h"
#include "objects/controllers/DroneController.h"

namespace orxonox
{
    class _OrxonoxExport Drone : public ControllableEntity
    {
        public:
            Drone(BaseObject* creator);
            virtual ~Drone();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);
            virtual void tick(float dt);

            
            virtual void moveFrontBack(const Vector2& value);
            virtual void moveRightLeft(const Vector2& value);
            virtual void moveUpDown(const Vector2& value);

            virtual void rotateYaw(const Vector2& value);
            virtual void rotatePitch(const Vector2& value);
            virtual void rotateRoll(const Vector2& value);
            
            
            inline void moveFrontBack(float value)
            { this->moveFrontBack(Vector2(value, 0)); }
            inline void moveRightLeft(float value)
            { this->moveRightLeft(Vector2(value, 0)); }
            inline void moveUpDown(float value)
            { this->moveUpDown(Vector2(value, 0)); }
            
            inline void rotateYaw(float value)
            { this->rotateYaw(Vector2(value, 0)); }
            inline void rotatePitch(float value)
            { this->rotatePitch(Vector2(value, 0)); }
            inline void rotateRoll(float value)
            { this->rotateRoll(Vector2(value, 0)); }
            
        private:
            DroneController *myController_;
            
            Vector3 steering_;
            btVector3 localLinearAcceleration_;
            btVector3 localAngularAcceleration_;
            float primaryThrust_;
            float auxilaryThrust_;
            float rotationThrust_;
    };

}

#endif /* _Drone_H__ */

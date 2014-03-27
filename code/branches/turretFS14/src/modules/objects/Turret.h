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
 *      Marian Runo
 *   Co-authors:
 *      ...
 *
 */

/**
    @file Turret.h
    @brief Definition of the Turret class.
    @ingroup Objects
*/

#ifndef _Turret_H__
#define _Turret_H__

#include "objects/ObjectsPrereqs.h"
#include "OgreQuaternion.h"

#include "worldentities/pawns/SpaceShip.h"

namespace orxonox
{
    class _ObjectsExport Turret : public SpaceShip
    {
        public:
            Turret(Context* context);
            virtual ~Turret();

            virtual void rotatePitch(const Vector2& value);
            virtual void rotateYaw(const Vector2& value);
            virtual void rotateRoll(const Vector2& value);

            void setAlertnessRadius(float value);
            float getAlertnessRadius();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);
            virtual void tick(float dt);


        protected:
            WaypointPatrolController* controller_;
        private:
            bool gotOrient_;
            Quaternion startOrient_;
            Quaternion firstOrient_;
    };
}

#endif


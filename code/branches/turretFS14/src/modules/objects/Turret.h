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
    class _ObjectsExport Turret : public Pawn
    {
        public:
            Turret(Context* context);
            virtual ~Turret();

            virtual void rotatePitch(const Vector2& value);
            virtual void rotateYaw(const Vector2& value);
            virtual void rotateRoll(const Vector2& value);

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);
            virtual void tick(float dt);

            inline void setMaxPitch(Ogre::Real pitch)
                {this->maxPitch_ = pitch;}

            inline Ogre::Real getMaxPitch()
                {return this->maxPitch_;}

            inline void setMaxYaw(Ogre::Real yaw)
                {this->maxYaw_ = yaw;}

            inline Ogre::Real getMaxYaw()
                {return this->maxYaw_;}

        private:
            bool gotOrient_;
            Ogre::Real maxPitch_;
            Ogre::Real maxYaw_;
            Quaternion startOrientInv_;
            float rotationThrust_;

            btVector3 localAngularAcceleration_;

            Ogre::Real boundBetween(float val, float lowerBound, float upperBound);
    };
}

#endif


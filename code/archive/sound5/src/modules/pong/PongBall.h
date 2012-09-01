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

#ifndef _PongBall_H__
#define _PongBall_H__

#include "pong/PongPrereqs.h"

#include "util/Math.h"
#include "worldentities/MovableEntity.h"

namespace orxonox
{
    class _PongExport PongBall : public MovableEntity
    {
        public:
            PongBall(BaseObject* creator);
            virtual ~PongBall();

            virtual void tick(float dt);

            void registerVariables();

            void setFieldDimension(float width, float height)
                { this->fieldWidth_ = width; this->fieldHeight_ = height; }
            void setFieldDimension(const Vector2& dimension)
                { this->setFieldDimension(dimension.x, dimension.y); }
            Vector2 getFieldDimension() const
                { return Vector2(this->fieldWidth_, this->fieldHeight_); }

            void setSpeed(float speed);
            float getSpeed() const
                { return this->speed_; }

            void setAccelerationFactor(float factor)
                { this->accelerationFactor_ = factor; }
            float getAccelerationFactor() const
                { return this->accelerationFactor_; }

            void setBatLength(float batlength)
                { this->batlength_ = batlength; }
            float getBatLength() const
                { return this->batlength_; }

            void setBats(PongBat** bats);
            void applyBats();

            static const float MAX_REL_Z_VELOCITY;

        private:
            float fieldWidth_;
            float fieldHeight_;
            float speed_;
            float accelerationFactor_;
            float batlength_;
            PongBat** bat_;
            unsigned int* batID_;
            float relMercyOffset_;
    };
}

#endif /* _PongBall_H__ */

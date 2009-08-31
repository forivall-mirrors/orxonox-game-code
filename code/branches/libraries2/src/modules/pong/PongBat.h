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

#ifndef _PongBat_H__
#define _PongBat_H__

#include "pong/PongPrereqs.h"
#include "worldentities/ControllableEntity.h"

namespace orxonox
{
    class _PongExport PongBat : public ControllableEntity
    {
        public:
            PongBat(BaseObject* creator);
            virtual ~PongBat() {}

            void registerVariables();
            virtual void tick(float dt);

            virtual void moveFrontBack(const Vector2& value);
            virtual void moveRightLeft(const Vector2& value);

            virtual void changedPlayer();

            void setSpeed(float speed)
                { this->speed_ = speed; }
            float getSpeed() const
                { return this->speed_; }

            void setFieldHeight(float height)
                { this->fieldHeight_ = height; }
            float getFieldHeight() const
                { return this->fieldHeight_; }

            void setLength(float length)
                { this->length_ = length; }
            float getLength() const
                { return this->length_; }

        private:
            float movement_;
            bool bMoveLocal_;
            float speed_;
            float length_;
            float fieldHeight_;
            bool bSteadiedPosition_;
    };
}

#endif /* _PongBat_H__ */

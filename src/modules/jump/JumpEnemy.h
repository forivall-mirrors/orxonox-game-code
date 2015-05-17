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
 *      Fabien Vultier
 *   Co-authors:
 *      ...
 *
 */

/**
    @file JumpEnemy.h
    @brief Declaration of the JumpEnemy class.
    @ingroup Jump
*/

#ifndef _JumpEnemy_H__
#define _JumpEnemy_H__

#include "jump/JumpPrereqs.h"
#include "worldentities/MovableEntity.h"

namespace orxonox
{
    class _JumpExport JumpEnemy : public MovableEntity
    {
        public:
            JumpEnemy(Context* context);
            virtual ~JumpEnemy();
            virtual void tick(float dt);
            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);
            void setFieldDimension(float width, float height)
                { this->fieldWidth_ = width; this->fieldHeight_ = height; }
            void setFieldDimension(const Vector2& dimension)
                { this->setFieldDimension(dimension.x, dimension.y); }
            Vector2 getFieldDimension() const
                { return Vector2(this->fieldWidth_, this->fieldHeight_); }
            void setWidth(const float width)
                { this->width_ = width; }
            float getWidth() const
                { return width_; }
            void setHeight(const float height)
                { this->height_ = height; }
            float getHeight() const
                { return height_; }

            virtual void setProperties(float newLeftBoundary, float newRightBoundary, float newLowerBoundary, float newUpperBoundary, float newHSpeed, float newVSpeed);
            void setFigure(WeakPtr<JumpFigure> bats);
            virtual void touchFigure();
            bool dead_;
        protected:
            float width_;
            float height_;
            float fieldWidth_;
            float fieldHeight_;
            float leftBoundary_;
            float rightBoundary_;
            float lowerBoundary_;
            float upperBoundary_;
            WeakPtr<JumpFigure> figure_;
    };
}

#endif /* _JumpEnemy_H__ */

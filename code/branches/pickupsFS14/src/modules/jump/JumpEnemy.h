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

/**
    @file JumpEnemy.h
    @brief Declaration of the JumpEnemy class.
    @ingroup Jump
*/

#ifndef _JumpEnemy_H__
#define _JumpEnemy_H__

#include "jump/JumpPrereqs.h"

#include "util/Math.h"

#include "worldentities/MovableEntity.h"


namespace orxonox
{

    /**
    @brief
        This class manages the ball for @ref orxonox::Jump "Jump".

        It is responsible for both the movement of the ball in the x,z-plane as well as its interaction with the boundaries of the playing field (defined by the @ref orxonox::JumpCenterpoint "JumpCenterpoint") and the @ref orxonox::JumpFigure "JumpFigures". Or more precisely, it makes the ball bounce off then upper and lower delimiters of the playing field, it makes the ball bounce off the bats and also detects when a player scores and takes appropriate measures.

    @author
        Fabian 'x3n' Landau

    @ingroup Jump
    */
    class _JumpExport JumpEnemy : public MovableEntity
    {
        public:
            JumpEnemy(Context* context);
            virtual ~JumpEnemy();

            virtual void tick(float dt);

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);

            /**
            @brief Set the dimensions of the playing field.
            @param width The width of the playing field.
            @param height The height of the playing field.
            */
            void setFieldDimension(float width, float height)
                { this->fieldWidth_ = width; this->fieldHeight_ = height; }
            /**
            @brief Get the dimensions of the playing field.
            @param dimension A vector with the width as the first and height as the second component.
            */
            void setFieldDimension(const Vector2& dimension)
                { this->setFieldDimension(dimension.x, dimension.y); }
            /**
            @brief Get the dimensions of the playing field.
            @return Returns a vector with the width as the first and height as the second component.
            */
            Vector2 getFieldDimension() const
                { return Vector2(this->fieldWidth_, this->fieldHeight_); }

            virtual void setProperties(float newLeftBoundary, float newRightBoundary, float newLowerBoundary, float newUpperBoundary, float newHSpeed, float newVSpeed);

            void setFigure(WeakPtr<JumpFigure> bats); //!< Set the bats for the ball.

            virtual void touchFigure();

        protected:
            float fieldWidth_; //!< The width of the playing field.
            float fieldHeight_; //!< The height of the playing field.

            float leftBoundary_;
            float rightBoundary_;
            float lowerBoundary_;
            float upperBoundary_;

            WeakPtr<JumpFigure> figure_; //!< An array with the two bats.
    };
}

#endif /* _JumpEnemy_H__ */

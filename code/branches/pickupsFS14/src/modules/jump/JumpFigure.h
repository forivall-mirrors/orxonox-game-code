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
    @file JumpFigure.h
    @brief Declaration of the JumpFigure class.
    @ingroup Jump
*/

#ifndef _JumpFigure_H__
#define _JumpFigure_H__

#include "jump/JumpPrereqs.h"

#include "worldentities/ControllableEntity.h"

namespace orxonox
{

    /**
    @brief
        The JumpFigure class manages the bats for @ref orxonox::Jump "Jump", which are the elements controlled by the players.

        It is responsible for the movement (controlled by the players) of the bat.

    @author
        Fabian 'x3n' Landau

    @ingroup Jump
    */
    class _JumpExport JumpFigure : public ControllableEntity
    {
        public:
            JumpFigure(Context* context); //!< Constructor. Registers and initializes the object.
            virtual ~JumpFigure() {}

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);

            virtual void tick(float dt);

            virtual void moveFrontBack(const Vector2& value); //!< Overloaded the function to steer the bat up and down.
            virtual void moveRightLeft(const Vector2& value); //!< Overloaded the function to steer the bat up and down.

            virtual void rotateYaw(const Vector2& value);
            virtual void rotatePitch(const Vector2& value);
            virtual void rotateRoll(const Vector2& value);

            virtual void fired(unsigned int firemode);

            virtual void JumpFromPlatform(JumpPlatform* platform);

            /**
            @brief Set the height of the playing field.
            @param height The height of the playing field.
            */
            void setFieldDimension(float width, float height)
                { this->fieldWidth_ = width; this->fieldHeight_ = height; }

            void setFieldDimension(const Vector2& dimension)
                { this->setFieldDimension(dimension.x, dimension.y); }

            void setMouseFactor(const float mouseFactor)
                { this->mouseFactor_ = mouseFactor; }

            const float getMouseFactor() const
                { return this->mouseFactor_; }

            /**
            @brief Get the height of the playing field.
            @return Returns the height of the playing field.
            */
            Vector2 getFieldDimension() const
                { return Vector2(this->fieldWidth_, this->fieldHeight_); }

            /**
            @brief Set the length of the bat.
            @param length The length of the bat (in z-direction) as percentage of the height of the playing field.
            */
            void setLength(float length)
                { this->length_ = length; }
            /**
            @brief Get the length of the bat.
            @return Returns the length of the bat (in z-direction) as percentage of the height of the playing field.
            */
            float getLength() const
                { return this->length_; }

            bool fireSignal;
        private:
            float movement_; //!< The amount (and direction), in z-direction, of movement of the bat.
            bool bMoveLocal_; //!< Helper to know whether the movement is caused by moveFrontBack() or moveRightLeft().
            float length_; //!< The length of the bat (in z-direction) as percentage of the height of the playing field.
            float fieldWidth_; //!< The height of the playing field.
            float fieldHeight_;
            bool bSteadiedPosition_; //!< Helper boolean, to keep track of when to steady the position, to ensure network synchronicity.
            float timeSinceLastFire;

            bool moveUpPressed;
            bool moveDownPressed;
            bool moveLeftPressed;
            bool moveRightPressed;
            bool firePressed;

            float gravityAcceleration;
            float mouseFactor_;
            float maxFireRate;

            float horizontalSpeed;
    };
}

#endif /* _JumpFigure_H__ */

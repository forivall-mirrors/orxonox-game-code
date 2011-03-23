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
    @file PongBat.h
    @brief Declaration of the PongBat class.
    @ingroup Pong
*/

#ifndef _PongBat_H__
#define _PongBat_H__

#include "pong/PongPrereqs.h"

#include "worldentities/ControllableEntity.h"

namespace orxonox
{

    /**
    @brief
        The PongBat class manages the bats for @ref orxonox::Pong "Pong", which are the elements controlled by the players.

        It is responsible for the movement (controlled by the players) of the bat.

    @author
        Fabian 'x3n' Landau

    @ingroup Pong
    */
    class _PongExport PongBat : public ControllableEntity
    {
        public:
            PongBat(BaseObject* creator); //!< Constructor. Registers and initializes the object.
            virtual ~PongBat() {}

            virtual void tick(float dt);

            virtual void moveFrontBack(const Vector2& value); //!< Overloaded the function to steer the bat up and down.
            virtual void moveRightLeft(const Vector2& value); //!< Overloaded the function to steer the bat up and down.

            virtual void changedPlayer(); //!< Is called when the player changed.

            /**
            @brief Set the speed of the bat.
            @param speed The speed to be set.
            */
            void setSpeed(float speed)
                { this->speed_ = speed; }
            /**
            @brief Get the speed of the bat.
            @return Returns the speed of the bat.
            */
            float getSpeed() const
                { return this->speed_; }

            /**
            @brief Set the height of the playing field.
            @param height The height of the playing field.
            */
            void setFieldHeight(float height)
                { this->fieldHeight_ = height; }
            /**
            @brief Get the height of the playing field.
            @return Returns the height of the playing field.
            */
            float getFieldHeight() const
                { return this->fieldHeight_; }

            /**
            @brief Set the length of the bat.
            @param length The length of the bat (in z-direction) as percentage of the height of the playing field.
            */
            void setLength(float length)
                { this->length_ = length; }
            /**
            @brief get the length of the bat.
            @return Returns the length of the bat (in z-direction) as percentage of the height of the playing field.
            */
            float getLength() const
                { return this->length_; }

        private:
            void registerVariables(); //!< Registers variables to be synchronized over the network.

            float movement_; //!< The amount (and direction), in z-direction, of movement of the bat.
            bool bMoveLocal_; //!< Helper to know whether the movement is caused by moveFrontBack() or moveRightLeft().
            float speed_; //!< The movement speed of the bat.
            float length_; //!< The length of the bat (in z-direction) as percentage of the height of the playing field.
            float fieldHeight_; //!< The height of the playing field.
            bool bSteadiedPosition_; //!< Helper boolean, to keep track of when to steady the position, to ensure network synchronicity.
    };
}

#endif /* _PongBat_H__ */

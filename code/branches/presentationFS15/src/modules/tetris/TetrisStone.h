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
 *      ...
 *   Co-authors:
 *      ...
 *
 */

/**
    @file TetrisStone.h
    @brief Declaration of the TetrisStone class.
    @ingroup Tetris
*/

#ifndef _TetrisStone_H__
#define _TetrisStone_H__

#include "tetris/TetrisPrereqs.h"

#include "worldentities/MovableEntity.h"
#include "tools/Timer.h"

namespace orxonox
{

    /**
    @brief

    @author

    @ingroup Tetris
    */
    class _TetrisExport TetrisStone : public MovableEntity
    {
        public:
            TetrisStone(Context* context); //!< Constructor. Registers and initializes the object.
            virtual ~TetrisStone() {}

            virtual void moveFrontBack(const Vector2& value); //!< Overloaded the function to steer the bat up and down.
            virtual void moveRightLeft(const Vector2& value); //!< Overloaded the function to steer the bat up and down.

            virtual void changedPlayer(); //!< Is called when the player changed.

            /**
            @brief Set the size of the stone.
            @param size The dimensions a stone has in the game world. (A stone is a cube)
            */
            void setSize(float size)
                { this->size_ = size; }
            /**
            @brief Get the size of the stone.
            @return Returns the dimensions a stone has in the game world. (A stone is a cube)
            */
            float getSize(void) const
                { return this->size_; }

            void setGame(Tetris* tetris)
                { assert(tetris); tetris_ = tetris; }

        private:
            void enableMovement(void)
                { this->delay_ = false; }
            void unlockRotation(void)
                { this->lockRotation_ = false; }

            float size_; //!< The dimensions a stone has in the game world.
            bool delay_;
            bool lockRotation_;
            Timer delayTimer_;
            Timer rotationTimer_; ///!< This timer is used to filter out multiple rotation inputs.

            Tetris* tetris_;
    };
}

#endif /* _TetrisStone_H__ */

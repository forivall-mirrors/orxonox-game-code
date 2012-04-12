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
    @file TetrisBrick.h
    @brief Declaration of the TetrisBrick class.
    @ingroup Tetris
*/

#ifndef _TetrisBrick_H__
#define _TetrisBrick_H__

#include "tetris/TetrisPrereqs.h"

#include "worldentities/ControllableEntity.h"
#include "tools/Timer.h"

namespace orxonox
{

    /**
    @brief
        ContainerClass in order to create TetrisBricks by combining TetrisStones.
    @author

    @ingroup Tetris
    */
    class _TetrisExport TetrisBrick : public ControllableEntity
    {
        public:
            TetrisBrick(BaseObject* creator); //!< Constructor. Registers and initializes the object.
            virtual ~TetrisBrick() {}

            virtual void moveFrontBack(const Vector2& value); //!< Overloaded the function to steer the bat up and down.
            virtual void moveRightLeft(const Vector2& value); //!< Overloaded the function to steer the bat up and down.
            virtual void changedPlayer(); //!< Is called when the player changed.

            bool isValidMove(const Vector3& position, bool isRotation);
            unsigned int getNumberOfStones(void) const
                { return this->brickStones_.size(); }
            TetrisStone* getStone(unsigned int i);
            bool contains(TetrisStone* stone);

            void setGame(Tetris* tetris)
                { assert(tetris); tetris_ = tetris; }
            unsigned int getRotationCount(void)
                { return this->rotationCount_;}

        protected:
            void createBrick(void); //!< Create a cluster of TetrisStones
            void formBrick(TetrisStone* stone, unsigned int i);
            Tetris* getTetris();

            unsigned int shapeIndex_; //!< My way to represent the different brick shapes.
            unsigned int stonesPerBrick_; //!< So many stones are contained in this brick.
            std::vector<TetrisStone*> brickStones_; //!< A list of all stones in a brick.


            void enableMovement(void)
                { this->delay_ = false; }
            void unlockRotation(void)
                { this->lockRotation_ = false; }

            float getSize(void) const
                { return this->size_; }


            float size_; //!< The dimensions a stone has in the game world. //TODO: get stone dimensions
            bool delay_;
            bool lockRotation_;

            unsigned int rotationCount_; //!< Stores the bricks orientation
            Timer delayTimer_;
            Timer rotationTimer_; ///!< This timer is used to filter out multiple rotation inputs.
            Tetris* tetris_; //<! The Tetris class is responsible to initialize this value.
           // TetrisCenterpoint* center_;

    };
}

#endif /* _TetrisBrick_H__ */

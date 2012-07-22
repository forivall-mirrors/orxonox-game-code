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
    @file Tetris.h
    @brief Declaration of the Tetris class.
    @ingroup Tetris
*/

#ifndef _Tetris_H__
#define _Tetris_H__

#include "tetris/TetrisPrereqs.h"

#include "tools/Timer.h"

#include "gametypes/Deathmatch.h"

namespace orxonox
{

    /**
    @brief

    @author

    @ingroup Tetris
    */
    class _TetrisExport Tetris : public Deathmatch
    {
        public:
            Tetris(BaseObject* creator); //!< Constructor. Registers and initializes the object.
            virtual ~Tetris(); //!< Destructor. Cleans up, if initialized.

            virtual void start(void); //!< Starts the Tetris minigame.
            virtual void end(void); ///!< Ends the Tetris minigame.

            virtual void tick(float dt);

            virtual void spawnPlayer(PlayerInfo* player); //!< Spawns the input player.

            void setCenterpoint(TetrisCenterpoint* center);

            PlayerInfo* getPlayer(void) const; //!< Get the player.
            WeakPtr<TetrisCenterpoint> getCenterpoint(void)
                { return this->center_; }

            bool isValidMove(TetrisStone* stone, const Vector3& position);
            bool isValidMove(TetrisBrick* brick, const Vector3& position, bool isRotation);
            Vector3 rotateVector(Vector3 position, unsigned int amount);

        protected:
            virtual void spawnPlayersIfRequested(); //!< Spawns player.


        private:
            void startBrick(void);
            void createBrick(void);
            void cleanup(void); //!< Cleans up the Gametype by destroying the ball and the bats.
            bool isValidStonePosition(TetrisStone* stone, const Vector3& position);
            bool isValidBrickPosition(TetrisBrick* brick, const Vector3& position);
            void findFullRows(void);
            void clearRow(unsigned int row);


            PlayerInfo* player_;

            WeakPtr<TetrisCenterpoint> center_; //!< The playing field.
            std::list<SmartPtr<TetrisStone> > stones_; //!< A list of all stones in play.
            WeakPtr<TetrisBrick> activeBrick_;
            WeakPtr<TetrisBrick> futureBrick_;

            Timer starttimer_; //!< A timer to delay the start of the game.
            float endGameCriteria_; //<! Works as a timer which is resetted, whenever a brick is created.
    };
}

#endif /* _Tetris_H__ */

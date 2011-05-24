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

            std::pair<bool, TetrisStone*> isValidMove(TetrisStone* stone, const Vector3& position);

        protected:
            virtual void spawnPlayersIfRequested(); //!< Spawns player.

        private:
            void startStone(void); //!< Starts with the first stone.
            void createStone(void);
            void cleanup(void); //!< Cleans up the Gametype by destroying the ball and the bats.
            std::pair<bool, TetrisStone*> isValidStonePosition(TetrisStone* stone, const Vector3& position);
            
            PlayerInfo* player_;

            WeakPtr<TetrisCenterpoint> center_; //!< The playing field.
            std::vector<TetrisStone*> stones_; //!< A list of all stones in play.
            std::vector< std::vector<bool> > grid_;
            TetrisStone* activeStone_;
            
            Timer starttimer_; //!< A timer to delay the start of the game.
    };
}

#endif /* _Tetris_H__ */

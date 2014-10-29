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
    @file Mini4Dgame.h
    @brief Declaration of the Mini4Dgame class.
*/

#ifndef _Mini4Dgame_H__
#define _Mini4Dgame_H__

#include "gametypes/Deathmatch.h"

namespace orxonox
{
    /**
    @brief

    @author

    */
    class Mini4Dgame : public Deathmatch
    {
        public:
    		Mini4Dgame(Context* context); //!< Constructor. Registers and initializes the object.
            virtual ~Mini4Dgame(); //!< Destructor. Cleans up, if initialized.

            virtual void start(void); //!< Starts the Mini4Dgame minigame.
            virtual void end(void); ///!< Ends the Mini4Dgame minigame.

            virtual void spawnPlayer(PlayerInfo* player); //!< Spawns the input player.

            void setCenterpoint(Mini4DgameCenterpoint* center)
            	{ this->center_ = center; }

//TODO: enum colors
            PlayerInfo* getPlayer(int color) const; //!< Get the player with the specified color.


            //bool isValidMove(Vector4* move, const Mini4DgameBoard* board);//!< Checks if the move is valid

        protected:
            virtual void spawnPlayersIfRequested(); //!< Spawns player.


        private:
            void cleanup(void); //!< Cleans up the Gametype by destroying the ball and the bats.

          PlayerInfo* players[3];

            WeakPtr<Mini4DgameCenterpoint> center_; //!< The playing field.

            //TODO: Board is missing up to now
    };
}

#endif /* _Mini4Dgame_H__ */

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
    @file Pong.h
    @brief Declaration of the Pong class.
    @ingroup Pong
*/

#ifndef _Pong_H__
#define _Pong_H__

#include "pong/PongPrereqs.h"

#include "tools/Timer.h"

#include "gametypes/Deathmatch.h"
#include "PongCenterpoint.h"

namespace orxonox
{

    /**
    @brief
        Implements a Pong minigame (<a href="http://en.wikipedia.org/wiki/Pong">Wikipedia::Pong</a>).
        It connects the different entities present in a game of Pong.

        - The @ref orxonox::PongCenterpoint "PongCenterpoint" is the playing field for the Pong minigame, it allows for configuration of the minigame, e.g. by setting the size of the playing field, or the length of the @ref orxonox::PongBat "PongBats". The playing field is always in the x,y-plane, the x-axis being the horizontal and the z-axis being the vertical axis.<br />
        The Pong class redistributes the important parameters defined in @ref orxonox::PongCenterpoint "PongCenterpoint" to the other entities, that need to know them, e.g. the @ref orxonox::PongBall "PongBall" and the @ref orxonox::PongBat "PongBats".<br />
        The @ref orxonox::PongCenterpoint "PongCenterpoint" needs to exist in a level with the @ref orxonox::Gametype "Gametype" <em>Pong</em>.
        - The @ref orxonox::PongBall "PongBall" is the ball both players play with. The @ref orxonox::PongBall "PongBall" both implements the movement of the ball, as well as the influence of the boundaries and consequently, also the bouncing (off the upper and lower delimiters, and as off the @ref orxonox::PongBat "PongBats") of the ball and the effects of the failure of a player to catch the ball (i.e. the scoring of the other player).
        - The two @ref orxonox::PongBat "PongBats" are the entities through which the players can actively participate in the game, by controlling them. The @ref orxonox::PongBat "PongBat" class manages the movement (and restrictions thereof) and the influence of the players on the bats.

    @author
        Fabian 'x3n' Landau

    @ingroup Pong
    */
    class _PongExport Pong : public Deathmatch
    {
        public:
            Pong(BaseObject* creator); //!< Constructor. Registers and initializes the object.
            virtual ~Pong(); //!< Destructor. Cleans up, if initialized.

            virtual void start(); //!< Starts the Pong minigame.
            virtual void end(); ///!< Ends the Pong minigame.

            virtual void spawnPlayer(PlayerInfo* player); //!< Spawns the input player.

            virtual void playerScored(PlayerInfo* player); //!< Is called when the player scored.

            /**
            @brief Set the PongCenterpoint (the playing field).
            @param center A pointer to the PongCenterpoint to be set.
            */
            void setCenterpoint(PongCenterpoint* center)
                { this->center_ = center; }
            void setConfigValues(); //!< Makes scoreLimit configurable.

            PlayerInfo* getLeftPlayer() const; //!< Get the left player.
            PlayerInfo* getRightPlayer() const; //!< Get the right player.

        protected:
            virtual void spawnPlayersIfRequested(); //!< Spawns players, and fills the rest up with bots.

            void startBall(); //!< Starts the ball with some default speed.
            void cleanup(); //!< Cleans up the Gametype by destroying the ball and the bats.

            WeakPtr<PongCenterpoint> center_; //!< The playing field.
            WeakPtr<PongBall> ball_; //!< The Pong ball.
            WeakPtr<PongBat> bat_[2]; //!< The two bats.
            Timer starttimer_; //!< A timer to delay the start of the game.
            int scoreLimit_; //!< If a player scored that much points, the game is ended.
    };
}

#endif /* _Pong_H__ */

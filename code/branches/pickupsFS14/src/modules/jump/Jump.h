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
    @file Jump.h
    @brief Declaration of the Jump class.
    @ingroup Jump
*/

#ifndef _Jump_H__
#define _Jump_H__

#include "jump/JumpPrereqs.h"

#include "tools/Timer.h"
#include "graphics/Camera.h"

#include "gametypes/Deathmatch.h"
#include "JumpCenterpoint.h"

namespace orxonox
{

    /**
    @brief
        Implements a Jump minigame (<a href="http://en.wikipedia.org/wiki/Jump">Wikipedia::Jump</a>).
        It connects the different entities present in a game of Jump.

        - The @ref orxonox::JumpCenterpoint "JumpCenterpoint" is the playing field for the Jump minigame, it allows for configuration of the minigame, e.g. by setting the size of the playing field, or the length of the @ref orxonox::JumpFigure "JumpFigures". The playing field is always in the x,y-plane, the x-axis being the horizontal and the z-axis being the vertical axis.<br />
        The Jump class redistributes the important parameters defined in @ref orxonox::JumpCenterpoint "JumpCenterpoint" to the other entities, that need to know them, e.g. the @ref orxonox::JumpPlatform "JumpPlatform" and the @ref orxonox::JumpFigure "JumpFigures".<br />
        The @ref orxonox::JumpCenterpoint "JumpCenterpoint" needs to exist in a level with the @ref orxonox::Gametype "Gametype" <em>Jump</em>.
        - The @ref orxonox::JumpPlatform "JumpPlatform" is the ball both players play with. The @ref orxonox::JumpPlatform "JumpPlatform" both implements the movement of the ball, as well as the influence of the boundaries and consequently, also the bouncing (off the upper and lower delimiters, and as off the @ref orxonox::JumpFigure "JumpFigures") of the ball and the effects of the failure of a player to catch the ball (i.e. the scoring of the other player).
        - The two @ref orxonox::JumpFigure "JumpFigures" are the entities through which the players can actively participate in the game, by controlling them. The @ref orxonox::JumpFigure "JumpFigure" class manages the movement (and restrictions thereof) and the influence of the players on the bats.

    @author
        Fabian 'x3n' Landau

    @ingroup Jump
    */
    class _JumpExport Jump : public Deathmatch
    {
        public:
            Jump(Context* context); //!< Constructor. Registers and initializes the object.
            virtual ~Jump(); //!< Destructor. Cleans up, if initialized.

            virtual void tick(float dt);

            virtual void start(); //!< Starts the Jump minigame.
            virtual void end(); ///!< Ends the Jump minigame.

            virtual void spawnPlayer(PlayerInfo* player); //!< Spawns the input player.

            virtual void playerScored(PlayerInfo* player, int score = 1); //!< Is called when the player scored.

            /**
            @brief Set the JumpCenterpoint (the playing field).
            @param center A pointer to the JumpCenterpoint to be set.
            */
            void setCenterpoint(JumpCenterpoint* center)
                { this->center_ = center; }
            void setConfigValues(); //!< Makes scoreLimit configurable.

            PlayerInfo* getPlayer() const; //!< Get the left player.

        protected:
            virtual void spawnPlayersIfRequested(); //!< Spawns players, and fills the rest up with bots.

            void startBall(); //!< Starts the ball with some default speed.
            void cleanup(); //!< Cleans up the Gametype by destroying the ball and the bats.

            WeakPtr<JumpCenterpoint> center_; //!< The playing field.
            WeakPtr<JumpPlatform> ball_; //!< The Jump ball.
            WeakPtr<JumpFigure> figure_; //!< The two bats.
            WeakPtr<Camera> camera;
            Timer starttimer_; //!< A timer to delay the start of the game.
            int scoreLimit_; //!< If a player scored that much points, the game is ended.

            float totalScreenShift;
    };
}

#endif /* _Jump_H__ */

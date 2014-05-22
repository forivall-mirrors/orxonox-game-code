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

#include <list>

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

            int getScore(PlayerInfo* player) const;
            bool getDead(PlayerInfo* player) const;

            /**
            @brief Set the JumpCenterpoint (the playing field).
            @param center A pointer to the JumpCenterpoint to be set.
            */
            void setCenterpoint(JumpCenterpoint* center)
                { center_ = center; }
            void setConfigValues(); //!< Makes scoreLimit configurable.

            PlayerInfo* getPlayer() const; //!< Get the left player.

        protected:
            void startBall(); //!< Starts the ball with some default speed.
            void cleanup(); //!< Cleans up the Gametype by destroying the ball and the bats.

            virtual void addPlatform(JumpPlatform* newPlatform, std::string platformTemplate, float xPosition, float zPosition);

            virtual JumpPlatformStatic* addPlatformStatic(float xPosition, float zPosition);
            virtual JumpPlatformHMove* addPlatformHMove(float xPosition, float zPosition, float leftBoundary, float rightBoundary, float speed);
            virtual JumpPlatformVMove* addPlatformVMove(float xPosition, float zPosition, float lowerBoundary, float upperBoundary, float speed);
            virtual JumpPlatformDisappear* addPlatformDisappear(float xPosition, float zPosition);
            virtual JumpPlatformTimer* addPlatformTimer(float xPosition, float zPosition, float time, float variance);
            virtual JumpPlatformFake* addPlatformFake(float xPosition, float zPosition);
            virtual void addProjectile(float xPosition, float zPosition);
            virtual void addEnemy(int type, float xPosition, float zPosition, float leftBoundary, float rightBoundary, float lowerBoundary, float upperBoundary, float xVelocity, float zVelocity);
            virtual void addSpring(float xPosition, float zPosition, float leftBoundary, float rightBoundary, float lowerBoundary, float upperBoundary, float xVelocity, float zVelocity);
            virtual void addSpring(JumpPlatform* platform);
            virtual void addRocket(float xPosition, float zPosition, float leftBoundary, float rightBoundary, float lowerBoundary, float upperBoundary, float xVelocity, float zVelocity);
            virtual void addRocket(JumpPlatform* platform);
            virtual void addPropeller(float xPosition, float zPosition, float leftBoundary, float rightBoundary, float lowerBoundary, float upperBoundary, float xVelocity, float zVelocity);
            virtual void addPropeller(JumpPlatform* platform);
            virtual void addBoots(float xPosition, float zPosition, float leftBoundary, float rightBoundary, float lowerBoundary, float upperBoundary, float xVelocity, float zVelocity);
            virtual void addBoots(JumpPlatform* platform);
            virtual void addShield(float xPosition, float zPosition, float leftBoundary, float rightBoundary, float lowerBoundary, float upperBoundary, float xVelocity, float zVelocity);
            virtual void addShield(JumpPlatform* platform);

            virtual void addStartSection();
            virtual void addSection();
            virtual bool addAdventure(int number);

            virtual float randomXPosition();
            virtual float randomXPosition(int totalColumns, int culomn);
            virtual float randomXPositionLeft(int totalColumns, int culomn);
            virtual float randomXPositionRight(int totalColumns, int culomn);
            virtual float randomZPosition(int totalRows, int row, float sectionBegin, float SectionEnd);
            virtual float randomZPositionLower(int totalRows, int row, float sectionBegin, float SectionEnd);
            virtual float randomZPositionUpper(int totalRows, int row, float sectionBegin, float SectionEnd);

            virtual float randomPosition(float lowerBoundary, float upperBoundary);
            virtual float randomSpeed();

            WeakPtr<JumpCenterpoint> center_; //!< The playing field.
            WeakPtr<JumpFigure> figure_; //!< The two bats.
            WeakPtr<Camera> camera;
            int scoreLimit_; //!< If a player scored that much points, the game is ended.

            float totalScreenShift;
            float screenShiftSinceLastUpdate;
            int sectionNumber_;
            int adventureNumber_;
            float platformWidth_;
            float platformHeight_;
    };
}

#endif /* _Jump_H__ */

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
    @file PongAI.h
    @brief Declaration of the PongAI class.
    @ingroup Pong
*/

#ifndef _PongAI_H__
#define _PongAI_H__

#include "pong/PongPrereqs.h"

#include <list>

#include "util/Math.h"
#include "tools/interfaces/Tickable.h"

#include "controllers/Controller.h"
#include "PongBall.h"

namespace orxonox
{

    /**
    @brief
        The PongAI is an artificial intelligence for the @ref orxonox::Pong "Pong" gametype.

    @author
        Fabian 'x3n' Landau

    @ingroup Pong
    */
    class _PongExport PongAI : public Controller, public Tickable
    {
        public:
            PongAI(Context* context); //!< Constructor. Registers and initializes the object.
            virtual ~PongAI();

            void setConfigValues();

            virtual void tick(float dt); //!< Implements the behavior of the PongAI (i.e. its intelligence).

            /**
            @brief Set the ball for the AI.
            @param ball A pointer to the ball.
            */
            void setPongBall(PongBall* ball)
                { this->ball_ = ball; }

        protected:
            void calculateRandomOffset(); //!< Calculates the random offset, that accounts for random errors the AI makes in order to be beatable.
            void calculateBallEndPosition(); //!< Calculate the end position the ball will be in.
            void move(char direction, bool bUseDelay); //!< Determine the movement the AI will undertake.
            void delayedMove(); //!< Is called, when a delayed move takes effect.

            WeakPtr<PongBall> ball_; //!< A weak pointer to the ball.
            Vector2 ballDirection_; //!< Vector to store the (x,z) direction in which the ball is flying.
            float ballEndPosition_; //!< The calculated end position of the ball.
            float randomOffset_; //!< A random offset to introduce random errors (weighted by the strength of the AI) into the AI's behavior.
            bool bChangedRandomOffset_; //!< Helper boolean, to change the random offset more often.
            float relHysteresisOffset_; //!< A hysteresis offset.
            float strength_; //!< The strength of the AI. Ranging from 0 to 1.

            std::list<std::pair<Timer*, char> > reactionTimers_; //!< A list of reaction timers and the directions that take effect when their timer expires.
            char movement_; //!< The planned movement.
            char oldMove_; //!< The previous movement.
            bool bOscillationAvoidanceActive_; //!< Boolean, to avoid oscillations.
    };
}

#endif /* _PongAI_H__ */

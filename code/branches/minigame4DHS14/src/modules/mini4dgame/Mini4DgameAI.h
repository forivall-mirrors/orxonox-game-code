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
    @file Mini4DgameAI.h
    @brief Declaration of the Mini4DgameAI class.
*/

#ifndef _Mini4DgameAI_H__
#define _Mini4DgameAI_H__

#include "mini4Dgame/Mini4DgamePrereqs.h"

#include <list>

#include "tools/interfaces/Tickable.h"

#include "controllers/Controller.h"
#include "Mini4DgameCenterpoint.h"
#include "Mini4Dgame.h"

namespace orxonox
{

    /**
    @brief
        The Mini4DgameAI is an artificial intelligence for the @ref orxonox::Mini4Dgame "Mini4Dgame" gametype.

    @author
        Oliver Richter
    */
    class _Mini4DgameExport Mini4DgameAI : public Controller, public Tickable
    {
        public:
    		Mini4DgameAI(Context* context); //!< Constructor. Registers and initializes the object.
            virtual ~Mini4DgameAI();

            //void setConfigValues();

            /**
                @brief
                    starts the AI to make a move. implements the intelligence of the AI
                @param timeout
                    The time the AI has to decide on a move. (Sets the strength of the AI)
                @return move
                	The move that the AI decided to make.
            */
            Vector4 makeMove(float timeout);


            virtual void tick(float dt);

            void setCenterpoint(Mini4DgameCenterpoint* center)
                        	{ this->center_ = center; }

        protected:

            std::list<std::pair<Timer*, char> > reactionTimers_; //!< A list of reaction timers and the directions that take effect when their timer expires.
            Mini4DgameCenterpoint* center_;

        private:

            std::list<Vector4> getPossibleMoves();
            void copyBoard();
            mini4DgamePlayerColor::color board_[4][4][4][4];
    };
}

#endif /* _Mini4DgameAI_H__ */

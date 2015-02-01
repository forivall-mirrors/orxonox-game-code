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
    @file Mini4DgameAI.cc
    @brief Implementation of the Mini4DgameAI class.
*/

#include "Mini4DgameAI.h"

#include "core/CoreIncludes.h"
#include "core/config/ConfigValueIncludes.h"
#include "core/command/Executor.h"
#include "tools/Timer.h"

#include <list>

#include "worldentities/ControllableEntity.h"

#include "Mini4DgameCenterpoint.h"

namespace orxonox
{
    RegisterUnloadableClass(Mini4DgameAI);

    //const static float MAX_REACTION_TIME = 0.4f;

    /**
    @brief
        Constructor. Registers and initializes the object.
    */
    Mini4DgameAI::Mini4DgameAI(Context* context) : Controller(context)
    {
        RegisterObject(Mini4DgameAI);

        this->setConfigValues();
        this->center_ = 0;
    }

    /**
    @brief
        Destructor. Cleans up the list fo reaction timers.
    */
    Mini4DgameAI::~Mini4DgameAI()
    {

    }

    Vector4 Mini4DgameAI::makeMove(float timeout)
    {
    	this->copyBoard();
    	std::list<Vector4> possibleMoves = this->getPossibleMoves();

    	//TODO: alphabeta ueber possibleMoves

    	//when timeout
    	Vector4 move = possibleMoves.front();
    	return move;
    }

    void Mini4DgameAI::copyBoard()
    {
		for(int i=0;i<4;i++){
			for(int j=0;j<4;j++){
				for(int k=0;k<4;k++){
					for(int l=0;l<4;l++){
						this->board_[i][j][k][l]=this->center_->board[i][j][k][l];
					}
				}
			}
		}
    }

    std::list<Vector4> Mini4DgameAI::getPossibleMoves()
    {
    	std::list<Vector4> possibleMoves = new std::list<Vector4>;
    	for(int i=0;i<4;i++){
			for(int j=0;j<4;j++){
				for(int k=0;k<4;k++){
					for(int l=0;l<4;l++){
						if (this->board_[i][j][k][l]==mini4DgamePlayerColor::none)
							possibleMoves.push_back(Vector4(i,j,k,l));
					}
				}
			}
		}
    	return possibleMoves;
    }

    /**
    @brief
        Is called each tick.
        Implements the behavior of the PongAI (i.e. its intelligence).
    @param dt
        The time that has elapsed since the last tick.
    */
    void Mini4DgameAI::tick(float dt)
    {

    }
}

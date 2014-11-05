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
    @file Mini4DgameCenterpoint.cc
    @brief Implementation of the Mini4DgameCenterpoint class.
*/

#include "Mini4DgameCenterpoint.h"

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"

#include "Mini4Dgame.h"

namespace orxonox
{
    RegisterClass(Mini4DgameCenterpoint);

    /**
    @brief
        Constructor. Registers and initializes the object and checks whether the gametype is actually Mini4Dgame.
    */
    Mini4DgameCenterpoint::Mini4DgameCenterpoint(Context* context) : StaticEntity(context)
    {
        RegisterObject(Mini4DgameCenterpoint);

        this->width_ = 200;
        this->height_ = 200;
        this->length_ = 200;

        for(int i=0;i<4;i++){
        	for(int j=0;j<4;j++){
        		for(int k=0;k<4;k++){
        			for(int l=0;l<4;l++){
        				this->board[i][j][k][l]=mini4DgamePlayerColor::none;
        			}
        		}
        	}
        }

        this->checkGametype();
    }

    /**
    @brief
        Method to create a Mini4DgameCenterpoint through XML.
    */
    void Mini4DgameCenterpoint::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(Mini4DgameCenterpoint, XMLPort, xmlelement, mode);

        XMLPortParam(Mini4DgameCenterpoint, "dimension", setFieldDimension, getFieldDimension, xmlelement, mode);
    }

    /**
    @brief
        Is called when the gametype has changed.
    */
    void Mini4DgameCenterpoint::changedGametype()
    {
        SUPER(Mini4DgameCenterpoint, changedGametype);

        // Check, whether it's still Mini4Dgame.
        this->checkGametype();
    }

    /**
    @brief
        Checks whether the gametype is Mini4Dgame and if it is, sets its centerpoint.
    */
    void Mini4DgameCenterpoint::checkGametype()
    {
        if (this->getGametype() != NULL && this->getGametype()->isA(Class(Mini4Dgame)))
        {
            Mini4Dgame* Mini4DgameGametype = orxonox_cast<Mini4Dgame*>(this->getGametype().get());
            Mini4DgameGametype->setCenterpoint(this);
        }
    }

    /**
        @brief checks if the move is valid
        @param the position where to put the stone plus the player who makes the move
    */
    bool Mini4DgameCenterpoint::isValidMove(const Vector4 move)
    {
    	return (this->board[(int)move.x][(int)move.y][(int)move.z][(int)move.w] == mini4DgamePlayerColor::none);
    }

    /**
    @brief makes a move on the logic playboard
    @param the position where to put the stone plus the player who makes the move
    */
    void Mini4DgameCenterpoint::makeMove(const Vector4 move, const mini4DgamePlayerColor::color playerColor)
    {
    	if(this->isValidMove(move))
    	{
    		this->board[(int)move.x][(int)move.y][(int)move.z][(int)move.w] = playerColor;
    		mini4DgamePlayerColor::color winner = this->getWinner();
    		if(winner != mini4DgamePlayerColor::none)
    		{
    			Mini4Dgame->win(winner);
    		}
    	}
    }

    mini4DgamePlayerColor::color Mini4DgameCenterpoint::getWinner()
    {
    	//check diagonals rows-columns-height-numbers
		for(int i=1; i<4; i++)
		{
			if(this->board[i][i][i][i]==mini4DgamePlayerColor::none || this->board[0][0][0][0] != this->board[i][i][i][i])
				break;
			if(i==3)
				return this->board[0][0][0][0];
		}
		for(int i=1; i<4; i++)
		{
			if(this->board[3-i][i][i][i]==mini4DgamePlayerColor::none || this->board[3][0][0][0] != this->board[3-i][i][i][i])
				break;
			if(i==3)
				return this->board[3][0][0][0];
		}
		for(int i=1; i<4; i++)
		{
			if(this->board[i][3-i][i][i]==mini4DgamePlayerColor::none || this->board[0][3][0][0] != this->board[i][3-i][i][i])
				break;
			if(i==3)
				return this->board[0][3][0][0];
		}
		for(int i=1; i<4; i++)
		{
			if(this->board[i][i][3-i][i]==mini4DgamePlayerColor::none || this->board[0][0][3][0] != this->board[i][i][3-i][i])
				break;
			if(i==3)
				return this->board[0][0][3][0];
		}
		for(int i=1; i<4; i++)
		{
			if(this->board[i][i][i][3-i]==mini4DgamePlayerColor::none || this->board[0][0][0][3] != this->board[i][i][i][3-i])
				break;
			if(i==3)
				return this->board[0][0][0][3];
		}
		for(int i=1; i<4; i++)
		{
			if(this->board[3-i][3-i][i][i]==mini4DgamePlayerColor::none || this->board[3][3][0][0] != this->board[3-i][3-i][i][i])
				break;
			if(i==3)
				return this->board[3][3][0][0];
		}
		for(int i=1; i<4; i++)
		{
			if(this->board[3-i][i][3-i][i]==mini4DgamePlayerColor::none || this->board[3][0][3][0] != this->board[3-i][i][3-i][i])
				break;
			if(i==3)
				return this->board[3][0][3][0];
		}
		for(int i=1; i<4; i++)
		{
			if(this->board[3-i][i][i][3-i]==mini4DgamePlayerColor::none || this->board[3][0][0][3] != this->board[3-i][i][i][3-i])
				break;
			if(i==3)
				return this->board[3][0][0][3];
		}

		//check diagonals rows-columns-height
		for(int l=0; l<4; l++)
		{
			for(int i=1; i<4; i++)
			{
				if(this->board[i][i][i][l]==mini4DgamePlayerColor::none || this->board[0][0][0][l] != this->board[i][i][i][l])
					break;
				if(i==3)
					return this->board[0][0][0][l];
			}
			for(int i=1; i<4; i++)
			{
				if(this->board[3-i][i][i][l]==mini4DgamePlayerColor::none || this->board[3][0][0][l] != this->board[3-i][i][i][l])
					break;
				if(i==3)
					return this->board[3][0][0][l];
			}
			for(int i=1; i<4; i++)
			{
				if(this->board[i][3-i][i][l]==mini4DgamePlayerColor::none || this->board[0][3][0][l] != this->board[i][3-i][i][l])
					break;
				if(i==3)
					return this->board[0][3][0][l];
			}
			for(int i=1; i<4; i++)
			{
				if(this->board[i][i][3-i][l]==mini4DgamePlayerColor::none || this->board[0][0][3][l] != this->board[i][i][3-i][l])
					break;
				if(i==3)
					return this->board[0][0][3][l];
			}
		}

		//check diagonals rows-columns-numbers
		for(int l=0; l<4; l++)
		{
			for(int i=1; i<4; i++)
			{
				if(this->board[i][i][l][i]==mini4DgamePlayerColor::none || this->board[0][0][l][0] != this->board[i][i][l][i])
					break;
				if(i==3)
					return this->board[0][0][l][0];
			}
			for(int i=1; i<4; i++)
			{
				if(this->board[3-i][i][l][i]==mini4DgamePlayerColor::none || this->board[3][0][l][0] != this->board[3-i][i][l][i])
					break;
				if(i==3)
					return this->board[3][0][l][0];
			}
			for(int i=1; i<4; i++)
			{
				if(this->board[i][3-i][l][i]==mini4DgamePlayerColor::none || this->board[0][3][l][0] != this->board[i][3-i][l][i])
					break;
				if(i==3)
					return this->board[0][3][l][0];
			}
			for(int i=1; i<4; i++)
			{
				if(this->board[i][i][l][3-i]==mini4DgamePlayerColor::none || this->board[0][0][l][3] != this->board[i][i][l][3-i])
					break;
				if(i==3)
					return this->board[0][0][l][3];
			}
		}

		//check diagonals rows-height-numbers
		for(int l=0; l<4; l++)
		{
			for(int i=1; i<4; i++)
			{
				if(this->board[i][l][i][i]==mini4DgamePlayerColor::none || this->board[0][l][0][0] != this->board[i][l][i][i])
					break;
				if(i==3)
					return this->board[0][l][0][0];
			}
			for(int i=1; i<4; i++)
			{
				if(this->board[3-i][l][i][i]==mini4DgamePlayerColor::none || this->board[3][l][0][0] != this->board[3-i][l][i][i])
					break;
				if(i==3)
					return this->board[3][l][0][0];
			}
			for(int i=1; i<4; i++)
			{
				if(this->board[i][l][3-i][i]==mini4DgamePlayerColor::none || this->board[0][l][3][0] != this->board[i][l][3-i][i])
					break;
				if(i==3)
					return this->board[0][l][3][0];
			}
			for(int i=1; i<4; i++)
			{
				if(this->board[i][l][i][3-i]==mini4DgamePlayerColor::none || this->board[0][l][0][3] != this->board[i][l][i][3-i])
					break;
				if(i==3)
					return this->board[0][l][0][3];
			}
		}

		//check diagonals columns-height-numbers
		for(int l=0; l<4; l++)
		{
			for(int i=1; i<4; i++)
			{
				if(this->board[l][i][i][i]==mini4DgamePlayerColor::none || this->board[l][0][0][0] != this->board[l][i][i][i])
					break;
				if(i==3)
					return this->board[l][0][0][0];
			}
			for(int i=1; i<4; i++)
			{
				if(this->board[l][3-i][i][i]==mini4DgamePlayerColor::none || this->board[l][3][0][0] != this->board[l][3-i][i][i])
					break;
				if(i==3)
					return this->board[l][3][0][0];
			}
			for(int i=1; i<4; i++)
			{
				if(this->board[l][i][3-i][i]==mini4DgamePlayerColor::none || this->board[l][0][3][0] != this->board[l][i][3-i][i])
					break;
				if(i==3)
					return this->board[l][0][3][0];
			}
			for(int i=1; i<4; i++)
			{
				if(this->board[l][i][i][3-i]==mini4DgamePlayerColor::none || this->board[l][0][0][3] != this->board[l][i][i][3-i])
					break;
				if(i==3)
					return this->board[l][0][0][3];
			}
		}

		//check diagonals rows-columns
		for(int k=0;k<4;k++){
    		for(int l=0;l<4;l++){
    			for(int i=1; i<4; i++)
				{
					if(this->board[i][i][k][l]==mini4DgamePlayerColor::none || this->board[0][0][l][0] != this->board[i][i][l][i])
						break;
					if(i==3)
						return this->board[0][0][l][0];
				}}}
    	//-------------------------------------------------------------------------------------------------------

    	//check rows
    	for(int j=0;j<4;j++){
    		for(int k=0;k<4;k++){
    			for(int l=0;l<4;l++){
    				if(this->board[0][j][k][l]!= mini4DgamePlayerColor::none
    				   && this->board[0][j][k][l]==this->board[1][j][k][l]
    				   && this->board[1][j][k][l]==this->board[2][j][k][l]
    				   && this->board[2][j][k][l]==this->board[3][j][k][l])
    				{
    					return this->board[0][j][k][l];
    				}
    			}
    		}
    	}

    	//check columns
    	for(int i=0;i<4;i++){
    		for(int k=0;k<4;k++){
    			for(int l=0;l<4;l++){
    				if(this->board[i][0][k][l]!= mini4DgamePlayerColor::none
    	    				   && this->board[i][0][k][l]==this->board[i][1][k][l]
    	    				   && this->board[i][1][k][l]==this->board[i][2][k][l]
    	    				   && this->board[i][2][k][l]==this->board[i][3][k][l])
    				{
    	    			return this->board[i][0][k][l];
    				}
    			}
    		}
    	}

    	//check height
    	for(int i=0;i<4;i++){
    		for(int j=0;j<4;j++){
    			for(int l=0;l<4;l++){
    				if(this->board[i][j][0][l]!= mini4DgamePlayerColor::none
    	    	    				   && this->board[i][j][0][l]==this->board[i][j][1][l]
    	    	    				   && this->board[i][j][1][l]==this->board[i][j][2][l]
    	    	    				   && this->board[i][j][2][l]==this->board[i][j][3][l])
    				{
    					return this->board[i][j][0][l];
    				}
    			}
    		}
    	}

    	//check numbers
    	for(int i=0;i<4;i++){
    		for(int j=0;j<4;j++){
    			for(int k=0;k<4;k++){
    				if(this->board[i][j][k][0]!= mini4DgamePlayerColor::none
    	    	    				   && this->board[i][j][k][0]==this->board[i][j][k][1]
    	    	    				   && this->board[i][j][k][1]==this->board[i][j][k][2]
    	    	    				   && this->board[i][j][k][2]==this->board[i][j][k][3])
    				{
    					return this->board[i][j][k][0];
    				}
    			}
    		}
    	}


    	return mini4DgamePlayerColor::none;
    }
}

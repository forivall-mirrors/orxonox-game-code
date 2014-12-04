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
    @file PongBall.cc
    @brief Implementation of the PongBall class.
*/

#include "Mini4DgameBoard.h"

#include "core/CoreIncludes.h"
#include "core/GameMode.h"

#include "gametypes/Gametype.h"
#include "graphics/BlinkingBillboard.h"

#include "core/XMLPort.h"

#include "Mini4Dgame.h"

namespace orxonox
{
    RegisterClass(Mini4DgameBoard);

    /**
    @brief
        Constructor. Registers and initializes the object.
    */
    Mini4DgameBoard::Mini4DgameBoard(Context* context) : StaticEntity(context)
    {
        RegisterObject(Mini4DgameBoard);

        //this->registerVariables();

        //initialize board
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
        Destructor.
    */
    Mini4DgameBoard::~Mini4DgameBoard()
    {
        if (this->isInitialized())
        {

        }
    }

    //xml port for loading sounds
    void Mini4DgameBoard::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(Mini4DgameBoard, XMLPort, xmlelement, mode);
    }

    /**
    @brief
        Register variables to synchronize over the network.

    void Mini4DgameBoard::registerVariables()
    {
        registerVariable( this->fieldWidth_ );
        registerVariable( this->fieldHeight_ );
        registerVariable( this->batlength_ );
        registerVariable( this->speed_ );
        registerVariable( this->relMercyOffset_ );
        registerVariable( this->batID_[0] );
        registerVariable( this->batID_[1], VariableDirection::ToClient, new NetworkCallback<PongBall>( this, &PongBall::applyBats) );
    }
     */

	/**
        @brief checks if the move is valid
        @param the position where to put the stone plus the player who makes the move
    */
    bool Mini4DgameBoard::isValidMove(const Vector4 move)
    {
    	return (this->board[(int)move.x][(int)move.y][(int)move.z][(int)move.w] == mini4DgamePlayerColor::none);
    }



    /**
    @brief makes a move on the logic playboard
    @param the position where to put the stone plus the player who makes the move
    */
    void Mini4DgameBoard::makeMove(const Vector4 move, const int playerColor)
    {
    	if(this->isValidMove(move))
    	{
    		this->board[(int)move.x][(int)move.y][(int)move.z][(int)move.w] = (mini4DgamePlayerColor::color) playerColor;
			BlinkingBillboard* bb = new BlinkingBillboard(this->getContext());
			orxout(user_status) << "Mini4Dgame: move.x " << move.x << endl;

			bb->setPosition(60*(int)move.x-90,60*(int)move.y-90,60*(int)move.z-90);
			bb->setFrequency(0.6);
			bb->setAmplitude(0.1);
			//bb->setMaterial("Flares/lensflare");
			bb->setMaterial("Numbers/One");
			bb->setColour(ColourValue(0,1,0));

			this->attach(bb);


    		Mini4DgameWinner winner = this->getWinner();
    		if(winner.color_ != mini4DgamePlayerColor::none)
    		{
    			orxout(user_status) << "Mini4Dgame: win!!!!!!!" << endl;
    			//win(winner);
    		}
    	}
    }

    Mini4DgameWinner Mini4DgameBoard::getWinner()
    {
    	Mini4DgameWinner winner;
		winner.color_ = mini4DgamePlayerColor::none;

    	//check diagonals rows-columns-height-numbers
		for(int i=1; i<4; i++)
		{
			if(this->board[i][i][i][i]==mini4DgamePlayerColor::none || this->board[0][0][0][0] != this->board[i][i][i][i])
				break;
			if(i==3)
			{
				winner.color_ = this->board[0][0][0][0];
				for(int x=0; x<4; x++){
					winner.winningRow[x] = x;
					winner.winningColumn[x] = x;
					winner.winningHeight[x] = x;
					winner.winningNumber[x] = x;
				}
				return winner;
			}
		}
		for(int i=1; i<4; i++)
		{
			if(this->board[3-i][i][i][i]==mini4DgamePlayerColor::none || this->board[3][0][0][0] != this->board[3-i][i][i][i])
				break;
			if(i==3)
			{
				winner.color_ = this->board[3][0][0][0];
				for(int x=0; x<4; x++){
					winner.winningRow[x] = 3-x;
					winner.winningColumn[x] = x;
					winner.winningHeight[x] = x;
					winner.winningNumber[x] = x;
				}
				return winner;
			}
		}
		for(int i=1; i<4; i++)
		{
			if(this->board[i][3-i][i][i]==mini4DgamePlayerColor::none || this->board[0][3][0][0] != this->board[i][3-i][i][i])
				break;
			if(i==3)
			{
				winner.color_ = this->board[0][3][0][0];
				for(int x=0; x<4; x++){
					winner.winningRow[x] = x;
					winner.winningColumn[x] = 3-x;
					winner.winningHeight[x] = x;
					winner.winningNumber[x] = x;
				}
				return winner;
			}
		}
		for(int i=1; i<4; i++)
		{
			if(this->board[i][i][3-i][i]==mini4DgamePlayerColor::none || this->board[0][0][3][0] != this->board[i][i][3-i][i])
				break;
			if(i==3)
			{
				winner.color_ = this->board[0][0][3][0];
				for(int x=0; x<4; x++){
					winner.winningRow[x] = x;
					winner.winningColumn[x] = x;
					winner.winningHeight[x] = 3-x;
					winner.winningNumber[x] = x;
				}
				return winner;
			}
		}
		for(int i=1; i<4; i++)
		{
			if(this->board[i][i][i][3-i]==mini4DgamePlayerColor::none || this->board[0][0][0][3] != this->board[i][i][i][3-i])
				break;
			if(i==3)
			{
				winner.color_ = this->board[0][0][0][3];
				for(int x=0; x<4; x++){
					winner.winningRow[x] = x;
					winner.winningColumn[x] = x;
					winner.winningHeight[x] = x;
					winner.winningNumber[x] = 3-x;
				}
				return winner;
			}
		}
		for(int i=1; i<4; i++)
		{
			if(this->board[3-i][3-i][i][i]==mini4DgamePlayerColor::none || this->board[3][3][0][0] != this->board[3-i][3-i][i][i])
				break;
			if(i==3)
			{
				winner.color_ = this->board[3][3][0][0];
				for(int x=0; x<4; x++){
					winner.winningRow[x] = 3-x;
					winner.winningColumn[x] = 3-x;
					winner.winningHeight[x] = x;
					winner.winningNumber[x] = x;
				}
				return winner;
			}
		}
		for(int i=1; i<4; i++)
		{
			if(this->board[3-i][i][3-i][i]==mini4DgamePlayerColor::none || this->board[3][0][3][0] != this->board[3-i][i][3-i][i])
				break;
			if(i==3)
			{
				winner.color_ = this->board[3][0][3][0];
				for(int x=0; x<4; x++){
					winner.winningRow[x] = 3-x;
					winner.winningColumn[x] = x;
					winner.winningHeight[x] = 3-x;
					winner.winningNumber[x] = x;
				}
				return winner;
			}
		}
		for(int i=1; i<4; i++)
		{
			if(this->board[3-i][i][i][3-i]==mini4DgamePlayerColor::none || this->board[3][0][0][3] != this->board[3-i][i][i][3-i])
				break;
			if(i==3)
			{
				winner.color_ = this->board[3][0][0][3];
				for(int x=0; x<4; x++){
					winner.winningRow[x] = 3-x;
					winner.winningColumn[x] = x;
					winner.winningHeight[x] = x;
					winner.winningNumber[x] = 3-x;
				}
				return winner;
			}
		}

		//check diagonals rows-columns-height
		for(int l=0; l<4; l++)
		{
			for(int i=1; i<4; i++)
			{
				if(this->board[i][i][i][l]==mini4DgamePlayerColor::none || this->board[0][0][0][l] != this->board[i][i][i][l])
					break;
				if(i==3)
				{
					winner.color_ = this->board[0][0][0][l];
					for(int x=0; x<4; x++){
						winner.winningRow[x] = x;
						winner.winningColumn[x] = x;
						winner.winningHeight[x] = x;
						winner.winningNumber[x] = l;
					}
					return winner;
				}
			}
			for(int i=1; i<4; i++)
			{
				if(this->board[3-i][i][i][l]==mini4DgamePlayerColor::none || this->board[3][0][0][l] != this->board[3-i][i][i][l])
					break;
				if(i==3)
				{
					winner.color_ = this->board[3][0][0][l];
					for(int x=0; x<4; x++){
						winner.winningRow[x] = 3-x;
						winner.winningColumn[x] = x;
						winner.winningHeight[x] = x;
						winner.winningNumber[x] = l;
					}
					return winner;
				}
			}
			for(int i=1; i<4; i++)
			{
				if(this->board[i][3-i][i][l]==mini4DgamePlayerColor::none || this->board[0][3][0][l] != this->board[i][3-i][i][l])
					break;
				if(i==3)
				{
					winner.color_ = this->board[0][3][0][l];
					for(int x=0; x<4; x++){
						winner.winningRow[x] = x;
						winner.winningColumn[x] = 3-x;
						winner.winningHeight[x] = x;
						winner.winningNumber[x] = l;
					}
					return winner;
				}
			}
			for(int i=1; i<4; i++)
			{
				if(this->board[i][i][3-i][l]==mini4DgamePlayerColor::none || this->board[0][0][3][l] != this->board[i][i][3-i][l])
					break;
				if(i==3)
				{
					winner.color_ = this->board[0][0][3][l];
					for(int x=0; x<4; x++){
						winner.winningRow[x] = x;
						winner.winningColumn[x] = x;
						winner.winningHeight[x] = 3-x;
						winner.winningNumber[x] = l;
					}
					return winner;
				}
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
				{
					winner.color_ = this->board[0][0][l][0];
					for(int x=0; x<4; x++){
						winner.winningRow[x] = x;
						winner.winningColumn[x] = x;
						winner.winningHeight[x] = l;
						winner.winningNumber[x] = x;
					}
					return winner;
				}
			}
			for(int i=1; i<4; i++)
			{
				if(this->board[3-i][i][l][i]==mini4DgamePlayerColor::none || this->board[3][0][l][0] != this->board[3-i][i][l][i])
					break;
				if(i==3)
				{
					winner.color_ = this->board[3][0][l][0];
					for(int x=0; x<4; x++){
						winner.winningRow[x] = 3-x;
						winner.winningColumn[x] = x;
						winner.winningHeight[x] = l;
						winner.winningNumber[x] = x;
					}
					return winner;
				}
			}
			for(int i=1; i<4; i++)
			{
				if(this->board[i][3-i][l][i]==mini4DgamePlayerColor::none || this->board[0][3][l][0] != this->board[i][3-i][l][i])
					break;
				if(i==3)
				{
					winner.color_ = this->board[0][3][l][0];
					for(int x=0; x<4; x++){
						winner.winningRow[x] = x;
						winner.winningColumn[x] = 3-x;
						winner.winningHeight[x] = l;
						winner.winningNumber[x] = x;
					}
					return winner;
				}
			}
			for(int i=1; i<4; i++)
			{
				if(this->board[i][i][l][3-i]==mini4DgamePlayerColor::none || this->board[0][0][l][3] != this->board[i][i][l][3-i])
					break;
				if(i==3)
				{
					winner.color_ = this->board[0][0][l][3];
					for(int x=0; x<4; x++){
						winner.winningRow[x] = x;
						winner.winningColumn[x] = x;
						winner.winningHeight[x] = l;
						winner.winningNumber[x] = 3-x;
					}
					return winner;
				}
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
				{
					winner.color_ = this->board[0][l][0][0];
					for(int x=0; x<4; x++){
						winner.winningRow[x] = x;
						winner.winningColumn[x] = l;
						winner.winningHeight[x] = x;
						winner.winningNumber[x] = x;
					}
					return winner;
				}
			}
			for(int i=1; i<4; i++)
			{
				if(this->board[3-i][l][i][i]==mini4DgamePlayerColor::none || this->board[3][l][0][0] != this->board[3-i][l][i][i])
					break;
				if(i==3)
				{
					winner.color_ = this->board[3][l][0][0];
					for(int x=0; x<4; x++){
						winner.winningRow[x] = 3-x;
						winner.winningColumn[x] = l;
						winner.winningHeight[x] = x;
						winner.winningNumber[x] = x;
					}
					return winner;
				}
			}
			for(int i=1; i<4; i++)
			{
				if(this->board[i][l][3-i][i]==mini4DgamePlayerColor::none || this->board[0][l][3][0] != this->board[i][l][3-i][i])
					break;
				if(i==3)
				{
					winner.color_ = this->board[0][l][3][0];
					for(int x=0; x<4; x++){
						winner.winningRow[x] = x;
						winner.winningColumn[x] = l;
						winner.winningHeight[x] = 3-x;
						winner.winningNumber[x] = x;
					}
					return winner;
				}
			}
			for(int i=1; i<4; i++)
			{
				if(this->board[i][l][i][3-i]==mini4DgamePlayerColor::none || this->board[0][l][0][3] != this->board[i][l][i][3-i])
					break;
				if(i==3)
				{
					winner.color_ = this->board[0][l][0][3];
					for(int x=0; x<4; x++){
						winner.winningRow[x] = x;
						winner.winningColumn[x] = l;
						winner.winningHeight[x] = x;
						winner.winningNumber[x] = 3-x;
					}
					return winner;
				}
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
				{
					winner.color_ = this->board[l][0][0][0];
					for(int x=0; x<4; x++){
						winner.winningRow[x] = l;
						winner.winningColumn[x] = x;
						winner.winningHeight[x] = x;
						winner.winningNumber[x] = x;
					}
					return winner;
				}
			}
			for(int i=1; i<4; i++)
			{
				if(this->board[l][3-i][i][i]==mini4DgamePlayerColor::none || this->board[l][3][0][0] != this->board[l][3-i][i][i])
					break;
				if(i==3)
				{
					winner.color_ = this->board[l][3][0][0];
					for(int x=0; x<4; x++){
						winner.winningRow[x] = l;
						winner.winningColumn[x] = 3-x;
						winner.winningHeight[x] = x;
						winner.winningNumber[x] = x;
					}
					return winner;
				}
			}
			for(int i=1; i<4; i++)
			{
				if(this->board[l][i][3-i][i]==mini4DgamePlayerColor::none || this->board[l][0][3][0] != this->board[l][i][3-i][i])
					break;
				if(i==3)
				{
					winner.color_ = this->board[l][0][3][0];
					for(int x=0; x<4; x++){
						winner.winningRow[x] = l;
						winner.winningColumn[x] = x;
						winner.winningHeight[x] = 3-x;
						winner.winningNumber[x] = x;
					}
					return winner;
				}
			}
			for(int i=1; i<4; i++)
			{
				if(this->board[l][i][i][3-i]==mini4DgamePlayerColor::none || this->board[l][0][0][3] != this->board[l][i][i][3-i])
					break;
				if(i==3)
				{
					winner.color_ = this->board[l][0][0][3];
					for(int x=0; x<4; x++){
						winner.winningRow[x] = l;
						winner.winningColumn[x] = x;
						winner.winningHeight[x] = x;
						winner.winningNumber[x] = 3-x;
					}
					return winner;
				}
			}
		}

		//check 2d diagonals
		for(int k=0;k<4;k++){
    		for(int l=0;l<4;l++){
    			//rows-columns
    			for(int i=1; i<4; i++)
				{
					if(this->board[i][i][k][l]==mini4DgamePlayerColor::none || this->board[0][0][k][l] != this->board[i][i][k][l])
						break;
					if(i==3)
					{
						winner.color_ = this->board[0][0][k][l];
						for(int x=0; x<4; x++){
							winner.winningRow[x] = x;
							winner.winningColumn[x] = x;
							winner.winningHeight[x] = k;
							winner.winningNumber[x] = l;
						}
						return winner;
					}
				}
				for(int i=1; i<4; i++)
				{
					if(this->board[3-i][i][k][l]==mini4DgamePlayerColor::none || this->board[3][0][k][l] != this->board[3-i][i][k][l])
						break;
					if(i==3)
					{
						winner.color_ = this->board[3][0][k][l];
						for(int x=0; x<4; x++){
							winner.winningRow[x] = 3-x;
							winner.winningColumn[x] = x;
							winner.winningHeight[x] = k;
							winner.winningNumber[x] = l;
						}
						return winner;
					}
				}
				//rows-height
    			for(int i=1; i<4; i++)
    			{
    				if(this->board[i][k][i][l]==mini4DgamePlayerColor::none || this->board[0][k][0][l] != this->board[i][k][i][l])
    					break;
    				if(i==3)
    				{
						winner.color_ = this->board[0][k][0][l];
						for(int x=0; x<4; x++){
							winner.winningRow[x] = x;
							winner.winningColumn[x] = k;
							winner.winningHeight[x] = x;
							winner.winningNumber[x] = l;
						}
						return winner;
					}
				}
				for(int i=1; i<4; i++)
    			{
    				if(this->board[3-i][k][i][l]==mini4DgamePlayerColor::none || this->board[3][k][0][l] != this->board[3-i][k][i][l])
    					break;
    				if(i==3)
    				{
						winner.color_ = this->board[3][k][0][l];
						for(int x=0; x<4; x++){
							winner.winningRow[x] = 3-x;
							winner.winningColumn[x] = k;
							winner.winningHeight[x] = x;
							winner.winningNumber[x] = l;
						}
						return winner;
					}
				}
				//rows-numbers
				for(int i=1; i<4; i++)
    			{
    				if(this->board[i][k][l][i]==mini4DgamePlayerColor::none || this->board[0][k][l][0] != this->board[i][k][l][i])
    					break;
    				if(i==3)
    				{
						winner.color_ = this->board[0][k][l][0];
						for(int x=0; x<4; x++){
							winner.winningRow[x] = x;
							winner.winningColumn[x] = k;
							winner.winningHeight[x] = l;
							winner.winningNumber[x] = x;
						}
						return winner;
					}
				}
				for(int i=1; i<4; i++)
    			{
    				if(this->board[3-i][k][l][i]==mini4DgamePlayerColor::none || this->board[3][k][l][0] != this->board[3-i][k][l][i])
    					break;
    				if(i==3)
    				{
						winner.color_ = this->board[3][k][l][0];
						for(int x=0; x<4; x++){
							winner.winningRow[x] = 3-x;
							winner.winningColumn[x] = k;
							winner.winningHeight[x] = l;
							winner.winningNumber[x] = x;
						}
						return winner;
					}
				}
				//column-height
				for(int i=1; i<4; i++)
    			{
    				if(this->board[k][i][i][l]==mini4DgamePlayerColor::none || this->board[k][0][0][l] != this->board[k][i][i][l])
    					break;
    				if(i==3)
    				{
						winner.color_ = this->board[k][0][0][l];
						for(int x=0; x<4; x++){
							winner.winningRow[x] = k;
							winner.winningColumn[x] = x;
							winner.winningHeight[x] = x;
							winner.winningNumber[x] = l;
						}
						return winner;
					}
				}
				for(int i=1; i<4; i++)
    			{
    				if(this->board[k][3-i][i][l]==mini4DgamePlayerColor::none || this->board[k][3][0][l] != this->board[k][3-i][i][l])
    					break;
    				if(i==3)
    				{
						winner.color_ = this->board[k][3][0][l];
						for(int x=0; x<4; x++){
							winner.winningRow[x] = k;
							winner.winningColumn[x] = 3-x;
							winner.winningHeight[x] = x;
							winner.winningNumber[x] = l;
						}
						return winner;
					}
				}
				//column-numbers
				for(int i=1; i<4; i++)
    			{
    				if(this->board[k][i][l][i]==mini4DgamePlayerColor::none || this->board[k][0][l][0] != this->board[k][i][l][i])
    					break;
    				if(i==3)
    				{
						winner.color_ = this->board[k][0][l][0];
						for(int x=0; x<4; x++){
							winner.winningRow[x] = k;
							winner.winningColumn[x] = x;
							winner.winningHeight[x] = l;
							winner.winningNumber[x] = x;
						}
						return winner;
					}
				}
				for(int i=1; i<4; i++)
    			{
    				if(this->board[k][3-i][l][i]==mini4DgamePlayerColor::none || this->board[k][3][l][0] != this->board[k][3-i][l][i])
    					break;
    				if(i==3)
    				{
						winner.color_ = this->board[k][3][l][0];
						for(int x=0; x<4; x++){
							winner.winningRow[x] = k;
							winner.winningColumn[x] = 3-x;
							winner.winningHeight[x] = l;
							winner.winningNumber[x] = x;
						}
						return winner;
					}
				}
				//height-numbers
				for(int i=1; i<4; i++)
    			{
    				if(this->board[k][l][i][i]==mini4DgamePlayerColor::none || this->board[k][l][0][0] != this->board[k][l][i][i])
    					break;
    				if(i==3)
    				{
						winner.color_ = this->board[k][l][0][0];
						for(int x=0; x<4; x++){
							winner.winningRow[x] = k;
							winner.winningColumn[x] = l;
							winner.winningHeight[x] = x;
							winner.winningNumber[x] = x;
						}
						return winner;
					}
				}
				for(int i=1; i<4; i++)
    			{
    				if(this->board[k][l][3-i][i]==mini4DgamePlayerColor::none || this->board[k][l][3][0] != this->board[k][l][3-i][i])
    					break;
    				if(i==3)
    				{
						winner.color_ = this->board[k][l][3][0];
						for(int x=0; x<4; x++){
							winner.winningRow[x] = k;
							winner.winningColumn[x] = l;
							winner.winningHeight[x] = 3-x;
							winner.winningNumber[x] = x;
						}
						return winner;
					}
				}
    		}
    	}

    	//check rows
    	for(int j=0;j<4;j++){
    		for(int k=0;k<4;k++){
    			for(int l=0;l<4;l++){
    				if(this->board[0][j][k][l]!= mini4DgamePlayerColor::none
    				   && this->board[0][j][k][l]==this->board[1][j][k][l]
    				   && this->board[1][j][k][l]==this->board[2][j][k][l]
    				   && this->board[2][j][k][l]==this->board[3][j][k][l])
    				{
    					winner.color_ = this->board[0][j][k][l];
						for(int x=0; x<4; x++){
							winner.winningRow[x] = x;
							winner.winningColumn[x] = j;
							winner.winningHeight[x] = k;
							winner.winningNumber[x] = l;
						}
						return winner;
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
    	    			winner.color_ = this->board[i][0][k][l];
						for(int x=0; x<4; x++){
							winner.winningRow[x] = i;
							winner.winningColumn[x] = x;
							winner.winningHeight[x] = k;
							winner.winningNumber[x] = l;
						}
						return winner;
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
    					winner.color_ = this->board[i][j][0][l];
						for(int x=0; x<4; x++){
							winner.winningRow[x] = i;
							winner.winningColumn[x] = j;
							winner.winningHeight[x] = x;
							winner.winningNumber[x] = l;
						}
						return winner;
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
    					winner.color_ = this->board[i][j][k][0];
						for(int x=0; x<4; x++){
							winner.winningRow[x] = i;
							winner.winningColumn[x] = j;
							winner.winningHeight[x] = k;
							winner.winningNumber[x] = x;
						}
						return winner;
    				}
    			}
    		}
    	}
    	return winner;
    }

    /**
    @brief
        Is called when the gametype has changed.
    */
    void Mini4DgameBoard::changedGametype()
    {
        SUPER(Mini4DgameBoard, changedGametype);

        // Check, whether it's still Mini4Dgame.
        this->checkGametype();
    }

    /**
    @brief
        Checks whether the gametype is Mini4Dgame and if it is, sets its centerpoint.
    */
    void Mini4DgameBoard::checkGametype()
    {
        if (this->getGametype() != NULL && this->getGametype()->isA(Class(Mini4Dgame)))
        {
            Mini4Dgame* Mini4DgameGametype = orxonox_cast<Mini4Dgame*>(this->getGametype().get());
            Mini4DgameGametype->setGameboard(this);
        }
    }
}

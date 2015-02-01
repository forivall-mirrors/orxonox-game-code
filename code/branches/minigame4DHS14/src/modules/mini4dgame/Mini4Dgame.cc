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
 *      Johannes Ritz
 *
 *
 *
 *
 *TASK c) end the game in a nicer way
 *TASK d) save the highscore
 *TASK e) eye candy
 */

/**
    @file Tetris.cc
    @brief Implementation of the Mini4Dgame class.
*/

#include "Mini4Dgame.h"

#include "core/CoreIncludes.h"
#include "core/EventIncludes.h"
#include "core/command/Executor.h"
#include "core/config/ConfigValueIncludes.h"
#include "infos/PlayerInfo.h"
#include "core/command/ConsoleCommand.h"

#include "gamestates/GSLevel.h"
#include "chat/ChatManager.h"

namespace orxonox
{

	SetConsoleCommand("Mini4Dgame", "setStone", &Mini4Dgame::setStone).addShortcut();
	SetConsoleCommand("Mini4Dgame", "undoStone", &Mini4Dgame::undoStone).addShortcut();

    RegisterUnloadableClass(Mini4Dgame);

    /**
    @brief
        Constructor. Registers and initializes the object.
    */
    Mini4Dgame::Mini4Dgame(Context* context) : Deathmatch(context)
    {
        RegisterObject(Mini4Dgame);

        this->board_ = 0;

        // Set the type of Bots for this particular Gametype.
        //this->botclass_ = Class(Mini4DgameBot);
    }

    /**
    @brief
        Destructor. Cleans up, if initialized.
    */
    Mini4Dgame::~Mini4Dgame()
    {
        if (this->isInitialized())
            this->cleanup();
    }

    /**
    @brief
        Cleans up the Gametype.
    */
    void Mini4Dgame::cleanup()
    {
    	if(this->board_ != NULL)// Destroy the board, if present.
    	{
    		//this->board_->destroy();
    		this->board_ = 0;
    	}
    }

    /**
    @brief
        Starts the mini4Dgame.
    */
    void Mini4Dgame::start()
    {
        if (this->board_ != NULL) // There needs to be a Mini4DgameCenterpoint, i.e. the area the game takes place.
        {
        	/*
        	if (this->board_ == NULL)
        	{
        		this->board_ = new Mini4DgameBoard(this->board_->getContext());
        		// Apply the template for the ball specified by the centerpoint.
        		this->board_->addTemplate(this->center_->getBoardtemplate());
        	}
        	*/

        	// Attach the board to the centerpoint and set the parameters as specified in the centerpoint, the ball is attached to.

        	this->board_->setPosition(0, 0, 0);

        }
        else // If no centerpoint was specified, an error is thrown and the level is exited.
        {
            orxout(internal_error) << "Mini4Dgame: No Centerpoint specified." << endl;
            GSLevel::startMainMenu();
            return;
        }

        // Set variable to temporarily force the player to spawn.
        bool temp = this->bForceSpawn_;
        this->bForceSpawn_ = true;

        // Call start for the parent class.
        Deathmatch::start();

        // Reset the variable.
        this->bForceSpawn_ = temp;
    }

    /**
    @brief
        Ends the Mini4Dgame minigame.
    */
    void Mini4Dgame::end()
    {
    	this->cleanup();

        // Call end for the parent class.
        Deathmatch::end();
    }


    /**
    @brief
        Spawns player.
    */
    void Mini4Dgame::spawnPlayersIfRequested()
    {
        // first spawn human players to assign always the left bat to the player in singleplayer
        for (std::map<PlayerInfo*, Player>::iterator it = this->players_.begin(); it != this->players_.end(); ++it)
            if (it->first->isHumanPlayer() && (it->first->isReadyToSpawn() || this->bForceSpawn_))
                this->spawnPlayer(it->first);
        // now spawn bots
        for (std::map<PlayerInfo*, Player>::iterator it = this->players_.begin(); it != this->players_.end(); ++it)
            if (!it->first->isHumanPlayer() && (it->first->isReadyToSpawn() || this->bForceSpawn_))
                this->spawnPlayer(it->first);
    }

    /**
    @brief
        Spawns the input player.
    @param player
        The player to be spawned.
    */
    void Mini4Dgame::spawnPlayer(PlayerInfo* player)
    {
        assert(player);

        if(false)//this->player_ == NULL)
        {
            //this->player_ = player;
            this->players_[player].state_ = PlayerState::Alive;
        }
    }

    void Mini4Dgame::undoStone()//Vector4 move, const int playerColor)
    {
    	ObjectList<Mini4DgameBoard>::iterator it = ObjectList<Mini4DgameBoard>::begin();
        it->undoMove();
    }

    //void Mini4Dgame::setStone(Vector4 move, const int playerColor, Mini4DgameBoard* board)
    void Mini4Dgame::setStone(int x,int y,int z,int w)//Vector4 move, const int playerColor)
    {
        Mini4DgamePosition move = Mini4DgamePosition(x,y,z,w);
        ObjectList<Mini4DgameBoard>::iterator it = ObjectList<Mini4DgameBoard>::begin();
        it->makeMove(move);
    }

    void Mini4Dgame::win(Mini4DgameWinner winner)
    {

    }
}

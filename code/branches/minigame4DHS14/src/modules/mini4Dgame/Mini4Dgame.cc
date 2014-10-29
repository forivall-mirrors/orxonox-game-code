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
#include "core/command/Executor.h"
#include "core/config/ConfigValueIncludes.h"

#include "gamestates/GSLevel.h"
#include "chat/ChatManager.h"

#include "Mini4DgameCenterpoint.h"

namespace orxonox
{

    RegisterUnloadableClass(Mini4Dgame);

    /**
    @brief
        Constructor. Registers and initializes the object.
    */
    Mini4Dgame::Mini4Dgame(Context* context) : Deathmatch(context)
    {
        RegisterObject(Mini4Dgame);

        this->center_ = NULL;
        //TODO: player Null setzen
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

    }

    /*
    bool Mini4Dgame::isValidMove(Vector4* move, const Mini4DgameBoard* board)
    {

    }
    */


    /**
    @brief
        Starts the Tetris minigame.
    */
    void Mini4Dgame::start()
    {
        if (this->center_ != NULL) // There needs to be a Mini4DgameCenterpoint, i.e. the area the game takes place.
        {
            //TODO: create all objects if they don't exist so far and attach the parameters specified in the centerpoint to them
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


    /**
    @brief
        Get the player.
    @return
        Returns a pointer to the player. If there is no player, NULL is returned.
    */
    //TODO: colors
    PlayerInfo* Mini4Dgame::getPlayer(int color) const
    {
        return players[color];
    	//for(int i=0;i<NUMBEROFPLAYERS;i++)
        	//if(color == this->mini4DgamePlayers[i].color)
        		//return this->mini4DgamePlayers[i].info;
    }

}

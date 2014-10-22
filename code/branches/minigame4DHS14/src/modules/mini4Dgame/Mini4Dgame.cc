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

        this->center_ = 0;
        this->player_[0] = NULL;
        this->player_[1] = NULL;
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

---------------------------------------------------------------------------------------------------------------------------------

    /**
    @brief
        Spawns player.
    */
    void Mini4Dgame::spawnPlayersIfRequested()
    {
        // Spawn a human player.
        for (std::map<PlayerInfo*, Player>::iterator it = this->players_.begin(); it != this->players_.end(); ++it)
            if (it->first->isHumanPlayer() && (it->first->isReadyToSpawn() || this->bForceSpawn_))
                this->spawnPlayer(it->first);
    }
    
    bool Tetris::playerLeft(PlayerInfo* player)
    {
        bool left = Gametype::playerLeft(player);
        if(player && player->isHumanPlayer())
        {
            if(this->activeBrick_ != NULL)
            {
                this->player_->stopControl();
            }
            this->cleanup();
        }
        return left;
    }

    /**
    @brief
        Spawns the input player.
    @param player
        The player to be spawned.
    */
    void Tetris::spawnPlayer(PlayerInfo* player)
    {
        assert(player);

        if(this->player_ == NULL)
        {
            this->player_ = player;
            this->players_[player].state_ = PlayerState::Alive;
        }
    }



    void Tetris::startBrick(void)
    {
        if(this->player_ == NULL)
            return;

        unsigned int cameraIndex = 0;
        if(this->activeBrick_ != NULL)
        {
            // Get camera settings
            cameraIndex = this->activeBrick_->getCurrentCameraIndex();
            this->player_->stopControl();
            // destroy old active brick
            this->activeBrick_->destroy();
        }

        // Make the last brick to be created the active brick.
        this->activeBrick_ = this->futureBrick_;
        this->futureBrick_ = 0;

        // set its position
        this->player_->startControl(this->activeBrick_);
        float xPos = (this->center_->getWidth()/2 + ((this->center_->getWidth() % 2)*2-1)/2.0f)*this->center_->getStoneSize();
        float yPos = (this->center_->getHeight()-0.5f)*this->center_->getStoneSize();
        this->activeBrick_->setPosition(xPos, yPos, 0.0f);
        this->activeBrick_->setVelocity(0.0f, -this->center_->getStoneSpeed(), 0.0f);
        this->activeBrick_->setCameraPosition(cameraIndex);

        // create a new future brick
        this->createBrick();

        // check if the new brick is in a valid position, otherwise end the game
        if (!this->isValidBrickPosition(this->activeBrick_))
            this->end();
    }

    void Tetris::createBrick(void)             //TODO: random rotation offset between 0 and 3 (times 90°)
    {
        // create new futureBrick_
        this->futureBrick_ = new TetrisBrick(this->center_->getContext());


        // Apply the stone template to the stone.
        this->futureBrick_->addTemplate(this->center_->getBrickTemplate());

        // Attach the brick to the Centerpoint and set the position of the brick to be at the left side.
        this->center_->attach(this->futureBrick_);
        float xPos = (this->center_->getWidth()*1.6f + ((this->center_->getWidth() % 2)*2-1)/2.0f)*this->center_->getStoneSize();
        float yPos = (this->center_->getHeight()-5.1f)*this->center_->getStoneSize();
        
        this->futureBrick_->setPosition(xPos, yPos, 0.0f);
        this->futureBrick_->setGame(this);
    }


    /**
    @brief
        Get the player.
    @return
        Returns a pointer to the player. If there is no player, NULL is returned.
    */
    PlayerInfo* Tetris::getPlayer(void) const
    {
        return this->player_;
    }

    /*TetrisCenterpoint* Tetris::getCenterpoint(void) const
    {
        return this->center_;
    }*/

    /**
    @brief Set the TetrisCenterpoint (the playing field).
    @param center A pointer to the TetrisCenterpoint to be set.
    */
    void Tetris::setCenterpoint(TetrisCenterpoint* center)
    {
        this->center_ = center;
    }

    /**
    @brief Check each row if it is full. Removes all full rows. Update
    @brief Manages score.
    */
    void Tetris::findFullRows()
    {
        unsigned int correctPosition = 0;
        unsigned int stonesPerRow = 0;
        for (unsigned int row = 0; row < this->center_->getHeight(); row++)
        {
            stonesPerRow = 0;
            for(std::list<SmartPtr<TetrisStone> >::iterator it = this->stones_.begin(); it != this->stones_.end(); )
            {
                std::list<SmartPtr<TetrisStone> >::iterator it_temp = it++;
                correctPosition = static_cast<unsigned int>(((*it_temp)->getPosition().y - 5)/this->center_->getStoneSize());
                if(correctPosition == row)
                {
                    stonesPerRow++;
                    if(stonesPerRow == this->center_->getWidth())
                    {
                        clearRow(row);
                        row--; //the row counter has to be decreased in order to detect multiple rows!
                        this->playerScored(this->player_);// add points
                        //increase the stone's speed
                        this->center_->setStoneSpeed(this->center_->getStoneSpeed()+1.0f);
                    }
                }
            }
        }
    }

    void Tetris::clearRow(unsigned int row)
    {// clear the full row
        for(std::list<SmartPtr<TetrisStone> >::iterator it = this->stones_.begin(); it != this->stones_.end(); )
        {
            if(static_cast<unsigned int>(((*it)->getPosition().y - 5)/this->center_->getStoneSize()) == row)
            {
                (*it)->destroy();
                this->stones_.erase(it++);
            }
            else
                ++it;
        }
      // adjust height of stones above the deleted row //TODO: check if this could be a source of a bug.
        for(std::list<SmartPtr<TetrisStone> >::iterator it = this->stones_.begin(); it != this->stones_.end(); ++it)
        {
            if(static_cast<unsigned int>(((*it)->getPosition().y - 5)/this->center_->getStoneSize()) > row)
                (*it)->setPosition((*it)->getPosition()-Vector3(0,10,0));
        }

    }


}

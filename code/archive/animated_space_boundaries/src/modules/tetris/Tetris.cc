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
 *      ...
 *
 */

/**
    @file Tetris.cc
    @brief Implementation of the Tetris class.
*/

#include "Tetris.h"

#include "core/CoreIncludes.h"
#include "core/EventIncludes.h"
#include "core/command/Executor.h"

#include "gamestates/GSLevel.h"

#include "TetrisCenterpoint.h"
#include "TetrisStone.h"
#include "infos/PlayerInfo.h"

namespace orxonox
{

    CreateUnloadableFactory(Tetris);

    /**
    @brief
        Constructor. Registers and initializes the object.
    */
    Tetris::Tetris(BaseObject* creator) : Deathmatch(creator)
    {
        RegisterObject(Tetris);

        this->activeStone_ = NULL;

        // Pre-set the timer, but don't start it yet.
        this->starttimer_.setTimer(1.0, false, createExecutor(createFunctor(&Tetris::startStone, this)));
        this->starttimer_.stopTimer();

        this->player_ = NULL;
    }

    /**
    @brief
        Destructor. Cleans up, if initialized.
    */
    Tetris::~Tetris()
    {
        if (this->isInitialized())
            this->cleanup();
    }

    /**
    @brief
        Cleans up the Gametype.
    */
    void Tetris::cleanup()
    {

    }

    void Tetris::tick(float dt)
    {
        SUPER(Tetris, tick, dt);

        if(this->activeStone_ != NULL)
        {
            if(!this->isValidStonePosition(this->activeStone_, this->activeStone_->getPosition()))
            {
                this->activeStone_->setVelocity(Vector3::ZERO);
                this->createStone();
                this->startStone();
            }
        }
    }

    bool Tetris::isValidMove(TetrisStone* stone, const Vector3& position)
    {
        assert(stone);

        if(position.x < this->center_->getStoneSize()/2.0)  //!< If the stone touches the left edge of the level
            return false;
        else if(position.x > (this->center_->getWidth()-0.5)*this->center_->getStoneSize()) //!< If the stone touches the right edge of the level
            return false;

        for(std::vector<TetrisStone*>::const_iterator it = this->stones_.begin(); it != this->stones_.end(); ++it)
        {
            if(stone == *it)
                continue;

            const Vector3& currentStonePosition = (*it)->getPosition(); //!< Saves the position of the currentStone

            if((position.x == currentStonePosition.x) && abs(position.y-currentStonePosition.y) < this->center_->getStoneSize())
                return false;
        }

        return true;
    }

    bool Tetris::isValidStonePosition(TetrisStone* stone, const Vector3& position)
    {
        assert(stone);

        // we use a reverse iterator because we have to check for collisions with the topmost stones first
        for(std::vector<TetrisStone*>::const_reverse_iterator it = this->stones_.rbegin(); it != this->stones_.rend(); ++it)
        {
            if(stone == *it)
                continue;

            const Vector3& currentStonePosition = (*it)->getPosition(); //!< Saves the position of the currentStone

            if((position.x == currentStonePosition.x) && (position.y < currentStonePosition.y + this->center_->getStoneSize()))
            {
                this->activeStone_->setPosition(Vector3(this->activeStone_->getPosition().x, currentStonePosition.y+this->center_->getStoneSize(), this->activeStone_->getPosition().z));
                return false;
            }// This case applies if the stones overlap partially vertically
        }

        // after we checked for collision with all stones, we also check for collision with the bottom
        if(position.y < this->center_->getStoneSize()/2.0f) //!< If the stone has reached the bottom of the level
        {
            stone->setPosition(Vector3(stone->getPosition().x, this->center_->getStoneSize()/2.0f, stone->getPosition().z));
            return false;
        }

        return true;
    }

    /**
    @brief
        Starts the Tetris minigame.
    */
    void Tetris::start()
    {
        if (this->center_ != NULL) // There needs to be a TetrisCenterpoint, i.e. the area the game takes place.
        {
            // Create the first stone.
            this->createStone();
        }
        else // If no centerpoint was specified, an error is thrown and the level is exited.
        {
            COUT(1) << "Error: No Centerpoint specified." << std::endl;
            GSLevel::startMainMenu();
            return;
        }

        // Start the timer. After it has expired the stone is started.
        this->starttimer_.startTimer();

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
        Ends the Tetris minigame.
    */
    void Tetris::end()
    {
        this->cleanup();

        // Call end for the parent class.
        Deathmatch::end();
    }

    /**
    @brief
        Spawns player.
    */
    void Tetris::spawnPlayersIfRequested()
    {
        // Spawn a human player.
        for (std::map<PlayerInfo*, Player>::iterator it = this->players_.begin(); it != this->players_.end(); ++it)
            if (it->first->isHumanPlayer() && (it->first->isReadyToSpawn() || this->bForceSpawn_))
                this->spawnPlayer(it->first);
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

    /**
    @brief
        Starts the first stone.
    */
    void Tetris::startStone(void)
    {
        if(this->player_ == NULL)
            return;

        unsigned int cameraIndex = 0;
        if(this->activeStone_ != NULL)
        {
            // Get camera settings
            cameraIndex = this->activeStone_->getCurrentCameraIndex();
            this->player_->stopControl();
        }
        
        // Make the last stone to be created the active stone.
        this->activeStone_ = this->stones_.back();
        
        this->player_->startControl(this->activeStone_);
        this->activeStone_->setVelocity(0.0f, -this->center_->getStoneSpeed(), 0.0f);
        this->activeStone_->setCameraPosition(cameraIndex);
    }

    /**
    @brief
        Creates a new stone.
    */
    void Tetris::createStone(void)
    {
        // Create a new stone and add it to the list of stones.
        TetrisStone* stone = new TetrisStone(this->center_);
        this->stones_.push_back(stone);
        
        // Apply the stone template to the stone.
        stone->addTemplate(this->center_->getStoneTemplate());
        
        // Attach the stone to the Centerpoint and set the position of the stone to be at the top middle.
        this->center_->attach(stone);
        float xPos = (this->center_->getWidth()/2 + ((this->center_->getWidth() % 2)*2-1)/2.0f)*this->center_->getStoneSize();
        float yPos = (this->center_->getHeight()-0.5f)*this->center_->getStoneSize();
        stone->setPosition(xPos, yPos, 0.0f);
        stone->setGame(this);
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

    /**
    @brief Set the TetrisCenterpoint (the playing field).
    @param center A pointer to the TetrisCenterpoint to be set.
    */
    void Tetris::setCenterpoint(TetrisCenterpoint* center)
    {
        this->center_ = center;
    }

}

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
 *BUG c) destroying the old stones causes segfault -> WeakPointer as solution ?
 *BUG d) wrong collision detection: sometimes stones "bounce off"
 *
 *
 *TASK c) end the game in a nicer way
 *TASK d) save the highscore
 *TASK e) eye candy
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
#include "TetrisBrick.h"
#include "infos/PlayerInfo.h"

namespace orxonox
{

    CreateUnloadableFactory(Tetris);

    /**
    @brief
        Constructor. Registers and initializes the object.
    @ingroup Tetris
    */
    Tetris::Tetris(BaseObject* creator) : Deathmatch(creator)
    {
        RegisterObject(Tetris);

        this->activeBrick_ = NULL;

        // Pre-set the timer, but don't start it yet.
        this->starttimer_.setTimer(1.0, false, createExecutor(createFunctor(&Tetris::startBrick, this)));
        this->starttimer_.stopTimer();

        this->player_ = NULL;
        this->endGameCriteria_ = 0.0f;
        this->setHUDTemplate("TetrisHUD");
        this->futureBrick_ = NULL;
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
        /*for(int i = 0;i < this->stones_.size(); i++) //TODO: Why isn't there any code like this
        {                                              // compensating the 'new' statement?
            delete this->stones_[i];
        }//*/

    }

    void Tetris::tick(float dt)
    {
        SUPER(Tetris, tick, dt);

        if((this->activeBrick_ != NULL)&&(!this->hasEnded()))
        {
        	this->endGameCriteria_ += dt;
            if(!this->isValidBrickPosition(this->activeBrick_, this->activeBrick_->getPosition()))
            {
                this->activeBrick_->setVelocity(Vector3::ZERO);
                this->activeBrick_->releaseStones(this->center_);
                //delete this->activeBrick_; //releasing the memory
                this->findFullRows();
                if(this->endGameCriteria_ < 0.1f) //end game if two bricks are created within a 0.1s interval.
                    this->end();
                this->createBrick();
                this->startBrick();
                this->endGameCriteria_ = 0.0f;
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

    /**
    @brief
        Check for each stone in a brick if it is moved the right way.
    */
    bool Tetris::isValidMove(TetrisBrick* brick, const Vector3& position, bool isRotation = false)
    {
        assert(brick);

        for (unsigned int i = 0; i < brick->getNumberOfStones(); i++ )
        {
            TetrisStone* stone = brick->getStone(i);
            Vector3 stonePosition; //the current stone's offset to position
            if(isRotation)
            	stonePosition = rotateVector(stone->getPosition(), brick->getRotationCount()+1);
            else
                stonePosition = rotateVector(stone->getPosition(), brick->getRotationCount());

            if(! this->isValidMove(stone, position + stonePosition ))
            {
                return false;
            }

            //catch illegal rotation (such that collisions with ground are not permitted)
            if(isRotation)
            {
                if((position + stonePosition).y < this->center_->getStoneSize()/2.0f) //!< If the stone has reached the bottom of the level
                {
                    return false;
                }
            }
        }
        return true;

    }



    bool Tetris::isValidStonePosition(TetrisStone* stone, const Vector3& position)
    {
        assert(stone);

        // we use a reverse iterator because we have to check for collisions with the topmost stones first
        for(std::vector<TetrisStone*>::const_reverse_iterator it = this->stones_.rbegin(); it != this->stones_.rend(); ++it)
        {
            if(this->activeBrick_->contains(*it))//skip the moving brick' stones
                continue;
            //Vector3 currentStonePosition = rotateVector((*it)->getPosition(), this->activeBrick_->getRotationCount());
            const Vector3& currentStonePosition = (*it)->getPosition(); //!< Saves the position of the currentStone
            //!< Saves the position of the currentStone

            //filter out cases where the falling stone is already below a steady stone
            if(position.y < currentStonePosition.y - this->center_->getStoneSize()/2.0f)
                continue;
            if((position.x == currentStonePosition.x) && (position.y < currentStonePosition.y + this->center_->getStoneSize()))
            {
                float y_offset = static_cast<int>((this->activeBrick_->getPosition().y-currentStonePosition.y+10)/10)*10 + currentStonePosition.y;
                if(y_offset < 0) //filter out extreme cases (very rare bug)
                	y_offset = 0;
                this->activeBrick_->setPosition(Vector3(this->activeBrick_->getPosition().x, y_offset, this->activeBrick_->getPosition().z));
                return false;
            }// This case applies if the stones overlap partially vertically
        }

        // after we checked for collision with all stones, we also check for collision with the bottom
        if(position.y < this->center_->getStoneSize()/2.0f) //!< If the stone has reached the bottom of the level
        {
        	float yOffset = stone->getPosition().y + this->center_->getStoneSize()/2.0f;//calculate offset
        	if(yOffset < 0) //catch brake-throughs
        	    yOffset = 0;
        	this->activeBrick_->setPosition(Vector3(this->activeBrick_->getPosition().x, yOffset, this->activeBrick_->getPosition().z));
            return false;
        }

        return true;
    }
    /**
     * @brief This function determines wether a brick touches another brick or the ground.
     *
     */
    bool Tetris::isValidBrickPosition(TetrisBrick* brick, const Vector3& position)
    {
        assert(brick);

        for (unsigned int i = 0; i < brick->getNumberOfStones(); i++ )
        {
            TetrisStone* stone = brick->getStone(i);
            Vector3 stonePosition = rotateVector(stone->getPosition(), brick->getRotationCount());
            if(! this->isValidStonePosition(stone, position + stonePosition) )
                return false;
        }
        return true;
    }

    /**
    @brief
        A Vector3 is rolled 90 * degrees * amount (anticlockwise rotation)
    */
    Vector3 Tetris::rotateVector(Vector3 position, unsigned int amount)
    {
    	float temp = 0;
        for(unsigned int i = 0; i < amount; i++)
        {
            temp = position.x;
            position.x = -position.y;
            position.y = temp;
        }
        return position;
    }

    /**
    @brief
        Starts the Tetris minigame.
    */
    void Tetris::start()
    {
        if (this->center_ != NULL) // There needs to be a TetrisCenterpoint, i.e. the area the game takes place.
        {
            // Create the futureBrick_
            this->createBrick();
            // Create the first brick.
            this->createBrick();
        }
        else // If no centerpoint was specified, an error is thrown and the level is exited.
        {
            orxout(internal_error) << "Tetris: No Centerpoint specified." << endl;
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
        this->activeBrick_->setVelocity(Vector3::ZERO);
        if(this->activeBrick_ != NULL)
        {
            this->player_->stopControl();
        }

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
        }

        // Make the last brick to be created the active brick.
        this->activeBrick_ = this->bricks_.back();

        this->player_->startControl(this->activeBrick_);
        this->activeBrick_->setVelocity(0.0f, -this->center_->getStoneSpeed(), 0.0f);
        this->activeBrick_->setCameraPosition(cameraIndex);
    }

    void Tetris::createBrick(void)             //TODO: random rotation offset between 0 and 3 (times 90°)
    {
        // Use the futureBrick_ as new currentBrick by setting its position and storing it in this->bricks
        if(this->futureBrick_ != NULL)
        {
            for (unsigned int i = 0; i < this->futureBrick_->getNumberOfStones(); i++)
            {
                this->stones_.push_back(this->futureBrick_->getStone(i));
            }
            float xPos = (this->center_->getWidth()/2 + ((this->center_->getWidth() % 2)*2-1)/2.0f)*this->center_->getStoneSize();
            float yPos = (this->center_->getHeight()-0.5f)*this->center_->getStoneSize();
            this->futureBrick_->setPosition(xPos, yPos, 0.0f);
            this->bricks_.push_back(this->futureBrick_);
        }
        // create new futureBrick_
        this->futureBrick_ = new TetrisBrick(this->center_);


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
            for(std::vector<TetrisStone*>::iterator it = this->stones_.begin(); it != this->stones_.end(); ++it)
            {
            	correctPosition = static_cast<unsigned int>(((*it)->getPosition().y - 5)/this->center_->getStoneSize());
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
        for(std::vector<TetrisStone*>::iterator it = this->stones_.begin(); it != this->stones_.end(); ++it)
        {
            if(static_cast<unsigned int>(((*it)->getPosition().y - 5)/this->center_->getStoneSize()) == row)
            	(*it)->setPosition(Vector3(-50,-50,100));
            	//{(*it)->destroy(); this->stones_.erase(it); orxout()<< "destroy row "<<endl;}//experimental
        }
      // adjust height of stones above the deleted row //TODO: check if this could be a source of a bug.
        for(std::vector<TetrisStone*>::iterator it2 = this->stones_.begin(); it2 != this->stones_.end(); ++it2)
        {
            if(static_cast<unsigned int>(((*it2)->getPosition().y - 5)/this->center_->getStoneSize()) > row)
                (*it2)->setPosition((*it2)->getPosition()-Vector3(0,10,0));
        }

    }


}

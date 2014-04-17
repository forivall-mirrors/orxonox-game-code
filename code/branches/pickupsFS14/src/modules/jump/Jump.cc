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
    @file Jump.cc
    @brief Implementation of the Jump class.
*/

#include "Jump.h"

#include "core/CoreIncludes.h"
#include "core/EventIncludes.h"
#include "core/command/Executor.h"
#include "core/config/ConfigValueIncludes.h"

#include "gamestates/GSLevel.h"
#include "chat/ChatManager.h"

#include "JumpCenterpoint.h"
#include "JumpPlatform.h"
#include "JumpFigure.h"

#include "infos/PlayerInfo.h"

namespace orxonox
{
    // Events to allow to react to scoring of a player, in the level-file.
    CreateEventName(JumpCenterpoint, right);
    CreateEventName(JumpCenterpoint, left);

    RegisterUnloadableClass(Jump);

    /**
    @brief
        Constructor. Registers and initializes the object.
    */
    Jump::Jump(Context* context) : Deathmatch(context)
    {
        RegisterObject(Jump);

        this->center_ = 0;
        this->ball_ = 0;
        this->figure_ = 0;
        this->camera = 0;

        //this->setHUDTemplate("JumpHUD");

        // Pre-set the timer, but don't start it yet.
        this->starttimer_.setTimer(1.0, false, createExecutor(createFunctor(&Jump::startBall, this)));
        this->starttimer_.stopTimer();


        this->scoreLimit_ = 10;
        this->setConfigValues();
    }

    /**
    @brief
        Destructor. Cleans up, if initialized.
    */
    Jump::~Jump()
    {
        if (this->isInitialized())
        {
            this->cleanup();
        }
    }

    void Jump::tick(float dt)
    {
    	SUPER(Jump, tick, dt);

    	if (figure_ != NULL)
    	{
    		Vector3 figurePosition = figure_->getPosition();

    		if (figurePosition.z > totalScreenShift)
    		{
    			totalScreenShift = figurePosition.z;
    		}

    		if (this->camera != NULL)
			{
				Vector3 cameraPosition = Vector3(0,totalScreenShift,0);
				camera->setPosition(cameraPosition);
			}
			else
			{
				orxout() << "no camera found" << endl;
				//this->camera = this->figure_->getCamera();
			}
    	}





    }


    void Jump::setConfigValues()
    {
        SetConfigValue(scoreLimit_, 10).description("The player first reaching those points wins.");
    }

    /**
    @brief
        Cleans up the Gametype by destroying the ball and the bats.
    */
    void Jump::cleanup()
    {
        if (this->ball_ != NULL) // Destroy the ball, if present.
        {
            this->ball_->destroy();
            this->ball_ = 0;
        }

        // Destroy both bats, if present.
		if (this->figure_ != NULL)
		{
			this->figure_->destroy();
			this->figure_ = 0;
		}
		this->camera = 0;
    }

    /**
    @brief
        Starts the Jump minigame.
    */
    void Jump::start()
    {
        if (this->center_ != NULL) // There needs to be a JumpCenterpoint, i.e. the area the game takes place.
        {
            if (this->ball_ == NULL) // If there is no ball, create a new ball.
            {
                this->ball_ = new JumpPlatform(this->center_->getContext());
                // Apply the template for the ball specified by the centerpoint.
                this->ball_->addTemplate(this->center_->getBalltemplate());
            }

            // Attach the ball to the centerpoint and set the parameters as specified in the centerpoint, the ball is attached to.
            this->center_->attach(this->ball_);
            this->ball_->setPosition(0, 0, 0);
            this->ball_->setFieldDimension(this->center_->getFieldDimension());

            // If one of the bats is missing, create it. Apply the template for the bats as specified in the centerpoint.
			if (this->figure_ == NULL)
			{
				this->figure_ = new JumpFigure(this->center_->getContext());
				this->figure_->addTemplate(this->center_->getBattemplate());
			}

            // Attach the bats to the centerpoint and set the parameters as specified in the centerpoint, the bats are attached to.
            this->center_->attach(this->figure_);
            this->figure_->setPosition(-this->center_->getFieldDimension().x / 2, 0, 0);
            this->figure_->yaw(Degree(-90));
            this->figure_->setSpeed(this->center_->getBatSpeed());
            this->figure_->setFieldDimension(this->center_->getFieldDimension());
            this->figure_->setLength(this->center_->getBatLength());

            // Set the bats for the ball.
            this->ball_->setFigure(this->figure_);
        }
        else // If no centerpoint was specified, an error is thrown and the level is exited.
        {
            orxout(internal_error) << "Jump: No Centerpoint specified." << endl;
            GSLevel::startMainMenu();
            return;
        }

        // Start the timer. After it has expired the ball is started.
        this->starttimer_.startTimer();

        // Set variable to temporarily force the player to spawn.
        bool temp = this->bForceSpawn_;
        this->bForceSpawn_ = true;

        // Call start for the parent class.
        Deathmatch::start();

        // Reset the variable.
        this->bForceSpawn_ = temp;

        if (this->figure_ != NULL)
        {
        	this->camera = this->figure_->getCamera();
        }

        totalScreenShift = 0.0;
    }

    /**
    @brief
        Ends the Jump minigame.
    */
    void Jump::end()
    {
        this->cleanup();

        // Call end for the parent class.
        Deathmatch::end();
    }

    /**
    @brief
        Spawns players, and fills the rest up with bots.
    */
    void Jump::spawnPlayersIfRequested()
    {

        // first spawn human players to assign always the left bat to the player in singleplayer
        for (std::map<PlayerInfo*, Player>::iterator it = this->players_.begin(); it != this->players_.end(); ++it)
            if (it->first->isHumanPlayer() && (it->first->isReadyToSpawn() || this->bForceSpawn_))
                this->spawnPlayer(it->first);
        // now spawn bots
        /*
        for (std::map<PlayerInfo*, Player>::iterator it = this->players_.begin(); it != this->players_.end(); ++it)
            if (!it->first->isHumanPlayer() && (it->first->isReadyToSpawn() || this->bForceSpawn_))
                this->spawnPlayer(it->first);
        */
    }

    /**
    @brief
        Spawns the input player.
    @param player
        The player to be spawned.
    */
    void Jump::spawnPlayer(PlayerInfo* player)
    {
        assert(player);

        // If the first (left) bat has no player.
        if (this->figure_->getPlayer() == NULL)
        {
            player->startControl(this->figure_);
            this->players_[player].state_ = PlayerState::Alive;
        }
        // If both bats are taken.
        else
        {
            return;
        }

    }

    /**
    @brief
        Is called when the player scored.
    */
    void Jump::playerScored(PlayerInfo* player, int score)
    {
    	/*
        Deathmatch::playerScored(player, score);
        if (this->center_ != NULL) // If there is a centerpoint.
        {
            // Fire an event for the player that has scored, to be able to react to it in the level, e.g. by displaying fireworks.
            if (player == this->getRightPlayer())
                this->center_->fireEvent(FireEventName(JumpCenterpoint, right));
            else if (player == this->getLeftPlayer())
                this->center_->fireEvent(FireEventName(JumpCenterpoint, left));

            // Also announce, that the player has scored.
            if (player != NULL)
                this->gtinfo_->sendAnnounceMessage(player->getName() + " scored");
        }

        // If there is a ball present, reset its position, velocity and acceleration.
        if (this->ball_ != NULL)
        {
            this->ball_->setPosition(Vector3::ZERO);
            this->ball_->setVelocity(Vector3::ZERO);
            this->ball_->setAcceleration(Vector3::ZERO);
            this->ball_->setSpeed(0);
        }

        // If there are bats reset them to the middle position.
        if (this->figure_[0] != NULL && this->figure_[1] != NULL)
        {
            this->figure_[0]->setPosition(-this->center_->getFieldDimension().x / 2, 0, 0);
            this->figure_[1]->setPosition( this->center_->getFieldDimension().x / 2, 0, 0);
        }

        // If a player gets enough points, he won the game -> end of game
        PlayerInfo* winningPlayer = NULL;
        if (this->getLeftPlayer() && this->getScore(this->getLeftPlayer()) >= scoreLimit_)
            winningPlayer = this->getLeftPlayer();
        else if (this->getRightPlayer() && this->getScore(this->getRightPlayer()) >= scoreLimit_)
            winningPlayer = getLeftPlayerthis->getRightPlayer();

        if (winningPlayer)
        {
             ChatManager::message(winningPlayer->getName() + " has won!");
             this->end();
        }

        // Restart the timer to start the ball.
        this->starttimer_.startTimer();

        */
    }

    /**
    @brief
        Starts the ball with some default speed.
    */
    void Jump::startBall()
    {

    }

    /**
    @brief
        Get the left player.
    @return
        Returns a pointer to the player playing on the left. If there is no left player, NULL is returned.
    */
    PlayerInfo* Jump::getPlayer() const
    {
        if (this->figure_ != NULL)
        {
            return this->figure_->getPlayer();
        }
        else
        {
            return 0;
        }
    }

}

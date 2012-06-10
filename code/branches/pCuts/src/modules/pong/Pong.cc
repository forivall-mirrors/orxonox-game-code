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
    @file Pong.cc
    @brief Implementation of the Pong class.
*/

#include "Pong.h"

#include "core/CoreIncludes.h"
#include "core/EventIncludes.h"
#include "core/command/Executor.h"
#include "core/ConfigValueIncludes.h"

#include "gamestates/GSLevel.h"
#include "chat/ChatManager.h"

#include "PongCenterpoint.h"
#include "PongBall.h"
#include "PongBat.h"
#include "PongBot.h"
#include "PongAI.h"
namespace orxonox
{
    // Events to allow to react to scoring of a player, in the level-file.
    CreateEventName(PongCenterpoint, right);
    CreateEventName(PongCenterpoint, left);

    CreateUnloadableFactory(Pong);

    /**
    @brief
        Constructor. Registers and initializes the object.
    */
    Pong::Pong(BaseObject* creator) : Deathmatch(creator)
    {
        RegisterObject(Pong);

        this->center_ = 0;
        this->ball_ = 0;
        this->bat_[0] = 0;
        this->bat_[1] = 0;

        this->setHUDTemplate("PongHUD");

        // Pre-set the timer, but don't start it yet.
        this->starttimer_.setTimer(1.0, false, createExecutor(createFunctor(&Pong::startBall, this)));
        this->starttimer_.stopTimer();

        // Set the type of Bots for this particular Gametype.
        this->botclass_ = Class(PongBot);

        this->scoreLimit_ = 10;
        this->setConfigValues();
    }

    /**
    @brief
        Destructor. Cleans up, if initialized.
    */
    Pong::~Pong()
    {
        if (this->isInitialized())
            this->cleanup();
    }

    void Pong::setConfigValues()
    {
        SetConfigValue(scoreLimit_, 10).description("The player first reaching those points wins.");
    }

    /**
    @brief
        Cleans up the Gametype by destroying the ball and the bats.
    */
    void Pong::cleanup()
    {
        if (this->ball_ != NULL) // Destroy the ball, if present.
        {
            this->ball_->destroy();
            this->ball_ = 0;
        }

        // Destroy both bats, if present.
        for (size_t i = 0; i < 2; ++i)
        {
            if (this->bat_[0] != NULL)
            {
                this->bat_[0]->destroy();
                this->bat_[0] = 0;
            }
        }
    }

    /**
    @brief
        Starts the Pong minigame.
    */
    void Pong::start()
    {
        if (this->center_ != NULL) // There needs to be a PongCenterpoint, i.e. the area the game takes place.
        {
            if (this->ball_ == NULL) // If there is no ball, create a new ball.
            {
                this->ball_ = new PongBall(this->center_);
                // Apply the template for the ball specified by the centerpoint.
                this->ball_->addTemplate(this->center_->getBalltemplate());
            }

            // Attach the ball to the centerpoint and set the parameters as specified in the centerpoint, the ball is attached to.
            this->center_->attach(this->ball_);
            this->ball_->setPosition(0, 0, 0);
            this->ball_->setFieldDimension(this->center_->getFieldDimension());
            this->ball_->setSpeed(0);
            this->ball_->setAccelerationFactor(this->center_->getBallAccelerationFactor());
            this->ball_->setBatLength(this->center_->getBatLength());

            // If one of the bats is missing, create it. Apply the template for the bats as specified in the centerpoint.
            for (size_t i = 0; i < 2; ++i)
            {
                if (this->bat_[i] == NULL)
                {
                    this->bat_[i] = new PongBat(this->center_);
                    this->bat_[i]->addTemplate(this->center_->getBattemplate());
                }
            }

            // Attach the bats to the centerpoint and set the parameters as specified in the centerpoint, the bats are attached to.
            this->center_->attach(this->bat_[0]);
            this->center_->attach(this->bat_[1]);
            this->bat_[0]->setPosition(-this->center_->getFieldDimension().x / 2, 0, 0);
            this->bat_[1]->setPosition( this->center_->getFieldDimension().x / 2, 0, 0);
            this->bat_[0]->yaw(Degree(-90));
            this->bat_[1]->yaw(Degree(90));
            this->bat_[0]->setSpeed(this->center_->getBatSpeed());
            this->bat_[1]->setSpeed(this->center_->getBatSpeed());
            this->bat_[0]->setFieldHeight(this->center_->getFieldDimension().y);
            this->bat_[1]->setFieldHeight(this->center_->getFieldDimension().y);
            this->bat_[0]->setLength(this->center_->getBatLength());
            this->bat_[1]->setLength(this->center_->getBatLength());

            // Set the bats for the ball.
            this->ball_->setBats(this->bat_);
        }
        else // If no centerpoint was specified, an error is thrown and the level is exited.
        {
            orxout(internal_error) << "Pong: No Centerpoint specified." << endl;
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
    }

    /**
    @brief
        Ends the Pong minigame.
    */
    void Pong::end()
    {
        this->cleanup();

        // Call end for the parent class.
        Deathmatch::end();
    }

    /**
    @brief
        Spawns players, and fills the rest up with bots.
    */
    void Pong::spawnPlayersIfRequested()
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
    void Pong::spawnPlayer(PlayerInfo* player)
    {
        assert(player);

        // If the first (left) bat has no player.
        if (this->bat_[0]->getPlayer() == NULL)
        {
            player->startControl(this->bat_[0]);
            this->players_[player].state_ = PlayerState::Alive;
        }
        // If the second (right) bat has no player.
        else if (this->bat_[1]->getPlayer() == NULL)
        {
            player->startControl(this->bat_[1]);
            this->players_[player].state_ = PlayerState::Alive;
        }
        // If both bats are taken.
        else
            return;

        // If the player is an AI, it receives a pointer to the ball.
        if (player->getController() != NULL && player->getController()->isA(Class(PongAI)))
        {
            PongAI* ai = orxonox_cast<PongAI*>(player->getController());
            ai->setPongBall(this->ball_);
        }
    }

    /**
    @brief
        Is called when the player scored.
    */
    void Pong::playerScored(PlayerInfo* player)
    {
        Deathmatch::playerScored(player);

        if (this->center_ != NULL) // If there is a centerpoint.
        {
            // Fire an event for the player that has scored, to be able to react to it in the level, e.g. by displaying fireworks.
            if (player == this->getRightPlayer())
                this->center_->fireEvent(FireEventName(PongCenterpoint, right));
            else if (player == this->getLeftPlayer())
                this->center_->fireEvent(FireEventName(PongCenterpoint, left));

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
        if (this->bat_[0] != NULL && this->bat_[1] != NULL)
        {
            this->bat_[0]->setPosition(-this->center_->getFieldDimension().x / 2, 0, 0);
            this->bat_[1]->setPosition( this->center_->getFieldDimension().x / 2, 0, 0);
        }

        // If a player gets enough points, he won the game -> end of game
        PlayerInfo* winningPlayer = NULL;
        if (this->getLeftPlayer() && this->getScore(this->getLeftPlayer()) >= scoreLimit_)
            winningPlayer = this->getLeftPlayer();
        else if (this->getRightPlayer() && this->getScore(this->getRightPlayer()) >= scoreLimit_)
            winningPlayer = this->getRightPlayer();

        if (winningPlayer)
        {
             ChatManager::message(winningPlayer->getName() + " has won!");
             this->end();
        }

        // Restart the timer to start the ball.
        this->starttimer_.startTimer();
    }

    /**
    @brief
        Starts the ball with some default speed.
    */
    void Pong::startBall()
    {
        if (this->ball_ != NULL && this->center_ != NULL)
            this->ball_->setSpeed(this->center_->getBallSpeed());
    }

    /**
    @brief
        Get the left player.
    @return
        Returns a pointer to the player playing on the left. If there is no left player, NULL is returned.
    */
    PlayerInfo* Pong::getLeftPlayer() const
    {
        if (this->bat_ != NULL && this->bat_[0] != NULL)
            return this->bat_[0]->getPlayer();
        else
            return 0;
    }

    /**
    @brief
        Get the right player.
    @return
        Returns a pointer to the player playing on the right. If there is no right player, NULL is returned.
    */
    PlayerInfo* Pong::getRightPlayer() const
    {
        if (this->bat_ != NULL && this->bat_[1] != NULL)
            return this->bat_[1]->getPlayer();
        else
            return 0;
    }
}

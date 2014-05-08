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
#include "JumpPlatformStatic.h"
#include "JumpPlatformHMove.h"
#include "JumpPlatformVMove.h"
#include "JumpPlatformDisappear.h"
#include "JumpPlatformTimer.h"
#include "JumpPlatformFake.h"
#include "JumpProjectile.h"
#include "JumpEnemy.h"
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
        this->figure_ = 0;
        this->camera = 0;
        this->fakeAdded_ = false;

        this->setHUDTemplate("JumpHUD");

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
    		Vector3 figureVelocity = figure_->getVelocity();

    		if (figurePosition.z > totalScreenShift)
    		{
    			screenShiftSinceLastUpdate += figurePosition.z - totalScreenShift;
    			totalScreenShift = figurePosition.z;

                // Falls noetig neue Platformen im neuen Bereich einfuegen
                if (screenShiftSinceLastUpdate > center_->getSectionLength())
                {
                	screenShiftSinceLastUpdate -= center_->getSectionLength();
                    addSection();
                }
    		}

    		// Spiel verloren wegen Ansturz?
    		if (figurePosition.z < totalScreenShift - center_->getFieldDimension().y && figureVelocity.z < 0)
    		{
    			end();
    		}

    		// Schiessen
    		if (figure_->fireSignal)
    		{
    			figure_->fireSignal = false;
    			addProjectile(figurePosition.x, figurePosition.z, 0.0, 150.0);
    		}


    		if (this->camera != NULL)
			{
				Vector3 cameraPosition = Vector3(0, totalScreenShift, 0);
				camera->setPosition(cameraPosition);
			}
			else
			{
				orxout() << "No camera found." << endl;
				//camera = figure_->getCamera();
			}
    	}
    	else
    	{
    		//orxout() << "No figure found." << endl;
    	}

		// Platformen, die zu weit unten sind entfernen
		ObjectList<JumpPlatform>::iterator beginPlatform = ObjectList<JumpPlatform>::begin();
		ObjectList<JumpPlatform>::iterator endPlatform = ObjectList<JumpPlatform>::end();
		ObjectList<JumpPlatform>::iterator itPlatform = beginPlatform;
		Vector3 platformPosition;

		while (itPlatform != endPlatform)
		{
			platformPosition = itPlatform->getPosition();
			if (platformPosition.z < totalScreenShift - center_->getFieldDimension().y)
			{
				ObjectList<JumpPlatform>::iterator temp = itPlatform;
				++ itPlatform;
				center_->detach(*temp);
				temp->destroy();
			}
			else
			{
				++ itPlatform;
			}
		}

		// DAS GEHT NICHT!!! it++ funktioniert nicht, falls eine Platform geloescht wurde -> Segmentation Error
		/*
		for (ObjectList<JumpPlatformDisappear>::iterator it = ObjectList<JumpPlatformDisappear>::begin(); orxout() << "E" << endl, it != ObjectList<JumpPlatformDisappear>::end(); orxout() << "F" << endl, ++it)
		{
			if (!it->isActive())
			{
				// Entferne Platform
				center_->detach(*it);
				it->destroy();
			}
		}
		*/

		// Deaktivierte Platformen entfernen
		ObjectList<JumpPlatformDisappear>::iterator beginDisappear = ObjectList<JumpPlatformDisappear>::begin();
		ObjectList<JumpPlatformDisappear>::iterator endDisappear = ObjectList<JumpPlatformDisappear>::end();
		ObjectList<JumpPlatformDisappear>::iterator itDisappear = beginDisappear;

		while (itDisappear != endDisappear)
		{
			if (!itDisappear->isActive())
			{
				ObjectList<JumpPlatformDisappear>::iterator temp = itDisappear;
				++ itDisappear;
				center_->detach(*temp);
				temp->destroy();
			}
			else
			{
				++ itDisappear;
			}
		}

		// Abgelaufene Timer-Platformen entfernen
		ObjectList<JumpPlatformTimer>::iterator beginTimer = ObjectList<JumpPlatformTimer>::begin();
		ObjectList<JumpPlatformTimer>::iterator endTimer = ObjectList<JumpPlatformTimer>::end();
		ObjectList<JumpPlatformTimer>::iterator itTimer = beginTimer;

		while (itTimer != endTimer)
		{
			if (!itTimer->isActive())
			{
				ObjectList<JumpPlatformTimer>::iterator temp = itTimer;
				++ itTimer;
				center_->detach(*temp);
				temp->destroy();
			}
			else
			{
				++ itTimer;
			}
		}

		// Projektile, die zu weit oben sind entfernen
		ObjectList<JumpProjectile>::iterator beginProjectile = ObjectList<JumpProjectile>::begin();
		ObjectList<JumpProjectile>::iterator endProjectile = ObjectList<JumpProjectile>::end();
		ObjectList<JumpProjectile>::iterator itProjectile = beginProjectile;
		Vector3 projectilePosition;

		while (itProjectile != endProjectile)
		{
			projectilePosition = itProjectile->getPosition();
			if (projectilePosition.z > totalScreenShift + 5*center_->getFieldDimension().y)
			{
				ObjectList<JumpProjectile>::iterator temp = itProjectile;
				++ itProjectile;
				center_->detach(*temp);
				temp->destroy();
			}
			else
			{
				++ itProjectile;
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
		if (this->figure_ != NULL)
		{
			//this->figure_->destroy();
			//this->figure_ = 0;
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


            // Attach the ball to the centerpoint and set the parameters as specified in the centerpoint, the ball is attached to.
            /*this->center_->attach(this->ball_);
            this->ball_->setPosition(0, 0, 0);
            this->ball_->setFieldDimension(this->center_->getFieldDimension());

            // Set the bats for the ball.
            this->ball_->setFigure(this->figure_);
            */

            // If one of the bats is missing, create it. Apply the template for the bats as specified in the centerpoint.
			if (this->figure_ == NULL)
			{
				this->figure_ = new JumpFigure(this->center_->getContext());
				this->figure_->addTemplate(this->center_->getFigureTemplate());
			}

            // Attach the bats to the centerpoint and set the parameters as specified in the centerpoint, the bats are attached to.
            this->center_->attach(this->figure_);
            this->figure_->setPosition(-this->center_->getFieldDimension().x / 2, 0, 0);
            this->figure_->setFieldDimension(this->center_->getFieldDimension());
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
        screenShiftSinceLastUpdate = 0.0;
        sectionNumber = 0;

        addStartSection();
        addSection();
        addSection();
    }

    /**
    @brief
        Ends the Jump minigame.
    */
    void Jump::end()
    {
    	cleanup();
    	GSLevel::startMainMenu();

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

    void Jump::addPlatform(JumpPlatform* newPlatform, std::string platformTemplate, float xPosition, float zPosition)
    {
    	if (newPlatform != NULL && center_ != NULL)
		{
    		newPlatform->addTemplate(platformTemplate);
    		newPlatform->setPosition(Vector3(xPosition, 0.0, zPosition));
    		newPlatform->setFieldDimension(center_->getFieldDimension());
    		newPlatform->setFigure(this->figure_);
    		center_->attach(newPlatform);
		}
    }

    void Jump::addPlatformStatic(float xPosition, float zPosition)
    {
		if (fakeAdded_ == false && rand()%5 == 0)
		{
			addPlatformFake(xPosition, zPosition);
		}
		else
		{
	    	JumpPlatformStatic* newPlatform = new JumpPlatformStatic(center_->getContext());
			addPlatform(newPlatform, center_->getPlatformStaticTemplate(), xPosition, zPosition);
		}
    }

    void Jump::addPlatformHMove(float xPosition, float zPosition, float leftBoundary, float rightBoundary, float speed)
    {
    	JumpPlatformHMove* newPlatform = new JumpPlatformHMove(center_->getContext());
		newPlatform->setProperties(leftBoundary, rightBoundary, speed);
		addPlatform(newPlatform, center_->getPlatformHMoveTemplate(), xPosition, zPosition);
    }

    void Jump::addPlatformVMove(float xPosition, float zPosition, float lowerBoundary, float upperBoundary, float speed)
    {
    	JumpPlatformVMove* newPlatform = new JumpPlatformVMove(center_->getContext());
		newPlatform->setProperties(lowerBoundary, upperBoundary, speed);
		addPlatform(newPlatform, center_->getPlatformVMoveTemplate(), xPosition, zPosition);
    }

    void Jump::addPlatformDisappear(float xPosition, float zPosition)
    {
		JumpPlatformDisappear* newPlatform = new JumpPlatformDisappear(center_->getContext());
		newPlatform->setProperties(true);
		addPlatform(newPlatform, center_->getPlatformDisappearTemplate(), xPosition, zPosition);
    }

    void Jump::addPlatformTimer(float xPosition, float zPosition, float time, float variance)
    {
		float additionalTime = (float)(rand()%100)/(100*variance) - variance/2;

    	JumpPlatformTimer* newPlatform = new JumpPlatformTimer(center_->getContext());
		newPlatform->setProperties(time + additionalTime);
		addPlatform(newPlatform, center_->getPlatformTimerTemplate(), xPosition, zPosition);
    }

    void Jump::addPlatformFake(float xPosition, float zPosition)
    {
    	fakeAdded_ = true;

		JumpPlatformFake* newPlatform = new JumpPlatformFake(center_->getContext());
		addPlatform(newPlatform, center_->getPlatformFakeTemplate(), xPosition, zPosition);
		newPlatform->setAngularVelocity(Vector3(0, 0, 2.0));
    }


    void Jump::addProjectile(float xPosition, float zPosition, float xVelocity, float zVelocity)
    {
    	JumpProjectile* newProjectile = new JumpProjectile(center_->getContext());
    	if (newProjectile != NULL && center_ != NULL)
		{
    		newProjectile->addTemplate(center_->getProjectileTemplate());
    		newProjectile->setPosition(Vector3(xPosition, 0.0, zPosition));
    		newProjectile->setVelocity(Vector3(xVelocity, 0.0, zVelocity));
    		newProjectile->setFieldDimension(center_->getFieldDimension());
    		newProjectile->setFigure(this->figure_);
    		center_->attach(newProjectile);
		}
    }

    void Jump::addEnemy1(float xPosition, float zPosition, float leftBoundary, float rightBoundary, float lowerBoundary, float upperBoundary, float xVelocity, float zVelocity)
    {
    	JumpEnemy* newEnemy = new JumpEnemy(center_->getContext());
    	if (newEnemy != NULL && center_ != NULL)
		{
    		newEnemy->addTemplate(center_->getEnemy1Template());
    		newEnemy->setPosition(Vector3(xPosition, 0.0, zPosition));
    		newEnemy->setProperties(leftBoundary, rightBoundary, lowerBoundary, upperBoundary, xVelocity, zVelocity);
    		newEnemy->setFieldDimension(center_->getFieldDimension());
    		newEnemy->setFigure(this->figure_);
    		center_->attach(newEnemy);
		}
    }

    void Jump::addStartSection()
    {
		JumpPlatform* newPlatform;

		float sectionLength = center_->getSectionLength();
		float platformLength = center_->getPlatformLength();

		for (float xPosition = -center_->getFieldDimension().x; xPosition <= center_->getFieldDimension().x; xPosition += platformLength)
		{
			newPlatform = new JumpPlatformStatic(center_->getContext());
			addPlatform(newPlatform, center_->getPlatformStaticTemplate(), xPosition, -0.05*sectionLength);
		}
    }

    void Jump::addSection()
    {
        float fieldWidth = center_->getFieldDimension().x;
        float sectionLength = center_->getSectionLength();
        float platformLength = center_->getPlatformLength();

        float sectionBegin = sectionNumber * sectionLength;
        float sectionEnd = (1.0 + sectionNumber) * sectionLength;

		int randPos1 = rand()%10;
		int randPos2 = rand()%10;
		int randPos3 = rand()%10;
		int randPos4 = rand()%10;

		if (rand()%5 == 0)
		{
			addEnemy1(randomXPosition(), sectionBegin + sectionLength/10, -fieldWidth, fieldWidth, sectionBegin + sectionLength/10, sectionBegin + sectionLength/10, 5.0, 0.0);
		}

        switch (rand()%12)
		{
		case 0:
			// Doppelt statisch
			for (int i = 0; i < 10; ++i)
			{
				for (int j = 0; j < 2; ++j)
				{
					addPlatformStatic(randomXPosition(2, j), sectionBegin + i*sectionLength/10);
				}
			}
			break;
		case 1:
			// Dreifach statisch
			for (int i = 0; i < 10; ++i)
			{
				for (int j = 0; j < 3; ++j)
				{
					addPlatformStatic(randomXPosition(3, j), sectionBegin + i*sectionLength/10);
				}
			}
			break;
		case 2:
			// statisch mit 1 horizontal
			for (int i = 0; i < 10; ++i)
			{
				if (i == randPos1)
				{
					addPlatformHMove(randomXPosition(), sectionBegin + i*sectionLength/10, -fieldWidth, fieldWidth, 30.0);
				}
				else
				{
					addPlatformStatic(randomXPosition(), sectionBegin + i*sectionLength/10);
				}
			}
			break;
		case 3:
			// statisch mit 2 horizontal
			for (int i = 0; i < 10; ++i)
			{
				if (i == randPos1 || i == randPos2)
				{
					addPlatformHMove(randomXPosition(), sectionBegin + i*sectionLength/10, -fieldWidth, fieldWidth, 30.0);
				}
				else
				{
					addPlatformStatic(randomXPosition(), sectionBegin + i*sectionLength/10);
				}
			}
			break;
		case 4:
			// statisch mit 3 horizontal
			for (int i = 0; i < 10; ++i)
			{
				if (i == randPos1 || i == randPos2 || i == randPos3)
				{
					addPlatformHMove(randomXPosition(), sectionBegin + i*sectionLength/10, -fieldWidth, fieldWidth, 30.0);
				}
				else
				{
					addPlatformStatic(randomXPosition(), sectionBegin + i*sectionLength/10);
				}
			}
			break;
		case 5:
			// statisch mit 4 horizontal
			for (int i = 0; i < 10; ++i)
			{
				if (i == randPos1 || i == randPos2 || i == randPos3 || i == randPos4)
				{
					addPlatformHMove(randomXPosition(), sectionBegin + i*sectionLength/10, -fieldWidth, fieldWidth, 30.0);
				}
				else
				{
					addPlatformStatic(randomXPosition(), sectionBegin + i*sectionLength/10);
				}
			}
			break;
			// Einfach horizontal
		case 6:
			for (int i = 0; i < 10; ++i)
			{
				addPlatformHMove(randomXPosition(), sectionBegin + i*sectionLength/10, -fieldWidth, fieldWidth, 30.0);
			}
			break;
			// Doppelt horizontal
		case 7:
			for (int i = 0; i < 10; ++i)
			{
					float mediumPos = randomXPosition(3, 1);
					addPlatformHMove(randomXPosition(3, 0), sectionBegin + i*sectionLength/10, -fieldWidth, mediumPos - platformLength/2, 30.0);
					addPlatformHMove(randomXPosition(3, 2), sectionBegin + i*sectionLength/10, mediumPos+platformLength/2, fieldWidth, 30.0);
			}
			break;
			// Einfach vertikal
		case 8:
			for (int i = 0; i < 7; ++i)
			{
				addPlatformVMove(-fieldWidth + i*fieldWidth*2/10, randomYPosition(sectionBegin, sectionEnd), sectionBegin, sectionEnd, 20.0);
			}
			break;
			// Doppelt vertikal
		case 9:
			for (int i = 0; i < 14; ++i)
			{
				addPlatformVMove(-fieldWidth + i*fieldWidth*2/10, randomYPosition(sectionBegin, sectionEnd), sectionBegin, sectionEnd, 20.0);
			}
			break;
			// Doppelt verschwindend
		case 10:
			for (int i = 0; i < 10; ++i)
			{
				for (int j = 0; j < 2; ++j)
				{
					addPlatformDisappear(randomXPosition(2, j), randomYPosition(sectionBegin, sectionEnd));
				}
			}
			break;
			// Doppelt Timer
		case 11:
			for (int i = 0; i < 10; ++i)
			{
				for (int j = 0; j < 2; ++j)
				{
					addPlatformTimer(randomXPosition(2, j), randomYPosition(sectionBegin, sectionEnd), 6.0, 1.5);
				}
			}
			break;
		}
		orxout() << "new section added with number "<< sectionNumber << endl;

		fakeAdded_ = false;

		++ sectionNumber;
    }

    float Jump::randomXPosition()
    {
        float fieldWidth = center_->getFieldDimension().x;

    	return (float)(rand()%(2*(int)fieldWidth)) - fieldWidth;
    }

    float Jump::randomXPosition(int totalColumns, int culomn)
    {
    	float fieldWidth = center_->getFieldDimension().x;

    	float width = 2*fieldWidth/totalColumns;
    	float leftBound = culomn*width-fieldWidth;
    	float platformLength = center_->getPlatformLength();

    	return (float)(rand()%(int)(width-platformLength)) + leftBound + platformLength/2;
    }

    float Jump::randomYPosition(float lowerBoundary, float upperBoundary)
    {
    	return (float)(rand()%(int)(100*(upperBoundary - lowerBoundary)))/100 + lowerBoundary;
    }

    int Jump::getScore(PlayerInfo* player) const
    {
        return sectionNumber - 2;
    }

}

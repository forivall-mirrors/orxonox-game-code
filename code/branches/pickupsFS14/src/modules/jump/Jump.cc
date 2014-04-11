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
 *      Florian Zinggeler
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

// ! HACK
#include "infos/PlayerInfo.h"

#include "JumpCenterPoint.h"
#include "JumpShip.h"
/*
#include "JumpEnemy.h"
#include "JumpEnemyShooter.h"*/

#include "core/command/ConsoleCommand.h"
#include "worldentities/BigExplosion.h"

namespace orxonox
{
    RegisterUnloadableClass(Jump);

    Jump::Jump(Context* context) : Deathmatch(context)
    {
        RegisterObject(Jump);
        platformList.clear();
        yScreenPosition = 0;
        screenShiftSinceLastUpdate = 0;

        //this->numberOfBots_ = 0; //sets number of default bots temporarly to 0
        //this->center_ = 0;
        //this->setHUDTemplate("JumpHUD");


    }


    /*void Jump::levelUp()
    {
        level++;
        if (getPlayer() != NULL)
        {
            for (int i = 0; i < 7; i++)
            {
                WeakPtr<BigExplosion> chunk = new BigExplosion(this->center_->getContext());
                chunk->setPosition(Vector3(600, 0, 100.f * i - 300));
                chunk->setVelocity(Vector3(1000, 0, 0));  //player->getVelocity()
                chunk->setScale(20);
            }
        }
        addPoints(multiplier * 42);
        multiplier *= 2;
        toggleShowLevel();
        showLevelTimer.setTimer(1.0f, false, createExecutor(createFunctor(&Jump::toggleShowLevel, this)));
    }*/

    WeakPtr<JumpShip> Jump::getPlayer()
    {
        if (player == NULL)
        {
        	for (ObjectList<JumpShip>::iterator it = ObjectList<JumpShip>::begin(); it != ObjectList<JumpShip>::end(); ++it)
        	{
                player = *it;
        	}
        }
        return player;
    }

    void Jump::tick(float dt)
    {


        if (getPlayer() != NULL)
        {
            Vector3 shipPosition = getPlayer()->getPosition();

        	// Bildschirmposition kann nur nach oben verschoben werden
        	if (shipPosition.y > yScreenPosition)
        	{
        		screenShiftSinceLastUpdate += shipPosition.y - yScreenPosition;

        		yScreenPosition = shipPosition.y;
        	}

        	// Kameraposition nachfuehren
        	if (camera == NULL)
        	{
        		camera = getPlayer()->getCamera();
        	}
            if (camera != NULL)
            {
                camera->setPosition(Vector3(-shipPosition.x, yScreenPosition-shipPosition.y, 100));
                //camera->setOrientation(Vector3::UNIT_Z, Degree(180));
            }

            if (screenShiftSinceLastUpdate > 200.0)
            {
            	screenShiftSinceLastUpdate -= 200.0;
            	orxout() << "new section added" << endl;
            	addPlatform(shipPosition.x, shipPosition.y + 300.0);
            }

        }

        SUPER(Jump, tick, dt);
    }


    /*void Jump::spawnEnemy()
    {
        if (getPlayer() == NULL)
            return;

        for (int i = 0; i < (3*log10(static_cast<double>(level)) + 1); i++)
        {
            WeakPtr<JumpEnemy> newPawn;
            if (rand() % 42/(1 + level*level) == 0)
            {
                newPawn = new JumpEnemyShooter(this->center_->getContext());
                newPawn->addTemplate("enemyjumpshooter");
            }
            else
            {
                newPawn = new JumpEnemy(this->center_->getContext());
                newPawn->addTemplate("enemyjump");
            }
            newPawn->setPlayer(player);
            newPawn->level = level;
            // spawn enemy at random points in front of player.
            newPawn->setPosition(player->getPosition() + Vector3(500.f + 100 * i, 0, float(rand())/RAND_MAX * 400 - 200));
        }
    }*/

    /*void Jump::costLife()
    {
        lives--;
        multiplier = 1;
        // end the game in 30 seconds.
        if (lives <= 0)
            enemySpawnTimer.setTimer(30.0f, false, createExecutor(createFunctor(&Jump::end, this)));
    };*/

    /*void Jump::comboControll()
    {
        if (b_combo)
            multiplier++;
        // if no combo was performed before, reset multiplier
        else
            multiplier = 1;
        b_combo = false;
    }*/


    void Jump::start()
    {
    	// Call start for the parent class.
    	Deathmatch::start();

        /*
        // Set variable to temporarily force the player to spawn.
        this->bForceSpawn_ = true;

        if (this->center_ == NULL)  // abandon mission!
        {
            orxout(internal_error) << "Jump: No Centerpoint specified." << endl;
            GSLevel::startMainMenu();
            return;
        }
        */

    	//addPlatform(0,0);

    }


    /*void Jump::addPoints(int numPoints)
    {
        if (!bEndGame)
        {
            point += numPoints * multiplier;
            b_combo = true;
        }
    }*/

    /*void Jump::end()
    {
        // DON'T CALL THIS!
        //      Deathmatch::end();
        // It will misteriously crash the game!
        // Instead startMainMenu, this won't crash.
        GSLevel::startMainMenu();
    }*/

    void Jump::addPlatform(float xPosition, float yPosition)
    {
		JumpPlatform* newPlatform = new JumpPlatform(center_->getContext());
		newPlatform->setPosition(Vector3(xPosition, yPosition, 0));
		platformList.push_front(newPlatform);
    }

}

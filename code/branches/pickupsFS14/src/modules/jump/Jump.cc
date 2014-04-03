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
        //this->numberOfBots_ = 0; //sets number of default bots temporarly to 0
        //this->center_ = 0;
       // init();
        //this->setHUDTemplate("JumpHUD");

    }

    void Jump::init()
    {
        /*bEndGame = false;
        lives = 3;
        level = 1;
        point = 0;
        bShowLevel = false;
        multiplier = 1;
        b_combo = false;*/
        // spawn enemy every 3.5 seconds
        //enemySpawnTimer.setTimer(3.5f, true, createExecutor(createFunctor(&Jump::spawnEnemy, this)));
        //comboTimer.setTimer(3.0f, true, createExecutor(createFunctor(&Jump::comboControll, this)));
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
        	ObjectList<JumpShip>::iterator it;
        	it = ObjectList<JumpShip>::begin();
        	/*for (ObjectList<JumpShip>::iterator it = ObjectList<JumpShip>::begin(); it != ObjectList<JumpShip>::end(); ++it)
        	{
                player = *it;
        	}*/
        }
        return player;
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

    /*void Jump::start()
    {
        init();
        // Set variable to temporarily force the player to spawn.
        this->bForceSpawn_ = true;

        if (this->center_ == NULL)  // abandon mission!
        {
            orxout(internal_error) << "Jump: No Centerpoint specified." << endl;
            GSLevel::startMainMenu();
            return;
        }
        // Call start for the parent class.
        Deathmatch::start();
    }*/

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
}

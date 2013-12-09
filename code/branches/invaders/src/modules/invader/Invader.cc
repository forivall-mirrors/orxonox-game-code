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
    @file Invader.cc
    @brief Implementation of the Invader class.
*/

#include "Invader.h"

#include "core/CoreIncludes.h"
#include "core/EventIncludes.h"
#include "core/command/Executor.h"
#include "core/config/ConfigValueIncludes.h"

#include "gamestates/GSLevel.h"
#include "chat/ChatManager.h"

// ! HACK
#include "infos/PlayerInfo.h"

#include "InvaderCenterPoint.h"
#include "InvaderShip.h"
#include "InvaderEnemy.h"
#include "InvaderEnemyShooter.h"

#include "core/command/ConsoleCommand.h"
#include "worldentities/BigExplosion.h"

namespace orxonox
{
    RegisterUnloadableClass(Invader);

    Invader::Invader(Context* context) : Deathmatch(context)
    {
        RegisterObject(Invader);
        this->center_ = 0;
        init();
        this->setHUDTemplate("InvaderHUD");
    }

    void Invader::init()
    {
        bEndGame = false;
        lives = 3;
        level = 1;
        point = 0;
        bShowLevel = false;
        multiplier = 1;
        b_combo = false;
        // spawn enemy every 3.5 seconds
        enemySpawnTimer.setTimer(3.5f, true, createExecutor(createFunctor(&Invader::spawnEnemy, this)));
        comboTimer.setTimer(3.0f, true, createExecutor(createFunctor(&Invader::comboControll, this)));
    }

    void Invader::levelUp()
    {
        level++;
        if (getPlayer() != NULL)
        {   
            for (int i = 0; i < 7; i++)
            {
                WeakPtr<BigExplosion> chunk = new BigExplosion(this->center_->getContext());
                chunk->setPosition(Vector3(600, 0, 100 * i - 300));
                chunk->setVelocity(Vector3(1000, 0, 0));  //player->getVelocity()
                chunk->setScale(20);
            }
        }
        addPoints(multiplier * 42);
        multiplier *= 2;
        toggleShowLevel();
        showLevelTimer.setTimer(1.0f, false, createExecutor(createFunctor(&Invader::toggleShowLevel, this)));
    }

    WeakPtr<InvaderShip> Invader::getPlayer()
    {
        if (player == NULL)
        {
            for (ObjectList<InvaderShip>::iterator it = ObjectList<InvaderShip>::begin(); it != ObjectList<InvaderShip>::end(); ++it)
                player = *it;
        }
        return player;
    }

    void Invader::spawnEnemy()
    {
        if (getPlayer() == NULL)
            return;
        srand(player->getPosition().x + player->getPosition().y);
        for (int i = 0; i < (3*log10(level) + 1); i++)
        {
            WeakPtr<InvaderEnemy> newPawn;
            if (rand() % 42/(1 + level*level) == 0)
            {
                newPawn = new InvaderEnemyShooter(this->center_->getContext());
                newPawn->addTemplate("enemyinvadershooter");
            }
            else
            {
                newPawn = new InvaderEnemy(this->center_->getContext());
                newPawn->addTemplate("enemyinvader");
            }
            newPawn->setPlayer(player);
            newPawn->level = level;
            // spawn enemy at random points in front of player.
            newPawn->setPosition(player->getPosition() + Vector3(500 + 100 * i, 0, float(rand())/RAND_MAX * 400 - 200));
        }
    }

    void Invader::costLife()
    {
        lives--;
        multiplier = 1;
        // end the game in 30 seconds.
        if (lives <= 0)
            enemySpawnTimer.setTimer(30.0f, false, createExecutor(createFunctor(&Invader::end, this)));
    };

    void Invader::comboControll()
    {
        if (b_combo)
            multiplier++;
        // if no combo was performed before, reset multiplier
        else
            multiplier = 1;
        b_combo = false;
    }

    void Invader::start()
    {
        init();
        // Set variable to temporarily force the player to spawn.
        this->bForceSpawn_ = true;

        if (this->center_ == NULL)  // abandon mission!
        {
            orxout(internal_error) << "Invader: No Centerpoint specified." << endl;
            GSLevel::startMainMenu();
            return;
        }
        // Call start for the parent class.
        Deathmatch::start();
    }
    void Invader::addPoints(int numPoints)
    {
        if (!bEndGame)
        {
            point += numPoints * multiplier;
            b_combo = true;
        }
    }

    void Invader::end()
    {
        // DON'T CALL THIS!
        //      Deathmatch::end();
        // It will misteriously crash the game!
        // Instead startMainMenu, this won't crash.
        GSLevel::startMainMenu();
    }
}

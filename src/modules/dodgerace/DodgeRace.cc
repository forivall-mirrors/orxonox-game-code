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
    @file DodgeRace.cc
    @brief Implementation of the DodgeRace class.
*/

#include "DodgeRace.h"
#include "DodgeRaceShip.h" // Necessary for getPlayer function. Do NOT include this in Header!
#include "DodgeRaceCube.h"
#include "core/CoreIncludes.h"

namespace orxonox
{
    RegisterUnloadableClass(DodgeRace);

    DodgeRace::DodgeRace(Context* context) : Deathmatch(context)
    {
        RegisterObject(DodgeRace);
        init();
        this->numberOfBots_ = 0; //sets number of default bots temporarly to 0
        this->center_ = 0;

        this->setHUDTemplate("DodgeRaceHUD");
    }

    void DodgeRace::init()
    {
        bEndGame = false;
        lives = 1;
        level = 1;
        point = 0;
        bShowLevel = false;
        multiplier = 1;
        b_combo = false;
        counter = 5000;
        pattern = 1;
        lastPosition = 0;
        // spawn enemy every 3.5 seconds
        //enemySpawnTimer.setTimer(3.5f, true, createExecutor(createFunctor(&DodgeRace::spawnEnemy, this)));
        comboTimer.setTimer(3.0f, true, createExecutor(createFunctor(&DodgeRace::comboControll, this)));
    }

    void DodgeRace::levelUp()
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
        showLevelTimer.setTimer(1.0f, false, createExecutor(createFunctor(&DodgeRace::toggleShowLevel, this)));
    }

    void DodgeRace::tick(float dt)
    {
        if (getPlayer() != NULL)
        {
            //WeakPtr<DodgeRaceShip> ship = getPlayer();

            currentPosition = getPlayer()->getWorldPosition().x;
            counter = counter + (currentPosition - lastPosition);
            lastPosition = currentPosition;
            point = (int) currentPosition;
            getPlayer()->speed = 830.0f - (point / 1000);

            for(unsigned int i=0; i < cubeList.size();i++)
            {
                if(cubeList.at(i)->getPosition().x < currentPosition-3000)
                {
                    cubeList.at(i)->destroy();
                    cubeList.erase(cubeList.begin()+i);
                }
            }

            if(counter >= 3000)
            {
                counter = 0;
                for(int i = 0; i<6; i++)
                {
                    WeakPtr<DodgeRaceCube> cube = new DodgeRaceCube(this->center_->getContext());
                    cubeList.push_back(cube);
                    switch(pattern)
                    {
                    case 1: cube->addTemplate("DodgeRaceCube01");
                    break;
                    case 2: cube->addTemplate("DodgeRaceCube02");
                    break;

                    }

                    cube->setPosition(getPlayer()->getWorldPosition() + Vector3(5000.0f, 0.0f, -3600.0f + (i*1200)));
                    //stEntity->setScale3D(50,50,50);
                }


                pattern %= 2;
                pattern ++;

            }

        }
        SUPER(DodgeRace, tick, dt);
    }

    WeakPtr<DodgeRaceShip> DodgeRace::getPlayer()
    {
        if (player == NULL)
        {
            for (ObjectList<DodgeRaceShip>::iterator it = ObjectList<DodgeRaceShip>::begin(); it != ObjectList<DodgeRaceShip>::end(); ++it)
            {
                player = *it;
            }
        }
        return player;
    }

    void DodgeRace::costLife()
    {
        //endGameTimer.setTimer(8.0f, false, createExecutor(createFunctor(&DodgeRace::end, this)));
        lives = 0;
    };

    void DodgeRace::comboControll()
    {
        if (b_combo)
            multiplier++;
        // if no combo was performed before, reset multiplier
        else
            multiplier = 1;
        b_combo = false;
    }

    void DodgeRace::start()
    {
        orxout() << "start" << endl;
        init();
        for(unsigned int i=0; i< cubeList.size();i++)
        {
            cubeList.at(i)->destroy();
            cubeList.erase(cubeList.begin()+i);

        }
        cubeList.clear();
        // Set variable to temporarily force the player to spawn.
        this->bForceSpawn_ = false;

        if (this->center_ == NULL)  // abandon mission!
        {
            orxout(internal_error) << "DodgeRace: No Centerpoint specified." << endl;
            GSLevel::startMainMenu();
            return;
        }
        // Call start for the parent class.
        Deathmatch::start();
    }

    void DodgeRace::playerPreSpawn(PlayerInfo* player)
    {
        if(lives <= 0)
        {
            this->end();
        }

        // Reset all the cubes
        /*
        orxout() << "prespawn" << endl;
        init();
        for(int i=0; i< cubeList.size();i++)
        {
            cubeList.at(i)->destroy();
            cubeList.erase(cubeList.begin()+i);
        }
        cubeList.clear();
        lives = 1;
        point = 0;
        lastPosition = 0;
        */
    }

    void DodgeRace::addPoints(int numPoints)
    {
        if (!bEndGame)
        {
            point += numPoints * multiplier;
            b_combo = true;
        }
    }

    void DodgeRace::end()
    {
        // DON'T CALL THIS!
        //      Deathmatch::end();
        // It will misteriously crash the game!
        // Instead startMainMenu, this won't crash.
        GSLevel::startMainMenu();
    }
}

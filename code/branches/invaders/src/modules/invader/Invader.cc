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

#include "core/command/ConsoleCommand.h"

namespace orxonox
{
    RegisterUnloadableClass(Invader);

    Invader::Invader(Context* context) : Deathmatch(context)
    {
        RegisterObject(Invader);
        this->center_ = 0;

        this->console_addEnemy = createConsoleCommand( "spawnEnemy", createExecutor( createFunctor(&Invader::spawnEnemy, this) ) );
        //this->context = context;
        this->setHUDTemplate("InvaderHUD");

        lives = 3;
        level = 1;
        point = 0;
        multiplier = 1;
        b_combo = false;
        enemySpawnTimer.setTimer(2.0f, true, createExecutor(createFunctor(&Invader::spawnEnemy, this)));
        comboTimer.setTimer(2.5f, true, createExecutor(createFunctor(&Invader::comboControll, this)));
    }

    Invader::~Invader()
    {
    }

    void Invader::spawnEnemy()
    {
        if (player == NULL)
        {
            for (ObjectList<InvaderShip>::iterator it = ObjectList<InvaderShip>::begin(); it != ObjectList<InvaderShip>::end(); ++it)
                player = *it;
        }
        if (player == NULL)
            return;

        for (int i = 0; i < level; i++)
        {
            SmartPtr<InvaderEnemy> newPawn = new InvaderEnemy(this->center_->getContext());
            newPawn->addTemplate("enemyinvader");
            newPawn->setPosition(player->getPosition() + Vector3(500 + 100 * i, 0, 0));
        }
    }

    void Invader::comboControll()
    {
        if (b_combo)
            multiplier++;
        else
            multiplier = 1;
        b_combo = false;
    }

    // inject custom player controller
 /**   void Invader::spawnPlayer(PlayerInfo* player)
    {
        assert(player);

        //player->startControl(new InvaderShip(this->center_->getContext() ) );
    }*/

    void Invader::start()
    {
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

    void Invader::end()
    {
        // Call end for the parent class.
        Deathmatch::end();
    }
}

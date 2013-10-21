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

#include "InvaderShip.h"
// ! HACK
#include "infos/PlayerInfo.h"

#include "InvaderCenterPoint.h"

namespace orxonox
{
    RegisterUnloadableClass(Invader);

    Invader::Invader(Context* context) : Deathmatch(context)
    {
        RegisterObject(Invader);

        this->center_ = 0;
        //this->context = context;
    }

    Invader::~Invader()
    {
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

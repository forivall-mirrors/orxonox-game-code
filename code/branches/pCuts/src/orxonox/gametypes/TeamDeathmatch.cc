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

#include "TeamDeathmatch.h"

#include "core/CoreIncludes.h"
#include "core/ConfigValueIncludes.h"
#include "chat/ChatManager.h"
#include "interfaces/TeamColourable.h"
#include "infos/PlayerInfo.h"
#include "worldentities/TeamSpawnPoint.h"
#include "worldentities/pawns/Pawn.h"

namespace orxonox
{
    CreateUnloadableFactory(TeamDeathmatch);

    TeamDeathmatch::TeamDeathmatch(BaseObject* creator) : TeamGametype(creator)
    {
        RegisterObject(TeamDeathmatch);
    }

    void TeamDeathmatch::start()
    {
        TeamGametype::start();

        std::string message("The match has started!");
        ChatManager::message(message);
    }

    void TeamDeathmatch::end()
    {
        TeamGametype::end();

        std::string message("The match has ended.");
        ChatManager::message(message);
    }

    void TeamDeathmatch::playerEntered(PlayerInfo* player)
    {
        TeamGametype::playerEntered(player);

        const std::string& message = player->getName() + " entered the game";
        ChatManager::message(message);
    }

    bool TeamDeathmatch::playerLeft(PlayerInfo* player)
    {
        bool valid_player = TeamGametype::playerLeft(player);

        if (valid_player)
        {
            const std::string& message = player->getName() + " left the game";
            ChatManager::message(message);
        }

        return valid_player;
    }
    bool TeamDeathmatch::playerChangedName(PlayerInfo* player)
    {
        bool valid_player = TeamGametype::playerChangedName(player);

        if (valid_player)
        {
            const std::string& message = player->getOldName() + " changed name to " + player->getName();
            ChatManager::message(message);
        }

        return valid_player;
    }

    void TeamDeathmatch::pawnKilled(Pawn* victim, Pawn* killer)
    {
        if (victim && victim->getPlayer())
        {
            std::string message;
            if (killer)
            {
                if (killer->getPlayer())
                    message = victim->getPlayer()->getName() + " was killed by " + killer->getPlayer()->getName();
                else
                    message = victim->getPlayer()->getName() + " was killed";
            }
            else
                message = victim->getPlayer()->getName() + " died";

            ChatManager::message(message);
        }

        Gametype::pawnKilled(victim, killer);
    }

    void TeamDeathmatch::playerScored(PlayerInfo* player)
    {
        TeamGametype::playerScored(player);

        if (player)
        {
            const std::string& message = player->getName() + " scores!";
            ChatManager::message(message);
        }
    }

}

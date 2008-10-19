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

#include "OrxonoxStableHeaders.h"
#include "PlayerInfo.h"

#include <OgreSceneManager.h>

#include "core/CoreIncludes.h"
#include "core/ConfigValueIncludes.h"
#include "core/XMLPort.h"

#include "network/Host.h"

#include "Settings.h"
#include "GraphicsEngine.h"
#include "objects/gametypes/Gametype.h"

namespace orxonox
{
    CreateUnloadableFactory(PlayerInfo);

    PlayerInfo::PlayerInfo()
    {
        RegisterObject(PlayerInfo);

        this->ping_ = -1;
        this->bLocalPlayer_ = Settings::isStandalone();
        this->bLocalPlayer_ = false;
        this->bHumanPlayer_ = false;
        this->bFinishedSetup_ = false;

        this->setConfigValues();
        this->registerVariables();

        COUT(0) << "created PlayerInfo (" << this->getObjectID() << ")" << std::endl;
    }

    PlayerInfo::~PlayerInfo()
    {
        Gametype* gametype = Gametype::getCurrentGametype();
        if (gametype)
            gametype->removePlayer(this);
        COUT(0) << "destroyed PlayerInfo (" << this->getObjectID() << ")" << std::endl;
    }

    void PlayerInfo::setConfigValues()
    {
        SetConfigValue(playerName_, "Player").callback(this, &PlayerInfo::checkName);
    }

    void PlayerInfo::checkName()
    {
std::cout << "# PI(" << this->getObjectID() << "): checkName: " << this->bLocalPlayer_ << std::endl;
        if (this->bLocalPlayer_ && Settings::isMaster())
            this->setName(this->playerName_);
    }

    void PlayerInfo::changedName()
    {
std::cout << "# PI(" << this->getObjectID() << "): changedName to " << this->getName() << std::endl;
    }

    void PlayerInfo::registerVariables()
    {
        REGISTERSTRING(name_,         network::direction::toclient, new network::NetworkCallback<PlayerInfo>(this, &PlayerInfo::changedName));
        REGISTERSTRING(playerName_,   network::direction::toserver, new network::NetworkCallback<PlayerInfo>(this, &PlayerInfo::clientChangedName));
        REGISTERDATA(clientID_,       network::direction::toclient, new network::NetworkCallback<PlayerInfo>(this, &PlayerInfo::checkClientID));
        REGISTERDATA(ping_,           network::direction::toclient);
        REGISTERDATA(bHumanPlayer_,   network::direction::toclient);
        REGISTERDATA(bFinishedSetup_, network::direction::bidirectional, new network::NetworkCallback<PlayerInfo>(this, &PlayerInfo::finishedSetup));
    }

    void PlayerInfo::clientChangedName()
    {
std::cout << "# PI(" << this->getObjectID() << "): clientChangedName() to " << this->playerName_ << std::endl;
        this->setName(this->playerName_);
    }

    void PlayerInfo::checkClientID()
    {
std::cout << "# PI(" << this->getObjectID() << "): checkClientID()" << std::endl;
        this->bHumanPlayer_ = true;

        if (this->clientID_ == network::Host::getPlayerID())
        {
std::cout << "# PI(" << this->getObjectID() << "): checkClientID(): it's the client's ID" << std::endl;
            this->bLocalPlayer_ = true;
//std::cout << "# PI(" << this->getObjectID() << "): checkClientID(): name: " << this->getName() << std::endl;

            if (Settings::isClient())
            {
std::cout << "# PI(" << this->getObjectID() << "): checkClientID(): we're on a client: set object mode to bidirectional" << std::endl;
                this->setObjectMode(network::direction::bidirectional);
                this->playerName_ += "blub";
std::cout << "# PI(" << this->getObjectID() << "): checkClientID(): proposed name: " << this->playerName_ << std::endl;
            }
            else
            {
std::cout << "# PI(" << this->getObjectID() << "): checkClientID(): we're not on a client: finish setup" << std::endl;
                this->clientChangedName();
                this->bFinishedSetup_ = true;
                this->finishedSetup();
            }
        }
    }

    void PlayerInfo::finishedSetup()
    {
std::cout << "# PI(" << this->getObjectID() << "): finishedSetup(): " << this->bFinishedSetup_ << std::endl;
        if (Settings::isClient())
        {
std::cout << "# PI(" << this->getObjectID() << "): finishedSetup(): we're a client: finish setup" << std::endl;
            this->bFinishedSetup_ = true;
        }
        else if (this->bFinishedSetup_)
        {
std::cout << "# PI(" << this->getObjectID() << "): finishedSetup(): we're a server: add player" << std::endl;
            Gametype* gametype = Gametype::getCurrentGametype();
            if (gametype)
                gametype->addPlayer(this);
        }
        else
        {
std::cout << "# PI(" << this->getObjectID() << "): finishedSetup(): we're a server: client not yet finished" << std::endl;
        }
    }
}

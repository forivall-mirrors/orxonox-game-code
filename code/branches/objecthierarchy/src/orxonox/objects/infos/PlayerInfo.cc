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
#include "core/Core.h"

#include "network/Host.h"

#include "GraphicsEngine.h"
#include "objects/gametypes/Gametype.h"
#include "objects/worldentities/ControllableEntity.h"
#include "objects/controllers/HumanController.h"

namespace orxonox
{
    CreateUnloadableFactory(PlayerInfo);

    PlayerInfo::PlayerInfo()
    {
        RegisterObject(PlayerInfo);

        this->ping_ = -1;
        this->clientID_ = network::CLIENTID_UNKNOWN;
        this->bLocalPlayer_ = false;
        this->bHumanPlayer_ = false;
        this->bFinishedSetup_ = false;

        this->pawn_ = 0;
        this->pawnID_ = network::OBJECTID_UNKNOWN;
        this->controller_ = 0;
        this->setDefaultController(Class(HumanController));

        this->setConfigValues();
        this->registerVariables();
    }

    PlayerInfo::~PlayerInfo()
    {
        if (this->isInitialized())
        {
            Gametype* gametype = Gametype::getCurrentGametype();
            if (gametype)
                gametype->removePlayer(this);

            if (this->controller_)
                delete this->controller_;

            if (this->pawn_)
                this->pawn_->removePlayer();
        }
    }

    void PlayerInfo::setConfigValues()
    {
        SetConfigValue(nick_, "Player").callback(this, &PlayerInfo::checkNick);
    }

    void PlayerInfo::checkNick()
    {
        if (this->bLocalPlayer_)
        {
            this->playerName_ = this->nick_;

            if (Core::isMaster())
                this->setName(this->playerName_);
        }
    }

    void PlayerInfo::changedName()
    {
        Gametype* gametype = Gametype::getCurrentGametype();
        if (gametype)
            gametype->playerChangedName(this);
    }

    void PlayerInfo::registerVariables()
    {
        REGISTERSTRING(name_,         network::direction::toclient, new network::NetworkCallback<PlayerInfo>(this, &PlayerInfo::changedName));
        REGISTERSTRING(playerName_,   network::direction::toserver, new network::NetworkCallback<PlayerInfo>(this, &PlayerInfo::clientChangedName));
        REGISTERDATA(clientID_,       network::direction::toclient, new network::NetworkCallback<PlayerInfo>(this, &PlayerInfo::checkClientID));
        REGISTERDATA(ping_,           network::direction::toclient);
        REGISTERDATA(bHumanPlayer_,   network::direction::toclient);
        REGISTERDATA(pawnID_,         network::direction::toclient, new network::NetworkCallback<PlayerInfo>(this, &PlayerInfo::updatePawn));
        REGISTERDATA(bFinishedSetup_, network::direction::bidirectional, new network::NetworkCallback<PlayerInfo>(this, &PlayerInfo::finishedSetup));
    }

    void PlayerInfo::clientChangedName()
    {
        this->setName(this->playerName_);
    }

    void PlayerInfo::checkClientID()
    {
        this->bHumanPlayer_ = true;

        if (this->clientID_ == network::Host::getPlayerID())
        {
            this->takeLocalControl();

            if (Core::isClient())
                this->setObjectMode(network::direction::bidirectional);
            else
            {
                this->clientChangedName();
                this->bFinishedSetup_ = true;
                this->finishedSetup();
            }
        }
    }

    void PlayerInfo::finishedSetup()
    {
        if (Core::isClient())
            this->bFinishedSetup_ = true;
        else if (this->bFinishedSetup_)
        {
            Gametype* gametype = Gametype::getCurrentGametype();
            if (gametype)
                gametype->addPlayer(this);
        }
    }

    void PlayerInfo::startControl(ControllableEntity* pawn)
    {
        pawn->setPlayer(this);
        this->pawn_ = pawn;
        this->pawnID_ = pawn->getObjectID();

        if (this->controller_)
            this->controller_->setPawn(this->pawn_);
    }

    void PlayerInfo::stopControl()
    {
        this->pawn_->removePlayer();
        this->pawn_ = 0;
        this->pawnID_ = network::OBJECTID_UNKNOWN;
    }

    void PlayerInfo::takeLocalControl()
    {
        this->bLocalPlayer_ = true;
        this->playerName_ = this->nick_;
        this->createController();
    }

    void PlayerInfo::createController()
    {
        this->controller_ = this->defaultController_.fabricate();
        this->controller_->setPawn(this->pawn_);
    }

    void PlayerInfo::updatePawn()
    {
        this->pawn_ = dynamic_cast<ControllableEntity*>(network::Synchronisable::getSynchronisable(this->pawnID_));
        if (this->pawn_ && (this->pawn_->getPlayer() != this))
            this->pawn_->setPlayer(this);
    }
}

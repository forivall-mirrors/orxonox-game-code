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
#include "HumanPlayer.h"

#include "core/Core.h"
#include "core/CoreIncludes.h"
#include "core/ConfigValueIncludes.h"
#include "network/ClientInformation.h"
#include "network/Host.h"
#include "objects/controllers/HumanController.h"
#include "objects/gametypes/Gametype.h"

namespace orxonox
{
    CreateUnloadableFactory(HumanPlayer);

    HumanPlayer::HumanPlayer(BaseObject* creator) : PlayerInfo(creator)
    {
        RegisterObject(HumanPlayer);

        this->server_ready_ = Core::isMaster();
        this->client_ready_ = false;

        this->bHumanPlayer_ = true;
        this->defaultController_ = Class(HumanController);

        this->setConfigValues();
        this->registerVariables();

COUT(0) << this->getObjectID() << ": HumanPlayer created" << std::endl;
        network::Synchronisable* temp = dynamic_cast<network::Synchronisable*>(creator);
        if (temp)
        {
COUT(0) << this->getObjectID() << ": CreatorID: " << temp->getObjectID() << std::endl;
        }
        else
        {
COUT(0) << this->getObjectID() << ": Creator is no Synchronisable" << std::endl;
        }
    unsigned int creatorID = network::OBJECTID_UNKNOWN;
    searchcreatorID:
    if (creator)
    {
        if (creator->isA(Class(Synchronisable)))
        {
            Synchronisable* synchronisable_creator = dynamic_cast<Synchronisable*>(creator);
            creatorID = synchronisable_creator->getObjectID();
        }
        else if (creator != creator->getCreator())
        {
            creator = creator->getCreator();
            goto searchcreatorID;
        }
    }
COUT(0) << this->getObjectID() << ": ### tranmitted creatorID: " << creatorID << std::endl;
    }

    HumanPlayer::~HumanPlayer()
    {
COUT(0) << this->getObjectID() << ": HumanPlayer destroyed" << std::endl;
    }

    void HumanPlayer::setConfigValues()
    {
        SetConfigValue(nick_, "Player").callback(this, &HumanPlayer::configvaluecallback_changednick);
    }

    void HumanPlayer::registerVariables()
    {
        REGISTERSTRING(this->synchronize_nick_, network::direction::toserver, new network::NetworkCallback<HumanPlayer>(this, &HumanPlayer::networkcallback_changednick));

        REGISTERDATA(this->clientID_,     network::direction::toclient, new network::NetworkCallback<HumanPlayer>(this, &HumanPlayer::networkcallback_clientIDchanged));
        REGISTERDATA(this->server_ready_, network::direction::toclient, new network::NetworkCallback<HumanPlayer>(this, &HumanPlayer::networkcallback_server_ready));
        REGISTERDATA(this->client_ready_, network::direction::toserver, new network::NetworkCallback<HumanPlayer>(this, &HumanPlayer::networkcallback_client_ready));
    }

    void HumanPlayer::configvaluecallback_changednick()
    {
        if (this->isLocalPlayer())
        {
            this->synchronize_nick_ = this->nick_;

            if (Core::isMaster())
                this->setName(this->nick_);
        }
    }

    void HumanPlayer::networkcallback_changednick()
    {
        this->setName(this->synchronize_nick_);
    }

    void HumanPlayer::networkcallback_clientIDchanged()
    {
COUT(0) << this->getObjectID() << ": PI: clientID changed to " << this->clientID_ << std::endl;
        if (this->clientID_ == network::Host::getPlayerID())
        {
COUT(0) << this->getObjectID() << ": PI: it's my clientID" << std::endl;
            this->bLocalPlayer_ = true;
            this->synchronize_nick_ = this->nick_;
            this->client_ready_ = true;

            if (!Core::isMaster())
            {
                this->setObjectMode(network::direction::bidirectional);
COUT(0) << this->getObjectID() << ": PI: set objectmode to bidirectional" << std::endl;
            }
            else
                this->setName(this->nick_);

            this->createController();
        }
    }

    void HumanPlayer::networkcallback_server_ready()
    {
        this->client_ready_ = true;
COUT(0) << this->getObjectID() << ": PI: server ready, client set ready too" << std::endl;
    }

    void HumanPlayer::networkcallback_client_ready()
    {
COUT(0) << this->getObjectID() << ": PI: client ready" << std::endl;
        if (this->getGametype())
        {
COUT(0) << this->getObjectID() << ": PI: adding client to gametype" << std::endl;
            this->getGametype()->playerEntered(this);
        }
    }

    bool HumanPlayer::isReady() const
    {
        return (this->server_ready_ && this->client_ready_);
    }

    float HumanPlayer::getPing() const
    {
        return network::ClientInformation::findClient(this->getClientID())->getRTT();
    }

    float HumanPlayer::getPacketLossRatio() const
    {
        return network::ClientInformation::findClient(this->getClientID())->getPacketLoss();
    }

    void HumanPlayer::setClientID(unsigned int clientID)
    {
COUT(0) << this->getObjectID() << ": PI: set clientID to " << clientID << std::endl;
        this->clientID_ = clientID;
        this->networkcallback_clientIDchanged();
    }
}

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

#include "HumanPlayer.h"

#include "core/CoreIncludes.h"
#include "core/ConfigValueIncludes.h"
#include "core/GameMode.h"
// #include "network/ClientInformation.h"
#include "network/Host.h"
#include "controllers/HumanController.h"
#include "controllers/NewHumanController.h"
#include "gametypes/Gametype.h"
#include "overlays/OverlayGroup.h"

namespace orxonox
{
    CreateUnloadableFactory(HumanPlayer);

    HumanPlayer::HumanPlayer(BaseObject* creator) : PlayerInfo(creator)
    {
        RegisterObject(HumanPlayer);

        this->server_initialized_ = GameMode::isMaster();
        this->client_initialized_ = false;

        this->bHumanPlayer_ = true;
        this->defaultController_ = Class(NewHumanController);

        this->humanHud_ = 0;
        this->gametypeHud_ = 0;

        this->setConfigValues();
        this->registerVariables();
    }

    HumanPlayer::~HumanPlayer()
    {
        if (this->BaseObject::isInitialized())
        {
            if (this->humanHud_)
                this->humanHud_->destroy();

            if (this->gametypeHud_)
                this->gametypeHud_->destroy();
        }
    }

    void HumanPlayer::setConfigValues()
    {
        SetConfigValue(nick_, "Player").callback(this, &HumanPlayer::configvaluecallback_changednick);
        SetConfigValue(hudtemplate_, "defaultHUD").callback(this, &HumanPlayer::configvaluecallback_changedHUDTemplate);
    }

    void HumanPlayer::registerVariables()
    {
        registerVariable(this->synchronize_nick_, VariableDirection::ToServer, new NetworkCallback<HumanPlayer>(this, &HumanPlayer::networkcallback_changednick));

        registerVariable(this->clientID_,           VariableDirection::ToClient, new NetworkCallback<HumanPlayer>(this, &HumanPlayer::networkcallback_clientIDchanged));
        registerVariable(this->server_initialized_, VariableDirection::ToClient, new NetworkCallback<HumanPlayer>(this, &HumanPlayer::networkcallback_server_initialized));
        registerVariable(this->client_initialized_, VariableDirection::ToServer, new NetworkCallback<HumanPlayer>(this, &HumanPlayer::networkcallback_client_initialized));
    }

    void HumanPlayer::configvaluecallback_changednick()
    {
        if (this->isLocalPlayer())
        {
            this->synchronize_nick_ = this->nick_;

            if (GameMode::isMaster())
                this->setName(this->nick_);
        }
    }

    void HumanPlayer::configvaluecallback_changedHUDTemplate()
    {
        this->setHumanHUDTemplate(this->hudtemplate_);
    }

    void HumanPlayer::networkcallback_changednick()
    {
        this->setName(this->synchronize_nick_);
    }

    void HumanPlayer::networkcallback_clientIDchanged()
    {
        if (this->clientID_ == Host::getPlayerID())
        {
            this->bLocalPlayer_ = true;
            this->synchronize_nick_ = this->nick_;
            this->client_initialized_ = true;

            if (!GameMode::isMaster())
                this->setSyncMode(ObjectDirection::Bidirectional);
            else
                this->setName(this->nick_);

            this->createController();
            this->updateHumanHUD();
        }
    }

    void HumanPlayer::networkcallback_server_initialized()
    {
        this->client_initialized_ = true;
    }

    void HumanPlayer::networkcallback_client_initialized()
    {
        if (this->getGametype())
            this->getGametype()->playerEntered(this);
    }

    bool HumanPlayer::isInitialized() const
    {
        return (this->server_initialized_ && this->client_initialized_);
    }

    float HumanPlayer::getPing() const
    {
//         return static_cast<float>(ClientInformation::findClient(this->getClientID())->getRTT());
        return 0.; //TODO: reimplement this
    }

    float HumanPlayer::getPacketLossRatio() const
    {
      //         return static_cast<float>(ClientInformation::findClient(this->getClientID())->getPacketLoss());
        return 0.; //TODO: reimplement this
    }

    void HumanPlayer::setClientID(unsigned int clientID)
    {
        this->clientID_ = clientID;
        this->networkcallback_clientIDchanged();
    }

    void HumanPlayer::changedGametype()
    {
        PlayerInfo::changedGametype();

        if (this->isInitialized() && this->isLocalPlayer())
        {
            if (this->getGametype() && !this->getGametype()->getHUDTemplate().empty())
                this->setGametypeHUDTemplate(this->getGametype()->getHUDTemplate());
            else
                this->setGametypeHUDTemplate("");
        }
    }

    void HumanPlayer::updateHumanHUD()
    {
        if (this->humanHud_)
        {
            this->humanHud_->destroy();
            this->humanHud_ = 0;
        }

        if (this->isLocalPlayer() && !this->humanHudTemplate_.empty() && GameMode::showsGraphics())
        {
            this->humanHud_ = new OverlayGroup(this);
            this->humanHud_->addTemplate(this->humanHudTemplate_);
            this->humanHud_->setOwner(this);
        }
    }

    void HumanPlayer::updateGametypeHUD()
    {
        if (this->gametypeHud_)
        {
            this->gametypeHud_->destroy();
            this->gametypeHud_ = 0;
        }

        if (this->isLocalPlayer() && !this->gametypeHudTemplate_.empty())
        {
            this->gametypeHud_ = new OverlayGroup(this);
            this->gametypeHud_->addTemplate(this->gametypeHudTemplate_);
            this->gametypeHud_->setOwner(this);
        }
    }
}

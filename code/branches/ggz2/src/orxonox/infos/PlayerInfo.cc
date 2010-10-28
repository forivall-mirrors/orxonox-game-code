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

#include <cassert>

#include "PlayerInfo.h"

#include "core/CoreIncludes.h"
#include "network/ClientInformation.h"
#include "gametypes/Gametype.h"
#include "worldentities/ControllableEntity.h"

namespace orxonox
{
    PlayerInfo::PlayerInfo(BaseObject* creator) : Info(creator)
    {
        RegisterObject(PlayerInfo);

        this->clientID_ = CLIENTID_UNKNOWN;
        this->bHumanPlayer_ = false;
        this->bLocalPlayer_ = false;
        this->bReadyToSpawn_ = false;
        this->bSetUnreadyAfterSpawn_ = true;
        this->controller_ = 0;
        this->controllableEntity_ = 0;
        this->controllableEntityID_ = CLIENTID_UNKNOWN;

        this->gtinfo_ = 0;
        this->gtinfoID_ = OBJECTID_UNKNOWN;
        this->updateGametypeInfo();

        this->registerVariables();
    }

    PlayerInfo::~PlayerInfo()
    {
        if (this->BaseObject::isInitialized())
        {
            this->stopControl();

            if (this->controller_)
            {
                delete this->controller_;
                this->controller_ = 0;
            }

            if (this->getGametype())
                this->getGametype()->playerLeft(this);
        }
    }

    void PlayerInfo::registerVariables()
    {
        registerVariable(this->name_,                 VariableDirection::ToClient, new NetworkCallback<PlayerInfo>(this, &PlayerInfo::changedName));
        registerVariable(this->controllableEntityID_, VariableDirection::ToClient, new NetworkCallback<PlayerInfo>(this, &PlayerInfo::networkcallback_changedcontrollableentityID));
        registerVariable(this->bReadyToSpawn_,        VariableDirection::ToServer);
        registerVariable(this->gtinfoID_,             VariableDirection::ToClient, new NetworkCallback<PlayerInfo>(this, &PlayerInfo::networkcallback_changedgtinfoID));
    }

    void PlayerInfo::changedName()
    {
        SUPER(PlayerInfo, changedName);

        if (this->isInitialized() && this->getGametype())
            this->getGametype()->playerChangedName(this);
    }

    void PlayerInfo::changedGametype()
    {
        this->updateGametypeInfo();

        if (this->isInitialized())
        {
            if (this->getOldGametype())
            {
                if (this->getGametype())
                    this->getOldGametype()->playerSwitched(this, this->getGametype());
                else
                    this->getOldGametype()->playerLeft(this);
            }

            if (this->getGametype())
            {
                if (this->getOldGametype())
                    this->getGametype()->playerSwitchedBack(this, this->getOldGametype());
                else
                    this->getGametype()->playerEntered(this);
            }
        }
    }

    void PlayerInfo::updateGametypeInfo()
    {
        this->gtinfo_ = 0;
        this->gtinfoID_ = OBJECTID_UNKNOWN;

        if (this->getGametype() && this->getGametype()->getGametypeInfo())
        {
            this->gtinfo_ = this->getGametype()->getGametypeInfo();
            this->gtinfoID_ = this->gtinfo_->getObjectID();
        }
    }

    void PlayerInfo::createController()
    {
        if (this->controller_)
        {
            delete this->controller_;
            this->controller_ = 0;
        }
        this->controller_ = this->defaultController_.fabricate(this);
        assert(this->controller_);
        this->controller_->setPlayer(this);
        if (this->controllableEntity_)
            this->controller_->setControllableEntity(this->controllableEntity_);
        this->changedController();
    }

    void PlayerInfo::startControl(ControllableEntity* entity)
    {
        if (!entity || entity == this->controllableEntity_)
            return;

        if (this->controllableEntity_)
            this->stopControl();

        this->controllableEntity_ = entity;
        this->controllableEntityID_ = entity->getObjectID();

        entity->setPlayer(this);

        this->bReadyToSpawn_ &= (!this->bSetUnreadyAfterSpawn_);

        if (this->controller_)
            this->controller_->setControllableEntity(entity);

        this->changedControllableEntity();
    }

    void PlayerInfo::stopControl()
    {
        ControllableEntity* entity = this->controllableEntity_;

        if (!entity)
            return;

        this->controllableEntity_ = 0;
        this->controllableEntityID_ = OBJECTID_UNKNOWN;

        if (this->controller_)
            this->controller_->setControllableEntity(0);

        entity->removePlayer();

        this->changedControllableEntity();
    }

    void PlayerInfo::networkcallback_changedcontrollableentityID()
    {
        if (this->controllableEntityID_ != OBJECTID_UNKNOWN)
        {
            Synchronisable* temp = Synchronisable::getSynchronisable(this->controllableEntityID_);
            ControllableEntity* entity = orxonox_cast<ControllableEntity*>(temp);
            this->startControl(entity);
        }
        else
        {
            this->stopControl();
        }
    }

    void PlayerInfo::networkcallback_changedgtinfoID()
    {
        if (this->gtinfoID_ != OBJECTID_UNKNOWN)
        {
            this->gtinfo_ = orxonox_cast<GametypeInfo*>(Synchronisable::getSynchronisable(this->gtinfoID_));

            if (!this->gtinfo_)
                this->gtinfoID_ = OBJECTID_UNKNOWN;
        }
    }
}
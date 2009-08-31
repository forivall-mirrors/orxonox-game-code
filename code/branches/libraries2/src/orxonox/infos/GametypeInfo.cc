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

#include "GametypeInfo.h"

#include "core/CoreIncludes.h"
#include "core/GameMode.h"
#include "network/NetworkFunction.h"
#include "network/Host.h"
#include "interfaces/GametypeMessageListener.h"

namespace orxonox
{
    CreateUnloadableFactory(GametypeInfo);

    registerMemberNetworkFunction(GametypeInfo, dispatchAnnounceMessage);
    registerMemberNetworkFunction(GametypeInfo, dispatchKillMessage);
    registerMemberNetworkFunction(GametypeInfo, dispatchDeathMessage);

    GametypeInfo::GametypeInfo(BaseObject* creator) : Info(creator)
    {
        RegisterObject(GametypeInfo);

        this->bStarted_ = false;
        this->bEnded_ = false;
        this->startCountdown_ = 0;
        this->bStartCountdownRunning_ = false;

        this->registerVariables();
    }

    GametypeInfo::~GametypeInfo()
    {
    }

    void GametypeInfo::registerVariables()
    {
        registerVariable(this->bStarted_,               VariableDirection::ToClient);
        registerVariable(this->bEnded_,                 VariableDirection::ToClient);
        registerVariable(this->startCountdown_,         VariableDirection::ToClient);
        registerVariable(this->bStartCountdownRunning_, VariableDirection::ToClient);
        registerVariable(this->hudtemplate_,            VariableDirection::ToClient);
    }

    void GametypeInfo::sendAnnounceMessage(const std::string& message)
    {
        if (GameMode::isMaster())
        {
            callMemberNetworkFunction(GametypeInfo, dispatchAnnounceMessage, this->getObjectID(), CLIENTID_UNKNOWN, message);
            this->dispatchAnnounceMessage(message);
        }
    }

    void GametypeInfo::sendAnnounceMessage(const std::string& message, unsigned int clientID)
    {
        if (GameMode::isMaster())
        {
            if (clientID == CLIENTID_SERVER)
                this->dispatchAnnounceMessage(message);
            else
                callMemberNetworkFunction(GametypeInfo, dispatchAnnounceMessage, this->getObjectID(), clientID, message);
        }
    }

    void GametypeInfo::sendKillMessage(const std::string& message, unsigned int clientID)
    {
        if (GameMode::isMaster())
        {
            if (clientID == CLIENTID_SERVER)
                this->dispatchKillMessage(message);
            else
                callMemberNetworkFunction(GametypeInfo, dispatchKillMessage, this->getObjectID(), clientID, message);
        }
    }

    void GametypeInfo::sendDeathMessage(const std::string& message, unsigned int clientID)
    {
        if (GameMode::isMaster())
        {
            if (clientID == CLIENTID_SERVER)
                this->dispatchDeathMessage(message);
            else
                callMemberNetworkFunction(GametypeInfo, dispatchDeathMessage, this->getObjectID(), clientID, message);
        }
    }

    void GametypeInfo::dispatchAnnounceMessage(const std::string& message)
    {
        for (ObjectList<GametypeMessageListener>::iterator it = ObjectList<GametypeMessageListener>::begin(); it != ObjectList<GametypeMessageListener>::end(); ++it)
            it->announcemessage(this, message);
    }

    void GametypeInfo::dispatchKillMessage(const std::string& message)
    {
        for (ObjectList<GametypeMessageListener>::iterator it = ObjectList<GametypeMessageListener>::begin(); it != ObjectList<GametypeMessageListener>::end(); ++it)
            it->killmessage(this, message);
    }

    void GametypeInfo::dispatchDeathMessage(const std::string& message)
    {
        for (ObjectList<GametypeMessageListener>::iterator it = ObjectList<GametypeMessageListener>::begin(); it != ObjectList<GametypeMessageListener>::end(); ++it)
            it->deathmessage(this, message);
    }
}

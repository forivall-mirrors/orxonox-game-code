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
 *      Damian 'Mozork' Frick
 *
 */

/**
    @file GametypeInfo.cc
    @brief Implementation of the GametypeInfo class
*/

#include "GametypeInfo.h"

#include "core/CoreIncludes.h"
#include "core/GameMode.h"
#include "network/Host.h"
#include "network/NetworkFunction.h"
#include "util/Convert.h"

#include "interfaces/GametypeMessageListener.h"
#include "interfaces/NotificationListener.h"

#include "PlayerInfo.h"

namespace orxonox
{
    CreateUnloadableFactory(GametypeInfo);

    registerMemberNetworkFunction(GametypeInfo, dispatchAnnounceMessage);
    registerMemberNetworkFunction(GametypeInfo, dispatchKillMessage);
    registerMemberNetworkFunction(GametypeInfo, dispatchDeathMessage);
    registerMemberNetworkFunction(GametypeInfo, dispatchStaticMessage);
    registerMemberNetworkFunction(GametypeInfo, dispatchFadingMessage);

    /*static*/ const std::string GametypeInfo::NOTIFICATION_SENDER("gameinfo");

    /**
    @brief
        Registers and initializes the object.
    */
    GametypeInfo::GametypeInfo(BaseObject* creator) : Info(creator)
    {
        RegisterObject(GametypeInfo);

        this->bActive_ = false; // At first the GametypeInfo is inactive.
        
        this->bStarted_ = false;
        this->bEnded_ = false;
        this->startCountdown_ = 0.0f;
        this->bStartCountdownRunning_ = false;
        this->counter_ = 0;

        this->registerVariables();
    }

    GametypeInfo::~GametypeInfo()
    {
    }

    void GametypeInfo::registerVariables()
    {
        registerVariable(this->bStarted_,               VariableDirection::ToClient, new NetworkCallback<GametypeInfo>(this, &GametypeInfo::changedStarted));
        registerVariable(this->bEnded_,                 VariableDirection::ToClient, new NetworkCallback<GametypeInfo>(this, &GametypeInfo::changedEnded));
        registerVariable(this->bStartCountdownRunning_, VariableDirection::ToClient, new NetworkCallback<GametypeInfo>(this, &GametypeInfo::changedStartCountdownRunning));
        registerVariable(this->startCountdown_,         VariableDirection::ToClient);
        registerVariable(this->counter_,                VariableDirection::ToClient, new NetworkCallback<GametypeInfo>(this, &GametypeInfo::changedCountdownCounter));
        registerVariable(this->hudtemplate_,            VariableDirection::ToClient);
    }

    /**
    @brief
        Is called when the activity has changed.
    */
    void GametypeInfo::changedActivity(void)
    {
        SUPER(GametypeInfo, changedActivity);

        // If the GametypeInfo has become active the "Press [Fire] to start the match" notification is sent.
        if(this->isActive())
            NotificationListener::sendNotification("Press [Fire] to start the match", GametypeInfo::NOTIFICATION_SENDER);
    }

    /**
    @brief
        Is called when the game has changed to started.
    */
    void GametypeInfo::changedStarted(void)
    {
        
    }

    /**
    @brief
        Is called when the game has changed to ended.
    */
    void GametypeInfo::changedEnded(void)
    {
        // If the game has ended, a "Game has ended" notification is displayed.
        if(this->hasEnded())
            NotificationListener::sendNotification("Game has ended", GametypeInfo::NOTIFICATION_SENDER);
    }

    /**
    @brief
        Is called when the start countdown has been either started or stopped.
    */
    void GametypeInfo::changedStartCountdownRunning(void)
    {
        // Clear the notifications if the countdown has stopped running.
        if(!this->hasStarted() && !this->isStartCountdownRunning() && !this->hasEnded())
            NotificationListener::sendCommand("clear", GametypeInfo::NOTIFICATION_SENDER);
        // Send first countdown notification if the countdown has started.
        if(!this->hasStarted() && this->isStartCountdownRunning() && !this->hasEnded())
            NotificationListener::sendNotification(multi_cast<std::string>(this->counter_), GametypeInfo::NOTIFICATION_SENDER);
    }

    /**
    @brief
        Is called when the start countdown counter has changed.
    */
    void GametypeInfo::changedCountdownCounter(void)
    {
        // Send countdown notification if the counter has gone down.
        if(!this->hasStarted() && this->isStartCountdownRunning() && !this->hasEnded())
            NotificationListener::sendNotification(multi_cast<std::string>(this->counter_), GametypeInfo::NOTIFICATION_SENDER);
    }

    /**
    @brief
        Is called when a player has become ready to spawn.
    @param player
        The player that has become ready to spawn.
    */
    void GametypeInfo::changedReadyToSpawn(PlayerInfo* player)
    {
        // Send "Waiting for other players" over the network to the right player if a player has become ready to spawn.
        if(!this->hasStarted() && !this->isStartCountdownRunning() && !this->hasEnded())
            NotificationListener::sendNotification("Waiting for other players", GametypeInfo::NOTIFICATION_SENDER, notificationMessageType::info, notificationSendMode::network, player->getClientID());
        // Clear the notifications if the player has respawned.
        if(this->hasStarted() && !this->hasEnded())
            NotificationListener::sendCommand("clear", GametypeInfo::NOTIFICATION_SENDER, notificationSendMode::network, player->getClientID());
    }

    /**
    @brief
        Inform the GametypeInfo that the game has started.
    */
    void GametypeInfo::start(void)
    {
        if(this->bStarted_)
            return;
        
        this->bStarted_ = true;
        this->changedStarted();
    }

    /**
    @brief
        Inform the GametypeInfo that the game has ended.
    */
    void GametypeInfo::end(void)
    {
        if(this->bEnded_)
            return;

        this->bEnded_ = true;
        this->changedEnded();
    }

    /**
    @brief
        Set the start countdown to the input value.
    @param countdown
        The countdown to be set.
    */
    void GametypeInfo::setStartCountdown(float countdown)
    {
        if(this->startCountdown_ == countdown || countdown < 0.0f)
            return;
        
        this->startCountdown_ = countdown;
        // Set the counter to the ceiling of the current countdown.
        this->counter_ = ceil(countdown);
        this->changedCountdownCounter();
    }

    /**
    @brief
        Count down the start countdown by the specified value.
    @param countDown
        The amount by which we count down.
    */
    void GametypeInfo::countdownStartCountdown(float countDown)
    {
        float newCountdown = this->startCountdown_ - countDown;
        // If we have switched integers or arrived at zero, we also count down the start countdown counter.
        if(ceil(newCountdown) != ceil(this->startCountdown_) || newCountdown <= 0.0f)
            this->countDown();
        this->startCountdown_ = newCountdown;
    }

    /**
    @brief
        Count down the start countdown counter.
    */
    void GametypeInfo::countDown()
    {
        if(this->counter_ == 0)
            return;
        
        this->counter_--;
        this->changedCountdownCounter();
    }

    /**
    @brief
        Inform the GametypeInfo about the start of the start countdown.
    */
    void GametypeInfo::startStartCountdown(void)
    {
        if(this->bStartCountdownRunning_)
            return;
        
        this->bStartCountdownRunning_ = true;
        this->changedStartCountdownRunning();
    }

    /**
    @brief
        Inform the GametypeInfo about the stop of the start countdown.
    */
    void GametypeInfo::stopStartCountdown(void)
    {
        if(!this->bStartCountdownRunning_)
            return;
        
        this->bStartCountdownRunning_ = false;
        this->changedStartCountdownRunning();
    }

    /**
    @brief
        Inform the GametypeInfo about a player that is ready to spawn.
    @param player
        The player that is ready to spawn.
    */
    void GametypeInfo::playerReadyToSpawn(PlayerInfo* player)
    {
        // If the player has spawned already.
        if(this->spawned_.find(player) != this->spawned_.end())
            return;

        this->spawned_.insert(player);
        this->changedReadyToSpawn(player);
    }

    /**
    @brief
        Inform the GametypeInfo about a player whose Pawn has been killed.
    @param player
        The player whose Pawn has been killed.
    */
    void GametypeInfo::pawnKilled(PlayerInfo* player)
    {
        NotificationListener::sendNotification("Press [Fire] to respawn", GametypeInfo::NOTIFICATION_SENDER, notificationMessageType::info, notificationSendMode::network, player->getClientID());
        // Remove the player from the list of players that have spawned, since it currently is not.
        this->spawned_.erase(player);
    }

    /**
    @brief
        Inform the GametypeInfo about a player that has spawned.
    @param player
        The player that has spawned.
    */
    void GametypeInfo::playerSpawned(PlayerInfo* player)
    {
        if(this->hasStarted() && !this->hasEnded())
            NotificationListener::sendCommand("clear", GametypeInfo::NOTIFICATION_SENDER, notificationSendMode::network, player->getClientID());
    }

    void GametypeInfo::sendAnnounceMessage(const std::string& message)
    {
        if (GameMode::isMaster())
        {
            callMemberNetworkFunction(GametypeInfo, dispatchAnnounceMessage, this->getObjectID(), NETWORK_PEER_ID_BROADCAST, message);
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

    void GametypeInfo::sendStaticMessage(const std::string& message, unsigned int clientID, const ColourValue& colour)
    {
        if (GameMode::isMaster())
        {
            if (clientID == CLIENTID_SERVER)
                this->dispatchStaticMessage(message, colour);
            else
                callMemberNetworkFunction(GametypeInfo, dispatchStaticMessage, this->getObjectID(), clientID, message, colour);
        }
    }

    void GametypeInfo::sendFadingMessage(const std::string& message, unsigned int clientID)
    {
        if (GameMode::isMaster())
        {
            if (clientID == CLIENTID_SERVER)
                this->dispatchFadingMessage(message);
            else
                callMemberNetworkFunction(GametypeInfo, dispatchFadingMessage, this->getObjectID(), clientID, message);
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

     void GametypeInfo::dispatchStaticMessage(const std::string& message, const ColourValue& colour)
    {
        for (ObjectList<GametypeMessageListener>::iterator it = ObjectList<GametypeMessageListener>::begin(); it != ObjectList<GametypeMessageListener>::end(); ++it)
            it->staticmessage(this, message, colour);
    }

     void GametypeInfo::dispatchFadingMessage(const std::string& message)
    {
        for (ObjectList<GametypeMessageListener>::iterator it = ObjectList<GametypeMessageListener>::begin(); it != ObjectList<GametypeMessageListener>::end(); ++it)
            it->fadingmessage(this, message);
    }
}

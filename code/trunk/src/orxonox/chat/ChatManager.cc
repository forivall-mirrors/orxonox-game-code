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

#include "ChatManager.h"
#include "ChatListener.h"

#include "util/ScopedSingletonManager.h"
#include "core/CoreIncludes.h"
#include "core/command/ConsoleCommand.h"
#include "network/Host.h"

#include "PlayerManager.h"
#include "infos/PlayerInfo.h"

namespace orxonox
{
    ManageScopedSingleton(ChatManager, ScopeID::Root, false);

    SetConsoleCommand("chat", &ChatManager::chat).defaultValue(1, NETWORK_PEER_ID_BROADCAST);

    ChatManager::ChatManager()
    {
        RegisterObject(ChatManager);
    }

    /**
        @brief Sends a message (usually gameplay related) without explicit sender to the chat.
        @param message The message
        @param targetID The client ID (network) of the receiver. Usually the broadcast ID
    */
    /*static*/ void ChatManager::message(const std::string& message, unsigned int targetID)
    {
        unsigned int sourceID = NETWORK_PEER_ID_UNKNOWN;

        // only the server should send gameplay messages through the network. otherwise display the message locally
        if (GameMode::isServer())
            Host::sendChat(message, sourceID, targetID);
        else
            ChatManager::getInstance().incomingChat(message, sourceID);
    }

    /**
        @brief Sends a chat message with the client as its sender to the chat.
        @param message The message
        @param targetID The client ID (network) of the receiver. Usually the broadcast ID
    */
    /*static*/ void ChatManager::chat(const std::string& message, unsigned int targetID)
    {
        unsigned int sourceID = Host::getPlayerID();

        // if connected to the network, send chat messages online. otherwise display chat locally
        if (GameMode::isServer() || GameMode::isClient())
            Host::sendChat(message, sourceID, targetID);
        else
            ChatManager::getInstance().incomingChat(message, sourceID);
    }

    /**
        @brief Inherited callback from NetworkChatListener, but gets also called directly
        by ChatManager itself if the client is offline. Distributes an incoming chat message
        to all ChatListeners.
        @param message The message
        @param sourceID The client ID (network) of the sender
    */
    void ChatManager::incomingChat(const std::string& message, unsigned int sourceID)
    {
        std::string text = message;
        std::string name;

        // get the name of the sender and prepend it to the text message
        if (sourceID != NETWORK_PEER_ID_UNKNOWN)
        {
            PlayerInfo* player = PlayerManager::getInstance().getClient(sourceID);
            if (player)
            {
                name = player->getName();
                text = name + ": " + message;
            }
        }

        // notify all listeners
        for (ObjectList<ChatListener>::iterator it = ObjectList<ChatListener>::begin(); it != ObjectList<ChatListener>::end(); ++it)
            it->incomingChat(text, name);
    }


    //////////////////////////////////////////////////////////////////////////
    // ChatListener                                                         //
    //////////////////////////////////////////////////////////////////////////

    ChatListener::ChatListener()
    {
        RegisterRootObject(ChatListener);
    }
}

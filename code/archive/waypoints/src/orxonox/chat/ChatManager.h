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

#ifndef _ChatManager_H__
#define _ChatManager_H__

#include "OrxonoxPrereqs.h"
#include "util/Singleton.h"
#include "network/NetworkChatListener.h"

namespace orxonox
{
    /**
        @brief This class collects and distributes chat messages. If the client is online,
        all chat messages are sent and received through the network. Otherwise chat is handled
        directly by this class.
    */
    class _OrxonoxExport ChatManager : public Singleton<ChatManager>, public NetworkChatListener
    {
        friend class Singleton<ChatManager>;

        public:
            ChatManager();
            virtual ~ChatManager() {}

            static void message(const std::string& message, unsigned int targetID = NETWORK_PEER_ID_BROADCAST);
            static void chat(const std::string& message, unsigned int targetID = NETWORK_PEER_ID_BROADCAST);

        protected:
            ChatManager(const ChatManager&);

            virtual void incomingChat(const std::string& message, unsigned int sourceID);

            static ChatManager* singletonPtr_s;
    };
}

#endif /* _ChatManager_H__ */

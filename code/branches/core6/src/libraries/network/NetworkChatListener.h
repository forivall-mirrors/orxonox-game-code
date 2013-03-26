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

#ifndef _NetworkChatListener_H__
#define _NetworkChatListener_H__

#include "NetworkPrereqs.h"

#include "core/object/Listable.h"

namespace orxonox
{
    /**
        @brief The only function of this interface, incomingChat(), gets called whenever a chat
        message was received through the network.

        This interface exists mostly to resolve circular dependencies between the chat functions
        in Host and ChatManager. ChatManager is the main derivative of this interface.
    */
    class _NetworkExport NetworkChatListener : virtual public Listable
    {
        friend class Host;

        public:
            NetworkChatListener(); // implemented in Host.cc
            virtual ~NetworkChatListener() {}

        protected:
            /**
                @brief Gets called when a chat message was received through the network.
                @param message The message
                @param sourceID The (network) client ID of the sender
            */
            virtual void incomingChat(const std::string& message, unsigned int sourceID) = 0;
    };
}

#endif /* _NetworkChatListener_H__ */

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

#ifndef _ChatListener_H__
#define _ChatListener_H__

#include "OrxonoxPrereqs.h"

#include "core/class/OrxonoxClass.h"

namespace orxonox
{
    /**
        @brief Classes inheriting from this interface get a notification whenever a chat
        message was sent through ChatManager.
    */
    class _OrxonoxExport ChatListener : virtual public OrxonoxClass
    {
        friend class ChatManager;

        public:
            ChatListener(); // implemented in ChatManager.cc
            virtual ~ChatListener() {}

        protected:
            /**
                @brief Gets called whenever a chat message was sent through ChatManager.
                @param message The whole message (including name of the sender if available)
                @param name The name of the sender (if available, otherwise "")
            */
            virtual void incomingChat(const std::string& message, const std::string& name) = 0;
    };
}

#endif /* _ChatListener_H__ */

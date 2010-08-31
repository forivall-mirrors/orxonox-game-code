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
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See thes
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 *   Author:
 *      Damian 'Mozork' Frick
 *   Co-authors:
 *      ...
 *
 */

#include "QuestNotification.h"
#include "core/CoreIncludes.h"

namespace orxonox {

    /*static*/ const std::string QuestNotification::SENDER("questsystem");

    CreateUnloadableFactory(QuestNotification);

    /**
    @brief
        Default Constructor. Creates a useless QuestNotification.
    */
    QuestNotification::QuestNotification(BaseObject* creator) : Notification(creator)
    {
        RegisterObject(QuestNotification);
    }

    /**
    @brief
        Creates a QuestNotification with the input message.
    @param message
        The message to be sent.
    */
    QuestNotification::QuestNotification(BaseObject* creator, const std::string & message) : Notification(creator, message)
    {
        RegisterObject(QuestNotification);
    }

    /**
    @brief
        Destructor.
    */
    QuestNotification::~QuestNotification()
    {

    }

    /**
    @brief
        Send the QuestNotification.
    @return
        Returns true if successful.
    */
    bool QuestNotification::send(void)
    {
        return this->Notification::send(QuestNotification::SENDER);
    }


}

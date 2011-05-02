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

/**
    @file NotificationListener.h
    @brief Definition of the NotificationListener class.
    @ingroup Notifications
*/

#ifndef _NotificationListener_H__
#define _NotificationListener_H__

#include "OrxonoxPrereqs.h"

#include <ctime>
#include <set>
#include <string>

#include "util/StringUtils.h"

#include "core/OrxonoxClass.h"

namespace orxonox
{
    class Notification;

    /**
    @brief
        NotificationListener interface.

        The NotificationListener interface presents a means to being informed when @ref orxonox::Notification "Notifications" in the target set of this NotificationListener change. (e.g. @ref orxonox::Notification "Notifications" were added or removed)
        When inheriting from a NotificationListener it is important to register (in the constructor) and unregister (in the destructor) it to and from the @ref orxonox::NotificationManager "NotificationManager".

    @author
        Fabian 'x3n' Landau

    @ingroup Notifications
    */
    class _OrxonoxExport NotificationListener : virtual public OrxonoxClass
    {
        public:
            NotificationListener();
            virtual ~NotificationListener() {}
            
            static const std::string ALL; //!< Static string to indicate a sender that sends to all NotificationListeners.
            static const std::string NONE; //!< Static string to indicare a sender that sends to no specific NotificationListener.
            
            static void sendNotification(const std::string& message, unsigned int clientId, const std::string& sender = NotificationListener::NONE, bool isLocal = false);
            
            virtual bool registerNotification(const std::string& message, const std::string& sender)
                { return false; }
    };
}

#endif /* _NotificationListener_H__ */

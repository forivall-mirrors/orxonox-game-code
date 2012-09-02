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

            /**
            @brief Get the senders that are targets of this NotificationListener.
            @return Returns the set of senders that are targets of this NotificationListener.
            */
            virtual const std::set<std::string> & getTargetsSet(void) = 0;

            /**
            @brief Updates the whole NotificationListener.
                   This is called by the @ref orxonox::NotificationManager "NotificationManager" when the @ref orxonox::Notification "Notifications" have changed so much, that the NotificationListener may have to re-initialize his operations.
            */
            virtual void update(void) = 0;
            /**
            @brief Updates the NotificationListener, when a new Notification has come in at the specified time.
            @param notification A pointer to the @ref orxonox::Notification "Notification".
            @param time The time the @ref orxonox::Notification "Notification" has come in.
            */
            virtual void update(Notification* notification, const std::time_t & time) = 0;
    };
}

#endif /* _NotificationListener_H__ */

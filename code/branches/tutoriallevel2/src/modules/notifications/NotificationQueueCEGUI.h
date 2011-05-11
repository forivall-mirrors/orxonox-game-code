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
 *      Damian 'Mozork' Frick
 *   Co-authors:
 *      ...
 *
 */

/**
    @file NotificationQueueCEGUI.h
    @brief Definition of the NotificationQueueCEGUI class.
    @ingroup Notifications
*/

#ifndef _NotificationOueueCEGUI_H__
#define _NotificationOueueCEGUI_H__

#include "notifications/NotificationsPrereqs.h"

#include <string>

#include "NotificationManager.h"
#include "NotificationQueue.h"

namespace orxonox // tolua_export
{ // tolua_export

    //TODO: Update.
    /**
    @brief
        Displays @ref orxonox::Notification "Notifications" from specific senders.

        There are quite some parameters that influence the behavior of the NotificationQueue:
        - @b name The name of the NotificationQueue. It needs to be unique.
        - @b senders The senders that are targets of this NotificationQueue, i.e. the names of senders whose Notifications this NotificationQueue displays.
        - @b size The size of the NotificationQueue, it specifies how many @ref orxonox::Notification "Notifications" are displayed at once at the most.
        - @b displayTime The time a @ref orxonox::Notification "Notification" is displayed with this NotificationQueue.

    @author
        Damian 'Mozork' Frick

    @ingroup Notifications
    */
    class _NotificationsExport NotificationQueueCEGUI // tolua_export
        : public NotificationQueue
    { // tolua_export

        public:
            NotificationQueueCEGUI(const std::string& name, const std::string& senders = NotificationListener::ALL, unsigned int size = NotificationQueue::DEFAULT_SIZE, unsigned int displayTime = NotificationQueue::DEFAULT_DISPLAY_TIME);
            virtual ~NotificationQueueCEGUI();

            //! Destroys the NotificationQueue.
            void destroy(bool noGraphics = false); // tolua_export
            
        protected:
            virtual void notificationPushed(Notification* notification); // Is called by the NotificationQueue when a notification was pushed
            virtual void notificationPopped(void); // Is called by the NotificationQueue when a notification was popped.
            virtual void notificationRemoved(unsigned int index); // Is called when a notification was removed.
            
            virtual void clear(bool noGraphics = false); // Clears the NotificationQueue by removing all NotificationContainers.
            
        private:
            void create(void); // Creates the NotificationQueue in lua.            

    }; // tolua_export

} // tolua_export

#endif /* _NotificationQueueCEGUI_H__ */

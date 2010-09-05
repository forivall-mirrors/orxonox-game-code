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
    @file NotificationQueue.h
    @brief Definition of the NotificationQueue class.
*/

#ifndef _NotificationOueue_H__
#define _NotificationOueue_H__

#include "notifications/NotificationsPrereqs.h"

#include <ctime>
#include <set>
#include <string>
#include <vector>

#include "tools/interfaces/Tickable.h"
#include "interfaces/NotificationListener.h"
#include "NotificationManager.h"

namespace orxonox
{

    //! Container to allow easy handling.
    struct NotificationContainer
    {
        Notification* notification; //!< The Notification displayed.
        time_t time; //!< The time the Notification was sent and thus first displayed.
    };

    //! Struct to allow ordering of NotificationContainers.
    struct NotificationContainerCompare {
        bool operator() (const NotificationContainer* const & a, const NotificationContainer* const & b) const
            { return a->time < b->time; } //!< Ordered by time.
    };

    /**
    @brief
        Displays Notifications from specific senders.
    @author
        Damian 'Mozork' Frick
    */
    class _NotificationsExport NotificationQueue : public Tickable, public NotificationListener
    {

        public:
            NotificationQueue(const std::string& name, const std::string& senders = NotificationManager::ALL, unsigned int size = NotificationQueue::DEFAULT_SIZE, unsigned int displayTime = NotificationQueue::DEFAULT_DISPLAY_TIME);
            virtual ~NotificationQueue();

            virtual void tick(float dt); //!< To update from time to time.

            void update(void); //!< Updates the queue.
            void update(Notification* notification, const std::time_t & time); //!< Adds a Notification to the queue.

            /**
            @brief Get the name of the NotificationQueue.
            @return Returns the name.
            */
            inline const std::string& getName() const
                { return this->name_; }

            /**
            @brief Returns the maximum number of Notifications displayed.
            @return Returns maximum size.
            */
            inline unsigned int getMaxSize() const
                { return this->maxSize_; }
            /**
            @brief Returns the current number of Notifications displayed.
            @return Returns the size of the queue.
            */
            inline unsigned int getSize() const
                { return this->size_; }
            /**
            @brief Returns the time interval the Notification is displayed.
            @return Returns the display time.
            */
            inline float getDisplayTime() const
                { return this->displayTime_; }

            /**
            @brief Returns the targets of this queue, reps. the senders which Notifications are displayed in this queue.
            @return Retuns a set of string holding the different targets.
            */
            inline const std::set<std::string> & getTargetsSet()
                { return this->targets_; }
            bool getTargets(std::string* string) const; //!< Returns a string consisting of the concatination of the targets.

        private:
            static const unsigned int DEFAULT_SIZE = 5; //!< The default maximum number of Notifications displayed.
            static const unsigned int DEFAULT_DISPLAY_TIME = 30; //!< The default display time.

            std::string name_; //!< The name of the NotificationQueue.

            unsigned int maxSize_; //!< The maximal number of Notifications displayed.
            unsigned int size_; //!< The number of Notifications displayed.
            unsigned int displayTime_; //!< The time a Notification is displayed.

            std::set<std::string> targets_; //!< The targets the Queue displays Notifications of.

            std::multiset<NotificationContainer*, NotificationContainerCompare> ordering_; //!< The NotificationContainers ordered by the time they were registered. //TODO: Would set work as well?
            std::vector<NotificationContainer*> notifications_; //!< The NotificationContainers in the order they were added to the NotificationQueue.

            float tickTime_; //!< Helper variable, to not have to check for Notifications that have been displayed too long, every tick.
            NotificationContainer timeLimit_; //!< Helper object to check against to determine whether Notifications have expired.

            bool registered_; //!< Helper variable to remember whether the NotificationQueue is registered already.

            void initialize(void); //!< Initializes the object.
            void create(void); //!< Creates the NotificationQueue in lua.

            bool setName(const std::string& name); //!< Sets the name of the NotificationQueue.

            void setMaxSize(unsigned int size); //!< Sets the maximum number of displayed Notifications.
            void setDisplayTime(unsigned int time); //!< Sets the maximum number of seconds a Notification is displayed.

            bool setTargets(const std::string & targets); //!< Set the targets of this NotificationQueue.

            void sizeChanged(void); //!< Adjusts the NotificationQueue, when the maximum size has changed.

            void push(Notification* notification, const std::time_t & time); //!< Add a Notification to the NotificationQueue.
            void pop(void); //!< Removes the least recently added Notification form the NotificationQueue.
            void remove(NotificationContainer* container); //!< Removes the Notification that is stored in the input container.

            void clear(void); //!< Clears the queue by removing all Notifications.

    };

}

#endif /* _NotificationOverlay_H__ */

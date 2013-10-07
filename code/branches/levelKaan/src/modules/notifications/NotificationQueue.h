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
    @ingroup Notifications
*/

#ifndef _NotificationOueue_H__
#define _NotificationOueue_H__

#include "notifications/NotificationsPrereqs.h"

#include <ctime>
#include <set>
#include <string>
#include <vector>

#include "NotificationManager.h"

#include "core/BaseObject.h"
#include "tools/interfaces/Tickable.h"
#include "network/synchronisable/Synchronisable.h"

namespace orxonox
{

    /**
    @brief
        Container to allow easy handling of the @ref orxonox::Notification "Notifications".

    @ingroup Notifications
    */
    struct NotificationContainer
    {
        Notification* notification; //!< The Notification displayed.
        time_t time; //!< The time the Notification was sent and thus first displayed.
    };

    /**
    @brief
        Struct to allow ordering of @ref orxonox::NotificationContainer "NotificationContainers".

    @ingroup Notifications
    */
    struct NotificationContainerCompare {
        bool operator() (const NotificationContainer* const & a, const NotificationContainer* const & b) const
            { return a->time < b->time; } //!< Ordering by time.
    };

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
    class _NotificationsExport NotificationQueue : public BaseObject, public Tickable, public Synchronisable
    {

        public:
            NotificationQueue(Context* context);
            virtual ~NotificationQueue();

            virtual void tick(float dt); // To update from time to time.
            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);

            virtual void changedName(void);
            
            void update(void); // Updates the NotificationQueue.
            void update(Notification* notification, const std::time_t & time); // Updates the NotificationQueue by adding an new Notification.

            // tolua_begin
            /**
            @brief Get the name of the NotificationQueue.
            @return Returns the name.
            */
            inline const std::string& getName(void) const
                { return this->BaseObject::getName(); }

            void setMaxSize(unsigned int size); // Sets the maximum number of displayed Notifications.
            /**
            @brief Returns the maximum number of Notifications displayed.
            @return Returns maximum size.
            */
            inline unsigned int getMaxSize(void) const
                { return this->maxSize_; }

            void setDisplayTime(int time); // Sets the maximum number of seconds a Notification is displayed.
            /**
            @brief Returns the time interval the Notification is displayed.
            @return Returns the display time.
            */
            inline int getDisplayTime(void) const
                { return this->displayTime_; }
            // tolua_end
            void maxSizeChanged(void); // Is called when the maximum number of displayed Notifications has changed.
            void displayTimeChanged(void);

            /**
            @brief Returns the current number of Notifications displayed.
            @return Returns the size of the NotificationQueue.
            */
            inline unsigned int getSize(void) const
                { return this->size_; }

            /**
            @brief Returns the targets of this NotificationQueue, reps. the senders which Notifications are displayed in this NotificationQueue.
            @return Returns a set of strings holding the different targets.
            */
            inline const std::set<std::string> & getTargetsSet(void)
                { return this->targets_; }

            void setTargets(const std::string & targets); // Set the targets of this NotificationQueue.
            const std::string& getTargets(void) const; // Returns a string consisting of the concatenation of the targets.
            void targetsChanged(void); // Is called when the NotificationQueue's targets have changed.

            /**
            @brief Check whether the NotificationQueue is registered with the NotificationManager.
            @return Returns true if it is registered, false if not.
            */
            inline bool isRegistered(void)
                { return this->registered_; }

            bool tidy(void); // Pops all Notifications from the NotificationQueue.
            
        protected:
            void registerVariables();
            
            /**
            @brief Is called when a notification was pushed.
            @param notification The Notification that was pushed.
            */
            virtual void notificationPushed(Notification* notification) {}
            /**
            @brief Is called when a notification was popped.
            */
            virtual void notificationPopped(void) {}
            /**
            @brief Is called when a notification was removed.
            @param index The index the removed notification was at.
            */
            virtual void notificationRemoved(unsigned int index) {}
            
            virtual void clear(bool noGraphics = false); // Clears the NotificationQueue by removing all NotificationContainers.

        protected:
            static const unsigned int DEFAULT_SIZE = 5; //!< The default maximum number of Notifications displayed.
            static const unsigned int DEFAULT_DISPLAY_TIME = 30; //!< The default display time.
            static const int INF = -1; //!< Constant denoting infinity.

            virtual void create(void); // Creates the NotificationQueue.

        private:
            time_t creationTime_; //!< The time this NotificationQueue was created.
            
            unsigned int maxSize_; //!< The maximal number of Notifications displayed.
            unsigned int size_; //!< The number of Notifications displayed.
            int displayTime_; //!< The time a Notification is displayed.

            bool registered_; //!< Helper variable to remember whether the NotificationQueue is registered already.

            std::set<std::string> targets_; //!< The targets the NotificationQueue displays Notifications of.

            std::multiset<NotificationContainer*, NotificationContainerCompare> ordering_; //!< The NotificationContainers ordered by the time they were registered.
            std::vector<NotificationContainer*> notifications_; //!< The NotificationContainers in the order they were added to the NotificationQueue.

            float tickTime_; //!< Helper variable, to not have to check for Notifications that have been displayed too long, every tick.
            NotificationContainer timeLimit_; //!< Helper object to check against to determine whether Notifications have expired.

            void setName(const std::string& name); //!< Sets the name of the NotificationQueue.

            void push(Notification* notification, const std::time_t & time); // Adds (pushes) a Notification to the NotificationQueue.
            void pop(void); // Removes (pops) the least recently added Notification form the NotificationQueue.
            void remove(const std::multiset<NotificationContainer*, NotificationContainerCompare>::iterator& containerIterator); // Removes the Notification that is stored in the input NotificationContainer.

    };

}

#endif /* _NotificationQueue_H__ */

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
    @file
    @brief Definition of the NotificationQueue class.
*/

#ifndef _NotificationOueue_H__
#define _NotificationOueue_H__

#include "notifications/NotificationsPrereqs.h"

#include <ctime>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "util/Math.h"
#include "core/OrxonoxClass.h"
#include "tools/interfaces/Tickable.h"
#include "interfaces/NotificationListener.h"
#include "NotificationManager.h"

namespace orxonox
{

    //! Container to allow easy handling.
    struct NotificationContainer
    {
        Notification* notification; //!< The Notification displayed by the overlay.
        time_t time; //!< The time the Notification was sent and thus first displayed.
    };

    //! Struct to allow ordering of NotificationOverlayContainers.
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
            NotificationQueue(const std::string& name, const std::string& senders = NotificationManager::ALL, unsigned int size = NotificationQueue::DEFAULT_SIZE, const Vector2& position = NotificationQueue::DEFAULT_POSITION, unsigned int length = NotificationQueue::DEFAULT_LENGTH, unsigned int displayTime = NotificationQueue::DEFAULT_DISPLAY_TIME);
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
            @brief Returns the maximum length in characters a Notification message is allowed to have.
            @return Returns the maximum Notification length.
            */
            inline unsigned int getNotificationLength() const
                { return this->notificationLength_; }
            /**
            @brief Returns the time interval the Notification is displayed.
            @return Returns the display time.
            */
            inline float getDisplayTime() const
                { return this->displayTime_; }
            /**
            @brief Returns the position of the NotificationQueue.
            @return Returns the position.
            */
            inline const Vector2 & getPosition() const
                { return this->position_; }
            /**
            @brief Returns the font size used to display the Notifications.
            @return  Returns the font size.
            */
            inline float getFontSize() const
                { return this->fontSize_; }
            /**
            @brief Returns the font used to display the Notifications.
            @return Returns the font.
            */
            inline const std::string & getFont() const
                { return this->font_; }

            /**
            @brief Returns the targets of this queue, reps. the senders which Notifications are displayed in this queue.
            @return Retuns a set of string holding the different targets.
            */
            inline const std::set<std::string> & getTargetsSet()
                { return this->targets_; }
            bool getTargets(std::string* string) const; //!< Returns a string consisting of the concatination of the targets.

            /**
            @brief Sets the position of the NotificationQueue.
            @param pos The position.
            */
            inline void setPosition(Vector2 pos)
                { this->position_ = pos; this->positionChanged(); }

        private:
            static const unsigned int DEFAULT_SIZE = 5; //!< The default maximum number of Notifications displayed.
            static const unsigned int DEFAULT_LENGTH = 64; //!< The default maximum number of characters displayed.
            static const unsigned int DEFAULT_DISPLAY_TIME = 30; //!< The default display time.
            static const float DEFAULT_FONT_SIZE; //!< The default font size.

            static const std::string DEFAULT_FONT; //!< The default font.
            static const Vector2 DEFAULT_POSITION; //!< the default position.

            std::string name_; //!< The name of the NotificationQueue.

            unsigned int maxSize_; //!< The maximal number of Notifications displayed.
            unsigned int size_; //!< The number of Notifications displayed.
            unsigned int notificationLength_; //!< The maximal number of characters a Notification-message is allowed to have.
            unsigned int displayTime_; //!< The time a Notification is displayed.
            Vector2 position_; //!< The position of the NotificationQueue.

            std::set<std::string> targets_; //!< The targets the Queue displays Notifications of.

            float fontSize_; //!< The font size.
            std::string font_; //!< The font.

            std::multiset<NotificationContainer*, NotificationContainerCompare> ordering_; //!< Multiset, because the ordering is based on, not necessarily unique, timestamps. //TODO: Would set work as well?
            std::vector<NotificationContainer*> notifications_;

            float tickTime_; //!< Helper variable, to not have to check for overlays that have been displayed too long, every tick.
            NotificationContainer timeLimit_; //!< Helper object to check against to determine whether Notifications have expired.

            bool registered_; //!< Helper variable to remember whether the NotificationQueue is registered already.

            void initialize(void); //!< Initializes the object.
            void create(void);

            bool setName(const std::string& name); //!< Sets the name of the NotificationQueue.

            void setMaxSize(unsigned int size); //!< Sets the maximum number of displayed Notifications.
            void setNotificationLength(unsigned int length); //!< Sets the maximum number of characters a Notification message displayed by this queue is allowed to have.
            void setDisplayTime(unsigned int time); //!< Sets the maximum number of seconds a Notification is displayed.

            bool setTargets(const std::string & targets); //!< Set the targets of this queue.

            bool setFontSize(float size); //!< Set the font size.
            bool setFont(const std::string & font); //!< Set the font.

            void positionChanged(void); //!< Aligns all the Notifications to the position of the NotificationQueue.

            void push(Notification* notification, const std::time_t & time); //!< Add a notification to the queue.
            void pop(void);
            void remove(NotificationContainer* container);

            void clear(void); //!< Clear the queue.

    };

}

#endif /* _NotificationOverlay_H__ */

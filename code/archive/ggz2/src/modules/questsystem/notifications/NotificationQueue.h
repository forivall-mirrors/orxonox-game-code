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

#include "questsystem/QuestsystemPrereqs.h"

#include <ctime>
#include <map>
#include <set>
#include <string>

#include "util/Math.h"
#include "tools/interfaces/Tickable.h"
#include "overlays/OverlayGroup.h"
#include "interfaces/NotificationListener.h"

namespace orxonox
{

    //! Container to allow easy handling.
    struct NotificationOverlayContainer
    {
        NotificationOverlay* overlay; //!< Pointer to the NotificationOverlay, everything is about.
        Notification* notification; //!< The Notification displayed by the overlay.
        time_t time; //!< The time the Notification was sent and thus first displayed.
        std::string name; //!< The name of the overlay.
    };

    //! Struct to allow ordering of NotificationOverlayContainers.
    struct NotificationOverlayContainerCompare {
        bool operator() (const NotificationOverlayContainer* const & a, const NotificationOverlayContainer* const & b) const
            { return a->time < b->time; } //!< Ordered by time.
    };

    /**
    @brief
        Displays Notifications from specific senders.
        Beware! The NotificationQueue is an OverlayGruop and thus cannot be be a sub-element of an OverlayGroup (at least no for now.)

        Creating a NotificationQueue through XML goes as follows:
        <NotificationQueue
            name = "SuperQueue" //Name of your OverlayQueue.
            maxSize = "5" //The maximum size of Notifications displayed.
            notificationLength = "64" //The maximum number of characters of a Notification, that are displayed. (Default is 5)
            displayTime = "30" //The time a Notification is displayed in seconds. (Default is 30)
            targets = "target1, target2" //The senders this NotificationQueue displays Notifications from. (all, if all Notifications should be displayed.)
            font = "VeraMono" //The font (Default is VeraMono)
            fontSize = '0.4' //The font size. (Default is 0.025)
            position = "0.0, .0.0" //The position of the NotificationQueue. (Default is 0.0,0.0)
        />
    @author
        Damian 'Mozork' Frick
    */

    class _QuestsystemExport NotificationQueue : public OverlayGroup, public Tickable, public NotificationListener
    {

        public:
            NotificationQueue(BaseObject* creator);
            virtual ~NotificationQueue();

            virtual void XMLPort(Element& xmlElement, XMLPort::Mode mode); //!< Method for creating a NotificationQueue object through XML.

            virtual void tick(float dt); //!< To update from time to time.

            void update(void); //!< Updates the queue.
            void update(Notification* notification, const std::time_t & time); //!< Adds a Notification to the queue.

            /**
            @brief Returns the maximum number of Notifications displayed.
            @return Returns maximum size.
            */
            inline int getMaxSize() const
                { return this->maxSize_; }
            /**
            @brief Returns the current number of Notifications displayed.
            @return Returns the size of the queue.
            */
            inline int getSize() const
                { return this->size_; }
            /**
            @brief Returns the maximum length in characters a Notification message is allowed to have.
            @return Returns the maximum Notification length.
            */
            inline int getNotificationLength() const
                { return this->notificationLength_; }
            /**
            @brief Returns the time interval the Notification is displayed.
            @return Returns the display time.
            */
            inline int getDisplayTime() const
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

            void scroll(const Vector2 pos); //!< Scrolls the NotificationQueue, meaning all NotificationOverlays are moved the input vector.

        private:
            static const int DEFAULT_SIZE = 5; //!< The default maximum number of Notifications displayed.
            static const int DEFAULT_LENGTH = 64; //!< The default maximum number of Notifications displayed.
            static const int DEFAULT_DISPLAY_TIME = 30; //!< The default display time.
            static const float DEFAULT_FONT_SIZE; //!< The default font size.

            static const std::string DEFAULT_FONT; //!< The default font.
            static const Vector2 DEFAULT_POSITION; //!< the default position.

            int maxSize_; //!< The maximal number of Notifications displayed.
            int size_; //!< The number of Notifications displayed.
            int notificationLength_; //!< The maximal number of characters a Notification-message is allowed to have.
            int displayTime_; //!< The time a Notification is displayed.
            Vector2 position_; //!< The position of the NotificationQueue.

            std::set<std::string> targets_; //!< The targets the Queue displays Notifications of.

            float fontSize_; //!< The font size.
            std::string font_; //!< The font.

            std::multiset<NotificationOverlayContainer*, NotificationOverlayContainerCompare> containers_; //!< Multiset, because the ordering is based on, not necessarily unique, timestamps.
            std::map<Notification*, NotificationOverlayContainer*> overlays_; //!< Mapping notifications to their corresponding overlay containers, for easier association and finding.

            float tickTime_; //!< Helper variable, to not have to check for overlays that have been displayed too long, every tick.
            NotificationOverlayContainer timeLimit_; //!< Helper object to check against to determine whether Notifications have expired.

            void initialize(void); //!< Initializes the object.
            void setDefaults(void); //!< Helper method to set the default values.

            bool setMaxSize(int size); //!< Sets the maximum number of displayed Notifications.
            bool setNotificationLength(int length); //!< Sets the maximum number of characters a Notification message displayed by this queue is allowed to have.
            bool setDisplayTime(int time); //!< Sets the maximum number of seconds a Notification is displayed.

            bool setTargets(const std::string & targets); //!< Set the targets of this queue.

            bool setFontSize(float size); //!< Set the font size.
            bool setFont(const std::string & font); //!< Set the font.

            void positionChanged(void); //!< Aligns all the Notifications to the position of the NotificationQueue.

            void addNotification(Notification* notification, const std::time_t & time); //!< Add a notification to the queue.
            bool removeContainer(NotificationOverlayContainer* container); //!< Remove a container from the queue.

            void clear(void); //!< Clear the queue.

    };

}

#endif /* _NotificationOverlay_H__ */

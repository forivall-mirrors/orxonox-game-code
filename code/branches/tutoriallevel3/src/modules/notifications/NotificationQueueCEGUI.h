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
#include "util/Math.h"

#include "NotificationManager.h"

#include "NotificationQueue.h"

namespace orxonox // tolua_export
{ // tolua_export

    /**
    @brief
        Displays @ref orxonox::Notification "Notifications" using CEGUI.

        Apart form the parameters inherited by the @ref orxonox::NotificationQueue "NotificationQueue", there are some more parameters that influence the behavior of the NotificationQueueCEGUI:
        - @b displaySize The size of the window that displays the NotificationQueue.
        - @b position The position if the window that displays the NotificationQueue.
        - @b alignment The horizontal alignment of the displayed Notifications.
        - @b fontSize The font size of the displayed Notifications.
        - @b fontColor The font color of the displayed Notifications.

    @author
        Damian 'Mozork' Frick

    @see NotificationQueue
    @ingroup Notifications
    */
    class _NotificationsExport NotificationQueueCEGUI // tolua_export
        : public NotificationQueue
    { // tolua_export

        public:
            NotificationQueueCEGUI(const std::string& name, const std::string& senders = NotificationListener::ALL, unsigned int size = NotificationQueue::DEFAULT_SIZE, unsigned int displayTime = NotificationQueue::DEFAULT_DISPLAY_TIME);
            virtual ~NotificationQueueCEGUI();

            void destroy(bool noGraphics = false); // Destroys the NotificationQueue.

            /**
            @brief Set the size of the window that displays the NotificationQueue.
            @param size A vector whose first component is the relative width of the window (a value between 0 and 1) and whose second component is the absolute width in pixels (additional to the relative width, can be negative). The height is set such that exactly as many Notifications fit as is the maximum size of the NotificationQueue (in terms of the number of Notifications).
            */
            inline void setDisplaySize(const Vector2& size)
                { this->setDisplaySize(Vector4(size.x, size.y, 0.0, 0.0)); }
            void setDisplaySize(const Vector4& size); // Set the size of the window that displays the NotificationQueue.
            /**
            @brief Get the size of the window that displays the NotificationQueue.
            @return Returns a vector with the display size.
            */
            inline const Vector4& getDisplaySize(void) const
                { return this->displaySize_; }

            void setPosition(const Vector4& position); // Set the position of the window that displays the NotificationQueue.
            /**
            @brief Get the position of the window that displays the NotificationQueue.
            @return Returns a vector with the position.
            */
            inline const Vector4& getPosition(void) const
                { return this->position_; }

            void setAlignment(const std::string& alignment); // Set the horizontal alignment of the Notifications text.
            /**
            @brief Get the horizontal alignment of the Notifications text.
            @return Returns a string with the horizontal alignment property.
            */
            inline const std::string& getAlignment(void)
                { return this->alignment_; }

            void setFontSize(unsigned int size); // Set the font size of the text displayed by this NotificationQueue.
            /**
            @brief Get the font size of the text displayed by this NotificationQueue.
            @return Returns the font size.
            */
            inline unsigned int getFontSize(void)
                { return this->fontSize_; }

            void setFontColor(const Vector4& color); // Set the font color if the text displayed by this NotificationQueue.
            /**
            @brief Get the font color of the text displayed by this NotificationQueue.
            @return Returns a vector with the components being RGBA, with values from 0 to 1.
            */
            inline const Vector4& getFontColor(void) const
                { return this->fontColor_; }
            /**
            @brief Get the font color of the text displayed by this NotificationQueue.
            @return Returns a string with the ARGB values in hexadecimal format.
            */
            inline const std::string& getFontColorStr(void) const
                { return this->fontColorStr_; }

            static NotificationQueueCEGUI* getQueue(const std::string& name); // tolua_export // Get the NotificationQueueCEGUI with the input name.
            
        protected:
            virtual void notificationPushed(Notification* notification); // Is called by the NotificationQueue when a notification was pushed
            virtual void notificationPopped(void); // Is called by the NotificationQueue when a notification was popped.
            virtual void notificationRemoved(unsigned int index); // Is called when a notification was removed.
            
            virtual void clear(bool noGraphics = false); // Clears the NotificationQueue by removing all NotificationContainers.

        protected:
            static const std::string NOTIFICATION_LAYER; //!< The name of the NotificationLayer menu sheet.
            
        private:
            Vector4 displaySize_; //!< The size of the window that displays the NotificationQueue.
            Vector4 position_; //!< The position of the window that displays the NotificationQueue.
            std::string alignment_; //!< The horizontal alignment of the Notifications text.
            unsigned int fontSize_; //!< The font size of the Notifications text.
            Vector4 fontColor_; //!< The font color of the Notifications text as a vector, in RGBA form, with values from 0 to 1.
            std::string fontColorStr_; //!< The font color of the Notifications text as a string with the ARGB hexadecimal values.
            
            void create(void); // Creates the NotificationQueue in lua.

    }; // tolua_export

} // tolua_export

#endif /* _NotificationQueueCEGUI_H__ */

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
    @brief Definition of the NotificationOverlay class.
*/


#ifndef _NotificationOverlay_H__
#define _NotificationOverlay_H__

#include "questsystem/QuestsystemPrereqs.h"

#include <string>
#include "overlays/OverlayText.h"

namespace orxonox
{

    /**
    @brief
        The NotificationOverlay is used to display single Notifications, then bundled in a NotificationQUeue.
    @author
        Damian 'Mozork' Frick
    */
    class _QuestsystemExport NotificationOverlay : public OverlayText
    {

        public:
            NotificationOverlay(BaseObject* creator);
            NotificationOverlay(NotificationQueue* queue, Notification* notification);
            virtual ~NotificationOverlay();

            bool processNotification(Notification* notification); //!< Processes the input Notification.

            /**
            @brief Sets the font size of this overlay's text.
            @param size The font size.
            */
            inline void setFontSize(float size)
                { this->setTextSize(size); }

        protected:
            std::string clipMessage(const std::string & message); //!< Clips the input message if too long.

        private:
            NotificationQueue* queue_; //!< The NotificationQeue this overlay belongs to.
            Notification* notification_; //!< The Notification this overlay displays.

            void initialize(void); //!< Initializes the object.
            void defineOverlay(void); //!< Sets some overlay-specific values.

    };

}

#endif /* _NotificationOverlay_H__ */

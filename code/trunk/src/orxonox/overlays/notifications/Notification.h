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

#ifndef _Notification_H__
#define _Notification_H__

#include "OrxonoxPrereqs.h"

#include <string>

#include "core/BaseObject.h"

namespace orxonox
{
    static const float NOTIFICATION_DISPLAY_TIME = 4.0;

    /**
    @brief
        This is rather temporary, so don't start relying on it, some better version will come soon but the Interface will not likely be the same.
    @author
        Damian 'Mozork' Frick
    */
    class _OrxonoxExport Notification : public BaseObject
    {
    public:
        Notification(BaseObject* creator);
        Notification(BaseObject* creator, const std::string & message, const std::string & title = "", float time = NOTIFICATION_DISPLAY_TIME);
        virtual ~Notification();
        
        bool send(void);
        
        inline bool isSent(void) const
            { return this->sent_; }
        inline const std::string & getTitle(void) const
            { return this->title_; }
        inline const std::string & getMessage(void) const
            { return this->message_; }
        inline const float getDisplayTime(void) const
            { return displayTime_; }
        
        bool setTitle(const std::string & title);
        bool setMessage(const std::string & message);
        bool setDisplayTime(float time);
        
    private:
        std::string title_; //!< The title of the Notification.
        std::string message_; //!< The Notification message.
        float displayTime_; //!< The time duration the Notification is displayed in seconds.
        bool sent_; //!< Whether Notification has been sent, if so it cannot be changed.
        
        void initialize(void);
    };
}

#endif /* _Notification_H__ */

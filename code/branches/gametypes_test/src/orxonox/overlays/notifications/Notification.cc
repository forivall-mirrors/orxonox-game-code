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
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See thes
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

#include "OrxonoxStableHeaders.h"
#include "Notification.h"

#include "core/CoreIncludes.h"

#include "NotificationManager.h"

namespace orxonox
{
    Notification::Notification(BaseObject* creator) : BaseObject(creator)
    {
        RegisterObject(Notification);
    }
    
    Notification::Notification(BaseObject* creator, const std::string & message, const std::string & title, float time) : BaseObject(creator)
    {
        this->title_ = title;
        this->message_ = message;
        if(time > 0)
            this->displayTime_ = time;
    }
    
    Notification::~Notification()
    {
    }
    
    void Notification::initialize(void)
    {
        RegisterObject(Notification);
        
        this->title_ = "";
        this->message_ = "";
        this->displayTime_ = NOTIFICATION_DISPLAY_TIME;
        this->sent_ = false;
    }
    
    bool Notification::send(void)
    {
        bool successful = NotificationManager::insertNotification(this);
        if(successful)
            this->sent_ = true;
        return successful;
    }
    
    bool Notification::setTitle(const std::string & title)
    {
        if(this->isSent())
            return false;
        this->title_ = title;
        return true;
    }
    
    bool Notification::setMessage(const std::string & message)
    {
        if(this->isSent())
            return false;
        this->message_ = message;
        return true;
    }
    
    bool Notification::setDisplayTime(float time)
    {
        if(this->isSent())
        {
            return false;
        }
        if(time > 0)
        {
            this->displayTime_ = time;
            return true;
        }
        return false;
    }
}

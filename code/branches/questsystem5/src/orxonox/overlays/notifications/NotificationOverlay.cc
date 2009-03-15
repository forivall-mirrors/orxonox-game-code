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

#include "OrxonoxStableHeaders.h"
#include "NotificationOverlay.h"

#include <OgreOverlayManager.h>
#include <OgreTextAreaOverlayElement.h>
#include <OgrePanelOverlayElement.h>

#include "core/CoreIncludes.h"
#include "util/Exception.h"

#include "Notification.h"
#include "NotificationQueue.h"

namespace orxonox
{

    NotificationOverlay::NotificationOverlay(BaseObject* creator) : OverlayText(creator)
    {
        this->initialize();
    }

    NotificationOverlay::NotificationOverlay(NotificationQueue* queue, Notification* notification) : OverlayText(this)
    {
        this->initialize();
        
        if(notification == NULL || queue == NULL)
        {
            ThrowException(Argument, "There were NULL-Pointer arguments in NotificationOverlay creation.");
        }

        this->queue_ = queue;
        this->defineOverlay();
        
        this->processNotification(notification);
    }
    
    void NotificationOverlay::initialize(void)
    {
        RegisterObject(NotificationOverlay);
        
        this->queue_ = NULL;
    }
    
    void NotificationOverlay::defineOverlay(void)
    {
        this->setFont(this->queue_->getFont());
        this->setTextSize(this->queue_->getFontSize());

        this->setPosition(this->queue_->getPosition());
    }

    NotificationOverlay::~NotificationOverlay()
    {
    }

    bool NotificationOverlay::processNotification(Notification* notification)
    {
        this->setCaption(clipMessage(notification->getMessage()));
        this->notification_ = notification;
        return true;
    }

    const std::string NotificationOverlay::clipMessage(const std::string & message)
    {
        if(message.length() <= (unsigned int)this->queue_->getNotificationLength())
            return message;
        return message.substr(0, this->queue_->getNotificationLength());
    }

}

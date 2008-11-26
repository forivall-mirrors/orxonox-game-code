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
#include "NotificationQueue.h"

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "core/Super.h"

#include "NotificationManager.h"

namespace orxonox {

    NotificationQueue* NotificationQueue::queue_s;
    
    CreateFactory(NotificationQueue);

    NotificationQueue::NotificationQueue(BaseObject* creator) : OverlayText(creator)
    {
        RegisterObject(NotificationQueue);
        //TDO: Does this work?
        if(queue_s != NULL)
	{
            COUT(2) << "There is now more than one NotificationQueue, this shouldn't happen, since only the first NotificationQueue will be targeted by the NotificationManager." << std::endl;
	}
	else
	{
            queue_s = this;
	}
    }
    
    NotificationQueue::~NotificationQueue()
    {
        
    }
    
    void NotificationQueue::XMLPort(Element& xmlElement, XMLPort::Mode mode)
    {
        SUPER(NotificationQueue, XMLPort, xmlElement, mode);
        
        XMLPortParam(NotificationQueue, "length", setLength, getLength, xmlElement, mode);
    }
    
    void NotificationQueue::tick(float dt)
    {
        NotificationManager::tick(dt);
    
        SUPER(NotificationQueue, tick, dt);
    }
    
    bool NotificationQueue::setLength(int length)
    {
        if(length > 0)
        {
            this->length_ = length;
            return true;
        }
        return false;
    }
    
    void NotificationQueue::setQueueText(const std::string & text)
    {
        this->queueText_ = text;
    }
    
    void NotificationQueue::update(void)
    {
        //TDO UTF string
        this->text_->setCaption(queueText_);
    }

}

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
#include "NotificationManager.h"

#include "core/CoreIncludes.h"

#include "Notification.h"

#include "NotificationQueue.h"

namespace orxonox
{
    std::list<NotificationContainer*> NotificationManager::notifications_s;

    NotificationManager::NotificationManager(BaseObject* creator) : BaseObject(creator)
    {
        RegisterObject(NotificationManager);
    }

    NotificationManager::~NotificationManager()
    {
        //TDO: Destroy the containers
    }

    void NotificationManager::tick(float dt)
    {
        bool update = false;

        for (std::list<NotificationContainer*>::iterator notification = notifications_s.begin(); notification != notifications_s.end(); ++notification)
        {
            NotificationContainer* container = *notification;
            if(container->remainingTime == 0)
            {
                continue;
            }
            else if(container->remainingTime - dt <= 0)
            {
                container->remainingTime = 0;
                update = true;
            }
            else
            {
                container->remainingTime = container->remainingTime -dt;
            }
        }

        if(update)
            updateQueue();
    }

    bool NotificationManager::insertNotification(Notification* notification)
    {
        if(notification == NULL)
            return false;

        NotificationContainer* container = new NotificationContainer;
        container->notification = notification;
        container->remainingTime = notification->getDisplayTime();
        notifications_s.push_front(container);

        updateQueue();

        COUT(4) << "Notification inserted. Title: " << notification->getTitle() << std::endl;

        return true;
    }

    void NotificationManager::updateQueue(void)
    {
        std::string text = "";

        if (!NotificationQueue::queue_s)
            return;

        int i = NotificationQueue::queue_s->getLength();
        for (std::list<NotificationContainer*>::iterator notification = notifications_s.begin(); notification != notifications_s.end() && i > 0; ++notification)
        {
            i--;
            NotificationContainer* container = *notification;
            if(container->remainingTime == 0.0)
                continue;

            text = text + "\n\n\n------------\n\n" + clipMessage(container->notification->getTitle()) + "\n\n" + clipMessage(container->notification->getMessage());
        }

        NotificationQueue::queue_s->setQueueText(text);
    }

    const std::string NotificationManager::clipMessage(const std::string & str)
    {

        std::string message = str;
        unsigned int i = 0;

        unsigned int found = message.find("\\n", i);
        while(found != std::string::npos)
        {
            message.replace(found, 2, "\n");
            i = found+2;
            found = message.find("\\n", i);
        }

        std::string clippedMessage = "";
        int wordLength = 0;
        i = 0;
        int widthLeft = NotificationQueue::queue_s->getWidth();
        while(i < message.length())
        {
            while(i < message.length() && message[i] != ' ' && message[i] != '\n')
            {
                i++;
                wordLength++;
            }

            if(wordLength <= widthLeft)
            {
                clippedMessage = clippedMessage + message.substr(i-wordLength, wordLength);
                if(i < message.length())
                {
                    clippedMessage = clippedMessage + message.substr(i,1);
                }
                widthLeft -= (wordLength+1);
                if(message[i] == '\n')
                {
                    widthLeft = NotificationQueue::queue_s->getWidth() - (wordLength+1);
                }
                wordLength = 0;
                i++;
            }
            else
            {
                clippedMessage.push_back('\n');
                clippedMessage = clippedMessage + message.substr(i-wordLength, wordLength);
                if(i < message.length())
                {
                    clippedMessage = clippedMessage + message.substr(i,1);
                }
                widthLeft = NotificationQueue::queue_s->getWidth() - (wordLength+1);
                i++;
                wordLength = 0;
            }
        }

        return clippedMessage;
    }

}

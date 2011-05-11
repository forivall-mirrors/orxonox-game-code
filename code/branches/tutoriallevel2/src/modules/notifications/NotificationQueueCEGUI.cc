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
    @file NotificationQueueCEGUI.cc
    @brief Implementation of the NotificationQueueCEGUI class.
*/

#include "NotificationQueueCEGUI.h"

#include "core/CoreIncludes.h"
#include "core/GameMode.h"
#include "core/GUIManager.h"
#include "core/LuaState.h"
#include "util/Convert.h"

#include "Notification.h"

namespace orxonox
{

    NotificationQueueCEGUI::NotificationQueueCEGUI(const std::string& name, const std::string& senders, unsigned int size, unsigned int displayTime) : NotificationQueue(name, senders, size, displayTime)
    {
        RegisterObject(NotificationQueueCEGUI);
        
        // Create the NotificationQueueCEGUI in lua.
        this->create();
    }
    
    NotificationQueueCEGUI::~NotificationQueueCEGUI()
    {
        
    }

    /**
    @brief
        Destroys the NotificationQueueCEGUI.
        Used in lua and NotificationManager.
    @param noGraphics
        If this is set to true (false is default), then the queue is not removed in lua. This is used to destroy the queue, after the GUIManager has been destroyed.
    */
    void NotificationQueueCEGUI::destroy(bool noGraphics)
    {
        // Remove the NotificationQueue in lua.
        if(GameMode::showsGraphics() && !noGraphics)
            GUIManager::getInstance().getLuaState()->doString("NotificationLayer.removeQueue(\"" + this->getName() +  "\")");

        NotificationQueue::destroy();
    }
    
    /**
    @brief
        Is called by the NotificationQueue when a notification was pushed.
    @param notification
        The Notification that was pushed.
    */
    void NotificationQueueCEGUI::notificationPushed(Notification* notification)
    {
         // Push the Notification to the GUI.
        if(GameMode::showsGraphics())
            GUIManager::getInstance().getLuaState()->doString("NotificationLayer.pushNotification(\"" + this->getName() + "\", \"" + notification->getMessage() + "\")");
    }
    
    /**
    @brief
        Is called by the NotificationQueue when a notification was popped.
    */
    void NotificationQueueCEGUI::notificationPopped(void)
    {
        // Pops the Notification from the GUI.
        if(GameMode::showsGraphics())
            GUIManager::getInstance().getLuaState()->doString("NotificationLayer.popNotification(\"" + this->getName() + "\")");
    }
    
    /**
    @brief Is called when a notification was removed.
    @param index The index the removed notification was at.
    */
    void NotificationQueueCEGUI::notificationRemoved(unsigned int index)
    {
        // Removes the Notification from the GUI.
        if(GameMode::showsGraphics())
            GUIManager::getInstance().getLuaState()->doString("NotificationLayer.removeNotification(\"" + this->getName() + "\", " + multi_cast<std::string>(index) + ")");
    }
    
    /**
    @brief
        Clears the NotificationQueue by removing all NotificationContainers.
    @param noGraphics
        If this is set to true the GUI is not informed of the clearing of the NotificationQueue. This is needed only internally.
    */
    void NotificationQueueCEGUI::clear(bool noGraphics)
    {
        NotificationQueue::clear(noGraphics);
        
        // Clear the NotificationQueue in the GUI.
        if(GameMode::showsGraphics() && !noGraphics)
            GUIManager::getInstance().getLuaState()->doString("NotificationLayer.clearQueue(\"" + this->getName() + "\")");
    }
    
    /**
    @brief
        Creates the NotificationQueue in lua.
    */
    void NotificationQueueCEGUI::create(void)
    {
        if(GameMode::showsGraphics())
            GUIManager::getInstance().getLuaState()->doString("NotificationLayer.createQueue(\"" + this->getName() +  "\", " + multi_cast<std::string>(this->getMaxSize()) + ")");
    }

}


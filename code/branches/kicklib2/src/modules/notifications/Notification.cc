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

/**
    @file Notification.cc
    @brief Implementation of the Notification class.
*/

#include "Notification.h"

#include "core/CoreIncludes.h"
#include "NotificationManager.h"

namespace orxonox
{

    /**
    @brief
        Constructor. Creates a Notification with the input message and sender.
    @param message
        The message of the Notification.
    @param sender
        The sender of the Notification.
    */
    Notification::Notification(const std::string& message, const std::string& sender)
    {
        RegisterRootObject(Notification);
        this->initialize();
        this->message_ = message;
        this->sender_ = sender;
    }

    /**
    @brief
        Destructor.
    */
    Notification::~Notification()
    {

    }

    /**
    @brief
        Registers the object and sets some default values.
    */
    void Notification::initialize(void)
    {
        this->message_.clear();
        this->sender_ = NotificationManager::NONE;
    }

}

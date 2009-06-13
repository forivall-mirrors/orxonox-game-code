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
    @brief Implementation of the QuestDescription class.
*/


#include "QuestDescription.h"

#include "core/CoreIncludes.h"
#include "QuestNotification.h"

namespace orxonox
{
    CreateFactory(QuestDescription);

    /**
    @brief
        Constructor. Registers and initializes the object.
    */
    QuestDescription::QuestDescription(BaseObject* creator) : BaseObject(creator)
    {
        RegisterObject(QuestDescription);
        
        this->title_ = "";
        this->description_ = "";
    }

    /**
    @brief
        Destructor.
    */
    QuestDescription::~QuestDescription()
    {

    }

    /**
    @brief
        Method for creating a QuestDescription object through XML.
    */
    void QuestDescription::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(QuestDescription, XMLPort, xmlelement, mode);

        XMLPortParam(QuestDescription, "title", setTitle, getTitle, xmlelement, mode);
        XMLPortParam(QuestDescription, "description", setDescription, getDescription, xmlelement, mode);
        XMLPortParam(QuestDescription, "failMessage", setFailMessage, getFailMessage, xmlelement, mode);
        XMLPortParam(QuestDescription, "completeMessage", setCompleteMessage, getCompleteMessage, xmlelement, mode);

        COUT(3) << "New QuestDescription with title '" << this->getTitle() << "' created." << std::endl;
    }
    
    /**
    @brief
        This method is a helper for sending QuestDescriptions as Notifications.
    @param item
        The item the QuestDescription is for.
    @param status
        The status the QuestDescription us for.
    @return
        Returns true if successful.
    @todo
        Make sure the messages meet the conditions.
    */
    bool QuestDescription::notificationHelper(const std::string & item, const std::string & status) const
    {
        std::string message = "";
        if(item == "hint")
        {
            message = "You received a hint: '" + this->title_ + "'";
        }
        else if(item == "quest")
        {
            if(status == "start")
            {
                message = "You received a new quest: '" + this->title_ + "'";
            }
            else if(status == "fail")
            {
                message = "You failed the quest: '" + this->title_ + "'";
            }
            else if(status == "complete")
            {
                message = "You successfully completed the quest: '" + this->title_ + "'";
            }
            else
            {
                COUT(2) << "Bad input in notificationHelper, this should not be happening!" << std::endl;
                return false;
            }
        }
        else
        {
            COUT(2) << "Bad input in notificationHelper, this should not be happening!" << std::endl;
            return false;
        }
        
        QuestNotification* notification = new QuestNotification(message);
        notification->send();
        return true;
    }


}

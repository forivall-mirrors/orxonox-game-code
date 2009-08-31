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
    @brief Definition of the QuestDescription class.
*/

#ifndef _QuestDescription_H__
#define _QuestDescription_H__

#include "questsystem/QuestsystemPrereqs.h"

#include <string>
#include "core/BaseObject.h"

// tolua_begin
namespace orxonox
{
    /**
    @brief
        This class is a description of a QuestItem.
        It holds a title and a description.

        Creating a QuestDescription through XML goes as follows:

        <QuestDescription title="Title" description="Description Text" failMessage="You fail." completeMessage="You win!" />
    @author
        Damian 'Mozork' Frick
    */
    class _QuestsystemExport QuestDescription : public BaseObject
    {
// tolua_end
        public:
            QuestDescription(BaseObject* creator);
            virtual ~QuestDescription();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode); //!< Method for creating a QuestDescription object through XML.

// tolua_begin
            /**
            @brief Returns the title.
            @return Returns a string containing the title of the QuestDescription.
            */
            inline const std::string & getTitle(void) const
                { return this->title_; }

            /**
            @brief Returns the description text.
            @return Returns a string containing the description text of the QuestDescription.
            */
            inline const std::string & getDescription(void) const
                { return this->description_; }
// tolua_end

            /**
            @brief Returns the fail message.
            @return Returns a string containing the fail message of the QuestDescription.
            */
            inline const std::string & getFailMessage(void) const
                { return this->failMessage_; }

            /**
            @brief Returns the complete message.
            @return Returns a string containing the complete message of the QuestDescription.
            */
            inline const std::string & getCompleteMessage(void) const
                { return this->completeMessage_; }

            /**
            @brief Sends a Notification displaying that a QuestHint was added.
            @return Returns true if successful.
            */
            inline bool sendAddHintNotification(void) const
                { return notificationHelper("hint", ""); }

            /**
            @brief Sends a Notification displaying that a Quest was added.
            @return Returns true if successful.
            */
            inline bool sendAddQuestNotification(void) const
                { return notificationHelper("quest", "start"); }

            /**
            @brief Sends a Notification displaying that a Quest was failed.
            @return Returns true if successful.
            */
            inline bool sendFailQuestNotification(void) const
                { return notificationHelper("quest", "fail"); }

            /**
            @brief Sends a Notification displaying that a Quest was completed.
            @return Returns true if successful.
            */
            inline bool sendCompleteQuestNotification(void) const
                { return notificationHelper("quest", "complete"); }

        private:
            std::string title_; //!< The title.
            std::string description_; //!< The description.
            std::string failMessage_; //!< The message displayed when the Quest is failed.
            std::string completeMessage_; //!< The message displayed when the Quest is completed.

            bool notificationHelper(const std::string & item, const std::string & status) const; //!< Helper for sending QuestDescriptions as Notifications.

            /**
            @brief Sets the title.
            @param title The title to be set.
            */
            inline void setTitle(const std::string & title)
                { this->title_ = title; }

            /**
            @brief Sets the description text.
            @param description The description text to be set.
            */
            inline void setDescription(const std::string & description)
                { this->description_ = description; }

            /**
            @brief Sets the fail message.
            @param message The fail message to be set.
            */
            inline void setFailMessage(const std::string & message)
                { this->failMessage_ = message; }

            /**
            @brief Sets the complete message.
            @param message The complete message to be set.
            */
            inline void setCompleteMessage(const std::string & message)
                { this->completeMessage_ = message; }

    }; // tolua_export

} // tolua_export

#endif /* _QuestDescription_H__ */

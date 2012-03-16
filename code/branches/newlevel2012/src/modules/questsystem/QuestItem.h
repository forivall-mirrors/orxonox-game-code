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
    @file QuestItem.h
    @brief Definition of the QuestItem class.
        The @ref orxonox::QuestItem "QuestItem" is the parent class of @ref orxonox::Quest "Quest" and @ref orxonox::QuestHint "QuestHint".
    @ingroup Questsystem
*/


#ifndef _QuestItem_H__
#define _QuestItem_H__

#include "questsystem/QuestsystemPrereqs.h"

#include <string>
#include "core/BaseObject.h"
#include "QuestManager.h"

namespace orxonox
{
    /**
    @brief
        Functions as a base class for quest classes such as @ref orxonox::Quest "Quest" or @ref orxonox::QuestHint "QuestHint".
        Has a unique identifier and a @ref orxonox::QuestDescription "QuestDescription".

    @author
        Damian 'Mozork' Frick

    @ingroup Questsystem
    */
    class _QuestsystemExport QuestItem : public BaseObject
    {

        public:
            QuestItem(BaseObject* creator);
            virtual ~QuestItem();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode); //!< Method for creating a QuestItem object through XML.

            /**
            @brief Returns the id of this QuestItem.
            @return Returns the id of the QuestItem.
            */
            inline const std::string & getId(void) const
                { return this->id_; }

            /**
            @brief Returns the QuestDescription of the QuestItem.
            @return Returns a pointer to the QuestDescription object of the QuestItem.
            */
            inline QuestDescription* getDescription(void) const
                { return this->description_; }

            /**
            @brief Check whether the QuestItem is registered with the QuestManager.
            @return Returns true if the QuestItem is registered with the QuestManager.
            */
            inline bool isRegistered(void)
                { return this->registered_; }
            /**
            @brief Set the QuestItem as being registered with the QuestManager.
            */
            inline void setRegistered(void)
                { this->registered_ = true; }

        protected:
            void setId(const std::string & id); //!< Sets the id of the QuestItem.

            /**
            @brief Sets the description of the QuestItem.
            @param description The QuestDescription to be set.
            */
            inline void setDescription(QuestDescription* description)
                { this->description_ = description; }

        private:
            std::string id_; //!< Identifier. Must be unique.
            QuestDescription* description_; //!< The QuestDescription of the QuestItem.

            bool registered_; //!< Whether the QuestItem is registered with the QuestManager.

    };

}

#endif /* _QuestItem_H__ */

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
    @brief Definition of the QuestItem class.
        The QuestItem is the parent class of Quest and QuestHint.
*/


#ifndef _QuestItem_H__
#define _QuestItem_H__

#include "questsystem/QuestsystemPrereqs.h"

#include <string>
#include "core/BaseObject.h"

namespace orxonox
{
    /**
    @brief
        Functions as a base class for quest classes such as Quest or QuestHint.
        Has a unique identifier and a description.
    @author
        Damian 'Mozork' Frick
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
            inline const QuestDescription* getDescription(void) const
                { return this->description_; }

            static bool isId(const std::string & id); //!< Checks whether a given id is valid.

        protected:
            void setId(const std::string & id); //!< Sets the id of the QuestItem.

            /**
            @brief Sets the description of the QuestItem.
            @param description The QuestDescription to be set.
            */
            inline void setDescription(QuestDescription* description)
                { this->description_ = description; }

        private:
            std::string id_; //!< Identifier. Should be of GUID form: http://en.wikipedia.org/wiki/Globally_Unique_Identifier#Basic_structure
            QuestDescription* description_; //!< The QuestDescription of the QuestItem.

    };

}

#endif /* _QuestItem_H__ */

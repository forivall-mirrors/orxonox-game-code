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
    @file QuestDescription.h
    @brief
    Definition of the QuestDescription class.
*/

#ifndef _QuestDescription_H__
#define _QuestDescription_H__

#include "OrxonoxPrereqs.h"

#include <string>

#include "core/BaseObject.h"
#include "core/XMLPort.h"

namespace orxonox {

    /**
    @brief
        This class is a description of a QuestItem.
        It holds a title and a description.
        
        Creating a QuestDescription through XML goes as follows:
        
        <QuestDescription title="Title" description="Description Text" />
    @author
        Damian 'Mozork' Frick
    */
    class _OrxonoxExport QuestDescription : public BaseObject {

        public:
            QuestDescription(BaseObject* creator);
            virtual ~QuestDescription();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode); //!< Method for creating a QuestDescription object through XML.

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

        private:
            std::string title_; //!< The title.
            std::string description_; //!< The description.

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

    };

}

#endif /* _QuestDescription_H__ */

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
    @file QuestDescription.cc
    @brief
    Implementation of the QuestDescription class.
*/

#include "OrxonoxStableHeaders.h"

#include "QuestDescription.h"

#include "core/CoreIncludes.h"

namespace orxonox {

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

        COUT(3) << "New QuestDescription with title '" << this->getTitle() << "' created." << std::endl;
    }


}

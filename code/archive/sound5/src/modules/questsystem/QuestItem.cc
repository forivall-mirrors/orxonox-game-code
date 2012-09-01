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
    @brief Implementation of the QuestItem class.
*/

#include "QuestItem.h"

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "QuestDescription.h"

namespace orxonox
{
    /**
    @brief
        Constructor. Registers and initializes the object.
    */
    QuestItem::QuestItem(BaseObject* creator) : BaseObject(creator)
    {
        RegisterObject(QuestItem);
    }

    /**
    @brief
        Destructor.
    */
    QuestItem::~QuestItem()
    {

    }

    /**
    @brief
        Method for creating a QuestItem object through XML.
    */
    void QuestItem::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(QuestItem, XMLPort, xmlelement, mode);

        XMLPortParam(QuestItem, "id", setId, getId, xmlelement, mode);
        XMLPortObject(QuestItem, QuestDescription, "", setDescription, getDescription, xmlelement, mode);
    }

    /**
    @brief
        Sets the id of the QuestItem.
    The id must be of GUID form. See 'http://en.wikipedia.org/wiki/Globally_Unique_Identifier#Basic_structure' for more information.
    @param id
        The id to be set.
    */
    void QuestItem::setId(const std::string & id)
    {
        if(!isId(id)) //!< Checks whether the id is a valid id.
        {
            COUT(2) << "Invalid id. QuestItem id {" << id << "} could not be set." << std::endl;
            return;
        }

        this->id_ = id;
    }

    /**
    @brief
        Checks whether an input id is of the required form.
    @param id
        The id to be checked.
    @return
        Returns true if the string is likely to be of the required form.
    */
    /*static*/ bool QuestItem::isId(const std::string & id)
    {
        return id.size() >= 32;
    }

}

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
    @file QuestItem.cc
    @brief Implementation of the QuestItem class.
*/

#include "QuestItem.h"

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"

#include "QuestDescription.h"

namespace orxonox
{

    CreateUnloadableFactory(QuestItem);

    /**
    @brief
        Constructor. Registers and initializes the object.
    */
    QuestItem::QuestItem(Context* context) : BaseObject(context)
    {
        this->registered_ = false;

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
        The id can be any string and must be unique in the context it is used (commonly a level file). To ensure uniqueness one could use GUIDs, however they are in general less readable, so make your own choice.
    @see
        http://en.wikipedia.org/wiki/Globally_Unique_Identifier#Basic_structure
    @param id
        The id to be set.
    */
    void QuestItem::setId(const std::string & id)
    {
        if(id == "") // Checks whether the id is a valid id.
        {
            orxout(internal_warning, context::quests) << "Invalid id. QuestItem id {" << id << "} could not be set." << endl;
            return;
        }

        this->id_ = id;
    }

}

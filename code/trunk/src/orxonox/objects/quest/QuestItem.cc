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

#include "core/CoreIncludes.h"

#include "QuestItem.h"

namespace orxonox {

    QuestItem::QuestItem(BaseObject* creator) : BaseObject(creator)
    {
        RegisterObject(QuestItem);

        this->initialize();
    }

    /**
    @brief
        Destructor.
    */
    QuestItem::~QuestItem()
    {

    }

    void QuestItem::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(QuestItem, XMLPort, xmlelement, mode);

        XMLPortParam(QuestItem, "id", setId, getId, xmlelement, mode);
        //Doesn't getDescription have to be of type getDescription(unsigned int) ?
        //XMLPortObjectTemplate(QuestItem, QuestDescription, "", setDescription, getDescription, xmlelement, mode, unsigned int);
        XMLPortObject(QuestItem, QuestDescription, "", setDescription, getDescription, xmlelement, mode);

    }


    /**
    @brief
        Initializes the object.
        Should be called first in every constructor of this class.
    */
    void QuestItem::initialize(void)
    {
        RegisterObject(QuestItem);

        this->id_ = "";
    }

    void QuestItem::setId(const std::string & id)
    {
        if(!isId(id))
        {
            COUT(2) << "Invalid id. QuestItem id {" << id << "} could not be set." << std::endl;
            return;
        }
        this->id_ = id;
    }

    //const QuestDescription* QuestItem::getDescription(unsigned int index) const //!< Returns the description of the QuestItem.
    //{
    //    if(index != 0)
    //        return NULL;
    //    return this->description_;
    //}

    /**
    @brief
        Checks whether an input id is of the required form.
    @param id
        The id to be checked.
    @return
        Returns true if the string is likely to be of the required form.
    @todo
        Clarify form, more vigorous checks.
    */
    bool QuestItem::isId(const std::string & id)
    {
        return id.size() >= 32;
    }

}

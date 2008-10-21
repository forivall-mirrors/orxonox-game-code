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

    CreateFactory(QuestItem);
    
    /**
    @brief
        Constructor. Needs as input a unique identifier to be able to identify different instances of this class (and subclasses).
    @param id
        The unique identifier. Should be of GUID form: http://en.wikipedia.org/wiki/Globally_Unique_Identifier#Basic_structure
    @param title
        The title of this QuestItem. Has an empty string as default.
    @param description
        The description of this QuestItem. Has an empty string as default.
    */
    QuestItem::QuestItem(std::string id, std::string title = "", std::string description = "") : BaseObject()
    {
        this->initialize();
        
        this->id_ = id;
        this->description_ = QuestDescription(title, description);
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
        Initializes the object.
        Should be called first in every constructor of this class.
    */
    void QuestItem::initialize(void)
    {
        RegisterObject(QuestItem);
        
        this->id_ = "";
    }

}

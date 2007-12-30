/*
 *   ORXONOX - the hottest 3D action shooter ever to exist
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
 *      ...
 *   Co-authors:
 *      ...
 *
 */

/*!
    @file BaseObject.cc
    @brief Implementation of the BaseObject class.
*/

#include "tinyxml/tinyxml.h"

#include "BaseObject.h"

namespace orxonox
{
    CreateFactory(BaseObject);

    /**
        @brief Constructor: Registers the object in the BaseObject-list.
    */
    BaseObject::BaseObject()
    {
        RegisterRootObject(BaseObject);
    }

    /**
        @brief Destructor
    */
    BaseObject::~BaseObject()
    {
    }

    /**
        @brief load general xml paramters
    */
    void BaseObject::loadParams(TiXmlElement* xmlElem)
    {
        if (xmlElem->Attribute("name"))
        {
            this->setName(xmlElem->Attribute("name"));
        }
    }
}

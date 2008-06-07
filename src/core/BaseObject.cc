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
 *      Fabian 'x3n' Landau
 *   Co-authors:
 *      ...
 *
 */

/**
    @file
    @brief Implementation of the BaseObject class.
*/

#include "BaseObject.h"
#include "tinyxml/tinyxml.h"
#include "CoreIncludes.h"
#include "XMLPort.h"
#include "Level.h"

namespace orxonox
{
    CreateFactory(BaseObject);

    /**
        @brief Constructor: Registers the object in the BaseObject-list.
    */
    BaseObject::BaseObject() :
        bInitialized_(false),
        bActive_(true),
        bVisible_(true),
        level_(0),
        namespace_(0)
    {
        RegisterRootObject(BaseObject);

        this->bInitialized_ = true;
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

    /**
        @brief XML loading and saving.
        @param xmlelement The XML-element
        @param loading Loading (true) or saving (false)
        @return The XML-element
    */
    void BaseObject::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        XMLPortParam(BaseObject, "name", setName, getName, xmlelement, mode);
    }

    /**
        @brief Returns the levelfile that loaded this object.
        @return The levelfile
    */
    const std::string& BaseObject::getLevelfile() const
    {
        return this->level_->getFile();
    }
}

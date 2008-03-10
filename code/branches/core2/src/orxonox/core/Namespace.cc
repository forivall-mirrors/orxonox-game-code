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
 *      Fabian 'x3n' Landau
 *   Co-authors:
 *      ...
 *
 */

#include "Namespace.h"
#include "CoreIncludes.h"
#include "XMLPort.h"

namespace orxonox
{
    CreateFactory(Namespace);

    Namespace::Namespace()
    {
        RegisterObject(Namespace);
    }

    Namespace::~Namespace()
    {
    }

    /**
        @brief XML loading and saving.
        @param xmlelement The XML-element
        @param loading Loading (true) or saving (false)
        @return The XML-element
    */
    void Namespace::XMLPort(Element& xmlelement, bool loading)
    {
        BaseObject::XMLPort(xmlelement, loading);

        XMLPortObject(Namespace, BaseObject, "", loadObjects, saveObjects, xmlelement, loading);
    }

    void Namespace::loadObjects(BaseObject* object)
    {
        object->setNamespace(this);
    }

    const BaseObject* Namespace::saveObjects(unsigned int index) const
    {
        return 0; // todo
    }
}

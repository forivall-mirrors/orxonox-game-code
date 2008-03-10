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

        this->representingNamespace_ = this;
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

        XMLPortObject(Namespace, BaseObject, "", loadObjects, saveObjects, xmlelement, loading, true, false);
    }

    void Namespace::loadObjects(BaseObject* object)
    {
        object->setNamespace(this->representingNamespace_);

        if (object->isA(Class(Namespace)))
            this->addSubnamespace((Namespace*)object);
    }

    void Namespace::addSubnamespace(Namespace* ns)
    {
        std::string name = ns->getName().substr(ns->getName().find("::"

        if (!this->hasSubnamespace(ns->getName()))
            this->namespaces_[ns->getName()] = ns;
        else
            ns->representingNamespace_ = object;
    }

    bool Namespace::hasSubnamespace(const std::string& name) const
    {
        return (this->namespaces_.find(name) != this->namespaces_.end());
    }

    const BaseObject* Namespace::saveObjects(unsigned int index) const
    {
        return 0; // todo
    }
}

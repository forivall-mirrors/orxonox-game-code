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

#include "Namespace.h"
#include "NamespaceNode.h"
#include "CoreIncludes.h"
#include "XMLPort.h"
#include "util/SubString.h"

namespace orxonox
{
    CreateFactory(Namespace);

    Namespace::Namespace() :
      bAutogeneratedFileRootNamespace_(false),
      bRoot_(false),
      operator_("or")
    {
        RegisterObject(Namespace);
    }

    Namespace::~Namespace()
    {
        if (this->bRoot_)
            for (std::set<NamespaceNode*>::iterator it = this->representingNamespaces_.begin(); it != this->representingNamespaces_.end(); ++it)
                delete (*it);
    }

    /**
        @brief XML loading and saving.
        @param xmlelement The XML-element
        @param loading Loading (true) or saving (false)
        @return The XML-element
    */
    void Namespace::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(Namespace, XMLPort, xmlelement, mode);

        std::string name = this->getName();
        unsigned int pos = 0;
        while ((pos = name.find(',')) != std::string::npos)
            name.replace(pos, 1, " ");
        while ((pos = name.find(';')) != std::string::npos)
            name.replace(pos, 1, " ");
        while ((pos = name.find('\n')) != std::string::npos)
            name.replace(pos, 1, " ");
        while ((pos = name.find('\t')) != std::string::npos)
            name.replace(pos, 1, " ");
        SubString tokens(name, " ", "", false, '\\', true, '"', true, '\0', '\0', true, '\0');
        if (this->bRoot_)
        {
            this->representingNamespaces_.insert(new NamespaceNode(this->getName()));
        }
        else
        {
            for (unsigned int i = 0; i < tokens.size(); i++)
            {
                for (std::set<NamespaceNode*>::iterator it = this->getNamespace()->representingNamespaces_.begin(); it != this->getNamespace()->representingNamespaces_.end(); ++it)
                {
                    std::set<NamespaceNode*> temp = (*it)->getNodeRelative(tokens[i]);
                    this->representingNamespaces_.insert(temp.begin(), temp.end());
                }
            }
        }

        XMLPortParam(Namespace, "operator", setOperator, getOperator, xmlelement, mode);
        XMLPortParam(Namespace, "bAutogenerated", setAutogenerated, isAutogenerated, xmlelement, mode);

        if (this->bAutogeneratedFileRootNamespace_)
        {
            for (std::set<NamespaceNode*>::iterator it = this->representingNamespaces_.begin(); it != this->representingNamespaces_.end(); ++it)
            {
                (*it)->setRoot(true);
                (*it)->setHidden(true);
            }
        }

        XMLPortObject(Namespace, BaseObject, "", loadObjects, saveObjects, xmlelement, mode, true, false);
    }

    void Namespace::loadObjects(BaseObject* object)
    {
        object->setNamespace(this);
    }

    const BaseObject* Namespace::saveObjects(unsigned int index) const
    {
        return 0; // todo
    }

    bool Namespace::includes(const Namespace* ns) const
    {
        for (std::set<NamespaceNode*>::const_iterator it1 = this->representingNamespaces_.begin(); it1 != this->representingNamespaces_.end(); ++it1)
        {
            for (std::set<NamespaceNode*>::const_iterator it2 = ns->representingNamespaces_.begin(); it2 != ns->representingNamespaces_.end(); ++it2)
            {
                if ((*it1)->includes(*it2))
                {
                    if (this->operator_ == "or")
                        return true;

                    if (this->operator_ == "not")
                        return false;
                }
                else
                {
                    if (this->operator_ == "and")
                        return false;
                }
            }
        }

        if (this->operator_ == "or")
            return false;
        else if (this->operator_ == "and")
            return true;
        else if (this->operator_ == "not")
            return true;

        return false;
    }

    std::string Namespace::toString() const
    {
        std::string output;

        int i = 0;
        for (std::set<NamespaceNode*>::const_iterator it = this->representingNamespaces_.begin(); it != this->representingNamespaces_.end(); i++, ++it)
        {
            if (i > 0)
                output += " / ";

            output += (*it)->toString();
        }

        return output;
    }

    std::string Namespace::toString(const std::string& indentation) const
    {
        std::string output;

        int i = 0;
        for (std::set<NamespaceNode*>::const_iterator it = this->representingNamespaces_.begin(); it != this->representingNamespaces_.end(); i++, ++it)
        {
            if (i > 0)
                output += "\n";

            output += (*it)->toString(indentation);
        }

        return output;
    }
}

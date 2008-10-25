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

#include "Template.h"

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "util/Debug.h"
#include "tinyxml/ticpp.h"

namespace orxonox
{
    CreateFactory(Template);

    Template::Template() : xmlelement_("")
    {
        RegisterObject(Template);

        this->bIsLink_ = false;
        this->bIsReturningXMLElement_ = false;
        this->baseclassIdentifier_ = 0;
    }

    Template::~Template()
    {
        Template::getTemplateMap().erase(this->getName());
    }

    void Template::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(Template, XMLPort, xmlelement, mode);

        XMLPortParam(Template, "link", setLink, getLink, xmlelement, mode);
        XMLPortParam(Template, "baseclass", setBaseclass, getBaseclass, xmlelement, mode);

        this->setXMLElement(*dynamic_cast<TiXmlElement*>(xmlelement.FirstChildElement(false)->GetTiXmlPointer()));
    }

    void Template::changedName()
    {
        if (this->getName() != "")
        {
            std::map<std::string, Template*>::iterator it;
            it = Template::getTemplateMap().find(this->getOldName());
            if (it != Template::getTemplateMap().end())
                Template::getTemplateMap().erase(it);

            it = Template::getTemplateMap().find(this->getName());
            if (it != Template::getTemplateMap().end())
                COUT(2) << "Warning: Template with name \"" << this->getName() << "\" already exists." << std::endl;
            else
                Template::getTemplateMap()[this->getName()] = this;
        }
    }

    const TiXmlElement& Template::getXMLElement() const
    {
        if (this->bIsLink_)
        {
            Template* temp = Template::getTemplate(this->link_);
            if (temp)
            {
                if (!temp->bIsReturningXMLElement_)
                {
                    this->bIsReturningXMLElement_ = true;
                    const TiXmlElement& element = temp->getXMLElement();
                    this->bIsReturningXMLElement_ = false;
                    return element;
                }
                else
                {
                    COUT(2) << "Warning: Linking from " << this->getName() << " to " << this->link_ << " leads to an infinite loop. Returning own element." << std::endl;
                }
            }
            else
            {
                COUT(2) << "Warning: " << this->link_ << " is not an existing Template name. Returning own element." << std::endl;
            }
        }

        return this->xmlelement_;
    }

    void Template::setBaseclass(const std::string& baseclass)
    {
        this->baseclassIdentifier_ = ClassByString(baseclass);
        if (this->baseclassIdentifier_)
            this->baseclass_ = baseclass;
    }

    void Template::applyOn(BaseObject* object)
    {
        if (this->baseclassIdentifier_)
        {
            if (!object->isA(this->baseclassIdentifier_))
            {
                COUT(1) << "Error: Can't apply template (name: " << this->getName() << "), object (name: " << object->getName() << ", class: " << object->getIdentifier()->getName() << ") is not a " << this->baseclassIdentifier_->getName() << std::endl;
                return;
            }
        }

        COUT(4) << object->getLoaderIndentation() << " aplying Template \"" << this->getName() << "\"..." << std::endl;

        Element temp = ((TiXmlElement*)&this->getXMLElement());
        object->XMLPort(temp, XMLPort::LoadObject);
    }

    std::map<std::string, Template*>& Template::getTemplateMap()
    {
        static std::map<std::string, Template*> templateMap;
        return templateMap;
    }

    Template* Template::getTemplate(const std::string& name)
    {
        std::map<std::string, Template*>::iterator it = Template::getTemplateMap().find(name);
        if (it != Template::getTemplateMap().end())
            return it->second;
        else
            return 0;
    }

    void Template::apply(const std::string& name, BaseObject* object)
    {
        std::map<std::string, Template*>::iterator it = Template::getTemplateMap().find(name);
        if (it != Template::getTemplateMap().end())
            it->second->applyOn(object);
    }
}

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

#include <tinyxml/tinyxml.h>
#include <tinyxml/ticpp.h>

#include "util/Output.h"
#include "CoreIncludes.h"
#include "XMLPort.h"

namespace orxonox
{
    CreateFactory(Template);

    Template::Template(BaseObject* creator) : BaseObject(creator)
    {
        this->xmlelement_ = new TiXmlElement("");

        RegisterObject(Template);

        this->bIsLink_ = false;
        this->bLoadDefaults_ = true;
        this->bIsReturningXMLElement_ = false;
        this->baseclassIdentifier_ = 0;
    }

    Template::~Template()
    {
        Template::getTemplateMap().erase(this->getName());
        delete this->xmlelement_;
    }

    void Template::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(Template, XMLPort, xmlelement, mode);

        XMLPortParam(Template, "link",      setLink,         getLink,         xmlelement, mode);
        XMLPortParam(Template, "baseclass", setBaseclass,    getBaseclass,    xmlelement, mode);
        XMLPortParam(Template, "defaults",  setLoadDefaults, getLoadDefaults, xmlelement, mode).defaultValues(true);

        Element* element = xmlelement.FirstChildElement(false);
        if (element)
        {
            TiXmlElement* tixmlelement = static_cast<TiXmlElement*>(element->GetTiXmlPointer());
            if (tixmlelement)
                this->setXMLElement(*tixmlelement);
        }
    }

    void Template::changedName()
    {
        SUPER(Template, changedName);

        if (!this->getName().empty())
        {
            std::map<std::string, Template*>::iterator it;
            it = Template::getTemplateMap().find(this->getOldName());
            if (it != Template::getTemplateMap().end())
                Template::getTemplateMap().erase(it);

            it = Template::getTemplateMap().find(this->getName());
            if (it != Template::getTemplateMap().end())
                orxout(internal_warning, context::templates) << "Template with name \"" << this->getName() << "\" already exists." << endl;
            else
                Template::getTemplateMap()[this->getName()] = this;
        }
    }

    void Template::setXMLElement(const TiXmlElement& xmlelement)
    {
        *this->xmlelement_ = xmlelement;
    }

    const TiXmlElement& Template::getXMLElement()
    {
        if (this->bIsLink_)
        {
            Template* temp = Template::getTemplate(this->link_);
            if (temp)
            {
                this->bLoadDefaults_ = temp->bLoadDefaults_;

                if (!temp->bIsReturningXMLElement_)
                {
                    this->bIsReturningXMLElement_ = true;
                    const TiXmlElement& element = temp->getXMLElement();
                    this->bIsReturningXMLElement_ = false;
                    return element;
                }
                else
                {
                    orxout(internal_warning, context::templates) << "Linking from \"" << this->getName() << "\" to \"" << this->link_ << "\" leads to an infinite loop. Returning own element." << endl;
                }
            }
            else
            {
                orxout(internal_warning, context::templates) << '"' << this->link_ << "\" is not an existing Template name. Returning own element." << endl;
            }
        }

        return *this->xmlelement_;
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
                orxout(internal_error, context::templates) << "Can't apply template (name: " << this->getName() << "), object (name: " << object->getName() << ", class: " << object->getIdentifier()->getName() << ") is not a " << this->baseclassIdentifier_->getName() << endl;
                return;
            }
        }

        orxout(verbose, context::templates) << object->getLoaderIndentation() << " aplying Template \"" << this->getName() << "\"..." << endl;

        Element temp = &const_cast<TiXmlElement&>(this->getXMLElement());

        if (this->bLoadDefaults_)
            object->XMLPort(temp, XMLPort::LoadObject);
        else
            object->XMLPort(temp, XMLPort::ExpandObject);
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
        {
            orxout(internal_warning, context::templates) << "Template with name " << name << " doesn't exist." << endl;
            return 0;
        }
    }

    void Template::apply(const std::string& name, BaseObject* object)
    {
        std::map<std::string, Template*>::iterator it = Template::getTemplateMap().find(name);
        if (it != Template::getTemplateMap().end())
            it->second->applyOn(object);
    }
}

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
#include "XMLFile.h"
#include "Template.h"
#include "util/String.h"

namespace orxonox
{
    CreateFactory(BaseObject);

    /**
        @brief Constructor: Registers the object in the BaseObject-list.
    */
    BaseObject::BaseObject(BaseObject* creator) : bInitialized_(false)
    {
        RegisterRootObject(BaseObject);

        this->bInitialized_ = true;

        this->bActive_ = true;
        this->bVisible_ = true;
        this->oldGametype_ = 0;

        this->setCreator(creator);
        if (this->creator_)
        {
            this->setFile(this->creator_->getFile());
            this->setNamespace(this->creator_->getNamespace());
            this->setScene(this->creator_->getScene());
            this->setGametype(this->creator_->getGametype());
        }
        else
        {
            this->file_ = 0;
            this->namespace_ = 0;
            this->scene_ = 0;
            this->gametype_ = 0;
        }
    }

    /**
        @brief Destructor
    */
    BaseObject::~BaseObject()
    {
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
        XMLPortParam(BaseObject, "visible", setVisible, isVisible, xmlelement, mode);
        XMLPortParam(BaseObject, "active", setActive, isActive, xmlelement, mode);

        XMLPortObjectTemplate(BaseObject, Template, "templates", addTemplate, getTemplate, xmlelement, mode, Template*);
    }

    /**
        @brief Returns the levelfile that loaded this object.
        @return The levelfile
    */
    const std::string& BaseObject::getFilename() const
    {
        if (this->file_)
            return this->file_->getFilename();
        else
            return BLANKSTRING;
    }

    /**
        @brief Adds a Template to the object.
        @param name The name of the Template
    */
    void BaseObject::addTemplate(const std::string& name)
    {
        Template* temp = Template::getTemplate(name);
        if (temp)
            this->addTemplate(temp);
        else
            COUT(1) << "Error: \"" << name << "\" is not a valid Template name (in class: " << this->getIdentifier()->getName() << ", name: " << this->getName() << ")." << std::endl;
    }

    /**
        @brief Adds a Template to the object.
        @param temp The Template
    */
    void BaseObject::addTemplate(Template* temp)
    {
        this->templates_.insert(temp);
        temp->applyOn(this);
    }

    /**
        @brief Returns the Template with the given index.
        @param index The index
    */
    Template* BaseObject::getTemplate(unsigned int index) const
    {
        unsigned int i = 0;
        for (std::set<Template*>::const_iterator it = this->templates_.begin(); it != this->templates_.end(); ++it)
        {
            if (i == index)
                return (*it);
            i++;
        }
        return 0;
    }
}

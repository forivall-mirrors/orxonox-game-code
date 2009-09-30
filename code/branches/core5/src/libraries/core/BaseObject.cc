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

#include <tinyxml/tinyxml.h>

#include "util/StringUtils.h"
#include "CoreIncludes.h"
#include "Event.h"
#include "EventIncludes.h"
#include "Functor.h"
#include "Iterator.h"
#include "Template.h"
#include "XMLFile.h"
#include "XMLNameListener.h"
#include "XMLPort.h"
#include "network/NetworkPrereqs.h"

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

        this->lastLoadedXMLElement_ = 0;

        this->functorSetMainState_ = 0;
        this->functorGetMainState_ = 0;

        this->setCreator(creator);
        if (this->creator_)
        {
            this->setFile(this->creator_->getFile());
            this->setNamespace(this->creator_->getNamespace());
            this->setScene(this->creator_->getScene(), this->creator_->getSceneID());
            this->setGametype(this->creator_->getGametype());
        }
        else
        {
            this->file_ = 0;
            this->namespace_ = 0;
            this->scene_ = 0;
            this->sceneID_ = OBJECTID_UNKNOWN;
            this->gametype_ = 0;
        }
    }

    /**
        @brief Destructor
    */
    BaseObject::~BaseObject()
    {
        if (this->isInitialized())
        {
            for (std::list<BaseObject*>::const_iterator it = this->events_.begin(); it != this->events_.end(); ++it)
                (*it)->unregisterEventListener(this);

            for (std::map<BaseObject*, std::string>::const_iterator it = this->eventListeners_.begin(); it != this->eventListeners_.end(); ++it)
                it->first->removeEvent(this);

            for (std::map<std::string, EventContainer*>::const_iterator it = this->eventContainers_.begin(); it != this->eventContainers_.end(); ++it)
                delete it->second;

            if (this->functorSetMainState_)
                delete this->functorSetMainState_;
            if (this->functorGetMainState_)
                delete this->functorGetMainState_;
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
        XMLPortParam(BaseObject, "name", setXMLName, getName, xmlelement, mode);
        XMLPortParam(BaseObject, "visible", setVisible, isVisible, xmlelement, mode);
        XMLPortParam(BaseObject, "active", setActive, isActive, xmlelement, mode);
        XMLPortParam(BaseObject, "mainstate", setMainStateName, getMainStateName, xmlelement, mode);

        XMLPortObjectTemplate(BaseObject, Template, "templates", addTemplate, getTemplate, xmlelement, mode, Template*);

        Element* events = xmlelement.FirstChildElement("events", false);

        if (events)
        {
            std::list<std::string> eventnames;

            if (mode == XMLPort::LoadObject || mode == XMLPort::ExpandObject)
            {
                for (ticpp::Iterator<ticpp::Element> child = events->FirstChildElement(false); child != child.end(); child++)
                    eventnames.push_back(child->Value());
            }
            else if (mode == XMLPort::SaveObject)
            {
                for (std::map<std::string, XMLPortObjectContainer*>::const_iterator it = this->getIdentifier()->getXMLPortEventMapBegin(); it != this->getIdentifier()->getXMLPortEventMapEnd(); ++it)
                    eventnames.push_back(it->first);
            }

            for (std::list<std::string>::iterator it = eventnames.begin(); it != eventnames.end(); ++it)
            {
                std::string sectionname = (*it);
                ExecutorMember<BaseObject>* loadexecutor = createExecutor(createFunctor(&BaseObject::addEvent), std::string( "BaseObject" ) + "::" + "addEvent");
                ExecutorMember<BaseObject>* saveexecutor = createExecutor(createFunctor(&BaseObject::getEvent), std::string( "BaseObject" ) + "::" + "getEvent");
                loadexecutor->setDefaultValue(1, sectionname);

                XMLPortClassObjectContainer<BaseObject, BaseObject>* container = 0;
                container = static_cast<XMLPortClassObjectContainer<BaseObject, BaseObject>*>(this->getIdentifier()->getXMLPortEventContainer(sectionname));
                if (!container)
                {
                    container = new XMLPortClassObjectContainer<BaseObject, BaseObject>(sectionname, this->getIdentifier(), loadexecutor, saveexecutor, false, true);
                    this->getIdentifier()->addXMLPortEventContainer(sectionname, container);
                }
                container->port(this, *events, mode);
            }
        }
    }

    /**
        @brief Loads the name of the object through XML and calls all XMLNameListener.
        @param name The name of the object
    */
    void BaseObject::setXMLName(const std::string& name)
    {
        this->setName(name);

        for (ObjectList<XMLNameListener>::iterator it = ObjectList<XMLNameListener>::begin(); it != ObjectList<XMLNameListener>::end(); ++it)
            it->loadedNewXMLName(this);
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

    void BaseObject::addEvent(BaseObject* event, const std::string& sectionname)
    {
        event->registerEventListener(this, sectionname);
        this->events_.push_back(event);
    }

    void BaseObject::removeEvent(BaseObject* event)
    {
        this->events_.remove(event);
    }

    BaseObject* BaseObject::getEvent(unsigned int index) const
    {
        unsigned int i = 0;
        for (std::list<BaseObject*>::const_iterator it = this->events_.begin(); it != this->events_.end(); ++it)
        {
            if (i == index)
                return (*it);
            ++i;
        }
        return 0;
    }

    void BaseObject::addEventContainer(const std::string& sectionname, EventContainer* container)
    {
        std::map<std::string, EventContainer*>::const_iterator it = this->eventContainers_.find(sectionname);
        if (it != this->eventContainers_.end())
        {
            COUT(2) << "Warning: Overwriting EventContainer in class " << this->getIdentifier()->getName() << "." << std::endl;
            delete (it->second);
        }

        this->eventContainers_[sectionname] = container;
    }

    EventContainer* BaseObject::getEventContainer(const std::string& sectionname) const
    {
        std::map<std::string, EventContainer*>::const_iterator it = this->eventContainers_.find(sectionname);
        if (it != this->eventContainers_.end())
            return ((*it).second);
        else
            return 0;
    }

    void BaseObject::fireEvent()
    {
        this->fireEvent(true);
        this->fireEvent(false);
    }

    void BaseObject::fireEvent(bool activate)
    {
        this->fireEvent(activate, this);
    }

    void BaseObject::fireEvent(bool activate, BaseObject* originator)
    {
        Event event(activate, originator);

        for (std::map<BaseObject*, std::string>::iterator it = this->eventListeners_.begin(); it != this->eventListeners_.end(); ++it)
        {
            event.sectionname_ = it->second;
            it->first->processEvent(event);
        }
    }

    void BaseObject::fireEvent(Event& event)
    {
        for (std::map<BaseObject*, std::string>::iterator it = this->eventListeners_.begin(); it != this->eventListeners_.end(); ++it)
            it->first->processEvent(event);
    }

    void BaseObject::processEvent(Event& event)
    {
        ORXONOX_SET_EVENT(BaseObject, "activity", setActive, event);
        ORXONOX_SET_EVENT(BaseObject, "visibility", setVisible, event);
    }

    void BaseObject::setMainStateName(const std::string& name)
    {
        if (this->mainStateName_ != name)
        {
            this->mainStateName_ = name;
            if (this->functorSetMainState_)
                delete this->functorSetMainState_;
            if (this->functorGetMainState_)
                delete this->functorGetMainState_;
            this->changedMainState();
            if (!this->functorSetMainState_)
                COUT(2) << "Warning: \"" << name << "\" is not a valid MainState." << std::endl;
        }
    }

    void BaseObject::setMainState(bool state)
    {
        if (this->functorSetMainState_)
            (*this->functorSetMainState_)(state);
        else
            COUT(2) << "Warning: No MainState defined in object \"" << this->getName() << "\" (" << this->getIdentifier()->getName() << ")" << std::endl;
    }

    bool BaseObject::getMainState() const
    {
        if (this->functorGetMainState_)
        {
            (*this->functorGetMainState_)();
            return this->functorGetMainState_->getReturnvalue();
        }
        else
        {
            COUT(2) << "Warning: No MainState defined in object \"" << this->getName() << "\" (" << this->getIdentifier()->getName() << ")" << std::endl;
            return false;
        }
    }

    void BaseObject::changedMainState()
    {
        SetMainState(BaseObject, "activity",   setActive,  isActive);
        SetMainState(BaseObject, "visibility", setVisible, isVisible);
    }
}

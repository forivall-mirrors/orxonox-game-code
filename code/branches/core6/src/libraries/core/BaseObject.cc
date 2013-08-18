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

#include "CoreIncludes.h"
#include "Event.h"
#include "EventIncludes.h"
#include "Template.h"
#include "XMLFile.h"
#include "XMLNameListener.h"
#include "XMLPort.h"
#include "command/Functor.h"
#include "object/Iterator.h"

#include "class/OrxonoxInterface.h" // we include this only to include OrxonoxInterface.h at least once in core to keep MSVC happy...

namespace orxonox
{
    RegisterClass(BaseObject);

    /**
        @brief Constructor: Registers the object in the BaseObject-list.
    */
    BaseObject::BaseObject(Context* context) : bInitialized_(false)
    {
        RegisterObject(BaseObject);

        this->bInitialized_ = true;

        this->bActive_ = true;
        this->bVisible_ = true;
        this->oldGametype_ = 0;
        this->bRegisteredEventStates_ = false;

        this->lastLoadedXMLElement_ = 0;

        this->mainStateFunctor_ = 0;

        if (context)
            this->setContext(context);

        BaseObject* creator = orxonox_cast<BaseObject*>(context);
        this->setCreator(creator);
        if (this->creator_)
        {
            this->setFile(this->creator_->getFile());
            this->setNamespace(this->creator_->getNamespace());
            this->setScene(this->creator_->getScene(), this->creator_->getSceneID());
            this->setGametype(this->creator_->getGametype());
            this->setLevel(this->creator_->getLevel());
        }
        else
        {
            this->file_ = 0;
            this->namespace_ = 0;
            this->scene_ = 0;
            this->sceneID_ = OBJECTID_UNKNOWN;
            this->gametype_ = 0;
            this->level_ = 0;
        }
    }

    /**
        @brief Destructor
    */
    BaseObject::~BaseObject()
    {
        if (this->isInitialized())
        {
            for (std::map<BaseObject*, std::string>::const_iterator it = this->eventSources_.begin(); it != this->eventSources_.end(); )
                this->removeEventSource((it++)->first);

            for (std::set<BaseObject*>::const_iterator it = this->eventListeners_.begin(); it != this->eventListeners_.end(); )
                (*(it++))->removeEventSource(this);

            for (std::map<std::string, EventState*>::const_iterator it = this->eventStates_.begin(); it != this->eventStates_.end(); ++it)
                delete it->second;
        }
    }

    /** @brief Adds an object which listens to the events of this object. */
    void BaseObject::registerEventListener(BaseObject* object)
    {
        orxout(verbose, context::events) << "New EventListener: " << object->getIdentifier()->getName() << " &(" << object << ")." << endl;
        this->eventListeners_.insert(object);
    }

    /**
        @brief XML loading and saving.
        @param xmlelement The XML-element
        @param mode The mode defines the operation that is being executed: loading or saving the object (from or to XML respectively)
    */
    void BaseObject::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        XMLPortParam(BaseObject, "name", setXMLName, getName, xmlelement, mode);
        XMLPortParam(BaseObject, "visible", setVisible, isVisible, xmlelement, mode);
        XMLPortParam(BaseObject, "active", setActive, isActive, xmlelement, mode);
        XMLPortParam(BaseObject, "mainstate", setMainStateName, getMainStateName, xmlelement, mode);
        XMLPortParamTemplate(BaseObject, "template", addTemplate, getSingleTemplate, xmlelement, mode, const std::string&);

        XMLPortObjectTemplate(BaseObject, Template, "templates", addTemplate, getTemplate, xmlelement, mode, Template*);
        XMLPortObject(BaseObject, BaseObject, "eventlisteners", addEventListener, getEventListener, xmlelement, mode);

        Element* events = 0;
        if (mode == XMLPort::LoadObject || mode == XMLPort::ExpandObject)
            events = xmlelement.FirstChildElement("events", false);
        else if (mode == XMLPort::SaveObject)
            {}
        if (events)
            this->XMLEventPort(*events, mode);
    }

    /**
        @brief Defines the possible event states of this object and parses eventsources from an XML file.
        @param xmlelement The XML-element
        @param mode The mode defines the operation that is being executed: loading or saving the object (from or to XML respectively)
    */
    void BaseObject::XMLEventPort(Element& xmlelement, XMLPort::Mode mode)
    {
        XMLPortEventState(BaseObject, BaseObject, "activity", setActive, xmlelement, mode);
        XMLPortEventState(BaseObject, BaseObject, "visibility", setVisible, xmlelement, mode);
        XMLPortEventState(BaseObject, BaseObject, "mainstate", setMainState, xmlelement, mode);

        this->bRegisteredEventStates_ = true;
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
            orxout(internal_error) << "\"" << name << "\" is not a valid Template name (in class: " << this->getIdentifier()->getName() << ", name: " << this->getName() << ")." << endl;
    }

    /**
        @brief Adds a Template to the object.
        @param temp The Template
    */
    void BaseObject::addTemplate(Template* temp)
    {
        // network
        if (temp->isLink())
        {
            this->networkTemplateNames_.insert(temp->getLink());

            Template* link;
            assert(!(link = Template::getTemplate(temp->getLink())) || !link->isLink());
            link = NULL;
        }
        else
            this->networkTemplateNames_.insert(temp->getName());

        // add template
        this->templates_.insert(temp);
        temp->applyOn(this);
    }

    /**
        @brief Returns the name of the first Template.
        @return The name as string.
    */
    const std::string& BaseObject::getSingleTemplate(void) const
    {
        if(this->templates_.empty())
            return BLANKSTRING;

        return (*this->templates_.begin())->getName();
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

    /**
        @brief Adds a new event source for a specific state.
        @param source The object which sends events to this object
        @param state The state of this object which will be affected by the events
    */
    void BaseObject::addEventSource(BaseObject* source, const std::string& state)
    {
        this->eventSources_[source] = state;
        source->registerEventListener(this);
    }

    /**
        @brief Removes an eventsource (but doesn't unregister itself at the source).
    */
    void BaseObject::removeEventSource(BaseObject* source)
    {
        this->eventSources_.erase(source);
        source->unregisterEventListener(this);
    }

    /**
        @brief Returns an eventsource with a given index.
    */
    BaseObject* BaseObject::getEventSource(unsigned int index, const std::string& state) const
    {
        unsigned int i = 0;
        for (std::map<BaseObject*, std::string>::const_iterator it = this->eventSources_.begin(); it != this->eventSources_.end(); ++it)
        {
            if (it->second != state)
                continue;

            if (i == index)
                return it->first;
            ++i;
        }
        return 0;
    }

    /**
        @brief Adds an object which listens to the events of this object. The events are sent to the other objects mainstate.
    */
    void BaseObject::addEventListener(BaseObject* listener)
    {
        this->eventListenersXML_.insert(listener);
        listener->addEventSource(this, "mainstate");
    }

    /**
        @brief Returns an event listener with a given index.
    */
    BaseObject* BaseObject::getEventListener(unsigned int index) const
    {
        unsigned int i = 0;
        for (std::set<BaseObject*>::const_iterator it = this->eventListenersXML_.begin(); it != this->eventListenersXML_.end(); ++it)
        {
            if (i == index)
                return *it;
            ++i;
        }
        return 0;
    }

    /**
        @brief Adds a new event-state to the object. Event-states are states which can be changed by events.
        @param name  The name of the event
        @param state The object containing information about the event-state
    */
    void BaseObject::addEventState(const std::string& name, EventState* state)
    {
        std::map<std::string, EventState*>::const_iterator it = this->eventStates_.find(name);
        if (it != this->eventStates_.end())
        {
            orxout(internal_warning, context::events) << "Overwriting EventState in class " << this->getIdentifier()->getName() << '.' << endl;
            delete (it->second);
        }

        this->eventStates_[name] = state;
    }

    /**
        @brief Returns the event-state with the given name.
    */
    EventState* BaseObject::getEventState(const std::string& name) const
    {
        std::map<std::string, EventState*>::const_iterator it = this->eventStates_.find(name);
        if (it != this->eventStates_.end())
            return (it->second);
        else
            return 0;
    }

    /**
        @brief Fires an event (without a state).
    */
    void BaseObject::fireEvent(const std::string& name)
    {
        this->fireEvent(true, name);
        this->fireEvent(false, name);
    }

    /**
        @brief Fires an event which activates or deactivates a state.
    */
    void BaseObject::fireEvent(bool activate, const std::string& name)
    {
        this->fireEvent(activate, this, name);
    }

    /**
        @brief Fires an event which activates or deactivates a state with agiven originator (the object which triggered the event).
    */
    void BaseObject::fireEvent(bool activate, BaseObject* originator, const std::string& name)
    {
        Event event(activate, originator, name);

        for (std::set<BaseObject*>::iterator it = this->eventListeners_.begin(); it != this->eventListeners_.end(); ++it)
        {
            event.statename_ = (*it)->eventSources_[this];
            (*it)->processEvent(event);
        }
    }

    /**
        @brief Fires an event, using the Event struct.
    */
    void BaseObject::fireEvent(Event& event)
    {
        for (std::set<BaseObject*>::iterator it = this->eventListeners_.begin(); it != this->eventListeners_.end(); ++it)
            (*it)->processEvent(event);
    }

    /**
        @brief Processing an event by calling the right main state.
        @param event The event struct which contains the information about the event
    */
    void BaseObject::processEvent(Event& event)
    {
        this->registerEventStates();

        orxout(verbose, context::events) << this->getIdentifier()->getName() << " (&" << this << ") processing event. originator: " << event.originator_->getIdentifier()->getName() << " (&" << event.originator_ << "), activate: " << event.activate_ << ", name: " << event.name_ << ", statename: " << event.statename_ << "." << endl;

        std::map<std::string, EventState*>::const_iterator it = this->eventStates_.find(event.statename_);
        if (it != this->eventStates_.end())
            it->second->process(event, this);
        else if (!event.statename_.empty())
            orxout(internal_warning, context::events) << "\"" << event.statename_ << "\" is not a valid state in object \"" << this->getName() << "\" of class " << this->getIdentifier()->getName() << "." << endl;
        else
            orxout(internal_warning, context::events) << "Event with invalid source sent to object \"" << this->getName() << "\" of class " << this->getIdentifier()->getName() << "." << endl;
    }

    /**
        @brief Sets the main state of the object to a given boolean value.

        Note: The main state of an object can be set with the @ref setMainStateName function.
        It's part of the eventsystem and used for event forwarding (when the target object can't specify a specific state,
        the main state is used by default).
    */
    void BaseObject::setMainState(bool state)
    {
        if (this->mainStateFunctor_)
        {
            if (this->mainStateFunctor_->getParamCount() == 0)
            {
                if (state)
                    (*this->mainStateFunctor_)();
            }
            else
            {
                (*this->mainStateFunctor_)(state);
            }
        }
        else
            orxout(internal_warning, context::events) << "No MainState defined in object \"" << this->getName() << "\" (" << this->getIdentifier()->getName() << ")" << endl;
    }

    /**
        @brief This function gets called if the main state name of the object changes.
    */
    void BaseObject::changedMainStateName()
    {
        this->mainStateFunctor_ = 0;

        if (!this->mainStateName_.empty())
        {
            this->registerEventStates();

            std::map<std::string, EventState*>::const_iterator it = this->eventStates_.find(this->mainStateName_);
            if (it != this->eventStates_.end() && it->second->getFunctor())
            {
                if (it->second->getFunctor()->getParamCount() <= 1)
                    this->mainStateFunctor_ = it->second->getFunctor();
                else
                    orxout(internal_warning, context::events) << "Can't use \"" << this->mainStateName_ << "\" as MainState because it needs a second argument." << endl;
            }
            else
                orxout(internal_warning, context::events) << "\"" << this->mainStateName_ << "\" is not a valid MainState." << endl;
        }
    }

    /**
        @brief Calls XMLEventPort with an empty XML-element to register the event states if necessary.
    */
    void BaseObject::registerEventStates()
    {
        if (!this->bRegisteredEventStates_)
        {
            Element xmlelement;
            this->XMLEventPort(xmlelement, XMLPort::NOP);
        }
    }

    /**
        @brief Manually loads all event states, even if the class doesn't officially support them. This is needed by some classes like @ref EventDispatcher or @ref EventTarget.
    */
    void BaseObject::loadAllEventStates(Element& xmlelement, XMLPort::Mode mode, BaseObject* object, Identifier* identifier)
    {
        Element* events = xmlelement.FirstChildElement("events", false);
        if (events)
        {
            // get the list of all states present
            std::list<std::string> eventnames;
            if (mode == XMLPort::LoadObject || mode == XMLPort::ExpandObject)
            {
                for (ticpp::Iterator<ticpp::Element> child = events->FirstChildElement(false); child != child.end(); child++)
                    eventnames.push_back(child->Value());
            }
            else if (mode == XMLPort::SaveObject)
            {
            }

            // iterate through all states and get the event sources
            for (std::list<std::string>::iterator it = eventnames.begin(); it != eventnames.end(); ++it)
            {
                const std::string& statename = (*it);

                // if the event state is already known, continue with the next state
                orxonox::EventState* eventstate = object->getEventState(statename);
                if (eventstate)
                    continue;

                XMLPortClassObjectContainer<BaseObject, BaseObject>* container = (XMLPortClassObjectContainer<BaseObject, BaseObject>*)(identifier->getXMLPortObjectContainer(statename));
                if (!container)
                {
                    const ExecutorMemberPtr<BaseObject>& setfunctor = createExecutor(createFunctor(&BaseObject::addEventSource), std::string( "BaseObject" ) + "::" + "addEventSource" + '(' + statename + ')');
                    const ExecutorMemberPtr<BaseObject>& getfunctor = createExecutor(createFunctor(&BaseObject::getEventSource), std::string( "BaseObject" ) + "::" + "getEventSource" + '(' + statename + ')');
                    setfunctor->setDefaultValue(1, statename);
                    getfunctor->setDefaultValue(1, statename);

                    container = new XMLPortClassObjectContainer<BaseObject, BaseObject>(statename, identifier, setfunctor, getfunctor, false, true);
                    identifier->addXMLPortObjectContainer(statename, container);
                }
                container->port(object, *events, mode);
            }
        }
    }
}

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

#include "EventListener.h"

#include "core/CoreIncludes.h"
#include "core/Event.h"
#include "core/XMLPort.h"

namespace orxonox
{
    RegisterClass(EventListener);

    EventListener::EventListener(Context* context) : BaseObject(context)
    {
        RegisterObject(EventListener);

        this->bActive_ = false;
    }

    EventListener::~EventListener()
    {
    }

    void EventListener::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(EventListener, XMLPort, xmlelement, mode);

        XMLPortParam(EventListener, "event", setEventName, getEventName, xmlelement, mode);
    }

    void EventListener::processEvent(Event& event)
    {
        if (this->bActive_)
        {
            orxout(internal_warning, context::events) << "Detected Event loop in EventListener \"" << this->getName() << '"' << endl;
            return;
        }

        orxout(verbose, context::events) << "EventListener, processing event: originator: " << event.originator_->getIdentifier()->getName() << " (&" << event.originator_ << ")" << ", activate: " << event.activate_ << ", name: " << event.name_ << endl;

        this->bActive_ = true;
        //TODO: Why not this->fireEvent(event)?
        this->fireEvent(event.activate_, event.originator_, event.name_);
        this->bActive_ = false;
    }

    void EventListener::setEventName(const std::string& eventname)
    {
        this->eventName_ = eventname;

        if (this->eventName_.empty())
            return;

        for (ObjectList<BaseObject>::iterator it = ObjectList<BaseObject>::begin(); it != ObjectList<BaseObject>::end(); ++it)
            if (it->getName() == this->eventName_)
                this->addEventSource(*it, "");
    }

    void EventListener::loadedNewXMLName(BaseObject* object)
    {
        if (this->eventName_.empty())
            return;

        if (object->getName() == this->eventName_)
            this->addEventSource(object, "");
    }
}

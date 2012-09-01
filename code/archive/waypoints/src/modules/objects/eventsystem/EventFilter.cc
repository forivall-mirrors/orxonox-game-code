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

#include "EventFilter.h"

#include "core/CoreIncludes.h"
#include "core/Event.h"
#include "core/XMLPort.h"
#include "EventName.h"

namespace orxonox
{
    CreateFactory(EventFilter);

    EventFilter::EventFilter(BaseObject* creator) : BaseObject(creator)
    {
        RegisterObject(EventFilter);

        this->bActive_ = false;
    }

    EventFilter::~EventFilter()
    {
        for (std::list<EventName*>::const_iterator it = this->names_.begin(); it != this->names_.end(); )
            (*(it++))->destroy();
    }

    void EventFilter::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(EventFilter, XMLPort, xmlelement, mode);

        XMLPortObject(EventFilter, BaseObject, "", addFilterSource, getFilterSource, xmlelement, mode);
        XMLPortObject(EventFilter, EventName, "names", addEventName, getEventName, xmlelement, mode);
    }

    void EventFilter::processEvent(Event& event)
    {
        if (this->bActive_)
        {
            orxout(internal_warning, context::events) << "Detected Event loop in EventFilter \"" << this->getName() << '"' << endl;
            return;
        }

        if (this->names_.size() > 0)
        {
            bool success = false;
            for (std::list<EventName*>::const_iterator it = this->names_.begin(); it != this->names_.end(); ++it)
            {
                if ((*it)->getName() == event.name_)
                {
                    success = true;
                    break;
                }
            }
            if (!success)
                return;
        }

        this->bActive_ = true;
        this->fireEvent(event.activate_, event.originator_, event.name_);
        this->bActive_ = false;
    }

    void EventFilter::addFilterSource(BaseObject* source)
    {
        this->sources_.push_back(source);
        this->addEventSource(source, "");
    }

    BaseObject* EventFilter::getFilterSource(unsigned int index) const
    {
        unsigned int i = 0;
        for (std::list<BaseObject*>::const_iterator it = this->sources_.begin(); it != this->sources_.end(); ++it)
        {
            if (i == index)
                return (*it);
            ++i;
        }
        return 0;
    }

    void EventFilter::addEventName(EventName* eventname)
    {
        this->names_.push_back(eventname);
    }

    EventName* EventFilter::getEventName(unsigned int index) const
    {
        unsigned int i = 0;
        for (std::list<EventName*>::const_iterator it = this->names_.begin(); it != this->names_.end(); ++it)
        {
            if (i == index)
                return (*it);
            ++i;
        }
        return 0;
    }
}

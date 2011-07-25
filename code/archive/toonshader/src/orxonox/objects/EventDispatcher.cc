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

#include "OrxonoxStableHeaders.h"
#include "EventDispatcher.h"

#include "core/CoreIncludes.h"
#include "core/EventIncludes.h"
#include "core/XMLPort.h"
#include "core/EventIncludes.h"
#include "EventTarget.h"

namespace orxonox
{
    CreateFactory(EventDispatcher);

    EventDispatcher::EventDispatcher(BaseObject* creator) : BaseObject(creator)
    {
        RegisterObject(EventDispatcher);
    }

    EventDispatcher::~EventDispatcher()
    {
        if (this->isInitialized())
            for (std::list<EventTarget*>::iterator it = this->targets_.begin(); it != this->targets_.end(); ++it)
                delete (*it);
    }

    void EventDispatcher::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(EventDispatcher, XMLPort, xmlelement, mode);

        XMLPortObject(EventDispatcher, EventTarget, "targets", addTarget, getTarget, xmlelement, mode);
    }

    void EventDispatcher::processEvent(Event& event)
    {
        for (std::list<EventTarget*>::iterator it = this->targets_.begin(); it != this->targets_.end(); ++it)
            (*it)->fireEvent(event);
    }

    void EventDispatcher::addTarget(EventTarget* target)
    {
        this->targets_.push_back(target);
    }

    EventTarget* EventDispatcher::getTarget(unsigned int index) const
    {
        unsigned int i = 0;
        for (std::list<EventTarget*>::const_iterator it = this->targets_.begin(); it != this->targets_.end(); ++it)
        {
            if (i == index)
                return (*it);
            ++i;
        }
        return 0;
    }
}

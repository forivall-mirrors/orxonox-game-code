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

#include "EventDispatcher.h"

#include "core/CoreIncludes.h"
#include "core/EventIncludes.h"
#include "core/XMLPort.h"

namespace orxonox
{
    CreateFactory(EventDispatcher);

    EventDispatcher::EventDispatcher(Context* context) : BaseObject(context)
    {
        RegisterObject(EventDispatcher);
    }

    EventDispatcher::~EventDispatcher()
    {
        if (this->isInitialized())
            for (std::list<BaseObject*>::iterator it = this->targets_.begin(); it != this->targets_.end(); ++it)
                (*it)->destroy();
    }

    void EventDispatcher::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(EventDispatcher, XMLPort, xmlelement, mode);

        XMLPortObject(EventDispatcher, BaseObject, "targets", addTarget, getTarget, xmlelement, mode);

        // since we need event sources mapped to any state, we have to parse XML by ourselves
        this->loadAllEventStates(xmlelement, mode, this, Class(EventDispatcher));
    }

    void EventDispatcher::processEvent(Event& event)
    {
        for (std::list<BaseObject*>::iterator it = this->targets_.begin(); it != this->targets_.end(); ++it)
            (*it)->processEvent(event);
    }

    void EventDispatcher::addTarget(BaseObject* target)
    {
        this->targets_.push_back(target);
    }

    BaseObject* EventDispatcher::getTarget(unsigned int index) const
    {
        unsigned int i = 0;
        for (std::list<BaseObject*>::const_iterator it = this->targets_.begin(); it != this->targets_.end(); ++it)
        {
            if (i == index)
                return (*it);
            ++i;
        }
        return 0;
    }
}

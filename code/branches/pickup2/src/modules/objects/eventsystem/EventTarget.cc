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

#include "EventTarget.h"
#include "core/CoreIncludes.h"
#include "core/XMLPort.h"

namespace orxonox
{
    CreateFactory(EventTarget);

    EventTarget::EventTarget(BaseObject* creator) : BaseObject(creator)
    {
        RegisterObject(EventTarget);

        this->bActive_ = false;
    }

    EventTarget::~EventTarget()
    {
    }

    void EventTarget::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(EventTarget, XMLPort, xmlelement, mode);

        XMLPortParam(EventTarget, "target", setTargetName, getTargetName, xmlelement, mode);

        // since we need event sources mapped to any state, we have to parse XML by ourselves
        this->loadAllEventStates(xmlelement, mode, this, Class(EventTarget));
    }

    void EventTarget::processEvent(Event& event)
    {
        if (this->bActive_)
        {
            COUT(2) << "Warning: Detected Event loop in EventTarget \"" << this->getName() << '"' << std::endl;
            return;
        }

        this->bActive_ = true;
        this->fireEvent(event);
        this->bActive_ = false;
    }

    void EventTarget::setTargetName(const std::string& name)
    {
        this->target_ = name;

        for (ObjectList<BaseObject>::iterator it = ObjectList<BaseObject>::begin(); it != ObjectList<BaseObject>::end(); ++it)
            if (it->getName() == this->target_)
                this->addEventTarget(*it);
    }

    void EventTarget::loadedNewXMLName(BaseObject* object)
    {
        if (this->target_.empty())
            return;

        if (object->getName() == this->target_)
            this->addEventTarget(object);
    }

    void EventTarget::addEventTarget(BaseObject* object)
    {
        if (object != static_cast<BaseObject*>(this))
            object->addEventSource(this, "");
    }
}

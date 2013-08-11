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
    @file EventTrigger.cc
    @brief Implementation of the EventTrigger class.
    @ingroup NormalTrigger
*/

#include "EventTrigger.h"

#include "core/CoreIncludes.h"
#include "core/EventIncludes.h"

namespace orxonox
{
    RegisterClass(EventTrigger);

    /**
    @brief
        Constructor. Registers and initializes the object.
    @param creator
        The creator of the EventTrigger.
    */
    EventTrigger::EventTrigger(Context* context) : Trigger(context)
    {
        RegisterObject(EventTrigger);

        this->bEventTriggered_ = false;
    }

    /**
    @brief
        Destructor.
    */
    EventTrigger::~EventTrigger()
    {
    }

    /**
    @brief
        Creates an event port.
    */
    void EventTrigger::XMLEventPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(EventTrigger, XMLEventPort, xmlelement, mode);

        XMLPortEventState(EventTrigger, BaseObject, "trigger", trigger, xmlelement, mode);
    }

    /**
    @brief
        Check whether the EventTrigger should be triggered.
        It should be triggered if it is triggered according just to its sub-triggers and if the last event that came in was an event that changed from not triggered to triggered.
    */
    bool EventTrigger::isTriggered(TriggerMode::Value mode)
    {
        if (Trigger::isTriggered(mode))
            return this->bEventTriggered_;
        else
            return false;
    }
}

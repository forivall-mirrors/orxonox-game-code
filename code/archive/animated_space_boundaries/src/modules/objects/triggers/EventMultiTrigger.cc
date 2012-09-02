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
 *      Damian 'Mozork' Frick
 *   Co-authors:
 *      ...
 *
 */

/**
    @file EventMultiTrigger.cc
    @brief Implementation of the EventMultiTrigger class.
    @ingroup MultiTrigger
*/

#include "EventMultiTrigger.h"

#include "core/CoreIncludes.h"
#include "core/EventIncludes.h"
#include "core/XMLPort.h"
#include "MultiTriggerContainer.h"

namespace orxonox
{

    CreateFactory(EventMultiTrigger);

    /**
    @brief
        Constructor. Registers the object.
    */
    EventMultiTrigger::EventMultiTrigger(BaseObject* creator) : MultiTrigger(creator)
    {
        RegisterObject(EventMultiTrigger);
    }

    /**
    @brief
        Destructor.
    */
    EventMultiTrigger::~EventMultiTrigger()
    {

    }

    /**
    @brief
        Method for creating an EventMultiTrigger object through XML.
    */
    void EventMultiTrigger::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(EventMultiTrigger, XMLPort, xmlelement, mode);

        this->setBroadcast(true);
    }

    /**
    @brief
        Creates an event port.
    */
    void EventMultiTrigger::XMLEventPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(EventMultiTrigger, XMLEventPort, xmlelement, mode);

        XMLPortEventState(EventMultiTrigger, BaseObject, "trigger", trigger, xmlelement, mode);
    }

    /**
    @brief
        Method that causes the EventMultiTrigger to trigger upon receiving an event.
    @param bTriggered
        Whether the event is a triggering or an un-triggering event.
    @param originator
        A pointer to the entity the event originates from.
    */
    void EventMultiTrigger::trigger(bool bTriggered, BaseObject* originator)
    {
        // If the originator is a MultiTriggerContainer, the event originates from a MultiTrigger and thus the event only triggers the EventMultiTrigger for the originator that caused the MultiTrigger to trigger.
        if(originator != NULL && originator->isA(ClassIdentifier<MultiTriggerContainer>::getIdentifier()))
        {
            MultiTriggerContainer* container = static_cast<MultiTriggerContainer*>(originator);
            // If the entity that triggered the MultiTrigger is no target of this EventMultiTrigger we process it as it weren't an event caused by a MultiTrigger.
            // But if it is the EventMultiTrigger only triggers for the entity tha caused the MultiTrigger to trigger.
            if(this->isTarget(container->getData()))
            {
                if(this->isTriggered(container->getData()) ^ bTriggered)
                    this->changeTriggered(container->getData());

                return;
            }
        }

        // If we don't know who exactly caused the event we just send a broadcast.
        if(this->isTriggered() ^ bTriggered)
            this->changeTriggered();
    }

}


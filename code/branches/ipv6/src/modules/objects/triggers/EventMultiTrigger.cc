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
*/

#include "EventMultiTrigger.h"

#include "core/CoreIncludes.h"
#include "core/EventIncludes.h"
#include "core/XMLPort.h"

namespace orxonox
{

    CreateFactory(EventMultiTrigger);

    EventMultiTrigger::EventMultiTrigger(BaseObject* creator) : MultiTrigger(creator)
    {
        RegisterObject(EventMultiTrigger);

        this->bEventTriggered_ = false;
    }

    EventMultiTrigger::~EventMultiTrigger()
    {

    }

    void EventMultiTrigger::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(EventMultiTrigger, XMLPort, xmlelement, mode);

        this->setBroadcast(true);
    }

    void EventMultiTrigger::XMLEventPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(EventMultiTrigger, XMLEventPort, xmlelement, mode);

        XMLPortEventState(EventMultiTrigger, BaseObject, "trigger", trigger, xmlelement, mode);
    }

}


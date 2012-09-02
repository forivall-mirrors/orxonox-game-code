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
    @file SimpleNotification.cc
    @brief Implementation of the SimpleNotification class.
*/

#include "SimpleNotification.h"

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"

namespace orxonox {

    CreateFactory(SimpleNotification);

    /**
    @brief
        Default Constructor. Registers the object and initializes variables.
    */
    SimpleNotification::SimpleNotification(BaseObject* creator) : NotificationDispatcher(creator)
    {
        RegisterObject(SimpleNotification);

        this->setSender("simpleNotification");

        this->setSyncMode(0x0);
    }

    /**
    @brief
        Destructor.
    */
    SimpleNotification::~SimpleNotification()
    {

    }

    /**
    @brief
        Method for creating a SimpleNotification object through XML.
    */
    void SimpleNotification::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(SimpleNotification, XMLPort, xmlelement, mode);

        XMLPortParam(SimpleNotification, "message", setMessage, getMessage, xmlelement, mode);
    }

}

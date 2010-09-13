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
    @file EventMultiTrigger.h
    @brief Definition of the EventMultiTrigger class.
*/

#ifndef _EventMultiTrigger_H__
#define _EventMultiTrigger_H__

#include "objects/ObjectsPrereqs.h"

#include "MultiTrigger.h"

namespace orxonox
{

    /**
    @brief
        The EventMultiTrigger class is the equivalent of the EventTrigger class for MultiTriggers.
        Consequentially what it does is it provides a way to have a MultiTrigger triggered by any kinds of events.
        Events that are not caused by a MultiTrigger or by a MultiTrigger with an originator that is no target of this EventMultiTrigger are broadcasted to all entities that are the target of this EventMultitrigger. Events that are caused by MultiTriggers with an originator that is a target of this EventMultiTrigger just trigger the EventMultiTrigger for the originator that caused the MultiTrigger to trigger. Thus showing the equivalent behavior to the EventTrigger.

        Example:
        @code
        <EventMultiTrigger invert="true">
            <events>
                <trigger>
                    <MultiTrigger ... />
                    <EventListener ... />
                </trigger>
            </events>
        </EventMultiTrigger>
        @endcode
    @see MultiTrigger.h
        For more information on MultiTriggers.
    @author
        Damian 'Mozork' Frick
    */
    class _ObjectsExport EventMultiTrigger : public MultiTrigger
    {

        public:
            EventMultiTrigger(BaseObject* creator); //!< Constructor. Registers the object.
            virtual ~EventMultiTrigger(); //!< Destructor.

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode); //!< Method for creating an EventMultiTrigger object through XML.
            virtual void XMLEventPort(Element& xmlelement, XMLPort::Mode mode);

        private:
            void trigger(bool bTriggered, BaseObject* originator); //!< Method that causes the EventMultiTrigger to trigger upon receiving an event.

    };

}

#endif // _EventMultiTrigger_H__

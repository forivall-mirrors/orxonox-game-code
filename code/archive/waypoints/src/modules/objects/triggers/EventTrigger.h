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
    @file EventTrigger.h
    @brief Definition of the EventTrigger class.
    @ingroup NormalTrigger
*/

#ifndef _EventTrigger_H__
#define _EventTrigger_H__

#include "objects/ObjectsPrereqs.h"

#include "Trigger.h"

namespace orxonox
{

    /**
    @brief
        The EventTrigger class provides a way to have a Trigger triggered by any kinds of @ref orxonox::Event "Events".

        Example:
        @code
        <EventTrigger invert="true">
            <events>
                <trigger>
                    <TriggerBase ... />
                    <EventListener ... />
                </trigger>
            </events>
        </EventTrigger>
        @endcode

    @see Trigger
        For more information on @ref orxonox::Trigger "Triggers".

    @author
        Fabian 'x3n' Landau

    @ingroup NormalTrigger
    */
    class _ObjectsExport EventTrigger : public Trigger
    {
        public:
            EventTrigger(BaseObject* creator); // Constructor. Registers and initializes the object.
            virtual ~EventTrigger();

            virtual void XMLEventPort(Element& xmlelement, XMLPort::Mode mode); // Creates an event port.

            /**
            @brief Method that is called when a new event comes in.
                   The tick call makes sure the state change takes effect immediately.
            @param bTriggered The state of the incoming event.
            */
            inline void trigger(bool bTriggered)
                { this->bEventTriggered_ = bTriggered; this->tick(0); }

        protected:
            virtual bool isTriggered(TriggerMode::Value mode); // Check whether the EventTrigger should be triggered.

        private:
            bool bEventTriggered_; //!< Boolean to keep track of what the state of the last event was.
    };
}

#endif /* _EventTrigger_H__ */

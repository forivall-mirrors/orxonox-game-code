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
    @defgroup Event Events
    @ingroup Core
*/

/**
    @file
    @ingroup Event
    @brief Declaration of the classes Event and EventState.
*/

#ifndef _Event_H__
#define _Event_H__

#include "CorePrereqs.h"
#include <string>

namespace orxonox
{
    /**
        @brief The Event struct contains information about a fired Event.
    */
    struct _CoreExport Event
    {
        Event(bool activate, BaseObject* originator, const std::string& name) : activate_(activate), originator_(originator), name_(name) {}

        bool        activate_;   //!< True if this is an activating event (the event source was inactive before and just triggered the event) - false otherwise
        std::string statename_;  //!< The name of the state this event affects
        BaseObject* originator_; //!< The object which triggered this event
        std::string name_;       //!< The name of this event

    };

    /**
        @brief The EventState contains information about an event state.

        An event state is a state of an object, which can be changed by events.
        Event states are changed through functions. Possible functions headers for set event states are:
         - memoryless state: <tt>function()</tt>
         - boolean state:    <tt>function(bool state)</tt>
         - individual state: <tt>function(bool state, SomeClass originator)</tt>

        Note that SomeClass may be any class deriving from BaseObject. You will not receive events from originators of other classes.
        The actual class for SomeClass must be specified as the second argument of the XMLPortEventState() macro.

        The this pointer of the affected object is hidden in the functors, because the events are processed in the BaseObject, but some
        statefunctions may be from child-classes.
    */
    class _CoreExport EventState
    {
        public:
            EventState(const FunctorPtr& statefunction, Identifier* subclass, bool bSink = false) : bProcessingEvent_(false), activeEvents_(0), statefunction_(statefunction), subclass_(subclass), bSink_(bSink) {}

            void process(const Event& event, BaseObject* object);

            const FunctorPtr& getFunctor() const
                { return this->statefunction_; }

        private:
            bool        bProcessingEvent_;  //!< This becomes true while the container processes an event (used to prevent loops)
            int         activeEvents_;      //!< The number of events which affect this state and are currently active
            FunctorPtr  statefunction_;     //!< A functor to set the state
            Identifier* subclass_;          //!< Originators must be an instance of this class (usually BaseObject, but some statefunctions allow a second argument with an originator of a specific class)
            bool        bSink_;             //!< Determines whether the EventState acts as an EventSink forwarding any Event (even if the state didn't change) or in the normal manner, only processing Events that change the state.
    };
}

#endif /* _Event_H__ */

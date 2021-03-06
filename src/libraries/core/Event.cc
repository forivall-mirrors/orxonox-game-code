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
    @file
    @brief Implementation of the classes Event and EventState.
*/

#include "Event.h"

#include "BaseObject.h"
#include "class/Identifier.h"

namespace orxonox
{
    /**
        @brief Processes an event (calls the set-function if the necessary conditions are met).

        @param event     The fired event
        @param object    The object whose state is affected by the event (only needed for debug output)
    */
    void EventState::process(const Event& event, BaseObject* object)
    {
        if (this->bProcessingEvent_)
        {
            orxout(internal_warning, context::events) << "Detected Event loop in section \"" << event.statename_ << "\" of object \"" << object->getName() << "\" and fired by \"" << event.originator_->getName() << '"' << endl;
            return;
        }

        this->bProcessingEvent_ = true;

        orxout(verbose, context::events) << "Processing event (EventState) : originator: " << event.originator_->getIdentifier()->getName() << " (&" << event.originator_ << "), activate: " << event.activate_ << ", name: " << event.name_ << ", statename: " << event.statename_ << ", object: " << object->getIdentifier()->getName() << " (&" << object << ")" << "." << endl;

        // check if the originator is an instance of the requested class
        if (event.originator_->isA(this->subclass_))
        {
            // If the EventState doesn't act as an EventSink.
            // actualize the activationcounter
            if(!this->bSink_)
            {
                if (event.activate_)
                    ++this->activeEvents_;
                else
                {
                    --this->activeEvents_;

                    if (this->activeEvents_ < 0)
                        this->activeEvents_ = 0;
                }
            }

            if (this->statefunction_->getParamCount() == 0 && event.activate_)
            {
                // if the eventfunction doesn't have a state, just call it whenever an activation-event comes in
                (*this->statefunction_)();
            }
            else if (this->bSink_ || (!this->bSink_ && ((this->activeEvents_ == 1 && event.activate_) || (this->activeEvents_ == 0 && !event.activate_)) ) )
            {
                // if the eventfunction needs a state, we just call the function if the state changed from 0 to 1 (state = true) or from 1 to 0 (state = false) [but not if activeEvents_ is > 1]
                if (this->statefunction_->getParamCount() == 1)
                {
                    // one argument: just the eventstate
                    (*this->statefunction_)(event.activate_);
                }
                else if (this->statefunction_->getParamCount() >= 2)
                {
                    // two arguments: the eventstate and the originator
                    if (this->subclass_->isExactlyA(ClassIdentifier<BaseObject>::getIdentifier()))
                    {
                        // if the subclass is BaseObject, we don't have to cast the pointer
                        (*this->statefunction_)(event.activate_, event.originator_);
                    }
                    else
                    {
                        // else cast the pointer to the desired class
                        void* castOriginator = event.originator_->getDerivedPointer(this->subclass_->getClassID());
                        (*this->statefunction_)(event.activate_, castOriginator);
                    }
                }
            }
        }

        this->bProcessingEvent_ = false;
    }
}

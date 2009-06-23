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

#include "Event.h"

#include "BaseObject.h"
#include "Executor.h"

namespace orxonox
{
    EventContainer::~EventContainer()
    {
        delete this->eventfunction_;
    }

    void EventContainer::process(BaseObject* object, const Event& event)
    {
        if (this->bActive_)
        {
            COUT(2) << "Warning: Detected Event loop in section \"" << this->eventname_ << "\" of object \"" << object->getName() << "\" and fired by \"" << event.originator_->getName() << "\"" << std::endl;
            return;
        }

        this->bActive_ = true;

        if (this->eventname_ == event.sectionname_)
        {
            if (event.originator_->isA(this->subclass_))
            {
                if (event.activate_)
                    ++this->activeEvents_;
                else
                {
                    --this->activeEvents_;

                    if (this->activeEvents_ < 0)
                        this->activeEvents_ = 0;
                }

                if (this->eventfunction_->getParamCount() == 0 && event.activate_)
                    (*this->eventfunction_)();
                else if ((this->activeEvents_ == 1 && event.activate_) || (this->activeEvents_ == 0 && !event.activate_))
                {
                    if (this->eventfunction_->getParamCount() == 1)
                        (*this->eventfunction_)(this->activeEvents_);
                    else if (this->eventfunction_->getParamCount() >= 2 && event.castedOriginator_)
                        (*this->eventfunction_)(this->activeEvents_, event.castedOriginator_);
                }
            }
        }

        this->bActive_ = false;
    }
}

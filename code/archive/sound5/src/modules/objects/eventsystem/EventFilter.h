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

#ifndef _EventFilter_H__
#define _EventFilter_H__

#include "objects/ObjectsPrereqs.h"

#include <list>
#include "core/BaseObject.h"

namespace orxonox
{
    class _ObjectsExport EventFilter : public BaseObject
    {
        public:
            EventFilter(BaseObject* creator);
            virtual ~EventFilter();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);

            virtual void processEvent(Event& event);

            void addFilterSource(BaseObject* source);
            BaseObject* getFilterSource(unsigned int index) const;

            void addEventName(EventName* eventname);
            EventName* getEventName(unsigned int index) const;

        private:
            std::list<BaseObject*> sources_;
            std::list<EventName*> names_;
            bool bActive_;
    };
}

#endif /* _EventFilter_H__ */

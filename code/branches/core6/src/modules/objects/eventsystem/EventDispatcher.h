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
    @file EventDispatcher.h
    @brief Definition of the EventDispatcher class.
    @ingroup Eventsystem
*/

#ifndef _EventDispatcher_H__
#define _EventDispatcher_H__

#include "objects/ObjectsPrereqs.h"

#include <list>
#include "core/BaseObject.h"

namespace orxonox
{
    class _ObjectsExport EventDispatcher : public BaseObject
    {
        public:
            EventDispatcher(Context* context);
            virtual ~EventDispatcher();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);

            virtual void processEvent(Event& event);

            void addTarget(BaseObject* target);
            BaseObject* getTarget(unsigned int index) const;

        private:
            std::list<BaseObject*> targets_;
    };
}

#endif /* _EventDispatcher_H__ */

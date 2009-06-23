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

#ifndef _Event_H__
#define _Event_H__

#include "CorePrereqs.h"

namespace orxonox
{
    struct _CoreExport Event
    {
            Event(bool activate, BaseObject* originator) : activate_(activate), originator_(originator), castedOriginator_(0) {}

            bool        activate_;
            BaseObject* originator_;
            void*       castedOriginator_;
            std::string sectionname_;
    };

    class _CoreExport EventContainer
    {
        public:
            EventContainer(const std::string& eventname, Executor* eventfunction, Identifier* subclass) : bActive_(false), eventname_(eventname), eventfunction_(eventfunction), subclass_(subclass), activeEvents_(0) {}
            virtual ~EventContainer();

            void process(BaseObject* object, const Event& event);

        private:
            bool bActive_;
            std::string eventname_;
            Executor* eventfunction_;
            Identifier* subclass_;

            int activeEvents_;
    };
}

#endif /* _Event_H__ */

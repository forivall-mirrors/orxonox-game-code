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

#ifndef _EventListener_H__
#define _EventListener_H__

#include "OrxonoxPrereqs.h"
#include "core/BaseObject.h"
#include "core/XMLNameListener.h"

namespace orxonox
{
    class _OrxonoxExport EventListener : public BaseObject, public XMLNameListener
    {
        public:
            EventListener(BaseObject* creator);
            virtual ~EventListener();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);

            virtual void processEvent(Event& event);

            void setEventName(const std::string& eventname);
            inline const std::string& getEventName() const
                { return this->eventName_; }

        private:
            virtual void loadedNewXMLName(BaseObject* object);
            void registerAsListener(BaseObject* object);

            std::string eventName_;
            bool bActive_;
    };
}

#endif /* _EventListener_H__ */

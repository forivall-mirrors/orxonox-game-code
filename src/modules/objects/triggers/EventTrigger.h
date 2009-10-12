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

#ifndef _EventTrigger_H__
#define _EventTrigger_H__

#include "objects/ObjectsPrereqs.h"
#include "Trigger.h"

namespace orxonox
{
    class _ObjectsExport EventTrigger : public Trigger
    {
        public:
            EventTrigger(BaseObject* creator);
            virtual ~EventTrigger();

            virtual void XMLEventPort(Element& xmlelement, XMLPort::Mode mode);

            inline void trigger(bool bTriggered)
                { this->bEventTriggered_ = bTriggered; this->tick(0); }

        protected:
            virtual bool isTriggered(TriggerMode::Value mode);

        private:
            bool bEventTriggered_;
    };
}

#endif /* _EventTrigger_H__ */

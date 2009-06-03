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

#ifndef _Attacher_H__
#define _Attacher_H__

#include "OrxonoxPrereqs.h"

#include <list>

#include "objects/worldentities/StaticEntity.h"
#include "core/XMLNameListener.h"

namespace orxonox
{
    class _OrxonoxExport Attacher : public StaticEntity, public XMLNameListener
    {
        public:
            Attacher(BaseObject* creator);
            virtual ~Attacher() {}

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);

            virtual void processEvent(Event& event);
            virtual void changedActivity();
            virtual void changedVisibility();

            void addObject(WorldEntity* object);
            WorldEntity* getObject(unsigned int index) const;

            void setTarget(const std::string& target);
            inline const std::string& getTarget() const
                { return this->targetname_; }

            void loadedNewXMLName(BaseObject* object);

        private:
            WorldEntity* target_;
            std::string targetname_;
            std::list<WorldEntity*> objects_;
    };
}

#endif /* _Attacher_H__ */

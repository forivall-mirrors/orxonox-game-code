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

#ifndef _SpawnPoint_H__
#define _SpawnPoint_H__

#include "OrxonoxPrereqs.h"

#include <string>
#include "core/class/SubclassIdentifier.h"
#include "worldentities/StaticEntity.h"

namespace orxonox
{
    class _OrxonoxExport SpawnPoint : public StaticEntity
    {
        public:
            SpawnPoint(Context* context);
            virtual ~SpawnPoint() {}

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);

            inline void setSpawnClass(Identifier* identifier)
                { this->spawnclass_ = identifier; this->spawnclassname_ = identifier->getName(); }
            inline Identifier* getSpawnClass() const
                { return this->spawnclass_; }

            void setTemplate(Template* temp);
            inline Template* getTemplate() const
                { return this->template_; }

            Pawn* spawn();
            void spawn(ControllableEntity* entity);

        private:
            void setSpawnClassName(const std::string& name);
            inline const std::string& getSpawnClassName() const
                { return this->spawnclassname_; }

            void setTemplateName(const std::string& name);
            inline const std::string& getTemplateName() const
                { return this->templatename_; }

            SubclassIdentifier<Pawn> spawnclass_;
            std::string spawnclassname_;
            Template* template_;
            std::string templatename_;
    };
}

#endif /* _SpawnPoint_H__ */

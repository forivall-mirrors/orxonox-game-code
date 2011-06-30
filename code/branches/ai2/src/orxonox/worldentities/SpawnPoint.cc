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

#include "SpawnPoint.h"

#include "core/CoreIncludes.h"
#include "core/Template.h"
#include "core/XMLPort.h"
#include "gametypes/Gametype.h"
#include "worldentities/pawns/Pawn.h"

namespace orxonox
{
    CreateFactory(SpawnPoint);

    SpawnPoint::SpawnPoint(BaseObject* creator) : StaticEntity(creator)
    {
        RegisterObject(SpawnPoint);

        this->template_ = 0;

        if (this->getGametype())
            this->getGametype()->registerSpawnPoint(this);
        else
            COUT(1) << "Error: SpawnPoint has no Gametype" << std::endl;

        this->setSyncMode(ObjectDirection::None);
    }

    void SpawnPoint::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(SpawnPoint, XMLPort, xmlelement, mode);

        XMLPortParam(SpawnPoint, "spawnclass", setSpawnClassName, getSpawnClassName, xmlelement, mode);
        XMLPortParam(SpawnPoint, "pawndesign", setTemplateName, getTemplateName, xmlelement, mode);
    }

    void SpawnPoint::setSpawnClassName(const std::string& name)
    {
        this->spawnclassname_ = name;
        this->spawnclass_ = ClassByString(name);
    }

    void SpawnPoint::setTemplateName(const std::string& name)
    {
        this->templatename_ = name;
        this->template_ = Template::getTemplate(name);
    }

    void SpawnPoint::setTemplate(Template* temp)
    {
        this->template_ = temp;
        this->templatename_ = temp->getName();
    }

    Pawn* SpawnPoint::spawn()
    {
        Pawn* entity = this->spawnclass_.fabricate(this);
        if (entity)
        {
            this->getGametype()->pawnPreSpawn(entity);

            this->spawn(entity);

            if (this->template_)
                entity->addTemplate(this->template_);

            entity->postSpawn();

            this->getGametype()->pawnPostSpawn(entity);

            this->fireEvent();
        }
        return entity;
    }

    void SpawnPoint::spawn(ControllableEntity* entity)
    {
        entity->setPosition(this->getWorldPosition());
        entity->setOrientation(this->getWorldOrientation());
    }
}

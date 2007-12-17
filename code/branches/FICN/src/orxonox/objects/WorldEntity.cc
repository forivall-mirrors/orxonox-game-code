/*
 *   ORXONOX - the hottest 3D action shooter ever to exist
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
 *      ...
 *   Co-authors:
 *      ...
 *
 */

#include <string>
#include <sstream>

#include "WorldEntity.h"
#include "../core/CoreIncludes.h"
#include "../orxonox.h"

namespace orxonox
{
    CreateFactory(WorldEntity);

    unsigned int WorldEntity::worldEntityCounter_s = 0;

    WorldEntity::WorldEntity()
    {
        RegisterObject(WorldEntity);

        if (Orxonox::getSingleton()->getSceneManager())
        {
            std::ostringstream name;
            name << (WorldEntity::worldEntityCounter_s++);
            this->setName("WorldEntity" + name.str());
            node_ = Orxonox::getSingleton()->getSceneManager()->getRootSceneNode()->createChildSceneNode(this->getName());
        }

        this->bStatic_ = true;
        this->velocity_ = Vector3(0, 0, 0);
        this->acceleration_ = Vector3(0, 0, 0);
        this->rotationAxis_ = Vector3(0, 1, 0);
        this->rotationRate_ = 0;
        this->momentum_ = 0;
    }

    WorldEntity::~WorldEntity()
    {
    }

    void WorldEntity::tick(float dt)
    {
        if (!this->bStatic_)
        {
            this->velocity_ += (dt * this->acceleration_);
            this->translate(dt * this->velocity_);

            this->rotationRate_ += (dt * this->momentum_);
            this->rotate(this->rotationAxis_, dt * this->rotationRate_);
        }
    }
    
    void WorldEntity::registerAllVariables(){
      
      
    }
}

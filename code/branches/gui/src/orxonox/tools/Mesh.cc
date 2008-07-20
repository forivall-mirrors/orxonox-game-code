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

#include "OrxonoxStableHeaders.h"
#include "Mesh.h"

#include <sstream>

#include <OgreSceneManager.h>

#include "GraphicsEngine.h"

namespace orxonox
{
    unsigned int Mesh::meshCounter_s = 0;

    Mesh::Mesh() :
      entity_(0)
    {
    }

    void Mesh::setMesh(const std::string& file)
    {
        std::ostringstream name;
        name << (Mesh::meshCounter_s++);
        this->entity_ = GraphicsEngine::getSingleton().getLevelSceneManager()->createEntity("Mesh" + name.str(), file);
    }

    Mesh::~Mesh()
    {
        if (this->entity_)
            GraphicsEngine::getSingleton().getLevelSceneManager()->destroyEntity(this->entity_);
    }
}

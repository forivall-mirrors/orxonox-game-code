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

#include "Mesh.h"

#include <cassert>
#include <sstream>
#include <string>
#include <OgreEntity.h>
#include <OgreSceneManager.h>

#include "util/Convert.h"
#include "core/GameMode.h"

namespace orxonox
{
    unsigned int Mesh::meshCounter_s = 0;

    Mesh::Mesh()
    {
        this->entity_ = 0;
        this->bCastShadows_ = true;
    }

    Mesh::~Mesh()
    {
        if (this->entity_ && this->scenemanager_)
            this->scenemanager_->destroyEntity(this->entity_);
    }

    void Mesh::setMeshSource(Ogre::SceneManager* scenemanager, const std::string& meshsource)
    {
        assert(scenemanager);

        this->scenemanager_ = scenemanager;

        if (this->entity_)
            this->scenemanager_->destroyEntity(this->entity_);

        if (GameMode::showsGraphics())
        {
            try
            {
                this->entity_ = this->scenemanager_->createEntity("Mesh" + multi_cast<std::string>(Mesh::meshCounter_s++), meshsource);
                this->entity_->setCastShadows(this->bCastShadows_);
            }
            catch (...)
            {
                COUT(1) << "Error: Couln't load mesh \"" << meshsource << '"' << std::endl;
                this->entity_ = 0;
            }
        }
    }

    void Mesh::setCastShadows(bool bCastShadows)
    {
        this->bCastShadows_ = bCastShadows;
        if (this->entity_)
            this->entity_->setCastShadows(this->bCastShadows_);
    }

    const std::string& Mesh::getName() const
    {
        if (this->entity_)
            return this->entity_->getName();
        else
            return BLANKSTRING;
    }

    void Mesh::setVisible(bool bVisible)
    {
        if (this->entity_)
            this->entity_->setVisible(bVisible);
    }

    bool Mesh::isVisible() const
    {
        if (this->entity_)
            return this->entity_->getVisible();
        else
            return false;
    }
}

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

#include "core/Core.h"
#include "GraphicsEngine.h"
#include "util/Convert.h"
#include "util/String.h"

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
        if (this->entity_ && Core::showsGraphics())
            GraphicsEngine::getInstance().getLevelSceneManager()->destroyEntity(this->entity_);
    }

    void Mesh::setMeshSource(const std::string& meshsource)
    {
        if (Core::showsGraphics())
        {
            if (this->entity_)
                GraphicsEngine::getInstance().getLevelSceneManager()->destroyEntity(this->entity_);

            try
            {
                this->entity_ = GraphicsEngine::getInstance().getLevelSceneManager()->createEntity("Mesh" + convertToString(Mesh::meshCounter_s++), meshsource);
                this->entity_->setCastShadows(this->bCastShadows_);
            }
            catch (...)
            {
                COUT(1) << "Error: Couln't load mesh \"" << meshsource << "\"" << std::endl;
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
            return blankString;
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

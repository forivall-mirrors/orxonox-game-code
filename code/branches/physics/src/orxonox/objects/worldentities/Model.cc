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

#include "Model.h"
#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "objects/Scene.h"

namespace orxonox
{
    CreateFactory(Model);

    Model::Model(BaseObject* creator) : StaticEntity(creator)
    {
        RegisterObject(Model);

        this->registerVariables();
    }

    Model::~Model()
    {
        if (this->isInitialized() && this->mesh_.getEntity())
            this->getNode()->detachObject(this->mesh_.getEntity());
    }

    void Model::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(Model, XMLPort, xmlelement, mode);

        XMLPortParam(Model, "mesh", setMeshSource, getMeshSource, xmlelement, mode);
        XMLPortParam(Model, "shadow", setCastShadows, getCastShadows, xmlelement, mode).defaultValues(true);
    }

    void Model::registerVariables()
    {
        REGISTERSTRING(this->meshSrc_,    network::direction::toclient, new network::NetworkCallback<Model>(this, &Model::changedMesh));
        REGISTERDATA(this->bCastShadows_, network::direction::toclient, new network::NetworkCallback<Model>(this, &Model::changedShadows));
    }

    void Model::changedMesh()
    {
        if (this->mesh_.getEntity())
            this->getNode()->detachObject(this->mesh_.getEntity());

        this->mesh_.setMeshSource(this->getScene()->getSceneManager(), this->meshSrc_);

        if (this->mesh_.getEntity())
        {
            this->getNode()->attachObject(this->mesh_.getEntity());
            this->mesh_.getEntity()->setCastShadows(this->bCastShadows_);
            this->mesh_.setVisible(this->isVisible());
        }
    }

    void Model::changedShadows()
    {
        this->mesh_.setCastShadows(this->bCastShadows_);
    }

    void Model::changedVisibility()
    {
        SUPER(Model, changedVisibility);

        this->mesh_.setVisible(this->isVisible());
    }
}

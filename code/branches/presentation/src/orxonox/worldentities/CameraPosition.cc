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

#include "CameraPosition.h"

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "graphics/Camera.h"

namespace orxonox
{
    CreateFactory(CameraPosition);

    CameraPosition::CameraPosition(BaseObject* creator) : StaticEntity(creator)
    {
        RegisterObject(CameraPosition);

        this->bDrag_ = false;
        this->bAllowMouseLook_ = false;
        this->bAbsolute_ = false;
        this->bRenderCamera_ = false;

        this->setSyncMode(ObjectDirection::None);
    }

    CameraPosition::~CameraPosition()
    {
    }

    void CameraPosition::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(CameraPosition, XMLPort, xmlelement, mode);

        XMLPortParam(CameraPosition, "drag", setDrag, getDrag, xmlelement, mode).defaultValues(false);
        XMLPortParam(CameraPosition, "mouselook", setAllowMouseLook, getAllowMouseLook, xmlelement, mode).defaultValues(false);
        XMLPortParam(CameraPosition, "absolute", setIsAbsolute, getIsAbsolute, xmlelement, mode).defaultValues(false);
        XMLPortParam(CameraPosition, "rendercamera", setRenderCamera, getRenderCamera, xmlelement, mode).defaultValues(false);
    }

    void CameraPosition::attachCamera(Camera* camera)
    {
        if (!this->bDrag_)
            camera->setDrag(false);

        this->attach(camera);

        if (this->bDrag_)
            camera->setDrag(true);
    }

    void CameraPosition::attachCamera(Ogre::Camera* camera)
    {
        this->attachOgreObject(camera);
    }
}

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
 *      ...
 *   Co-authors:
 *      ...
 *
 */

#include "OrxonoxStableHeaders.h"
#include "Skybox.h"

#include <string>

#include <OgreSceneManager.h>

#include "tinyxml/tinyxml.h"
#include "GraphicsEngine.h"
#include "core/CoreIncludes.h"
#include "core/Debug.h"
#include "core/XMLPort.h"

namespace orxonox
{
    CreateFactory(Skybox);

    Skybox::Skybox()
    {
        RegisterObject(Skybox);
        registerAllVariables();
    }

    Skybox::~Skybox()
    {
    }

    void Skybox::setSkybox(const std::string& skyboxname)
    {
        GraphicsEngine::getSingleton().getLevelSceneManager()->setSkyBox(true, skyboxname);
    }

    void Skybox::setSkyboxSrc(const std::string& src)
    {
        this->skyboxSrc_ = src;
    }

    /**
        @brief XML loading and saving.
        @param xmlelement The XML-element
        @param loading Loading (true) or saving (false)
        @return The XML-element
    */
    void Skybox::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        BaseObject::XMLPort(xmlelement, mode);

        XMLPortParamLoadOnly(Skybox, "src", setSkyboxSrc, xmlelement, mode);
        create();
    }

    bool Skybox::create()
    {
        this->setSkybox(this->skyboxSrc_);
        return Synchronisable::create();
    }

    void Skybox::registerAllVariables()
    {
        registerVar(&skyboxSrc_, skyboxSrc_.length()+1 ,network::STRING);
    }

    void Skybox::changedVisibility()
    {
        BaseObject::changedVisibility();
        GraphicsEngine::getSingleton().getLevelSceneManager()->setSkyBox(this->isVisible(), this->skyboxSrc_);
    }
}

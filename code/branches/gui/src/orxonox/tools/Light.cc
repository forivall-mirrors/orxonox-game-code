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
#include "Light.h"

#include <sstream>

#include <OgreSceneManager.h>

#include "GraphicsEngine.h"

namespace orxonox
{
    unsigned int Light::lightCounter_s = 0;

    Light::Light()
    {
        this->light_ = 0;
    }

    void Light::setLight(Ogre::Light::LightTypes type, const ColourValue& diffuse, const ColourValue& specular)
    {
        std::ostringstream name;
        name << (Light::lightCounter_s++);
        this->light_ = GraphicsEngine::getInstance().getLevelSceneManager()->createLight("Light" + name.str());
        this->light_->setType(type);
        this->light_->setDiffuseColour(diffuse);
        this->light_->setSpecularColour(specular);
    }

    Light::~Light()
    {
        if (this->light_)
            GraphicsEngine::getInstance().getLevelSceneManager()->destroyLight(this->light_);
    }
}

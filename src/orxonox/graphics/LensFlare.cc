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
 *      Reto Grieder (physics)
 *   Co-authors:
 *      ...
 *
 */

/**
  @file LensFlare.cc
  @brief Implementation of the LensFlare class.
*/

#include "LensFlare.h"

#include "core/XMLPort.h"
#include "graphics/Billboard.h"
#include "CameraManager.h"

namespace orxonox
{
    CreateFactory(LensFlare);
    
    LensFlare::LensFlare(BaseObject* creator) : StaticEntity(creator)
    {
        RegisterObject(LensFlare);
        
        this->createBillboards();
        
        this->registerVariables();
    }

    LensFlare::~LensFlare()
    {
    }

    void LensFlare::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(LensFlare, XMLPort, xmlelement, mode);
    }
    
    void LensFlare::registerVariables()
    {
      
    }
    
    void LensFlare::createBillboards()
    {
        Billboard* burst = new Billboard(this);
        burst->setMaterial("lensflare/burst");
        burst->setPosition(this->getPosition());
        burst->setVisible(false);
        this->attach(burst);
    }
    
    void LensFlare::updateBillboardPositions()
    {
        Ogre::Camera* camera=CameraManager::getInstance().getActiveCamera()->getOgreCamera(); //get active Ogre Camera Instance, so we can check whether the light source is visible
        bool lightIsVisible=camera->isVisible(this->getPosition()); //is the light source visible from our point of view?
        int scale=camera->getPosition().distance(this->getPosition());
        
        Billboard* burst=static_cast<Billboard*>(getAttachedObject(0));
        burst->setPosition(this->getPosition());
        burst->setVisible(lightIsVisible);
        burst->setDefaultDimensions(scale,scale);
    }

    void LensFlare::tick(float dt)
    {
        if(this->isVisible())
        {
            updateBillboardPositions();
        }
    }

    void LensFlare::changedVisibility()
    {
    }
}
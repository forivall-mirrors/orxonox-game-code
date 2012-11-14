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
#include "RenderQueueListener.h"

#include <OgreSphere.h>
#include <OgreRenderWindow.h>

namespace orxonox
{
    CreateFactory(LensFlare);
    
    LensFlare::LensFlare(BaseObject* creator) : StaticEntity(creator), scale_(1.0f)
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
        XMLPortParam(LensFlare, "scale", setScale, getScale, xmlelement, mode).defaultValues(1.0f);
    }
    
    void LensFlare::registerVariables()
    {
        registerVariable(this->scale_, VariableDirection::ToClient, new NetworkCallback<LensFlare>(this, &LensFlare::updateBillboardPositions));
    }
    
    void LensFlare::createBillboards()
    {
        this->occlusionBillboard_ = new Billboard(this);
        this->occlusionBillboard_->setMaterial("lensflare/hoq");
        this->occlusionBillboard_->setPosition(this->getPosition());
        this->occlusionBillboard_->setVisible(false);
        this->occlusionBillboard_->setRenderQueueGroup(RENDER_QUEUE_HOQ);
        this->attach(this->occlusionBillboard_);
        
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
        this->cameraDistance_=camera->getPosition().distance(this->getPosition());
        unsigned int dimension=this->cameraDistance_*this->scale_;
        
        this->occlusionBillboard_->setPosition(this->getPosition());
        this->occlusionBillboard_->setVisible(lightIsVisible);
        this->occlusionBillboard_->setDefaultDimensions(dimension,dimension);
        
        Billboard* burst=static_cast<Billboard*>(getAttachedObject(1));
        burst->setPosition(this->getPosition());
        burst->setVisible(lightIsVisible);
        burst->setDefaultDimensions(dimension,dimension);
    }

    void LensFlare::tick(float dt)
    {
        if(this->isVisible())
        {
            updateBillboardPositions();
            if(this->occlusionBillboard_->isVisible()) {
                unsigned int dimension=this->cameraDistance_*this->scale_;
                Ogre::Sphere* sphere=new Ogre::Sphere(this->getPosition(),dimension*0.25);
                Ogre::Camera* camera=CameraManager::getInstance().getActiveCamera()->getOgreCamera();
                float left, right, top, bottom;
                camera->projectSphere(*sphere,&left,&top,&right,&bottom);//approximate maximum pixel count of billboard with a sphere
                delete sphere;
                
                Ogre::RenderWindow* window = GraphicsManager::getInstance().getRenderWindow();
                float maxCount=(right-left)*(top-bottom)*window->getWidth()*window->getHeight()*0.25;
                float pixelCount=this->getScene()->getRenderQueueListener()->getPixelCount();//get pixel count
                float ratio=pixelCount/maxCount;
                //orxout() << "maxCount: " << maxCount << " HOQ: " << pixelCount << " ratio: " << ratio << std::endl;
                ColourValue* colour = new ColourValue(1.0f,1.0f,1.0f,ratio); //adjust alpha of billboard
                
                Billboard* burst=static_cast<Billboard*>(getAttachedObject(1));
                burst->setColour(*colour);
                delete colour;
            }
        }
    }

    void LensFlare::changedVisibility()
    {
    }
}
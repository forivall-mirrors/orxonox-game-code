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
    
    LensFlare::LensFlare(BaseObject* creator) : StaticEntity(creator), scale_(1.0f), fadeOnViewBorder_(true), fadeResolution_(7), fadeExponent_(2.0f), colour_(new ColourValue(1.0f,0.9f,0.9f))
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
        XMLPortParam(LensFlare, "fadeOnViewBorder", setFadeOnViewBorder, isFadeOnViewBorder, xmlelement, mode).defaultValues(true);
        XMLPortParam(LensFlare, "fadeResolution", setFadeResolution, getFadeResolution, xmlelement, mode).defaultValues(7);
        XMLPortParam(LensFlare, "fadeExponent", setFadeExponent, getFadeExponent, xmlelement, mode).defaultValues(2.0f);
    }
    
    void LensFlare::registerVariables()
    {
        registerVariable(this->scale_, VariableDirection::ToClient);
        registerVariable(this->fadeOnViewBorder_, VariableDirection::ToClient);
        registerVariable(this->fadeResolution_, VariableDirection::ToClient);
    }

    /**
    @brief
        This function creates all the billboards needed for the flare effect
    */
    void LensFlare::createBillboards()
    {
        //TODO: add more billboards, possibly do some cleaning up, by using a loop
        this->occlusionBillboard_ = new Billboard(this);
        this->occlusionBillboard_->setMaterial("lensflare/hoq");
        this->occlusionBillboard_->setPosition(this->getPosition());
        this->occlusionBillboard_->setVisible(false);
        this->occlusionBillboard_->disableFrustumCulling();
        this->occlusionBillboard_->setRenderQueueGroup(RENDER_QUEUE_HOQ);
        this->attach(this->occlusionBillboard_);
        
        Billboard* burst = new Billboard(this);
        burst->setMaterial("lensflare/burst");
        burst->setPosition(this->getPosition());
        burst->disableFrustumCulling();
        burst->setVisible(true);
        this->attach(burst);
        
        Billboard* bursthalo = new Billboard(this);
        bursthalo->setMaterial("lensflare/bursthalo");
        bursthalo->setPosition(this->getPosition());
        bursthalo->disableFrustumCulling();
        bursthalo->setVisible(true);
        this->attach(bursthalo);
        
        bursthalo = new Billboard(this);
        bursthalo->setMaterial("lensflare/halo1");
        bursthalo->setPosition(this->getPosition());
        bursthalo->disableFrustumCulling();
        bursthalo->setVisible(true);
        this->attach(bursthalo);
        
        bursthalo = new Billboard(this);
        bursthalo->setMaterial("lensflare/halo2");
        bursthalo->setPosition(this->getPosition());
        bursthalo->disableFrustumCulling();
        bursthalo->setVisible(true);
        this->attach(bursthalo);
        
        bursthalo = new Billboard(this);
        bursthalo->setMaterial("lensflare/halo3");
        bursthalo->setPosition(this->getPosition());
        bursthalo->disableFrustumCulling();
        bursthalo->setVisible(true);
        this->attach(bursthalo);
    }

    /**
    @brief
        This function updates the states of all the billboards, i.e. their positions, visibilty and dimensions
    @param viewDirection
        normalised vector pointing from the current camera to the point light center
    @param dimension
        the current dimension of the main billboard, we're always using square billboards
    @param lightIsVisible
        is the (point-)light source currently visible
    */
    void LensFlare::updateBillboardStates(Vector3& viewDirection, float dimension, bool lightIsVisible)
    { 
        //TODO: develop a more sane method for determining positions and scale factors of the flare components
        //A good solution would probably be to introduce a data structure which stores a lens flare configuration
        int i=0;
        float step=0.0f;
        Vector3 position=CameraManager::getInstance().getActiveCamera()->getOgreCamera()->getDerivedPosition();
        for(std::set<WorldEntity*>::const_iterator it = this->getAttachedObjects().begin(); it != this->getAttachedObjects().end(); it++) {
            Billboard* billboard=static_cast<Billboard*>(*it);
            billboard->setPosition(this->getPosition()-viewDirection*step);
            billboard->setVisible(lightIsVisible);
            billboard->setDefaultDimensions((i<0?0.5f:1.0f)*(i>2?0.25f:1.0f)*dimension*std::pow((1.0f-step),-1.0f),(i<0?0.5f:1.0f)*(i>2?0.25f:1.0f)*dimension*std::pow((1.0f-step),-1.0f));
            step=0.25f*(i>2?(i-2):0);
            i++;
        }
    }

    /**
    @brief
        This function updates the alpha values for all billboards except for the one used for Hardware Occlusion Querying
    @param alpha
        the new alpha value all visible billboards should use
    */
    void LensFlare::updateBillboardAlphas(float alpha)
    {
        this->colour_->a=alpha;
        std::set<WorldEntity*>::const_iterator it = this->getAttachedObjects().begin();
        it++;
        for(int i=0;it!=this->getAttachedObjects().end(); it++) {
            if(i==2)
            {
                this->colour_->a*=0.5f;
            }
            Billboard* billboard=static_cast<Billboard*>(*it);
            billboard->setColour(*(this->colour_));
            i++;
        }
    }
    
    /**
    @brief
        This function generates point samples of the main burst billboard according to the fadeResolution and returns how many of them are in the view port
    @param dimension
        the current dimension of the main billboard, we're always using square billboards
    @return
        the absolute amount of point samples that are currently captured by the camera of the view port
    */
    unsigned int LensFlare::getPointCount(float dimension) const
    {
        Ogre::Camera* camera=CameraManager::getInstance().getActiveCamera()->getOgreCamera();
        Vector3 position = this->getPosition();
        Vector3 nX = camera->getDerivedOrientation().xAxis().normalisedCopy();
        Vector3 nY = camera->getDerivedOrientation().yAxis().normalisedCopy();
        int halfRes=fadeResolution_/2;
        float resDim=dimension/fadeResolution_;
        unsigned int count=0;
        for(int i=-halfRes;i<=halfRes;i++)
        {
            for(int j=-halfRes;j<=halfRes;j++)
            {
                Vector3 point=position+(i*resDim)*nX+(j*resDim)*nY;//generate point samples
                if(camera->isVisible(point))
                {
                    count++;
                }
            }
        }
        return count;
    }

    void LensFlare::tick(float dt)
    {
        if(this->isVisible())
        {
            Ogre::Camera* camera=CameraManager::getInstance().getActiveCamera()->getOgreCamera(); //get active Ogre Camera Instance, so we can check whether the light source is visible
            this->cameraDistance_=camera->getPosition().distance(this->getPosition());
            float dimension=this->cameraDistance_*this->scale_;
            if(!this->fadeOnViewBorder_)
            {
                this->fadeResolution_=3;//this is so we can still determine when the billboard has left the screen
            }
            unsigned int pointCount=this->getPointCount(dimension*0.5f);
            Vector3 viewDirection=this->getPosition()-camera->getDerivedPosition()-camera->getDerivedDirection()*this->cameraDistance_;
            updateBillboardStates(viewDirection,dimension,pointCount>0);
            if(pointCount>0) {
                Ogre::Sphere* sphere=new Ogre::Sphere(this->getPosition(),dimension*0.25f);
                float left, right, top, bottom;
                camera->projectSphere(*sphere,&left,&top,&right,&bottom);//approximate maximum pixel count of billboard with a sphere
                delete sphere;
                
                Ogre::RenderWindow* window = GraphicsManager::getInstance().getRenderWindow();
                float maxCount=(right-left)*(top-bottom)*window->getWidth()*window->getHeight()*0.25f;
                float pixelCount=this->getScene()->getRenderQueueListener()->getPixelCount();//get pixel count
                float ratio=(maxCount<0.0f)?0.0f:(pixelCount/maxCount);//prevent underflow and division by zero
                float borderRatio=1.0f;
                if(this->fadeOnViewBorder_)
                {
                    borderRatio=((float) pointCount)/(((float) fadeResolution_)*((float) fadeResolution_));//ratio for the border fade
                }
                //update alpha values of all billboards except the HOQ billboard
                this->updateBillboardAlphas(std::min(1.0f,std::pow(std::min(ratio,borderRatio),this->fadeExponent_)));
            }
        }
    }

    void LensFlare::changedVisibility()
    {
      
    }
}
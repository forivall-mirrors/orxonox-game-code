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
        
        this->lensConfiguration_=new std::vector<LensFlare::Lens*>();
        this->lensConfiguration_->push_back(new LensFlare::Lens(new std::string("lensflare/burst"),1.0f,1.0f,1.0f)); //main burst
        this->lensConfiguration_->push_back(new LensFlare::Lens(new std::string("lensflare/bursthalo"),0.7f,0.9f,1.0f)); //halo around main burst
        this->lensConfiguration_->push_back(new LensFlare::Lens(new std::string("lensflare/halo1"),0.4f,0.2f,0.8f)); //all the different distanced lenses
        this->lensConfiguration_->push_back(new LensFlare::Lens(new std::string("lensflare/halo2"),0.7f,0.3f,0.7f));
        this->lensConfiguration_->push_back(new LensFlare::Lens(new std::string("lensflare/halo3"),0.3f,0.4f,0.6f));
        this->lensConfiguration_->push_back(new LensFlare::Lens(new std::string("lensflare/halo3"),0.1f,0.8f,0.4f));
        this->lensConfiguration_->push_back(new LensFlare::Lens(new std::string("lensflare/halo1"),0.15f,0.5f,0.35f));
        
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
        XMLPortParam(LensFlare, "colour", setColour, getColour, xmlelement, mode);
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
        this->occlusionBillboard_ = new Billboard(this);
        this->occlusionBillboard_->setMaterial("lensflare/hoq");
        this->occlusionBillboard_->setVisible(false);
        this->occlusionBillboard_->disableFrustumCulling();
        this->occlusionBillboard_->setRenderQueueGroup(RENDER_QUEUE_HOQ);
        this->attach(this->occlusionBillboard_);
        
        for(std::vector<LensFlare::Lens*>::iterator it = lensConfiguration_->begin(); it != lensConfiguration_->end(); ++it) {
            Billboard* lensPart=new Billboard(this);
            lensPart->setMaterial(*(*it)->material_);
            lensPart->disableFrustumCulling();
            lensPart->setVisible(true);
            this->attach(lensPart);
        }
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
        this->occlusionBillboard_->setDefaultDimensions(dimension*0.5f,dimension*0.5f);
        this->occlusionBillboard_->setVisible(lightIsVisible);
        std::set<WorldEntity*>::const_iterator it = this->getAttachedObjects().begin();
        it++;
        for(int i=0; it != this->getAttachedObjects().end(); it++) {
            Billboard* billboard=static_cast<Billboard*>(*it);
            LensFlare::Lens* lens=lensConfiguration_->at(i);
            billboard->setPosition(-viewDirection*(1.0f-lens->position_));
            billboard->setVisible(lightIsVisible);
            billboard->setDefaultDimensions(dimension*lens->scale_,dimension*lens->scale_);
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
            ColourValue* cur=new ColourValue(0,0,0,0);
            (*cur)+= *(this->colour_);
            cur->a*=lensConfiguration_->at(i)->alpha_;
            Billboard* billboard=static_cast<Billboard*>(*it);
            billboard->setColour(*cur);
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
        Vector3 position = this->getWorldPosition();
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
            this->cameraDistance_=camera->getDerivedPosition().distance(this->getPosition());
            float dimension=this->cameraDistance_*this->scale_;
            if(!this->fadeOnViewBorder_)
            {
                this->fadeResolution_=3;//this is so we can still determine when the billboard has left the screen
            }
            unsigned int pointCount=this->getPointCount(dimension*0.25f*this->scale_);
            Vector3 viewDirection=this->getWorldPosition()-camera->getDerivedPosition()-camera->getDerivedDirection()*this->cameraDistance_;
            updateBillboardStates(viewDirection,dimension,pointCount>0);
            if(pointCount>0) {
                Ogre::Sphere* sphere=new Ogre::Sphere(this->getPosition(),dimension*0.25f*this->scale_);
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
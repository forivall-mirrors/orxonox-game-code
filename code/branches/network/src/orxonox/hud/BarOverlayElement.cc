/*
*   ORXONOX - the hottest 3D action shooter ever to exist
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
*      Yuning Chai
*   Co-authors:
*      Felix Schulthess
*
*/

#include "OrxonoxStableHeaders.h"
#include "BarOverlayElement.h"
#include <OgreOverlayManager.h>
#include "GraphicsEngine.h"

namespace orxonox
{
    using namespace Ogre;

    BarOverlayElement::BarOverlayElement(const String& name):PanelOverlayElement(name){
        name_ = name;
    }

    BarOverlayElement::~BarOverlayElement(){}

    void BarOverlayElement::init(Real leftRel, Real topRel, Real dimRel, OverlayContainer* container){
        // init some values...
        container_ = container;
        om = &OverlayManager::getSingleton();
        value_ = 0;
        color_ = 2;
        autoColor_ = true;
        left2Right = false; 	// default is right to left progress
        leftRel_ = leftRel;
        topRel_ = topRel;
        dimRel_ = dimRel;

        // create background...
        background_ = static_cast<OverlayContainer*>(om->createOverlayElement("Panel", name_+"container"));
        background_->show();
        container_->addChild(background_);
        background_->setMetricsMode(GMM_PIXELS);
        background_->setMaterialName("Orxonox/BarBackground");

        // calculate absolute coordinates...
        resize();

        show();
        setMetricsMode(GMM_PIXELS);
        setMaterialName("Orxonox/Green");
        background_->addChild(this);
    }

    void BarOverlayElement::resize(){
        windowW_ = GraphicsEngine::getSingleton().getWindowWidth();
        windowH_ = GraphicsEngine::getSingleton().getWindowHeight();
        // calculate new absolute coordinates...
        left_ = (int) (leftRel_ * windowW_);
        top_ = (int) (topRel_ * windowH_);
        width_ = (int) (dimRel_ * windowW_);
        height_ = (int) (0.1*width_);	// the texture has dimensions height:length = 1:10
        // adapt background
        background_->setPosition(left_, top_);
        background_->setDimensions(width_, height_);
        // adapt bar
        setValue(value_);
    }

    void BarOverlayElement::setValue(float value){
        value_ = value;
        // set color, if nescessary
        if(autoColor_){
            if (value_>0.5) {setColor(BarOverlayElement::GREEN);}
            else if (value_>0.25) {setColor(BarOverlayElement::YELLOW);}
            else setColor(BarOverlayElement::RED);
        }
        // set value
        if(left2Right){ // backward case
            setPosition(0+width_-width_*value_, 0);
            setDimensions(width_*value_,height_);
        }else{          // default case
            setPosition(0, 0);
            setDimensions(width_*value_,height_);
        }
        if(value_ != 0) setTiling(value_, 1.0);
    }

    void BarOverlayElement::setColor(int color){
        color_ = color;
        switch(color){
        case 0:
            setMaterialName("Orxonox/Red");
            break;
        case 1:
            setMaterialName("Orxonox/Yellow");
            break;
        case 2:
            setMaterialName("Orxonox/Green");
        }
    }

    float BarOverlayElement::getValue(){
        return(value_);
    }

    int BarOverlayElement::getBarColor(){
        return(color_);
    }
}

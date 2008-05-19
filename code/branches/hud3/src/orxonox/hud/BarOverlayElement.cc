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

#include <OgreOverlayManager.h>
#include <OgreOverlayElement.h>
#include <OgrePanelOverlayElement.h>
#include "GraphicsEngine.h"
#include "BarOverlayElement.h"

namespace orxonox
{
  using namespace Ogre;

    BarOverlayElement::BarOverlayElement(const String& name):Ogre::PanelOverlayElement(name){
        name_ = name;
    }

    BarOverlayElement::~BarOverlayElement(){}

    void BarOverlayElement::init(Real leftRel, Real topRel, Real widthRel, Real heightRel, Ogre::OverlayContainer* container){
        // init some values...
        container_ = container;
        om = &Ogre::OverlayManager::getSingleton();
        value_ = 0;
        color_ = 2;
        autoColor_ = true;
        dir_ = BarOverlayElement::RIGHT;

        // get window data...
        windowW_ = GraphicsEngine::getSingleton().getWindowWidth();
        windowH_ = GraphicsEngine::getSingleton().getWindowHeight();
        leftRel_ = leftRel;
        topRel_ = topRel;
        widthRel_ = widthRel;
        heightRel_ = heightRel;

        // cálculate absolute coordinates...
        left_ = leftRel_ * windowW_;
        top_ = topRel_ * windowH_;
        width_ = widthRel_ * windowW_;
        height_ = heightRel_ * windowH_;

        // create background...
        bar_ = static_cast<PanelOverlayElement*>(om->createOverlayElement("Panel", name_+"Bar"));
        bar_->show();
        container_->addChild(bar_);
        bar_->setPosition(left_, top_);
        bar_->setDimensions(width_, height_);
        bar_->setMetricsMode(Ogre::GMM_PIXELS);
        bar_->setMaterialName("Orxonox/Green");

        setPosition(left_,top_);
        setDimensions(width_,height_);
        setMetricsMode(Ogre::GMM_PIXELS);
        setMaterialName("Orxonox/BarBackground");
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
        switch(dir_){
        case BarOverlayElement::DOWN:
            bar_->setPosition(left_,top_);
            bar_->setDimensions(width_,height_*value_);
            break;
        case BarOverlayElement::LEFT:
            bar_->setPosition(left_+width_-width_*value_,top_);
            bar_->setDimensions(width_*value_,height_);
            break;
        case BarOverlayElement::UP:
            bar_->setPosition(left_,top_+height_-height_*value_);
            bar_->setDimensions(width_,height_*value_);
            break;
        default:
            bar_->setPosition(left_,top_);
            bar_->setDimensions(width_*value_,height_);
            break;
        }
    }

    void BarOverlayElement::setDir(int dir){
    }

    void BarOverlayElement::setColor(int color){
        color_ = color;
        switch(color){
        case 0:
            bar_->setMaterialName("Orxonox/Red");
            break;
        case 1:
            bar_->setMaterialName("Orxonox/Yellow");
            break;
        case 2:
            bar_->setMaterialName("Orxonox/Green");
        }
    }

    float BarOverlayElement::getValue(){
        return(value_);
    }

    int BarOverlayElement::getBarColor(){
        return(color_);
    }
}



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
 *      Yuning Chai
 *   Co-authors:
 *      Felix Schulthess
 *
 */

#include "OrxonoxStableHeaders.h"
#include "BarOverlayElement.h"
#include <OgreOverlayManager.h>
#include "GraphicsEngine.h"
#include "util/Math.h"

namespace orxonox
{
    using namespace Ogre;

    BarOverlayElement::BarOverlayElement(const String& name) : PanelOverlayElement(name)
    {
        name_ = name;
        widthratio_ = 100.0f / 800.0f; // calculates the ratio (backgroundwidth - barwidth) / backgroundwidth
    }

    BarOverlayElement::~BarOverlayElement()
    {
        OverlayManager::getSingleton().destroyOverlayElement(this->background_);
    }

    void BarOverlayElement::init(Real leftRel, Real topRel, Real dimRel, OverlayContainer* container){
        // init some values...
        value_ = 0;
        colour_ = 2;
        autoColour_ = true;
        right2Left_ = false; // default is left to right progress
        leftRel_ = leftRel;
        topRel_ = topRel;
        dimRel_ = dimRel;

        // create background...
        background_ = static_cast<OverlayContainer*>(OverlayManager::getSingleton().createOverlayElement("Panel", name_+"container"));
        background_->show();
        container->addChild(background_);
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
        // calculate new absolute coordinates...
        left_ = (int) (leftRel_ * GraphicsEngine::getSingleton().getWindowWidth());
        top_ = (int) (topRel_ * GraphicsEngine::getSingleton().getWindowHeight());
        width_ = (int) (dimRel_ * GraphicsEngine::getSingleton().getWindowWidth());
        height_ = (int) (0.1 * width_);	// the texture has dimensions height:length = 1:10
        offset_ = (int) (width_ * widthratio_ * 0.5);
        barwidth_ = (int) (width_ * (1.0f - widthratio_));

        // adapt background
        background_->setPosition(left_, top_);
        background_->setDimensions(width_, height_);
        // adapt bar
        setValue(value_);
    }

    void BarOverlayElement::setValue(float value){
        value_ = clamp<float>(value, 0, 1);
        // set colour, if nescessary
        if(autoColour_){
            if (value_>0.5) {setColour(BarOverlayElement::GREEN);}
            else if (value_>0.25) {setColour(BarOverlayElement::YELLOW);}
            else setColour(BarOverlayElement::RED);
        }

        // set value
        if(right2Left_){ // backward case
            setPosition(offset_ + barwidth_ * (1 - value_), 0);
            setDimensions(barwidth_ * value_, height_);
        }else{          // default case
            setPosition(offset_, 0);
            setDimensions(barwidth_ * value_, height_);
        }
        if(value_ != 0) setTiling(value_, 1.0);
    }

    void BarOverlayElement::setColour(int colour){
        colour_ = colour;
        switch(colour){
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
}

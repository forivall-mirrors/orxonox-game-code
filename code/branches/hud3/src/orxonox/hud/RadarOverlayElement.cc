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
*      ...
*
*/

/*	local coordinate system of space ship at the beginning:

			y
			+   z
			|  +
			| /
			|/
   x +------O
*/

#include <OgreOverlayManager.h>
#include <OgreOverlayElement.h>
#include <OgrePanelOverlayElement.h>
#include <OgreStringConverter.h>
#include "RadarOverlayElement.h"
#include "GraphicsEngine.h"

namespace orxonox
{
    using namespace Ogre;

    RadarOverlayElement::RadarOverlayElement(const String& name):Ogre::PanelOverlayElement(name){
    }

    RadarOverlayElement::~RadarOverlayElement(){
    }

    void RadarOverlayElement::initialise(){
        PanelOverlayElement::initialise();
    }

    void RadarOverlayElement::init(Real leftRel, Real topRel, Real dimRel, Ogre::OverlayContainer* container){
        count_ = 0;
        dimRel_ = dimRel;
        leftRel_ = leftRel;
        topRel_ = topRel;
        container_ = container;

        shipPos_ = Vector3(0.0, 0.0, 0.0);
        targetPos_ = Vector3(1337.0, 0.0, 0.0);
        initialDir_ = Vector3(1.0, 0.0, 0.0);
        currentDir_ = initialDir_;
        initialOrth_ = Vector3(0.0, 0.0, 1.0);
        currentOrth_ = initialOrth_;

        setMetricsMode(Ogre::GMM_PIXELS);
        setMaterialName("Orxonox/Radar");
        resize();

        om = &Ogre::OverlayManager::getSingleton();
        point = static_cast<PanelOverlayElement*>(om->createOverlayElement("Panel", "point"));
        point->show();
        container->addChild(point);
        point->setMaterialName("Orxonox/RedDot");
        point->setDimensions(5,5);
        point->setMetricsMode(Ogre::GMM_PIXELS);

    }

    void RadarOverlayElement::resize() {
        // if window is resized, we must adapt these...
        windowW_ = GraphicsEngine::getSingleton().getWindowWidth();
        windowH_ = GraphicsEngine::getSingleton().getWindowHeight();
        dim_ = dimRel_*windowH_;
        left_ = leftRel_*windowW_-dim_/2;
        top_ = topRel_*windowH_-dim_/2;
        setPosition(left_, top_);
        setDimensions(dim_,dim_);
    }

    void RadarOverlayElement::update() {
        resize();
        shipPos_ = SpaceShip::instance_s->getPosition();
        currentDir_ = SpaceShip::instance_s->getOrientation()*initialDir_; 		// according to beni....
		currentOrth_ = SpaceShip::instance_s->getOrientation()*initialOrth_;

		radius_ = acos((currentDir_.dotProduct(targetPos_ - shipPos_))/((targetPos_ - shipPos_).length()*currentDir_.length()));
        phi_ = acos((currentOrth_.dotProduct(targetPos_ - shipPos_))/((targetPos_ - shipPos_).length()*currentOrth_.length()));
        if((currentDir_.crossProduct(currentOrth_)).dotProduct(targetPos_ - shipPos_) > 0) right_ = true;
        else right_=false;

        if (right_){
            point->setPosition(sin(phi_)*radius_/3.5*dim_/2+dim_/2+left_-2,-cos(phi_)*radius_/3.5*dim_/2+dim_/2+top_-2);
        }
        else {
            point->setPosition(-sin(phi_)*radius_/3.5*dim_/2+dim_/2+left_-2,-cos(phi_)*radius_/3.5*dim_/2+dim_/2+top_-2);
        }
    }

}




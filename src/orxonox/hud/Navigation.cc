/*
*   ORXONOX - the hotnavText_ 3D action shooter ever to exist
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
*      Felix Schulthess
*   Co-authors:
*      ...
*
*/

#include <OgreOverlayManager.h>
#include <GraphicsEngine.h>
#include "objects/SpaceShip.h"
#include "HUD.h"
#include "Navigation.h"

namespace orxonox
{
    using namespace Ogre;

    Navigation::Navigation(OverlayContainer* container){
        container_ = container;
        focus_ = NULL;
        init();
    }

    Navigation::Navigation(OverlayContainer* container, RadarObject* focus){
        container_ = container;
        focus_ = focus;
        init();
    }

    void Navigation::init(){
		om = &OverlayManager::getSingleton();
        // create nav text
        navText_ = static_cast<TextAreaOverlayElement*>(om->createOverlayElement("TextArea", "navText"));
        navText_->show();
        navText_->setMetricsMode(Ogre::GMM_RELATIVE);
        navText_->setDimensions(0.3, 0.3);
        navText_->setPosition(0.02, 0.02);
        navText_->setFontName("Console");
        navText_->setCaption("init");
        container_->addChild(navText_);


        // create nav marker ...
        navMarker_ = static_cast<PanelOverlayElement*>(om->createOverlayElement("Panel", "NavMarker"));
        navMarker_->setMetricsMode(GMM_PIXELS);
        navMarker_->setMaterialName("Orxonox/NavMarker");
        navMarker_->setDimensions(16,16);
        navMarker_->hide();
        container_->addChild(navMarker_);
	}

	void Navigation::update(){
        if(focus_ == NULL) return;
        shipPos_ = SpaceShip::instance_s->getPosition();
        windowW_ = GraphicsEngine::getSingleton().getWindowWidth();
        windowH_ = GraphicsEngine::getSingleton().getWindowHeight();
        // from the angle we find out where to draw the marker
        // and which of the four arrows to take
        float r1 = atan((float)(windowW_)/(float)(windowH_));
        float phi = focus_->phi_;
        if(focus_->right_){
            if(phi<r1){
                navMarker_->setPosition(tan(phi)*windowH_/2+windowW_/2, 0);
                navMarker_->setUV(0.5, 0.0, 1.0, 0.5);
            }
            else if(phi>3.14-r1){
                navMarker_->setPosition(-tan(phi)*windowH_/2+windowW_/2, windowH_-16);
                navMarker_->setUV(0.0, 0.5, 0.5, 1.0);
            }
            else {
                navMarker_->setPosition(windowW_-16, -tan((3.14-2*phi)/2)*windowW_/2+windowH_/2);
                navMarker_->setUV(0.5, 0.5, 1.0, 1.0);
            }
        }
        else{
            if(phi<r1) {
                navMarker_->setPosition(-tan(phi)*windowH_/2+windowW_/2, 0);
                navMarker_->setUV(0.5, 0.0, 1.0, 0.5);
            }
            else if(phi>3.14-r1) {
                navMarker_->setPosition(tan(phi)*windowH_/2+windowW_/2, windowH_-16);
                navMarker_->setUV(0.0, 0.5, 0.5, 1.0);
            }
            else {
                navMarker_->setPosition(0, -tan((3.14-2*phi)/2)*windowW_/2+windowH_/2);
                navMarker_->setUV(0.0, 0.0, 0.5, 0.5);
            }
        }

        int d = (float)(getDist2Focus()/10);
        if(d) test->setCaption("Distance: " + Ogre::StringConverter::toString(d));
        else test->setCaption("");
    }

    void Navigation::cycleFocus(){
	    if(focus_ == NULL){
            focus_ = HUD::getSingleton().getFirstRadarObject();
	    }
        else{
            focus_->panel_->setMaterialName("Orxonox/RedDot");
            focus_ = focus_->next;
        }

        if(focus_ == NULL){
            navMarker_->hide();
        }
        else{
            navMarker_->show();
            focus_->panel_->setMaterialName("Orxonox/WhiteDot");
        }
	}

	float Navigation::getDist2Focus(){
	    if(focus_ == NULL) return(0.0);
	    return((focus_->pos_-shipPos_).length());
	}
}

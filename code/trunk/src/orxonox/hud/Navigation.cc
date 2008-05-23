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
#include <OgrePanelOverlayElement.h>
#include <OgreTextAreaOverlayElement.h>
#include <OgreStringConverter.h>
#include <OgreMatrix4.h>
#include <GraphicsEngine.h>
#include "core/Iterator.h"
#include "objects/SpaceShip.h"
#include "objects/CameraHandler.h"
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
//		for (Iterator<Model> it = ObjectList<Model>::begin(); it; ++it)
//		{
//            if(it->getPosition() == Vector(2000.0, 1000.0, 1000.0)){
//                COUT(3) << "TARGET FOUND\n";
//
//
//        }
		cam_ = NULL;
        // create nav text
        navText_ = static_cast<TextAreaOverlayElement*>(om->createOverlayElement("TextArea", "navText"));
        navText_->show();
        navText_->setMetricsMode(Ogre::GMM_PIXELS);
        navText_->setDimensions(0.001, 0.001);
        navText_->setPosition(0.02, 0.02);
        navText_->setFontName("Console");
        navText_->setCharHeight(20);
        navText_->setCaption("");
        container_->addChild(navText_);


        // create nav marker ...
        navMarker_ = static_cast<PanelOverlayElement*>(om->createOverlayElement("Panel", "NavMarker"));
        navMarker_->setMetricsMode(GMM_PIXELS);
        navMarker_->hide();
        navText_->hide();
        container_->addChild(navMarker_);
	}

	void Navigation::update(){
        if(focus_ == NULL) return;
        shipPos_ = SpaceShip::getLocalShip()->getPosition();
        windowW_ = GraphicsEngine::getSingleton().getWindowWidth();
        windowH_ = GraphicsEngine::getSingleton().getWindowHeight();
        updateMarker();
    }

    void Navigation::updateMarker(){
        // set text
        int dist = (float)(getDist2Focus()/100);
        navText_->setCaption(Ogre::StringConverter::toString(dist));

        if(cam_ == NULL) cam_ = SpaceShip::getLocalShip()->getCamera()->cam_;
        Vector3 pos = focus_->pos_;
        // transform to screen coordinates
        pos = cam_->getProjectionMatrix()*cam_->getViewMatrix()*pos;
        float xPosRel = 0.5*pos.x+0.5;
        float yPosRel = 1-(0.5*pos.y+0.5);
        int xPos = xPosRel*windowW_;
        int yPos = yPosRel*windowH_;
        // is object in view?
        bool outOfView = (xPosRel<0 || xPosRel>1 || yPosRel<0 || yPosRel>1);
        // if object is behind us, it is out of view anyway:
        if(!outOfView && focus_->radius_>3.14/2) outOfView = true;

        if(outOfView){
            // NO!
            navMarker_->setMaterialName("Orxonox/NavArrows");
            navMarker_->setDimensions(16,16);
            float phiUpRight = atan((float)(windowW_)/(float)(windowH_));
            // from the angle we find out where to draw the marker
            // and which of the four arrows to take
            float phi = focus_->phi_;
            if(focus_->right_){
                if(phi<phiUpRight){
                    // arrow up
                    navMarker_->setPosition(tan(phi)*windowH_/2+windowW_/2, 0);
                    navMarker_->setUV(0.5, 0.0, 1.0, 0.5);
                    navText_->setLeft(navMarker_->getLeft()+navMarker_->getWidth());
                    navText_->setTop(navMarker_->getHeight());
                }
                else if(phi>3.14-phiUpRight){
                    // arrow down
                    navMarker_->setPosition(-tan(phi)*windowH_/2+windowW_/2, windowH_-16);
                    navMarker_->setUV(0.0, 0.5, 0.5, 1.0);
                    navText_->setLeft(navMarker_->getLeft()+navMarker_->getWidth());
                    navText_->setTop(navMarker_->getTop()-navMarker_->getHeight());
                }
                else {
                    // arrow right
                    navMarker_->setPosition(windowW_-16, -tan((3.14-2*phi)/2)*windowW_/2+windowH_/2);
                    navMarker_->setUV(0.5, 0.5, 1.0, 1.0);
                    navText_->setLeft(navMarker_->getLeft()-navMarker_->getWidth());
                    navText_->setTop(navMarker_->getTop()+navMarker_->getHeight());
                }
            }
            else{
                if(phi<phiUpRight) {
                    // arrow up
                    navMarker_->setPosition(-tan(phi)*windowH_/2+windowW_/2, 0);
                    navMarker_->setUV(0.5, 0.0, 1.0, 0.5);
                    navText_->setLeft(navMarker_->getLeft()+navMarker_->getWidth());
                    navText_->setTop(navMarker_->getHeight());
                }
                else if(phi>3.14-phiUpRight) {
                    // arrow down
                    navMarker_->setPosition(tan(phi)*windowH_/2+windowW_/2, windowH_-16);
                    navMarker_->setUV(0.0, 0.5, 0.5, 1.0);
                    navText_->setLeft(navMarker_->getLeft()+navMarker_->getWidth());
                    navText_->setTop(navMarker_->getTop()-navMarker_->getHeight());
                }
                else {
                    // arrow left
                    navMarker_->setPosition(0, -tan((3.14-2*phi)/2)*windowW_/2+windowH_/2);
                    navMarker_->setUV(0.0, 0.0, 0.5, 0.5);
                    navText_->setLeft(navMarker_->getWidth());
                    navText_->setTop(navMarker_->getTop()+navMarker_->getHeight());
                }
            }
        }
        else{
            // YES!
            navMarker_->setMaterialName("Orxonox/NavTDC");
            navMarker_->setDimensions(24,24);
            navMarker_->setUV(0.0,0.0,1.0,1.0);
            navMarker_->setPosition(xPos-navMarker_->getWidth()/2, yPos-navMarker_->getHeight()/2);
            navText_->setPosition(xPos+navMarker_->getWidth()/2, yPos+navMarker_->getHeight()/2);
        }
	}

    void Navigation::cycleFocus(){
	    if(focus_ == NULL){
            focus_ = HUD::getSingleton().getFirstRadarObject();
	    }
        else{
            focus_->panel_->setMaterialName("Orxonox/RedDot");
            if(focus_ != NULL) focus_ = focus_->next;
        }

        if(focus_ == NULL){
            navMarker_->hide();
            navText_->hide();
        }
        else{
            navMarker_->show();
            navText_->show();
            focus_->panel_->setMaterialName("Orxonox/WhiteDot");
        }
	}

	float Navigation::getDist2Focus(){
	    if(focus_ == NULL) return(0.0);
	    return((focus_->pos_-shipPos_).length());
	}
}

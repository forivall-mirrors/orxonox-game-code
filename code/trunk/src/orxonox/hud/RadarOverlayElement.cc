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
#include <string.h>


#include <string.h>
#include <OgreOverlayManager.h>
#include <OgreStringConverter.h>
#include <OgrePanelOverlayElement.h>

#include "GraphicsEngine.h"
#include "core/Tickable.h"
#include "core/ConsoleCommand.h"
#include "objects/SpaceShip.h"
#include "RadarOverlayElement.h"

namespace orxonox
{

    using namespace Ogre;

    RadarOverlayElement::RadarOverlayElement(const String& name):PanelOverlayElement(name){
    }

    RadarOverlayElement::~RadarOverlayElement(){
    }

    void RadarOverlayElement::init(Real leftRel, Real topRel, Real dimRel, OverlayContainer* container){
        // some initial data
		om = &OverlayManager::getSingleton();
        dimRel_ = dimRel;
        leftRel_ = leftRel;
        topRel_ = topRel;
        container_ = container;
        firstRadarObject_ = NULL;
        lastRadarObject_ = NULL;
        focus_ = NULL;

        // create nav marker ...
        navMarker_ = static_cast<PanelOverlayElement*>(om->createOverlayElement("Panel", "NavMarker"));
        navMarker_->setMetricsMode(GMM_PIXELS);
        navMarker_->setMaterialName("Orxonox/NavMarker");
        navMarker_->setDimensions(16,16);
        navMarker_->setPosition(0,386);
        navMarker_->hide();
        container_->addChild(navMarker_);

        // these have to fit the data in the level
        shipPos_ = Vector3(0.0, 0.0, 0.0);
        initialDir_ = Vector3(1.0, 0.0, 0.0);
        currentDir_ = initialDir_;
        initialOrth_ = Vector3(0.0, 0.0, 1.0);
        currentOrth_ = initialOrth_;
        plane = Plane(currentDir_, shipPos_);

        setMetricsMode(GMM_PIXELS);
        setMaterialName("Orxonox/Radar");
        resize();

        container_->addChild(this);
    }

    void RadarOverlayElement::resize() {
        // if window is resized, we must adapt these...
        windowW_ = GraphicsEngine::getSingleton().getWindowWidth();
        windowH_ = GraphicsEngine::getSingleton().getWindowHeight();
        dim_ = (int) (dimRel_*windowH_);
        left_ = (int) (leftRel_*windowW_-dim_/2);
        top_ = (int) (topRel_*windowH_-dim_/2);
        setPosition(left_, top_);
        setDimensions(dim_,dim_);
    }

    void RadarOverlayElement::update() {
        shipPos_ = SpaceShip::instance_s->getPosition();
        currentDir_ = SpaceShip::instance_s->getOrientation()*initialDir_; 		// according to beni....
		currentOrth_ = SpaceShip::instance_s->getOrientation()*initialOrth_;
        plane = Plane(currentDir_, shipPos_);

        RadarObject* ro = firstRadarObject_;
        // iterate through all RadarObjects
		while(ro != NULL){
		    // calc position on radar...
            ro->radius_ = calcRadius(ro);
            ro->phi_ = calcPhi(ro);
            ro->right_ = calcRight(ro);

            // set size to fit distance...
            float d = (ro->pos_-shipPos_).length();
            if(d<4000) ro->panel_->setDimensions(4,4);
            else if(d<8000) ro->panel_->setDimensions(3,3);
            else if(d<16000) ro->panel_->setDimensions(2,2);
            else ro->panel_->setDimensions(1,1);

            if (ro->right_){
                ro->panel_->setPosition(sin(ro->phi_)*ro->radius_/
                    3.5*dim_/2+dim_/2+left_-2,-cos(ro->phi_)*ro->radius_/3.5*dim_/2+dim_/2+top_-2);
            }
            else {
                ro->panel_->setPosition(-sin(ro->phi_)*ro->radius_/
                    3.5*dim_/2+dim_/2+left_-2,-cos(ro->phi_)*ro->radius_/3.5*dim_/2+dim_/2+top_-2);
            }
            ro = ro->next;
		}
		updateNavMarker();
    }

    void RadarOverlayElement::updateNavMarker(){
        if(focus_ == NULL) return;
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
    }

    void RadarOverlayElement::addObject(Vector3 pos){
        if(firstRadarObject_ == NULL){
            firstRadarObject_ = new RadarObject(container_, pos);
            lastRadarObject_ = firstRadarObject_;
        }
        else{
            lastRadarObject_->next = new RadarObject(container_, pos);
            lastRadarObject_ = lastRadarObject_->next;
        }
	}

	void RadarOverlayElement::listObjects(){
	    int i = 0;
	    RadarObject* ro = firstRadarObject_;
	    COUT(3) << "List of RadarObjects:\n";
	    // iterate through all Radar Objects
	    while(ro != NULL) {
	        COUT(3) << i++ << ": " << ro->pos_ << std::endl;
	        ro = ro->next;
	    }
	}

	float RadarOverlayElement::getDist2Focus(){
	    if(focus_ == NULL) return(0.0);
	    return((focus_->pos_-shipPos_).length());
	}

	float RadarOverlayElement::calcRadius(RadarObject* obj){
	    return(acos((currentDir_.dotProduct(obj->pos_ - shipPos_))/
			((obj->pos_ - shipPos_).length()*currentDir_.length())));
	}

	float RadarOverlayElement::calcPhi(RadarObject* obj){
	    // project difference vector on our plane...
	    Vector3 proj = plane.projectVector(obj->pos_ - shipPos_);
	    // ...and find out the angle
	    return(acos((currentOrth_.dotProduct(proj))/
            (currentOrth_.length()*proj.length())));
	}

	bool RadarOverlayElement::calcRight(RadarObject* obj){
	    if((currentDir_.crossProduct(currentOrth_)).dotProduct(obj->pos_ - shipPos_) > 0)
        	return true;
        else return false;
	}

	void RadarOverlayElement::cycleFocus(){
	    if(focus_ == NULL){
            focus_ = firstRadarObject_;
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
}

/* my local clipboard...
COUT(3) << "WWWWWWWWWWWWWWWWWWWWWWWWWWWW\n";
COUT(3) << firstRadarObject_->radius_ << "  " << firstRadarObject_->phi_ << std::endl;
COUT(3) << "WWWWWWWWWWWWWWWWWWWWWWWWWWWW\n";
*/

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
 *      Felix Schulthess
 *   Co-authors:
 *      ...
 *
 */

#include "OrxonoxStableHeaders.h"
#include "Navigation.h"

#include <OgreOverlayManager.h>
#include <OgreStringConverter.h>

#include "GraphicsEngine.h"
// TODO: remove the SpaceShip and CameraHandler dependencies
#include "objects/SpaceShip.h"
#include "objects/CameraHandler.h"
#include "RadarObject.h"
#include "RadarOverlayElement.h"
#include "HUD.h"
#include "core/Debug.h"

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
        navCam_ = NULL;
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
        navCamPos_ = SpaceShip::getLocalShip()->getPosition();
        currentDir_ = SpaceShip::getLocalShip()->getDir();
        currentOrth_ = SpaceShip::getLocalShip()->getOrth();

        windowW_ = GraphicsEngine::getSingleton().getWindowWidth();
        windowH_ = GraphicsEngine::getSingleton().getWindowHeight();
        updateMarker();
    }

    void Navigation::updateMarker(){
        // set text
        int dist = (int) getDist2Focus()/100;
        navText_->setCaption(Ogre::StringConverter::toString(dist));

        if(navCam_ == NULL) navCam_ = SpaceShip::getLocalShip()->getCamera()->cam_;
        Vector3 pos = focus_->getPosition();
        // transform to screen coordinates
        pos = navCam_->getProjectionMatrix()*navCam_->getViewMatrix()*pos;
        float xPosRel = 0.5*pos.x+0.5;
        float yPosRel = 1-(0.5*pos.y+0.5);
        int xPos = (int) (xPosRel*windowW_);
        int yPos = (int) (yPosRel*windowH_);
        int xFromCenter = xPos-windowW_/2;
        int yFromCenter = yPos-windowH_/2;
        // is object in view?
        float radius = RadarOverlayElement::calcRadius(navCamPos_, currentDir_, currentOrth_, focus_);
        bool isRight = (currentDir_.crossProduct(currentOrth_)).dotProduct(focus_->getPosition() - navCamPos_)>0;
        bool isAbove = currentOrth_.dotProduct(focus_->getPosition() - navCamPos_)>0;
        bool outOfView = (xPosRel<0 || xPosRel>1 || yPosRel<0 || yPosRel>1);
        // if object is behind us, it is out of view anyway:
        if(!outOfView && radius>3.14/2) outOfView = true;

        if(outOfView){
            // object is not in view
            navMarker_->setMaterialName("Orxonox/NavArrows");
            navMarker_->setDimensions(16,16);
            float phiUpperCorner = atan((float)(windowW_)/(float)(windowH_));
            // from the angle we find out on which edge to draw the marker
            // and which of the four arrows to take
            float phiNav = atan((float) xFromCenter / (float) yFromCenter);

            if(isAbove && isRight){
                // top right quadrant
                if(-phiNav<phiUpperCorner){
                    //COUT(3) << "arrow up\n";
                    navMarker_->setPosition(-tan(phiNav)*windowH_/2+windowW_/2, 0);
                    navMarker_->setUV(0.5, 0.0, 1.0, 0.5);
                    navText_->setLeft(navMarker_->getLeft()+navMarker_->getWidth());
                    navText_->setTop(navMarker_->getHeight());
                }
                else {
                    //COUT(3) << "arrow right\n";
                    navMarker_->setPosition(windowW_-16, tan((3.14-2*phiNav)/2)*windowW_/2+windowH_/2);
                    navMarker_->setUV(0.5, 0.5, 1.0, 1.0);
                    navText_->setLeft(navMarker_->getLeft()-navMarker_->getWidth());
                    navText_->setTop(navMarker_->getTop()+navMarker_->getHeight());
                }
            }
            if(!isAbove && isRight){
                // bottom right quadrant
                if(phiNav<phiUpperCorner) {
                    //COUT(3) << "arrow down\n";
                    navMarker_->setPosition(tan(phiNav)*windowH_/2+windowW_/2, windowH_-16);
                    navMarker_->setUV(0.0, 0.5, 0.5, 1.0);
                    navText_->setLeft(navMarker_->getLeft()+navMarker_->getWidth());
                    navText_->setTop(navMarker_->getTop()-navMarker_->getHeight());
                }
                else {
                    //COUT(3) << "arrow right\n";
                    navMarker_->setPosition(windowW_-16, tan((3.14-2*phiNav)/2)*windowW_/2+windowH_/2);
                    navMarker_->setUV(0.5, 0.5, 1.0, 1.0);
                    navText_->setLeft(navMarker_->getLeft()-navMarker_->getWidth());
                    navText_->setTop(navMarker_->getTop()+navMarker_->getHeight());
                }
            }
            if(isAbove && !isRight){
                // top left quadrant
                if(phiNav<phiUpperCorner){
                    //COUT(3) << "arrow up\n";
                    navMarker_->setPosition(-tan(phiNav)*windowH_/2+windowW_/2, 0);
                    navMarker_->setUV(0.5, 0.0, 1.0, 0.5);
                    navText_->setLeft(navMarker_->getLeft()+navMarker_->getWidth());
                    navText_->setTop(navMarker_->getHeight());
                }
                else {
                    //COUT(3) << "arrow left\n";
                    navMarker_->setPosition(0, -tan((3.14-2*phiNav)/2)*windowW_/2+windowH_/2);
                    navMarker_->setUV(0.0, 0.0, 0.5, 0.5);
                    navText_->setLeft(navMarker_->getWidth());
                    navText_->setTop(navMarker_->getTop()+navMarker_->getHeight());
                }
            }
            if(!isAbove && !isRight){
                // bottom left quadrant
                if(phiNav>-phiUpperCorner) {
                    //COUT(3) << "arrow down\n";
                    navMarker_->setPosition(tan(phiNav)*windowH_/2+windowW_/2, windowH_-16);
                    navMarker_->setUV(0.0, 0.5, 0.5, 1.0);
                    navText_->setLeft(navMarker_->getLeft()+navMarker_->getWidth());
                    navText_->setTop(navMarker_->getTop()-navMarker_->getHeight());
                }
                else {
                    //COUT(3) << "arrow left\n";
                    navMarker_->setPosition(0, -tan((3.14-2*phiNav)/2)*windowW_/2+windowH_/2);
                    navMarker_->setUV(0.0, 0.0, 0.5, 0.5);
                    navText_->setLeft(navMarker_->getWidth());
                    navText_->setTop(navMarker_->getTop()+navMarker_->getHeight());
                }
            }
        }
        else{
            // object is in view
            navMarker_->setMaterialName("Orxonox/NavTDC");
            navMarker_->setDimensions(24,24);
            navMarker_->setUV(0.0, 0.0, 1.0, 1.0);
            navMarker_->setPosition(xPos-navMarker_->getWidth()/2, yPos-navMarker_->getHeight()/2);
            navText_->setPosition(xPos+navMarker_->getWidth()/2, yPos+navMarker_->getHeight()/2);
        }
    }

    void Navigation::cycleFocus(){
        if(focus_ == NULL){
        COUT(3) << "focus = null\n";
            it_ = HUD::getSingleton().roSet.begin();
            focus_ = *it_;
            ++it_;
        }
        else{
        COUT(3) << "focus = " << focus_ << std::endl;
            focus_->resetColour();
            if(it_ != HUD::getSingleton().roSet.end()){
                focus_ = *it_;
                ++it_;
            }
            else focus_ = NULL;
        }
        if(focus_ == NULL){
            navMarker_->hide();
            navText_->hide();
        }
        else{
            navMarker_->show();
            navText_->show();
            focus_->setColour(RadarObject::WHITE);
        }
    }

    float Navigation::getDist2Focus(){
        if(focus_ == NULL) return(0.0);
        return((focus_->getPosition()-SpaceShip::getLocalShip()->getPosition()).length());
    }
}

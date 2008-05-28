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
#include "RadarOverlayElement.h"

#include <string>
#include <OgreOverlayManager.h>
#include <OgreStringConverter.h>

#include "GraphicsEngine.h"
#include "core/Tickable.h"
#include "core/ConsoleCommand.h"
#include "objects/SpaceShip.h"
#include "RadarObject.h"
#include "HUD.h"

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
        shipPos_ = SpaceShip::getLocalShip()->getPosition();
        currentDir_ = SpaceShip::getLocalShip()->getDir();
        currentOrth_ = SpaceShip::getLocalShip()->getOrth();
        RadarObject* ro = HUD::getSingleton().getFirstRadarObject();
        // iterate through all RadarObjects
        while(ro != NULL){
        // calc position on radar...
            float radius = calcRadius(shipPos_, currentDir_, currentOrth_, ro);
            float phi = calcPhi(shipPos_, currentDir_, currentOrth_, ro);
            bool right = calcRight(shipPos_, currentDir_, currentOrth_, ro);

            // set size to fit distance...
            float d = (ro->getPosition()-shipPos_).length();
            if(d<4000) ro->panel_->setDimensions(4,4);
            else if(d<8000) ro->panel_->setDimensions(3,3);
            else if(d<16000) ro->panel_->setDimensions(2,2);
            else ro->panel_->setDimensions(1,1);

            if (right){
                ro->panel_->setPosition(sin(phi)*radius/
                    3.5*dim_/2+dim_/2+left_-2,-cos(phi)*radius/3.5*dim_/2+dim_/2+top_-2);
            }
            else {
                ro->panel_->setPosition(-sin(phi)*radius/
                    3.5*dim_/2+dim_/2+left_-2,-cos(phi)*radius/3.5*dim_/2+dim_/2+top_-2);
            }
            ro = ro->next;
        }
    }

    void RadarOverlayElement::listObjects(){
        int i = 0;
        RadarObject* ro = HUD::getSingleton().getFirstRadarObject();
        COUT(3) << "List of RadarObjects:\n";
        // iterate through all Radar Objects
        while(ro != NULL) {
            COUT(3) << i++ << ": " << ro->getPosition() << std::endl;
            ro = ro->next;
        }
    }

    float RadarOverlayElement::calcRadius(Vector3 pos, Vector3 dir, Vector3 orth, RadarObject* obj){
        return(acos((dir.dotProduct(obj->getPosition() - pos))/
        ((obj->getPosition() - pos).length()*dir.length())));
    }

    float RadarOverlayElement::calcPhi(Vector3 pos, Vector3 dir, Vector3 orth, RadarObject* obj){
        // project difference vector on our plane...
        Vector3 proj = Plane(dir, pos).projectVector(obj->getPosition() - pos);
        // ...and find out the angle
        return(acos((orth.dotProduct(proj))/
              (orth.length()*proj.length())));
    }

    bool RadarOverlayElement::calcRight(Vector3 pos, Vector3 dir, Vector3 orth, RadarObject* obj){
        if((dir.crossProduct(orth)).dotProduct(obj->getPosition() - pos) > 0)
            return true;
        else return false;
    }
}

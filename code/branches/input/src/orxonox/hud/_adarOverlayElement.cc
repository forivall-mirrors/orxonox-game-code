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
#include "RadarOverlayElement.h"

#include <string>
#include <OgreOverlayManager.h>
#include <OgreStringConverter.h>

#include "GraphicsEngine.h"
#include "core/ConsoleCommand.h"
#include "objects/Tickable.h"
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
        // iterate through all RadarObjects
        for(std::set<RadarObject*>::iterator it=HUD::getSingleton().roSet.begin(); it!=HUD::getSingleton().roSet.end(); it++){
        // calc position on radar...
            float radius = calcRadius(shipPos_, currentDir_, currentOrth_, (*it));
            float phi = calcPhi(shipPos_, currentDir_, currentOrth_, (*it));
            bool right = calcRight(shipPos_, currentDir_, currentOrth_, (*it));

            // set size to fit distance...
            float d = ((*it)->getPosition()-shipPos_).length();
            if(d<10000) (*it)->panel_->setDimensions(4,4);
            else if(d<20000) (*it)->panel_->setDimensions(3,3);
            else (*it)->panel_->setDimensions(2,2);

            if (right){
                (*it)->panel_->setPosition(sin(phi)*radius/
                    3.5*dim_/2+dim_/2+left_-2,-cos(phi)*radius/3.5*dim_/2+dim_/2+top_-2);
            }
            else {
                (*it)->panel_->setPosition(-sin(phi)*radius/
                    3.5*dim_/2+dim_/2+left_-2,-cos(phi)*radius/3.5*dim_/2+dim_/2+top_-2);
            }
        }
    }

    void RadarOverlayElement::listObjects(){
        int i = 0;
        COUT(3) << "List of RadarObjects:\n";
        // iterate through all Radar Objects
        for(std::set<RadarObject*>::iterator it=HUD::getSingleton().roSet.begin(); it!=HUD::getSingleton().roSet.end(); it++){
            COUT(3) << i++ << ": " << (*it)->getPosition() << std::endl;
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

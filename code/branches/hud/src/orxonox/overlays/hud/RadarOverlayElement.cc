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

#include "core/ConsoleCommand.h"
#include "objects/Tickable.h"
#include "objects/SpaceShip.h"
#include "util/Math.h"

#include "GraphicsEngine.h"
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
        dimRel_ = dimRel;
        leftRel_ = leftRel;
        topRel_ = topRel;

        setMetricsMode(GMM_PIXELS);
        setMaterialName("Orxonox/Radar");
        resize();

        container->addChild(this);
    }

    void RadarOverlayElement::resize() {
        // if window is resized, we must adapt these...
        dim_ = (int) (dimRel_*GraphicsEngine::getSingleton().getWindowHeight());
        left_ = (int) (leftRel_*GraphicsEngine::getSingleton().getWindowWidth()-dim_/2);
        top_ = (int) (topRel_*GraphicsEngine::getSingleton().getWindowHeight()-dim_/2);
        setPosition(left_, top_);
        setDimensions(dim_,dim_);
    }

    void RadarOverlayElement::update() {
        // iterate through all RadarObjects
        for(std::list<RadarObject*>::iterator it=HUD::getSingleton().getRadarObjects().begin(); it!=HUD::getSingleton().getRadarObjects().end(); it++)
        {
            // calc position on radar...
            // set size to fit distance...
            float distance = ((*it)->getPosition() - SpaceShip::getLocalShip()->getPosition()).length();
            if (distance > 20000) (*it)->getPanel()->setDimensions(1, 1);
            else if (distance > 10000) (*it)->getPanel()->setDimensions(2, 2);
            else if (distance > 5000) (*it)->getPanel()->setDimensions(3, 3);
            else if (distance > 2500) (*it)->getPanel()->setDimensions(4, 4);
            else if (distance > 1000) (*it)->getPanel()->setDimensions(5, 5);
            else (*it)->getPanel()->setDimensions(6,6);

            Vector2 coord = get2DViewcoordinates(SpaceShip::getLocalShip()->getPosition(), SpaceShip::getLocalShip()->getDir(), SpaceShip::getLocalShip()->getOrth(), (*it)->getPosition());
            coord = coord * Ogre::Math::PI / 3.5; // small adjustment to make it fit the texture
            float dimfactor = dim_ / 2.0;
            (*it)->getPanel()->setPosition((1 + coord.x) * dimfactor + left_ - 2,
                                           (1 - coord.y) * dimfactor + top_ - 2);
        }
    }

    void RadarOverlayElement::listObjects() const {
        int i = 0;
        COUT(3) << "List of RadarObjects:\n";
        // iterate through all Radar Objects
        for(std::list<RadarObject*>::const_iterator it=HUD::getSingleton().getRadarObjects().begin(); it!=HUD::getSingleton().getRadarObjects().end(); ++it){
            COUT(3) << i++ << ": " << (*it)->getPosition() << std::endl;
        }
    }
}

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
*      Felix Schulthess
*   Co-authors:
*      ...
*
*/

#include <OgreOverlayManager.h>
#include <OgreStringConverter.h>
#include "OrxonoxStableHeaders.h"
#include "GraphicsEngine.h"
#include "RadarObject.h"

namespace orxonox
{
    using namespace Ogre;

    int RadarObject::count = 0;		// initialize static variable

    RadarObject::RadarObject(OverlayContainer* container, SceneNode* node, int colour){
        container_ = container;
        node_ = node;
        colour_ = colour;
        next = NULL;
        om = &OverlayManager::getSingleton();
        panel_ = static_cast<PanelOverlayElement*>(om->createOverlayElement("Panel",
            "Object"+StringConverter::toString(count)));
        setColour(colour_);
        panel_->setDimensions(3,3);
        panel_->setMetricsMode(Ogre::GMM_PIXELS);
        panel_->show();
        index_ = count;
        count++;
        container_->addChild(panel_);
    }

    RadarObject::~RadarObject(){
        // todo: clean up stuff
    }

    void RadarObject::setColour(int colour){
        switch(colour){
        case RED: panel_->setMaterialName("Orxonox/RedDot"); break;
        case YELLOW: panel_->setMaterialName("Orxonox/YellowDot"); break;
        case GREEN: panel_->setMaterialName("Orxonox/GreenDot"); break;
        case BLUE: panel_->setMaterialName("Orxonox/BlueDot"); break;
        case WHITE: panel_->setMaterialName("Orxonox/WhiteDot"); break;
        default: panel_->setMaterialName("Orxonox/RedDot"); break;
        }
    }

    void RadarObject::resetColour(){
        setColour(colour_);
    }

    Vector3 RadarObject::getPosition(){
        return node_->getPosition();
    }
}


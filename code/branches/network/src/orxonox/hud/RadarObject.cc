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

#include "OrxonoxStableHeaders.h"
#include "RadarObject.h"

#include <OgreOverlayManager.h>
#include <OgreStringConverter.h>

namespace orxonox
{
    using namespace Ogre;

    int RadarObject::count = 0;		// initialize static variable

    RadarObject::RadarObject(OverlayContainer* container){
        container_ = container;
        pos_ = Vector3(0.0, 0.0, 0.0);
        init();
    }

    RadarObject::RadarObject(OverlayContainer* container, Vector3 pos){
        container_ = container;
        pos_ = pos;
        init();
    }

    RadarObject::~RadarObject(){}

    void RadarObject::init(){
        next = NULL;
        om = &OverlayManager::getSingleton();
        panel_ = static_cast<PanelOverlayElement*>(om->createOverlayElement("Panel",
          "Object"+StringConverter::toString(count)));
        panel_->setMaterialName("Orxonox/RedDot");
        panel_->setDimensions(3,3);
        panel_->setMetricsMode(Ogre::GMM_PIXELS);
        panel_->show();
        index_ = count;
        count++;
        container_->addChild(panel_);
    }
}

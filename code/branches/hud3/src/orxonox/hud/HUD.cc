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
#include <OgreOverlay.h>
#include <OgreOverlayContainer.h>
#include <OgreOverlayManager.h>
#include <OgreSceneNode.h>
#include <OgreEntity.h>
#include "core/Debug.h"
#include "objects/SpaceShip.h"
#include "HUD.h"
#include "BarOverlayElement.h"
#include "RadarOverlayElement.h"
#include "OverlayElementFactories.h"

namespace orxonox
{
    using namespace Ogre;

    HUD::HUD(int zoom){
        om = &Ogre::OverlayManager::getSingleton();

		// create Factories
        BarOverlayElementFactory *barOverlayElementFactory = new BarOverlayElementFactory();
        om->addOverlayElementFactory(barOverlayElementFactory);
        RadarOverlayElementFactory *radarOverlayElementFactory = new RadarOverlayElementFactory();
        om->addOverlayElementFactory(radarOverlayElementFactory);

        orxonoxHUD = om->create("Orxonox/HUD");
        container = static_cast<Ogre::OverlayContainer*>(om->createOverlayElement("Panel", "Orxonox/HUD/container"));
        // create energy bar
        energyBar = static_cast<BarOverlayElement*>(om->createOverlayElement("Bar", "energyBar"));
        energyBar->show();
        // create speedo bar
        speedoBar = static_cast<BarOverlayElement*>(om->createOverlayElement("Bar", "speedoBar"));
        speedoBar->show();
        // create radar
        radar = static_cast<RadarOverlayElement*>(om->createOverlayElement("Radar", "radar"));
        radar->show();

		// set up screen-wide container
        container->show();

        orxonoxHUD->add2D(container);
        orxonoxHUD->show();
        container->setLeft(0.0);
        container->setTop(0.0);
        container->setWidth(1.0);
        container->setHeight(1.0);
        container->setMetricsMode(Ogre::GMM_RELATIVE);
        energyBar->init(0.01, 0.94, 0.4, container);
        energyBar->setValue(1);
        speedoBar->init(0.01, 0.90, 0.4, container);
        radar->init(0.5, 0.9, 0.2, container);
        radar->addObject(Vector3(1500.0, 0.0, 0.0));
        radar->addObject(Vector3(0.0, 4000.0, 0.0));
        RadarOverlayElement::cycleFocus();
    }

    void HUD::tick(float dt)
    {
        energyBar->resize();

        float v = SpaceShip::instance_s->getVelocity().length();
        float vmax = SpaceShip::instance_s->getMaxSpeed();
        speedoBar->setValue(v/vmax);
        speedoBar->resize();

        radar->resize();
        radar->update();
    }

    HUD::~HUD(void){
    }
}







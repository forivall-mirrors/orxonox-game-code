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

#include "HUD.h"
#include "BarOverlayElement.h"
#include "RadarOverlayElement.h"
#include "OverlayElementFactories.h"
#include "DialogBox.h"

namespace orxonox
{
    using namespace Ogre;

    HUD::HUD(int zoom){
        om = &Ogre::OverlayManager::getSingleton();

        BarOverlayElementFactory *barOverlayElementFactory = new BarOverlayElementFactory();
        om->addOverlayElementFactory(barOverlayElementFactory);

        RadarOverlayElementFactory *radarOverlayElementFactory = new RadarOverlayElementFactory();
        om->addOverlayElementFactory(radarOverlayElementFactory);

        orxonoxHUD = om->create("Orxonox/HUD");

        container = static_cast<Ogre::OverlayContainer*>(om->createOverlayElement("Panel", "Orxonox/HUD/container"));

        energyCounter = static_cast<BarOverlayElement*>(om->createOverlayElement("Bar", "energyCounter"));
        energyCounter->show();

        speedo = static_cast<BarOverlayElement*>(om->createOverlayElement("Bar", "speedo"));
        speedo->show();

        radar = static_cast<RadarOverlayElement*>(om->createOverlayElement("Radar", "radar"));
        radar->show();

        container->show();
        orxonoxHUD->add2D(container);
        orxonoxHUD->show();
        container->setLeft(0.0);
        container->setTop(0.0);
        container->setWidth(1.0);
        container->setHeight(1.0);
        container->setMetricsMode(Ogre::GMM_RELATIVE);

        energyCounter->init(0.01, 0.95, 0.4, 0.04, container);
        energyCounter->setValue(1);

        speedo->init(0.01, 0.90, 0.4, 0.04, container);

        radar->init(0.5, 0.9, 0.2, container);

        Dial
    }

    void HUD::tick(float dt)
    {
        energyCounter->resize();

        float v = SpaceShip::instance_s->getVelocity().length();
        float vmax = SpaceShip::instance_s->getMaxSpeed();
        speedo->setValue(v/vmax);
        speedo->resize();

        radar->resize();
        radar->update();
    }

    HUD::~HUD(void){
    }
}







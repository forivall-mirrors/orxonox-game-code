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
#include "HUD.h"

#include <string>
#include <OgreOverlay.h>
#include <OgreOverlayContainer.h>
#include <OgreOverlayManager.h>
#include <OgreStringConverter.h>

#include "core/Debug.h"
#include "core/ConsoleCommand.h"
#include "objects/SpaceShip.h"
#include "GraphicsEngine.h"
#include "BarOverlayElement.h"
#include "RadarObject.h"
#include "RadarOverlayElement.h"
#include "Navigation.h"
#include "OverlayElementFactories.h"

namespace orxonox
{
    SetConsoleCommandShortcut(HUD, cycleNavigationFocus).setAccessLevel(AccessLevel::User);
    SetConsoleCommandShortcut(HUD, toggleFPS).setAccessLevel(AccessLevel::User);
    SetConsoleCommandShortcut(HUD, toggleRenderTime).setAccessLevel(AccessLevel::User);

    using namespace Ogre;

    HUD::HUD(){
        om = &Ogre::OverlayManager::getSingleton();
        sm = GraphicsEngine::getSingleton().getSceneManager();
        firstRadarObject = NULL;
        lastRadarObject = NULL;
        showFPS = true;
        showRenderTime = true;

        // create Factories
        BarOverlayElementFactory *barOverlayElementFactory = new BarOverlayElementFactory();
        om->addOverlayElementFactory(barOverlayElementFactory);
        RadarOverlayElementFactory *radarOverlayElementFactory = new RadarOverlayElementFactory();
        om->addOverlayElementFactory(radarOverlayElementFactory);

        orxonoxHUD = om->create("Orxonox/HUD");
        container = static_cast<Ogre::OverlayContainer*>(om->createOverlayElement("Panel", "Orxonox/HUD/container"));

        // creating text to display fps
        fpsText = static_cast<TextAreaOverlayElement*>(om->createOverlayElement("TextArea", "fpsText"));
        fpsText->show();
        fpsText->setMetricsMode(Ogre::GMM_PIXELS);
        fpsText->setDimensions(0.001, 0.001);
        fpsText->setPosition(10, 10);
        fpsText->setFontName("Console");
        fpsText->setCharHeight(20);
        fpsText->setCaption("init");

        // creating text to display render time ratio
        rTRText = static_cast<TextAreaOverlayElement*>(om->createOverlayElement("TextArea", "rTRText"));
        rTRText->show();
        rTRText->setMetricsMode(Ogre::GMM_PIXELS);
        rTRText->setDimensions(0.001, 0.001);
        rTRText->setPosition(10, 30);
        rTRText->setFontName("Console");
        rTRText->setCharHeight(20);
        rTRText->setCaption("init");

        // create energy bar
        energyBar = static_cast<BarOverlayElement*>(om->createOverlayElement("Bar", "energyBar"));
        energyBar->show();
        // create speedo bar
        speedoBar = static_cast<BarOverlayElement*>(om->createOverlayElement("Bar", "speedoBar"));
        speedoBar->show();
        // create radar
        radar = static_cast<RadarOverlayElement*>(om->createOverlayElement("Radar", "radar"));
        radar->show();

        // create Navigation
        nav = new Navigation(container);

        // set up screen-wide container
        container->show();

        orxonoxHUD->add2D(container);
        orxonoxHUD->show();
        container->setLeft(0.0);
        container->setTop(0.0);
        container->setWidth(1.0);
        container->setHeight(1.0);
        container->setMetricsMode(Ogre::GMM_RELATIVE);
        container->addChild(fpsText);
        container->addChild(rTRText);

        energyBar->init(0.01, 0.94, 0.4, container);
        energyBar->setValue(1);

        speedoBar->init(0.01, 0.90, 0.4, container);

        radar->init(0.5, 0.9, 0.2, container);
        SceneNode* node;
        node = sm->getRootSceneNode()->createChildSceneNode("tomato1", Vector3(2000.0, 0.0, 0.0));
        addRadarObject(node);
        node = sm->getRootSceneNode()->createChildSceneNode("tomato2", Vector3(0.0, 2000.0, 0.0));
        addRadarObject(node);
        node = sm->getRootSceneNode()->createChildSceneNode("tomato3", Vector3(0.0, 0.0, 2000.0));
        addRadarObject(node);
    }

    HUD::~HUD(){
        //todo: clean up objects
    }

    void HUD::tick(float dt)
    {
        energyBar->resize();

        if(!SpaceShip::getLocalShip())
          return;
        float v = SpaceShip::getLocalShip()->getVelocity().length();
        float vmax = SpaceShip::getLocalShip()->getMaxSpeed();
        speedoBar->setValue(v/vmax);
        speedoBar->resize();

        radar->resize();
        radar->update();

        nav->update();

        setFPS();
    }

    void HUD::setRenderTimeRatio(float ratio)
    {
        if(showRenderTime){
            rTRText->setCaption("Render time ratio: " + Ogre::StringConverter::toString(ratio));
        }
        else{
            rTRText->setCaption("");
            return;
        }
    }

    void HUD::setFPS(){
        if(showFPS){
            float fps = GraphicsEngine::getSingleton().getAverageFPS();
            fpsText->setCaption("FPS: " + Ogre::StringConverter::toString(fps));
        }
        else{
            fpsText->setCaption("");
            return;
        }
    }

    void HUD::addRadarObject(SceneNode* node, int colour){
        // check if this is the first RadarObject to create
        if(firstRadarObject == NULL){
            firstRadarObject = new RadarObject(container, node, colour);
            lastRadarObject = firstRadarObject;
        }
        else{ // if not, append to list
            lastRadarObject->next = new RadarObject(container, node, colour);
            lastRadarObject = lastRadarObject->next;
        }
    }

    RadarObject* HUD::getFirstRadarObject(){
        return firstRadarObject;
    }

    /*static*/ HUD& HUD::getSingleton(){
        static HUD theInstance;
        return theInstance;
    }

    /*static*/ void HUD::setEnergy(float value){
        HUD::getSingleton().energyBar->setValue(value);
    }

    /*static*/ void HUD::cycleNavigationFocus(){
        HUD::getSingleton().nav->cycleFocus();
    }

    /*static*/ void HUD::toggleFPS(){
        if(HUD::getSingleton().showFPS) HUD::getSingleton().showFPS = false;
        else HUD::getSingleton().showFPS = true;
    }

    /*static*/ void HUD::toggleRenderTime(){
        if(HUD::getSingleton().showRenderTime) HUD::getSingleton().showRenderTime = false;
        else HUD::getSingleton().showRenderTime = true;
    }
}







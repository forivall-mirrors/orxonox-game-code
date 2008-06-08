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
#include "HUD.h"

#include <string>
#include <set>
#include <OgreOverlay.h>
#include <OgreOverlayContainer.h>
#include <OgreOverlayManager.h>
#include <OgreStringConverter.h>

#include "core/Debug.h"
#include "core/ConsoleCommand.h"
#include "objects/SpaceShip.h"
#include "objects/WorldEntity.h"
#include "GraphicsEngine.h"
#include "BarOverlayElement.h"
#include "RadarObject.h"
#include "RadarOverlayElement.h"
#include "Navigation.h"
#include "OverlayElementFactories.h"

namespace orxonox
{
    SetConsoleCommandShortcut(HUD, cycleNavigationFocus).setAccessLevel(AccessLevel::User);
    SetConsoleCommandShortcut(HUD, releaseNavigationFocus).setAccessLevel(AccessLevel::User);
    SetConsoleCommandShortcut(HUD, toggleFPS).setAccessLevel(AccessLevel::User);
    SetConsoleCommandShortcut(HUD, toggleRenderTime).setAccessLevel(AccessLevel::User);

    using namespace Ogre;

    HUD::HUD()
    {
        showFPS_ = true;
        showRenderTime_ = true;

        // create Factories
        barOverlayElementFactory_ = new BarOverlayElementFactory();
        Ogre::OverlayManager::getSingleton().addOverlayElementFactory(barOverlayElementFactory_);
        radarOverlayElementFactory_ = new RadarOverlayElementFactory();
        Ogre::OverlayManager::getSingleton().addOverlayElementFactory(radarOverlayElementFactory_);

        orxonoxHUD_ = Ogre::OverlayManager::getSingleton().create("Orxonox/HUD");
        container_ = static_cast<Ogre::OverlayContainer*>(Ogre::OverlayManager::getSingleton().createOverlayElement("Panel", "Orxonox/HUD/container"));

        // creating text to display fps
        fpsText_ = static_cast<TextAreaOverlayElement*>(Ogre::OverlayManager::getSingleton().createOverlayElement("TextArea", "fpsText"));
        fpsText_->show();
        fpsText_->setMetricsMode(Ogre::GMM_PIXELS);
        fpsText_->setDimensions(0.001, 0.001);
        fpsText_->setPosition(10, 10);
        fpsText_->setFontName("Console");
        fpsText_->setCharHeight(20);
        fpsText_->setCaption("init");

        // creating text to display render time ratio
        rTRText_ = static_cast<TextAreaOverlayElement*>(Ogre::OverlayManager::getSingleton().createOverlayElement("TextArea", "rTRText"));
        rTRText_->show();
        rTRText_->setMetricsMode(Ogre::GMM_PIXELS);
        rTRText_->setDimensions(0.001, 0.001);
        rTRText_->setPosition(10, 30);
        rTRText_->setFontName("Console");
        rTRText_->setCharHeight(20);
        rTRText_->setCaption("init");

        // create energy bar
        energyBar_ = static_cast<BarOverlayElement*>(Ogre::OverlayManager::getSingleton().createOverlayElement("Bar", "energyBar"));
        energyBar_->show();
        // create speedo bar
        speedoBar_ = static_cast<BarOverlayElement*>(Ogre::OverlayManager::getSingleton().createOverlayElement("Bar", "speedoBar"));
        speedoBar_->show();
        // create radar
        radar_ = static_cast<RadarOverlayElement*>(Ogre::OverlayManager::getSingleton().createOverlayElement("Radar", "radar"));
        radar_->show();

        // create Navigation
        nav_ = new Navigation(container_);

        // set up screen-wide container
        container_->show();

        orxonoxHUD_->add2D(container_);
        orxonoxHUD_->show();
        container_->setLeft(0.0);
        container_->setTop(0.0);
        container_->setWidth(1.0);
        container_->setHeight(1.0);
        container_->setMetricsMode(Ogre::GMM_RELATIVE);
        container_->addChild(fpsText_);
        container_->addChild(rTRText_);

        energyBar_->init(0.01, 0.94, 0.4, container_);
        energyBar_->setValue(1);

        speedoBar_->init(0.01, 0.90, 0.4, container_);

        radar_->init(0.5, 0.9, 0.2, container_);

        WorldEntity* object;
        object = new WorldEntity();
        object->setPosition(2000.0, 0.0, 0.0);
        addRadarObject(object, ColourValue(0.5, 0, 0, 1));
        object = new WorldEntity();
        object->setPosition(0.0, 2000.0, 0.0);
        addRadarObject(object, ColourValue(0.5, 0, 0, 1));
        object = new WorldEntity();
        object->setPosition(0.0, 0.0, 2000.0);
        addRadarObject(object, ColourValue(0.5, 0, 0, 1));
        object = new WorldEntity();
        object->setPosition(10000.0,16000.0,0.0);
        addRadarObject(object);
    }

    HUD::~HUD()
    {
        Ogre::OverlayManager::getSingleton().destroyOverlayElement(this->container_);
        Ogre::OverlayManager::getSingleton().destroyOverlayElement(this->fpsText_);
        Ogre::OverlayManager::getSingleton().destroyOverlayElement(this->rTRText_);
        Ogre::OverlayManager::getSingleton().destroyOverlayElement(this->energyBar_);
        Ogre::OverlayManager::getSingleton().destroyOverlayElement(this->speedoBar_);
        Ogre::OverlayManager::getSingleton().destroyOverlayElement(this->radar_);

        delete this->nav_;
        delete this->barOverlayElementFactory_;
        delete this->radarOverlayElementFactory_;
    }

    void HUD::tick(float dt)
    {
        if(!SpaceShip::getLocalShip())
          return;

        float v = SpaceShip::getLocalShip()->getVelocity().length();
        float vmax = SpaceShip::getLocalShip()->getMaxSpeed();
        speedoBar_->setValue(v/vmax);

        radar_->update();
        nav_->update();

        setFPS();
    }

    void HUD::resize()
    {
        this->speedoBar_->resize();
        this->energyBar_->resize();
        this->radar_->resize();
    }

    void HUD::setRenderTimeRatio(float ratio)
    {
        if(showRenderTime_){
            rTRText_->setCaption("Render time ratio: " + Ogre::StringConverter::toString(ratio));
        }
        else{
            rTRText_->setCaption("");
            return;
        }
    }

    void HUD::setFPS(){
        if(showFPS_){
            float fps = GraphicsEngine::getSingleton().getAverageFPS();
            fpsText_->setCaption("FPS: " + Ogre::StringConverter::toString(fps));
        }
        else{
            fpsText_->setCaption("");
            return;
        }
    }

    void HUD::addRadarObject(WorldEntity* object, const ColourValue& colour){
        RadarObject* obj = new RadarObject(container_, object, colour);
        roSet_.insert(roSet_.end(), obj);
//        // check if this is the first RadarObject to create
//        if(firstRadarObject == NULL){
//            firstRadarObject = new RadarObject(container_, object, colour);
//            lastRadarObject = firstRadarObject;
//        }
//        else{ // if not, append to list
//            lastRadarObject->next = new RadarObject(container_, object, colour);
//            lastRadarObject = lastRadarObject->next;
//        }
    }

    void HUD::removeRadarObject(WorldEntity* object){
        for(std::list<RadarObject*>::iterator it=roSet_.begin(); it!=roSet_.end(); ++it){
            if ((*it)->getObject() == object)
            {
                if (this->nav_->getFocus() == (*it))
                    this->nav_->setFocus(0);

                delete (*it);
                roSet_.erase(it);
                return;
            }
        }
    }

    /*static*/ HUD& HUD::getSingleton(){
        static HUD theInstance;
        return theInstance;
    }

    /*static*/ void HUD::setEnergy(float value){
        HUD::getSingleton().energyBar_->setValue(value);
    }

    /*static*/ void HUD::cycleNavigationFocus(){
        HUD::getSingleton().nav_->cycleFocus();
    }

    /*static*/ void HUD::releaseNavigationFocus(){
        HUD::getSingleton().nav_->setFocus(0);
    }

    /*static*/ void HUD::toggleFPS(){
        HUD::getSingleton().showFPS_ = !HUD::getSingleton().showFPS_;
    }

    /*static*/ void HUD::toggleRenderTime(){
        HUD::getSingleton().showRenderTime_ = !HUD::getSingleton().showRenderTime_;
    }
}

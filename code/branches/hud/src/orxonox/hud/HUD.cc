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
#include <assert.h>
#include <OgreOverlay.h>
#include <OgreOverlayContainer.h>
#include <OgreOverlayManager.h>
#include <OgreStringConverter.h>

#include "core/Debug.h"
#include "core/ConsoleCommand.h"
#include "core/CoreIncludes.h"
#include "objects/SpaceShip.h"
#include "objects/WorldEntity.h"
#include "GraphicsEngine.h"
#include "HUDBar.h"
#include "RadarObject.h"
#include "RadarOverlayElement.h"
#include "Navigation.h"

namespace orxonox
{
    CreateFactory(HUD);

    SetConsoleCommandShortcut(HUD, cycleNavigationFocus).setAccessLevel(AccessLevel::User);
    SetConsoleCommandShortcut(HUD, releaseNavigationFocus).setAccessLevel(AccessLevel::User);
    SetConsoleCommand(HUD, toggleVisibility, false).setAccessLevel(AccessLevel::User);

    HUD* HUD::instance_s = 0;

    using namespace Ogre;

    HUD::HUD()
    {
        assert(instance_s == 0);
        instance_s = this;
        RegisterObject(HUD);

        // Singleton like in Ogre. Constructor and destructor are public,
        // but the assert prevents from having multiple instances.

        orxonoxHUD_ = 0;
        container_ = 0;
        fpsText_ = 0;
        rTRText_ = 0;
        energyBar_ = 0;
        speedoBar_ = 0;
        radar_ = 0;
        nav_ = 0;
        showFPS_ = true;
        showRenderTime_ = true;
    }

    HUD::~HUD()
    {
        if (this->isInitialized())
        {
            if (this->container_)
                Ogre::OverlayManager::getSingleton().destroyOverlayElement(this->container_);
            this->container_ = 0;
            if (this->fpsText_)
                Ogre::OverlayManager::getSingleton().destroyOverlayElement(this->fpsText_);
            this->fpsText_ = 0;
            if (this->rTRText_)
                Ogre::OverlayManager::getSingleton().destroyOverlayElement(this->rTRText_);
            this->rTRText_ = 0;
            if (this->energyBar_)
                delete this->energyBar_;
            this->energyBar_ = 0;
            /*if (this->speedoBar_)
                Ogre::OverlayManager::getSingleton().destroyOverlayElement(this->speedoBar_);*/
            this->speedoBar_ = 0;
            if (this->radar_)
                Ogre::OverlayManager::getSingleton().destroyOverlayElement(this->radar_);
            this->radar_ = 0;
            if (this->orxonoxHUD_)
                Ogre::OverlayManager::getSingleton().destroy(this->orxonoxHUD_);
            this->orxonoxHUD_ = 0;
        }

        instance_s = 0;
    }

    void HUD::XMLPort(Element& xmlElement, XMLPort::Mode mode)
    {
        BaseObject::XMLPort(xmlElement, mode);

        showFPS_ = true;
        showRenderTime_ = true;

        XMLPortObject(HUD, HUDOverlay, "", addHUDElement, getHUDElement, xmlElement, mode, false, true);

        // create Factories
        Ogre::OverlayManager::getSingleton().addOverlayElementFactory(&radarOverlayElementFactory_);

        // set up screen-wide container
        container_ = static_cast<Ogre::OverlayContainer*>(Ogre::OverlayManager::getSingleton().createOverlayElement("Panel", "Orxonox/HUD/container"));
        container_->setLeft(0.0);
        container_->setTop(0.0);
        container_->setWidth(1.0);
        container_->setHeight(1.0);
        container_->setMetricsMode(Ogre::GMM_RELATIVE);

        orxonoxHUD_ = Ogre::OverlayManager::getSingleton().create("Orxonox/HUD");
        orxonoxHUD_->add2D(container_);

        // create radar
        radar_ = static_cast<RadarOverlayElement*>(Ogre::OverlayManager::getSingleton().createOverlayElement("Radar", "radar"));
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

        orxonoxHUD_->show();
    }

    void HUD::addHUDElement(HUDOverlay* element)
    {
        if (hudElements_.find(element->getName()) != hudElements_.end())
        {
          COUT(1) << "Ambiguous names encountered while load the HUD overlays" << std::endl;
        }
        else
          hudElements_[element->getName()] = element;
    }

    HUDOverlay* HUD::getHUDElement(unsigned int index)
    {
        if (index < this->hudElements_.size())
        {
          std::map<std::string, HUDOverlay*>::const_iterator it = hudElements_.begin();
          for (unsigned int i = 0; i != index; ++it, ++i)
            ;
          return (*it).second;
        }
        else
            return 0;
    }

    void HUD::tick(float dt)
    {
        radar_->update();
    }

    void HUD::windowResized(int newWidth, int newHeight)
    {
        this->radar_->resize();
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
                /*if (this->nav_ && this->nav_->getFocus() == (*it))
                    this->nav_->releaseFocus();*/

                delete (*it);
                roSet_.erase(it);
                return;
            }
        }
    }

    /*static*/ HUD& HUD::getSingleton()
    {
        assert(instance_s);
        return *instance_s;
    }

    /*static*/ void HUD::toggleVisibility(const std::string& name)
    {
        if (HUD::getSingleton().hudElements_.find(name) != HUD::getSingleton().hudElements_.end())
        {
            HUD::getSingleton().hudElements_[name]->setVisibility(!HUD::getSingleton().hudElements_[name]->isVisible());
        }
    }

    /*static*/ void HUD::setEnergy(float value){
        HUD::getSingleton().energyBar_->setValue(value);
    }

    /*static*/ void HUD::cycleNavigationFocus()
    {
        if (HUD::getSingleton().hudElements_.find("Navigation") != HUD::getSingleton().hudElements_.end())
        {
            Navigation* navi = dynamic_cast<Navigation*>(HUD::getSingleton().hudElements_["Navigation"]);
            navi->cycleFocus();
        }
    }

    /*static*/ void HUD::releaseNavigationFocus(){
        //HUD::getSingleton().nav_->releaseFocus();
    }
}

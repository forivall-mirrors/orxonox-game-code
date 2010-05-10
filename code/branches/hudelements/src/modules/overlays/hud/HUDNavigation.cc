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
 *      Felix Schulthess
 *   Co-authors:
 *      Reto Grieder
 *
 */

#include "HUDNavigation.h"
#include <utility>

#include <string>
#include <OgreCamera.h>
#include <OgreOverlayManager.h>
#include <OgreTextAreaOverlayElement.h>
#include <OgrePanelOverlayElement.h>
#include "util/Math.h"
#include "util/Convert.h"
#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "CameraManager.h"
#include "Scene.h"
#include "Radar.h"
#include "graphics/Camera.h"
#include "controllers/HumanController.h"
#include "worldentities/pawns/Pawn.h"
#include "worldentities/WorldEntity.h"

namespace orxonox
{
CreateFactory(HUDNavigation);

HUDNavigation::HUDNavigation(BaseObject* creator)
        : OrxonoxOverlay(creator)
{
    RegisterObject(HUDNavigation);
    
     
     const std::set<RadarViewable*>& respawnObjectSet_ = this->getOwner()->getScene()->getRadar()->getRadarObjects();
     std::set<RadarViewable*>::const_iterator respawnObjectSetIt_;
     
     if(!respawnObjectSet_.empty()){
     for(respawnObjectSetIt_ = respawnObjectSet_.begin(); respawnObjectSetIt_ != respawnObjectSet_.end();
	 ++respawnObjectSetIt_){ addObject(*respawnObjectSetIt_);}
     }
    
//         // create nav text
//         navText_ = static_cast<Ogre::TextAreaOverlayElement*>(Ogre::OverlayManager::getSingleton()
//             .createOverlayElement("TextArea", "HUDNavigation_navText_" + getUniqueNumberString()));
//
//         // create nav marker
//         navMarker_ = static_cast<Ogre::PanelOverlayElement*>(Ogre::OverlayManager::getSingleton()
//             .createOverlayElement("Panel", "HUDNavigation_navMarker_" + getUniqueNumberString()));
//         navMarker_->setMaterialName("Orxonox/NavArrows");

    /*
            // create aim marker
            aimMarker_ = static_cast<Ogre::PanelOverlayElement*>(Ogre::OverlayManager::getSingleton()
                .createOverlayElement("Panel", "HUDNavigation_aimMarker_" + getUniqueNumberString()));
            aimMarker_->setMaterialName("Orxonox/NavCrosshair");
            this->wasOutOfView_ = true; // Ensure the material is changed right the first time..


    */
    /*
            background_->addChild(navMarker_);*/
//        background_->addChild(aimMarker_);
//         background_->addChild(navText_);

    // hide at first
//         this->setVisible(false);


    setFont("Monofur");
    setTextSize(0.05f);
    setNavMarkerSize(0.05f);
//         setAimMarkerSize(0.04f);
}

HUDNavigation::~HUDNavigation()
{

    if (this->isInitialized())
    {
        if (!activeObjectList_.empty())
        {
            for (tempRadarViewable = activeObjectList_.begin(); tempRadarViewable!=activeObjectList_.end(); ++tempRadarViewable)
            {
                Ogre::OverlayManager::getSingleton().destroyOverlayElement(tempRadarViewable->second.first);
                Ogre::OverlayManager::getSingleton().destroyOverlayElement(tempRadarViewable->second.second);
//            Ogre::OverlayManager::getSingleton().destroyOverlayElement(this->aimMarker_);
            }
        }
    }
}

void HUDNavigation::XMLPort(Element& xmlElement, XMLPort::Mode mode)
{
    SUPER(HUDNavigation, XMLPort, xmlElement, mode);

    XMLPortParam(HUDNavigation, "font",     setFont,     getFont,     xmlElement, mode);
    XMLPortParam(HUDNavigation, "textSize", setTextSize, getTextSize, xmlElement, mode);
    XMLPortParam(HUDNavigation, "navMarkerSize", setNavMarkerSize, getNavMarkerSize, xmlElement, mode);
//        XMLPortParam(HUDNavigation, "aimMarkerSize", setAimMarkerSize, getAimMarkerSize, xmlElement, mode);
}

void HUDNavigation::setFont(const std::string& font)
{
    fontName_ = font;
    if (!activeObjectList_.empty())
    {
        for (tempRadarViewable = activeObjectList_.begin(); tempRadarViewable!=activeObjectList_.end(); ++tempRadarViewable)
        {
            if (tempRadarViewable->second.second && !fontName_.empty())
                tempRadarViewable->second.second->setFontName(fontName_);
        }
    }
}

const std::string& HUDNavigation::getFont() const
{
    return fontName_;
}

void HUDNavigation::setTextSize(float size)
{
    textSize_ = size;
    if (!activeObjectList_.empty())
    {
        for (tempRadarViewable = activeObjectList_.begin(); tempRadarViewable!=activeObjectList_.end(); ++tempRadarViewable)
        {
            if (tempRadarViewable->second.second && size >= 0.0f)
                tempRadarViewable->second.second->setCharHeight(size);
        }
    }
}

float HUDNavigation::getTextSize() const
{
    return textSize_;
}

void HUDNavigation::tick(float dt)
{
    SUPER(HUDNavigation, tick, dt);

// 	updateActiveObjectList(activeObjectList_);

    // Get radar
//         Radar* radar = this->getOwner()->getScene()->getRadar();
//
//         if (!radar->getFocus())
//         {
//             this->overlay_->hide();
//             return;
//         }
//         else
//         {
//             this->overlay_->show();
//         }

    // set text
//         int dist = static_cast<int>(getDist2Focus());
//         navText_->setCaption(multi_cast<std::string>(dist));
//         float textLength = multi_cast<std::string>(dist).size() * navText_->getCharHeight() * 0.3f;

    if (!activeObjectList_.empty()) {
        for (tempRadarViewable = activeObjectList_.begin(); tempRadarViewable!=activeObjectList_.end(); ++tempRadarViewable)
        {
            COUT(0) << "check 174" << std::endl;

            //get Distance to HumanController and save it in the TextAreaOverlayElement.
            int dist = (int)(tempRadarViewable->first->getRVWorldPosition() - HumanController::getLocalControllerEntityAsPawn()->getWorldPosition()).length();
            tempRadarViewable->second.second->setCaption(multi_cast<std::string>(dist));
            float textLength = multi_cast<std::string>(dist).size() * tempRadarViewable->second.second->getCharHeight() * 0.3f;

            COUT(0) << "check 181" << std::endl;

            orxonox::Camera* cam = CameraManager::getInstance().getActiveCamera();
            if (!cam)
                return;
            const Matrix4& transform = cam->getOgreCamera()->getProjectionMatrix() * cam->getOgreCamera()->getViewMatrix();
            // transform to screen coordinates
            Vector3 pos = transform * tempRadarViewable->first->getRVWorldPosition();

            COUT(0) << "check 190" << std::endl;

            bool outOfView;
            if (pos.z > 1.0)
            {
                // z > 1.0 means that the object is behind the camera
                outOfView = true;
                // we have to switch all coordinates (if you don't know why,
                // try linear algebra lectures, because I can't explain..)
                pos.x = -pos.x;
                pos.y = -pos.y;
            }
            else
                outOfView = pos.x < -1.0 || pos.x > 1.0 || pos.y < -1.0 || pos.y > 1.0;

            if (outOfView)
            {
                // object is not in view
//            aimMarker_->hide();

                if (!wasOutOfView_)
                {
                    tempRadarViewable->second.first->setMaterialName("Orxonox/NavArrows");
                    wasOutOfView_ = true;
                }

                COUT(0) << "check 174" << std::endl;

                if (pos.x < pos.y)
                {
                    if (pos.y > -pos.x)
                    {
                        // up
                        float position = pos.x / pos.y + 1.0f;
                        tempRadarViewable->second.first->setPosition((position - tempRadarViewable->second.first->getWidth()) * 0.5f, 0.0f);
                        tempRadarViewable->second.first->setUV(0.5f, 0.0f, 1.0f, 0.5f);
                        tempRadarViewable->second.second->setLeft((position - textLength) * 0.5f);
                        tempRadarViewable->second.second->setTop(tempRadarViewable->second.first->getHeight());
                    }
                    else
                    {
                        // left
                        float position = pos.y / pos.x + 1.0f;
                        tempRadarViewable->second.first->setPosition(0.0f, (position - tempRadarViewable->second.first->getWidth()) * 0.5f);
                        tempRadarViewable->second.first->setUV(0.0f, 0.0f, 0.5f, 0.5f);
                        tempRadarViewable->second.second->setLeft(tempRadarViewable->second.first->getWidth() + 0.01f);
                        tempRadarViewable->second.second->setTop((position - tempRadarViewable->second.second->getCharHeight()) * 0.5f);
                    }
                }
                else
                {

                    if (pos.y < -pos.x)
                    {
                        // down
                        float position = -pos.x / pos.y + 1.0f;
                        tempRadarViewable->second.first->setPosition((position - tempRadarViewable->second.first->getWidth()) * 0.5f, 1.0f - tempRadarViewable->second.first->getHeight());
                        tempRadarViewable->second.first->setUV(0.0f, 0.5f, 0.5f, 1.0f);
                        tempRadarViewable->second.second->setLeft((position - textLength) * 0.5f);
                        tempRadarViewable->second.second->setTop(1.0f - tempRadarViewable->second.first->getHeight() - tempRadarViewable->second.second->getCharHeight());
                    }
                    else
                    {
                        // right
                        float position = -pos.y / pos.x + 1.0f;
                        tempRadarViewable->second.first->setPosition(1.0f - tempRadarViewable->second.first->getWidth(), (position - tempRadarViewable->second.first->getHeight()) * 0.5f);
                        tempRadarViewable->second.first->setUV(0.5f, 0.5f, 1.0f, 1.0f);
                        tempRadarViewable->second.second->setLeft(1.0f - tempRadarViewable->second.first->getWidth() - textLength - 0.01f);
                        tempRadarViewable->second.second->setTop((position - tempRadarViewable->second.second->getCharHeight()) * 0.5f);
                    }
                }
            }
            else
            {
                COUT(0) << "check 264" << std::endl;

                // object is in view
                /*
                            Vector3 aimpos = transform * getPredictedPosition(SpaceShip::getLocalShip()->getPosition(),
                                    Projectile::getSpeed(), Radar::getInstance().getFocus()->getRVWorldPosition(), Radar::getInstance().getFocus()->getRVOrientedVelocity());
                */
                if (wasOutOfView_)
                {
                    tempRadarViewable->second.first->setMaterialName("Orxonox/NavTDC");
                    wasOutOfView_ = false;
                }

                // object is in view
                tempRadarViewable->second.first->setUV(0.0f, 0.0f, 1.0f, 1.0f);
                tempRadarViewable->second.first->setLeft((pos.x + 1.0f - tempRadarViewable->second.first->getWidth()) * 0.5f);
                tempRadarViewable->second.first->setTop((-pos.y + 1.0f - tempRadarViewable->second.first->getHeight()) * 0.5f);


//                 aimMarker_->show();
//                 aimMarker_->setLeft((aimpos.x + 1.0f - aimMarker_->getWidth()) * 0.5f);
//                 aimMarker_->setTop((-aimpos.y + 1.0f - aimMarker_->getHeight()) * 0.5f);
//
                tempRadarViewable->second.second->setLeft((pos.x + 1.0f + tempRadarViewable->second.first->getWidth()) * 0.5f);
                tempRadarViewable->second.second->setTop((-pos.y + 1.0f + tempRadarViewable->second.first->getHeight()) * 0.5f);
            }
	          
    tempRadarViewable->second.first->show();
    tempRadarViewable->second.second->show();
    }

    }

}


float HUDNavigation::getDist2Focus() const {

    Radar* radar = this->getOwner()->getScene()->getRadar();
    if (radar->getFocus() && HumanController::getLocalControllerEntityAsPawn())
        return (radar->getFocus()->getRVWorldPosition() - HumanController::getLocalControllerEntityAsPawn()->getWorldPosition()).length();
    else
        return 0;
}

/**
@brief Overridden method of OrxonoxOverlay. Usually the entire overlay
       scales with scale(). Here we obviously have to adjust this.
*/
void HUDNavigation::sizeChanged() {
    // use size to compensate for aspect ratio if enabled.
    float xScale = this->getActualSize().x;
    float yScale = this->getActualSize().y;

    if (!activeObjectList_.empty())
    {
        for (tempRadarViewable = activeObjectList_.begin(); tempRadarViewable!=activeObjectList_.end(); ++tempRadarViewable)
        {

            if (tempRadarViewable->second.first)
                tempRadarViewable->second.first->setDimensions(navMarkerSize_ * xScale, navMarkerSize_ * yScale);
//            if (this->aimMarker_)
//            aimMarker_->setDimensions(aimMarkerSize_ * xScale, aimMarkerSize_ * yScale);
            if (tempRadarViewable->second.second)
                tempRadarViewable->second.second->setCharHeight(tempRadarViewable->second.second->getCharHeight() * yScale);
        }
    }
}


void HUDNavigation::addObject(RadarViewable* object) {
    if (object == dynamic_cast<RadarViewable*>(this->getOwner()))
        return;

    assert(object);

    // Make sure the object hasn't been added yet
    assert( this->activeObjectList_.find(object) == this->activeObjectList_.end() );

    // Create everything needed to display the object on the radar and add it to the map

    // create nav marker
    Ogre::PanelOverlayElement* panel = static_cast<Ogre::PanelOverlayElement*>(Ogre::OverlayManager::getSingleton()
                                       .createOverlayElement("Panel", "HUDNavigation_navMarker_" + getUniqueNumberString()));

    panel->setMaterialName("Orxonox/NavArrows");

    Ogre::TextAreaOverlayElement* text = static_cast<Ogre::TextAreaOverlayElement*>(Ogre::OverlayManager::getSingleton()
                                         .createOverlayElement("TextArea", "HUDNavigation_navText_" + getUniqueNumberString()));

    float xScale = this->getActualSize().x;
    float yScale = this->getActualSize().y;

    panel->setDimensions(navMarkerSize_ * xScale, navMarkerSize_ * yScale);
    text->setCharHeight(text->getCharHeight() * yScale);

    activeObjectList_[object] = std::make_pair (panel, text) ;

    this->background_->addChild(panel);
    this->background_->addChild(text);

// 	background_->addChild(activeObjectList_[object].first);
// 	background_->addChild(activeObjectList_[object].second);
    COUT(0) << "check 357" << std::endl;
}

void HUDNavigation::removeObject(RadarViewable* viewable)
{
    activeObjectListType::iterator it = activeObjectList_.find(viewable);

    if (activeObjectList_.find(viewable) != activeObjectList_.end())
    {
        // Remove overlays from Ogre
        this->background_->removeChild(it->second.first->getName());
        this->background_->removeChild(it->second.second->getName());

        activeObjectList_.erase(viewable);
    }
}

// 	void updateActiveObjectList(map activeObjectList_){}
//
// 	void HUDNavigation::objectChanged(RadarViewable* viewable){}
//
//
//         float HUDNavigation::getRadarSensitivity(){}
//         void HUDNavigation::radarTick(float dt){}


}



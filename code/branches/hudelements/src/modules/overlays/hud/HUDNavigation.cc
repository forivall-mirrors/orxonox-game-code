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
#include "interfaces/RadarViewable.h"

namespace orxonox
{
CreateFactory(HUDNavigation);

HUDNavigation::HUDNavigation(BaseObject* creator)
        : OrxonoxOverlay(creator)
{
    RegisterObject(HUDNavigation);

    setFont("Monofur");
    setTextSize(0.05f);
    setNavMarkerSize(0.05f);


    
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



//         setAimMarkerSize(0.04f);
    
}

HUDNavigation::~HUDNavigation()
{
    if (this->isInitialized())
    {
        for (activeObjectListType::iterator it = activeObjectList_.begin(); it != activeObjectList_.end();)
            removeObject((it++)->first);
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
            if (tempRadarViewable->second.text_ && !fontName_.empty())
                tempRadarViewable->second.text_->setFontName(fontName_);
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
            if (tempRadarViewable->second.text_ && size >= 0.0f)
                tempRadarViewable->second.text_->setCharHeight(size);
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

    if (!activeObjectList_.empty())
    {
        for (tempRadarViewable = activeObjectList_.begin(); tempRadarViewable!=activeObjectList_.end(); ++tempRadarViewable)
        {


            //get Distance to HumanController and save it in the TextAreaOverlayElement.
            int dist = (int)(tempRadarViewable->first->getRVWorldPosition() - HumanController::getLocalControllerEntityAsPawn()->getWorldPosition()).length();
            tempRadarViewable->second.text_->setCaption(multi_cast<std::string>(dist));
            float textLength = multi_cast<std::string>(dist).size() * tempRadarViewable->second.text_->getCharHeight() * 0.3f;



            orxonox::Camera* cam = CameraManager::getInstance().getActiveCamera();
            if (!cam)
                return;
            const Matrix4& transform = cam->getOgreCamera()->getProjectionMatrix() * cam->getOgreCamera()->getViewMatrix();
            // transform to screen coordinates
            Vector3 pos = transform * tempRadarViewable->first->getRVWorldPosition();




            if (pos.z > 1.0)
            {
                // z > 1.0 means that the object is behind the camera
                tempRadarViewable->second.outOfView_ = true;
                // we have to switch all coordinates (if you don't know why,
                // try linear algebra lectures, because I can't explain..)
                pos.x = -pos.x;
                pos.y = -pos.y;
            }
            else
                tempRadarViewable->second.outOfView_ = pos.x < -1.0 || pos.x > 1.0 || pos.y < -1.0 || pos.y > 1.0;

            if (tempRadarViewable->second.outOfView_)
            {
                // object is not in view
//            aimMarker_->hide();

                if (!tempRadarViewable->second.wasOutOfView_)
                {
                    tempRadarViewable->second.panel_->setMaterialName("Orxonox/NavArrows");
                    tempRadarViewable->second.wasOutOfView_ = true;
                }



                if (pos.x < pos.y)
                {
                    if (pos.y > -pos.x)
                    {
                        // up
                        float position = pos.x / pos.y + 1.0f;
                        tempRadarViewable->second.panel_->setPosition((position - tempRadarViewable->second.panel_->getWidth()) * 0.5f, 0.0f);
                        tempRadarViewable->second.panel_->setUV(0.5f, 0.0f, 1.0f, 0.5f);
                        tempRadarViewable->second.text_->setLeft((position - textLength) * 0.5f);
                        tempRadarViewable->second.text_->setTop(tempRadarViewable->second.panel_->getHeight());
                    }
                    else
                    {
                        // left
                        float position = pos.y / pos.x + 1.0f;
                        tempRadarViewable->second.panel_->setPosition(0.0f, (position - tempRadarViewable->second.panel_->getWidth()) * 0.5f);
                        tempRadarViewable->second.panel_->setUV(0.0f, 0.0f, 0.5f, 0.5f);
                        tempRadarViewable->second.text_->setLeft(tempRadarViewable->second.panel_->getWidth() + 0.01f);
                        tempRadarViewable->second.text_->setTop((position - tempRadarViewable->second.text_->getCharHeight()) * 0.5f);
                    }
                }
                else
                {

                    if (pos.y < -pos.x)
                    {
                        // down
                        float position = -pos.x / pos.y + 1.0f;
                        tempRadarViewable->second.panel_->setPosition((position - tempRadarViewable->second.panel_->getWidth()) * 0.5f, 1.0f - tempRadarViewable->second.panel_->getHeight());
                        tempRadarViewable->second.panel_->setUV(0.0f, 0.5f, 0.5f, 1.0f);
                        tempRadarViewable->second.text_->setLeft((position - textLength) * 0.5f);
                        tempRadarViewable->second.text_->setTop(1.0f - tempRadarViewable->second.panel_->getHeight() - tempRadarViewable->second.text_->getCharHeight());
                    }
                    else
                    {
                        // right
                        float position = -pos.y / pos.x + 1.0f;
                        tempRadarViewable->second.panel_->setPosition(1.0f - tempRadarViewable->second.panel_->getWidth(), (position - tempRadarViewable->second.panel_->getHeight()) * 0.5f);
                        tempRadarViewable->second.panel_->setUV(0.5f, 0.5f, 1.0f, 1.0f);
                        tempRadarViewable->second.text_->setLeft(1.0f - tempRadarViewable->second.panel_->getWidth() - textLength - 0.01f);
                        tempRadarViewable->second.text_->setTop((position - tempRadarViewable->second.text_->getCharHeight()) * 0.5f);
                    }
                }
            }
            else
            {


                // object is in view
                /*
                            Vector3 aimpos = transform * getPredictedPosition(SpaceShip::getLocalShip()->getPosition(),
                                    Projectile::getSpeed(), Radar::getInstance().getFocus()->getRVWorldPosition(), Radar::getInstance().getFocus()->getRVOrientedVelocity());
                */
                if (tempRadarViewable->second.wasOutOfView_)
                {
                    tempRadarViewable->second.panel_->setMaterialName("Orxonox/NavTDC");
                    tempRadarViewable->second.wasOutOfView_ = false;
                }

                // object is in view
                tempRadarViewable->second.panel_->setUV(0.0f, 0.0f, 1.0f, 1.0f);
                tempRadarViewable->second.panel_->setLeft((pos.x + 1.0f - tempRadarViewable->second.panel_->getWidth()) * 0.5f);
                tempRadarViewable->second.panel_->setTop((-pos.y + 1.0f - tempRadarViewable->second.panel_->getHeight()) * 0.5f);


//                 aimMarker_->show();
//                 aimMarker_->setLeft((aimpos.x + 1.0f - aimMarker_->getWidth()) * 0.5f);
//                 aimMarker_->setTop((-aimpos.y + 1.0f - aimMarker_->getHeight()) * 0.5f);
//
                tempRadarViewable->second.text_->setLeft((pos.x + 1.0f + tempRadarViewable->second.panel_->getWidth()) * 0.5f);
                tempRadarViewable->second.text_->setTop((-pos.y + 1.0f + tempRadarViewable->second.panel_->getHeight()) * 0.5f);
            }

            tempRadarViewable->second.panel_->show();
            tempRadarViewable->second.text_->show();
//             tempRadarViewable->second.text_->hide();
        }

    }

}


// float HUDNavigation::getDist2Focus() const {
// 
//     Radar* radar = this->getOwner()->getScene()->getRadar();
//     if (radar->getFocus() && HumanController::getLocalControllerEntityAsPawn())
//         return (radar->getFocus()->getRVWorldPosition() - HumanController::getLocalControllerEntityAsPawn()->getWorldPosition()).length();
//     else
//         return 0;
// }

/**
@brief Overridden method of OrxonoxOverlay. Usually the entire overlay
       scales with scale(). Here we obviously have to adjust this.
*/
void HUDNavigation::sizeChanged()
{
    // use size to compensate for aspect ratio if enabled.
    float xScale = this->getActualSize().x;
    float yScale = this->getActualSize().y;

    if (!activeObjectList_.empty())
    {
        for (tempRadarViewable = activeObjectList_.begin(); tempRadarViewable!=activeObjectList_.end(); ++tempRadarViewable)
        {

            if (tempRadarViewable->second.panel_)
                tempRadarViewable->second.panel_->setDimensions(navMarkerSize_ * xScale, navMarkerSize_ * yScale);
//            if (this->aimMarker_)
//            aimMarker_->setDimensions(aimMarkerSize_ * xScale, aimMarkerSize_ * yScale);
            if (tempRadarViewable->second.text_)
                tempRadarViewable->second.text_->setCharHeight(tempRadarViewable->second.text_->getCharHeight() * yScale);
        }
    }
}


void HUDNavigation::addObject(RadarViewable* object)
{
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

    objectStruct tempStruct = {panel, text, true};
    activeObjectList_[object] = tempStruct;

    this->background_->addChild(panel);
    this->background_->addChild(text);

// 	background_->addChild(activeObjectList_[object].first);
// 	background_->addChild(activeObjectList_[object].second);

}

void HUDNavigation::removeObject(RadarViewable* viewable)
{
    activeObjectListType::iterator it = activeObjectList_.find(viewable);

    if (activeObjectList_.find(viewable) != activeObjectList_.end())
    {
        // Remove overlays from Ogre
        this->background_->removeChild(it->second.panel_->getName());
        this->background_->removeChild(it->second.text_->getName());
        // correctly destroy the overlay elements
        Ogre::OverlayManager::getSingleton().destroyOverlayElement(it->second.panel_);
        Ogre::OverlayManager::getSingleton().destroyOverlayElement(it->second.text_);

        activeObjectList_.erase(viewable);
    }
   
}

void HUDNavigation::changedOwner()
{
    respawnObjectSetType respawnObjectSet = this->getOwner()->getScene()->getRadar()->getRadarObjects();
    respawnObjectSetType::iterator respawnObjectSetIt_;
    for (respawnObjectSetIt_ = respawnObjectSet.begin(); respawnObjectSetIt_ != respawnObjectSet.end(); ++respawnObjectSetIt_)
    {
        if (!(*respawnObjectSetIt_)->isHumanShip_)
            this->addObject(*respawnObjectSetIt_);
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



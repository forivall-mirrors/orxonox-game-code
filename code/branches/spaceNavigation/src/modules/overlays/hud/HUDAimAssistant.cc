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
 *      Baptiste Mottet
 *   Co-authors:
 *      ...
 *
 */



#include "HUDAimAssistant.h"

#include <OgreCamera.h>
#include <OgreFontManager.h>
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
#include "core/ConfigValueIncludes.h"
#include "tools/TextureGenerator.h"

namespace orxonox
{
    CreateFactory ( HUDAimAssistant );

    static bool compareDistance(std::pair<RadarViewable*, unsigned int> a, std::pair<RadarViewable*, unsigned int> b)
    {
        return a.second < b.second;
    }


    HUDAimAssistant::HUDAimAssistant(BaseObject* creator) : OrxonoxOverlay(creator)
    {
        RegisterObject(HUDAimAssistant);
        this->setConfigValues();

        // Set default values
        this->setPointerSize(0.03f);
        this->setDetectionLimit(10000.0f);
    }

    HUDAimAssistant::~HUDAimAssistant()
    {
        if (this->isInitialized())
        {
            for (std::map<RadarViewable*, ObjectInfo>::iterator it = this->activeObjectList_.begin(); it != this->activeObjectList_.end();)
                removeObject((it++)->first);
        }
        this->sortedObjectList_.clear();
    }

    void HUDAimAssistant::setConfigValues()
    {
        SetConfigValue(markerLimit_, 3);
    }

    void HUDAimAssistant::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(HUDAimAssistant, XMLPort, xmlelement, mode);

        XMLPortParam(HUDAimAssistant, "pointerSize",    setPointerSize,    getPointerSize,  xmlelement, mode);
        XMLPortParam(HUDAimAssistant, "detectionLimit", setDetectionLimit, getDetectionLimit, xmlelement, mode);
    }

    void HUDAimAssistant::tick(float dt)
    {
        SUPER(HUDAimAssistant, tick, dt);

        Camera* cam = CameraManager::getInstance().getActiveCamera();
        if (cam == NULL)
            return;
        const Matrix4& camTransform = cam->getOgreCamera()->getProjectionMatrix() * cam->getOgreCamera()->getViewMatrix();


        for (std::list<std::pair<RadarViewable*, unsigned int> >::iterator listIt = this->sortedObjectList_.begin(); listIt != this->sortedObjectList_.end(); ++listIt)
            listIt->second = (int)((listIt->first->getRVWorldPosition() - HumanController::getLocalControllerSingleton()->getControllableEntity()->getWorldPosition()).length() + 0.5f);

        this->sortedObjectList_.sort(compareDistance);

        unsigned int markerCount = 0;
        bool closeEnough = false; // only display objects that are close enough to be relevant for the player

        for (std::list<std::pair<RadarViewable*, unsigned int> >::iterator listIt = this->sortedObjectList_.begin(); listIt != this->sortedObjectList_.end(); ++markerCount, ++listIt)
        {
            std::map<RadarViewable*, ObjectInfo>::iterator it = this->activeObjectList_.find(listIt->first);
            closeEnough = listIt->second < this->detectionLimit_;
            // display radarviewables on HUD if the marker limit and max-distance is not exceeded
            if (markerCount < this->markerLimit_ && (closeEnough ||  this->detectionLimit_ < 0))
            {
                // Get Distance to HumanController and save it in the TextAreaOverlayElement.
                int dist = listIt->second;
                float textLength = 0.0f;

               // Transform to screen coordinates
                Vector3 pos = camTransform * it->first->getRVWorldPosition();

                bool outOfView = true;
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
                    // Object is not in view

                    // Hide if outOfView changed
                    if (!it->second.wasOutOfView_)
                    {
                        it->second.panel_->hide();
                        it->second.wasOutOfView_ = true;
                    }

                }
                else
                {
                    // Object is in view

                    // Change material only if outOfView changed
                    if (it->second.wasOutOfView_)
                    {
                        //it->second.panel_->setMaterialName("Orxonox/NavTDC");
                        it->second.panel_->setMaterialName(TextureGenerator::getMaterialName("aimassistant.png", it->first->getRadarObjectColour()));
                        it->second.panel_->setDimensions(this->pointerSize_ * this->getActualSize().x, this->pointerSize_ * this->getActualSize().y);
                        it->second.wasOutOfView_ = false;
                    }

                    // Position marker
                    it->second.panel_->setUV(0.0f, 0.0f, 1.0f, 1.0f);
                    it->second.panel_->setLeft((pos.x + 1.0f - it->second.panel_->getWidth()) * 0.5f);
                    it->second.panel_->setTop((-pos.y + 1.0f - it->second.panel_->getHeight()) * 0.5f);
                }

                // Make sure the overlays are shown
                it->second.panel_->show();
            }
            else // do not display on HUD
            {
                it->second.panel_->hide();
            }
        }
    }

    /** Overridden method of OrxonoxOverlay.
    @details
        Usually the entire overlay scales with scale().
        Here we obviously have to adjust this.
    */
    void HUDAimAssistant::sizeChanged()
    {
        // Use size to compensate for aspect ratio if enabled.
        float xScale = this->getActualSize().x;
        float yScale = this->getActualSize().y;

        for (std::map<RadarViewable*, ObjectInfo>::iterator it = this->activeObjectList_.begin(); it != this->activeObjectList_.end(); ++it)
        {
            if (it->second.panel_ != NULL)
                it->second.panel_->setDimensions(this->pointerSize_ * xScale, this->pointerSize_ * yScale);
        }
    }

    void HUDAimAssistant::addObject(RadarViewable* object)
    {
        if (showObject(object) == false)
            return;

        if (this->activeObjectList_.size() >= this->markerLimit_)
            if (object == NULL)
                return;

        // Object hasn't been added yet (we know that)
        assert(this->activeObjectList_.find(object) == this->activeObjectList_.end());

        // Scales used for dimensions and text size
        float xScale = this->getActualSize().x;
        float yScale = this->getActualSize().y;

        // Create everything needed to display the object on the radar and add it to the map

        // Create arrow/marker
        Ogre::PanelOverlayElement* panel = static_cast<Ogre::PanelOverlayElement*>( Ogre::OverlayManager::getSingleton()
                                           .createOverlayElement("Panel", "HUDAimAssistant_pointer_" + getUniqueNumberString()));
        //panel->setMaterialName("Orxonox/NavTDC");
        panel->setMaterialName(TextureGenerator::getMaterialName("tdc.png", object->getRadarObjectColour()));
        panel->setDimensions(this->pointerSize_ * xScale, this->pointerSize_ * yScale);
        //panel->setColour(object->getRadarObjectColour());


        ObjectInfo tempStruct = {panel, false /*, TODO: initialize wasOutOfView_ */};
        this->activeObjectList_[object] = tempStruct;

        this->background_->addChild(panel);

        this->sortedObjectList_.push_front(std::make_pair(object, (unsigned int)0));
    }

    void HUDAimAssistant::removeObject(RadarViewable* viewable)
    {
        std::map<RadarViewable*, ObjectInfo>::iterator it = this->activeObjectList_.find(viewable);

        if (this->activeObjectList_.find(viewable) != this->activeObjectList_.end())
        {
            // Detach overlays
            this->background_->removeChild(it->second.panel_->getName());
            // Properly destroy the overlay elements (do not use delete!)
            Ogre::OverlayManager::getSingleton().destroyOverlayElement(it->second.panel_);
            // Remove from the list
            this->activeObjectList_.erase(viewable);
        }

        for (std::list<std::pair<RadarViewable*, unsigned int> >::iterator listIt = this->sortedObjectList_.begin(); listIt != this->sortedObjectList_.end(); ++listIt)
        {
            if ((listIt->first) == viewable)
            {
                this->sortedObjectList_.erase(listIt);
                break;
            }
        }
    }

    void HUDAimAssistant::objectChanged(RadarViewable* viewable)
    {
        // TODO: niceification neccessary ;)
        removeObject(viewable);
        addObject(viewable);
    }

    bool HUDAimAssistant::showObject(RadarViewable* rv)
    {
        if (rv == orxonox_cast<RadarViewable*>(this->getOwner()))
            return false;
        assert(rv->getWorldEntity());
        if (rv->getWorldEntity()->isVisible() == false || rv->getRadarVisibility() == false)
            return false;
        return true;
    }

    void HUDAimAssistant::changedOwner()
    {
        const std::set<RadarViewable*>& respawnObjects = this->getOwner()->getScene()->getRadar()->getRadarObjects();
        for (std::set<RadarViewable*>::const_iterator it = respawnObjects.begin(); it != respawnObjects.end(); ++it)
        {
            if (!(*it)->isHumanShip_)
                this->addObject(*it);
        }
    }
}

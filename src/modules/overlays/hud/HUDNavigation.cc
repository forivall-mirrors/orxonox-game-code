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
 *      Oliver Scheuss
 *      Matthias Spalinger
 *
 */

#include "HUDNavigation.h"

#include <OgreCamera.h>
#include <OgreFontManager.h>
#include <OgreOverlayManager.h>
#include <OgreTextAreaOverlayElement.h>
#include <OgrePanelOverlayElement.h>

#include <typeinfo>

#include "util/Math.h"
#include "util/Convert.h"
#include "core/command/ConsoleCommand.h"
#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "CameraManager.h"
#include "Scene.h"
#include "Radar.h"
#include "graphics/Camera.h"
#include "controllers/HumanController.h"
#include "worldentities/pawns/Pawn.h"
#include "worldentities/WorldEntity.h"
#include "core/config/ConfigValueIncludes.h"
#include "tools/TextureGenerator.h"
// #include <boost/bind/bind_template.hpp>


namespace orxonox
{

    SetConsoleCommand("HUDNavigation","selectClosest", &HUDNavigation::selectClosestTarget).addShortcut().keybindMode(KeybindMode::OnPress);
    SetConsoleCommand("HUDNavigation","selectNext", &HUDNavigation::selectNextTarget).addShortcut().keybindMode(KeybindMode::OnPress);

    static bool compareDistance(std::pair<RadarViewable*, unsigned int> a,
            std::pair<RadarViewable*, unsigned int> b)
    {
        return a.second < b.second;
    }
    RegisterClass ( HUDNavigation );

    HUDNavigation* HUDNavigation::localHUD_s = 0;

    HUDNavigation::HUDNavigation(Context* context) :
        OrxonoxOverlay(context)
    {
        RegisterObject(HUDNavigation)
;        this->setConfigValues();

        // Set default values
        this->setFont("Monofur");
        this->setTextSize(0.05f);
        this->setNavMarkerSize(0.03f);
        this->setAimMarkerSize(0.02f);

        this->setDetectionLimit(10000.0f);
        this->currentMunitionSpeed_ = 2500.0f;

        this->closestTarget_ = true;
        this->nextTarget_ = false;
        HUDNavigation::localHUD_s = this;
    }

    HUDNavigation::~HUDNavigation()
    {
        if (this->isInitialized())
        {
            for (std::map<RadarViewable*, ObjectInfo>::iterator it = this->activeObjectList_.begin(); it != this->activeObjectList_.end();)
            removeObject((it++)->first);
        }
        this->sortedObjectList_.clear();
    }

    void HUDNavigation::setConfigValues()
    {
        SetConfigValue(markerLimit_, 3);
        SetConfigValue(showDistance_, false);
    }

    void HUDNavigation::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(HUDNavigation, XMLPort, xmlelement, mode);

        XMLPortParam(HUDNavigation, "font", setFont, getFont, xmlelement, mode);
        XMLPortParam(HUDNavigation, "textSize", setTextSize, getTextSize, xmlelement, mode);
        XMLPortParam(HUDNavigation, "navMarkerSize", setNavMarkerSize, getNavMarkerSize, xmlelement, mode);
        XMLPortParam(HUDNavigation, "detectionLimit", setDetectionLimit, getDetectionLimit, xmlelement, mode);
        XMLPortParam(HUDNavigation, "aimMarkerSize", setAimMarkerSize, getAimMarkerSize, xmlelement, mode);
    }

    void HUDNavigation::setFont(const std::string& font)
    {
        const Ogre::ResourcePtr& fontPtr = Ogre::FontManager::getSingleton().getByName(font);
        if (fontPtr.isNull())
        {
            orxout(internal_warning) << "HUDNavigation: Font '" << font << "' not found" << endl;
            return;
        }
        this->fontName_ = font;
        for (std::map<RadarViewable*, ObjectInfo>::iterator it = this->activeObjectList_.begin(); it != this->activeObjectList_.end(); ++it)
        {
            if (it->second.text_ != NULL)
            it->second.text_->setFontName(this->fontName_);
        }
    }

    const std::string& HUDNavigation::getFont() const
    {
        return this->fontName_;
    }

    void HUDNavigation::setTextSize(float size)
    {
        if (size <= 0.0f)
        {
            orxout(internal_warning) << "HUDNavigation: Negative font size not allowed" << endl;
            return;
        }
        this->textSize_ = size;
        for (std::map<RadarViewable*, ObjectInfo>::iterator it = this->activeObjectList_.begin(); it!=this->activeObjectList_.end(); ++it)
        {
            if (it->second.text_)
            it->second.text_->setCharHeight(size);
        }
    }

    float HUDNavigation::getTextSize() const
    {
        return this->textSize_;
    }

    float HUDNavigation::getArrowSizeX(int dist) const
    {
        if (dist < 600)
        dist = 600;
        return this->getActualSize().x * 900 * this->navMarkerSize_ / dist;
    }

    float HUDNavigation::getArrowSizeY(int dist) const
    {
        if (dist < 600)
        dist = 600;
        return this->getActualSize().y * 900 * this->navMarkerSize_ / dist;
    }

    void HUDNavigation::tick(float dt)
    {
        SUPER(HUDNavigation, tick, dt);

        Camera* cam = CameraManager::getInstance().getActiveCamera();
        if (cam == NULL)
        return;
        const Matrix4& camTransform = cam->getOgreCamera()->getProjectionMatrix() * cam->getOgreCamera()->getViewMatrix();

        for (std::list<std::pair<RadarViewable*, unsigned int> >::iterator listIt = this->sortedObjectList_.begin(); listIt != this->sortedObjectList_.end(); ++listIt)
        listIt->second = (int)((listIt->first->getRVWorldPosition() - HumanController::getLocalControllerSingleton()->getControllableEntity()->getWorldPosition()).length() + 0.5f);

        this->sortedObjectList_.sort(compareDistance);

        unsigned int markerCount = 0;
        bool closeEnough = false; // only display objects that are close enough to be relevant for the player

        // if the selected object doesn't exist any more or is now out of range select the closest object
        std::map<RadarViewable*, ObjectInfo>::iterator selectedActiveObject = this->activeObjectList_.find(this->selectedTarget_);
        if(selectedActiveObject == this->activeObjectList_.end())
        {
            this->closestTarget_ = true;
        }
        else if(this->detectionLimit_ < (this->selectedTarget_->getRVWorldPosition() - HumanController::getLocalControllerSingleton()->getControllableEntity()->getWorldPosition()).length() + 0.5f)
        {
            this->closestTarget_ = true;
            selectedActiveObject->second.selected_ = false;
        }

        bool nextHasToBeSelected = false;

        for (std::list<std::pair<RadarViewable*, unsigned int> >::iterator listIt = this->sortedObjectList_.begin(); listIt != this->sortedObjectList_.end(); ++markerCount, ++listIt)
        {
            std::map<RadarViewable*, ObjectInfo>::iterator it = this->activeObjectList_.find(listIt->first);
            closeEnough = listIt->second < this->detectionLimit_;
            // display radarviewables on HUD if the marker limit and max-distance is not exceeded
            if (markerCount < this->markerLimit_ && (closeEnough || this->detectionLimit_ < 0))
            {
                // Get Distance to HumanController and save it in the TextAreaOverlayElement.
                int dist = listIt->second;
                float textLength = 0.0f;

                if (this->showDistance_)
                {
                    //display distance next to cursor
                    it->second.text_->setCaption(multi_cast<std::string>(dist));
                    textLength = multi_cast<std::string>(dist).size() * it->second.text_->getCharHeight() * 0.3f;
                }
                else
                {
                    //display name next to cursor
                    it->second.text_->setCaption(it->first->getRadarName());
                    textLength = it->first->getRadarName().size() * it->second.text_->getCharHeight() * 0.3f;
                }

                // select the object that aim-assistant indicates
                if(this->closestTarget_)
                // select the closest object
                {
                    if(listIt == this->sortedObjectList_.begin())
                    {
                        it->second.selected_ = true;
                        this->selectedTarget_ = it->first;
                    }
                    else if(it->second.selected_)
                    {
                        it->second.selected_ = false;
                    }

                }
                else if(this->nextTarget_)
                // select the next object in sortedObjectList
                {
                    if(nextHasToBeSelected){
                        it->second.selected_ = true;
                        this->selectedTarget_ = it->first;
                        nextHasToBeSelected = false;
                    }
                    else if(it->second.selected_)
                    {
                        nextHasToBeSelected = true;
                        it->second.selected_ = false;

                        // check if there's a next object
                        listIt++;
                        if(listIt != this->sortedObjectList_.end())
                        {
                            // and if the marker limit and max-distance are not exceeded for it
                            if (markerCount + 1 >= this->markerLimit_ ||
                                    (listIt->second > this->detectionLimit_ && detectionLimit_ >= 0))
                            {
                                // otherwise select the closest object
                                this->activeObjectList_.find(this->sortedObjectList_.begin()->first)->second.selected_ = true;
                                this->selectedTarget_ = it->first;
                                nextHasToBeSelected = false;
                            }
                        }
                        listIt--;
                    }
                }


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

                    // Change material only if outOfView changed
                    if (!it->second.wasOutOfView_)
                    {
                        it->second.panel_->setMaterialName(TextureGenerator::getMaterialName("arrows.png", it->first->getRadarObjectColour()));
                        it->second.wasOutOfView_ = true;
                        it->second.target_->hide();
                    }

                    //float xDistScale = this->getActualSize().x * 1000.0f * this->navMarkerSize_ / dist;
                    //float yDistScale = this->getActualSize().y * 1000.0f * this->navMarkerSize_ / dist;

                    // Adjust Arrowsize according to distance
                    it->second.panel_->setDimensions(getArrowSizeX(dist), getArrowSizeY(dist));

                    // Switch between top, bottom, left and right position of the arrow at the screen border
                    if (pos.x < pos.y)
                    {
                        if (pos.y > -pos.x)
                        {
                            // Top
                            float position = pos.x / pos.y + 1.0f;
                            it->second.panel_->setPosition((position - it->second.panel_->getWidth()) * 0.5f, 0.0f);
                            it->second.panel_->setUV(0.5f, 0.0f, 1.0f, 0.5f);
                            it->second.text_->setLeft((position - textLength) * 0.5f);
                            it->second.text_->setTop(it->second.panel_->getHeight());
                        }
                        else
                        {
                            // Left
                            float position = pos.y / pos.x + 1.0f;
                            it->second.panel_->setPosition(0.0f, (position - it->second.panel_->getWidth()) * 0.5f);
                            it->second.panel_->setUV(0.0f, 0.0f, 0.5f, 0.5f);
                            it->second.text_->setLeft(it->second.panel_->getWidth() + 0.01f);
                            it->second.text_->setTop((position - it->second.text_->getCharHeight()) * 0.5f);
                        }
                    }
                    else
                    {
                        if (pos.y < -pos.x)
                        {
                            // Bottom
                            float position = -pos.x / pos.y + 1.0f;
                            it->second.panel_->setPosition((position - it->second.panel_->getWidth()) * 0.5f, 1.0f - it->second.panel_->getHeight());
                            it->second.panel_->setUV(0.0f, 0.5f, 0.5f, 1.0f );
                            it->second.text_->setLeft((position - textLength) * 0.5f);
                            it->second.text_->setTop(1.0f - it->second.panel_->getHeight() - it->second.text_->getCharHeight());
                        }
                        else
                        {
                            // Right
                            float position = -pos.y / pos.x + 1.0f;
                            it->second.panel_->setPosition(1.0f - it->second.panel_->getWidth(), (position - it->second.panel_->getHeight()) * 0.5f);
                            it->second.panel_->setUV(0.5f, 0.5f, 1.0f, 1.0f);
                            it->second.text_->setLeft(1.0f - it->second.panel_->getWidth() - textLength - 0.01f);
                            it->second.text_->setTop((position - it->second.text_->getCharHeight()) * 0.5f);
                        }
                    }
                }
                else
                {
                    // Object is in view

                    // Change material only if outOfView changed
                    if (it->second.wasOutOfView_)
                    {
                        //it->second.panel_->setMaterialName("Orxonox/NavTDC");
                        it->second.panel_->setMaterialName(TextureGenerator::getMaterialName("tdc.png", it->first->getRadarObjectColour()));
                        it->second.panel_->setDimensions(this->navMarkerSize_ * this->getActualSize().x, this->navMarkerSize_ * this->getActualSize().y);
                        it->second.target_->setDimensions(this->aimMarkerSize_ * this->getActualSize().x, this->aimMarkerSize_ * this->getActualSize().y);
                        it->second.wasOutOfView_ = false;
                    }

                    // Position marker
                    it->second.panel_->setUV(0.0f, 0.0f, 1.0f, 1.0f);
                    it->second.panel_->setLeft((pos.x + 1.0f - it->second.panel_->getWidth()) * 0.5f);
                    it->second.panel_->setTop((-pos.y + 1.0f - it->second.panel_->getHeight()) * 0.5f);

                    // Position text
                    it->second.text_->setLeft((pos.x + 1.0f + it->second.panel_->getWidth()) * 0.5f);
                    it->second.text_->setTop((-pos.y + 1.0f + it->second.panel_->getHeight()) * 0.5f);

                    // Make sure the overlays are shown
                    it->second.panel_->show();
                    it->second.text_->show();

                    // Target marker
                    const Pawn* pawn = dynamic_cast<const Pawn*>(it->first->getWorldEntity());
                    /* Pawn* humanPawn = HumanController::getLocalControllerEntityAsPawn();*/
                    if(!it->second.selected_
                            || it->first->getRVVelocity().squaredLength() == 0
                            || pawn == NULL
                            /* TODO : improve getTeam in such a way that it works
                             * || humanPawn == NULL
                             * || pawn->getTeam() == humanPawn->getTeam()*/)
                    {
                        // don't show marker for not selected enemies nor if the selected doesn't move
                        it->second.target_->hide();
                    }
                    else // object is selected and moves
                    {
                        // get the aim position
                        Vector3* targetPos = this->toAimPosition(it->first);
                        // Transform to screen coordinates
                        Vector3 screenPos = camTransform * (*targetPos);
                        // Check if the target marker is in view too
                        if(screenPos.z > 1 || screenPos.x < -1.0 || screenPos.x > 1.0
                                || screenPos.y < -1.0 || screenPos.y > 1.0)
                        {
                            it->second.target_->hide();
                        }
                        else
                        {
                            it->second.target_->setLeft((screenPos.x + 1.0f - it->second.target_->getWidth()) * 0.5f);
                            it->second.target_->setTop((-screenPos.y + 1.0f - it->second.target_->getHeight()) * 0.5f);
                            it->second.target_->show();
                        }
                        delete targetPos;
                    }

                }
            }
            else // do not display on HUD

            {
                it->second.panel_->hide();
                it->second.text_->hide();
                it->second.target_->hide();
            }
        }

        this->closestTarget_ = false;
        this->nextTarget_ = false;
    }

    /** Overridden method of OrxonoxOverlay.
     @details
     Usually the entire overlay scales with scale().
     Here we obviously have to adjust this.
     */
    void HUDNavigation::sizeChanged()
    {
        // Use size to compensate for aspect ratio if enabled.
        float xScale = this->getActualSize().x;
        float yScale = this->getActualSize().y;

        for (std::map<RadarViewable*, ObjectInfo>::iterator it = this->activeObjectList_.begin(); it != this->activeObjectList_.end(); ++it)
        {
            if (it->second.panel_ != NULL)
                it->second.panel_->setDimensions(this->navMarkerSize_ * xScale, this->navMarkerSize_ * yScale);
            if (it->second.text_ != NULL)
                it->second.text_->setCharHeight(it->second.text_->getCharHeight() * yScale);
            if (it->second.target_ != NULL)
                it->second.target_->setDimensions(this->aimMarkerSize_ * xScale, this->aimMarkerSize_ * yScale);
        }
    }

    void HUDNavigation::addObject(RadarViewable* object)
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
                .createOverlayElement("Panel", "HUDNavigation_navMarker_" + getUniqueNumberString()));
        //panel->setMaterialName("Orxonox/NavTDC");
        panel->setMaterialName(TextureGenerator::getMaterialName("tdc.png", object->getRadarObjectColour()));
        panel->setDimensions(this->navMarkerSize_ * xScale, this->navMarkerSize_ * yScale);
        //panel->setColour(object->getRadarObjectColour());

        // Create target marker
        Ogre::PanelOverlayElement* target = static_cast<Ogre::PanelOverlayElement*>(Ogre::OverlayManager::getSingleton()
                    .createOverlayElement("Panel", "HUDNavigation_targetMarker_" + getUniqueNumberString()));
        target->setMaterialName(TextureGenerator::getMaterialName("target.png", object->getRadarObjectColour()));
        target->setDimensions(this->aimMarkerSize_ * xScale, this->aimMarkerSize_ * yScale);

        // Create text
        Ogre::TextAreaOverlayElement* text = static_cast<Ogre::TextAreaOverlayElement*>( Ogre::OverlayManager::getSingleton()
                .createOverlayElement("TextArea", "HUDNavigation_navText_" + getUniqueNumberString()));
        text->setFontName(this->fontName_);
        text->setCharHeight(text->getCharHeight() * yScale);
        text->setColour(object->getRadarObjectColour());

        panel->hide();
        target->hide();
        text->hide();

        ObjectInfo tempStruct =
        {   panel, target, text, false, false, false};
        this->activeObjectList_[object] = tempStruct;

        this->background_->addChild(panel);
        this->background_->addChild(target);
        this->background_->addChild(text);

        this->sortedObjectList_.push_front(std::make_pair(object, (unsigned int)0));
    }

    void HUDNavigation::removeObject(RadarViewable* viewable)
    {
        std::map<RadarViewable*, ObjectInfo>::iterator it = this->activeObjectList_.find(viewable);

        if (this->activeObjectList_.find(viewable) != this->activeObjectList_.end())
        {
            // Detach overlays
            this->background_->removeChild(it->second.panel_->getName());
            this->background_->removeChild(it->second.target_->getName());
            this->background_->removeChild(it->second.text_->getName());
            // Properly destroy the overlay elements (do not use delete!)
            Ogre::OverlayManager::getSingleton().destroyOverlayElement(it->second.panel_);
            Ogre::OverlayManager::getSingleton().destroyOverlayElement(it->second.target_);
            Ogre::OverlayManager::getSingleton().destroyOverlayElement(it->second.text_);
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

    void HUDNavigation::objectChanged(RadarViewable* viewable)
    {
        // TODO: niceification neccessary ;)
        removeObject(viewable);
        addObject(viewable);
    }

    bool HUDNavigation::showObject(RadarViewable* rv)
    {
        if (rv == orxonox_cast<RadarViewable*>(this->getOwner()))
        return false;
        assert(rv->getWorldEntity());
        if (rv->getWorldEntity()->isVisible() == false || rv->getRadarVisibility() == false)
        return false;
        return true;
    }

    void HUDNavigation::changedOwner()
    {
        const std::set<RadarViewable*>& respawnObjects = this->getOwner()->getScene()->getRadar()->getRadarObjects();
        for (std::set<RadarViewable*>::const_iterator it = respawnObjects.begin(); it != respawnObjects.end(); ++it)
        {
            if (!(*it)->isHumanShip_)
            this->addObject(*it);
        }
    }

    Vector3* HUDNavigation::toAimPosition(RadarViewable* target) const
    {
        Vector3 wePosition = HumanController::getLocalControllerSingleton()->getControllableEntity()->getWorldPosition();
        Vector3 targetPosition = target->getRVWorldPosition();
        Vector3 targetSpeed = target->getRVVelocity();
        Vector3 relativePosition = targetPosition - wePosition; //Vector from attacker to target

        float p_half = relativePosition.dotProduct(targetSpeed)/(targetSpeed.squaredLength() - this->currentMunitionSpeed_ * this->currentMunitionSpeed_);
        float time1 = -p_half + sqrt(p_half * p_half - relativePosition.squaredLength()/(targetSpeed.squaredLength() - this->currentMunitionSpeed_ * this->currentMunitionSpeed_));

        Vector3* result = new Vector3(targetPosition + targetSpeed * time1);
        return result;
    }

    void HUDNavigation::selectClosestTarget()
    {
        if(HUDNavigation::localHUD_s)
        {
            HUDNavigation::localHUD_s->closestTarget_ = true;
        }
    }

    void HUDNavigation::selectNextTarget()
    {
        if(HUDNavigation::localHUD_s)
        {
            HUDNavigation::localHUD_s->nextTarget_ = true;
        }
    }
}

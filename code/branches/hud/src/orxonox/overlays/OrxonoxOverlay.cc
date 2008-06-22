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
 *      Reto Grieder
 *   Co-authors:
 *      ...
 *
 */

#include "OrxonoxStableHeaders.h"
#include "OrxonoxOverlay.h"

#include <cmath>
#include <OgreOverlayManager.h>
#include <OgrePanelOverlayElement.h>
#include "util/Convert.h"
#include "util/String.h"
#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "core/ConsoleCommand.h"
#include "GraphicsEngine.h"

namespace orxonox
{
    unsigned int OrxonoxOverlay::hudOverlayCounter_s = 0;
    std::map<std::string, OrxonoxOverlay*> OrxonoxOverlay::overlays_s;

    SetConsoleCommand(OrxonoxOverlay, scaleOverlay, false).setAccessLevel(AccessLevel::User);
    SetConsoleCommand(OrxonoxOverlay, scrollOverlay, false).setAccessLevel(AccessLevel::User);
    SetConsoleCommand(OrxonoxOverlay, rotateOverlay, false).setAccessLevel(AccessLevel::User);

    OrxonoxOverlay::OrxonoxOverlay()
        : overlay_(0)
        , background_(0)
        , windowAspectRatio_(1.0f)
        , bCorrectAspect_(false)
        , size_(1.0f, 1.0f)
        , sizeCorrection_(1.0f, 1.0f)
        , angle_(0.0f)
        , position_(0.0f, 0.0f)
        , origin_(0.0f, 0.0f)
    {
        RegisterObject(OrxonoxOverlay);
    }

    OrxonoxOverlay::~OrxonoxOverlay()
    {
        if (this->background_)
            Ogre::OverlayManager::getSingleton().destroyOverlayElement(this->background_);

        std::map<std::string, OrxonoxOverlay*>::iterator it = overlays_s.find(this->getName());
        if (it != overlays_s.end())
            overlays_s.erase(it);
    }

    void OrxonoxOverlay::XMLPort(Element& xmlElement, XMLPort::Mode mode)
    {
        BaseObject::XMLPort(xmlElement, mode);

        if (mode == XMLPort::LoadObject)
        {
            if (overlays_s.find(this->getName()) != overlays_s.end())
            {
                COUT(1) << "Overlay names should be unique or you cannnot access them via console." << std::endl;
            }
            overlays_s[this->getName()] = this;

            overlay_ = Ogre::OverlayManager::getSingleton().create("OrxonoxOverlay_overlay_"
                + convertToString(hudOverlayCounter_s++));

            // create background
            this->background_ = static_cast<Ogre::PanelOverlayElement*>(
                Ogre::OverlayManager::getSingleton().createOverlayElement("Panel",
                "OrxonoxOverlay_background_" + convertToString(hudOverlayCounter_s++)));
            this->overlay_->add2D(this->background_);

            this->windowResized(GraphicsEngine::getSingleton().getWindowWidth(),
                GraphicsEngine::getSingleton().getWindowHeight());
        }

        XMLPortParam(OrxonoxOverlay, "correctAspect", setAspectCorrection, getAspectCorrection, xmlElement, mode);
        XMLPortParam(OrxonoxOverlay, "size", setSize, getUncorrectedSize, xmlElement, mode);
        XMLPortParam(OrxonoxOverlay, "rotation", setRotation, getRotation, xmlElement, mode);
        XMLPortParam(OrxonoxOverlay, "origin", setOrigin, getOrigin, xmlElement, mode);
        XMLPortParam(OrxonoxOverlay, "position", setPosition, getPosition, xmlElement, mode);
        XMLPortParam(OrxonoxOverlay, "background", setBackgroundMaterial, getBackgroundMaterial, xmlElement, mode);

        if (mode == XMLPort::LoadObject)
        {
            this->overlay_->show();
            if (!this->isVisible())
                this->overlay_->hide();

            this->sizeCorrectionChanged();
            this->sizeChanged();
            this->positionChanged();
            this->angleChanged();
        }
    }

    void OrxonoxOverlay::setBackgroundMaterial(const std::string& material)
    {
        if (this->background_ && material != "")
            this->background_->setMaterialName(material);
    }

    const std::string& OrxonoxOverlay::getBackgroundMaterial() const
    {
        if (this->background_)
            return this->background_->getMaterialName();
        else
            return blankString;
    }

    void OrxonoxOverlay::changedVisibility()
    {
        if (this->overlay_)
        {
            if (this->isVisible())
                this->overlay_->show();
            else
                this->overlay_->hide();
        }
    }

    void OrxonoxOverlay::windowResized(int newWidth, int newHeight)
    {
        this->windowAspectRatio_ = newWidth/(float)newHeight;

        this->setAspectCorrection(this->bCorrectAspect_);
    }

    void OrxonoxOverlay::setAspectCorrection(bool val)
    {
        this->bCorrectAspect_ = val;
        this->sizeCorrectionChanged();
    }

    void OrxonoxOverlay::sizeCorrectionChanged()
    {
        if (this->bCorrectAspect_)
        {
            float angle = this->angle_.valueDegrees();
            if (angle < 0.0)
                angle = -angle;
            angle -= 180.0 * (int)(angle / 180.0);

            float tempAspect;
            if (angle > 89.0 && angle < 91.0)
                tempAspect = 1.0 / this->windowAspectRatio_;
            else if (angle > 179 || angle < 1)
                tempAspect = this->windowAspectRatio_;
            else
                tempAspect = 1.0;

            // note: this is only an approximation that is mostly valid when the
            // magnitude of the width is about the magnitude of the height.
            // Correctly we would have to take the square root of width*height
            this->sizeCorrection_.x = 2.0 / (tempAspect + 1.0);
            this->sizeCorrection_.y = tempAspect * this->sizeCorrection_.x;
        }
        else
        {
            this->sizeCorrection_ = Vector2::UNIT_SCALE;
        }
        this->sizeChanged();
    }

    /**
    @remarks
        This function can be overriden by any derivative.
    */
    void OrxonoxOverlay::sizeChanged()
    {
        this->overlay_->setScale(size_.x * sizeCorrection_.x, size_.y * sizeCorrection_.y);
        positionChanged();
    }

    /**
    @remarks
        This function can be overriden by any derivative.
    */
    void OrxonoxOverlay::angleChanged()
    {
        this->overlay_->setRotate(this->angle_);
        this->sizeCorrectionChanged();
    }

    /**
    @remarks
        This function can be overriden by any derivative.
    */
    void OrxonoxOverlay::positionChanged()
    {
        float angle = this->angle_.valueRadians();
        if (angle < 0.0)
            angle = -angle;
        angle -= Ogre::Math::PI * (int)(angle / (Ogre::Math::PI));
        if (angle > Ogre::Math::PI * 0.5)
            angle = Ogre::Math::PI - angle;
        Vector2 actualSize = size_ * sizeCorrection_;
        float radius = actualSize.length();
        float phi = atan(actualSize.y / actualSize.x);
        Vector2 boundingBox(radius * cos(angle - phi), radius * sin(angle + phi));
        Vector2 scroll = (position_ - 0.5 - boundingBox * (origin_ - 0.5)) * 2.0;
        this->overlay_->setScroll(scroll.x, -scroll.y);
    }


    /*static*/ void OrxonoxOverlay::scaleOverlay(const std::string& name, float scale)
    {
        std::map<std::string, OrxonoxOverlay*>::const_iterator it = overlays_s.find(name);
        if (it != overlays_s.end())
            (*it).second->scale(Vector2(scale, scale));
    }

    /*static*/ void OrxonoxOverlay::scrollOverlay(const std::string& name, const Vector2& scroll)
    {
        std::map<std::string, OrxonoxOverlay*>::const_iterator it = overlays_s.find(name);
        if (it != overlays_s.end())
            (*it).second->scroll(scroll);
    }

    /*static*/ void OrxonoxOverlay::rotateOverlay(const std::string& name, const Degree& angle)
    {
        std::map<std::string, OrxonoxOverlay*>::const_iterator it = overlays_s.find(name);
        if (it != overlays_s.end())
            (*it).second->rotate(angle);
    }
}

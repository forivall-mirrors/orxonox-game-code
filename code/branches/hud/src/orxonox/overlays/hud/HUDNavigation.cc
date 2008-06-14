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

#include "OrxonoxStableHeaders.h"
#include "HUDNavigation.h"

#include <OgreOverlayManager.h>
#include <OgreStringConverter.h>

//#include "GraphicsEngine.h"
// TODO: remove the SpaceShip and CameraHandler dependencies
#include "core/Debug.h"
#include "core/CoreIncludes.h"
#include "core/ConsoleCommand.h"
#include "objects/SpaceShip.h"
#include "objects/Projectile.h"
#include "objects/CameraHandler.h"
#include "overlays/OverlayGroup.h"
#include "RadarObject.h"
#include "HUDRadar.h"

namespace orxonox
{
    CreateFactory(HUDNavigation);

    SetConsoleCommand(HUDNavigation, cycleNavigationFocus, true).setAccessLevel(AccessLevel::User);
    SetConsoleCommand(HUDNavigation, releaseNavigationFocus, true).setAccessLevel(AccessLevel::User);

    HUDNavigation* HUDNavigation::instance_s = 0;

    using namespace Ogre;

    HUDNavigation::HUDNavigation()
      : container_(0)
      , navMarker_(0)
      , aimMarker_(0)
      , navText_(0)
      , focus_(0)
    {
        RegisterObject(HUDNavigation);
        
        assert(instance_s == 0); // singleton class
        HUDNavigation::instance_s = this;
    }

    HUDNavigation::~HUDNavigation()
    {
        if (this->isInitialized())
        {
            if (this->container_)
                OverlayManager::getSingleton().destroyOverlayElement(this->container_);
            if (this->navMarker_)
                OverlayManager::getSingleton().destroyOverlayElement(this->navMarker_);
            if (this->navText_)
                OverlayManager::getSingleton().destroyOverlayElement(this->navText_);
            if (this->aimMarker_)
                OverlayManager::getSingleton().destroyOverlayElement(this->aimMarker_);
        }

        HUDNavigation::instance_s = 0;
    }

    void HUDNavigation::XMLPort(Element& xmlElement, XMLPort::Mode mode)
    {
        OrxonoxOverlay::XMLPort(xmlElement, mode);

        if (mode == XMLPort::LoadObject)
        {
            // create container
            container_ = static_cast<OverlayContainer*>(Ogre::OverlayManager::getSingleton().createOverlayElement("Panel", getName() + "_navContainer"));

            // create nav text
            navText_ = static_cast<TextAreaOverlayElement*>(Ogre::OverlayManager::getSingleton().createOverlayElement("TextArea", getName() + "_navText"));
            navText_->setCharHeight(0.05f);
            navText_->setFontName("Monofur");

            // create nav marker
            navMarker_ = static_cast<PanelOverlayElement*>(OverlayManager::getSingleton().createOverlayElement("Panel", getName() + "_navMarker"));
            navMarker_->setMaterialName("Orxonox/NavArrows");
            navMarkerSize_ = 0.05; //default
            wasOutOfView_ = true; // just to ensure the material is changed right the first time..

            // create aim marker
            aimMarker_ = static_cast<PanelOverlayElement*>(OverlayManager::getSingleton().createOverlayElement("Panel", getName() + "_aimMarker"));
            aimMarker_->setMaterialName("Orxonox/NavCrosshair");
            aimMarkerSize_ = 0.04; // default
            
            container_->addChild(navMarker_);
            container_->addChild(aimMarker_);
            container_->addChild(navText_);
            container_->show();

            overlay_->add2D(container_);
            overlay_->hide();
        }

        XMLPortParam(HUDNavigation, "font", setFont, getFont, xmlElement, mode);
        XMLPortParam(HUDNavigation, "textsize", setTextSize, getTextSize, xmlElement, mode);
        XMLPortParam(HUDNavigation, "navmarkersize", setNavMarkerSize, getNavMarkerSize, xmlElement, mode);
        XMLPortParam(HUDNavigation, "aimmarkersize", setAimMarkerSize, getAimMarkerSize, xmlElement, mode);

        if (mode == XMLPort::LoadObject)
        {
            this->sizeChanged();
        }
    }

    void HUDNavigation::setNavMarkerSize(float size)
    {
        this->navMarkerSize_ = size;
    }

    float HUDNavigation::getNavMarkerSize() const
    {
        return this->navMarkerSize_;
    }

    void HUDNavigation::setAimMarkerSize(float size)
    {
        this->aimMarkerSize_ = size;
    }

    float HUDNavigation::getAimMarkerSize() const
    {
        return this->aimMarkerSize_;
    }

    void HUDNavigation::setFont(const std::string& font)
    {
        if (this->navText_ && font != "")
            this->navText_->setFontName(font);
    }

    std::string HUDNavigation::getFont() const
    {
        if (this->navText_)
            return this->navText_->getFontName();
        else
            return "";
    }

    void HUDNavigation::setTextSize(float size)
    {
        if (this->navText_ && size >= 0.0f)
            this->navText_->setCharHeight(size);
    }

    float HUDNavigation::getTextSize() const
    {
        if (this->navText_)
            return this->navText_->getCharHeight();
        else
            return 0.0f;
    }

    void HUDNavigation::tick(float dt)
    {
        if (!focus_)
            return;

        // set text
        int dist = (int) getDist2Focus()/100.0f;
        navText_->setCaption(convertToString(dist));
        float textLength = convertToString(dist).size() * navText_->getCharHeight() * 0.3;

        Ogre::Camera* navCam = SpaceShip::getLocalShip()->getCamera()->cam_;
        Matrix4 transformationMatrix = navCam->getProjectionMatrix() * navCam->getViewMatrix();
        // transform to screen coordinates
        Vector3 pos = transformationMatrix * focus_->getPosition();

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
            aimMarker_->hide();

            if (!wasOutOfView_)
            {
                navMarker_->setMaterialName("Orxonox/NavArrows");
                wasOutOfView_ = true;
            }

            if (pos.x < pos.y)
            {
                if (pos.y > -pos.x)
                {
                    // up
                    float position = pos.x / pos.y + 1.0;
                    navMarker_->setPosition((position - navMarker_->getWidth()) * 0.5, 0.0);
                    navMarker_->setUV(0.5, 0.0, 1.0, 0.5);
                    navText_->setLeft((position - textLength) * 0.5);
                    navText_->setTop(navMarker_->getHeight());
                }
                else
                {
                    // left
                    float position = pos.y / pos.x + 1.0;
                    navMarker_->setPosition(0.0, (position - navMarker_->getWidth()) * 0.5);
                    navMarker_->setUV(0.0, 0.0, 0.5, 0.5);
                    navText_->setLeft(navMarker_->getWidth() + 0.01);
                    navText_->setTop((position - navText_->getCharHeight()) * 0.5);
                }
            }
            else
            {
                if (pos.y < -pos.x)
                {
                    // down
                    float position = -pos.x / pos.y + 1.0;
                    navMarker_->setPosition((position - navMarker_->getWidth()) * 0.5, 1.0 - navMarker_->getHeight());
                    navMarker_->setUV(0.0, 0.5, 0.5, 1.0);
                    navText_->setLeft((position - textLength) * 0.5);
                    navText_->setTop(1.0 - navMarker_->getHeight() - navText_->getCharHeight());
                }
                else
                {
                    // right
                    float position = -pos.y / pos.x + 1.0;
                    navMarker_->setPosition(1.0 - navMarker_->getWidth(), (position - navMarker_->getHeight()) * 0.5);
                    navMarker_->setUV(0.5, 0.5, 1.0, 1.0);
                    navText_->setLeft(1.0 - navMarker_->getWidth() - textLength - 0.01);
                    navText_->setTop((position - navText_->getCharHeight()) * 0.5);
                }
            }
        }
        else
        {
            // object is in view

            Vector3 aimpos = transformationMatrix * getPredictedPosition(SpaceShip::getLocalShip()->getPosition(),
                    Projectile::getSpeed(), focus_->getPosition(), focus_->getOrientedVelocity());

            if (wasOutOfView_)
            {
                navMarker_->setMaterialName("Orxonox/NavTDC");
                wasOutOfView_ = false;
            }

            // object is in view
            navMarker_->setUV(0.0, 0.0, 1.0, 1.0);
            navMarker_->setLeft((pos.x + 1.0 - navMarker_->getWidth()) * 0.5);
            navMarker_->setTop((-pos.y + 1.0 - navMarker_->getHeight()) * 0.5);

            aimMarker_->show();
            aimMarker_->setLeft((aimpos.x + 1.0 - aimMarker_->getWidth()) * 0.5);
            aimMarker_->setTop((-aimpos.y + 1.0 - aimMarker_->getHeight()) * 0.5);

            navText_->setLeft((pos.x + 1.0 + navMarker_->getWidth()) * 0.5);
            navText_->setTop((-pos.y + 1.0 + navMarker_->getHeight()) * 0.5);
        }
    }

    void HUDNavigation::cycleFocus()
    {
        if (!focus_)
        {
            // Get closest object
            float distance = (unsigned int) -1;
            Vector3 shipPos = SpaceShip::getLocalShip()->getPosition();
            it_ = HUDRadar::getInstance().getRadarObjects().begin();

            for (std::list<RadarObject*>::iterator it = HUDRadar::getInstance().getRadarObjects().begin(); it != HUDRadar::getInstance().getRadarObjects().end(); ++it)
            {
                float newdist = (*it)->getPosition().squaredDistance(shipPos);
                if (newdist < distance)
                {
                    distance = newdist;
                    it_ = it;
                }
            }

            if (it_ != HUDRadar::getInstance().getRadarObjects().end())
            {
                focus_ = *it_;

                // move the focused object to the begin of the list, so we will iterate through all other objects when cycling
                HUDRadar::getInstance().getRadarObjects().erase(it_);
                HUDRadar::getInstance().getRadarObjects().insert(HUDRadar::getInstance().getRadarObjects().begin(), focus_);
                it_ = HUDRadar::getInstance().getRadarObjects().begin();
            }
        }
        else if (it_ != HUDRadar::getInstance().getRadarObjects().end())
        {
            focus_->resetMaterial();
            ++it_;
            if (it_ != HUDRadar::getInstance().getRadarObjects().end())
                focus_ = *it_;
            else
                focus_ = 0;
        }
        else
        {
            focus_ = 0;
        }
        updateFocus();
    }

    void HUDNavigation::updateFocus()
    {
        if (focus_)
        {
            overlay_->show();
            focus_->setColour(ColourValue::White);
        }
        else
        {
            overlay_->hide();
        }
    }

    void HUDNavigation::releaseFocus()
    {
        this->focus_ = 0;
        this->updateFocus();
    }

    float HUDNavigation::getDist2Focus() const
    {
        if (focus_)
            return (focus_->getPosition() - SpaceShip::getLocalShip()->getPosition()).length();
        else
            return 0;
    }

    /**
    @brief Overridden method of OrxonoxOverlay. Usually the entire overlay
           scales with scale(). Here we obviously have to adjust this.
    */
    void HUDNavigation::sizeChanged()
    {
        // use size to compensate for apspect ratio if enabled.
        float xScale = this->getSize().x;
        float yScale = this->getSize().y;
        if (this->navMarker_)
            navMarker_->setDimensions(navMarkerSize_ * xScale, navMarkerSize_ * yScale);
        if (this->aimMarker_)
            aimMarker_->setDimensions(aimMarkerSize_ * xScale, aimMarkerSize_ * yScale);
        if (this->navText_)
            navText_->setCharHeight(navText_->getCharHeight() * yScale);
    }

    /*static*/ HUDNavigation& HUDNavigation::getInstance()
    {
        assert(instance_s);
        return *instance_s;
    }

    /*static*/ void HUDNavigation::cycleNavigationFocus()
    {
        // avoid using getInstance because of the assert().
        // User might call this fuction even if HUDNavigation doesn't exist.
        if (instance_s)
            instance_s->cycleFocus();
    }

    /*static*/ void HUDNavigation::releaseNavigationFocus()
    {
        // avoid using getInstance because of the assert().
        // User might call this fuction even if HUDNavigation doesn't exist.
        if (instance_s)
            instance_s->releaseFocus();
    }
}

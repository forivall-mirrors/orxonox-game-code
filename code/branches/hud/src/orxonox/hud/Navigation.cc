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
#include "Navigation.h"

#include <OgreOverlayManager.h>
#include <OgreStringConverter.h>

//#include "GraphicsEngine.h"
// TODO: remove the SpaceShip and CameraHandler dependencies
#include "objects/SpaceShip.h"
#include "objects/Projectile.h"
#include "objects/CameraHandler.h"
#include "HUD.h"
#include "RadarObject.h"
#include "RadarOverlayElement.h"
#include "core/Debug.h"
#include "core/CoreIncludes.h"

namespace orxonox
{
    CreateFactory(Navigation);

    using namespace Ogre;

    Navigation::Navigation()
      : container_(0)
      , navMarker_(0)
      , aimMarker_(0)
      , navText_(0)
      , focus_(0)
    {
        RegisterObject(Navigation);
    }

    Navigation::~Navigation()
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
    }

    void Navigation::XMLPort(Element& xmlElement, XMLPort::Mode mode)
    {
        HUDOverlay::XMLPort(xmlElement, mode);

        if (mode == XMLPort::LoadObject)
        {
            // create container
            container_ = static_cast<OverlayContainer*>(Ogre::OverlayManager::getSingleton().createOverlayElement("Panel", getName() + "_navContainer"));
            container_->setMetricsMode(Ogre::GMM_RELATIVE);
            container_->setLeft(0.0);
            container_->setTop(0.0);
            container_->setWidth(1.0);
            container_->setHeight(1.0);

            // create nav text
            navText_ = static_cast<TextAreaOverlayElement*>(Ogre::OverlayManager::getSingleton().createOverlayElement("TextArea", getName() + "_navText"));
            navText_->setMetricsMode(Ogre::GMM_RELATIVE);
            navText_->setPosition(0.0f, 0.0f);
            navText_->setCharHeight(0.05f);
            navText_->setFontName("Monofur");

            // create nav marker
            navMarker_ = static_cast<PanelOverlayElement*>(OverlayManager::getSingleton().createOverlayElement("Panel", getName() + "_navMarker"));
            navMarker_->setMetricsMode(GMM_RELATIVE);
            navMarker_->setMaterialName("Orxonox/NavArrows");
            this->navMarkerSize_ = 0.05;
            this->wasOutOfView_ = true; // just a to ensure the material is changed right the first time..

            // create aim marker
            aimMarker_ = static_cast<PanelOverlayElement*>(OverlayManager::getSingleton().createOverlayElement("Panel", getName() + "_aimMarker"));
            aimMarker_->setMetricsMode(GMM_RELATIVE);
            aimMarker_->setMaterialName("Orxonox/NavCrosshair");
            this->aimMarkerSize_ = 0.04;
            
            container_->addChild(navMarker_);
            container_->addChild(aimMarker_);
            container_->addChild(navText_);
            container_->show();

            this->overlay_->add2D(container_);
            this->overlay_->hide();
        }

        XMLPortParam(Navigation, "font", setFont, getFont, xmlElement, mode);
        XMLPortParam(Navigation, "textsize", setTextSize, getTextSize, xmlElement, mode);
        XMLPortParam(Navigation, "navmarkersize", setNavMarkerSize, getNavMarkerSize, xmlElement, mode);
        XMLPortParam(Navigation, "aimmarkersize", setAimMarkerSize, getAimMarkerSize, xmlElement, mode);

        if (mode == XMLPort::LoadObject)
        {
            this->sizeChanged();
        }
    }

    void Navigation::setNavMarkerSize(float size)
    {
        this->navMarkerSize_ = size;
    }

    float Navigation::getNavMarkerSize() const
    {
        return this->navMarkerSize_;
    }

    void Navigation::setAimMarkerSize(float size)
    {
        this->aimMarkerSize_ = size;
    }

    float Navigation::getAimMarkerSize() const
    {
        return this->aimMarkerSize_;
    }

    void Navigation::setFont(const std::string& font)
    {
        if (this->navText_ && font != "")
            this->navText_->setFontName(font);
    }

    std::string Navigation::getFont() const
    {
        if (this->navText_)
            return this->navText_->getFontName();
        else
            return "";
    }

    void Navigation::setTextSize(float size)
    {
        if (this->navText_ && size >= 0.0f)
            this->navText_->setCharHeight(size);
    }

    float Navigation::getTextSize() const
    {
        if (this->navText_)
            return this->navText_->getCharHeight();
        else
            return 0.0f;
    }

    void Navigation::tick(float dt)
    {
        if (!focus_)
            return;

        updateMarker();
    }

    void Navigation::updateMarker()
    {
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

    void Navigation::cycleFocus()
    {
        if (!focus_)
        {
            // Get closest object
            float distance = (unsigned int) -1;
            Vector3 shipPos = SpaceShip::getLocalShip()->getPosition();
            it_ = HUD::getSingleton().getRadarObjects().begin();

            for (std::list<RadarObject*>::iterator it = HUD::getSingleton().getRadarObjects().begin(); it != HUD::getSingleton().getRadarObjects().end(); ++it)
            {
                float newdist = (*it)->getPosition().squaredDistance(shipPos);
                if (newdist < distance)
                {
                    distance = newdist;
                    it_ = it;
                }
            }

            if (it_ != HUD::getSingleton().getRadarObjects().end())
            {
                focus_ = *it_;

                // move the focused object to the begin of the list, so we will iterate through all other objects when cycling
                HUD::getSingleton().getRadarObjects().erase(it_);
                HUD::getSingleton().getRadarObjects().insert(HUD::getSingleton().getRadarObjects().begin(), focus_);
                it_ = HUD::getSingleton().getRadarObjects().begin();
            }
        }
        else if (it_ != HUD::getSingleton().getRadarObjects().end())
        {
            focus_->resetMaterial();
            ++it_;
            if (it_ != HUD::getSingleton().getRadarObjects().end())
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

    void Navigation::updateFocus()
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

    void Navigation::releaseFocus()
    {
        this->focus_ = 0;
        this->updateFocus();
    }

    float Navigation::getDist2Focus() const
    {
        if (focus_)
            return (focus_->getPosition() - SpaceShip::getLocalShip()->getPosition()).length();
        else
            return 0;
    }

    void Navigation::sizeChanged()
    {
        float xScale = this->getActualSize().x;
        float yScale = this->getActualSize().y;
        if (this->navMarker_)
            navMarker_->setDimensions(navMarkerSize_ * xScale, navMarkerSize_ * yScale);
        if (this->aimMarker_)
            aimMarker_->setDimensions(aimMarkerSize_ * xScale, aimMarkerSize_ * yScale);
        if (this->navText_)
            navText_->setCharHeight(navText_->getCharHeight() * yScale);
    }
}

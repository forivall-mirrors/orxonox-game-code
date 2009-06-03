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

#include <OgreOverlayManager.h>
#include <OgreTextAreaOverlayElement.h>
#include <OgrePanelOverlayElement.h>

#include "util/Math.h"
#include "util/String.h"
#include "util/Convert.h"
#include "core/ConsoleCommand.h"
#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "objects/Radar.h"

namespace orxonox
{
    CreateFactory(HUDNavigation);

    HUDNavigation::HUDNavigation(BaseObject* creator)
        : OrxonoxOverlay(creator)
    {
        RegisterObject(HUDNavigation);

        // create nav text
        navText_ = static_cast<Ogre::TextAreaOverlayElement*>(Ogre::OverlayManager::getSingleton()
            .createOverlayElement("TextArea", "HUDNavigation_navText_" + getUniqueNumberString()));

        // create nav marker
        navMarker_ = static_cast<Ogre::PanelOverlayElement*>(Ogre::OverlayManager::getSingleton()
            .createOverlayElement("Panel", "HUDNavigation_navMarker_" + getUniqueNumberString()));
        navMarker_->setMaterialName("Orxonox/NavArrows");

        // create aim marker
        aimMarker_ = static_cast<Ogre::PanelOverlayElement*>(Ogre::OverlayManager::getSingleton()
            .createOverlayElement("Panel", "HUDNavigation_aimMarker_" + getUniqueNumberString()));
        aimMarker_->setMaterialName("Orxonox/NavCrosshair");
        this->wasOutOfView_ = true; // Ensure the material is changed right the first time..

        setFont("Monofur");
        setTextSize(0.05f);
        setNavMarkerSize(0.05f);
        setAimMarkerSize(0.04f);

        background_->addChild(navMarker_);
        background_->addChild(aimMarker_);
        background_->addChild(navText_);

        // hide at first
        this->setVisible(false);
    }

    HUDNavigation::~HUDNavigation()
    {
        if (this->isInitialized())
        {
            Ogre::OverlayManager::getSingleton().destroyOverlayElement(this->navMarker_);
            Ogre::OverlayManager::getSingleton().destroyOverlayElement(this->navText_);
            Ogre::OverlayManager::getSingleton().destroyOverlayElement(this->aimMarker_);
        }
    }

    void HUDNavigation::XMLPort(Element& xmlElement, XMLPort::Mode mode)
    {
        SUPER(HUDNavigation, XMLPort, xmlElement, mode);

        XMLPortParam(HUDNavigation, "font",     setFont,     getFont,     xmlElement, mode);
        XMLPortParam(HUDNavigation, "textSize", setTextSize, getTextSize, xmlElement, mode);
        XMLPortParam(HUDNavigation, "navMarkerSize", setNavMarkerSize, getNavMarkerSize, xmlElement, mode);
        XMLPortParam(HUDNavigation, "aimMarkerSize", setAimMarkerSize, getAimMarkerSize, xmlElement, mode);
    }

    void HUDNavigation::setFont(const std::string& font)
    {
        if (this->navText_ && font != "")
            this->navText_->setFontName(font);
    }

    const std::string& HUDNavigation::getFont() const
    {
        if (this->navText_)
            return this->navText_->getFontName();
        else
            return BLANKSTRING;
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
        SUPER(HUDNavigation, tick, dt);

        if (!Radar::getInstance().getFocus())
        {
            this->overlay_->hide();
            return;
        }
        else
        {
            this->overlay_->show();
        }

        // set text
        int dist = (int) getDist2Focus();
        navText_->setCaption(convertToString(dist));
        float textLength = convertToString(dist).size() * navText_->getCharHeight() * 0.3;

/*
        Ogre::Camera* navCam = SpaceShip::getLocalShip()->getCamera()->cam_;
        Matrix4 transformationMatrix = navCam->getProjectionMatrix() * navCam->getViewMatrix();
*/
        // transform to screen coordinates
        Vector3 pos = /*transformationMatrix * */Radar::getInstance().getFocus()->getRVWorldPosition();

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
/*
            Vector3 aimpos = transformationMatrix * getPredictedPosition(SpaceShip::getLocalShip()->getPosition(),
                    Projectile::getSpeed(), Radar::getInstance().getFocus()->getRVWorldPosition(), Radar::getInstance().getFocus()->getRVOrientedVelocity());
*/
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
/*
            aimMarker_->setLeft((aimpos.x + 1.0 - aimMarker_->getWidth()) * 0.5);
            aimMarker_->setTop((-aimpos.y + 1.0 - aimMarker_->getHeight()) * 0.5);
*/
            navText_->setLeft((pos.x + 1.0 + navMarker_->getWidth()) * 0.5);
            navText_->setTop((-pos.y + 1.0 + navMarker_->getHeight()) * 0.5);
        }
    }

    float HUDNavigation::getDist2Focus() const
    {
/*
        if (Radar::getInstance().getFocus())
            return (Radar::getInstance().getFocus()->getRVWorldPosition() - SpaceShip::getLocalShip()->getPosition()).length();
        else
*/
            return 0;
    }

    /**
    @brief Overridden method of OrxonoxOverlay. Usually the entire overlay
           scales with scale(). Here we obviously have to adjust this.
    */
    void HUDNavigation::sizeChanged()
    {
        // use size to compensate for apspect ratio if enabled.
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

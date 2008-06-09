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
 *      ...
 *
 */

#include "OrxonoxStableHeaders.h"
#include "Navigation.h"

#include <OgreOverlayManager.h>
#include <OgreStringConverter.h>

#include "GraphicsEngine.h"
// TODO: remove the SpaceShip and CameraHandler dependencies
#include "objects/SpaceShip.h"
#include "objects/Projectile.h"
#include "objects/CameraHandler.h"
#include "RadarObject.h"
#include "RadarOverlayElement.h"
#include "HUD.h"
#include "core/Debug.h"
#include "util/Math.h"

namespace orxonox
{
    using namespace Ogre;

    Navigation::Navigation(OverlayContainer* container)
    {
        container_ = container;
        focus_ = NULL;
        init();
    }

    Navigation::~Navigation()
    {
        OverlayManager::getSingleton().destroyOverlayElement(this->navText_);
        OverlayManager::getSingleton().destroyOverlayElement(this->navMarker_);
        OverlayManager::getSingleton().destroyOverlayElement(this->aimMarker_);
    }

    void Navigation::init()
    {
        // create nav text
        navText_ = static_cast<TextAreaOverlayElement*>(OverlayManager::getSingleton().createOverlayElement("TextArea", "navText"));
        navText_->show();
        navText_->setMetricsMode(Ogre::GMM_PIXELS);
        navText_->setDimensions(0.001, 0.001);
        navText_->setPosition(0.02, 0.02);
        navText_->setFontName("Console");
        navText_->setCharHeight(20);
        navText_->setCaption("");
        navText_->hide();
        container_->addChild(navText_);


        // create nav marker ...
        navMarker_ = static_cast<PanelOverlayElement*>(OverlayManager::getSingleton().createOverlayElement("Panel", "NavMarker"));
        aimMarker_ = static_cast<PanelOverlayElement*>(OverlayManager::getSingleton().createOverlayElement("Panel", "aimMarker"));
        navMarker_->setMetricsMode(GMM_PIXELS);
        aimMarker_->setMetricsMode(GMM_PIXELS);
        navMarker_->hide();
        aimMarker_->hide();
        aimMarker_->setMaterialName("Orxonox/NavCrosshair");
        aimMarker_->setDimensions(20, 20);
        aimMarker_->setUV(0.0, 0.0, 1.0, 1.0);
        container_->addChild(navMarker_);
        container_->addChild(aimMarker_);
    }

    void Navigation::update()
    {
        if (!focus_)
            return;

        updateMarker();
    }

    void Navigation::updateMarker()
    {
        int windowW = GraphicsEngine::getSingleton().getWindowWidth();
        int windowH = GraphicsEngine::getSingleton().getWindowHeight();

        // set text
        int dist = (int) getDist2Focus()/100;
        navText_->setCaption(Ogre::StringConverter::toString(dist));

        Ogre::Camera* navCam = SpaceShip::getLocalShip()->getCamera()->cam_;
        // transform to screen coordinates
        Vector3 pos = navCam->getProjectionMatrix() * navCam->getViewMatrix() * focus_->getPosition();
        Vector3 aimpos = navCam->getProjectionMatrix() * navCam->getViewMatrix() * getPredictedPosition(SpaceShip::getLocalShip()->getPosition(), Projectile::getSpeed(), focus_->getPosition(), focus_->getOrientedVelocity());

        float xPosRel = 0.5*pos.x+0.5;
        float yPosRel = 1-(0.5*pos.y+0.5);
        float xAimPosRel = 0.5*aimpos.x+0.5;
        float yAimPosRel = 1-(0.5*aimpos.y+0.5);
        int xPos = (int) (xPosRel*windowW);
        int yPos = (int) (yPosRel*windowH);
        int xAimPos = (int) (xAimPosRel*windowW);
        int yAimPos = (int) (yAimPosRel*windowH);
        int xFromCenter = xPos-windowW/2;
        int yFromCenter = yPos-windowH/2;

        // is object in view?
        Vector3 navCamPos = SpaceShip::getLocalShip()->getPosition();
        Vector3 currentDir = SpaceShip::getLocalShip()->getDir();
        Vector3 currentOrth = SpaceShip::getLocalShip()->getOrth();
        float radius = getAngle(navCamPos, currentDir, focus_->getPosition());
        bool isRight = (currentDir.crossProduct(currentOrth)).dotProduct(focus_->getPosition() - navCamPos)>0;
        bool isAbove = currentOrth.dotProduct(focus_->getPosition() - navCamPos)>0;
        bool outOfView = (xPosRel<0 || xPosRel>1 || yPosRel<0 || yPosRel>1);

        // if object is behind us, it is out of view anyway:
        if (!outOfView && radius > Ogre::Math::PI / 2)
            outOfView = true;

        if (outOfView)
        {
            // object is not in view
            navMarker_->setMaterialName("Orxonox/NavArrows");
            navMarker_->setDimensions(16,16);
            aimMarker_->hide();
            float phiUpperCorner = atan((float)(windowW)/(float)(windowH));
            // from the angle we find out on which edge to draw the marker
            // and which of the four arrows to take
            float phiNav = atan((float) xFromCenter / (float) yFromCenter);

            if (isAbove && isRight)
            {
                // top right quadrant
                if (-phiNav < phiUpperCorner)
                {
                    //COUT(3) << "arrow up\n";
                    navMarker_->setPosition(-tan(phiNav)*windowH/2+windowW/2, 0);
                    navMarker_->setUV(0.5, 0.0, 1.0, 0.5);
                    navText_->setLeft(navMarker_->getLeft()+navMarker_->getWidth());
                    navText_->setTop(navMarker_->getHeight());
                }
                else
                {
                    //COUT(3) << "arrow right\n";
                    navMarker_->setPosition(windowW-16, tan((3.14-2*phiNav)/2)*windowW/2+windowH/2);
                    navMarker_->setUV(0.5, 0.5, 1.0, 1.0);
                    navText_->setLeft(navMarker_->getLeft()-navMarker_->getWidth());
                    navText_->setTop(navMarker_->getTop()+navMarker_->getHeight());
                }
            }
            if (!isAbove && isRight)
            {
                // bottom right quadrant
                if (phiNav < phiUpperCorner)
                {
                    //COUT(3) << "arrow down\n";
                    navMarker_->setPosition(tan(phiNav)*windowH/2+windowW/2, windowH-16);
                    navMarker_->setUV(0.0, 0.5, 0.5, 1.0);
                    navText_->setLeft(navMarker_->getLeft()+navMarker_->getWidth());
                    navText_->setTop(navMarker_->getTop()-navMarker_->getHeight());
                }
                else
                {
                    //COUT(3) << "arrow right\n";
                    navMarker_->setPosition(windowW-16, tan((3.14-2*phiNav)/2)*windowW/2+windowH/2);
                    navMarker_->setUV(0.5, 0.5, 1.0, 1.0);
                    navText_->setLeft(navMarker_->getLeft()-navMarker_->getWidth());
                    navText_->setTop(navMarker_->getTop()+navMarker_->getHeight());
                }
            }
            if (isAbove && !isRight)
            {
                // top left quadrant
                if (phiNav<phiUpperCorner)
                {
                    //COUT(3) << "arrow up\n";
                    navMarker_->setPosition(-tan(phiNav)*windowH/2+windowW/2, 0);
                    navMarker_->setUV(0.5, 0.0, 1.0, 0.5);
                    navText_->setLeft(navMarker_->getLeft()+navMarker_->getWidth());
                    navText_->setTop(navMarker_->getHeight());
                }
                else
                {
                    //COUT(3) << "arrow left\n";
                    navMarker_->setPosition(0, -tan((3.14-2*phiNav)/2)*windowW/2+windowH/2);
                    navMarker_->setUV(0.0, 0.0, 0.5, 0.5);
                    navText_->setLeft(navMarker_->getWidth());
                    navText_->setTop(navMarker_->getTop()+navMarker_->getHeight());
                }
            }
            if (!isAbove && !isRight)
            {
                // bottom left quadrant
                if (phiNav>-phiUpperCorner)
                {
                    //COUT(3) << "arrow down\n";
                    navMarker_->setPosition(tan(phiNav)*windowH/2+windowW/2, windowH-16);
                    navMarker_->setUV(0.0, 0.5, 0.5, 1.0);
                    navText_->setLeft(navMarker_->getLeft()+navMarker_->getWidth());
                    navText_->setTop(navMarker_->getTop()-navMarker_->getHeight());
                }
                else
                {
                    //COUT(3) << "arrow left\n";
                    navMarker_->setPosition(0, -tan((3.14-2*phiNav)/2)*windowW/2+windowH/2);
                    navMarker_->setUV(0.0, 0.0, 0.5, 0.5);
                    navText_->setLeft(navMarker_->getWidth());
                    navText_->setTop(navMarker_->getTop()+navMarker_->getHeight());
                }
            }
        }
        else
        {
            // object is in view
            navMarker_->setMaterialName("Orxonox/NavTDC");
            navMarker_->setDimensions(35, 35);
            navMarker_->setUV(0.0, 0.0, 1.0, 1.0);
            navMarker_->setPosition(xPos-navMarker_->getWidth()/2, yPos-navMarker_->getHeight()/2);

            aimMarker_->show();
            aimMarker_->setPosition(xAimPos-aimMarker_->getWidth()/2, yAimPos-aimMarker_->getHeight()/2);

            navText_->setPosition(xPos+navMarker_->getWidth()/2, yPos+navMarker_->getHeight()/2);
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
            navMarker_->show();
            navText_->show();
            focus_->setColour(ColourValue::White);
        }
        else
        {
            navMarker_->hide();
            aimMarker_->hide();
            navText_->hide();
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
}

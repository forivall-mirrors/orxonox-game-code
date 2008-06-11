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
#include "HUDOverlay.h"

#include <OgreOverlayManager.h>
#include "util/Convert.h"
#include "core/CoreIncludes.h"
#include "GraphicsEngine.h"

namespace orxonox
{
  unsigned int HUDOverlay::hudOverlayCounter_s = 0;

  HUDOverlay::HUDOverlay()
    : overlay_(0)
  {
    RegisterObject(HUDOverlay);
  }

  void HUDOverlay::XMLPort(Element& xmlElement, XMLPort::Mode mode)
  {
    BaseObject::XMLPort(xmlElement, mode);

    if (mode == XMLPort::LoadObject)
    {
      overlay_ = Ogre::OverlayManager::getSingleton().create("HUDOverlay"
            + convertToString(hudOverlayCounter_s++) + "_" + this->getName());

      this->windowWidth_ = GraphicsEngine::getSingleton().getWindowWidth();
      this->windowHeight_ = GraphicsEngine::getSingleton().getWindowHeight();
      this->windowAspectRatio_ = windowWidth_/(float)windowHeight_;
    }

    XMLPortParam(HUDOverlay, "scale", setScale, getScale, xmlElement, mode);
    XMLPortParam(HUDOverlay, "scroll", setScroll, getScroll, xmlElement, mode);
    XMLPortParam(HUDOverlay, "rotation", setRotation, getRotation, xmlElement, mode);

    if (mode == XMLPort::LoadObject)
    {
      this->overlay_->show();
      if (!this->isVisible())
          this->overlay_->hide();
    }
  }

  HUDOverlay::~HUDOverlay()
  {
  }

  void HUDOverlay::changedVisibility()
  {
      if (this->overlay_)
      {
          if (this->isVisible())
              overlay_->show();
          else
              overlay_->hide();
      }
  }

  void HUDOverlay::windowResized(int newWidth, int newHeight)
  {
    this->windowWidth_ = newWidth;
    this->windowHeight_ = newHeight;
    this->windowAspectRatio_ = windowWidth_/(float)windowHeight_;
  }

}

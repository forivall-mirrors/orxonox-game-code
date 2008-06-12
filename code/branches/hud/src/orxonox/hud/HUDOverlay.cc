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
    , windowAspectRatio_(1.0f)
    , bCorrectAspect_(false)
    , size_(1.0f)
    , sizeCorrection_(1.0f)
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

      this->windowResized(GraphicsEngine::getSingleton().getWindowWidth(),
            GraphicsEngine::getSingleton().getWindowHeight());
    }

    XMLPortParam(HUDOverlay, "size", setSize, getSize, xmlElement, mode);
    XMLPortParam(HUDOverlay, "correctAspect", setAspectCorrection, getAspectCorrection, xmlElement, mode);
    XMLPortParam(HUDOverlay, "scroll", setScroll, getScroll, xmlElement, mode);
    XMLPortParam(HUDOverlay, "rotation", setRotation, getRotation, xmlElement, mode);

    if (mode == XMLPort::LoadObject)
    {
      this->overlay_->show();
      if (!this->isVisible())
          this->overlay_->hide();

      this->sizeChanged();
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
        this->overlay_->show();
      else
        this->overlay_->hide();
    }
  }

  void HUDOverlay::windowResized(int newWidth, int newHeight)
  {
    this->windowAspectRatio_ = newWidth/(float)newHeight;

    this->setAspectCorrection(this->bCorrectAspect_);
  }

  void HUDOverlay::setAspectCorrection(bool val)
  {
    if (val)
    {
      // note: this is only an approximation that is mostly valid when the
      // magnitude of the width is about the magnitude of the height.
      // Correctly we would have to take the square root of width*height
      this->sizeCorrection_.x = 2.0 / (this->windowAspectRatio_ + 1.0);
      this->sizeCorrection_.y = this->windowAspectRatio_ * this->sizeCorrection_.x;
    }
    else
    {
      this->sizeCorrection_ = Vector2::UNIT_SCALE;
    }

    this->bCorrectAspect_ = val;
    this->sizeChanged();
  }

  /**
    @remarks
      This function can be overriden by any derivative.
  */
  void HUDOverlay::sizeChanged()
  {
    this->overlay_->setScale(size_.x * sizeCorrection_.x, size_.x * sizeCorrection_.y);
  }

}

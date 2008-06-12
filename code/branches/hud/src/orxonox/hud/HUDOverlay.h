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

#ifndef _HUDOverlay_H__
#define _HUDOverlay_H__

#include "OrxonoxPrereqs.h"

#include <OgrePrerequisites.h>
#include "tools/WindowEventListener.h"
#include "util/Math.h"
#include "core/BaseObject.h"

namespace orxonox
{
  class _OrxonoxExport HUDOverlay : public BaseObject, public WindowEventListener
  {
    public:
      HUDOverlay();
      virtual ~HUDOverlay();

      virtual void XMLPort(Element& xmlElement, XMLPort::Mode mode);

      void setAspectCorrection(bool val);
      bool getAspectCorrection() { return this->bCorrectAspect_; }

      /** Sets the scrolling factor of this overlay. */
      void setScroll(Vector2 scroll) { overlay_->setScroll(scroll.x, scroll.y); }

      /** Gets the current scroll value */
      Vector2 getScroll() const { return Vector2(overlay_->getScrollX(), overlay_->getScrollY()); }

      /** Scrolls the overlay by the offsets provided. */
      void scroll(Vector2 scroll) { overlay_->scroll(scroll.x, scroll.y); }

      /** Sets the rotation applied to this overlay.*/
      void setRotation(const Ogre::Radian& angle) { overlay_->setRotate(angle); }

      /** Gets the rotation applied to this overlay, in degrees.*/
      const Radian& getRotation() const { return overlay_->getRotate(); }

      /** Adds the passed in angle to the rotation applied to this overlay. */
      void rotate(const Radian& angle) { overlay_->rotate(angle); }

      /** Sets the scaling factor of this overlay. */
      void setSize(const Vector2& size) { this->size_ = size; this->sizeChanged(); }

      /** Gets the current size (not corrected) */
      Vector2 getSize() const { return this->size_; }

      /** Gets the current size (corrected) */
      Vector2 getActualSize() const { return this->size_ * this->sizeCorrection_; }

      /** Gets the current size correction */
      Vector2 getSizeCorrection() const { return this->sizeCorrection_; }

      /** Scales the overlay */
      void scale(Vector2 scale) { this->size_ *= scale; this->sizeChanged(); }

    protected:
      virtual void changedVisibility();
      virtual void sizeChanged();
      float getWindowAspectRatio() { return windowAspectRatio_; } 

      Ogre::Overlay* overlay_;

    private:
      void windowResized(int newWidth, int newHeight);

      float windowAspectRatio_;
      bool bCorrectAspect_;
      Vector2 size_;
      Vector2 sizeCorrection_;

      static unsigned int hudOverlayCounter_s;
  };
}

#endif /* _HUDOverlay_H__ */

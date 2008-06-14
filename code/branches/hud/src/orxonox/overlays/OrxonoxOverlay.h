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

#ifndef _OrxonoxOverlay_H__
#define _OrxonoxOverlay_H__

#include "OrxonoxPrereqs.h"

#include <OgrePrerequisites.h>
#include "tools/WindowEventListener.h"
#include "util/Math.h"
#include "core/BaseObject.h"

namespace orxonox
{
  class _OrxonoxExport OrxonoxOverlay : public BaseObject, public WindowEventListener
  {
    public:
      Ogre::Overlay* getOverlay() { return this->overlay_; }
      OrxonoxOverlay();
      virtual ~OrxonoxOverlay();

      virtual void XMLPort(Element& xmlElement, XMLPort::Mode mode);

      void setAspectCorrection(bool val);
      bool getAspectCorrection() { return this->bCorrectAspect_; }

      /** Sets the position of this overlay. */
      void setPosition(Vector2 pos) { this->position_ = pos; this->positionChanged(); }

      /** Gets the current position. */
      Vector2 getPosition() const { return this->position_; }

      /** Scrolls the overlay by the offsets provided. */
      void scroll(Vector2 scroll) { this->position_ += scroll; this->positionChanged(); }

      /** Sets the origin point of this overlay. */
      void setOrigin(Vector2 pos) { this->origin_ = pos; this->positionChanged(); }

      /** Gets the origin point of this overlay */
      Vector2 getOrigin() const { return this->origin_; }

      /** Sets the rotation applied to this overlay.*/
      void setRotation(const Ogre::Radian& angle) { this->angle_ = angle; this->angleChanged(); }

      /** Gets the rotation applied to this overlay, in degrees.*/
      const Radian& getRotation() const { return this->angle_; }

      /** Adds the passed in angle to the rotation applied to this overlay. */
      void rotate(const Radian& angle) { this->angle_ += angle; this->angleChanged(); }

      /** Sets the size of this overlay. */
      void setSize(const Vector2& size) { this->size_ = size; this->sizeChanged(); }

      /** Gets the current size (not corrected) */
      Vector2 getUncorrectedSize() const { return this->size_; }

      /** Gets the current size (corrected) */
      Vector2 getSize() const { return this->size_ * this->sizeCorrection_; }

      /** Gets the current size correction */
      Vector2 getSizeCorrection() const { return this->sizeCorrection_; }

      /** Scales the overlay */
      void scale(Vector2 scale) { this->size_ *= scale; this->sizeChanged(); }

    protected:
      virtual void changedVisibility();
      virtual void sizeChanged();
      virtual void angleChanged();
      virtual void positionChanged();
      float getWindowAspectRatio() { return windowAspectRatio_; } 

      Ogre::Overlay* overlay_;

    private:
      void windowResized(int newWidth, int newHeight);

      float windowAspectRatio_;
      bool bCorrectAspect_;
      Vector2 size_;
      Vector2 sizeCorrection_;
      Radian angle_;
      Vector2 position_;
      Vector2 origin_;

      static unsigned int hudOverlayCounter_s;
  };
}

#endif /* _OrxonoxOverlay_H__ */

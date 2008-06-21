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
 *      Yuning Chai
 *   Co-authors:
 *      Felix Schulthess
 *      Fabian 'x3n' Landau
 *      Reto Grieder
 *
 */

#ifndef _HUDBar_H__
#define _HUDBar_H__

#include "OrxonoxPrereqs.h"

#include <map>
#include <OgrePrerequisites.h>
#include "util/Math.h"
#include "overlays/OrxonoxOverlay.h"

namespace orxonox
{
  class _OrxonoxExport HUDBar : public OrxonoxOverlay
  {
    public:
      HUDBar();
      virtual ~HUDBar();

      virtual void XMLPort(Element& xmlElement, XMLPort::Mode mode);

      void setValue(float value);
      void addColour(float value, const ColourValue& colour);
      void clearColours();

      inline void setRightToLeft(bool r2l)
        { this->right2Left_ = r2l; }
      inline bool getRightToLeft() const
        { return this->right2Left_; }
      inline float getValue() const
        { return this->value_; }

    private:
      static unsigned int materialcount_s;
      bool right2Left_;
      bool autoColour_;                   // whether bar changes colour automatically
      float value_;                       // progress of bar
      Ogre::PanelOverlayElement* bar_;
      Ogre::TextureUnitState* textureUnitState_;
      std::map<float, ColourValue> colours_;

      float barWidth_s;
      float barHeight_s;
      float barOffsetLeft_s;
      float barOffsetTop_s;
    };
}
#endif /* _HUDBar_H__ */

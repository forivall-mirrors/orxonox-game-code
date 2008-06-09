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
 *
 */

#ifndef _BarOverlayElement_H__
#define _BarOverlayElement_H__

#include "OrxonoxPrereqs.h"

#include <map>

#include <OgrePrerequisites.h>
#include <OgrePanelOverlayElement.h>

#include "util/Math.h"

namespace orxonox
{
  class _OrxonoxExport BarOverlayElement : public Ogre::PanelOverlayElement
  {
    public:
      BarOverlayElement(const Ogre::String& name);
      virtual ~BarOverlayElement();

      void init(Real leftRel, Real topRel, Real dimRel, Ogre::OverlayContainer* container);
      void resize();
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
      int left_;
      int top_;
      int width_;
      int height_;
      float widthratio_;
      int offset_;
      int barwidth_;
      Ogre::Real leftRel_;
      Ogre::Real topRel_;
      Ogre::Real dimRel_;
      Ogre::OverlayContainer* background_;
      Ogre::String name_;
      Ogre::TextureUnitState* textureUnitState_;
      std::map<float, ColourValue> colours_;
    };
}
#endif /* _BarOverlayElement_H__ */

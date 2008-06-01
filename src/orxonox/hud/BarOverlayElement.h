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
 *
 */

#ifndef _BarOverlayElement_H__
#define _BarOverlayElement_H__

#include "OrxonoxPrereqs.h"

#include <OgrePrerequisites.h>
#include <OgrePanelOverlayElement.h>

namespace orxonox
{
  class _OrxonoxExport BarOverlayElement : public Ogre::PanelOverlayElement
  {
    private:
      bool autoColor_;                    // whether bar changes color automatically
      float value_;                       // progress of bar
      int color_;
      int left_;
      int top_;
      int width_;
      int height_;
      int windowW_, windowH_;
      Ogre::Real leftRel_;
      Ogre::Real topRel_;
      Ogre::Real dimRel_;
      Ogre::OverlayManager* om;           // our overlay manager
      Ogre::OverlayContainer* container_; // our parent container to attach to
      Ogre::OverlayContainer* background_;
      Ogre::String name_;

    public:
      bool left2Right;
      static const int RED = 0;           // predefined colors
      static const int YELLOW = 1;
      static const int GREEN = 2;

      BarOverlayElement(const Ogre::String& name);
      virtual ~BarOverlayElement();
      void init(Real leftRel, Real topRel, Real dimRel, Ogre::OverlayContainer* container);
      void resize();
      void setValue(float value);
      void setColor(int color);
      float getValue();
      int getBarColor();
    };
}
#endif /* _BarOverlayElement_H__ */

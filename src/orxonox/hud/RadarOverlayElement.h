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

#ifndef _RadarOverlayElement_H__
#define _RadarOverlayElement_H__

#include "OrxonoxPrereqs.h"

#include <OgrePrerequisites.h>
#include <OgrePanelOverlayElement.h>
#include "util/Math.h"

namespace orxonox
{
    class _OrxonoxExport RadarOverlayElement : public Ogre::PanelOverlayElement
    {
      public:
        RadarOverlayElement(const Ogre::String& name);
        ~RadarOverlayElement();
        void init(Real leftRel, Real topRel, Real dimRel, Ogre::OverlayContainer* container);
        void resize();
        void update();
        void listObjects() const;

      private:
        Ogre::Real leftRel_, topRel_, dimRel_;  // relative position/dimension
        int left_, top_, dim_;                  // absolute position/dimension
  };
}

#endif /* _RadarOverlayElement_H__ */

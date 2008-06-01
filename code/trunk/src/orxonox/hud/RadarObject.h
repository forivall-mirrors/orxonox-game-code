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

#ifndef _RadarObject_H__
#define _RadarObject_H__

#include <OgrePrerequisites.h>
#include <OgreSceneNode.h>
#include <OgrePanelOverlayElement.h>
#include "OrxonoxPrereqs.h"
#include "util/Math.h"

namespace orxonox
{
    class _OrxonoxExport RadarObject
    {
      private:
        Ogre::OverlayManager* om;				// our one and only overlay manager
        Ogre::SceneNode* node_;					// node of object
        int colour_;

      public:
        RadarObject(Ogre::OverlayContainer* container, Ogre::SceneNode* node, int colour = 0);
        ~RadarObject();
        void setColour(int colour);
        void resetColour();
        Vector3 getPosition();
        Ogre::SceneNode* getNode();

        bool right_;
        int index_;                             // index number of object
        Ogre::OverlayContainer* container_;
        Ogre::PanelOverlayElement* panel_;		// the panel used to show the dot

        static int count;
        static const int RED = 0;
        static const int YELLOW = 1;
        static const int GREEN = 2;
        static const int BLUE = 3;
        static const int WHITE = 99;            // used if object got nav focus
  };
}

#endif /* _RadarObject_H__ */

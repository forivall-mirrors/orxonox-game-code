/*
*   ORXONOX - the hottest 3D action shooter ever to exist
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
*      ...
*
*/

#ifndef _RadarObject_H__
#define _RadarObject_H__

#include "OrxonoxPrereqs.h"

#include <OgrePrerequisites.h>
#include <OgrePanelOverlayElement.h>
#include "util/Math.h"

namespace orxonox
{
    class _OrxonoxExport RadarObject
    {
      private:
        Ogre::OverlayManager* om;				// our one and only overlay manager
        void init();

      public:
        RadarObject(Ogre::OverlayContainer* container);
        RadarObject(Ogre::OverlayContainer* container, Vector3 pos);
        ~RadarObject();

        bool right_;
        int index_;                             // index number of object
        Vector3 pos_;							// position in space
        Ogre::OverlayContainer* container_;
        Ogre::PanelOverlayElement* panel_;		// the panel used to show the dot
        RadarObject* next;                      // next pointer of linked list

        static int count;
  };
}

#endif /* _RadarObject_H__ */

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

#ifndef _NAVIGATION_H__
#define _NAVIGATION_H__

#include <OgrePrerequisites.h>
#include <OgrePanelOverlayElement.h>
#include "../OrxonoxPrereqs.h"
#include "util/Math.h"
#include "RadarObject.h"

namespace orxonox
{
    class _OrxonoxExport Navigation
    {
    	private:
    		Ogre::OverlayManager* om;				    // our one and only overlay manager
    		Vector3 shipPos_;                           // position of ship
    		int windowW_, windowH_;

    		void init();

    	public:
    		Navigation(Ogre::OverlayContainer* container);
    		Navigation(Ogre::OverlayContainer* container, RadarObject* focus);
    		~Navigation();
            Ogre::OverlayContainer* container_;
			Ogre::PanelOverlayElement* navMarker_;     // the panel used to show the dot
			RadarObject* focus_;                        // next pointer of linked list

    		void update();
    		void cycleFocus();
    		float getDist2Focus();
	};
}

#endif

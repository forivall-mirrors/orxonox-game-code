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

#ifndef _RADAR_H__
#define _RADAR_H__

#include <string.h>
#include <OgreOverlayManager.h>
#include <OgreStringConverter.h>
#include <OgreOverlayElement.h>
#include <OgrePanelOverlayElement.h>
#include <OgrePrerequisites.h>

#include <util/Math.h>
#include <string.h>
#include "core/Tickable.h"
#include "objects/SpaceShip.h"
#include "../OrxonoxPrereqs.h"
#include "RadarObject.h"
#include "GraphicsEngine.h"

namespace orxonox
{
    class _OrxonoxExport RadarOverlayElement : public Ogre::PanelOverlayElement
    {
        private:
            Ogre::OverlayManager* om;               // our one and only overlay manager
            Ogre::OverlayContainer* container_;
            Vector3 initialDir_;	                // direction of nose
            Vector3 currentDir_;
            Vector3 initialOrth_;                   // direction of normal
            Vector3 currentOrth_;
            Vector3 shipPos_;                       // position of ship
            Ogre::Plane plane;                      // plane perpendicular to dir

			RadarObject* firstRadarObject_;         // start of linked list
			RadarObject* lastRadarObject_;          // end of linked list

            Ogre::Real leftRel_, topRel_, dimRel_;  // relative position/dimension
            int left_, top_, dim_;                  // absolute position/dimension
            int windowW_, windowH_;             	// absolute window dimensions

        public:
            RadarOverlayElement(const Ogre::String& name);
            ~RadarOverlayElement();
            void init(Real leftRel, Real topRel, Real dimRel, Ogre::OverlayContainer* container);
			void resize();
            void update();
            void addObject(Vector3 pos);
            void listObjects();
            float calcRadius(RadarObject* obj);
            float calcPhi(RadarObject* obj);
            bool calcRight(RadarObject* obj);
	};
}

#endif

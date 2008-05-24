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
*      Felix Schulthess
*
*/


#ifndef _HUD_H__
#define _HUD_H__

#include "OrxonoxPrereqs.h"

#include <OgrePrerequisites.h>
#include <OgreTextAreaOverlayElement.h>
#include "core/Tickable.h"
#include "util/Math.h"

namespace orxonox
{
    class _OrxonoxExport HUD : public Tickable
    {
      private:
        HUD();
        HUD(HUD& instance);
        ~HUD();
        Ogre::OverlayManager* om;
        Ogre::Overlay* orxonoxHUD;
        Ogre::OverlayContainer* container;
        Ogre::TextAreaOverlayElement* fpsText;
        BarOverlayElement* energyBar;
        BarOverlayElement* speedoBar;
        RadarOverlayElement* radar;
        RadarObject* firstRadarObject;
        RadarObject* lastRadarObject;
        Navigation* nav;

      public:
        virtual void tick(float);
        void setFPS(float fps);
        void addRadarObject(Vector3 pos);
        RadarObject* getFirstRadarObject();

        static HUD* instance_s;
        static HUD& getSingleton();
        static void setEnergy(float value);
        static void cycleNavigationFocus();
    };
}

#endif /* _HUD_H__ */

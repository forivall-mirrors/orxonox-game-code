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


#ifndef _HUD_H__
#define _HUD_H__

#include "OrxonoxPrereqs.h"

#include <OgrePrerequisites.h>
#include <OgreTextAreaOverlayElement.h>
#include <OgreSceneNode.h>
#include "objects/Tickable.h"
#include "util/Math.h"

namespace orxonox
{
    class _OrxonoxExport HUD : public TickableReal
    {
      private:
        HUD();
        HUD(HUD& instance);
        ~HUD();
        Ogre::OverlayManager* om;
        Ogre::SceneManager* sm;
        Ogre::Overlay* orxonoxHUD;
        Ogre::OverlayContainer* container;
        Ogre::TextAreaOverlayElement* fpsText;
        Ogre::TextAreaOverlayElement* rTRText;
        BarOverlayElement* energyBar;
        BarOverlayElement* speedoBar;
        RadarOverlayElement* radar;
        Navigation* nav;

        bool showFPS;
        bool showRenderTime;

      public:
        virtual void tick(float);
        void addRadarObject(Ogre::SceneNode* node, int colour = 0);
        void removeRadarObject(Ogre::SceneNode* node);
        void setRenderTimeRatio(float ratio);
        void setFPS();

        std::set<RadarObject*> roSet;

        static HUD* instance_s;
        static HUD& getSingleton();
        static void setEnergy(float value);
        static void cycleNavigationFocus();
        static void toggleFPS();
        static void toggleRenderTime();
    };
}

#endif /* _HUD_H__ */

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


#ifndef _OverlayGroup_H__
#define _OverlayGroup_H__

#include "OrxonoxPrereqs.h"

#include <OgrePrerequisites.h>
#include <OgreTextAreaOverlayElement.h>
#include "core/BaseObject.h"
#include "objects/Tickable.h"
#include "util/Math.h"
#include "hud/OverlayElementFactories.h"
#include "tools/WindowEventListener.h"

namespace orxonox
{
    class HUDBar;
    class OrxonoxOverlay;

    class _OrxonoxExport OverlayGroup : public BaseObject, public TickableReal, public WindowEventListener
    {
      public:
        OverlayGroup();
        ~OverlayGroup();

        virtual void XMLPort(Element& xmlElement, XMLPort::Mode mode);

        virtual void tick(float);

        void resize();
        void addRadarObject(WorldEntity* object, const ColourValue& colour = ColourValue(0.5, 0.5, 0.5, 1));
        void removeRadarObject(WorldEntity* object);
        void setFPS();

        inline std::list<RadarObject*>& getRadarObjects()
            { return this->roSet_; }

        static OverlayGroup& getHUD();

        static void setEnergy(float value);
        static void cycleNavigationFocus();
        static void releaseNavigationFocus();
        static void toggleVisibility(const std::string& name);

      private:
        OverlayGroup(const OverlayGroup& instance);

        void addElement(OrxonoxOverlay* element);
        OrxonoxOverlay* getElement(unsigned int index);

        void windowResized(int newWidth, int newHeight);

        std::map<std::string, OrxonoxOverlay*> hudElements_;

        std::list<RadarObject*> roSet_;

        RadarOverlayElementFactory radarOverlayElementFactory_;

        Ogre::Overlay* orxonoxHUD_;
        Ogre::OverlayContainer* container_;
        Ogre::TextAreaOverlayElement* fpsText_;
        Ogre::TextAreaOverlayElement* rTRText_;
        HUDBar* energyBar_;
        BarOverlayElement* speedoBar_;
        RadarOverlayElement* radar_;
        HUDNavigation* nav_;

        bool showFPS_;
        bool showRenderTime_;

        static OverlayGroup* hudInstance_s;
    };
}

#endif /* _OverlayGroup_H__ */

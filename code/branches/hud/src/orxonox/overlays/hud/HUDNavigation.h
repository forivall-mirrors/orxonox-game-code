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
 *      Reto Grieder
 *
 */

#ifndef _HUDNavigation_H__
#define _HUDNavigation_H__

#include "OrxonoxPrereqs.h"

#include <OgrePrerequisites.h>
#include <OgreTextAreaOverlayElement.h>
#include <OgrePanelOverlayElement.h>
#include "overlays/OrxonoxOverlay.h"
#include "util/Math.h"

namespace orxonox
{
    class _OrxonoxExport HUDNavigation : public OrxonoxOverlay, public Tickable
    {
      public:
        HUDNavigation();
        ~HUDNavigation();

        void XMLPort(Element& xmlElement, XMLPort::Mode mode);

        void tick(float dt);

        //void cycleFocus();
        float getDist2Focus() const;

        /*inline RadarObject* getFocus() const
            { return this->focus_; }
        void releaseFocus();*/

        /*static void cycleNavigationFocus();
        static void releaseNavigationFocus();
        static HUDNavigation& getInstance();*/

      protected:
        void sizeChanged();
        void angleChanged() { }
        void positionChanged() { }

      private:
        HUDNavigation(HUDNavigation& instance);
        void updateMarker();
        void updateFocus();

        // XMLPort accessors
        void setNavMarkerSize(float size);
        float getNavMarkerSize() const;
        void setAimMarkerSize(float size);
        float getAimMarkerSize() const;
        void setTextSize(float size);
        float getTextSize() const;
        void setFont(const std::string& font);
        std::string getFont() const;

        Ogre::OverlayContainer* container_;         //!< Container that holds the navigation elements
        Ogre::PanelOverlayElement* navMarker_;      //!< the panel used to show the arrow and the target marker
        float navMarkerSize_;                       //!< One paramter size of the navigation marker
        Ogre::PanelOverlayElement* aimMarker_;      //!< Panel used to show the aim Marker
        float aimMarkerSize_;                       //!< One paramter size of the aim marker
        Ogre::TextAreaOverlayElement* navText_;     //!< Text overlay to display the target distance
        bool wasOutOfView_;                         //!< Performance booster variable: setMaterial is not cheap

        //static HUDNavigation* instance_s;
  };
}

#endif /* _HUDNavigation_H__ */

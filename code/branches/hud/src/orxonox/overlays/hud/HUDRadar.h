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

#ifndef _HUDRadar_H__
#define _HUDRadar_H__

#include "OrxonoxPrereqs.h"

#include <OgrePrerequisites.h>
#include <OgrePanelOverlayElement.h>
#include "overlays/OrxonoxOverlay.h"
#include "objects/Tickable.h"

namespace orxonox
{
    class _OrxonoxExport HUDRadar : public OrxonoxOverlay, public Tickable
    {
      public:
        HUDRadar();
        ~HUDRadar();

        void XMLPort(Element& xmlElement, XMLPort::Mode mode);

        void tick(float dt);

        void listObjects();

        inline std::list<RadarObject*>& getRadarObjects()
            { return this->roSet_; }

        void addRadarObject(WorldEntity* object, const ColourValue& colour = ColourValue(0.5, 0.5, 0.5, 1));
        void removeRadarObject(WorldEntity* object);

        static HUDRadar& getInstance();

      private:
        HUDRadar(HUDRadar& instance);

        std::list<RadarObject*> roSet_;
        Ogre::PanelOverlayElement* background_;

        static HUDRadar* instance_s;
    };
}

#endif /* _HUDRadar_H__ */

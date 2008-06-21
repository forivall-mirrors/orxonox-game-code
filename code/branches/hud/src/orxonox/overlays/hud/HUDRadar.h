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
 *      Felix Schulthess
 *   Co-authors:
 *      Reto Grieder
 *
 */

#ifndef _HUDRadar_H__
#define _HUDRadar_H__

#include "OrxonoxPrereqs.h"

#include <vector>
#include <map>
#include <OgrePrerequisites.h>
#include "overlays/OrxonoxOverlay.h"
#include "RadarListener.h"
#include "RadarViewable.h"

namespace orxonox
{
    class _OrxonoxExport HUDRadar : public OrxonoxOverlay, public RadarListener
    {
      public:
        HUDRadar();
        ~HUDRadar();

        void XMLPort(Element& xmlElement, XMLPort::Mode mode);

        float getRadarSensitivity() const { return this->sensitivity_; }
        void setRadarSensitivity(float sensitivity) { this->sensitivity_ = sensitivity; }

        float getHalfDotSizeDistance() const { return this->halfDotSizeDistance_; }
        void setHalfDotSizeDistance(float distance) { this->halfDotSizeDistance_ = distance; }

        float getMaximumDotSize() const { return this->maximumDotSize_; }
        void setMaximumDotSize(float size) { this->maximumDotSize_ = size; }

      private:
        void displayObject(RadarViewable* viewable, bool bIsMarked);
        float getRadarSensitivity() { return 1.0f; }
        void radarTick(float dt);

        std::map<RadarViewable::Shape, std::string> shapeMaterials_;

        std::vector<Ogre::PanelOverlayElement*> radarDots_;
        std::vector<Ogre::PanelOverlayElement*>::iterator itRadarDots_;
        Ogre::PanelOverlayElement* marker_;

        float halfDotSizeDistance_;
        float maximumDotSize_;

        float sensitivity_;
    };
}

#endif /* _HUDRadar_H__ */

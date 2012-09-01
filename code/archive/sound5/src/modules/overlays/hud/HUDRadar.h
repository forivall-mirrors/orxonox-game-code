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

#include "overlays/OverlaysPrereqs.h"

#include <map>
#include <vector>

#include "util/OgreForwardRefs.h"
#include "interfaces/RadarListener.h"
#include "interfaces/RadarViewable.h"
#include "overlays/OrxonoxOverlay.h"

namespace orxonox
{
    class _OverlaysExport HUDRadar : public OrxonoxOverlay, public RadarListener
    {
    public:
        HUDRadar(BaseObject* creator);
        virtual ~HUDRadar();

        virtual void XMLPort(Element& xmlElement, XMLPort::Mode mode);
        virtual void changedOwner();

    private:
        // XML accessors
        float getHalfDotSizeDistance() const { return this->halfDotSizeDistance_; }
        void setHalfDotSizeDistance(float distance) { this->halfDotSizeDistance_ = distance; }

        float getMaximumDotSize() const { return this->maximumDotSize_; }
        void setMaximumDotSize(float size) { this->maximumDotSize_ = size; }

        float getRadarSensitivity() const { return this->sensitivity_; }
        // used also by RadarListener interface!
        void setRadarSensitivity(float sensitivity) { this->sensitivity_ = sensitivity; }

        // RadarListener interface
        void displayObject(RadarViewable* viewable, bool bIsMarked);
        void radarTick(float dt);

        std::map<RadarViewable::Shape, std::string> shapeMaterials_;

        std::vector<Ogre::PanelOverlayElement*> radarDots_;
        std::vector<Ogre::PanelOverlayElement*>::iterator itRadarDots_;
        Ogre::PanelOverlayElement* marker_;

        float halfDotSizeDistance_;
        float maximumDotSize_;

        float sensitivity_;

        Pawn* owner_;
    };
}

#endif /* _HUDRadar_H__ */

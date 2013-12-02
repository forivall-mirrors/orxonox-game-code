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
#include <string>

#include "util/OgreForwardRefs.h"
#include "interfaces/RadarListener.h"
#include "interfaces/RadarViewable.h"
#include "overlays/OrxonoxOverlay.h"

namespace orxonox
{
    class _OverlaysExport HUDRadar : public OrxonoxOverlay, public RadarListener
    {
    public:
        HUDRadar(Context* context);
        virtual ~HUDRadar();

        virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);
        virtual void changedOwner();
        void setConfigValues();

    private:
        // XML accessors
        float getHalfDotSizeDistance() const { return this->halfDotSizeDistance_; }
        void setHalfDotSizeDistance(float distance) { this->halfDotSizeDistance_ = distance; }

        void setDetectionLimit( float limit )
        { this->detectionLimit_ = limit; }
        float getDetectionLimit() const
        { return this->detectionLimit_; }

        float getMaximumDotSize() const { return this->maximumDotSize_; }
        void setMaximumDotSize(float size) { this->maximumDotSize_ = size; }

        float getMaximumDotSize3D() const { return this->maximumDotSize3D_; }
        void setMaximumDotSize3D(float size) { this->maximumDotSize3D_ = size;}

        std::string get2DMaterial() const {return this->material2D_; }
        void set2DMaterial(std::string material2D) { this->material2D_ = material2D; }

        std::string get3DMaterial() const {return this->material3D_; }
        void set3DMaterial(std::string material3D) { this->material3D_ = material3D; }

        float getRadarSensitivity() const { return this->sensitivity_; }
        // used also by RadarListener interface!
        void setRadarSensitivity(float sensitivity) { this->sensitivity_ = sensitivity; }

        // Determines angle between line of sight and x/z-plain on the 3D minimap
        float getMapAngle() const { return this->mapAngle_; }
        void setMapAngle(float mapAngle) { this->mapAngle_ = mapAngle; }

        // RadarListener interface
        virtual void addObject(RadarViewable* viewable);
        virtual void removeObject(RadarViewable* viewable);
        virtual void objectChanged( RadarViewable* rv );
        void radarTick(float dt);
        bool showObject( RadarViewable* rv ); //!< Do not display an object on radar, if showObject(.) is false.

        void gatherObjects();

        std::map<RadarViewable::Shape, std::string> shapeMaterials_;

//         std::vector<Ogre::PanelOverlayElement*> radarDots_;
//         std::vector<Ogre::PanelOverlayElement*>::iterator itRadarDots_;
        std::map<RadarViewable*, Ogre::PanelOverlayElement*> radarObjects_;
        Ogre::PanelOverlayElement* marker_;

        bool RadarMode_; // Determines, if Radar runs in 3D or 2D Mode

        float halfDotSizeDistance_;
        float maximumDotSize_;
        float maximumDotSize3D_;
        float mapAngle_;

        std::string material2D_;
        std::string material3D_;

        float sensitivity_;
        float detectionLimit_;
        ControllableEntity* owner_;
    };
}

#endif /* _HUDRadar_H__ */

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
#include "core/BaseObject.h"
#include "util/Math.h"
#include "core/Debug.h"
#include "Radar.h"

namespace orxonox
{
    template <class T, int Dummy>
    class _OrxonoxExport RadarAttribute : public BaseObject
    {
      public:
        RadarAttribute();
        ~RadarAttribute() { }

        void XMLPort(Element& xmlElement, XMLPort::Mode mode);
        void loadAttribute(Element& xmlElement, XMLPort::Mode mode);

        void setAttribute(const T& attribute) { this->attribute_ = attribute; }
        const T& getAttribute() const { return this->attribute_; }

        void setIndex(unsigned int index); 
        unsigned int getIndex() { return this->index_; }

      private:
        unsigned int index_;
        T attribute_;
    };

    template <class T, int Dummy>
    void RadarAttribute<T, Dummy>::setIndex(unsigned int index)
    {
        if (index > 0xFF)
        {
            COUT(1) << "Shape index was larger than 255 while parsing a RadarAttribute. "
              << "Using random number!!!" << std::endl;
            this->index_ = rand() & 0xFF;
        }
        else
            this->index_ = index;
    }

    typedef RadarAttribute<std::string, 1> RadarShape;

    template <>
    RadarShape::RadarAttribute() : index_(0)
        { RegisterObject(RadarShape); }

    template <>
    void RadarShape::XMLPort(Element& xmlElement, XMLPort::Mode mode)
    {
        BaseObject::XMLPort(xmlElement, mode);
        XMLPortParam(RadarShape, "shape", setAttribute, getAttribute, xmlElement, mode);
        XMLPortParam(RadarShape, "index", setIndex, getIndex, xmlElement, mode);
    }


    class _OrxonoxExport HUDRadar : public OrxonoxOverlay, public RadarListener//, public Tickable
    {
      public:
        HUDRadar();
        ~HUDRadar();

        void XMLPort(Element& xmlElement, XMLPort::Mode mode);

        /*void addShape(RadarShape* shape);
        RadarShape* getShape(unsigned int index) const;*/

        float getRadarSensitivity() const { return this->sensitivity_; }
        void setRadarSensitivity(float sensitivity) { this->sensitivity_ = sensitivity; }

        float getHalfDotSizeDistance() const { return this->halfDotSizeDistance_; }
        void setHalfDotSizeDistance(float distance) { this->halfDotSizeDistance_ = distance; }

        float getMaximumDotSize() const { return this->maximumDotSize_; }
        void setMaximumDotSize(float size) { this->maximumDotSize_ = size; }

        //void tick(float dt);

        //void listObjects();

      private:
        void displayObject(RadarViewable* viewable, bool bIsMarked);
        void hideMarker() { this->marker_->hide(); }
        float getRadarSensitivity() { return 1.0f; }
        void radarTick(float dt);

        std::map<RadarViewable::Shape, std::string> shapeMaterials_;

        Ogre::PanelOverlayElement* background_;
        std::map<RadarViewable*, Ogre::PanelOverlayElement*> radarDots_;
        Ogre::PanelOverlayElement* marker_;

        float halfDotSizeDistance_;
        float maximumDotSize_;

        float sensitivity_;
    };
}

#endif /* _HUDRadar_H__ */

/*
 * HUDAimAssistant.h
 *
 *  Created on: Oct 15, 2012
 *      Author: mottetb
 */

#ifndef HUDAIMASSISTANT_H_
#define HUDAIMASSISTANT_H_

#include "overlays/OverlaysPrereqs.h"

#include <map>
#include <string>

#include "util/OgreForwardRefs.h"
#include "tools/interfaces/Tickable.h"
#include "interfaces/RadarListener.h"
#include "overlays/OrxonoxOverlay.h"

namespace orxonox
{

    class HUDAimAssistant : public OrxonoxOverlay, public Tickable, public RadarListener
    {
        public:
            HUDAimAssistant(BaseObject* creator);
            virtual ~HUDAimAssistant();

            void setConfigValues();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);
            virtual void tick(float dt);

            // RadarListener interface
            virtual void addObject(RadarViewable* object);
            virtual void removeObject(RadarViewable* viewable);
            virtual void objectChanged(RadarViewable* viewable);

            virtual void changedOwner();
            virtual void sizeChanged();
            virtual void angleChanged() { }
            virtual void positionChanged() { }
            virtual void radarTick(float dt) {}

            inline float getRadarSensitivity() const
                { return 1.0f; }

            inline unsigned int getMarkerLimit() const
                { return this->markerLimit_; }

        private:
            struct ObjectInfo
            {
                Ogre::PanelOverlayElement* panel_;
                bool outOfView_;
                bool wasOutOfView_;
            };

            bool showObject(RadarViewable* rv);

            // XMLPort accessors
            inline void setPointerSize(float size)
            {
                pointerSize_ = size;
                this->sizeChanged();
            }
            inline float getPointerSize() const
                { return pointerSize_; }
            inline void setDetectionLimit(float limit)
                { this->detectionLimit_ = limit; }
            inline float getDetectionLimit() const
                { return this->detectionLimit_; }

            std::map<RadarViewable*, ObjectInfo> activeObjectList_;
            std::list<std::pair<RadarViewable*, unsigned int> > sortedObjectList_;

            float pointerSize_;
            unsigned int markerLimit_;
            float detectionLimit_; //!< Objects that are more far away than detectionLimit_ are not displayed on the HUD. 10000.0f is the default value.
    };

}

#endif /* HUDAIMASSISTANT_H_ */

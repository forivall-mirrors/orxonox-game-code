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
 *      Fabian 'x3n' Landau
 *      Reto Grieder (physics)
 *   Co-authors:
 *      ...
 *
 */

/**
  @file LensFlare.h
  @brief Definition of the LensFlare class.
*/

#ifndef _LensFlare_H__
#define _LensFlare_H__

#include "OrxonoxPrereqs.h"

#include "OgreBillboardSet.h"

#include "core/GraphicsManager.h"
#include "util/Math.h"
#include "worldentities/StaticEntity.h"
#include "graphics/Billboard.h"

namespace orxonox
{
      /**
    @brief
        This class adds a configurable LensFlare effect by adding multiple billboards for the several components of lens flare.
        It uses orxonox::Billboard to render this effect, the used images can be changed in lensflare.material

    @author
        David 'davidsa' Salvisberg
    */
    //TODO: The Hardware Occlusion only works properly for a single Flare on the screen,
    // if we have multiple strong lights it'll become way more complicated to determine how much of every object is occluded individually
    // there's below a 100 render queue groups, so maybe we should take turns for each object to be tested, so we only have one of the objects rendered at a time
    // obviously we shouldn't use too many of these effects anyways, since they use a lot of processing power, so not sure whether it's worth implementing a solution that works for multiple lens flares on screen 
    class _OrxonoxExport LensFlare : public StaticEntity, public Tickable
    {
        public:
            LensFlare(BaseObject* creator);
            virtual ~LensFlare();
            
            inline void setScale(float scale)
                { this->scale_=scale; }
            inline float getScale() const
                { return this->scale_; }
                
            /**
            @brief
                This sets the resolution of the out-of-screen-fade-effect
                
                the higher the resolution, the smoother the transition, but it will also have a greater impact on the performance
                this happens with O(n^2) since it's a two dimensional operation.
            @param fadeResolution
                how many point samples should be used per axis
                
                note: this will always be an odd number, so the center point is included in the samples
            */
            inline void setFadeResolution(unsigned int fadeResolution)
                { this->fadeResolution_=fadeResolution>0?fadeResolution:1; }
            /**
            @brief
                This returns the resolution of the out-of-screen-fade-effect
            @return how many point samples are being used per axis
            */
            inline unsigned int getFadeResolution() const
                { return this->fadeResolution_; }
                
            /**
            @brief
                This sets the exponent of the fade-out function
            @param exponent
                how strong should the fade-out effect be
            */
            inline void setFadeExponent(float exponent)
                { this->fadeExponent_=exponent; }
            /**
            @brief
                This returns the exponent of the fade-out function
            @return the exponent of the fade-out function
            */
            inline float getFadeExponent() const
                { return this->fadeExponent_; }
                
            /**
            @brief
               Turn the out-of-screen-fade-effect on or off
            @param fadeOnViewBorder
                true to turn the effect on, false to turn it off
            */
            inline void setFadeOnViewBorder(bool fadeOnViewBorder)
                { this->fadeOnViewBorder_=fadeOnViewBorder; }
            /**
            @brief
               Determine whether the out-of-screen-fade-effect is on or off
            @return
                true if the effect is on, false if it is off
            */
            inline bool isFadeOnViewBorder() const
                { return this->fadeOnViewBorder_; }

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);

            virtual void tick(float dt);

            virtual void changedVisibility();

        private:
            void registerVariables();
            
            void createBillboards();
            
            void updateBillboardStates(unsigned int dimension, bool isLightVisible);
            
            void updateBillboardAlphas(float alpha);
            
            unsigned int getPointCount(unsigned int dimension) const;
            
            Billboard* occlusionBillboard_;//!< this is a transparent billboard used solely for the Hardware Occlusion Query
            unsigned int cameraDistance_;//!< current distance of the lensflare center from the camera
            float scale_;//!< this factor is used to scale the billboard to the desired size
            bool fadeOnViewBorder_;//!< should the effect fade out on the border of the view?
            unsigned int fadeResolution_;//!< how many points should be sampled per axis for the screen border fade. High number => smooth fade, but uses more processing power
            float fadeExponent_;//!< this determines how fast the flare fades away as it gets obstructed
    };
}

#endif /* _LensFlare_H__ */
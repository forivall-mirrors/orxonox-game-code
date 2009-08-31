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
 *   Co-authors:
 *      ...
 *
 */

#ifndef _Light_H__
#define _Light_H__

#include "OrxonoxPrereqs.h"

#include <string>
#include "util/Math.h"
#include "interfaces/TeamColourable.h"
#include "worldentities/StaticEntity.h"

namespace orxonox
{
    class _OrxonoxExport Light : public StaticEntity, public TeamColourable
    {
        public:
            enum LightTypes // Copy from the Ogre enum
            {
                /// Point light sources give off light equally in all directions, so require only position not direction
                Point,
                /// Directional lights simulate parallel light beams from a distant source, hence have direction but no position
                Directional,
                /// Spotlights simulate a cone of light from a source so require position and direction, plus extra values for falloff
                Spotlight
            };

        public:
            Light(BaseObject* creator);
            virtual ~Light();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);
            void registerVariables();

            virtual void changedVisibility();

            inline Ogre::Light* getLight()
                { return this->light_; }

            inline void setType(Light::LightTypes type)
                { this->type_ = type; this->updateType(); }
            inline Light::LightTypes getType() const
                { return this->type_; }

            inline void setDiffuseColour(const ColourValue& colour)
                { this->diffuse_ = colour; this->updateDiffuseColour(); }
            inline const ColourValue& getDiffuseColour() const
                { return this->diffuse_; }

            inline void setSpecularColour(const ColourValue& colour)
                { this->specular_ = colour; this->updateSpecularColour(); }
            inline const ColourValue& getSpecularColour() const
                { return this->specular_; }

            virtual void setTeamColour(const ColourValue& colour)
                { this->setDiffuseColour(colour); this->setSpecularColour(colour); }

            /**
                @brief Sets the attenuation parameters of the light source i.e. how it diminishes with distance.

                @param attenuation.x range (The absolute upper range of the light in world units)
                @param attenuation.y constant (The constant factor in the attenuation formula: 1.0 means never attenuate, 0.0 is complete attenuation)
                @param attenuation.z linear (The linear factor in the attenuation formula: 1 means attenuate evenly over the distance)
                @param attenuation.w quadratic (The quadratic factor in the attenuation formula: adds a curvature to the attenuation formula)

                Quote from the Ogre API:
                Lights normally get fainter the further they are away. Also, each light is given a maximum range beyond which it cannot affect any objects.
                Light attenuation is not applicable to directional lights since they have an infinite range and constant intensity.
                This follows a standard attenuation approach - see any good 3D text for the details of what they mean since i don't have room here!

                Quote from the Ogre wiki:
                "Using these numbers, the light has 100% intensity at 0 distance, and
                trails off to near black at a distance equal to the Range. Keep in mind
                that most of the light falls in the first 20% of the range."

                Range   Constant   Linear     Quadratic
                3250,     1.0,     0.0014,    0.000007
                600,      1.0,     0.007,     0.0002
                325,      1.0,     0.014,     0.0007
                200,      1.0,     0.022,     0.0019
                160,      1.0,     0.027,     0.0028
                100,      1.0,     0.045,     0.0075
                65,       1.0,     0.07,      0.017
                50,       1.0,     0.09,      0.032
                32,       1.0,     0.14,      0.07
                20,       1.0,     0.22,      0.20
                13,       1.0,     0.35,      0.44
                7,        1.0,     0.7,       1.8
            */
            inline void setAttenuation(const Vector4& attenuation)
                { this->attenuation_ = attenuation; this->updateAttenuation(); }
            inline const Vector4& getAttenuation() const
                { return this->attenuation_; }

            /**
                @brief Sets the range of a spotlight, i.e. the angle of the inner and outer cones and the rate of falloff between them.

                @param spotlightRange.x innerAngle (The angle covered by the bright inner cone)
                @param spotlightRange.x outerAngle (The angle covered by the outer cone)
                @param spotlightRange.x falloff (The rate of falloff between the inner and outer cones. 1.0 means a linear falloff, less means slower falloff, higher means faster falloff.)
            */
            inline void setSpotlightRange(const Vector3& spotlightRange)
                { this->spotlightRange_ = spotlightRange; this->updateSpotlightRange(); }
            inline const Vector3& getSpotlightRange() const
                { return this->spotlightRange_; }

        private:
            void setTypeString(const std::string& type);
            std::string getTypeString() const;

            void updateType();
            void updateDiffuseColour();
            void updateSpecularColour();
            void updateAttenuation();
            void updateSpotlightRange();

            Ogre::Light* light_;
            LightTypes type_;
            ColourValue diffuse_;
            ColourValue specular_;
            Vector4 attenuation_;
            Vector3 spotlightRange_;
    };
}

#endif /* _Light_H__ */

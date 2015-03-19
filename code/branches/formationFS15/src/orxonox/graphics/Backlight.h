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

#ifndef _Backlight_H__
#define _Backlight_H__

#include "OrxonoxPrereqs.h"

#include <string>
#include "tools/interfaces/TimeFactorListener.h"
#include "FadingBillboard.h"

namespace orxonox
{
    class _OrxonoxExport Backlight : public FadingBillboard, public TimeFactorListener
    {
        public:
            Backlight(Context* context);
            virtual ~Backlight();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);

            virtual void tick(float dt);
            virtual void changedVisibility();

            inline void setWidth(float width)
                { this->width_ = width; this->update_width(); }
            inline float getWidth() const
                { return this->width_; }

            inline void setLifetime(float lifetime)
                { this->lifetime_ = lifetime; this->update_lifetime(); }
            inline float getLifetime() const
                { return this->lifetime_; }

            inline void setLength(float length)
                { this->length_ = length; this->update_length(); }
            inline float getLength() const
                { return this->length_; }

            inline void setMaxElements(size_t maxelements)
                { this->maxelements_ = maxelements; this->update_maxelements(); }
            inline size_t getMaxElements() const
                { return this->maxelements_; }

            inline void setTrailMaterial(const std::string& material)
                { this->trailmaterial_ = material; this->update_trailmaterial(); }
            inline const std::string& getTrailMaterial() const
                { return this->trailmaterial_; }

            virtual void changedScale();

        protected:
            virtual void changedTimeFactor(float factor_new, float factor_old);

        private:
            void registerVariables();
            virtual void startturnonoff();
            virtual void stopturnonoff();
            virtual void poststopturnonoff();
            virtual void changedColour();
            void update_width();
            void update_lifetime();
            void update_length();
            void update_maxelements();
            void update_trailmaterial();

            Ogre::RibbonTrail* ribbonTrail_;
            Ogre::SceneNode* ribbonTrailNode_;
            float width_;
            float length_;
            float lifetime_;
            size_t maxelements_;
            std::string trailmaterial_;
            char tickcount_;
    };
}

#endif /* _Backlight_H__ */

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

#include "PositionableEntity.h"
#include "tools/BillboardSet.h"

namespace orxonox
{
    class _OrxonoxExport Backlight : public PositionableEntity
    {
        public:
            Backlight(float maxspeed = 1.0, float brakingtime = 1.0, float scale = 1.0);
            virtual ~Backlight();

            void setConfigValues();
            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);
            virtual void tick(float dt);
            virtual void changedVisibility();
            virtual bool create();

            void setColour(const ColourValue& colour);
            void setTimeFactor(float factor);

        private:
            void configure(float maxspeed, float brakingtime, float scale = 1);
            void updateColourChange();

            static float timeFactor_s;
            BillboardSet billboard_;
            Ogre::SceneNode* ribbonTrailNode_;
            Ogre::RibbonTrail* ribbonTrail_;

            float maxLifeTime_;
            float trailSegmentLength_;
            float width_;

            float brakefactor_;

            float maxTraillength_;
            float traillength_;

            size_t maxTrailsegments_;
    };
}

#endif /* _Backlight_H__ */

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
 *      Reto Grieder
 *   Co-authors:
 *      ...
 *
 */

#ifndef _RadarViewable_H__
#define _RadarViewable_H__

#include "OrxonoxPrereqs.h"
#include <string>
#include "core/OrxonoxClass.h"
#include "util/Math.h"

namespace orxonox
{
    /**
    @brief Interface for receiving window events.
    */
    class _OrxonoxExport RadarViewable : virtual public OrxonoxClass
    {
    public:
        RadarViewable();
        virtual ~RadarViewable() { unregisterFromRadar(); }

        void unregisterFromRadar();

        float getRadarObjectVisibility() const { return this->radarObjectVisibility_; }
        void setRadarObjectVisibility(float visibility) { this->radarObjectVisibility_ = visibility; }

        const std::string& getRadarObjectType() const { return this->radarObjectType_; }
        void setRadarObjectType(const std::string& type) { this->radarObjectType_ = type; }

        const ColourValue& getRadarObjectColour() const { return this->radarObjectColour_; }
        void setRadarObjectColour(const ColourValue& colour) { this->radarObjectColour_ = colour; }

    private:
        float radarObjectVisibility_;
        WorldEntity* radarObject_;
        std::string radarObjectType_;
        ColourValue radarObjectColour_;
    };
}

#endif /* _RadarViewable_H__ */

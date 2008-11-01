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
#include <cassert>
#include "util/Math.h"
#include "util/Debug.h"
#include "core/OrxonoxClass.h"

namespace orxonox
{
    /**
    @brief Interface for receiving window events.
    */
    class _OrxonoxExport RadarViewable : virtual public OrxonoxClass
    {
    class WorldEntity;

    public:
        enum Shape
        {
            Square,
            Dot,
            Triangle
        };

    public:
        RadarViewable();
        virtual ~RadarViewable() { }

        float getRadarObjectCamouflage() const { return this->radarObjectCamouflage_; }
        void setRadarObjectCamouflage(float camouflage) { this->radarObjectCamouflage_ = camouflage; }

        const ColourValue& getRadarObjectColour() const { return this->radarObjectColour_; }
        void setRadarObjectColour(const ColourValue& colour) { this->radarObjectColour_ = colour; }

        const std::string& getRadarObjectDescription() const { return this->radarObjectDescription_; }
        void setRadarObjectDescription(const std::string& str);

        const WorldEntity* getWorldEntity() const { return this->radarObject_; }
        const Vector3& getWorldPosition() const;
        Vector3 getOrientedVelocity() const;

        Shape getRadarObjectType() const { return this->radarObjectType_; }

    protected:
        WorldEntity* radarObject_;

    private:
        void validate() const { if (!this->radarObject_)
        { COUT(1) << "Assertation: Every RadarViewable has to be assigned a WorldEntity pointer!" << std::endl; assert(0); } }

        float radarObjectCamouflage_;
        Shape radarObjectType_;
        std::string radarObjectDescription_;
        ColourValue radarObjectColour_;
    };
}

#endif /* _RadarViewable_H__ */

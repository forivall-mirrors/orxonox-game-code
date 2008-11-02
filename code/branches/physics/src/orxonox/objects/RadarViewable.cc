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

#include "OrxonoxStableHeaders.h"
#include "RadarViewable.h"
#include "util/Debug.h"
#include "core/CoreIncludes.h"
//#include "objects/WorldEntity.h"
#include "Radar.h"

namespace orxonox
{
    /**
        @brief Constructor.
    */
    RadarViewable::RadarViewable()
        : radarObject_(0)
        , radarObjectCamouflage_(0.0f)
        , radarObjectType_(Dot)
        , radarObjectDescription_("staticObject")
    {
        RegisterRootObject(RadarViewable);
    }

    void RadarViewable::setRadarObjectDescription(const std::string& str)
    {
        Radar* radar = Radar::getInstancePtr();
        if (radar)
            this->radarObjectType_ = radar->addObjectDescription(str);
        else
        {
            CCOUT(2) << "Attempting to access the radar, but the radar is non existent." << std::endl;
        }
        this->radarObjectDescription_ = str;
    }

    const Vector3& RadarViewable::getWorldPosition() const
    {
        validate();
        return Vector3::ZERO;//this->radarObject_->getWorldPosition();
    }

    Vector3 RadarViewable::getOrientedVelocity() const
    {
        validate();
        return Vector3::ZERO;//this->radarObject_->getOrientation() * this->radarObject_->getVelocity();
    }
}

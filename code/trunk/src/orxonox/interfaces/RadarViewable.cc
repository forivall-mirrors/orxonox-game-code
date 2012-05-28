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

#include "RadarViewable.h"

#include "util/StringUtils.h"
#include "core/CoreIncludes.h"
#include "core/GameMode.h"
#include "worldentities/WorldEntity.h"
#include "Radar.h"
#include "Scene.h"

namespace orxonox
{
    /**
        @brief Constructor.
    */
    RadarViewable::RadarViewable(BaseObject* creator, const WorldEntity* wePtr)
        : isHumanShip_(false)
        , bVisibility_(true)
        , bInitialized_(false)
        , creator_(creator)
        , wePtr_(wePtr)
        , radarObjectCamouflage_(0.0f)
        , radarObjectShape_(Dot)
        , radarObjectDescription_("staticObject")
        , scale_(1.0f)
    {
        RegisterRootObject(RadarViewable);

        this->uniqueId_=getUniqueNumberString();
        if( GameMode::showsGraphics() )
        {
            this->radar_ = this->creator_->getScene()->getRadar();
            this->radar_->addRadarObject(this);
        }
        this->bInitialized_ = true;
    }


    RadarViewable::~RadarViewable()
    {

        if( this->bInitialized_ )
        {
            if( GameMode::showsGraphics() )
            {
                this->radar_->removeRadarObject(this);
            }
        }
    }

//     void RadarViewable::setRadarObjectDescription(const std::string& str)
//     {
//         Radar* radar = this->getWorldEntity()->getScene()->getRadar();
//         if (radar)
//             this->radarObjectShape_ = radar->addObjectDescription(str);
//         else
//         {
//             orxout(internal_warning) << "Attempting to access the radar, but the radar is non existent." << endl;
//         }
//         this->radarObjectDescription_ = str;
//     }

    const Vector3& RadarViewable::getRVWorldPosition() const
    {
        const WorldEntity* object = this->getWorldEntity();
        validate(object);
        return object->getWorldPosition();
    }

    Vector3 RadarViewable::getRVOrientedVelocity() const
    {
        const WorldEntity* object = this->getWorldEntity();
        validate(object);
        return object->getWorldOrientation() * object->getVelocity();
    }

    void RadarViewable::validate(const WorldEntity* object) const
    {
        if (!object)
        {
            orxout(user_error) << "Assertion: Every RadarViewable has to be assigned a WorldEntity pointer!" << endl;
            assert(0);
        }
    }

    void RadarViewable::settingsChanged()
    {
        if( GameMode::showsGraphics() )
        {
            this->radar_->radarObjectChanged(this);
        }
    }
}

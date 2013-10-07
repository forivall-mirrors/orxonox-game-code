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
 *      Felix Schulthess
 *
 */

/**
@file
@brief
*/

#include "Radar.h"

#include <cassert>

//#include "util/Math.h"
#include "core/object/ObjectList.h"
#include "core/command/ConsoleCommand.h"
#include "interfaces/RadarListener.h"
#include "controllers/HumanController.h"
#include "worldentities/pawns/Pawn.h"

namespace orxonox
{

    Radar::Radar()
        : itFocus_(0)
        , focus_(0)
        , objectTypeCounter_(0)
    {
        // TODO: make this mapping configurable. Maybe there's a possibility with self configured
        //       configValues..
        this->objectTypes_["Asteroid"] = RadarViewable::Dot;
        this->objectTypes_["SpaceShip"] = RadarViewable::Square;
        this->objectTypes_["AsdfQwerty"] = RadarViewable::Triangle;

        /*WorldEntity* object;
        object = new WorldEntity();
        object->setPosition(2000.0, 0.0, 0.0);
        addRadarObject(object, ColourValue(0.5, 0, 0, 1));
        object = new WorldEntity();
        object->setPosition(0.0, 2000.0, 0.0);
        addRadarObject(object, ColourValue(0.5, 0, 0, 1));
        object = new WorldEntity();
        object->setPosition(0.0, 0.0, 2000.0);
        addRadarObject(object, ColourValue(0.5, 0, 0, 1));
        object = new WorldEntity();
        object->setPosition(10000.0,16000.0,0.0);
        addRadarObject(object);*/

    }

    Radar::~Radar()
    {
    }

    void Radar::addRadarObject(RadarViewable* rv)
    {
        assert( this->radarObjects_.find(rv) == this->radarObjects_.end() );
        this->radarObjects_.insert(rv);
        // iterate through all radarlisteners and notify them
        for (ObjectList<RadarListener>::iterator itListener = ObjectList<RadarListener>::begin(); itListener; ++itListener)
        {
            (*itListener)->addObject(rv);
        }
    }

    void Radar::removeRadarObject(RadarViewable* rv)
    {
        assert( this->radarObjects_.find(rv) != this->radarObjects_.end() );
        this->radarObjects_.erase(rv);
        // iterate through all radarlisteners and notify them
        for (ObjectList<RadarListener>::iterator itListener = ObjectList<RadarListener>::begin(); itListener; ++itListener)
        {
            (*itListener)->removeObject(rv);
        }
    }

    const RadarViewable* Radar::getFocus()
    {
        if (this->itFocus_)
            return *(this->itFocus_);
        else
            return 0;
    }

    RadarViewable::Shape Radar::addObjectDescription(const std::string& name)
    {
        std::map<std::string, RadarViewable::Shape>::iterator it = this->objectTypes_.find(name);
        if (it == this->objectTypes_.end())
            return this->objectTypes_[name] = RadarViewable::Square; // default, configure!!
        else
            return this->objectTypes_[name];
    }


    void Radar::tick(float dt)
    {
        SUPER(Radar, tick, dt);

        if (this->itFocus_ && (this->focus_ != *(this->itFocus_)))
        {
            // focus object was deleted, release focus
            this->focus_ = 0;
            this->itFocus_ = 0;
        }

        for (ObjectList<RadarListener>::iterator itListener = ObjectList<RadarListener>::begin(); itListener; ++itListener)
        {
            (*itListener)->radarTick(dt);
        }
    }

    void Radar::cycleFocus()
    {
        if (ObjectList<RadarViewable>::begin() == ObjectList<RadarViewable>::end())
        {
            // list is empty
            this->itFocus_ = 0;
            this->focus_ = 0;
        }

        else if (HumanController::getLocalControllerEntityAsPawn())
        {
            Vector3 localPosition = HumanController::getLocalControllerEntityAsPawn()->getWorldPosition();
            Vector3 targetPosition = localPosition;
            if (this->itFocus_ && *(this->itFocus_))
                targetPosition = this->itFocus_->getRVWorldPosition();

            // find the closest object further away than targetPosition
            float currentDistance = localPosition.squaredDistance(targetPosition);
            float nextDistance = FLT_MAX;
            float minimumDistance = FLT_MAX;
            ObjectList<RadarViewable>::iterator itFallback = 0;

            for (ObjectList<RadarViewable>::iterator it = ObjectList<RadarViewable>::begin(); it; ++it)
            {
                if (*it == static_cast<RadarViewable*>(HumanController::getLocalControllerEntityAsPawn()))
                    continue;

                float targetDistance = localPosition.squaredDistance((*it)->getRVWorldPosition());
                if (targetDistance > currentDistance && targetDistance < nextDistance)
                {
                    this->itFocus_ = it;
                    nextDistance = targetDistance;
                }
                if (targetDistance < minimumDistance)
                {
                    itFallback = it;
                    minimumDistance = targetDistance;
                }
            }

            if (nextDistance == FLT_MAX)
            {
                // we already had the furthest object
                this->itFocus_ = itFallback;
                this->focus_ = *itFallback;
            }
            else
            {
                this->focus_ = *(this->itFocus_);
            }
        }
    }

    void Radar::releaseFocus()
    {
        this->itFocus_ = 0;
        this->focus_ = 0;
    }

    void Radar::listObjects() const
    {
        orxout(debug_output) << "List of RadarObjects:" << endl;
        // iterate through all Radar Objects
        unsigned int i = 0;
        for (ObjectList<RadarViewable>::iterator it = ObjectList<RadarViewable>::begin(); it; ++it, ++i)
        {
            orxout(debug_output) << i++ << ": " << (*it)->getRVWorldPosition() << endl;
        }
    }

    void Radar::radarObjectChanged(RadarViewable* rv)
    {
        for (ObjectList<RadarListener>::iterator itListener = ObjectList<RadarListener>::begin(); itListener; ++itListener)
        {
          (*itListener)->objectChanged(rv);
        }
    }
}

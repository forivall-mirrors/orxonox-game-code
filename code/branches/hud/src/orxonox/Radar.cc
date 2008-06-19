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

/**
    @file
    @brief
*/

#include "OrxonoxStableHeaders.h"
#include "Radar.h"
#include <float.h>
#include "objects/WorldEntity.h"
#include "objects/SpaceShip.h"
#include "core/CoreIncludes.h"
//#include "core/ConfigValueIncludes.h"

namespace orxonox
{
    RadarListener::RadarListener()
    {
        RegisterRootObject(RadarListener);
    }

    SetConsoleCommand(Radar, cycleNavigationFocus, true).setAccessLevel(AccessLevel::User);
    SetConsoleCommand(Radar, releaseNavigationFocus, true).setAccessLevel(AccessLevel::User);

    Radar* Radar::instance_s = 0;

    Radar::Radar()
        : focus_(0)
        , objectTypeCounter_(0)
    {
        assert(instance_s == 0);
        instance_s = this;

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
        instance_s = 0;
    }

    /*void Radar::unregisterObject(RadarViewable* object)
    {
        if (this->focus_ == object)
            this->focus_ = 0;
        // TODO: check for focus
    }*/

    const RadarViewable* Radar::getFocus()
    {
        return *(this->itFocus_);
    }

    RadarViewable::Shape Radar::addObjectDescription(const std::string name)
    {
        std::map<std::string, RadarViewable::Shape>::iterator it = this->objectTypes_.find(name);
        if (it == this->objectTypes_.end())
            return this->objectTypes_[name] = RadarViewable::Square; // default, configure!!
        else
            return this->objectTypes_[name];
    }


    void Radar::tick(float dt)
    {
        if (this->focus_ != *(this->itFocus_))
        {
            // focus object was deleted, release focus
            this->focus_ = 0;
            this->itFocus_ = 0;
        }

        for (Iterator<RadarListener> itListener = ObjectList<RadarListener>::begin(); itListener; ++itListener)
        {
            for (Iterator<RadarViewable> itElement = ObjectList<RadarViewable>::begin(); itElement; ++itElement)
            {
                if ((*itElement) != SpaceShip::getLocalShip() && (*itListener)->getRadarSensitivity() > (*itElement)->getRadarObjectCamouflage())
                    (*itListener)->displayObject(*itElement, *itElement == this->focus_);
            }

            (*itListener)->radarTick(dt);

            if (this->focus_ == 0)
                (*itListener)->hideMarker();
        }
    }

    void Radar::cycleFocus()
    {
        if (*(ObjectList<RadarViewable>::begin()) == 0)
        {
            // list is empty
            this->itFocus_ = 0;
            this->focus_ = 0;
        }
        else
        {
            Vector3 localPosition = SpaceShip::getLocalShip()->getPosition();
            Vector3 targetPosition = localPosition;
            if (*(this->itFocus_))
                targetPosition = this->itFocus_->getWorldPosition();

            // find the closed object further away than targetPosition
            float currentDistance = localPosition.squaredDistance(targetPosition);
            float nextDistance = FLT_MAX;
            float minimumDistance = FLT_MAX;
            Iterator<RadarViewable> itFallback = 0;

            for (Iterator<RadarViewable> it = ObjectList<RadarViewable>::begin(); it; ++it)
            {
                if (*it == SpaceShip::getLocalShip())
                    continue;

                float targetDistance = localPosition.squaredDistance((*it)->getWorldPosition());
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


    /*static*/ Radar& Radar::getInstance()
    {
        assert(instance_s);
        return *instance_s;
    }

    /*static*/ void Radar::cycleNavigationFocus()
    {
        // avoid using getInstance because of the assert().
        // User might call this fuction even if HUDNavigation doesn't exist.
        if (instance_s)
            instance_s->cycleFocus();
    }

    /*static*/ void Radar::releaseNavigationFocus()
    {
        // avoid using getInstance because of the assert().
        // User might call this fuction even if HUDNavigation doesn't exist.
        if (instance_s)
            instance_s->releaseFocus();
    }
}

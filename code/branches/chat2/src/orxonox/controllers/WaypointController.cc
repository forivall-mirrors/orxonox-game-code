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

#include "WaypointController.h"

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "worldentities/ControllableEntity.h"

namespace orxonox
{
    CreateFactory(WaypointController);

    WaypointController::WaypointController(BaseObject* creator) : ArtificialController(creator)
    {
        RegisterObject(WaypointController);

        this->currentWaypoint_ = 0;
        this->setAccuracy(100);
    }

    WaypointController::~WaypointController()
    {
        if (this->isInitialized())
        {
            for (size_t i = 0; i < this->waypoints_.size(); ++i)
                this->waypoints_[i]->destroy();
        }
    }

    void WaypointController::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(WaypointController, XMLPort, xmlelement, mode);

        XMLPortParam(WaypointController, "accuracy", setAccuracy, getAccuracy, xmlelement, mode).defaultValues(100.0f);
        XMLPortObject(WaypointController, WorldEntity, "waypoints", addWaypoint, getWaypoint,  xmlelement, mode);
    }

    void WaypointController::tick(float dt)
    {
        if (!this->isActive())
            return;

        if (this->waypoints_.size() == 0 || !this->getControllableEntity())
            return;

        if (this->waypoints_[this->currentWaypoint_]->getWorldPosition().squaredDistance(this->getControllableEntity()->getPosition()) <= this->squaredaccuracy_)
            this->currentWaypoint_ = (this->currentWaypoint_ + 1) % this->waypoints_.size();

        this->moveToPosition(this->waypoints_[this->currentWaypoint_]->getWorldPosition());
    }

    void WaypointController::addWaypoint(WorldEntity* waypoint)
    {
        this->waypoints_.push_back(waypoint);
    }

    WorldEntity* WaypointController::getWaypoint(unsigned int index) const
    {
        if (index < this->waypoints_.size())
            return this->waypoints_[index];
        else
            return 0;
    }
}

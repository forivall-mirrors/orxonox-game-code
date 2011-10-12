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

#ifndef _WaypointController_H__
#define _WaypointController_H__

#include "OrxonoxPrereqs.h"

#include <vector>
#include "tools/interfaces/Tickable.h"
#include "ArtificialController.h"

namespace orxonox
{
    class _OrxonoxExport WaypointController : public ArtificialController, public Tickable
    {
        public:
            WaypointController(BaseObject* creator);
            virtual ~WaypointController();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);
            virtual void tick(float dt);

            void addWaypoint(WorldEntity* waypoint);
            WorldEntity* getWaypoint(unsigned int index) const;

            inline void setAccuracy(float accuracy)
                { this->squaredaccuracy_ = accuracy*accuracy; }
            inline float getAccuracy() const
                { return sqrt(this->squaredaccuracy_); }

        protected:
            std::vector<WorldEntity*> waypoints_;
            size_t currentWaypoint_;
            float squaredaccuracy_;
    };
}

#endif /* _WaypointController_H__ */

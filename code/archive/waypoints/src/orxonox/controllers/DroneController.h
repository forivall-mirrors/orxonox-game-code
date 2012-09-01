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
 *      Oli Scheuss
 *   Co-authors:
 *      ...
 *
 */

#ifndef _DroneController_H__
#define _DroneController_H__

#include "OrxonoxPrereqs.h"

#include "AIController.h"
#include "core/WeakPtr.h"
#include "tools/interfaces/Tickable.h"

#include "tools/Timer.h"


namespace orxonox
{
    /**
    @brief
        Controller for the Drone of the PPS tutorial.
    @author
        Oli Scheuss
    */
    class _OrxonoxExport DroneController : public ArtificialController, public Tickable
    {
        public:
            DroneController(BaseObject* creator);
            virtual ~DroneController();

            virtual void tick(float dt); //!< The controlling happens here. This method defines what the controller has to do each tick.

            void setOwner(Pawn* owner);
            inline Pawn* getOwner() const
                { return this->owner_; }

            void setDrone(Drone*  drone);
            inline Drone* getDrone() const
                { return this->drone_; }

        protected:
            virtual void action();
            void ownerDied();
            bool friendlyFire(); //<! returns true if the owner_ would be hit.
            bool isShooting_;

        private:
            static const float ACTION_INTERVAL;

            Timer actionTimer_;
            WeakPtr<Pawn> owner_;
            Drone* drone_;
    };
}

#endif /* _DroneController_H__ */

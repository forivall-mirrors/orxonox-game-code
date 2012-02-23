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
 *      Damian 'Mozork' Frick
 *
 */

#ifndef _AutonomousDroneController_H__
#define _AutonomousDroneController_H__

#include "OrxonoxPrereqs.h"

#include "Controller.h"
#include "tools/interfaces/Tickable.h"

namespace orxonox
{
    /**
    @brief
        Controller for the AutonomousDrone of the PPS tutorial.
    @author
        Oli Scheuss
    */
    class _OrxonoxExport AutonomousDroneController : public Controller, public Tickable
    {
        public:
            AutonomousDroneController(BaseObject* creator);
            virtual ~AutonomousDroneController();

            virtual void tick(float dt); //!< The controlling happens here. This method defines what the controller has to do each tick.

        protected:

        private:
    };
}

#endif /* _AutonomousDroneController_H__ */
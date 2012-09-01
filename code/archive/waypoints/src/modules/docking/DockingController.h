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
 *      Sven Stucki
 *   Co-authors:
 *      ...
 *
 */

#ifndef _DockingController_H__
#define _DockingController_H__

#include "DockingPrereqs.h"

#include "infos/PlayerInfo.h"
#include "controllers/ArtificialController.h"
#include "tools/interfaces/Tickable.h"
#include "DockingController.h"

namespace orxonox
{
    class _DockingExport DockingController : public ArtificialController, public Tickable
    {
        public:
            DockingController(BaseObject* creator);
            virtual ~DockingController();

            virtual void tick(float dt);

            void takeControl(bool docking);

            void setDock(Dock* dock) { this->dock_ = dock; }
            void setPlayer(PlayerInfo* player) { this->player_ = player; }

        protected:
            virtual void positionReached();

        private:
            bool docking_;

            Dock* dock_;
            PlayerInfo* player_;

            ControllableEntity* entity_;
    };
}

#endif /* _DockingController_H__ */

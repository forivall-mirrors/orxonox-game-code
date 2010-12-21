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
 *      Gabriel Nadler, Originalfile: Oli Scheuss
 *   Co-authors:
 *      ...
 *
 */

#ifndef _RocketController_H__
#define _RocketController_H__

#include "weapons/WeaponsPrereqs.h"

#include "controllers/Controller.h"
#include "tools/interfaces/Tickable.h"
#include "weapons/projectiles/SimpleRocket.h"
#include "weapons/weaponmodes/SimpleRocketFire.h"

namespace orxonox
{
    /**
    @brief
        Controller for the Rocket (targetseeking)
    @author
        Gabriel Nadler, Originalfile: Oli Scheuss
    */
    class _WeaponsExport RocketController : public Controller, public Tickable
    {
        public:
            RocketController(BaseObject* creator);
            virtual ~RocketController();

            virtual void tick(float dt);
            SimpleRocket* getRocket() const
             {  return this->rocket_;  };
            void setTarget(WorldEntity* target);
        protected:
            void moveToPosition(const Vector3& target);
            void setTargetPosition();
            void moveToTargetPosition();

        private:
            SimpleRocket* rocket_; //!<The Rocket it controlls
            Vector3 targetPosition_;
            WeakPtr<PlayerInfo> player_;

            WeakPtr<WorldEntity> target_;


    };
}

#endif /* _RocketController_H__ */

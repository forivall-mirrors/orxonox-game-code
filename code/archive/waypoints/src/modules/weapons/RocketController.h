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
 *      Gabriel Nadler
 *   Co-authors:
 *      ...
 *
 */

/**
    @file RocketController.h
    @brief Definition of the RocketController class.
*/

#ifndef _RocketController_H__
#define _RocketController_H__

#include "weapons/WeaponsPrereqs.h"

#include "tools/interfaces/Tickable.h"
#include "controllers/Controller.h"

namespace orxonox
{
    /**
    @brief
        Controller for the SimpleRocket (target seeking)
    @author
        Gabriel Nadler
    @ingroup Weapons
    */
    class _WeaponsExport RocketController : public Controller, public Tickable
    {
        public:
            RocketController(BaseObject* creator);
            virtual ~RocketController();

            virtual void tick(float dt);
            /**
            @brief Get the rocket that is controlled by this controller.
            @return Returns a pointer to the rocket.
            */
            SimpleRocket* getRocket() const
                { return this->rocket_; };

            /**
            @brief Set the target of the rocket.
            @param target A pointer to the target.
            */
            void setTarget(WorldEntity* target)
                { this->target_ = target; }

        protected:
            void updateTargetPosition();
            void moveToPosition(const Vector3& target);

        private:
            SimpleRocket* rocket_; //!< The Rocket it controls.
            Vector3 targetPosition_; //!< The position of the target.
            WeakPtr<PlayerInfo> player_; //!< The player the rocket belongs to.

            WeakPtr<WorldEntity> target_; //!< The target.
    };
}

#endif /* _RocketController_H__ */

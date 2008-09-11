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

#ifndef _SpaceShipAI_H__
#define _SpaceShipAI_H__

#include "OrxonoxPrereqs.h"

#include "SpaceShip.h"
#include "tools/Timer.h"
#include "util/Math.h"

#define NUM_AI_TEAMS 3

namespace orxonox
{
    class SpaceShipAI : public SpaceShip
    {
        public:
            SpaceShipAI();
            virtual ~SpaceShipAI();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);
            virtual void tick(float dt);
            static void createEnemy(int num);
            static void killEnemies(int num);
            void shipDied(SpaceShip* ship);
            void damage(float damage);
            void kill();

        private:
            virtual ColourValue getProjectileColour() const;

            void action();

            void moveToTargetPosition(float dt);
            void searchNewTargetPosition();
            void forgetTarget();
            void searchNewTarget();
            void aimAtTarget();
            bool isCloseAtTarget(float distance);
            bool isLookingAtTarget(float angle);

            Timer<SpaceShipAI> actionTimer_;

            bool bHasTargetPosition_;
            Vector3 targetPosition_;
            SpaceShip* target_;
            bool bShooting_;

            ColourValue teamColours_[NUM_AI_TEAMS + 1];
    };
}

#endif /* _SpaceShipAI_H__ */

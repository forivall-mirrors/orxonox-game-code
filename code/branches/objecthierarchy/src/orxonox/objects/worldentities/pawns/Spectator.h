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

#ifndef _Spectator_H__
#define _Spectator_H__

#include "OrxonoxPrereqs.h"

#include "objects/worldentities/ControllableEntity.h"

namespace orxonox
{
    class _OrxonoxExport Spectator : public ControllableEntity
    {
        public:
            Spectator(BaseObject* creator);
            virtual ~Spectator();

            void registerVariables();
            virtual void tick(float dt);

            virtual void setPlayer(PlayerInfo* player);
            virtual void startLocalControl();

            virtual void moveFrontBack(float value);
            virtual void moveRightLeft(float value);
            virtual void moveUpDown(float value);

            virtual void rotateYaw(float value);
            virtual void rotatePitch(float value);
            virtual void rotateRoll(float value);

            virtual void fire();
            virtual void greet();

        private:
            void changedGreeting();
            void changedFlareVisibility();

            BillboardSet* greetingFlare_;
            bool bGreetingFlareVisible_;
            bool bGreeting_;

            float speed_;
            float rotationSpeed_;

            float yaw_;
            float pitch_;
            float roll_;

            // test test test
            Mesh* testmesh_;
            Ogre::SceneNode* testnode_;
            // test test test
    };
}

#endif /* _Spectator_H__ */

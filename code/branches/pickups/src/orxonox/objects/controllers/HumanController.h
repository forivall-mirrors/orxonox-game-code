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

#ifndef _HumanController_H__
#define _HumanController_H__

#include "OrxonoxPrereqs.h"

#include "util/Math.h"
#include "Controller.h"

namespace orxonox
{
    class _OrxonoxExport HumanController : public Controller
    {
        public:
            HumanController(BaseObject* creator);
            virtual ~HumanController();

            static void moveFrontBack(const Vector2& value);
            static void moveRightLeft(const Vector2& value);
            static void moveUpDown(const Vector2& value);

            static void rotateYaw(const Vector2& value);
            static void rotatePitch(const Vector2& value);
            static void rotateRoll(const Vector2& value);

            static void fire();
            static void altFire();

            static void boost();
            static void greet();
            static void switchCamera();
            static void mouseLook();
            static void dropItems();
            static void useItem();

            static void suicide();

            static void addBots(unsigned int amount);
            static void killBots(unsigned int amount = 0);

        private:
            static HumanController* localController_s;
    };
}

#endif /* _HumanController_H__ */

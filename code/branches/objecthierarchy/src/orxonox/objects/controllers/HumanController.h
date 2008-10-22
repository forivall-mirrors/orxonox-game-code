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

#include "Controller.h"

namespace orxonox
{
    class _OrxonoxExport HumanController : public Controller
    {
        public:
            HumanController();
            virtual ~HumanController();

            static void moveFrontBack(float value);
            static void moveRightLeft(float value);
            static void moveUpDown(float value);

            static void rotateYaw(float value);
            static void rotatePitch(float value);
            static void rotateRoll(float value);

            static void fire();
            static void altFire();

            static void greet();
            static void use();
            static void switchCamera();

        private:
            static HumanController* localController_s;
    };
}

#endif /* _HumanController_H__ */
